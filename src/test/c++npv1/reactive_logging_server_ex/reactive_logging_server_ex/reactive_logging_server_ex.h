#include "logging_server.h"
#include "logging_handle.h"
#include "ace/Handle_Set.h"
#include "ace/config-macros.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/FILE_IO.h"
#include "ace/Null_Mutex.h"

typedef ACE_Hash_Map_Manager<ACE_HANDLE,ACE_FILE_IO*,ACE_Null_Mutex>	LOG_MAP;

class reactive_logging_server_ex : public logging_server {
  protected:

    LOG_MAP log_map_;

    ACE_Handle_Set master_handle_set_;
    ACE_Handle_Set active_handle_set_;
  public:
    virtual int open(unsigned short __port) {
        logging_server::open(__port);
        master_handle_set_.set_bit(acceptor().get_handle());
        acceptor().enable(ACE_NONBLOCK);
        return 0;
    }

    virtual int wait_for_multiple_event() {
        active_handle_set_ = master_handle_set_;
        int __width = (int)active_handle_set_.max_set() + 1;
        return ACE::select(__width,active_handle_set_);
    }

    virtual int handle_connections() {
        ACE_INET_Addr __logging_peer_addr;
        ACE_SOCK_Stream __logging_peer;
        while (acceptor().accept(__logging_peer,&__logging_peer_addr) != -1) {
            ACE_FILE_IO* __log_file = new ACE_FILE_IO();
            make_log_file(*__log_file,&__logging_peer);
            log_map_.bind(__logging_peer.get_handle(),__log_file);
            master_handle_set_.set_bit(__logging_peer.get_handle());
            ACE_DEBUG ((LM_DEBUG, "Accepted connection from %s\n",__logging_peer_addr.get_host_name ()));
        }
        active_handle_set_.clr_bit (acceptor ().get_handle ());
        return 0;
    }

    virtual int handle_data(ACE_SOCK_Stream*) {
        ACE_Handle_Set_Iterator peer_iterator (active_handle_set_);

        for (ACE_HANDLE __handle; (__handle = peer_iterator ()) != ACE_INVALID_HANDLE;) {
            ACE_FILE_IO* __log_file = NULL;
            log_map_.find(__handle,__log_file);
            if (!__log_file) {
                continue;
            }
            logging_handle __logging_handle(*__log_file,__handle);
            if (__logging_handle.log_record () == -1) {
                __logging_handle.close();
                master_handle_set_.clr_bit (__handle);
                __log_file->close ();
                log_map_.unbind(__handle);
                delete __log_file;
            }
        }
        return 0;
    }
};

