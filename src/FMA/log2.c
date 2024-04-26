#ifndef LOG2F_C
#define LOG2F_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"


static double coeffs[] = {
	0x1.71547652bcbe6p+0,
	-0x1.715476937c465p-1,
	0x1.ec718e128e52ep-2,
	-0x1.71fc5a5edbe33p-2,
	0x1.4d95a4703311cp-2
};


double rlibm34_log2f(float x)
{
	union {float f; unsigned int x;} fix = {.f = x};
	int m = 0;

	if(fix.x < 0x800000 || fix.x >= 0x7F800000) {
		if((fix.x & 0x7FFFFFFF) == 0) { // log(0)= -infty
			fix.x = 0xFF800000;
			return fix.f;
		}
		if(fix.x > 0x7FFFFFFF) { // log(negative) = NaN
			return (x-x)/0.0;
		}
		if(fix.x >= 0x7F800000) {// log(NaN)=NaN, log
			return x;
		}
		int exp;
		union{float f; unsigned int x;} spec = {frexpf(fix.f, &exp)};
		if(spec.x == 0x3f000000) return (double)(exp-1); /// x is power of 2
		fix.f *= 8388608.0;
		m-=23;
	}
	
	m+= fix.x >> 23;
	m -= 127;
	fix.x &= 0x007fffff;
/*
	if(fix.x == 0) { // x is power of 2
		return (double)m;
	}
*/
	fix.x |= 0x3f800000;
	
	union {float f; unsigned int x;} fit = {.x = fix.x & 0x007f0000};
	int FIndex = fit.x >> 16;
	fit.x |= 0x3f800000;

	double f = fix.f - fit.f; // exact
	f = multiply(f, oneByF[FIndex]);

	double fsquare = multiply(f,f);
	double temp1 = rndz_fma(f, coeffs[4], coeffs[3]);
	double temp2 = rndz_fma(f, coeffs[2], coeffs[1]);
	double temp3 = rndz_fma(fsquare, temp1, temp2);
	double temp4 = multiply(fsquare, temp3);
	double temp5 = rndz_fma(f, coeffs[0], temp4);
	double temp6 = add(temp5, logBase2[FIndex]);

	return add(temp6, m);
}

#endif
