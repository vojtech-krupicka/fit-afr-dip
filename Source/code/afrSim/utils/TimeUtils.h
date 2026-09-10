/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    TimeUtils.h														  *	
 * @date	14. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief   Contains macros, definitions and function for time operations.    *
 **************************************************************************** */
 
 
// Conditional compilation 
#ifndef _AFR_TIME_UTILS_H
#define _AFR_TIME_UTILS_H


// Including system libraries
#include <string>
#include <sstream>
#include <GL/gl.h>
#include <GL/glut.h>


// Used namespaces
using namespace std;



// Global variable for simulated animation time
static unsigned simulatedClock = 0;



/**
 * Method for simulating time in application.
 * @param unsigned i Number of milisecond which elapsed since last update.
 * @return unsigned Returns new ticks in miliseconds.
 */ 
inline unsigned UpdateSimulatedClock(const unsigned &i)
{
	simulatedClock += i;
	return simulatedClock;
} // SetSimulatedTicks()



/**
 * Function for get actual processor time.
 * @return unsigned Returns actual processor time.
 */ 
inline unsigned Now()
{
	return UpdateSimulatedClock(0);
} // Now()



/**
 * Fuction for get time span between start time and actual time. If second 
 * parameter is not defined, time span will be in milisecond.
 * @return unsigned Returns actual processor time in miliseconds.
 */ 
inline float TimeSpan(const unsigned &start, const unsigned &units = 1) 
{
	return (units < 2) ? (Now() - start) : ((Now() - start) / (float)units);
} // TimeSpan()



#endif // _AFR_TIME_UTILS_H



/** ***************************************************************************/
/** end of file TimeUtils.h													  */
/** ***************************************************************************/
