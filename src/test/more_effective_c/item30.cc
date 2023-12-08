#include "easy_cppunit_proxy.h"
#include <iostream>
//
// TestCase class
//
class TestItem30 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem30);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem30);

//
// tests implementation
//

template<typename T>
class Array2D {
  public:

    Array2D(int dim1,int dim2)
        : dim1_(dim1),
          dim2_(dim2) {

    }
#if 1	// not use proxy class
    T& operator() ( int index1, int index2) {
        return array_[index1,index2];
    }

    const T& operator() (int index1, int index2) const {
        return array_[index1,index2];
    }
#else
    class ArrayID {
      public:
        T& operator[] (int index) { }
        const T& operator [] (int index) const  { }
    };
    ArrayID& operator() ( int index1, int index3) { }

    const ArrayID& operator() (int index1, int index2) const { }
#endif

  private:
    T	dim1_;
    T	dim2_;
    T**	array_;
};

void process_input(int dim1,int dim2) {
    //	int data[dim1][dim2];	//	error  expected constant expression
    Array2D<int>(dim1,dim2);
}

class RCObject {
  public:
    RCObject()
        :ref_count_(0),
         shareable_(true) {}

    RCObject(const RCObject& rhs)
        :ref_count_(0),
         shareable_(true) { }

    RCObject& operator = (const RCObject& rhs) {
        return *this;
    }

    virtual ~RCObject() { }

    void add_reference() {
        ++ref_count_;
    }

    void remove_reference() {
        if(0 == --ref_count_) delete this;
    }

    void mark_unshareable() {
        shareable_ = false;
    }

    bool shareable() const {
        return shareable_;
    }

    bool is_shared() const {
        return ref_count_ > 1;
    }
  private:
    int ref_count_;

    bool shareable_;
};

template<typename _Tp>
class RCPtr {
  public:
    RCPtr(_Tp* real_ptr = 0) : pointee_(real_ptr) {
        _init();
    }

    RCPtr(const RCPtr& rhs) : pointee_(rhs.pointee_) {
        _init();
    }

    ~RCPtr() {
        if(pointee_) pointee_->remove_reference();
    }

    RCPtr& operator = (const RCPtr& rhs) {
        if(pointee_ != rhs.pointee_) {
            if(pointee_) {
                pointee_->remove_reference();
            }
            pointee_ = rhs.pointee_;
            _init();
        }
        return *this;
    }

    _Tp* operator-> () const {
        return pointee_;
    }

    _Tp& operator* () const {
        return *pointee_;
    }
  private:
    void _init() {
        if(0 == pointee_) {
            return ;
        }
        if(false == pointee_->is_shared()) {
            pointee_ = new _Tp(*pointee_);
        }
        pointee_->add_reference();
    }
    _Tp* pointee_;
};

class String {
  public:

    class CharProxy {
      public:
        CharProxy(String& str,int index) : the_string_(str), char_index_(index) { }
        CharProxy& operator= (const CharProxy& rhs) {
            if (the_string_.value_->is_shared()) {
                the_string_.value_ = new StringValue(the_string_.value_->data_);
            }
            the_string_.value_->data_[char_index_] = rhs.the_string_.value_->data_[char_index_];
            return *this;
        }
        CharProxy& operator = (char c) {
            if (the_string_.value_->is_shared()) {
                the_string_.value_ = new StringValue(the_string_.value_->data_);
            }
            the_string_.value_->data_[char_index_] = c;
            return *this;
        }
        operator char() const {
            return the_string_.value_->data_[char_index_];
        }
      private:
        String& the_string_;
        int char_index_;
    };

    String(const char* data = "") : value_(new StringValue(data)) { }

    friend class CharProxy;

    const CharProxy operator[] (int index) const {
        return CharProxy(const_cast<String&>(*this),index);
    }

    CharProxy operator[] (int index) {
        return CharProxy(*this,index);
    }
  private:
    struct StringValue : public RCObject {
        StringValue(const char* data = "") {
            _init(data);
        }

        StringValue(const StringValue& rhs) {
            _init(rhs.data_);
        }

        void _init(const char* data) {
            data_ = new char[strlen(data) + 1];
            strcpy(data_,data);
        }

        ~StringValue() {
            delete [] data_;
        }

        char* data_;
    };
  private:
    RCPtr<StringValue> value_;
};
void TestItem30::test() {
    process_input(3,3);

    Array2D<int> data1(3,3);
    Array2D<float>* data2 = new Array2D<float> (10,10);

    //	std::cout << data1(3,3);

    Array2D<float>	data3(10,10);
    //	std::cout << data3[3][3];

    //////////////////////////////////////////////////////////////////////////
    String s1("lee");
    String s2("sophia");

    std::cout << s1[2];

#if 0	// not use proxy class
    //	s1[2] = 'a';	//	'=' : left operand must be l-value
    //	s1[1] = s2[1];	//	'=' : left operand must be l-value
#else
    s1[2] = 'a';
    s1[1] = s2[1];
#endif
}

