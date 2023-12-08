/********************************************************************
	created:	2011/12/09
	created:	9:12:2011   11:49
	filename: 	C:\Users\Administrator\Desktop\cygwin\easy\src\base\easy_new_handle_support.h
	file path:	C:\Users\Administrator\Desktop\cygwin\easy\src\base
	file base:	easy_new_handle_support
	file ext:	h
	author:		Lee

	purpose:	a template class to use std::set_new_handle
*********************************************************************/
#ifndef easy_new_handle_support_h__
#define easy_new_handle_support_h__
#include <new>
namespace easy {
template<typename T>
class EasyNewHandleSupport {
  public:
    static std::new_handler set_new_handle(std::new_handler p) throw();

    static void* operator new (std::size_t size) throw(std::bad_alloc) /*std::nothrow()*/;

  private:
    static std::new_handler current_handle_;
};

template<typename T>
std::new_handler EasyNewHandleSupport<T>::current_handle_ = 0;

template<typename T>
std::new_handler EasyNewHandleSupport<T>::set_new_handle(std::new_handler p) {
    std::new_handler old_handle = current_handle_;
    current_handle_ = p;
    return old_handle;
}

template<typename T>
void* EasyNewHandleSupport<T>::operator new(size_t size) {
    set_new_handle(out_of_memory);
    return ::operator new(size);
}

void out_of_memory() {

}
}

#endif // easy_new_handle_support_h__