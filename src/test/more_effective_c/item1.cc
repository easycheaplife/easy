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

//	����1 ����֪������ָ��һ�������Ҳ���ı���ָ��ʱ�����������ز�������Ϊ��ֹ����Ҫ���������ʱ���㲻Ӧ��ʹ��ָ�롣
//	������֮�����������£���Ӧ��ʹ��ָ�룡
void TestIteml::test() {
    //	std::string& str;	//	'str' : references must be initialized
    std::string s = "lee";
    std::string& ss = s;

    std::string* s1;	//ok
    s1 = new std::string;

    char* p = NULL;
    char& c = *p;
    //std::cout<< c << std::endl;	//	Access violation reading location 0x00000000.

    //	�����ڿ�ֵ�����ô���Ч��Ҫ��ʹ��ָ��Ҫ�ߣ�����Ҫ��ָ������Ƿ���Ч���ж�
    double d = 2.000;
    PrintfDouble1(d);

    PrintfDouble2(&d);

    //	ָ����Ա���ֵָ������һ�����󣬶�����������ָ���ʼ��ʱ�Ķ����Ժ��ܸı䡣

    std::string str1("lee");
    std::string str2("sophia");
    std::string& rs = str1;
    std::string* ps = &str1;
    rs = str2;	//��Ȼ��str1�����ã�ֵ�Ѿ��ı�
    ps = &str2;
    std::cout << "rs  = " << rs << std::endl;
    std::cout << "ps = " << *ps <<std::endl;
}
