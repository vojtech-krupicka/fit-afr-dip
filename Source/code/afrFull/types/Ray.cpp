/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Ray.cpp															  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Ray.									  *
 **************************************************************************** */


// Including project libraries
#include "Ray.h"



/**
 * Input operator. From input stream read four float values and stores into a
 * vector to each vector's component.
 * @param istream is Input stream,
 * @param Ray r Input ray, which will be filled with read values.
 * @return istream Returns input stream for fluent interface.
 */ 
istream &operator>>(istream &is, Ray &r)
{
	Vector3 origin, direction;
	is >> origin >> direction;
	
	r.o(origin).d(direction);
	return is;
} // operator>>



/**
 * Output operator. Write all four vector's component to output stream.
 * @param ostream os Output stream,
 * @param Ray r Output ray, which will be write to stream.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const Ray &r)
{
	os << "(" << r.o() << " + t*" << r.d() << ")";
	return os;
} // operator<<



/** ***************************************************************************/
/** end of file Ray.cpp														  */
/** ***************************************************************************/
