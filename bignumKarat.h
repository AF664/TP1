#ifndef _BIGNUMKARAT_H_
#define _BIGNUMKARAT_H_

#include "bignumBase.h"

#define UMBRAL_KARAT 3

class bignumKarat : public bignumBase 
{
 
    public:
    bignumKarat() : bignumBase() {};
    bignumKarat(size_t precision) : bignumBase(precision) {};
    bignumKarat(const bignumBase &original) : bignumBase(original) {};
    bignumKarat(const string &linea, size_t precision) : bignumBase(linea, precision) {};
    ~bignumKarat() override;
    
    bignumKarat *nuevoBignum() const override ;
    bignumKarat *clonarBignum() const  override ;

    // si puede, devuelve el módulo (valor sin signo) del numero
    // convertido a entero. Si la longitud del límite excede 
    // a la de la representación del entero, devuelve 
    // std::numeric_limits<int>::max() 
    int modulo_int() const;

    bignumKarat mult_karatsuba(const bignumKarat &x, const bignumKarat &y);
    bignumKarat karatsuba(bignumKarat factor1, bignumKarat factor2);
    bignumKarat borrar_digitos(size_t inicio, size_t fin) const;
    bignumBase &operator*=(const bignumBase &f1) override;
    bignumKarat operator/=(int numero);     // cambiar /= por +=
    //friend bignumKarat operator*(const bignumKarat &factor1, const bignumKarat &factor2);
    friend bignumKarat operator+(const bignumKarat &sumando1, const bignumKarat &sumando2);
    friend bignumKarat operator-(const bignumKarat &minuendo, const bignumKarat &sustraendo);
    // friend bignumKarat &operator/(const bignumKarat &dividendo, const bignumKarat &divisor);

};



#endif