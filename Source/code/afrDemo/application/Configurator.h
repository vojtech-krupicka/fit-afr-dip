/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Configurator.h													  *	
 * @date	06. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Configurator.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_CONFIGURATOR_H
#define _AFR_CONFIGURATOR_H



// Including system libraries
#include <string>



// Including project libraries
#include "Sampler.h"
#include "../exceptions/Exceptions.h"
#include "../utils/MathUtils.h"
#include "../types/Array.h"



// Predefinition of classes
class Camera;
class Scene;
class Raytracer;
class Application;
class Sampler;



/**
 * @brief Structure for store informations about capture frame, unique ID,
 * output file name mask, etc.
 */  
typedef struct s_cfi {
	string 			mask;		///< file name mask
	string 			name;		///< file name without extension
	string 			ext;		///< file extension
	unsigned		id;			///< unique ID
	Array<string>	parts;		///< string of file name parts
} TCaptureFrameInfo;




/**
 * Class Configurator
 * @brief Class for store and handle all setings from command line or from
 * configuration file. Represents actual context (system container). 
 */ 
class Configurator
{
public:
	///< Implicit constructors
	Configurator();
	Configurator(const string &fn);
	
	///< Implicit destructors
	~Configurator();
	
	
	///< Configurator operations
	Configurator *Init();
	Configurator *Init(const string &fn);
	Configurator *Setup();
	
	
	///< Service getters (lazy loading)
	Application	*GetApplicationService() { return this->application; };
	Sampler		*GetSamplerService() { return this->sampler; };
	Scene		*GetSceneService() { return this->scene; };
	Raytracer	*GetTracerService() { return this->tracer; };
	
	
	///< Getters
	inline unsigned		GetRedrawMode() const { return this->redrawMode; };
	inline float		GetRedrawFps() const { return this->redrawFps; };
	inline float		GetRedrawTimeStep() const { return this->redrawTimeStep; };
	inline unsigned		GetRedrawFixedPixels() const { return this->redrawFixed; };
	inline unsigned		GetRedrawMinPixels() const { return this->redrawMinPixels; };
	inline bool			IsDrawFPS() const { return this->drawFPS; };
	inline bool			IsDrawAnimTime() const { return this->drawAnimTime; };
	inline bool			IsPredrawScreen() const { return this->predrawScreen; };
	inline string		GetBackgroundImage() const { return this->backgroundImage; };
	inline bool			HasBackgroundImage() const { return this->useBackgroundImage; };
	
	inline bool			IsCaptureFrame() const { return this->captureFrame; };
	inline unsigned		GetCaptureNthFrame() const { return this->captureNthFrame; };
	string				GetNextFrameName(const unsigned &bufferType = Sampler::BUFFER_NORMAL);
	
	inline bool			IsPlaying() const { return this->playback; };
	inline unsigned	 	GetSampleCacheSize() { return this->sampleCacheSize; };
	inline unsigned 	GetHorizontalTiles() const { return this->hTiles; };
	inline unsigned 	GetVerticalTiles() const { return this->vTiles; };
	inline bool 		UseGuidingSampling() { return this->sampler->IsGuiding(); };
	inline bool 		UseReconstruction() const { return this->useReconstruction; };
	inline float	 	GetSigmaFactor() const { return this->sigmaFactor; };
	inline unsigned 	GetAgingFactor() const { return this->agingFactor; };
	
	inline string 		GetSceneFile() const { return this->sceneFile; };
	inline bool 		UseKdTree() { return this->GetBooleanValue(this->useKdTree); };
	inline unsigned		GetKdTreeDepth() const { return this->kdTreeDepth; };
	inline unsigned 	GetKdTreeMinPrim() const { return this->kdTreeMinPrim; };
	inline unsigned		kGetKdTreeMinDepth() const { return this->kdTreeMinDepth; };
	
	inline unsigned 	GetTracerMsaa() const { return this->tracerMsaa; };
	inline bool 		UseTracerMsaa() const { return this->useTracerMsaa; };
	inline unsigned 	GetTracerDepth() const { return this->tracerDepth; };
	
	
	///< Setters
	bool Set(const string &key, const string &value);
	
