/********************************************************************
	created:	2011/10/21
	created:	21:10:2011   17:41
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_util.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_util
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_util_h__
#define easy_util_h__
#include <vector>
#include <string>

#ifdef __WINDOWS
#include <windows.h>
#elif defined __LINUX
#include <unistd.h>
#endif //__WINDOWS

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

namespace easy
{
	struct Tokens: public std::vector<char*>
	{
		Tokens(const std::string &src, const char sep, easy_uint32 vector_reserve = 0);
		~Tokens() { delete[] m_str; }

		char* m_str;
	};

	class Util
	{
	public:
		static void sleep(easy_uint32 __mseconds)
		{
#ifdef __WINDOWS
			::Sleep(__mseconds);
#elif defined __LINUX
			usleep(__mseconds);
#endif //__WINDOWS
		}
	};

}
#endif // easy_util_h__
