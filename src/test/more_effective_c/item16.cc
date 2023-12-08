#include "easy_cppunit_proxy.h"
#include <iostream>
//
// TestCase class
//
class TestItem16 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem16);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(exception_per_test);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void exception_per_test();
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem16);

//
// tests implementation
//

#define EXCEPTION_COUNT		10000000
void TestItem16::exception_per_test() {
    int val = 0;
    for ( int i = 0; i < EXCEPTION_COUNT; ++i) {
        try {
            val += i;
        } catch(...) {

        }
    }
}

void TestItem16::test() {
    int val = 0;
    for ( int i = 0; i < EXCEPTION_COUNT; ++i) {
        val += i;
    }
}

/*
#define EXCEPTION_COUNT		100000
TestItem16::exception_per_test 0.689067 msec
TestItem16::test 0.622144 msec

#define EXCEPTION_COUNT		1000000
TestItem16::exception_per_test 2.546320 msec
TestItem16::test 2.599621 msec

#define EXCEPTION_COUNT		10000000
TestItem16::exception_per_test 25.941426 msec
TestItem16::test 29.575683 msec
*/
