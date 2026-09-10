/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Cylinder.h														  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Cylinder.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_CYLINDER_H
#define _AFR_CYLINDER_H


// Including project libraries
#include "Primitive.h"


// Predefinition of classes
class BBox;



/**
 * Class Cylinder
 * @brief Implementation of cylinder primitive.
 */ 
class Cylinder : public Primitive
{
public:
	///< Implicit constructors
	Cylinder();
	Cylinder(Material *m, Texture *t);
	Cylinder(Material *m, Texture *t, const Vector3 &b, const Vector3 &a, const float &r);
	
	///< Implicit destructors
	virtual ~Cylinder() {};
	
	///< Vector operations
	virtual Primitive *Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull = true);
	virtual bool GetHitInfo(const Ray &r, THitInfo &info);
	
	virtual BBox *GetBBox();
	virtual bool IntersectBBox(const BBox &box) const;
	
	virtual Primitive *Transform(const Matrix &m);
	virtual void Render() const;
	
protected:
	bool HitCaps(const Ray &r, const Vector3 &center, const float &tmin, float &tmax) const;

	Vector3 base;
	Vector3 apex;
	float	radius;
}; // class Cylinder



#endif // #ifndef _AFR_CYLINDER_H



/** ***************************************************************************/
/** end of file Cylinder.h													  */
/** ***************************************************************************/
