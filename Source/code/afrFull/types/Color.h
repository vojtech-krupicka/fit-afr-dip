/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Color.h															  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief   New color type definition.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_COLOR_H
#define _AFR_COLOR_H


// Including project libraries
#include "ColorRGB.h"


/**
 * @brief Definition of new type for color. This type is the same as ColorRGB 
 * and this hack is only for the reason portability. Class "ColorRGB" 
 * should be replace with some other class (ColorRGBA, SpectralColor, etc). 
 */
typedef ColorRGB Color;



#endif // #ifndef _AFR_COLOR_H



/** ***************************************************************************/
/** end of file Color.h													  */
/** ***************************************************************************/
