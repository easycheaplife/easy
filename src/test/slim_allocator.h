/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   16:22
	filename: 	d:\Sever_Redeploy\iwserver\src\RoleServer\slim_allocator.h
	file path:	d:\Sever_Redeploy\iwserver\src\RoleServer
	file base:	slim_allocator
	file ext:	h
	author:		slim shady
	
	purpose:	
*********************************************************************/
#ifndef slim_allocator_h__
#define slim_allocator_h__
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <map>
#include <list>
//#include <>

#include "exception_define.h"

/**
*	default fill 100 node to use 
*/
#define  _DEFAULT_FILL_NODE 100

/**
*	for record map obj point
*/
#define	_STRUCT_HEAD_SIZE			1024

#define	_DATA_HEAD_OFFSET			((sizeof(size_t*))*( (((_MAX_BYTES) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)) +2 ) + _STRUCT_HEAD_SIZE)

#define	_STRUCT_HEAD_OFFSET			0

#define _FREE_LIST_HEAD_OFFSET		( _STRUCT_HEAD_OFFSET + _STRUCT_HEAD_SIZE )

#define _STRUCT_HEAD(__p_head)		( (char*)__p_head + _STRUCT_HEAD_OFFSET )

#define _FREE_LIST_HEAD(__p_head)	( (char*)__p_head + _FREE_LIST_HEAD_OFFSET )

#define _DATA_HEAD(__p_head)		( (char*)__p_head + _DATA_HEAD_OFFSET )

/**
*	macros
*/
#define  _add_to_free_list(__my_free_list,__node)		\
{														\
	*reinterpret_cast<size_t*>(__node) =				\
							*__my_free_list;			\
	*__my_free_list = (char*)(__node) - _head_point;	\
}

#define MY_ALIGN(A,L)	(((A) + (L) - 1) & ~((L) - 1))

#define _BASE_ADDR		( 0x04111111 )

//////////////////////////////////temp code begin////////////////////////////////////////

class Lock_Null_Mutex
{
public:
	Lock_Null_Mutex(void):lock_(0){}

	~Lock_Null_Mutex(void){}

	int remove(void) {return 0;}

	int acquire(void) {return 0;}

	int try_acquire(void) {return 0;}

	int acquire_write(void) {return 0;}

	int try_acquire_write(void) {return 0;}

	int acquire_read(void) {return 0;}

	int try_acquire_read(void) {return 0;}

	int release(void) {return 0;}

	int release_write(void) {return 0;}

	int release_read(void) {return 0;}

	int lock_;
};


template<class OSF_LOCK>
class OSF_Guard
{
public:
	OSF_Guard(Lock_Null_Mutex &) {}

	OSF_Guard(Lock_Null_Mutex &, bool) {}

	OSF_Guard(Lock_Null_Mutex &, bool, int) {}

	~OSF_Guard(void) {}

	int acquire(void) { return 0;}

	int try_acquire(void) {return 0;}

	int release(void) {return 0;}

	void disown(void) {}

	bool locked(void) const {return true;}

	int remove(void) {return 0;}	
private:
	OSF_Guard(const OSF_Guard<Lock_Null_Mutex> &);
	void operator=(const OSF_Guard<Lock_Null_Mutex> &);
};

template <class TYPE , class OSF_LOCK>
class Singleton
{ 
public:
	static TYPE* instance(void)
	{
		Singleton<TYPE , OSF_LOCK>* &singleton = Singleton<TYPE , OSF_LOCK>::instance_i();
		if ( singleton == 0 )
		{
			OSF_LOCK* &lock = Singleton<TYPE , OSF_LOCK>::instance_lock();
			if ( lock == 0 )
				lock = new OSF_LOCK();

			OSF_Guard<OSF_LOCK> guard(*lock);
			if ( singleton == 0 )
				singleton = new Singleton<TYPE , OSF_LOCK>();
		}
		return &singleton->instance_;
	};

	static void close(void)
	{
		Singleton<TYPE , OSF_LOCK>* &singleton = Singleton<TYPE , OSF_LOCK>::instance_i();
		if ( singleton )
			singleton->cleanup();
	};

protected:
	TYPE instance_;

	Singleton(void){};

	void cleanup(void)
	{
		delete this;
		Singleton<TYPE , OSF_LOCK>::instance_i() = 0;
		OSF_LOCK*& lock = Singleton<TYPE , OSF_LOCK>::instance_lock();
		SAFE_DELETE( lock );
		Singleton<TYPE , OSF_LOCK>::instance_lock() = 0;
	};

	static Singleton<TYPE , OSF_LOCK>*& instance_i()
	{
		static Singleton<TYPE , OSF_LOCK>* singleton = 0;
		return singleton;
	};

