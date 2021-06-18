#include "bignumBase.h"
#include "bignumMult.h"
#include "bignumKarat.h"
#include "_error.h"
#include "queue.h"
#include "stack.h"
#include "debug.h"
#include "calculadora2.h"
#include <iostream>
#include <string>

#include "calculadora.h"

 int main()
 {
     calculadora operacion;
     string linea = "(9+11)*12";
     bignumBase *res;
     
     operacion = linea;
     res = operacion.resultado();
     cout<<*res<<endl;
 }