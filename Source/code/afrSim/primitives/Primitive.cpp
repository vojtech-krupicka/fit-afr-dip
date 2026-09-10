/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Primitive.cpp													  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Primitive.								  *
 **************************************************************************** */


// Including project libraries
#include "Primitive.h"
#include "BBox.h"



/**
 * Primitive constructor.
 * @return Primitive.
 */ 
Primitive::Primitive()
{
	this->material = NULL;
	this->texture = NULL;
	this->bbox = NULL;
} // Primitive()



/**
 * Primitive constructor. Assign material and texture to primitive.
 * @param TMaterial m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @return Primitive.
 */ 
Primitive::Primitive(Material *m, Texture *t = NULL)
{
	this->material = m;
	this->texture = t;
	this->bbox = NULL;
} // Primitive()



/**
 * Implicit primitive destructor.
 */ 
Primitive::~Primitive()
{
	this->material = NULL;
	this->texture = NULL;
	
	if(this->bbox != NULL) {
		delete this->bbox;
		this->bbox = NULL;
	}
} // ~Primitive()



/** ***************************************************************************/
/** end of file Primitive.cpp												  */
/** ***************************************************************************/
