/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Image.h															  *	
 * @date	11. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Image.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_IMAGE_H
#define _AFR_IMAGE_H


// Including system libraries
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


// Including project libraries
#include "../exceptions/Exceptions.h"
#include "../utils/MathUtils.h"
#include "../types/Color.h"


// Used namespaces
using namespace std;



/**
 * Class Image
 * @brief Image class for read, write and keep raster informations from bitmap 
 * image from file system.
 */ 
class Image
{
public:
	///< Implicit constructors
	Image();
	Image(const int &w, const int &h);
	Image(const int &w, const int &h, const Color &c);
	Image(const string &f);
	Image(const Image &im);

	///< Implicit destructors
	~Image();
	
	///< Accessors methods
	inline Image *SetBiLerp(const float &x, const float &y, const Color &clr);
	inline Image *SetLerp(const int &x, const int &y, const Color &clr);
	inline Image *SetPixel(const int &x, const int &y, const Color &clr);
	inline Color GetPixel(const int &x, const int &y) const;
	
	///< Getters
	inline int GetWidth() const { return this->width; };
	inline int GetHeight() const { return this->height; };
	
	///< Image operations
	Image* Clear(const Color &c);
	Image* Clear(const int &w, const int &h, const Color &c);
	Image* GammaCorrection(const float &gamma);
	Image* Grayscale(const float &r = 0.299f, const float &g = 0.587f, const float &b = 0.114f);
	Image* Channel(const float &r = 1.0f, const float &g = 1.0f, const float &b = 1.0f);
	Image* Crop(const float &x = 0.0f, const float &y = 0.0f, const float &w = 1.0f, const float &h = 1.0f);
	Image* Fill(const float &x, const float &y, const float &w, const float &h, const Color &clr, const float &o = 1.0f);
	Image* Fill(const Image* const im, const float &dox, const float &doy, const float &sox, const float &soy, const float &sw, const float &sh, const float &o = 1.0f);
	Image* Subtract(const Image* const im, const float &t = 0.05f);
	Image* Diff(const Image* const im, const Color &clr = Color::Green(), const float &t = 0.0f);
	Image* Copy(const float &x = 0.0f, const float &y = 0.0f, const float &w = 1.0f, const float &h = 1.0f);
	float  Statistics(const float &t = 0.05f);
	
	///< Image IO operations
	bool Load(const string &f);
	bool Save(const string &f);
	
	unsigned ToFloat(float *(&buffer), const unsigned depth = 3) const;
	
private:
	string 	name;		///< Filename for read and save.
	
	Color 	**data;		///< 2D raster of pixels with its colors.
	
	int 	width;		///< Image width.
	int		height;		///< Image height.
}; // class Image



inline Image *Image::SetBiLerp(const float &x, const float &y, const Color &clr)
{
	if(this->data && y >= 0 && y < this->height && x >= 0 && x < this->width) {
		// fetch a bilinearly filtered texel
		float fu = (x + 1000.0f) * this->width;
		float fv = (y + 1000.0f) * this->height;
		
		int u1 = (((int)fu) % this->width);
		int u2 = (u1 + 1) % this->width;
		
		int v1 = (((int)fv) % this->height);
		int v2 = (v1 + 1) % this->height;
		
		// calculate fractional parts of u and v
		float fracu = fu - floorf( fu );
		float fracv = fv - floorf( fv );
		// calculate weight factors
		float w1 = (1 - fracu) * (1 - fracv);
		float w2 = fracu * (1 - fracv);
		float w3 = (1 - fracu) * fracv;
		float w4 = fracu *  fracv;
		// fetch four texels
		Color c1 = this->data[v1][u1];
		Color c2 = this->data[v1][u2];
		Color c3 = this->data[v2][u1];
		Color c4 = this->data[v2][u2];
		
		int yy = y * this->height;
		int xx = x * this->width;
		
		// scale and sum the four colors
		this->data[yy][xx] = (clr + (c1 * w1 + c2 * w2 + c3 * w3 + c4 * w4)) / 2.0f;
	}
	
	return this;
} // SetBiLerp()



inline Image *Image::SetLerp(const int &x, const int &y, const Color &clr)
{
	if(this->data && y >= 0 && y < this->height && x >= 0 && x < this->width) {
		this->data[y][x] = (1.5f * clr + 0.5f * this->data[y][x]) / 2.0f;
	}
	
	return this;
} // SetLerp()



/**
 * Set color to pixel at position x,y.
 * @param int x Column in raster,
 * @param int y Row in raster,
 * @param Color clr Color of the pixel.
 * @return Image* Return image for fluent interface.
 */ 
inline Image *Image::SetPixel(const int &x, const int &y, const Color &clr)
{
	if(this->data && y >= 0 && y < this->height && x >= 0 && x < this->width) {
		this->data[y][x] = clr;
	}
	
	return this;
} // SetPixel()



/**
 * Get a pixel's color at position x,y.
 * @param int x Column in raster,
 * @param int y Row in raster,
 * @return Color Returns pixel color.
 */ 
inline Color Image::GetPixel(const int &x, const int &y) const
{
	if(this->data && y >= 0 && y < this->height && x >= 0 && x < this->width) {
		return this->data[y][x];
	}
	
	return Color::Black();
} // GetPixel()



#endif // #ifndef _AFR_IMAGE_H



/** ***************************************************************************/
/** end of file Image.h													  */
/** ***************************************************************************/
