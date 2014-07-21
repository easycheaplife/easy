#include "easy_cppunit_proxy.h"
#include "easy_threading.h"
#include "easy_allocator.h"
#include "easy_lock.h"
#include <iostream>
//
// TestCase class
//
//#define __USE_CRITICAL_SECTION
//#define __NO_THREAD
#define MAX_REPEAT_COUNT		10000000

#ifndef __WINDOWS
typedef  int 	BOOL;
typedef  long 	LONG;
#define   FALSE		0
#define   TRUE		1
#endif	//__WINDOWS

class TestLock : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestLock);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(macro_test);
	CPPUNIT_TEST(mutex_lock);
	//CPPUNIT_TEST(spin_lock);
	CPPUNIT_TEST(spin_lock_critical_section);
	CPPUNIT_TEST(spin_lock_mine);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
	void macro_test();
	void mutex_lock();
	void spin_lock();
	void spin_lock_critical_section();
	void spin_lock_mine();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLock);

//
// tests implementation
//

class widget : public easy::my_alloc
{

};

void TestLock::test()
{
	std::cout<< "lock test begin" << std::endl;
	widget* __widget = new widget();
	delete __widget;
	easy::mutex_lock	__lock_outer;
	__lock_outer.acquire_lock();
	__lock_outer.release_lock();

	easy::mutex_lock	__inner_lock;
	easy::auto_lock __lock(__inner_lock);
	easy::auto_lock __lock2(__inner_lock);	//	will be dead lock if not use __USE_CRITICAL_SECTION
}

void TestLock::macro_test()
{
#if defined __EASY_WIN_THREAD
	std::cout << "__EASY_WIN_THREAD" << std::endl;
#elif defined __EASY_PTHREAD
	std::cout << "__EASY_PTHREAD" << std::endl;
#elif defined __EASY_NO_THREAD
	std::cout << "__EASY_NO_THREAD" << std::endl;
#endif
}

void TestLock::mutex_lock()
{
	unsigned long	__l1 = 10;
	unsigned long	__l2 = 20;
	unsigned long	__l3 = easy::_Atomic_swap(&__l1,__l2);
	std::cout << "__l3 " << __l3 << std::endl;
	easy::mutex_lock  __lock;
	__lock.acquire_lock();
	__lock.release_lock();
	__lock.acquire_lock();
	__lock.release_lock();
}

BOOL __resource_in_use = FALSE;
LONG __count = 0;
easy::mutex_lock	__lock1;
void fun_1()
{
#if 1
	//	TestAllocator::spin_lock 4.186851 msec
	//	TestAllocator::spin_lock 4.776427 msec
	//	TestAllocator::spin_lock 5.282202 msec
	while(easy::_Atomic_swap((unsigned long*)&__resource_in_use,TRUE) == TRUE)
	{
		//	use the resource
		//	...
		++__count;
		easy::_Atomic_swap((unsigned long*)&__resource_in_use,FALSE);
	}
#else
	//	TestAllocator::spin_lock 5.338761 msec
	//	TestAllocator::spin_lock 5.583356 msec
	//	TestAllocator::spin_lock 5.369261 msec

	//	TestAllocator::spin_lock 4.526205 msec(__USE_CRITICAL_SECTION-InitializeCriticalSection)
	//	TestAllocator::spin_lock 4.445364 msec(__USE_CRITICAL_SECTION-InitializeCriticalSection)
	//	TestAllocator::spin_lock 4.458098 msec(__USE_CRITICAL_SECTION-InitializeCriticalSection)
	
	//	TestAllocator::spin_lock 4.221201 msec(__USE_CRITICAL_SECTION-InitializeCriticalSectionAndSpinCount)	0x000004000
	//	TestAllocator::spin_lock 4.360378 msec(__USE_CRITICAL_SECTION-InitializeCriticalSectionAndSpinCount)	0x000004000
	//	TestAllocator::spin_lock 4.587798 msec(__USE_CRITICAL_SECTION-InitializeCriticalSectionAndSpinCount)	0x000004000
	__lock1.acquire_lock();
	++__count;
	__lock1.release_lock();
#endif
}

easy::mutex_lock	__lock2;
void fun_2()
{
#if 1
	while(easy::_Atomic_swap((unsigned long*)&__resource_in_use,TRUE) == TRUE)
	{
		//	use the resource
		//	...
		++__count;
		easy::_Atomic_swap((unsigned long*)&__resource_in_use,FALSE);
	}
#else
	__lock2.acquire_lock();
	++__count;
	__lock2.release_lock();
#endif
}

class test_thread1 : public easy::EasyThread
{
public:
	test_thread1(){}
	~test_thread1() {}
	easy_int32 _Run( void* p )
	{
		
		for(;;)
		{
			fun_1();
		}
		
		return 0;
	}
};

class test_thread2 : public easy::EasyThread
{
public:
	test_thread2(){}
	~test_thread2() {}
	easy_int32 _Run( void* p )
	{
		for(;;)
		{
			fun_2();
		}
		return 0;
	}
};
void TestLock::spin_lock()
{
	test_thread1*	__thread1 = new test_thread1();
	test_thread2*	__thread2 = new test_thread2();
	for(;;)
	{
		if (__count > 10000)
		{
			break;
		}
#ifdef __WINDOWS
			::Sleep(10);
#elif defined __LINUX
		//	sleep(0.001);
#endif
	}
	delete __thread1;
	delete __thread2;
}

//	ifdef __USE_CRITICAL_SECTION
//	TestAllocator::spin_lock_critical_section 1103.981906 msec
//	TestAllocator::spin_lock_mine 1170.406227 msec

//	ifndef __USE_CRITICAL_SECTION
//	TestAllocator::spin_lock_critical_section 1482.122155 msec
//	TestAllocator::spin_lock_mine 1600.672016 msec
void TestLock::spin_lock_critical_section()
{
	easy::mutex_lock	__lock;
	for (int __i = 0; __i < MAX_REPEAT_COUNT; ++__i)
	{
		__lock.acquire_lock();
		__lock.release_lock();
	}
}

void TestLock::spin_lock_mine()
{
//#undef __USE_CRITICAL_SECTION
	easy::mutex_lock	__lock;
	for (int __i = 0; __i < MAX_REPEAT_COUNT; ++__i)
	{
		__lock.acquire_lock();
		__lock.release_lock();
	}
}
