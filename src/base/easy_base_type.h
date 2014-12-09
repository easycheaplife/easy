/****************************************************************************
 Copyright (c) 2013-2014 King Lee

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
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