#ifndef NODO_H
#define NODO_H

#include <iostream>
#include <cstdlib>

using namespace std;

template <class T>
class nodo
{
    private:
    nodo *ant_;
    nodo *sig_;
    T dato_;

    public:
    nodo();
    nodo(const T &);
    ~nodo();
    nodo * getSig();
    nodo * getAnt();
    T getDato();
    void setSig(nodo *);
    void setAnt(nodo *);
};

template <class T>
nodo<T>::nodo(const T & dato)
{
    dato_ = dato;
    sig_ = NULL;
    ant_ = NULL;
}

template <class T>
nodo<T>::~nodo()
{

}

template <class T>
nodo<T> * nodo<T>::getSig()
{
    return sig_;
}

template <class T>
nodo<T> * nodo<T>::getAnt()
{
    return ant_;
}

template <class T>
T nodo<T>::getDato()
{
    return dato_;
}

template <class T>
void nodo<T>::setSig(nodo<T> *nuevo)
{
    sig_ = nuevo;
}

template <class T>
void nodo<T>::setAnt(nodo<T> *nuevo)
{
    ant_ = nuevo;
}

#endif