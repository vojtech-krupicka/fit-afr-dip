/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Cone.h															  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Cone.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_CONE_H
#define _AFR_CONE_H


// Including project libraries
#include "Primitive.h"


// Predefinition of classes
class BBox;



/**
 * Class Cone
 * @brief Implementation of cone primitive.
 */ 
class Cone : public Primitive
{
public:
	///< Implicit constructors
	Cone();
	Cone(Material *m, Texture *t);
	Cone(Material *m, Texture *t, const Vector3 &b, const float &br, const Vector3 &a, const float &ar);
	
	///< Implicit destructors
	virtual ~Cone() {};
	
	///< Vector operations
	virtual Primitive *Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull = true);
	virtual bool GetHitInfo(const Ray &r, THitInfo &info);
	
	virtual BBox *GetBBox();
	virtual bool IntersectBBox(const BBox &box) const;
	
	virtual Primitive *Transform(const Matrix &m);
	virtual void Render() const;
	
protected:
	Vector3 base;
	Vector3 apex;
	
	float	baseRadius;
	float 	apexRadius;
}; // class Cone



#endif // #ifndef _AFR_CONE_H



/** ***************************************************************************/
/** end of file Cone.h														  */
/** ***************************************************************************/
