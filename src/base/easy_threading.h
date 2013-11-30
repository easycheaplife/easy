/********************************************************************
	created:	2011/11/21
	created:	21:11:2011   16:35
	filename: 	E:\lee-private-project\trunk\easy\src\os\easy_threading.h
	file path:	E:\lee-private-project\trunk\easy\src\os
	file base:	easy_threading
	file ext:	h
	author:		Lee
	
	purpose:	about os thread 
*********************************************************************/
#ifndef easy_threading_h__
#define easy_threading_h__

#ifdef __WINDOWS
#include <process.h>
#elif __LINUX
#include <pthread.h>
#endif 

#ifndef easy_atomic_op_h__
#include "easy_atomic_op.h"
#endif //easy_atomic_op_h__

#ifndef easy_mutex_h__
#include "easy_mutex.h"
#endif //easy_mutex_h__

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

#ifndef easy_copy_disabled_h__
#include "easy_copy_disabled.h"
#endif //easy_copy_disabled_h__

namespace easy
{
	class EasyRunnable
	{
	public:
		virtual ~EasyRunnable() { }

		virtual void run() = 0;

		void inc_reference() { ++refs_;}

		void dec_reference()
		{
			if (--refs_)
			{
				delete this;
			}
		}
	private:
		EasyAtomicOp<EasyMutex,easy_long>	refs_;
	};

	enum Priority
	{
		kIdle,
		kLowest,
		kLow,
		kNormal,
		kHigh,
		kHighest,
		kRealtime,
	};

	#define MAXPRIORITYNUM (kRealtime + 1)

	class EasyThreadPriority 
	{
	public:
		EasyThreadPriority();

		easy_int32 priority(Priority p) const
		{
			if (p < kIdle)
			{
				p = kIdle;
			}
			if (p > kRealtime)
			{
				p = kRealtime;
			}
			return priority_[p];
		}
	private:
		easy_int32 priority_[MAXPRIORITYNUM];
	};

	class EasyThread : public EasyCopyDisabled
	{
	public:
		EasyThread();

		void Start();

		void Suspend();

		easy_bool Wait(easy_uint32 time_out) const;

		void Resume();

		void Stop();

		void set_priority(easy_int32 prority);

		easy_int32 priority() const ;

		~EasyThread();

	private:
		virtual easy_int32 _Run( void* p ) = 0;

#ifdef __WINDOWS
		HANDLE		thread_handle_;
		unsigned	thread_id_;
		static unsigned int __stdcall _ThreadFunction( void* p );
#elif __LINUX
		pthread_t	thread_id_;
		static void*  _ThreadFunction( void* p );
#endif //__WINDOWS
	};
}
#endif // easy_threading_h__