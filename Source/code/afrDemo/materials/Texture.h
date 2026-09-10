/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Texture.h														  *	
 * @date	11. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Texture.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_TEXTURE_H
#define _AFR_TEXTURE_H



// Including system libraries
#include <string>



// Including project libraries
#include "../types/Array.h"
#include "../types/Color.h"
#include "../types/Vector2.h"
#include "../types/Vector3.h"



/**
 * Class Texture
 * @brief Virtual class for all textures.
 */ 
class Texture
{
public:
	///< Default constructors
	Texture(const string &n, const bool &u = true) { this->name = n; this->useNormal = u; };
	
	/// Default destructors
	virtual ~Texture() {};

	///< Texture operations
	virtual Color 	GetTexel(const Vector2 &uv, const Vector3 &p) = 0;	
	virtual Vector3	GetNormal(const Vector2 &uv, const Vector3 &p) = 0;	
	
	///< Getters
	inline string 	GetName() const { return this->name; };
	inline bool		HasNormalMap() const { return this->useNormal; };
	
protected:
	string 	name;
	bool	useNormal;
}; // class Texture



/**
 * Define new type for list of textures as dynamic array of textures where 
 * texture is a pointer on a class Texture.
 */ 
typedef Array<Texture*> TTextureList;



#endif // #ifndef _AFR_TEXTURE_H



/** ***************************************************************************/
/** end of file Texture.h													  */
/** ***************************************************************************/
