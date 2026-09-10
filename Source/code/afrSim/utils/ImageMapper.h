/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    ImageMapper.h													  *	
 * @date	04. 12. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class ImageMapper.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_IMAGE_MAPPER_H
#define _AFR_IMAGE_MAPPER_H


// Including system libraries
#include <string>
#include <fstream>
#include <iostream>


// Including project libraries
#include "../exceptions/Exceptions.h"
#include "../types/Color.h"
#include "../utils/StringUtils.h"

#include "./imageIO/ImagePPM.h"



// Predefinition of classes
#define IMAGE_TYPE_DEFAULT 	4
#define IMAGE_TYPE_BMP		1
#define IMAGE_TYPE_JPEG		2
#define IMAGE_TYPE_PNG		3
#define IMAGE_TYPE_PPM		4
#define IMAGE_TYPE_TIFF		5
#define IMAGE_TYPE_TGA		6
#define IMAGE_TYPE_HDR		7
#define IMAGE_TYPE_RGBA		8
#define IMAGE_TYPE_AUTO	 	9




/**
 * Class ImageMapper
 * @brief Helper static class for read and save images in specific format.
 */ 
class ImageMapper
{
public:
	
	
	/**
	 * Static method for read image file with name 'fn' from disk in specified
	 * image format. If format is not recognized, use default PPM format instead.
	 * @param string fn Image file name,
	 * @param Color data Two dimensional array of image data as colors,
	 * @param int width Real width of image,
	 * @param int height Real height of image,
	 * @param short type Image format type.
	 * @return bool Returns true, if image has been sucessfully read, false otherwise.
	 */ 
	static bool Read(const string &fn, Color **(&data), int &width, int &height, short type = IMAGE_TYPE_AUTO)
	{
		// Try to load file with given name
		ifstream in;
		in.open(fn.c_str(), ios::in | ios::binary);
		if(!in.is_open()) {
			throw FileNotFoundException("", 0, fn);
			return false;
		}
		
		string ext = "";
		
		// Handle situation if image type is auto. Type will be recognized by
		// file extension from name, if no extension founded, use PPM as default
		if(type == IMAGE_TYPE_AUTO) {
			size_t pos = fn.find_last_of(".");
			if(pos != string::npos) {
				ext = StrToLower(fn.substr(pos+1));
			}
			else {
				type = IMAGE_TYPE_DEFAULT;
			}
		}
		
		// Choose write utility class to read image from file in right format
		if(type == IMAGE_TYPE_BMP || ext == "bmp") {
			//return ImageBMP::Read(in, data, width, height);
		}
		else if(type == IMAGE_TYPE_JPEG || ext == "jpg" || ext == "jpeg") {
			//return ImageJPEG::Read(in, data, width, height);
		}
		else if(type == IMAGE_TYPE_PNG || ext == "png") {
			//return ImagePNG::Read(in, data, width, height);
		}
		else if(type == IMAGE_TYPE_TIFF || ext == "tif" || ext == "tiff") {
			//return ImageTIFF::Read(in, data, width, height);
		}
		else if(type == IMAGE_TYPE_TGA || ext == "tga") {
			//return ImageTGA::Read(in, data, width, height);
		}
		else if(type == IMAGE_TYPE_HDR || ext == "hdg") {
			//return ImageHDR::Read(in, data, width, height);
		}
		else if(type == IMAGE_TYPE_RGBA || ext == "rgba") {
			//return ImageRGBA::Read(in, data, width, height);
		}
		else {
			return ImagePPM::Read(in, data, width, height);
		}
	
		return false;
	} // Read()
	
	
	
	/**
	 * Static method for write image file with name 'fn' from disk in specified
	 * image format. If format is not recognized, use default PPM format instead.
	 * @param string fn Image file name,
	 * @param Color data Two dimensional array of image data as colors,
	 * @param int width Real width of image,
	 * @param int height Real height of image,
	 * @param short type Image format type.
	 * @return bool Returns true, if image has been sucessfully written, false otherwise.
	 */ 
	static bool Write(const string &fn, Color **(&data), const int &width, const int &height, short type = IMAGE_TYPE_AUTO)
	{
		// Assert
		if(!data || width < 0 || height < 0) {
			return false;
		}
		
		// Try to load file with given name
		ofstream out;
		out.open(fn.c_str(), ios::out | ios::binary);
		if(!out.is_open()) {
			throw FileNotFoundException("", 0, fn);
			return false;
		}
	
		string ext = "";
		
		// Handle situation if image type is auto. Type will be recognized by
		// file extension from name, if no extension founded, use PPM as default
		if(type == IMAGE_TYPE_AUTO) {
			size_t pos = fn.find_last_of(".");
			if(pos != string::npos) {
				ext = StrToLower(fn.substr(pos+1));
			}
			else {
				type = IMAGE_TYPE_DEFAULT;
			}
		}
		
		// Choose write utility class to read image from file in right format
		if(type == IMAGE_TYPE_BMP || ext == "bmp") {
			//return ImageBMP::Write(out, data, width, height);
		}
		else if(type == IMAGE_TYPE_JPEG || ext == "jpg" || ext == "jpeg") {
			//return ImageJPEG::Write(out, data, width, height);
		}
		else if(type == IMAGE_TYPE_PNG || ext == "png") {
			//return ImagePNG::Write(out, data, width, height);
		}
		else if(type == IMAGE_TYPE_TIFF || ext == "tif" || ext == "tiff") {
			//return ImageTIFF::Write(out, data, width, height);
		}
		else if(type == IMAGE_TYPE_TGA || ext == "tga") {
			//return ImageTGA::Write(out, data, width, height);
		}
		else if(type == IMAGE_TYPE_HDR || ext == "hdr") {
			//return ImageHDR::Write(out, data, width, height);
		}
		else if(type == IMAGE_TYPE_RGBA || ext == "rgba") {
			//return ImageRGBA::Write(out, data, width, height);
		}
		else {
			return ImagePPM::Write(out, data, width, height);
		}
		
		return false;
	} // Write()
	

}; // class ImageMapper



#endif // #ifndef _AFR_IMAGE_MAPPER_H



/** ***************************************************************************/
/** end of file ImageMapper.h												  */
/** ***************************************************************************/
