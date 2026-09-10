/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Matrix.h														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Matrix.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_MATRIX_H
#define _AFR_MATRIX_H


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
class Vector4;


/**
 * Class Matrix
 * @brief Class for representation of 4x4 matrix for transformations 
 * (translation, rotation and scale).
 */ 
class Matrix
{
public:
	///< Friendly classes
	friend class Vector2;
	friend class Vector3;
	friend class Vector4;

	///< Implicit constructors
	inline Matrix();
	inline Matrix(const float data[16]);
	inline Matrix(const Matrix &m);
	
	///< Implicit destructors
	~Matrix() {};
	
	///< Accessors methods
	inline float *x() { return this->data[0]; };
	inline float *y() { return this->data[1]; };
	inline float *z() { return this->data[2]; };
	inline float *w() { return this->data[3]; };
	
	inline float *operator[] (const unsigned &i) const { return (float*)this->data[i]; };

	///< Boolean operators
	friend bool operator==(const Matrix &m1, const Matrix &m2);
	friend bool operator!=(const Matrix &m1, const Matrix &m2);
	
	///< IO operators
	friend istream &operator>>(istream &is, Matrix &m);
	friend ostream &operator<<(ostream &os, const Matrix &m);
	
	///< Basic operators
	friend Matrix operator+(const Matrix &m1, const Matrix &m2);
	friend Matrix operator+(const Matrix &m, const float &s);
	friend Matrix operator+(const float &s, const Matrix &m);	
	friend Matrix operator-(const Matrix &m1, const Matrix &m2);
	friend Matrix operator-(const Matrix &m, const float &s);	
	friend Matrix operator*(const Matrix &m1, const Matrix &m2);
	friend Matrix operator*(const Matrix &m, const float &s);
	friend Matrix operator*(const float &s, const Matrix &m);
	friend Vector4 operator*(const Matrix &m, const Vector4 &v);
	friend Vector4 operator*(const Vector4 &v, const Matrix &m);
	friend Vector3 operator*(const Matrix &m, const Vector3 &v);
	friend Vector3 operator*(const Vector3 &v, const Matrix &m);
	friend Vector2 operator*(const Matrix &m, const Vector2 &v);
	friend Vector2 operator*(const Vector2 &v, const Matrix &m);	
	friend Matrix operator/(const Matrix &m, const float &s);
	
	Matrix operator+() const;
	Matrix operator-() const;
	
	Matrix &operator=(const Matrix &m);
	Matrix &operator+=(const Matrix &m);
	Matrix &operator+=(const float &s);
	Matrix &operator-=(const Matrix &m);
	Matrix &operator-=(const float &s);
	
	Matrix &operator*=(const Matrix &m);
	Matrix &operator*=(const float &s);
	Matrix &operator/=(const float &s);
	
	///< Matrix operations
	Matrix Transpose() const;
	
	///< Static methods, used for creating common matrices
	static Matrix Zeros();
	static Matrix Ones();
	static Matrix Identity();
	
private:
	float data[4][4];
}; // class Matrix



/**
 * Implicit constructor. Creates an identity matrix.
 * @return Matrix.
 */ 
inline Matrix::Matrix()
{
	*this = Matrix::Identity();
} // Matrix()



/**
 * Implicit constructor. Creates a matrix from 16-link array.
 * @param float data A 16-link input array.
 * @return Matrix.
 */ 
inline Matrix::Matrix(const float data[16])
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			this->data[i][j] = data[i*4 + j];
		}
	}
} // Matrix()



/**
 * Copy constructor. Creates a matrix from an original matrix.
 * @param Matrix m Original matrix.
 * @return Matrix.
 */ 
inline Matrix::Matrix(const Matrix &m)
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			this->data[i][j] = m[i][j];
		}
	}
} // Matrix()



/**
 * Operator for comparation of two matrices.
 * @param Matrix m1 A first operand,
 * @param Matrix m2 A second operand.
 * @return bool Return true, if all compoments are equal, false otherwise.
 */
inline bool operator==(const Matrix &m1, const Matrix &m2)
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(m1[i][j] != m2[i][j]) {
				return false;
			}
		}
	}
	
	return true; 
} // operator==



/**
 * Operator for comparation of two matrices.
 * @param Matrix m1 A first operand,
 * @param Matrix m2 A second operand.
 * @return bool Return true, if one of the compoments are not equal, false otherwise.
 */ 
inline bool operator!=(const Matrix &m1, const Matrix &m2)
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(m1[i][j] != m2[i][j]) {
				return true;
			}
		}
	}
	
	return false;
} // operator!=
	
	

/**
 * Operator for addition of two matrices.
 * @param Matrix m1 A first operand,
 * @param Matrix m2 A second operand.
 * @return Matrix Return new matrix as a result of additioning.
 */ 
inline Matrix operator+(const Matrix &m1, const Matrix &m2)
{
	Matrix temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = m2[i][j] + m1[i][j];
		}
	}
	
	return temp;
} // operator+



/**
 * Operator for addition of matrix and scalar.
 * @param Matrix m1 A first operand,
 * @param float s A second operand.
 * @return Matrix Return new matrix as a result of additioning.
 */
inline Matrix operator+(const Matrix &m, const float &s)
{
	Matrix temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = m[i][j] + s;
		}
	}
	
	return temp;
} // operator+



/**
 * Operator for addition of scalar and matrix.
 * @param float s A first operand,
 * @param Matrix m1 A second operand.
 * @return Matrix Return new matrix as a result of additioning.
 */
