#include "easy_big_int.h"
#include <string.h>
#include <iostream>
namespace easy {
EasyBigInt::EasyBigInt( easy_uint32 /*= 0*/ u ) {
    easy_uint32 v = u;
    for (ndigits_ = 1; (v/=10) > 0; ndigits_++ ) {
        ;
    }
    digits_ = new easy_int8[size_ = ndigits_];
    for (easy_uint32 i = 0; i < ndigits_; ++i) {
        digits_[i] = u % 10;
        u /= 10;
    }
}

EasyBigInt::EasyBigInt( const easy_int8* s ) {
    if (s[0] == '\0') {
        s = "0";
    }
    size_ = ndigits_ = strlen(s);
    digits_ = new easy_int8[ndigits_];
    for (easy_uint32 i = 0; i < ndigits_; ++i) {
        digits_[i] = s[ndigits_ - 1 - i] ;
    }
}

EasyBigInt::EasyBigInt( const EasyBigInt& copy_from) {
    size_ = ndigits_ = copy_from.ndigits_;
    digits_ = new easy_int8[ndigits_];
    for (easy_uint32 i = 0; i < ndigits_; ++i) {
        digits_[i] = copy_from.digits_[i] ;
    }
}

//	operational construct, bigint  = left + right
EasyBigInt::EasyBigInt( const EasyBigInt& left,const EasyBigInt& right) {
    size_ = 1 + (left.ndigits_ > right.ndigits_ ? left.ndigits_: left.ndigits_);
    digits_ = new easy_int8[size_];
    ndigits_ = left.ndigits_;
    for (easy_uint32 i = 0; i < ndigits_; ++i) {
#pragma warning(suppress: 6386)
        digits_[i] = left.digits_[i] ;
    }
    *this += right;
}

EasyBigInt::~EasyBigInt() {
    if (digits_) {
        delete [] digits_;
        digits_ = easy_null;
    }
}

EasyBigInt& EasyBigInt::operator=( const EasyBigInt& rhs) {
    if ( this == &rhs ) {
        return *this;
    }
    ndigits_ = rhs.ndigits();
    if ( ndigits_ > size_ ) {
        delete [] digits_;
        digits_ = new easy_int8[ndigits_];
    }
    for ( easy_uint32 i = 0; i < ndigits_; ++i ) {
        digits_[i] = rhs.digits_[i];
    }
    return *this;
}

EasyBigInt& EasyBigInt::operator+=( const EasyBigInt& rhs) {
    easy_uint32 max = 1 + ( rhs.ndigits_ > ndigits_ ? rhs.ndigits_ : ndigits_);
    if (size_ < max) {
        easy_int8* d = new easy_int8[size_ = max];
        for (easy_uint32 i = 0; i < ndigits_; ++i) {
            d[i] = digits_[i];
        }
        delete [] digits_;
        digits_ = d;
    }
    while(ndigits_ < max) {
        digits_[ndigits_++] = 0;
    }
    for ( easy_uint32 i = 0; i < ndigits_; ++i ) {
        digits_[i] += rhs._Fetch(i);
        if (digits_[i] >= 10) {
            digits_[i] -= 10;
        }
        digits_[i + 1] += 1;

        if (digits_[ndigits_ - 1] == 0) {
            --ndigits_;
        }
    }
    return *this;
}

inline easy_int8 EasyBigInt::_Fetch( easy_uint32 pos ) const {
    return 1 < ndigits_ ? digits_[pos] : 0;
}

std::ostream& operator << (std::ostream& os, EasyBigInt& bi) {
    char c;
    const char* d = bi.digits();
    for( easy_uint32 i = 0; i < bi.ndigits() - 1; ++i ) {
        c = d[i] + '0';
        os << c;
    }
    os << std::endl;
    return os;
}

EasyBigInt operator+( const EasyBigInt& left, const EasyBigInt& right) {
    return EasyBigInt(left,right);
}
}

