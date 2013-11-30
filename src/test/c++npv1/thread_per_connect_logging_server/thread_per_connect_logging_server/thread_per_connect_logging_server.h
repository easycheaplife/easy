#include "logging_server.h"
#include "logging_handle.h"
#include "ace/Thread_Manager.h"
#include <vector>

class thread_per_connect_logging_server : public logging_server
{
public:
	class thread_args
	{
	public:
		thread_args(thread_per_connect_logging_server* __lsp) : this_(__lsp) { }
		thread_per_connect_logging_server* this_;
		ACE_SOCK_Stream logging_peer_;
	};

	static void* run_svc(void* __arg)
	{
		thread_args* __thread_args = static_cast<thread_args*>(__arg);
		__thread_args->this_->handle_data(&__thread_args->logging_peer_);
		__thread_args->logging_peer_.close();
		delete __thread_args;
		return 0;
	}

public:
	virtual int open(unsigned short __port)
	{
		logging_server::open(__port);
		return 0;
	}

	virtual int wait_for_multiple_event()
	{
		return 0;
	}

	virtual int handle_connections()
	{
		thread_args* __thread_args = new thread_args(this);
		if (-1 == acceptor().accept(__thread_args->logging_peer_))
		{
			return -1;
		}
		else if(ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)thread_per_connect_logging_server::run_svc,static_cast<void *>(__thread_args),THR_DETACHED | THR_SCOPE_SYSTEM) == -1)
		{
			return -1;
		}
		vec_thr_arg_.push_back(__thread_args);
		return 0;
	}

	virtual int handle_data(ACE_SOCK_Stream* __client)
	{
		for (VEC_THR_ARG::iterator __it = vec_thr_arg_.begin(); __it != vec_thr_arg_.end(); ++__it)
		{
			__client = &(*__it)->logging_peer_;
			ACE_FILE_IO __log_file;
			make_log_file(__log_file,__client);
			__client->disable(ACE_NONBLOCK);
			logging_handle __logging_handle(__log_file,*__client);
			ACE_Thread_Manager* __thread_mgr = ACE_Thread_Manager::instance();
			ACE_thread_t __me = __thread_mgr->thr_self();
			while(!__thread_mgr->testcancel(__me) && __logging_handle.log_record() != -1)
			{
				continue;
			}
			__log_file.close();
		}
		return 0;
	}

private:
	typedef std::vector<thread_per_connect_logging_server::thread_args*>	VEC_THR_ARG;
	VEC_THR_ARG		vec_thr_arg_;
};

