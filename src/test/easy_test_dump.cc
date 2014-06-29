#include "easy_cppunit_proxy.h"
#include "easy_dump.h"

//	reference from http://blog.csdn.net/b_fushuaibing/article/details/5560304

//
// TestCase class
//
class TestDump : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestDump);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(test_signal);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDump);

//
// tests implementation
//

static void /* "static" means don't export the symbol... */ myfunc2(void)
{
    dump(0);
}

void myfunc(int ncalls)
{
    if (ncalls > 1)
        myfunc(ncalls - 1);
    else
        myfunc2();
}

void dump_print()
{
	signal(SIGSEGV, dump);
}

void segv_fun()
{
	unsigned char* __ptr = 0x00;
	*__ptr = 0x00;
}

void TestDump::test()
{
	printf("TestDump::test\n");
	myfunc(1);
}

void TestDump::test_signal()
{
	dump_print();
}

