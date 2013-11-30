#include "easy_cppunit_proxy.h"
#include "ace/Atomic_Op.h"
#include "ace/Task.h"
#include "ace/Log_Msg.h"
#include "ace/Synch.h"
//
// TestCase class
//
class Test_atomic_op : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(Test_atomic_op);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Test_atomic_op);

//
// tests implementation
//

typedef ACE_Atomic_Op<ACE_Null_Mutex,unsigned int>	safe_uint;
typedef ACE_Atomic_Op<ACE_Null_Mutex,int>	safe_int;

static const unsigned int __q_size = 2;
static const int max_producer = 20;

int check_termination(int __item)
{
	return (__item == max_producer);
}

class producer : public ACE_Task_Base
{
public:
	producer(int* __buf,safe_uint& __val_in,safe_uint& __val_out) : buf_(__buf),in_(__val_in),out_(__val_out) { }
	virtual int svc (void);
private:
	int* buf_;
	safe_uint& in_;
	safe_uint& out_;
};


int producer::svc( void )
{
	safe_uint __item_no = 0;
	while(1)
	{
		do 
		{
		} while (in_.value() - out_.value() == __q_size);
		++__item_no;
		buf_[in_.value()%__q_size] = __item_no.value();
		++in_;
		ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("producer %d\n"),__item_no.value ()));
		if (check_termination(__item_no.value()))
		{
			break;
		}
	}
	return 0;
}

class consumer : ACE_Task_Base
{
public:
	consumer(int* __buf,safe_uint& __val_in,safe_uint& __val_out) : buf_(__buf),in_(__val_in),out_(__val_out) { }

	virtual int svc (void);
private:
	int* buf_;
	safe_uint& in_;
	safe_uint& out_;
};

int consumer::svc( void )
{
	while(1)
	{
		int __item = 0;
		do 
		{
		} while (in_.value() - out_.value() == 0);
		__item = buf_[out_.value()%__q_size];
		++out_;
		ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("consumer %d\n"),__item));
		if (check_termination(__item))
		{
			break;
		}
	}
	return 0;
}

void Test_atomic_op::test()
{
	int __buf[__q_size];
	safe_uint __val_in = 0;
	safe_uint __val_out = 0;
	producer __producer(__buf,__val_in,__val_out);
	producer __consumer(__buf,__val_in,__val_out);
	__producer.activate();
	__consumer.activate();
	__producer.wait();
	__consumer.wait();
}
