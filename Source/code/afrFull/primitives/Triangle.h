/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Triangle.h														  *	
 * @date	11. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Triangle.								  	  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_TRIANGLE_H
#define _AFR_TRIANGLE_H


// Including project libraries
#include "Primitive.h"



// Predefinition of classes
class BBox;



/**
 * Class Triangle
 * @brief Implementation of sphere primitive.
 */ 
class Triangle : public Primitive
{
public:
	///< Implicit constructors
	Triangle();
	Triangle(Material *m, Texture *t);
	Triangle(Material *m, Texture *t, const Vector3 v[3], const Vector3 *n, const Vector2 *c, const bool &s);
	Triangle(const Triangle &t);
	
	///< Implicit destructors
	virtual ~Triangle();
	
	///< Vector operations
	virtual Primitive *Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull = true);
	virtual bool GetHitInfo(const Ray &r, THitInfo &info);
	
	virtual BBox *GetBBox();
	virtual bool IntersectBBox(const BBox &box) const;
	
	virtual Primitive *Transform(const Matrix &m);
	virtual void Render() const;
	
	///< Getters
	inline Vector3 GetVertex(const unsigned &i) const { return this->vertices[i]; };
	inline Vector3 GetNormal(const unsigned &i) const { return this->normals[i]; };
	inline Vector2 GetCoord(const unsigned &i) const { return this->coords[i]; };
	
	inline Vector3 *GetVertices() { return this->vertices; };
	inline Vector3 *GetNormals() { return this->normals; };
	inline Vector2 *GetCoords() { return this->coords; };
	
	///< Setters
	inline Triangle *SetVertex(const unsigned &i, const Vector3 &v) { this->vertices[i] = v; return this; };
	inline Triangle *SetNormal(const unsigned &i, const Vector3 &n) { this->normals[i] = n; return this; };
	inline Triangle *SetCoord(const unsigned &i, const Vector3 &c) { this->coords[i] = c; return this; };
	
protected:
	static bool AxisX1(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v2, Vector3 &hs);
	static bool AxisX2(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v1, Vector3 &hs);
	static bool AxisY1(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v1, Vector3 &hs);
	static bool AxisY2(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v2, Vector3 &hs);
	static bool AxisZ1(float a, float b, float fa, float fb, Vector3 &v0, Vector3 &v1, Vector3 &hs);
	static bool AxisZ2(float a, float b, float fa, float fb, Vector3 &v1, Vector3 &v2, Vector3 &hs);
	
	Vector3 vertices[3];
	Vector3 normals[3];
	Vector2 coords[3];
	Vector3 edges[2];
	
	float 	alpha;
	float 	beta;
	float	gamma;
	
	bool	isPatch;
	
	Triangle *origin;
	Matrix	 matrix;
}; // class Triangle



#endif // #ifndef _AFR_TRIANGLE_H



/** ***************************************************************************/
/** end of file Triangle.h													  */
/** ***************************************************************************/
