
#include "easy_cppunit_proxy.h"
#include<stdio.h>
//
// TestCase class
//
class TestNull : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestNull);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestNull);

//
// tests implementation
//

void TestNull::test()
{
	printf("...................\n");
}
