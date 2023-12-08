/********************************************************************
	created:	2012/03/03
	created:	3:3:2012   18:26
	filename: 	E:\lee_private_project\trunk\easy\src\base\inet_addr.h
	file path:	E:\lee_private_project\trunk\easy\src\base
	file base:	inet_addr
	file ext:	h
	author:		Lee

	purpose:	define a c++ wrapper facade for the internet domain address family format
*********************************************************************/
#ifndef inet_addr_h__
#define inet_addr_h__
#include "easy_addr.h"
#include "easy_os_socket.h"

#ifdef __WINDOWS
#include <winsock.h>
#elif defined __LINUX

#endif

namespace easy {
//	helper function to get a port number from a port name.
static int get_port_number_from_name (const easy_char __port_name[],
                                      const easy_char __protocol[]) {
    return 0;
}
class easy_inet_addr : public easy_addr {
  public:
    easy_inet_addr() : easy_addr(AF_INET,sizeof (inet_addr_)) {
        reset();
    }

    easy_inet_addr( const easy_inet_addr& __sa) : easy_addr(__sa.get_type(),__sa.get_size()) {
        this->reset();
        this->set(__sa);
    }

    //	initialize underlying inet_addr_ default values
    void reset() {
        memset(&inet_addr_,0,sizeof(inet_addr_));
        if (get_type() == AF_INET) {
#ifdef EASY_HAS_SOCKADDR_IN_SIN_LEN
            this->inet_addr_.in4_.sin_len = sizeof(this->inet_addr_.in4_);
#endif //EASY_HAS_SOCKADDR_IN_SIN_LEN
            this->inet_addr_.in4_.sin_family = AF_INET;
        }
#if defined (EASY_HAS_IPV6)
        else if (get_type() == AF_INET6) {
#ifdef EASY_HAS_SOCKADDR_IN6_SIN6_LEN
            this->inet_addr_.in6_.sin_len = sizeof(this->inet_addr_.in6_);
#endif //EASY_HAS_SOCKADDR_IN6_SIN6_LEN
            this->inet_addr_.in6_.sin_family = AF_INET6;
        }
#endif //EASY_HAS_IPV6
    }
    //	these methods are useful after the object has been constructed
    easy_int32 set(const easy_inet_addr& __sa) {
        if (__sa.get_type() == AF_ANY) {
            //	ugh,this is realy a base class, so don't copy it
            memset(&this->inet_addr_,0,sizeof(this->inet_addr_));
        } else {
            memcpy(&this->inet_addr_,&__sa.inet_addr_,__sa.get_size());
            this->set_type(__sa.get_type());
            this->set_size(__sa.get_size());
        }
        return 0;
    }

    //	initializes an easy_inet_addr from a @a port_number and the remote @a host_home,
    //	if @a encode is non-zero then @a port_number is converted into network byte order,
    //	otherwise it is assumed to be in network byte order, otherwise it is assumed to be
    //	in network byte order already and are passed straight through.
    //	address_family can be used to select IPv4/IPv6 if the OS has
    //	IPv6 capability (EASY_HAS_IPV6 is defined). To specify IPv6, use
    //	the value AF_INET6. To specify IPv4, use AF_INET.
    easy_int32 set(easy_uint16 __port_number,const easy_char* __host_name[],easy_int32 __encode = 1, easy_int32 __address_family = AF_UNSPEC) {
        if ( 0 == __host_name) {
            return -1;
        }
        memset((void*)&this->inet_addr_,0,sizeof(this->inet_addr_));
        __address_family = AF_INET;
        set_type(__address_family);
        inet_addr_.in4_.sin_family = static_cast<easy_int16>(__address_family);
#ifdef EASY_HAS_SOCKADDR_IN_SIN_LEN
        this->inet_addr_.in4_.sin_len = sizeof (this->inet_addr_.in4_);
#endif //EASY_HAS_SOCKADDR_IN_SIN_LEN
        struct in_addr __addrv4;
        //	 to be continue ...

        return 0;
    }

