/********************************************************************
	created:	2011/10/28
	created:	28:10:2011   15:52
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_atomic_op_t.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_atomic_op_t
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_atomic_op_t_h__
#define easy_atomic_op_t_h__

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

#include <algorithm>

namespace easy {
template<typename TYPE>
struct EasyTypeTrait {
    typedef TYPE const& parameter_type;
};

template<>
struct EasyTypeTrait<easy_bool> {
    typedef easy_bool parameter_type;
};

template<>
struct EasyTypeTrait<easy_char> {
    typedef easy_char parameter_type;
};

template<>
struct EasyTypeTrait<easy_schar> {
    typedef easy_schar parameter_type;
};

template<>
struct EasyTypeTrait<easy_uint8> {
    typedef easy_uint8 parameter_type;
};

template<>
struct EasyTypeTrait<easy_int16> {
    typedef easy_int16 parameter_type;
};

template<>
struct EasyTypeTrait<easy_uint16> {
    typedef easy_uint16 parameter_type;
};

template<>
struct EasyTypeTrait<easy_int32> {
    typedef easy_int32 parameter_type;
};

template<>
struct EasyTypeTrait<easy_uint32> {
    typedef easy_uint32 parameter_type;
};

template<>
struct EasyTypeTrait<easy_long> {
    typedef easy_long parameter_type;
};

template<>
struct EasyTypeTrait<easy_ulong> {
    typedef easy_ulong parameter_type;
};

template<>
struct EasyTypeTrait<easy_int64> {
    typedef easy_int64 parameter_type;
};

template<>
struct EasyTypeTrait<easy_uint64> {
    typedef easy_uint64 parameter_type;
};

template<>
struct EasyTypeTrait<easy_float> {
    typedef easy_float parameter_type;
};

template<>
struct EasyTypeTrait<easy_double> {
    typedef easy_double parameter_type;
};

template<typename TYPE>
struct EasyTypeTrait<TYPE*> {
    typedef TYPE* parameter_type;
};

template <class LOCK, typename TYPE>
class EasyAtomicOpEx {
  public:
    typedef typename EasyTypeTrait<TYPE>::parameter_type arg_type;

    EasyAtomicOpEx (LOCK & mtx) :mutex_(mtx), value_(0) { }

    // Initialize @c value_ to c.
    EasyAtomicOpEx (LOCK & mtx, arg_type c) :mutex_(mtx),value_(c) { }

    // Atomically pre-increment @c value_.
    TYPE operator++ (void) {
        return ++this->value_;
    };

    // Atomically post-increment @c value_.
    TYPE operator++ (int) {
        return this->value_++;
    }

    // Atomically increment @c value_ by rhs.
    TYPE operator+= (arg_type rhs) {
        return this->value_ += rhs;
    }

    // Atomically pre-decrement @c value_.
    TYPE operator-- (void) {
        return --this->value_;
    }

    // Atomically post-decrement @c value_.
    TYPE operator-- (int) {
        return this->value_--;
    }

    // Atomically decrement @c value_ by rhs.
    TYPE operator-= (arg_type rhs) {
        return this->value_ -= rhs;
    }

    // Atomically compare @c value_ with rhs.
    easy_bool operator== (arg_type rhs) const {
        return this->value_ == rhs;
    }

    // Atomically compare @c value_ with rhs.
    bool operator!= (arg_type rhs) const {
        return this->value_ != rhs;
    }

    // Atomically check if @c value_ greater than or equal to rhs.
    bool operator>= (arg_type rhs) const {
        return this->value_ >= rhs;
    }

    // Atomically check if @c value_ greater than rhs.
    bool operator> (arg_type rhs) const {
        return this->value_ > rhs;
    }

    // Atomically check if @c value_ less than or equal to rhs.
    bool operator<= (arg_type rhs) const {
        return this->value_ <= rhs;
    }

    // Atomically check if @c value_ less than rhs.
    bool operator< (arg_type rhs) const {
        return this->value_ < rhs;
    }

    // Atomically assign rhs to @c value_.
    EasyAtomicOpEx<LOCK, TYPE> &operator= (arg_type rhs) {
        EasyAtomicOpEx<LOCK, TYPE> tmp (rhs);
        std::swap (this->value_, tmp.value_);
        return *this;
    }

    // Atomically assign <rhs> to @c value_.
    EasyAtomicOpEx<LOCK, TYPE> &operator= ( EasyAtomicOpEx<LOCK, TYPE> const & rhs) {
        this->value_ = rhs.value_;
        return *this;
    }

    // Explicitly return @c value_.
    TYPE value (void) const {
        return this->value_;
    }

    // Dump the state of an object.
    void dump (void) const { }

    /// Manage copying...
    EasyAtomicOpEx (EasyAtomicOpEx<LOCK, TYPE> const & rhs)
        : mutex_ (rhs.mutex_)
        , value_ (rhs.value ()) { // rhs.value() returns atomically

    }

    /**
    * Returns a reference to the underlying LOCK.  This makes it
    * possible to acquire the lock explicitly, which can be useful in
    * some cases if you instantiate the EasyAtomicOpEx with an
    * EasyRecursiveMutex or EasyProcess_Mutex.
    *
    * @note The right name would be lock_, but HP/C++ will choke on that!
    */
    LOCK & mutex (void) {
        return mutex_;
    }

    /**
    * Explicitly return @c value_ (by reference).  This gives the user
    * full, unrestricted access to the underlying value.  This method
    * will usually be used in conjunction with explicit access to the
    * lock.  Use with care ;-)
    */
    TYPE & value (void) {
        return value_;
    }

  private:
    // Type of synchronization mechanism.
    LOCK & mutex_;

    // Current object decorated by the atomic op.
    TYPE value_;
};

