#ifndef _BIGNUM_H_INCLUDED_
#define _BIGNUM_H_INCLUDED_

#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include "utils.h"
#include "_error.h"
using namespace std;

 typedef enum signo{
     POSITIVO=0,
     NEGATIVO=1
 } signo_t;


class bignum
{
    private:
        unsigned short *_digitos;
        size_t _largo;
        size_t _precision;
        signo_t _signo;
        status_t _estado;

        

        // Asigna memoria a los digitos
        unsigned short *_crear_digitos(unsigned precision);

        // Cuenta los digitos no nulos desde la derecha a la izquierda
        void _actualizar_largo();

        // Realiza la suma sin tener en cuenta el signo, guarda el valor 
        // final del Carry.
        bignum &_suma_sin_signo(bignum const &s2 , int &carry);
        
        // Esta función realiza el complemento en base 10.
        bignum &_complemento_base_10();

        // desplaza a la izquierda y llena los espacios con cero, si pasara
        // un dígito distinto de cero por fuera de la precisión, pone la bandera
        // de estado en ERROR_OVERFLOW. 
       bignum &_desplazamiento_izq(unsigned shift);

        // Pone en cero al numero
        bignum &_poner_a_cero();

        bignum &_cambiar_precision(size_t precision);

    public:
        bignum();
        bignum(unsigned precision);
        bignum(const bignum &original);
        bignum(const string &linea, size_t precision);
        ~bignum();

         

        size_t precision() const;
        size_t largo() const;
        signo_t signo() const;
        status_t estado() const;
        void set_estado(status_t nuevo);
        bool cero() const;
        bool good() const;

        bignum &operator=(const bignum &original);
        bignum &operator=(const string &linea) ;
        bignum &operator=(int numero);
        bignum &operator+=(const bignum &sumando);
        bignum &operator*=(int numero);
        friend bignum operator+(const bignum &sumando1 , const bignum &sumando2);
        friend bignum operator-(const bignum &minuendo, const bignum &sustraendo);
        friend bignum operator*(const bignum &factor1, const bignum &factor2);
        friend bignum operator*(const bignum &factor1, int mult);
        friend std::ostream& operator<<(std::ostream& ,const bignum&);
        friend std::istream& operator>>(std::istream& ,bignum &num);
};

#endif
