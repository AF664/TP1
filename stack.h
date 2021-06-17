#ifndef _STACK_H_
#define _STACK_H_

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

	size_t tos_;
	size_t len_;
	T *ptr_;
};

template<typename T>
stack<T>::stack() : tos_(0), len_(0), ptr_(0)
{
}

template<typename T>
stack<T>::stack(size_t len): tos_(0), len_(0), ptr_(0)
{
	ptr_ = new T[len_ = len];
}

template<typename T>
stack<T>::stack(const stack &st): tos_(0), len_(0), ptr_(0)
{

	ptr_ = new T[len_ = st.tos_];

	for (tos_ = 0; tos_ < st.tos_; ++tos_)
		ptr_[tos_] = st.ptr_[tos_];
}

template<typename T>
stack<T> &stack<T>::operator=(const stack<T> &rhs)
{
	if (this != &rhs) 
	{

		if (rhs.tos_ > len_) {
			delete[] ptr_, len_ = 0;
			ptr_ = new T[len_ = rhs.tos_];
		}

		for (tos_ = 0; tos_ < rhs.tos_; ++tos_)
			ptr_[tos_] = rhs.ptr_[tos_];
	}
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
	if(!ptr_)
		abort();
	return ptr_[--tos_];
}

template<typename T>
void stack<T>::pull(T &top)
{
	if(!ptr_)
		abort();
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

	if (len_ != 0)
		len = len_ << 1;
	else
		len = 1;

	ptr = new T[len];

	for (tos = 0; tos < tos_; ++tos)
		ptr[tos] = ptr_[tos];

	delete[] ptr_;

	tos_ = tos;
	len_ = len;
	ptr_ = ptr;
}

template<typename T>
void stack<T>::contract()
{
	std::abort();
}
#endif
