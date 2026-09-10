/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06	 									  *
 * @file    Sampler.h														  *	
 * @date	10. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Sampler.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_SAMPLER_H
#define _AFR_SAMPLER_H



// Including system libraries
#include <string>



// Including project libraries
#include "../exceptions/Exceptions.h"
#include "../utils/MathUtils.h"
#include "../utils/StringUtils.h"
#include "../types/Array.h"
#include "../types/Vector2.h"
#include "../types/Sample.h"



// Predefinition of classes
class Image;
class Vector2;
class Tile;
class TileManager;
class Raytracer;
class SampleBuffer;



/**
 * Class Sampler
 * @brief Main class for Adaptive Frameless Renderer. Handle actualization of
 * samples, choosing its position and handle guiding sampling and reconstruction.
 */ 
class Sampler
{
public:
	///< Implicit constructors
	Sampler();
	Sampler(Raytracer *t, Image *bg, const bool &predraw, const unsigned &h, const unsigned &v, const unsigned &sd) throw(InvalidArgumentException);
	
	///< Implicit destructors
	~Sampler();
	
	
	///< Sampler operations
	bool	GetNextSample();
	
	Image	*GetTilesBuffer();
	Image	*GetImageBuffer() const { return this->imageBuffer; };
	Image 	*GetDerivativesBuffer() const { return this->derivativesImage; };
	Image	*GetTemporalBuffer() const { return this->temporalImage; };
	
	Sampler	*CaptureFrame(const string &fn, const unsigned &type = Sampler::BUFFER_NORMAL);
	Sampler	*UpdateSamplingRate(const unsigned &timeSpan, const unsigned &now);
	
	
	///< Getters
	inline Raytracer	*GetTracer() const { return this->tracer; };
	
	inline bool			IsGuiding() const { return this->guiding; };
	inline bool			IsReconstruct() const { return this->reconstruct; };
	
	
	///< Setters
	Sampler				*SetSamplingMethod(const string &m);
	Sampler				*SetSamplingMethod(const unsigned &m);
	Sampler				*SetSampleCacheSize(const unsigned &s);
	
	inline Sampler		*SetGuiding(const bool &flag = true) { this->guiding = flag; return this; };
	inline Sampler		*SetReconstruction(const bool &flag = true) { this->reconstruct = flag; return this; };
	inline Sampler		*SetRecReady(const bool &flag = true) { this->recReady = flag; return this; };
	inline Sampler		*SetHTilesCount(const unsigned &c) { this->hTilesCnt = c; return this; };
	inline Sampler		*SetVTilesCount(const unsigned &c) { this->vTilesCnt = c; return this; };
	inline Sampler		*SetSigmaFactor(const float &f) { this->sigmaFactor = f; return this; };
	inline Sampler		*SetAgingFactor(const unsigned &f) { this->agingFactor = f; return this; };
	
	
	///< Define enumerator for store sampling methods
	enum {
		METHOD_RANDOM,
		METHOD_HALTON,
		METHOD_JITTER,
		METHOD_MULTIJITTER,
		METHOD_POISSON,
		METHOD_ROOKS,
		METHOD_MAX
	};
	
	
	///< Define enumerator for identify buffer types
	enum {
		BUFFER_NORMAL,
		BUFFER_TILES,
		BUFFER_DERIVATIVES,
		BUFFER_TEMPORAL,
		BUFFER_MAX
	};
	

private:
	///< Private application operations
	Sampler 		*Init();		
	
	Vector3			CalculateDerivatives(const unsigned &x, const unsigned &y);
	Color 			DrawSample(const unsigned &x, const unsigned &y, const TSample* const smpl, const float &scale, const float &spatialWeight);
	
	
	///< Sobels kernels and binomial filter for compute spatial and temporal derivatives
	static int		sobel3X[3][3];
	static int		sobel3Y[3][3];
	static int		sobel3T[3][3];
	static int		blurFilter[3];
	
	
	///< Private class properties
	unsigned		hTilesCnt;			///< number of horizontal tiles
	unsigned		vTilesCnt;			///< number of vertical tiles
	bool			guiding; 			///< flag if guiding sampling and subdividing image plane is active
	bool			reconstruct; 		///< flag if image reconstruction is available
	bool			recReady;
	float			sigmaFactor;		///< sigma factor for gaussian filter
	unsigned		agingFactor;		///< scale factor s for aging function lambda in guiding sampling and  resonstruction temporal filter function
	
	Tile			*tile;				///< if guided sampling is off, select from one tile
	TileManager  	*tileManager;		///< manager for handle all tiles for guiding sampling method
	float 			pixelsPerTile;		///< tile.width * tile.height, i.e. pixels covered by single tile
	
	Vector2			resolution;			///< full image plane size in pixel as 2D vector (w*h)
	Raytracer		*tracer;			///< underlying raytracer
	
	Image			*imageBuffer;		///< whole screen plane as image
	
	unsigned		depth;				///< sample's buffer depth sd
	SampleBuffer	*sampleBuffer;		///< main buffer for store all samples to depth sd
	SampleBuffer	*temporalBuffer;	///< auxiliary buffer for storing samples in left top corner for calculate derivatives
	Vector2			*offsetBuffer;		///< auxiliary buffer for storing sd offset from left top corner for supersampling
	
	Image			*derivativesImage;	///< image buffer for storing all derivatives, d_x as red, d_y as green and d_t as blue
	Image			*temporalImage;		///< image buffer for storing temporal derivative d_t as green
	
	float			*ageWeightSum;		///< float array N*M for storing spatial weights for all pixels
	float			*lastSampleTime;	///< unsigned array N*M for storing last sample for all pixels
}; // class Sampler







#endif // #ifndef _AFR_SAMPLER_H



/** ***************************************************************************/
/** end of file Sampler.h												  	  */
/** ***************************************************************************/
