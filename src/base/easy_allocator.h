/********************************************************************
	created:	2011/10/18
	created:	18:10:2011   20:19
	file base:	easy_allocator
	file ext:	h
	author:		Lee
	
	purpose:	a common allocator which reference from sgi
*********************************************************************/
#ifndef easy_allocator_h__
#define easy_allocator_h__

#ifdef WIN32
#ifndef __WINDOWS
#define __WINDOWS
#endif //__WINDOWS
#endif //WIN32

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

#ifndef __EASY_ASSERTIONS
# include <stdio.h>
# define __easy_assert(expr) \
	if (!(expr)) { fprintf(stderr, "%s:%d __easy_assert failure: %s\n", __FILE__, __LINE__, # expr); exit(1); }
#else
# define __easy_assert(expr)
#endif

#ifndef __THROW_BAD_ALLOC
#include <stdio.h>
#include <stdlib.h>
#    define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)
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

	template <int __inst>
	class __malloc_alloc_template 
	{
	private:
		static void* _S_oom_malloc(size_t);
		static void* _S_oom_realloc(void*, size_t);
		static void (* __malloc_alloc_oom_handler)();
	public:
		static void* allocate(size_t __n)
		{
			mutex_lock	__lock;
			__lock.acquire_lock();
			void* __result = NULL;
			try{
				__result = malloc(__n);
				if (0 == __result) 
					__result = _S_oom_malloc(__n);
			}
			catch(std::bad_alloc)
			{
				__lock.release_lock();
				return __result;
			}

			__lock.release_lock();
			return __result;
		}

		static void deallocate(void* __p, size_t /* __n */)
		{
			mutex_lock	__lock;
			__lock.acquire_lock();
			if (__p)
			{
				free(__p);
				__p = 0;
			}
			__lock.release_lock();
		}

		static void* reallocate(void* __p, size_t /* old_sz */, size_t __new_sz)
		{
			mutex_lock	__lock;
			__lock.acquire_lock();
			void* __result = realloc(__p, __new_sz);
			if (0 == __result) 
				__result = _S_oom_realloc(__p, __new_sz);
			__lock.release_lock();
			return __result;
		}

		static void (* __set_malloc_handler(void (*__f)()))()
		{
			void (* __old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = __f;
			return(__old);
		}
	};

	template <int __inst>
	void (* __malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = 0;

	template <int __inst>
	void* __malloc_alloc_template<__inst>::_S_oom_malloc(size_t __n)
	{
		void (* __my_malloc_handler)();
		void* __result;

		for (;;) 
		{
			__my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*__my_malloc_handler)();
			__result = malloc(__n);
			if (__result) return(__result);
		}
	}

	template <int __inst>
	void* __malloc_alloc_template<__inst>::_S_oom_realloc(void* __p, size_t __n)
	{
		void (* __my_malloc_handler)();
		void* __result;

		for (;;) 
		{
			__my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*__my_malloc_handler)();
			__result = realloc(__p, __n);
			if (__result) return(__result);
		}
	}

	typedef __malloc_alloc_template<0> malloc_alloc;

	template<class _Tp, class _Alloc /*= malloc_alloc*/>
	class simple_alloc 
	{
	public:
		static _Tp* allocate(size_t __n)
		{ return 0 == __n ? 0 : (_Tp*) _Alloc::allocate(__n * sizeof (_Tp)); }
		static _Tp* allocate(void)
		{ return (_Tp*) _Alloc::allocate(sizeof (_Tp)); }
		static void deallocate(_Tp* __p, size_t __n)
		{ if (0 != __n) _Alloc::deallocate(__p, __n * sizeof (_Tp)); }
		static void deallocate(_Tp* __p)
		{ _Alloc::deallocate(__p, sizeof (_Tp)); }
	};

	// Allocator adaptor to check size arguments for debugging.
	// Reports errors using __easy_assert.  Checking can be disabled with
	// NDEBUG, but it's far better to just use the underlying allocator
	// instead when no checking is desired.
	// There is some evidence that this can confuse Purify.
	template <class _Alloc /*= malloc_alloc*/>
	class debug_alloc 
	{
	private:

		enum {_S_extra = 8};  // Size of space used to store size.  Note
		// that this must be large enough to preserve
		// alignment.

	public:

		static void* allocate(size_t __n)
		{
			char* __result = (char*)_Alloc::allocate(__n + (int) _S_extra);
			*(size_t*)__result = __n;
			return __result + (int) _S_extra;
		}

		static void deallocate(void* __p, size_t __n)
		{
			char* __real_p = (char*)__p - (int) _S_extra;
			__easy_assert(*(size_t*)__real_p == __n);
			_Alloc::deallocate(__real_p, __n + (int) _S_extra);
		}

		static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz)
		{
			char* __real_p = (char*)__p - (int) _S_extra;
			__easy_assert(*(size_t*)__real_p == __old_sz);
			char* __result = (char*)
				_Alloc::reallocate(__real_p, __old_sz + (int) _S_extra,
				__new_sz + (int) _S_extra);
			*(size_t*)__result = __new_sz;
			return __result + (int) _S_extra;
		}

	};

	template< bool threads, int inst >
	class __default_alloc_template
	{
		enum {_ALIGN = 8};
		enum {_MAX_BYTES = 128};
		enum {_NFREELISTS = 16}; // _MAX_BYTES/_ALIGN
	
		static size_t _S_round_up(size_t __bytes) { return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); }

		static  size_t _S_freelist_index(size_t __bytes) { return (((__bytes) + (size_t)_ALIGN-1)/(size_t)_ALIGN - 1); }

		union _Obj 
		{
			union _Obj* _M_free_list_link;
			char _M_client_data[1];    /* The client sees this.        */
		};
		static _Obj* volatile _S_free_list[_NFREELISTS]; 

		// Returns an object of size __n, and optionally adds to size __n free list.
		static void* _S_refill(size_t __n);

		// Allocates a chunk for nobjs of size size.  nobjs may be reduced
		// if it is inconvenient to allocate the requested number.
		static char* _S_chunk_alloc(size_t __size, int& __nobjs);

		static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz);

		// Chunk allocation state.
		static char*	_S_start_free;
		static char*	_S_end_free;
		static size_t	_S_heap_size;

	public:
		static void* allocate(size_t __n)
		{
			void* __ret = 0;
			if (__n > (size_t) _MAX_BYTES) 
			{
				__ret = malloc_alloc::allocate(__n);
			}
			else
			{
				mutex_lock	__lock;
				__lock.acquire_lock();
				_Obj* volatile* __my_free_list = _S_free_list + _S_freelist_index(__n);
				_Obj* volatile __result = *__my_free_list;
				if (__result == 0)
				{
					__ret = _S_refill(_S_round_up(__n));
				}
				else 
				{
					*__my_free_list = __result -> _M_free_list_link;
					__ret = __result;
				}
				__lock.release_lock();
			}
			return __ret;
		}

		/* __p may not be 0 */
		static void deallocate(void* __p, size_t __n)
		{
			if (__n > (size_t) _MAX_BYTES)
			{
				 malloc_alloc::deallocate(__p, __n);
			}
			else 
			{
				mutex_lock	__lock;
				__lock.acquire_lock();
				  _Obj* volatile*  __my_free_list = _S_free_list + _S_freelist_index(__n);
				  _Obj* __q = (_Obj*)__p;
				  __q -> _M_free_list_link = *__my_free_list;
				  *__my_free_list = __q;
				  __lock.release_lock();
			}
		}
	};

	template <bool __threads, int __inst>
	inline bool operator==(const __default_alloc_template<__threads, __inst>&,
		const __default_alloc_template<__threads, __inst>&)
	{
		return true;
	}

	template <bool __threads, int __inst>
	inline bool operator!=(const __default_alloc_template<__threads, __inst>&,
		const __default_alloc_template<__threads, __inst>&)
	{
		return false;
	}

	/* We allocate memory in large chunks in order to avoid fragmenting     */
	/* the malloc heap too much.                                            */
	/* We assume that size is properly aligned.                             */
	/* We hold the allocation lock.                                         */
	template <bool __threads, int __inst>
	char*	__default_alloc_template<__threads, __inst>::_S_chunk_alloc(size_t __size, int& __nobjs)
	{
		//::_set_new_handler(_out_of_memory);
		char* __result;
		size_t __total_bytes = __size * __nobjs;
		size_t __bytes_left = _S_end_free - _S_start_free;
		//	enough memory to alloc
		if (__bytes_left >= __total_bytes) 
		{
			__result = _S_start_free;
			_S_start_free += __total_bytes;
			return(__result);
		} 
		//	only more than __size can be alloc
		else if (__bytes_left >= __size) 
		{
			__nobjs = (int)(__bytes_left/__size);
			__total_bytes = __size * __nobjs;
			__result = _S_start_free;
			_S_start_free += __total_bytes;
			return(__result);
		} 
		else 
		{
			size_t __bytes_to_get = 2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
			// Try to make use of the left-over piece.
			if (__bytes_left > 0) 
			{
				_Obj* volatile* __my_free_list = _S_free_list + _S_freelist_index(__bytes_left);
				((_Obj*)_S_start_free) -> _M_free_list_link = *__my_free_list;
				*__my_free_list = (_Obj*)_S_start_free;
			}
			//	alloc __bytes_to_get again
			_S_start_free = (char*)malloc(__bytes_to_get);

			//	alloc failed
			if (0 == _S_start_free) 
			{
				size_t __i;
				_Obj* volatile* __my_free_list;
				_Obj* __p;
				// Try to make do with what we have.  That can't
				// hurt.  We do not try smaller requests, since that tends
				// to result in disaster on multi-process machines.
				for (__i = __size; __i <= (size_t) _MAX_BYTES; __i += (size_t) _ALIGN) 
				{
					__my_free_list = _S_free_list + _S_freelist_index(__i);
					__p = *__my_free_list;
					if (0 != __p) 
					{
						*__my_free_list = __p -> _M_free_list_link;
						_S_start_free = (char*)__p;
						_S_end_free = _S_start_free + __i;
						return(_S_chunk_alloc(__size, __nobjs));
						// Any leftover piece will eventually make it to the
						// right free list.
					}
				}
				_S_end_free = 0;	// In case of exception.
				_S_start_free = (char*) malloc(__bytes_to_get);
				// This should either throw an
				// exception or remedy the situation.  Thus we assume it
				// succeeded.
			}
			_S_heap_size += __bytes_to_get;
			_S_end_free = _S_start_free + __bytes_to_get;
			return(_S_chunk_alloc(__size, __nobjs));
		}
	}

	/* Returns an object of size __n, and optionally adds to size __n free list.*/
	/* We assume that __n is properly aligned.                                */
	/* We hold the allocation lock.                                         */
	template <bool __threads, int __inst>
	void* __default_alloc_template<__threads, __inst>::_S_refill(size_t __n)
	{
		int __nobjs = 20;
		char* __chunk = _S_chunk_alloc(__n, __nobjs);
		_Obj* volatile* __my_free_list;
		_Obj* __result;
		_Obj* __current_obj;
		_Obj* __next_obj;
		int __i;

		if (1 == __nobjs) 
		{
			return(__chunk);
		}
		__my_free_list = _S_free_list + _S_freelist_index(__n);

		/* Build free list in chunk */
		__result = (_Obj*)__chunk;
		*__my_free_list = __next_obj = (_Obj*)(__chunk + __n);
		for (__i = 1; ; __i++) 
		{
			__current_obj = __next_obj;
			__next_obj = (_Obj*)((char*)__next_obj + __n);
			if (__nobjs - 1 == __i) 
			{
				__current_obj -> _M_free_list_link = 0;
				break;
			} 
			else 
			{
				__current_obj -> _M_free_list_link = __next_obj;
			}
		}
		return(__result);
	}

	template <bool threads, int inst>
	void* __default_alloc_template<threads, inst>::reallocate(void* __p, size_t __old_sz, size_t __new_sz)
	{
		mutex_lock	__lock;
		__lock.acquire_lock();
		void* __result;
		size_t __copy_sz;

		if (__old_sz > (size_t) _MAX_BYTES && __new_sz > (size_t) _MAX_BYTES) 
		{
			__lock.release_lock();
			return(realloc(__p, __new_sz));
		}
		if (_S_round_up(__old_sz) == _S_round_up(__new_sz)) 
		{
			__lock.release_lock();
			return(__p);
		}
		__result = allocate(__new_sz);
		__copy_sz = __new_sz > __old_sz? __old_sz : __new_sz;
		memcpy(__result, __p, __copy_sz);
		deallocate(__p, __old_sz);
		__lock.release_lock();
		return(__result);
	}

	template< bool threads, int inst >
	char* __default_alloc_template<threads, inst>::_S_start_free = 0;

	template< bool threads, int inst >
	char* __default_alloc_template<threads, inst>::_S_end_free = 0;

	template< bool threads, int inst >
	size_t __default_alloc_template<threads, inst>::_S_heap_size = 0;

	template <bool __threads, int __inst>
	typename __default_alloc_template<__threads, __inst>::_Obj* volatile
		__default_alloc_template<__threads, __inst> ::_S_free_list[_NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

	//	single thread
	typedef __default_alloc_template<false, 0> memory_pool;		//	for test
	typedef __default_alloc_template<false, 0> alloc;			//	for use

	// This implements allocators as specified in the C++ standard.  
	//
	// Note that standard-conforming allocators use many language features
	// that are not yet widely implemented.  In particular, they rely on
	// member templates, partial specialization, partial ordering of function
	// templates, the typename keyword, and the use of the template keyword
	// to refer to a template member of a dependent type.
	template <class _Tp>
	class allocator 
	{
		typedef alloc _Alloc;          // The underlying allocator.
	public:
		typedef size_t     size_type;
		typedef ptrdiff_t  difference_type;
		typedef _Tp*       pointer;
		typedef const _Tp* const_pointer;
		typedef _Tp&       reference;
		typedef const _Tp& const_reference;
		typedef _Tp        value_type;

		template <class _Tp1> struct rebind 
		{
			typedef allocator<_Tp1> other;
		};

		allocator() throw() {}
		allocator(const allocator&) throw() {}
		template <class _Tp1> allocator(const allocator<_Tp1>&) throw() {}
		~allocator() throw() {}

		pointer address(reference __x) const { return &__x; }
		const_pointer address(const_reference __x) const { return &__x; }

		// __n is permitted to be 0.  The C++ standard says nothing about what
		// the return value is when __n == 0.
		_Tp* allocate(size_type __n, const void* = 0) 
		{
			return __n != 0 ? static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp))) : 0;
		}

		// __p is not permitted to be a null pointer.
		void deallocate(pointer __p, size_type __n)
		{ _Alloc::deallocate(__p, __n * sizeof(_Tp)); }

		size_type max_size() const throw() 
		{ return size_t(-1) / sizeof(_Tp); }

		void construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }
		void destroy(pointer __p) { __p->~_Tp(); }
	};

	template<>
	class allocator<void> {
	public:
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		typedef void*       pointer;
		typedef const void* const_pointer;
		typedef void        value_type;

		template <class _Tp1> struct rebind {
			typedef allocator<_Tp1> other;
		};
	};


	template <class _T1, class _T2>
	inline bool operator==(const allocator<_T1>&, const allocator<_T2>&) 
	{
		return true;
	}

	template <class _T1, class _T2>
	inline bool operator!=(const allocator<_T1>&, const allocator<_T2>&)
	{
		return false;
	}

	// Allocator adaptor to turn an SGI-style allocator (e.g. alloc, malloc_alloc)
	// into a standard-conforming allocator.   Note that this adaptor does
	// *not* assume that all objects of the underlying alloc class are
	// identical, nor does it assume that all of the underlying alloc's
	// member functions are static member functions.  Note, also, that 
	// __my_allocator<_Tp, alloc> is essentially the same thing as allocator<_Tp>.

	template <class _Tp, class _Alloc>
	struct __my_allocator 
	{
		_Alloc __underlying_alloc;

		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;
		typedef _Tp*       pointer;
		typedef const _Tp* const_pointer;
		typedef _Tp&       reference;
		typedef const _Tp& const_reference;
		typedef _Tp        value_type;

		template <class _Tp1> struct rebind {
			typedef __my_allocator<_Tp1, _Alloc> other;
		};

		__my_allocator() throw() {}
		__my_allocator(const __my_allocator& __a) throw()
			: __underlying_alloc(__a.__underlying_alloc) {}
		template <class _Tp1> 
		__my_allocator(const __my_allocator<_Tp1, _Alloc>& __a) throw()
			: __underlying_alloc(__a.__underlying_alloc) {}
		~__my_allocator() throw() {}

		pointer address(reference __x) const { return &__x; }
		const_pointer address(const_reference __x) const { return &__x; }

		// __n is permitted to be 0.
		_Tp* allocate(size_type __n, const void* = 0) {
			return __n != 0 
				? static_cast<_Tp*>(__underlying_alloc.allocate(__n * sizeof(_Tp))) 
				: 0;
		}

		// __p is not permitted to be a null pointer.
		void deallocate(pointer __p, size_type __n)
		{ __underlying_alloc.deallocate(__p, __n * sizeof(_Tp)); }

		size_type max_size() const throw() 
		{ return size_t(-1) / sizeof(_Tp); }

		void construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }
		void destroy(pointer __p) { __p->~_Tp(); }
	};

	template <class _Alloc>
	class __my_allocator<void, _Alloc> {
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		typedef void*       pointer;
		typedef const void* const_pointer;
		typedef void        value_type;

		template <class _Tp1> struct rebind {
			typedef __my_allocator<_Tp1, _Alloc> other;
		};
	};

	template <class _Tp, class _Alloc>
		inline bool operator==(const __my_allocator<_Tp, _Alloc>& __a1,
		const __my_allocator<_Tp, _Alloc>& __a2)
	{
		return __a1.__underlying_alloc == __a2.__underlying_alloc;
	}
			
		// Comparison operators for all of the predifined SGI-style allocators.
		// This ensures that __my_allocator<malloc_alloc> (for example) will
		// work correctly.
		template <int inst>
		inline bool operator==(const __malloc_alloc_template<inst>&,
			const __malloc_alloc_template<inst>&)
		{
			return true;
		}

		template <class _Alloc>
		inline bool operator==(const debug_alloc<_Alloc>&,
			const debug_alloc<_Alloc>&) {
				return true;
		}

		// Another allocator adaptor: _Alloc_traits.  This serves two
		// purposes.  First, make it possible to write containers that can use
		// either SGI-style allocators or standard-conforming allocator.
		// Second, provide a mechanism so that containers can query whether or
		// not the allocator has distinct instances.  If not, the container
		// can avoid wasting a word of memory to store an empty object.

		// This adaptor uses partial specialization.  The general case of
		// _Alloc_traits<_Tp, _Alloc> assumes that _Alloc is a
		// standard-conforming allocator, possibly with non-equal instances
		// and non-static members.  (It still behaves correctly even if _Alloc
		// has static member and if all instances are equal.  Refinements
		// affect performance, not correctness.)

		// There are always two members: allocator_type, which is a standard-
		// conforming allocator type for allocating objects of type _Tp, and
		// _S_instanceless, a static const member of type bool.  If
		// _S_instanceless is true, this means that there is no difference
		// between any two instances of type allocator_type.  Furthermore, if
		// _S_instanceless is true, then _Alloc_traits has one additional
		// member: _Alloc_type.  This type encapsulates allocation and
		// deallocation of objects of type _Tp through a static interface; it
		// has two member functions, whose signatures are
		//    static _Tp* allocate(size_t)
		//    static void deallocate(_Tp*, size_t)

		// The fully general version.

		template <class _Tp, class _Allocator>
		struct _Alloc_traits
		{
			static const bool _S_instanceless = false;
			typedef typename _Allocator::template rebind<_Tp>::other 
				allocator_type;
		};

		template <class _Tp, class _Allocator>
		const bool _Alloc_traits<_Tp, _Allocator>::_S_instanceless;

		// The version for the default allocator.

		template <class _Tp, class _Tp1>
		struct _Alloc_traits<_Tp, allocator<_Tp1> >
		{
			static const bool _S_instanceless = true;
			typedef simple_alloc<_Tp, alloc> _Alloc_type;
			typedef allocator<_Tp> allocator_type;
		};

		// Versions for the predefined SGI-style allocators.

		template <class _Tp, int __inst>
		struct _Alloc_traits<_Tp, __malloc_alloc_template<__inst> >
		{
			static const bool _S_instanceless = true;
			typedef simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
			typedef __my_allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
		};

		template <class _Tp, bool __threads, int __inst>
		struct _Alloc_traits<_Tp, __default_alloc_template<__threads, __inst> >
		{
			static const bool _S_instanceless = true;
			typedef simple_alloc<_Tp, __default_alloc_template<__threads, __inst> > 
				_Alloc_type;
			typedef __my_allocator<_Tp, __default_alloc_template<__threads, __inst> > 
				allocator_type;
		};

		template <class _Tp, class _Alloc>
		struct _Alloc_traits<_Tp, debug_alloc<_Alloc> >
		{
			static const bool _S_instanceless = true;
			typedef simple_alloc<_Tp, debug_alloc<_Alloc> > _Alloc_type;
			typedef __my_allocator<_Tp, debug_alloc<_Alloc> > allocator_type;
		};

		// Versions for the __my_allocator adaptor used with the predefined
		// SGI-style allocators.

		template <class _Tp, class _Tp1, int __inst>
		struct _Alloc_traits<_Tp, 
			__my_allocator<_Tp1, __malloc_alloc_template<__inst> > >
		{
			static const bool _S_instanceless = true;
			typedef simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
			typedef __my_allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
		};

		template <class _Tp, class _Tp1, bool __thr, int __inst>
		struct _Alloc_traits<_Tp, 
			__my_allocator<_Tp1, 
			__default_alloc_template<__thr, __inst> > >
		{
			static const bool _S_instanceless = true;
			typedef simple_alloc<_Tp, __default_alloc_template<__thr,__inst> > 
				_Alloc_type;
			typedef __my_allocator<_Tp, __default_alloc_template<__thr,__inst> > 
				allocator_type;
		};

		template <class _Tp, class _Tp1, class _Alloc>
		struct _Alloc_traits<_Tp, __my_allocator<_Tp1, debug_alloc<_Alloc> > >
		{
			static const bool _S_instanceless = true;
			typedef simple_alloc<_Tp, debug_alloc<_Alloc> > _Alloc_type;
			typedef __my_allocator<_Tp, debug_alloc<_Alloc> > allocator_type;
		};

		//	operator new and delete
		class my_alloc
		{
		public:
			//	nothrow new/delete
			void* operator new (size_t __size) throw()
			{
				return alloc::allocate(__size);
			}
			void operator delete(void* __p,size_t __size) throw()
			{
				alloc::deallocate(__p,__size);
			}
		};

		template<class _Tp1> 
		struct auto_ptr_ref 
		{
			_Tp1* _M_ptr;
			auto_ptr_ref(_Tp1* __p) : _M_ptr(__p) {}
		};

		template <class _Tp> 
		class auto_ptr 
		{
		private:
			_Tp* _M_ptr;

		public:
			typedef _Tp element_type;

			explicit auto_ptr(_Tp* __p = 0)  : _M_ptr(__p) {}
			auto_ptr(auto_ptr& __a)  : _M_ptr(__a.release()) {}


			template <class _Tp1> auto_ptr(auto_ptr<_Tp1>& __a) 
				: _M_ptr(__a.release()) {}

			auto_ptr& operator=(auto_ptr& __a)  
			{
				if (&__a != this) 
				{
					delete _M_ptr;
					_M_ptr = __a.release();
				}
				return *this;
			}

			//	for implicit type convert
			template <class _Tp1> auto_ptr& operator=(auto_ptr<_Tp1>& __a)  
			{
				if (__a.get() != this->get()) 
				{
					delete _M_ptr;
					_M_ptr = __a.release();
				}
				return *this;
			}
			// Note: The C++ standard says there is supposed to be an empty throw
			// specification here, but omitting it is standard conforming.  Its 
			// presence can be detected only if _Tp::~_Tp() throws, but (17.4.3.6/2)
			// this is prohibited.
			~auto_ptr() 
			{ 
				delete _M_ptr; 
			}

			_Tp& operator*() const 
			{
				return *_M_ptr;
			}

			_Tp* operator->() const 
			{
				return _M_ptr;
			}

			_Tp* get() const 
			{
				return _M_ptr;
			}

			_Tp* release() 
			{
				_Tp* __tmp = _M_ptr;
				_M_ptr = 0;
				return __tmp;
			}

			void reset(_Tp* __p = 0) 
			{
				if (__p != _M_ptr) {
					delete _M_ptr;
					_M_ptr = __p;
				}
			}

		public:
			auto_ptr(auto_ptr_ref<_Tp> __ref) 
				: _M_ptr(__ref._M_ptr) {}

			auto_ptr& operator=(auto_ptr_ref<_Tp> __ref) 
			{
				if (__ref._M_ptr != this->get()) 
				{
					delete _M_ptr;
					_M_ptr = __ref._M_ptr;
				}
				return *this;
			}

			template <class _Tp1> operator auto_ptr_ref<_Tp1>() 
			{		
				return auto_ptr_ref<_Tp1>(this->release());
			}
			template <class _Tp1> operator auto_ptr<_Tp1>() 
			{ 
				return auto_ptr<_Tp1>(this->release());
			}
		};
}
#endif // easy_allocator_h__
