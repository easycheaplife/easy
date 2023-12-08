#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem28 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem28);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem28);

//
// tests implementation
//

template<typename _Tp>
class auto_ptr {
  public:
    auto_ptr(_Tp* val = 0) : value_(val) { }

    auto_ptr(auto_ptr<_Tp>& rhs) {
        value_ = rhs.value_;
        rhs.value_ = 0;
    }

    const auto_ptr& operator= (auto_ptr& rhs) {
        if (this == &rhs) {
            return *this;
        }
        delete value_;
        value_ = rhs.value_;
        rhs.value_ = 0;
        return  *this;
    }

    _Tp& operator* () const {
        return *value_;
    }

    _Tp* operator-> () const {
        return value_;
    }

    operator void*()  {
        return 0;
    }

    //	convert dumb pointer
    operator _Tp* () {
        return value_;
    }

    template<typename _NewTp>
    operator auto_ptr<_NewTp>() {
        return auto_ptr<_NewTp>(value_);
    }

    ~auto_ptr() {
        delete value_;
    }
  private:
    _Tp*	value_;
};

#include <iostream>
class Widget {
  public:
    Widget(int val) : value_(val) { }

    void show_value() {
        std::cout << value_ <<std::endl;
    }
  private:
    int	value_;
};

class Derive : public Widget {

};

void Print( auto_ptr<Widget> p) {
    p->show_value();
}

void Print2( const auto_ptr<Widget>& p) {
    p->show_value();
}

void Print3(Widget* p) {
    p->show_value();
}
//////////////////////////////////////////////////////////////////////////
class MusicProduct {
  public:
    MusicProduct() { }

    MusicProduct(const std::string& title) { }

    virtual void play() = 0;

    virtual void display_title() = 0;
};

class Cassette : public MusicProduct {
  public:
    Cassette(const std::string& title) { }

    virtual void play() { }

    virtual void display_title() { }
};

class CD : public MusicProduct {
  public:
    CD(const std::string& title) { }

    virtual void play() { }

    virtual void display_title() { }
};

#if 0
template<>
class auto_ptr<Cassette> {
  public:
    operator auto_ptr<MusicProduct> () {
        return auto_ptr<MusicProduct>(value_);
    }
  private:
    Cassette* value_;
};

template<>
class auto_ptr<CD> {
  public:
    operator auto_ptr<MusicProduct> () {
        return auto_ptr<MusicProduct>(value_);
    }
  private:
    CD* value_;
};
#endif

void displayer_adn_player(const MusicProduct* pmp, int num) {
    for (int i = 0; i <= num; ++i) {
        //	pmp->play();
    }
}

void displayer_adn_player2(const auto_ptr<MusicProduct>& pmp, int num) {
    for (int i = 0; i <= num; ++i) {
        //	pmp->play();
    }
}

void TestItem28::test() {
    auto_ptr<Widget> p1 = new Widget(1);
    auto_ptr<Widget> p2 = new Widget(2);

    auto_ptr<Widget> p3 (new Widget(3));
    auto_ptr<Widget> p4 = p3;
    p1 = p2;
    p1->show_value();
    //	p2->show_value();	//	error
    //	p3->show_value();	//	error
    p4->show_value();
    //	Print(p4);	// copy construct function called, this will cause the p4 is not value any more
    Print2(p4);		//	ok pass by reference

    auto_ptr<Widget>	p5;
    //if (p5)	{}	// conditional expression of type 'auto_ptr<_Tp>' is illegal
    //if (0 == p5)	{}	//	binary '==' : no global operator found which takes type 'auto_ptr<_Tp>' (or there is no acceptable conversion)
    //if (!p5) { }	//	auto_ptr<_Tp>' does not define this operator or a conversion to a type acceptable to the predefined operator

#if 0
    //	after operator void*()
    if (p5)	{}			//	ok
    if (0 == p5)	{}	//	ok
    if (!p5) { }		//	ok
#endif


    auto_ptr<Derive>	p6;
    if (p6 == p5) {	//	ok but dangerous! all of p6,p5 will be convert void*

    }

    //	Print3(p5);		//	'Print3' : cannot convert parameter 1 from 'auto_ptr<_Tp>' to 'Widget *'
    Print3(&*p5);		//	ok
    //	after operator _Tp* ()
    Print3(p5);

    Cassette* fun_music = new Cassette("alapalooza");
    CD* night_mare_music = new CD("disco hits of the 70s");
    displayer_adn_player(fun_music,10);
    displayer_adn_player(night_mare_music,10);

    auto_ptr<Cassette> fun_music2(new Cassette("alapalooza"));
    auto_ptr<CD> night_mare_music2(new CD("disco hits of the 70s"));
    //	after use member member function
    //	displayer_adn_player2(fun_music2,10);	//	cannot convert parameter 1 from 'auto_ptr<_Tp>' to 'const auto_ptr<_Tp> &'
    //	displayer_adn_player2(night_mare_music2,10);

    //	after use member member function
    displayer_adn_player2(fun_music2,10);			//	ok
    displayer_adn_player2(night_mare_music2,10);	//	ok

}
