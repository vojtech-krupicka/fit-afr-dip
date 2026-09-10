/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    BBox.h															  *	
 * @date	01. 12. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class BBox.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_BBOX_H
#define _AFR_BBOX_H


// Including project libraries
#include "Primitive.h"


// Predefinition of classes



/**
 * Class BBox
 * @brief Implementation of Axis Aligned Bounding Box (AABB) primitive.
 */ 
class BBox : public Primitive
{
public:
	///< Implicit constructors
	BBox();
	BBox(const Vector3 &pMin, const Vector3 &pMax);
	BBox(const BBox &b);
	
	///< Implicit destructors
	virtual ~BBox() {};
	
	///< Operators
	inline BBox &operator=(const BBox &b) { min = b.min; max = b.max; size = b.size; return *this; };
	
	///< Getters
	inline Vector3 GetMin() const { return min; };
	inline Vector3 GetMax() const { return max; };
	inline Vector3 GetSize() const { return size; };
	
	///< Setters
	inline BBox SetMin(const Vector3 &p) { min = p; size = max - min; return *this; };
	inline BBox SetMax(const Vector3 &p) { max = p; size = max - min; return *this; };
	
	///< Vector operations
	virtual Primitive *Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull = true);
	virtual bool GetHitInfo(const Ray &r, THitInfo &info);
	
	virtual BBox *GetBBox() { return this; };
	virtual bool IntersectBBox(const BBox &box) const;
	
	virtual Primitive *Transform(const Matrix &m) { return this; };
	virtual void Render() const {};
	
protected:
	Vector3 min;
	Vector3 max;
	Vector3	size;
}; // class BBox



#endif // #ifndef _AFR_BBOX_H



/** ***************************************************************************/
/** end of file BBox.h														  */
/** ***************************************************************************/
