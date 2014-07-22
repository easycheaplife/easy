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
#include <cstddef>
#include <condition_variable>
#ifdef WIN32
#ifndef __WINDOWS
#define __WINDOWS
#endif //__WINDOWS
#endif //WIN32

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
#endif //__LINUX

namespace easy
{
	//	reference count, thread safe
	struct _Refcount_Base
	{
#ifdef __EASY_WIN_THREAD	
		typedef long	_RC_t;
#else
		typedef size_t	_RC_t;
#endif //__EASY_WIN_THREAD
		//	data member reference count
		volatile		_RC_t		ref_count_;

#ifdef __EASY_PTHREAD	
		pthread_mutex_t	ref_count_lock_;
		_Refcount_Base(_RC_t __n) : ref_count_(__n) {  pthread_mutex_init(&ref_count_lock_, 0);}
#else
		_Refcount_Base(_RC_t __n) : ref_count_(__n) { }
#endif //__EASY_PTHREAD

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
#endif //__EASY_WIN_THREAD
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
#endif //__EASY_WIN_THREAD/

	struct mutex_lock
	{
		mutex_lock() { initialize(); }
		~mutex_lock() { uninitialize(); }
#ifdef __EASY_WIN_THREAD
		//	we also use windows  CRITICAL SECTION and spin lock to implement it.
		CRITICAL_SECTION lock_;
		//	set the spin count times is 4000,that means after check 4000 times, if we can not visit the 
		//	resource also, just switch the kernel mode. we also can use SetCriticalSectionSpinCount set the spin count times.
#pragma warning(suppress: 6031)
		void initialize() { if (1){ InitializeCriticalSectionAndSpinCount(&lock_,0x000004000);} else {InitializeCriticalSection(&lock_);} }
		int acquire_lock() { EnterCriticalSection(&lock_); return 0;}
		int tryacquire_lock() { TryEnterCriticalSection(&lock_); return 0; }
		int release_lock() { LeaveCriticalSection(&lock_); return 0; }
		void uninitialize() { DeleteCriticalSection(&lock_); }
#elif defined __EASY_PTHREAD
		pthread_mutex_t lock_;
		void initialize()   { pthread_mutex_init(&lock_, NULL); }
		int acquire_lock() { return pthread_mutex_lock(&lock_); }
		int tryacquire_lock() { return pthread_mutex_trylock(&lock_); }
		int release_lock() { return pthread_mutex_unlock(&lock_); }
		void uninitialize() { }
#elif defined __EASY_NO_THREAD
		void initialize()   {  }
		int acquire_lock() {  return 0; }
		int tryacquire_lock() {  return 0; }
		int release_lock() {  return 0; }
		void uninitialize() { }
#endif //__EASY_PTHREAD
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

	struct spin_lock
	{
		spin_lock() { initialize(); }
		~spin_lock() { uninitialize(); }
#ifdef __EASY_WIN_THREAD
		void initialize()   {  }
		int acquire_lock() {  return 0; }
		int tryacquire_lock() {  return 0; }
		int release_lock() {  return 0; }
		void uninitialize() { }
#elif defined __EASY_PTHREAD
		pthread_spinlock_t spinlock_;
		void initialize()   {  pthread_spin_init(&spinlock_, 0); }
		int acquire_lock() {  return pthread_spin_lock(&spinlock_); }
		int release_lock() {  return pthread_spin_unlock(&spinlock_); }
		void uninitialize() { pthread_spin_destroy(&spinlock_); }
#elif defined __EASY_NO_THREAD
		void initialize()   {  }
		int acquire_lock() {  return 0; }
		int tryacquire_lock() {  return 0; }
		int release_lock() {  return 0; }
		void uninitialize() { }
#endif //__EASY_WIN_THREAD
	};

	struct rw_lock
	{
		rw_lock() { initialize(); }
		~rw_lock() { uninitialize(); }
#ifdef __EASY_WIN_THREAD
		//	actually, it will work linux well also.it use c++11 condition_variable,which is cross-platform.
		//	you also can implement condition_variable use windows event kernel object or posix pthread_cond_t.
		std::mutex					cont_lock_;
		std::condition_variable		cont_var_;
		int							r_cnt_;
		int							w_cnt_;
		void initialize()   {  r_cnt_ = 0; w_cnt_ = 0; }
		int acquire_r_lock() 
		{ 
			cont_lock_.lock();
			while (w_cnt_ > 0)
			{
				std::unique_lock<std::mutex> __lk(cont_lock_); 
				cont_var_.wait(__lk);
			}
			++r_cnt_;
			cont_lock_.unlock();
			return 0; 
		}
		int release_r_lock() 
		{ 
			cont_lock_.lock();
			--r_cnt_;
			while (0 == r_cnt_)
			{
				cont_var_.notify_one();
			}
			cont_lock_.unlock();
			return 0; 
		}
		int acquire_w_lock() 
		{ 
			cont_lock_.lock();
			while (r_cnt_ + w_cnt_ > 0)
			{
				std::unique_lock<std::mutex> __lk(cont_lock_); 
				cont_var_.wait(__lk);
			}
			++w_cnt_;
			cont_lock_.unlock();
			return 0; 
		}
		int release_w_lock()
		{ 
			cont_lock_.lock();
			--w_cnt_;
			cont_var_.notify_all();
			cont_lock_.unlock();
			return 0; 
		}
		void uninitialize() { }
#elif defined __EASY_PTHREAD
		pthread_rwlock_t rwlock_;
		void initialize()   {  pthread_rwlock_init(&rwlock_, 0); }
		int acquire_r_lock() {  return pthread_rwlock_rdlock(&rwlock_); }
		int release_r_lock() {  return pthread_rwlock_unlock(&rwlock_); }
		int acquire_w_lock() {  return pthread_rwlock_wrlock(&rwlock_); }
		int release_w_lock() {  return pthread_rwlock_unlock(&rwlock_); }
		void uninitialize() { pthread_rwlock_destroy(&rwlock_); }
#endif //__EASY_WIN_THREAD
	};
}
#endif // easy_lock_h__
