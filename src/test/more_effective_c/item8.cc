#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem8 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem8);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem8);

//
// tests implementation
//
#include <string>
#if 0
void* operator new(size_t size)
{
	return malloc(size);
}

void* operator new(size_t, void* location)
{
	return location;
}

void operator delete(void* p)
{

}

void* operator new [] (size_t cb)
{

}

void operator delete [] (void* p)
{

}
#endif

class Widget
{
public:
	Widget(int widget_size) { }
};

Widget* construct_widget_in_buffer(void* buffer, int widget_size)
{
	return new (buffer) Widget(widget_size);
}

void TestItem8::test()
{
	std::string* ps = new std::string("memory management");
	delete ps;

	void* raw_memory = operator new(sizeof(std::string));
	operator delete(raw_memory);

	Widget* widget = new Widget(10);
	construct_widget_in_buffer(static_cast<void*>(widget),sizeof(Widget));
	widget->~Widget();
	delete widget;

	std::string* arr = new std::string [10];
	delete [] arr;
}
