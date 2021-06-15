#include "bignumBase.h"
#include "bignumKarat.h"
#include <limits>

bignumKarat::~bignumKarat()
{}

// si puede, devuelve el módulo (valor sin signo) del numero
// convertido a entero. Si la longitud del límite excede 
// a la de la representación del entero, devuelve 
// std::numeric_limits<int>::max() 
int bignumKarat::modulo_int() const
{
    int numero;
    int peso;
    size_t i;
    if( largo() > std::numeric_limits<int>::digits10)
        return std::numeric_limits<int>::max();
    for(peso=1 , numero =0, i=0 ; i < largo(); i++ , peso*=10)
        numero+= digito(i)*peso;
    return numero;

 }



 bignumKarat *bignumKarat::nuevoBignum() const 
 {
     return new bignumKarat();

 }

 bignumKarat *bignumKarat::clonarBignum() const
 {
     return new bignumKarat(*this);
 }


bignumKarat bignumKarat::mult_karatsuba(const bignumKarat &x, const bignumKarat &y)
{
    const bignumKarat *pmin, *pmax;
    bignumKarat resultado;
    signo_t sig = (x.signo() != y.signo() ) ? NEGATIVO: POSITIVO;
    pmin = (x.largo() < y.largo())? &x : &y;
    pmax = (x.largo() < y.largo())? &y : &x;
 
    if( pmin->largo() <= UMBRAL_KARAT)
    {
        bignumBase *pbn = pmax->clonarBignum();
        int aux = pmin->modulo_int();
        *pbn *= aux;
        resultado = *pbn;
        delete pbn;

        resultado.set_signo( sig );
        return resultado;
    }
    bignumKarat x1(x), x0(x), y1(y) , y0(y);
    bignumKarat z0,z1,z2;
    x1._desplazamiento_der(UMBRAL_KARAT);
    
    x0._mascara_der(UMBRAL_KARAT);

    y1._desplazamiento_der(UMBRAL_KARAT);
    y0._mascara_der(UMBRAL_KARAT);
    z2= mult_karatsuba(x1,y1);
    z0= mult_karatsuba(x0,y0);
    z1= mult_karatsuba(x1+x0, y1+y0);
    z1 -= z0 + z2;
    resultado = z2._desplazamiento_izq(UMBRAL_KARAT*2);
    resultado += z1._desplazamiento_izq(UMBRAL_KARAT) ;
    resultado += z0;
    resultado.set_signo(sig);
    return resultado;
   
}

bignumBase &bignumKarat::operator*=(const bignumBase &f1)
{
    *this=mult_karatsuba(*this,f1);
    return *this;
}
/*
bignumKarat operator*(const bignumKarat &factor1, const bignumKarat &factor2)
{
    return bignumKarat::mult_karatsuba(factor1,factor2);
}
*/

bignumKarat operator+(const bignumKarat &s1, const bignumKarat &s2)
{
    bignumKarat aux(s1);
    aux += s2;
    return aux;
}

bignumKarat operator-(const bignumKarat &minuendo, const bignumKarat &sustraendo)
{
    bignumKarat aux(sustraendo);
    aux -= minuendo;
    return aux;
}
