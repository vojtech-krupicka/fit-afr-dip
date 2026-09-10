/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Polygon.cpp														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Polygon.								  *
 **************************************************************************** */


// Including project libraries
#include "Polygon.h"
#include "BBox.h"
#include "../types/Ray.h"
#include "../types/Vector4.h"

#ifdef __AFR_OPEN_GL__
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif



/**
 * Polygon constructor.
 * @return Polygon.
 */ 
Polygon::Polygon()
	: Primitive()
{
	this->count = 0;
	this->verts = NULL;
	this->normals = NULL;
} // Polygon()



/**
 * Polygon constructor. Assign material and texture to primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @return Polygon.
 */ 
Polygon::Polygon(Material *m, Texture *t = NULL)
	: Primitive(m, t)
{
	this->count = 0;
	this->verts = NULL;
	this->normals = NULL;
} // Polygon()



/**
 * Polygon constructor. Assign material and texture to primitive and other 
 * components to Polygon primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @param Vector3 v Array of all polygon vertices,
 * @param Vector3 n Array of all polygon normals,
 * @param unsigned vc Count of all vertices,
 * @return Polygon.
 * @throw InvalidArgumentException Throws when there are no vertices.
 * @throw RuntimeException Throws when memory is to low.
 */ 
Polygon::Polygon(Material *m, Texture *t, const Vector3 *v, const Vector3 *n, const unsigned &vc)
	: Primitive(m, t)
{
	// Check if there are any vertices
	if(v == NULL || vc < 4) {
		throw InvalidArgumentException("Can't create polygon! Number of vertices must be 4 or higher.");
	}
	
	// Init properties
	this->count = vc;
	this->verts = NULL;
	this->normals = NULL;
	
	// Allocates memory
	this->verts = new(nothrow) Vector3[vc];
	this->normals = new(nothrow) Vector3[vc];
	if(this->verts == NULL || this->normals == NULL) {
		throw RuntimeException("Can't create polygon! Can't alloc memory.");
	}
	
	// If there is no normals, count plane normal
	Vector3 normal;
	if(n == NULL) {
		Vector3 AB = v[1] - v[0];
		Vector3 BC = v[2] - v[1];
		normal = AB.Cross(BC).Normalize();
	}
	
	// Copy all vertices and theirs normals
	for(unsigned i = 0; i < this->count; i++) {
		this->verts[i] = v[i];
		
		if(n != NULL) {
			this->normals[i] = n[i];
		}
		else {
			this->normals[i] = normal;
		}
	}
} // Polygon()



/**
 * Default destructor. Delete vertices and normals array.
 */ 
Polygon::~Polygon()
{
	if(this->verts != NULL) {
		delete [] this->verts;
	}
	
	if(this->normals != NULL) {
		delete [] this->normals;
	}
} // ~Polygon()



/**
 * Virtual method for find intersect between primitive and ray.
 * @see Primitive::Hit().
 */ 
Primitive *Polygon::Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull)
{
	return NULL;
} // Hit()



/**
 * Virtual method for fill hit info structure with informations about intersect.
 * @see Primitive::GetHitInfo().
 */ 
bool Polygon::GetHitInfo(const Ray &r, THitInfo &info)
{
	return false;
} // GetHitInfo()



/**
 * Virtual method for create axes aligned bounding box around primitive.
 * @return BBox Returns primitives bounding box.
 * @see Primitive::GetBBox().
 */ 
BBox *Polygon::GetBBox() 
{
	return new BBox();
} // GetBBox()



/**
 * Virtual method for test, if primitive intersection given bounding box. If 
 * primitives lies in box or its intersecting box, return true.
 * @param BBox box Given bounding box.
 * @return bool Return true, if primitives lies in box or intersecting it, false otherwise.
 * @see Primitive::IntersectBBox().
 */ 
bool Polygon::IntersectBBox(const BBox &box) const 
{
	return false;
} // IntersectBBox()



/**
 * Virtual method for transform primitive with tranformation matrix m.
 * @see Primitive::Transform()
 */ 
Primitive *Polygon::Transform(const Matrix &m)
{
	Matrix inverse = m.Transpose();
	for(unsigned i = 0; i < this->count; i++) {
		this->verts[i] *= m;
		this->normals[i] *= inverse;
		this->normals[i] = this->normals[i].Normalize();
	}
	
	return this;
} // Transform()



/**
 * Virtual method for render primitive to scene in OpenGL - for test purposes only!
 * @see Primitive::Render()
 * @deprecated Only for test purposes.
 */ 
void Polygon::Render() const
{
	#ifdef __AFR_OPEN_GL__
		glBegin(GL_QUADS);
			if(this->material != NULL) {
				Color color = this->material->GetDiffuse();
				glColor3f(color.r(), color.g(), color.b());
			}
			
			for(unsigned i = 0; i < 4; i++) {
				Vector3 v = this->verts[i];
				Vector3 n = this->normals[i];
				
				glVertex3f(v.x(), v.y(), v.z());
				glNormal3f(n.x(), n.y(), n.z());
			}
		glEnd();
	#endif
	
	return;
} // Render()



/** ***************************************************************************/
/** end of file Polygon.cpp													  */
/** ***************************************************************************/
