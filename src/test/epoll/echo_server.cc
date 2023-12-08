#include <fcntl.h>			//	fcntl
#include <stdlib.h>			//	exit
#include <sys/epoll.h>		//	epoll_event 
#include <sys/socket.h>		//	socket
#include <netinet/in.h>		//	sockaddr_in
#include <strings.h>		//	bzero
#include <arpa/inet.h>		//	inet_addr
#include <errno.h>			//	errno
#include <unistd.h>			//	read
#include <iostream>

void set_no_block(int sock) {
    int opts = fcntl(sock,F_GETFL);
    if(0 > opts) {
        std::cout << "error at fcntl(sock,F_GETFL)" << std::endl;
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts) < 0) {
        std::cout << "error at fcntl(sock,F_SETFL,opts)" << std::endl;
        exit(1);
    }
}

void epoll_ctl_err_show() {
    std::cout << "error at epoll_ctl" << std::endl;
    if(EBADF == errno) {
        std::cout << "error at epoll_ctl, error  is EBADF" << std::endl;
    } else if(EEXIST == errno) {
        std::cout << "error at epoll_ctl, error  is EEXIST" << std::endl;
    } else if(EINVAL == errno) {
        std::cout << "error at epoll_ctl, error  is EINVAL" << std::endl;
    } else if(ENOENT == errno) {
        std::cout << "error at epoll_ctl, error  is ENOENT" << std::endl;
    } else if(ENOMEM == errno) {
        std::cout << "error at epoll_ctl, error  is ENOMEM" << std::endl;
    } else if(ENOSPC == errno) {
        std::cout << "error at epoll_ctl, error  is ENOSPC" << std::endl;
    }
    exit(1);
}

int main() {
    std::cout << "start epoll echo server... ..." << std::endl;
    int epfd = epoll_create(256);
    if(-1 == epfd) {
        std::cout << "error at epoll_create" << std::endl;
        exit(1);
    }
    struct sockaddr_in clientaddr;
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == listenfd) {
        std::cout << "error at socket" << std::endl;
        exit(1);
    }
    struct epoll_event ev;
    ev.data.fd = listenfd;
    ev.events = EPOLLIN|EPOLLET;
    int err_code = 0;
    err_code = epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    if(-1 == err_code) {
        epoll_ctl_err_show();
    }
    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9876);
    inet_aton("127.0.0.1",&(serveraddr.sin_addr));
    bind(listenfd,(sockaddr*)&serveraddr,sizeof(sockaddr_in));

    listen(listenfd,10);

    int nfds = 0;
    const int time_out = 500;
    const int max_events = 20;
    int connfd = 0;
    int sockfd = 0;
    ssize_t read_size = 0;
    int read_data = 0;
    ssize_t send_size = 0;
    socklen_t	client_addr_len;
    struct epoll_event events[max_events];
    for(;;) {
        nfds = epoll_wait(epfd,events,max_events,time_out);
        for(int i = 0; i < nfds; ++i) {
            sockfd = events[i].data.fd;
            if(sockfd == listenfd) {
                connfd = accept(listenfd,(sockaddr*)&clientaddr,&client_addr_len);
                if(-1 == connfd) {
                    std::cout << "error at accept" << std::endl;
                    exit(1);
                }
                std::cout << "accept a connection from " << inet_ntoa(clientaddr.sin_addr) <<std::endl;
                ev.data.fd = connfd;
                ev.events = EPOLLIN|EPOLLET;
                err_code = epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
                if(-1 == err_code) {
                    epoll_ctl_err_show();
                }
            } else if(events[i].events&EPOLLIN) {
                //	sockfd is equal connfd actully
                if((read_size = read(sockfd,(void*)&read_data,sizeof(int))) < 0 ) {
                    if(ECONNRESET == errno) {
                        close(sockfd);
                        events[i].data.fd = -1;
                    } else {
                        std::cout << "read error" <<std::endl;
                    }
                } else if(0 == read_size) {
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                std::cout << read_size << " bytes data read: " << read_data << std::endl;
                ev.data.fd = connfd;
                ev.events = EPOLLOUT|EPOLLET;
                err_code = epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
                if(-1 == err_code) {
                    epoll_ctl_err_show();
                }
            } else if(events[i].events&EPOLLOUT) {
#if 0
                send_size = write(sockfd,(void*)&read_data,sizeof(int));
#else
                send_size = send(sockfd,(void*)&read_data,sizeof(int),0);
#endif
                if(-1 != send_size) {
                    std::cout << send_size << " bytes data send: " << read_data << std::endl;
                } else {
                    std::cout <<" data send error" << std::endl;
                }
                ev.data.fd = sockfd;
                ev.events = EPOLLIN|EPOLLET;
                err_code = epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
                if(-1 == err_code) {
                    epoll_ctl_err_show();
                }
            }
        }
    }
    return 0;
}
