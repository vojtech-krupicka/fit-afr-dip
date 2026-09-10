/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Sphere.h														  *	
 * @date	11. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Sphere.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_SPHERE_H
#define _AFR_SPHERE_H


// Including project libraries
#include "Primitive.h"


// Predefinition of classes
class BBox;



/**
 * Class Sphere
 * @brief Implementation of sphere primitive.
 */ 
class Sphere : public Primitive
{
public:
	///< Implicit constructors
	Sphere();
	Sphere(Material *m, Texture *t);
	Sphere(Material *m, Texture *t, const Vector3 &c, const float &r, const bool &s);
	Sphere(const Sphere &s);
	
	///< Implicit destructors
	virtual ~Sphere();
	
	///< Vector operations
	virtual Primitive *Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull = true);
	virtual bool GetHitInfo(const Ray &r, THitInfo &info);
	
	virtual BBox *GetBBox();
	virtual bool IntersectBBox(const BBox &box) const;
	
	virtual Primitive *Transform(const Matrix &m);
	virtual void Render() const;
	
protected:
	Sphere 		*origin;
	Matrix	 	matrix;
	
	Vector3	 	center;
	float		radius, sqrRadius, rRadius;
	Vector3		vEast, vNorth, vUp;
}; // class Sphere



#endif // #ifndef _AFR_SPHERE_H



/** ***************************************************************************/
/** end of file Sphere.h													  */
/** ***************************************************************************/
