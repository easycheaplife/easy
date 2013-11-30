#include <iostream>
#include "easy_cppunit_proxy.h"
#include "easy_singleton.h"
//
// TestCase class
//
class TestSingleton : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestSingleton);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSingleton);

//
// tests implementation
//

class Foo
{
	friend class easy::EasySingleton<Foo>;
public:
	void Printf() { std::cout << "Foo called" << std::endl; }
private:
	Foo () { std::cout << "Foo constructed" << std::endl; }
	~Foo () { std::cout << "Foo destroyed" << std::endl; }
};

#define s_foo easy::EasySingleton<Foo>::Instance()

class Test : public easy::Singleton<Test>
{
public:
	Test () { std::cout << "Test constructed" << std::endl; }
	~Test () { std::cout << "Test destroyed" << std::endl; }
};

template<>Test* easy::Singleton<Test>::singleton_ = 0;

void TestSingleton::test()
{
	s_foo->Printf();
	easy::EasySingleton<Foo>::Close();

	new Test();
	delete Test::GetSingletonPtr();
}
