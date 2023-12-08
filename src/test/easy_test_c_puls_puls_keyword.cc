#include "easy_cppunit_proxy.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////////explicit
class TestExplcit1 {
  public:
    TestExplcit1( int num, int other = 0 ) {
        num_ = num;
        other_ = other;
    }
  private:
    int num_;
    int other_;
};

class TestExplcit2 {
  public:
    explicit TestExplcit2( int num, int other = 0 ) {
        num_ = num;
        other_ = other;
    }
  private:
    int num_;
    int other_;
};

//////////////////////////////////////////////////////////////////////////volatile
#include <stdio.h>
void NotUseVolatile() {
    int i = 10;
    int a = i;
    printf("i = %d\n",a);
    __asm {
        mov dword ptr [ebp-4], 20h
    }
    int b = i;
    printf("i = %d\n",b);
};

void UseVolatile() {
    volatile int i = 10;
    int a = i;
    printf("i = %d\n",a);
    __asm {
        mov dword ptr [ebp-4], 20h
    }
    int b = i;
    printf("i = %d\n",b);
};

//////////////////////////////////////////////////////////////////////////typename
template< class T >
struct MyIter {
    typedef T value_type;
    T* ptr;
    MyIter(T* p) : ptr(p) {}
    T& operator* () const {
        return *ptr;
    }
};

template< class I >
typename I::value_type
func(I iter) {
    return *iter;
}

//////////////////////////////////////////////////////////////////////////

//
// TestCase class
//
class TestCPulsPulsKeyWord : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestCPulsPulsKeyWord);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(Explcit);
    CPPUNIT_TEST(Volatile);
    CPPUNIT_TEST(TypeName);
    CPPUNIT_TEST(Const);
    CPPUNIT_TEST(typeinfo);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void Explcit();
    void Volatile();
    void TypeName();
    void Const();
    void typeinfo();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCPulsPulsKeyWord);

void TestCPulsPulsKeyWord::Explcit() {
    TestExplcit1 t1 = 88;	//	ok
    //TestExplcit2 t2 = 88;	//	error C2440: 'initializing' : cannot convert from 'int' to 'TestExplcit2'
    TestExplcit2 t2(88);
}

void TestCPulsPulsKeyWord::Volatile() {
    //	why not useful at vs 2010?
    NotUseVolatile();
    UseVolatile();
    return;
}

void TestCPulsPulsKeyWord::TypeName() {
    MyIter<int> ite( new int(9));
    std::cout << func(ite) <<std::endl;
}

#include <iostream>
#include <vector>
#include <string>
class TextBlock {
  public:
    explicit TextBlock(const std::string& text) : text_(text)  {}
    const char& operator[] (std::size_t pos) const {
        return text_[pos];
    }

    char& operator[] (std::size_t pos) {
        return text_[pos];
    }

  private:
    std::string text_;
};

void Printf(const TextBlock& tcb) {
    std::cout << tcb[0];
    //tcb[0]  = 'l';	//error! you cannot assign to a variable that is const
}

void Printf(TextBlock& cb) {
    std::cout << cb[0];
    cb[0]  = 'l';
}

class CTextBlock {
  public:
    explicit CTextBlock(char* text) : text_(text), length_(0), length_invalid_(false) {}

    char& operator[] (std::size_t pos) const {
        return text_[pos];
    }
    std::size_t Length() const {
        if (!length_invalid_) {
            length_ = std::strlen(text_);
            length_invalid_ = true;
        }
        return length_;
    }
  private:
    char* text_;
#if 0
    std::size_t length_;
    bool length_invalid_;
#else	//use mutable, you can update const value
    mutable std::size_t length_;
    mutable bool length_invalid_;
#endif

};

void TestCPulsPulsKeyWord::Const() {
    //	如果关键字const 出现在* 左边，表示被指物位常量，
    //	如果关键字const 出现在*右边，表示指针自身为常量
    //	如果关键字const 出现在*左右两边，表示被指物和指针均为常量
    //	const Widget* pw 与 Widget const* pw 理论上是没有区别的
    char greeting[] = "hello";
    char* p = greeting;
    const char* cp = greeting;
    char* const cp1 = greeting;
    const char* const ccp = greeting;

    std::vector<int>	vec;
    vec.push_back(0);
    vec.push_back(1);
    vec.push_back(2);
    const std::vector<int>::iterator it = vec.begin();	//	it as  T* const
    *it = 20;		//	ok
    //++it;			//	error! no operator found which takes a left-hand operand of type 'const std::_Vector_iterator<_Myvec>' (or there is no acceptable conversion)
    std::vector<int>::const_iterator cit = vec.begin();	//	cit as const T*
    //*cit = 30;	//	error! you cannot assign to a variable that is const
    ++cit;			//	ok

    TextBlock blok("sophia");
    const TextBlock cblok("sophia");
    Printf(blok);
    Printf(cblok);

    const CTextBlock ccblock("sophia");
    char* pc = &ccblock[0];
    //*pc = 'L';	//error! why?
    std::size_t length = ccblock.Length();
}

#include <vector>
#include <map>

template < typename T >
T max( T arg1, T arg2 ) {
    std::cout << typeid( T ).name() << "s compared." << std::endl;
    return ( arg1 > arg2 ? arg1 : arg2 );
}

class Base {
  public:
    virtual void vvfunc() {}
};

class Derived : public Base {};


void TestCPulsPulsKeyWord::typeinfo() {
    bool equel = (typeid(int) == typeid(int&)); // evaluates to true

    std::vector<int>	v1;
    equel = (typeid(v1) == typeid(std::vector<int>&));
    equel = (typeid(v1) == typeid(std::vector<float>&));
    equel = (typeid(float) == typeid(std::vector<int>&));
    equel = (typeid(std::map<int,float>) == typeid(std::vector<int>&));

    max(1,2);

    Derived* pd = new Derived;
    Base* pb = dynamic_cast<Base*>(pd);
    std::cout << typeid( pb ).name() << std::endl;   //prints "class Base *"
    std::cout << typeid( *pb ).name() << std::endl;   //prints "class Derived"
    std::cout << typeid( pd ).name() << std::endl;   //prints "class Derived *"
    std::cout << typeid( *pd ).name() << std::endl;   //prints "class Derived"
    delete pd;

}

