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



bignumBase &bignumMult::operator*=(const bignumBase &f1)
{
    bignumMult mult(*this);
    bignumBase *aux = f1.clonarBignum();

    size_t i; // iterador

    if( largo() == 0 ||  f1.largo() == 0)
        this->_poner_a_cero();

    else{
        mult._poner_a_cero();
        for(i=0; i < largo() && mult.good() ; i++ )
        {
            *aux = f1;
            *aux *= digito(i);
            aux->_desplazamiento_izq(i);
            mult += *aux;
        }
        
        if( f1.signo() == NEGATIVO)
            mult.set_signo( (signo() == NEGATIVO) ? POSITIVO : NEGATIVO );
        else
            mult.set_signo( (signo() == NEGATIVO ) ? NEGATIVO : POSITIVO);

        *this = mult;
        _actualizar_largo();
    }

    delete aux;

    return *this;
}

bignumMult operator*(const bignumMult &factor1, const bignumMult &factor2)
{
    bignumMult aux(factor1);
    aux *= factor2;
    return aux;
}


bignumMult operator+(const bignumMult &s1, const bignumMult &s2)
{
    bignumMult aux(s1);
    aux += s2;
    return aux;
}

bignumMult operator-(const bignumMult &minuendo, const bignumMult &sustraendo)
{
    bignumMult aux(sustraendo);
    aux -= minuendo;
    return aux;
}
