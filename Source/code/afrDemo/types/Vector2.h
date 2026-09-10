/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Vector2.h														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Vector2.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_VECTOR_2_H
#define _AFR_VECTOR_2_H


// Including system libraries
#include <iostream>
#include <string>
#include <cmath>


// Including project libraries
#include "../utils/MathUtils.h"


// Used namespaces
using namespace std;


// Predefinition of classes
class Vector3;
class Vector4;
class Matrix;


/**
 * Class Vector2
 * @brief Class for two components vector for store 2D space vectors (texture 
 * coords).
 */ 
class Vector2
{
public:
	///< Friendly classes
	friend class Vector3;
	friend class Vector4;
	friend class Matrix;

	///< Implicit constructors
	inline Vector2();
	inline Vector2(const float data[2]);
	inline Vector2(const float &x, const float &y);
	inline Vector2(const Vector2 &v);
	Vector2(const Vector3 &v);
	Vector2(const Vector4 &v);
	
	///< Implicit destructors
	~Vector2() {};
	
	///< Accessors methods
	inline float x() const { return this->data[0]; };
	inline float y() const { return this->data[1]; };
	
	inline float operator[]	(const unsigned &i) const { return this->data[i]; };
	inline float &operator[] (const unsigned &i) { return this->data[i]; };
	
	///< Setters
	inline Vector2& operator() (const float &x, const float &y);
	inline Vector2& x(const float &f) { this->data[0] = f; return *this; };
	inline Vector2& y(const float &f) { this->data[1] = f; return *this; };
	
	///< Boolean operators
	friend bool operator==(const Vector2 &v1, const Vector2 &v2);
	friend bool operator!=(const Vector2 &v1, const Vector2 &v2);
	
	///< IO operators
	friend istream &operator>>(istream &is, Vector2 &v);
	friend ostream &operator<<(ostream &os, const Vector2 &v);
	
	///< Basic operators
	friend Vector2 operator+(const Vector2 &v1, const Vector2 &v2);
	friend Vector2 operator+(const Vector2 &v, const float &s);
	friend Vector2 operator+(const float &s, const Vector2 &v);
	friend Vector2 operator-(const Vector2 &v1, const Vector2 &v2);
	friend Vector2 operator-(const Vector2 &v, const float &s);
	friend Vector2 operator*(const Vector2 &v, const Vector2 &v2);
	friend Vector2 operator*(const Vector2 &v, const float &s);
	friend Vector2 operator*(const float &s, const Vector2 &v);
	friend Vector2 operator/(const Vector2 &v1, const Vector2 &v2);
	friend Vector2 operator/(const Vector2 &v, const float &s);
	
	Vector2 operator+() const;
	Vector2 operator-() const;
	
	Vector2 &operator=(const Vector2 &v);
	Vector2 &operator+=(const Vector2 &v);
	Vector2 &operator+=(const float &s);
	Vector2 &operator-=(const Vector2 &v);
	Vector2 &operator-=(const float &s);
	Vector2 &operator*=(const Vector2 &v);
	Vector2 &operator*=(const float &s);
	Vector2 &operator*=(const Matrix &m);
	Vector2 &operator/=(const Vector2 &v);
	Vector2 &operator/=(const float &s);
	
	///< Vector operations
	inline float MinComponent() const;
	inline float MaxComponent() const;
	inline float MinAbsComponent() const;
	inline float MaxAbsComponent() const;
	
	inline unsigned MinComponentIndex() const;
	inline unsigned MaxComponentIndex() const;
	inline unsigned MinAbsComponentIndex() const;
	inline unsigned MaxAbsComponentIndex() const;
	
	Vector2 Normalize() const;
	float	Cross(const Vector2 &v) const;
	float 	Dot(const Vector2 &v) const;
	float 	Length(const Vector2 &v) const;
	float 	Length() const;
	float 	SquaredLength() const;	
	float	Sum() const;
	float	Mult() const;
	
	friend Vector2 Min(const Vector2 &v1, const Vector2 &v2);
	friend Vector2 Max(const Vector2 &v1, const Vector2 &v2);
	friend Vector2 Abs(const Vector2 &v);
	
	///< Static methods, used for creating common vectors
	static Vector2 Zero() { return Vector2(0.0, 0.0); };
	static Vector2 One() { return Vector2(1.0, 1.0); };
	static Vector2 XAxis() { return Vector2(1.0, 0.0); };
	static Vector2 YAxis() { return Vector2(0.0, 1.0); };
	
private:
	float data[2];
}; // class Vector2



/**
 * Implicit constructor. All components are zero.
 * @return Vector2.
 */ 
