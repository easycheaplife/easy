/********************************************************************
	created:	2011/10/13
	created:	13:10:2011   16:52
	file base:	easy_reference
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_reference_h__
#define easy_reference_h__

#ifndef easy_linked_list_h__
#include "easy_linked_list.h"
#endif //easy_linked_list_h__

namespace easy {
template< typename TO, typename FROM >
class Reference	: public LinkedListElement {
  private:
    TO* ref_to_;
    FROM* ref_from_;
  protected:
    // Tell our refTo (target) object that we have a link
    virtual void TargetObjectBuildLink() = 0;

    // Tell our refTo (taget) object, that the link is cut
    virtual void TargetObjectDestroyLink() = 0;

    // Tell our refFrom (source) object, that the link is cut (Target destroyed)
    virtual void SourceObjectDestroyLink() = 0;
  public:
    Reference() {
        ref_to_ = easy_null;
        ref_from_ = easy_null;
    }
    virtual ~Reference() {}

    //	create a new link
    void Link(TO* to_obj, FROM* from_obj) {
        assert(from_obj);                                // fromObj MUST not be NULL
        if(IsValid()) {
            UnLink();
        }
        if(to_obj != easy_null) {
            ref_to_ = to_obj;
            ref_from_ = from_obj;
            TargetObjectBuildLink();
        }
    }

    // We don't need the reference anymore. Call comes from the refFrom object
    // Tell our refTo object, that the link is cut
    void UnLink() {
        TargetObjectDestroyLink();
        DeLink();
        ref_to_ = easy_null;
        ref_from_ = easy_null;
    }

    // Link is invalid due to destruction of referenced target object. Call comes from the refTo object
    // Tell our refFrom object, that the link is cut
    void Invalidate() {                                 // the iRefFrom MUST remain!!
        SourceObjectDestroyLink();
        DeLink();
        ref_to_ = easy_null;
    }

    bool IsValid() const {                              // Only check the iRefTo
        return ref_to_ != easy_null;
    }

    Reference<TO, FROM>       * next()       {
        return((Reference<TO, FROM>       *) LinkedListElement::next());
    }
    Reference<TO, FROM> const* next() const {
        return((Reference<TO, FROM> const *) LinkedListElement::next());
    }
    Reference<TO, FROM>       * prev()       {
        return((Reference<TO, FROM>       *) LinkedListElement::prev());
    }
    Reference<TO, FROM> const* prev() const {
        return((Reference<TO, FROM> const *) LinkedListElement::prev());
    }

    Reference<TO, FROM>       * nocheck_next()       {
        return((Reference<TO, FROM>       *) LinkedListElement::nocheck_next());
    }
    Reference<TO, FROM> const* nocheck_next() const {
        return((Reference<TO, FROM> const *) LinkedListElement::nocheck_next());
    }
    Reference<TO, FROM>       * nocheck_prev()       {
        return((Reference<TO, FROM>       *) LinkedListElement::nocheck_prev());
    }
    Reference<TO, FROM> const* nocheck_prev() const {
        return((Reference<TO, FROM> const *) LinkedListElement::nocheck_prev());
    }

    TO* operator ->() const {
        return ref_to_;
    }
    TO* GetTarget() const {
        return ref_to_;
    }

    FROM* GetSource() const {
        return ref_from_;
    }
};
}
#endif // easy_reference_h__