    //	initializes an easy_inet_addr from a @a port_number and an Internet
    //	@a ip_addr.  If @a encode is non-zero then the port number and IP address
    //	are converted into network byte order, otherwise they are assumed to be
    //	in network byte order already and are passed straight through.
    //	If @a map is non-zero and IPv6 support has been compiled in,
    //	 then this address will be set to the IPv4-mapped IPv6 address of it.
    easy_int32 set(easy_uint16 __port_number,easy_uint32 __ip_address = INADDR_ANY,easy_int32 __encode = 1,easy_int32 __map = 0) {
        this->set_address(reinterpret_cast<const char*>(&__ip_address),sizeof(__ip_address),__encode,__map);
        this->set_port_number(__port_number,__encode);
        return 0;
    }

    //	ues getservbyname() to initialize an easy_inet_addr from a
    //	@a port_name, the remote @a host_name, and the @a protocol.
    easy_int32 set(const char __port_name[],const char __host_name[],const char __protocol[] = "tcp") {
        easy_int32 const __port_number = get_port_number_from_name(__port_name,__protocol);
        if (-1 == __port_number) {

        }
        //	 to be continue ...
        return 0;
    }

    //	ses getservbyname() to initialize an easy_inet_addr from a
    //	@a port_name, an @a ip_addr, and the @a protocol.  This assumes that
    //	@a ip_addr is already in network byte order.
    easy_int32 set(const char __port_name[],easy_uint32 __ip_addr,const char __protocol[] = "tcp") {
        return 0;
    }

    //	initializes an easy_inet_addr from the @a addr, which can be
    //	"ip-number:port-number" (e.g., "tango.cs.wustl.edu:1234" or
    //	"128.252.166.57:1234").  If there is no ':' in the @a address it
    //  is assumed to be a port number, with the IP address being INADDR_ANY.
    easy_int32 set(const char __addr[], easy_int32 __address_family = AF_UNSPEC) {
        return 0;
    }

    //	creates an easy_inet_addr from a sockaddr_in structure.
    easy_int32 set(const sockaddr_in * __addr,easy_int32 __len) {
        if (__addr->sin_family == AF_INET) {
            easy_int32 __max_len = static_cast<int>(sizeof(this->inet_addr_.in4_));
            if (__len > __max_len) {
                __len = __max_len;
            }
            memcpy(&this->inet_addr_.in4_,&__addr,__len);
            this->base_set(AF_INET,__len);
            return 0;
        }
        return -1;
    }

#if defined (EASY_HAS_WCHAR)
    easy_int32 set (easy_uint16 __port_number,
                    const easy_wchar __host_name[],
                    easy_int32 __encode = 1,
                    easy_int32 __address_family = AF_UNSPEC);

    easy_int32 set (const easy_wchar __port_name[],
                    const easy_wchar __host_name[],
                    const easy_wchar __protocol[] = ACE_TEXT_WIDE ("tcp"));

    easy_int32 set (const easy_wchar __port_name[],
                    easy_uint32 __ip_addr,
                    const easy_wchar __protocol[] = ACE_TEXT_WIDE ("tcp"));

    easy_int32 set (const easy_wchar addr[], easy_int32 address_family = AF_UNSPEC);
#endif /* EASY_HAS_WCHAR */

    //	sets the port number without affecting the host name.  If
    //	@a encode is enabled then @a port_number is converted into network
    //	byte order, otherwise it is assumed to be in network byte order
    //	already and are passed straight through.
    easy_int32 set_address(const char* __ip_addr, easy_int32 __len, easy_int32 __encode = 0, easy_int32 __map = 0) {
        return 0;
    }

    //	sets the address without affecting the port number.  If
    //	@a encode is enabled then @a ip_addr is converted into network
    //	byte order, otherwise it is assumed to be in network byte order
    //	already and are passed straight through.  The size of the address
    //	is specified in the @a len parameter.
    //	If @a map is non-zero, IPv6 support has been compiled in, and
    //	@a ip_addr is an IPv4 address, then this address is set to the IPv4-mapped
    //	IPv6 address of it.
    void set_port_number(easy_int16,easy_int32 = 1) {

    }

    union {
        sockaddr_in  in4_;
#if defined (EASY_HAS_IPV6)
        sockaddr_in6 in6_;
#endif /* EASY_HAS_IPV6 */
    } inet_addr_;
};
}


#endif // inet_addr_h__
