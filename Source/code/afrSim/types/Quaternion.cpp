/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Quaternion.cpp													  *	
 * @date	10. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Quaternion.								  *
 **************************************************************************** */


// Including project libraries
#include "Vector4.h"
#include "Matrix.h"
#include "Quaternion.h"



Quaternion::Quaternion(const Vector4 &v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
	this->data[3] = v[3];
} // Quaternion()



/**
 * Input operator. From input stream read four float values and stores into a
 * quaternion to each quaternion's component.
 * @param istream is Input stream,
 * @param Quaternion v Input quaternion, which will be filled with read values.
 * @return istream Returns input stream for fluent interface.
 */ 
istream &operator>>(istream &is, Quaternion &q)
{
	is >> q[0] >> q[1] >> q[2] >> q[3];
	return is;
} // operator>>



/**
 * Output operator. Write all four quaternion's component to output stream.
 * @param ostream os Output stream,
 * @param Quaternion v Output quaternion, which will be write to stream.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const Quaternion &q)
{
	os << "(" << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << ")";
	return os;
} // operator<<



/**
 * Operator for 'FromRotationMatrix' operation. From specified matrix creates
 * new quaternion a fill its components. Function returns the original matrix.
 * This behavior is same as in istream operator >>.
 * @param Matrix m Matrix from which the quaternion will be create.
 * @param Quaternion q Quaternion which will be created.
 * @return Matrix Returns input matrix for fluent interface.
 */ 
Matrix operator>>(Matrix &m, Quaternion &q)
{
	static int next[3] = {1, 2, 0};
	float trace = m[0][0] + m[1][1] + m[2][2];
	float root;
    float *quat[3];
    int i, j, k;
	
	if(trace > 0.0) {
        root = sqrt(trace + 1.0);
        q.w(0.5 * root);
        root = 0.5 / root;
        q.x((m[2][1] - m[1][2]) * root).y((m[0][2] - m[2][0]) * root).z((m[1][0] - m[0][1]) * root);
    }
    else
    {
        i = 0;
        if(m[1][1] > m[0][0]) {
            i = 1;
		}
        if(m[2][2] > m[i][i]) {
            i = 2;
		}
        j = next[i];
        k = next[j];

        root = sqrt(m[i][i] - m[j][j] - m[k][k] + 1.0);
		
		*quat[0] = q.x();
		*quat[1] = q.y(); 
		*quat[2] = q.z();
        *quat[i] = 0.5*root;
		
        root = 0.5/root;
		
        q.w((m[k][j] - m[j][k]) * root);
        *quat[j] = (m[j][i] + m[i][j]) * root;
        *quat[k] = (m[k][i] + m[i][k]) * root;
    }
	
	return m;
} // operator>>



/**
 * Operator for 'ToRotationMatrix' operation. From quaternion creates a new 
 * rotation matrix. Returns matrix as in the ostream operator <<.
 * @param Matrix m Matrix which will be created.
 * @param Quaternion q Quaternion from which the matrix will be create
 * @return Matrix Returns output matrix for fluent interface.
 */ 
Matrix operator<<(Matrix &m, const Quaternion &q)
{
	float x2 = 2.0f * q.x(); 
	float y2 = 2.0f * q.y(); 
	float z2 = 2.0f * q.z();

	float xx = q.x() * x2;   
	float xy = q.x() * y2;   
	float xz = q.x() * z2;
	
	float yy = q.y() * y2;   
	float yz = q.y() * z2;   
	float zz = q.z() * z2;
	
	float wx = q.w() * x2;   
	float wy = q.w() * y2;   
	float wz = q.w() * z2;

	m[0][0] = 1.0 - (yy + zz); 
	m[0][1] = xy - wz;
	m[0][2] = xz + wy;
	m[0][3] = 0.0;

	m[1][0] = xy + wz;
	m[1][1] = 1.0 - (xx + zz);
	m[1][2] = yz - wx;
	m[1][3] = 0.0;

	m[2][0] = xz - wy;
	m[2][1] = yz + wx;
	m[2][2] = 1.0 - (xx + yy);
	m[2][3] = 0.0;

	m[3][0] = 0; 
	m[3][1] = 0;
	m[3][2] = 0;  
	m[3][3] = 1;
	
	return m;
} // operator<<