	static OSF_LOCK* & instance_lock()
	{
		static OSF_LOCK* lock = 0;
		return lock;
	};

};
#define  __WIN32__
#define SM_KEY_BUF_SIZE	64

#if defined (__WIN32__)
#	define INVALID_SHAREMEMORY_HANDLE	((void*)0)
#elif defined (__LINUX__)
#	define INVALID_SHAREMEMORY_HANDLE -1
#endif

#if	defined(__WIN32__)
#include <Windows.h>
#else
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#endif // #endif __WIN32__

typedef unsigned int		SM_KEY;

typedef unsigned long long	uint64;

typedef	int					int32;

typedef unsigned int		uint32;

typedef char				my_char;

typedef unsigned short		uint16;

// define SM_HANDLE
#if defined(__WIN32__)
typedef HANDLE	SM_HANDLE;
#elif defined(__LINUX__)
typedef int		SM_HANDLE;
#endif // #endif __WIN32__



SM_HANDLE CreateShareMemory(SM_KEY	nKey,size_t nSize)
{
	SM_HANDLE smHandle = INVALID_SHAREMEMORY_HANDLE;

#if defined (__LINUX__)		

		smHandle = shmget(nKey,nSize,IPC_CREAT|IPC_EXCL|0666);
		SLIM_LOG3("ShareMemory Created handle = %d ,key = %d ,error: %d \r\n",smHandle,nKey,errno);

#elif defined (__WIN32__)

		char keybuf[SM_KEY_BUF_SIZE];
		memset(keybuf,0,SM_KEY_BUF_SIZE);
		sprintf_s(keybuf,SM_KEY_BUF_SIZE,"%d",nKey);
		smHandle =  ::CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,nSize,keybuf);
		SLIM_LOG3( "ShareMemory Created handle = %d ,key = %s ,error: %d \r\n",smHandle,keybuf,::GetLastError() );

#endif

		return smHandle;
}


SM_HANDLE OpenShareMemory(SM_KEY nKey,size_t nSize)
{
	SM_HANDLE smHandle = INVALID_SHAREMEMORY_HANDLE;

#if defined (__LINUX__)

		smHandle = shmget(nKey,nSize,0);
		SLIM_LOG3("ShareMemory Created handle = %d ,key = %d ,error: %d \r\n",smHandle,nKey,errno);

#elif defined (__WIN32__)
		char keybuf[SM_KEY_BUF_SIZE];
		memset(keybuf,0,SM_KEY_BUF_SIZE);
		sprintf_s(keybuf,SM_KEY_BUF_SIZE,"%d",nKey);
		smHandle = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS,TRUE,keybuf);

		SLIM_LOG3( "ShareMemory Open handle = %d ,key = %s ,error: %d \r\n",smHandle,keybuf,::GetLastError() );
#endif

		return smHandle;
}


char*	MapShareMemory( SM_HANDLE smHandle , void* __start)
{
#if defined (__LINUX__)

		return (char*)shmat(smHandle,(char*)__start,0);

#elif defined (__WIN32__)

		return (char*)::MapViewOfFileEx(smHandle,FILE_MAP_ALL_ACCESS,0,0,0,__start);

#endif
}


void UnMapShareMemory( char*	memPtr )
{
#if defined (__LINUX__)
		shmdt(memPtr);
#elif defined (__WIN32__)
		::UnmapViewOfFile(memPtr);
#endif
}


void CloseShareMemory( SM_HANDLE smHandle )
{
#if defined (__LINUX__)
		shmctl(smHandle,IPC_RMID,0);
#elif defined (__WIN32__)
		::CloseHandle(smHandle);
#endif
}


void OSSleep( uint32 nMilliSeconds )
{
#if defined (__LINUX__)
	usleep(nMilliSeconds*1000);
#elif defined (__WIN32__)
	::Sleep(nMilliSeconds);
#endif

}

//////////////////////////////////temp code end////////////////////////////////////////


#if		defined(__LINUX__)
#include <atomic.h>
#elif	defined(__WIN32__)
#include <WinBase.h>
#include <Windows.h>
#endif

/**
 * @function	atomic_cas
 *
 * @brief		must be SMP or preemptive scheduling environment;
 *
 */
int32	atomic_cas(int32 volatile* __dest,int32 __new_val , int32 __old_val)
{
		int32 volatile __ret = 0;	

#if		defined(__LINUX__)
		__ret = atomic_cmpxchg(__dest,__new_val,__old_val);
#elif	defined(__WIN32__)
		__ret = ::InterlockedCompareExchange( (long volatile*)__dest,__new_val,__old_val );
#endif

		return __ret;
}

