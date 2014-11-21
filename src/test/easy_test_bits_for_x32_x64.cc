#include "easy_cppunit_proxy.h"
#include <stdio.h>

//
// TestCase class
//
class TestBits : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestBits);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestBits);

//
// tests implementation
//

void TestBits::test()
{
	/*
	win32
	__bits_char' value is 1
	__bits_short_int' value is 2
	__bits_int' value is 4
	__bits_unsigned_int' value is 4
	__bits_float' value is 4
	__bits_double' value is 8
	__bits_long' value is 4
	__bits_unsigned_long' value
	__bits_long_long' value is 8
	__bits_size_t' value is 4

	win64
	__bits_char' value is 1
	__bits_short_int' value is 2
	__bits_int' value is 4
	__bits_unsigned_int' value is 4
	__bits_float' value is 4
	__bits_double' value is 8
	__bits_long' value is 4
	__bits_unsigned_long' value is 4
	__bits_long_long' value is 8
	__bits_size_t' value is 8
	
	linux 64
	__bits_char' value is 1
	__bits_short_int' value is 2
	__bits_int' value is 4
	__bits_unsigned_int' value is 4
	__bits_float' value is 4
	__bits_double' value is 8
	__bits_long' value is 8
	__bits_unsigned_long' value is 8
	__bits_long_long' value is 8
	__bits_size_t' value is 8
	*/
	


	//	base data type
	int __bits_char = sizeof(char);
	int __bits_short_int = sizeof(short int);
	int __bits_int = sizeof(int);
	int __bits_unsigned_int = sizeof(unsigned int);
	int __bits_float = sizeof(float);
	int __bits_double = sizeof(double);
	int __bits_long = sizeof(long);
	int __bits_unsigned_long = sizeof(unsigned long);
	int __bits_long_long = sizeof(long long);
	int __bits_size_t = sizeof(size_t);
	printf("__bits_char' value is %d\n",__bits_char);
	printf("__bits_short_int' value is %d\n",__bits_short_int);
	printf("__bits_int' value is %d\n",__bits_int);
	printf("__bits_unsigned_int' value is %d\n",__bits_unsigned_int);
	printf("__bits_float' value is %d\n",__bits_float);
	printf("__bits_double' value is %d\n",__bits_double);
	printf("__bits_long' value is %d\n",__bits_long);
	printf("__bits_unsigned_long' value is %d\n",__bits_unsigned_long);
	printf("__bits_long_long' value is %d\n",__bits_long_long);
	printf("__bits_size_t' value is %d\n",__bits_size_t);
}
