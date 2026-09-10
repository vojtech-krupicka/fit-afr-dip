/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Quaternion.h													  *	
 * @date	10. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Quaternion.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_QUATERNION_H
#define _AFR_QUATERNION_H


// Including system libraries
#include <iostream>
#include <string>
#include <cmath>


// Including project libraries
#include "../utils/MathUtils.h"


// Defined macros
#define QUAT_EPS 1e-03


// Used namespaces
using namespace std;


// Predefinition of classes
class Vector4;
class Matrix;


/**
 * Class Quaternion
 * @brief class for four component quaternion for better handling rotation
 * transformations in 3D.
 */ 
class Quaternion
{
public:
	///< Friendly classes
	friend class Vector4;
	friend class Matrix;
	
	///< Implicit constructors
	inline Quaternion();
	inline Quaternion(const float data[4]);
	inline Quaternion(const float &w, const float &x, const float &y, const float &z);
	inline Quaternion(const Quaternion &q);
	Quaternion(const Vector4 &v);
	
	///< Implicit destructors
	~Quaternion() {};
	
	///< Accessors methods
	inline float w() const { return this->data[0]; };
	inline float x() const { return this->data[1]; };
	inline float y() const { return this->data[2]; };
	inline float z() const { return this->data[3]; };
	
	inline float operator[]	(const unsigned &i) const { return this->data[i]; };
	inline float &operator[] (const unsigned &i) { return this->data[i]; };
	
	///< Setters
	inline Quaternion& operator() (const float &w, const float &x, const float &y, const float &z);
	inline Quaternion& operator() (const Quaternion &q);
	inline Quaternion& w(const float &f) { this->data[0] = f; return *this; };
	inline Quaternion& x(const float &f) { this->data[1] = f; return *this; };
	inline Quaternion& y(const float &f) { this->data[2] = f; return *this; };
	inline Quaternion& z(const float &f) { this->data[3] = f; return *this; };
	
	///< Boolean operators
	friend bool operator==(const Quaternion &q1, const Quaternion &q2);
	friend bool operator!=(const Quaternion &q1, const Quaternion &q2);
	
	///< IO operators
	friend istream &operator>>(istream &is, Quaternion &q);
	friend ostream &operator<<(ostream &os, const Quaternion &q);
	
	///< Basic operators
	friend Quaternion operator+(const Quaternion &q1, const Quaternion &q2);
	friend Quaternion operator-(const Quaternion &q1, const Quaternion &q2);
	friend Quaternion operator*(const Quaternion &q1, const Quaternion &q2);
	friend Quaternion operator*(const Quaternion &q, const float &s);
	friend Quaternion operator*(const float &s, const Quaternion &q);
	
	Quaternion operator+() const;
	Quaternion operator-() const;
	
	Quaternion &operator=(const Quaternion &q);
	Quaternion &operator+=(const Quaternion &q);
	Quaternion &operator-=(const Quaternion &q);
	Quaternion &operator*=(const Quaternion &q);
	Quaternion &operator*=(const float &s);
	
	///< Quaternion operations
	float		Dot(const Quaternion &v) const;
	float		Normalize() const;
	float		Angle() const;
	float		SquaredAngle() const;
	Quaternion	Inverse() const;
	Quaternion	UnitInverse() const;
	Quaternion	Exp() const;
	Quaternion	Log() const;
	
	static Quaternion Squad(const float &time, Quaternion &p, Quaternion &a, Quaternion &b, Quaternion &q);
	static Quaternion Slerp(const float &time, Quaternion &p, Quaternion &q);
	
	friend Matrix 	operator>>(Matrix &m, Quaternion &q);
	friend Matrix 	operator<<(Matrix &m, const Quaternion &q);
	friend Vector4 	operator>>(Vector4 &v, Quaternion &q);
	friend Vector4	operator<<(Vector4 &v, const Quaternion &q);

private:
	float data[4];	///< w, x, y, z
}; // class Quaternion



/**
 * Implicit constructor. All components are zero.
 * @return Quaternion.
 */ 