inline Vector2::Vector2()
{
	this->data[0] = 0.0;
	this->data[1] = 0.0;
} // Vector2()



/**
 * Implicit constructor from two-link float array.
 * @param float data[2] Two-link float array.
 * @return Vector2.
 */ 
inline Vector2::Vector2(const float data[2])
{
	this->data[0] = data[0];
	this->data[1] = data[1];
} // Vector2()



/**
 * Implicit constructor from four floats.
 * @param float x First vector's component,
 * @param float y Second vector's component.
 * @return Vector2.
 */ 
inline Vector2::Vector2(const float &x, const float &y)
{
	this->data[0] = x;
	this->data[1] = y;
} // Vector2()



/**
 * Copy contructor. Creates a copy of the vector 'v'.
 * @param Vector2 v Original vector, which will be copied.
 * @return Vector2.
 */ 
inline Vector2::Vector2(const Vector2 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
} // Vector2()



/**
 * Setter operator, which sets all components to vector.
 * @param float x First vector's component,
 * @param float y Second vector's component.
 * @return Vector2.
 */ 
inline Vector2 &Vector2::operator() (const float &x, const float &y)
{
	this->data[0] = x;
	this->data[1] = y;
	return *this;
} // operator()



/**
 * Method for getting min component from vector.
 * @return float Returs the minimal vector's component.
 */ 
inline float Vector2::MinComponent() const
{
	return Min(this->x(), this->y());
} // MinComponent()



/**
 * Method for getting max component from vector.
 * @return float Returs the maximal vector's component.
 */ 
inline float Vector2::MaxComponent() const
{
	return Max(this->x(), this->y());
} // MaxComponent()



/**
 * Method for getting min absolut component from vector.
 * @return float Returs the minimal absolut vector's component.
 */ 
inline float Vector2::MinAbsComponent() const
{
	return Min(Abs(this->x()), Abs(this->y()));
} // MinAbsComponent()



/**
 * Method for getting max absolut component from vector.
 * @return float Returs the maximal absolut vector's component.
 */ 
inline float Vector2::MaxAbsComponent() const
{
	return Max(Abs(this->x()), Abs(this->y()));
} // MaxAbsComponent()



/**
 * Method for getting an index of min component of vector.
 * @return unsigned Returns the index with minimal value.
 */ 
inline unsigned Vector2::MinComponentIndex() const
{
	return (this->x() <= this->y()) ? 0 : 1;
} // MinComponentIndex()



/**
 * Method for getting an index of max component of vector.
 * @return unsigned Returns the index with maximal value.
 */ 
inline unsigned Vector2::MaxComponentIndex() const
{
	return (this->x() >= this->y()) ? 0 : 1;
} // MaxComponentIndex()



/**
 * Method for getting an index of min absolut component of vector.
 * @return unsigned Returns the index with minimal absolut value.
 */ 
inline unsigned Vector2::MinAbsComponentIndex() const
{
	return (Abs(this->x()) <= Abs(this->y())) ? 0 : 1;
} // MinAbsComponentIndex()



/**
 * Method for getting an index of max component of vector.
 * @return unsigned Returns the index with maximal value.
 */ 
inline unsigned Vector2::MaxAbsComponentIndex() const
{
	return (Abs(this->x()) >= Abs(this->y())) ? 0 : 1;
} // MaxAbsComponentIndex()



/**
 * Method for normalizating vetor. Creates a new unit vector from vector. All
 * components are in range 0 .. 1.
 * @return Vector2 Returns new unit vector.
 */ 
inline Vector2 Vector2::Normalize() const
{
	float len = this->Length();
	return Vector2(this->x()/len, this->y()/len);
} // Normalize()



/**
 * Method for cross product of two vectors. This will returns only Z-axis of 
 * three-link vector.
 * @param Vector2 v A second operand of operation.
 * @return Vector2 Returns a result of cross product as a new vector.
 */ 
inline float Vector2::Cross(const Vector2 &v) const
{
	return (this->x() * v.y() - this->y() * v.x());
} // Cross()



/**
 * Method for dot product of two vectors.
 * @param Vector2 v A second operand of operation.
 * @return float Returns a dot product as a scalar.
 */ 
inline float Vector2::Dot(const Vector2 &v) const
{
	return (this->x() * v.x() + this->y() * v.y());
} // Dot()



/**
 * Method for calc length between two vectors.
 * @param Vector2 v A second operand of operation.
 * @return float Returns a distance between two vectors.
 */ 
inline float Vector2::Length(const Vector2 &v) const
{
	Vector2 dif(v - *this);	
	return sqrt(dif.x()*dif.x() + dif.y()*dif.y());
} // Length()