	inline Configurator	*SetRedrawMode(const string &m);
	inline Configurator	*SetRedrawMode(const unsigned &m);
	inline Configurator *SetRedrawFps(const float &fps);
	inline Configurator *SetRedrawTimeStep(const float &ts) { this->redrawTimeStep = Abs(ts); return this; };
	inline Configurator	*SetRedrawFixedPixels(const unsigned &fp) { this->redrawFixed = Max((const unsigned)1, fp); return this; };
	inline Configurator *SetRedrawMinPixels(const unsigned &mp) { this->redrawMinPixels = Max((const unsigned)1, mp); return this; };
	inline Configurator	*SetDrawFPS(const bool &flag) { this->drawFPS = flag; return this; };
	inline bool			ToggleDrawFPS() { this->drawFPS = !this->drawFPS; this->drawFPS; };
	inline Configurator	*SetDrawAnimTime(const bool &flag) { this->drawAnimTime = flag; return this; };
	inline bool			ToggleDrawAnimTime() { this->drawAnimTime = !this->drawAnimTime; this->drawAnimTime; };
	inline Configurator	*SetPredrawScreen(const bool &flag) { this->predrawScreen = flag; return this; };
	inline Configurator	*SetBackgroundImage(const string &bg) { this->backgroundImage = bg; return this; };
	inline Configurator	*SetUseBackgroundImage(const bool &flag) { this->useBackgroundImage = flag; return this; };
	
	inline Configurator *SetCaptureFrame(const bool &flag) { this->captureFrame = flag; return this; };
	inline Configurator *SetCaptureNthFrame(const unsigned &n) { this->captureNthFrame = n; return this; };
	inline Configurator *SetCaptureFrameMask(const string &mask);
	
	inline Configurator *SetPlayback(const bool &flag = true) { this->playback = flag; return this; };
	inline bool			TogglePlayback() { this->playback = !this->playback; return this->playback; };
	inline Configurator *SetSampleCacheSize(const unsigned &n) { this->sampleCacheSize = n; return this; };
	inline Configurator	*SetSamplingMethod(const string &m) { this->sampler->SetSamplingMethod(m); return this; };
	inline Configurator	*SetHorizontalTiles(const unsigned &t) { this->hTiles = Max((const unsigned)1, t); return this; };
	inline Configurator	*SetVerticalTiles(const unsigned &t) { this->vTiles = Max((const unsigned)1, t); return this; };
	inline Configurator	*SetUseGuidingSampling(const bool &flag) { this->useGuiding = flag; this->sampler->SetGuiding(flag); return this; };
	inline bool			ToggleGuidingSampling() { this->useGuiding = !this->useGuiding; this->sampler->SetGuiding(this->useGuiding); return this->useGuiding; };
	inline Configurator	*SetUseReconstruction(const bool &flag) { this->useReconstruction = flag; this->sampler->SetReconstruction(flag); return this; };
	inline bool			ToggleReconstruction() { this->useReconstruction = !this->useReconstruction; this->sampler->SetReconstruction(this->useReconstruction); return this->useReconstruction; };
	inline Configurator	*SetSigmaFactor(const float &f) { this->sigmaFactor = f; this->sampler->SetSigmaFactor(f); return this; };
	inline Configurator	*SetAgingFactor(const unsigned &f) { this->agingFactor = f; this->sampler->SetAgingFactor(f); return this; };
	
	inline Configurator *SetSceneFile(const string &fn) { this->sceneFile = fn; return this; };
	inline Configurator *SetUseKdTree(const string &use) { this->useKdTree = use; return this; };
	inline Configurator *SetKdTreeDepth(const unsigned &d) { this->kdTreeDepth = Max((const unsigned)2, d); return this; };
	inline Configurator *SetKdTreeMinPrim(const unsigned &p) { this->kdTreeMinPrim = Max((const unsigned)1, p); return this; };
	inline Configurator *SetKdTreeMinDepth(const unsigned &d) { this->kdTreeMinDepth = Max((const unsigned)2, d); return this; };
	
	inline Configurator *SetTracerMsaa(const unsigned &aa) { this->tracerMsaa = Max((const unsigned)1, aa); return this; };
	inline Configurator *SetUseTracerMsaa(const bool &flag) { this->useTracerMsaa = flag; return this; };
	inline Configurator *SetTracerDepth(const unsigned &d) { this->tracerDepth = Max((const unsigned)1, d); return this; };
	
	
	///< Define enumerator for store modes
	enum {
		MODE_AUTO,
		MODE_FPS,
		MODE_FIXED,
		MODE_MAX
	};
	

private:
	///< Private application operations
	inline bool	GetBooleanValue(const string &value);
	
	inline Application 	*CreateApplicationService();
	inline Sampler		*CreateSamplerService();
	inline Scene		*CreateSceneService();
	inline Raytracer	*CreateTracerService();
	
	///< Service properties
	Application	*application;
	Sampler		*sampler;
	Scene		*scene;
	Raytracer	*tracer;
	
