#include "easy_threading.h"

#ifndef easy_exception_h__
#include "easy_exception.h"
#endif //easy_exception_h__

namespace easy
{

	EasyThreadPriority::EasyThreadPriority()
	{

	}

	EasyThread::EasyThread()
	{
#ifdef __WINDOWS
		thread_handle_ = INVALID_HANDLE_VALUE;
		thread_id_ = -1;
#elif defined __LINUX
#endif //__WINDOWS
		Start();
	}

	void EasyThread::Start()
	{
#ifdef __WINDOWS
		uintptr_t res = _beginthreadex( NULL, 0, &_ThreadFunction, this, 0, &thread_id_ );
		if (0 == res)
		{
			EASY_EXCEPTION( EasyException::kErrorInvalidParams, "_beginthreadex exception!", "_beginthreadex", kExceptionLevText );
		}
		thread_handle_ = (HANDLE)res;
#elif defined __LINUX
		pthread_create(&thread_id_,NULL,_ThreadFunction,this);
#endif //__WINDOWS
	}

	void EasyThread::Suspend()
	{
#ifdef __WINDOWS
		SuspendThread(thread_handle_);
#elif defined __LINUX

#endif //__WINDOWS
	}

	void EasyThread::Resume()
	{
#ifdef __WINDOWS
		ResumeThread(thread_handle_);
#elif defined __LINUX

#endif //__WINDOWS
	}

	easy_bool EasyThread::Wait( easy_uint32 time_out ) const
	{
		easy_bool res = false;
#ifdef __WINDOWS
		easy_ulong32 result = ::WaitForSingleObject(thread_handle_,time_out);
		if (WAIT_TIMEOUT == result)
		{
			res = false;
		}
		else if (WAIT_OBJECT_0 == res)
		{
			res = true;
		}
#elif defined __LINUX

#endif //__WINDOWS
		return res;
	}

	void EasyThread::set_priority( easy_int32 prority )
	{
#ifdef __WINDOWS
		if (!SetThreadPriority(thread_handle_,prority))
		{
			EASY_EXCEPTION( EasyException::kErrorInvalidParams, "SetPriority exception!", "SetPriority", kExceptionLevText );
		}
#elif defined __LINUX
#endif //__WINDOWS
	}

	easy_int32 EasyThread::priority() const
	{
#ifdef __WINDOWS
		return  GetThreadPriority(thread_handle_);
#endif //__WINDOWS
		return kIdle;
	}

	void EasyThread::Stop()
	{
#ifdef __WINDOWS
		_endthreadex(0);
#elif defined __LINUX
#endif //__WINDOWS
	}

	__RETURN_VAL __STDCALL EasyThread::_ThreadFunction( void* p )
	{
		if (!p)
		{
#ifdef __WINDOWS
		return 0;
#elif defined __LINUX
#endif //__WINDOWS
		}
		EasyThread* _this = (EasyThread*)p;
		if (_this)
		{
			_this->_Run(p);
		}
		_this->Stop();
		return 0;
#ifdef __WINDOWS
		return 0;
#elif defined __LINUX
#endif //__WINDOWS
	}

	EasyThread::~EasyThread()
	{
#ifdef __WINDOWS
		CloseHandle(thread_handle_);
#elif defined __LINUX
#endif //__WINDOWS
	}

}