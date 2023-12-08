#include "easy_cppunit_proxy.h"
//
// TestCase class
//
class TestItem3 : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestItem3);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestItem3);

//
// tests implementation
//
#include <iostream>
#include <iosfwd>
class BST {

};

class BalancedBST : public BST {

};

void PrintBSTArray(std::ostream& os,BST* bst_array, int num) {
    for (int i = 0; i < num; ++i) {
        os << "bst_array[i]";
    }
}

void DeleteBSTArray(std::ostream& os, BST bst_arr[]) {
    os << "delete BST array " ;
    delete [] bst_arr;
}

void TestItem3::test() {
    BST bst_array[10];
    PrintBSTArray(std::cout,bst_array,10);	//	ok

    BalancedBST balance_bst_array[10];
    PrintBSTArray(std::cout,balance_bst_array,10);	// is all right? it will be error at running time,BalancedBST 's size is more than BST 's size.
    //	ͨ��һ������ָ����ɾ��һ������������Ķ�������飬������ǲ�ȷ���ģ�
    BST* pbst = new BST [10];
    DeleteBSTArray(std::cout,pbst);

}
