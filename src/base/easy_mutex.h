/********************************************************************
	created:	2011/12/10
	created:	10:12:2011   9:42
	filename: 	F:\lee-private-project\trunk\easy\src\base\easy_mutex.h
	file path:	F:\lee-private-project\trunk\easy\src\base
	file base:	easy_mutex
	file ext:	h
	author:		Lee

	purpose:	extremely light-wight wrapper classes for os thread synchronization.
				configuration:	for now, we just choose between pthread or win32 mutex or none.
*********************************************************************/
#ifndef easy_mutex_h__
#define easy_mutex_h__

#ifndef easy_copy_disabled_h__
#include "easy_copy_disabled.h"
#endif //easy_copy_disabled_h__

#define EASY_MUTEXT_HELPER_NONE		0
#define EASY_MUTEXT_HELPER_WIN32		1
#define EASY_MUTEXT_HELPER_PTHREAD	2

#ifdef __WINDOWS
#define EASY_MUTEXT_HELPER	EASY_MUTEXT_HELPER_WIN32
#  include <windows.h>
#elif __LINUX
#define EASY_MUTEXT_HELPER	EASY_MUTEXT_HELPER_PTHREAD
#  include <pthread.h>
#else
#define EASY_MUTEXT_HELPER	EASY_MUTEXT_HELPER_NONE
#endif

namespace easy {
#ifdef __WINDOWS
class EasyWin32Mutex : public EasyCopyDisabled {
  public:
    EasyWin32Mutex() {
        ::InitializeCriticalSection(&mtx_);
    }

    ~EasyWin32Mutex() {
        ::DeleteCriticalSection(&mtx_);
    }

    void lock() {
        ::EnterCriticalSection(&mtx_);
    }

    void unlock() {
        ::LeaveCriticalSection(&mtx_);
    }
  private:
    ::CRITICAL_SECTION	mtx_;
};
#endif

#ifdef __LINUX
class EasyPthreadMutex : public EasyCopyDisabled {
  public:
    EasyPthreadMutex() {
        ::pthread_mutex_init(&mtx_,0);
    }

    ~EasyPthreadMutex() {
        ::pthread_mutex_destroy(&mtx_);
    }

    void lock() {
        ::pthread_mutex_lock(&mtx_);
    }

    void unlock() {
        ::pthread_mutex_unlock(&mtx_);
    }
  private:
    ::pthread_mutex_t	mtx_;
};
#endif

class EasyNullMutex : public EasyCopyDisabled {
  public:
    EasyNullMutex() {  }

    ~EasyNullMutex() {  }

    void lock() {  }

    void unlock() {  }
};

#if EASY_MUTEXT_HELPER == EASY_MUTEXT_HELPER_WIN32
typedef EasyWin32Mutex EasyMutex;
#elif EASY_MUTEXT_HELPER == EASY_MUTEXT_HELPER_PTHREAD
typedef EasyPthreadMutex EasyMutex;
#elif EASY_MUTEXT_HELPER == EASY_MUTEXT_HELPER_NONE
typedef EasyNullMutex EasyMutex;
#endif

template<typename _Mutex>
class EasyGuard : public EasyCopyDisabled {
  public:
    explicit EasyGuard(_Mutex& mtx) : mtx_(mtx) {
        mtx_.lock();
    }

    ~EasyGuard() {
        mtx_.unlock();
    }
  private:
    _Mutex& mtx_;
};
}
#endif // easy_mutex_h__