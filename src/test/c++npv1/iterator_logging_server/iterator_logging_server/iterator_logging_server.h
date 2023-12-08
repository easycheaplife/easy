#include "logging_handle.h"
#include "logging_server.h"
#include "ace/FILE_IO.h"
#include "ace/Log_Msg.h"

class iterator_logging_server : public logging_server {
  protected:
    ACE_FILE_IO log_file_;
    logging_handle logging_handle_;
  public:
    iterator_logging_server() : logging_handle_(log_file_) { }

    virtual ~iterator_logging_server() {
        log_file_.close();
    }

    logging_handle& get_logging_handle() {
        return logging_handle_;
    }

  protected:
    virtual int open(unsigned short __port) {
        if (-1 == make_log_file(log_file_)) {
            ACE_ERROR_RETURN((LM_ERROR,"%p\n","make_log_file"),-1);
        }
        return logging_server::open(__port);
    }

    virtual int handle_connections() {
        ACE_INET_Addr __logging_peer_addr;
        if (-1 == acceptor().accept(logging_handle_.peer(),&__logging_peer_addr)) {
            ACE_ERROR_RETURN((LM_ERROR,"%p\n","acceptor().accept"),-1);
        }
        ACE_DEBUG ((LM_DEBUG, "Accepted connection from %s\n",__logging_peer_addr.get_host_name ()));
        return 0;
    }

    virtual int handle_data(ACE_SOCK_Stream* = 0) {
        while(logging_handle_.log_record() != -1) {
            continue;
        }
        logging_handle_.close();
        return 0;
    }
};