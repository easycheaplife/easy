/********************************************************************
	created:	2011/09/25
	created:	25:9:2011   10:43
	filename: 	F:\easy\src\base\easy_base_type.h
	file path:	F:\easy\src\base
	file base:	easy_base_type
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_base_type_h__
#define easy_base_type_h__

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

#endif // easy_base_type_h__