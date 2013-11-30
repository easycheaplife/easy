#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem23 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem23);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem23);
#include <string>
//
// tests implementation
//
//	imple1	use inherit
//	class forward declare
class record;
class primary_key;
class genereate_table_algoritem1
{
public:
	genereate_table_algoritem1(const std::string& __table) { }
	virtual ~genereate_table_algoritem1() { }
	bool process()
	{
		//	to be continue...
		return true;
	}
private:
	virtual bool filter(const record&) { return true; }

	virtual bool process_row(const primary_key&) = 0;

	struct genereate_table_algoritem_impl* impl_;
};

class my_algoritem : public genereate_table_algoritem1
{
public:
	my_algoritem(const std::string& __table):genereate_table_algoritem1(__table) { }
	bool filter(const record&) { return true; }

	bool process_row(const primary_key&){ return true; }
};
//////////////////////////////////////////////////////////////////////////
//	imple2	
//	class forward declare
class my_client;
class genereate_table_algoritem2
{
public:
	genereate_table_algoritem2(const std::string& __table,my_client& __my_client){}
	~genereate_table_algoritem2(){}

	bool process() { return true; }

private:
	struct genereate_table_algoritem_impl* impl_;
};

class my_client
{
public:
	my_client() { }

	virtual ~my_client() {};

	virtual bool filter(const record&) { return true; }

	virtual bool process_row(const primary_key&) = 0;
};

class my_worker : public my_client
{
public:
	my_worker() { }
	~my_worker() { }
	bool filter(const record&) { return true; }

	bool process_row(const primary_key&){ return true; }
};

//	imple3
bool genereate_table_algoritem3(const std::string& __table,my_client& __my_client)
{
	//	process sth...
	return true;
}

//	imple3
template<class __MyClient>
bool genereate_table_algoritem4(const std::string& __table)
{
	//	process sth...
	return true;
}

void TestItem23::test()
{
	//	test1
	my_algoritem __a("customer");
	__a.process();

	//	test2
	genereate_table_algoritem2 __b("customer",my_worker());
	__b.process();
	my_client* __my_worker = new my_worker();
	delete __my_worker;

	//	test3
	genereate_table_algoritem3("customer",my_worker());

	//	tes4
	genereate_table_algoritem4<my_worker>("customer");
}
