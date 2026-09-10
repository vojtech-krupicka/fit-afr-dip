/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    ImageTexture.cpp												  *	
 * @date	11. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class ImageTexture.							  *
 **************************************************************************** */


// Including project libraries
#include "ImageTexture.h"
#include "../types/Image.h"



/**
 * Implicit constructor. Stored texture filename for future load.
 * @param string f Texture filename.
 * @param bool u Use Normal map flag.
 * @return ImageTexture.
 */ 
ImageTexture::ImageTexture(const string &n, const bool &u)
	: Texture(n, u), image(NULL), normalMap(NULL)
{} // ImageTexture()



/**
 * Implicit destructor. Deletes image memory.
 */ 
ImageTexture::~ImageTexture()
{
	if(this->image) {
		delete this->image;
	}
	
	if(this->normalMap) {
		delete this->normalMap;
	}
} // ImageTexture()



/**
 * Virtual method for determine a real pixel color. Do bilinear interpolation.
 * @param Vector2 uv Texture's coordinates on pritive's face.
 * @param Vector3 p Hit point, intersect between primitive face and ray.
 * @return Color Return real color on the surface.
 */ 
Color ImageTexture::GetTexel(const Vector2 &uv, const Vector3 &p)
{
	// Test if image has been already loaded
	if(this->image == NULL) {
		try {
			// Create Image object
			this->image = new Image();
			
			// Something wrong, can't read texture image, returns black color
			if(this->image == NULL || !this->image->Load(this->name)) {
				return Color::Black();
			}
		}
		catch(FileNotFoundException &e) {
			return Color::Black();
		}
	}
	
	// fetch a bilinearly filtered texel
	float fu = (uv.x() + 1000.0f) * this->image->GetWidth();
	float fv = (uv.y() + 1000.0f) * this->image->GetHeight();
	
	int u1 = (((int)fu) % this->image->GetWidth());
	int u2 = (u1 + 1) % this->image->GetWidth();
	
	int v1 = (((int)fv) % this->image->GetHeight());
	int v2 = (v1 + 1) % this->image->GetHeight();
	
	// calculate fractional parts of u and v
	float fracu = fu - floorf( fu );
	float fracv = fv - floorf( fv );
	// calculate weight factors
	float w1 = (1 - fracu) * (1 - fracv);
	float w2 = fracu * (1 - fracv);
	float w3 = (1 - fracu) * fracv;
	float w4 = fracu *  fracv;
	// fetch four texels
	Color c1 = this->image->GetPixel(u1, v1);
	Color c2 = this->image->GetPixel(u1, v2);
	Color c3 = this->image->GetPixel(u2, v1);
	Color c4 = this->image->GetPixel(u2, v2);
	// scale and sum the four colors
	return c1 * w1 + c2 * w2 + c3 * w3 + c4 * w4;
} // GetTexel()



/**
 * Virtual method for get normal from normal map.
 * @param Vector2 uv Texture's coordinates on pritive's face.
 * @param Vector3 p Hit point, intersect between primitive face and ray.
 * @return Vector Return real normal vector on the surface.
 */ 
Vector3 ImageTexture::GetNormal(const Vector2 &uv, const Vector3 &p)
{
	// If no normal map exists or using normal map is forbiden return unit vector
	if(!this->useNormal) {
		return Vector3::One();
	}
	
	// If normal bitmap hasn't been loaded yet, try to load it
	if(this->normalMap == NULL) {
		// Create normal map name 'name' + '_normal' + '.ext'
		string nmName = this->name;
		size_t pos = nmName.find_last_of(".");
		if(pos != string::npos) {
			string ext = nmName.substr(pos+1);
			nmName = nmName.substr(0, pos);
			nmName = nmName + "_normal." + ext;
		}
		else {
			nmName = nmName + "_normal.ppm";
		}

		// Create Image object
		try {
			this->normalMap = new Image();
			
			// Something wrong, can't read texture image, returns black color
			if(this->normalMap == NULL || !this->normalMap->Load(nmName)) {
				this->useNormal = false;
				return Vector3::One();
			}
		}
		catch(FileNotFoundException &e) {
			this->useNormal = false;
			return Vector3::One();
		}
	}
	
	// fetch a bilinearly filtered texel
	float fu = (uv.x() + 1000.0f) * this->normalMap->GetWidth();
	float fv = (uv.y() + 1000.0f) * this->normalMap->GetHeight();
	
	int u1 = (((int)fu) % this->normalMap->GetWidth());
	int u2 = (u1 + 1) % this->normalMap->GetWidth();
	
	int v1 = (((int)fv) % this->normalMap->GetHeight());
	int v2 = (v1 + 1) % this->normalMap->GetHeight();
	
	// calculate fractional parts of u and v
	float fracu = fu - floorf( fu );
	float fracv = fv - floorf( fv );
	
	// calculate weight factors
	float w1 = (1 - fracu) * (1 - fracv);
	float w2 = fracu * (1 - fracv);
	float w3 = (1 - fracu) * fracv;
	float w4 = fracu *  fracv;
	
	// fetch four texels
	Color c1 = this->normalMap->GetPixel(u1, v1);
	Color c2 = this->normalMap->GetPixel(u1, v2);
	Color c3 = this->normalMap->GetPixel(u2, v1);
	Color c4 = this->normalMap->GetPixel(u2, v2);
	
	Vector3 n1(c1.r(), c1.g(), c1.b());
	Vector3 n2(c2.r(), c2.g(), c2.b());
	Vector3 n3(c3.r(), c3.g(), c3.b());
	Vector3 n4(c4.r(), c4.g(), c4.b());
	
	// scale and sum the four colors
	Vector3 vn = (n1 * w1 + n2 * w2 + n3 * w3 + n4 * w4);
	return (2.0f * (vn - 0.5f));
} // GetNormal()



/** ***************************************************************************/
/** end of file ImageTexture.cpp											  */
/** ***************************************************************************/
