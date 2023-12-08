#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem2l : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem2l);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem2l);

//
// tests implementation
//
#include <iostream>
#include <complex>
class base {
  public:
    //virtual ~base() { }

    virtual void f(int ) {
        std::cout << "base::f(int )" <<std::endl;
    }

#if 0
    virtual void f(float ) {
        std::cout << "base::f(float )" <<std::endl;
    }
#endif

    virtual void g(int __i = 10) {
        std::cout << "base::g" << __i <<std::endl;
    }
};

class derived : public base {
  public:
    void f(std::complex<double>) {
        std::cout << "derived::f(std::complex<double>)" <<std::endl;
    }

    void g(int __i  = 20 ) {
        std::cout << "derived::g" << __i <<std::endl;
    }
};

void TestItem2l::test() {
    base __b;
    derived __d;
    base* __pb = new derived();
    __b.f(1.0);
    __d.f(1.0);
    __pb->f(1.0);

    __b.g(1.0);
    __d.g(1.0);
    __pb->g(1.0);

    //	some error here if virtual ~base() not defined
    delete __pb;

    //	c++ code analysis
    char __src[2];
    char __des[] = "lee";
    //	warning C6202: Buffer overrun for '__src', which is possibly stack allocated, in call to 'strcpy': length '4' exceeds buffer size '2'
    strcpy(__src,__des);
}
