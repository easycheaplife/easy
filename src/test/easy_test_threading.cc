#include "easy_cppunit_proxy.h"
#include "easy_threading.h"
#include <iostream>
#ifdef __LINUX
#include <unistd.h>
#elif defined __WINDOWS
#include <windows.h>
#endif //__LINUX
//
// TestCase class
//
class TestThreading : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestThreading);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestThreading);

//
// tests implementation
//

class ThreadTest : public easy::EasyThread {
  public:
    ThreadTest() {}
    ~ThreadTest() {}
  private:
    easy_int32 _Run( void* p ) {
        static int index = 0;
        while(1) {
            ++index;
            std::cout << index << std::endl;
#ifdef __LINUX
            sleep(1);
#elif defined __WINDOWS
            ::Sleep(100);
#endif //__LINUX
        }
        return 0;
    }
};

void TestThreading::test() {
    ThreadTest* thread = new ThreadTest();
    thread->Suspend();
    thread->Resume();
    while(1) {
#ifdef __LINUX
        sleep(1);
#elif defined __WINDOWS
        ::Sleep(1);
#endif //__LINUX
    }
    delete thread;
}
