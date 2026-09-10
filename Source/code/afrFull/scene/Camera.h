/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Camera.h														  *	
 * @date	15. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Camera.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_CAMERA_H
#define _AFR_CAMERA_H


// Including project libraries
#include "../types/Vector2.h"
#include "../types/Vector3.h"
#include "../types/Matrix.h"
#include "../utils/MathUtils.h"



/**
 * Class Camera
 * @brief Class for description of a camera, its position, direction and
 * resolution in scene.
 */ 
class Camera
{
public:
	///< Default constructors
	inline Camera();
	inline Camera(const Vector3 &from, const Vector3 &at, const Vector3 &up, const float &angle, const float &hither, const Vector2 &res);
	
	///< Default destructors
	inline ~Camera() {};
	
	/// Getters
	inline Vector3 	GetPosition() const { return this->pos; };
	inline Vector3 	GetLookAt() const { return this->at; };
	inline Vector3 	GetDirection() const { return this->dir; };
	inline Vector3 	GetUp() const { return this->up; };
	inline float 	GetFOV() const { return this->fov; };
	inline float 	GetHither() const { return this->hither; };
	inline unsigned	GetWidth() const { return this->width; };
	inline unsigned	GetHeight() const { return this->height; };
	inline float	GetImWidth() const { return this->imWidth; };
	inline float	GetImHeight() const { return this->imHeight; };
	inline Vector3	GetHStep() const { return this->hStep; };
	inline Vector3	GetVStep() const { return this->vStep; };
	inline Vector2 	GetResolution() const { return Vector2(this->width, this->height); };
	
	inline Vector3	*GetScreen() { return this->screen; };
	inline Vector3	GetScreen(const unsigned &i) const { return this->screen[i]; };
	
	inline Vector3	GetTLCorner() const { return this->screen[0]; };
	inline Vector3	GetTRCorner() const { return this->screen[1]; };
	inline Vector3	GetBRCorner() const { return this->screen[2]; };
	inline Vector3	GetBLCorner() const { return this->screen[3]; };
	
	inline Vector3  GetOffset(const unsigned &x, const unsigned &y) const { 
		int xx = Min(x, this->width-1);
		int yy = Min(y, this->height-1);
		return this->matrix[yy * this->width + xx]; 
	};
	
	/// Setters
	inline Camera	*SetPosition(const Vector3 &from) { this->pos = from; return this; };
	inline Camera 	*SetLookAt(const Vector3 &at) { this->at = at; return this; };
	inline Camera 	*SetUp(const Vector3 &up) { this->up = up; return this; };	
	
	/// Camera operations
	inline Camera	*Transform();
	
private:
	inline Camera	*InitResolution(const unsigned &w, const unsigned &h);
	inline Camera	*InitScreen();
	
	Vector3		pos;		///< eye position in scene
	Vector3		at;			///< position of point, where camera looks
	Vector3		dir;		///< camera's direction vector
	Vector3		up;			///< camera's up vector
	float 		fov;		///< FOVy angle
	float		hither;		///< near plane distance
	unsigned	width;		///< real width in pixels
	unsigned 	height;		///< real height in pixels
	float		imWidth;	///< imaginary width in abstract units
	float 		imHeight;	///< imaginary height in abstract units
	Vector3		screen[4];	///< four points of near plane (top-left, top-right, bottom-right, bottom-left)
	Vector3		hStep;		///< horizontal increment (width step)
	Vector3		vStep;		///< vertical increment (height step)
	
	Vector3		*matrix;
}; // class Camera




/**
 * Implicit constructor. All components are zero.
 * @return Camera.
 */ 
inline Camera::Camera()
{
	this->pos = Vector3::Zero();
	this->at = -Vector3::ZAxis();
	this->dir = -Vector3::ZAxis();
	this->up = Vector3::YAxis();
	this->fov = 45.0f;
	this->hither = 1.0f;
	this->matrix = NULL;
	
	this->InitResolution(640, 480)
		->InitScreen();
} // Camera()



