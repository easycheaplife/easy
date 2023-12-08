/********************************************************************
	created:	2011/12/18
	created:	18:12:2011   9:41
	filename: 	F:\lee-private-project\trunk\easy\src\base\easy_algorithm.h
	file path:	F:\lee-private-project\trunk\easy\src\base
	file base:	easy_algorithm
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_algorithm_h__
#define easy_algorithm_h__

#ifndef easy_macro_h__
#include "easy_macro.h"
#endif //easy_macro_h__

namespace easy {
template<typename _Tp>
inline void swap(_Tp& __a,_Tp& __b) {
    __EASY_REQUIRES(_Tp,Assignable);
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

//	usually it reference at stl_algobase.h
template<typename _InputIter1,typename _InputIter2>
bool lexicographical_compare(_InputIter1 __first1,_InputIter1 __last1,_InputIter1 __first2,_InputIter1 __last2) {
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
}
#endif // easy_algorithm_h__