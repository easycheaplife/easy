/********************************************************************
	created:	2012/03/23
	created:	23:3:2012   16:24
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_thread_mutex.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_thread_mutex
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_thread_mutex_h__
#define easy_thread_mutex_h__
#include "easy_lock.h"
#include "easy_base_type.h"
namespace easy
{
	class thread_mutex
	{
	public:
		thread_mutex() { lock_.initialize();}

		~thread_mutex(){ lock_.uninitialize(); }

		easy_int32 acquire()
		{
			return lock_.acquire_lock();
		}

		easy_int32 tryacquire()
		{
			return lock_.tryacquire_lock();
		}

		easy_int32 release (void)
		{
			return lock_.release_lock();
		}

		easy_int32 acquire_read (void)
		{
			return lock_.acquire_lock();
		}

		easy_int32 acquire_write (void)
		{
			return lock_.acquire_lock();
		}

		easy_int32 tryacquire_read (void)
		{
			return lock_.tryacquire_lock();
		}

		easy_int32 tryacquire_write (void)
		{
			return lock_.tryacquire_lock();
		}

	private:
		mutex_lock lock_;

	private :
		void operator = (const thread_mutex &);
		thread_mutex (const thread_mutex &);
	};
}
#endif // easy_thread_mutex_h__