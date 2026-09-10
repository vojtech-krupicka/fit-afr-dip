/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Vector4.h														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Vector4.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_VECTOR_4_H
#define _AFR_VECTOR_4_H


// Including system libraries
#include <iostream>
#include <string>
#include <cmath>


// Including project libraries
#include "../utils/MathUtils.h"


// Used namespaces
using namespace std;


// Predefinition of classes
class Vector2;
class Vector3;
class Matrix;


/**
 * Class Vector4
 * @brief Class for four component vector for store 3D space or 4D space vector.
 */ 
class Vector4
{
public:
	///< Friendly classes
	friend class Vector2;
	friend class Vector3;
	friend class Matrix;

	///< Implicit constructors
	inline Vector4();
	inline Vector4(const float data[4]);
	inline Vector4(const float &x, const float &y, const float &z, const float &w);
	inline Vector4(const Vector4 &v);
	Vector4(const Vector2 &v);
	Vector4(const Vector3 &v);
	
	///< Implicit destructors
	~Vector4() {};
	
	///< Accessors methods
	inline float x() const { return this->data[0]; };
	inline float y() const { return this->data[1]; };
	inline float z() const { return this->data[2]; };
	inline float w() const { return this->data[3]; };
	
	inline float operator[]	(const unsigned &i) const { return this->data[i]; };
	inline float &operator[] (const unsigned &i) { return this->data[i]; };
	
	///< Setters
	inline Vector4& operator() (const float &x, const float &y, const float &z, const float &w);
	inline Vector4& x(const float &f) { this->data[0] = f; return *this; };
	inline Vector4& y(const float &f) { this->data[1] = f; return *this; };
	inline Vector4& z(const float &f) { this->data[2] = f; return *this; };
	inline Vector4& w(const float &f) { this->data[3] = f; return *this; };
	
	///< Boolean operators
	friend bool operator==(const Vector4 &v1, const Vector4 &v2);
	friend bool operator!=(const Vector4 &v1, const Vector4 &v2);
	
	///< IO operators
	friend istream &operator>>(istream &is, Vector4 &v);
	friend ostream &operator<<(ostream &os, const Vector4 &v);
	
	///< Basic operators
	friend Vector4 operator+(const Vector4 &v1, const Vector4 &v2);
	friend Vector4 operator+(const Vector4 &v, const float &s);
	friend Vector4 operator+(const float &s, const Vector4 &v);
	friend Vector4 operator-(const Vector4 &v1, const Vector4 &v2);
	friend Vector4 operator-(const Vector4 &v, const float &s);
	friend Vector4 operator*(const Vector4 &v1, const Vector4 &v2);
	friend Vector4 operator*(const Vector4 &v, const float &s);
	friend Vector4 operator*(const float &s, const Vector4 &v);
	friend Vector4 operator/(const Vector4 &v1, const Vector4 &v2);
	friend Vector4 operator/(const Vector4 &v, const float &s);
	
	Vector4 operator+() const;
	Vector4 operator-() const;
	
	Vector4 &operator=(const Vector4 &v);
	Vector4 &operator+=(const Vector4 &v);
	Vector4 &operator+=(const float &s);
	Vector4 &operator-=(const Vector4 &v);
	Vector4 &operator-=(const float &s);
	Vector4 &operator*=(const Vector4 &v);
	Vector4 &operator*=(const float &s);
	Vector4 &operator*=(const Matrix &m);
	Vector4 &operator/=(const Vector4 &v);
	Vector4 &operator/=(const float &s);
	
	///< Vector operations
	inline float MinComponent() const;
	inline float MaxComponent() const;
	inline float MinAbsComponent() const;
	inline float MaxAbsComponent() const;
	
	inline unsigned MinComponentIndex() const;
	inline unsigned MaxComponentIndex() const;
	inline unsigned MinAbsComponentIndex() const;
	inline unsigned MaxAbsComponentIndex() const;
	
	Vector4 Normalize() const;
	Vector4 Cross(const Vector4 &v) const;
	float 	Dot(const Vector4 &v) const;
	float 	Length(const Vector4 &v) const;
	float 	Length() const;
	float 	SquaredLength() const;	
	float	Sum() const;
	float	Mult() const;
	
	friend Vector4 Min(const Vector4 &v1, const Vector4 &v2);
	friend Vector4 Max(const Vector4 &v1, const Vector4 &v2);
	friend Vector4 Abs(const Vector4 &v);
	
