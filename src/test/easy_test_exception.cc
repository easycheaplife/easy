#include "easy_cppunit_proxy.h"
#include "easy_exception.h"
//
// TestCase class
//
class TestException : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestException);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestException);

//
// tests implementation
//
#include <Windows.h>
using namespace easy;

void TestException::test() {
    try {
        //create a file ,if the file not exist,create it ,else open it.
        HANDLE hFile = CreateFile(
                           L"test_exception_file.txt",
                           GENERIC_READ|GENERIC_WRITE,
                           FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
        if (INVALID_HANDLE_VALUE == hFile) {
            EASY_EXCEPTION( EasyException::kErrorFileNotFound,"file not find!", "just a test", kExceptionLevText );
        }
    } catch (...) {

    }

}
