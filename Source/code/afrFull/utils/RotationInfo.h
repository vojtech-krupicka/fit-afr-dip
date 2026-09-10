/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    RotationInfo.h													  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class RotationInfo.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_ROTATION_INFO_H
#define _AFR_ROTATION_INFO_H



// Including project libraries
#include "../types/Vector4.h"
#include "../types/Quaternion.h"



/**
 * @brief Structure for store informations about rotation key for calculate actual 
 * rotation at the given time.
 */ 
typedef struct s_rotkey {
	float 		time;
	Vector4 	rot;
	float 		tension, continuity, bias;
} TRotationKey;



/**
 * @brief Structure for store informations about one spline quad for rotations.
 */ 
typedef struct s_squadinfo {
	Quaternion 	p, q, a, b;
	float 		tMin, tMax, tRange;
} TSquadInfo;



/**
 * Class RotationInfo
 * @brief Helper function for describe animations in rotation transformations.
 */ 
class RotationInfo
{
public:
	///< Implicit constructors
	RotationInfo();
	
	///< Implicit destructors
	~RotationInfo();
	
	///< Rotation operations
	RotationInfo *Initialize(TRotationKey *(&keys), const unsigned &count);
	Vector4 Interpolate(const float &time);
	
private:
	TSquadInfo	*squads;
	unsigned	count;
}; // class RotationInfo



#endif // #ifndef _AFR_ROTATION_INFO_H



/** ***************************************************************************/
/** end of file RotationInfo.h												  */
/** ***************************************************************************/
