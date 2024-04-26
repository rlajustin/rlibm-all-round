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

	if(f < 0x1.789107b0e4883p-9) {
		if(f == 0x1.7096969696969p-11) y = 0x1.707567c76c2p-11;
		else if(f == 0x1.67f6db6db6db6p-10) y = 0x1.67b7a57466p-10;
		else if(f == 0x1.d6f7e432f7e42p-10) y = 0x1.d68bb6f7c8p-10;
		else if(f == 0x1.1f7a3bf6c657ap-9)  y = 0x1.1f29a54ca12p-9;
		else if(f == 0x1.4f89898989898p-9)  y = 0x1.4f1bc6ab118p-9;
		else if(f == 0x1.57497b425ed09p-9)  y = 0x1.56d6991a2fep-9;
		else if(f == 0x1.6ab366cd9b366p-9)  y = 0x1.6a33296237bp-9;
		else {
			double coeffs[4] = {0x1.fffffffffffd1p-1, -0x1.ffffff9340e1ep-2, 0x1.55534dcac44c4p-2, -0x1.faa7ea7154c22p-3};

			double tmp1 = multiply(coeffs[1], f);
			double tmp2 = multiply(coeffs[3], f);

			double tmp3 = add(tmp1, coeffs[0]);
			double tmp4 = add(tmp2, coeffs[2]);

			double tmp5 = multiply(tmp4, fsquare);

			double tmp6 = add(tmp3, tmp5);

			y = multiply(tmp6, f);
		}
	}
	else {
		if(f == 0x1.78d3dcb08d3dcp-9) y = 0x1.78496eb2accp-9;
		else if(f == 0x1.7b4a2b10bf66dp-9) y = 0x1.7abdf3201p-9;
		else if(f == 0x1.91efdb195e8efp-9) y = 0x1.91526927082p-9;
		else if(f == 0x1.be41a41a41a41p-9) y = 0x1.bd7f8c2ea534p-9;
		else if(f == 0x1.e05c28f5c28f5p-9) y = 0x1.df7b5c64657p-9;
		else if(f == 0x1.e8a1fd1b7af01p-9) y = 0x1.e7b9668c4bp-9;
		else if(f == 0x1.3155555555555p-8) y = 0x1.309fcf6433c8p-8;
		else if(f == 0x1.9c0a2c145828bp-8) y = 0x1.9abff5d8cb92p-8;
		else {
			double coeffs[4] = {0x1.fffffffed98b2p-1, -0x1.fffff898c2121p-2, 0x1.554cabfd15d83p-2, -0x1.f7497a800d667p-3};

			double tmp1 = multiply(coeffs[1], f);
			double tmp2 = multiply(coeffs[3], f);

			double tmp3 = add(tmp1, coeffs[0]);
			double tmp4 = add(tmp2, coeffs[2]);

			double tmp5 = multiply(tmp4, fsquare);

			double tmp6 = add(tmp3, tmp5);

			y = multiply(tmp6, f);
		}
	}
	y = add(y, lnF[FIndex]);
	double extra = multiply(m, LN2);
	y = add(y, extra);

	return y;
}
#endif
