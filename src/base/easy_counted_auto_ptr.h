/********************************************************************
	created:	2011/10/28
	created:	28:10:2011   17:47
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_counted_auto_ptr.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_counted_auto_ptr
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_counted_auto_ptr_h__
#define easy_counted_auto_ptr_h__

#ifndef easy_auto_ptr_h__
#include "easy_auto_ptr.h"
#endif //easy_auto_ptr_h__

#ifndef easy_atomic_op_h__
#include "easy_atomic_op.h"
#endif //easy_atomic_op_h__

namespace easy {
// Forward declare.
template <class X, class LOCK> class EasyRefcountedAutoPtrRep;
template <class X, class LOCK> class EasyRefcountedAutoPtr;

template <class X, class LOCK>
class EasyRefcountedAutoPtr {
  public:
    //	Constructor that initializes an EasyRefcountedAutoPtr to the specified pointer value.
    explicit EasyRefcountedAutoPtr (X *p = 0)
        : rep_ (AUTO_REFCOUNTED_PTR_REP::create (p)) {}

    //	Copy constructor binds the new EasyRefcountedAutoPtr to the representation object referenced by @a r.
    //	An EasyRefcountedAutoPtrRep is created if necessary.
    EasyRefcountedAutoPtr (const EasyRefcountedAutoPtr<X, LOCK> &r)
        : rep_ (AUTO_REFCOUNTED_PTR_REP::attach (((EasyRefcountedAutoPtr<X, LOCK> &) r).rep_)) {}

    /// Destructor. Releases the reference to the underlying representation.
    /// If the release of that reference causes its reference count to reach 0,
    /// the representation object will also be destroyed.
    virtual ~EasyRefcountedAutoPtr (void) {
        AUTO_REFCOUNTED_PTR_REP::detach (rep_);
    }

    /// Assignment operator that binds the current object and @a r to the same
    /// EasyRefcountedAutoPtrRep. An EasyRefcountedAutoPtrRep
    /// is created if necessary.
    void operator = (const EasyRefcountedAutoPtr<X, LOCK> &r) {
        //  bind <this> to the same <EasyRefcountedAutoPtrRep> as <r>.
        AUTO_REFCOUNTED_PTR_REP *old_rep = this->rep_;
        if (rhs.rep_ != 0) {
            this->rep_ = AUTO_REFCOUNTED_PTR_REP::attach(const_cast<EasyRefcountedAutoPtr<X, LOCK>& > (rhs).rep_);
            if (this->rep_ != 0)
                AUTO_REFCOUNTED_PTR_REP::detach (old_rep);
        } else { // Assign a 0 rep to this
            AUTO_REFCOUNTED_PTR_REP::detach (old_rep);
            this->rep_ = 0;
        }
    }

    /// Equality operator that returns @c true if both
    /// ACE_Refcounted_Auto_Ptr objects point to the same underlying
    /// representation. It does not compare the actual pointers.
    /**
    * @note It also returns @c true if both objects have just been
    *       instantiated and not used yet.
    */
    bool operator == (const EasyRefcountedAutoPtr<X, LOCK> &r) const {
        return r.rep_ == this->rep_;
    }

    /// Inequality operator, which is the opposite of equality.
    bool operator != (const EasyRefcountedAutoPtr<X, LOCK> &r) const {
        return r.rep_ != this->rep_;
    }

    /// Redirection operator
    X *operator-> (void) const {
        return this->rep_->get();
    }

    /// Accessor method.
    X &operator *() const {
        return *this->rep_->get ();
    }

    /// Check rep easily.
    bool operator !() const {
        return this->rep_->get () == 0;
    }

    /// Check rep easily.
    operator bool () const {
        return this->rep_->get () != 0;
    }

    /// Releases the reference to the underlying representation object.
    /// @retval The pointer value prior to releasing it.
    X *release (void) {
        X *p = this->get ();
        AUTO_REFCOUNTED_PTR_REP::detach (this->rep_);
        this->rep_ = 0;
        return p;
    }

    /// Releases the current pointer value and then sets a new
    /// pointer value specified by @a p.
    void reset (X *p = 0) {
        // Avoid deleting the underlying auto_ptr if assigning the same actual
        // pointer value.
        if (this->get () == p)
            return;

        AUTO_REFCOUNTED_PTR_REP *old_rep = this->rep_;
        if ((this->rep_ = AUTO_REFCOUNTED_PTR_REP::create (p)) != 0)
            AUTO_REFCOUNTED_PTR_REP::detach (old_rep);
        else
            this->rep_ = old_rep;
        return;
    }

    /// Get the pointer value.
    X *get (void) const {
        return this->rep_->get ();
    }

    /// Get the reference count value.
    long count (void) const {
        return this->ref_count_.value();
    }

    /// Returns @c true if this object does not contain a valid pointer.
    bool null (void) const {
        return (this->rep_ == 0 || this->rep_->get () == 0);
    }

  protected:
    /// the ACE_Refcounted_Auto_Ptr_Rep
    typedef EasyRefcountedAutoPtrRep<X, LOCK> AUTO_REFCOUNTED_PTR_REP;

    /// Protect operations on the ACE_Refcounted_Auto_Ptr.
    AUTO_REFCOUNTED_PTR_REP *rep_;
};

template <class X, class LOCK>
class EasyRefcountedAutoPtrRep {
  private:
    friend class EasyRefcountedAutoPtr<X, LOCK>;

    /// Get the pointer value.
    X *get (void) const {
        return this->ptr_.get ();
    }

    /// Get the reference count value.
    long count (void) const {
        return this->ref_count_.value();
    }

    /// Allocate a new EasyRefcountedAutoPtrRep<X, LOCK> instance,
    /// returning NULL if it cannot be created.
    static EasyRefcountedAutoPtrRep<X, LOCK> *internal_create (X *p) {
        EasyRefcountedAutoPtrRep<X, LOCK> *temp = 0;
        temp = new (EasyRefcountedAutoPtrRep<X, LOCK>) (p);
        return temp;
    }

    /// Create a EasyRefcountedAutoPtrRep<X, LOCK> and initialize
    /// the reference count.
    static EasyRefcountedAutoPtrRep<X, LOCK> *create (X *p) {
        // Yes set ref count to zero.
        EasyRefcountedAutoPtrRep<X, LOCK> *temp = internal_create (p);
        return temp;
    }

    /// Increase the reference count on @a rep.
    ///
    /// @retval @a rep if success, 0 if there's an error obtaining the lock
    ///         on @a rep.
    static EasyRefcountedAutoPtrRep<X, LOCK> *attach (EasyRefcountedAutoPtrRep<X, LOCK> *&rep) {
        if (rep == 0)
            return 0;

        ++rep->ref_count_;

        return rep;
    }

    /// Decreases the reference count and and deletes rep if there are no
    /// more references to rep.
    ///
    /// Precondition (rep != 0)
    static void detach (EasyRefcountedAutoPtrRep<X, LOCK> *&rep) {
        if (rep == 0)
            return;

        if (rep->ref_count_-- == 0)
            delete rep;
    }

    // = Constructor and destructor private.
    EasyRefcountedAutoPtrRep (X *p = 0)
        : ptr_ (p),
          ref_count_ (0) { }

    ~EasyRefcountedAutoPtrRep (void) { }

    /// Pointer to the result.
    EasyAutoBasicPtr<X> ptr_;

    /// Reference count.
    mutable EasyAtomicOp<LOCK, long> ref_count_;
};
}
#endif // easy_counted_auto_ptr_h__