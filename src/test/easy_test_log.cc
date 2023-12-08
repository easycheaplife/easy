#include "easy_cppunit_proxy.h"
#include "easy_log.h"
//
// TestCase class
//
class TestLog : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestLog);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLog);

//
// tests implementation
//

void TestLog::test() {
    easy::EasyLog::SaveLog("..\\log\\test.log",easy::kErrors,"test log.................%d",12);
    easy::EasyLog::SaveLog("..\\log\\test.log",easy::kWarnings,"test log.................%d",12);
    easy::EasyLog::SaveLog("..\\log\\test.log",easy::kStandard,"test log.................%d",12);
    easy::EasyLog::SaveLog("..\\log\\test.log",easy::kInformative,"test log.................%d",12);

    using namespace easy;
    s_log->SaveLog("..\\log\\test.log",easy::kErrors,"test log.................%d",1000);
    s_log->OpenLogFile("..\\log\\test1.log");
    s_log->set_level(kErrors);
    s_log->SaveLog(".................kErrors...................");
    s_log->set_level(kWarnings);
    s_log->SaveLog("..................kWarnings.................");
    EasySingleton<EasyLog>::Close();
}
