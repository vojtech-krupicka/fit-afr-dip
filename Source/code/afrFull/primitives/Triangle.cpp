/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Triangle.cpp													  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Triangle.								  *
 **************************************************************************** */


// Including project libraries
#include "Triangle.h"
#include "BBox.h"
#include "../types/Ray.h"
#include "../types/Vector4.h"

#ifdef __AFR_OPEN_GL__
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif



/**
 * Triangle constructor.
 * @return Triangle.
 */ 
Triangle::Triangle()
	: Primitive()
{
} // Triangle()



/**
 * Triangle constructor. Assign material and texture to primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @return Triangle.
 */ 
Triangle::Triangle(Material *m, Texture *t = NULL)
	: Primitive(m, t)
{
} // Triangle()



/**
 * Triangle constructor. Assign material and texture to primitive and other 
 * components to sphere primitive.
 * @param Material m Pointer at material,
 * @param Texture t Pointer at instance of texture.
 * @param Vector3 v 3-link array of triangle vertices,
 * @param Vector3 n 3-link array of triangle normals (optional),
 * @param Vector2 c 3-link array of triangle texture coordinates (optional),
 * @param bool s Flag, if this primitive is static or dynamic (animated) - creates origin.
 * @return Triangle.
 */ 
Triangle::Triangle(Material *m, Texture *t, const Vector3 v[3], const Vector3 *n, const Vector2 *c, const bool &s)
	: Primitive(m, t)
{
	Vector3 normal;
	if(n == NULL) {
		Vector3 AB = v[1] - v[0];
		Vector3 BC = v[2] - v[1];
		
		normal = AB.Cross(BC).Normalize();
	}
	
	for(unsigned i = 0; i < 3; i++) {
		this->vertices[i] = v[i];
		this->normals[i] = (n != NULL) ? n[i] : normal;  
		this->coords[i] = (c != NULL && t != NULL) ? c[i] : Vector2::Zero();
	}
	
	this->edges[0] = this->vertices[0] - this->vertices[1];
	this->edges[1] = this->vertices[0] - this->vertices[2];	
	
	this->isPatch = (n != NULL);
	this->alpha = 0.0f;
	this->beta = 0.0f;
	this->gamma = 0.0f;
	
	// If this primitiv is animated, creates origin object
	this->origin = NULL;
	if(s) {
		this->origin = new Triangle(*this);
	}
} // Triangle()



/**
 * Copy constructor. Creates an identical triangle primitiv.
 * @param Triangle t Origin triangle, which will be copied.
 * @return Triangle Returns new triangle object.
 */ 
Triangle::Triangle(const Triangle &t)
	: Primitive(t.material, t.texture)
{
	this->origin = NULL;
	this->edges[0] = t.edges[0];
	this->edges[1] = t.edges[1];
	this->alpha = t.alpha;
	this->beta = t.beta;
	this->gamma = t.gamma;
	
	
	for(unsigned i = 0; i < 3; i++) {
		this->vertices[i] = t.vertices[i];
		this->normals[i] = t.normals[i];
		this->coords[i] = t.coords[i];
	}
} // Triangle()



/**
 * Default destructor.
 */ 
Triangle::~Triangle()
{
	if(this->origin != NULL) {
		delete this->origin;
		this->origin = NULL;
	}
} // Triangle()



/**
 * Virtual method for find intersect between primitive and ray.
 * @see Primitive::Hit().
 */ 
