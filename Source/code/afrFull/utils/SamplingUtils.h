/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    SamplingUtils.h													  *	
 * @date	14. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief   Contains macros, definitions and function for sampling operations *
 * 			for create new sample at specified position by one of the 		  *	
 * 			sampling methods.												  *
 **************************************************************************** */
 
 
// Conditional compilation 
#ifndef _AFR_SAMPLING_UTILS_H
#define _AFR_SAMPLING_UTILS_H


// Including system libraries
#include <string>
#include <sstream>



// Including project libraries
#include "Random.h"


// Used namespaces
using namespace std;



// Global variable for random generate pseudorandom numbers in sampling methods
static Random rnd;



/** ***************************************************************************/
/** Implementation of helpers function for regular sampling methods			  */
/** ***************************************************************************/



/**
 * Helper function for Cubic spline filter method.
 * @param float r Number to solve.
 * @return float Returns solved value.
 */ 
inline float SolveHelper(float r)
{
	float u = r;
	for(int i = 0; i < 5; i++) {
		u = (11.0f * r + u * u * (6.0f + u * (8.0f - 9.0f * u))) /
			(4.0f + 12.0f * u * (1.0f + u * (1.0f - u)));
	}
	return u;
} // SolveHelper()



/**
 * Helper function for Cubic spline filter, which compute cubic spline
 * interpolation of given number.
 * @param float x Nubmer to solve.
 * @return float Returns original value filterd by cubic spline filter.
 */ 
inline float CubicFilterHelper(float x)
{
	if(x < 1.0f / 24.0f) {
		return pow(24* x, 0.25f) - 2.0f;
	}
	else if(x < 0.5) {
		return SolveHelper(24.0f * (x - 1.0f / 24.0f) / 11.0f) - 1.0f;
	}
	else if(x < 23.0f / 24.0f) {
		return 1.0f - SolveHelper(24.0f * (24.0f / 24.0f - x) / 11.0f);
	}
	else {
		return 2.0f - pow(24.0f * (1.0f - x), 0.25f);
	}
	
	return x;
} // CubicFilterHelper()



/**
 * Helper function for halton sequence. Calculates n-th item of sequence, 
 * where n = index of given base.
 * @param unsigned index N-th item of sequence,
 * @param unsigned base Base for halton sequence, shloud be 2 or 3.
 * @return float Return N-th item of halton sequence.
 */ 
inline float HaltonHelper(const unsigned &index, const unsigned &base, float factor)
{
	float result = 0.0f;
	unsigned i = index;
	
	while(i > 0) {
		result += factor * (i % base);
		i = floor((float)i / (float)base);
		factor /= (float)base;
	} 
	
	return result;
} // HaltonHelper()



/**
 * Helper function for Poissons discs. Test if the sample lies in the 
 * circle around one of the samples.
 * @param Vector2 samples Array of all samples,
 * @param Vector2 sample New sample, which is tested,
 * @param float radius Radisu around each sample in samples array,
 * @param unsigned count Number of samples in samples array.
 * @return bool Return true, if sample lies in circle around one of the samples, false otherwise.
 */ 
inline bool PoinssonHelper(Vector2 *samples, Vector2 sample, const float &radius, const unsigned &count)
{
	for(unsigned i = 0; i < count; i++) {
		Vector2 diff = samples[i] - sample;
		if(diff.Length() < radius*radius) {
			return false;
		}
	}
	
	return true;
} // PoinssonHelper()



/** ***************************************************************************/
/** Implementation of sampling methods for create N new samples            	  */
/** ***************************************************************************/



/**
 * Function for create N new samples in pseudo random order.
 * @param Vector2 samples New samples in 1D array,
 * @param unsigned count Number of samples in samples array.
 * @return unsigned Return real number of new samples, which will be created.
 */ 
inline unsigned CreateRandom(Vector2 *samples, const unsigned &count)
{
	for(unsigned i = 0; i < count; i++) {
		samples[i].x(rnd()).y(rnd());
	}
	
	return count;
} // CreateRandom()



/**
 * Function for compute N new points in halton sequence.
 * @param Vector2 samples New samples in 1D array,
 * @param unsigned index Next index into halton sequence,
 * @param unsigned count Number of samples in samples array.
 * @return unsigned Returns actual index into halton sequence.
 */ 
inline unsigned CreateHalton(Vector2 *samples, unsigned &index, const unsigned &count)
{
	for(unsigned i = 0; i < count; i++) {
		samples[i].x(HaltonHelper(index, 2, 0.5f))
				  .y(HaltonHelper(index, 3, 0.33333f));
				  
		index++;
	}
	
	return count;
} // CreateHalton()



/**
 * Function for create N new samples by Jitterd (or stratified) sampling method. 
 * This method divides screen plane into N cells and computes N new samples. 
 * Each sample lies in one cell.
 * @param Vector2 samples New samples in 1D array,
 * @param unsigned count Number of samples in samples array.
 * @return unsigned Return real number of new samples, which will be created.
 */ 
inline unsigned CreateJitter(Vector2 *samples, const unsigned &count)
{
	unsigned size = (unsigned)sqrt(count);
	for(unsigned i = 0; i < size; i++) {
		for(unsigned j = 0; j < size; j++) {
			samples[i * size + j].x((j + rnd()) / size)
								 .y((i + rnd()) / size);
		}
	}
	
	return count;
} // CreateJitter()



