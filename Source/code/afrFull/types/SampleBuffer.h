/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    SampleBuffer.h													  *	
 * @date	29. 04. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class SampleBuffer.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_SAMPLE_BUFFER_H
#define _AFR_SAMPLE_BUFFER_H



// Including system libraries
#include <string>



// Including project libraries
#include "Sample.h"



// Predefinition of classes
class Vector2;



/**
 * Class SampleBuffer
 * @brief Class implementing 3D matrix for storing ResX * ResY * sd samples into
 * 3D dimensional array.
 */ 
class SampleBuffer
{
public:
	///< Implicit constructors
	SampleBuffer();
	SampleBuffer(const unsigned &resX, const unsigned &resY, const unsigned &sd);
	
	~SampleBuffer();
	
	
	///< SampleBuffer operations
	bool Init();
	
	
	///< Getters
	inline Vector2 	GetResolution() const { return this->resolution; };
	inline unsigned GetDepth() const { return this->depth; };
	
	inline TSample 	*GetSampleAt(const unsigned &x, const unsigned &y, const unsigned &i = -1);
	inline TSample 	*GetNextSampleAt(const unsigned &x, const unsigned &y);
	inline unsigned	GetDepthIndexAt(const unsigned &x, const unsigned &y) { return this->idxPtr[x][y]; };
	
	
	
private:
	///< Private class methods
	
	
	///< Private class properties
	Vector2 	resolution;		///< buffer resolution x * y
	unsigned	depth;			///< buffer depth (sd)
	TSample		***samples;		///< 3D array of samples (x * y * sd)
	unsigned	**idxPtr;		///< 2D array (x * y) of pointers to sample array
	
}; // class SampleBuffer



/**
 * Get sample at n-th depth position if position is specified.
 * @param unsigned x X-axis
 * @param unsigned y Y-axis
 * @param int i Depth buffer index, if -1 the current sample has been taken.
 * @return TSample Return reference of desired sample.
 */ 
inline TSample *SampleBuffer::GetSampleAt(const unsigned &x, const unsigned &y, const unsigned &i)
{
	// If depth index is negative, return current sample
	if(i < 0) {
		return &this->samples[x][y][this->idxPtr[x][y]];
	}
	
	return &this->samples[x][y][i];
} // GetSampleAt()



/**
 * Get next sample at specified position from current depth.
 * @param unsigned x X-axis
 * @param unsigned y Y-axis
 * @return TSample Return reference of desired sample.
 */ 
inline TSample *SampleBuffer::GetNextSampleAt(const unsigned &x, const unsigned &y)
{
	this->idxPtr[x][y] = (this->idxPtr[x][y] + 1) % this->depth;
	return &this->samples[x][y][this->idxPtr[x][y]];
} // GetNextSampleAt()



#endif // #ifndef _AFR_SAMPLE_BUFFER_H



/** ***************************************************************************/
/** end of file SampleBuffer.h											  	  */
/** ***************************************************************************/

