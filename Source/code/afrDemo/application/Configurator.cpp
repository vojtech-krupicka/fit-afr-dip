/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Configurator.cpp												  *	
 * @date	06. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Configurator.							  *
 **************************************************************************** */


// Including system libraries
#include <fstream>
#include <iostream>


// Including project libraries
#include "Configurator.h"

#include "Application.h"
#include "Sampler.h"

#include "../scene/Scene.h"

#include "../raytracer/KdTree.h"
#include "../raytracer/Raytracer.h"

#include "../types/Image.h"

#include "../utils/MathUtils.h"
#include "../utils/StringUtils.h"
#include "../utils/TimeUtils.h"



/**
 * Default constructor.
 * @return Configurator.
 */ 
Configurator::Configurator()
{
	this->Init();
} // Configurator()



/**
 * Constructor with config file name. Initialize configurator to default state.
 * @param string fn Config file name.
 * @return Configurator.
 */ 
Configurator::Configurator(const string &fn)
{
	this->Init(fn);
} // Configurator()



/**
 * Default desctructor. Destroy configurator.
 */ 
Configurator::~Configurator() 
{
	// Destroy tracer
	if(this->tracer != NULL) {
		// Firstly, delete kd tree
		KdTree *tree = this->tracer->GetKdTree();
		if(tree != NULL) {
			delete tree;
			tree = NULL;
			
			this->tracer->SetKdTree(NULL);
		}
		
		delete this->tracer;
		this->tracer = NULL;
	}
	
	// Destroy scene
	if(this->scene != NULL) {
		delete this->scene;
		this->scene = NULL;
	}
	
	// Destroy sampler
	if(this->sampler != NULL) {
		delete this->sampler;
		this->sampler = NULL;
	}
	
	// Destroy application
	if(this->application != NULL) {
		delete this->application;
		this->application = NULL;
	}
	
	// Destroy info structure for capture frame
	if(this->cfInfo != NULL) {
		delete this->cfInfo;
		this->cfInfo = NULL;
	}
	
	// At the end, init to default state
	this->Init();
} // ~Configurator()



/** ***************************************************************************/
/** Configurator main operations											  */
/** ***************************************************************************/



/**
 * Init configurator to default state and values.
 * @return Configurator Provides fluent interface.
 */ 
Configurator *Configurator::Init()
{
	// Initialize services (not create)
	this->application = NULL;
	this->sampler = NULL;
	this->scene = NULL;
	this->tracer = NULL;
	
	this->redrawMode = Configurator::MODE_FPS; // default mode is fixed fps
	this->SetRedrawFps(60); // 1 / 60Hz = 0,0167ms
	this->redrawFixed = 1000; // 1000 per frame
	this->redrawMinPixels = 20; // min 20 pixels each loop
	this->drawFPS = true; // show FPS
	this->drawAnimTime = false; // show anim time
	this->predrawScreen = true; // prerender screen
	this->backgroundImage = ""; // no BG image
	this->useBackgroundImage = false; // no BG image used
	
	this->captureFrame = false;
	this->cfInfo = NULL;
	
	this->playback = true;
	this->sampleDepth = 4;
	this->hTiles = 1;
	this->vTiles = 1;
	this->useGuiding = false;
	this->useReconstruction = false; // use reconstruction
	this->sigmaFactor = 3;
	this->agingFactor = 100;
	
	this->sceneFile = ""; // no scene
	this->useKdTree = "smart"; // use kd tree, "smart", "true", "false"
	this->kdTreeDepth = 5;
	this->kdTreeMinPrim = 100;
	this->kdTreeMinDepth = 3;
	
	this->tracerMsaa = 1; // no msaa
	this->useTracerMsaa = false;
	this->tracerDepth = 3;
	
	return this;
} // Init()



/**
 * Init configurator from config file.
 * @param string fn Config file name.
 * @return Configurator Provides fluent interface.
 */ 
Configurator *Configurator::Init(const string &fn)
{
	// Init to default values
	this->Init();
	
	// Try open config file
	ifstream ifs;
	ifs.open(fn.c_str(), ios_base::in);
	
	// If file doesn't exists, throw exception
	if(!ifs.is_open()) {
		throw FileNotFoundException("", 0, fn);
	}
	
	// Read file
	string line;
	while(!ifs.eof()) {
		// Read line
		getline(ifs, line);
		
		// Explode line to key and value and set property
		string key, value;
		size_t pos = StrExplode(line, key, value);
		
		if(pos != string::npos) {
			this->Set(key, value);
		}
	}
	
	//exit(EXIT_SUCCESS);
	return this;
} // Init()	



/**
 * Main method which must be called after configurator's initiation. Creates
 * and configure all necessary services (application, scene, tracer and sampler).
 * @return Configurator Provides fluent interface.
 */ 