inline Quaternion::Quaternion()
{
	this->data[0] = 0.0;
	this->data[1] = 0.0;
	this->data[2] = 0.0;
	this->data[3] = 0.0;
} // Quaternion()



/**
 * Implicit constructor from four-link float array.
 * @param float data[4] Four-link float array.
 * @return Quaternion.
 */ 
inline Quaternion::Quaternion(const float data[4])
{
	this->data[0] = data[0];
	this->data[1] = data[1];
	this->data[2] = data[2];
	this->data[3] = data[3];
} // Quaternion()



/**
 * Implicit constructor from four floats.
 * @param float w First quaternion's component,
 * @param float x Second quaternion's component,
 * @param float y Third quaternion's component,
 * @param float z Fourth quaternion's component.
 * @return Quaternion.
 */ 
inline Quaternion::Quaternion(const float &w, const float &x, const float &y, const float &z)
{
	this->data[0] = w;
	this->data[1] = x;
	this->data[2] = y;
	this->data[3] = z;
} // Quaternion()



/**
 * Copy contructor. Creates a copy of the quaternion 'q'.
 * @param Quaternion q Original quaternion, which will be copied.
 * @return Quaternion.
 */ 
inline Quaternion::Quaternion(const Quaternion &q)
{
	this->data[0] = q[0];
	this->data[1] = q[1];
	this->data[2] = q[2];
	this->data[3] = q[3];
} // Quaternion()



/**
 * Setter operator, which sets all components to quaternion.
 * @param float w First quaternion's component,
 * @param float x Second quaternion's component,
 * @param float y Third quaternion's component,
 * @param float z Fourth quaternion's component.
 * @return Quaternion.
 */ 
inline Quaternion &Quaternion::operator() (const float &w, const float &x, const float &y, const float &z)
{
	this->data[0] = w;
	this->data[1] = x;
	this->data[2] = y;
	this->data[3] = z;
	return *this;
} // operator()



/**
 * Copy quaternion's components into this quaternion.
 * @param Quaternion q Original quaternion, which will be copied.
 * @return Quaternion.
 */ 
inline Quaternion &Quaternion::operator() (const Quaternion &q)
{
	this->data[0] = q[0];
	this->data[1] = q[1];
	this->data[2] = q[2];
	this->data[3] = q[3];
	return *this;
} // operator()



/**
 * Method for Dot product of two quaternions.
 * @param Quaternion q Second quaternion.
 * @retrun float Returns dot product.
 */ 
inline float Quaternion::Dot(const Quaternion &q) const
{
	return ((this->w() * q.w()) + (this->x() * q.x()) + (this->y() * q.y()) + (this->z() * q.z()));
} // Dot()



/**
 * Method for normalize size (length) of quaternion.
 * @return float Returns normalized size of quaternion.
 */ 
inline float Quaternion::Normalize() const
{
	return (this->w()*this->w() + this->x()*this->x() + this->y()*this->y() + this->z()*this->z());
} // Normalize()



/**
 * Method for calculate angle.
 * @return float Returns angle.
 */ 
inline float Quaternion::Angle() const
{
	return sqrt(this->x()*this->x() + this->y()*this->y() + this->z()*this->z());
} // Angle()



/**
 * Method for calculate squared angle.
 * @return float Returns squared angle.
 */ 
inline float Quaternion::SquaredAngle() const
{
	return (this->x()*this->x() + this->y()*this->y() + this->z()*this->z());
} // SquaredAngle()



/**
 * Method for inverse quaternion. Returns new inversed quaternion.
 * @return Quaternion Returns new inversed quaternion.
 */ 
inline Quaternion Quaternion::Inverse() const
{
	Quaternion q;
	float norm = this->Normalize();

	if(norm > 0.0) {
		norm = 1.0 / norm;
		q.w(this->w() * norm);
		q.x(-(this->x()) * norm);
		q.y(-(this->y()) * norm);
		q.z(-(this->z()) * norm);
	}
	else {
		q.w(0.0f).x(0.0f).y(0.0f).z(0.0f);
	}
	
	return q;
} // Inverse()



