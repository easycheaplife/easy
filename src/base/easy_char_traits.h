/********************************************************************
	created:	2011/12/29
	created:	29:12:2011   10:06
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_char_traits.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_char_traits
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_char_traits_h__
#define easy_char_traits_h__
#include <string.h>
#include <wchar.h>
namespace easy {
template<typename _CharT,typename _IntT>
class __char_traits_base {
  public:
    typedef _CharT	char_type;
    typedef _IntT	int_type;
    static void assign(char_type& __c1, char_type& __c2) {
        __c1 = __c2;
    }

    static bool eq(const _CharT& __c1,const _CharT& __c2) {
        return __c1 == __c2;
    }

    static bool lt(const _CharT& __cl,const _CharT& __c2) {
        return __cl < __c2;
    }

    //	return value:
    static int compare(const _CharT* __s1,const _CharT* __s2,size_t __n) {
        for (size_t i = 0; i < __n; ++i) {
            if (!eq(__s1[i],__s2[i])) {
                return lt(__s1[i],__s2[i]) ? -1 : 1;
            }
        }
        return 0;
    }

    //	the length not include '\0'
    static size_t length(const _CharT* __s) {
        const _CharT __nullchar = _CharT();
        size_t __i = 0;
        for (__i = 0; !eq(__s[__i],__nullchar); ++__i) { }
        return __i;
    }

    static const _CharT* find(const _CharT* __s,size_t __n, const _CharT& __c) {
        for (; __n > 0; ++__s,--__n) {
            if (eq(*__s,__c)) {
                return __s;
            }
        }
        return 0;
    }

    static _CharT* move(_CharT* __s1,const _CharT* __s2, size_t __n) {
        memmove(__s1,__s2,__n*sizeof(_CharT));
        return __s1;
    }

    static _CharT* copy(_CharT* __s1,const _CharT* __s2, size_t __n) {
        memcpy(__s1,__s2,__n*sizeof(_CharT));
        return __s1;
    }

    static _CharT* assign(_CharT* __s,size_t __n,_CharT __c) {
        for(size_t __i = 0; __i < __n; ++__i) {
            __s[__i] = __c;
        }
        return __s;
    }

    static int_type not_eof(const int_type& __c) {
        return !eq_int_type(__c,eof()) ? __c : 0;
    }

    static char_type to_char_type(const int_type& __c) {
        return static_cast<char_type>(__c);
    }

    static int_type to_int_type(const char_type& __c) {
        return static_cast<int_type>(__c);
    }

    static bool eq_int_type(const int_type& __c1, const int_type& __c2) {
        return __c1 == __c2;
    }

    static int_type eof() {
        return static_cast<int_type>(-1);
    }
};

// Generic char_traits class.  Note that this class is provided only
//  as a base for explicit specialization; it is unlikely to be useful
//  as is for any particular user-defined type.  In particular, it
//  *will not work* for a non-POD type.

template<typename _CharT>
class char_traits : public __char_traits_base<_CharT,_CharT> { };

//	specialization for char
template<>
class char_traits<char> : public __char_traits_base<char,int> {
  public:
    static char_type to_char_type(const int_type& __c) {
        return static_cast<char_type>(static_cast<unsigned char>(__c));
    }

    static int_type to_int_type(const char_type& __c) {
        return static_cast<unsigned char>(__c);
    }

    static int compare(const char* __s1, const char* __s2, size_t __n) {
        return memcmp(__s1, __s2, __n);
    }

    static size_t length(const char* __s) {
        return strlen(__s);
    }

    static void assign(char& __c1, const char& __c2) {
        __c1 = __c2;
    }

    static char* assign(char* __s, size_t __n, char __c) {
        memset(__s, __c, __n);
        return __s;
    }
};

template<>	class char_traits<wchar_t> : public __char_traits_base<wchar_t,int> { };
}
#endif // easy_char_traits_h__