// resolve ABA problem when @__old_val is a point value
#define _atomic_cas_and_test(__dest,__new_val,__old_val)	( __old_val == atomic_cas(__dest,__new_val,__old_val) )

#define _atomic_cas_add(__dest,__val)						{ int32 __old = 0 ; do { __old = (int32)*__dest; } while( !_atomic_cas_and_test(__dest, __old + __val , __old ) ); }

#define _atomic_cas_sub(__dest,__val)						{ int32 __old = 0 ; do { __old = (int32)*__dest; } while( !_atomic_cas_and_test(__dest, __old - __val , __old ) ); }

#define _atomic_cas_increament(__dest)						_atomic_cas_add(__dest,1)

#define _atomic_cas_decreament(__dest)						_atomic_cas_sub(__dest,1)

/**
 * @function	atomic_dcas
 *
 * @brief		multi-compare-and-multi-swap
 *
 */
int32	atomic_dcas_test(int32 volatile* __dest_x,int32 __new_val_x , int32 __old_val_x ,int32 volatile* __dest_y,int32 __new_val_y , int32 __old_val_y , int32 __identify)
{
	if(__old_val_x == __identify)
		return 0;

	if( !_atomic_cas_and_test(__dest_x,__identify,__old_val_x) )
		return 0;

	if ( !_atomic_cas_and_test(__dest_y,__new_val_y,__old_val_y) )
	{
		*__dest_x = __old_val_x;
		return 0;
	}
	*__dest_x = __new_val_x;

	return 1;

}

/**
 * @function	atomic_dcas_increament_y_test
 *
 * @brief		multi-compare-and-multi-swap
 *
 */
int32	atomic_dcas_increament_y_test(int32 volatile* __dest_x,int32 __new_val_x , int32 __old_val_x ,int32 volatile* __dest_y , int32 __identify)
{
	int32 __old_y = 0;

	__old_y = *__dest_y;

	return atomic_dcas_test(__dest_x,__new_val_x,__old_val_x,__dest_y,__old_y + 1,__old_y,__identify);
}


/**
 * @function	atomic_dcas_decreament_y
 *
 * @brief		multi-compare-and-multi-swap
 *
 */
int32	atomic_dcas_decreament_y_test(int32 volatile* __dest_x,int32 __new_val_x , int32 __old_val_x ,int32 volatile* __dest_y , int32 __identify)
{
	int32 __old_y = 0;

	__old_y = *__dest_y;

	return atomic_dcas_test(__dest_x,__new_val_x,__old_val_x,__dest_y,__old_y - 1,__old_y,__identify);
}

/**
 * @class spin_rw_lock
 *
 * @brief must be SMP or preemptive scheduling environment and support CAS;
 *
 */
class spin_rw_lock
{
public:
	spin_rw_lock( int32 volatile* __lock , int32 volatile* __count ):
	  _lock(__lock),
	 _read_count(__count)
	{

	}

	~spin_rw_lock()
	{

	}

	int32 remove(void)
	{
		return 0;
	}

	int32 acquire_write(void)
	{

		while( !_atomic_cas_and_test(_lock,LOCK_STATE_WRITE,LOCK_STATE_NULL) )
			OSSleep(0);

		return 1;
	}

	int32 try_acquire_write(void)
	{
		return *_lock == LOCK_STATE_NULL;
	}

	int32 acquire_read(void)
	{
	
		do 
		{
			if( atomic_dcas_test(_lock,LOCK_STATE_READ,LOCK_STATE_NULL,_read_count,1,0,LOCK_STATE_BUSY) )
				return 1;

		} while ( !atomic_dcas_increament_y_test(_lock,LOCK_STATE_READ,LOCK_STATE_READ,_read_count,LOCK_STATE_BUSY) );

		return 1;
	}

	int32 try_acquire_read(void)
	{
		return (*_lock == LOCK_STATE_NULL || *_lock == LOCK_STATE_READ);
	}


	int32 release_write(void)
	{
		while( !_atomic_cas_and_test(_lock,LOCK_STATE_NULL,LOCK_STATE_WRITE) )
		{
			SLIM_LOG1("error here [%d]",*_lock);
		}

		return 1;
	}

	int32 release_read(void)
	{
		do 
		{
			if( atomic_dcas_test(_lock,LOCK_STATE_NULL,LOCK_STATE_READ,_read_count,0,1,LOCK_STATE_BUSY) )
				return 1;

		} while ( !atomic_dcas_decreament_y_test(_lock,LOCK_STATE_READ,LOCK_STATE_READ,_read_count,LOCK_STATE_BUSY) );

		return 1;
	}

private:

