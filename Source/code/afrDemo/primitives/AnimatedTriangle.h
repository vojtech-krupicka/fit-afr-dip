/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    AnimatedTriangle.h												  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class AnimatedTriangle.							  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_ANIMATED_TRIANGLE_H
#define _AFR_ANIMATED_TRIANGLE_H



// Including project libraries
#include "Primitive.h"
#include "../types/Array.h"



// Predefinition of classes
class Triangle;
class BBox;



/**
 * Class AnimatedTriangle
 * @brief Implementation of Animated triangle primitive.
 */ 
class AnimatedTriangle : public Primitive
{
public:
	///< Implicit constructors
	AnimatedTriangle();
	AnimatedTriangle(Material *m, Texture *t);
	AnimatedTriangle(Material *m, Texture *t, const unsigned &c, float *f, Vector3 *v, Vector3 *n);
	
	///< Implicit destructors
	virtual ~AnimatedTriangle();
	
	///< Vector operations
	virtual Primitive *Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull = true);
	virtual bool GetHitInfo(const Ray &r, THitInfo &info);
	
	virtual BBox *GetBBox();
	virtual bool IntersectBBox(const BBox &box) const;
	
	virtual Primitive *Transform(const Matrix &m);
	virtual void Render() const;
	
	///< Getters
	inline Triangle			*GetTriangle() const { return this->triangle; }; 
	
	///< Special animated triangle operations
	AnimatedTriangle 		*AddTriangle(const float&, Triangle *);
	AnimatedTriangle 		*AddTriangle(const float&, const Vector3 v[3], const Vector3 *n);
	bool			 		Lerp(const float&);
	
	inline static Vector3 	Lerp(const float &p, const Vector3 &v0, const Vector3 &v1) { return ((v1 - v0) * p + v0); };
	
protected:	
	unsigned			frames;
	Triangle			*triangle;
	Array<Triangle*>	triangles;
	float				*times;
	
}; // class AnimatedTriangle



/**
 * Definition for new type as list of animated triangle primitives.
 */ 
typedef Array<AnimatedTriangle *> TAnimatedTriangleList;



#endif // #ifndef _AFR_ANIMATED_TRIANGLE_H



/** ***************************************************************************/
/** end of file AnimatedTriangle.h											  */
/** ***************************************************************************/
