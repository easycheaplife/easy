#include "easy_cppunit_proxy.h"
#include "easy_byte_convertrt.h"
#include "easy_byte_buffer.h"
#include <string>
#include <iostream>
//
// TestCase class
//
class TestByteBuffer : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestByteBuffer);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

template<typename _Tp>
void _swap(_Tp& __lhs, _Tp& __rhs) {
    _Tp __tmp;
    __tmp = __lhs;
    __lhs = __rhs;
    __rhs = __tmp;
}
CPPUNIT_TEST_SUITE_REGISTRATION(TestByteBuffer);

//
// tests implementation
//

class other_int {
  public:
    other_int(int __a, int __b) : a_(__a),b_(__b) { }
    int& a()  {
        return a_;
    }
    int& b()  {
        return b_;
    }
//	private:
    int a_;
    int b_;
};

void TestByteBuffer::test() {
    std::cout << "------start test byte buffer-----" << std::endl;

    other_int __other_int(5,6);
    int __a = 3;
    int __b = 4;
    _swap(__a,__b);
    _swap(__other_int.a_,__other_int.b_);
    _swap(__other_int.a(),__other_int.b());

    std::vector<easy_uint8>	storage_;
    storage_.reserve(100);
    int __capacity = storage_.capacity();
    std::cout << "__capacity=" << __capacity << std::endl;

    try {
        char p[] = "sophia";
        easy::apply<char>(p);

        int arr[] =  {200,300};
        easy::apply<int>(arr);

        std::string str_arr[] = {"lee","sophia"};
        easy::apply<std::string>(str_arr);			//	error

        easy::EasyByteBufferException(true,20,30,40);

        easy::EasyByteBuffer byte_buf;

        byte_buf << (easy_uint8)'d';
        easy_uint8 u8 = 0;
        byte_buf >> u8;

        std::string name_go =  "lee";
        byte_buf << name_go;
        std::string name_back;
        byte_buf >> name_back;

#if 0
        char name_go1[] = "sophia";
        byte_buf << name_go1;
        char name_back1[16];
        byte_buf >> name_back1;
#endif

        byte_buf << (easy_uint8)10;
        easy_uint8 val = byte_buf[5];
        std::cout << "val=" << val << std::endl;

        byte_buf.append(4);
        byte_buf.append('c');
        byte_buf.append("tomy");

        byte_buf.put(1,"sophia");

        byte_buf << (easy_uint16)10;

        byte_buf << (easy_uint32)20;

        byte_buf << (easy_uint64)30;

        int j = 0;
        for (int i = 0; i <= MAX_PACKET_LENGTH/10; ++i) {
            byte_buf << i;
            byte_buf >> j;
        }

        const easy_uint8* __address = byte_buf.contents();
        std::cout << "__address=" << __address << std::endl;
        byte_buf.clear();

        easy::EasyByteBuffer byte_buf_temp;
        easy_uint8	__val1 = 1;
        easy_uint32 __val2 = 2;
        easy_float32 __val3 = 3.0;
        easy_uint8	__val1_ = 1;
        easy_uint32 __val2_ = 2;
        easy_float32 __val3_ = 3.0;
        byte_buf_temp << __val1 << __val2 << __val3;
        byte_buf_temp >> __val1_ >> __val2_ >> __val3_;

        easy::EasyByteBuffer byte_buf_str;
        byte_buf_str.append((unsigned char*)"sophia",6);
        byte_buf_str << "lee";
        byte_buf_str.put(0,(unsigned char*)"aihpos",6);
        const unsigned char* __buff = byte_buf_str.contents();
        std::cout << "__buff=" << __buff << std::endl;
    } catch(...) {

    }
}
