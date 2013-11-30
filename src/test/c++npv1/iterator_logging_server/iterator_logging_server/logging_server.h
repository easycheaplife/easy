#include "ace/SOCK_Acceptor.h"

class ACE_SOCK_Stream;
class ACE_FILE_IO;

class logging_server
{
public:
	virtual int run(unsigned short __port);
protected:
	virtual int open(unsigned short __port);

	virtual int wait_for_multiple_event() { return 0; }

	virtual int handle_connections() = 0;

	virtual int handle_data(ACE_SOCK_Stream* = 0) = 0;

	int make_log_file(ACE_FILE_IO&,ACE_SOCK_Stream* = 0);

	virtual ~logging_server() { acceptor_.close(); }

	ACE_SOCK_Acceptor& acceptor() {return acceptor_; }
private:
	ACE_SOCK_Acceptor acceptor_;

};