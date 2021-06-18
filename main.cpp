#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "calculadora2.h"
#include "cmdline.h"
#include "_error.h"
#include "utils.h"


#define CLASSIC 0
#define KARATSUBA 1


#define MSJ_AYUDA "USO calculadora.exe -i <input file> -o <output file> -p <precision>\
\n<input file>  nombre del archivo de entrada. Stdin por defecto\
\n<output file> nombre del archivo de salida. Stdout por defecto\
\n<precision>   precision de los números a utilizar \
\n<multiplicacion>   Algoritmo de multiplicacion a utilizar. classic o karatsuba \n" 
using namespace std;


// Abre el flujo de entrada segun el nombre de archivo, o stdin por defecto
static void opt_input(string const &nombre_archivo);

// Abre el flujo de salida segun el nombre de archivo, o stdout por defecto
static void opt_output(string const &nombre_archivo);

// Imprime mensaje de error en cerr
static void imprimir_ayuda(string const &arg);

static void opt_precision(string const &precision);

static void opt_multiplication(string const &mul);

option_t config[] = {
    {1, "i", "input","-", opt_input, OPT_DEFAULT},
    {1, "o", "output","-", opt_output, OPT_DEFAULT},
    {1, "p", "precision",PRECISION_DEFAULT,opt_precision, OPT_DEFAULT},
    {1, "m", "multiplicacion", MULTIPLICATION_DEFAULT, opt_multiplication, OPT_DEFAULT},
	{0, "h", "help",nullptr,imprimir_ayuda, OPT_DEFAULT},
    {0,},// Fin de opciones

};


static int precision;
static int multiplication;
static istream *iss;
static ostream *oss;
static fstream ifs;
static fstream ofs;

int main(int argc,char *const argv[])
{  
    cmdline comandos(config);
    comandos.parse(argc, argv);
    calculadora cuenta;
	bignumBase *res;
	
	if(multiplication == CLASSIC)
	{
    	while((*iss>>cuenta)) 
    	{
			bignumMult operando1, operando2;
			res = cuenta.resultado(&operando1, &operando2);
			if(!cuenta.good())
			{
				error_msj(cuenta.estado());
				return NOK;
			}
        	*oss <<*res<<endl;		
    	}
	}
	else if(multiplication == KARATSUBA)
	{		
		while((*iss>>cuenta)) 
    	{
			bignumKarat operando1, operando2;
			res = cuenta.resultado(&operando1, &operando2);
			if( !cuenta.good())
			{
				error_msj(cuenta.estado());
				return NOK;
			}			
			*oss<<*res<<endl;
    	}
	}
	else
		return NOK;
    return OK;
}


void opt_input(string const &arg)
{
	// Si el nombre del archivos es "-", usaremos la entrada
	// estándar. De lo contrario, abrimos un archivo en modo
	// de lectura.
	//
	if (arg == "-") {
		iss = &cin;
	} else {
		ifs.open(arg.c_str(), ios::in);
		iss = &ifs;
	}

	// Verificamos que el stream este OK.
	//
	if (!iss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void opt_output(string const &arg)
{
	// Si el nombre del archivos es "-", usaremos la salida
	// estándar. De lo contrario, abrimos un archivo en modo
	// de escritura.
	//
	if (arg == "-") {
		oss = &cout;
	} else {
		ofs.open(arg.c_str(), ios::out);
		oss = &ofs;
	}

	// Verificamos que el stream este OK.
	//
	if (!oss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void opt_precision(string const &arg)
{
	istringstream iss(arg);
	
	// Intentamos extraer el factor de la línea de comandos.
	// Para detectar argumentos que únicamente consistan de 
	// números enteros, vamos a verificar que EOF llegue justo
	// después de la lectura exitosa del escalar.
	//
	if (!(iss >> precision)
	    || !iss.eof()) {
		cerr << "non-integer factor: "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}

	if (iss.bad()) {
		cerr << "cannot read integer factor."
		     << endl;
		exit(1);
	}
}

static void opt_multiplication(string const &arg)
{
	istringstream iss(arg);
	string word;
	// Intentamos extraer el factor de la línea de comandos.
	// Para detectar argumentos tipo char que conforman una
	// palabra, vamos a verificar que EOF llegue justo
	// después de la lectura exitosa del escalar.
	//

	if (!(iss >> word)
	    || !iss.eof()) {
		cerr << "non-integer factor: "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}

	if (iss.bad()) {
		cerr << "cannot read integer factor."
		     << endl;
		exit(1);
	}
	if (word == "classic" || word == "standard")
		multiplication = CLASSIC;
	else multiplication = KARATSUBA;
}

static void imprimir_ayuda(string const &arg)
{
    if(arg != "")
        cerr    << "argumento: "
                << arg
                << " no reconocido"
                << '\n';
    
    cerr    << MSJ_AYUDA
            << '\n';
    exit(1);
}
