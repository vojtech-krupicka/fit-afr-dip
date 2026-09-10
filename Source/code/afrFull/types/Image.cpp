/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Image.cpp														  *	
 * @date	11. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Image.									  *
 **************************************************************************** */


// Including project libraries
#include "Image.h"
#include "../utils/ImageMapper.h"


/**
 * Default constructor for Image. Sets dimension to -1.
 * @return Image.
 */ 
Image::Image()
{
	this->width = -1;
	this->height = -1;
	this->data = NULL;
} // Image()



/**
 * Constructor for creates image with width and height and allocs memory for
 * store pixel and fill raster with default black color.
 * @param int w Image width in pixels,
 * @param int h Image height in pixel.
 * @return Image.
 */ 
Image::Image(const int &w, const int &h)
{
	this->width = w;
	this->height = h;
	this->data = NULL;
	
	this->Clear(Color::Black());
} // Image()



/**
 * Constructor for creates image with width and height and allocs memory for
 * store pixel and fill raster with color c.
 * @param int w Image width in pixels,
 * @param int h Image height in pixel.
 * @param Color c Background color for all pixels in raster.
 * @return Image.
 */ 
Image::Image(const int &w, const int &h, const Color &c)
{
	this->width = w;
	this->height = h;
	this->data = NULL;
	
	this->Clear(c);
} // Image()



/**
 * Constructor for creates image from PPM file in file system with name f.
 * @param string f Image filename in file system.
 * @return Image.
 */ 
Image::Image(const string &f)
{
	this->width = -1;
	this->height = -1;
	this->data = NULL;
	
	this->Load(f);
} // Image()



/**
 * Constructor for creates image from PPM file in file system with name f.
 * @param string f Image filename in file system.
 * @return Image.
 */ 
Image::Image(const Image &im)
{
	this->width = im.GetWidth();
	this->height = im.GetHeight();
	this->data = NULL;
	
	this->Clear(Color::Black());
	
	for(int y = 0; y < this->height; y++) {
		for(int x = 0; x < this->width; x++) {
			this->data[y][x] = im.GetPixel(x, y);
		}
	}
} // Image()



/**
 * Default destructor. Destroy pixel raster and sets dimensions to -1.
 */ 
Image::~Image()
{
	for(int i = 0; i < this->height; i++) {
		delete [] this->data[i];
	}
	delete [] this->data;
	this->data = NULL;
	
	this->width = -1;
	this->height = -1;
} // Image()



/**
 * Method for clear raster and sets all pixel to color c.
 * @param Color c New background color.
 * @return Image Returns image for fluent interface.
 */ 
Image* Image::Clear(const Color &c)
{
	if(this->width < 0 || this->height < 0) {
		return this;
	}
	else if(this->data == NULL) {
		this->data = new Color*[this->height];
		for(int i = 0; i < this->height; i++) {
			this->data[i] = new Color[this->width];
			for(int j = 0; j < this->width; j++) {
				this->data[i][j] = c;
			}
		}
	}
	else {
		for(int i = 0; i < this->height; i++) {
			for(int j = 0; j < this->width; j++) {
				this->data[i][j] = c;
			}
		}
	}
	
	return this;
} // Clear()



/**
 * Method for clear raster and sets new dimensions and all pixels to color c.
 * @param int w Image width in pixels,
 * @param int h Image height in pixel.
 * @param Color c New background color.
 * @return Image Returns image for fluent interface.
 */ 
Image* Image::Clear(const int &w, const int &h, const Color &c)
{
	if(this->data && this->width != w && this->height != h) {
		for(int i = 0; i < this->height; i++) {
			delete [] this->data[i];
		}
		delete [] this->data;
		this->data = NULL;
		
		this->width = w;
		this->height = h;
	}
	
	return this->Clear(c);
} // Clear()



