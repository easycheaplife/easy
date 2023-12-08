/********************************************************************
	created:	2011/10/13
	created:	13:10:2011   15:33
	file base:	easy_linked_list
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_linked_list_h__
#define easy_linked_list_h__
#include <xutility>

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif // easy_base_type_h__

namespace easy {
//	class forward declare
class LinkedListHead;
class LinkedListElement {
  private:
    friend class LinkedListHead;

    LinkedListElement* next_;
    LinkedListElement* prev_;
  public:
    LinkedListElement() {
        prev_ = easy_null;
        next_ = easy_null;
    }
    ~LinkedListElement() {
        Delink();
    }

    easy_bool HasNext() const {
        return (next_&& next_->next_ != easy_null);
    }
    easy_bool HasPrev() const {
        return (prev_ && prev_->prev_ != easy_null);
    }
    easy_bool IsInList() const {
        prev_ != easy_null && next_ != easy_null;
    }

    LinkedListElement* next( ) {
        return HasNext() ? next_ : easy_null;
    }
    LinkedListElement* const next( ) const {
        return HasNext() ? next_ : easy_null;
    }
    LinkedListElement* prev( ) {
        return HasPrev() ? prev_ : easy_null;
    }
    LinkedListElement* const prev( ) const {
        return HasPrev() ? prev_ : easy_null;
    }

    LinkedListElement* nocheck_next( ) {
        return next_;
    }
    LinkedListElement* const nocheck_next( ) const {
        return next_;
    }
    LinkedListElement* nocheck_prev( ) {
        return prev_;
    }
    LinkedListElement* const nocheck_prev( ) const {
        return prev_;
    }

    void Delink() {
        if ( IsInList() ) {
            next_->prev_ = prev_;
            prev_->next_ = next_;
            next_ = easy_null;
            prev_ = easy_null;
        }
    }

    void InsertBefore(LinkedListElement* elem) {
        elem->next_ = this;
        elem->prev_ = prev_;
        prev_->next_ = elem;
        prev_ = elem;
    }

    void InsertAfter(LinkedListElement* elem) {
        elem->prev_ = this;
        elem->next_ = next_;
        next_->prev_ = elem;
        next_ = elem;
    }
};

class LinkedListHead {
  private:
    LinkedListElement first_;
    LinkedListElement last_;
    easy_uint32 size_;
  public:
    LinkedListHead() : size_(0) {
        first_.next_ = &last_;
        last_.prev_ = &first_;
    }

    easy_bool IsEmpty() const {
        return (!first_.next_->IsInList());
    }

    LinkedListElement* GetFirst() {
        return (IsEmpty() ? easy_null : first_.next_);
    }
    LinkedListElement* const GetFirst() const {
        return (IsEmpty() ? easy_null : first_.next_);
    }

    LinkedListElement* GetLast() {
        return (IsEmpty() ? easy_null : last_.prev_);
    }
    LinkedListElement* const GetLast() const {
        return (IsEmpty() ? easy_null : last_.prev_);
    }

    void InsertFirst(LinkedListElement* elem) {
        elem->InsertBefore(elem);
    }

    void InsertLast(LinkedListElement* elem) {
        elem->InsertAfter(elem);
    }

    easy_uint32 GetSize() const {
        if (!size_) {
            easy_uint32 result = 0;
            LinkedListElement const* elem = GetFirst();
            while(elem) {
                ++result;
                elem = elem->next();
            }
        } else {
            return size_;
        }
    }

    void IncSize() {
        ++size_;
    }
    void DecSize() {
        --size_;
    }

    template< typename _Ty >
    class Iterator {
      public:
        typedef std::bidirectional_iterator_tag     iterator_category;
        typedef _Ty                                 value_type;
        typedef ptrdiff_t                           difference_type;
        typedef ptrdiff_t                           distance_type;
        typedef _Ty*                                pointer;
        typedef _Ty const*                          const_pointer;
        typedef _Ty&                                reference;
        typedef _Ty const &                         const_reference;

        Iterator() : ptr_(0) {					 // construct with null node pointer

        }

        Iterator(pointer pointer_node) : ptr_(pointer_node) {
            // construct with node pointer _Pnode
        }

        Iterator& operator=(Iterator const &_right) {
            return (*this) = _right.ptr_;
        }

        Iterator& operator=(const_pointer const &_right) {
            ptr_ = (pointer)_right;
            return (*this);
        }


        reference operator*() {
            // return designated value
            return *ptr_;
        }

        pointer operator->() {
            // return pointer to class object
            return ptr_;
        }

        Iterator& operator++() {
            // pre increment
            ptr_ = ptr_->next();
            return (*this);
        }

        Iterator operator++(int) {
            // post increment
            iterator _tmp = *this;
            ++*this;
            return (_tmp);
        }

        Iterator& operator--() {
            // pre decrement
            ptr_ = ptr_->prev();
            return (*this);
        }

        Iterator operator--(int) {
            // post decrement
            iterator _tmp = *this;
            --*this;
            return (_tmp);
        }

        bool operator==(Iterator const &_right) const {
            // test for iterator equality
            return (ptr_ == _right.ptr_);
        }

        bool operator!=(Iterator const &_right) const {
            // test for iterator inequality
            return (!(*this == _right));
        }

        bool operator==(pointer const &_right) const {
            // test for pointer equality
            return (ptr_ != _right);
        }

        bool operator!=(pointer const &_right) const {
            // test for pointer equality
            return (!(*this == _right));
        }

        bool operator==(const_reference _right) const {
            // test for reference equality
            return (ptr_ == &_right);
        }

        bool operator!=(const_reference _right) const {
            // test for reference equality
            return (ptr_ != &_right);
        }

        pointer MyNode() {
            // return node pointer
            return (ptr_);
        }
      protected:
        pointer ptr_;                               // pointer to node
    };

    typedef Iterator<LinkedListElement> iterator;
};
}
#endif // easy_linked_list_h__
