/********************************************************************
	created:	2011/10/26
	created:	26:10:2011   12:20
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_ref_count_object.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_ref_count_object
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_ref_count_object_h__
#define easy_ref_count_object_h__

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

namespace easy {
class EasyRefCountObject {
  public:
    void AddReference() {
        ++ref_count_;
    }

    void RemoveReference() {
        --ref_count_;
        if ( 0 == ref_count_) {
            delete this;
        }
    }

    void MarkUnShareable() { }

    easy_bool shareable() {
        return shareable_;
    }

    easy_uint32 IsShared() {
        return ref_count_ > 1;
    }

  protected:
    EasyRefCountObject() : ref_count_(0), shareable_(true) { }

    EasyRefCountObject(const EasyRefCountObject& rhs) : ref_count_(0), shareable_(true) { }

    EasyRefCountObject& operator = (const EasyRefCountObject& rhs) {
        return *this;
    }

  private:
    easy_uint32 ref_count_;

    easy_bool shareable_;
};
}
#endif // easy_ref_count_object_h__