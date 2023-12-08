#include "easy_cppunit_proxy.h"
#include "easy_obj_pool.h"
#include "easy_mutex.h"
//
// TestCase class
//
class TestObjPool : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestObjPool);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestObjPool);

//
// tests implementation
//
#include <vector>
class CObject {
    std::vector<int>	v1;
  public:
    void test() {
        for (int i = 0; i < 1000; ++i) {
            v1.push_back(i);
        }
        v1.clear();
#if 0
        //	if not do this, memory leaks.
        std::vector<int>(v1).swap(v1);
#endif
    }
};

void TestPool() {
    //	old style
#if 0
    easy::EasyObjectPool<CObject*>* pObjPool = new easy::EasyObjectPool<CObject*>(sizeof(CObject));
    for (;;) {
        CObject* pNewObject = pObjPool->allocate();
        memset(pNewObject,0,sizeof(CObject));
        pNewObject->test();
        pObjPool->destrory(pNewObject);
        ::_sleep(10);
    }
#else
    for (;;) {
        CObject* pNewObject = new CObject;
        if (pNewObject) {
            pNewObject->test();
            ::_sleep(10);
            delete pNewObject;
        }
    }

#endif


}

void TestObjPool::test() {
    TestPool();
}
