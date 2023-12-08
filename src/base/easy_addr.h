/********************************************************************
	created:	2012/03/03
	created:	3:3:2012   18:05
	filename: 	E:\lee_private_project\trunk\easy\src\base\easy_addr.h
	file path:	E:\lee_private_project\trunk\easy\src\base
	file base:	easy_addr
	file ext:	h
	author:		Lee

	purpose:	defines the base class for the address family independent address format
*********************************************************************/
#include "easy_base_type.h"

namespace easy {
class easy_addr {
  public:
    easy_addr( easy_int32 __type,easy_int32 __size) : addr_type_(__type), addr_size_(__size) { }

    ~easy_addr() { }

    easy_int32 get_size() const {
        return addr_size_;
    }

    void set_size(easy_int32 __size) {
        addr_size_ = __size;
    }

    easy_int32 get_type() const {
        return addr_type_;
    }

    void set_type(easy_int32 __type) {
        addr_type_ = __type;
    }

    virtual void* get_addr() const {
        return 0;
    }

    virtual void set_addr(void*, easy_int32 __len) { }

    bool operator == (const easy_addr& __sap) const {
        return (__sap.addr_size_ == this->addr_size_) && (__sap.addr_type_ == this->addr_type_);
    }

    bool operator != (const easy_addr& __sap) const {
        return (__sap.addr_size_ != this->addr_size_) || (__sap.addr_type_ != this->addr_type_);
    }

    void base_set(easy_int32 __type,easy_int32 __size) {
        addr_type_ = __type;
        addr_size_ = __size;
    }

    //	return the hash value, this should be overwritten by a subclass that can produce a better hash value
    virtual easy_ulong hash() const {
        return 0;
    }

    void dump() const { }

    //	wild card address
    static const easy_addr eap_any_;
  private:
    //	AF_UNIX, AF_INET, AF_SPIPE, etc.
    easy_int32 addr_type_;
    //	number of bytes in the address
    easy_int32 addr_size_;
};
//	const easy_addr easy_addr::sap_any(AF_ANY, -1);
}