	enum LOCK_STATE
	{
		LOCK_STATE_NULL	= 0,
		LOCK_STATE_READ = 1,
		LOCK_STATE_WRITE = 2,
		LOCK_STATE_BUSY	 = 3,
	};

	int32 volatile* _lock;
	int32 volatile* _read_count;
};

#if defined (__LINUX__)
#include <fcntl.h>
#define MY_INVALID_HANDLE_VALUE -1
#elif defined (__WIN32__)
#define MY_INVALID_HANDLE_VALUE INVALID_HANDLE_VALUE
#endif

/**
 * @class flock_t
 *
 * @brief file handle
 *
 */
class flock_t
{
public:

#if defined (__LINUX__)
	struct flock lock;
#elif defined (__WIN32__)
	OVERLAPPED overlapped;
#endif
	/// Name of this filelock.
	my_char const* lockname;

	/// Handle to the underlying file.
	SM_HANDLE handle;
};


/**
 * @class process_rw_lock
 *
 * @brief use file handle
 *
 */
class process_rw_mutex
{
public:

	process_rw_mutex( my_char const* name )
	{
		_file.overlapped.Internal = 0;
		_file.overlapped.InternalHigh = 0;
		_file.overlapped.OffsetHigh = 0;
		_file.overlapped.hEvent = 0;
		_file.handle = MY_INVALID_HANDLE_VALUE;
		_file.lockname = 0;

#if defined (__LINUX__)
		_file.handle = ::open( name , O_CREAT|O_RDWR );
#elif defined(__WIN32__)
		_file.handle = ::CreateFileA(name,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE ,0,OPEN_EXISTING,0,0);
#endif

		if (_file.handle == MY_INVALID_HANDLE_VALUE )
		{
			assert(0);
		}
	}


	~process_rw_mutex()
	{

	}

	int32 remove(void)
	{
		if (_file.handle != MY_INVALID_HANDLE_VALUE)
		{

		}
	}

	int32 acquire_write(void)
	{

#if defined (__LINUX__)
		_file.lock.l_type = F_WRLCK;
		_file.lock.l_whence = SEEK_SET;
		_file.lock.l_start = 0;
		_file.lock.l_len = 0;
		return ::fcntl(_file.handle,F_SETLKW,&_file.lock);
#elif defined(__WIN32__)
		return ::LockFileEx(_file.handle,LOCKFILE_EXCLUSIVE_LOCK ,0,0,0,&_file.overlapped);
#endif
	
	}

	int32 try_acquire_write(void)
	{
#if defined (__LINUX__)
		_file.lock.l_type = F_WRLCK;
		_file.lock.l_whence = SEEK_SET;
		_file.lock.l_start = 0;
		_file.lock.l_len = 0;
		return ::fcntl(_file.handle,F_SETLK,&_file.lock);
#elif defined(__WIN32__)
		return ::LockFileEx(_file.handle,LOCKFILE_FAIL_IMMEDIATELY | LOCKFILE_EXCLUSIVE_LOCK ,0,0,0,&_file.overlapped);
#endif
	}

	int32 acquire_read(void)
	{

#if defined (__LINUX__)
		_file.lock.l_type = F_RDLCK;
		_file.lock.l_whence = SEEK_SET;
		_file.lock.l_start = 0;
		_file.lock.l_len = 0;
		return ::fcntl(_file.handle,F_SETLKW,&_file.lock);
#elif defined(__WIN32__)
		return ::LockFileEx(_file.handle,0 ,0,0,0,&_file.overlapped);
#endif

	}

	int32 try_acquire_read(void)
	{
#if defined (__LINUX__)
		_file.lock.l_type = F_RDLCK;
		_file.lock.l_whence = SEEK_SET;
		_file.lock.l_start = 0;
		_file.lock.l_len = 0;
		return ::fcntl(_file.handle,F_SETLK,&_file.lock);
#elif defined(__WIN32__)
		return ::LockFileEx(_file.handle, LOCKFILE_FAIL_IMMEDIATELY ,0,0,0,&_file.overlapped);
#endif
	}


	int32 release_write(void)
	{
#if defined (__LINUX__)
		_file.lock.l_type = F_UNLCK;
		_file.lock.l_whence = SEEK_SET;
		_file.lock.l_start = 0;
		_file.lock.l_len = 0;
		return ::fcntl(_file.handle,F_SETLK,&_file.lock);
#elif defined(__WIN32__)
		return ::UnlockFile(_file.handle, 0 ,0,0,0);
#endif
	}

