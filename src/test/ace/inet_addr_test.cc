
#include "easy_cppunit_proxy.h"
#include "ace/INET_Addr.h"
//
// TestCase class
//
class test_inet_addr : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(test_inet_addr);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_inet_addr);

//
// tests implementation
//

void test_inet_addr::test()
{
	WSADATA wsaData = {0};
	int iResult = 0;
	// Initialize Winsock
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		wprintf(L"WSAStartup failed: %d\n", iResult);
	}

	const char __host_name[] = "127.0.0.1";
	const char __port_name[] = "5150";
	unsigned short __port = 5150;
	ACE_INET_Addr __inet_addr1();

	sockaddr_in ___sock_addr_in;
	___sock_addr_in.sin_family = AF_INET;
	___sock_addr_in.sin_port = htons(5150) ;
	___sock_addr_in.sin_addr.s_addr = inet_addr(__host_name); 
	ACE_INET_Addr __inet_addr2(&___sock_addr_in,sizeof(___sock_addr_in));

	ACE_INET_Addr _inet_addr3(__port,__host_name,AF_INET);

	ACE_INET_Addr __inet_addr4(__host_name,AF_INET);

	ACE_INET_Addr __inet_addr5(__port);

	ACE_INET_Addr __inet_addr6(__port_name,__host_name);
}
