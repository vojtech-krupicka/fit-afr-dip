/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Animation.cpp													  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Animation.								  *
 **************************************************************************** */


// Including project libraries
#include "Animation.h"
#include "../types/Vector3.h"
#include "../types/Vector4.h"
#include "../types/Matrix.h"



/**
 * Default constructor.
 * @return Animation.
 */ 
Animation::Animation()
{
	this->name = "";
	
	this->translationInfo = NULL;
	this->rotationInfo = NULL;
	this->scaleInfo = NULL;
} // Animation()



/**
 * Default constructor. Initialize animation with given name.
 * @param string n Animation name.
 * @return Animation.
 */ 
Animation::Animation(const string &n)
{
	this->name = n;
	
	this->translationInfo = NULL;
	this->rotationInfo = NULL;
	this->scaleInfo = NULL;
} // Animation()



/**
 * Default desctructor.
 */ 
Animation::~Animation() 
{
	if(this->translationInfo != NULL) {
		delete this->translationInfo;
		this->translationInfo = NULL;
	}
	if(this->rotationInfo != NULL) {
		delete this->rotationInfo;
		this->rotationInfo = NULL;
	}
	if(this->scaleInfo != NULL) {
		delete this->scaleInfo;
		this->scaleInfo = NULL;
	}
} // ~Animation()



/**
 * Calculates animation's position at given time.
 * @param float time Animation time.
 * @return Vector3 Returns vector of position for transformation.
 */ 
Vector3	Animation::GetTranslation(const float &time)
{
	if(this->HasTranslations()) {
		return this->translationInfo->Interpolate0(time);		
	}
	
	return Vector3(0.0, 0.0, 0.0);
} // GetTranslation()



/**
 * Calculates animation's rotation at given time.
 * @param float time Animation time.
 * @return Vector4 Returns vector of rotation for transformation.
 */ 
Vector4	Animation::GetRotation(const float &time)
{
	if(this->HasRotations()) {
		return this->rotationInfo->Interpolate(time);		
	}
	
	return Vector4(1.0, 0.0, 0.0, 0.0);
} // GetRotation()



/**
 * Calculates animation's scale at given time.
 * @param float time Animation time.
 * @return Vector3 Returns vector of sacle for transformation.
 */ 
Vector3	Animation::GetScale(const float &time)
{
	if(this->HasScales()) {
		return this->scaleInfo->Interpolate0(time);		
	}
	
	return Vector3(1.0, 1.0, 1.0);
} // GetScale()



/**
 * Gets visibility flag for object in animation at given time.
 * @param float time Animation time.
 * @return bool Returns true if object is visible, false otherwise.
 */ 
bool Animation::GetVisibility(const float &time)
{
	if(this->HasVisibilities()) {
		bool visible = true;
		float prevTime = numeric_limits<float>::min( );
		
		for(unsigned i = 0; i < this->visInfo.Size(); i++) {
			if(time >= prevTime && time < this->visInfo[i].time) {
				return visible;
			}
			
			visible = this->visInfo[i].visible;
			prevTime = this->visInfo[i].time;
		}
		
		return visible;
	}
	
	// Pokud nejni definovana viditelnost, vraci vzdy true
	return true;
} // GetVisibility()



/**
 * Calculates animation transformation matrix at given time.
 * @param float time Animation time.
 * @return Matrix Returns whole transformation matrix at given time.
 */ 
Matrix Animation::GetMatrix(const float &time)
{
	// m = t * r * s
	Matrix matrix; // identita
	
	if(this->HasRotations()) {
		Quaternion quat;
		Vector4 rotation = this->GetRotation(time);		
		
		rotation >> quat;
		matrix << quat;
	}
	
	if(this->HasScales()) {
		Vector3 scale = this->GetScale(time);
		
		matrix[0][0] *= scale[0]; matrix[0][1] *= scale[1]; matrix[0][2] *= scale[2];
		matrix[1][0] *= scale[0]; matrix[1][1] *= scale[1]; matrix[1][2] *= scale[2];
		matrix[2][0] *= scale[0]; matrix[2][1] *= scale[1]; matrix[2][2] *= scale[2];
	}
	
	if(this->HasTranslations()) {
		Vector3 transl = this->GetTranslation(time);
		
		matrix[0][3] = transl[0];
		matrix[1][3] = transl[1];
		matrix[2][3] = transl[2];
	}
	
 	return matrix;
} // GetMatrix()



/**
 * Sets translations keyframes and creates translation info object.
 * @param TPositionKey keys Position keys from which will be count translation path.
 * @param unsigned count Count of position keys.
 * @return Animation Returns this to provide fluent interface.
 */ 
Animation *Animation::SetTranslations(TPositionKey *(&keys), const unsigned &count)
{
	if(this->HasTranslations()) {
		delete this->translationInfo;
		this->translationInfo = NULL;
	}
	
	this->translationInfo = new PositionInfo();
	this->translationInfo->Initialize(keys, count);
	
	return this;
} // SetTranslations()



/**
 * Sets rotations keyframes and creates rotation info object
 * @param TRotationKey keys Rotation keys from which will be count rotation path.
 * @param unsigned count Count of rotations keys.
 * @return Animation Returns this to provide fluent interface.
 */ 
Animation *Animation::SetRotations(TRotationKey *(&keys), const unsigned &count)
{
	if(this->HasRotations()) {
		delete this->rotationInfo;
		this->rotationInfo = NULL;
	}
	
	this->rotationInfo = new RotationInfo();
	this->rotationInfo->Initialize(keys, count);
	
	return this;
} // SetRotations()



/**
 * Sets scales keyframes and creates scale info object.
 * @param TPositionKey keys Position keys from which will be count scale path.
 * @param unsigned count Count of position keys.
 * @return Animation Returns this to provide fluent interface.
 */ 
Animation *Animation::SetScales(TPositionKey *(&keys), const unsigned &count)
{
	if(this->HasScales()) {
		delete this->scaleInfo;
		this->scaleInfo = NULL;
	}
	
	this->scaleInfo = new PositionInfo();
	this->scaleInfo->Initialize(keys, count);
	
	return this;
} // SetScales()



/**
 * Sets visibility keyframes and creates vis info object.
 * @param float time Keyframe time.
 * @param bool vis Flag if object is visible at given time or not.
 * @return Animation Returns this to provide fluent interface.
 */ 
Animation *Animation::AddVisibilityKey(const float &time, const bool &vis)
{
	TVisibilityKey key;
	key.time = time;
	key.visible = vis;
	
	this->visInfo.Add(key);
	return this;
} // SetVisibility()



/** ***************************************************************************/
/** end of file Animation.cpp												  */
/** ***************************************************************************/