/**
 * Method for correct pixels colors with given gamma. Image will be darker
 * (gamma < 1.0) or brighter (gamma > 0).
 * @param float gamma Gamma constant.
 * @return Image Returns image for fluent interface.
 */ 
Image* Image::GammaCorrection(const float &gamma)
{
	if(!this->data) {
		return this;
	}
	
	Color temp;
	float power = 1.0 / gamma;
	for(int i = 0; i < this->height; i++) {
		for(int j = 0; j < this->width; j++) {
			temp = this->data[i][j];
			this->data[i][j] = Color(pow(temp.r(), power), pow(temp.g(), power), pow(temp.b(), power));
		}
	}
	
	return this;
} // GammaCorrection()



/**
 * Method to convert image to grayscale.
 * @param float r Red multiplier,
 * @param float g Green multiplier,
 * @param float b Blue multiplier.
 * @return Image Returns image for fluent interface.
 */ 
Image* Image::Grayscale(const float &r, const float &g, const float &b)
{
	if(!this->data) {
		return this;
	}
	
	for(int i = 0; i < this->height; i++) {
		for(int j = 0; j < this->width; j++) {
			this->data[i][j] = this->data[i][j].Grayscale(r, g, b);
		}
	}
	
	return this;
} // Grayscale()



/**
 * Method to mask color channel.
 * @param float r Red multiplier,
 * @param float g Green multiplier,
 * @param float b Blue multiplier.
 * @return Image Returns image for fluent interface.
 */ 
Image* Image::Channel(const float &r, const float &g, const float &b)
{
	if(!this->data) {
		return this;
	}
	
	for(int i = 0; i < this->height; i++) {
		for(int j = 0; j < this->width; j++) {
			this->data[i][j] = this->data[i][j].Channel(r, g, b);
		}
	}
	
	return this;
} // Channel()



/**
 * Method for crop image from curent offset to specified resolution.
 * @param float x X offset where crop starts,
 * @param float y Y offset where crop starts,
 * @param float w Width of crop,
 * @param float h Height of crop.
 * @return Image Provides fluent interface.
 */ 
Image *Image::Crop(const float &x, const float &y, const float &w, const float &h)
{
	// Test, if data exists
	if(!this->data) {
		return this;
	}
	
	// Absolute position and size
	int xx = (int)(x*this->width);
	int yy = (int)(y*this->height);	
	int ww = Max(0, Min((int)(w*this->width), this->width-xx));
	int hh = Max(0, Min((int)(h*this->height), this->height-yy));
	
	// Copy pixels from old image to new one
	Color **tmp = new Color*[hh];
	for(int r = 0; r < hh; r++, yy++) {
		tmp[r] = new Color[ww];
		
		int cc = xx;
		for(int c = 0; c < ww; c++, cc++) {
			tmp[r][c] = this->data[yy][cc];
		}
	}
	
	// Delete actual data
	for(int i = 0; i < this->height; i++) {
		delete [] this->data[i];
	}
	delete [] this->data;
	this->data = tmp;
	tmp = NULL;
	
	// Sets new resolution
	this->width = ww;
	this->height = hh;
	
	return this;
} // Crop()



/**
 * Method for draw rectangle with solid color.
 * @param float x X offset where fill starts,
 * @param float y Y offset where fill starts,
 * @param float w Width of rectangle,
 * @param float h Height of rectangle,
 * @param Color clr Color to fill.
 * @param float o Opacity.
 * @return Image Provides fluent interface.
 */ 
Image* Image::Fill(const float &x, const float &y, const float &w, const float &h, const Color &clr, const float &o)
{
	// Test, if data exists
	if(!this->data) {
		return this;
	}
	
	// Absolute position and size
	int xx = Round(x*this->width);
	int yy = Round(y*this->height);	
	int ww = Max(0, Min((int)(Round(w*this->width)), this->width-xx));
	int hh = Max(0, Min((int)(Round(h*this->height)), this->height-yy));
	
	
	// Fill rectangle
	for(int r = 0; r < hh; r++, yy++) {
		int cc = xx;
		for(int c = 0; c < ww; c++, cc++) {
			this->data[yy][cc] = (o >= 0.99999f) ? clr : ((1.0f-o)*this->data[yy][cc]) + (o*clr);
		}
	}
	
	return this;
} // Fill()



