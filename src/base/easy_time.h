/********************************************************************
	created:	2011/11/18
	created:	18:11:2011   13:53
	file base:	easy_time
	file ext:	h
	author:		lee
	
	purpose:	some things about time operator
*********************************************************************/
#ifndef easy_time_h__
#define easy_time_h__
#include <time.h>

#if 0
#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__
#else
#include "easy_assist.h"
#endif

namespace easy
{
	class EasyTime
	{
	public:
		static easy_ulong32 time_to_number()
		{
			time_t	cur_time;
			time(&cur_time);
			tm t = *localtime(&cur_time);
			easy_ulong32 ret_time = 0;
			ret_time += t.tm_year + 1900;
			ret_time -= 2000;
			ret_time *= 100;

			ret_time += t.tm_mon+1;
			ret_time *= 100;

			ret_time += t.tm_mday ;
			ret_time *= 100;

			ret_time += t.tm_hour;
			ret_time *= 100;

			ret_time += t.tm_min;
			return ret_time;
		}

		static easy_uint64 get_cur_sys_time()
		{
#ifdef __WINDOWS
			SYSTEMTIME  __sys;
			GetLocalTime(&__sys);
			tm      __time;
			__time.tm_year = __sys.wYear - 1900;
			__time.tm_mon  = __sys.wMonth-1;
			__time.tm_mday = __sys.wDay;
			__time.tm_hour = __sys.wHour;
			__time.tm_min  = __sys.wMinute;
			__time.tm_sec  = __sys.wSecond;
			time_t __cur_time = _mktime64(&__time);
			return (easy_uint64)(__cur_time * 1000 + __sys.wMilliseconds);
#elif __LINUX
			return 0;
#endif // __WINDOWS
		}
	};
}

#endif // easy_time_h__