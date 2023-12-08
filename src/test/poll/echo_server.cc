#include <fcntl.h>			//	fcntl
#include <stdlib.h>			//	exit
#include <sys/socket.h>		//	socket
#include <netinet/in.h>		//	sockaddr_in
#include <strings.h>		//	bzero
#include <arpa/inet.h>		//	inet_addr
#include <unistd.h>			//	read
#include <stdio.h>			//	perror
#include <poll.h>
#include <iostream>

int main() {
    std::cout << "start poll echo server..." << std::endl;

    static const int max_back_log = 10;
    static const int max_poll_fd = 2;
    struct pollfd fds[max_poll_fd];

    for(int i = 0; i < max_poll_fd; ++i) {
        fds[i].fd = -1;
    }
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == listenfd) {
        perror("socket");
        exit(1);
    }

    int option_name = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option_name, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9876);
    inet_aton("127.0.0.1",&(serveraddr.sin_addr));
    if( -1 == bind(listenfd,(sockaddr*)&serveraddr,sizeof(sockaddr_in))) {
        perror("bind");
        exit(1);
    }

    if( -1 == listen(listenfd,max_back_log)) {
        perror("bind");
        exit(1);
    }

    int cur_poll_fd_num = 0;
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    ++cur_poll_fd_num;

    fds[1].fd = listenfd;
    fds[1].events = POLLIN;
    ++cur_poll_fd_num;

    int timeout_msecs = 50000;
    int connectfd = 0;
    struct sockaddr_in clientaddr;
    socklen_t	client_addr_len;
    int read_data = 0;
    int send_size = 0;
    int read_size = 0;
    while(1) {
        int ret = poll(fds,cur_poll_fd_num,timeout_msecs);
        if(-1 == ret) {
            perror("poll");
            exit(1);
        } else if(0 == ret) {
            perror("time out");
            continue;
        }
        for(int i = 0; i <cur_poll_fd_num; ++i) {
            if(fds[i].revents & POLLIN) {
                if(0 == fds[i].fd) {
                    std::cout << read_size << " fds[i].fd = 0 " << read_data << std::endl;
                } else if(listenfd == fds[i].fd) {
                    connectfd = accept(listenfd,(sockaddr*)&clientaddr,&client_addr_len);
                    if(-1 == connectfd) {
                        perror("accept");
                        continue;
                    }
                    fds[cur_poll_fd_num].fd = connectfd;
                    fds[cur_poll_fd_num].events = POLLIN;
                    ++cur_poll_fd_num;
                    std::cout << "new connection coming at " << inet_ntoa(clientaddr.sin_addr) << ": " << ntohs(clientaddr.sin_port) << std::endl;
                } else {
                    if((read_size = read(fds[i].fd,(void*)&read_data,sizeof(int))) < 0 ) {
                        perror("read");
                    } else {
                        std::cout << read_size << " bytes data read: " << read_data << std::endl;
                    }
                    if((send_size = write(fds[i].fd,(void*)&read_data,sizeof(int))) < 0 ) {
                        perror("write");
                    } else {
                        std::cout << send_size << " bytes data send: " << read_data << std::endl;
                    }
                }
            }
        }
    }
    return 0;
}
