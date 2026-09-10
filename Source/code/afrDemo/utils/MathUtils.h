/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    MathUtils.h														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief   Contains macros, definitions and function for math operations.	  *
 **************************************************************************** */
 
 
// Conditional compilation 
#ifndef _AFR_MATH_UTILS_H
#define _AFR_MATH_UTILS_H 


// Including system libraries
#include <cmath>
#include <limits>
#include <algorithm>



// Define basic limits
#ifndef MAXFLOAT
#define MAXFLOAT		((float)3.40282347e+38) 
#endif
#ifndef MAXDOUBLE
#define MAXDOUBLE		1.7976931348623157e+308
#endif
#ifndef MAXINTEGER
#define MAXINTEGER		((int)(~(unsigned int)0 >> 1))
#endif
#ifndef MININTEGER
#define MININTEGER		((int)(~(unsigned int)0))
#endif



// Define basic math constants (PI)
#undef M_PI
#define M_PI         3.14159265358979323846f  

#undef M_1_PI
#define M_1_PI       0.31830988618379067154f

#undef M_PI_4
#define M_PI_4       0.78539816339744830962f

#undef M_PI_2
#define M_PI_2       1.57079632679489661923f

#undef M_2_PI
#define M_2_PI       0.63661977236758134308f

#define M_2PI        (2.0f*M_PI)
#define M_4PI        (4.0f*M_PI)
#define M_1_2PI      (0.5f*M_1_PI)
#define M_1_4PI      (0.25f*M_1_PI)



// Define macros for converting angle from degrees to radians and vice versa
#define DEG_TO_RAD(d) 	(d*M_PI / 180.0)
#define RAD_TO_DEG(r) 	(r*180.0 / M_PI) 



///< Define macro MIN2 for determine minimal of two values.
#ifndef MIN2
	#define MIN2(a, b) (((a)<(b))?(a):(b)) 
#endif


///< Define macro MIN3 for determine minimal of three values.
#ifndef MIN3
	#define MIN3(a, b, c) (((MIN(a,b)<(c))?(MIN(a,b):(c)) 
#endif


///< Define macro MIN4 for determine minimal of four values.
#ifndef MIN4
	#define MIN4(a, b, c, d) (((MIN(a,b,c)<(d))?(MIN(a,b,c):(d)) 
#endif


///< Define macro MAX2 for determine maximal of two values.
#ifndef MAX2
	#define MAX2(a, b) (((a)>(b))?(a):(b)) 
#endif


///< Define macro MAX3 for determine maximal of three values.
#ifndef MAX3
	#define MAX3(a, b, c) (((MAX(a,b)>(c))?(MAX(a,b):(c)) 
#endif


///< Define macro MAX4 for determine maximal of four values.
#ifndef MAX4
	#define MAX4(a, b, c, d) (((MAX(a,b,c)>(d))?(MAX(a,b,c):(d)) 
#endif


///< Define macro AVG for average of two values.
#ifndef AVG
	#define AVG(a,b)          ((a)+(b) / 2.0)
#endif


///< Define macro ABS for determine absolut value of variable 'a'.
#ifndef ABS
	#define ABS(a)          ((a) >= 0.0 ? (a) : -(a))
#endif

///< Define macro ROUND for round variable 'a'.
#ifndef ROUND
	#define ROUND(a)          (((a) >= 0.0) ? ((int)((a) + 0.99999)) : ((int)((a) - 0.99999)))
#endif



/**
 * Template function Min for determine minimal value of two value.
 */ 
template < typename T >
inline T Min(const T a, const T b)
{
    return ((a < b) ? a : b);
} // Min()



/**
 * Template function Min for determine minimal value of three value.
 */ 
template < typename T >
inline T Min(const T a, const T b, const T c)
{
    return (Min(Min(a, b), c));
} // Min()



/**
 * Template function Min for determine minimal value of four value.
 */ 
template < typename T >
inline T Min(const T a, const T b, const T c, const T d)
{
    return (Min(Min(Min(a, b), c), d));
} // Min()



/**
 * Template function Max for determine maxnimal value of two value.
 */ 
template < typename T >
inline T Max(const T a, const T b)
{
    return ((a > b) ? a : b);
} // Max()



/**
 * Template function Max for determine maxnimal value of three value.
 */ 
template < typename T >
inline T Max(const T a, const T b, const T c)
{
    return (Max(Max(a, b), c));
} // Max()



/**
 * Template function Max for determine maxnimal value of four value.
 */ 
template < typename T >
inline T Max(const T a, const T b, const T c, const T d)
{
    return (Max(Max(Max(a, b), c), d));
} // Max()



/**
 * Template function Avg for calc average of two values.
 */ 
template < typename T >
inline T Avg(const T a, const T b)
{
    return ((a + b) / 2);
} // Avg()



/**
 * Template function Avs for get absolut value of variable 'a'.
 */ 
template < typename T >
inline T Abs(const T a)
{
    return ((a >= 0.0) ? a : -a);
}  // Abs()



/**
 * Template function Round for round variable 'a'.
 */ 
template < typename T >
inline T Round(const T a)
{
    return (a >= 0.0) ? static_cast<int>(a + 0.99999) : static_cast<int>(a - 0.99999);
}  // Round()



#endif // _AFR_MATH_UTILS_H



/** ***************************************************************************/
/** end of file MathUtils.h													  */
/** ***************************************************************************/