Configurator *Configurator::Setup() 
{
	// Creates scene
	this->scene = new Scene(this->GetSceneFile());
	
	
	// Creates kd tree (if necessary) and tracer
	KdTree *tree = NULL;
	
	// First, creates kd tree
	if(this->useKdTree == "smart" || this->GetBooleanValue(this->useKdTree)) {
		bool use = true;
		unsigned depth = this->kdTreeDepth;
		
		// If flag is "smart", it must be compute kd tree depth
		if(this->useKdTree == "smart") {
			// If number of primitives in scene is smaller than threshold, not create kd tree
			if(this->scene->GetPrimitiveCount() < this->kdTreeMinPrim) {
				use = false;
			}
			// Else compute depth as log2(prim_count)
			else {
				depth = log2(this->scene->GetPrimitiveCount());
				
				// If depth is smaller than threshold, not create kd tree
				if(depth < this->kdTreeMinDepth) {
					use = false;
				}
			}
		}
	
		
		// Create new tree
		if(use) {
			tree = new KdTree(this->scene, depth);
			this->useKdTree = "true";
		}
		else {
			this->useKdTree = "false";
		}
	}
	
	
	// Create the tracer and customize its properties
	this->tracer = new Raytracer(scene, tree);
	this->tracer->SetMaxDepth(this->tracerDepth)
				->SetUseKdTree(this->GetBooleanValue(this->useKdTree))
				->SetUseMsaa(this->useTracerMsaa)
				->SetMsaa(this->tracerMsaa);



	// Load background image if it sets
	Image *bg = NULL;
	if(!this->predrawScreen && this->useBackgroundImage) {
		bg = new Image(this->backgroundImage);
	}
	
	
	// Creates sampler
	this->sampler = new Sampler(this->tracer, bg, this->predrawScreen, this->hTiles, this->vTiles, this->sampleDepth);	
	this->sampler->SetSamplingMethod(this->samplingMethod)
				 ->SetSampleCacheSize(this->sampleCacheSize)
				 ->SetGuiding(this->useGuiding)
				 ->SetReconstruction(this->useReconstruction)
				 ->SetSigmaFactor(this->sigmaFactor)
				 ->SetAgingFactor(this->agingFactor);
				 
				 
	// Delete bg image
	if(bg != NULL) {
		delete bg;
		bg = NULL;
	}
				 	
	
	// Creates application
	this->application = new Application(this);
	
	return this;
} // Setup()



/**
 * Usefull method for set value to context by its key. 
 * @param string key Key name to determine which value will be set,
 * @param string value New property value.
 * @return bool Returns true, if property value exists and sets properly, false otherwise. 
 */ 
