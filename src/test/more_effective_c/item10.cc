#include "easy_cppunit_proxy.h"
#include "easy_memory.h"
//
// TestCase class
//
class TestItem10 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem10);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem10);

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

class Container
{
public:
	Container()
	{
		derive_ = new Derive1();
	}

	~Container()
	{

	}

	void Print() { derive_->Process();}
private:
	easy::auto_ptr<Derive1>	derive_;
};

class Container2
{
public:
	Container2()
	{
		try
		{
			derive_ = new Derive2();
		}
		catch(...)
		{
			_Clear();
			throw;
		}
	}

	~Container2()
	{

	}

	void Print() { derive_->Process();}

	void _Clear() { delete derive_; }
private:
	Derive2*	derive_;
};

void TestItem10::test()
{
	Container container1;
	try
	{
		container1.Print();
	}
	catch(...)
	{
		std::cout << "container1.Print();" << std::endl;
	}

	Container2 container2;
	try
	{
		container2.Print();
	}
	catch(...)
	{
		std::cout << "container2.Print();" << std::endl;
		container2._Clear();
		throw;
	}
	container2._Clear();
}
