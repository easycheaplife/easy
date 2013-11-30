/********************************************************************
	created:	2011/11/19
	created:	19:11:2011   19:29
	filename: 	F:\lee-private-project\trunk\easy\src\base\easy_byte_convertrt.h
	file path:	F:\lee-private-project\trunk\easy\src\base
	file base:	easy_byte_convertrt
	file ext:	h
	author:		Lee
	
	purpose:	convert all kinds of base data type to byte 
*********************************************************************/
#ifndef easy_byte_convertrt_h__
#define easy_byte_convertrt_h__
#include <algorithm>
#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

namespace easy
{
	template<size_t T>
	inline void convert(easy_char* val)
	{
		std::swap(*val,*(val + T + 1));
		convert<T - 2>(val + 1);
	}

	template<> inline void convert<0>(char*) {}
	template<> inline void convert<1>(char*) {}

	template<typename T>
	inline void apply(T* val)
	{
		convert<sizeof(T)>((easy_char*)(val));
	}
}
#endif // easy_byte_convertrt_h__