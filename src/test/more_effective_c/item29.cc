#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem29 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem29);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem29);

//
// tests implementation
//
#include <iostream>
class String1 {
  public:
    String1(const char* data = "") {
        data_  = new char[strlen(data) + 1];
        strcpy(data_,data);
        std::cout << "construction function String1 called" << std::endl;
    }
    String1& operator= (const String1& rhs) {
        if (this == &rhs) {
            return *this;
        }
        delete [] data_;
        int len = strlen(rhs.data_);
        data_  = new char[strlen(rhs.data_) + 1];
        strcpy(data_,rhs.data_);
        return *this;
    }
    ~String1() {
        delete [] data_;
        std::cout << "destruction function String1 called" << std::endl;
    }
  private:
    char* data_;
};

class String2 {
  public:
    String2(const char* data = "")
        : value_(new StringValue(data)) {
        std::cout << "construction function String2 called" << std::endl;
    }

    String2(const String2& rhs) {
        if (rhs.value_->shareable_) {
            value_ = rhs.value_;
            ++value_->ref_count_;
        } else {
            value_ = new StringValue(rhs.value_->data_);
        }
    }

    String2& operator= (const String2& rhs) {
        if (value_ == rhs.value_) {
            return *this;
        }
        if (0 == --value_->ref_count_) {
            delete value_;
        }
        value_ = rhs.value_;
        ++value_->ref_count_;
        return *this;
    }

    const char& operator[] (int index) const {
        return value_->data_[index];
    }

    char& operator[] (int index) {
        if (value_->ref_count_ > 1) {
            --value_->ref_count_;
            value_ = new StringValue(value_->data_);
        }
        value_->shareable_ = false;
        return value_->data_[index];
    }

    ~String2() {
        if (0 == --value_->ref_count_) {
            delete value_;
        }
        std::cout << "destruction function String1 called" << std::endl;
    }
  private:
    struct StringValue {
        int ref_count_;
        char* data_;
        bool shareable_;
        StringValue(const char* val)
            : ref_count_(1),
              shareable_(true) {
            data_ = new char[strlen(val) + 1];
            strcpy(data_,val);
        }
        ~StringValue() {
            delete [] data_;
        }
    };

    StringValue* value_;
};

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
    String(const char* data = "") : value_(new StringValue(data)) { }
    const char operator[] (int index) const {
        return value_->data_[index];
    }
    char operator[] (int index) {
        if(value_->is_shared()) {
            value_ = new StringValue(value_->data_);
        }
        value_->mark_unshareable();
        return value_->data_[index];
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

void TestItem29::test() {
    String1 a,b,c,d,e;
    a = b = c = d = e = "hello";

    String2 s1("more effect c++");
    String2 s2(s1);
    String2 s3("more effect c++");
    s3 = s2;
    String2 s4("more");
    s3 = s4;
    String2 s5 = "hello";
    char* p = &s5[1];
    *p = 'x';

    String string1("lee");
    String string2(string1);
    String string3 = string2;

}
