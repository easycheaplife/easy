#include "easy_cppunit_proxy.h"
#include "easy_ring_buffer.h"
#include "easy_threading.h"
#include "easy_allocator.h"
#include <iostream>
//
// TestCase class
//
class TestRingBuffer : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestRingBuffer);
#if 0
	CPPUNIT_IGNORE;
#endif
	//CPPUNIT_TEST(test);
	CPPUNIT_TEST(press_test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
	void press_test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestRingBuffer);

//
// tests implementation
//
easy::EasyRingbuffer<easy_uint8,easy::alloc> buf(10240);
void TestRingBuffer::test()
{
	std::cout << "------start test ring buffer-----" << std::endl;
	
	easy_bool b = true;
	buf << b;
	easy_bool b_ = false;
	buf >> b_;

	easy_uint8 u8 = '8';
	buf << u8;
	easy_uint8 u8_ = 0;
	buf >>u8_;

	easy_uint16 u16 = 16;
	buf << u16;
	easy_uint16 u16_ = 0;
	buf >> u16_;

	easy_uint32 u32 = 32;
	buf << u32;
	easy_uint32 u32_ = 0;
	buf >> u32_;

	easy_uint64 u64 = 64;
	buf << u64;
	easy_uint64 u64_ = 0;
	buf >> u64_;

	std::string str_name = "lee";
	buf << str_name;
	char  str_name_[256];
	//	buf << str_name_; error, can not be overload
	buf.read((easy_uint8*)str_name_,strlen("lee"));

	std::string str_name2 = "sophia";
	buf << str_name2;
	char  str_name2_[256];
	//	buf << str_name2_; error, can not be overload
	buf.read((easy_uint8*)str_name2_,strlen("sophia"));
}

class WriteRingBuffer : public easy::EasyThread
{
public:
	WriteRingBuffer() { }
	~WriteRingBuffer() {  }
private:
	easy_int32 _Run( void* p )
	{
		while(1)
		{
			static easy_uint32 index = 0;
			buf << (easy_uint32)sizeof(index);
			buf << index++;
#if 1
			std::cout<< "write size = " << sizeof(index) << " index = " << index << std::endl;
#endif
		}
		return 0;
	}
};

#define MAX_THREAD_NUM	1
void TestRingBuffer::press_test()
{
	WriteRingBuffer* write_ring_buffer = new WriteRingBuffer[MAX_THREAD_NUM];
	while(1)
	{
		easy_uint32 read_size = 0;
		buf >> read_size;
		easy_uint32 index = 0;
		buf >> index;
#if 1
		std::cout<< "read size = " << read_size << " index = " << index << std::endl;
#endif
	}
	delete [] write_ring_buffer;
}