/**
 * Operator for 'FromAngleAxis' operation. From vector creates a new 
 * quaternion. Returns vector as in the istream operator >>.
 * @param Vector4 v Vector from which the quaternion will be create.
 * @param Quaternion q Quaternion which will be created.
 * @return Vector4 Returns input vector for fluent interface.
 */ 
Vector4 operator>>(Vector4 &v, Quaternion &q)
{
	float halfAngle = 0.5 * v.w();
    float sn = sin(halfAngle);
	Vector4 vn = v.Normalize();
	
    q.w(cos(halfAngle)).x(sn * vn.x()).y(sn * vn.y()).z(sn * vn.z());	 
	return v;
} // operator>>



/**
 * Operator for 'ToAngleAxis' operation. From quaternion creates a new 
 * look at vector. Returns vector as in the ostream operator <<.
 * @param Vector4 v Vector which will be created.
 * @param Quaternion q Quaternion from which the matrix will be create
 * @return Vector Returns output vector for fluent interface.
 */
Vector4 operator<<(Vector4 &v, const Quaternion &q)
{
	float invlen;
    float length2 = q.SquaredAngle();
    if(length2 > 0.0) {
		invlen = 1.0 / sqrt(length2);
        v.x(q.x() * invlen).y(q.y() * invlen).z(q.z() * invlen).w(2.0 * acos(q.w()));
    }
    else
    {
		v.x(1.0).y(0.0).z(0.0).w(0.0);
    }
	
	return v;
} // operator<<



/**
 * Method for make SQUAD from four quaternions in time.
 * @param float time Specified time,
 * @param Quaternion p First quaternion,
 * @param Quaternion a Second quaternion,
 * @param Quaternion b Third quaternion,
 * @param Quaternion q Fourth quaternion,
 * @return Quaternion New quaternion as a result of Squad.
 */ 
Quaternion Quaternion::Squad(const float &time, Quaternion &p, Quaternion &a, Quaternion &b, Quaternion &q)
{
    Quaternion q1 = Quaternion::Slerp(time, p, q);
	Quaternion q2 = Quaternion::Slerp(time, a, b);
	
	return Quaternion::Slerp(2*time * (1-time), q1, q2);
} // Squad()



/**
 * Method for make slerp from two quaternions in time.
 * @param float time Specified time,
 * @param Quaternion p First quaternion,
 * @param Quaternion q Second quaternion.
 * @return Quaternion New quaternion as a result of Slerp.
 */ 
Quaternion Quaternion::Slerp(const float &time, Quaternion &p, Quaternion &q) 
{
    /* assert:  p.Dot(q) >= 0 */
	Quaternion temp;
    float dAngle, dSin, dInvSin, dCoeff0, dCoeff1;
    float dCos = p.Dot(q);

    /* numerical round-off error could create problems in call to acos */
    if(dCos < -1.0) {
        dCos = -1.0;
	}
    else if (dCos > 1.0) {
        dCos = 1.0;
	}

    dAngle = acos(dCos);
    dSin = sin(dAngle);  /* fSin >= 0 since fCos >= 0 */

    if (dSin < QUAT_EPS) {
        temp = p;
    }
    else
    {
        dInvSin = 1.0 / dSin;
        dCoeff0 = sin((1.0 - time) * dAngle) * dInvSin;
		dCoeff1 = sin(time * dAngle) * dInvSin;
	
		temp.w(dCoeff0 * p.w() + dCoeff1 * q.w())
			.x(dCoeff0 * p.x() + dCoeff1 * q.x())
			.y(dCoeff0 * p.y() + dCoeff1 * q.y())
			.z(dCoeff0 * p.z() + dCoeff1 * q.z());
    }
	
	return temp;
} // Slerp()



/** ***************************************************************************/
/** end of file Quaternion.cpp												  */
/** ***************************************************************************/
