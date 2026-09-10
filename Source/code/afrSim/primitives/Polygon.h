/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Polygon.h														  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Polygon.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_POLYGON_H
#define _AFR_POLYGON_H


// Including project libraries
#include "Primitive.h"
#include "../exceptions/Exceptions.h"


// Predefinition of classes
class BBox;



/**
 * Class Polygon
 * @brief Implementation of polygon primitive.
 */ 
class Polygon : public Primitive
{
public:
	///< Implicit constructors
	Polygon();
	Polygon(Material *m, Texture *t);
	Polygon(Material *m, Texture *t, const Vector3 *v, const Vector3 *n, const unsigned &vc);
	
	///< Implicit destructors
	virtual ~Polygon();
	
	///< Vector operations
	virtual Primitive *Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull = true);
	virtual bool GetHitInfo(const Ray &r, THitInfo &info);
	
	virtual BBox *GetBBox();
	virtual bool IntersectBBox(const BBox &box) const;
	
	virtual Primitive *Transform(const Matrix &m);
	virtual void Render() const;
	
protected:
	Vector3 	*verts;
	Vector3		*normals;
	unsigned	count;
}; // class Polygon



#endif // #ifndef _AFR_POLYGON_H



/** ***************************************************************************/
/** end of file Polygon.h													  */
/** ***************************************************************************/
