#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem6 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem6);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem6);

//
// tests implementation
//
class UPInt {
  public:
    //	UPInt& operator++ () { ++(*this); return *this;}	// warning C4717: 'UPInt::operator++' : recursive on all control paths, function will cause runtime stack overflow

    UPInt& operator++ () {
        (*this) += 1;
        return *this;
    }

    const UPInt operator++(int) {
        UPInt old = *this;
        ++(*this);
        return old;
    }

    //	UPInt& operator-- () { --(*this); return *this; }	// warning C4717: 'UPInt::operator++' : recursive on all control paths, function will cause runtime stack overflow

    UPInt& operator-- () {
        (*this) -= 1;
        return *this;
    }

    const UPInt operator-- (int) {
        UPInt old = *this;
        --(*this);
        return old;
    }

    UPInt& operator += (int) {
        //	to be continue
        return *this;
    }

    UPInt& operator -= (int) {
        //	to be continue
        return *this;
    }
};

void TestItem6::test() {
    UPInt i;
    ++i;
    i++;
    --i;
    i--;

    //	i++++;	//error C2678: binary '++' : no operator found which takes a left-hand operand of type 'const UPInt' (or there is no acceptable conversion)
}
