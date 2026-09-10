/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    PositionInfo.cpp												  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class PositionInfo.							  *
 **************************************************************************** */


// Including project libraries
#include "PositionInfo.h"


// Define macros
#define DEGREE 5



/**
 * Default constructor. Initializes both counters and both arrays.
 * @return PositionInfo.
 */ 
PositionInfo::PositionInfo()
{
	this->count = 0;
	this->totalLength = 0;
	this->polyInfo = NULL;
	this->lengths = NULL;
} // RotationInfo()



/**
 * Default destructor.
 */ 
PositionInfo::~PositionInfo() 
{
	if(this->polyInfo != NULL) {
		delete [] this->polyInfo;
		this->polyInfo = NULL;
	}
	
	if(this->lengths != NULL) {
		delete [] this->lengths;
		this->lengths = NULL;
	}
	
	this->count = 0;
	this->totalLength = 0;
} // ~PositionInfo()



/**
 * Method for initializing position info structure from position keys from AFF
 * file. Creates curve with key frames a stores in array.
 * @param TPositionKey keys Position key frames for create curve,
 * @param unsigned count Total count of position keys.
 * @return PositionInfo Returns PositionInfo to provides fluent interface.
 */ 
PositionInfo *PositionInfo::Initialize(TPositionKey *(&keys), const unsigned &count)
{
	float omt0, omc0, opc0, omb0, opb0, adj0, out0, out1;
    float omt1, omc1, opc1, omb1, opb1, adj1, in0, in1;
    Vector3 Tout, Tin;
    Vector3 DP;
	unsigned i0 = 0, i1 = 1, i2 = 2, i3 = 3;
	
	this->count = count - 3;
    this->polyInfo = new TCubicPolynom[this->count];
	
	
	for(/**/; i0 < this->count; i0++, i1++, i2++, i3++) {
		/* build P[i2]-P[i1]; */
        DP[0] = keys[i2].pos.x() - keys[i1].pos.x();
        DP[1] = keys[i2].pos.y() - keys[i1].pos.y();
        DP[2] = keys[i2].pos.z() - keys[i1].pos.z();

        /* build multipliers at point P[i1] */
        omt0 = 1 - keys[i1].tension;
        omc0 = 1 - keys[i1].continuity;
        opc0 = 1 + keys[i1].continuity;
        omb0 = 1 - keys[i1].bias;
        opb0 = 1 + keys[i1].bias;
        adj0 = 2 * (keys[i2].time - keys[i1].time) / (keys[i2].time - keys[i0].time);
        out0 = 0.5 * adj0 * omt0 * opc0 * opb0;
        out1 = 0.5 * adj0 * omt0 * omc0 * omb0;

        /* build outgoing tangent at P[i1] */
        Tout.x(out1 * (DP.x()) + out0 * (keys[i1].pos.x() - keys[i0].pos.x()))
			.y(out1 * (DP.y()) + out0 * (keys[i1].pos.y() - keys[i0].pos.y()))
			.z(out1 * (DP.z()) + out0 * (keys[i1].pos.z() - keys[i0].pos.z()));

        /* build multipliers at point P[i2] */
        omt1 = 1 - keys[i2].tension;
        omc1 = 1 - keys[i2].continuity;
        opc1 = 1 + keys[i2].continuity;
        omb1 = 1 - keys[i2].bias;
        opb1 = 1 + keys[i2].bias;
        adj1 = 2 * (keys[i2].time - keys[i1].time) / (keys[i3].time - keys[i1].time);
        in0 = 0.5 * adj1 * omt1 * omc1 * opb1;
        in1 = 0.5 * adj1 * omt1 * opc1 * omb1;

        /* build incoming tangent at P[i2] */
        Tin.x(in1 *(keys[i3].pos.x() - keys[i2].pos.x()) + in0 * (DP.x()))
		   .y(in1 *(keys[i3].pos.y() - keys[i2].pos.y()) + in0 * (DP.y()))
           .z(in1 *(keys[i3].pos.z() - keys[i2].pos.z()) + in0 * (DP.z()));

        this->polyInfo[i0].C0.x(keys[i1].pos.x())
						     .y(keys[i1].pos.y())
							 .z(keys[i1].pos.z());

        this->polyInfo[i0].C1.x(Tout.x())
							 .y(Tout.y())
							 .z(Tout.z());

        this->polyInfo[i0].C2.x(3*DP.x() - 2*Tout.x() - Tin.x())
							 .y(3*DP.y() - 2*Tout.y() - Tin.y())
							 .z(3*DP.z() - 2*Tout.z() - Tin.z());

        this->polyInfo[i0].C3.x(-2*DP.x() + Tout.x() + Tin.x())
							 .y(-2*DP.y() + Tout.y() + Tin.y())
							 .z(-2*DP.z() + Tout.z() + Tin.z());

        this->polyInfo[i0].tMin = keys[i1].time;
        this->polyInfo[i0].tMax = keys[i2].time;
        this->polyInfo[i0].tRange = this->polyInfo[i0].tMax - this->polyInfo[i0].tMin;
    }
	
    this->CalcArcLength();

    return this;
} // Initialize()



/**
 * First interpolation at given time.
 * @param float time Animation actual time.
 * @return Vector3 Return actual position for transformation.
 */ 
Vector3 PositionInfo::Interpolate0(const float& time)
{
    unsigned i; float u;
    this->CalcUI(time, u, i);
    return this->InterpolateSingle0(this->polyInfo[i], u);
} // Interpolate0()



/**
 * Second interpolation at given time.
 * @param float time Animation actual time.
 * @return Vector3 Return actual position for transformation.
 */ 
