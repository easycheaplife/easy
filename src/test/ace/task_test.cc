#include "easy_cppunit_proxy.h"
#include "ace/Task.h"
//
// TestCase class
//
class Test_task : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(Test_task);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

class my_task : public ACE_Task_Base
{
	virtual int svc (void)
	{
		while(1)
		{
			::Sleep(1);
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(Test_task);

//
// tests implementation
//

void Test_task::test()
{
	my_task __task;
	__task.activate();
	__task.wait();
}
