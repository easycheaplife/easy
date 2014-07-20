/********************************************************************
	created:	2014/7/20
	created:	20:7:2014   10:44
	file base:	easy_lock
	file ext:	h
	author:		King Lee
	
	purpose:	implement all kinds of lock for windows or linux platform.
*********************************************************************/
#ifndef easy_lock_h__
#define easy_lock_h__

#ifdef WIN32
#ifndef __WINDOWS
#define __WINDOWS
#endif //__WINDOWS
#endif //WIN32

#ifndef __USE_CRITICAL_SECTION
#define __USE_CRITICAL_SECTION
#endif // __USE_CRITICAL_SECTION

#include <cstddef>
#if defined __LINUX && !defined __NO_THREAD
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define	__EASY_PTHREAD 
#elif defined __WINDOWS && !defined __NO_THREAD
#include <Windows.h>
#define	__EASY_WIN_THREAD
#elif defined __NO_THREAD
#define	__EASY_NO_THREAD
#endif

namespace easy
{
	//	reference count, thread safe
	struct _Refcount_Base
	{
#ifdef __EASY_WIN_THREAD	
		typedef long	_RC_t;
#else
		typedef size_t	_RC_t;
#endif
		//	data member reference count
		volatile		_RC_t		ref_count_;

#ifdef __EASY_PTHREAD	
		pthread_mutex_t	ref_count_lock_;
		_Refcount_Base(_RC_t __n) : ref_count_(__n) {  pthread_mutex_init(&ref_count_lock_, 0);}
#else
		_Refcount_Base(_RC_t __n) : ref_count_(__n) { }
#endif

#ifdef __EASY_PTHREAD	
		void incr()	
		{ 
			pthread_mutex_lock(&ref_count_lock_);
			++ref_count_;
			pthread_mutex_unlock(&ref_count_lock_);
		}
		_RC_t decr() 
		{ 
			pthread_mutex_lock(&ref_count_lock_);
			volatile _RC_t __tmp = --ref_count_;
			pthread_mutex_unlock(&ref_count_lock_);
			return __tmp;
		}
#elif defined __EASY_WIN_THREAD
		void incr()	{ ::InterlockedIncrement((_RC_t*)&ref_count_); }
		_RC_t decr() { return ::InterlockedDecrement((_RC_t*)&ref_count_); }
#else	//	no use thread
		void incr()	{  }
		_RC_t decr() {  }
#endif
	};

	//	atomic swap on unsigned long
#ifdef __EASY_PTHREAD
	// we use a template here only to get a unique initialized instance.
	template<int __dummy>
	struct _Swap_lock_struct 
	{
		static pthread_mutex_t _S_swap_lock;
	};
	template<int __dummy>
	pthread_mutex_t _Swap_lock_struct<__dummy>::_S_swap_lock = PTHREAD_MUTEX_INITIALIZER;
	// this should be portable, but performance is expected
	// to be quite awful.  This really needs platform specific
	// code.
	
	inline unsigned long _Atomic_swap(unsigned long * __p, unsigned long __q) 
	{
		pthread_mutex_lock(&_Swap_lock_struct<0>::_S_swap_lock);
		unsigned long __result = *__p;
		*__p = __q;
		pthread_mutex_unlock(&_Swap_lock_struct<0>::_S_swap_lock);
		return __result;
	}
#elif defined __EASY_WIN_THREAD
	inline unsigned long _Atomic_swap(unsigned long* __p,unsigned long __q)
	{
		return (unsigned long) ::InterlockedExchange((LPLONG)__p,(LONG)__q);
	}
#else 
	static inline unsigned long _Atomic_swap(unsigned long * __p, unsigned long __q) {
		unsigned long __result = *__p;
		*__p = __q;
		return __result;
	}
#endif

	template <int __inst>
	struct mutex_spin 
	{
		enum { __low_max = 30, __high_max = 1000 };
		// Low if we suspect uniprocessor, high for multiprocessor.

		static unsigned __max;
		static unsigned __last;
	};

	template <int __inst>
	unsigned mutex_spin<__inst>::__max = mutex_spin<__inst>::__low_max;

	template <int __inst>
	unsigned mutex_spin<__inst>::__last = 0;

