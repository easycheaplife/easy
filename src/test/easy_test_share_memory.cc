#include "easy_cppunit_proxy.h"
#include"slim_allocator.h"
//
// TestCase class
//
class TestSahreMemory : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestSahreMemory);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSahreMemory);

//
// tests implementation
//

struct ARole {
    unsigned int oid;
    char		sz_name[16];

    /*virtual*/ void		foo() {
        SLIM_LOG1("\nARole::%d\n",oid);
    }
};

struct ROLE : public ARole {

    void		foo() {
        SLIM_LOG1("\nROLE::%d\n",oid);
    }
};

void TestSahreMemory::test() {
    //	create
#ifdef CREATE_SHARE_MEMORY
    std::map<int,ROLE,std::less<int>,slim_allocator<std::pair<int, ROLE> > >* _map =  commom_pool->get_struct<std::map<int,ROLE,std::less<int>,slim_allocator<std::pair<int, ROLE> > > >(STRUCT_INDEX_ROLE);
    ROLE _role;
    _role.oid = 10;
    strcpy_s(_role.sz_name,16,"sxsx");
    (*_map)[1] = _role;
    ROLE* __p = &(*_map)[1];
    __p->foo();
    system("pause");
#else
    //	use
    std::map<int,ROLE,std::less<int>,slim_allocator<std::pair<int, ROLE> > >* _map =  commom_pool->get_struct<std::map<int,ROLE,std::less<int>,slim_allocator<std::pair<int, ROLE> > > >(STRUCT_INDEX_ROLE);
    ROLE* __p = &(*_map)[1];
    __p->foo();
#endif

}