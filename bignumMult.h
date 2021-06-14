#ifndef _BIGNUMMULT_H_
#define _BIGNUMMULT_H_

#include "bignumBase.h"

class bignumMult : public bignumBase 
{
 
    public:
    bignumMult() : bignumBase() {};
    bignumMult(unsigned precision) : bignumBase(precision) {};
    bignumMult(const bignumBase &original) : bignumBase(original) {};
    bignumMult(const string &linea, size_t precision) : bignumBase(linea, precision) {};
    ~bignumMult();

    bignumMult *nuevoBignum() const override ;
    bignumMult *clonarBignum() const  override ;

    bignumBase &operator*(const bignumBase &f1) override;

};



#endif