/**
 * Method for unit inverse. 
 * @return Quaternion Returns new unit inversed quaternion.
 */ 
inline Quaternion Quaternion::UnitInverse() const
{
	return Quaternion(this->w(), (-this->x()), (-this->y()), (-this->z()));
} // UnitInverse()



/**
 * Method for calc exp from quaternion.
 * @return Quaternion Returns new exp quaternion.
 */ 
inline Quaternion Quaternion::Exp() const
{
	Quaternion q;
	float coeff;
    float angle = this->Angle();
    float sn = sin(angle);

    q.w(cos(angle));

    if(Abs(sn) >= QUAT_EPS) {
        coeff = sn / angle;
        q.x(coeff * this->x())
		 .y(coeff * this->y())
		 .z(coeff * this->z());
    }
    else {
        q.x(this->x())
		 .y(this->y())
		 .z(this->z());
    }
	
	return q;
} // Exp()



/**
 * Method for calc log from quaternion.
 * @return Quaternion Returns new log quaternion.
 */ 
inline Quaternion Quaternion::Log() const
{
	Quaternion q;	
	float angle, sn, coeff, w;
    w = this->w();
    q.w(0.0);

    if(fabs(w) < 1.0)
    {
        angle = acos(w);
        sn = sin(angle);
        if (fabs(sn) >= QUAT_EPS)
        {
            coeff = angle / sn;
            q.x(coeff * this->x())
             .y(coeff * this->y())
             .z(coeff * this->z());
            return q;
        }
    }

    q.x(this->x()).y(this->y()).z(this->z());	
	return q;
} // Log()	



/**
 * Operator for comparation of two quaternions.
 * @param Quaternion q1 A first operand,
 * @param Quaternion q2 A second operand.
 * @return bool Return true, if all compoments are equal, false otherwise.
 */ 
inline bool operator==(const Quaternion &q1, const Quaternion &q2) 
{
	return ((q1.w() == q2.w()) && (q1.x() == q2.x()) && (q1.y() == q2.y()) && (q1.z() == q2.z()));
} // operator==



/**
 * Operator for comparation of two quaternions.
 * @param Quaternion q1 A first operand,
 * @param Quaternion q2 A second operand.
 * @return bool Return true, if one of the compoments are not equal, false otherwise.
 */ 
inline bool operator!=(const Quaternion &q1, const Quaternion &q2)
{
	return ((q1.w() != q2.w()) || (q1.x() != q2.x()) || (q1.y() != q2.y()) || (q1.z() != q2.z()));
} // operator!=



/**
 * Operator for addition of two vectors.
 * @param Quaternion q1 A first operand,
 * @param Quaternion q2 A second operand.
 * @return Quaternion Return new quaternion as a result of additioning.
 */ 
inline Quaternion operator+(const Quaternion &q1, const Quaternion &q2) 
{
	return Quaternion(q1.w() + q2.w(), q1.x() + q2.x(), q1.y() + q2.y(), q1.z() + q2.z());
} // operator+



/**
 * Operator for subtration of two vectors.
 * @param Quaternion q1 A first operand,
 * @param Quaternion q2 A second operand.
 * @return Quaternion Return new quaternion as a result of subtrationing.
 */ 
inline Quaternion operator-(const Quaternion &q1, const Quaternion &q2) 
{
	return Quaternion(q1.w() - q2.w(), q1.x() - q2.x(), q1.y() - q2.y(), q1.z() - q2.z());
} // operator-



/**
 * Operator for multiplication of two vectors.
 * @param Quaternion q1 A first operand,
 * @param Quaternion q2 A second operand.
 * @return Quaternion Return new quaternion as a result of multiplication.
 */ 
