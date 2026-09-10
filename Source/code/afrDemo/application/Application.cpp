/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Application.cpp													  *	
 * @date	14. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Application.							  *
 **************************************************************************** */


// Including project libraries
#include "Application.h"

#include "Configurator.h"
#include "GLController.h"

#include "../scene/Scene.h"
#include "../scene/Camera.h"
#include "../scene/AnimationManager.h"

#include "../raytracer/KdTree.h"
#include "../raytracer/Raytracer.h"

#include "../types/Image.h"

#include "../utils/MathUtils.h"
#include "../utils/StringUtils.h"
#include "../utils/TimeUtils.h"



/**
 * Default constructor.
 * @return Application.
 */ 
Application::Application()
{
	// Sets class properties
	this->scene = NULL;
	this->sampler = NULL;
} // Application()



/**
 * Constructor with config file name. Initialize application to default state.
 * @param Configurator c System container object.
 * @return Application.
 */ 
Application::Application(Configurator *c)
{
	// Sets configurator
	this->context = c;	
	
	// Check if context existst
	if(this->context == NULL) {
		throw InvalidStateException("Application: context has not been instantiated yet!");
	}
	
	
	// Sets class properties
	this->scene = this->context->GetSceneService();
	this->sampler = this->context->GetSamplerService();
	
	// Init application
	this->Init();
} // Application()



/**
 * Default desctructor. Destroy application.
 */ 
Application::~Application() 
{

} // ~Application()



/**
 * Method for initialize application. This method requires context object, which
 * must be instantiated before!
 * @throw InvalidStateException Throw when context has not been instantiated yet.
 * @return Application Provides fluent interface.
 */ 
Application *Application::Init()
{
	// Check if context existst
	if(!this->context || !this->scene || !this->sampler) {
		throw InvalidStateException("Application: context has not been instantiated yet!");
	}
	
	
	// Set misc properties
	this->appStartTime = Now();
	this->animStartTime = 0;
	this->runStartTime = 0;
	
	this->hasAnimations = this->scene->HasAnimations();
	this->resolution = this->scene->GetCamera()->GetResolution();
	
	// If mode is animation, set time step
	if(this->context->GetRedrawMode() == Configurator::MODE_AUTO) {
		if(this->hasAnimations) {
			// Set time step in miliseconds (*1000)
			this->context->SetRedrawTimeStep(this->scene->GetAnimationManager()->GetTimeStep() * 1000.0f);
		}
		else {
			this->context->SetRedrawMode(Configurator::MODE_FPS);
			this->context->SetRedrawFps(this->context->GetRedrawFps());
		}
	}
	
	
	return this;
} // Init()



/**
 * Main method which run application and calculates new pixels with frameless
 * rendering method.
 * @return int Returns count of compute pixels, or -1 if an error has been detected.
 */ 
int Application::Run()
{
	int samplesCnt = 0;
	unsigned minPixels = this->context->GetRedrawMinPixels();
	unsigned timeSpan = (unsigned)(this->scene->GetAnimationManager()->GetTimeStep() * 1000.0f * 2.0f);
	
	this->ClearRunStartTime();
	this->sampler->UpdateSamplingRate(timeSpan, Now());

	
	// Start updating samples
	do {
		for(unsigned i = 0; i < minPixels; i++) {
			if(this->sampler->GetNextSample()) {
				samplesCnt++;
			}
		}
	} while(this->CheckRunningState(samplesCnt));

	
	return samplesCnt;
} // Run()



/**
 * Auxiliary method, which determine if curent run should be stoped or not.
 * @param int samples Number of samples currently actualized,
 * @return bool Returns true, if run shouldn't be stop, false otherwise.
 */ 
bool Application::CheckRunningState(const int &samples)
{
	// Try to go to the next animation frame
	this->NextFrame();
	
	// If mode is FIXED samples count
	if(this->context->GetRedrawMode() == Configurator::MODE_FIXED) {
		// Check, if all need samples has been computed
		if(samples >= (int)this->context->GetRedrawFixedPixels()) {
			return false;
		}
	}
	else {
		// Check, if refresh rate has been exceeded
		if(this->GetRunTime() >= this->context->GetRedrawTimeStep()) {
			return false;
		}
	}
	
	return true;	
} // CheckRunningState()



/**
 * Method for synchronize refresh rate with animation and if it necessary, 
 * goto next animation frame.
 * @return bool Returns true, if animation go to next frame, false otherwise.
 */ 
bool Application::NextFrame()
{
	// If animation has been stopped
	if(!this->context->IsPlaying()) {
		return false;
	}
	
	// First, check, if animation time has been exceeded
	if(this->hasAnimations && this->GetAnimTime() >= this->scene->GetAnimationManager()->GetTimeStep() * 1000.0f) {
		if(this->context->IsCaptureFrame()) {
			if(this->context->GetCaptureNthFrame() == -1 || this->context->GetCaptureNthFrame() == this->scene->GetAnimationManager()->GetActualFrame()) {
				if(this->scene->GetAnimationManager()->IsEnded()) {
					this->context->SetCaptureFrame(false);
				}
				else {
					string fn = this->CaptureFrame(true, Sampler::BUFFER_NORMAL);
					cout << "Frame '" << fn << "' has been captured!" << endl;
					
					if(this->context->UseGuidingSampling()) {
						fn = this->CaptureFrame(true, Sampler::BUFFER_TILES);
						cout << "Frame '" << fn << "' has been captured!" << endl;
						
						if(this->context->UseReconstruction()) {
							fn = this->CaptureFrame(true, Sampler::BUFFER_DERIVATIVES);
							cout << "Frame '" << fn << "' has been captured!" << endl;
							
							fn = this->CaptureFrame(true, Sampler::BUFFER_TEMPORAL);
							cout << "Frame '" << fn << "' has been captured!" << endl;
						}
					}
				}
			}
		}

		this->scene->Next();
		this->ClearAnimStartTime();
		return true;
	}
	
	return false;
} // NextFrame()



/**
 * Method for capture actual frame buffer into output image file.
 * @param bool need Flag if this capture is really needed or not.
 * @return Application Provides fluent interface.
 */ 
string Application::CaptureFrame(const bool &need, const unsigned &type)
{
	if(!need && !this->sampler && (!this->context->IsCaptureFrame() || !this->context->IsPlaying())) {
		return string();
	}
	
	string fn = this->context->GetNextFrameName(type);
	this->sampler->CaptureFrame(fn, type);
	
	return fn;
} // CaptureFrame()



/** ***************************************************************************/
/** end of file Application.cpp												  */
/** ***************************************************************************/
