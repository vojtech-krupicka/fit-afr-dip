/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Tile.cpp														  *	
 * @date	15. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Tile.									  *
 **************************************************************************** */



// Including project libraries
#include "Tile.h"

#include "../utils/SamplingUtils.h"
#include "../utils/TimeUtils.h"
#include "../application/Sampler.h"



/**
 * Default constructor.
 * @return Tile.
 */ 
Tile::Tile()
{
	this->Init();
} // Tile()



/**
 * Constructor with number of tiles on horizontal and vertical direction.
 * @param float v Initial tile's variance,
 * @param Vector2 p Tile's position,
 * @param Vector2 o Tile's offset,
 * @param Vector2 s Tile's size.
 * @return Tile.
 */ 
Tile::Tile(const float &v, const unsigned &m, const Vector2 &p, const Vector2 &o, const Vector2 &s)
{
	// Initialize tile
	this->Init();
	
	// Sets position, offset and size and initial variance
	this->variance = (v <= TILE_STARV_TRESH) ? TILE_STARV_TRESH : v;
	this->maxRecentSamples = m;
	this->position = p;
	this->offset = o;
	this->size = s;
} // TileManager()



/**
 * Default desctructor.
 */ 
Tile::~Tile() 
{	
	// Delete sample cache
	if(this->sampleCache != NULL) {
		delete [] this->sampleCache;
		this->sampleCache = NULL;
	}
	
	
	// Init to default
	this->Init();
} // ~Tile() 


/** ***************************************************************************/
/** Tile manager operations												      */
/** ***************************************************************************/



/**
 * Initialize tile's properties into default state.
 * @return Tile.
 */ 
Tile *Tile::Init()
{	
	// Set sampling method
	this->method = Sampler::METHOD_RANDOM;
	
	// Set sampling iterators
	this->sampleCache = NULL;
	this->nextSampleIdx = 0;
	this->nextHaltonIdx = 0;
	this->sampleCacheSize = 64;
	this->cachedSamplesNum = 1;

	// Set tile properties
	this->totalSamplesNum = 0;
	this->lastSampleTime = Now();
	this->ageWeightSum = 1.0f;
	this->variance = TILE_STARV_TRESH;
	this->expectedVal = 0.0f;
	this->expectedValSqr = 0.0f;
	
	// Set tile resolution
	this->position = Vector2::Zero();
	this->offset = Vector2::Zero();
	this->size = Vector2::One();
	
	this->validSamplesNum = 0;
	this->lastValidSampleIdx = 0;
	this->maxRecentSamples = 0;
	this->recentSamplesTime.Clear();
	
	// Create sample cache
	this->sampleCache = NULL;
	this->CreateSampleCache(this->sampleCacheSize);
	CreateRandom(this->sampleCache, this->sampleCacheSize);
	
	return this;
} // Init()



/**
 * Creates new cache to store N new samples.
 * @param unsigned size Number of samples which will be cached.
 * @return Tile.
 */ 
Tile *Tile::CreateSampleCache(const unsigned &size)
{
	// If cache is already existst, destroy it
	if(this->sampleCache != NULL) {
		delete [] this->sampleCache;
		this->sampleCache = NULL;
	}
	
	// Create new cache for store N samples
	this->sampleCache = new Vector2[size];
	
	return this;
} // CreateSampleCache()



/**
 * Method for get next sample.
 * @return TSample Returns new sample.
 */ 
Vector2 Tile::GetNextSample()
{
	// Return new sample absolute position (i.e. position at screen in range <0,1>)
	return (this->GetSamplePosition() * this->size) + this->offset;
} // GetNextSample()



/**
 * Method for add new sample informations to tile. Compute values for 
 * guinding sampling process.
 * @param Sample smpl New sample wich will be added into tile,
 * @param float scale Aging scale factor.
 * @return Tile Provides fluent interface.
 */ 
Tile *Tile::AddSample(const TSample* const smpl, const float &scale, const float &sw, const float &st)
{
	// Auxiliary variables
	float factor = smpl->color.Intensity(); 
	float weight = expf((this->lastSampleTime - smpl->age) / scale) * this->ageWeightSum;
	float nextWeight = weight + 1.0f;
	
	// Calculate expected values i' and i'^{2}
	this->expectedVal = ((weight * this->expectedVal) + factor) / nextWeight;
	this->expectedValSqr = ((weight * this->expectedValSqr) + (factor * factor)) / nextWeight;
	
	// Update tile's variance (steiners theorem and starvation treshold
	this->variance = this->expectedValSqr - (this->expectedVal * this->expectedVal);
	this->variance = (this->variance <= st) ? st : this->variance;
	
	// Update ages
	this->ageWeightSum = nextWeight;
	this->lastSampleTime = smpl->age;
	
	// Update counters
	this->totalSamplesNum++;
	this->validSamplesNum++;
	this->recentSamplesTime.Add(smpl->age);
	
	return this;
} // AddSample()



