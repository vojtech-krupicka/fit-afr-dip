/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    AnimatedTriangle.cpp											  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class AnimatedTriangle.						  *
 **************************************************************************** */


// Including project libraries
#include "AnimatedTriangle.h"
#include "Triangle.h"
#include "BBox.h"
#include "../types/Ray.h"



/**
 * AnimatedTriangle constructor.
 * @return AnimatedTriangle.
 */ 
AnimatedTriangle::AnimatedTriangle()
	: Primitive()
{
} // AnimatedTriangle()



/**
 * AnimatedTriangle constructor. Assign material and texture to primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @return AnimatedTriangle.
 */ 
AnimatedTriangle::AnimatedTriangle(Material *m, Texture *t = NULL)
	: Primitive(m, t)
{
} // AnimatedTriangle()



/**
 * AnimatedTriangle constructor. Assign material and texture to primitive and other 
 * components to sphere primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture,
 * @param unsigned c Count of key frames,
 * @param float f Array of times for each key frame,
 * @param Vector3 v Array of all vertices in all key frames,
 * @param Vector3 n Array of all normals in all key frames,
 * @return AnimatedTriangle.
 */ 
AnimatedTriangle::AnimatedTriangle(Material *m, Texture *t, const unsigned &c, float *f, Vector3 *v, Vector3 *n)
	: Primitive(m, t)
{
	this->frames = c;
	this->times = new float[this->frames];
	this->triangles.Clear();
	
	for(unsigned i = 0; i < this->frames; i++) {
		float time = f[i];
		Vector3 verts[3];
		Vector3 normals[3];
		
		for(unsigned j = 0; j < 3; j++) {
			verts[j] = v[i*3+j];
			normals[j] = n[i*3+j];
		}
		
		this->AddTriangle(time, verts, normals);
	}
	
	if(this->triangles.Size()) {
		Triangle *tri = this->triangles.First();
		this->triangle = new Triangle(this->material, this->texture, tri->GetVertices(), tri->GetNormals(), tri->GetCoords(), false);
	}
} // AnimatedTriangle()



/**
 * Default destructor.
 */ 
AnimatedTriangle::~AnimatedTriangle()
{
	// Deletes triangles
	for(unsigned i = 0; i < this->triangles.Size(); i++) {
		delete this->triangles[i];
	}
	this->triangles.Clear();
	
	// Delete key frames
	if(this->times != NULL) {
		delete [] this->times;
	}
	
	// Delete triangle
	if(this->triangle != NULL) {
		delete this->triangle;
	}	
} // ~AnimatedTriangle()



/**
 * Virtual method for find intersect between primitive and ray.
 * @see Primitive::Hit().
 */ 
Primitive *AnimatedTriangle::Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull)
{
	return NULL;
} // Hit()



/**
 * Virtual method for fill hit info structure with informations about intersect.
 * @see Primitive::GetHitInfo().
 */ 
bool AnimatedTriangle::GetHitInfo(const Ray &r, THitInfo &info)
{
	return false;
} // GetHitInfo()



/**
 * Virtual method for create axes aligned bounding box around primitive.
 * @return BBox Returns primitives bounding box.
 * @see Primitive::GetBBox().
 */ 
BBox *AnimatedTriangle::GetBBox() 
{
	return this->triangle->GetBBox();
} // GetBBox()



/**
 * Virtual method for test, if primitive intersection given bounding box. If 
 * primitives lies in box or its intersecting box, return true.
 * @param BBox box Given bounding box.
 * @return bool Return true, if primitives lies in box or intersecting it, false otherwise.
 * @see Primitive::IntersectBBox().
 */ 
bool AnimatedTriangle::IntersectBBox(const BBox &box) const 
{
	return this->triangle->IntersectBBox(box);
} // IntersectBBox()



/**
 * AnimatedTriangle method for transform primitive with tranformation matrix m.
 * @see Primitive::Transform()
 */ 
Primitive *AnimatedTriangle::Transform(const Matrix &m)
{
	return this;
} // Transform()



/**
 * Virtual method for render primitive to scene in OpenGL - for test purposes only!
 * @see Primitive::Render()
 * @deprecated Only for test purposes.
 */ 
