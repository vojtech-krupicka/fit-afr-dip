/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    ImagePPM.h														  *	
 * @date	04. 12. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class ImagePPM.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_IMAGE_PPM_H
#define _AFR_IMAGE_PPM_H



// Including system libraries
#include <string>
#include <fstream>
#include <iostream>



// Including project libraries
#include "../../types/Color.h"



/**
 * Class ImagePPM
 * @brief Static class for read and store image in PPM format.
 */ 
class ImagePPM
{
public:
	
	
	/**
	 * Static method for read image file with name 'fn' from disk in specified
	 * image format. If format is not recognized, use default PPM format instead.
	 * @param ifstream in Input file stream,
	 * @param Color data Two dimensional array of image data as colors,
	 * @param int width Real width of image,
	 * @param int height Real height of image,
	 * @return bool Returns true, if image has been sucessfully read, false otherwise.
	 */ 
	static bool Read(ifstream &in, Color **(&data), int &width, int &height)
	{
		// Test, if input stream has been already opened
		if(!in.is_open()) {
			return false;
		}
		
		ImagePPM::EatWhitespace(in);
		ImagePPM::EatComments(in);
		ImagePPM::EatWhitespace(in);
		
		// Read PPM file header (P6 are supported)
		string header;
		in >> header;
		if(header != "P6") {
			return false;
		}
		
		ImagePPM::EatWhitespace(in);
		ImagePPM::EatComments(in);
		ImagePPM::EatWhitespace(in);
		
		// Read PPM file header (width, height and depth)
		int depth = 0;
		in >> width >> height >> depth;
		if(width <= 0 || height <= 0 || depth != 255) {
			return false;
		}
		
		char ch = in.get();
		if(ch == '#') {
			in.unget();
			ImagePPM::EatComments(in);
		}
		
		
		// Allocate raster data
		data = new Color*[height];
		for(int i = 0; i < height; i++) {
			data[i] = new Color[width];
		}
		
		// Read data from file and store into raster
		char red, green, blue;
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				in.get(red);
				in.get(green);
				in.get(blue);
				
				data[i][j] = Color((float)((unsigned char)red) / 255.0,
								   (float)((unsigned char)green) / 255.0,
								   (float)((unsigned char)blue) / 255.0);
			}			
		}
		
		// Close input file
		in.close();
		
		return true;
	} // Read()
	
	
	
	/**
	 * Static method for write image file with name 'fn' from disk in specified
	 * image format. If format is not recognized, use default PPM format instead.
	 * @param ofstream out Output file stream,
	 * @param Color data Two dimensional array of image data as colors,
	 * @param int width Real width of image,
	 * @param int height Real height of image,
	 * @return bool Returns true, if image has been sucessfully written, false otherwise.
	 */ 
	static bool Write(ofstream &out, Color **(&data), const int &width, const int &height)
	{
		// Test, if input stream has been already opened
		if(!out.is_open()) {
			return false;
		}
		
		// Write output PPM header (P6, width, height and depth)
		out << "P6" << endl;
		out << width << " " << height << endl;
		out << "255" << endl;
		
		// Write raster to output stream
		unsigned red, green, blue;
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				// Clamped values to [0, 255] range
				red = (unsigned)(256*data[i][j].r());
				green = (unsigned)(256*data[i][j].g());
				blue = (unsigned)(256*data[i][j].b());
				
				red = (red > 255) ? 255 : red;
				green = (green > 255) ? 255 : green;
				blue = (blue > 255) ? 255 : blue;
				
				// Write each color component to output
				out.put((unsigned char)(red));
				out.put((unsigned char)(green));
				out.put((unsigned char)(blue));
			}	
		}
		
		return true;
	} // Write()
	
	
private:
	
	
	/**
	 * Auxiliary method for read and skip comments in PPM file.
	 * @param ifstream Stream with input PPM file.
	 */ 
	static void EatComments(ifstream &in) 
	{
		char ch;
		ch = in.get();
		if(ch == '#') {
			string comment;
			getline(in, comment);
			
			ch = in.get();
		}

		in.unget();
		
		return;
	} // EatComments()
	
	
	/**
	 * Auxiliary method for read and skip white characters in PPM file.
	 * @param ifstream Stream with input PPM file.
	 */ 
	static void EatWhitespace(ifstream &in)
	{
		char ch;
		ch = in.get();
		while(ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\f') {
			ch = in.get();
		}
		
		in.unget();

		return;
	} // EatWhitespace()
	

}; // class ImagePPM



#endif // #ifndef _AFR_IMAGE_PPM_H



/** ***************************************************************************/
/** end of file ImagePPM.h													  */
/** ***************************************************************************/
