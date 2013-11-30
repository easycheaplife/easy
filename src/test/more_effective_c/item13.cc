#include "easy_cppunit_proxy.h"
#include <exception>
#include <iostream>
//
// TestCase class
//
class TestItem13 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem13);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

void SoemFun()
{
	static std::exception	ex;		//	must be static, if not, the object will be invalid after the end of function called
	// ...
	throw &ex;
}

void DoSomething()
{
	try
	{
		SoemFun();
	}
	catch(std::exception* ex)
	{
		std::cout << "DoSomething exception happened" <<  ex->what() <<  std::endl;
	}
}

void SoemFun1()
{
	static std::runtime_error ex("runtime_error");		
	// ...
	throw  &ex;
}

void DoSomething1()
{
	try
	{
		SoemFun1();
	}
	catch(std::exception* ex)
	{
		std::cout << "DoSomething1 exception happened" <<  ex->what() <<  std::endl;
	}
}

void SoemFun2()
{
	static std::logic_error ex("runtime_error");		
	// ...
	throw  ex;
}

void DoSomething2()
{
	try
	{
		SoemFun2();
	}
	catch(std::exception& ex)
	{
		std::cout << "DoSomething1 exception happened" <<  ex.what() <<  std::endl;
	}
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem13);

//
// tests implementation
//
void TestItem13::test()
{
	DoSomething();
	DoSomething1();
	DoSomething2();
}
