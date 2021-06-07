
/*  
    Archivo: bignum.cpp
    Archivo de definición de las funciones de la clase bignum
*/

#include "bignum.h"


unsigned short *bignum::_crear_digitos(unsigned precision)
{
    unsigned short *digitos;
    
    digitos = precision ? new unsigned short[precision]: NULL;
    for( unsigned i=0; i< precision ; ++i)
        digitos[i]=0; 
    return digitos;
}

void bignum::_actualizar_largo()
{
    if( _precision > 0)
    {
        for(_largo = _precision -1; !_digitos[_largo] && _largo > 0 ; _largo --)
            ;
        _largo++;
    }
    else
        _largo = 0;
}

void bignum::set_estado(status_t nuevo)
{
   
    if(nuevo != OK)
    {
        this->_poner_a_cero();
        _largo=0;
    }
    _estado = nuevo;
}

bignum::bignum()
{
    _digitos = NULL;
    _precision = 0;
    _largo = 0;
    _signo = POSITIVO;
    _estado = OK;

}

bignum::bignum(unsigned precision)
{
    _digitos = _crear_digitos(precision);
    _precision = precision;
    _largo = (precision > 0)? 1 : 0;
    _signo = POSITIVO;
    _estado = OK;

}

bignum::bignum(const bignum &original)
{
    _precision = original._precision;
    _largo = original._largo;
    _digitos = _crear_digitos(_precision);
    _signo = original._signo;

    for(size_t i=0; i<_largo;i++)
        _digitos[i]=original._digitos[i];
    _estado=original._estado;
}

bignum::bignum(const string &linea, size_t precision)
{
    
     _digitos = _crear_digitos(precision);
     _precision = precision;

     *this = linea;    

}

bignum::~bignum()
{
    if(_digitos!=NULL)
        delete [] _digitos;
    
}

size_t bignum::precision() const
{
    return _precision;
}

size_t bignum::largo() const
{
    return _largo;
}

signo_t bignum::signo() const
{
    return _signo;
}

status_t bignum::estado() const
{
    return _estado;
}

bool bignum::good() const
{
    return (_estado == OK)? true : false;
}

bool bignum::cero() const
{
    return (_largo==1 && _digitos[0]==0) ? true : false;

}

bignum &bignum::operator=(const bignum &original)
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

bignum &bignum::operator=(const string &linea)
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

bignum &bignum::operator=(int numero)
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


std::ostream& operator<<(std::ostream &fo,const bignum &numero)  
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

bignum &bignum::_suma_sin_signo(bignum const &s2 , int &carry)
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

bignum &bignum::_complemento_base_10()
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

bignum operator+(const bignum &sumando1 , const bignum &sumando2)
{
    // podría modificar la interfaz y hacer que pasen por copia
    // pero por una cuestion de coherencia lo dejamos así
    bignum s1(sumando1);
    bignum s2(sumando2);

    int carry; 
    int signo = s1._signo + s2._signo;
    if( !s1.good() || !s2.good())
    {
        s1.set_estado(NOK);
        return s1;
    }

    if( s1._signo == NEGATIVO)
        s1._complemento_base_10();
    if( s2._signo == NEGATIVO)
        s2._complemento_base_10();
    
    s1._suma_sin_signo( s2 , carry );

    signo -= carry ;
    if (signo < 0)
        s1.set_estado(ERROR_OVERFLOW);
    else if ( signo == 1)
    {  
        s1._signo = NEGATIVO;
        s1._complemento_base_10();
    }
    else    
        s1._signo = POSITIVO;
    s1._actualizar_largo();

    return s1;

}


bignum operator-(const bignum &minuendo, const bignum &sustraendo)
{
    bignum resta(sustraendo);
    if( !resta.cero())
        resta._signo = (resta._signo == POSITIVO) ? NEGATIVO : POSITIVO;
    return minuendo + resta;

}

bignum &bignum::_poner_a_cero()
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