	///< Static methods, used for creating common vectors
	static Vector4 Zero() { return Vector4(0.0, 0.0, 0.0, 0.0); };
	static Vector4 One() { return Vector4(1.0, 1.0, 1.0, 1.0); };
	static Vector4 XAxis() { return Vector4(1.0, 0.0, 0.0, 0.0); };
	static Vector4 YAxis() { return Vector4(0.0, 1.0, 0.0, 0.0); };
	static Vector4 ZAxis() { return Vector4(0.0, 0.0, 1.0, 0.0); };
	static Vector4 WAxis() { return Vector4(0.0, 0.0, 0.0, 1.0); };
	
private:
	float data[4];
}; // class Vector4



/**
 * Implicit constructor. All components are zero.
 * @return Vector4.
 */ 
inline Vector4::Vector4()
{
	this->data[0] = 0.0;
	this->data[1] = 0.0;
	this->data[2] = 0.0;
	this->data[3] = 0.0;
} // Vector4()



/**
 * Implicit constructor from four-link float array.
 * @param float data[4] Four-link float array.
 * @return Vector4.
 */ 
inline Vector4::Vector4(const float data[4])
{
	this->data[0] = data[0];
	this->data[1] = data[1];
	this->data[2] = data[2];
	this->data[3] = data[3];
} // Vector4()



/**
 * Implicit constructor from four floats.
 * @param float x First vector's component,
 * @param float y Second vector's component,
 * @param float z Third vector's component,
 * @param float w Fourth vector's component.
 * @return Vector4.
 */ 
inline Vector4::Vector4(const float &x, const float &y, const float &z, const float &w)
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
	this->data[3] = w;
} // Vector4()



/**
 * Copy contructor. Creates a copy of the vector 'v'.
 * @param Vector4 v Original vector, which will be copied.
 * @return Vector4.
 */ 
inline Vector4::Vector4(const Vector4 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
	this->data[3] = v[3];
} // Vector4()



/**
 * Setter operator, which sets all components to vector.
 * @param float x First vector's component,
 * @param float y Second vector's component,
 * @param float z Third vector's component,
 * @param float w Fourth vector's component.
 * @return Vector4.
 */ 
inline Vector4 &Vector4::operator() (const float &x, const float &y, const float &z, const float &w)
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
	this->data[3] = w;
	return *this;
} // operator()



/**
 * Method for getting min component from vector.
 * @return float Returs the minimal vector's component.
 */ 
inline float Vector4::MinComponent() const
{
	return Min(this->x(), this->y(), this->z(), this->w());
} // MinComponent()



/**
 * Method for getting max component from vector.
 * @return float Returs the maximal vector's component.
 */ 
inline float Vector4::MaxComponent() const
{
	return Max(this->x(), this->y(), this->z(), this->w());
} // MaxComponent()



/**
 * Method for getting min absolut component from vector.
 * @return float Returs the minimal absolut vector's component.
 */ 
inline float Vector4::MinAbsComponent() const
{
	return Min(Abs(this->x()), Abs(this->y()), Abs(this->z()), Abs(this->w()));
} // MinAbsComponent()



/**
 * Method for getting max absolut component from vector.
 * @return float Returs the maximal absolut vector's component.
 */ 
inline float Vector4::MaxAbsComponent() const
{
	return Max(Abs(this->x()), Abs(this->y()), Abs(this->z()), Abs(this->w()));
} // MaxAbsComponent()



/**
 * Method for getting an index of min components of vector.
 * @return unsigned Returns the index with minimal value.
 */ 
inline unsigned Vector4::MinComponentIndex() const
{
	unsigned index = 0;
	float temp = this->data[0];
	
	if(this->data[1] < temp) {
		temp = this->data[1];
		index = 1;
	}
	if(this->data[2] < temp) {
		temp = this->data[2];
		index = 2;
	}
	if(this->data[3] < temp) {
		index = 3;
	}
	
	return index;
} // MinComponentIndex()



/**
 * Method for getting an index of max component of vector.
 * @return unsigned Returns the index with maximal value.
 */ 
