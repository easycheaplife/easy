/********************************************************************
	created:	2012/01/06
	created:	6:1:2012   14:02
	file base:	easy_assist
	file ext:	h
	author:		Lee
	
	purpose:	some help function and template class to support stl
*********************************************************************/
#ifndef easy_assist_h__
#define easy_assist_h__
#include <new>
#include <cstddef>
#define __EASY_REQUIRES(__type_var, __concept) do {} while(0)

#ifndef easy_null
#ifdef __cplusplus
#define easy_null    0
#else
#define easy_null    ((void *)0)
#endif
#endif

typedef bool					easy_bool;
typedef char					easy_my_bool;
typedef char					easy_char;
typedef signed char				easy_schar;
typedef wchar_t					easy_wchar;
typedef char					easy_int8;
typedef unsigned char			easy_uint8;
typedef short					easy_int16;
typedef unsigned short			easy_uint16;
typedef int						easy_int32;
typedef unsigned int			easy_uint32;
typedef long					easy_long;
typedef unsigned long			easy_ulong;
typedef float					easy_float;
typedef double					easy_double;
typedef float					easy_float32;
typedef double					easy_float64;
typedef unsigned int			easy_ulong32;
typedef long long				easy_int64;
typedef unsigned long long		easy_uint64;

namespace easy
{
	// This file contains all of the general iterator-related utilities.
	// The internal file easy_iterator.h contains predefined iterators, 
	// such as front_insert_iterator and istream_iterator.

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};	
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	// The base classes input_iterator, output_iterator, forward_iterator,
	// bidirectional_iterator, and random_access_iterator are not part of
	// the C++ standard.  (They have been replaced by struct iterator.)
	template <class _Tp, class _Distance> 
	struct input_iterator 
	{
		typedef input_iterator_tag iterator_category;
		typedef _Tp                value_type;
		typedef _Distance          difference_type;
		typedef _Tp*               pointer;
		typedef _Tp&               reference;
	};

	struct output_iterator 
	{
		typedef output_iterator_tag iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;
	};

	template <class _Tp, class _Distance> 
	struct forward_iterator 
	{
		typedef forward_iterator_tag iterator_category;
		typedef _Tp                  value_type;
		typedef _Distance            difference_type;
		typedef _Tp*                 pointer;
		typedef _Tp&                 reference;
	};

	template <class _Tp, class _Distance> 
	struct bidirectional_iterator 
	{
		typedef bidirectional_iterator_tag iterator_category;
		typedef _Tp                        value_type;
		typedef _Distance                  difference_type;
		typedef _Tp*                       pointer;
		typedef _Tp&                       reference;
	};

	template <class _Tp, class _Distance> 
	struct random_access_iterator 
	{
		typedef random_access_iterator_tag iterator_category;
		typedef _Tp                        value_type;
		typedef _Distance                  difference_type;
		typedef _Tp*                       pointer;
		typedef _Tp&                       reference;
	};

	template <class _Category, class _Tp, class _Distance = ptrdiff_t,
	class _Pointer = _Tp*, class _Reference = _Tp&>
	struct iterator 
	{
		typedef _Category  iterator_category;
		typedef _Tp        value_type;
		typedef _Distance  difference_type;
		typedef _Pointer   pointer;
		typedef _Reference reference;
	};

	//	partial specialization
	template <class _Iterator>
	struct iterator_traits 
	{
		typedef typename _Iterator::iterator_category iterator_category;
		typedef typename _Iterator::value_type        value_type;
		typedef typename _Iterator::difference_type   difference_type;
		typedef typename _Iterator::pointer           pointer;
		typedef typename _Iterator::reference         reference;
	};

	template <class _Tp>
	struct iterator_traits<_Tp*> 
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef _Tp                         value_type;
		typedef ptrdiff_t                   difference_type;
		typedef _Tp*                        pointer;
		typedef _Tp&                        reference;
	};

	template <class _Tp>
	struct iterator_traits<const _Tp*> 
	{
		typedef random_access_iterator_tag iterator_category;
		typedef _Tp                         value_type;
		typedef ptrdiff_t                   difference_type;
		typedef const _Tp*                  pointer;
		typedef const _Tp&                  reference;
	};

	// The overloaded functions iterator_category, distance_type, and
	// value_type are not part of the C++ standard.  (They have been
	// replaced by struct iterator_traits.)  They are included for
	// backward compatibility with the HP STL.

	// We introduce internal names for these functions.

	template <class _Iter>
	inline typename iterator_traits<_Iter>::iterator_category
		__iterator_category(const _Iter&)
	{
		typedef typename iterator_traits<_Iter>::iterator_category _Category;
		return _Category();
	}

	template <class _Iter>
	inline typename iterator_traits<_Iter>::difference_type*
		__distance_type(const _Iter&)
	{
		return static_cast<typename iterator_traits<_Iter>::difference_type*>(0);
	}

	template <class _Iter>
	inline typename iterator_traits<_Iter>::value_type*
		__value_type(const _Iter&)
	{
		return static_cast<typename iterator_traits<_Iter>::value_type*>(0);
	}

	template <class _Iter>
	inline typename iterator_traits<_Iter>::iterator_category
		iterator_category(const _Iter& __i) { return __iterator_category(__i); }


	template <class _Iter>
	inline typename iterator_traits<_Iter>::difference_type*
		distance_type(const _Iter& __i) { return __distance_type(__i); }

	template <class _Iter>
	inline typename iterator_traits<_Iter>::value_type*
		value_type(const _Iter& __i) { return __value_type(__i); }

