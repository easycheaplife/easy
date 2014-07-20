#include "easy_cppunit_proxy.h"
#include "easy_allocator.h"
#include "easy_log.h"
#include <iostream>
/*
#ifdef __WINDOWS
#include <Windows.h>
#include <Winnt.h>
#endif
*/

//
// TestCase class
//
class TestAllocator : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestAllocator);
#if 0
	CPPUNIT_IGNORE;
#endif
	
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(malloc_alloc_template);
	CPPUNIT_TEST(simple_alloc);
	CPPUNIT_TEST(debug_alloc);
	CPPUNIT_TEST_SUITE_END();

protected:
	
	void test();
	void malloc_alloc_template();
	void simple_alloc();
	void debug_alloc();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAllocator);

//
// tests implementation
//

void TestAllocator::test()
{
#if 0
	int* __p = 0;
#else
	int* __p = new int;
#endif
	__easy_assert(__p);
	delete __p;
}

void out_of_memory()
{
	std::cout << "out of memory" << std::endl;
	exit(1);
}
void TestAllocator::malloc_alloc_template()
{
	typedef easy::malloc_alloc __alloc;
	__alloc::__set_malloc_handler(out_of_memory);
	void* __p = __alloc::allocate(128);
	__alloc::deallocate(__p,128);
}

void TestAllocator::simple_alloc()
{
	typedef easy::simple_alloc<int,easy::malloc_alloc> __alloc;
	int* __p = __alloc::allocate();
	__alloc::deallocate(__p);
}

void TestAllocator::debug_alloc()
{
	typedef easy::debug_alloc<easy::malloc_alloc> __alloc;
	__alloc::allocate(128);
}
