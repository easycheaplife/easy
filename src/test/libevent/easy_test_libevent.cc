#include "easy_cppunit_proxy.h"
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "event.h"

#ifdef __WINDOWS
#include <Windows.h>
/*
libevent.lib
libevent_core.lib
libevent_extras.lib
ws2_32.lib
*/
#if 0
#pragma comment(lib,"libevent")
#pragma comment(lib,"libevent_core")
#pragma comment(lib,"libevent_extras")
#pragma comment(lib,"ws2_32")
#endif

#elif __LINUX
#include <arpa/inet.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

//
// TestCase class
//
//#define ECHO_SERVER
#define SOCKET_VERSION_HIGH				2
#define SOCKET_VERSION_LOW				2
BOOL InitSock()
{
#ifdef __WINDOWS
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(SOCKET_VERSION_HIGH,SOCKET_VERSION_LOW);
	if (0 != WSAStartup( wVersionRequested, &wsaData))
	{
		//Tell the user that we could not find a usable WinSock DLL.
		return FALSE;
	}
	if ( LOBYTE( wsaData.wVersion ) != SOCKET_VERSION_HIGH ||
		HIBYTE( wsaData.wVersion ) != SOCKET_VERSION_LOW ) 
	{
		// Tell the user that we could not find a usable WinSock DLL.                               
		WSACleanup();
		return FALSE; 
	}
#endif
	return TRUE;
}

static void echo_read_cb(struct bufferevent* bev, void* ctx)
{
	//	This callback is invoked when there is data to read on bev. 
	struct evbuffer* input = bufferevent_get_input(bev);
	struct evbuffer* output = bufferevent_get_output(bev);
	//	Copy all the data from the input buffer to the output buffer
	evbuffer_add_buffer(output,input);
	printf("echo_read_cb called\n");
}

static void echo_event_cb(struct bufferevent* bev, short events,void* ctx)
{
	if (events & BEV_EVENT_ERROR)
	{
		perror("Error from bufferevent");
	}
	if (events & (BEV_EVENT_ERROR | BEV_EVENT_EOF))
	{
		bufferevent_free(bev);
	}
	printf("echo_event_cb called\n");
}

static void accept_conn_cb(struct evconnlistener* listener,evutil_socket_t fd,struct sockaddr* address,int socklen,void* ctx)
{
	//	We got a new connection! Set up a bufferevent for it
	struct event_base* base = evconnlistener_get_base(listener);
	struct bufferevent* bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev,echo_read_cb,NULL,echo_event_cb,NULL);
	printf("echo_event_cb called\n");
}

static void accept_error_cb(struct evconnlistener* listener, void* ctx)
{
	struct event_base* base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Got an error %d (%s) on the listener. " "Shutting down.\n", err, evutil_socket_error_to_string(err));
	event_base_loopexit(base,NULL);
	printf("accept_error_cb called\n");
}

class TestLibEvent : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestLibEvent);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(libevent_test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
	void libevent_test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLibEvent);

//
// tests implementation
//

void TestLibEvent::test()
{
	InitSock();
	int port = 9876;
#ifdef ECHO_SERVER
	struct event_base* base;
	struct evconnlistener* listener;
	struct sockaddr_in	sin;
	
	base = event_base_new();
	if (!base)
	{
		puts("could not open event base");
		return ;
	}
	//	Clear the sockaddr before using it, in case there are extra platform-specific fields that can mess us up.
	memset(&sin,0,sizeof(sin));
	//	this is an inet address
	sin.sin_family = AF_INET;
	//	Listen on 0.0.0.0
	sin.sin_addr.s_addr = /*htonl(0)*/inet_addr("127.0.0.1");
	//	Listen on the given port
	sin.sin_port = htons(port);

	listener = evconnlistener_new_bind(base,accept_conn_cb,NULL,LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,-1,(struct sockaddr*)&sin,sizeof(sin));
	if (!listener)
	{
		perror("could not create listener!");
		return;
	}
	evconnlistener_set_error_cb(listener,accept_error_cb);
	event_base_dispatch(base);
#else
	SOCKET Listen;
	SOCKADDR_IN InternetAddr;
	if ((Listen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0,
		WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket() failed with error %d\n", WSAGetLastError());
		return ;
	} 
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	InternetAddr.sin_port = htons(port);
	// Connect to server.

	if (connect( Listen, (SOCKADDR*) &InternetAddr,sizeof(InternetAddr)) == SOCKET_ERROR) 
	{
		printf( "Failed to connect at %d.\n", WSAGetLastError());
		WSACleanup();
		return ;
	}
	int send_bytes = send(Listen,(char*)(123456),4,0);
	printf( "send %d bytes success.\n", send_bytes);
#endif
	return;
}

static int timer_count = 0;
struct event ev;
struct timeval tv;
void time_cb(int fd,short event,void* argc)
{
	++timer_count;
	printf("timer wakeup timer_count = %d\n",*static_cast<int*>(argc));
	event_add(&ev,&tv);
}


void TestLibEvent::libevent_test()
{
	struct event_base* base = event_init();
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	evtimer_set(&ev,time_cb,&timer_count);
	event_add(&ev,&tv);
	event_base_dispatch(base);
}