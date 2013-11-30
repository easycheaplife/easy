#include "easy_cppunit_proxy.h"
#include <iostream>
//
// TestCase class
//
class TestItem27 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem27);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem27);

//
// tests implementation
//

class UPNumber
{
public:
	UPNumber() { }
	UPNumber(int val) { }
	UPNumber(double val) { }
	void destroy() const { delete this; }
private:
	~UPNumber() { }
};

class UPNumber1
{
public:
	UPNumber1() { }
protected:
	~UPNumber1() { }
private:
	/*
	~UPNumber1() { }
	UPNumber1() { }
	*/
};

class NonNegativeUPNumber : public UPNumber1	//if UPNumber1 ' construct and desconstruct function is private,'NonNegativeUPNumber' : destructor could not be generated because a base class destructor is inaccessible
{

};

class Assert
{
	//	UPNumber1	upnumber;	//if UPNumber1 ' construct and desconstruct function is private, 'Assert' : destructor could not be generated because a base class destructor is inaccessible
	UPNumber1*	upnumber;
};

bool on_heap(const void* address)
{
	char on_the_stack;
	return address < &on_the_stack;
}

#include <list>
class HeapTracked
{
public:
	class MissingAddress { };
	virtual ~HeapTracked() = 0;
	static void* operator new(size_t size) ;
	static void operator delete(void* p);
	bool is_on_heap() const;
private:
	typedef const void* raw_address;
	static std::list<raw_address> address;
};

std::list<HeapTracked::raw_address> HeapTracked::address;

HeapTracked::~HeapTracked() { }

void* HeapTracked::operator new(size_t size)
{
	void* p = ::operator new(size);
	address.push_front(p);
	return p;
}
#include <algorithm>
void HeapTracked::operator delete(void* p)
{
	std::list<raw_address>::iterator it = find(address.begin(),address.end(),p);
	if (it != address.end())
	{
		address.erase(it);
		::operator delete(p);
	}
	else
	{
		throw MissingAddress();
	}
}

bool HeapTracked::is_on_heap() const
{
	const void* _raw_address = dynamic_cast<const void*>(this);	//把一个指针dynamic_cast 成void* (const void*, volatile void*)生成的指针将指向“原指针指向对象内存”的开始处。而且只能用于“指向至少具有一个虚拟函数的对象”的指针上
	std::list<raw_address>::iterator it = find(address.begin(),address.end(),_raw_address);
	return it != address.end();
}

void TestItem27::test()
{
	//UPNumber	upnumber;	//'UPNumber::~UPNumber' : cannot access private member declared in class 'UPNumber'
	UPNumber*	upnumber = new UPNumber();

	//delete upnumber;		//UPNumber::~UPNumber' : cannot access private member declared in class 'UPNumber'
	upnumber->destroy();

	char c = 'c';
	char* p = new char[10];
	bool is_on_heap = on_heap(&c);
	is_on_heap = on_heap(p);
}
