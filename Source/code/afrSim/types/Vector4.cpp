/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Vector4.cpp														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Vector4.								  *
 **************************************************************************** */


// Including project libraries
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"



/**
 * Copy constructor. Creates 4D vector from 2D vector. Unused components are 0.
 * @param Vector2 v Original vector, which will be copied.
 * @return Vector4.
 */ 
Vector4::Vector4(const Vector2 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = 0;
	this->data[3] = 0;
} // Vector4()



/**
 * Copy constructor. Creates 4D vector from 3D vector. Unused components are 0.
 * @param Vector3 v Original vector, which will be copied.
 * @return Vector4.
 */ 
Vector4::Vector4(const Vector3 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
	this->data[3] = 0;
} // Vector4()



/**
 * Input operator. From input stream read four float values and stores into a
 * vector to each vector's component.
 * @param istream is Input stream,
 * @param Vector4 v Input vector, which will be filled with read values.
 * @return istream Returns input stream for fluent interface.
 */ 
istream &operator>>(istream &is, Vector4 &v)
{
	is >> v[0] >> v[1] >> v[2] >> v[3];
	return is;
} // operator>>



/**
 * Output operator. Write all four vector's component to output stream.
 * @param ostream os Output stream,
 * @param Vector4 v Output vector, which will be write to stream.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const Vector4 &v)
{
	os << "(" << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << ")";
	return os;
} // operator<<



/**
 * Operator for multiply 4D vector with 4x4 matrix and store result into same
 * vector. This vector will be returns as l-value.
 * @param Matrix m Matrix for multiplication.
 * @return Vector4 Returns result of multiplication vector and matrix.
 */ 
Vector4 &Vector4::operator*=(const Matrix &m)
{
	*this = *this * m;
	return *this;
} // operator*=



/** ***************************************************************************/
/** end of file Vector4.cpp													  */
/** ***************************************************************************/
