#ifndef _CALCULADORA_H_INCLUDED_
#define _CALCULADORA_H_INCLUDED_

#include "bignumBase.h"
#include "bignumMult.h"
#include "_error.h"
#include "utils.h"
#include <iostream>
#include <string>

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
        bignumBase *_operando1;
        bignumBase *_operando2;
        status_t _estado;
        operacion_t _operacion;
   

    public:

        calculadora();    
        calculadora(bignumBase *op1, bignumBase *op2, unsigned precision);
        // calculadora(unsigned precision );
        ~calculadora();

        void set_operacion(operacion_t op);
        status_t estado();
        bool good();
        

        bignumBase *resultado();

        calculadora &operator=(const string &linea);
    
        friend std::istream& operator>>(std::istream& ,calculadora &entrada);

};

#endif
