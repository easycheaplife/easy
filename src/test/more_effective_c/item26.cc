#include "easy_cppunit_proxy.h"
#include <iostream>
//
// TestCase class
//
class TestItem26 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem26);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem26);

//
// tests implementation
//

class CanBeInstantiated
{
private:
	CanBeInstantiated() {} 
	CanBeInstantiated(const CanBeInstantiated&);
};

class PrintJob;

class Printer
{
public:
	void submit_job(const PrintJob& job) { }
	void reset() { }
	void perform_self_test(){ }
	friend Printer& the_printer(); 

	//	anther way
	static Printer& the_printer2(); 
	static Printer& the_printer3()
	{
		return print3;
	}
	
private:
	static Printer print3;	//	class static data member construct function will called always.	
	Printer() { std::cout << "construct function called!"<< std::endl; }
	Printer(const Printer&);
};

Printer Printer::print3;

Printer& Printer::the_printer2()
{	
	static Printer print;//	non- class static data member  only once it be used.
	return print;
}

Printer& the_printer()
{
	static Printer print;
	return print;
}


class Printer2
{
	class too_many_object{ };
public:
	Printer2() 
	{
		if (number_obj >= 1)
		{
			throw too_many_object();
		}
		++number_obj;
	}
	~Printer2() { --number_obj; }
private:
	static size_t number_obj;
	Printer2(const Printer2& rhs);
};

size_t Printer2::number_obj = 0;


template<typename _BeingCounted>
class Counted
{
public:
	enum 
	{
		NUM_OBJ = 0,
		MAX_NUM_OBJ = 10
	};
	class too_many_obj { };
	static int obj_count() { return num_object_; }
protected:
	Counted() 
	{
		_init();
	}
	Counted(const Counted&) { _init(); }
	~Counted() { --num_object_; }
private:
	//const static int num_object_ = NUM_OBJ;
	//const static int max_num_obj_ = MAX_NUM_OBJ;

	static int num_object_;
	static int max_num_obj_ ;

	void _init()
	{
		if (num_object_ >= max_num_obj_)
		{
			throw too_many_obj();
		}
		++num_object_;
	}
};

class Printer3 : public Counted<Printer3>
{
public:
	static Printer3* make_printer() { return new Printer3();}

	static Printer3* make_printer(const Printer3& rhs) { return new Printer3(rhs);}

	void submit_job(const PrintJob& job) { }
	void reset() { }
	void perform_self_test(){ }

	using Counted<Printer3>::obj_count;
	using Counted<Printer3>::too_many_obj;
private:
	Printer3() { }
	Printer3(const Printer&) { }
};

int Counted<Printer3>::num_object_ = 0;
int Counted<Printer3>::max_num_obj_ = 10;

void TestItem26::test()
{
	//	CanBeInstantiated intance;	//error  cannot access private member declared in class 'CanBeInstantiated'

	the_printer().reset();
	the_printer().perform_self_test();
	Printer::the_printer2().reset();
	Printer::the_printer2().perform_self_test();

	Printer::the_printer3().reset();
	Printer::the_printer3().perform_self_test();

	Printer2 printer2;
	Printer2 printer22;

	Printer3::make_printer()->reset();
	Printer3::make_printer()->perform_self_test();
}