template <class LOCK, typename TYPE>
class EasyAtomicOp {
  public:

    typedef typename EasyTypeTrait<TYPE>::parameter_type arg_type;

    // Initialize @c value_ to 0.
    EasyAtomicOp (void) : impl_(own_mutex_) { }

    // Initialize @c value_ to c.
    EasyAtomicOp (arg_type c) :impl_(own_mutex_,c) { }

    // Atomically assign @a rhs to @c value_.
    EasyAtomicOp<LOCK, TYPE> & operator = (arg_type rhs) {
        this->impl_ = rhs;
        return *this;
    }

    // Atomically assign @a rhs to @c value_.
    EasyAtomicOp<LOCK, TYPE> & operator= (EasyAtomicOp<LOCK, TYPE> const & rhs) {
        this->impl_ = rhs.impl_;
        return *this;
    }

    // Atomically pre-increment @c value_.
    easy_long operator++ (void) {
        return ++this->impl_;
    }

    // Atomically post-increment @c value_.
    easy_long operator++ (int) {
        return this->impl_++;
    }

    // Atomically increment @c value_ by rhs.
    easy_long operator+= (long rhs) {
        return this->impl_ += rhs;
    }

    // Atomically decrement @c value_ by rhs.
    easy_long operator-= (long rhs) {
        return this->impl_ -= rhs;
    }

    // Atomically pre-decrement @c value_.
    easy_long operator-- (void) {
        return --this->impl_;
    }

    // Atomically post-decrement @c value_.
    easy_long operator-- (int) {
        return this->impl_--;
    }

    // Atomically compare @c value_ with rhs.
    easy_bool operator== (easy_long rhs) const {
        return this->impl_ == rhs;
    }

    // Atomically compare @c value_ with rhs.
    easy_bool operator!= (easy_long rhs) const {
        return this->impl_ != rhs;
    }

    // Atomically check if @c value_ greater than or equal to rhs.
    easy_bool operator>= (easy_long rhs) const {
        return this->impl_ >= rhs;
    }

    // Atomically check if @c value_ greater than rhs.
    easy_bool operator> (easy_long rhs) const {
        return this->impl_ > rhs;
    }

    // Atomically check if @c value_ less than or equal to rhs.
    easy_bool operator<= (easy_long rhs) const {
        return this->impl_ <= rhs;
    }

    // Atomically check if @c value_ less than rhs.
    easy_bool operator< (easy_long rhs) const {
        return this->impl_ < rhs;
    }

    // Explicitly return @c value_.
    TYPE value (void) const {
        return this->impl_.value();
    }

    // Dump the state of an object.
    void dump (void) const {
        return this->impl_.dump();
    }

    /**
    * Explicitly return @c value_ (by reference).  This gives the user
    * full, unrestricted access to the underlying value.  This method
    * will usually be used in conjunction with explicit access to the
    * lock.  Use with care ;-)
    */
    TYPE & value_i (void) {
        return this->impl_.value_i ();
    }

  private:
    // Type of synchronization mechanism.
    LOCK own_mutex_;

    // Underlying atomic op implementation.
    EasyAtomicOpEx <LOCK, TYPE> impl_;
};
}
#endif // easy_atomic_op_t_h__