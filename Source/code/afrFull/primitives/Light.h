/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Light.h															  *	
 * @date	15. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief   Type for definition of light entity with its position, 			  *
 * 			color and name (for animations).								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_LIGHT_H
#define _AFR_LIGHT_H



// Including system libraries
#include <string>



// Including project libraries
#include "../types/Array.h"
#include "../types/Color.h"
#include "../types/Vector3.h"



/**
 * @brief Definition of structure for gathering informations about light entity in 
 * scene. Creates new type TLight for stored this structure.
 */ 
typedef struct s_light {
	string		name;
	Color		color;
	Vector3		position;
} TLight;



/**
 * @brief Define new type for list of light as dynamic array of lights where 
 * light is a pointer on a type TLight.
 */ 
typedef Array<TLight*> TLightList;



#endif // #ifndef _AFR_LIGHT_H



/** ***************************************************************************/
/** end of file Light.h														  */
/** ***************************************************************************/
