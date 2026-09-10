/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    RotationInfo.cpp												  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class RotationInfo.							  *
 **************************************************************************** */


// Including project libraries
#include "RotationInfo.h"



/**
 * Default constructor. Initialize squads count and squads array.
 * @return RotationInfo.
 */ 
RotationInfo::RotationInfo()
{
	this->count = 0;
	this->squads = NULL;
} // RotationInfo()



/**
 * Default destructor. Delete squads array.
 */ 
RotationInfo::~RotationInfo() 
{
	if(this->squads != NULL) {
		delete [] this->squads;
		this->squads = NULL;
	}
	
	this->count = 0;
} // ~RotationInfo()



/**
 * Method for initialize rotation info from rotation keys and creates squads
 * array for other use in interpolate method.
 * @param TRotationKey keys Array of rotation keys from AFF file,
 * @param unsigned count Count of rotation keys.
 * @return RotationInfo Returns RotationInfo for provides fluent interface.
 */ 
RotationInfo *RotationInfo::Initialize(TRotationKey *(&keys), const unsigned &count)
{
	double omt0, omc0, opc0, omb0, opb0, adj0, out0, out1;
    double omt1, omc1, opc1, omb1, opb1, adj1, in0, in1;
    unsigned i0 = 0, i1 = 1, i2 = 2, i3 = 3;
    Quaternion q0, q1, q2, q3;
    Quaternion prod;
    Quaternion logdq, Tout, Tin, arg0, arg1;
	
	this->count = count - 3;
	this->squads = new TSquadInfo[this->count];
	
	for (/**/; i0 < this->count; i0++, i1++, i2++, i3++)
    {
		keys[i0].rot >> q0;
		keys[i1].rot >> q1;
		keys[i2].rot >> q2;
		keys[i3].rot >> q3;
		
		if(q1.Dot(q2) < 0.0) {
			q2 = -q2;
	        keys[i2].rot << q2;
        }
		
		prod = q1.UnitInverse() * q2;
		logdq = prod.Log();
		
		omt0 = 1 - keys[i1].tension;
        omc0 = 1 - keys[i1].continuity;
        opc0 = 1 + keys[i1].continuity;
        omb0 = 1 - keys[i1].bias;
		
		opb0 = 1 + keys[i1].bias;
		adj0 = 2 * (keys[i2].time - keys[i1].time) / (keys[i2].time - keys[i0].time);
		out0 = 0.5 * adj0 * omt0 * opc0 * opb0;
		out1 = 0.5 * adj0 * omt0 * omc0 * omb0;
		
		prod = q0.UnitInverse() * q1;
	    Tout = logdq * out1 + prod.Log() * out0;
		
		omt1 = 1 - keys[i2].tension;
		omc1 = 1 - keys[i2].continuity;
		opc1 = 1 + keys[i2].continuity;
		omb1 = 1 - keys[i2].bias;
		opb1 = 1 + keys[i2].bias;
		adj1 = 2 * (keys[i2].time - keys[i1].time) / (keys[i3].time - keys[i1].time);
		in0 = 0.5 * adj1 * omt1 * omc1 * opb1;
		in1 = 0.5 * adj1 * omt1 * opc1 * omb1;
		
		prod = q2.UnitInverse() * q3;
		Tin = prod.Log() * in1 + logdq * in0;
		
		this->squads[i0].p = q1;
        this->squads[i0].q = q2;
		
		arg0 = (Tout-logdq) * 0.5;
		arg1 = (logdq-Tin) * 0.5;
		
		this->squads[i0].a = q1*arg0.Exp();
        this->squads[i0].b = q2*arg1.Exp();
		
		this->squads[i0].tMin = keys[i1].time;
        this->squads[i0].tMax = keys[i2].time;
        this->squads[i0].tRange = this->squads[i0].tMax - this->squads[i0].tMin;
	}
	
	return this;
}  // Initialize()



/**
 * Method for interpolate rotation info at the given time. Returs actual axis
 * and angle for rotation transformation as 4link vector.
 * @param float time.
 * @return Vector4 Returns actual axis and angle in radians for transformation.
 */ 
Vector4 RotationInfo::Interpolate(const float &time)
{
	unsigned i;
	double s;
	Vector4 vec;
	
	if(this->squads[0].tMin < time) {
		if(time < this->squads[this->count-1].tMax) {
			for(i = 0; i < this->count; i++) {
				if(time < this->squads[i].tMax) {
					break;
				}
			}
			
			s = (time - this->squads[i].tMin) / this->squads[i].tRange;
		}
		else {
			i = this->count-1;
			s = 1;
		}
	}
	else {
		i = 0;
		s = 0.0;
	}
	
	TSquadInfo si = this->squads[i];
	Quaternion squad = Quaternion::Squad(s, si.p, si.a, si.b, si.q);	
	vec << squad;
	
	return vec;
}  // Interpolate()



/** ***************************************************************************/
/** end of file RotationInfo.cpp											  */
/** ***************************************************************************/
