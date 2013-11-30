#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem42 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem42);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem42);

//
// tests implementation
//
#include <iostream>
#include <vector>

template<typename C>
void Ptintf2nd(const C& container)
{
	if (container.size() >= 2)
	{
#if 0
		C::const_iterator iter(container.begin());		//	ok, why?	
#else
		typename C::const_iterator iter(container.begin());		
#endif
	 
		++iter;
		int val = *iter;
		std::cout << val;
	}
}

template<typename C>
void f(const C& container, typename C::iterator iter)
{

}

template<typename T>
class Base
{
public:
	Base() { }
	~Base() { }
};

template<typename T>
class Derived : public Base<T>::Nested
{
public:
	explicit Derived(int x)
		:Base<T>::Nested(x)
	{
		typename Base<T>::Nested temp;
	}
};

template<typename iterT>
void WorkWithIterator(iterT iter)
{
#if 1
	typename std::iterator_traits<iterT>::value_type value_type(*iter);
#else
	typedef typename std::iterator_traits<iterT>::value_type value_type;
	value_type temp(*iter);
#endif
}

void TestItem42::test()
{
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	Ptintf2nd<std::vector<int>>(v);

	f<std::vector<int>>(v,v.begin());

	WorkWithIterator(v.begin());
}
