
/*  
    Archivo: bignumBase.cpp
    Archivo de definición de las funciones de la clase bignumBase
*/

#include "bignumBase.h"

string bignumBase::digitos() const{
    string str;
    if(this->good()){
        if(_signo == NEGATIVO) str = "-";
        for(size_t i=_largo-1 ; i < _largo  ; --i)
            str += ((char) (_digitos[i] + '0'));
    }else{
        str = "0";
    }
    cout << "Numero: " << str << endl;
    return str;
}


unsigned short *bignumBase::_crear_digitos(unsigned precision)
{
    unsigned short *digitos;
    
    digitos = precision ? new unsigned short[precision]: NULL;
    for( unsigned i=0; i< precision ; ++i)
        digitos[i]=0; 
   
    return digitos;
}

void bignumBase::_actualizar_largo()
{
    if( _precision > 0)
    {
        for(_largo = _precision -1; !_digitos[_largo] && _largo > 0 ; _largo --)
            ;
        _largo++;
    }
    else
        _largo = 0;
    if( cero() )
        _signo = POSITIVO;
}

void bignumBase::set_estado(status_t nuevo)
{
   
    if(nuevo != OK)
    {
        this->_poner_a_cero();
        _largo=0;
    }
    _estado = nuevo;
}

 void bignumBase::set_signo( signo_t signo)
 {
    _signo = signo;
 }


bignumBase::bignumBase()
{
    _digitos = NULL;
    _precision = 0;
    _largo = 0;
    _signo = POSITIVO;
    _estado = OK;

}

bignumBase::bignumBase(size_t precision)
{
     _digitos = _crear_digitos(precision);
    _precision = precision;
    _largo = (precision > 0)? 1 : 0;
    _signo = POSITIVO;
    _estado = OK;

}

bignumBase::bignumBase(const bignumBase &original)
{
    _precision = original._precision;
    _largo = original._largo;
    _digitos = _crear_digitos(_precision);
    _signo = original._signo;

    for(size_t i=0; i<_largo;i++)
        _digitos[i]=original._digitos[i];
    _estado=original._estado;
}

bignumBase::bignumBase(const string &linea, size_t precision)
{      
     _digitos = _crear_digitos(precision);
     _precision = precision;

     *this = linea;    

}

bignumBase::bignumBase(const string &linea)
{   
    string aux(linea);
    size_t longitud = stringDigits(aux);     
     _digitos = _crear_digitos(longitud);
     _precision = longitud;

     *this = linea;    

}

bignumBase::~bignumBase()
{
    if(_digitos!=NULL)
        delete [] _digitos;
    
    
}

size_t bignumBase::precision() const
{
    return _precision;
}

size_t bignumBase::largo() const
{
    return _largo;
}

signo_t bignumBase::signo() const
{
    return _signo;
}

status_t bignumBase::estado() const
{
    return _estado;
}
short bignumBase::digito(size_t pos) const
{   
    return (largo() > pos ) ? _digitos[pos] : 0;
}

bool bignumBase::good() const
{
    return (_estado == OK)? true : false;
}

bool bignumBase::cero() const
{
    if( _digitos == NULL)
        return true;
    return (_largo==1 && _digitos[0]==0) ? true : false;

}

bignumBase &bignumBase::operator=(const bignumBase &original)
{
    if(this != &original)
    {
        if(_precision != original._precision)
        {
            delete [] _digitos;
            _precision = original._precision;
            _digitos= new unsigned short[_precision];
        }
        for(unsigned i=0; i< original._precision; i++)
            _digitos[i]=original._digitos[i];
        _largo = original._largo;
        _estado = original._estado;
        _signo = original._signo;
    }
    return *this;
}

bignumBase &bignumBase::operator=(const string &linea)
{
    string aux(linea);
    size_t longitud = stringDigits(aux);      
 
    if( !(longitud) )   
    {
        this->_poner_a_cero();
        this->set_estado(ERROR_DIGITOS);  // Corrección de problema con identificación de 
    }                                     // números mal conformados.
    
    else if(  --longitud > _precision )
        this->set_estado(ERROR_PRECISION) ;
        

    else {
        this->set_estado(OK); 
        _signo = ( aux[0] == '+')? POSITIVO : NEGATIVO;
        _largo = longitud  ;
        for(int i=longitud, j =0 ; i>0 ; --i , ++j)
            _digitos[j]= aux[i] -'0' ;                       
       
        for( unsigned i=longitud; i< _precision ; i++)
            _digitos[i]= 0;
        this->_actualizar_largo();
        
        // corrijo el signo si la entrada fue -0
        if(this->cero())
            _signo= POSITIVO ;
        }

    return *this; 

}

