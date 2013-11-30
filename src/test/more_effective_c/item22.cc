#include "easy_cppunit_proxy.h"
#include <iostream>
#include <list>
//
// TestCase class
//
class TestItem22 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem22);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem22);

//
// tests implementation
//

class NLComponent
{
public:
	virtual NLComponent* clone() const = 0;

	virtual std::ostream& operator<< (std::ostream& os) const = 0;
};

class TextBook : public NLComponent
{
public:
	virtual TextBook* clone() const { return new TextBook(*this); }

	std::ostream& operator<< (std::ostream& os) const
	{
		os << "TextBook-std::ostream& operator<< (std::ostream& os) const";
		return os;
	}
};

class Graphic : public NLComponent
{
public:
	virtual Graphic* clone() const  { return new Graphic(*this); }

	std::ostream& operator<< (std::ostream& os) const
	{
		os << "Graphic-std::ostream& operator<< (std::ostream& os) const";
		return os;
	}
};

class NewsLetter
{
public:
	NewsLetter(std::istream& os)
	{
		NLComponent* component = NULL;
		while (os)
		{
			component = _read_component(os);
		}
		list_.push_back(component);
	}

	NewsLetter(const NewsLetter& rhs)
	{
		for (std::list<NLComponent*>::iterator it = list_.begin(); it != list_.end(); ++it)
		{
			list_.push_back((*it)->clone());
		}
	}

	static NLComponent* _read_component(std::istream& os)
	{
		NLComponent* component = NULL;
		int read = 0;
		os >> read;
		if ( 1 == read)
		{
			component = dynamic_cast<NLComponent*>(new TextBook());
		}
		else if (2 == read)
		{
			component = dynamic_cast<NLComponent*>(new Graphic()); 
		}
		return component;
	}
private:
	std::list<NLComponent*>		list_;
};

void TestItem22::test()
{
#if 0
	NewsLetter new_letter(std::cin);
#endif

	TextBook t;
	Graphic g;
	t << std::cout;
	g << std::cout;
}
