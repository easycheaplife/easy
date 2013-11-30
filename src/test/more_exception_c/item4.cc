#include "easy_cppunit_proxy.h"
#include <vector>
//
// TestCase class
//
class TestItem4 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem4);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem4);

//
// tests implementation
//
class Test
{
public:
	Test* clone () const { return new Test(); }
	void test() {}
};

template<typename T>
class C1
{
public:
	void some_fun()
	{
		this->clone();
	}
};

template<typename T>
class C2
{
public:
	~C2() 
	{
		T* (T::test)() const = &T::clone;
		test;
	}
};

class Cloneable{ };

template<typename D,typename B>
class IsDeriveFrom
{
	class No{};
	class Yes{ No no_[2]; };
	static Yes Test(B*);
	static No Test(...);
	
	static void Constraints(D* __p) { B* __pb =__p; __pb =__p;}
public:
	enum { Is = sizeof( Test(static_cast<D*>(0)) ) == sizeof(Yes) } ;
	//	IsDeriveFrom( void (*p)(D*) == Constraints; )
};

template<typename T,int>
class XImpl
{
public:
	XImpl() { std::cout << "XImpl" << std::endl; }
};

template<typename T>
class XImpl<T,1>
{
public:
	XImpl() { std::cout << "XImpl<T,1>" << std::endl; }
};

template<typename T>
class XImpl<T,0>
{
public:
	XImpl() { std::cout << "XImpl<T,0>" << std::endl; }
};

template<typename T>
class X
{
	XImpl<T,IsDeriveFrom<T,Cloneable>::Is>  impl_;
};

class MyCloneable : public Cloneable {};

#include<iostream>
void TestItem4::test()
{
	C1<int>	__c1;	
	//	__c1.some_fun();	//	error: ¡®class C1<int>¡¯ has no member named ¡®clone¡¯
	
	//C2<Test>	__c2;
	
	std::cout << "to test gdb, set break point at filename:linenum" << std::endl;
	int __i = 100;
	__i = 200;
	std::cout << std::endl << __i << std::endl;
	
	//	test trait
	X<MyCloneable> __x1;
	X<int> __x2;
	return ;
}
