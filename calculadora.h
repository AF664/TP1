#ifndef _CALCULADORA_H_INCLUDED_
#define _CALCULADORA_H_INCLUDED_

#include "bignum.h"
#include "_error.h"
#include "queue.h"
#include "stack.h"
#include <iostream>
#include <string>

bool is_digits(const std::string &str);
bool is_operation(const std::string &str);

typedef enum operacion {
    SUMAR,
    RESTAR,
    MULTIPLICAR,
    DIVISION,
    NO_OP
} operacion_t;


class calculadora
{
    private:
        queue <string> _cuenta;
        status_t _estado;

        status_t crearColaRPN(const string &, queue <string> &);
        bool checkPrecedence(const char &, const char &);
        //Remueve espacios
        string removeSpaces(string);
        bignum _operando1;
        bignum _operando2;
        operacion_t _operacion;
    public:

        calculadora();    
        calculadora(const string &);
        ~calculadora();

        status_t estado();
        bool good();
        
        void ordenar_stack();
        bignum resultado();

        calculadora &operator=(const string &linea);
    
        friend std::istream& operator>>(std::istream& ,calculadora &entrada);

};

#endif
