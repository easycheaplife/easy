#include "easy_cppunit_proxy.h"
#include <iostream>
#include <boost/pool/pool.hpp>
//
// TestCase class
//
class TestBoostPool : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestBoostPool);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestBoostPool);

//
// tests implementation
//
using namespace std;
using namespace boost;

class Test
{
public:
	Test() { }
	~Test() { }
};

void TestBoostPool::test()
{
	const int block_size = sizeof(int);
	boost::pool<> alloc(block_size);
	for (int i = 0; i < block_size; ++i)
	{
		int* p = (int*)alloc.malloc();
		cout << *p << endl;
		alloc.free(p);
	}
	
}
