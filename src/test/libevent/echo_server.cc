/*
  libevent version:V2.0.21
  this is a echo server,which receive packet from client and unpack data,then send back client.it reference 
  from ../../../dep/libevent/sample/hello-world.c.

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
  compile:
	$g++ -g -o echo_server echo_server.cc -I../../../dep/libevent/include -L../../../dep/libevent/.libs -levent
  run: 
	$export LD_LIBRARY_PATH=../../../dep/libevent/.libs
    $./echo_server
*/


#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>			//	exit,atoi
#ifndef WIN32
#include <netinet/in.h>
#include <fcntl.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

static void listener_cb(struct evconnlistener *, evutil_socket_t,struct sockaddr *, int socklen, void *);
static void conn_readcb(struct bufferevent *, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);
static void signal_cb(evutil_socket_t, short, void *);

int
main(int argc, char **argv)
{
	if(3 != argc)
	{
		printf("param error,please input correct param,for example : ./echo_server 192.168.22.61 9876\n");
		exit(1);
	}
	const char* __host = argv[1];
	unsigned int __port = atoi(argv[2]);
	struct event_base *base;
	struct evconnlistener *listener;
	struct event *signal_event;

	struct sockaddr_in sin;
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(__port);
	sin.sin_addr.s_addr = inet_addr(__host);
	listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
	    (struct sockaddr*)&sin,
	    sizeof(sin));

	if (!listener) {
		fprintf(stderr, "Could not create a listener!\n");
		return 1;
	}

	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

	if (!signal_event || event_add(signal_event, NULL)<0) {
		fprintf(stderr, "Could not create/add a signal event!\n");
		return 1;
	}

	event_base_dispatch(base);

	evconnlistener_free(listener);
	event_free(signal_event);
	event_base_free(base);

	printf("done\n");
	return 0;
}


static void
listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
	struct event_base *base = static_cast<event_base*>(user_data);
	struct bufferevent *bev;

	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}
	evutil_make_socket_nonblocking(fd);
	printf("socket = %d\n",fd);
	bufferevent_setcb(bev, conn_readcb, NULL/*conn_writecb*/, conn_eventcb, NULL);
	bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);
}

static void
conn_readcb(struct bufferevent *bev, void *user_data)
{
	struct evbuffer *input = bufferevent_get_input(bev);
	struct evbuffer *output = bufferevent_get_output(bev);
#if 0
	//copy all the data from the input buffer to the output buffer. 
	evbuffer_add_buffer(output, input);
#else
	//	parse packet
	int __packet_length = 0;
	int __log_level = 0;
	int __frame_number = 0;
	static const int __packet_head_size = 12;
	unsigned char __packet_head[__packet_head_size] = {};
	int __head = 0;
	unsigned int __guid = 0;
	//	read packet head and see if there is enough bytes
	ev_ssize_t __actually_size = evbuffer_copyout(input,__packet_head,__packet_head_size);
	if(__actually_size == __packet_head_size)
	{
		memcpy(&__packet_length,__packet_head,4);
		memcpy(&__head,__packet_head + 4,4);
		memcpy(&__guid,__packet_head + 8,4);
		//	read packet content
		static const int __max_buffer_size = 1024*8;
		char __read_buf[__max_buffer_size] = {};
		int __read_bytes = evbuffer_remove(input,__read_buf,__packet_length + __packet_head_size);
		if(-1 != __read_bytes)
		{
			evbuffer_add(output,__read_buf,__packet_length + __packet_head_size);
		}
	}
	else if(__actually_size < __packet_head_size)
	{
#ifdef __DEBUG
		printf("__actually_size < __packet_head_size __actually_size = %d\n",__actually_size);
#endif //__DEBUG
		return;
	}
#endif 
}

static void
conn_writecb(struct bufferevent *bev, void *user_data)
{
	struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) {
		printf("flushed answer\n");
		bufferevent_free(bev);
	}
}

static void
conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	evutil_socket_t __fd = bufferevent_getfd(bev);
	if (events & BEV_EVENT_EOF) {
		printf("Connection closed %d.\n",__fd);
	} else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
		    strerror(errno));/*XXX win32*/
	}
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	bufferevent_free(bev);
}

static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = static_cast<event_base*>(user_data);
	struct timeval delay = { 2, 0 };

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}
