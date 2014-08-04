#include "easy_cppunit_proxy.h"
#include "easy_ring_buffer.h"
#include "easy_threading.h"
#include "easy_allocator.h"
#include <iostream>
#include <time.h>
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
	CPPUNIT_TEST(reallocate_test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
	void reallocate_test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestRingBuffer);

//
// tests implementation
//
// 

static std::string __random_string[] = 
{
	"[0x000085e4][T]AdvertisingIndentitifer: '', IdentifierForVendor: '', DeviceName: 'PC', ModelName: 'x86', SystemName: '', SystemVersion: '', HardwareID: '74d435046509'",
	"nice to meet you!",
	"It is the tears of the earth that keep here smiles in bloom.",
	"The mighty desert is burning for the love of a blade of grass who shakes her head and laughs and flies away.",
	"If you shed tears when you miss the sun, you also miss the stars.",
	"Her wishful face haunts my dreams like the rain at night.",
	"Once we dreamt that we were strangers.We wake up to find that we  were dear to each other.",
	"Sorrow is hushed into peace in my heart like the evening among the silent trees.",
	"Some unseen fingers, like an idle breeze, are playing upon my heart the music of the ripples.",
	"Listen, my heart, to the whispers of the world with which it makes love to you.",
	"Do not seat your love upon a precipice because it is high.",
	"I sit at my window this morning where the world like a passer-by stops for a moment, nods to me and goes.",
	"There little thoughts are the rustle of leaves; they have their whisper of joy in my mind.",
	"What you are you do not see, what you see is your shadow.",
	"My wishes are fools, they shout across thy song, my Master.Let me but listen.",
	"They throw their shadows before them who carry their lantern on their back.",
	"That I exist is  a perpetual surprise which is life.",
	"We, the rustling leaves, have a voice that answers the storms,but who are you so silent?I am a mere flower.",
	"Do not blame your food because you have no appetite.",
	"Success is not final, failure is not fatal: it is the courage to continue that counts.",
	"I cannot tell why this heart languishes in silence.It is for small needs it never asks, or knows or remembers.",
	"The bird wishes it were a cloud.The cloud wishes it were a bird."
};

static int __random_string_size = 22;

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

void TestRingBuffer::reallocate_test()
{
	std::string __str_name = "lee";
	std::string __str_name2 = "sophia";
	static const int __buffer_size = 256;

	//	case 1: rpos_ <= wpos_
	easy::EasyRingbuffer<easy_uint8,easy::alloc> __buf(1024);
	__buf << __str_name;
	__buf << __str_name2;

	__buf.reallocate(__buf.size());
	
	char  __str_name_[__buffer_size] = {};
	__buf.read((easy_uint8*)__str_name_,__str_name.size());
	
	memset(__str_name_,0,__buffer_size);
	__buf.read((easy_uint8*)__str_name_,__str_name2.size());

	//	case 2: rpos_ > wpos_ 
	srand( (unsigned)time(NULL)); 
	easy::EasyRingbuffer<easy_uint8,easy::alloc> __buf2(256);
	static int __max_count = 100;
	size_t __head_size = 0;
	char __read_buffe[__buffer_size] = {};
	for (int __i = 0; __i < __max_count; ++__i)
	{
		int __random_index = rand()%__random_string_size;
		__buf2 << __random_string[__random_index].size();
		__buf2 << __random_string[__random_index];
		int __read_flag = rand()%__max_count;
		if(__read_flag % 2)
		{
			__buf2 >> __head_size;
			memset(__read_buffe,0,__buffer_size);
			__buf2.read((easy_uint8*)__read_buffe,__head_size);
			std::cout << __read_buffe << std::endl;
		}
	}
	
}