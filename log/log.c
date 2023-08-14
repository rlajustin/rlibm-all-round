#ifndef LOGF_C
#define LOGF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"
#include "log.h"

double rlibm34_log(float x)
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
		fix.f *= 8388608.0;
		m-=23;
	}

	m+= fix.x >> 23;
	m -= 127;
	fix.x &= 0x007fffff;
	fix.x |= 0x3f800000;
	
	union {float f; unsigned int x;} fit = {.x = fix.x & 0x007f0000};
	int FIndex = fit.x >> 16;
	fit.x |= 0x3f800000;

	double f = fix.f - fit.f; // exact
	f = multiply(f, oneByF[FIndex]); // through testing, we find that this can be done normally and still work for all rounding modes
	
	union {double d; unsigned long x;} dX = {f};
	unsigned long index = dX.x & 0xFF;
	const double* coeffs = logcoeffs[index];
	
	double extra = m * LN2HIGH;
	double y = multiply(coeffs[4], f);
	y = add(y, coeffs[3]);
	y = multiply(y, f);
	y = add(y, coeffs[2]);
	y = multiply(y, f);
	y = add(y, coeffs[1]);
	y = multiply(y, f);
	y = add(y, coeffs[0]);
	y = multiply(y, f);
	y = add(y, lnF[FIndex]);
	y = add(y, extra);
	
	return y;
}

#endif
