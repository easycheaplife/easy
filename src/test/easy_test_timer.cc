
#include "easy_cppunit_proxy.h"
#include "easy_timer.h"
#include "easy_singleton.h"
#include <iostream>
//
// TestCase class
//
class Testtimer : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(Testtimer);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Testtimer);

//
// tests implementation
//


#define easy_timer_mgr easy::EasySingleton<easy::timer>::Instance()
long  g_lTickCount= 0 ;
void AddTimerEvent( easy::base_event* pTimerEvent, int nTickData );
easy::timer __timer;
struct timernodetick : public easy::timer_node
{
	int nTickData;
	timernodetick() : nTickData( 10000 )
	{

	}
};

class  CTimerTest : public   easy::base_event
{
public:
	long time_out( easy::timer_node* pTimerNode )
	{
		switch ( pTimerNode->timer_type_ )
		{
		case kTimerTypeTick:
			{
				timernodetick* pTimerNodeTick = (timernodetick*)pTimerNode ;
				CTimerTest* firstTimer = new CTimerTest();
				AddTimerEvent( /*this*/firstTimer, g_lTickCount );
				std::cout <<"g_lTickCount = "<< g_lTickCount ++ << ";tick data "<< pTimerNodeTick->nTickData << std::endl;
				delete this;
			}
			break;
		}
		return 0;
	}
};

void AddTimerEvent( easy::base_event* pTimerEvent, int nTickData )
{
	timernodetick* tim = new timernodetick ;
	tim->expire_    = easy::EasyTime::get_cur_sys_time() + 100;
	tim->fun_     = pTimerEvent;
	tim->timer_type_    = kTimerTypeTick;
	tim->nTickData = nTickData;
	__timer.add_timer( tim );
#if 1
	if (10 == nTickData)
	{
		__timer.delete_timer(tim);
		g_lTickCount += 10;
		tim = new timernodetick ;
		tim->expire_    = easy::EasyTime::get_cur_sys_time() + 100;
		tim->fun_     = pTimerEvent;
		tim->timer_type_    = kTimerTypeTick;
		tim->nTickData = nTickData;
		__timer.add_timer( tim );
	}

#endif
}

void Testtimer::test()
{
	__timer.init(1000);
	CTimerTest* firstTimer = new CTimerTest();
	AddTimerEvent( firstTimer, g_lTickCount);
	while (1)
	{
		__timer.expires(  );
	}
}