/**
 * Method for calc vector's length.
 * @return float Returns a length of this vector.
 */
inline float Vector2::Length() const
{
	return sqrt(this->x()*this->x() + this->y()*this->y());
} // Length()



/**
 * Method for calc vector's squared length.
 * @return float Returns a squared length of this vector.
 */
inline float Vector2::SquaredLength() const
{
	return (this->x()*this->x() + this->y()*this->y());
} // SquaredLength()



/**
 * Method for sum all vector's components.
 * @return float Returns a sum of all vector's components.
 */
inline float Vector2::Sum() const
{
	return (this->x() + this->y());
} // Sum()



/**
 * Method for mult all vector's components.
 * @return float Returns a mult of all vector's components.
 */
inline float Vector2::Mult() const
{
	return (this->x() * this->y());
} // Mult()



/**
 * Function for create a new vector with minimal components of two vectors.
 * @param Vector2 v1 A first operand,
 * @param Vector2 v2 A second operand.
 * @return Vector2 Returns a minimal vector.
 */ 
inline Vector2 Min(const Vector2 &v1, const Vector2 &v2)
{
	return Vector2(Min(v1.x(), v2.x()), Min(v1.y(), v2.y()));
} // Min()



/**
 * Function for create a new vector with maximal components of two vectors.
 * @param Vector2 v1 A first operand,
 * @param Vector2 v2 A second operand.
 * @return Vector2 Returns a maximal vector.
 */ 
inline Vector2 Max(const Vector2 &v1, const Vector2 &v2)
{
	return Vector2(Max(v1.x(), v2.x()), Max(v1.y(), v2.y()));
} // Max()



/**
 * Function for create a new vector with absolute all components.
 * @param Vector2 v An operand.
 * @return Vector2 Returns a absolute vector.
 */ 
inline Vector2 Abs(const Vector2 &v)
{
	return Vector2(Abs(v.x()), Abs(v.y()));
} // Abs()



/**
 * Operator for comparation of two vectors.
 * @param Vector2 v1 A first operand,
 * @param Vector2 v2 A second operand.
 * @return bool Return true, if all compoments are equal, false otherwise.
 */ 
inline bool operator==(const Vector2 &v1, const Vector2 &v2) 
{
	return ((v1.x() == v2.x()) && (v1.y() == v2.y()));
} // operator==



/**
 * Operator for comparation of two vectors.
 * @param Vector2 v1 A first operand,
 * @param Vector2 v2 A second operand.
 * @return bool Return true, if one of the compoments are not equal, false otherwise.
 */ 
inline bool operator!=(const Vector2 &v1, const Vector2 &v2)
{
	return ((v1.x() != v2.x()) || (v1.y() != v2.y()));
} // operator!=



/**
 * Operator for addition of two vectors.
 * @param Vector2 v1 A first operand,
 * @param Vector2 v2 A second operand.
 * @return Vector2 Return new vector as a result of additioning.
 */ 
inline Vector2 operator+(const Vector2 &v1, const Vector2 &v2)
{
	return Vector2(v1.x() + v2.x(), v1.y() + v2.y());
} // operator+



/**
 * Operator for addition vector with scalar.
 * @param Vector2 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector2 Return new vector as a result of additioning.
 */ 
inline Vector2 operator+(const Vector2 &v, const float &s)
{
	return Vector2(v.x() + s, v.y() + s);
} // operator+



/**
 * Operator for addition scalar with vector.
 * @param float s A second operand (scalar).
 * @param Vector2 v A first operand (vector),
 * @return Vector2 Return new vector as a result of additioning.
 */ 
inline Vector2 operator+(const float &s, const Vector2 &v)
{
	return Vector2(v.x() + s, v.y() + s);
} // operator+



/**
 * Operator for subtration of two vectors.
 * @param Vector2 v1 A first operand,
 * @param Vector2 v2 A second operand.
 * @return Vector2 Return new vector as a result of subtrationing.
 */ 
inline Vector2 operator-(const Vector2 &v1, const Vector2 &v2)
{
	return Vector2(v1.x() - v2.x(), v1.y() - v2.y());
} // operator-



/**
 * Operator for subtration of vector and scalar.
 * @param Vector2 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector2 Return new vector as a result of subtrationing.
 */ 
inline Vector2 operator-(const Vector2 &v, const float &s)
{
	return Vector2(v.x() - s, v.y() - s);
} // operator-



/**
 * Operator for multiplication of two vectors.
 * @param Vector2 v1 A first operand,
 * @param Vector2 v2 A second operand.
 * @return Vector2 Return new vector as a result of multiplication.
 */ 
