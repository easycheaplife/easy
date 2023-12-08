#include "ace/FILE_IO.h"
#include "ace/SOCK_Stream.h"

class ACE_Message_Block;
class logging_handle {
  protected:
    ACE_FILE_IO& logging_file_;
    ACE_SOCK_Stream logging_peer_;
  public:
    logging_handle(ACE_FILE_IO& __logging_file) : logging_file_(__logging_file) { }

    logging_handle(ACE_FILE_IO& __logging_file,ACE_HANDLE __handle) : logging_file_(__logging_file) {
        logging_peer_.set_handle(__handle);
    }

    logging_handle(ACE_FILE_IO& __logging_file,const ACE_SOCK_Stream& __logging_peer) : logging_file_(__logging_file),logging_peer_(__logging_peer) { }

    int close() {
        return logging_peer_.close();
    }

    int recv_log_record(ACE_Message_Block*& __log_record);

    int write_log_record(ACE_Message_Block* __log_record);

    int log_record();

    ACE_SOCK_Stream& peer() {
        return logging_peer_;
    }

    ~logging_handle() { }
};