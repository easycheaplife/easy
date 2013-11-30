#include "easy_cppunit_proxy.h"
#include "easy_assert.h"
#include <list>
//
// TestCase class
//
class TestAssert : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestAssert);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAssert);

//
// tests implementation
//
#include <string>
void TestAssert::test()
{
	easy_assert(0);

	std::string file = _EASY_CRT_APPEND("file1","file2");

	std::string val = _EASY_CRT_STRINGIZE(2);

}
