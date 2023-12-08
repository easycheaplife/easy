#include "easy_cppunit_proxy.h"
#include "easy_stdandard_allocator.h"
//
// TestCase class
//
class TestStandardAllocactor : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestStandardAllocactor);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestStandardAllocactor);

//
// tests implementation
//

#include <iostream>

class Widget : public easy::easy_stdand_allocator {
  public:
    using easy_stdand_allocator::operator new;
    using easy_stdand_allocator::operator delete;

    static void* operator new (std::size_t size, std::ostream& log_stream) throw(std::bad_alloc) {
        return NULL;
    }

    static void operator delete (void* memory, std::ostream& log_stream) throw(std::bad_alloc) { }

    ~Widget() { }
};

void TestStandardAllocactor::test() {
    std::cout<< "TestStandardAllactor" <<std::endl;
    Widget* widget1 = new Widget();
    Widget* widget2 = new Widget();
    new (widget1) Widget(*widget2);

    widget1->~Widget();

    delete widget1;
    delete widget2;
}
