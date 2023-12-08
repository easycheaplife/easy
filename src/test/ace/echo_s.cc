#include "easy_cppunit_proxy.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"
#include "ace/os_include/os_netdb.h"
#include "ace/os_include/sys/os_types.h"
//
// TestCase class
//
class echo_s : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(echo_s);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(echo_s);

//
// tests implementation
//
#pragma comment(lib,"Ws2_32.lib")

int start_server() {
    WSADATA wsaData = {0};
    int iResult = 0;
    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        wprintf(L"WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ACE_INET_Addr __port_to_listen(5000,ACE_LOCALHOST);
    ACE_SOCK_Acceptor __acceptor;
    if (-1 == __acceptor.open(__port_to_listen,1)) {
        ACE_ERROR_RETURN((LM_ERROR,ACE_TEXT("%p\n"),ACE_TEXT("acceptor.open")),100);
    }
    while (1) {
        ACE_SOCK_STREAM __peer;
        ACE_INET_Addr __peer_addr;
        ACE_Time_Value __timeout(10,0);
        if (-1 == __acceptor.accept(__peer)) {
            ACE_ERROR_RETURN((LM_ERROR,ACE_TEXT("(%p|%t) failed to accept\n"),ACE_TEXT("client connection")),100);
        } else {
            ACE_TCHAR __peer_name[MAXHOSTNAMELEN];
            __peer_addr.addr_to_string(__peer_name,MAXHOSTNAMELEN);
            printf("connection from %s\n",__peer_name);
            char __buffer[64];
            ssize_t __byte_received ;
            while ((__byte_received = __peer.recv(__buffer,sizeof(__buffer)) != -1)) {
                printf("%d bytes received %s\n",__byte_received,__buffer);
                __peer.send_n(__buffer,strlen(__buffer));
                printf("%d bytes send %s\n",strlen(__buffer),__buffer);
            }
            __peer.close();
        }
    }
    return 0;
}
void echo_s::test() {
    start_server();
}
