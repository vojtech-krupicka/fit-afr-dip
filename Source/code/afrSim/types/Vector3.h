/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Vector3.h														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Vector3.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_VECTOR_3_H
#define _AFR_VECTOR_3_H


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
class Vector4;
class Matrix;


/**
 * Class Vector3
 * @brief Class for three components vector for store 3D space vectors.
 */ 
class Vector3
{
public:
	///< Friendly classes
	friend class Vector2;
	friend class Vector4;
	friend class Matrix;

	///< Implicit constructors
	inline Vector3();
	inline Vector3(const float data[3]);
	inline Vector3(const float &x, const float &y, const float &z);
	inline Vector3(const Vector3 &v);
	Vector3(const Vector2 &v);
	Vector3(const Vector4 &v);
	
	///< Implicit destructors
	~Vector3() {};
	
	///< Accessors methods
	inline float x() const { return this->data[0]; };
	inline float y() const { return this->data[1]; };
	inline float z() const { return this->data[2]; };
	
	inline float operator[]	(const unsigned &i) const { return this->data[i]; };
	inline float &operator[] (const unsigned &i) { return this->data[i]; };
	
	///< Setters
	inline Vector3& operator() (const float &x, const float &y, const float &z);
	inline Vector3& x(const float &f) { this->data[0] = f; return *this; };
	inline Vector3& y(const float &f) { this->data[1] = f; return *this; };
	inline Vector3& z(const float &f) { this->data[2] = f; return *this; };
	
	///< Boolean operators
	friend bool operator==(const Vector3 &v1, const Vector3 &v2);
	friend bool operator!=(const Vector3 &v1, const Vector3 &v2);
	
	///< IO operators
	friend istream &operator>>(istream &is, Vector3 &v);
	friend ostream &operator<<(ostream &os, const Vector3 &v);
	
