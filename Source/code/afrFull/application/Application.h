/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Application.h													  *	
 * @date	06. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Application.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_APPLICATION_H
#define _AFR_APPLICATION_H



// Including system libraries
#include <string>



// Including project libraries
#include "Sampler.h"
#include "../exceptions/Exceptions.h"
#include "../types/Vector2.h"
#include "../utils/TimeUtils.h"


// Predefinition of classes
class GLController;
class Configurator;
class Image;
class Sampler;
class Scene;
class Raytracer;
class Vector2;



/**
 * Class Application
 * @brief Main class for defining of whole application, which synchronize 
 * application run time with animation time and asks for new samples.
 */ 
class Application
{
public:
	///< Implicit constructors
	Application();
	Application(Configurator *c);
	
	///< Implicit destructors
	~Application();

	///< Application operations
	Application *Init();
	int 		Run();
	string		CaptureFrame(const bool &need = true);
	
	
	///< Getters
	inline Configurator	*GetContext() const { return this->context; };
	inline Sampler		*GetSampler() const { return this->sampler; };
	inline Scene		*GetScene() const { return this->scene; };
	inline Image		*GetFrameBuffer() const { return this->sampler->GetImageBuffer(); };
	
	inline unsigned		GetAppStartTime() const { return this->appStartTime; };
	inline unsigned		GetRunStartTime() const { return this->runStartTime; };
	inline unsigned		GetAnimStartTime() const { return this->animStartTime; };
	
	inline float		GetAppTime() const { return TimeSpan(this->appStartTime); };
	inline float		GetRunTime() const { return TimeSpan(this->runStartTime); };
	inline float		GetAnimTime() const { return TimeSpan(this->animStartTime); };
	
	
	///< Setters
	inline unsigned		ClearRunStartTime() { return (this->runStartTime = Now()); };
	inline unsigned		ClearAnimStartTime() { return (this->animStartTime = Now()); };
	
private:
	///< Private application operations
	bool CheckRunningState(const int &pixels);
	bool NextFrame();
	
	///< Class properties
	Configurator 	*context;
	Sampler			*sampler;
	Scene			*scene;
	
	Vector2			resolution;
	
	unsigned		appStartTime;
	unsigned		runStartTime;
	unsigned		animStartTime;
	
	bool 			hasAnimations;
}; // class Application



#endif // #ifndef _AFR_APPLICATION_H



/** ***************************************************************************/
/** end of file Application.h												  */
/** ***************************************************************************/
