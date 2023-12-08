/********************************************************************
	created:	2011/10/18
	created:	18:10:2011   16:40
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_constructor.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_constructor
	file ext:	h
	author:		Lee

	purpose:	global construct and destruct function
*********************************************************************/
#ifndef easy_constructor_h__
#define easy_constructor_h__
#ifdef __WINDOWS
#include <new.h>
#elif defined __LINUX
#include <new>
#endif

namespace easy {
// construct and destroy.
// Internal names
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __v) {
#if 1
    void  *_Vptr = __p;
    ::new (_Vptr) _T1(__v);
#else
    new ((void*) __p) _T1(__v);
#endif


}

template <class _T1>
inline void _Construct(_T1* __p) {
    new ((void*) __p) _T1();
}

template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
    __pointer->~_Tp();
}

template <class _T1, class _T2>
inline void construct(_T1* __p, const _T2& __v) {
    _Construct(__p, __v);
}

template <class _T1>
inline void construct(_T1* __p) {
    _Construct(__p);
}

template <class _Tp>
inline void destroy(_Tp* __pointer) {
    _Destroy(__pointer);
}
}

#endif // easy_constructor_h__

