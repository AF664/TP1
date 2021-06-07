// utils.cpp

#include "utils.h"



using namespace std;

size_t stringRightTrim(string &s)
{
    size_t posicion= s.find_last_not_of("  \t\f\v\n\r");
    if( posicion != string::npos)
        s.erase(posicion + 1);
    else
        s.erase(); // si no encuentra, son todos espacios en blanco
      
    return s.length();
}

size_t stringLeftTrim(string &s)
{
     size_t posicion= s.find_first_not_of("  \t\f\v\n\r");
    if( posicion != string::npos)
        s.erase(0,posicion);
    else
        s.erase(); // si no encuentra, son todos espacios en blanco
      
    return s.length();
}

size_t stringTrim(string &s)
{
    stringRightTrim(s);
    stringLeftTrim(s);

    return s.length();
}


size_t stringDigits(string &s)
{
    
    // quito los espacios en blanco, si los hubiera, a izquierda
    // y derecha.
    stringTrim(s);
  
    // si no hay signo, asumo que es positivo y le agrego el signo
    if( s.find_first_of("+-") == string::npos)
        s.insert(0,1,'+');
    
    // si hay algún caracter que no sea un digito, sin contar el signo
    // el número representado se considera mal conformado
   

    if ( s.find_first_not_of(DIGITOS,1) < s.length() || 
        s.find_last_of(DIGITOS) == string::npos)
            s.erase();

    return s.length();
}
