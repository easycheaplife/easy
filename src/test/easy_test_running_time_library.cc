#include "easy_cppunit_proxy.h"
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
//
// TestCase class
//
class TestRunningTimeLibrary : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestRunningTimeLibrary);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(fun_abort);
	CPPUNIT_TEST(fun_fprintf);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
	void fun_fprintf();
	void fun_abort();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestRunningTimeLibrary);

//
// tests implementation
//

void TestRunningTimeLibrary::test()
{
	
}

void TestRunningTimeLibrary::fun_fprintf()
{
	FILE *stream;
	int    i = 10;
	double fp = 1.5;
	char   s[] = "this is a string";
	char   c = '\n';

	fopen_s( &stream, "fprintf.out", "w" );
	fprintf( stream, "%s%c", s, c );
	fprintf( stream, "%d\n", i );
	fprintf( stream, "%f\n", fp );
	fclose( stream );
	system( "type fprintf.out" );

	fprintf(stderr, "out of memory\n"); exit(1);
}

void TestRunningTimeLibrary::fun_abort()
{
	FILE    *stream = NULL;
	errno_t err = 0;

	err = fopen_s(&stream, "NOSUCHF.ILE", "r" );
	if ((err != 0) || (stream == NULL))
	{
		perror( "File could not be opened" );
		abort();
	}
	else
	{
		fclose( stream );
	}

}
