// echo_c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/Sock_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"

#pragma comment(lib,"Ws2_32.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData = {0};
	int iResult = 0;
	// Initialize Winsock
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}
	ACE_INET_Addr __srvr(5000,ACE_LOCALHOST);
	ACE_SOCK_Connector __connector;
	ACE_SOCK_STREAM __peer;
	if (-1 == __connector.connect(__peer,__srvr))
	{
		ACE_ERROR_RETURN((LM_ERROR,ACE_TEXT("%p\n"),ACE_TEXT("connect")),1);
	}
	while(1)
	{
		char __buf[64];
		memset(__buf,0,64);
		strcpy(__buf,"hello lee!");
		ssize_t __byte_send ;
		ssize_t __byte_recv ;
		ACE_Message_Block __message_block;
		
		__byte_send = __peer.send_n(__buf,sizeof(__buf));
		printf("%d bytes send: %s\n",__byte_send,__buf);
		if (-1 == __byte_send)
		{
			break;
		}
		__byte_recv = __peer.recv(__buf,sizeof(__buf));
		printf("%d bytes recv: %s\n",__byte_recv,__buf);
		if (-1 == __byte_recv)
		{
			break;
		}
	}
	__peer.close();
	return 0;
}