/**
 * Method for fill actual image with other image.
 * @param Image im Source image,
 * @param float dox Destination X offset,
 * @param float doy Destination Y offset,
 * @param float sox Source X offset,
 * @param float soy Source Y offset,
 * @param float sw Source width,
 * @param float sy Source height,
 * @param float o Opacity.
 * @return Image Provides fluent interface.
 */ 
Image* Image::Fill(const Image* const im, const float &dox, const float &doy, const float &sox, const float &soy, const float &sw, const float &sh, const float &o)
{
	// Test, if data exists
	if(!this->data || !im) {
		return this;
	}
	
	
	// Absolute position and size
	int dxx = Round(dox*this->width);
	int dyy = Round(doy*this->height);	
	int sxx = Round(sox*im->GetWidth());
	int syy = Round(soy*im->GetHeight());
	int ww = Max(0, Min((int)(Round(sw*im->GetWidth())), im->GetWidth()-sxx));
	int hh = Max(0, Min((int)(Round(sh*im->GetHeight())), im->GetHeight()-syy));
	ww = Max(0, Min(ww, this->width-dxx));
	hh = Max(0, Min(hh, this->height-dyy));
	
	
	// Fill rectangle
	for(int r = 0; r < hh; r++, dyy++, syy++) {
		int dcc = dxx;
		int scc = sxx;
		for(int c = 0; c < ww; c++, dcc++, scc++) {
			this->data[dyy][dcc] = (o >= 0.99999f) ? im->GetPixel(scc, syy) : ((1.0f-o)*this->data[dyy][dcc]) + (o*im->GetPixel(scc, syy));
		}
	}
	
	
	return this;
} // Fill()



/**
 * Method for subtract image from this image.
 * @param Image im Second image,
 * @param float t Threshold, if subtracted color value is lower then threshold, its zero.
 * @return Image Provides fluent interface.
 */ 
Image* Image::Subtract(const Image* const im, const float &t)
{
	// Test, if data exists
	if(!this->data || !im) {
		return this;
	}
	
	
	// Determine new sizes and ofsets
	int ww = Max(0, Min(this->width, im->GetWidth()));
	int hh = Max(0, Min(this->height, im->GetHeight()));
	
	
	for(int r = 0; r < ww; r++) {
		for(int c = 0; c < hh; c++) {
			Color clr = Abs(this->data[r][c] - im->GetPixel(c, r));
			this->data[r][c] = (clr.Intensity() <= t) ? Color::Black() : clr;
		}
	}	
	
	
	return this;
} // Subtract()



/**
 * Method for do diff between two images. If pixels are different, new color 
 * will be set.
 * @param Image im Second image,
 * @param Color clr Color which will be se to pixels which are different,
 * @param float t Threshold, if color value is different then threshold.
 * @return Image Provides fluent interface.
 */ 
Image* Image::Diff(const Image* const im, const Color &clr, const float &t)
{
	// Test, if data exists
	if(!this->data || !im) {
		return this;
	}
	
	
	// Determine new sizes and ofsets
	int ww = Max(0, Min(this->width, im->GetWidth()));
	int hh = Max(0, Min(this->height, im->GetHeight()));
	
	
	for(int r = 0; r < ww; r++) {
		for(int c = 0; c < hh; c++) {
			Color color = Abs(this->data[r][c] - im->GetPixel(c, r));
			if((color.r() > t) && (color.g() > t) && (color.b() > t)) {
				this->data[r][c] = clr;
			}
		}
	}	
	
	
	return this;
} // Subtract()



