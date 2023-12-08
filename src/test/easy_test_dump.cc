#include "easy_cppunit_proxy.h"
#include "easy_dump.h"

//	reference from http://blog.csdn.net/b_fushuaibing/article/details/5560304
//	to show function by address: addr2line -e easy_main -f 0x4018d6

//
// TestCase class
//
class TestDump : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestDump);
#if 0
    CPPUNIT_IGNORE;
#endif
    //CPPUNIT_TEST(test);
    //CPPUNIT_TEST(test_signal_segv);
    //CPPUNIT_TEST(test_signal_segv2);
    //CPPUNIT_TEST(test_signal_abrt);
    CPPUNIT_TEST(test_dump_for_gdb);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
    void test_signal_segv();
    void test_signal_segv2();
    void test_signal_abrt();
    void test_dump_for_gdb();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDump);

//
// tests implementation
//

static void /* "static" means don't export the symbol... */ myfunc2(void) {
    dump(0);
}

void myfunc(int __call_times) {
    if (__call_times > 1)
        myfunc(__call_times - 1);
    else
        myfunc2();
}

void register_signal(int __signal) {
    signal(__signal, dump);
}

void register_signal_for_gdb(int __signal) {
    signal(__signal, dump_for_gdb);
}

void segv_fun() {
    unsigned char* __ptr = 0x00;
    *__ptr = 0x00;
}

struct sth {
    int val_;
};
void segv_fun2() {
    sth* __sth = NULL;
    __sth->val_ = 1;
}

void abrt_fun() {
    abort();
}

void TestDump::test() {
    printf("TestDump::test---\n");
    myfunc(3);
}

void TestDump::test_signal_segv() {
    printf("TestDump::test_signal SIGSEGV\n");
    register_signal(SIGSEGV);
    segv_fun();
}

void TestDump::test_signal_segv2() {
    printf("TestDump::test_signal, maybe a core file generate\n");
    segv_fun();
}

void TestDump::test_signal_abrt() {
    printf("TestDump::test_signal SIGABRT\n");
    register_signal(SIGABRT);
    abrt_fun();
}

void TestDump::test_dump_for_gdb() {
    printf("TestDump::test_dump_for_gdb SIGSEGV\n");
    register_signal_for_gdb(SIGSEGV);
    segv_fun2();
}


