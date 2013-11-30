#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem43 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem43);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem43);

//
// tests implementation
//
#include <string>
class CompanyA
{
public:
	void SendClearText(const std::string& msg) {}
	void SendEncrypted(const std::string& msg) {}
};

class CompanyB
{
public:
	void SendClearText(const std::string& msg) {}
	void SendEncrypted(const std::string& msg) {}
};

class CompanyZ
{
public:
	void SendEncrypter(const std::string& msg) {}
};

class MsgInfo{};

template<typename __Company>
class MsgSender
{
public:
	void SendClear(const MsgInfo& info)
	{
		std::string msg;
		__Company	c;
		c.SendClearText(msg);
	}

	void SendSecret(const MsgInfo& info)
	{
		std::string msg;
		__Company	c;
		c.SendEncrypted(msg);
	}
};

template<>
class MsgSender<CompanyZ>
{
public:
	void SendSecret(const MsgInfo& info)
	{
		//	...
	}
};

template<typename __Company>
class LoggingMsgSender : public MsgSender<__Company>
{
public:
	using MsgSender<__Company>::SendClear;
	//	log...
	void SendClearMsg(const MsgInfo& info)
	{
		//	log...
		SendClear(info);	

		this->SendClear(info);

		MsgSender<__Company>::SendClear(info);
	}
};

void TestItem43::test()
{
	MsgInfo	msg_info;
	LoggingMsgSender<CompanyA>	log_ca;
	log_ca.SendClearMsg(msg_info);
	LoggingMsgSender<CompanyB>	log_cb;
	log_cb.SendClearMsg(msg_info);

	//	LoggingMsgSender<CompanyZ>	log_cz;
	//	log_cz.SendClearMsg(msg_info);	//error
}
