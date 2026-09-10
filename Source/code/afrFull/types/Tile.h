/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Tile.h															  *	
 * @date	15. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Tile.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_TILE_H
#define _AFR_TILE_H



// Including system libraries
#include <string>



// Including project libraries
#include "Array.h"
#include "Color.h"
#include "Vector2.h"
#include "Sample.h"
#include "../application/Sampler.h"



// Define macros
#define TILE_STARV_TRESH 0.01f		///< tile's starvation treshold



// Predefinition of classes
class Vector2;



/**
 * Class Tile
 * @brief Class reprezentating of one tile in image for image tiling in 
 * guiding sampling in adaptive algorithm.
 */ 
class Tile
{
public:
	///< Implicit constructors
	Tile();
	Tile(const float &v, const unsigned &m, const Vector2 &p, const Vector2 &o, const Vector2 &s);
	
	~Tile();
	
	
	///< Sampler operations
	inline Color 	ToColor() const { return Color(this->variance, 0.0f, 0.0f); };
	Vector2			GetNextSample();
	Tile			*AddSample(const TSample* const smpl, const float &scale, const float &sw = 1.0f, const float &st = 0.01f);
	Tile			*UpdateSamplingRate(const unsigned &timeSpan, const unsigned &now);
	
	
	///< Getters
	inline float 	GetValidSamplesNum() { return (this->validSamplesNum > this->maxRecentSamples) ? this->maxRecentSamples : this->validSamplesNum; } ;
	inline unsigned	GetNextHaltonIndex() const { return this->nextHaltonIdx; };
	
	inline unsigned	GetTotalSamplesNum() const { return this->totalSamplesNum; };
	inline float	GetLastSampleTime() const { return this->lastSampleTime; };
	inline float	GetAgeWeightSum() const { return this->ageWeightSum; };
	inline float	GetVariance() const { return this->variance; };
	inline float	GetExpectedValue() const { return this->expectedVal; };
	inline float	GetExpectedSquaredValue() const { return this->expectedValSqr; };
	
	inline Vector2	GetPosition() const { return this->position; };
	inline Vector2	GetOffset() const { return this->offset; };
	inline Vector2	GetSize() const { return this->size; };
	
	
	///< Setters
	inline Tile*	SetPosition(const Vector2 &p) { this->position = p; return this; };
	inline Tile*	SetOffset(const Vector2 &o) { this->offset = o; return this; };
	inline Tile*	SetSize(const Vector2 &s) { this->size = s; return this; };
	
	
private:
	///< Private class methods
	Tile 		*Init();
	
	///< Private class properties
	unsigned	method;				///< actualy selected sampling method
	
	unsigned	nextHaltonIdx;		///< index of the next sample for halton sequence
	
	unsigned	totalSamplesNum;	///< total number of samples wich has been took from tile
	float		lastSampleTime;		///< time when the last sample has been added into a tile
	float 		ageWeightSum;		///< sum of all age weight of tile for all samples which has been taken
	float 		variance;			///< tile's variance
	float 		expectedVal;		///< expected value
	float		expectedValSqr;		///< square of expected value
	
	Vector2		position;			///< tile position on image plane in int (i.e. [2,1] denotes third row and second col)
	Vector2 	offset;				///< real offset of tile's left top corner of image plane in range <0, 1)
	Vector2		size;				///< real size of tile in range <0, 1> where 1 is size of image plane
	
	unsigned	validSamplesNum;	///< number of all actual valid samples in tile
	unsigned	lastValidSampleIdx;	///< index of last valid sample, e.g. sample time is less than [timeSpan] ms
	Array<unsigned> recentSamplesTime;	///< time of all recent samples
	unsigned	maxRecentSamples;	///< maximum number of recent sample stored in above array
}; // class Tile



/**
 * @brief Defines new type for list of tiles in the image plane as dynamic array of 
 * pointers at tile instance.
 */
typedef Array<Tile*> TTileList;



#endif // #ifndef _AFR_TILE_H



/** ***************************************************************************/
/** end of file Tile.h													  	  */
/** ***************************************************************************/