	int32 release_read(void)
	{

#if defined (__LINUX__)
		_file.lock.l_type = F_UNLCK;
		_file.lock.l_whence = SEEK_SET;
		_file.lock.l_start = 0;
		_file.lock.l_len = 0;
		return ::fcntl(_file.handle,F_SETLK,&_file.lock);
#elif defined(__WIN32__)
		return ::UnlockFile(_file.handle, 0 ,0,0,0);
#endif

	}

private:
	flock_t	_file;
};

class spin_write_lock_guard
{
public:
	spin_write_lock_guard(spin_rw_lock& __lock):
	  _owner(0),
	 _lock(&__lock)
	{
		_owner = _lock->acquire_write();
	}

	int32 locked()
	{
		return _owner;
	}

	~spin_write_lock_guard()
	{
		if (_owner)
		{
			_owner = 0;
			_lock->release_write();
		}
	}

private:
	spin_rw_lock*	_lock;
	int32			_owner;
};

class spin_read_lock_guard
{
public:
	spin_read_lock_guard(spin_rw_lock& __lock):
	  _owner(0),
		  _lock(&__lock)
	  {
		  _owner = _lock->acquire_read();
	  }

	  int32 locked()
	  {
		  return _owner;
	  }

	  ~spin_read_lock_guard()
	  {
		  if (_owner)
		  {
			  _owner = 0;
			  _lock->release_read();
		  }
	  }

private:
	spin_rw_lock*	_lock;
	int32			_owner;
};


#define WRITE_LOCK_GUARD(__lock,__guard_name) \
	spin_write_lock_guard __guard_name(__lock);	\
	if(!__guard_name.locked() ) assert(0);

#define READ_LOCK_GUARD(__lock,__guard_name) \
	spin_read_lock_guard __guard_name(__lock);	\
	if(!__guard_name.locked() ) assert(0);

enum eSTRUCT_INDEX
{
	STRUCT_INDEX_ROLE,
	// ........
	STRUCT_INDEX_END = _STRUCT_HEAD_SIZE, 
};

/**
 * @class _SH_TRAIT
 *
 * @brief TRAIT
 *
 */
struct _SH_TRAIT_BASE
{
	virtual bool			get_lru() = 0;				//use lru;

	virtual size_t			max_lru_queue_len() = 0;	//max obj number

};

/**
 * @class _SH_TRAIT_ROLE
 *
 * @brief role trait
 *
 */
struct _SH_TRAIT_ROLE : public _SH_TRAIT_BASE
{

	bool			get_lru()
	{
		return true;
	}

	size_t max_lru_queue_len()
	{
		return 1000;
	}
};

/**
 * @class _SH_CONFIG
 *
 * @brief config share memory
 *
 */
#define CREATE_SHARE_MEMORY
struct _SH_CONFIG
{
	virtual size_t	mapping_size()
	{
		size_t __s = 1024*1024;
		__s *= 102 ;
		__s += __s/4;
		return __s; // 1.25GB
	}

	virtual SM_KEY	mapping_key()
	{
		return 12345;
	}

	virtual bool	creator()
	{
#ifdef CREATE_SHARE_MEMORY
		// create
		return true;
#else
		//	use
		return false;
#endif
	}
};

/**
 * @class share_memory
 *
 * @brief 
 *
 */
template < typename _CONFIG >
class share_memory
{
public:
	share_memory():
	_sh_head(0),
	_handle(INVALID_SHAREMEMORY_HANDLE)
	{

	}

	~share_memory()
	{

	}