/**
 * Method for update sampling rate for actual tile. Sampling rate R(U_i) 
 * represents the number of samples per pixel per a chosen interval in time.
 * @param unsigned timeSpan Time span for evaluate which samples are valid,
 * @param unsigned now Current animation time.
 * @return Tile Provides fluent interface.
 */ 
Tile *Tile::UpdateSamplingRate(const unsigned &timeSpan, const unsigned &now)
{
	unsigned i = this->lastValidSampleIdx;
	while(i < this->recentSamplesTime.Size() && this->validSamplesNum > 0 && (now - this->recentSamplesTime[i]) > timeSpan) {
		this->validSamplesNum--;
		i++;
	}
	
	this->lastValidSampleIdx = i;
	
	return this;
} // UpdateSamplingRate()



/**
 * Gets new sample from cache according actualy selected sampling method.
 * @return Vector2 Return new sample.
 */ 
Vector2 Tile::GetSamplePosition()
{
	// According to selected sampling method
	switch(this->method) {
		// Halton sequence
		case Sampler::METHOD_HALTON:
			if(this->nextSampleIdx >= this->cachedSamplesNum) {
				// Create new cache for store samples
				this->CreateSampleCache(this->sampleCacheSize);
				
				// Generate N new samples and reset iterator
				this->cachedSamplesNum = CreateHalton(this->sampleCache, this->nextHaltonIdx, this->sampleCacheSize);
				this->nextSampleIdx = 0;
			}
			
			return this->sampleCache[this->nextSampleIdx++];
			break;
			
		// JITTERED (or stratified) sampling method
		case Sampler::METHOD_JITTER:
			if(this->nextSampleIdx >= this->cachedSamplesNum) {
				// Create new cache for store samples
				this->CreateSampleCache(this->sampleCacheSize);
				
				// Generate N new samples, apply filters and reset iterator
				this->cachedSamplesNum = CreateJitter(this->sampleCache, this->sampleCacheSize);
				this->nextSampleIdx = 0;
			}
			
			return this->sampleCache[this->nextSampleIdx++];
			break;
			
		// MULTI JITTERED (combined with N Rooks) sampling method
		case Sampler::METHOD_MULTIJITTER:
			if(this->nextSampleIdx >= this->cachedSamplesNum) {
				// Create new cache for store samples
				this->CreateSampleCache(this->sampleCacheSize);
				
				// Generate N new samples, apply filters and reset iterator
				this->cachedSamplesNum = CreateMultiJitter(this->sampleCache, this->sampleCacheSize);
				this->nextSampleIdx = 0;
			}
			
			return this->sampleCache[this->nextSampleIdx++];
			break;
		
		// POISSON DISCS sampling method
		case Sampler::METHOD_POISSON:
			if(this->nextSampleIdx >= this->cachedSamplesNum) {
				// Create new cache for store samples
				this->CreateSampleCache(this->sampleCacheSize);
				
				// Generate N new samples, apply filters and reset iterator
				this->cachedSamplesNum = CreatePoisson(this->sampleCache, this->sampleCacheSize);
				this->nextSampleIdx = 0;
			}
			
			return this->sampleCache[this->nextSampleIdx++];
			break;
			
		// N ROOKS sampling method
		case Sampler::METHOD_ROOKS:
			if(this->nextSampleIdx >= this->cachedSamplesNum) {
				// Create new cache for store samples
				this->CreateSampleCache(this->sampleCacheSize);
				
				// Generate N new samples, apply filters and reset iterator
				this->cachedSamplesNum = CreateRooks(this->sampleCache, this->sampleCacheSize);
				this->nextSampleIdx = 0;
			}
			
			return this->sampleCache[this->nextSampleIdx++];
			break;			
		
		// Default method is RANDOM
		case Sampler::METHOD_RANDOM:		
		default :
			if(this->nextSampleIdx >= this->cachedSamplesNum) {
				// Create new cache for store samples
				this->CreateSampleCache(this->sampleCacheSize);
				
				// Generate N new samples, apply filters and reset iterator
				this->cachedSamplesNum = CreateRandom(this->sampleCache, this->sampleCacheSize);
				this->nextSampleIdx = 0;
			}
			
			return this->sampleCache[this->nextSampleIdx++];
			break;
	}
	
	
	return Vector2(0.5f, 0.5f);
} // GetSamplePosition()



/** ***************************************************************************/
/** end of file Tile.cpp													  */
/** ***************************************************************************/