	///< Basic operators
	friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator+(const Vector3 &v, const float &s);
	friend Vector3 operator+(const float &s, const Vector3 &v);
	friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator-(const Vector3 &v, const float &s);
	friend Vector3 operator*(const Vector3 &v, const Vector3 &v2);
	friend Vector3 operator*(const Vector3 &v, const float &s);
	friend Vector3 operator*(const float &s, const Vector3 &v);
	friend Vector3 operator/(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator/(const Vector3 &v, const float &s);
	
	Vector3 operator+() const;
	Vector3 operator-() const;
	
	Vector3 &operator=(const Vector3 &v);
	Vector3 &operator+=(const Vector3 &v);
	Vector3 &operator+=(const float &s);
	Vector3 &operator-=(const Vector3 &v);
	Vector3 &operator-=(const float &s);
	Vector3 &operator*=(const Vector3 &v);
	Vector3 &operator*=(const float &s);
	Vector3 &operator*=(const Matrix &m);
	Vector3 &operator/=(const Vector3 &v);
	Vector3 &operator/=(const float &s);
	
	///< Vector operations
	inline float MinComponent() const;
	inline float MaxComponent() const;
	inline float MinAbsComponent() const;
	inline float MaxAbsComponent() const;
	
	inline unsigned MinComponentIndex() const;
	inline unsigned MaxComponentIndex() const;
	inline unsigned MinAbsComponentIndex() const;
	inline unsigned MaxAbsComponentIndex() const;
	
	Vector3 Normalize() const;
	Vector3 Cross(const Vector3 &v) const;
	float 	Dot(const Vector3 &v) const;
	float 	Length(const Vector3 &v) const;
	float 	Length() const;
	float 	SquaredLength() const;	
	float	Sum() const;
	float	Mult() const;
	
	friend Vector3 Min(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 Max(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 Abs(const Vector3 &v);
	
	///< Static methods, used for creating common vectors
	static Vector3 Zero() { return Vector3(0.0, 0.0, 0.0); };
	static Vector3 One() { return Vector3(1.0, 1.0, 1.0); };
	static Vector3 XAxis() { return Vector3(1.0, 0.0, 0.0); };
	static Vector3 YAxis() { return Vector3(0.0, 1.0, 0.0); };
	static Vector3 ZAxis() { return Vector3(0.0, 0.0, 1.0); };
	
private:
	float data[3];
}; // class Vector3



/**
 * Implicit constructor. All components are zero.
 * @return Vector3.
 */ 
inline Vector3::Vector3()
{
	this->data[0] = 0.0;
	this->data[1] = 0.0;
	this->data[2] = 0.0;
} // Vector3()



/**
 * Implicit constructor from three-link float array.
 * @param float data[3] Three-link float array.
 * @return Vector3.
 */ 
inline Vector3::Vector3(const float data[3])
{
	this->data[0] = data[0];
	this->data[1] = data[1];
	this->data[2] = data[2];
} // Vector3()



/**
 * Implicit constructor from four floats.
 * @param float x First vector's component,
 * @param float y Second vector's component,
 * @param float z Third vector's component.
 * @return Vector3.
 */ 
inline Vector3::Vector3(const float &x, const float &y, const float &z)
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
} // Vector3()



/**
 * Copy contructor. Creates a copy of the vector 'v'.
 * @param Vector3 v Original vector, which will be copied.
 * @return Vector3.
 */ 
inline Vector3::Vector3(const Vector3 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
} // Vector3()



/**
 * Setter operator, which sets all components to vector.
 * @param float x First vector's component,
 * @param float y Second vector's component,
 * @param float z Third vector's component.
 * @return Vector3.
 */ 
inline Vector3 &Vector3::operator() (const float &x, const float &y, const float &z)
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
	return *this;
} // operator()



/**
 * Method for getting min component from vector.
 * @return float Returs the minimal vector's component.
 */ 
inline float Vector3::MinComponent() const
{
	return Min(this->x(), this->y(), this->z());
} // MinComponent()



/**
 * Method for getting max component from vector.
 * @return float Returs the maximal vector's component.
 */ 
inline float Vector3::MaxComponent() const
{
	return Max(this->x(), this->y(), this->z());
} // MaxComponent()



/**
 * Method for getting min absolut component from vector.
 * @return float Returs the minimal absolut vector's component.
 */ 
inline float Vector3::MinAbsComponent() const
{
	return Min(Abs(this->x()), Abs(this->y()), Abs(this->z()));
} // MinAbsComponent()



/**
 * Method for getting max absolut component from vector.
 * @return float Returs the maximal absolut vector's component.
 */ 
inline float Vector3::MaxAbsComponent() const
{
	return Max(Abs(this->x()), Abs(this->y()), Abs(this->z()));
} // MaxAbsComponent()



/**
 * Method for getting an index of min component of vector.
 * @return unsigned Returns the index with minimal value.
 */ 
inline unsigned Vector3::MinComponentIndex() const
{
	unsigned index = 0;
	float temp = this->data[0];
	
	if(this->data[1] < temp) {
		temp = this->data[1];
		index = 1;
	}
	if(this->data[2] < temp) {
		index = 2;
	}
	
	return index;
} // MinComponentIndex()



/**
 * Method for getting an index of max component of vector.
 * @return unsigned Returns the index with maximal value.
 */ 
inline unsigned Vector3::MaxComponentIndex() const
{
	unsigned index = 0;
	float temp = this->data[0];
	
	if(this->data[1] > temp) {
		temp = this->data[1];
		index = 1;
	}
	if(this->data[2] > temp) {
		index = 2;
	}
	
	return index;
} // MaxComponentIndex()



/**
 * Method for getting an index of min absolut component of vector.
 * @return unsigned Returns the index with minimal absolut value.
 */ 
inline unsigned Vector3::MinAbsComponentIndex() const
{
	unsigned index = 0;
	float temp = Abs(this->data[0]);
	
	if(Abs(this->data[1]) < temp) {
		temp = Abs(this->data[1]);
		index = 1;
	}
	if(Abs(this->data[2]) < temp) {
		index = 2;
	}
	
	return index;
} // MinAbsComponentIndex()



/**
 * Method for getting an index of max component of vector.
 * @return unsigned Returns the index with maximal value.
 */ 
inline unsigned Vector3::MaxAbsComponentIndex() const
{
	unsigned index = 0;
	float temp = Abs(this->data[0]);
	
	if(Abs(this->data[1]) > temp) {
		temp = Abs(this->data[1]);
		index = 1;
	}
	if(Abs(this->data[2]) > temp) {
		index = 2;
	}
	
	return index;
} // MaxAbsComponentIndex()



/**
 * Method for normalizating vetor. Creates a new unit vector from vector. All
 * components are in range 0 .. 1.
 * @return Vector3 Returns new unit vector.
 */ 
inline Vector3 Vector3::Normalize() const
{
	float len = this->Length();
	return Vector3(this->x()/len, this->y()/len, this->z()/len);
} // Normalize()



/**
 * Method for cross product of two vectors.
 * @param Vector3 v A second operand of operation.
 * @return Vector3 Returns a result of cross product as a new vector.
 */ 
inline Vector3 Vector3::Cross(const Vector3 &v) const
{
	return Vector3( this->y() * v.z() - this->z() * v.y(), 
					this->z() * v.x() - this->x() * v.z(), 
					this->x() * v.y() - this->y() * v.x());
} // Cross()



/**
 * Method for dot product of two vectors.
 * @param Vector3 v A second operand of operation.
 * @return float Returns a dot product as a scalar.
 */ 
inline float Vector3::Dot(const Vector3 &v) const
{
	return (this->x() * v.x() + this->y() * v.y() + this->z() * v.z());
} // Dot()



/**
 * Method for calc length between two vectors.
 * @param Vector3 v A second operand of operation.
 * @return float Returns a distance between two vectors.
 */ 
inline float Vector3::Length(const Vector3 &v) const
{
	Vector3 dif(v - *this);	
	return sqrt(dif.x()*dif.x() + dif.y()*dif.y() + dif.z()*dif.z());
} // Length()



/**
 * Method for calc vector's length.
 * @return float Returns a length of this vector.
 */
inline float Vector3::Length() const
{
	return sqrt(this->x()*this->x() + this->y()*this->y() + this->z()*this->z());
} // Length()



/**
 * Method for calc vector's squared length.
 * @return float Returns a squared length of this vector.
 */
inline float Vector3::SquaredLength() const
{
	return (this->x()*this->x() + this->y()*this->y() + this->z()*this->z());
} // SquaredLength()



/**
 * Method for sum all vector's components.
 * @return float Returns a sum of all vector's components.
 */
inline float Vector3::Sum() const
{
	return (this->x() + this->y() + this->z());
} // Sum()



/**
 * Method for mult all vector's components.
 * @return float Returns a mult of all vector's components.
 */
inline float Vector3::Mult() const
{
	return (this->x() * this->y() * this->z());
} // Mult()



/**
 * Function for create a new vector with minimal components of two vectors.
 * @param Vector3 v1 A first operand,
 * @param Vector3 v2 A second operand.
 * @return Vector3 Returns a minimal vector.
 */ 
inline Vector3 Min(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(Min(v1.x(), v2.x()), Min(v1.y(), v2.y()), Min(v1.z(), v2.z()));
} // Min()



/**
 * Function for create a new vector with maximal components of two vectors.
 * @param Vector3 v1 A first operand,
 * @param Vector3 v2 A second operand.
 * @return Vector3 Returns a maximal vector.
 */ 
inline Vector3 Max(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(Max(v1.x(), v2.x()), Max(v1.y(), v2.y()), Max(v1.z(), v2.z()));
} // Max()



/**
 * Function for create a new vector with absolute all components.
 * @param Vector3 v An operand.
 * @return Vector3 Returns a absolute vector.
 */ 
inline Vector3 Abs(const Vector3 &v)
{
	return Vector3(Abs(v.x()), Abs(v.y()), Abs(v.z()));
} // Abs()



/**
 * Operator for comparation of two vectors.
 * @param Vector3 v1 A first operand,
 * @param Vector3 v2 A second operand.
 * @return bool Return true, if all compoments are equal, false otherwise.
 */ 
inline bool operator==(const Vector3 &v1, const Vector3 &v2) 
{
	return ((v1.x() == v2.x()) && (v1.y() == v2.y()) && (v1.z() == v2.z()));
} // operator==



/**
 * Operator for comparation of two vectors.
 * @param Vector3 v1 A first operand,
 * @param Vector3 v2 A second operand.
 * @return bool Return true, if one of the compoments are not equal, false otherwise.
 */ 
inline bool operator!=(const Vector3 &v1, const Vector3 &v2)
{
	return ((v1.x() != v2.x()) || (v1.y() != v2.y()) || (v1.z() != v2.z()));
} // operator!=



/**
 * Operator for addition of two vectors.
 * @param Vector3 v1 A first operand,
 * @param Vector3 v2 A second operand.
 * @return Vector3 Return new vector as a result of additioning.
 */ 
inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
} // operator+



