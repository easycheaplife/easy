#include "iterator_logging_server.h"
#include "ace/Handle_Set.h"
#include "ace/config-macros.h"

class reactive_logging_server : public iterator_logging_server
{
protected:
	ACE_Handle_Set master_handle_set_;
	ACE_Handle_Set active_handle_set_;
public:
	virtual int open(unsigned short __port)
	{
		iterator_logging_server::open(__port);
		master_handle_set_.set_bit(acceptor().get_handle());
		acceptor().enable(ACE_NONBLOCK);
		return 0;
	}

	virtual int wait_for_multiple_event()
	{
		active_handle_set_ = master_handle_set_;
		int __width = (int)active_handle_set_.max_set() + 1;
		if (!select(__width,active_handle_set_.fdset(),0,0,0))
		{
			return -1;
		}
		active_handle_set_.sync((ACE_HANDLE) ((intptr_t) active_handle_set_.max_set () + 1));
		return 0;
	}

	virtual int handle_connections()
	{
		ACE_INET_Addr __logging_peer_addr;
		if (active_handle_set_.is_set(acceptor().get_handle()))
		{
			while (0 == acceptor().accept(get_logging_handle().peer(),&__logging_peer_addr))
			{
				master_handle_set_.set_bit(get_logging_handle().peer().get_handle());
			}
			ACE_DEBUG ((LM_DEBUG, "Accepted connection from %s\n",__logging_peer_addr.get_host_name ()));
			active_handle_set_.clr_bit(acceptor().get_handle());
		}
		return 0;
	}

	virtual int handle_data(ACE_SOCK_Stream*)
	{
		ACE_Handle_Set_Iterator peer_iterator (active_handle_set_);

		for (ACE_HANDLE __handle;(__handle = peer_iterator ()) != ACE_INVALID_HANDLE;) 
		{
			get_logging_handle ().peer ().set_handle (__handle);
			if (get_logging_handle ().log_record () == -1) 
			{
				// Handle connection shutdown or comm failure.
				master_handle_set_.clr_bit (__handle);
				get_logging_handle ().close ();
			}
		}
		return 0;
	}
};