	/**
	*	name	:	init
	*	params	:	@void
	*	return	:	bool --- if memory exist ; just map it and return false; else create file mapping and map ; return true
	*	specific:	create file mapping
	*	remark	:	
	*	
	*/
	bool init()
	{
		char* __p = 0;
		_handle =	OpenShareMemory(_config.mapping_key(),_config.mapping_size());

		if ( _config.creator() )
		{// creator

			if (_handle != INVALID_SHAREMEMORY_HANDLE)
			{// exist , exception reboot
				__p = MapShareMemory( _handle,(void*)MY_ALIGN(_BASE_ADDR,64*1024) ); // align with 64
				if (!__p)
				{
					SLIM_LOG1( "MapShareMemory failed , handle = [%d]", _handle );
					system("pause");
				}

				_sh_head = reinterpret_cast<SH_MEM_HEAD*>(__p);

				return false;
			}
			else
			{
				_handle = CreateShareMemory(_config.mapping_key(),_config.mapping_size());
				if ( _handle == INVALID_SHAREMEMORY_HANDLE )
				{
					SLIM_LOG1( "CreateShareMemory failed , handle = [%d]", _handle );
					system("pause");
					// return false
				}

				__p = MapShareMemory( _handle,(void*)MY_ALIGN(_BASE_ADDR,64*1024) ); // align with 64
				if (!__p)
				{
					SLIM_LOG1( "MapShareMemory failed , handle = [%d]", _handle );
					system("pause");
				}

				_sh_head = reinterpret_cast<SH_MEM_HEAD*>(__p);
				memset( _sh_head,0,sizeof(SH_MEM_HEAD) );
				_sh_head->head_point = __p + sizeof(SH_MEM_HEAD);
				_sh_head->key = _config.mapping_key();
				_sh_head->mem_valible = 1;
				_sh_head->mem_size = _config.mapping_size();

				return true;
			}


		}
		else
		{ // customer
	
			while ( _handle == INVALID_SHAREMEMORY_HANDLE )
			{
				SLIM_LOG1( "OpenShareMemory failed , continue , handle = [%d] \r\n", _handle );
				OSSleep(1000); // sleep 1 second
				_handle =	OpenShareMemory(_config.mapping_key(),_config.mapping_size());
			}

			SLIM_LOG1( "OpenShareMemory succeed , handle = [%d]", _handle );

			__p = MapShareMemory( _handle,(void*)MY_ALIGN(_BASE_ADDR,64*1024) ); // align with 64

			if (!__p)
			{
				SLIM_LOG1( "MapShareMemory failed , handle = [%d] \r\n", _handle );
				system("pause");
			}

			_sh_head = reinterpret_cast<SH_MEM_HEAD*>(__p);

			return false;
		}
	}


	char* get_head_point()
	{
		assert( _sh_head && _sh_head->head_point );
		return( _sh_head->head_point );
	}

	char* get_tail_point()
	{
		assert( _sh_head && _sh_head->head_point );
		return(_sh_head->head_point + _sh_head->mem_size);
	}

	uint64 get_last_save_time()
	{
		assert( _sh_head );
		return (_sh_head->last_save_time);
	}

	void set_last_save_time(uint64 __time)
	{
		assert( _sh_head );
		_sh_head->last_save_time = __time;
	}

private:

	struct SH_MEM_HEAD
	{
		size_t			mem_size;
		SM_KEY			key;
		uint64			last_save_time;	
		int32			mem_valible;
		char*			head_point;
	};

	SH_MEM_HEAD*	_sh_head;

	SM_HANDLE		_handle;

	_CONFIG			_config;
};




/**
 * @class slim_pool
 *
 * @brief trunk memory pool that build in MEMSOURCE " may be share memory"
 *
 */
template<typename MEMSOURCE,size_t _ALIGN,size_t _MAX_BYTES>
class slim_pool
{
	friend class Singleton<slim_pool<MEMSOURCE,_ALIGN,_MAX_BYTES> , Lock_Null_Mutex >;
public:
	slim_pool():_head_point(0),_struct_head(0)
	{
		bool __res =  _mem_source.init();
		_head_point = _mem_source.get_head_point();
		_struct_head = (void**)_STRUCT_HEAD(_head_point);
		_free_list	= reinterpret_cast<size_t*>( _FREE_LIST_HEAD(_head_point) );
		_start_free = reinterpret_cast<size_t*>( _DATA_HEAD(_head_point) );
		_start_free -= 2;
		_tail_free	= _start_free + 1;
		if (__res) // firstly
			memset(_head_point,0,_DATA_HEAD_OFFSET);

		*_start_free = _DATA_HEAD_OFFSET;
		*_tail_free	= _mem_source.get_tail_point() - _head_point;

		if (*_start_free >= *_tail_free)
		{
			SLIM_LOG1("error: memory too small, chunk memory must be great than [%d]",*_start_free);
			abort();
		}

	}

	~slim_pool()
	{

	}

	void*	allocate(size_t __size)
	{
		void* __ret = 0;

		if (__size > (size_t)_MAX_BYTES || !_free_list)
		{
			SLIM_LOG1("byte size [%d] more than _MAX_BYTES",__size);
			return __ret;
		}

		size_t* volatile __my_free_list = _free_list + _freelist_index(__size);
		if (0 == (*__my_free_list))
		{
			__ret = _refill(_round_up(__size),_DEFAULT_FILL_NODE);
		}
		else
		{
			__ret = _head_point + *__my_free_list;
			*__my_free_list = *reinterpret_cast<size_t*>(__ret);

		}

		SLIM_LOG2("byte size [%d] allocated , at = [0x%p] \n", __size , __ret );

		return __ret;
	}