bignum &bignum::_desplazamiento_izq(unsigned shift)
{
    unsigned i,j;
    
    if( shift >= _precision)
    {
        this->set_estado(ERROR_OVERFLOW);
        return *this;
    }
    for(i= _precision , j =_precision - shift ;  j > 0 ; )
        _digitos[--i] = _digitos[--j];

    while ( i > 0)
        _digitos[--i]=0;
   
    this->set_estado( (shift > (_precision - _largo))? ERROR_OVERFLOW : OK );
    this->_actualizar_largo();
    return *this; 

}
bignum &bignum::operator+=(const bignum &sumando)
{
    *this = *this + sumando;
    return *this ;
}
bignum &bignum::_cambiar_precision(size_t precision)
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

bignum &bignum::operator*=(int numero)
{
    unsigned i;
    bignum acumulador(_precision);
    bignum aux(_precision );
    
    if( numero < 0)
    {
        _signo = (_signo == POSITIVO)? NEGATIVO : POSITIVO;
        numero *= -1;
    }
    acumulador._poner_a_cero();
    for(i=0 ; i < _largo ; i++)
    {
        aux = _digitos[i] * numero;
        acumulador += aux._desplazamiento_izq(i);
    }
    acumulador._actualizar_largo();
    *this = acumulador;
   

    return *this;
}

bignum operator*(const bignum &factor1, int mult)
{
    bignum aux(factor1);
    aux*= mult;

    return aux;
}

bignum operator*(const bignum &f1, const bignum &f2)
{
    bignum mult(f1._precision );
    bignum aux(f1._precision );

    unsigned i; // iterador

    if( f2._largo == 0 ||  f1._largo == 0)
        return mult;
 
    for(i=0; i < f2._largo && mult.good() ; i++ )
    {
        aux = f1;
        aux*= f2._digitos[i];
        aux._desplazamiento_izq(i);
        mult += aux;
    }
    if( f1._signo == NEGATIVO)
        mult._signo = (f2._signo == NEGATIVO) ? POSITIVO : NEGATIVO;
    else
        mult._signo = (f2._signo == NEGATIVO ) ? NEGATIVO : POSITIVO;

    mult._actualizar_largo();
    return mult;
}

std::istream& operator>>(std::istream &is ,bignum &num)
{
    string linea;
    is >> linea;
    if(is.eof())
        num._poner_a_cero();
    else    
        num = linea;
    return is;

}

#ifdef _TEST_BIGNUM
int main()
{
    bignum numero1(20);
    bignum numero2(20);
    bignum operacion;
    string linea("1234");
    //int operando;

    while( !cin.eof())
    {
       

        cout << "Prueba suma: ingrese primer valor"<< '\n';
        getline(cin,linea);
        numero1=linea;
        cout << "Prueba suma: ingrese segundo valor"<< '\n';
        getline(cin,linea);
        numero2=linea;
        operacion = numero1 + numero2;
        cout << numero1 << " + " << numero2 << " = " << operacion << '\n';
        cout << "estado: " << operacion.estado() << '\n';

        cout << "Prueba resta: ingrese primer valor"<< '\n';
        getline(cin,linea);
        numero1=linea;
        cout << "Prueba resta: ingrese segundo valor"<< '\n';
        getline(cin,linea);
        numero2=linea;
        operacion = numero1 - numero2;
        
        cout << numero1 << " - " << numero2 << " = " << operacion << '\n';
        cout << "estado: " << operacion.estado() << '\n';

       
        cout << "Prueba multiplicacion: ingrese primer valor"<< '\n';
        getline(cin,linea);
        numero1=linea;
        cout << "Prueba multiplicacion: ingrese segundo valor"<< '\n';
        getline(cin,linea);
        numero2=linea;
        operacion = numero1 * numero2;
        
        cout << numero1 << " * " << numero2 << " = " << operacion << '\n';
        cout << "estado: " << operacion.estado() << '\n';
       
       cout << "Prueba >>: ingrese un valor" << '\n';
       cin >> numero1;
       cout << "el valor ingresado es: " << numero1 << '\n';

    }

    return 0;
    
}
#endif
