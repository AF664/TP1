#ifndef _CALCULADORA_H_INCLUDED_
#define _CALCULADORA_H_INCLUDED_

#include "bignum.h"
#include "_error.h"
#include "queue.h"
#include "stack.h"
#include <iostream>
#include <string>

typedef enum operacion {
    SUMAR,
    RESTAR,
    MULTIPLICAR,
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

    public:

        calculadora();    
        calculadora(const string &);
        ~calculadora();

        status_t estado();
        bool good();
        

        bignum resultado();

        calculadora &operator=(const string &linea);
    
        friend std::istream& operator>>(std::istream& ,calculadora &entrada);

};

#endif
