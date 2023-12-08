#include "easy_cppunit_proxy.h"
#define  __USE_CRITICAL_SECTION
#include "easy_guard.h"
#include "easy_thread_mutex.h"
//
// TestCase class
//
class Test_guard : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(Test_guard);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Test_guard);

//
// tests implementation
//

int guard_test() {
#if 1
    easy::thread_mutex __mtx;
    EASY_GUARD_RETURN(easy::thread_mutex,__object,__mtx,-1);
#else
    easy::thread_mutex __mtx_write;
    EASY_WRITE_GUARD_RETURN(easy::thread_mutex,__object_write,__mtx_write,-1);

    easy::thread_mutex __mtx_read;
    EASY_READ_GUARD_RETURN(easy::thread_mutex,__object_read,__mtx_read,-1);
#endif
    return 0;
}
void Test_guard::test() {
    guard_test();
}
