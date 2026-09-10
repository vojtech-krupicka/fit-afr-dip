/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    BBox.cpp														  *	
 * @date	01. 12. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class BBox.									  *
 **************************************************************************** */


// Including project libraries
#include "BBox.h"
#include "../types/Ray.h"



/**
 * BBox constructor.
 * @return BBox.
 */ 
BBox::BBox()
	: Primitive()
{
	this->min = Vector3::Zero();
	this->max = Vector3::Zero();
	this->size = Vector3::Zero();
} // BBox()



/**
 * BBox constructor. Sets position and size.
 * @param Vector3 p BBox position - left bottom front point,
 * @param Vector3 s Size of BBox.
 * @return BBox.
 */ 
BBox::BBox(const Vector3 &pMin, const Vector3 &pMax)
	: Primitive()
{
	this->min = pMin;
	this->max = pMax;
	this->size = pMax - pMin;
} // BBox()



/**
 * Copy constructor.
 * @param BBox b Original BBox which will be copied.
 * @return BBox.
 */ 
BBox::BBox(const BBox &b)
	: Primitive()
{
	this->min = b.min;
	this->max = b.max;
	this->size = b.size;
} // BBox()



/**
 * Virtual method for find intersect between primitive and ray.
 * @see Primitive::Hit().
 */ 
Primitive *BBox::Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull)
{
	return false;
} // Hit()



/**
 * Virtual method for fill hit info structure with informations about intersect.
 * @see Primitive::GetHitInfo().
 */ 
bool BBox::GetHitInfo(const Ray &r, THitInfo &info)
{
	return false;
} // GetHitInfo()



/**
 * Virtual method for test, if primitive intersection given bounding box. If 
 * primitives lies in box or its intersecting box, return true.
 * @param BBox box Given bounding box.
 * @return bool Return true, if primitives lies in box or intersecting it, false otherwise.
 * @see Primitive::IntersectBBox().
 */ 
bool BBox::IntersectBBox(const BBox &box) const 
{
	return false;
} // IntersectBBox()



/** ***************************************************************************/
/** end of file BBox.cpp													  */
/** ***************************************************************************/
