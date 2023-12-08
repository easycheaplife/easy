
#include "easy_cppunit_proxy.h"
#include<stdio.h>
//
// TestCase class
//
class Testmemmove : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(Testmemmove);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Testmemmove);

//
// tests implementation
//
char __str[7] = "aabbcc";
void Testmemmove::test() {
    printf("the string is %s\n",__str);
    //	use memcpy
    memcpy(__str+2,__str,4);
    printf("the new string is %s\n",__str);

    strcpy(__str,"aabbcc");

    printf("the new string is %s\n",__str);
    //	use memmove
    memmove(__str+2,__str,4);
    printf("the new string is %s\n",__str);
}
