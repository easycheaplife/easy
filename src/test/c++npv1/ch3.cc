
#include "easy_cppunit_proxy.h"
#include "ace/INET_Addr.h"
#include "ace/Sock_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Time_Value.h"
#include "ace/Auto_Ptr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Mem_Map.h"
//
// TestCase class
//
class TestCh3 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestCh3);
#if 0
	CPPUNIT_IGNORE;
#endif
#if 1
	CPPUNIT_TEST(web_server);
#else
	CPPUNIT_TEST(web_client);
#endif
	CPPUNIT_TEST_SUITE_END();

protected:
	void web_client();
	void web_server();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCh3);

//
// tests implementation
//

void TestCh3::web_client()
{
	const char* __path_name = "index.html";
	const char* __host_name = "127.0.0.1";
	ACE_SOCK_CONNECTOR __connector;
	ACE_SOCK_Stream	__peer;
	ACE_INET_Addr __peer_addr;
	if (__peer_addr.set(80,__host_name) == -1)
	{
		return ;
	}
	else if (__connector.connect(__peer,__peer_addr,&ACE_Time_Value::zero) == -1)
	{
		if (errno == EWOULDBLOCK)
		{
			if (__connector.complete(__peer,0,&ACE_Time_Value::zero) == -1)
			{
				//	to do something
			}
		}
		
		return;
	}
}

void TestCh3::web_server()
{
	ACE_INET_Addr __server_addr;
	ACE_SOCK_Acceptor __acceptor;
	ACE_SOCK_Stream	__peer;
	if (__server_addr.set(80) == -1) 
	{
		return;
	}
	if (__acceptor.open(__server_addr) == -1)
	{
		return ;
	}
	for(;;)
	{
		if (__acceptor.accept(__peer) == -1)
		{
			return;
		}
		ACE_Auto_Array_Ptr<char> __path_name("127.0.0.1");
		ACE_Mem_Map __maped_file(__path_name.get());
		if (__peer.send_n(__maped_file.addr(),__maped_file.size()) == -1)
		{
			return;
		}
		__peer.close();
	}
}
