#ifndef _BIGNUMKARAT_H_
#define _BIGNUMKARAT_H_

#include "bignumBase.h"

#define UMBRAL_KARAT 2

class bignumKarat : public bignumBase 
{
    private:

    // multiplica el dígito d (d tiene que estar dentro del rango 0~9)
    // la intención es que sea una función rápida de multiplicación para
    // la implementación de la multiplicación con el algoritmo Karatsuba
    bignumKarat &mult_digito(int d);

    // si puede, devuelve el módulo (valor sin signo) del numero
    // convertido a entero. Si la longitud del límite excede 
    // a la de la representación del entero, devuelve 
    // std::numeric_limits<int>::max() 
    int modulo_int() const;

    // Multiplica factor1 con factor2 por el método de Karatsuba
    bignumKarat karatsuba(bignumKarat factor1, bignumKarat factor2);

    // pone en cero los dígitos indicados desde inicio hasta fin
    bignumKarat borrar_digitos(size_t inicio, size_t fin) const;
 
    public:
    bignumKarat() : bignumBase() {};
    bignumKarat(size_t precision) : bignumBase(precision) {};
    bignumKarat(const bignumBase &original) : bignumBase(original) {};
    bignumKarat(const string &linea, size_t precision) : bignumBase(linea, precision) {};
    ~bignumKarat() override;
    
    // Son funciones obligatorias declaradas en la clase madre bignumBase
    // se utilizan para poder compartir métodos comunes a las implementaciones
    // hijas.
    bignumKarat *nuevoBignum() const override ;
    bignumKarat *clonarBignum() const  override ;

    
    bignumBase &operator*=(const bignumBase &f1) override;
    bignumKarat &operator*=(int numero);
    bignumKarat &operator=(const bignumKarat &original);
    bignumKarat &operator=(int numero);

    //friend bignumKarat operator*(const bignumKarat &factor1, const bignumKarat &factor2);
    friend bignumKarat operator+(const bignumKarat &sumando1, const bignumKarat &sumando2);
    friend bignumKarat operator-(const bignumKarat &minuendo, const bignumKarat &sustraendo);
    // friend bignumKarat &operator/(const bignumKarat &dividendo, const bignumKarat &divisor);

};



#endif