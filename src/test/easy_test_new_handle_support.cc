#include "easy_cppunit_proxy.h"
#include "easy_new_handle_support.h"
//
// TestCase class
//
class TestNewHandleSupport : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestNewHandleSupport);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestNewHandleSupport);

//
// tests implementation
//
class Widget : public easy::EasyNewHandleSupport<Widget> {

};
void TestNewHandleSupport::test() {
    Widget* p = new Widget;
    delete p;
}