bignumBase &bignumBase::operator=(int numero)
{
   unsigned digitos, i; // iteraor
   int aux = numero;

    for(digitos=1; aux/=10 ; digitos++ )
    ;
    if( digitos > _precision )
    {   
        this->set_estado(ERROR_PRECISION);
        return *this;
    }
  
    this->_poner_a_cero();

    if( numero < 0)
    {
        _signo = NEGATIVO;
        numero *= (-1);
    }

    for(i = 0 ; numero ; ++i ,  numero/= 10 )
        _digitos[i] = numero % 10 ;

    this->set_estado(OK);
    this->_actualizar_largo();
    return *this;

}


std::ostream& operator<<(std::ostream &fo,const bignumBase &numero)  
{
    if( numero.good())  
    {
        fo<< (char) ((numero._signo == POSITIVO) ? ' ':'-' ) ;

        for(size_t i=numero._largo-1 ; i < numero._largo  ; --i)  
            fo << (char) (numero._digitos[i] + '0');
    }
    else
        fo << "linea no procesada" << '\n';

    return fo;
     
}

bignumBase &bignumBase::_suma_sin_signo(bignumBase const &s2 , int &carry)
{
    // como se va a usar para sumar numeros que eventualmente estén representados
    // por su complemento, en el caso de los negativos, es necesario que tengan 
    // la misma precisión.
    
    if( !this->good() || 
        !s2.good() || 
        (_precision != s2._precision) )
            this->set_estado(NOK);
        
    else{
        carry=0;
        for( unsigned i = 0 , parcial = 0; i < _precision ; ++i)
        {
            parcial = _digitos[i] + s2._digitos[i]; 
            parcial += carry;
            _digitos[i]= (parcial > 9)? parcial % 10 : parcial;
            carry = parcial / 10;
        }
    }

    this->_actualizar_largo();
    return *this;

}

bignumBase &bignumBase::_complemento_base_10()
{
    unsigned i;
    int carry=1 , parcial=0 ;
    for( i=0; i < _precision ; ++i)
    {
        parcial = carry + 9;
        parcial  -= _digitos[i];
        _digitos[i] = ( parcial > 9)? parcial % 10 : parcial;
        carry = parcial / 10;
    }
    this->_actualizar_largo();
    return *this;
}

bignumBase &bignumBase::operator+=(const bignumBase &sumando1)
{
    // se hace el clonado porque no se puede instanciar la clase
    // bingumBase, ya que es abstracta.  
    bignumBase *s1 = sumando1.clonarBignum();
    int carry; 
    int signo = _signo + s1->_signo;
    if( !good() || !s1->good())
    {
        set_estado(NOK);
        delete s1;
        return *this;
    }

    if(_precision < s1->_precision)
        _cambiar_precision(s1->_precision);
    else if( _precision > s1->_precision)
        s1->_cambiar_precision(_precision);

    if( _signo == NEGATIVO)
        _complemento_base_10();
    if( s1->_signo == NEGATIVO)
        s1->_complemento_base_10();
    this->_suma_sin_signo( *s1 , carry );
    carry -= signo ;
    
    if ( carry == -1 || signo == 2)
    {  
        _signo = NEGATIVO;
        _complemento_base_10();
        _actualizar_largo();
    }
    else    
        _signo = POSITIVO;
    
    if (carry > 0)
    {        
        _actualizar_largo();
        _digitos[_largo]=1;
    }

    _actualizar_largo();
    
    delete s1;  

    return *this;

}

bignumBase &bignumBase::operator+=(int numero)
{
    bignumBase *aux = clonarBignum();
    *aux = numero;
    *this += *aux;
    delete aux;
    return *this;

}

bignumBase &bignumBase::operator-=(const bignumBase &sustraendo)
{
    bignumBase *aux = sustraendo.clonarBignum();
    aux->set_signo( (aux->signo() == POSITIVO)? NEGATIVO : POSITIVO  );
    *this += *aux;
    delete aux;
    return *this;
     
}



bignumBase &bignumBase::_poner_a_cero()
{
    if (_precision > 0)
    {
        for(; _largo != 0 ; ) 
            _digitos[--_largo]=0;
        _largo = 1;
    }
    this->set_estado(OK);
    _signo = POSITIVO;
    return *this;
}

bignumBase &bignumBase::_desplazamiento_izq(unsigned shift)
{
    unsigned i,j;

    if( shift + _largo > _precision )
        _cambiar_precision( shift + _largo);
    
    for(i= _precision , j =_precision - shift ;  j > 0 ; )
        _digitos[--i] = _digitos[--j];

    while ( i > 0)
        _digitos[--i]=0;
   
    _actualizar_largo();
    
    return *this; 

}