bool Configurator::Set(const string &key, const string &value)
{
	// Convert key to lower case and trim spaces from begin and end key and value
	string tkey = StrToLower(StrTrim(key));
	string tvalue = StrTrim(value);
	
	// Check if key and value are not empty
	if(tkey.empty() || tvalue.empty() || key[0] == ';' || key[0] == '#') {
		return false;
	}
	// Check if value is string (start and ends width "") 
	if(tvalue[0] == '"' && tvalue[tvalue.size()-1] == '"') {
		tvalue = tvalue.substr(1, tvalue.size()-2);
	}
	
	cout << "'" << tkey << "' = '" << tvalue << "'" << endl;
	
	// Set value to proper property
	if(tkey == "redraw_mode") {
		this->SetRedrawMode(tvalue);
	}
	else if(tkey == "redraw_fps") {
		this->SetRedrawFps(strtod(tvalue.c_str(), NULL));
	}
	else if(tkey == "redraw_fixed") {
		this->SetRedrawFixedPixels(atoi(tvalue.c_str()));
	}
	else if(tkey == "redraw_min_pixels") {
		this->SetRedrawMinPixels(atoi(tvalue.c_str()));
	}
	else if(tkey == "draw_fps") {
		this->SetDrawFPS(this->GetBooleanValue(tvalue));
	}
	else if(tkey == "draw_anim_time") {
		this->SetDrawAnimTime(this->GetBooleanValue(tvalue));
	}
	else if(tkey == "predraw_screen") {
		this->SetPredrawScreen(this->GetBooleanValue(tvalue));
	}
	else if(tkey == "bg_image") {
		this->SetBackgroundImage(tvalue);
	}
	else if(tkey == "use_bg_image") {
		this->SetUseBackgroundImage(this->GetBooleanValue(tvalue));
	}
	
	else if(tkey == "capture_frame") {
		this->SetCaptureFrame(this->GetBooleanValue(tvalue));
	}
	else if(tkey == "capture_nth_frame") {
		this->SetCaptureNthFrame(atoi(tvalue.c_str()));
	}
	else if(tkey == "frame_name_mask") {
		this->SetCaptureFrameMask(tvalue);
	}
	
	
	else if(tkey == "auto_start") {
		this->SetPlayback(this->GetBooleanValue(tvalue));
	}
	else if(tkey == "sampling_method") {
		this->samplingMethod = tvalue;
	}
	else if(tkey == "samples_cache_size") {
		this->sampleCacheSize = atoi(tvalue.c_str());
	}
	else if(tkey == "sample_depth") {
		this->sampleDepth = atoi(tvalue.c_str());
	}
	else if(tkey == "use_guiding_sampling") {
		this->useGuiding = this->GetBooleanValue(tvalue);
	}
	else if(tkey == "horizontal_tiles") {
		this->hTiles = atoi(tvalue.c_str());
	}
	else if(tkey == "vertical_tiles") {
		this->vTiles = atoi(tvalue.c_str());
	}
	else if(tkey == "use_recontruction") {
		this->useReconstruction = this->GetBooleanValue(tvalue);
	}
	else if(tkey == "sigma_factor") {
		this->sigmaFactor = strtod(tvalue.c_str(), NULL);
	}
	else if(tkey == "aging_factor") {
		this->agingFactor = atoi(tvalue.c_str());
	}
	
	
	else if(tkey == "scene_file") {
		this->SetSceneFile(tvalue);
	}
	else if(tkey == "use_scene_kdtree") {
		this->SetUseKdTree(tvalue);
	}
	else if(tkey == "scene_kdtree_depth") {
		this->SetKdTreeDepth(atoi(tvalue.c_str()));
	}
	else if(tkey == "scene_kdtree_min_primitives") {
		this->SetKdTreeMinPrim(atoi(tvalue.c_str()));
	}
	else if(tkey == "scene_kdtree_min_depth") {
		this->SetKdTreeMinDepth(atoi(tvalue.c_str()));
	}
	
	else if(tkey == "tracer_msaa") {
		this->SetTracerMsaa(atoi(tvalue.c_str()));
	}
	else if(tkey == "use_tracer_msaa") {
		this->SetUseTracerMsaa(this->GetBooleanValue(tvalue));
	}
	else if(tkey == "tracer_depth") {
		this->SetTracerDepth(atoi(tvalue.c_str()));
	}
	else {
		return false;
	}
	
	return true;
} // Set()



/**
 * Create new output file name from mask for actual captured frame.
 * @param string bufferType Type of selected buffer.
 * @return string Returns new output file name for captured frame.
 */ 
string Configurator::GetNextFrameName(const unsigned &bufferType)
{
	// Test if cf info is exists
	if(this->cfInfo == NULL) {
		this->SetCaptureFrameMask("afr_%G_%N.ppm");
	}
	
	// Prepare variables
	string name = "";
	unsigned size = this->cfInfo->parts.Size();
	
	// Go throught name parts and compile final file name
	for(unsigned i = 0; i < size; i++) {
		string part = this->cfInfo->parts[i];
		
		// Escaped % char
		if(part == "%%") {
			name += "%";
		}
		// Unique number of actual image
		else if(part == "%N") {
			char id[11];
			sprintf(id, "%05d", this->cfInfo->id);
			name += id;
		}
		// Animation actual time
		else if(part == "%T") {
			float time = Now();
			if(this->GetSceneService()->HasAnimations()) {
				time = this->GetSceneService()->GetAnimationManager()->GetActualTime();
			}
			
			char tt[11];
			sprintf(tt, "%6.4f", time);
			name += tt;
		}
		// Animation actual frame
		else if(part == "%F") {
			if(this->GetSceneService()->HasAnimations()) {
				char frame[11];
				sprintf(frame, "%05d", this->GetSceneService()->GetAnimationManager()->GetActualFrame());
				name += frame;
			}
			// Else skip
		}
		// Global time, time when application has been started
		else if(part == "%G") {
			unsigned time = this->GetApplicationService()->GetAppStartTime();
			name += Dump(time);
		}
		// Actual application time, e.g. actual time - time, when app started
		else if(part == "%A") {
			unsigned time = this->GetApplicationService()->GetAppTime();
			name += Dump(time);
		}
		// Sampling method name
		else if(part == "%M") {
			name += this->samplingMethod;
		}
		// Buffer type
		else if(part == "%B") {
			switch(bufferType) {
				case Sampler::BUFFER_TILES: 
					name += "tiles";
					break;
				case Sampler::BUFFER_DERIVATIVES: 
					name += "deriv";
					break;
				case Sampler::BUFFER_TEMPORAL: 
					name += "temp";
					break;
				default: 
					name += "normal";
					break;
			}
		}
		else {
			name += part;
		}
	}
	
	name += "." + this->cfInfo->ext;
	this->cfInfo->id++;
	
	
	return name;
} // GetNextFrameName()



/** ***************************************************************************/
/** end of file Configurator.cpp											  */
/** ***************************************************************************/
