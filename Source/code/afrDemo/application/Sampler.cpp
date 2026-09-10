/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06	 									  *
 * @file    Sampler.cpp														  *	
 * @date	10. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Sampler.								  *
 **************************************************************************** */



// Including project libraries
#include "Sampler.h"
#include "TileManager.h"
#include "../scene/Scene.h"
#include "../raytracer/Raytracer.h"
#include "../types/Image.h"
#include "../types/SampleBuffer.h"
#include "../utils/MathUtils.h"
#include "../utils/SamplingUtils.h"
#include "../utils/TimeUtils.h"



// Initialize sobel's kernels and binomial filter kernel for calculate derivatives
int Sampler::sobel3X[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
int Sampler::sobel3Y[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int Sampler::sobel3T[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int Sampler::blurFilter[3] = {1, 2, 1};



/**
 * Default constructor.
 * @return Sampler.
 */ 
Sampler::Sampler()
{
	this->Init();
} // Sampler()



/**
 * Constructor with sampling method name. Initialize sampler to default state.
 * @param string m Sampler method name as string.
 * @return Sampler.
 */ 
Sampler::Sampler(Raytracer *t, Image *bg, const bool &predraw, const unsigned &h, const unsigned &v, const unsigned &sd) throw(InvalidArgumentException)
{
	// Assert tracer
	if(t == NULL) {
		throw InvalidArgumentException("<Sampler>: Raytracer or image buffer is not specified!");
	}
	
	// Init sampler to default state
	this->Init();
	
	
	// Sets underlying raytracer
	this->tracer = t;
	this->resolution = this->tracer->GetResolution();
	
	
	// Sets number horizontal and vertical tiles and create tile manager
	this->hTilesCnt = h;
	this->vTilesCnt = v;	
	this->tileManager = new TileManager(this->resolution, sd, this->hTilesCnt, this->vTilesCnt);
	this->pixelsPerTile = this->tileManager->GetPixelsPerTile();
	
	// Creates one tile over whole image plane
	this->tile = new Tile(0.01f, (this->resolution.Mult() * sd), Vector2(0, 0), Vector2::Zero(), Vector2::One());
	
	
	
	// Init samples buffers
	this->depth = sd;
	this->sampleBuffer = new SampleBuffer(this->resolution.x(), this->resolution.y(), this->depth);
	this->temporalBuffer = new SampleBuffer(this->resolution.x(), this->resolution.y(), 2);
	
	// Init offset buffer
	unsigned index = 0;
	this->offsetBuffer = new Vector2[this->depth];
	CreateHalton(this->offsetBuffer, index, this->depth);
	
	
	// Init temporal buffer and derivatives buffer
	this->derivativesImage = new Image((int)this->resolution.x(), (int)this->resolution.y());
	this->temporalImage = new Image((int)this->resolution.x(), (int)this->resolution.y());
	
	// Init image buffer
	this->imageBuffer = new Image(this->resolution.x(), this->resolution.y(), this->tracer->GetScene()->GetBackgroundColor());
	if(predraw) {
		this->tracer->Run(this->imageBuffer);
	}
	else if(bg != NULL) {
		delete this->imageBuffer;
		this->imageBuffer = bg->Copy();
	}
	
	
	// Set samples in temporal buffer with color values
	for(int x = 0; x < this->resolution.x(); x++) {
		for(int y = 0; y < this->resolution.y(); y++) {
			for(int d = 0; d < 2; d++) {
				TSample *smpl = this->temporalBuffer->GetSampleAt(x, y, d);
				smpl->color = this->imageBuffer->GetPixel(x, y);
				smpl->intensity = smpl->color.Intensity();
			}
		}
	}
	
	
	// Init pixels weights and pixels update time arrays
	unsigned now = Now();
	unsigned size = (int)this->resolution.Mult();
	this->ageWeightSum = new float[size];
	this->lastSampleTime = new float[size];
	
	for(unsigned i = 0; i < size; i++) {
		this->ageWeightSum[i] = 1.0f;
		this->lastSampleTime[i] = now;
	}
} // Sampler()



/**
 * Default desctructor. Destroy sampler.
 */ 
Sampler::~Sampler() 
{	
	// Set raytracer to NULL
	this->tracer = NULL;
	
	// Delete tile manager
	if(this->tileManager != NULL) {
		delete this->tileManager;
		this->tileManager = NULL;
	}
	
	// Delete tile
	if(this->tile != NULL) {
		delete this->tile;
		this->tile = NULL;
	}
	
	
	// Delete samples buffers
	if(this->sampleBuffer != NULL) {
		delete this->sampleBuffer;
		this->sampleBuffer = NULL;
	}
	
	if(this->temporalBuffer != NULL) {
		delete this->temporalBuffer;
		this->temporalBuffer = NULL;
	}
	
	if(this->offsetBuffer != NULL) {
		delete this->offsetBuffer;
		this->offsetBuffer = NULL;
	}
	
	
	
	// Delete image buffer
	if(this->imageBuffer != NULL) {
		delete this->imageBuffer;
		this->imageBuffer = NULL;
	}	
	
	// Delete derivatives buffer
	if(this->derivativesImage != NULL) {
		delete this->derivativesImage;
		this->derivativesImage = NULL;
	}
	
	// Delete temporal buffer
	if(this->temporalImage != NULL) {
		delete this->temporalImage;
		this->temporalImage = NULL;
	}
	
	
	// Delete weight sum array
	if(this->ageWeightSum != NULL) {
		delete [] this->ageWeightSum;
		this->ageWeightSum = NULL;
	}
	
	// Delete array of samples time
	if(this->lastSampleTime != NULL) {
		delete [] this->lastSampleTime;
		this->lastSampleTime = NULL;
	}
} // ~Sampler()



/** ***************************************************************************/
/** Sampler operations												          */
/** ***************************************************************************/



/**
 * Initialize sampler's properties into default state.
 * @return Sampler.
 */ 
Sampler *Sampler::Init()
{
	// Set guiding informations
	this->guiding = false;
	this->reconstruct = false;
	this->recReady = false;
	this->hTilesCnt = 1;
	this->vTilesCnt = 1;
	this->sigmaFactor = 3.0f;
	this->agingFactor = 100;
	
	this->pixelsPerTile = 0;
	this->tileManager = NULL;	
	this->tile = NULL;
	this->tracer = NULL;
	
	this->depth = 0;
	this->sampleBuffer = NULL;
	this->temporalBuffer = NULL;
	
	this->imageBuffer = NULL;
	
	this->derivativesImage = NULL;
	this->temporalImage = NULL;
	
	this->ageWeightSum = NULL;
	this->lastSampleTime = NULL;
	
	return this;
} // Init()



/**
 * Method for creates new 2D vector as next sample. For new sample position
 * will be used sampling method which has been set in config file.
 * @return bool Returns true, if new sample has been successfully computed.
 */ 
bool Sampler::GetNextSample()
{
	// If guiding sampling is activated
	if(this->guiding) {
		// Get tile 
		Tile *currTile = this->tileManager->GetTileWithinRange();
		if(currTile == NULL) { 
			currTile = this->tile;
		}
		
		
		// Get next sample position in pixels in tile of interest
		Vector2 smplPos = currTile->GetNextSample() * this->resolution; 
		unsigned x = (unsigned)smplPos.x();
		unsigned y = (unsigned)smplPos.y();
		
		
		// First sample always at top left corner of pixel - for spatial and temporal derivative computation
		TSample *temporalSample = this->temporalBuffer->GetNextSampleAt(x, y);
		temporalSample->iPos = Vector2(x, y);
		temporalSample->fPos = temporalSample->iPos + this->offsetBuffer[0];
		temporalSample->color = this->tracer->RunOnce(temporalSample->fPos.x(), temporalSample->fPos.y());
		temporalSample->intensity = temporalSample->color.Intensity();
		temporalSample->age = Now();
		
		
		// Second sample at next offset coordinates - for actual image
		TSample *imageSample = this->sampleBuffer->GetNextSampleAt(x, y);
		
		// If offset coordinates are same as temporalSample, both samples will have the same color
		if(this->sampleBuffer->GetDepthIndexAt(x, y) == 0) {
			*(imageSample) = *(temporalSample);
		}
		else {
			// Calculate new sample color with its offset
			imageSample->iPos = Vector2(x, y);
			imageSample->fPos = imageSample->iPos + this->offsetBuffer[this->sampleBuffer->GetDepthIndexAt(x, y)];
			imageSample->color = this->tracer->RunOnce(imageSample->fPos.x(), imageSample->fPos.y());
			imageSample->age = temporalSample->age; // sample age should be the same
		}
		
		
		// RECONSTUCTION 
		if(this->reconstruct) {		
			// Calculate derivatives and set values into auxiliary color buffers
			Vector3 derv = this->CalculateDerivatives(x, y);
			this->derivativesImage->SetPixel(x, y, Color(derv.x(), derv.y(), derv.z()));
			this->temporalImage->SetPixel(x, y, Color(0.0f, derv.z(), 0.0f));
			
			if(derv.z() > 0.0001f) {
				// Filter extents and filter volume			
				float ex = 1.0f - (derv.x());
				float ey = 1.0f - (derv.y());
				float et = 1.0f - (derv.z());
				float eSum = ex + ey + et;
				
				// Calculate filter volume as V_s = 1 / R(U_i)
				float Vs = this->sigmaFactor * this->pixelsPerTile / (float)(currTile->GetValidSamplesNum() + 1);
				float ratio = Vs / eSum;
				ex *= ratio; ey *= ratio; et *= ratio;
				ex = Min(this->tileManager->GetTileSizeInPixels().x(), ex); ey = Min(this->tileManager->GetTileSizeInPixels().y(), ey);
				
				float scaleFactor = this->agingFactor * et;
				float gaussFactor = 9.0f / (2.0f * M_PI * ex * ey);
				float exSqr = ex * ex;
				float eySqr = ey * ey;
				
				int eex = (int)(ex);
				int eey = (int)(ey);
				
				// Add sample to tile and reacumulate variances for actual tile
				currTile->AddSample(temporalSample, scaleFactor);
				this->tileManager->UpdateVariances(currTile);
				
				// Cover spatial filter extents in x ace
				for(int dx = -eex; dx <= eex; dx++) {
					int cx = x + dx;
					if(cx < 0 || cx >= (int)this->resolution.x()) continue;
					
					float tx = Abs(this->offsetBuffer[this->sampleBuffer->GetDepthIndexAt(x, y)].x() - dx - 0.5f);
					tx = (tx*tx) / exSqr;
					
					// And in y axis
					for(int dy = -eey; dy <= eey; dy++) {
						int cy = y + dy;
						if(cy < 0 || cy >= (int)this->resolution.y()) continue;
						
						float ty= Abs(this->offsetBuffer[this->sampleBuffer->GetDepthIndexAt(x, y)].y() - dy - 0.5f);
						ty = (ty*ty) / eySqr;
						
						// Calculate spatial filter weight and draw sample into canvas
						float spatialWeight = Max(0.0001f, gaussFactor * expf(-4.5f * (tx + ty)));
						this->DrawSample(cx, cy, imageSample, scaleFactor / 5.0f, spatialWeight);
					}
				}
			}
			else {
				this->DrawSample(x, y, imageSample, 1500.0f, 1.0f);
				
				// Add sample to tile and reacumulate variances for actual tile
				currTile->AddSample(temporalSample, 500.0f);
				this->tileManager->UpdateVariances(currTile);
			}
		}
		
		// Guiding sampling without reconstruction
		else {
			currTile->AddSample(temporalSample, this->agingFactor);
			this->tileManager->UpdateVariances(currTile);
			this->imageBuffer->SetPixel(x, y, temporalSample->color);
		}
	}
	else {
		// Get next sample position in pixels in tile of interest
		Vector2 smplPos = this->tile->GetNextSample() * this->resolution; 
		unsigned x = smplPos.x();
		unsigned y = smplPos.y();
		
		
		Color clr = this->tracer->RunOnce(x, y);
		this->imageBuffer->SetPixel(x, y, clr);
	}
	
	return true;
} // GetNextSample()



/**
 * Main method for draw new sample into image plane and to output canvas.
 * @param unsigned x X position on image plane,
 * @param unsigned y Y position on image plane,
 * @param TSample smpl Actual sample which will be drawn,
 * @param float scale Scale factor for this sample,
 * @param float spatialWeight Spatial weight of this sample.
 * @return Color Returns new reweighted color for actual pixel on image plane.
 */ 
Color Sampler::DrawSample(const unsigned &x, const unsigned &y, const TSample* const smpl, const float &scale, const float &spatialWeight)
{
	// Prepare variables
	unsigned index = x+y*this->resolution.x();
	Color clr = this->imageBuffer->GetPixel(x, y);
	float weight = expf(((int)this->lastSampleTime[index] - (int)smpl->age) / scale) * this->ageWeightSum[index];
	float nextWeight = weight + spatialWeight;
	
	// Update pixel color
	clr.r(((spatialWeight*smpl->color.r()) + (weight*clr.r())) / nextWeight);
	clr.g(((spatialWeight*smpl->color.g()) + (weight*clr.g())) / nextWeight);
	clr.b(((spatialWeight*smpl->color.b()) + (weight*clr.b())) / nextWeight);
	
	// Update weights
	this->ageWeightSum[index] = nextWeight;
	this->lastSampleTime[index] = smpl->age;
	
	// Set new color
	this->imageBuffer->SetPixel(x, y, clr);
	
	return clr;
} // DrawSample()



/**
 * Method for calculate three derivatives approximating by sobel operator.
 * @param unsigned x X pixel's position on image plane,
 * @param unsigned y Y pixel's position on image plane.
 * @return Vector3 Returns all three derivatives as components of 3D vector.
 */ 
Vector3 Sampler::CalculateDerivatives(const unsigned &x, const unsigned &y)
{	
	float dervX = 0.0f;
	float dervY = 0.0f;
	float dervT[] = {0.0f, 0.0f, 0.0f};
	int cx, cy, ptrIdx;
	float clr;
	
	for(int dx = -1; dx <= 1; dx++) {
		for(int dy = -1; dy <= 1; dy++) {
			cx = x + dx;
			cy = y + dy;
			
			cx = (cx < 0) ? 0 : (cx > (int)this->resolution.x()-1) ? this->resolution.x()-1 : cx;
			cy = (cy < 0) ? 0 : (cy > (int)this->resolution.y()-1) ? this->resolution.y()-1 : cy;
			
			ptrIdx = this->temporalBuffer->GetDepthIndexAt(x, y);
			clr = this->temporalBuffer->GetSampleAt(cx, cy, ptrIdx)->intensity;
			dervX += (float) Sampler::sobel3X[dx+1][dy+1] * clr;
			dervY += (float) Sampler::sobel3Y[dx+1][dy+1] * clr;
			
			for(int dt = -1; dt <= 1; dt++) {
				ptrIdx = (dt >= 0) ? (int)this->temporalBuffer->GetDepthIndexAt(cx, cy) : (int)this->temporalBuffer->GetDepthIndexAt(cx, cy) + dt;
				ptrIdx = (ptrIdx < 0) ? (2 + ptrIdx) : ptrIdx;
				
				clr = this->temporalBuffer->GetSampleAt(cx, cy, ptrIdx)->intensity;
				dervT[dx+1] += (float) Sampler::blurFilter[dx+1] * (float) Sampler::sobel3T[dy+1][dt+1] * clr;
			}
		}
	}
	
	float ddX = Abs(0.25f * dervX);
	float ddY = Abs(0.25f * dervY);
	float ddT = (Abs(dervT[0]) + Abs(dervT[1]) + Abs(dervT[2])) * 0.25f * 0.25f;
	
	// Creates new vector with derivatives
	return Vector3(ddX, ddY, ddT);
} // CalculateDerivatives()



/**
 * Method for capture actual content of one of image buffers.
 * @param string fn New captured image name,
 * @param unsigned type Which buffer will be captured.
 * @return Sampler Provides fluent interface.
 */ 
Sampler *Sampler::CaptureFrame(const string &fn, const unsigned &type)
{
	switch(type) {
		case Sampler::BUFFER_TILES: 
			this->tileManager->GetTilesBuffer()->Save(fn);
			break;
		case Sampler::BUFFER_DERIVATIVES: 
			this->derivativesImage->Save(fn);
			break;
		case Sampler::BUFFER_TEMPORAL: 
			this->temporalImage->Save(fn);
			break;
		default: 
			this->imageBuffer->Save(fn);
			break;
	}
	
	return this;
} // CaptureFrame()



/** ***************************************************************************/
/** Sampler setters	and getters										          */
/** ***************************************************************************/



/**
 * Shortcut for setting sampling method as a string.
 * @param string m Method name.
 * @return Provides fluent interface.
 */ 
Sampler *Sampler::SetSamplingMethod(const string &m) 
{ 
	if(this->tileManager && this->tile) {
		this->tile->SetSamplingMethod(m);
		this->tileManager->SetSamplingMethod(m); 
	}
	
	return this;
} // SetSamplingMethod()



/**
 * Shortcut for setting sampling method with its ID.
 * @param unsigned m Method id.
 * @return Provides fluent interface.
 */ 
Sampler *Sampler::SetSamplingMethod(const unsigned &m) 
{
	if(this->tileManager && this->tile) {
		this->tile->SetSamplingMethod(m);
		this->tileManager->SetSamplingMethod(m); 
	}
	
	return this;
} // SetSamplingMethod()



/**
 * Shortcut for setting sampling cache size for storing precompute samples.
 * @param unsigned s New cache size.
 * @return Provides fluent interface.
 */ 
Sampler *Sampler::SetSampleCacheSize(const unsigned &s) 
{ 
	if(this->tileManager && this->tile) {
		this->tile->SetSampleCacheSize(s);
		this->tileManager->SetSampleCacheSize(s); 
	}
	
	return this;
} // SetSampleCacheSize()



/**
 * Shortcut for generating and getting tiles buffer from tiles manager.
 * @return Image Returns tiles buffer as image.
 */ 
Image *Sampler::GetTilesBuffer()
{
	// Check if tiles buffer exists
	if(this->tileManager != NULL) {
		return this->tileManager->GetTilesBuffer();
	}
	
	return NULL;
} // GetTilesBuffer()



/**
 * Method for update sample rate of all tiles in tile manager.
 * @param unsigned timeSpan Time span for evaluate which samples are valid,
 * @param unsigned now Current animation time.
 * @return Sampler Provides fluent interface.
 */ 
Sampler *Sampler::UpdateSamplingRate(const unsigned &timeSpan, const unsigned &now) 
{ 
	if(this->tileManager && this->tile) {
		this->tile->UpdateSamplingRate(timeSpan, now);
		this->tileManager->UpdateSamplingRate(timeSpan, now); 
	}
	
	return this;
} // UpdateSamplingRate()



/** ***************************************************************************/
/** end of file Sampler.cpp													  */
/** ***************************************************************************/