	void	deallocate(void* __p,size_t __size)
	{
		if (__size > (size_t)_MAX_BYTES || !__p)
		{
			SLIM_LOG1("byte size [%d] more than _MAX_BYTES",__size);
			return;
		}

		SLIM_LOG2( "deallocate , size = [%d] , at = [0x%p] \r\n", __size,__p );

		size_t* volatile __my_free_list = _free_list + _freelist_index(__size);
		_add_to_free_list(__my_free_list,__p);
	}

	void*	get_head_point()
	{
		return _head_point;
	}

	template< typename _TYPE >
	_TYPE*	get_struct( int32 __struct_index )
	{
		void* __p = *(_struct_head + __struct_index);

		if (!__p)
		{
			__p = new (this->allocate( sizeof(_TYPE) ) ) _TYPE();
			*(_struct_head + __struct_index) = __p;
		}

		return reinterpret_cast<_TYPE*>( __p );
	}

	uint64 get_last_save_time()
	{
		return _mem_source.get_last_save_time();
	}

	void set_last_save_time(uint64 __time)
	{
		_mem_source.set_last_save_time(__time);
	}

private:

	size_t	_round_up(size_t __bytes)
	{
		return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1));
	}

	size_t _freelist_index(size_t __bytes)
	{
		return (((__bytes) + (size_t)_ALIGN-1)/(size_t)_ALIGN - 1);
	}

	/**
	*	refill free list with __n*__size;
	*	remark : may be only one node filled
	*/
	void*	_refill(size_t __s,size_t __n)
	{
		void* __ret = 0;
		size_t* volatile __my_free_list = 0;
		size_t	__next = 0;
		size_t	__current = 0;
		char* __chunk = _chunk_mem(__s,__n);


		if (0 == __n || 0 == __chunk)
			return __ret;

		if( 1 == __n)
			return( __ret = __chunk );

		//more than one node be allocated
		__my_free_list = _free_list + _freelist_index(__s);
		__ret = __chunk;
		*__my_free_list = __next =  __chunk + __s - _head_point;


		for(size_t i = 1 ; ; ++i)
		{
			__current = __next;
			__next = __current + __s;
			if (i == __n -1)
			{// set next null
				*reinterpret_cast<size_t*>(_head_point + __current) = 0;
				break;
			}
			else
			{// add to free list
				*reinterpret_cast<size_t*>(_head_point + __current) = __next;
			}
		}

		return __ret;

	}

	char*	_chunk_mem(size_t __s , size_t& __n)
	{
		char* __ret = 0;
		size_t __size_byte_want = __s * __n;
		size_t __size_byte_left = *_tail_free - *_start_free;

		if(__size_byte_left > __size_byte_want)
		{//OK it's enough;
			__ret = _head_point + *_start_free;
			*_start_free += __size_byte_want;

			return __ret;
		}
		else
		{// pool memory lack
			if (__size_byte_left >= __s)
			{// try to get less __n object
				// modify __n;
				__n = (size_t)(__size_byte_left/__s);		// actually number
				__ret = _head_point + *_start_free;


				*_start_free += (__s * __n);

				return __ret;
			}
			else
			{// can't provide , even if the __n is one
			 // try to get memory from _free_list

				if(__size_byte_left > 0)
				{// take the left memory alloc to _free_list
				 	size_t* volatile __my_free_list = _free_list + _freelist_index(__size_byte_left);
					_add_to_free_list(__my_free_list,_head_point + *_start_free);

					*_start_free += __size_byte_left;
				}
				
				size_t* volatile __my_free_list = 0;
				size_t* __p = 0;
				// try to great than __s from _free_list
				for (size_t __i = __s;__i <= (size_t) _MAX_BYTES;__i += (size_t) _ALIGN)
				{
					__my_free_list = _free_list + _freelist_index(__i);
					__p = __my_free_list;

					if (0 != *__p)
					{
						*_start_free = *__p;
						*_tail_free = *_start_free + __i;
						*__my_free_list = *reinterpret_cast<size_t*>( _head_point + *__p );

						// to correct __nobjs
						return(_chunk_mem(__s, __n));

					}
				}

				// if no more memory can be used
				// call lack handler


			}

		}

		return __ret;
	}

	/**
	*	_free_list accessed frequently so volatile is necessary;
	*	access just one current ; so align with cache is useless
	*/
	size_t* volatile	_free_list;

	size_t volatile*	_start_free;
	size_t volatile*	_tail_free;
	char*	_head_point;
	void**	_struct_head;
	MEMSOURCE _mem_source;

};

#define  commom_pool  Singleton<slim_pool<share_memory<_SH_CONFIG>,8,1024> , Lock_Null_Mutex >::instance()

/**
 * @class slim_allocator
 *
 * @brief allocate fixed trunk memory
 *
 */

