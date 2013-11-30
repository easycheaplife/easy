#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem4 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestItem4);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem4);

//
// tests implementation
//

class EquipmentPrice
{
public:
	EquipmentPrice(int price) { }
};

void TestItem4::test()
{
	//	EquipmentPrice equipment_price_array[10];	//	error C2512: 'EquipmentPrice' : no appropriate default constructor available
	//	EquipmentPrice* p_equipment_price = new EquipmentPrice [10]; //	error C2512: 'EquipmentPrice' : no appropriate default constructor available
	int id1 = 0;
	int id2 = 1;
	int id3 = 2;
	EquipmentPrice equipment_price_array[3] = { EquipmentPrice(id1),EquipmentPrice(id2),EquipmentPrice(id3) };	//	ok

	typedef EquipmentPrice* PEP;
	PEP best_price[10];	// ok

	PEP* p_best_price = new PEP[10];	//ok
	for ( int i = 0; i < 10; ++i)
	{
		best_price[i] = new EquipmentPrice(i);
		p_best_price[i] = new EquipmentPrice(i);
	}
	
	void* raw_memory = operator new [] (10*sizeof(EquipmentPrice));
	EquipmentPrice* best_price2 = static_cast<EquipmentPrice*>(raw_memory);
	for ( int i = 0; i < 10; ++i)
	{
		// new (&best_price2[i]) EquipmentPrice(i);	//	error! why? 
	}

	for ( int i = 9; i >= 0; --i)
	{
		best_price2[i].~EquipmentPrice();
	}
	operator delete [] (raw_memory);
}
