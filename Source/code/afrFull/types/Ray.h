/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Ray.h															  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Ray.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_RAY_H
#define _AFR_RAY_H


// Including project libraries
#include "Vector3.h"


// Used namespaces
using namespace std;


/**
 * Class Ray
 * @brief Class for reprezentation of ray for raytracing.
 */ 
class Ray
{
public:
	///< Implicit constructors
	inline Ray() { this->data[0] = Vector3::Zero(); this->data[1] = Vector3::Zero(); };
	inline Ray(const Vector3 data[2]) { this->data[0] = data[0]; this->data[1] = data[1]; };
	inline Ray(const Vector3 &o, const Vector3 &d) { this->data[0] = o; this->data[1] = d; };
	inline Ray(const Ray &r) { *this = r; };
	
	///< Implicit destructors
	~Ray() {};
	
	///< Getters
	inline Vector3 o() const { return this->data[0]; };
	inline Vector3 d() const { return this->data[1]; };
	inline Vector3 Origin() const { return this->data[0]; };
	inline Vector3 Direction() const { return this->data[1]; };
	
	///< Setters
	inline Ray o(const Vector3 &o) { this->data[0] = o; return *this; };
	inline Ray d(const Vector3 &d) { this->data[1] = d; return *this; };
	inline Ray Origin(const Vector3 &o) { this->data[0] = o; return *this; };
	inline Ray Direction(const Vector3 &d) { this->data[1] = d; return *this; };
	
	///< IO operators
	friend istream &operator>>(istream &is, Ray &r);
	friend ostream &operator<<(ostream &os, const Ray &r);
	
	///< Basic operators
	inline Ray &operator=(const Ray &r) { this->o(r.o()); this->d(r.d()); return *this; };
	
	///< Ray operations
	inline Vector3 PointAt(const float &t) const { return this->o() + t * this->d(); };
	
	
private:
	Vector3 data[2];
}; // class Ray



#endif // #ifndef _AFR_RAY_H



/** ***************************************************************************/
/** end of file Ray.h														  */
/** ***************************************************************************/

