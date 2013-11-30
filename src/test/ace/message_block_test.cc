
#include "easy_cppunit_proxy.h"
#include "ace/ACE.h"
#include "ace/Message_Block.h"
//
// TestCase class
//
class Test_message_blcok : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(Test_message_blcok);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Test_message_blcok);

//
// tests implementation
//
#define BUF_SIZE	8
void Test_message_blcok::test()
{
	ACE_Message_Block* __head = new ACE_Message_Block(BUF_SIZE);
	ACE_Message_Block* __mblk = __head;
	for (int __i = 0; __i < 2;++__i)
	{
		ssize_t __bytes = ACE::read_n(ACE_STDIN,__mblk->wr_ptr(),__mblk->size());
		if (__bytes <= 0)
		{
			break;
		}
		__mblk->wr_ptr(__bytes);
		__mblk->cont(new ACE_Message_Block(BUF_SIZE));
		__mblk = __mblk->cont();
	}
	
	for (__mblk = __head; __mblk != 0; __mblk = __mblk->cont())
	{
		ACE::write_n(ACE_STDOUT,__mblk->rd_ptr(),__mblk->length());
	}
	__head->release();
	return ;
}
