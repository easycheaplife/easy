#include "logging_server.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/os_include/os_netdb.h"
#include "ace/OS_NS_string.h"
#include "ace/FILE_Connector.h"

int logging_server::run( unsigned short __port )
{
	if (-1 == open(__port))
	{
		return -1;
	}
	for ( ; ; )
	{
		if (-1 == wait_for_multiple_event())
		{
			return -1;
		}
		if (-1 == handle_connections())
		{
			return -1;
		}
		if (-1 == handle_data())
		{
			return -1;
		}
	}
}

int logging_server::open( unsigned short __port )
{
	ACE::set_handle_limit();
	ACE_INET_Addr __server_addr;
	int __res = __server_addr.set(__port,(ACE_UINT32)INADDR_ANY);
	if (-1 == __res)
	{
		return -1;
	}
	return acceptor_.open(__server_addr,1);
}

int logging_server::make_log_file( ACE_FILE_IO& __logging_file,ACE_SOCK_Stream* __logging_peer)
{
	char __file_name[MAXHOSTNAMELEN + sizeof("*.log")] = {0};
	if (__logging_peer != 0)
	{
		ACE_INET_Addr __logging_peer_addr;
		__logging_peer->get_remote_addr(__logging_peer_addr);
		__logging_peer_addr.get_host_name(__file_name,MAXHOSTNAMELEN);
		ACE_OS::strcat(__file_name,".log");
	}
	else
	{
		ACE_OS::strcat(__file_name,"logging_server.log");
	}
	ACE_FILE_Connector __connector; 
	return __connector.connect(__logging_file,ACE_FILE_Addr(__file_name),0,ACE_Addr::sap_any,0,O_RDWR|O_CREAT|O_APPEND,ACE_DEFAULT_FILE_PERMS);
}