inline Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion temp;
	
	float A = (q1.w() + q1.x()) * (q2.w() + q2.x());
    float B = (q1.z() - q1.y()) * (q2.y() - q2.z());
    float C = (q1.x() - q1.w()) * (q2.y() + q2.z());
    float D = (q1.y() + q1.z()) * (q2.x() - q2.w());
    float E = (q1.x() + q1.z()) * (q2.x() + q2.y());
    float F = (q1.x() - q1.z()) * (q2.x() - q2.y());
    float G = (q1.w() + q1.y()) * (q2.w() - q2.z());
    float H = (q1.w() - q1.y()) * (q2.w() + q2.z());
	
	float EpF = E + F, EmF = E - F;
    float GpH = G + H, GmH = G - H;
	
	temp[0] = B + 0.5 * (GpH - EpF);
	temp[1] = A - 0.5 * (GpH + EpF);
    temp[2] = -C + 0.5 * (EmF + GmH);
    temp[3] = -D + 0.5 * (EmF - GmH);
	
	return temp;
} // operator*



/**
 * Operator for multiplication of quaternion and scalar.
 * @param Quaternion q A first operand (quaternion),
 * @param float s A second operand (scalar).
 * @return Quaternion Return new quaternion as a result of multiplication.
 */
inline Quaternion operator*(const Quaternion &q, const float &s)
{
	return Quaternion(q.w() * s, q.x() * s, q.y() * s, q.z() * s);
} // operator*



/**
 * Operator for multiplication of quaternion and scalar.
 * @param float s A second operand (scalar).
 * @param Quaternion q A first operand (quaternion),
 * @return Quaternion Return new quaternion as a result of multiplication.
 */
inline Quaternion operator*(const float &s, const Quaternion &q)
{
	return Quaternion(q.w() * s, q.x() * s, q.y() * s, q.z() * s);
} // operator*



/**
 * Operator for get the same quaternion.
 * @return Quaternion Returns new quaternion, which has same components as the original.
 */ 
inline Quaternion Quaternion::operator+() const
{
	return Quaternion(this->w(), this->x(), this->y(), this->z());
} // operator+



/**
 * Operator for negation of quaternion's components.
 * @return Quaternion Returns new quaternion, which has negate components as the original.
 */ 
inline Quaternion Quaternion::operator-() const
{
	return Quaternion(-this->w(), -this->x(), -this->y(), -this->z());
} // operator-



/**
 * Operator for assignement.
 * @param Quaternion q An original quaternion (right value).
 * @return Quaternion Return a left value (quaternion) after assignement.
 */
inline Quaternion &Quaternion::operator=(const Quaternion &q)
{
	this->data[0] = q[0];
	this->data[1] = q[1];
	this->data[2] = q[2];
	this->data[3] = q[3];
	return *this;
} // operator=



/**
 * Operator for addition with quaternion.
 * @param Quaternion q An original quaternion (right value), which will be added to this.
 * @return Quaternion Return a left value (quaternion) after addition.
 */
inline Quaternion &Quaternion::operator+=(const Quaternion &q)
{
	*this = *this + q;
	return *this;
} // operator+=



/**
 * Operator for subtraction with quaternion.
 * @param Quaternion q An original quaternion (right value), which will be subtract from this.
 * @return Quaternion Return a left value (quaternion) after subtraction.
 */
inline Quaternion &Quaternion::operator-=(const Quaternion &q)
{
	*this = *this - q;
	return *this;
} // operator-=



/**
 * Operator for multiplication with quaternion.
 * @param Quaternion q An original quaternion (right value), which will be mult to this.
 * @return Quaternion Return a left value (quaternion) after multiplication.
 */
inline Quaternion &Quaternion::operator*=(const Quaternion &q)
{
	*this = *this * q;
	return *this;
} // operator*=



/**
 * Operator for multiplication with scalar.
 * @param float s An right scalar value, which will be multiplied with this.
 * @return Quaternion Return a left value (quaternion) after multiplication.
 */
inline Quaternion &Quaternion::operator*=(const float &s)
{
	*this = *this * s;
	return *this;
} // operator*=



#endif // #ifndef _AFR_QUATERNION_H



/** ***************************************************************************/
/** end of file Quaternion.h												  */
/** ***************************************************************************/
