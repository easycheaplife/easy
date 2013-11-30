/********************************************************************
	created:	2011/11/09
	created:	9:11:2011   18:07
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_uninitialized.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_uninitialized
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_uninitialized_h__
#define easy_uninitialized_h__

#ifndef easy_constructor_h__
#include "easy_constructor.h"
#endif //easy_constructor_h__

#ifndef easy_type_traits_h__
#include "easy_test_type_traits.h"
#endif //easy_type_traits_h__

namespace easy
{
	template <class _ForwardIter, class _Size, class _Tp> 
	_ForwardIter 
		__uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
		const _Tp& __x, __false_type)
	{
		_ForwardIter __cur = __first;
		__EASY_TRY 
		{
			for ( ; __n > 0; --__n, ++__cur)
			{
				_Construct(&*__cur, __x);
			}
			return __cur;
		}
		__EASY_UNWIND(_Destroy(__first, __cur));
	}

	// Valid if copy construction is equivalent to assignment, and if the
	//  destructor is trivial.
	template <class _ForwardIter, class _Size, class _Tp>
	inline _ForwardIter 
		__uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
		const _Tp& __x, __true_type)
	{
		return fill_n(__first, __n, __x);
	}

	template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
	inline _ForwardIter 
		__uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, _Tp1*)
	{
		typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
		return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
	}

	template <class _ForwardIter, class _Size, class _Tp>
	inline _ForwardIter 
		uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
	{
		return __uninitialized_fill_n(__first, __n, __x, __VALUE_TYPE(__first));
	}

	// uninitialized_copy

	// Valid if copy construction is equivalent to assignment, and if the
	//  destructor is trivial.
	template <class _InputIter, class _ForwardIter>
	inline _ForwardIter 
		__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
		_ForwardIter __result,
		__true_type)
	{
		return copy(__first, __last, __result);
	}

	template <class _InputIter, class _ForwardIter>
	_ForwardIter 
		__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
		_ForwardIter __result,
		__false_type)
	{
		_ForwardIter __cur = __result;
		__STL_TRY {
			for ( ; __first != __last; ++__first, ++__cur)
				_Construct(&*__cur, *__first);
			return __cur;
		}
		__STL_UNWIND(_Destroy(__result, __cur));
	}


	template <class _InputIter, class _ForwardIter, class _Tp>
	inline _ForwardIter
		__uninitialized_copy(_InputIter __first, _InputIter __last,
		_ForwardIter __result, _Tp*)
	{
		typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
		return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
	}

	template <class _InputIter, class _ForwardIter>
	inline _ForwardIter
		uninitialized_copy(_InputIter __first, _InputIter __last,
		_ForwardIter __result)
	{
		return __uninitialized_copy(__first, __last, __result,
			__VALUE_TYPE(__result));
	}

	inline char* uninitialized_copy(const char* __first, const char* __last,
		char* __result) {
			memmove(__result, __first, __last - __first);
			return __result + (__last - __first);
	}

	inline wchar_t* 
		uninitialized_copy(const wchar_t* __first, const wchar_t* __last,
		wchar_t* __result)
	{
		memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
		return __result + (__last - __first);
	}
}

#endif // easy_uninitialized_h__