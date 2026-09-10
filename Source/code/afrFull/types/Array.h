/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Array.h														  	  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Array.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_ARRAY_H
#define _AFR_ARRAY_H


// Including system libraries
#include <iostream>
#include <string>


// Including project libraries
#include "../utils/MathUtils.h"


// Used namespaces
using namespace std;


/**
 * Template class Array
 * @brief Template class for one dimensional array for store elements.
 */ 
template <class T> class Array
{
public:
	///< Implicit constructors
	inline Array();
	inline Array(const unsigned &n);
	inline Array(const Array<T> &a);
	
	///< Implicit destructors
	~Array();
	
	///< Accessors methods
	inline T First() { return this->data[0]; };
	inline T Last() { return this->data[this->size-1]; };
	inline const T& operator[](unsigned int i) const { return this->data[i]; };
	inline T& operator[](unsigned int i) { return this->data[i]; };
	
	///< IO operators
	template <class U> friend ostream& operator<<(ostream &os, const Array<U> &a);
	
	///< Others operators
	inline Array<T> &operator=(const Array<T> &a);
	
	///< Getters
	inline T 	*GetData() { return this->data; };
	
	///< Vector operations
	int 	Add(const T& item);
	int 	Append(const T& item);
	
	int 	Truncate();
	inline void Clear() { this->size = 0; };
	
	inline unsigned Length() const { return this->size; };
	inline unsigned Size() const { return this->size; };
	inline bool 	Empty() const { return (this->size == 0); };
	
private:
	T 			*data;
	unsigned 	size;
	unsigned 	allocSize;
}; // template class Array



/**
 * Implicit constructor. Alloc memory for 2 items.
 * @return Array().
 */ 
template<class T>
inline Array<T>::Array()
{	
	this->size = 0;
	this->allocSize = 2;
	this->data = new T[this->allocSize];
} // Array()



/**
 * Implicit constructor. Alloc memory for n items.
 * @return Array().
 */ 
template<class T>
inline Array<T>::Array(const unsigned &n)
{	
	this->size = 0;
	this->allocSize = n;
	this->data = new T[this->allocSize];
} // Array()



/**
 * Copy constructor. Copy all data items into new object.
 * @return Array().
 */ 
template<class T>
inline Array<T>::Array(const Array<T> &a)
{	
	this->size = a.Length();
	this->allocSize = this->size;
	this->data = new T[this->allocSize];
	
	if(this->data) {
		for(unsigned i = 0; i < this->allocSize; i++) {
			this->data[i] = a[i];
		}
	}
} // Array()



/**
 * Implicit destructor. Clear array size and dealloc data memory.
 */ 
template<class T>
Array<T>::~Array()
{
	this->size = 0;
	this->allocSize = 0;
	
	if(this->data) {
		delete [] this->data;
	}
} // ~Array()



/**
 * Method for add new item to array. Always adds item to the end of array. This
 * is shortcut for Append().
 * @see Array<T>::Append().
 */ 
template<class T>
int Array<T>::Add(const T& item)
{
	return this->Append(item);
} // Add()



/**
 * Method for add new item to array. Always adds item to the end of array.
 * @param T item A new item which will be add to the end of the array.
 * @return int Returns an actual item's index (position) in array (starts with 0).
 */ 
template<class T>
int Array<T>::Append(const T& item)
{
	if(this->size == this->allocSize) {
		this->allocSize *= 2;
		T *temp = this->data;
		
		this->data = new T[this->allocSize];
		if(this->data == NULL) {
			return -1;
		}
		
		for(unsigned i = 0; i < this->size; i++) {
			this->data[i] = temp[i];
		}
		
		delete [] temp;
	}
	
	this->data[this->size++] = item;
	return (this->size-1);
} // Append()



/**
 * Method for modify array allocated size to the real array size.
 * @return int Returns actual real array size, which is equal to allocated size.
 */ 
template<class T>
int Array<T>::Truncate()
{
	if(this->size != this->allocSize) {
		T *temp = this->data;
		this->allocSize = this->size;
		
		this->data = new T[this->allocSize];
		if(this->data == NULL) {
			return -1;
		}
		
		for(unsigned i = 0; i < this->size; i++) {
			this->data[i] = temp[i];
		}
		
		delete [] temp;
	}
	
	return this->size;
} // Truncate()



/**
 * Operator for assignement.
 * @param Vector4 v An original vector (right value).
 * @return Vector4 Return a left value (vector) after assignement.
 */
template<class T>
inline Array<T>& Array<T>::operator=(const Array<T> &a)
{
	if(this == &a) {
		return *this;
	}
	
	delete [] this->data;
	this->size = a.Length();
	this->allocSize = this->size;
	
	this->data = new T[this->allocSize];
	if(this->data) {
		for(int i = 0; i < this->allocSize; i++) {
			this->data[i] = a[i];
		}
	}
	
	return *this;
} // operator=



/**
 * Output operator. Write all four vector's component to output stream.
 * @param ostream os Output stream,
 * @param Vector4 v Output vector, which will be write to stream.
 * @return ostream Returns output stream for fluent interface.
 */
template <class T>
ostream& operator<<(ostream &os, const Array<T> &a)
{
	os << endl;
	for(unsigned i = 0; i < a.Length(); i++) {
		os << a[i] << endl;
	}
	os << endl;
	
	return os;
} // operator<<




#endif // #ifndef _AFR_ARRAY_H



/** ***************************************************************************/
/** end of file Array.h														  */
/** ***************************************************************************/
