#include "easy_cppunit_proxy.h"
#include <iostream>
//
// TestCase class
//
class TestItem14 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem14);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem14);

//
// tests implementation
//

class Session {
  public:
    ~Session() {
        try {
            LogDestruction(this);
        } catch(std::string& exception) {
            std::cout << exception.c_str() <<std::endl;
        }
    }

    static void LogDestruction(Session* session) throw() {
        std::cout << "LogDestruction" <<std::endl;
    }
};
void TestItem14::test() {
    Session session;
}
