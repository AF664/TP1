// archivo _error.cpp

#include "_error.h"
using namespace std;

const char *DiccionarioMsjError[_CNT_MSJ_ERROR]={
    _MSJ_OK,
    _MSJ_NOK,
    _MSJ_ERROR_DIGITOS,
    _MSJ_ERROR_PRECISION,
    _MSJ_ERROR_OVERFLOW,
    _MSJ_ERROR_ENTRADA,
    _MSJ_ERROR_DIVISION,
};

void error_msj(status_t codigo)
{
    cerr << DiccionarioMsjError[(unsigned)codigo];
}