template<typename _Ty>
class slim_allocator
{

public:
	typedef typename _Ty		value_type;

	typedef value_type*			pointer;
	typedef value_type&			reference;
	typedef value_type const*	const_pointer;
	typedef value_type const&	const_reference;

	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;

	template< class _Other >
	struct rebind
	{	// convert this type to _ALLOCATOR<_Other>
		typedef slim_allocator<_Other> other;
	};

	pointer address(reference _Val) const
	{	// return address of mutable _Val
		return ((pointer) &(char&)_Val);
	}

	const_pointer address(const_reference _Val) const
	{	// return address of nonmutable _Val
		return ((const_pointer) &(char&)_Val);
	}

	slim_allocator() throw()
	{	// construct default allocator (do nothing)
	}

	slim_allocator(const slim_allocator<_Ty>&) throw()
	{	// construct by copying (do nothing)
		// just have static member
	}

	template<class _Other>
	slim_allocator(const slim_allocator<_Other>&) throw()
	{	// construct from a related allocator (do nothing)
	}

	template<class _Other>
	slim_allocator<_Ty>& operator=(const slim_allocator<_Other >&)
	{	// assign from a related allocator (do nothing)
		return (*this);
	}

	void deallocate(pointer _Ptr, size_type)
	{	// deallocate object at _Ptr
		commom_pool->deallocate(_Ptr,sizeof(_Ty));
	}

	pointer allocate(size_type _Count)
	{	// allocate array of _Count elements
		return ( (pointer)commom_pool->allocate(sizeof (_Ty) ) );
	}

	pointer allocate(size_type _Count, const void *)
	{	// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
	}


 	void construct(pointer _Ptr, const _Ty& _Val)
 	{	// construct object at _Ptr with value _Val

		//SLIM_LOG1(" [%p] constructed \n ", _Ptr );

		::new(_Ptr) _Ty(_Val);
 	}

	
 	void construct(pointer _Ptr, _Ty&& _Val)
 	{	// construct object at _Ptr with value _Val

		//SLIM_LOG1(" [%p] constructed  \n", _Ptr );

		::new(_Ptr) _Ty(_Val);
 	}


 	template<class _Other>
 	void construct(pointer _Ptr, _Other&& _Val)
 	{	// construct object at _Ptr with value _Val

		//SLIM_LOG1(" [%p] constructed \n ", _Ptr );

		::new(_Ptr) _Ty(_Val);
 	}

	void destroy(pointer _Ptr)
	{	// destroy object at _Ptr
		_Ptr->~_Ty();
	}

	size_t max_size() const throw()
	{	// estimate maximum array size
		size_t _Count = (size_t)(-1) / sizeof (_Ty);
		return (0 < _Count ? _Count : 1);
	}
};


/**
 * @class AccessAdaptor
 *
 * @brief provide efficiently traverse
 *
 */
 template < typename _Kty , typename _Vty >
class	AccessAdaptor
{
	typedef _Kty																											key_type;
	typedef _Vty																											val_type;
	typedef std::list<val_type,slim_allocator<val_type> >																	my_list_type;
	typedef typename my_list_type::iterator																					my_list_it_type;
	typedef	std::map<key_type,my_list_it_type,std::less<key_type>,slim_allocator<std::pair<key_type, my_list_it_type> > >	my_map_type;
	typedef typename my_map_type::iterator																					my_map_it_type;

public:
	typedef typename my_list_it_type																						iterator;
	typedef spin_rw_lock																									lock_type;

public:
	AccessAdaptor():_lock(){}
	~AccessAdaptor(){}

	iterator	find(key_type const& __key)
	{
		my_map_it_type _map_it_l = _map.find(__key);
		if (_map_it_l != _map.end())
		{
			return _map_it_l->second;
		}
		return _list.end();
	}

	iterator	begin()
	{
		return _list.begin();
	}

	iterator	end()
	{
		return _list.end();
	}

	// if left value need lock
	val_type& operator [](key_type const& __key)
	{
		WRITE_LOCK_GUARD(_lock,__write_guard);
		return _map[__key];
	}

	bool	erase(key_type const& __key)
	{
		WRITE_LOCK_GUARD(_lock,__write_guard);
		my_map_it_type _map_it_l = _map.find(__key);
		if (_map_it_l != _map.end())
		{
			_list.erase(_map_it_l->second);
			_map.erase(_map_it_l);
			return true;
		}
		return false;
	}

	void	insert(key_type const& __key)
	{

	}

private:
	my_map_type			_map;
	my_list_type		_list;
	lock_type			_lock;
};

#endif // slim_allocator_h__