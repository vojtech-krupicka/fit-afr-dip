/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    ColorRGB.cpp													  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class ColorRGB.								  *
 **************************************************************************** */


// Including project libraries
#include "ColorRGB.h"



/**
 * Input operator. From input stream read three float values and stores into a
 * color to each R, G, B element.
 * @param istream is Input stream,
 * @param ColorRGB v Input color, which will be filled with read values.
 * @return istream Returns input stream for fluent interface.
 */ 
istream &operator>>(istream &is, ColorRGB &c)
{
	is >> c[0] >> c[1] >> c[2];
	return is;
} // operator>>



/**
 * Output operator. Write all three color's RGB elements to output stream.
 * @param ostream os Output stream,
 * @param ColorRGB v Output color, which will be write to stream.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const ColorRGB &c)
{
	os << "(" << c[0] << " " << c[1] << " " << c[2] << ")";
	return os;
} // operator<<



/** ***************************************************************************/
/** end of file ColorRGB.cpp												  */
/** ***************************************************************************/