inline Vector2 operator*(const Vector2 &v1, const Vector2 &v2)
{
	return Vector2(v1.x() * v2.x(), v1.y() * v2.y());
} // operator*



/**
 * Operator for multiplication of vector and scalar.
 * @param Vector2 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector2 Return new vector as a result of multiplication.
 */ 
inline Vector2 operator*(const Vector2 &v, const float &s)
{
	return Vector2(v.x() * s, v.y() * s);
} // operator*



/**
 * Operator for multiplication of scalar and vector.
 * @param float s A second operand (scalar).
 * @param Vector2 v A first operand (vector),
 * @return Vector2 Return new vector as a result of multiplication.
 */ 
inline Vector2 operator*(const float &s, const Vector2 &v)
{
	return Vector2(v.x() * s, v.y() * s);
} // operator*



/**
 * Operator for division of two vectors.
 * @param Vector2 v1 A first operand,
 * @param Vector2 v2 A second operand.
 * @return Vector2 Return new vector as a result of division.
 */
inline Vector2 operator/(const Vector2 &v1, const Vector2 &v2)
{
	return Vector2(v1.x() / v2.x(), v1.y() / v2.y());
} // operator/	



/**
 * Operator for division of vector and scalar.
 * @param Vector2 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector2 Return new vector as a result of division.
 */ 
inline Vector2 operator/(const Vector2 &v, const float &s)
{
	return Vector2(v.x() / s, v.y() / s);
} // operator/



/**
 * Operator for get the same vector.
 * @return Vector2 Returns new vector, which has same components as the original.
 */ 
inline Vector2 Vector2::operator+() const
{
	return Vector2(this->x(), this->y());
} // operator+



/**
 * Operator for negation of vector's components.
 * @return Vector2 Returns new vector, which has negate components as the original.
 */ 
inline Vector2 Vector2::operator-() const
{
	return Vector2(-this->x(), -this->y());
} // operator-



/**
 * Operator for assignement.
 * @param Vector2 v An original vector (right value).
 * @return Vector2 Return a left value (vector) after assignement.
 */
inline Vector2 &Vector2::operator=(const Vector2 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	return *this;
} // operator=



/**
 * Operator for addition with vector.
 * @param Vector2 v An original vector (right value), which will be added to this.
 * @return Vector2 Return a left value (vector) after addition.
 */
inline Vector2 &Vector2::operator+=(const Vector2 &v)
{
	*this = *this + v;
	return *this;
} // operator+=



/**
 * Operator for addition vector with scalar.
 * @param float s An original vector (right value), which will be added to this.
 * @return Vector2 Return a left value (vector) after addition.
 */
inline Vector2 &Vector2::operator+=(const float &s)
{
	*this = *this + s;
	return *this;
} // operator+=



/**
 * Operator for subtraction with vector.
 * @param Vector2 v An original vector (right value), which will be subtract from this.
 * @return Vector2 Return a left value (vector) after subtraction.
 */
inline Vector2 &Vector2::operator-=(const Vector2 &v)
{
	*this = *this - v;
	return *this;
} // operator-=



/**
 * Operator for subtraction with scalar.
 * @param float s An original vector (right value), which will be subtract from this.
 * @return Vector2 Return a left value (vector) after subtraction.
 */
inline Vector2 &Vector2::operator-=(const float &s)
{
	*this = *this - s;
	return *this;
} // operator-=



/**
 * Operator for multiplication with vector.
 * @param Vector2 v An original vector (right value), which will be mult to this.
 * @return Vector2 Return a left value (vector) after multiplication.
 */
inline Vector2 &Vector2::operator*=(const Vector2 &v)
{
	*this = *this * v;
	return *this;
} // operator*=



/**
 * Operator for multiplication with scalar.
 * @param float s An right scalar value, which will be multiplied with this.
 * @return Vector2 Return a left value (vector) after multiplication.
 */
inline Vector2 &Vector2::operator*=(const float &s)
{
	*this = *this * s;
	return *this;
} // operator*=



/**
 * Operator for division with vector.
 * @param Vector2 v Denominator (right value).
 * @return Vector2 Return a left value (vector) after division.
 */
inline Vector2 &Vector2::operator/=(const Vector2 &v)
{
	*this = *this / v;
	return *this;
} // operator/=



/**
 * Operator for division with scalar.
 * @param float s Denominator (right value). No zero division control!
 * @return Vector2 Return a left value (vector) after division.
 */
inline Vector2 &Vector2::operator/=(const float &s)
{
	*this = *this / s;
	return *this;
} // operator/=



#endif // #ifndef _AFR_VECTOR_2H



/** ***************************************************************************/
/** end of file Vector2.h													  */
/** ***************************************************************************/
