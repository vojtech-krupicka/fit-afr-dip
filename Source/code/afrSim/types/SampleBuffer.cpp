/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    SampleBuffer.cpp												  *	
 * @date	29. 04. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class SampleBuffer.							  *
 **************************************************************************** */



// Including project libraries
#include "SampleBuffer.h"



/**
 * Default constructor.
 * @return SampleBuffer.
 */ 
SampleBuffer::SampleBuffer()
{
	this->resolution = Vector2::Zero();
	this->depth = 0;
	this->samples = NULL;
	this->idxPtr = NULL;
} // SampleBuffer()



/**
 * Constructor with number of tiles on horizontal and vertical direction.
 * @param unsigned resX Width of buffer in pixels,
 * @param unsigned resY Height of buffer in pixels,
 * @param unsigned sd Depth of buffer.
 * @return SampleBuffer.
 */ 
SampleBuffer::SampleBuffer(const unsigned &resX, const unsigned &resY, const unsigned &sd)
{
	// Init resolutions
	this->resolution = Vector2((float)resX, (float)resY);
	this->depth = sd;
	
	// Init sample array and index pointer array
	this->samples = new TSample**[resX];
	this->idxPtr = new unsigned*[resX];
	
	for(int x = 0; x < this->resolution.x(); x++) {
		this->samples[x] = new TSample*[resY];
		this->idxPtr[x] = new unsigned[resY];
		
		for(int y = 0; y < this->resolution.y(); y++) {
			this->samples[x][y] = new TSample[sd];
			this->idxPtr[x][y] = sd-1;	// set to the last position: the first sample will be take next -> see GetNextSample() method.
		}
	}	
} // SampleBuffer()



/**
 * Default desctructor.
 */ 
SampleBuffer::~SampleBuffer() 
{	
	// Delete all samples and indices in both buffers
	for(int x = 0; x < this->resolution.x(); x++) {
		for(int y = 0; y < this->resolution.y(); y++) {
			delete [] this->samples[x][y];
		}
		
		delete [] this->samples[x];
		delete [] this->idxPtr[x];
	}
	
	delete [] this->samples;
	delete [] this->idxPtr;
	
	// Reinit all properties	
	this->resolution = Vector2::Zero();
	this->depth = 0;
	this->samples = NULL;
	this->idxPtr = NULL;
} // ~SampleBuffer() 



/** ***************************************************************************/
/** end of file SampleBuffer.cpp											  */
/** ***************************************************************************/

