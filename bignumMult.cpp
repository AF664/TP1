#include "bignumBase.h"
#include "bignumMult.h"

bignumMult::~bignumMult()
{
    
}


 bignumMult *bignumMult::nuevoBignum() const
 {
     return new bignumMult();

 }

 bignumMult *bignumMult::clonarBignum() const
 {
     return new bignumMult(*this);
 }



bignumBase &bignumMult::operator*(const bignumBase &f1)
{
    bignumMult mult(*this);
    bignumMult aux(f1);

    unsigned i; // iterador

    if( largo() == 0 ||  f1.largo() == 0)
        return *this;
 
    for(i=0; i < largo() && mult.good() ; i++ )
    {
        aux = f1;
        aux*= digito(i);
        aux._desplazamiento_izq(i);
        mult += aux;
    }
    if( f1.signo() == NEGATIVO)
        mult.set_signo( (signo() == NEGATIVO) ? POSITIVO : NEGATIVO );
    else
        mult.set_signo( (signo() == NEGATIVO ) ? NEGATIVO : POSITIVO);

    *this = mult;
    _actualizar_largo();

    return *this;
}
