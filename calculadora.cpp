// archivo calculadora.cpp
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

#include "calculadora.h"

using namespace std ;

char DiccionarioOperaciones[]= {'+','-','*','(', ')', 'n', 'p' , '\0'};
string DiccionarioOp[]= {"+","-","*", "/"};

calculadora::calculadora(const string &scuenta)
{   
    string str;

    str = removeSpaces(scuenta);
    if(crearColaRPN(str, _cuenta) == OK)
        _estado = OK ;
    else
        _estado = NOK;
    cout<<_cuenta<<endl;
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

bignum calculadora::resultado()
{
    stack <string> aux;     
    bool estado = false;
    size_t cantidad_num;
    string str;
    
    if(this->good()){
        while (_cuenta.length() > 2){
            ordenar_stack();
            // Buscamos en la pila la operacion y los numeros
            estado = false;
            cantidad_num = 0;
            while((!_cuenta.empty())){
                str = _cuenta.pull();
                aux.push(str);
                if(is_digits(str))cantidad_num++;
                if(cantidad_num >= 2 && is_operation(str)){
                    estado = true;
                    break;
                } 
            }
            if(estado){
                cout << "Operacion" << endl;
                size_t i;
                bignum res;
                stack<string> aux2;
                string op = aux.pull();
                string num1, num2, resul = "0";
                for( i=0 ; i< NO_OP && op.find(DiccionarioOp[i]) == std::string::npos ; i++);
                _operacion = ( i < NO_OP) ? ( operacion_t )i : NO_OP;
                num2 = aux.pull();
                num1 = aux.pull();
                cout << num1 << op << num2 << endl;
                // Guardo en _cuenta, el resultado en el lugar de num1, num2, op
                aux.push(resul);
                while (_cuenta.length())aux.push(_cuenta.pull());
                while (aux.length())aux2.push(aux.pull());
                while (aux2.length())_cuenta.push(aux2.pull());
		   // Aca se hacen las cuentas
                //if( _operacion == SUMAR)res = _operando1 + _operando2;
                //else if( _operacion == RESTAR) res = _operando1 - _operando2;
                //else  if( _operacion == MULTIPLICAR) res = _operando1 * _operando2;
                //else  if( _operacion == DIVISION) res = _operando1 / _operando2;
            }
        }
        // Las operaciones restantes son unitarias (solo quedan 2 elementos)
        string val1, val2;
        val1 = _cuenta.pull();
        val2 = _cuenta.pull();
        val1 = val2 == "p" ? val1 : val1 = is_operation(val1) ? val1.erase(0,1) : val1.insert(0,"-");
        _cuenta.push(val1);  
    }
    // retorno el unico elemento que queda en _cuenta
    cout << _cuenta << endl;
    str = _cuenta.pull();
    bignum res;     // falta inicializarlo con el str
    return res;
}



calculadora &calculadora::operator=(const string &linea)
{
    /*unsigned i; // iterador
    
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
    }*/

    return *this;

}

istream& operator>>(std::istream &is ,calculadora &entrada)
{
/*    string linea;
    getline(is,linea );
    if( !is.good())
    {
        entrada._estado=ERROR_ENTRADA;
        return is;
    }
    entrada = linea;
*/   
    return is;

}

/*#ifdef _TEST_CALCULADORA
 
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

 }*/


status_t calculadora::crearColaRPN(const string &linea, queue <string> &salida)
{
    stack <string> pila;
    size_t pos1, pos2, l;
    string str1, str2;

    pos1 = 0;
    pos2 = 0;
    l = linea.length();

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
                    return NOK;
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



// Tomo numeros con signo
bool is_digits(const std::string &str)
{
    return str.find_first_not_of("-0123456789") == std::string::npos;
}

bool is_operation(const std::string &str)
{
    return str.find_first_not_of("-+*/") == std::string::npos;
}
