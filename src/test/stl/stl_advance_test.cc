#include "easy_cppunit_proxy.h"
//
// TestCase class
//
#include <iostream>
#include <string>
#include <list>
using namespace std ;

typedef list<string> STRLIST;


class stl_advance_test : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(stl_advance_test);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(stl_advance_test);

//
// tests implementation
//

void stl_advance_test::test()
{
	STRLIST List;
    STRLIST::iterator iList;
    STRLIST::difference_type dTheDiff;

    List.push_back("A1");
    List.push_back("B2");
    List.push_back("C3");
    List.push_back("D4");
    List.push_back("E5");
    List.push_back("F6");
    List.push_back("G7");

    // Print out the list
    iList=List.begin();
    cout << "The list is: ";
    for (int i = 0; i < 7 ; i++, iList++)
        cout << *iList  << "  ";

    // Initialize to the first element"
    iList=List.begin();
    cout << "\n\nAdvance to the 3rd element." << endl;
    advance(iList,2);
    cout << "The element is " << *iList << endl;
    dTheDiff = distance( List.begin(), iList);
	if(dTheDiff){}	//warning: variable ‘dTheDiff’ set but not used [-Wunused-but-set-variable]
}
