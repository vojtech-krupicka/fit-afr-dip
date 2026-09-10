/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Animation.h														  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Animation.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_ANIMATION_H
#define _AFR_ANIMATION_H


// Including system libraries
#include <string>
#include <limits>


// Including project libraries
#include "../utils/PositionInfo.h"
#include "../utils/RotationInfo.h"
#include "../types/Array.h"


// Predefinition of classes
class Vector3;
class Vector4;
class Matrix;



/**
 * @brief Structure for store informations about rotation key for calculate actual 
 * rotation at the given time.
 */ 
typedef struct s_viskey {
	float 		time;
	bool 		visible;
} TVisibilityKey;



/**
 * @brief Type for list of visibility keys.
 */ 
typedef Array<TVisibilityKey> TVisibilityList;



/**
 * Class Animation
 * @brief Class for description of animation of camera or primitives in scene.
 */ 
class Animation
{
public:
	///< Implicit constructors
	Animation();
	Animation(const string &n);
	
	///< Implicit destructors
	~Animation();	
	
	///< Setters
	Animation	*SetTranslations(TPositionKey *(&keys), const unsigned &count);
	Animation	*SetRotations(TRotationKey *(&keys), const unsigned &count);
	Animation	*SetScales(TPositionKey *(&keys), const unsigned &count);
	Animation	*AddVisibilityKey(const float &time, const bool &vis);
	
	///< Getters
	inline string 	GetName() const { return this->name; };
	Vector3			GetTranslation(const float &time);
	Vector4			GetRotation(const float &time);
	Vector3			GetScale(const float &time);
	bool			GetVisibility(const float &time);
	Matrix			GetMatrix(const float &time);
	
	///< Others methods
	inline bool		HasTranslations() const { return (this->translationInfo != NULL); };
	inline bool		HasRotations() const { return (this->rotationInfo != NULL); };
	inline bool		HasScales() const { return (this->scaleInfo != NULL); };
	inline bool		HasVisibilities() const { return !this->visInfo.Empty(); };
	
private:
	string			name;
	PositionInfo	*translationInfo;
	RotationInfo	*rotationInfo;
	PositionInfo	*scaleInfo;
	TVisibilityList visInfo;
}; // class Animation



/**
 * @brief Type for new list of animations as dynamic array.
 */ 
typedef Array<Animation*> TAnimationList;



#endif // #ifndef _AFR_ANIMATION_H



/** ***************************************************************************/
/** end of file Animation.h													  */
/** ***************************************************************************/
