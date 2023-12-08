#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem2 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem2);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem2);

//
// tests implementation
//

//	for c++ convert style: static_cast,const_cast,dynamic_cast,reinterpret_cast
class Widget {

};

class Widget2 {
  public:
    virtual ~Widget2() {}
};

class SpecialWidget : public Widget {

};

class SpecialWidget2 : public Widget2 {

};

void Update(SpecialWidget* special_widget) {

}

void Update2(SpecialWidget2* special_widget) {

}

void UpdateRef2(SpecialWidget2& special_widget) {

}

typedef void (*FunPtr)();

int DoSomething() {
    return 0;
}

void TestItem2::test() {
    //	static_cast
    int frist_num = 1;
    int sec_num = 2;
    double res = static_cast<double>(frist_num/sec_num);

    //	const_cast
    SpecialWidget sw;
    const SpecialWidget& csw = sw;
    Update(&sw);	//ok
    //Update(csw);	//	'Update' : cannot convert parameter 1 from 'const SpecialWidget' to 'SpecialWidget *'
    Update(const_cast<SpecialWidget*>(&csw));	//ok now

    //	dynamic_cast
    Widget* pw = new SpecialWidget();
    //	Update(const_cast<SpecialWidget*>(pw));	// 'const_cast' : cannot convert from 'Widget *' to 'SpecialWidget *',
    //	Update(dynamic_cast<SpecialWidget*>(pw)); // You cannot use dynamic_cast to convert from a non-polymorphic class (a class with no virtual functions).
    Update(static_cast<SpecialWidget*>(pw));
    try {
        Widget2* pw2 = new SpecialWidget2();
        Update2(dynamic_cast<SpecialWidget2*>(pw2));	// ok

        Widget2 w;
        UpdateRef2(dynamic_cast<SpecialWidget2&>(w));
    } catch (...) {

    }

    //	reintpret_cast
    FunPtr fun_ptr_arr[10];
    // fun_ptr_arr[0] = DoSomething; //error C2440: '=' : cannot convert from 'int (__cdecl *)(void)' to 'FunPtr'
    fun_ptr_arr[0] = reinterpret_cast<FunPtr>(DoSomething);
}

#if 0
#define static_cast(TYPE,EXPR)
#define const_cast(TYPE,EXPR)
#define reinterpret_cast(TYPE,EXPR)
#endif