	struct mutex_lock
	{
		mutex_lock() { initialize(); }
		~mutex_lock() { uninitialize(); }
#ifdef __EASY_WIN_THREAD
#ifdef __USE_CRITICAL_SECTION
		//	we also use windows  CRITICAL SECTION and spin lock to implement it.
		CRITICAL_SECTION lock_;
		//	set the spin count times is 4000,that means after check 4000 times, if we can not visit the 
		//	resource also, just switch the kernel mode. we also can use SetCriticalSectionSpinCount set the spin count times.
		void initialize() { InitializeCriticalSectionAndSpinCount(&lock_,0x000004000); /*InitializeCriticalSection(&lock_);*/ }
		int acquire_lock() { EnterCriticalSection(&lock_); return 0;}
		int tryacquire_lock() { TryEnterCriticalSection(&lock_); return 0; }
		int release_lock() { LeaveCriticalSection(&lock_); return 0; }
		void uninitialize() { DeleteCriticalSection(&lock_); }
#else
		// It should be relatively easy to get this to work on any modern Unix.
		volatile	unsigned long lock_;
		void initialize() { lock_ = 0;}
		static void nsec_sleep(int __log_nsec)
		{
			if (__log_nsec <= 20)
			{
				Sleep(0);
			}
			else
			{
				Sleep( 1 << (__log_nsec - 20));
			}
		}

		int acquire_lock()
		{
			volatile unsigned long* __lock = &this->lock_;
			if (!_Atomic_swap((unsigned long*)__lock,1))
			{ 
				return 0;
			}
			unsigned __my_spin_max = mutex_spin<0>::__max;		//	30
			unsigned __my_last_spins = mutex_spin<0>::__last;	//	0
			//	no matter the value of __junk
			volatile unsigned __junk = 17;	
			unsigned __i;
			for (__i = 0; __i < __my_spin_max; ++__i)
			{
				if (__i < __my_spin_max/2 || *__lock)
				{
					//	__junk ^ 4
					__junk *= __junk;
					__junk *= __junk;
					__junk *= __junk;
					__junk *= __junk;
					continue;
				}
				//	when __lock is 0,record the value __i and return 
				if (!_Atomic_swap((unsigned long*)__lock, 1))
				{
					// got it!
					// Spinning worked.  Thus we're probably not being scheduled
					// against the other process with which we were contending.
					// Thus it makes sense to spin longer the next time.
					mutex_spin<0>::__last = __i;
					mutex_spin<0>::__max = mutex_spin<0>::__high_max;	//	1000
					return 0;
				}
			}
			//	waiting until the value of __lock is 0, no time out
			mutex_spin<0>::__last = mutex_spin<0>::__low_max;			//	30
			for (__i = 0; ; ++__i)
			{
				int __log_nsec = __i + 6;
				if (__log_nsec > 27)
				{
					__log_nsec = 27;
				}
				if (!_Atomic_swap((unsigned long*)__lock,1))
				{
					return 0;
				}
				nsec_sleep(__log_nsec);
			}
			return 0;
		}

		int tryacquire_lock() {  return 0; }

		int release_lock()
		{
			volatile unsigned long* __lock = &this->lock_;
			*__lock = 0;
			return 0;	
		}
		void uninitialize() { }
#endif
#elif defined __EASY_PTHREAD
		pthread_mutex_t lock_;
		void initialize()   { pthread_mutex_init(&lock_, NULL); }
		int acquire_lock() { return pthread_mutex_lock(&lock_); }
		int tryacquire_lock() { return pthread_mutex_trylock(&lock_); }
		int release_lock() { return pthread_mutex_unlock(&lock_); }
		void uninitialize() { }
#elif defined __EASY_NO_THREAD
		void initialize()   {  }
		int acquire_lock() {  return0; }
		int tryacquire_lock() {  return0; }
		int release_lock() {  return0; }
		void uninitialize() { }
#endif
	};

	struct auto_lock
	{
		mutex_lock&	lock_;
		auto_lock(mutex_lock& __lock) : lock_(__lock) { lock_.acquire_lock(); }
		~auto_lock() { lock_.release_lock(); }
	private:
		void operator=(const auto_lock&);
		auto_lock(const auto_lock&);
	};
}
#endif // easy_lock_h__
