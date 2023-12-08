#include "easy_cppunit_proxy.h"
#include "easy_ref_count_big_int.h"
#include <stdio.h>

class TestBigInt : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestBigInt);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(BigIntCreate);
    CPPUNIT_TEST(RefCountBigIntCreate);

    CPPUNIT_TEST(BigIntAssign);
    CPPUNIT_TEST(RefCountBigIntAssign);

    CPPUNIT_TEST(CommonTest);

    CPPUNIT_TEST_SUITE_END();

  protected:
    void BigIntCreate();
    void RefCountBigIntCreate();

    void BigIntAssign();
    void RefCountBigIntAssign();

    void CommonTest();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestBigInt);

void TestBigInt::BigIntCreate() {
    printf("BigIntCreate\n");
    for (int i = 0; i < UNIT_TEST_RECORDS; ++i) {
        easy::EasyBigInt a = i;
        easy::EasyBigInt b = i + 1;
        easy::EasyBigInt c = i + 1;
        easy::EasyBigInt d = i + 1;
    }
}

void TestBigInt::RefCountBigIntCreate() {
    printf("RefCountBigIntCreate\n");
    for (int i = 0; i < UNIT_TEST_RECORDS; ++i) {
        easy::EasyRefCountBigInt a = i;
        easy::EasyRefCountBigInt b = i + 1;
        easy::EasyRefCountBigInt c = i + 1;
        easy::EasyRefCountBigInt d = i + 1;
    }
}

void TestBigInt::BigIntAssign() {
    printf("BigIntAssign\n");
    easy::EasyBigInt a, b, c;
    easy::EasyBigInt d = 1;
    for (int i = 0; i < UNIT_TEST_RECORDS; ++i) {
        a = b = c = d;
    }
}

void TestBigInt::RefCountBigIntAssign() {
    printf("RefCountBigIntAssign\n");
    easy::EasyRefCountBigInt a, b, c;
    easy::EasyRefCountBigInt d = 1;
    for (int i = 0; i < UNIT_TEST_RECORDS; ++i) {
        a = b = c = d;
    }
}

void TestBigInt::CommonTest() {
    easy::EasyRefCountBigInt a = 100;
    easy::EasyRefCountBigInt b = "99";
    easy::EasyRefCountBigInt c(b);
    easy::EasyRefCountBigInt d = a + b;
    easy::EasyRefCountBigInt e = d + 456;
}






