#include "easy_cppunit_proxy.h"
#include "easy_memory.h"
#include <new>

class MemoryTest : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(MemoryTest);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(TestAutoPtr);
	CPPUNIT_TEST(TestPlacementNew);
	CPPUNIT_TEST(TestDefaultAllocTemplate);
	CPPUNIT_TEST(test__malloc_alloc_template);
	CPPUNIT_TEST(test__simple_alloc);
	CPPUNIT_TEST(test_debug_alloc);
	
	CPPUNIT_TEST_SUITE_END();

protected:
	void TestAutoPtr();
	void TestPlacementNew();
	void TestDefaultAllocTemplate();
	void test__malloc_alloc_template();
	void test__simple_alloc();
	void test_debug_alloc();
};

CPPUNIT_TEST_SUITE_REGISTRATION(MemoryTest);

class A
{
protected:
	int a_;
public:
	A( int a)
		:a_(a)
	{

	}
	A() { }

	~A() { }

	void set_a(int value) { a_ = value; } 

	int a( ) { return a_; }
};

class B 
{

};
#if 0
void TestConstruct()
{
	A a(10);
	B b;
	A* p;
	easy::_Construct(&a, a);
	easy::_Construct(&b);
}
#endif


//	reference http://www.programfan.com/club/showtxt.asp?id=320940
void MemoryTest::TestPlacementNew()
{
	char* buff = new char[sizeof(A)];
	A* p = new (buff) A;
	p->set_a(100);
	int res = p->a();
	p->~A();
	delete [] buff;

	A* pa = new A(100);
	pa->~A();
	delete pa;	
}

struct __false_type 
{
};

struct __true_type
{

};

//Copy an array of elements which have non-trivial copy constructors
template <class T> void copy(T* source, T* destination, int n,  __false_type);

//Copy an array of any type by using the most efficient copy mechanism
template <class T> inline void copy(T* source,T* destination,int n) 
{
	copy(source, destination, n,
		typename __type_traits<T>::has_trivial_copy_constructor());
}

#include <iostream>
#include <stdio.h>
#include <new.h>


int __cdecl out_of_memory(size_t )
{
	std::cout<<"out of memory£¡"<<std::endl;
	exit(0);
}

int handle_program_memory_depletion( size_t )
{
	// Your code
	return 0;
}

#define BIG_NUMBER 0x1fffffff
#define SMALL_NUMBER 127
void TestSetNewHandle()
{
	::_set_new_handler(out_of_memory);
	_set_new_handler( handle_program_memory_depletion );
#if 0
	int *pi = new int[BIG_NUMBER];
#endif
}

void MemoryTest::TestDefaultAllocTemplate()
{
	void* p = easy::memory_pool::allocate( SMALL_NUMBER );
	p = easy::memory_pool::allocate( BIG_NUMBER );
	easy::memory_pool::deallocate(p,SMALL_NUMBER);
	for ( int i = 0; i < SMALL_NUMBER; ++i )
	{
		easy::memory_pool::allocate( SMALL_NUMBER );
	}
}

#include <iostream>
#include <string>

easy::auto_ptr<int> CreateAutoPtr(int val)
{ 
	return easy::auto_ptr<int>(new int(val)); 
}

bool CheckEquality(easy::auto_ptr<int> pint, int val)
{ 
	return *pint == val; 
}

void MemoryTest::TestAutoPtr()
{
	/*
	 explicit auto_ptr(_Tp* __p = 0)  : _M_ptr(__p) {}
	 */
	easy::auto_ptr<std::string> ps( new std::string("Lee"));
	std::cout<< *ps<<std::endl;
	std::cout<< ps->size()<<std::endl;
	//	do not need delete ps, ps will release automatic

	/*
	auto_ptr(auto_ptr& __a)  : _M_ptr(__a.release()) {}
	*/
	easy::auto_ptr<std::string> ps1(ps);
	easy::auto_ptr<std::string> ps2 = ps1;
	easy::auto_ptr<std::string> ps3;
	ps3 = ps2;

	/*
	template <class _Tp1> auto_ptr(auto_ptr<_Tp1>& __a) 
	*/
	easy::auto_ptr<std::string> ps_extra( easy::auto_ptr<std::string> );

	/*
	auto_ptr& operator=(auto_ptr& __a)  
	*/
	easy::auto_ptr<std::string> ps4( new std::string("sophia"));
	ps = ps4;

	std::string* str =  ps.get();

	//	reference stlport unit test
	{
		easy::auto_ptr<int> pint(new int(1));
		CPPUNIT_ASSERT( *pint == 1 );
		*pint = 2;
		CPPUNIT_ASSERT( *pint == 2 );
	}

	{
		 easy::auto_ptr<int> pint(CreateAutoPtr(3));
		 CPPUNIT_ASSERT( *pint == 3 );
		 CPPUNIT_ASSERT( CheckEquality(pint, 3) );
	}

	{
		easy::auto_ptr<const int> pint(new int(2));
		 CPPUNIT_ASSERT( *pint == 2 );
	}

	{
		easy::auto_ptr<volatile int> pint(new int(2));
		CPPUNIT_ASSERT( *pint == 2 );
	}

	{
		easy::auto_ptr<const volatile int> pint(new int(2));
		CPPUNIT_ASSERT( *pint == 2 );
	}
}

void MemoryTest::test__malloc_alloc_template()
{
	void* p = easy::malloc_alloc::allocate(SMALL_NUMBER);
	p = easy::malloc_alloc::reallocate(p,SMALL_NUMBER,SMALL_NUMBER*2);
	easy::malloc_alloc::deallocate(p,SMALL_NUMBER*2);
}

void MemoryTest::test__simple_alloc()
{
	int* p1 = easy::simple_alloc<int,easy::malloc_alloc>::allocate(SMALL_NUMBER);
	easy::simple_alloc<int,easy::malloc_alloc>::deallocate(p1,SMALL_NUMBER);

	int* p2 = easy::simple_alloc<int,easy::malloc_alloc>::allocate();
	easy::simple_alloc<int,easy::malloc_alloc>::deallocate(p2);
}

void MemoryTest::test_debug_alloc()
{
#if 1
	void* p = easy::debug_alloc<easy::malloc_alloc>::allocate(SMALL_NUMBER);
	easy::debug_alloc<easy::malloc_alloc>::reallocate(p,SMALL_NUMBER,SMALL_NUMBER*2);
	easy::debug_alloc<easy::malloc_alloc>::deallocate(p,SMALL_NUMBER*2);
#endif
}

