/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    ImageTexture.h													  *	
 * @date	11. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class ImageTexture.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_IMAGE_TEXTURE_H
#define _AFR_IMAGE_TEXTURE_H


// Including system libraries
#include <string>


// Including project libraries
#include "Texture.h"


// Used namespaces
using namespace std;


// Predefinition of classes
class Image;


/**
 * Class ImageTexture
 * @brief Class for textures from image files.
 */ 
class ImageTexture : public Texture
{
public:
	///< Implicit constructors
	ImageTexture(const string &n, const bool &u = true);
	
	///< Implicit destructors
	virtual ~ImageTexture();

	///< Texture operations
	virtual Color 	GetTexel(const Vector2 &uv, const Vector3 &p);
	virtual Vector3	GetNormal(const Vector2 &uv, const Vector3 &p);	
	
private:
	Image 	*image;
	Image	*normalMap;
}; // class ImageTexture



#endif // #ifndef _AFR_IMAGE_TEXTURE_H



/** ***************************************************************************/
/** end of file ImageTexture.h													  */
/** ***************************************************************************/
