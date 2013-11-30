#include "easy_cppunit_proxy.h"
#include "easy_char_traits.h"
//
// TestCase class
//
class TestChar_Traits : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestChar_Traits);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestChar_Traits);

//
// tests implementation
//

void TestChar_Traits::test()
{
	int __res = easy::__char_traits_base<char,int>::compare("lee","lee",3);
	__res = easy::__char_traits_base<char,int>::compare("lee","lee_sophia",7);
	__res = easy::__char_traits_base<char,int>::compare("lee_sophia","lee",7);

	size_t __len = easy::__char_traits_base<char,int>::length("lee");

	const char* __s = easy::__char_traits_base<char,int>::find("sophia",6,'h');

	char __s1[7];
	char* __s2 = "sophia";
	easy::__char_traits_base<char,int>::move(__s1, __s2,6);
	__s1[6] = '\0';

	easy::__char_traits_base<char,int>::assign(__s1,6,'d');

	easy::__char_traits_base<char,int>::not_eof(10);

	easy::__char_traits_base<char,int>::not_eof(-1);

	easy::char_traits<char>::to_char_type(10);
}
