#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestIteml : public CPPUNIT_NS::TestCase {
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
#include<string>
#include <iostream>

void PrintfDouble1(const double& d) {
    std::cout<<d<<std::endl;
}

void PrintfDouble2(double* d) {
    if (d) {
        std::cout<<*d<<std::endl;
    }
}

//	条款1 当你知道必须指向一个对象并且不想改变其指向时，或者在重载操作符并为防止不必要的语义误解时，你不应该使用指针。
//	而除此之外的其他情况下，则应该使用指针！
void TestIteml::test() {
    //	std::string& str;	//	'str' : references must be initialized
    std::string s = "lee";
    std::string& ss = s;

    std::string* s1;	//ok
    s1 = new std::string;

    char* p = NULL;
    char& c = *p;
    //std::cout<< c << std::endl;	//	Access violation reading location 0x00000000.

    //	不存在空值的引用代码效率要比使用指针要高，不需要对指针进行是否有效的判断
    double d = 2.000;
    PrintfDouble1(d);

    PrintfDouble2(&d);

    //	指针可以被赋值指向另外一个对象，而引用则总是指向初始化时的对象，以后不能改变。

    std::string str1("lee");
    std::string str2("sophia");
    std::string& rs = str1;
    std::string* ps = &str1;
    rs = str2;	//仍然是str1的引用，值已经改变
    ps = &str2;
    std::cout << "rs  = " << rs << std::endl;
    std::cout << "ps = " << *ps <<std::endl;
}
