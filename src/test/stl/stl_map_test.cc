#include "easy_cppunit_proxy.h"
//
// TestCase class
//
#include <stdio.h>
#include <sys/time.h>
#include <map>
#include <vector>
using namespace std ;


class stl_map_test : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(stl_map_test);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(stl_map_test);

//
// tests implementation
//

void stl_map_test::test()
{
	static const int __max_count = 20000;
	std::map<int,int*>	__map;
	std::vector<int>	__vec;
	int __array[__max_count];
	
	for(int __i = 0; __i < __max_count; ++__i)
	{
		__map[__i] = &__i;
		__array[__i] = __i;
		__vec.push_back(__i);
	}
	struct timeval __start_timeval,__end_timeval;
	gettimeofday(&__start_timeval, NULL);
	long __start_time = __start_timeval.tv_usec;
	for(std::map<int,int*>::iterator __it = __map.begin(); __it != __map.end(); ++__it)
	{
		if(__it->second)
		{
			int __val_ = __it->first;
			//	do nothing
		}
	}
	gettimeofday(&__end_timeval, NULL);
	long __end_time = __end_timeval.tv_usec;
	long __time_round_trip = __end_time - __start_time;
	printf("map test:start time = %ld, end time = %ld,time round trip = %ld\n",__start_time,__end_time,__time_round_trip);
	
	gettimeofday(&__start_timeval, NULL);
	__start_time = __start_timeval.tv_usec;
	for(int __i = 0; __i < __max_count; ++__i)
	{
		int __val_ = __vec[__i];
	}
	gettimeofday(&__end_timeval, NULL);
	__end_time = __end_timeval.tv_usec;
	__time_round_trip = __end_time - __start_time;
	printf("vector test:start time = %ld, end time = %ld,time round trip = %ld\n",__start_time,__end_time,__time_round_trip);
	
	
	gettimeofday(&__start_timeval, NULL);
	__start_time = __start_timeval.tv_usec;
	for(int __i = 0; __i < __max_count; ++__i)
	{
		int __val_ = __array[__i];
	}
		gettimeofday(&__end_timeval, NULL);
	__end_time = __end_timeval.tv_usec;
	__time_round_trip = __end_time - __start_time;
	printf("array test:start time = %ld, end time = %ld,time round trip = %ld\n",__start_time,__end_time,__time_round_trip);
}
