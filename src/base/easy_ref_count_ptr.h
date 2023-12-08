/********************************************************************
	created:	2011/10/26
	created:	26:10:2011   12:20
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_ref_count_ptr.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_ref_count_ptr
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_ref_count_ptr_h__
#define easy_ref_count_ptr_h__

namespace easy {
template<class T>
class EasyRefCountPtr {
  public:
    EasyRefCountPtr(T* real_ptr = 0) : pointee_( real_ptr ) {
        _init();
    }

    EasyRefCountPtr(const EasyRefCountPtr& rhs) : pointee_( rhs.pointee_) {
        _init();
    }

    ~EasyRefCountPtr() {
        if (pointee_) {
            pointee_->RemoveReference();
        }
    }

    EasyRefCountPtr& operator= (const EasyRefCountPtr& rhs) {
        if (pointee_ != rhs.pointee_) {
            if (pointee_) {
                pointee_->RemoveReference();
                pointee_ = rhs.pointee_;
                _init();
            }
        }
        return *this;
    }

    T* operator -> () const {
        return pointee_;
    }

    T& operator * () const {
        return *pointee_;
    }
  private:
    void _init();

    T* pointee_;
};

template<class T>
void easy::EasyRefCountPtr<T>::_init() {
    if (0 == pointee_) {
        return;
    }
    if (false == pointee_->shareable()) {
        pointee_ = new T(*pointee_);
    }
    pointee_->AddReference();
}
}
#endif // easy_ref_count_ptr_h__