/********************************************************************
	created:	2011/10/26
	created:	26:10:2011   12:22
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_ref_count_big_int.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_ref_count_big_int
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_ref_count_big_int_h__
#define easy_ref_count_big_int_h__

#ifndef easy_big_int_h__
#include "easy_big_int.h"
#endif //easy_big_int_h__

#ifndef easy_ref_count_ptr_h__
#include "easy_ref_count_ptr.h"
#endif //easy_ref_count_ptr_h__

namespace easy {
class EasyRefCountBigInt {
    friend EasyRefCountBigInt operator + (const EasyRefCountBigInt&, const EasyRefCountBigInt&);
  public:
    EasyRefCountBigInt(const easy_int8* p) : value_( new EasyBigInt(p)) { }

    EasyRefCountBigInt(easy_uint32 u = 0) : value_ ( new EasyBigInt(u) ) { }

    EasyRefCountBigInt(const EasyBigInt& bi) : value_( new EasyBigInt(bi) ) { }

  private:
    EasyRefCountPtr<EasyBigInt> value_;
};

EasyRefCountBigInt operator+( const EasyRefCountBigInt& left, const EasyRefCountBigInt& right) {
    return EasyRefCountBigInt(*(left.value_) + *(right.value_));
}
}
#endif // easy_ref_count_big_int_h__