inline unsigned Vector4::MaxComponentIndex() const
{
	unsigned index = 0;
	float temp = this->data[0];
	
	if(this->data[1] > temp) {
		temp = this->data[1];
		index = 1;
	}
	if(this->data[2] > temp) {
		temp = this->data[2];
		index = 2;
	}
	if(this->data[3] > temp) {
		index = 3;
	}
	
	return index;
} // MaxComponentIndex()



/**
 * Method for getting an index of min absolut component of vector.
 * @return unsigned Returns the index with minimal absolut value.
 */ 
inline unsigned Vector4::MinAbsComponentIndex() const
{
	unsigned index = 0;
	float temp = Abs(this->data[0]);
	
	if(Abs(this->data[1]) < temp) {
		temp = Abs(this->data[1]);
		index = 1;
	}
	if(Abs(this->data[2]) < temp) {
		temp = Abs(this->data[2]);
		index = 2;
	}
	if(Abs(this->data[3]) < temp) {
		index = 3;
	}
	
	return index;
} // MinAbsComponentIndex()



/**
 * Method for getting an index of max component of vector.
 * @return unsigned Returns the index with maximal value.
 */ 
inline unsigned Vector4::MaxAbsComponentIndex() const
{
	unsigned index = 0;
	float temp = Abs(this->data[0]);
	
	if(Abs(this->data[1]) > temp) {
		temp = Abs(this->data[1]);
		index = 1;
	}
	if(Abs(this->data[2]) > temp) {
		temp = Abs(this->data[2]);
		index = 2;
	}
	if(Abs(this->data[3]) > temp) {
		index = 3;
	}
	
	return index;
} // MaxAbsComponentIndex()



/**
 * Method for normalizating vetor. Creates a new unit vector from vector. All
 * component are in range 0 .. 1.
 * @return Vector4 Returns new unit vector.
 */ 
inline Vector4 Vector4::Normalize() const
{
	float len = this->Length();
	return Vector4(this->x()/len, this->y()/len, this->z()/len, 1.0);
} // Normalize()



/**
 * Method for cross product of two vectors. Last component will be always 1.
 * @param Vector4 v A second operand of operation.
 * @return Vector4 Returns a result of cross product as a new vector.
 */ 
inline Vector4 Vector4::Cross(const Vector4 &v) const
{
	return Vector4( this->y() * v.z() - this->z() * v.y(), 
					this->z() * v.x() - this->x() * v.z(), 
					this->x() * v.y() - this->y() * v.x(),
					1.0);
} // Cross()



/**
 * Method for dot product of two vectors.
 * @param Vector4 v A second operand of operation.
 * @return float Returns a dot product as a scalar.
 */ 
inline float Vector4::Dot(const Vector4 &v) const
{
	return (this->x() * v.x() + this->y() * v.y() + this->z() * v.z());
} // Dot()



/**
 * Method for calc length between two vectors.
 * @param Vector4 v A second operand of operation.
 * @return float Returns a distance between two vectors.
 */ 
inline float Vector4::Length(const Vector4 &v) const
{
	Vector4 dif(v - *this);	
	return sqrt(dif.x()*dif.x() + dif.y()*dif.y() + dif.z()*dif.z());
} // Length()



/**
 * Method for calc vector's length.
 * @return float Returns a length of this vector.
 */
inline float Vector4::Length() const
{
	return sqrt(this->x()*this->x() + this->y()*this->y() + this->z()*this->z());
} // Length()



/**
 * Method for calc vector's squared length.
 * @return float Returns a squared length of this vector.
 */
inline float Vector4::SquaredLength() const
{
	return (this->x()*this->x() + this->y()*this->y() + this->z()*this->z());
} // SquaredLength()



/**
 * Method for sum all vector's components.
 * @return float Returns a sum of all vector's components.
 */
inline float Vector4::Sum() const
{
	return (this->x() + this->y() + this->z() + this->w());
} // Sum()



/**
 * Method for mult all vector's components.
 * @return float Returns a mult of all vector's components.
 */
inline float Vector4::Mult() const
{
	return (this->x() * this->y() * this->z() * this->w());
} // Mult()



/**
 * Function for create a new vector with minimal components of two vectors.
 * @param Vector4 v1 A first operand,
 * @param Vector4 v2 A second operand.
 * @return Vector4 Returns a minimal vector.
 */ 
inline Vector4 Min(const Vector4 &v1, const Vector4 &v2)
{
	return Vector4(Min(v1.x(), v2.x()), Min(v1.y(), v2.y()), Min(v1.z(), v2.z()), Min(v1.w(), v2.w()));
} // Min()



