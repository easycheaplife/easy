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
//	provide simple mongocxx interface for use
#ifndef easy_mongocxx_wrapper_h__
#define easy_mongocxx_wrapper_h__
#ifdef WIN32
#include <WinSock2.h>
#endif //WIN32
#include "mongo/client/dbclient.h"

namespace easy
{
	class MongocxxWrapper 
	{
	public:

		MongocxxWrapper(const char* __uri_string);

		~MongocxxWrapper();

		static MongocxxWrapper* instance (const char* __uri_string = "192.168.22.66:27017");

		static void destrory();

		mongo::DBClientConnection& db_client_connection() { return conn_; }

	private:
		mongo::DBClientConnection conn_;

		static MongocxxWrapper*	inst_;
	};
}

#endif // easy_mongocxx_wrapper_h__
