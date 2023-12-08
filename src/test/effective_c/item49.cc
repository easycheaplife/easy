#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem49 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem42);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem49);

//
// tests implementation
//
#include <iostream>
typedef void (*my_new_handle) ();

my_new_handle set_my_new_handle(my_new_handle my_handle) throw () {
    return my_handle;
}

void out_of_memory() {
    std::cout << "get more memory, or install another new_handle,or unstall new_handle, or throw bad_alloc" <<std::endl;
}

class NewhHandleHolder {
  public:
    NewhHandleHolder(std::new_handler p) : handle_(p) { }

    ~NewhHandleHolder() {
        std::set_new_handler(handle_);
    }
  private:
    std::new_handler handle_;
    NewhHandleHolder (const NewhHandleHolder&);
    NewhHandleHolder& operator = (const NewhHandleHolder&);
};

class Widget {
  public:
    static std::new_handler set_new_handle(std::new_handler p) throw();
    static void* operator new (std::size_t size) throw (std::bad_alloc) {
        NewhHandleHolder h(set_new_handle(current_handle));
        return ::operator new(size);
    }
  private:
    static std::new_handler current_handle;
};

std::new_handler Widget::current_handle = 0;

std::new_handler Widget::set_new_handle(std::new_handler p) throw() {
    std::new_handler old_handle = current_handle;
    current_handle = p;
    return old_handle;
}

template<typename T>
class NewHanleSupport {
  public:
    static std::new_handler set_new_handle(std::new_handler p) throw();

    static void* operator new (std::size_t size) throw(std::bad_alloc) /*std::nothrow()*/;

  private:
    static std::new_handler current_handle_;
};

template<typename T>
std::new_handler NewHanleSupport<T>::current_handle_ = 0;

template<typename T>
std::new_handler NewHanleSupport<T>::set_new_handle(std::new_handler p) {
    std::new_handler old_handle = current_handle_;
    current_handle_ = p;
    return old_handle;
}

template<typename T>
void* NewHanleSupport<T>::operator new(size_t size) {
    set_new_handle(out_of_memory);
    return ::operator new(size);
}

class WidgetSupport : public NewHanleSupport<WidgetSupport> {

};

void TestItem49::test() {
    set_my_new_handle(out_of_memory);
    int* p_arr = new int[100000000L];
    delete[] p_arr;

    Widget::set_new_handle(out_of_memory);
    Widget* pw1 = new Widget;
    Widget::set_new_handle(0);
    delete pw1;

    WidgetSupport* pw2 = new WidgetSupport;
    delete pw2;

    //	std::bad_alloc and st::nothrow
    //	to be continue..

}
