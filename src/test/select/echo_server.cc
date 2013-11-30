#include <fcntl.h>			//	fcntl
#include <stdlib.h>			//	exit
#include <sys/select.h>		//	select
#include <sys/socket.h>		//	socket
#include <netinet/in.h>		//	sockaddr_in
#include <strings.h>		//	bzero
#include <arpa/inet.h>		//	inet_addr
#include <errno.h>			//	errno
#include <unistd.h>			//	read
#include <stdio.h>			//	perror
#include <string.h>			//	memset
#include <iostream>

int main()
{
	std::cout << "start select echo server..." << std::endl;

	static const int max_back_log = 10;
	int cur_conn_nun = 0;
	int fd_conn_arr[max_back_log];

	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == listenfd)
	{
		std::cout << "error at socket" << std::endl;
		exit(1);
	}

	int option_name = 1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option_name, sizeof(int)) == -1)
	{
        perror("setsockopt");
        exit(1);
    }

	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9877);
	inet_aton("127.0.0.1",&(serveraddr.sin_addr));
	bind(listenfd,(sockaddr*)&serveraddr,sizeof(sockaddr_in));

	listen(listenfd,max_back_log);

	fd_set fd_flag;
	int connectfd = 0;
	struct sockaddr_in clientaddr;
	struct timeval tv;
	socklen_t	client_addr_len;
	int read_size = 0;
	int send_size = 0;
	int read_data = 0;
	int fd_max = listenfd;
	memset(fd_conn_arr,0,sizeof(int)*max_back_log);
	while(1)
	{
		FD_ZERO(&fd_flag);
		FD_SET(listenfd,&fd_flag);
		
		tv.tv_sec = 15;
		tv.tv_usec = 0;
		
		for(int i = 0; i < max_back_log; ++i)
		{
			if(fd_conn_arr[i] != 0)
			{	
				FD_SET(fd_conn_arr[i],&fd_flag);
			}
		}
		int ret = select(fd_max + 1,&fd_flag,NULL,NULL,&tv);
		if(-1 > ret)
		{
			perror("error at select");
			exit(1);
		}
		else if(0 == ret)
		{
			perror("select time out");
			continue;
		}

		for(int i = 0; i < cur_conn_nun; ++i)
		{
			if(FD_ISSET(fd_conn_arr[i],&fd_flag))
			{
				if((read_size = read(fd_conn_arr[i],(void*)&read_data,sizeof(int))) < 0 )
				{
					perror("read");
				}
				else
				{
					std::cout << read_size << " bytes data read: " << read_data << std::endl;
				}
				if((send_size = write(fd_conn_arr[i],(void*)&read_data,sizeof(int))) < 0 )
				{
					perror("write");
				}
				else
				{
					std::cout << send_size << " bytes data send: " << read_data << std::endl;
				}
			}
		}
		if(FD_ISSET(listenfd,&fd_flag))
		{
			connectfd = accept(listenfd,(sockaddr*)&clientaddr,&client_addr_len);
			if(-1 == connectfd)
			{
				perror("accept");
				continue;
			}	
			if(cur_conn_nun < max_back_log)
			{
				fd_conn_arr[cur_conn_nun++] = connectfd;
				if(fd_max < connectfd)
				{
					fd_max = connectfd;
				}
			}

			std::cout << "new connection coming at " << inet_ntoa(clientaddr.sin_addr) << ": " << ntohs(clientaddr.sin_port) << std::endl;
		}
	}
	return 0;
}