Primitive *Triangle::Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull)
{
	float A = this->edges[0].x();
	float B = this->edges[0].y();
	float C = this->edges[0].z();
	
	float D = this->edges[1].x();
	float E = this->edges[1].y();
	float F = this->edges[1].z();
	
	float G = r.d().x();
	float H = r.d().y();
	float I = r.d().z();
	
	float J = this->vertices[0].x() - r.o().x();
	float K = this->vertices[0].y() - r.o().y();
	float L = this->vertices[0].z() - r.o().z();
	
	float EIHF = E*I - H*F;
	float GFDI = G*F - D*I;
	float DHEG = D*H - E*G;
	
	float denom = (A*EIHF + B*GFDI + C*DHEG);
	float beta = (J*EIHF + K*GFDI + L*DHEG) / denom;
	
	if(beta <= 0.0f || beta >= 1.0f) {
		return NULL;
	}
	
	float AKJB = A*K - J*B;
	float JCAL = J*C - A*L;
	float BLKC = B*L - K*C;
	
	float gamma = (I*AKJB + H*JCAL + G*BLKC) / denom;
	if(gamma <= 0.0f || beta+gamma >= 1.0f) {
		return NULL;
	}
	
	float tval = -(F*AKJB + E*JCAL + D*BLKC) / denom;
	if(tval >= tmin && tval <= tmax) {
		float alpha = 1.0 - beta - gamma;
		
		// Cull back face
		if(cull) {
			Vector3 n0 = this->normals[0];
			Vector3 n1 = this->normals[1];
			Vector3 n2 = this->normals[2];			

			Vector3 n;
			n.x((n0.x() * alpha) + (n1.x() * beta) + (n2.x() * gamma))
			 .y((n0.y() * alpha) + (n1.y() * beta) + (n2.y() * gamma))
			 .z((n0.z() * alpha) + (n1.z() * beta) + (n2.z() * gamma));
			n = n.Normalize();
			
			if(r.d().Dot(n) >= 0.0f) {
				return NULL;
			} 
		}
		
		tmax = tval;
		this->alpha = alpha;		
		this->beta = beta;
		this->gamma = gamma;
		
		return this;
	}
	
	return NULL;
} // Hit()



/**
 * Virtual method for fill hit info structure with informations about intersect.
 * @see Primitive::GetHitInfo().
 */ 
bool Triangle::GetHitInfo(const Ray &r, THitInfo &info)
{
	info.primitive = this;
	info.normal = this->normals[0];
	
	if(this->isPatch) {
		info.normal.x((this->normals[0].x() * this->alpha) + (this->normals[1].x() * this->beta) + (this->normals[2].x() * this->gamma))
				   .y((this->normals[0].y() * this->alpha) + (this->normals[1].y() * this->beta) + (this->normals[2].y() * this->gamma))
				   .z((this->normals[0].z() * this->alpha) + (this->normals[1].z() * this->beta) + (this->normals[2].z() * this->gamma));
		info.normal = info.normal.Normalize();
	}
	
	if(this->texture != NULL && this->coords) {
		info.coords.x((this->coords[0].x() * this->alpha) + (this->coords[1].x() * this->beta) + (this->coords[2].x() * this->gamma))
				   .y((this->coords[0].y() * this->alpha) + (this->coords[1].y() * this->beta) + (this->coords[2].y() * this->gamma));
				   
		if(this->texture->HasNormalMap()) {
			Vector3 n = this->texture->GetNormal(info.coords, info.hit);
			Vector3 e10 = (this->vertices[1] - this->vertices[0]).Normalize();
			Vector2 c10 = (this->coords[1] - this->coords[0]).Normalize();
			
			Vector3 T;
			if(c10.x() != 0.0f) {
				T = e10 / c10.x();
			}
			else {
				Vector3 e20 = (this->vertices[2] - this->vertices[0]).Normalize();
				Vector2 c20 = (this->coords[2] - this->coords[0]).Normalize();

				if(c20.x() != 0.0f) {
					T = e20 / c20.x();
				}
				else {
					Vector3 e21 = (this->vertices[1] - this->vertices[2]).Normalize();
					Vector2 c21 = (this->coords[1] - this->coords[2]).Normalize();
					
					T = e21 / c21.x();
				}
			}
			
			T = T.Normalize();
			Vector3 B = (T.Cross(info.normal));
			
			float data[16] = {T.x(), T.y(), T.z(), 0.0, B.x(), B.y(), B.z(), 0.0, info.normal.x(), info.normal.y(), info.normal.z(), 0.0, 0.0, 0.0, 0.0, 0.0};
			Matrix tbn(data);
			
			info.normal = (n * tbn).Normalize();
		}
	}
	
	info.sign = -(r.d().Dot(info.normal));
	this->alpha = 0.0f;
	this->beta = 0.0f;
	this->gamma = 0.0f;
	
	
	return true;
} // GetHitInfo()



/**
 * Virtual method for create axes aligned bounding box around primitive.
 * @return BBox Returns primitives bounding box.
 * @see Primitive::GetBBox().
 */ 
