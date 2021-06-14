#ifndef _STACK_H_
#define _STACK_H_

#include <debug.h>
#include <cstdlib>

template<typename T>
class stack {
public:
	stack();
	stack(size_t);
	stack(const stack &);
	stack &operator=(const stack &);
	~stack();

	T pull();
	void pull(T &);
	void push(const T &);
	bool empty() const;
	size_t length() const;
private:
	void expand();
	void contract();

	// Cada instancia consta de tres atributos:
	//
	// o tos_: top of stack, el índice definido dentro de los límites
	//   del arreglo apuntado por ptr_ para apuntar al primer elemento
	//   libre dentro de esa secuencia.
	//
	// o len_: capacidad de almancenamiento del arreglo apuntado por 
	//   ptr_, en términos de cantidad de objetos de tipo T que pueden
	//   ser alojados en ese arreglo.
	//
	// o ptr_: apunta al primer elemento de los len_ objetos de tipo T,
	//   consecutivos, que dan soporte de almacenamiento a la pila.
	//
	// Invariantes:
	//
	// I1. El índice tos_ indica en todo momento el primer elemento del
	//     arreglo disponible para apilar, o vale len_ cuando todos los
	//     elementos del arreglo contienen objetos en la pila. Cuando 
	//     ptr_ es NULL, el valor de tos_ también lo es.
	//
	// I2. Como consecuencia de esto, los valores que puede adoptar tos_
	//     están siempre dentro del intervalo 0 <= tos_ <= len_.
	//
	// I3. ptr_: apunta siempre al primer elemento del arreglo, o NULL
	//     cuando la pila está vacía.
	//
	// I4. len_: indica en todo momento la capacidad de almacenamiento
	//     del arreglo apuntado por ptr_, medida en términos de cantidad
	//     de objetos de tipo T, y vale cero cuanto ptr_ es NULL.
	//
	size_t tos_;
	size_t len_;
	T *ptr_;
};

template<typename T>
stack<T>::stack() 
	: tos_(0), 
	  len_(0), 
	  ptr_(0)
{
}

template<typename T>
stack<T>::stack(size_t len) 
	: tos_(0), 
	  len_(0), 
	  ptr_(0)
{
	ptr_ = new T[len_ = len];
}

template<typename T>
stack<T>::stack(const stack &st)
	: tos_(0), 
	  len_(0), 
	  ptr_(0)
{
	// La pila que estamos construyendo debe tener suficiente espacio
	// de almacenamiento para alojar todos los elementos de la pila de
	// origen. Es decir, no es necesario que coincidan las longitudes
	// de los arreglos que usamos internamente para dar el soporte de
	// almacenamiento de la instancia stack<T>.
	//
	// 
	ptr_ = new T[len_ = st.tos_];

	// Copiamos todos los elementos, uno a uno, respetando el orden y
	// actualizando el índice tos_ sobre la marcha.
	//
	for (tos_ = 0; tos_ < st.tos_; ++tos_)
		ptr_[tos_] = st.ptr_[tos_];

	ASSERT(tos_ <= len_);
	ASSERT(tos_ == st.tos_);
}

template<typename T>
stack<T> &stack<T>::operator=(const stack<T> &rhs)
{
	if (this != &rhs) {
		// Verificamos si la pila de destino cuenta con suficiente
		// capacidad de almacenamiento para alojar los objetos que
		// se encuentran en la pila de origen (rhs).
		//
		// Como ocurre en el constructor de copia, no es necesario
		// verificar la longitud del arreglo origen (rhs.len_), sino 
		// que la cantidad de espacio disponible en destino (len_)
		// sea suficiente para alojar la cantidad efectiva de datos
		// en la pila de origen (rhs.tos_).
		//
		if (rhs.tos_ > len_) {
			delete[] ptr_, len_ = 0;
			ptr_ = new T[len_ = rhs.tos_];
		}

		// Copiamos todos los elementos, uno a uno en orden correcto,
		// actualizando el índice tos_ sobre la marcha.
		//
		for (tos_ = 0; tos_ < rhs.tos_; ++tos_)
			ptr_[tos_] = rhs.ptr_[tos_];
	}

	ASSERT(tos_ <= len_);
	ASSERT(tos_ == rhs.tos_);

	return *this;
}

template<typename T>
stack<T>::~stack()
{
	delete[] ptr_;
}

template<typename T>
T stack<T>::pull()
{

	// Desapilar de una pila vacía es considerado un error, y es por
	// ello que la pila provee un método empty() para evitar este tipo
	// de situaciones.
	//
	ASSERT(tos_ != 0);
	ASSERT(tos_ <= len_);
	ASSERT(ptr_ != NULL);

	// Desapilamos un elemento de la secuencia. Según el invariante I1,
	// tos_ apunta al primer elemento disponible de la secuencia, así que
	// debemos decrementarlo para poder acceder al objeto que se encuentra
	// en el tope de la pila, y quitarlo de allí.
	//
	return ptr_[--tos_];
}

template<typename T>
void stack<T>::pull(T &top)
{
	top = this->pull();
}

template<typename T>
void stack<T>::push(const T &top)
{
	if (tos_ >= len_)
		expand();

	ptr_[tos_++] = top;
}

template<typename T>
bool stack<T>::empty() const
{
	// Según el invariante I1, cuando la pila está vacía top_ debe 
	// valer cero para poder apuntar al primer elemento del arreglo 
	// apuntado por ptr_.
	//
	return tos_ == 0 ? true : false;
}

template<typename T>
size_t stack<T>::length() const
{
	return tos_;
}

template<typename T>
void stack<T>::expand()
{
	size_t len;
	size_t tos;
	T *ptr = 0;

	// Este método es el encargado de expander la capacidad del soporte
	// de almacenamiento de la pila. Dado que todos los atributos de
	// la instancia van a ser modificados, generamos una copia local de
	// los nuevos valores de los mismos, y recién al completar el
	// proceso es cuando vamos a actualizar los atributos del objetos.
	//
	// Para expander la capacidad del soporte de almacenamiento de la
	// pila, vamos a duplicar la cantidad de elementos del arreglo que
	// es apuntado por ptr_, comenzando con 1 cuando ptr_ es NULL.
	//
	if (len_ != 0)
		len = len_ << 1;
	else
		len = 1;

	ptr = new T[len];

	// Copiamos todos los objetos alojados en el soporte de la pila, 
	// en el orden correspondiente en el que ingresaron a la misma.
	// 
	for (tos = 0; tos < tos_; ++tos)
		ptr[tos] = ptr_[tos];

	// Ahora que ya están copiados todos los datos, debemos liberar
	// la memoria del soporte original para evitar tener una fuga de
	// memoria en el momento de actualizar los atributos.
	//
	delete[] ptr_;

	// Sólo resta actualizar los atributos del objeto, uno a uno, usando
	// la variable local que creamos más arriba con el mismo nombre.
	//
	tos_ = tos;
	len_ = len;
	ptr_ = ptr;

	ASSERT(tos_ < len_);
	ASSERT(ptr_ != NULL);
}

template<typename T>
void stack<T>::contract()
{
	std::abort();
}
#endif