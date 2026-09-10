/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Cone.cpp														  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Cone.									  *
 **************************************************************************** */


// Including project libraries
#include "Cone.h"
#include "BBox.h"
#include "../types/Ray.h"

#ifdef __AFR_OPEN_GL__
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#include "../types/Vector4.h"




/**
 * Cone constructor.
 * @return Cone.
 */ 
Cone::Cone()
	: Primitive()
{
	this->base = Vector3::Zero();
	this->apex = Vector3::Zero();
	this->baseRadius = 0.0f;
	this->apexRadius = 0.0f;
} // Cone()


/**
 * Cone constructor. Assign material and texture to primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @return Cone.
 */ 
Cone::Cone(Material *m, Texture *t = NULL)
	: Primitive(m, t)
{
	this->base = Vector3::Zero();
	this->apex = Vector3::Zero();
	this->baseRadius = 0.0f;
	this->apexRadius = 0.0f;
} // Cone()



/**
 * Cone constructor. Assign material and texture to primitive and other 
 * components to Cone primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture,
 * @param Vector3 b Center of base,
 * @param Vector3 a Center of apex,
 * @param float br Radius at base,
 * @param float ar Radius at apex.
 * @return Cone.
 */ 
Cone::Cone(Material *m, Texture *t, const Vector3 &b, const float &br, const Vector3 &a, const float &ar)
	: Primitive(m, t)
{
	this->base = b;
	this->apex = a;
	this->baseRadius = br;
	this->apexRadius = ar;
} // Cone()



/**
 * Virtual method for find intersect between primitive and ray.
 * @see Primitive::Hit().
 */ 
Primitive *Cone::Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull)
{
	return NULL;
} // Hit()



/**
 * Virtual method for fill hit info structure with informations about intersect.
 * @see Primitive::GetHitInfo().
 */ 
bool Cone::GetHitInfo(const Ray &r, THitInfo &info)
{
	return false;
} // GetHitInfo()



/**
 * Virtual method for create axes aligned bounding box around primitive.
 * @return BBox Returns primitives bounding box.
 * @see Primitive::GetBBox().
 */ 
BBox *Cone::GetBBox() 
{
	if(this->bbox = NULL) {
		Vector3 min = this->base - this->baseRadius;
		Vector3 max = this->apex + this->apexRadius;
		this->bbox = new BBox(min, max);
	}
	
	return this->bbox;
} // GetBBox()



/**
 * Virtual method for test, if primitive intersection given bounding box. If 
 * primitives lies in box or its intersecting box, return true.
 * @param BBox box Given bounding box.
 * @return bool Return true, if primitives lies in box or intersecting it, false otherwise.
 * @see Primitive::IntersectBBox().
 */ 
bool Cone::IntersectBBox(const BBox &box) const 
{
	return false;
} // IntersectBBox()



/**
 * Virtual method for transform primitive with tranformation matrix m.
 * @see Primitive::Transform()
 */ 
Primitive *Cone::Transform(const Matrix &m)
{
	this->base *= m;
	this->apex *= m;
	
	return this;
} // Transform()



/**
 * Virtual method for render primitive to scene in OpenGL - for test purposes only!
 * @see Primitive::Render()
 * @deprecated Only for test purposes.
 */ 
void Cone::Render() const
{
	#ifdef __AFR_OPEN_GL__
		if(this->material != NULL) {
			Color color = this->material->GetDiffuse();
			glColor3f(color.r(), color.g(), color.b());
		}
		
		Vector3 diff = this->base - this->apex;
		
		glPushMatrix();
		glTranslated(this->base.x(), this->base.y(), this->base.z());
		glRotated(-90, 1, 0, 0);
		glutSolidCone(this->baseRadius, diff.Length(), 30, 30);
		glPopMatrix();
	#endif
	
	return;
} // Render()



/** ***************************************************************************/
/** end of file Cone.cpp													  */
/** ***************************************************************************/