BBox *Triangle::GetBBox() 
{
	if(this->bbox == NULL) {
		Vector3 min = Min(this->vertices[0], Min(this->vertices[1], this->vertices[2]));
		Vector3 max = Max(this->vertices[0], Max(this->vertices[1], this->vertices[2]));
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
bool Triangle::IntersectBBox(const BBox &box) const 
{
	Vector3 bHalfSize = 0.5f * box.GetSize();
	Vector3 bCenter = box.GetMin() + bHalfSize;
	
	Vector3 v0 = this->vertices[0] - bCenter;
	Vector3 v1 = this->vertices[1] - bCenter;
	Vector3 v2 = this->vertices[2] - bCenter;
	Vector3 e0 = v1 - v0;
	Vector3 e1 = v2 - v1;
	Vector3 e2 = v0 - v2;
	
	Vector3 tmp = Abs(e0);
	if(!Triangle::AxisX1(e0.z(), e0.y(), tmp.z(), tmp.y(), v0, v2, bHalfSize)) return false;
	if(!Triangle::AxisY2(e0.z(), e0.x(), tmp.z(), tmp.x(), v0, v2, bHalfSize)) return false;
	if(!Triangle::AxisZ2(e0.y(), e0.x(), tmp.y(), tmp.x(), v1, v2, bHalfSize)) return false;
	
	tmp = Abs(e1);
	if(!Triangle::AxisX1(e1.z(), e1.y(), tmp.z(), tmp.y(), v0, v2, bHalfSize)) return false;
	if(!Triangle::AxisY2(e1.z(), e1.x(), tmp.z(), tmp.x(), v0, v2, bHalfSize)) return false;
	if(!Triangle::AxisZ1(e1.y(), e1.x(), tmp.y(), tmp.x(), v0, v1, bHalfSize)) return false;
	
	tmp = Abs(e2);
	if(!Triangle::AxisX2(e2.z(), e2.y(), tmp.z(), tmp.y(), v0, v1, bHalfSize)) return false;
	if(!Triangle::AxisY1(e2.z(), e2.x(), tmp.z(), tmp.x(), v0, v1, bHalfSize)) return false;
	if(!Triangle::AxisZ2(e2.y(), e2.x(), tmp.y(), tmp.x(), v1, v2, bHalfSize)) return false;
	
	tmp = Vector3(v0.x(), v1.x(), v2.x());
	if(tmp.MinComponent() > bHalfSize.x() || tmp.MaxComponent() < -bHalfSize.x()) return false;
	
	tmp = Vector3(v0.y(), v1.y(), v2.y());
	if(tmp.MinComponent() > bHalfSize.y() || tmp.MaxComponent() < -bHalfSize.y()) return false;
	
	tmp = Vector3(v0.z(), v1.z(), v2.z());
	if(tmp.MinComponent() > bHalfSize.z() || tmp.MaxComponent() < -bHalfSize.z()) return false;
	
	Vector3 vMin, vMax;
	Vector3 norm = e0.Cross(e1);
	float decr = v0.x();
	if(norm.x() > 0.0) {
		vMin.x(-bHalfSize.x() - decr);
		vMax.x(bHalfSize.x() - decr);
	}
	else {
		vMin.x(bHalfSize.x() - decr);
		vMax.x(-bHalfSize.x() - decr);
	}

	decr = v0.y();
	if(norm.y() > 0.0) {
		vMin.y(-bHalfSize.y() - decr);
		vMax.y(bHalfSize.y() - decr);
	}
	else {
		vMin.y(bHalfSize.y() - decr);
		vMax.y(-bHalfSize.y() - decr);
	}

	decr = v0.z();
	if(norm.z() > 0.0) {
		vMin.z(-bHalfSize.z() - decr);
		vMax.z(bHalfSize.z() - decr);
	}
	else {
		vMin.z(bHalfSize.z() - decr);
		vMax.z(-bHalfSize.z() - decr);
	}
	
	if(norm.Dot(vMin) > 0.0) {
		return false;
	}
	if(norm.Dot(vMax) >= 0.0) {
		return true;
	}
	
	return false;
} // IntersectBBox()
	


/**
 * Triangle method for transform primitive with tranformation matrix m.
 * @see Primitive::Transform()
 */ 
Primitive *Triangle::Transform(const Matrix &m)
{
	this->matrix = m;
	
	if(this->origin == NULL) {
		for(unsigned i = 0; i < 3; i++) {
			this->vertices[i] = m * this->vertices[i];
			this->normals[i] = (m * this->normals[i]).Normalize();
		}
	}
	else {
		for(unsigned i = 0; i < 3; i++) {
			this->vertices[i] = m * this->origin->vertices[i];
			this->normals[i] = (m * this->origin->normals[i]).Normalize();
		}
	}
	
	this->edges[0] = this->vertices[0] - this->vertices[1];
	this->edges[1] = this->vertices[0] - this->vertices[2];	
	
	return this;
} // Transform()



/**
 * Virtual method for render primitive to scene in OpenGL - for test purposes only!
 * @see Primitive::Render()
 * @deprecated Only for test purposes.
 */ 
void Triangle::Render() const
{
	#ifdef __AFR_OPEN_GL__
		glBegin(GL_TRIANGLES);
			Color color;
			if(this->material != NULL) {
				Color color = this->material->GetDiffuse();
				glColor3f(color.r(), color.g(), color.b());
			}
			
			for(unsigned i = 0; i < 3; i++) {
				Vector3 v = this->vertices[i];
				Vector3 n = this->normals[i];
				
				glColor3f(color.r(), color.g(), color.b());
				glVertex3f(v.x(), v.y(), v.z());
				glNormal3f(n.x(), n.y(), n.z());
			}
		glEnd();
	#endif
	
	return;
} // Render()



/** ***************************************************************************/
/** Helper static methods for find intersection between triangle and AABB     */
/** ***************************************************************************/



/**
 * Find first intersection at axis X.
 */ 
bool Triangle::AxisX1(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v2, Vector3 &hs)
{
	float p0 = a * v0.y() - b * v0.z(); 
	float p2 = a * v2.y() - b * v2.z();
	float min, max;
	if(p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}

	float rad = fa * hs.y() + fb * hs.z();
	return (min > rad || max < -rad) ? false : true; 
} // AxisX1()



/**
 * Find second intersection at axis X.
 */ 
bool Triangle::AxisX2(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v1, Vector3 &hs)
{
	float p0 = a * v0.y() - b * v0.z(); 
	float p1 = a * v1.y() - b * v1.z();
	float min, max;
	if(p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}

	float rad = fa * hs.y() + fb * hs.z();
	return (min > rad || max < -rad) ? false : true;
} // AxisX2()



/**
 * Find first intersection at axis Y.
 */ 
bool Triangle::AxisY1(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v1, Vector3 &hs)
{
	float p0 = -a * v0.x() + b * v0.z(); 
	float p1 = -a * v1.x() + b * v1.z();
	float min, max;
	if(p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}

	float rad = fa * hs.x() + fb * hs.z();
	return (min > rad || max < -rad) ? false : true;
} // AxisY1()



/**
 * Find second intersection at axis Y.
 */ 
bool Triangle::AxisY2(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v2, Vector3 &hs)
{
	float p0 = -a * v0.x() + b * v0.z();
	float p2 = -a * v2.x() + b * v2.z();
	float min, max;
	if(p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}

	float rad = fa * hs.x() + fb * hs.z();
	return (min > rad || max < -rad) ? false : true;
} // AxisY2()



/**
 * Find first intersection at axis Z.
 */ 
bool Triangle::AxisZ1(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v1, Vector3 &hs)
{
	float p0 = a * v0.x() - b * v0.y(); 
	float p1 = a * v1.x() - b * v1.y();
	float min, max;
	if(p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}

	float rad = fa * hs.x() + fb * hs.y();
	return (min > rad || max < -rad) ? false : true;
} // AxisZ1()



/**
 * Find second intersection at axis Z.
 */ 
bool Triangle::AxisZ2(float a, float b, float fa, float fb, Vector3 &v1, Vector3 &v2, Vector3 &hs)
{
	float p1 = a * v1.x() - b * v1.y();
	float p2 = a * v2.x() - b * v2.y();
	float min, max;
	if(p2 < p1) {
		min = p2;
		max = p1;
	}
	else {
		min = p1;
		max = p2;
	}

	float rad = fa * hs.x() + fb * hs.y();
	return (min > rad || max < -rad) ? false : true;
} // AxisZ2()



/** ***************************************************************************/
/** end of file Triangle.cpp												  */
/** ***************************************************************************/