/**
 * Operator for addition vector with scalar.
 * @param Vector3 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector3 Return new vector as a result of additioning.
 */ 
inline Vector3 operator+(const Vector3 &v, const float &s)
{
	return Vector3(v.x() + s, v.y() + s, v.z() + s);
} // operator+



/**
 * Operator for addition scalar with vector.
 * @param float s A second operand (scalar).
 * @param Vector3 v A first operand (vector),
 * @return Vector3 Return new vector as a result of additioning.
 */ 
inline Vector3 operator+(const float &s, const Vector3 &v)
{
	return Vector3(v.x() + s, v.y() + s, v.z() + s);
} // operator+



/**
 * Operator for subtration of two vectors.
 * @param Vector3 v1 A first operand,
 * @param Vector3 v2 A second operand.
 * @return Vector3 Return new vector as a result of subtrationing.
 */ 
inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
} // operator-



/**
 * Operator for subtration of vector and scalar.
 * @param Vector3 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector3 Return new vector as a result of subtrationing.
 */ 
inline Vector3 operator-(const Vector3 &v, const float &s)
{
	return Vector3(v.x() - s, v.y() - s, v.z() - s);
} // operator-



/**
 * Operator for multiplication of two vectors.
 * @param Vector3 v1 A first operand,
 * @param Vector3 v2 A second operand.
 * @return Vector3 Return new vector as a result of multiplication.
 */ 
