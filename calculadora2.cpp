// archivo calculadora.cpp
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

#include "calculadora2.h"

using namespace std ;

char DiccionarioOperaciones[]= {'+','-','*','/','(', ')', 'n', 'p' , '\0'};
string DiccionarioOp[]= {"+","-","*", "/"};

calculadora::calculadora(const string &scuenta)
{   
    string str;

    str = removeSpaces(scuenta);
    _estado = crearColaRPN(str, _cuenta);
}

calculadora::calculadora()
{
     _estado = OK ;
}

calculadora::~calculadora()
{
}

status_t calculadora::estado()
{
    return _estado;
}

bool calculadora::good()
{
    return (_estado == OK ? true : false );
}

calculadora &calculadora::operator=(const string &linea)
{
    string aux;

    aux = removeSpaces(linea);
    _estado = crearColaRPN(aux, _cuenta);
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

bignumBase *calculadora::resultado(bignumBase *operando1, bignumBase *operando2)
{
   string soperando1, soperando2, sres, sop;
   bignumBase *res  = NULL;
   stack <string> pila;
   res = operando1->nuevoBignum();

   if(_estado != OK)
        return res;
   while((_cuenta.length() > 0) || (pila.length() > 1))
   {
       //Mientras no se llegue a un operador y no se acabe la cola
        while(_cuenta.length() && is_digits((sop = _cuenta.pull())))
            pila.push(sop);
        if(is_binary_operator(sop) == true)
        {
            if(pila.length() < 2)
            {
                _estado = NOK;
                return res;
            }
            soperando2 = pila.pull();
            soperando1 = pila.pull();
            if(is_digits(soperando2) == false || is_digits(soperando1) == false)
            {
                pila.push(soperando1);
                pila.push(soperando2);
            }
            else
            {
                *operando1 = soperando1;
                *operando2 = soperando2;
                res = resolve_binary(operando1, operando2, sop);
                pila.push(res->bignum_to_string());
            }
        }
        if(is_unary_operator(sop) == true)
        {
            if(pila.length() < 1)
            {
                _estado = NOK;
                return res;
            }
            soperando1 = pila.pull();
            if(is_digits(soperando1) == false)
                pila.push(soperando1);
            else
            {
                *operando1 = soperando1;
                cout<<"operación: "<<sop<<*operando1<<endl;
                res = resolve_unary(operando1, sop);
                pila.push(res->bignum_to_string());
            }
        }
   }
   *res = pila.pull();
   return res;
}

// ordenar stack agrupa los numeros con sus signos 121 n --> -121
void calculadora::ordenar_stack(){
    string ant;
    string actual;
    stack <string> nueva;
    stack <string> aux;

    actual = _cuenta.pull();
    nueva.push(actual);
    while(_cuenta.length()){
        ant = actual;
        actual = _cuenta.pull();
        nueva.push(actual);
        if(actual == "n" && is_digits(ant)){
            ant = nueva.pull();
            actual = nueva.pull().insert(0,"-");
            nueva.push(actual);
        }
    }
    while(nueva.length())aux.push(nueva.pull());
    while(aux.length())_cuenta.push(aux.pull());
}

status_t calculadora::crearColaRPN(const string &linea, queue <string> &salida)
{
    stack <string> pila;
    size_t pos1, pos2, l;
    string str1, str2;

    pos1 = 0;
    pos2 = 0;
    if(!(l = linea.length()))
        return ERROR_ENTRADA;

    //Si el primer caracter es un menos o más es operador unario
    if(linea[0] == '+')
    {
        pila.push("p");
        pos1++;
        pos2++;
    }
    else if(linea[0] == '-')
    {
        pila.push("n");
        pos1++;
        pos2++;
    }

    while(pos1 != l)
    {

        if((pos2 = linea.find_first_of(DiccionarioOperaciones, pos1)) == string::npos)
            pos2 = l;

        //Si es un número
        if(pos1 != pos2)
        {
            str1 = linea.substr(pos1, pos2 - pos1);
            if(str1.find_first_not_of("0123456789") != string::npos)
                return ERROR_ENTRADA;
            salida.push(str1);
        }
        //Si es alguno de los operadores
        else
        {
            str1 = linea.substr(pos2, 1);
            
            //Si es un paréntesis cerrado
            if(str1[0] == ')')
            {
                //Saco de la pila y mando a salida hasta el (, el cual es descartado
                while (pila.length())
                {
                    str1 = pila.pull();
                    if(str1[0] != '(')
                        salida.push(str1);
                    else 
                        break;
                }
                //Si no se encuentra el otro paréntesis es un error
                if(str1[0] != '(')
                    return ERROR_ENTRADA;
            }
            
            //En caso de que sea cualquier otro operador
            else
            {
                //(
                if(str1[0] == '(')
                    pila.push(str1);

                //+, -, / o *            
                else
                {
                    while(pila.length())
                    {
                        str2 = pila.pull();
                        if(checkPrecedence(str1[0], str2[0]) == true)
                        {
                            pila.push(str2);
                            break;
                        }
                        salida.push(str2);
                    }
                    pila.push(str1);
                }

                //Si hay operadores unarios
                if(linea[pos2 + 1] == '+')
                {
                    pila.push("p");
                    pos1++;
                    pos2++;
                }
                else if(linea[pos2 + 1] == '-')
                {
                    pila.push("n");
                    pos1++;
                    pos2++;
                }
            }
        }
        if(pos1 == pos2)
            pos1++;
        else
            pos1 = pos2;
    }
    //Lo que quedó en la pila lo mando a la salida
    while(pila.length())
        salida.push(pila.pull());
    return OK;
}

bool calculadora::checkPrecedence(const char &c1, const char &c2)
{
    if(c1 == '*' && c2 == '+')
        return true;
    if(c1 == '*' && c2 == '-')
        return true;
    if(c1 == '/' && c2 == '+')
        return true;
    if(c1 == '/' && c2 == '-')
        return true;
    if(c2 == '(')
        return true;
    return false;
}

string calculadora::removeSpaces(string str)
{
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(remove(str.begin(), str.end(), '\t'), str.end());
	return str;
}


bignumBase * calculadora::resolve_binary(bignumBase *operando1, bignumBase *operando2, string op)
{
    bignumBase *res = operando1->clonarBignum();

    if(op[0] == '+')
        *res += *operando2;
    else if(op[0] == '-')
        *res -= *operando2;
    else if(op[0] == '*')
        *res *= *operando2;
    else
        *res /= *operando2;
    return res;
}

bignumBase * calculadora::resolve_unary(bignumBase *operando, string op)
{
    bignumBase *res = operando->clonarBignum();

    if(op[0] == 'n')
        res->set_signo(NEGATIVO);
    else
        res->set_signo(POSITIVO);
    return res;
}

// Tomo numeros con signo
bool calculadora::is_digits(const std::string &str)
{
    if(str.length() == 1)
        return str.find_first_not_of("0123456789") == std::string::npos;
    else
        return str.find_first_not_of("0123456789", 1) == std::string::npos;
}

bool calculadora::is_binary_operator(const std::string &str)
{
    return str.find_first_not_of("-+*/") == std::string::npos;
}

bool calculadora::is_unary_operator(const std::string &str)
{
    return str.find_first_not_of("np") == std::string::npos;
}


#ifdef _TEST_CALCULADORA
 
 int main()
 {
     calculadora operacion((9+11)*12);
     string linea;
     
     cout<<operacion.resultado()<<endl;
 }

#endif