CXXARGS  = -g -Wall -ansi -std=c++17
CXXFLAGS = -I. $(CXXARGS) -g
ERROR =1
SHELL := /bin/bash

all: tp1
	

tp1: main.o cmdline.o bignumBase.o bignumMult.o bignumKarat.o calculadora.o utils.o _error.o
	$(CXX) $(CXXFLAGS) -o tp1 bignumBase.o bignumMult.o bignumKarat.o calculadora.o cmdline.o main.o utils.o _error.o 

main.o: main.cpp cmdline.h calculadora.h bignumBase.h bignumMult.h bignumKarat.h utils.h _error.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cc cmdline.h
	$(CXX) $(CXXFLAGS) -c cmdline.cc

calculadora.o: calculadora.cpp calculadora.h 
	$(CXX) $(CXXFLAGS) -c calculadora.cpp

bignumBase.o: bignumBase.cpp bignumBase.h bignumMult.h bignumKarat.h
	$(CXX) $(CXXFLAGS) -c bignumBase.cpp

bignumMult.o: bignumMult.cpp bignumBase.h bignumMult.h bignumKarat.h
	$(CXX) $(CXXFLAGS) -c bignumMult.cpp

bignumKarat.o: bignumKarat.cpp bignumBase.h bignumMult.h bignumKarat.h
	$(CXX) $(CXXFLAGS) -c bignumKarat.cpp

utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp

_error.o: _error.cpp _error.h
	$(CXX) $(CXXFLAGS) -c _error.cpp	

test-karatsuba: tp1
	@set -e;
	@echo Archivos de prueba: entrada1 salida1 salida1ref con karatsuba.;
	@if ./tp1 -i entrada1.txt -o salida1.txt -m karatsuba; then\
		echo Test básico ok.;\
	else\
		echo Test fallido.;\
	fi
	@echo Probando si el archivo de salida es correcto...;
	@if diff -b salida1ref.txt salida1.txt >/dev/null 2>&1; then\
		echo Archivo de salida correcto, test ok.;\
	else\
		echo Archivo de salida erróneo, test fallido.;\
	fi
	@echo -------------------------------------------------------------------------------;

test-valgrind-karatsuba: tp1
	@set -e;
	@echo Archivos de prueba: entrada1 salida1 salida1ref con karatsuba.;
	@echo Probando si hubieron fugas de memoria...;
	@if valgrind --tool=memcheck --log-file="valgrind-f.txt" ./tp1 -i entrada1.txt -o salida1.txt -m karatsuba; then\
		echo Test ok.;\
	else\
		echo Test fallido.;\
	fi
	@echo Ver valgrind-1.txt para más detalles fugas de memoria.;
	@echo -------------------------------------------------------------------------------;

test-standard: tp1
	@set -e;
	@echo Archivos de prueba: entrada2 salida2 salida2ref con standard.;
	@if ./tp1 -i entrada2.txt -o salida2.txt -m standard; then\
		echo Test básico ok.;\
	else\
		echo Test fallido.;\
	fi
	@echo Probando si el archivo de salida es correcto...;
	@if diff -b salida2ref.txt salida2.txt >/dev/null 2>&1; then\
		echo Archivo de salida correcto, test ok.;\
	else\
		echo Archivo de salida erróneo, test fallido.;\
	fi
	@echo -------------------------------------------------------------------------------;

test-valgrind-standard: tp1
	@set -e;
	@echo Archivos de prueba: entrada2 salida2 salida2ref con standard.;
	@echo Probando si hubieron fugas de memoria...;
	@if valgrind --tool=memcheck --log-file="valgrind-2.txt" ./tp1 -i entrada2.txt -o salida2.txt -m standard; then\
		echo Test ok.;\
	else\
		echo Test fallido.;\
	fi
	@echo Ver valgrind-2.txt para más detalles fugas de memoria.;
	@echo -------------------------------------------------------------------------------;

clean:
	$(RM) -vf *.o *.t *.out *.err main