inline Matrix operator+(const float &s, const Matrix &m)
{
	Matrix temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = m[i][j] + s;
		}
	}
	
	return temp;
} // operator+



/**
 * Operator for subtration of two matrices.
 * @param Matrix m1 A first operand,
 * @param Matrix m2 A second operand.
 * @return Matrix Return new matrix as a result of subtrationing.
 */ 
inline Matrix operator-(const Matrix &m1, const Matrix &m2)
{
	Matrix temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = m1[i][j] - m2[i][j];
		}
	}
	
	return temp;
} // operator-
 


/**
 * Operator for subtration of matrix and scalar.
 * @param Matrix m1 A first operand,
 * @param float s A second operand.
 * @return Matrix Return new matrix as a result of subtrationing.
 */
inline Matrix operator-(const Matrix &m, const float &s)
{
	Matrix temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = m[i][j] - s;
		}
	}
	
	return temp;
} // operator-



/**
 * Operator for multiplication of two matrices.
 * @param Matrix m1 A first operand,
 * @param Matrix m2 A second operand.
 * @return Matrix Return new matrix as a result of multiplication.
 */ 
inline Matrix operator*(const Matrix &m1, const Matrix &m2)
{
	Matrix temp;
	
	for(unsigned i = 0; i < 4; i++) {
		for(unsigned j = 0; j < 4; j++) {
			temp[i][j] = 0;
			for(unsigned k = 0; k < 4; k++) {
				temp[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	
	return temp;
} // operator*



/**
 * Operator for multiplication of matrix and scalar.
 * @param Matrix m1 A first operand,
 * @param float s A second operand.
 * @return Matrix Return new matrix as a result of multiplication.
 */ 
inline Matrix operator*(const Matrix &m, const float &s)
{
	Matrix temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = m[i][j] * s;
		}
	}
	
	return temp;
} // operator*



/**
 * Operator for multiplication of scalar and matrix. 
 * @param float s A first operand,
 * @param Matrix m1 A second operand.
 * @return Matrix Return new matrix as a result of multiplication.
 */ 
inline Matrix operator*(const float &s, const Matrix &m)
{
	Matrix temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = m[i][j] * s;
		}
	}
	
	return temp;
} // operator*



/**
 * Operator for division of matrix and scalar.
 * @param Matrix m1 A first operand,
 * @param float s A second operand.
 * @return Matrix Return new matrix as a result of division.
 */
inline Matrix operator/(const Matrix &m, const float &s)
{
	Matrix temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = m[i][j] / s;
		}
	}
	
	return temp;
} // operator/
	
	
	
/**
 * Operator for get the same vector (copy operator).
 * @return Matrix Returns new matrix, which has same components as the original.
 */ 
inline Matrix Matrix::operator+() const
{
	return Matrix(*this);
} // operator+



/**
 * Operator for negation of matrix's components.
 * @return Matrix Returns new matrix, which has negate components as the original.
 */ 
inline Matrix Matrix::operator-() const
{
	Matrix temp(*this);
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[i][j] = -(temp[i][j]);
		}
	}
	
	return temp;
} // operator-



/**
 * Operator for assignement.
 * @param Matrix v An original matrix (right value).
 * @return Matrix Return a left value (matrix) after assignement.
 */
inline Matrix &Matrix::operator=(const Matrix &m)
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			this->data[i][j] = m[i][j];
		}
	}
	return *this;
} // operator=



/**
 * Operator for addition with matrix.
 * @param Matrix v An original matrix (right value), which will be added to this.
 * @return Matrix Return a left value (matrix) after addition.
 */
inline Matrix &Matrix::operator+=(const Matrix &m)
{
	*this = *this + m;
	return *this;
} // operator+=



/**
 * Operator for addition matrix with scalar.
 * @param float s An original matrix (right value), which will be added to this.
 * @return Matrix Return a left value (matrix) after addition.
 */
inline Matrix &Matrix::operator+=(const float &s)
{
	*this = *this + s;
	return *this;
} // operator+=



/**
 * Operator for subtraction with matrix.
 * @param Matrix v An original matrix (right value), which will be subtract from this.
 * @return Matrix Return a left value (matrix) after subtraction.
 */
inline Matrix &Matrix::operator-=(const Matrix &m)
{
	*this = *this - m;
	return *this;
} // operator-=



/**
 * Operator for subtraction with scalar.
 * @param float s An original matrix (right value), which will be subtract from this.
 * @return Matrix Return a left value (matrix) after subtraction.
 */
inline Matrix &Matrix::operator-=(const float &s)
{
	*this = *this - s;
	return *this;
} // operator-=



/**
 * Operator for multiplication with matrix.
 * @param Matrix v An original matrix (right value), which will be mult to this.
 * @return Matrix Return a left value (matrix) after multiplication.
 */
inline Matrix &Matrix::operator*=(const Matrix &m)
{
	*this = *this * m;
	return *this;
} // operator*=



/**
 * Operator for multiplication with scalar.
 * @param float s An right scalar value, which will be multiplied with this.
 * @return Matrix Return a left value (matrix) after multiplication.
 */
inline Matrix &Matrix::operator*=(const float &s)
{
	*this = *this * s;
	return *this;
} // operator*=



/**
 * Operator for division with scalar.
 * @param float s Denominator (right value). No zero division control!
 * @return Matrix Return a left value (matrix) after division.
 */
inline Matrix &Matrix::operator/=(const float &s)
{
	*this = *this / s;
	return *this;
} // operator/=



#endif // #ifndef _AFR_MATRIX_H



/** ***************************************************************************/
/** end of file Matrix.h													  */
/** ***************************************************************************/