Vector3 PositionInfo::Interpolate1(const float& time)
{
    unsigned i; float u;
    this->CalcUI(time, u, i);
    return this->InterpolateSingle1(this->polyInfo[i], u);
} // Interpolate1()



/**
 * Third interpolation at given time.
 * @param float time Animation actual time.
 * @return Vector3 Return actual position for transformation.
 */ 
Vector3 PositionInfo::Interpolate2(const float& time)
{
    unsigned i; float u;
    this->CalcUI(time, u, i);
    return this->InterpolateSingle2(this->polyInfo[i], u);
} // Interpolate2()



/**
 * Method for actual interpolation in given polynom with parameter 'u'.
 * @param TCubicPolynom poly Given polynom to interpolate,
 * @param float u Interpolation parameter.
 * @return Vector3 Return actual position for transformation.
 */ 
Vector3 PositionInfo::InterpolateSingle0(const TCubicPolynom& poly, const float& u)
{
	Vector3 v;

    v.x(poly.C0.x() + u*(poly.C1.x() + u*(poly.C2.x() + u*poly.C3.x())));
    v.y(poly.C0.y() + u*(poly.C1.y() + u*(poly.C2.y() + u*poly.C3.y())));
    v.z(poly.C0.z() + u*(poly.C1.z() + u*(poly.C2.z() + u*poly.C3.z())));
	
	return v;
} // InterpolateSingle0()



/**
 * Method for actual interpolation in given polynom with parameter 'u'.
 * @param TCubicPolynom poly Given polynom to interpolate,
 * @param float u Interpolation parameter.
 * @return Vector3 Return actual position for transformation.
 */ 
Vector3 PositionInfo::InterpolateSingle1(const TCubicPolynom& poly, const float& u)
{ 
	Vector3 v;
	
	v.x(poly.C1.x() + u*(2*poly.C2.x() + u*3*poly.C3.x()));
    v.y(poly.C1.y() + u*(2*poly.C2.y() + u*3*poly.C3.y()));
    v.z(poly.C1.z() + u*(2*poly.C2.z() + u*3*poly.C3.z()));
	
	return v;
} // InterpolateSingle1()



/**
 * Method for actual interpolation in given polynom with parameter 'u'.
 * @param TCubicPolynom poly Given polynom to interpolate,
 * @param float u Interpolation parameter.
 * @return Vector3 Return actual position for transformation.
 */ 
Vector3 PositionInfo::InterpolateSingle2(const TCubicPolynom& poly, const float& u)
{
	Vector3 v;
	
	v.x(2*poly.C2.x() + u*6*poly.C3.x());
    v.y(2*poly.C2.y() + u*6*poly.C3.y());
    v.z(2*poly.C2.z() + u*6*poly.C3.z());
	
	return v;
} // InterpolateSingle2()



/**
 * Method for calculate interpolation parameters 'u' and 'i' in given time.
 * @param float time Given actual animation time,
 * @param float u Interpolation parameter 'u',
 * @param float i Interpolation parameter 'i'.
 * @return PositionInfo Returns this to provides fluent interface.
 */ 
PositionInfo *PositionInfo::CalcUI(const float& time, float& u, unsigned& i)
{
    if(this->polyInfo[0].tMin < time) {
        if(time < this->polyInfo[this->count-1].tMax )
        {
            for(i = 0; i < this->count; i++)
            {
                if ( time < this->polyInfo[i].tMax )
                    break;
            }
            u = (time - this->polyInfo[i].tMin) / this->polyInfo[i].tRange;
        }
        else
        {
            i = this->count-1;
            u = 1.0;
        }
    }
    else
    {
        i = 0;
        u = 0.0;
    }
	
    return this;
} // CalcUI()



/**
 * Method for calculate lengths for each cubic polynom and store in object.
 * @return PositionInfo Returns this to provides fluent interface.
 */ 
PositionInfo *PositionInfo::CalcArcLength()
{
	float ln;

    this->lengths = new float[this->count+1];
    this->lengths[0] = 0.0;

    for (unsigned i = 0; i < this->count; i++) {
        /* length of current polynomial */
        ln = this->CalcLength(this->polyInfo[i], 1.0);
        
        /* total length of curve between poly[0] and poly[i+1] */
        this->lengths[i+1] = this->lengths[i] + ln;
    }

    this->totalLength = this->lengths[this->count];
	
	return this;
} // CalcArcLength()



/**
 * Method for calculate length for actual polynom in curve.
 * @param TCubicPolynom poly Given polynom,
 * @param float u Parameter 'u',
 * @return float Returns length of given polynom.
 */ 
float PositionInfo::CalcLength(const TCubicPolynom& poly, const float& u)
{
	float result = 0.0;
    
	static float modRoot[DEGREE] = {
        /* Legendre roots mapped to (root+1)/2 */
        0.046910077,
        0.230765345,
        0.5,
        0.769234655,
        0.953089922
    };
    static float modCoeff[DEGREE] = {
        /* original coefficients divided by 2 */
        0.118463442,
        0.239314335,
        0.284444444,
        0.239314335,
        0.118463442
    };
	
	for(unsigned i = 0; i < DEGREE; i++) {
        result += modCoeff[i] * this->CalcSpeed(poly, u*modRoot[i]);
	}
	
    return (result*u);
} // CalcLength()



/**
 * Method for calculate speed in given polynom.
 * @param TCubicPolynom poly Given polynom,
 * @param float u Parameter 'u',
 * @return float Returns speed.
 */ 
float PositionInfo::CalcSpeed(const TCubicPolynom& poly, const float& u)
{
    Vector3 velocity = this->InterpolateSingle1(poly, u);
    return velocity.Length();
} // CalcSpeed()



/** ***************************************************************************/
/** end of file PositionInfo.cpp											  */
/** ***************************************************************************/
