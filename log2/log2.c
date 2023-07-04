#ifndef LOG2_C
#define LOG2_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"
#include "log2.h"

double rlibm34_log2(float x)
{
	union {float f; unsigned int x;} fix = {x};
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

	if(fix.x == 0) { // x is power of 2
		return (double)m;
	}

	fix.x |= 0x3f800000;
	
	union {float f; unsigned int x;} fit = {.x = fix.x & 0x007f0000};
	int FIndex = fit.x >> 16;
	fit.x |= 0x3f800000;

	double f = fix.f - fit.f;
	f *= oneByF[FIndex];
	
	double y = poly[5];
	for(int i=4;i>=0;i--)
	{
		y = multiply(y, f);
		y = add(y, poly[i]);
	}

	y = add(y, logbase2[FIndex]);
	y = add(y, m);

	return y;
}

#endif