inline Vector3 operator*(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
} // operator*



/**
 * Operator for multiplication of vector and scalar.
 * @param Vector3 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector3 Return new vector as a result of multiplication.
 */ 
inline Vector3 operator*(const Vector3 &v, const float &s)
{
	return Vector3(v.x() * s, v.y() * s, v.z() * s);
} // operator*



/**
 * Operator for multiplication of scalar and vector.
 * @param float s A second operand (scalar).
 * @param Vector3 v A first operand (vector),
 * @return Vector3 Return new vector as a result of multiplication.
 */ 
inline Vector3 operator*(const float &s, const Vector3 &v)
{
	return Vector3(v.x() * s, v.y() * s, v.z() * s);
} // operator*



/**
 * Operator for division of two vectors.
 * @param Vector3 v1 A first operand,
 * @param Vector3 v2 A second operand.
 * @return Vector3 Return new vector as a result of division.
 */
inline Vector3 operator/(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
} // operator/	



/**
 * Operator for division of vector and scalar.
 * @param Vector3 v A first operand (vector),
 * @param float s A second operand (scalar).
 * @return Vector3 Return new vector as a result of division.
 */ 
inline Vector3 operator/(const Vector3 &v, const float &s)
{
	return Vector3(v.x() / s, v.y() / s, v.z() / s);
} // operator/



/**
 * Operator for get the same vector.
 * @return Vector3 Returns new vector, which has same components as the original.
 */ 
inline Vector3 Vector3::operator+() const
{
	return Vector3(this->x(), this->y(), this->z());
} // operator+



/**
 * Operator for negation of vector's components.
 * @return Vector3 Returns new vector, which has negate components as the original.
 */ 
inline Vector3 Vector3::operator-() const
{
	return Vector3(-this->x(), -this->y(), -this->z());
} // operator-



/**
 * Operator for assignement.
 * @param Vector3 v An original vector (right value).
 * @return Vector3 Return a left value (vector) after assignement.
 */
inline Vector3 &Vector3::operator=(const Vector3 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
	return *this;
} // operator=



/**
 * Operator for addition with vector.
 * @param Vector3 v An original vector (right value), which will be added to this.
 * @return Vector3 Return a left value (vector) after addition.
 */
inline Vector3 &Vector3::operator+=(const Vector3 &v)
{
	*this = *this + v;
	return *this;
} // operator+=



/**
 * Operator for addition vector with scalar.
 * @param float s An original vector (right value), which will be added to this.
 * @return Vector3 Return a left value (vector) after addition.
 */
inline Vector3 &Vector3::operator+=(const float &s)
{
	*this = *this + s;
	return *this;
} // operator+=



/**
 * Operator for subtraction with vector.
 * @param Vector3 v An original vector (right value), which will be subtract from this.
 * @return Vector3 Return a left value (vector) after subtraction.
 */
inline Vector3 &Vector3::operator-=(const Vector3 &v)
{
	*this = *this - v;
	return *this;
} // operator-=



/**
 * Operator for subtraction with scalar.
 * @param float s An original vector (right value), which will be subtract from this.
 * @return Vector3 Return a left value (vector) after subtraction.
 */
inline Vector3 &Vector3::operator-=(const float &s)
{
	*this = *this - s;
	return *this;
} // operator-=



/**
 * Operator for multiplication with vector.
 * @param Vector3 v An original vector (right value), which will be mult to this.
 * @return Vector3 Return a left value (vector) after multiplication.
 */
inline Vector3 &Vector3::operator*=(const Vector3 &v)
{
	*this = *this * v;
	return *this;
} // operator*=



/**
 * Operator for multiplication with scalar.
 * @param float s An right scalar value, which will be multiplied with this.
 * @return Vector3 Return a left value (vector) after multiplication.
 */
inline Vector3 &Vector3::operator*=(const float &s)
{
	*this = *this * s;
	return *this;
} // operator*=



/**
 * Operator for division with vector.
 * @param Vector3 v Denominator (right value).
 * @return Vector3 Return a left value (vector) after division.
 */
inline Vector3 &Vector3::operator/=(const Vector3 &v)
{
	*this = *this / v;
	return *this;
} // operator/=



/**
 * Operator for division with scalar.
 * @param float s Denominator (right value). No zero division control!
 * @return Vector3 Return a left value (vector) after division.
 */
inline Vector3 &Vector3::operator/=(const float &s)
{
	*this = *this / s;
	return *this;
} // operator/=



#endif // #ifndef _AFR_VECTOR_3H



/** ***************************************************************************/
/** end of file Vector3.h													  */
/** ***************************************************************************/
