#ifndef _CALCULADORA_H_INCLUDED_
#define _CALCULADORA_H_INCLUDED_

#include "bignumBase.h"
#include "bignumMult.h"
#include "bignumKarat.h"
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
        operacion_t _operacion;

        status_t crearColaRPN(const string &, queue <string> &);
        bool checkPrecedence(const char &, const char &);
        //Remueve espacios
        string removeSpaces(string);
        bool is_digits(const std::string &);
        bool is_binary_operator(const std::string &);
        bool is_unary_operator(const std::string &);
        bignumBase * resolve_binary(bignumBase *, bignumBase *, string);
        bignumBase * resolve_unary(bignumBase *, string);

    public:

        calculadora();    
        calculadora(const string &);
        ~calculadora();

        status_t estado();
        bool good();
        
        void ordenar_stack();
        bignumBase *resultado(bignumBase *, bignumBase *);

        calculadora &operator=(const string &linea);
    
        friend std::istream& operator>>(std::istream& ,calculadora &entrada);

};

#endif