#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestIteml : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestIteml);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestIteml);

//
// tests implementation
//
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
using namespace std;

void TestIteml::test()
{
	vector<int>	e;
	copy( istream_iterator<int>(std::cin),istream_iterator<int>(),back_inserter(e));
	vector<int>::iterator first = find(e.begin(),e.end(),1);
	vector<int>::iterator last = find(e.begin(),e.end(),2);		
	*last = 10;			// last may be invalid if can not find the value of 2
	copy(first,last,ostream_iterator<int>(cout,"\n"));	// maybe between first and last is incorrect,first may be at the last of last!
	e.insert(--e.end(),999); // e.end() maybe incorrect!
	copy(first,last,ostream_iterator<int>(cout,"\n"));	
}
