#ifndef _CALCULADORA_H_INCLUDED_
#define _CALCULADORA_H_INCLUDED_

#include "bignum.h"
#include "_error.h"
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
        bignum _operando1;
        bignum _operando2;
        status_t _estado;
      
        operacion_t _operacion;

    public:

        calculadora();    
        calculadora(bignum &op1, bignum &op2, unsigned precision);
        calculadora(unsigned precision );
        ~calculadora();

        void set_operacion(operacion_t op);
        status_t estado();
        bool good();
        

        bignum resultado();

        calculadora &operator=(const string &linea);
    
        friend std::istream& operator>>(std::istream& ,calculadora &entrada);

};

#endif
