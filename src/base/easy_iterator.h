/********************************************************************
	created:	2011/12/17
	created:	17:12:2011   22:25
	filename: 	F:\lee-private-project\trunk\easy\src\base\easy_iterator.h
	file path:	F:\lee-private-project\trunk\easy\src\base
	file base:	easy_iterator
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_iterator_h__
#define easy_iterator_h__

namespace easy {
// This is the new version of reverse_iterator, as defined in the
//  draft C++ standard.  It relies on the iterator_traits template,
//  which in turn relies on partial specialization.  The class
//  reverse_bidirectional_iterator is no longer part of the draft
//  standard, but it is retained for backward compatibility.
template<typename _Iterator>
class reverse_iterator {
  protected:
    _Iterator current;
  public:
    typedef typename iterator_traits<_Iterator>::iterator_category	iterator_category;
    typedef typename iterator_traits<_Iterator>::value_type			value_type;
    typedef typename iterator_traits<_Iterator>::difference_type	difference_type;
    typedef typename iterator_traits<_Iterator>::pointer			pointer;
    typedef typename iterator_traits<_Iterator>::reference			reference;

    typedef _Iterator iterator_type;
    typedef reverse_iterator<_Iterator> _Self;

  public:
    reverse_iterator() { }
    explicit reverse_iterator(iterator_type __x) : current(__x) { }

    reverse_iterator(const _Self& __x) : current(__x.current) { }

    template <class _Iter>
    reverse_iterator(const reverse_iterator<_Iter>& __x)
        : current(__x.base()) {}

    iterator_type base() const {
        return current;
    }

    reference operator*() const {
        _Iterator __tmp = current;
        return *--__tmp;
    }

    pointer operator->() const {
        return &(operator*());
    }

    _Self& operator++() {
        --current;
        return *this;
    }

    _Self operator++(int) {
        _Self __tmp = *this;
        --current;
        return __tmp;
    }

    _Self& operator--() {
        ++current;
        return *this;
    }

    _Self operator--(int) {
        _Self __tmp = *this;
        ++current;
        return __tmp;
    }

    _Self operator+(difference_type __n) const {
        return _Self(current - __n);
    }

    _Self& operator+= (difference_type __n) {
        current -= __n;
        return *this;
    }

    _Self operator-(difference_type __n) const {
        return _Self(current + __n);
    }

    _Self& operator-= (difference_type __n) {
        current += __n;
        return *this;
    }

    reference operator[] (difference_type __n) const {
        return *(*this + __n);
    }
};

template<typename _Iterator>
inline bool operator==(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return __x.base() == __y.base();
}

template<typename _Iterator>
inline bool operator<(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return __x.base() < __y.base();
}

template<typename _Iterator>
inline bool operator!=(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return __x != __y;
}

template<typename _Iterator>
inline bool operator>(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return __y < __x;
}

template<typename _Iterator>
inline bool operator<=(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return !(__y < __x);
}

template<typename _Iterator>
inline bool operator>=(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return !(__x < __y);
}

template<typename _Iterator>
inline typename reverse_iterator<_Iterator>::difference_type
operator-(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return __y.base() - __x.base();
}

template<typename _Iterator>
inline reverse_iterator<_Iterator>
operator+(typename reverse_iterator<_Iterator>::difference_type __n,const reverse_iterator<_Iterator>& __x) {
    return reverse_iterator<_Iterator>(__x.base() - __n);
}
}
#endif // easy_iterator_h__