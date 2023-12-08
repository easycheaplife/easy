/********************************************************************
	created:	2011/12/10
	created:	10:12:2011   12:38
	filename: 	F:\lee-private-project\trunk\easy\src\base\easy_stdandard_allocator.h
	file path:	F:\lee-private-project\trunk\easy\src\base
	file base:	easy_stdandard_allocator
	file ext:	h
	author:		Lee

	purpose:	standard new/delete
*********************************************************************/
#ifndef easy_stdandard_allocator_h__
#define easy_stdandard_allocator_h__
#include <new>

namespace easy {
class easy_stdand_allocator {
  public:
    //	normal new/delete
    static void* operator new(std::size_t size) throw( std::bad_alloc ) {
        return ::operator new (size);
    }

    static void operator delete(void* ptr) throw() {
        ::operator delete(ptr);
    }

    // placement new/delete
    static void* operator new (std::size_t size, void* ptr) throw() {
        return ::operator new (size,ptr);
    }
    static void operator delete(void* memory,void* ptr) throw() {
        return ::operator delete(memory,ptr);
    }

    //	nothrow new/delete
    static void* operator new(std::size_t size, const std::nothrow_t& nt) throw() {
        return ::operator new(size,nt);
    }
    static void operator delete (void* memory,const std::nothrow_t&) throw() {
        ::operator delete(memory);
    }
};

struct easy_default_user_allocator_new_delete {

};

}
#endif // easy_stdandard_allocator_h__