#include "easy_cppunit_proxy.h"
#include "easy_inet_addr.h"
//
// TestCase class
//
class test_inet_addr : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(test_inet_addr);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_inet_addr);

//
// tests implementation
//

void test_inet_addr::test() {

}
