#include "easy_cppunit_proxy.h"
#include "ace/OS.h"
#include "ace/Process.h"
//
// TestCase class
//
class Test_process : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(Test_process);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Test_process);

//
// tests implementation
//

//	
int factorrial(int __n)
{
	ACE_Process_Options __option;
	__option.command_line("%s %d","easy.exe",__n);
	ACE_Process __child;
	__child.spawn(__option);
	__child.wait();
	return __n * __child.exit_code();
}
void Test_process::test()
{
	int __res = factorrial(10);
}
