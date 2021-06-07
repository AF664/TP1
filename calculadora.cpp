// archivo calculadora.cpp

#include "calculadora.h"

using namespace std ;

char DiccionarioOperaciones[]= {'+','-','*','\0'};

calculadora::calculadora(unsigned precision =0 )
{   
    
    _operando1 = bignum(precision);
    _operando2 = bignum(precision);
    _estado = OK ;
    _operacion = NO_OP;
}

calculadora::calculadora()
{
    _operando1 = bignum(0);
    _operando2 = bignum(0);
     _estado = OK ;
    _operacion = NO_OP;
}

calculadora::~calculadora()
{}


void calculadora::set_operacion(operacion_t op)
{
    _operacion=op;
}

status_t calculadora::estado()
{
    return _estado;
}

bool calculadora::good()
{
    return (_estado == OK ? true : false );
}

bignum calculadora::resultado()
{
    bignum res;
    if(this->good() )    
    {
        if( _operacion == SUMAR)
            res = _operando1 + _operando2;
    
        else if( _operacion == RESTAR)
            res = _operando1 - _operando2;
    
        else  if( _operacion == MULTIPLICAR)
            res = _operando1 * _operando2;
        this->_estado = res.estado();
    }
    else
    {
        this->_estado = NOK;
        res.set_estado(NOK);
    }
    return res;
}

calculadora &calculadora::operator=(const string &linea)
{
    unsigned i; // iterador
    
    size_t num1 ;
    size_t op ;
    if( (num1= linea.find_first_of("0123456789")) == string::npos ||
        (op=linea.find_first_of(DiccionarioOperaciones, num1)) == string::npos)
    { 
        _operacion = NO_OP;
        _estado = NOK;
    }
    else{

        string sbignum1 = linea.substr(0,op);
        string sop = linea.substr(op,1);
        string sbignum2 = linea.substr(op+1);

        _operando1 = sbignum1;
        _operando2 = sbignum2;


        for( i=0 ; i< NO_OP && sop.find(DiccionarioOperaciones[i]) == std::string::npos ; i++)
            ;

        _estado = (_operando1.good() && _operando2.good()) ? OK : NOK;
        _operacion = ( i < NO_OP) ? ( operacion_t )i : NO_OP;
    }

    return *this;

}

istream& operator>>(std::istream &is ,calculadora &entrada)
{
    string linea;
    getline(is,linea );
    if( !is.good())
    {
        entrada._estado=ERROR_ENTRADA;
        return is;
    }
    entrada = linea;
   
    return is;

}

#ifdef _TEST_CALCULADORA
 
 int main()
 {
     calculadora operacion(20);
     string linea;

     while(!cin.eof())
     {
         cout << "Ingrese la cuenta: " << '\n';
         getline(cin,linea);
         operacion = linea;
         cout << operacion.resultado() << '\n';
         cout << (int )operacion.estado() << '\n';

     }

 }

 #endif