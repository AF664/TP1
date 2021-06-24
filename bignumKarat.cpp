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


bignumBase &bignumKarat::operator*=(const bignumBase &f1)
{
    signo_t signo = (this->signo() == f1.signo()) ? POSITIVO : NEGATIVO; 
    *this= karatsuba(*this,f1);
    this->set_signo(signo);
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

bignumKarat operator-(const bignumKarat &sustraendo, const bignumKarat &minuendo)
{
    bignumKarat aux(sustraendo);
    aux -= minuendo;
    return aux;
}



// Falta asignar signo pero lo pondria por fuera del metodo
bignumKarat bignumKarat::karatsuba(bignumKarat factor1, bignumKarat factor2){
    size_t m = min(factor1.largo(), factor2.largo()) / 2;
    // Multiplicacion entre Bignum y un digito (int)
    if(factor1.largo() < 2) return (factor2 *= int(factor1.digito(0))); 
    if(factor2.largo() < 2) return (factor1 *= int(factor2.digito(0)));
    // Creo los subnumeros
    bignumKarat low1 = factor1.borrar_digitos(0, m);
    bignumKarat low2 = factor2.borrar_digitos(0, m);    
    bignumKarat high1 = factor1.borrar_digitos(m, factor1.largo());
    bignumKarat high2 = factor2.borrar_digitos(m, factor2.largo());
    
    // Llamadas a karatsuba 
    bignumKarat z0 = karatsuba(low1, low2);
    bignumKarat z1 = karatsuba((low1 + high1), (low2 + high2));
    bignumKarat z2 = karatsuba(high1, high2);
    // Operaciones finales
    bignumKarat z3 = ((z1 - z2) - z0);
    z3._desplazamiento_izq(m);
    z3._actualizar_largo();
    // Desplazo Z2 
    z2._desplazamiento_izq((m)*2);
    z2._actualizar_largo();
    // return = (z2*10^(2*m)) + ((z1-z2-z0)*10^m) + z0
    return ( z2 + z3 + z0);
}

// Este metodo  devuelve un bignum igual a "numero" pero recortando los
// digitos entre [inicio, fin] 
bignumKarat bignumKarat::borrar_digitos(size_t inicio, size_t fin) const{
    bignumKarat aux = bignumKarat(this->precision());
    // Copia un cero en el ultimo digito cuando crea la parte alta,
    // no es problema porque despues se actualiza su tamaño 
    for (size_t i = inicio; i < fin; i++){
        aux.set_digito(this->digito(i),i-inicio);
    }
    aux._actualizar_largo();
    return aux;
}

bignumKarat &bignumKarat::operator*=(int numero)
{
    bignumBase *producto=this;
    *producto *= numero;
    return *this;
}
bignumKarat &bignumKarat::operator=(const bignumKarat &original)
{
    bignumBase *destino = this;
    bignumBase const *origen = &original;
    *destino = *origen;
    return *this;
}

bignumKarat &bignumKarat::operator=(int numero)
{
    bignumBase *destino=this;
    *destino = numero;
    return *this;
}