#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "debug.h"
#include "stack.h"
#include <cstdlib>
#include <iostream>

using namespace std;

template<typename T>
class queue {
public:
	queue();
	queue(const queue &);
	queue &operator=(const queue &);
	~queue();

	T pull();
	void pull(T &);
	void push(const T &);
	bool empty() const;
	size_t length() const;

	template <class TT>
	friend ostream &operator<<(ostream &, queue &);
private:
	// Usamos dos pilas para dar soporte de almacenamiento a cola, e
	// implementar una política FIFO para el manejo de datos.
	//
	// o La pila egress_ es la pila de salida, y aloja una secuencia
	//   de elementos en orden de salida, de tal forma que el tope de
	//   la misma sea el elemento que más tiempo lleva encolado (FIFO).
	//
	// o La pila ingress_ es la pila de entrada, y contiene elementos
	//   que han ingresado más recientemente a la cola, dispuestos en
	//   orden inverso de llegada (LIFO).
	//
	// En el momento de desacolar, vamos a volcar la secuencia de la 
	// pila ingress_ en egress_ utilizando el criterio LIFO. De esta
	// manera, los objetos en la pila egress_ quedarán dispuestos en
	// función del criterio FIFO.
	//
	stack<T> egress_;
	stack<T> ingress_;
};

template<typename T>
queue<T>::queue() 
{
}

template<typename T>
queue<T>::queue(const queue &q)
	: egress_(q.egress_),
	  ingress_(q.ingress_)
{
}

template<typename T>
queue<T> &queue<T>::operator=(const queue<T> &rhs)
{
	if (this != &rhs) {
		egress_ = rhs.egress_;
		ingress_ = rhs.ingress_;
	}

	return *this;
}

template<typename T>
queue<T>::~queue()
{
}

template<typename T>
T queue<T>::pull()
{
	// Si la pila de salida tiene al menos un elemento, el tope de la
	// misma será el elemento que lleva más tiempo en la misma. En caso
	// contrario, cuando la pila de salida está vacía, volcamos todos
	// los elementos de la pila de entrada en la salida de tal forma 
	// que queden en el orden que comentamos antes.
	//
	if (egress_.empty()) {
		while (ingress_.empty() == false)
			egress_.push(ingress_.pull());
	}

	// Tomamos el tope de la pila de salida. Notar que, en este punto,
	// si la pila de salida está vacía es porque ambas pilas lo están,
	// y por ende se generará una excepción.
	//
	return egress_.pull();
}

template<typename T>
void queue<T>::pull(T &top)
{
	return this->pull();
}

template<typename T>
void queue<T>::push(const T &item)
{
	// Colocamos el elemento en el tope de la pila de entrada.
	//
	ingress_.push(item);
}

template<typename T>
bool queue<T>::empty() const
{
	return egress_.empty() && ingress_.empty() ? true : false;
}

template<typename T>
size_t queue<T>::length() const
{
	// La longitud de la cola está dada por la cantidad total de 
	// elementos alojada en la misma. Cada uno de esos elementos estará
	// alojado en una y solo una de las dos pilas que dan soporte de
	// almacenamiento: por ende, la longitud efectiva de la cola será
	// la suma de las longitudes efectivas de ambas pilas.
	//
	return egress_.length() + ingress_.length();
}


template <class TT>
ostream & operator<<(ostream &os, queue <TT> &cola)
{
	queue <TT> aux(cola);
	while (aux.length())
		os<<aux.pull()<<" ";
	return os;
}

#endif