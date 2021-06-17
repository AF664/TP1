#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "debug.h"
#include "stack.h"
#include <cstdlib>
#include <iostream>

using namespace std;

template<typename T>
class queue {
	private:
	stack<T> egress_;
	stack<T> ingress_;
	
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
	if (egress_.empty()) 
	{
		while (ingress_.empty() == false)
			egress_.push(ingress_.pull());
	}
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