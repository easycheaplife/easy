#include "easy_cppunit_proxy.h"
#include "easy_counted_auto_ptr.h"
#include "easy_mutex.h"
//
// TestCase class
//
class TestCountAutoPtr : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestCountAutoPtr);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCountAutoPtr);

//
// tests implementation
//
#include <iostream>
class Widget {
  public:
    void Printf() {
        std::cout << "Widget::Printf" << std::endl;
    }
};
void TestCountAutoPtr::test() {
    easy::EasyNullMutex mutex;
    easy::EasyAtomicOpEx<easy::EasyNullMutex,long> atomic(mutex);
    ++atomic;
    atomic++;
    --atomic;
    atomic--;
    atomic += 10;
    atomic -= 5;
    easy::EasyAtomicOpEx<easy::EasyNullMutex,long> atomic_temp(mutex);
    atomic_temp = atomic;

    if(atomic_temp == 0) {
        return;
    }

    float f = 100;
    easy::EasyAtomicOpEx<easy::EasyNullMutex,float> atomic_type(mutex,f);
    ++atomic_type;
    atomic_type++;
    --atomic_type;
    atomic_type--;

    Widget* widget = new Widget();
    typedef easy::EasyRefcountedAutoPtr<Widget, easy::EasyNullMutex> RefcountedAutoPtrWidget;
    RefcountedAutoPtrWidget ref_count_auto_ptr(widget);
    ref_count_auto_ptr->Printf();

    RefcountedAutoPtrWidget ref_count_auto_ptr_null(NULL);
}


