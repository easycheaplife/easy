#include "easy_cppunit_proxy.h"
#include <ace/Global_Macros.h>
#include <ace/MMAP_Memory_Pool.h>
#include <ace/Malloc_T.h>
#include <ace/Null_Mutex.h>
//
// TestCase class
//
class Test_share_memory_non_of_business_position : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(Test_share_memory_non_of_business_position);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Test_share_memory_non_of_business_position);

//
// tests implementation
//

typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL,ACE_Null_Mutex>	ALLOACTOR;

typedef ACE_Malloc_LIFO_Iterator <ACE_MMAP_MEMORY_POOL,ACE_Null_Mutex>	MALLOC_LIFO_ITERATOR;

ALLOACTOR*	alloactor;

#define BACKING_STORE	"backing.store"

class record
{
public:
	record(int __id1,int __id2,char* __name)
		:id1_(__id1),id2_(__id2),name_(0)
	{
		size_t __len = ACE_OS::strlen(__name) + 1;
		this->name_ = reinterpret_cast<char *>(alloactor->malloc(__len));
		ACE_OS::strcpy(this->name_,__name);
	}
	~record() { alloactor->free(name_); }
	int id1() const { return id1_; }
	int id2() const { return id2_; }
	char* name() const { return name_; }
private:
	int id1_;
	int id2_;
	char* name_;
};

int add_record()
{
	char __buf[32];
	for (int __i = 0; __i < 10; ++__i)
	{
		sprintf(__buf,"%s:%d","record",__i);
		void* __memory = alloactor->malloc(sizeof(record));
		if (0 == __memory)
		{
			ACE_ERROR_RETURN((LM_ERROR,ACE_TEXT("%p\n"),ACE_TEXT("unable to alloc")),-1);
		}
		record* __rec = new (__memory) record(__i,__i+1,__buf);
		if (alloactor->bind(__buf,__rec) == -1)
		{
			ACE_ERROR_RETURN((LM_ERROR,ACE_TEXT("%p\n"),ACE_TEXT("bind failed")),-1);
		}
	}
	return 0;
}

void show_record()
{
	ACE_DEBUG((LM_ERROR,ACE_TEXT("the following record were found:\n")));
	MALLOC_LIFO_ITERATOR __iter(*alloactor);
	for(void* __temp = 0;__iter.next(__temp) != 0; __iter.advance())
	{
		record* __record = reinterpret_cast<record*>(__temp);
		ACE_DEBUG((LM_ERROR,ACE_TEXT("record name:%C|id1:%d|id2:%d\n"),__record->name(),__record->id1(),__record->id2()));
	}
}
int share_memory_persistent()
{
	ACE_NEW_RETURN(alloactor,ALLOACTOR(BACKING_STORE),-1);
#if 1
	add_record();
#else
	show_record();
#endif
	alloactor->sync();
	delete alloactor;
	return 0;
}
void Test_share_memory_non_of_business_position::test()
{
	share_memory_persistent();
}
