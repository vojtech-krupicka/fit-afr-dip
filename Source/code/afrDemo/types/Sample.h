/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Sample.h														  *	
 * @date	16. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief   New type definition of sample and its list.						  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_SAMPLE_H
#define _AFR_SAMPLE_H


// Including project libraries
#include "Array.h"
#include "Color.h"
#include "Vector2.h"



// Used namespaces
using namespace std;



// Predefinition of classes
class Vector2;



/**
 * @brief Structure Sample for definition sample as object with its position on image
 * plane, color at its position and time when this sample has been taken.
 */ 
typedef struct s_smpl {
	Vector2		iPos;		///< sample position at image plane in integer value (i.e. pixel in raster)
	Vector2 	fPos;		///< sample position at image plane in float value with offset
	Color 		color;		///< sample color at its position
	float		intensity;	///< sample's color intensity (grayscale)
	unsigned	age;		///< sample age, i.e. the time when sample has been taken
} TSample;



/**
 * @brief Defines new type for list of samples.
 */
typedef Array<TSample*> TSampleList;



#endif // #ifndef _AFR_SAMPLE_H



/** ***************************************************************************/
/** end of file Sample.h													  */
/** ***************************************************************************/