void AnimatedTriangle::Render() const
{
	return;
} // Render()



/**
 * Method for adds new triangle at specific key frame given by time.
 * @param float t Key frame time,
 * @param Trinagle t Triangle primitive.
 * @return AnimatedTriangle Provides fluent interface.
 */ 
AnimatedTriangle *AnimatedTriangle::AddTriangle(const float &time, Triangle *t)
{
	this->triangles.Add(t);
	this->times[this->triangles.Size()-1] = time;	
	
	return this;
} // AddTriangle()



/**
 * Method for adds new triangle at specific key frame given by time.
 * @param float t Key frame time,
 * @param Vector3 v Triangle vertices,
 * @param Vector3 n Triangle normals.
 * @return AnimatedTriangle Provides fluent interface.
 */ 
AnimatedTriangle *AnimatedTriangle::AddTriangle(const float &time, const Vector3 v[3], const Vector3 *n)
{
	Triangle *tri = new Triangle(this->material, this->texture, v, n, NULL, false);
	return this->AddTriangle(time, tri);
} // AddTriangle()



/**
 * Method for count linear interpolation for triangle between two key frames.
 * @param float t Actual animation time.
 * @return bool Returns true, if LERP is finished ok, false otherwise.
 */ 
bool AnimatedTriangle::Lerp(const float &t)
{
	if(t <= this->times[0]) {
		this->triangle->SetVertex(0, this->triangles[0]->GetVertex(0))
					  ->SetVertex(1, this->triangles[0]->GetVertex(1))
					  ->SetVertex(2, this->triangles[0]->GetVertex(2));
		
		this->triangle->SetNormal(0, this->triangles[0]->GetNormal(0))
					  ->SetNormal(1, this->triangles[0]->GetNormal(1))
					  ->SetNormal(2, this->triangles[0]->GetNormal(2));
	}
	else if(t >= this->times[this->frames-1]) {
		unsigned idx = this->frames-1;
		this->triangle->SetVertex(0, this->triangles[idx]->GetVertex(0))
					  ->SetVertex(1, this->triangles[idx]->GetVertex(1))
					  ->SetVertex(2, this->triangles[idx]->GetVertex(2));
		
		this->triangle->SetNormal(0, this->triangles[idx]->GetNormal(0))
 					  ->SetNormal(1, this->triangles[idx]->GetNormal(1))
					  ->SetNormal(2, this->triangles[idx]->GetNormal(2));
	}
	else {
		unsigned idx = 0;
		double param = 0.0;
		
		for(idx = 0; idx < this->frames-1; idx++) {
			if(t >= this->times[idx] && t < this->times[idx+1]) {
				break;
			}
		}
		
		param = (t - this->times[idx])/(this->times[idx+1] - this->times[idx]);
		
		this->triangle->SetVertex(0, this->Lerp(param, this->triangles[idx]->GetVertex(0), this->triangles[idx+1]->GetVertex(0)))
					  ->SetVertex(1, this->Lerp(param, this->triangles[idx]->GetVertex(1), this->triangles[idx+1]->GetVertex(1)))
					  ->SetVertex(2, this->Lerp(param, this->triangles[idx]->GetVertex(2), this->triangles[idx+1]->GetVertex(2)));
		
		this->triangle->SetNormal(0, this->Lerp(param, this->triangles[idx]->GetNormal(0), this->triangles[idx+1]->GetNormal(0)))
					  ->SetNormal(1, this->Lerp(param, this->triangles[idx]->GetNormal(1), this->triangles[idx+1]->GetNormal(1)))
					  ->SetNormal(2, this->Lerp(param, this->triangles[idx]->GetNormal(2), this->triangles[idx+1]->GetNormal(2)));
		
		this->triangle->SetNormal(0, this->triangle->GetNormal(0).Normalize())
					  ->SetNormal(1, this->triangle->GetNormal(1).Normalize())
					  ->SetNormal(2, this->triangle->GetNormal(2).Normalize());
	}
	
	
	return true;
} // Lerp()



/** ***************************************************************************/
/** end of file AnimatedTriangle.cpp										  */
/** ***************************************************************************/
