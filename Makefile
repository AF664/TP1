CXXARGS  = -g -Wall -ansi -std=c++17
CXXFLAGS = -I. $(CXXARGS) -g
ERROR =1
SHELL := /bin/bash

all: tp1
	

tp1: main.o cmdline.o bignumBase.o bignumMult.o bignumKarat.o calculadora2.o utils.o _error.o
	$(CXX) $(CXXFLAGS) -o tp1 bignumBase.o bignumMult.o bignumKarat.o calculadora2.o cmdline.o main.o utils.o _error.o 

main.o: main.cpp cmdline.h calculadora2.h bignumBase.h bignumMult.h bignumKarat.h utils.h _error.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cc cmdline.h
	$(CXX) $(CXXFLAGS) -c cmdline.cc

calculadora.o: calculadora2.cpp calculadora2.h 
	$(CXX) $(CXXFLAGS) -c calculadora2.cpp

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

test-fine: tp1
	@set -e;
	@echo Archivos de prueba: entrada1 salida1 salida1ref con precisión 20.;
	@if ./tp1 -i entrada1.txt -o salida1.txt -p 20; then\
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

test-valgrind-fine: tp1
	@set -e;
	@echo Archivos de prueba: entrada1 salida1 salida1ref con precisión 20.;
	@echo Probando si hubieron fugas de memoria...;
	@if valgrind --tool=memcheck --log-file="valgrind-f.txt" ./tp1 -i entrada1.txt -o salida1.txt -p 20; then\
		echo Test ok.;\
	else\
		echo Test fallido.;\
	fi
	@echo Ver valgrind-f.txt para más detalles fugas de memoria.;
	@echo -------------------------------------------------------------------------------;

test-wrong: tp1
	@set -e;
	@echo Archivos de prueba: entrada2 salida2 salida2ref, tiene mal el formato y precisión 1.;
	@if ./tp1 -i entrada2.txt -o salida2.txt -p 1; then\
		echo Test fallido.;\
	else\
		echo Test básico ok.;\
	fi
	@echo Probando si el archivo de salida es correcto...;
	@if diff -b salida2ref.txt salida2.txt >/dev/null 2>&1; then\
		echo Archivo de salida correcto, test ok.;\
	else\
		echo Archivo de salida erróneo, test fallido.;\
	fi
	@echo -------------------------------------------------------------------------------;

test-valgrind-wrong: tp1
	@set -e;
	@echo Archivos de prueba: entrada2 salida2 tiene mal el formato y precisión 1.;
	@echo Probando si hubieron fugas de memoria...;
	@if valgrind --tool=memcheck  --log-file="valgrind-w.txt" ./tp1 -i entrada2.txt -o salida2.txt -p 1; then\
		echo Test fallido.;\
	else\
		echo Test básico ok.;\
		echo En este caso no significa que no hubieron fugas.;\
	fi
	@echo Ver valgrind-w.txt para analizar si hubieron fugas.;
	@echo -------------------------------------------------------------------------------;

clean:
	$(RM) -vf *.o *.t *.out *.err main tp1
