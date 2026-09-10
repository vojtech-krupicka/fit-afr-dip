/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    PositionInfo.h													  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class PositionInfo.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_POSITION_INFO_H
#define _AFR_POSITION_INFO_H


// Including project libraries
#include "../types/Vector3.h"



/**
 * @brief Structure for store informations about rotation key for calculate actual 
 * rotation at the given time.
 */ 
typedef struct s_poskey {
	float 		time;
	Vector3 	pos;
	float 		tension, continuity, bias;
} TPositionKey;



/**
 * @brief Structure for store informations about cubic polynom for creates key nodes 
 * of curve which will be interpolated in specific time.
 */ 
typedef struct s_cpoly {
	Vector3		C0, C1, C2, C3;
	float 		tMin, tMax, tRange;
} TCubicPolynom;



/**
 * Class RotationInfo
 * @brief Helper function for describe animations in translation and scale 
 * transformations.
 */ 
class PositionInfo
{
public:
	///< Implicit constructors
	PositionInfo();
	
	///< Implicit destructors
	~PositionInfo();
	
	///< Position operations
	PositionInfo *Initialize(TPositionKey *(&keys), const unsigned &count);
	Vector3 Interpolate0(const float &time);
	Vector3 Interpolate1(const float &time);
	Vector3 Interpolate2(const float &time);
private:
	///< Private operations
	PositionInfo 	*CalcUI(const float&, float&, unsigned&); 
	PositionInfo	*CalcArcLength();
	float 	CalcLength(const TCubicPolynom&, const float&);
	float 	CalcSpeed(const TCubicPolynom&, const float&);
	
	Vector3 InterpolateSingle0(const TCubicPolynom&, const float&);
	Vector3 InterpolateSingle1(const TCubicPolynom&, const float&);
	Vector3 InterpolateSingle2(const TCubicPolynom&, const float&);

	TCubicPolynom	*polyInfo;
	unsigned		count;
	
	float			*lengths;
	float			totalLength;
}; // class PositionInfo



#endif // #ifndef _AFR_POSITION_INFO_H



/** ***************************************************************************/
/** end of file PositionInfo.h												  */
/** ***************************************************************************/
