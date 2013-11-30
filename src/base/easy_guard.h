/********************************************************************
	created:	2012/03/23
	created:	23:3:2012   14:34
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_guard.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_guard
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_guard_h__
#define easy_guard_h__

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

namespace easy
{
	template<typename LOCK>
	class guard
	{
	public:
		guard(LOCK& __lock) : lock_(&__lock),owner_(0)
		{
			this->acquire();
		}

		guard(LOCK& __lock,easy_bool __block) : lock_(&__lock),owner_(0)
		{
			if (__block)
			{
				this->acquire();
			}
			else
			{
				this->tryacquire();
			}
		}

		guard(LOCK& __lock,easy_bool __block,easy_int32 __become_owner) : lock_(&__lock),owner_(__become_owner == 0 ? -1 : 0 ){ }

		~guard()
		{
			this->release();
		}

		easy_int32 acquire(void)
		{
			return this->owner_ = this->lock_->acquire();
		}

		easy_int32 tryacquire(void)
		{
			return this->owner_ = this->lock_->tryacquire();
		}

		easy_int32 release()
		{
			if (this->owner_ == -1)
			{
				return -1;
			}
			else
			{
				this->owner_ = -1;
				return this->lock_->release();
			}
		}

		easy_int32 locked() const 
		{
			return this->owner_ == -1;
		}

		void remove()
		{
			return this->lock_->remove();
		}

		//	relinquish ownership of the lock so that it is not released implicitly in the destructor.
		void disown()
		{
			this->owner_ = -1;
		}
	protected:
		guard(LOCK* __lock) : lock_(lock_),owner_(0) { }

		LOCK* lock_;

		//	keep track of whether we acquired the lock or failed
		easy_int32 owner_;
	private:
		guard(const guard&);
		guard operator = (const guard&);
	};

	template<typename LOCK>
	class write_guard : public guard<LOCK>
	{
	public:
		write_guard(LOCK& __lock) : guard<LOCK>(&__lock)
		{
			this->acquire_write();
		}

		write_guard(LOCK& __lock,easy_bool __block) : guard<LOCK>(&__lock)
		{
			if (__block)
			{
				this->acquire_write();
			}
			else
			{
				this->tryacquire_write();
			}
		}

		easy_int32 acquire()
		{
			return this->owner_ = this->lock_->acquire_write();
		}

		easy_int32 tryacquire()
		{
			return this->owner_ = this->lock_->tryacquire_write();
		}

		easy_int32 acquire_write()
		{
			return this->owner_ = this->lock_->acquire_write();
		}

		easy_int32 tryacquire_write()
		{
			return this->owner_ = this->lock_->tryacquire_write();
		}
	};

	template<typename LOCK>
	class read_guard : public guard<LOCK>
	{
	public:
		read_guard(LOCK& __lock) : guard<LOCK>(&__lock)
		{
			this->acquire_read();
		}

		read_guard(LOCK& __lock,easy_bool __block) : guard<LOCK>(&__lock)
		{
			if (__block)
			{
				this->acquire_read();
			}
			else
			{
				this->tryacquire_read();
			}
		}

		easy_int32 acquire_read (void)
		{
			return this->owner_ = this->lock_->acquire_read();
		}

		easy_int32 acquire (void)
		{
			return this->owner_ = this->lock_->acquire_read();
		}

		easy_int32 tryacquire_read (void)
		{
			return this->owner_ = this->lock_->tryacquire_read();
		}

		easy_int32 tryacquire (void)
		{
			return this->owner_ = this->lock_->tryacquire_read();
		}
	};
}

#if !defined (EASY_GUARD_ACTION)
#define EASY_GUARD_ACTION(MUTEX, OBJ, LOCK, ACTION, REACTION) \
	easy::guard< MUTEX > OBJ (LOCK); \
	if (OBJ.locked () != 0) { ACTION; } \
   else { REACTION; }
#endif /* !EASY_GUARD_ACTION */

#if !defined (EASY_GUARD_REACTION)
#define EASY_GUARD_REACTION(MUTEX, OBJ, LOCK, REACTION) \
	EASY_GUARD_ACTION(MUTEX, OBJ, LOCK, ;, REACTION)
#endif /* !EASY_GUARD_REACTION */

#if !defined (EASY_GUARD)
#define EASY_GUARD(MUTEX, OBJ, LOCK) \
	EASY_GUARD_REACTION(MUTEX, OBJ, LOCK, return)
#endif /* !EASY_GUARD */

#if !defined (EASY_GUARD_RETURN)
#define EASY_GUARD_RETURN(MUTEX, OBJ, LOCK, RETURN) \
	EASY_GUARD_REACTION(MUTEX, OBJ, LOCK, return RETURN)
#endif /* !EASY_GUARD_RETURN */

#if !defined (EASY_WRITE_GUARD)
# define EASY_WRITE_GUARD(MUTEX,OBJ,LOCK) \
	easy::write_guard< MUTEX > OBJ (LOCK); \
	if (OBJ.locked () == 0) return;
#endif /* !EASY_WRITE_GUARD */

#if !defined (EASY_WRITE_GUARD_RETURN)
# define EASY_WRITE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
	easy::write_guard< MUTEX > OBJ (LOCK); \
	if (OBJ.locked () == 0) return RETURN;
#endif /* EASY_WRITE_GUARD_RETURN */

#if !defined (EASY_READ_GUARD)
# define ACE_READ_GUARD(MUTEX,OBJ,LOCK) \
	easy::read_guard< MUTEX > OBJ (LOCK); \
	if (OBJ.locked () == 0) return;
#endif /* !EASY_READ_GUARD */

#if !defined (EASY_READ_GUARD_RETURN)
# define EASY_READ_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
	easy::read_guard< MUTEX > OBJ (LOCK); \
	if (OBJ.locked () == 0) return RETURN;
#endif /* !EASY_READ_GUARD_RETURN */

#endif // easy_guard_h__