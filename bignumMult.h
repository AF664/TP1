#ifndef _BIGNUMMULT_H_
#define _BIGNUMMULT_H_

#include "bignumBase.h"

class bignumMult : public bignumBase 
{
 
    public:
    bignumMult() : bignumBase() {};
    bignumMult(size_t precision) : bignumBase(precision) {};
    bignumMult(const bignumBase &original) : bignumBase(original) {};
    bignumMult(const string &linea, size_t precision) : bignumBase(linea, precision) {};
    ~bignumMult() override;

    bignumMult *nuevoBignum() const override ;
    bignumMult *clonarBignum() const  override ;

    bignumBase &operator*=(const bignumBase &f1) override;
    friend bignumMult operator*(const bignumMult &factor1, const bignumMult &factor2);
    friend bignumMult operator+(const bignumMult &sumando1, const bignumMult &sumando2);
    friend bignumMult operator-(const bignumMult &minuendo, const bignumMult &sustraendo);
    // friend bignumMult &operator/(const bignumMult &dividendo, const bignumMult &divisor);

};



#endif