/**
 * Function for create N new samples by Multijitter method.
 * @param Vector2 samples New samples in 1D array,
 * @param unsigned count Number of samples in samples array.
 * @return unsigned Return real number of new samples, which will be created.
 */ 
inline unsigned CreateMultiJitter(Vector2 *samples, const unsigned &count)
{
	unsigned size = (unsigned)sqrt(count);
	float subcell = 1.0f / (float)count;
	
	for(unsigned i = 0; i < size; i++) {
		for(unsigned j = 0; j < size; j++) {
			samples[i*size + j].x((i*size + j)*subcell + (rnd()*subcell));
			samples[i*size + j].y((j*size + i)*subcell + (rnd()*subcell));
		}
	}
	
	for(unsigned i = 0; i < size; i++) {
		for(unsigned j = 0; j < size; j++) {
			unsigned target = j + (unsigned)(rnd() * (size - j));
			float tmp = samples[i*size + j].x();			
			samples[i*size + j].x(samples[i*size + target].x());
			samples[i*size + target].x(tmp);
			
			target = j + (int)(rnd() * (size - j));
			tmp = samples[j*size + i].y();
			samples[j*size + i].y(samples[target*size + i].y());
			samples[target*size + i].y(tmp);
		}
	}
	
	return count;
} // CreateMultiJitter()



/**
 * Function for create N new samples by Poisson Discs method. This function 
 * creates as many as possible new samples (max N), each new sample is generated 
 * max 100 times and tests if lies in circle around other samples.
 * @param Vector2 samples New samples in 1D array,
 * @param unsigned count Number of samples in samples array.
 * @return unsigned Return real number of new samples, which will be created.
 */ 
inline unsigned CreatePoisson(Vector2 *samples, const unsigned &count)
{
	unsigned result = 0;
	float radius = 1.0f / count * 4.0f;
	
	for(unsigned i = 0; i < count; i++) {
		bool ok = true;
		unsigned attempts = 0;
		while(ok && attempts < 100) {
			attempts++;
			Vector2 sample(rnd(), rnd());
			
			if(PoinssonHelper(samples, sample, radius, result)) {
				samples[i] = sample;
				result++;
				ok = false;
			}
		}
	}
	
	return result;	
} // CreatePoisson()



/**
 * Function for create N new samples by N-rooks method. This function divides 
 * screen plane into N*N cells and computes N new samples.
 * @param Vector2 samples New samples in 1D array,
 * @param unsigned count Number of samples in samples array.
 * @return unsigned Return real number of new samples, which will be created.
 */ 
inline unsigned CreateRooks(Vector2 *samples, const unsigned &count)
{
	// Creates random samples in grid
	for(unsigned i = 0; i < count; i++) {
		samples[i].x(((double)i + rnd()) / (double)(count))
				  .y(((double)i + rnd()) / (double)(count));
	}
	
	// Shuffle samples in the x coord
	for(int i = count - 2; i >= 0; i--) {
		int target = (int)(rnd() * (double)(count));
		float tmp = samples[i+1].x();
		samples[i+1].x(samples[target].x());
		samples[target].x(tmp);
	}
	
	return count;
} // CreateRooks()



/** ***************************************************************************/
/** Implementation of static helper methods for applying filters              */
/** ***************************************************************************/



/**
 * Function for applying basic box filter to all samples in sample array.
 * @param Vector2 samples All samples in 1D array,
 * @param unsigned count Number of samples in samples array.
 * @return void.
 */ 
inline void ApplyBoxFilter(Vector2 *samples, const unsigned &count)
{
	for(unsigned i = 0; i < count; i++) {
		samples[i].x(2.0f * (samples[i].x() - 0.5f))
				  .y(2.0f * (samples[i].y() - 0.5f));
	}
	
	return;
} // ApplyBoxFilter()



/**
 * Function for applying tent filter to all samples in sample array.
 * @param Vector2 samples All samples in 1D array,
 * @param unsigned count Number of samples in samples array.
 * @return void.
 */ 
inline void ApplyTentFilter(Vector2 *samples, const unsigned &count)
{
	for(unsigned i = 0; i < count; i++) {
		float x = samples[i].x();
		float y = samples[i].y();
		
		if(x < 0.5f) {
			samples[i].x(sqrt(2.0f * x) - 1.0f);
		}
		else {
			samples[i].x(1.0f - sqrt(2.0f - 2.0f * x));
		}
		
		if(y < 0.5f) {
			samples[i].y(sqrt(2.0f * y) - 1.0f);
		}
		else {
			samples[i].y(1.0f - sqrt(2.0f - 2.0f * y));
		}
	}
	
	return;
} // ApplyTentFilter()



/**
 * Function for applying cubic spline filter to all samples in sample array.
 * @param Vector2 samples All samples in 1D array,
 * @param unsigned count Number of samples in samples array.
 * @return void.
 */ 
inline void ApplyCubicSplingFilter(Vector2 *samples, const unsigned &count)
{
	for(unsigned i = 0; i < count; i++) {
		samples[i].x(CubicFilterHelper(samples[i].x()))
				  .y(CubicFilterHelper(samples[i].y()));
	}
	
	return;
} // ApplyCubicSplingFilter()



#endif // _AFR_SAMPLING_UTILS_H



/** ***************************************************************************/
/** end of file SamplingUtils.h												  */
/** ***************************************************************************/
