#include "easy_cppunit_proxy.h"
#include "easy_list.h"
//
// TestCase class
//
class TestList : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestList);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(test_remove_if);
	CPPUNIT_TEST(test_unique);
	CPPUNIT_TEST(test_merge);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
	void test_remove_if();
	void test_unique();
	void test_merge();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestList);

//
// tests implementation
//
#include <iostream>
void TestList::test()
{
	easy::list<int>	l;
	l.push_back(1);
	l.push_back(2);
	l.push_back(3);
	l.push_front(0);
	std::cout << "before erase" << std::endl;
	for (easy::list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
	l.erase(l.begin());
	std::cout << "after erase" << std::endl;
	for (easy::list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
	easy::list<int>	l2;
	l.push_back(10);
	l.push_back(20);
	l.push_back(30);
	l.push_front(100);
	bool _empty = l.empty();
	std::cout << "_empty "<< _empty << std::endl;
	int val = *(l.begin());
#ifdef __WINDOWS
	val = *(l.rbegin());
#elif __LINUX
	if(val){}	//	warning: unused variable ‘val’ [-Wunused-variable]
#endif

	size_t	size = l.size();
	size_t	max_size = l.max_size();
	std::cout << "size "<< size << std::endl;
	std::cout << "max_size "<< max_size << std::endl;
	int front = l.front();
	int back = l.back();
	std::cout << "front "<< front << std::endl;
	std::cout << "back "<< back << std::endl;
	l.insert(l.end(),4);
	l.insert(l.end());
	std::cout << "before insert" << std::endl;
	for (easy::list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
	l.insert(l.end(),l2.begin(),l2.end());
	std::cout << "after insert" << std::endl;
	for (easy::list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}

	std::cout << "before erase" << std::endl;
	for (easy::list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
#if 0
	l.erase(l.begin(),l.end());
#else
	l.clear();
#endif
	
	std::cout << "after erase" << std::endl;
	for (easy::list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
	
	size = l.size();
	std::cout <<  "before resize" << size << std::endl;
	l.resize(10,10);
	size = l.size();
	std::cout <<  "after resize" << size << std::endl;
	for (easy::list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
	l.pop_back();
	l.pop_front();
	std::cout <<  "after pop_back,pop_front" << size << std::endl;
	for (easy::list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}

	easy::list<int> l3(10,100);
	for (easy::list<int>::iterator it = l3.begin(); it != l3.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}

	easy::list<int> l4(10);
	for (easy::list<int>::iterator it = l4.begin(); it != l4.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}

	easy::list<int> l5(l4.begin(),l4.end());
	for (easy::list<int>::iterator it = l5.begin(); it != l5.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}

	easy::list<int> l6(l3);
	for (easy::list<int>::iterator it = l6.begin(); it != l6.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
	l5 = l6;
	for (easy::list<int>::iterator it = l5.begin(); it != l5.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}

	std::cout <<  "before assign" << std::endl;
	easy::list<int> l7(20,111);
	for (easy::list<int>::iterator it = l7.begin(); it != l7.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
	l7.assign(3,222);
	std::cout <<  "after assign" << std::endl;
	for (easy::list<int>::iterator it = l7.begin(); it != l7.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}
	easy::list<int> l8(5);
	l8.assign(l7.begin(),l7.end());
	for (easy::list<int>::iterator it = l8.begin(); it != l8.end(); ++it)
	{
		std::cout << (*it) << std::endl;
	}

	easy::list<int> list1(3,2);
	easy::list<int> list2(3,3);
	for (easy::list<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		std::cout << "list1 's element "<< (*it) << std::endl;
	}
	for (easy::list<int>::iterator it = list2.begin(); it != list2.end(); ++it)
	{
		std::cout << "list2 's element "<< (*it) << std::endl;
	}
	list1.splice(list1.end(),list2);
	std::cout << "after list1.splice(list1.end(),list2); "<< std::endl;
	for (easy::list<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		std::cout << "list1 's element "<< (*it) << std::endl;
	}
	for (easy::list<int>::iterator it = list2.begin(); it != list2.end(); ++it)
	{
		std::cout << "list2 's element "<< (*it) << std::endl;
	}
	if (list2.empty())
	{
		std::cout << "after list1.splice(list1.end(),list2); list2 is empty"<< std::endl;
	}
	easy::list<int> list3(4,4);
	list1.splice(list1.end(),list3,list3.begin());
	for (easy::list<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		std::cout << "list1 's element "<< (*it) << std::endl;
	}
	list1.remove(2);
	for (easy::list<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		std::cout << "after remove 2,list1 's element "<< (*it) << std::endl;
	}
	list1.unique();
	for (easy::list<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		std::cout << "after unique,list1 's element "<< (*it) << std::endl;
	}
	for (easy::list<int>::iterator it = list3.begin(); it != list3.end(); ++it)
	{
		std::cout << "list3 's element "<< (*it) << std::endl;
	}
	list1.merge(list3);
	for (easy::list<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		std::cout << "after merge list3,list1 's element "<< (*it) << std::endl;
	}
	for (easy::list<int>::iterator it = list3.begin(); it != list3.end(); ++it)
	{
		std::cout << "after merge list3,list3 's element "<< (*it) << std::endl;
	}
	if (list3.empty())
	{
		std::cout << "after merge list3,list3 's element is empty " << std::endl;
	}
	
	list1.reverse();
	for (easy::list<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		std::cout << "after reverse,list1 's element "<< (*it) << std::endl;
	}
	easy::list<int>	list4;
	for (int i = 10; i  > 0; --i)
	{
		list4.push_back(i);
	}
	for (easy::list<int>::iterator it = list4.begin(); it != list4.end(); ++it)
	{
		std::cout << "list4 's element "<< (*it) << std::endl;
	}
	list4.sort();
	for (easy::list<int>::iterator it = list4.begin(); it != list4.end(); ++it)
	{
		std::cout << "after reverse,list4 's element "<< (*it) << std::endl;
	}

	easy::list<int>	list5(10,55);
	easy::list<int> list6;
	list6 = list5;
	bool equal = list6 == list5;
	std::cout << "equal "<< equal << std::endl;
}

template<typename T> 
class is_odd : public std::unary_function<T,bool>
{
public:
	bool operator() (T& val)
	{
		return (val % 2) == 1;
	}
};

void TestList::test_remove_if()
{
	easy::list <int> c1;
	easy::list <int>::iterator c1_Iter, c2_Iter;

	c1.push_back( 3 );
	c1.push_back( 4 );
	c1.push_back( 5 );
	c1.push_back( 6 );
	c1.push_back( 7 );
	c1.push_back( 8 );

	std::cout << "The initial list is c1 =";
	for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
		std::cout << " " << *c1_Iter;
	std::cout << std::endl;

	easy::list <int> c2 = c1;
	c2.remove_if( is_odd<int>( ) );

	std::cout << "After removing the odd elements, "
		<< "the list becomes c2 =";
	for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
		std::cout << " " << *c2_Iter;
	std::cout << std::endl;
}

template<class Type>
struct not_equal_to : public std::binary_function<Type, Type, bool> 
{
	bool operator()(
		const Type& _Left, 
		const Type& _Right
		) const
	{
		return _Left != _Right;
	}
};

void TestList::test_unique()
{
	easy::list <int> c1;
	easy::list <int>::iterator c1_Iter, c2_Iter,c3_Iter;
	not_equal_to<int> mypred;

	c1.push_back( -10 );
	c1.push_back( 10 );
	c1.push_back( 10 );
	c1.push_back( 20 );
	c1.push_back( 20 );
	c1.push_back( -10 );

	std::cout << "The initial list is c1 =";
	for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
		std::cout << " " << *c1_Iter;
	std::cout << std::endl;

	easy::list <int> c2 = c1;
	c2.unique( );
	std::cout << "After removing successive duplicate elements, c2 =";
	for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
		std::cout << " " << *c2_Iter;
	std::cout << std::endl;

	easy::list <int> c3 = c2;
	c3.unique( mypred );
	std::cout << "After removing successive unequal elements, c3 =";
	for ( c3_Iter = c3.begin( ); c3_Iter != c3.end( ); c3_Iter++ )
		std::cout << " " << *c3_Iter;
	std::cout << std::endl;
}

template<class Type>
struct greater : public std::binary_function <Type, Type, bool> 
{
	bool operator()(
		const Type& _Left, 
		const Type& _Right
		) const
	{
		bool res = true;
		if (_Left > _Right)
		{
			res = true;
		}
		else if (_Left <= _Right)
		{
			res = false;
		}
		return res;
	}
};

void TestList::test_merge()
{
	easy::list <int> c1, c2, c3;
	easy::list <int>::iterator c1_Iter, c2_Iter, c3_Iter;

	c1.push_back( 3 );
	c1.push_back( 6 );
	c2.push_back( 2 );
	c2.push_back( 4 );
	c3.push_back( 5 );
	c3.push_back( 1 );

	std::cout << "c1 =";
	for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
		std::cout << " " << *c1_Iter;
	std::cout << std::endl;

	std::cout << "c2 =";
	for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
		std::cout << " " << *c2_Iter;
	std::cout << std::endl;
	
	c2.merge( c1 );  // Merge c1 into c2 in (default) ascending order
	std::cout << "After merging c1 with c2 >: c2 =";
	for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
		std::cout << " " << *c2_Iter;
	std::cout << std::endl;

	c2.sort( greater<int>( ) );
	std::cout << "After merging c1 with c2 and sorting with >: c2 =";
	for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
		std::cout << " " << *c2_Iter;
	std::cout << std::endl;

	std::cout << "c3 =";
	for ( c3_Iter = c3.begin( ); c3_Iter != c3.end( ); c3_Iter++ )
		std::cout << " " << *c3_Iter;
	std::cout << std::endl;

	c2.merge( c3, greater<int>( ) );
	std::cout << "After merging c3 with c2 according to the '>' comparison relation: c2 =";
	for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
		std::cout << " " << *c2_Iter;
	std::cout << std::endl;
}
