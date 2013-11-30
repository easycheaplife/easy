#include "easy_cppunit_proxy.h"
#include "easy_memory.h"
//
// TestCase class
//
class TestItem9 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem9);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem9);

//
// tests implementation
//
#include <iostream>
class Base
{
public:
	virtual void Process() = 0;
};

class Derive1 : public Base
{
public:
	virtual void Process() { throw std::string("Derive1,if not use std::string but c style string,exception will bot be catch,can not be convered");}
};

class Derive2 : public Base
{
public:
	virtual void Process() { throw "Derive2, will be catch by char*!"; }
};

Base* CreateObject(int i)
{
	if (0 == i / 2)
	{
		return new Derive1();
	}
	else
	{
		return new Derive2();
	}
}

void ProcessException()
{
	for ( int i = 0; i < 10; ++i)
	{
		Base* base = CreateObject(i);
		try
		{
			base->Process();
		}
		catch(const std::string& exception)
		{
			std::cout << exception.c_str() << std::endl;
			delete base;
			//	throw;
			continue;
		}
		catch(const char* exception)
		{
			std::cout << exception << std::endl;
			delete base;
			continue;
		}
		delete base;
	}
}

void ProcessAutoPtr()
{
	for ( int i = 0; i < 10; ++i)
	{
		easy::auto_ptr<Base> base = CreateObject(i);
		try
		{
			base->Process();
		}
		catch(const std::string& exception)
		{
			std::cout << exception.c_str() << std::endl;
			continue;
		}
		catch(const char* exception)
		{
			std::cout << exception << std::endl;
			continue;
		}
	}
}

void TestItem9::test()
{
	ProcessException();
	ProcessAutoPtr();
}