/**
 * Method for do diff between two images. If pixels are different, new color 
 * will be set.
 * @param Image im Second image,
 * @param Color clr Color which will be se to pixels which are different,
 * @param float t Threshold, if color value is different then threshold.
 * @return Image Provides fluent interface.
 */ 
Image* Image::Copy(const float &x, const float &y, const float &w, const float &h)
{
	// Test, if data exists
	if(!this->data) {
		return this;
	}
	
	
	// Absolute position and size
	int xx = (int)(x*this->width);
	int yy = (int)(y*this->height);	
	int ww = Max(0, Min((int)(w*this->width), this->width-xx));
	int hh = Max(0, Min((int)(h*this->height), this->height-yy));
	
	Image *im = new Image(ww, hh);
	
	// Copy pixels from old image to new one
	for(int r = 0; r < hh; r++, yy++) {
		int cc = xx;
		for(int c = 0; c < ww; c++, cc++) {
			im->SetPixel(c, r, this->data[yy][cc]);
		}
	}
	
	
	return im;
} // Subtract()



/**
 * Method for read image with given name from file system.
 * @param string f Image filename in file system.
 * @return bool Return true, if image has been succefully read, false otherwise.
 */ 
bool Image::Load(const string &f)
{
	return ImageMapper::Read(f, this->data, this->width, this->height);
} // Load()



/**
 * Method for write image to given output stream. Only PPM are supported.
 * @param string f Image filename in file system.
 * @return bool Return true, if image has been succefully written, false otherwise.
 */ 
bool Image::Save(const string &f) 
{
	return ImageMapper::Write(f, this->data, this->width, this->height);	
} // Save()



/**
 * Method for store all pixels into float dynamic array where each pixel will be
 * store on three indices for each color component (R, G, and B).
 * @param float buffer Float array, where all pixels will be store,
 * @param unsigned depth Color component's count, for RGB color it will be 3.
 * @return unsigned Return size of buffer - width * height * depth.
 */ 
unsigned Image::ToFloat(float *(&buffer), const unsigned depth) const
{
	unsigned size = 0;
	
	// If buffer has been alloc before, delete it
	if(buffer != NULL) {
		delete [] buffer;
		buffer = NULL;
	}
	
	// Alloc new memory for buffer
	buffer = new float[this->width * this->height * depth];
	
	// Copy all pixels into buffer
	for(int y = 0; y < this->height; y++) {
		for(int x = 0; x < this->width; x++) {
			Color clr = this->GetPixel(x, y);
			for(int c = 0; c < 3; c++) {
				buffer[(((this->height - y - 1) * depth*this->width) + (depth * x)) + c] = clr[c];
				size++;
			}
		}
	}
	
	// Return real buffer size
	return size;
} // ToFloat()



/**
 * Calculate image statistic according to equation:
 * 		var = 1/(w*h) * E^{w}_{x=0}E^{h}_{y=0} (i(x, y)),
 * 		where:
 *			var is the final variance of image color,
 *			w is width in pixels,
 * 			h is height in pixels,
 * 			i is color intensity at [x,y] pixel.
 * @param float t Minimum valid intensity - threshold.
 * @return float Returns image variance.
 */ 
float Image::Statistics(const float &t)
{
	// Test, if data exists
	if(!this->data) {
		return 0.0f;
	}
	
	
	// Final variance
	float variance = 0.0f;
	
	
	// Calculate variance over all pixels
	for(int r = 0; r < this->height; r++) {
		for(int c = 0; c < this->width; c++) {
			float intensity = this->data[r][c].Intensity();
			variance += (intensity <= t) ? 0.0f : intensity;
		}
	}	
	
	
	// Normalize variance with image resolution in pixels
	variance /= (float)(this->width * this->height);
	
	
	return variance;
} // Statistics()



/** ***************************************************************************/
/** end of file Image.cpp													  */
/** ***************************************************************************/
