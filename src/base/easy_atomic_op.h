/********************************************************************
	created:	2011/10/28
	created:	28:10:2011   15:53
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_atomic_op.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_atomic_op
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_atomic_op_h__
#define easy_atomic_op_h__

#ifndef easy_mutex_h__
#include "easy_mutex.h"
#endif //easy_thread_mutex_h__

#ifndef easy_atomic_op_t_h__
#include "easy_atomic_op_t.h"
#endif //easy_atomic_op_t_h__

namespace easy {
template<>
class  EasyAtomicOp<EasyMutex, easy_long> {
  public:
    // Initialize @c value_ to 0.
    EasyAtomicOp (void) : value_ (0) { }

    // Initialize @c value_ to c.
    EasyAtomicOp (easy_long c) :value_(c) { }

    // Manage copying...
    EasyAtomicOp( const EasyAtomicOp<EasyMutex,easy_long>& c) :value_(c.value_) { }

    // Atomically pre-increment @c value_.
    easy_long operator++ (void) {
#ifdef __WINDOWS
        return ::InterlockedIncrement (const_cast<long *> (&this->value_));
#elif __LINUX
        return (*increment_fn_) (&this->value_);
#endif
    }

    // Atomically post-increment @c value_.
    easy_long operator++ (easy_int32) {
        return ++*this - 1;
    }

    // Atomically pre-decrement @c value_.
    easy_long operator-- (void) {
#ifdef __WINDOWS
        return ::InterlockedDecrement (const_cast<long *> (&this->value_));
#elif __LINUX
        return (*decrement_fn_) (&this->value_);
#endif
    }

    // Atomically post-decrement @c value_.
    easy_long operator-- (easy_int32) {
        return --*this + 1;
    }

    // Atomically increment @c value_ by rhs.
    easy_long operator+= (easy_long rhs) {
#ifdef __WINDOWS
        return ::InterlockedExchangeAdd (const_cast<long *> (&this->value_),rhs) + rhs;
#elif __LINUX
        return (*exchange_add_fn_) (&this->value_, rhs) + rhs;
#endif
    }

    // Atomically decrement @c value_ by rhs.
    easy_long operator-= (easy_long rhs) {
#ifdef __WINDOWS
        return ::InterlockedExchangeAdd (const_cast<long *> (&this->value_),-rhs) - rhs;
#elif __LINUX
        return (*exchange_add_fn_) (&this->value_, -rhs) - rhs;
#endif
    }

    // Atomically compare @c value_ with rhs.
    easy_bool operator== (easy_long rhs) const {
        return (this->value_ == rhs);
    }

    // Atomically compare @c value_ with rhs.
    easy_bool operator!= (easy_long rhs) const {
        return (this->value_ != rhs);
    }

    // Atomically check if @c value_ greater than or equal to rhs.
    easy_bool operator>= (easy_long rhs) const {
        return (this->value_ >= rhs);
    }

    // Atomically check if @c value_ greater than rhs.
    easy_bool operator> (easy_long rhs) const {
        return (this->value_ > rhs);
    }

    // Atomically check if @c value_ less than or equal to rhs.
    easy_bool operator<= (easy_long rhs) const {
        return (this->value_ <= rhs);
    }

    // Atomically check if @c value_ less than rhs.
    easy_bool operator< (easy_long rhs) const {
        return (this->value_ < rhs);
    }

    // Atomically assign rhs to @c value_.
    EasyAtomicOp<EasyMutex, easy_long> &operator= (easy_long rhs) {
#ifdef __WINDOWS
        ::InterlockedExchange (const_cast<long *> (&this->value_), rhs);
#elif __LINUX
        (*exchange_fn_) (&this->value_, rhs);
#endif
        return *this;
    }

    // Atomically assign <rhs> to @c value_.
    EasyAtomicOp<EasyMutex, easy_long> &operator= (const EasyAtomicOp<EasyMutex, easy_long> &rhs) {
#ifdef __WINDOWS
        ::InterlockedExchange (const_cast<long *> (&this->value_), rhs.value_);
#elif __LINUX
        (*exchange_fn_) (&this->value_, rhs.value_);
#endif
        return *this;
    }

    // Explicitly return @c value_.
    easy_long value (void) const {
        return value_;
    }

    // Dump the state of an object.
    void dump (void) const { }

    // Explicitly return @c value_ (by reference).
    volatile easy_long &value_i (void) {
        return this->value_;
    }

    // Used during  object manager initialization to optimize the fast
    // atomic op implementation according to the number of CPUs.
    static void init_functions (void) { }

  private:

    // This function cannot be supported by this template specialization.
    // If you need access to an underlying lock, use the EasyAtomicOpEx
    // template instead.
    EasyMutex &mutex (void);

    // Current object decorated by the atomic op.
    volatile easy_long value_;

    // Pointers to selected atomic op implementations.
    static long (*increment_fn_) (volatile long *);
    static long (*decrement_fn_) (volatile long *);
    static long (*exchange_fn_) (volatile long *, long);
    static long (*exchange_add_fn_) (volatile long *, long);
};
}

#endif // easy_atomic_op_h__
