#ifndef _CALCULADORA_H_INCLUDED_
#define _CALCULADORA_H_INCLUDED_

#include "bignumBase.h"
#include "bignumMult.h"
#include "bignumKarat.h"
#include "_error.h"
#include "utils.h"
#include <iostream>
#include <string>
#include "queue.h"
#include "stack.h"

bool is_digits(const std::string &str);
bool is_operation(const std::string &str);

typedef enum operacion {
    SUMAR,
    RESTAR,
    MULTIPLICAR,
    DIVIDIR,
    NO_OP
} operacion_t;


class calculadora
{
    private:
        queue <string> _cuenta;
        status_t _estado;
        bignumBase *_operando1;
        bignumBase *_operando2;
        operacion_t _operacion;

        status_t crearColaRPN(const string &, queue <string> &);
        bool checkPrecedence(const char &, const char &);
        //Remueve espacios
        string removeSpaces(string);

    public:

        calculadora();    
        calculadora(bignumBase *op1, bignumBase *op2, unsigned precision);
        // calculadora(unsigned precision );
        ~calculadora();

        void set_operacion(operacion_t op);
        status_t estado();
        bool good();
        
        void ordenar_stack();
        bignumBase *resultado();
        calculadora &operator=(const string &linea);    
        friend std::istream& operator>>(std::istream& ,calculadora &entrada);

};

#endif
