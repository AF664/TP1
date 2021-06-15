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

 #define AUMENTO_BLOQUE_PRECISION 10

// define la clase abstracta bignumBase. 
class bignumBase
{
    private:
        unsigned short *_digitos;
        size_t _largo;
        size_t _precision;
        signo_t _signo;
        status_t _estado;

        

        // Asigna memoria a los digitos
        unsigned short *_crear_digitos(unsigned precision);

        
        // Realiza la suma sin tener en cuenta el signo, guarda el valor 
        // final del Carry.
        bignumBase &_suma_sin_signo(bignumBase const &s2 , int &carry);
        
        // Esta función realiza el complemento en base 10.
        bignumBase &_complemento_base_10();



    public:
        bignumBase();
        bignumBase(size_t precision);
        bignumBase(const bignumBase &original);
        bignumBase(const string &linea, size_t precision);
        virtual ~bignumBase();
        
        // constructores virtuales para poder aprovechas el código
        // en común de las dos clases bignum 
        virtual bignumBase *nuevoBignum() const = 0 ;
        virtual bignumBase *clonarBignum() const  = 0 ;         

        size_t precision() const;
        size_t largo() const;
        signo_t signo() const;
        status_t estado() const;
        void set_estado(status_t nuevo);
        void set_signo( signo_t);
        short digito(size_t pos) const;
        bool cero() const;
        bool good() const;

        // Pone en cero al numero
        bignumBase &_poner_a_cero();

        // desplaza a la izquierda y llena los espacios con cero, si pasara
        // un dígito distinto de cero por fuera de la precisión, pone la bandera
        // de estado en ERROR_OVERFLOW. 
        bignumBase &_desplazamiento_izq(unsigned shift);

        bignumBase &_desplazamiento_der(unsigned shift);
        // enmascara con cero a cada digito hacia la izquierda a partir de la 
        // cantidad de digitos "digit"
        bignumBase &_mascara_der(unsigned digit);

        // Cuenta los digitos no nulos desde la derecha a la izquierda
        void _actualizar_largo();

        bignumBase &_cambiar_precision(size_t precision);



        bignumBase &operator=(const bignumBase &original);
        bignumBase &operator=(const string &linea) ;
        bignumBase &operator=(int numero);
        bignumBase &operator+=(int numero);
        bignumBase &operator+=(const bignumBase &sumando);
        bignumBase &operator-=(const bignumBase &sustraendo);
        bignumBase &operator*=(int numero);
        bignumBase &operator/=(const bignumBase &divisor);
        bignumBase &operator++(int); // operador prefijo

        friend bool operator<(bignumBase const &a, bignumBase const &b);
        friend bool operator>(bignumBase const &a, bignumBase const &b);
        friend bool operator==(bignumBase const &a, bignumBase const &b);
        friend bool operator<=(bignumBase const &a, bignumBase const &b);
        friend bool operator>=(bignumBase const &a, bignumBase const &b);
        
       
        virtual bignumBase &operator*=(const bignumBase &factor1) = 0;
        
     
        friend std::ostream& operator<<(std::ostream& ,const bignumBase&);
        friend std::istream& operator>>(std::istream& ,bignumBase &num);
};

#endif
