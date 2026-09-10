/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    SolidTexture.h													  *	
 * @date	11. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class SolidTexture.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_SOLID_TEXTURE_H
#define _AFR_SOLID_TEXTURE_H


// Including project libraries
#include "Texture.h"


// Used namespaces
using namespace std;


/**
 * Class SolidTexture
 * @brief Class for texture with one solid color.
 */ 
class SolidTexture : public Texture
{
public:
	///< Implicit constructors
	SolidTexture(const string &n, const Color &c, const bool &u) : Texture(n, u) { this->color = c; };
	
	///< Implicit destructors
	virtual ~SolidTexture() {};

	///< Texture operations
	virtual Color 	GetTexel(const Vector2 &uv, const Vector3 &p) { return this->color; };
	virtual Vector3	GetNormal(const Vector2 &uv, const Vector3 &p) { return Vector3::One(); };	
	
private:
	Color color;
}; // class SolidTexture



#endif // #ifndef _AFR_SOLID_TEXTURE_H



/** ***************************************************************************/
/** end of file SolidTexture.h													  */
/** ***************************************************************************/
