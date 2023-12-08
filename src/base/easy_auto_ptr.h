/********************************************************************
	created:	2011/10/28
	created:	28:10:2011   18:28
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_auto_ptr.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_auto_ptr
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_auto_ptr_h__
#define easy_auto_ptr_h__

namespace easy {
template <typename X>
class EasyAutoBasicPtr {
  public:
    typedef X element_type;

    explicit EasyAutoBasicPtr (X * p = 0) : p_ (p) {}

    EasyAutoBasicPtr (EasyAutoBasicPtr<X> & ap): p_ (rhs.release ()) { }

    EasyAutoBasicPtr<X> &operator= (EasyAutoBasicPtr<X> & rhs) {
        if (this != &rhs) {
            this->reset (rhs.release ());
        }
        return *this;
    }

    ~EasyAutoBasicPtr (void) {
        delete this->get ();
    }

    X &operator *() const {
        return *p_;
    }

    X *get (void) const {
        return this->p_;
    }

    X *release (void) {
        X *old = this->p_;
        this->p_ = 0;
        return old;
    }

    void reset (X * p = 0) {
        if (this->get () != p) {
            delete this->get ();
        }
        this->p_ = p;
    }

  protected:
    X *p_;
};
}
#endif // easy_auto_ptr_h__