bignumBase &bignumBase::_desplazamiento_der(unsigned shift)
{
    unsigned i;
   
    if( shift > _largo )
        this->_poner_a_cero();
    
    else{
        for( i=0 ; shift < _largo ;)
            _digitos[i++] = _digitos[shift++];
        while(i< _largo)
            _digitos[i++]=0;
        _actualizar_largo();
    }
    return *this;
}

bignumBase &bignumBase::_mascara_der(unsigned digit)
{
    for(; digit < _largo; digit ++)
        _digitos[digit]=0;

    _actualizar_largo();
    return *this;
}

bignumBase &bignumBase::_cambiar_precision(size_t precision)
{
    unsigned short *tmp;
    size_t i;
    if( precision < _largo ) 
        this->set_estado(ERROR_OVERFLOW);
       
    else{
        tmp = new unsigned short[precision];
        for( i=0 ; i < _largo; i++)
            tmp[i]=_digitos[i];
        for( i=_largo ;i< precision; i++ )
            tmp[i] =0;
        delete [] _digitos;
        _digitos = tmp;
        this->set_estado(OK);
        _precision = precision;
    }
    return *this;
}

bignumBase &bignumBase::operator*=(int numero)
{
    unsigned i;
    bignumBase *acumulador = clonarBignum();
    bignumBase *aux = clonarBignum();
    if( numero < 0)
    {
        _signo = (_signo == POSITIVO)? NEGATIVO : POSITIVO;
        numero *= -1;
    }
    acumulador->_poner_a_cero();
    for(i=0 ; i < _largo  ; i++)
    {
        *aux = _digitos[i] * numero;
        *acumulador += aux->_desplazamiento_izq(i);
    }
    acumulador->_actualizar_largo();

    *this = *acumulador;

    delete acumulador;
    delete aux;
    
    return *this;
}

bignumBase &bignumBase::operator/=(const bignumBase &div)
{
    int i;
    bignumBase *resultado = this->clonarBignum();
    bignumBase *divisor = div.clonarBignum();
    signo_t signo_resultado  = ( signo() == div.signo() )? POSITIVO : NEGATIVO;

    if(divisor->cero()){
        delete resultado;
        delete divisor;
        this->set_estado(ERROR_DIVISION);
        this->_poner_a_cero();
        return *this;
    }

    set_signo( POSITIVO);
    divisor->set_signo(POSITIVO);

    if( *this < *divisor)
        this->_poner_a_cero();
    else 
    {
        resultado->_poner_a_cero();
        
        i = largo()- div.largo();
        resultado->_largo= i+1;
        for( divisor->_desplazamiento_izq(i) ; i>= 0 && !cero(); i--)
        {
            while(*divisor <= *this && !cero() )   
            {
                (resultado->_digitos[i])++ ;
                *this -= *divisor;
            }
            
            divisor->_desplazamiento_der(1);
        }
        *this = *resultado;
        set_signo(signo_resultado);
        _actualizar_largo();
    }    
    delete resultado;
    delete divisor;
    
    return *this;
    
}

std::istream& operator>>(std::istream &is ,bignumBase &num)
{
    string linea;
    is >> linea;
    if(is.eof())
        num._poner_a_cero();
    else    
        num = linea;
    return is;

}

bool operator<(bignumBase const &a, bignumBase const &b)
{
    bool resultado;
    bignumBase *aux = a.clonarBignum();
    *aux -=b;
    
    resultado = (aux->signo() == NEGATIVO) ;
    delete aux;
    return resultado;
}

bool operator>(bignumBase const &a, bignumBase const &b)
{
    bool resultado;
    bignumBase *aux = a.clonarBignum();
    *aux -=b;
    // puede ser cero y el signo estar en positivo
    resultado = (aux->signo() == NEGATIVO)? false : true;
    delete aux;
    return resultado;
}

bool operator==(bignumBase const &a, bignumBase const &b)
{
    bool resultado;
    bignumBase *aux = a.clonarBignum();
    *aux -=b;
    resultado = aux->cero();
    delete aux;
    return resultado;
}

bool operator<=(bignumBase const &a, bignumBase const &b)
{
    bool resultado;
    bignumBase *aux = a.clonarBignum();
    *aux -=b;
    resultado = (aux->signo() == NEGATIVO) || aux->cero();
    delete aux;
    return resultado;
   
    return (a >b || a == b);
}

bool operator>=(bignumBase const &a, bignumBase const &b)
{
    bool resultado;
    bignumBase *aux = a.clonarBignum();
    *aux -=b;
    resultado = (aux->signo() == POSITIVO) || aux->cero();
    delete aux;
    return resultado;
   
    return (a >b || a == b);
}


bignumBase &bignumBase::operator++(int)
{
   return *this +=1;
}


