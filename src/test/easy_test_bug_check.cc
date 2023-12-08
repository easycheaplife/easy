#include <assert.h>
#include "easy_cppunit_proxy.h"
#include "KException.h"
//
// TestCase class
//
class TestBugCheck : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestBugCheck);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestBugCheck);

//
// tests implementation
//

#define KYLIN_TRY	try{
#define KYLIN_CATCH(str)			\
}						\
	catch (...)				\
{						\
	ExceptionPushBack(str);			\
	throw;					\
};

void GameTest() {
    KYLIN_TRY
#if 0
    assert(NULL);
#else
    char* p = NULL;
    *p = 'a';
#endif
    KYLIN_CATCH("GameTest1");
}

void Game() {
    try {
        GameTest();
    } catch (const std::exception& e) {
        ExceptionPushPop(e.what());
        throw;
    } catch (...) {
        throw;
    }
}

void TestBugCheck::test() {
    char	tReprotStr[]="adfasefbasdrobadbasdasdfgasdf";
    __try {
        Game();
    } __except( ExceptionParse( GetExceptionInformation(), true, tReprotStr, 0 ),
                TerminateProcess( GetCurrentProcess(), 0 ),
                EXCEPTION_EXECUTE_HANDLER ) {

    }
}
