// archivo: error.h

#ifndef _ERROR_H_INCLUDED
#define _ERROR_H_INCLUDED

#include <iostream>

#define _CNT_MSJ_ERROR 7
#define _MSJ_OK ""
#define _MSJ_NOK "Se encontraron errores en el proceso\n"
#define _MSJ_ERROR_DIGITOS "Error, entrada no reconocida\n"
#define _MSJ_ERROR_PRECISION "Error, precision no adecuada\n"
#define _MSJ_ERROR_OVERFLOW "Error, operacion sobrepaso la precision\n"
#define _MSJ_ERROR_ENTRADA "Error, no se pudo leer la entrada\n"
#define _MSJ_ERROR_DIVISION "El dividendo es nulo\n"

typedef enum status{
    OK,
    NOK,
    ERROR_DIGITOS,
    ERROR_PRECISION,
    ERROR_OVERFLOW,
    ERROR_ENTRADA,
    ERROR_DIVISION
} status_t;

void error_msj(status_t codigo);

#endif