	///< Private properties
	unsigned	redrawMode;
	float		redrawFps;
	float		redrawTimeStep;
	unsigned	redrawFixed;
	unsigned	redrawMinPixels;
	bool		drawFPS;
	bool		drawAnimTime;
	bool		predrawScreen;
	string 		backgroundImage;
	bool		useBackgroundImage;
	
	bool				captureFrame;
	unsigned			captureNthFrame;
	TCaptureFrameInfo	*cfInfo;		
	
	bool		playback;
	
	string 		samplingMethod;
	unsigned	sampleCacheSize;
	unsigned	sampleDepth;
	unsigned	hTiles;
	unsigned	vTiles;
	bool		useGuiding;
	bool 		useReconstruction;
	float		sigmaFactor;
	unsigned	agingFactor;
	
	string 		sceneFile;
	string		useKdTree;
	unsigned	kdTreeDepth;
	unsigned	kdTreeMinPrim;
	unsigned	kdTreeMinDepth;
	
	unsigned	tracerMsaa;
	bool		useTracerMsaa;
	unsigned	tracerDepth;
	
}; // class Configurator



/**
 * Sets redraw mode. This determine when actual computation step ends and 
 * control will be convey to GLController.
 * @param string m String name of mode.
 * @return Provides fluent interface.
 */ 
inline Configurator *Configurator::SetRedrawMode(const string &m)
{
	if(m == "fps") {
		this->redrawMode = Configurator::MODE_FPS;
	}
	else if(m == "fixed") {
		this->redrawMode = Configurator::MODE_FIXED;
	}
	else {
		this->redrawMode = Configurator::MODE_AUTO;
	}
	
	return this;
} // SetRedrawMode()



/**
 * Sets redraw mode. This determine when actual computation step ends and 
 * control will be convey to GLController.
 * @param unsigned m ID of mode.
 * @return Provides fluent interface.
 */ 
inline Configurator *Configurator::SetRedrawMode(const unsigned &m)
{
	if(m < MODE_MAX) {
		this->redrawMode = m;
	}
	else {
		this->redrawMode = Configurator::MODE_AUTO;
	}
	
	return this;
} // SetRedrawMode()



/**
 * Sets frequency of redraw screen. Parameter is frames per second (FPS), result
 * is time span between two frames in miliseconds. 
 * @param float fps Frames per second
 * @return Provides fluent interface.
 */ 
inline Configurator *Configurator::SetRedrawFps(const float &fps)
{
	this->redrawFps = fps;
	this->redrawTimeStep = (1.0f / fps) * 1000.0f;	
	return this;
} // SetRedrawFps()



/**
 * Determine if value is true or false.
 * @param string value String value from configuration file.
 * @return Returns true, if value is 'true', 'yes' or '1', false otherwise.
 */ 
inline bool	Configurator::GetBooleanValue(const string &value)
{
	if(value == "true" || value == "yes" || value == "on" || value == "1") {
		return true;
	}
	
	return false;
} // GetBooleanValue()



/**
 * Sets new file name mask for capture frame.
 */ 
inline Configurator *Configurator::SetCaptureFrameMask(const string &mask)
{
	// If capture frame info struct exists, delete it
	if(this->cfInfo != NULL) {
		delete this->cfInfo;
		this->cfInfo = NULL;
	}
	
	// Creates new capture frame info struct
	this->cfInfo = new TCaptureFrameInfo;	
	this->cfInfo->mask = mask;
	this->cfInfo->name = mask;
	this->cfInfo->id = 1;
	this->cfInfo->ext = "ppm"; // default extension is PPM
	this->cfInfo->parts.Clear();
	
	// Find and store extension
	size_t pos = mask.find_last_of(".");
	if(pos != string::npos) {
		this->cfInfo->name = mask.substr(0, pos);
		this->cfInfo->ext = mask.substr(pos+1);		
	}
	
	string name = this->cfInfo->name;
	while((pos = name.find("%")) != string::npos) {
		string before = name.substr(0, pos);
		string after = name.substr(pos+1);
		
		if(before != "") {
			this->cfInfo->parts.Add(before);
		}
		
		// If not the end of string
		string ctrlChar = "";
		if(after != "") {
			ctrlChar = "%" + after.substr(0, 1);
			this->cfInfo->parts.Add(ctrlChar);
			after = after.substr(1);
		}
	
		name = after;
	}
	
	return this;
} // SetCaptureFrameMask()



#endif // #ifndef _AFR_CONFIGURATOR_H



/** ***************************************************************************/
/** end of file Configurator.h												  */
/** ***************************************************************************/
