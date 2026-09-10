/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Vector2.cpp														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Vector2.								  *
 **************************************************************************** */


// Including project libraries
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"



/**
 * Copy constructor. Creates 2D vector from 3D vector.
 * @param Vector3 v Original vector, which will be copied.
 * @return Vector2.
 */ 
Vector2::Vector2(const Vector3 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
} // Vector2()



/**
 * Copy constructor. Creates 2D vector from 4D vector.
 * @param Vector4 v Original vector, which will be copied.
 * @return Vector2.
 */ 
Vector2::Vector2(const Vector4 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
} // Vector2()



/**
 * Input operator. From input stream read two float values and stores into a
 * vector to each vector's component.
 * @param istream is Input stream,
 * @param Vector2 v Input vector, which will be filled with read values.
 * @return istream Returns input stream for fluent interface.
 */ 
istream &operator>>(istream &is, Vector2 &v)
{
	is >> v[0] >> v[1];
	return is;
} // operator>>



/**
 * Output operator. Write all three vector's component to output stream.
 * @param ostream os Output stream,
 * @param Vector2 v Output vector, which will be write to stream.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const Vector2 &v)
{
	os << "(" << v[0] << " " << v[1] << ")";
	return os;
} // operator<<



/**
 * Operator for multiply 2D vector with 4x4 matrix and store result into same
 * vector. This vector will be returns as l-value.
 * @param Matrix m Matrix for multiplication.
 * @return Vector2 Returns result of multiplication vector and matrix.
 */ 
Vector2 &Vector2::operator*=(const Matrix &m)
{
	*this = *this * m;
	return *this;
} // operator*=



/** ***************************************************************************/
/** end of file Vector2.cpp													  */
/** ***************************************************************************/
