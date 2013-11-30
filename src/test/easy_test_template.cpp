#include "easy_cppunit_proxy.h"
#include <iostream>
//
// TestCase class
//
class TestTemplate : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestNull);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(TemplateSpecialization);
	CPPUNIT_TEST(explicit_specialization);
	CPPUNIT_TEST(partial_specialization);
	CPPUNIT_TEST(partial_order);
	CPPUNIT_TEST(param_int);
	CPPUNIT_TEST_SUITE_END();

protected:
	void TemplateSpecialization();
	void explicit_specialization();
	void partial_specialization();
	void partial_order();
	void param_int();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestTemplate);

//////////////////////////////////////////////////////////////////////////
/*
类模板可以偏特化或者显式特化，而函数模板只能显示特化（全特化）
可以这么理解么？偏特化只是针对类型特化，如 T特化为T*,const T*,等等；而全特化则具体到类型，如int,int*,const int*
*/
template<class T>	//	a
class X
{
private:
	int a_;
};

template<class T> //	b
void f(T)
{
	 
}

template<class T>	//	c
void f(int,T,double)
{

}

template<class T>	// 	对a的针对指针类型的偏特化
class X <T*>
{

};

template<>			// 	对a的针对int类型的偏特化
class X <int>
{

};

template<class T>	//	d	重载b,c；函数模板不能偏特化
void f(T*) {} 

template<>			//	e  关于int的显示特化
void f(int) {} 

void f(double)		//	f	
{

}

void TestTemplate::TemplateSpecialization()
{
	bool b = false;
	int i = 0;
	double d = 0.0f;
	f(b);		//	b
	f(i,42,d);	//	c
	f(&i);		//	d
	f(i);		//	e
	f(d);		//	f
}

#include <iostream>
template<typename _Key>
struct hash
{
	void operator()() { std::cout << "hash<T>" << std::endl;}
};

//	explicit specialization
template<>
struct hash<char>
{
	void operator()() { std::cout << "hash<char>" << std::endl;}
};

template<>
struct hash<unsigned char>
{
	void operator()() { std::cout << "hash<unsigned char>" << std::endl;}
};

void TestTemplate::explicit_specialization()
{
	hash<int> __h1;
	hash<char> __h2;
	hash<unsigned char> __h3;

	__h1();
	__h2();
	__h3();

}

template<typename I,typename O>
class test_class
{
public:
	test_class() { std::cout << "test_class I, O" << std::endl; }
};

template<typename T>
class test_class<T*,T*>
{
public:
	test_class() { std::cout << "test_class *T,*T" << std::endl; }
};

template<typename T>
class test_class<const T*,T*>
{
public:
	test_class() { std::cout << "test_class const *T,*T" << std::endl; }
};

void TestTemplate::partial_specialization()
{
	test_class<int,char>	__test1;
	test_class<int*,int*>	__test2;
	test_class<const int*,int*>	__test3;
}

template <class T> void f1(T) 
{
	std::cout << "Less specialized function called" << std::endl;
}

template <class T> void f1(T*) 
{
	std::cout << "More specialized function called" << std::endl;
}

template <class T> void f1(const T*) 
{
	std::cout << "Even more specialized function for const T*" << std::endl;
}

void TestTemplate::partial_order()
{
	int i = 0;
	const int j = 0;
	int *pi = &i;
	const int *cpi = &j;
	f1(i);
	f1(pi);
	f1(cpi);
}

template<typename T, int _Max>
void Param_Int(T _Type)
{
	std::cout << "Param_Int " << _Max << std::endl;
}

void TestTemplate::param_int()
{
	Param_Int<int,4>(3);
}
