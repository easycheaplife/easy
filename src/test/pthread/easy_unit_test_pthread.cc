#include "easy_cppunit_proxy.h"
#include <stdio.h>
#include <pthread.h>  
#include "easy_util.h"
//	for pthread win32
#if defined __WINDOWS || defined WIN32
	#ifdef _WIN64
		#pragma comment(lib,"x64/pthreadVC2.lib")
	#else
		#pragma comment(lib,"x86/pthreadVC2.lib")
	#endif //_WIN64
#endif //WIN32

//
// TestCase class
//
class TestPthread : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestPthread);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPthread);

//
// tests implementation
//

void* pthread_thread_fun1(void* __param)  
{  
	while (true)
	{
		pthread_t __thread_id = pthread_self();  
		printf("thread id=%d\n ", __thread_id);  
		easy::Util::sleep(1000*1000);
	}
	return NULL;  
}  

class pthread_test
{
public:
	pthread_test()
	{
		pthread_attr_init(&__attr);
		pthread_attr_setscope(&__attr, PTHREAD_SCOPE_PROCESS);  
		pthread_attr_setdetachstate(&__attr, PTHREAD_CREATE_DETACHED); 
		pthread_create(&__pid, &__attr, pthread_thread_fun2, this);  
	}
	~pthread_test()
	{
		pthread_attr_destroy(&__attr);  
	}
	static void* pthread_thread_fun2(void* __param)  
	{  
		pthread_test* __pthis = (pthread_test*)__param;
		while (true)
		{
			pthread_t __thread_id = pthread_self();  
			printf("thread id=%d\n ", __thread_id);  
			easy::Util::sleep(1000*1000);
		}
		return NULL;  
	}  
private:
	pthread_t __pid; 
	pthread_attr_t __attr;  
};

void TestPthread::test()
{
	pthread_t __pid;  
	pthread_attr_t __attr;  
	pthread_attr_init(&__attr);
	pthread_attr_setscope(&__attr, PTHREAD_SCOPE_PROCESS);  
	pthread_attr_setdetachstate(&__attr, PTHREAD_CREATE_DETACHED); 
	pthread_create(&__pid, &__attr, pthread_thread_fun1, NULL);  
	while (true)
	{ 
		easy::Util::sleep(1000*1000);
	}
}
