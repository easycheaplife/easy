/********************************************************************
	created:	2011/10/26
	created:	26:10:2011   10:10
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_big_int.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_big_int
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_big_int_h__
#define easy_big_int_h__

#include <iosfwd>

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

#ifndef easy_ref_count_object_h__
#include "easy_ref_count_object.h"
#endif easy_ref_count_object_h__


namespace easy
{
	class EasyBigInt : public EasyRefCountObject
	{
		friend EasyBigInt operator + (const EasyBigInt&, const EasyBigInt&);
	public:
		EasyBigInt(const easy_int8*);

		EasyBigInt(easy_uint32 = 0);

		EasyBigInt(const EasyBigInt& copy_from);

		EasyBigInt& operator = (const EasyBigInt&);

		EasyBigInt& operator += (const EasyBigInt&);

		~EasyBigInt();

		easy_int8* digits() const { return digits_; }

		easy_uint32 ndigits() const { return ndigits_; }
	private:
		easy_int8*	digits_;

		easy_uint32 ndigits_;

		easy_uint32 size_;

		EasyBigInt(const EasyBigInt&,const EasyBigInt&);

		easy_int8 _Fetch(easy_uint32 pos) const ;
	};
}
#endif // easy_big_int_h__