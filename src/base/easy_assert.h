/********************************************************************
	created:	2011/11/19
	created:	19:11:2011   22:02
	filename: 	F:\lee-private-project\trunk\easy\src\base\easy_assert.h
	file path:	F:\lee-private-project\trunk\easy\src\base
	file base:	easy_assert
	file ext:	h
	author:		Lee

	purpose:	Defines the easy_assert(exp) macro.
*********************************************************************/
#ifndef easy_assert_h__
#define easy_assert_h__

#ifndef easy_log_h__
#include "easy_log.h"
#endif //easy_log_h__

#define LOG_FILE_ASSERT "..\\log\\assert.log"

//	preprocessor string helpers
#ifndef _EASY_CRT_STRINGIZE
#define __EASY_CRT_STRINGIZE(_Value) #_Value
#define _EASY_CRT_STRINGIZE(_Value) __CRT_STRINGIZE(_Value)
#endif

#ifndef _EASY_CRT_WIDE
#ifdef __WINDOWS
#define __EASY_CRT_WIDE(_String) L ## _String
#elif
#define __EASY_CRT_WIDE(_String) _String
#endif
#define _EASY_CRT_WIDE(_String) __EASY_CRT_WIDE(_String)
#endif

#ifndef _EASY_CRT_APPEND
#define __EASY_CRT_APPEND(_Value1, _Value2) _Value1 ## _Value2
#define _EASY_CRT_APPEND(_Value1, _Value2) __EASY_CRT_APPEND(_Value1, _Value2)
#endif

void  _easy_wassert(const char* message, const char* file, unsigned line) {
    easy::EasyLog::SaveLog(LOG_FILE_ASSERT,easy::kErrors,"assert failed at %s ,file %s ,line %d",message,file,line);
}

#ifdef __UNICODE
#define easy_assert(_Expression) (void)( (!!(_Expression)) || (_easy_wassert(_EASY_CRT_WIDE(#_Expression), _EASY_CRT_WIDE(__FILE__), __LINE__), 0) )
#else
#define easy_assert(_Expression) (void)( (!!(_Expression)) || (_easy_wassert(_Expression, (__FILE__), __LINE__), 0) )
#endif


#endif // easy_assert_h__