#define __ITERATOR_CATEGORY(__i) __iterator_category(__i)
#define __DISTANCE_TYPE(__i)     __distance_type(__i)
#define __VALUE_TYPE(__i)        __value_type(__i)

	template <class _InputIterator, class _Distance>
	inline void __distance(_InputIterator __first, _InputIterator __last,
		_Distance& __n, input_iterator_tag)
	{
		while (__first != __last) { ++__first; ++__n; }
	}

	template <class _RandomAccessIterator, class _Distance>
	inline void __distance(_RandomAccessIterator __first, 
		_RandomAccessIterator __last, 
		_Distance& __n, random_access_iterator_tag)
	{
		__EASY_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
		__n += __last - __first;
	}

	template <class _InputIterator, class _Distance>
	inline void distance(_InputIterator __first, 
		_InputIterator __last, _Distance& __n)
	{
		__EASY_REQUIRES(_InputIterator, _InputIterator);
		__distance(__first, __last, __n, iterator_category(__first));
	}

	template <class _InputIterator>
	inline typename iterator_traits<_InputIterator>::difference_type
		__distance(_InputIterator __first, _InputIterator __last, input_iterator_tag)
	{
		typename iterator_traits<_InputIterator>::difference_type __n = 0;
		while (__first != __last) 
		{
			++__first; ++__n;
		}
		return __n;
	}

	template <class _RandomAccessIterator>
	inline typename iterator_traits<_RandomAccessIterator>::difference_type
		__distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
		random_access_iterator_tag) 
	{
		__EASY_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
		return __last - __first;
	}

	template <class _InputIterator>
	inline typename iterator_traits<_InputIterator>::difference_type
		distance(_InputIterator __first, _InputIterator __last) 
	{
		typedef typename iterator_traits<_InputIterator>::iterator_category 
			_Category;
		__EASY_REQUIRES(_InputIterator, _InputIterator);
		return __distance(__first, __last, _Category());
	}

	template <class _InputIter, class _Distance>
	inline void __advance(_InputIter& __i, _Distance __n, input_iterator_tag) 
	{
		while (__n--) ++__i;
	}

	template <class _BidirectionalIterator, class _Distance>
	inline void __advance(_BidirectionalIterator& __i, _Distance __n, 
		bidirectional_iterator_tag) 
	{
		__EASY_REQUIRES(_BidirectionalIterator, _BidirectionalIterator);
		if (__n >= 0)
			while (__n--) ++__i;
		else
			while (__n++) --__i;
	}

	template <class _RandomAccessIterator, class _Distance>
	inline void __advance(_RandomAccessIterator& __i, _Distance __n, 
		random_access_iterator_tag) 
	{
		__EASY_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
		__i += __n;
	}

	template <class _InputIterator, class _Distance>
	inline void advance(_InputIterator& __i, _Distance __n) 
	{
		__EASY_REQUIRES(_InputIterator, _InputIterator);
		__advance(__i, __n, iterator_category(__i));
	}

	//////////////////////////////////////////////////////////////////////////
	// construct and destroy.  
	// Internal names
	template <class _T1, class _T2>
	inline void _Construct(_T1* __p, const _T2& __v) 
	{
#if 1
		void  *_Vptr = __p;
		::new (_Vptr) _T1(__v);
#else
		new ((void*) __p) _T1(__v);
#endif


	}

	template <class _T1>
	inline void _Construct(_T1* __p) 
	{
		new ((void*) __p) _T1();
	}

	template <class _Tp>
	inline void _Destroy(_Tp* __pointer) 
	{
		__pointer->~_Tp();
	}

	template <class _T1, class _T2>
	inline void construct(_T1* __p, const _T2& __v) 
	{
		_Construct(__p, __v);
	}

	template <class _T1>
	inline void construct(_T1* __p) 
	{
		_Construct(__p);
	}

	template <class _Tp>
	inline void destroy(_Tp* __pointer) 
	{
		_Destroy(__pointer);
	}

	//////////////////////////////////////////////////////////////////////////
	// This is the new version of reverse_iterator, as defined in the
	//  draft C++ standard.  It relies on the iterator_traits template,
	//  which in turn relies on partial specialization.  The class
	//  reverse_bidirectional_iterator is no longer part of the draft
	//  standard, but it is retained for backward compatibility.
	template<typename _Iterator>
	class reverse_iterator
	{
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

		iterator_type base() const { return current; }

		reference operator*() const {
			_Iterator __tmp = current;
			return *--__tmp;
		}

		pointer operator->() const { return &(operator*()); }

		_Self& operator++()
		{
			--current;
			return *this;
		}

		_Self operator++(int)
		{
			_Self __tmp = *this;
			--current;
			return __tmp;
		}

		_Self& operator--()
		{
			++current;
			return *this;
		}

		_Self operator--(int)
		{
			_Self __tmp = *this;
			++current;
			return __tmp;
		}

		_Self operator+(difference_type __n) const { return _Self(current - __n); }

		_Self& operator+= (difference_type __n) { current -= __n; return *this; }

		_Self operator-(difference_type __n) const { return _Self(current + __n); }

		_Self& operator-= (difference_type __n) { current += __n; return *this; }

		reference operator[] (difference_type __n) const { return *(*this + __n); } 
	};

	template<typename _Iterator>
	inline bool operator==(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y)
	{
		return __x.base() == __y.base();
	}

	template<typename _Iterator>
	inline bool operator<(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y)
	{
		return __x.base() < __y.base();
	}

	template<typename _Iterator>
	inline bool operator!=(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y)
	{
		return __x != __y;
	}

	template<typename _Iterator>
	inline bool operator>(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y)
	{
		return __y < __x;
	}

	template<typename _Iterator>
	inline bool operator<=(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y)
	{
		return !(__y < __x);
	}

	template<typename _Iterator>
	inline bool operator>=(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y)
	{
		return !(__x < __y);
	}

	template<typename _Iterator>
	inline typename reverse_iterator<_Iterator>::difference_type
		operator-(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y)
	{
		return __y.base() - __x.base();
	}

	template<typename _Iterator>
	inline reverse_iterator<_Iterator>
		operator+(typename reverse_iterator<_Iterator>::difference_type __n,const reverse_iterator<_Iterator>& __x)
	{
		return reverse_iterator<_Iterator>(__x.base() - __n);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename _Tp>
	inline void swap(_Tp& __a,_Tp& __b)
	{
		__EASY_REQUIRES(_Tp,Assignable);
		_Tp __tmp = __a;
		__a = __b;
		__b = __tmp;
	}

	//	usually it reference at stl_algobase.h
	template<typename _InputIter1,typename _InputIter2>
	bool lexicographical_compare(_InputIter1 __first1,_InputIter1 __last1,_InputIter1 __first2,_InputIter1 __last2)
	{
		__EASY_REQUIRES(_InputIter1, _InputIterator);
		__EASY_REQUIRES(_InputIter2, _InputIterator);
		__EASY_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
			_LessThanComparable);
		__EASY_REQUIRES(typename iterator_traits<_InputIter2>::value_type,
			_LessThanComparable);
		for ( ; __first1 != __last1 && __first2 != __last2
			; ++__first1, ++__first2) {
				if (*__first1 < *__first2)
					return true;
				if (*__first2 < *__first1)
					return false;
		}
		return __first1 == __last1 && __first2 != __last2;
	}

	//////////////////////////////////////////////////////////////////////////
		struct __true_type 
	{
	};

	struct __false_type 
	{
	};

	template <class _Tp>
	struct __type_traits { 
		typedef __true_type     this_dummy_member_must_be_first;
		/* Do not remove this member. It informs a compiler which
		automatically specializes __type_traits that this
		__type_traits template is special. It just makes sure that
		things work if an implementation is using a template
		called __type_traits for something unrelated. */

		/* The following restrictions should be observed for the sake of
		compilers which automatically produce type specific specializations 
		of this class:
		- You may reorder the members below if you wish
		- You may remove any of the members below if you wish
		- You must not rename members without making the corresponding
		name change in the compiler
		- Members you add will be treated like regular members unless
		you add the appropriate support in the compiler. */


		typedef __false_type    has_trivial_default_constructor;
		typedef __false_type    has_trivial_copy_constructor;
		typedef __false_type    has_trivial_assignment_operator;
		typedef __false_type    has_trivial_destructor;
		typedef __false_type    is_POD_type;
	};

	// Provide some specializations.  This is harmless for compilers that
	//  have built-in __types_traits support, and essential for compilers
	//  that don't.
	template<> struct __type_traits<bool> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<char> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<signed char> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<unsigned char> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<wchar_t> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<short> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<unsigned short> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<int> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<unsigned int> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<long> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<unsigned long> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<long long> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<unsigned long long> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<float> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<double> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template<> struct __type_traits<long double> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	template <class _Tp>
	struct __type_traits<_Tp*> {
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	// The following could be written in terms of numeric_limits.  
	// We're doing it separately to reduce the number of dependencies.
	template <class _Tp> struct _Is_integer {
		typedef __false_type _Integral;
	};

	template<> 
	struct _Is_integer<bool>
	{
		typedef __true_type _Integral;
	};

	template<> struct _Is_integer<char> 
	{
		typedef __true_type _Integral;
	};

	template<> struct _Is_integer<signed char> 
	{
		typedef __true_type _Integral;
	};

	template<> struct _Is_integer<unsigned char> 
	{
		typedef __true_type _Integral;
	};

	template<> struct _Is_integer<wchar_t> {
		typedef __true_type _Integral;
	};

	template<>  struct _Is_integer<short> {
		typedef __true_type _Integral;
	};

	template<>  struct _Is_integer<unsigned short> {
		typedef __true_type _Integral;
	};

	template<>  struct _Is_integer<int> {
		typedef __true_type _Integral;
	};

	template<>  struct _Is_integer<unsigned int> {
		typedef __true_type _Integral;
	};

	template<>  struct _Is_integer<long> {
		typedef __true_type _Integral;
	};

	template<>  struct _Is_integer<unsigned long> {
		typedef __true_type _Integral;
	};

	template<>  struct _Is_integer<long long> {
		typedef __true_type _Integral;
	};

	template<>  struct _Is_integer<unsigned long long> {
		typedef __true_type _Integral;
	};
}
#endif // easy_assist_h__
