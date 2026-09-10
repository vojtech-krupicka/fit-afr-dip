/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Raytracer.h														  *	
 * @date	24. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Raytracer.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_RAYTRACER_H
#define _AFR_RAYTRACER_H


// Including system libraries
#include <string>



// Including project libraries
#include "../primitives/Light.h"
#include "../primitives/Primitive.h"
#include "../types/Color.h"



// Define some macros and constants
#define MC_GRID_SIZE 4



// Used namespaces
using namespace std;



// Predefinition of classes
class Scene;
class KdTree;
class Camera;
class Vector2;
class Image;



/**
 * Class Raytracer
 * @brief Implementation of own raytracer for determine new pixel color or 
 * for computation whole image at actual scene state.
 */ 
class Raytracer
{
public:
	///< Default constructors
	Raytracer();
	Raytracer(Scene *s, KdTree *t = NULL);

	///< Default destructors
	~Raytracer();	
	
	
	///< Raytracer main operations
	bool 		Run(Image *(&image));
	Color 		RunOnce(const float &x, const float &y);
	
	
	///< Getters
	inline 	Scene 		*GetScene() const { return this->scene; };
	inline 	KdTree 		*GetKdTree() const { return this->tree; };
	inline	bool		UseKdTree() const { return this->useTree; };
	
	inline 	Color 		GetBackgroundColor() const { return this->bgColor; };
	inline  unsigned	GetImagePlaneWidth() const { return this->width; };
	inline  unsigned	GetImagePlaneHeight() const { return this->height; };
	inline 	Vector2		GetResolution() const { return Vector2(this->width, this->height); };
	inline 	float		GetIndexOfRefr() const { return this->ior; };
	inline	unsigned	GetMaxDepth() const { return this->depth; };
	inline  unsigned	GetMsaa() const { return this->msaa; };
	inline 	bool		UseMsaa() const { return this->useMsaa; };
	
	
	///< Setters	
	inline Raytracer	*SetScene(Scene *s) { this->scene = s; return this; };
	inline Raytracer	*SetKdTree(KdTree *t = NULL) { this->tree = t; this->useTree = (this->tree == NULL) ? false : this->useTree; return this; };
	inline Raytracer	*SetUseKdTree(const bool &flag = false) { this->useTree = flag; return this; };
	
	inline Raytracer	*SetBackgroundColor(const Color &c) { this->bgColor = c; return this; };
	inline Raytracer	*SetIndexOfRefr(const float &ior) { this->ior = ior; return this; };
	inline Raytracer	*SetMaxDepth(const unsigned &d) { this->depth = d; return this; };
	inline Raytracer	*SetMsaa(const unsigned &s) { this->msaa = s; this->subSamples = s*s; this->useMsaa = (this->subSamples > 1) ? this->useMsaa : false; return this; };
	inline Raytracer	*SetUseMsaa(const bool &flag = false) { this->useMsaa = flag; return this; };
	

private:
	Raytracer 	*Init();
	Primitive  	*Trace(const Ray &ray, Color &color, const float &ior = 1.0f, const unsigned &level = 0, const bool &cull = true);
	Primitive	*Hit(const Ray &ray, Primitive *p, float &tmax, const bool &cull = true);
	
	Color		Multisample(const Vector3 &pos);
	
	Color		CalcShading(const Ray &ray, THitInfo &info);
	Color 		CalcReflection(const Ray &ray, THitInfo &info, const unsigned &level = 0);
	Color		CalcRefraction(const Ray &ray, THitInfo &info, const float &ior = 1.0f, const unsigned &level = 0);
	bool		CalcShadow(TLight *light, THitInfo &info);
	
	Scene		*scene;			///< instance of scene object
	KdTree		*tree;			///< instance of kd tree
	bool		useTree;		///< use kd tree for find nearest primitive, or not
	
	Camera 		*camera;		///< instance of camera object
	unsigned	width;			///< image plane width
	unsigned	height;			///< image plane height
	
	Color		bgColor;		///< background color
	float 		ior;			///< global index of refraction
	unsigned	depth;			///< max level of nesting
	unsigned	msaa; 			///< multisampling index - number of subsamples = msaa*msaa
	unsigned	subSamples;		///< number of subsamples = msaa*msaa
	bool		useMsaa;		///< use multisampling or not
	
	///< 4x4 grid for monte carlo calculations (multisampling, soft shadows, diffuse)
	Vector2		mcGrid[MC_GRID_SIZE][MC_GRID_SIZE];	
}; // class Raytracer



#endif // #ifndef _AFR_RAYTRACER_H



/** ***************************************************************************/
/** end of file Raytracer.h													  */
/** ***************************************************************************/