/**
 * Implicit constructor from four-link float array.
 * @param Vector3 from Camera position in the scene,
 * @param Vector3 at Camera direction vector (look at),
 * @param Vector3 up Camera up direction (mostly y axis),
 * @param float angle Angle between top and bottom bar of view - field of view,
 * @param float hither Distance between camera position and near plane,
 * @param Vector2 res Camera resolution - i.e. width and height of the final picture.
 * @return Camera.
 */ 
inline Camera::Camera(const Vector3 &from, const Vector3 &at, const Vector3 &up, const float &angle, const float &hither, const Vector2 &res)
{
	this->pos = from;
	this->at = at;
	this->dir = (at - from).Normalize();
	this->up = up;
	this->fov = angle;
	this->hither = hither;
	this->matrix = NULL;
	
	this->InitResolution(res.x(), res.y())
		->InitScreen()
		->Transform();
} // Camera()



/**
 * Method for sets camera real width and height in pixels and calculate 
 * imaginary resolution in abstract unit for future use in raytracer.
 * @param unsigned w Real screen width pixels,
 * @param unsigned h Real screen heihgt pixels,
 * @return Camera Provides fluent interface.
 */ 
inline Camera *Camera::InitResolution(const unsigned &w, const unsigned &h)
{
	// Set resolution in pixels
	this->width = w;
	this->height = h;
	
	// Init auxiliary variables for imaginary resolutions
	float ratio = (float)this->width / (float)this->height;
	float tanTmp = tan(DEG_TO_RAD(this->fov/2.0));
	
	// Calculate imaginary resolutions and screen
	this->imHeight = this->hither * tanTmp;
	this->imWidth = this->imHeight * ratio;
	
	if(this->matrix != NULL) {
		delete [] this->matrix;
		this->matrix = NULL;
	}
	this->matrix = new Vector3[this->width * this->height];
	
	
	return this;
} // InitResolution()



/**
 * Method for sets camera screen corners point in abstract unit in scene space.
 * @return Camera Provides fluent interface.
 */ 
inline Camera *Camera::InitScreen()
{
	// Set screen corners
	this->screen[0] = Vector3(-this->imWidth, this->imHeight, this->hither);
	this->screen[1] = Vector3(this->imWidth, this->imHeight, this->hither);
	this->screen[2] = Vector3(this->imWidth, -this->imHeight, this->hither);
	this->screen[3] = Vector3(-this->imWidth, -this->imHeight, this->hither);

	return this;
} // InitResolution()



/**
 * Method for transform camera screen corners and calculates new 
 * camera direction.
 * @return Camera Provides fluent interface.
 */ 
inline Camera *Camera::Transform()
{
	// Reinit screen corners position
	this->InitScreen();
	
	// Calculate direction unit vector
	this->dir = (this->at - this->pos).Normalize();
	
	// Auxiliary variables
	Vector3 x, y, z;
	z = this->dir;
	x = this->up.Cross(-z);
	y = x.Cross(z);
	
	// Prepare transformation matrix for screen corners
	Matrix view;
	view[0][0] = x.x(); view[1][0] = x.y(); view[2][0] = x.z();
	view[0][1] = y.x(); view[1][1] = y.y(); view[2][1] = y.z();
	view[0][2] = z.x(); view[1][2] = z.y(); view[2][2] = z.z();
	
	view[3][0] = this->pos.x();
	view[3][1] = this->pos.y();
	view[3][2] = this->pos.z();
	
	// Transform
	this->screen[0] *= view;	// top left
	this->screen[1] *= view;	// top right
	this->screen[2] *= view;	// bottom right
	this->screen[3] *= view;	// bottom left
	
	// Calculate horizontal and vertical steps (increment between samples)
	this->hStep = (this->screen[1] - this->screen[0]) / (float)this->width;
	this->vStep = (this->screen[3] - this->screen[0]) / (float)this->height;
	
	Vector3 col = this->screen[0];
	Vector3 row = col;
	for(unsigned i = 0; i < this->height; i++) {
		for(unsigned j = 0; j < this->width; j++) {
			this->matrix[i * width + j] = col;
			col += this->hStep;
		}
		
		row += this->vStep;
		col = row;
	}

	return this;
} // Transform()



#endif // #ifndef _AFR_CAMERA_H



/** ***************************************************************************/
/** end of file Camera.h													  */
/** ***************************************************************************/