/**
 * Function for create a new vector with maximal components of two vectors.
 * @param Vector4 v1 A first operand,
 * @param Vector4 v2 A second operand.
 * @return Vector4 Returns a maximal vector.
 */ 
inline Vector4 Max(const Vector4 &v1, const Vector4 &v2)
{
	return Vector4(Max(v1.x(), v2.x()), Max(v1.y(), v2.y()), Max(v1.z(), v2.z()), Max(v1.w(), v2.w()));
} // Max()



/**
 * Function for create a new vector with absolute all components.
 * @param Vector4 v An operand.
 * @return Vector4 Returns a absolute vector.
 */ 
inline Vector4 Abs(const Vector4 &v)
{
	return Vector4(Abs(v.x()), Abs(v.y()), Abs(v.z()), Abs(v.w()));
} // Abs()



/**
 * Operator for comparation of two vectors.
 * @param Vector4 v1 A first operand,
 * @param Vector4 v2 A second operand.
 * @return bool Return true, if all compoments are equal, false otherwise.
 */ 
inline bool operator==(const Vector4 &v1, const Vector4 &v2) 
{
	return ((v1.x() == v2.x()) && (v1.y() == v2.y()) && (v1.z() == v2.z()) && (v1.w() == v2.w()));
} // operator==



/**
 * Operator for comparation of two vectors.
 * @param Vector4 v1 A first operand,
 * @param Vector4 v2 A second operand.
 * @return bool Return true, if one of the compoments are not equal, false otherwise.
 */ 
inline bool operator!=(const Vector4 &v1, const Vector4 &v2)
{
	return ((v1.x() != v2.x()) || (v1.y() != v2.y()) || (v1.z() != v2.z()) || (v1.w() != v2.w()));
} // operator!=



/**
 * Operator for addition of two vectors.
 * @param Vector4 v1 A first operand,
 * @param Vector4 v2 A second operand.
 * @return Vector4 Return new vector as a result of additioning.
 */ 
inline Vector4 operator+(const Vector4 &v1, const Vector4 &v2)
{
	return Vector4(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z(), v1.w() + v2.w());
} // operator+



/**
 * Operator for addition vector with scalar.
 * @param Vector4 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector4 Return new vector as a result of additioning.
 */ 
inline Vector4 operator+(const Vector4 &v, const float &s)
{
	return Vector4(v.x() + s, v.y() + s, v.z() + s, v.w() + s);
} // operator+



/**
 * Operator for addition scalar with vector.
 * @param float s A second operand (scalar).
 * @param Vector4 v A first operand (vector),
 * @return Vector4 Return new vector as a result of additioning.
 */ 
inline Vector4 operator+(const float &s, const Vector4 &v)
{
	return Vector4(v.x() + s, v.y() + s, v.z() + s, v.w() + s);
} // operator+



/**
 * Operator for subtration of two vectors.
 * @param Vector4 v1 A first operand,
 * @param Vector4 v2 A second operand.
 * @return Vector4 Return new vector as a result of subtrationing.
 */ 
inline Vector4 operator-(const Vector4 &v1, const Vector4 &v2)
{
	return Vector4(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z(), v1.w() - v2.w());
} // operator-



/**
 * Operator for subtration of vector and scalar.
 * @param Vector4 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector4 Return new vector as a result of subtrationing.
 */ 
inline Vector4 operator-(const Vector4 &v, const float &s)
{
	return Vector4(v.x() - s, v.y() - s, v.z() - s, v.w() - s);
} // operator-



/**
 * Operator for multiplication of two vectors.
 * @param Vector4 v1 A first operand,
 * @param Vector4 v2 A second operand.
 * @return Vector4 Return new vector as a result of multiplication.
 */ 
inline Vector4 operator*(const Vector4 &v1, const Vector4 &v2)
{
	return Vector4(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z(), v1.w() * v2.w());
} // operator*



/**
 * Operator for multiplication of vector and scalar.
 * @param Vector4 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector4 Return new vector as a result of multiplication.
 */ 
inline Vector4 operator*(const Vector4 &v, const float &s)
{
	return Vector4(v.x() * s, v.y() * s, v.z() * s, v.w() * s);
} // operator*



