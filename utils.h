#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

#include <iostream>
#include <string>


#define DIGITOS "0123456789"

using namespace std;

// stringLeftTrim quita los espacios en blanco contiguos a 
// la derecha en el string s. Devuelve el tamaño final de s
size_t stringRightTrim(string &s);

// stringLeftTrim quita los espacios en blanco contiguos a 
// la izquierda en el string s. Devuelve el tamaño final de s
size_t stringLeftTrim(string &s);

// stringTrim quita los espacios contiguos a la derecha y a la 
// izquierda. devuelve el tamaño final de s.
size_t stringTrim(string &s);

// stringBignum modifica al string s para que quede bien conformado
// como cadena de caracteres representando un número entero
size_t stringDigits(string &s);




#endif