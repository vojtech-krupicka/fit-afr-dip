/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Random.h														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Random.								 		  *
 **************************************************************************** */
 
 
// Conditional compilation 
#ifndef _AFR_RANDOM_H
#define _AFR_RANDOM_H


// Including system libraries
#include <cmath>
#include <limits>
#include <algorithm>
#include <ctime>



/**
 * Class Random
 * @brief Class for generate pseudorandom numbers.
 */ 
class Random
{
public:
	///< Implicit constructor and desctructor
	inline Random(const unsigned long long &s);
	~Random() {};
	
	///< Operators
	inline float operator()();
	
private:
	unsigned long long seed;
	unsigned long long mult;
	unsigned long long lmax;
	float fmax;
}; // class Random



/**
 * Implicit constructor. Sets seed and other properties.
 * @return Random.
 */ 
inline Random::Random(const unsigned long long &s = 0)
{
	this->seed = (s == 0) ? (unsigned long long)(time(NULL)) : s;
	this->mult = 62089911ULL;
	this->lmax = 4294967295ULL;
	this->fmax = 4294967295.0f;	
} // Random()



/**
 * Gets a random number from interval [0, 1).
 * @return float Returns random nuberm.
 */ 
inline float Random::operator()()
{
	this->seed *= this->mult;
	return float(this->seed % this->lmax) / this->fmax;
} // operator()




#endif // #ifndef _AFR_RANDOM_H



/** ***************************************************************************/
/** end of file Random.h													  */
/** ***************************************************************************/

