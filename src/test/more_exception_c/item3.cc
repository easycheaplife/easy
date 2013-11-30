#include "easy_cppunit_proxy.h"
#include <vector>
//
// TestCase class
//
class TestItem3 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem3);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem3);

//
// tests implementation
//
bool greater_than1(int __i)
{
	return __i > 5;
}

template<typename _Iter,typename _Pred>
_Iter find_if(_Iter __first,_Iter __last,_Pred __pred)
{
	while(__first != __last && !__pred(*__first))
	{
		++__first;
	}
	return __first;
}

bool is_any_element_greater_than_five(std::vector<int>& __v)
{
	return find_if(__v.begin(),__v.end(),greater_than1) != __v.end();
}

class greater_than2 : public std::unary_function<int, bool>
{
public:
	bool operator() (int __i)
	{
		return __i > 5;
	}
};

bool is_any_element_greater_than_five2(std::vector<int>& __v)
{
	return find_if(__v.begin(),__v.end(),greater_than2()) != __v.end();
}

template<typename T>
class greater_than3 : public std::unary_function<T, bool>
{
public:
	greater_than3(T __val) : val_(__val) { }
	bool operator() (const T& __i)
	{
		return __i > val_;
	}
private:
	const T val_;
};

bool is_any_element_greater_than_five3(std::vector<int>& __v)
{
	return find_if(__v.begin(),__v.end(),greater_than3<int>(5)) != __v.end();
}

void TestItem3::test()
{
	std::vector<int> __v1;
	__v1.push_back(1);
	__v1.push_back(2);
	__v1.push_back(3);
	__v1.push_back(8);
	bool __res = is_any_element_greater_than_five(__v1);
	__res = is_any_element_greater_than_five2(__v1);
	__res = is_any_element_greater_than_five3(__v1);
	return;
}
