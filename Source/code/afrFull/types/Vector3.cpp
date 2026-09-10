/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Vector3.cpp														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Vector3.								  *
 **************************************************************************** */


// Including project libraries
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"



/**
 * Copy constructor. Creates 3D vector from 2D vector. Unused components are 0.
 * @param Vector2 v Original vector, which will be copied.
 * @return Vector3.
 */ 
Vector3::Vector3(const Vector2 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = 0;
} // Vector3()



/**
 * Copy constructor. Creates 3D vector from 4D vector.
 * @param Vector3 v Original vector, which will be copied.
 * @return Vector3.
 */ 
Vector3::Vector3(const Vector4 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
} // Vector3()



/**
 * Input operator. From input stream read three float values and stores into a
 * vector to each vector's component.
 * @param istream is Input stream,
 * @param Vector3 v Input vector, which will be filled with read values.
 * @return istream Returns input stream for fluent interface.
 */ 
istream &operator>>(istream &is, Vector3 &v)
{
	is >> v[0] >> v[1] >> v[2];
	return is;
} // operator>>



/**
 * Output operator. Write all three vector's component to output stream.
 * @param ostream os Output stream,
 * @param Vector3 v Output vector, which will be write to stream.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const Vector3 &v)
{
	os << "(" << v[0] << " " << v[1] << " " << v[2] << ")";
	return os;
} // operator<<



/**
 * Operator for multiply 3D vector with 4x4 matrix and store result into same
 * vector. This vector will be returns as l-value.
 * @param Matrix m Matrix for multiplication.
 * @return Vector3 Returns result of multiplication vector and matrix.
 */ 
Vector3 &Vector3::operator*=(const Matrix &m)
{
	*this = *this * m;
	return *this;
} // operator*=



/** ***************************************************************************/
/** end of file Vector3.cpp													  */
/** ***************************************************************************/
