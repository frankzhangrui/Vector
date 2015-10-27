// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Rui Zhang

#include <iostream> // debugging
#include "Vector.h"
#include <cmath>
// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector() {
	count = 0;
	reserved = 0;
	elements = NULL;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs) {
	count = rhs.count;
	reserved = rhs.reserved;
	elements = (T*)malloc(reserved*sizeof(T));
	for(size_t i=0 ; i < count; ++i) {
		new (elements+i) T(rhs.elements[i]);
	}
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) {
	if(this == &rhs) return *this;
	for(size_t i=0; i< count; ++i) elements[i].~T();
    if(count>0) free(elements);
	count = rhs.count;
	reserved = rhs.reserved;
	elements = (T*) malloc(reserved*sizeof(T));
	for(size_t i =0; i < count ; ++i) new (elements+i) T(rhs.elements[i]);
	return *this;	
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved){ // Initialize with reserved memory
	count = 0;
	reserved = nReserved;
	elements = (T*)malloc(reserved *sizeof(T));
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t) { 
// Initialize with "n" copies of "t"
	reserved = max(2*n,size_t(1));
	count = n;
	elements = (T*) malloc(reserved *sizeof(T));
	for(size_t i=0; i<n; ++i) {
		new (elements+i)T(t);
	}
}

template <typename T>
void Vector<T>::Reserve(size_t nReserved) { 
  if(reserved < nReserved) {
  	reserved = nReserved;  
  	T* newAddress = (T*)malloc(reserved * sizeof(T));
  	for(size_t i= 0; i< count ; ++i) {
  		new (newAddress+i)T(elements[i]);
  		elements[i].~T();
  	}
  	free(elements);
  	elements = newAddress;
  }                             
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector() {
	for(size_t i=0; i<count; ++i) elements[i].~T();
	free(elements);
	count = 0;	
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs) {
	if(count < reserved) {
		new (elements+count)T(rhs);
		count++;		
	}
	else {
		T* newAddress = (T*)malloc(max(count*2,size_t(1))*sizeof(T));
		for(size_t i=0 ; i< count; ++i) {
			new (newAddress+i)T(elements[i]);
			elements[i].~T();
		}
		free(elements);
		elements = newAddress;
		new (elements+count)T(rhs);
		reserved = max(2*count,size_t(1));	
		count ++;
        }
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs){
	if(count < reserved) {
		for(size_t i = count; i>0 ; --i) {
			new(elements+i)T(elements[i-1]);
		}
		new (elements)T(rhs);
		count++;
	}
	else {
		T* newAddress = (T*)malloc(max(count*2,size_t(1))*sizeof(T));
		for(size_t i= count ; i>0; --i){
			new (newAddress+i)T(elements[i-1]);
			elements[i-1].~T();
		}			
		free(elements);
		elements= newAddress;
		new (elements)T(rhs);
		reserved = max(2*count,size_t(1));
		count++;		
	}
}


template <typename T>
void Vector<T>::Pop_Back(){ // Remove last element
	if(count >0){
		elements[count-1].~T();
		count--;
	}
}

template <typename T>
void Vector<T>::Pop_Front(){ // Remove first element
	if(count >0){
		elements[0].~T();
		for(size_t i=0 ; i< count -1; ++i){
			new (elements +i ) T(elements[i+1]);
			elements[i+1].~T();		
		}
		count --;

	}
}

// Element Access
template <typename T>
T& Vector<T>::Front() const {
	return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const {
	return elements[count-1];
}

template <typename T>
T& Vector<T>::operator[](size_t i) const {
	return *(elements+i);
}

template <typename T>
size_t Vector<T>::Size() const {
	return count;
}


template <typename T>
bool Vector<T>::Empty() const {
	return count==0;
}

// Implement clear
template <typename T>
void Vector<T>::Clear() {
	for(size_t i =0 ; i < count ; ++i) elements[i].~T();
	count =0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const {
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const {
	return VectorIterator<T>(elements+count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it) {
	count --;
	VectorIterator<T> iterator;
	size_t pos =0;
	for(iterator = elements; iterator != (elements + count-1); ++iterator)
	{
		pos ++;
		if(iterator == it) break;
	}	
	elements[pos-1].~T();
	for(size_t i = pos -1; i< count ; ++i)
	{
		new (elements+i)T(elements[i+1]);
		elements[i+1].~T();
	}
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
	count++;
	T* newAddress = static_cast<T*>(malloc(count*sizeof(T)));
	VectorIterator<T> iterator;
	size_t pos=0;
	for(iterator =elements ; iterator != (elements + count -1); ++ iterator)
	{
		pos++;
		if(iterator == it) break;
	}
	for(size_t i =0; i< pos-1; ++i)
	{
		new (newAddress+i)T(elements[i]);
		elements[i].~T();
	}
	new(newAddress+pos-1)T(rhs);
	for(size_t i =pos;i<count; ++i)
	{
		new(newAddress+i)T(elements[i-1]);
		elements[i-1].~T();
	}
	free(elements);
	elements = newAddress;
}

#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
	current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
	current = c;
}


// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
	current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
	return *current;
}	

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
	current ++;
	return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
	VectorIterator<T> iterator(current);
	current ++;
	return iterator;
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
	return current != rhs.current;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
	return current == rhs.current;
}




