#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include "myadd.h"
#include "easy_big_int.h"
#include "easy_log.h"
#include "easy_exception.h"
#include "easy_thread_mutex.h"
#include "easy_threading.h"
#include "easy_mutex.h"
#include "easy_obj_pool.h"


class ThreadTest : public easy::EasyThread {
  public:
    ThreadTest() {}
    ~ThreadTest() {}
  private:
    easy_int32 _Run( void* p ) {
        static int index = 0;
        while(1) {
            ++index;
            std::cout << index << std::endl;
            sleep(1);
        }
        return 0;
    }
};

#include <vector>
class CObject {
    std::vector<int>	v1;
  public:
    void test() {
        for (int i = 0; i < 1000; ++i) {
            v1.push_back(i);
        }
        v1.clear();
#if 1
        //	if not do this, memory leaks.
        std::vector<int>(v1).swap(v1);
#endif
    }
};

void TestPool() {
#if 1
    easy::EasyObjectPool<CObject*>* pObjPool = new easy::EasyObjectPool<CObject*>(sizeof(CObject));
    for (;;) {
        CObject* pNewObject = pObjPool->allocate();
        memset(pNewObject,0,sizeof(CObject));
        pNewObject->test();
        pObjPool->destrory(pNewObject);
        sleep(1);
    }
#else
    for (;;) {
        CObject* pNewObject = new CObject;
        if (pNewObject) {
            pNewObject->test();
            sleep(1);
            delete pNewObject;
        }
    }

#endif
}

#define UNIT_TEST_RECORDS			10
using namespace easy;

int main() {
    std::cout<<"use static lib at linux"<<std::endl;
    std::vector<int> v1;
    v1.push_back(1000);
    std::cout<<"first use stl,the first element of vector is " << v1[0] <<std::endl;
    int a = 3;
    int b = 4;
    int c = myadd(a,b);
    std::cout<<"a + b = "<< c << std::endl;

    std::cout<<"test EasyBigInt"<<std::endl;
    EasyBigInt aa = 10;

    easy::EasyLog::SaveLog("./test.log",easy::kErrors,"test log.................%d",12);

    int aaa = 10;
    int bbb = 0;
    try {
        if(0 == bbb) {
            EASY_EXCEPTION( EasyException::kErrorInvalidParams,"file not find!", "just a test", kExceptionLevText );
        }
    } catch(...) {

    }

    EasyThreadMutex	mutex;
    mutex.Acquire();
    mutex.Release();

    easy::EasyMutex	mtx;
    mtx.lock();
    mtx.unlock();
    easy::EasyGuard<easy::EasyMutex> guard(mtx);

#if 0
    TestPool();
#endif

    ThreadTest* thread = new ThreadTest();
    while(1) {
        sleep(1);
    }
    delete thread;

    return 0;
}
