/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Cylinder.cpp													  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Cylinder.								  *
 **************************************************************************** */


// Including project libraries
#include "Cylinder.h"
#include "BBox.h"
#include "../types/Ray.h"
#include "../types/Vector4.h"

#ifdef __AFR_OPEN_GL__
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif



/**
 * Cylinder constructor.
 * @return Cylinder.
 */ 
Cylinder::Cylinder()
	: Primitive()
{
	this->base = Vector3::Zero();
	this->apex = Vector3::Zero();
	this->radius = 0.0f;
} // Cylinder()


/**
 * Cylinder constructor. Assign material and texture to primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @return Cylinder.
 */ 
Cylinder::Cylinder(Material *m, Texture *t = NULL)
	: Primitive(m, t)
{
	this->base = Vector3::Zero();
	this->apex = Vector3::Zero();
	this->radius = 0.0f;
} // Cylinder()



/**
 * Cylinder constructor. Assign material and texture to primitive and other 
 * components to Cylinder primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture,
 * @param Vector3 b Center of base,
 * @param Vector3 a Center of apex,
 * @param float r Radius of cylinder.
 * @return Cylinder.
 */ 
Cylinder::Cylinder(Material *m, Texture *t, const Vector3 &b, const Vector3 &a, const float &r)
	: Primitive(m, t)
{
	this->base = b;
	this->apex = a;
	this->radius = r;
} // Cylinder()



/**
 * Virtual method for find intersect between primitive and ray.
 * @see Primitive::Hit().
 */ 
Primitive *Cylinder::Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull)
{
	Vector3 temp = r.o() - this->base;
	Vector3 d = r.d();
	
	float a = d.x()*d.x() + d.z()*d.z();
	float b = 2*temp.x()*d.x() + 2*temp.z()*d.z();
	float c = temp.x()*temp.x() + temp.z()*temp.z() - (this->radius * this->radius);
	
	float discriminant = b*b - 4*a*c;
	
	if(discriminant > 0.0) {
		discriminant = sqrt(discriminant);
		float t = (-b - discriminant) / (2*a);
		
		// Check valid interval
		if(t < tmin) {
			t = (-b + discriminant) / (2*a);
		}
		if(t < tmin || t > tmax) {
			return NULL;
		}
		
		Vector3 hit = r.PointAt(t);
		if(hit.y() >= this->base.y() && hit.y() <= this->apex.y()) {
			tmax = t;
			return this;
		}
	}	
	
	return NULL;
} // Hit()



/**
 * Virtual method for fill hit info structure with informations about intersect.
 * @see Primitive::GetHitInfo().
 */ 
bool Cylinder::GetHitInfo(const Ray &r, THitInfo &info)
{
	Vector3 axis = (this->apex - this->base).Normalize();
	float height = this->base.Length(this->apex);
	
	Vector3 v1 = info.hit - this->base;
	Vector3 v2 = axis * axis.Dot(v1);
	Vector3 nv = (v1 - v2).Normalize();

	
	// First calculate UV coords
	if(this->texture != NULL) {
		Vector3 n = info.normal;
		float phi = atan2(n.z(), n.x());
		
		if(phi < 0.0) {
			phi += M_2PI;
		}
		info.coords = Vector2(-phi*M_1_2PI, (this->apex.y()-info.hit.y()) / height);

		Vector3 nm = this->texture->GetNormal(info.coords, info.hit);
		if(this->texture->HasNormalMap()) {
			nv = (nv * nm).Normalize();
		}
	}
	
	info.normal = nv;
	info.primitive = this;
	info.sign = -(r.d().Dot(info.normal));

	
	return true;
} // GetHitInfo()



bool Cylinder::HitCaps(const Ray &r, const Vector3 &center, const float &tmin, float &tmax) const
{
	Vector3 temp = center - r.o();
	Vector3 dir = r.d();
	
	float ab2 = dir.Dot(dir);
	float acab = temp.Dot(dir);
	tmax = acab / ab2;
	
	Vector3 P = r.PointAt(tmax);
	Vector3 H = P - center;
	float h2 = H.Dot(H);
	float r2 = this->radius*this->radius;
	
	if(h2 <= r2) {
		return true;
	}
	
	return false;
} // HitCaps()



/**
 * Virtual method for create axes aligned bounding box around primitive.
 * @return BBox Returns primitives bounding box.
 * @see Primitive::GetBBox().
 */ 
BBox *Cylinder::GetBBox() 
{
	if(this->bbox == NULL) {
		Vector3 min = this->base - this->radius;
		Vector3 max = this->apex + this->radius;
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
bool Cylinder::IntersectBBox(const BBox &box) const 
{
	Vector3 bMin = box.GetMin();
	Vector3 bMax = box.GetMax();
	
	Vector3 B = this->base + this->radius;	
	if(bMin.x() > B.x() || bMin.z() > B.z()) {
		return false;	
	}
	
	Vector3 A = this->apex - this->radius;
	if(bMax.x() < A.x() || bMax.z() < A.z()) {
		return false;
	}
	
	if(bMax.y() < this->base.y() || bMin.y() > this->apex.y()) {
		return false;
	}
	
	return true;
} // IntersectBBox()



/**
 * Virtual method for transform primitive with tranformation matrix m.
 * @see Primitive::Transform()
 */ 
Primitive *Cylinder::Transform(const Matrix &m)
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
void Cylinder::Render() const
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
		glutSolidCone(this->radius, diff.Length(), 30, 30);
		glPopMatrix();
	#endif 
	
	return;
} // Render()



/** ***************************************************************************/
/** end of file Cylinder.cpp												  */
/** ***************************************************************************/
