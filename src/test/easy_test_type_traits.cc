#include "easy_cppunit_proxy.h"
#include "easy_type_traits.h"
//
// TestCase class
//
class TestTypeTraits : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestTypeTraits);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestTypeTraits);

//
// tests implementation
//

//Copy an array of elements which have non-trivial copy constructors
template <class T> void copy(T* source, T* destination, int n, __false_type) 
{
	while (n)
	{
		*source = *destination;
		++source;
		++destination;
		--n;
	}
}
//Copy an array of elements which have trivial copy constructors. Use memcpy.
template <class T> void copy(T* source, T* destination, int n, __true_type)
{
	while (n)
	{
		*source = *destination;
		++source;
		++destination;
		--n;
	}
}

//Copy an array of any type by using the most efficient copy mechanism
template <class T> inline void copy(T* source,T* destination,int n) 
{
	copy(source, destination, n,
		typename __type_traits<T>::has_trivial_copy_constructor());
}

void TestTypeTraits::test()
{
	int arr[5];
	int arr2[5] = { 1,2,3,4,5};
	copy<int>(arr,arr2,5);
}
