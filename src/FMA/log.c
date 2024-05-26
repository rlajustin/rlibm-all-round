#ifndef LOGF_C
#define LOGF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

double rlibm34_logf(float x)
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
	
	double fsquare = multiply(f, f);
	double y = 0.0;

	if(f < 0x1.79f31219dbcc3p-9) {
		if(f == 0x1.7096969696969p-11) y = 0x1.707567c76c2p-11;
		else if(f == 0x1.67f6db6db6db6p-10) y = 0x1.67b7a57466p-10;
		else if(f == 0x1.d6f7e432f7e42p-10) y = 0x1.d68bb6f7c8p-10;
		else if(f == 0x1.ebdfbf7efdfbfp-10) y = 0x1.eb69c2c46f6p-10;
		else if(f == 0x1.1f7a3bf6c657ap-9) y = 0x1.1f29a54ca12p-9;
		else if(f == 0x1.57497b425ed09p-9) y = 0x1.56d6991a2fep-9;
		else if(f == 0x1.6ab366cd9b366p-9) y = 0x1.6a33296237bp-9;
		else if(f == 0x1.78d3dcb08d3dcp-9) y = 0x1.78496eb2accp-9;
		else {
			double coeffs[4] = {0x1.fffffffffffcdp-1, -0x1.ffffff5d2d35dp-2, 0x1.555267302d338p-2, -0x1.f90351975c4dap-3};
			double temp1 = fma(f, coeffs[2], coeffs[1]);
			double temp2 = fma(fsquare, coeffs[3], temp1);
			double temp3 = multiply(fsquare, temp2);
			y = fma(f, coeffs[0], temp3);
		}
	}
	else {
		if(f == 0x1.7b4a2b10bf66dp-9) y = 0x1.7abdf3201p-9;
		else if(f == 0x1.bab0df6b0df6ap-9) y = 0x1.b9f1db48f18p-9;
		else if(f == 0x1.be41a41a41a41p-9) y = 0x1.bd7f8c2ea534p-9;
		else if(f == 0x1.e05c28f5c28f5p-9) y = 0x1.df7b5d64657p-9;
		else if(f == 0x1.e8a1fd1b7af01p-9) y = 0x1.e7b9668c4bp-9;
		else if(f == 0x1.1d085fb37072dp-8) y = 0x1.1c6a27473698p-8;
		else if(f == 0x1.3155555555555p-8) y = 0x1.309fcf6433c8p-8;
		else if(f == 0x1.9c0a2c145828bp-8) y = 0x1.9abff5d8cb92p-8;
		else {
			double coeffs[4] = {0x1.fffffffed9792p-1, -0x1.fffff8985b2c3p-2, 0x1.554cab9fba2bp-2, -0x1.f7494631f6a56p-3};
			double temp1 = fma(f, coeffs[2], coeffs[1]);
			double temp2 = fma(fsquare, coeffs[3], temp1);
			double temp3 = multiply(fsquare, temp2);
			y = fma(f, coeffs[0], temp3);
		}
	}
	y = add(y, lnF[FIndex]);
	double extra = multiply(m, LN2);
	y = add(y, extra);

	return y;
}
#endif
