/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Sphere.cpp														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Sphere.									  *
 **************************************************************************** */


// Including project libraries
#include "Sphere.h"
#include "BBox.h"
#include "../types/Ray.h"
#include "../types/Vector4.h"

#ifdef __AFR_OPEN_GL__
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif


/**
 * Sphere constructor.
 * @return Sphere.
 */ 
Sphere::Sphere()
	: Primitive()
{
	this->center = Vector3::Zero();
	this->radius = 0.0f;
	this->rRadius = 0.0f;
	this->sqrRadius = 0.0f;
	this->vEast = Vector3::Zero();
	this->vNorth = Vector3::Zero();
	this->vUp = Vector3::Zero();
} // Sphere()



/**
 * Sphere constructor. Assign material and texture to primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @return Sphere.
 */ 
Sphere::Sphere(Material *m, Texture *t = NULL)
	: Primitive(m, t)
{
	this->center = Vector3::Zero();
	this->radius = 0.0;	
	this->rRadius = 0.0f;
	this->sqrRadius = 0.0f;
	this->vEast = Vector3::Zero();
	this->vNorth = Vector3::Zero();
	this->vUp = Vector3::Zero();
} // Sphere()



/**
 * Sphere constructor. Assign material and texture to primitive and other 
 * components to sphere primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @param Vector3 c Sphere center,
 * @param float r Sphere radius.
 * @param bool s Flag, if this primitive is static or dynamic (animated) - creates origin.
 * @return Sphere.
 */ 
Sphere::Sphere(Material *m, Texture *t, const Vector3 &c, const float &r, const bool &s)
	: Primitive(m, t)
{
	this->center = c;
	this->radius = r;
	
	this->sqrRadius = r*r;
	this->rRadius = 1.0f / r;
	this->vEast = Vector3::XAxis();
	this->vNorth = Vector3::YAxis();
	this->vUp = this->vNorth.Cross(this->vEast);
	
	// If this primitiv is animated, creates origin object
	this->origin = NULL;
	if(s) {
		this->origin = new Sphere(*this);
	}
} // Sphere()



/**
 * Copy constructor. Creates an identical sphere primitiv.
 * @param Sphere s Origin sphere, which will be copied.
 * @return Sphere Returns new sphere object.
 */ 
Sphere::Sphere(const Sphere &s)
	: Primitive(s.material, s.texture)
{
	this->origin = NULL;
	this->center = s.center;
	this->radius = s.radius;
	
	this->rRadius = s.rRadius;
	this->sqrRadius = s.sqrRadius;
	this->vEast = s.vEast;
	this->vNorth = s.vNorth;
	this->vUp = s.vUp;
} // Sphere()



/**
 * Default destructor.
 */ 
Sphere::~Sphere()
{
	if(this->origin != NULL) {
		delete this->origin;
		this->origin = NULL;
	}
} // Sphere()



/**
 * Virtual method for find intersect between primitive and ray.
 * @see Primitive::Hit().
 */ 
Primitive *Sphere::Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull)
{
	Vector3 temp = r.o() - this->center;
	
	double a = r.d().SquaredLength();
	double b = 2*temp.Dot(r.d());
	double c = temp.SquaredLength() - this->sqrRadius;
	
	double discriminant = b*b - 4*a*c;
	
	// If ray intersect the sphere
	if(discriminant > 0.0) {
		discriminant = sqrt(discriminant);
		double t = (-b - discriminant) / (2*a);
		
		// Check valid interval
		if(t < tmin) {
			t = (-b + discriminant) / (2*a);
		}
		if(t < tmin || t > tmax) {
			return NULL;
		}
		
		// Cull back face
		if(!cull) {
			Vector3 h = r.PointAt(t);
			Vector3 n = ((h - this->center) * this->rRadius).Normalize();
			
			if(r.d().Dot(n) >= 0.0f) {
				return NULL;
			}
		}
		
		// Is valid hit
		tmax = t;
		return this;
	}
	
	return NULL;
} // Hit()



/**
 * Virtual method for fill hit info structure with informations about intersect.
 * @see Primitive::GetHitInfo().
 */ 
bool Sphere::GetHitInfo(const Ray &r, THitInfo &info)
{
	Vector3 nv = ((info.hit - this->center) * this->rRadius).Normalize();
	if(this->texture != NULL) {
		float theta = acos(nv.Dot(this->vNorth));
		float phi = atan2(this->vEast.Dot(nv), this->vUp.Dot(nv));
		
		if(phi < 0.0) {
			phi += M_2PI;
		}
		info.coords = Vector2(-phi*M_1_2PI, -(M_PI - theta)*M_1_PI);	
		
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



/**
 * Virtual method for create axes aligned bounding box around primitive.
 * @return BBox Returns primitives bounding box.
 * @see Primitive::GetBBox().
 */ 
BBox *Sphere::GetBBox() 
{
	if(this->bbox == NULL) {
		Vector3 min = this->center - radius;
		Vector3 max = this->center + radius;
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
bool Sphere::IntersectBBox(const BBox &box) const 
{
	float a, b;
    float dMin = 0.0f;
	float dMax = 0.0f;
    float r2 = this->radius * this->radius;
	
	for(int i = 0; i < 3; i++) {
		a = this->center[i] - box.GetMin()[i];
		b = this->center[i] - box.GetMax()[i];
		
		a *= a;
		b *= b;
		
		dMax += Max(a, b);
		
		if(this->center[i] < box.GetMin()[i]) {
			dMin += a;
		}
		else if(this->center[i] > box.GetMax()[i]) {
			dMin += b;
		}
	}
	
	return (dMin <= r2 && r2 <= dMax);
} // IntersectBBox()



/**
 * Virtual method for transform primitive with tranformation matrix m.
 * @see Primitive::Transform()
 */ 
Primitive *Sphere::Transform(const Matrix &m)
{
	this->matrix = m;
	
	if(this->origin == NULL) {
		this->center = this->center * m;
		this->vEast = (this->vEast * m);
		this->vNorth = (this->vNorth * m);
		this->vUp = (this->vNorth.Cross(this->vEast));
	}
	else {
		this->center = this->origin->center * m;
		this->vEast = (this->origin->vEast * m);
		this->vNorth = (this->origin->vNorth * m);
		this->vUp = (this->vNorth.Cross(this->vEast));
	}
	
	return this;
} // Transform()



/**
 * Virtual method for render primitive to scene in OpenGL - for test purposes only!
 * @see Primitive::Render()
 * @deprecated Only for test purposes.
 */ 
void Sphere::Render() const
{
	#ifdef __AFR_OPEN_GL__
		if(this->material != NULL) {
			Color color = this->material->GetDiffuse();
			glColor3f(color.r(), color.g(), color.b());
		}
		
		glPushMatrix();
		glTranslated(this->center.x(), this->center.y(), this->center.z());
		glutSolidSphere(this->radius, 30, 30);
		glPopMatrix();
	#endif
	
	return;
} // Render()



/** ***************************************************************************/
/** end of file Sphere.cpp													  */
/** ***************************************************************************/
