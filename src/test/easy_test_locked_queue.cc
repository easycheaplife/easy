#include <iostream>
#include <map>
#if 0
#include "easy_list.h"
#else
#include "easy_lock.h"
#include <list>
#endif
#include "easy_cppunit_proxy.h"
#include "easy_locked_queue.h"

//
// TestCase class
//
class TestLockQueue : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestLockQueue);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLockQueue);

//
// tests implementation
//
class Widget //: public easy::my_alloc
{
public:
	explicit Widget(int __val) 
	{
		std::cout << "construction function Widget called!" << std::endl;
	}
	void init(){}
	void clear(){}
	~Widget() 
	{
		std::cout << "destruction function Widget called!" << std::endl;
	}

	static const size_t MAX_POOL_SIZE = 1024;
	typedef  int _Key;
};
void TestLockQueue::test()
{
	//	new style
	easy::lock_queue<Widget,easy::mutex_lock,std::list<Widget*> >	__queue;
	Widget* __widget = __queue.allocate(1);
	__queue.deallcate(__widget);
	size_t __size = __queue.free_size();
	__queue.clear();

	//	explicit specialization

	easy::lock_queue<Widget,easy::mutex_lock,std::list<Widget*> >	__queue_es;
	Widget* __widget_cs = __queue_es.allocate(1);
	__queue_es.deallcate(__widget_cs);
	__queue_es.clear();

	easy::lock_container<Widget,easy::mutex_lock,unsigned int,std::map<unsigned int,Widget*> >	__container;
	Widget* __widget1 = new Widget(1);
	__container.insert(1,__widget1);
	Widget* __find = __container.find(1);
	__container.clear();
	__container.remove(1);
	delete __widget1;
}