/**
 * Operator for multiplication of scalar and vector.
 * @param float s A second operand (scalar).
 * @param Vector4 v A first operand (vector),
 * @return Vector4 Return new vector as a result of multiplication.
 */ 
inline Vector4 operator*(const float &s, const Vector4 &v)
{
	return Vector4(v.x() * s, v.y() * s, v.z() * s, v.w() * s);
} // operator*



/**
 * Operator for division of two vectors.
 * @param Vector4 v1 A first operand,
 * @param Vector4 v2 A second operand.
 * @return Vector4 Return new vector as a result of division.
 */
inline Vector4 operator/(const Vector4 &v1, const Vector4 &v2)
{
	return Vector4(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z(), v1.w() / v2.w());
} // operator/	



/**
 * Operator for division of vector and scalar.
 * @param Vector4 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector4 Return new vector as a result of division.
 */ 
inline Vector4 operator/(const Vector4 &v, const float &s)
{
	return Vector4(v.x() / s, v.y() / s, v.z() / s, v.w() / s);
} // operator/



/**
 * Operator for get the same vector.
 * @return Vector4 Returns new vector, which has same components as the original.
 */ 
inline Vector4 Vector4::operator+() const
{
	return Vector4(this->x(), this->y(), this->z(), this->w());
} // operator+



/**
 * Operator for negation of vector's components.
 * @return Vector4 Returns new vector, which has negate components as the original.
 */ 
inline Vector4 Vector4::operator-() const
{
	return Vector4(-this->x(), -this->y(), -this->z(), -this->w());
} // operator-
	
	
	
/**
 * Operator for assignement.
 * @param Vector4 v An original vector (right value).
 * @return Vector4 Return a left value (vector) after assignement.
 */
inline Vector4 &Vector4::operator=(const Vector4 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
	this->data[3] = v[3];
	return *this;
} // operator=



/**
 * Operator for addition with vector.
 * @param Vector4 v An original vector (right value), which will be added to this.
 * @return Vector4 Return a left value (vector) after addition.
 */
inline Vector4 &Vector4::operator+=(const Vector4 &v)
{
	*this = *this + v;
	return *this;
} // operator+=



/**
 * Operator for addition vector with scalar.
 * @param float s An original vector (right value), which will be added to this.
 * @return Vector4 Return a left value (vector) after addition.
 */
inline Vector4 &Vector4::operator+=(const float &s)
{
	*this = *this + s;
	return *this;
} // operator+=



/**
 * Operator for subtraction with vector.
 * @param Vector4 v An original vector (right value), which will be subtract from this.
 * @return Vector4 Return a left value (vector) after subtraction.
 */
inline Vector4 &Vector4::operator-=(const Vector4 &v)
{
	*this = *this - v;
	return *this;
} // operator-=



/**
 * Operator for subtraction with scalar.
 * @param float s An original vector (right value), which will be subtract from this.
 * @return Vector4 Return a left value (vector) after subtraction.
 */
inline Vector4 &Vector4::operator-=(const float &s)
{
	*this = *this - s;
	return *this;
} // operator-=



/**
 * Operator for multiplication with vector.
 * @param Vector4 v An original vector (right value), which will be mult to this.
 * @return Vector4 Return a left value (vector) after multiplication.
 */
inline Vector4 &Vector4::operator*=(const Vector4 &v)
{
	*this = *this * v;
	return *this;
} // operator*=



/**
 * Operator for multiplication with scalar.
 * @param float s An right scalar value, which will be multiplied with this.
 * @return Vector4 Return a left value (vector) after multiplication.
 */
inline Vector4 &Vector4::operator*=(const float &s)
{
	*this = *this * s;
	return *this;
} // operator*=



/**
 * Operator for division with vector.
 * @param Vector4 v Denominator (right value).
 * @return Vector4 Return a left value (vector) after division.
 */
inline Vector4 &Vector4::operator/=(const Vector4 &v)
{
	*this = *this / v;
	return *this;
} // operator/=



/**
 * Operator for division with scalar.
 * @param float s Denominator (right value). No zero division control!
 * @return Vector4 Return a left value (vector) after division.
 */
inline Vector4 &Vector4::operator/=(const float &s)
{
	*this = *this / s;
	return *this;
} // operator/=



#endif // #ifndef _AFR_VECTOR_4_H



/** ***************************************************************************/
/** end of file Vector4.h													  */
/** ***************************************************************************/
