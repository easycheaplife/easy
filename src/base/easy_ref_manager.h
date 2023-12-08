/********************************************************************
	created:	2011/10/13
	created:	13:10:2011   16:53
	file base:	easy_ref_manager
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_ref_manager_h__
#define easy_ref_manager_h__

#ifndef easy_linked_list_h__
#include "easy_linked_list.h"
#endif //easy_linked_list_h__

namespace easy {
template< typename TO, typename FROM >
class RefManager : public LinkedListHead {
  public:
    typedef LinkedListHead::Iterator< Reference<TO, FROM> > iterator;

    RefManager() { }

    virtual ~RefManager() {
        ClearReferences();
    }

    Reference<TO, FROM>* GetFirst() {
        return ((Reference<TO, FROM>*) LinkedListHead::GetFirst());
    }
    Reference<TO, FROM> const* GetFirst() const {
        return ((Reference<TO, FROM> const*) LinkedListHead::GetFirst());
    }
    Reference<TO, FROM>* GetLast() {
        return ((Reference<TO, FROM>*) LinkedListHead::GetLast());
    }
    Reference<TO, FROM> const* GetLast() const {
        return ((Reference<TO, FROM> const*) LinkedListHead::GetLast());
    }

    iterator begin() {
        return iterator(getFirst());
    }
    iterator end() {
        return iterator(easy_null);
    }
    iterator rbegin() {
        return iterator(getLast());
    }
    iterator rend() {
        return iterator(easy_null);
    }

    void ClearReferences() {
        LinkedListElement* ref;
        while((ref = GetFirst()) != easy_null) {
            ((Reference<TO, FROM>*) ref)->Invalidate();
            ref->Delink();                              // the delink might be already done by invalidate(), but doing it here again does not hurt and insures an empty list
        }
    }
};
}

#endif // easy_ref_manager_h__