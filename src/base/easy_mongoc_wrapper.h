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

//	provide simple mongoc interface for use

#ifndef easy_mongoc_wrapper_h__
#define easy_mongoc_wrapper_h__
#include <mongoc.h>
#include <map>
#include <string>

namespace easy
{
	class MongocWrapper{
	public:

		MongocWrapper(const char* __db = "test",const char* __uri_string = "mongodb://192.168.22.61/");

		~MongocWrapper();

		mongoc_collection_t*  get_collection(const char* __collection_name);

		bool collection_insert(const char* __collection_name,bson_t* __doc);

		mongoc_cursor_t* collection_find(const char* __collection_name,bson_t* __query);

		static MongocWrapper* instance ();

		static void destrory();

	private:
		typedef std::map<std::string,mongoc_collection_t*>	collections;
		collections				collections_;

		std::string				db_;

		mongoc_client_t*		client_;

		static MongocWrapper*	inst_;
	};
}
#endif // easy_mongoc_wrapper_h__
