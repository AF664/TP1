#include <iostream>
#include <cstdlib>
#include "nodo.h"

#ifndef LISTA_H
#define LISTA_H

using namespace std;

template <class T>
class lista
{
    private:
    nodo <T> *pri_;
    nodo <T> *ult_;
    size_t tam_;

    public:
    lista();
    lista(const lista &);
    ~lista();
    bool vacia() const;
    void altaprin(const T &);
    void altafin(const T &);
    void altapos(const T &, const size_t &);
    void bajapos(const size_t &);
    void baja(const T &);
    T getDato(const size_t &);
    size_t getTam();
    size_t buscar(const T &);
    nodo <T> * getPri();


    template <class TT>
    friend ostream & operator<<(ostream &, lista &);
};

template <class T>
lista<T>::lista()
{
    pri_ = NULL;
    ult_ = NULL;
    tam_ = 0;
}

template <class T>
lista<T>::lista(const lista<T> &orig)
{
    nodo <T> *aux;

    pri_ = NULL;
    ult_ = NULL;
    tam_ = 0;
    if(&orig == this || orig.vacia() == true)
        return;
    for(aux = orig.pri_; aux; aux = aux->getSig())
        this->altaprin(aux->getDato);
    tam_ = orig.tam_;
}

template <class T>
lista<T>::~lista()
{
    nodo <T> *aux;
    if(pri_)
    {
        for(aux = pri_->getSig(); aux; aux = aux->getSig())
            delete aux->getAnt();
        delete ult_;
    }
}

template <class T>
bool lista<T>::vacia() const
{
    return pri_ == NULL? true:false;
}

template <class T>
void lista<T>::altaprin(const T &dato)
{
    nodo <T> *aux = new nodo <T>(dato);
    if(pri_ == NULL)
    {
        pri_ = aux;
        ult_ = aux;
        aux->setAnt(NULL);
        aux->setSig(NULL);
    }
    else
    {
        aux->setAnt(NULL);
        aux->setSig(pri_);
        pri_->setAnt(aux);
        pri_ = aux;
    }
    tam_++;
}

template <class T>
void lista<T>::altafin(const T &dato)
{
    nodo <T> *aux = new nodo <T>(dato);

    if(pri_ == NULL)
    {
        pri_ = aux;
        ult_ = aux;
        aux->setAnt(NULL);
        aux->setSig(NULL);
        return;
    }
    else
    {
        aux->setSig(NULL);
        aux->setAnt(ult_);
        ult_->setSig(aux);
        ult_ = aux;
    }
    tam_++;
}

template <class T>
void lista<T>::altapos(const T &dato, const size_t &pos)
{
    nodo <T> *aux1;
    nodo <T> *aux2;
    nodo <T> *nuevo; 
    if(pos >= tam_)
    {
        altafin(dato);
        return;
    }
    aux1 = pri_;
    for(size_t i = 0; i<pos; i++)
        aux1 = aux1->getSig();
    nuevo = new nodo <T>(dato);
    aux2 = aux1->getAnt();
    nuevo->setAnt(aux2);
    nuevo->setSig(aux1);
    aux1->setAnt(nuevo);
    aux2->setSig(nuevo);
    tam_++;
}

template <class T>
void lista<T>::bajapos(const size_t &pos)
{
    nodo <T> *aux1;
    nodo <T> *aux2; 
    if(pos >= tam_)
        return;
    if(!pos)
    {
        aux1 = pri_;
        pri_ = pri_->getSig();
        pri_->setAnt(NULL);
    }
    else if(pos == (tam_ - 1))
    {
        aux1 = ult_;
        ult_ = ult_->getAnt();
        ult_->setSig(NULL);
    }
    else
    {
        aux1 = pri_;
        for(size_t i = 0; i<pos; i++)
            aux1 = aux1->getSig();
        aux2 = aux1->getAnt();
        aux2->setSig(aux1->getSig());
        aux2 = aux1->getSig();
        aux2->setAnt(aux1->getAnt());
    }
    delete aux1;
    tam_--;
}

template <class T>
void lista<T>::baja(const T &dato)
{

}

template <class T>
T lista<T>::getDato(const size_t &pos)
{
    nodo <T> *aux;

    aux = pri_;
    for(size_t i = 0; i<pos; i++)
        aux = aux->getSig();
    return aux->getDato();
}

template <class T>
size_t lista<T>::getTam()
{
    return tam_;
}

template <class T>
nodo <T> * lista<T>::getPri()
{
    return pri_;
}

template <class TT>
ostream & operator<<(ostream &os,lista<TT> &list)
{
    nodo <TT> *aux;

    if(list.vacia() == true)
        return os;
    for(aux = list.getPri(); aux; aux = aux->getSig())
        os<<aux->getDato()<<'\t';
    
    return os;
}

#endif