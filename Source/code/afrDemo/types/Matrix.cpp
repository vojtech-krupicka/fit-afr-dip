/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Matrix.cpp														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Matrix.									  *
 **************************************************************************** */


// Including project libraries
#include "Matrix.h"
#include "Vectors.h"



/**
 * Input operator. From input stream read 16 float values and stores into a
 * matrix.
 * @param istream is Input stream,
 * @param Matrix m Input matrix, which will be filled with read values.
 * @return istream Returns input stream for fluent interface.
 */ 
istream &operator>>(istream &is, Matrix &m)
{
	is >> m[0][0] >> m[0][1] >> m[0][2] >> m[0][3]
	   >> m[1][0] >> m[1][1] >> m[1][2] >> m[1][3]
	   >> m[2][0] >> m[2][1] >> m[2][2] >> m[2][3]
	   >> m[3][0] >> m[3][1] >> m[3][2] >> m[3][3];
	return is;
} // operator>>



/**
 * Output operator. Write matrix to output stream.
 * @param ostream os Output stream,
 * @param Matrix m Output matrix, which will be write to stream.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const Matrix &m)
{
	os 	<< "(" 
		<< m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3]
		<< ", " << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3]
		<< ", " << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3]
		<< ", " << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] 
		<< ")";
	return os;
} // operator<<



/**
 * Method for multiply matrix with 4D vector. Returns new 4D vector as result.
 * @param Matrix m Input (left operand) matrix,
 * @param Vector4 v Input (right operand) vector.
 * @return Vector4 Returns new vector as result of multiplication.
 */ 
Vector4 operator*(const Matrix &m, const Vector4 &v)
{
	Vector4 temp;
	
	temp[0] = v.x() * m[0][0] + v.y() * m[1][0] + v.z() * m[2][0] + v.w() * m[3][0];
	temp[1] = v.x() * m[0][1] + v.y() * m[1][1] + v.z() * m[2][1] + v.w() * m[3][1];
	temp[2] = v.x() * m[0][2] + v.y() * m[1][2] + v.z() * m[2][2] + v.w() * m[3][2];
	temp[3] = v.x() * m[0][3] + v.y() * m[1][3] + v.z() * m[2][3] + v.w() * m[3][3];
	
	return temp;
} // operator*



/**
 * Method for multiply matrix with 4D vector. Returns new 4D vector as result.
 * @param Vector4 v Input (left operand) vector.
 * @param Matrix m Input (right operand) matrix,
 * @return Vector4 Returns new vector as result of multiplication.
 */ 
Vector4 operator*(const Vector4 &v, const Matrix &m)
{
	return m*v;
} // operator*



/**
 * Method for multiply matrix with 3D vector. Returns new 4D vector as result.
 * @param Matrix m Input (left operand) matrix,
 * @param Vector3 v Input (right operand) vector.
 * @return Vector3 Returns new vector as result of multiplication.
 */ 
Vector3 operator*(const Matrix &m, const Vector3 &v)
{
	Vector3 temp;
	
	temp[0] = v.x() * m[0][0] + v.y() * m[1][0] + v.z() * m[2][0] + m[3][0];
	temp[1] = v.x() * m[0][1] + v.y() * m[1][1] + v.z() * m[2][1] + m[3][1];
	temp[2] = v.x() * m[0][2] + v.y() * m[1][2] + v.z() * m[2][2] + m[3][2];
	
	return temp;
} // operator*



/**
 * Method for multiply matrix with 3D vector. Returns new 4D vector as result.
 * @param Vector3 v Input (left operand) vector.
 * @param Matrix m Input (right operand) matrix,
 * @return Vector3 Returns new vector as result of multiplication.
 */ 
Vector3 operator*(const Vector3 &v, const Matrix &m)
{
	return m * v;
} // operator*



/**
 * Method for multiply matrix with 2D vector. Returns new 4D vector as result.
 * @param Matrix m Input (left operand) matrix,
 * @param Vector2 v Input (right operand) vector.
 * @return Vector2 Returns new vector as result of multiplication.
 */ 
Vector2 operator*(const Matrix &m, const Vector2 &v)
{
	Vector2 temp;
	
	temp[0] = v.x() * m[0][0] + v.y() * m[1][0];
	temp[1] = v.x() * m[0][1] + v.y() * m[1][1];
	
	return temp;
} // operator*



/**
 * Method for multiply matrix with 2D vector. Returns new 4D vector as result.
 * @param Vector2 v Input (left operand) vector.
 * @param Matrix m Input (right operand) matrix,
 * @return Vector2 Returns new vector as result of multiplication.
 */ 
Vector2 operator*(const Vector2 &v, const Matrix &m)
{
	return m*v;
} // operator*



/**
 * Method for transposing matrix. Returns new transposed matrix.
 * @return Matrix Returns new transposed matrix.
 */ 
Matrix Matrix::Transpose() const
{
	Matrix temp;
	
	for(unsigned i = 0; i < 4; i++) {
		for(unsigned j = 0; j < 4; j++) {
			temp[j][i] = this->data[i][j];
		}
	}
	
	return temp;
} // Transpose()



/**
 * Static method for creating default matrix. All values are zeros.
 * @return Matrix Retunrs zero matrix.
 */ 
Matrix Matrix::Zeros()
{
	float temp[] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	return Matrix(temp);
} // Zeros()



/**
 * Static method for creating default matrix. All values are ones.
 * @return Matrix Retunrs one matrix.
 */ 
Matrix Matrix::Ones()
{
	float temp[] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
	return Matrix(temp);
} // Ones()



/**
 * Static method for creating default matrix. All values are zeros. Values at
 * the diagonal will be one - identity matrix.
 * @return Matrix Retunrs identity matrix.
 */ 
Matrix Matrix::Identity()
{
	float temp[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	return Matrix(temp);
} // Identity()



/** ***************************************************************************/
/** end of file Matrix.cpp													  */
/** ***************************************************************************/
