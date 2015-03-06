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
#ifndef easy_util_h__
#define easy_util_h__
#include <vector>
#include <string>

#if defined __WINDOWS || defined WIN32
#include <windows.h>
#include <time.h>
#elif defined __LINUX
#include <unistd.h>
#include <stdlib.h>
#endif //__WINDOWS

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

namespace easy
{
	struct Tokens: public std::vector<easy_char*>
	{
		Tokens(const std::string &src, const easy_char sep, easy_uint32 vector_reserve = 0);
		~Tokens() { delete[] m_str; }

		easy_char* m_str;
	};

	class Util
	{
	public:
		static void sleep(easy_uint32 __microseconds)
		{
#ifdef __WINDOWS
			//	unit is mill second
			::Sleep(__microseconds/1000);
#elif defined __LINUX
			usleep(__microseconds);
#endif //__WINDOWS
		}

		static void seed()
		{
			srand((unsigned)time(NULL)); 
		}

		static easy_int64 random(easy_int64 __max)
		{
			return rand()%__max;
		}
	};

}
#endif // easy_util_h__
