#ifndef LOG2_C
#define LOG2_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

static double coeffs[] = {
	0x1.71547652bd049p+0,
	-0x1.7154769772017p-1,
	0x1.ec719b8bb516dp-2,
	-0x1.7204c611c8824p-2,
	0x1.4f5a329d5ba62p-2
};


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
/* this evaluation results in a function that is 1.5x slower (~110 vs 70 clock cycles)
	double fsquare = multiply(f,f);
	double tmp1 = multiply(coeffs[2], f);
	double tmp2 = multiply(coeffs[4], f);

	double ffour = multiply(fsquare, fsquare);
	double tmp3 = add(tmp1, coeffs[1]);
	double tmp4 = add(tmp2, coeffs[3]);

	double tmp5 = multiply(tmp3, fsquare);
	double tmp6 = multiply(tmp4, ffour);
	double tmp7 = multiply(coeffs[0], f);
	
	double tmp8 = add(tmp5, tmp7);
	double extra = add(logBase2[FIndex], m);
	
	double y = add(tmp8, tmp6);
*/	
	double extra = add(logBase2[FIndex], m);
	double y = multiply(coeffs[4], f);
	double fsquare = multiply(f, f);

	y = add(y, coeffs[3]);
	double tmp = multiply(coeffs[2], f);

	tmp = add(tmp, coeffs[1]);
	y = multiply(y, fsquare);

	tmp = multiply(tmp, f);

	tmp = add(tmp, coeffs[0]);
	y = multiply(y, fsquare);

	tmp = multiply(tmp, f);

	y = add(y, tmp);

	return add(y, extra);
}

#endif
