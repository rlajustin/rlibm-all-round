#ifndef LOG1PF_C
#define LOG1PF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

double rlibm34_log1pf(float x)
{
	union {float f; unsigned int x;} fix = {x};
	double val = (double)x;
	int exp = ((fix.x & 0x7FFFFFFF) >> 23) - 127;
	int FIndex = -1;
	double y = 0.0;
	unsigned int ix = fix.x;
	if(ix == 0x7f800000 || x==0.0 || (ix<<1) > 0xff000000) return x;        // positive infinity, log1p(x)=x, NaN
	else if(ix <= 0x33b504f3)
	{
		union {double d; unsigned long long x;} dbl = {x};
		dbl.x--;
		return dbl.d;
	}
	else if(ix <= 0xb3b504f2 && ix >= 0x80000000)
	{
		union {double d; unsigned long long x;} dbl = {x};
		dbl.x++;
		return dbl.d;
	}
	else if(ix >= 0xbf800000)
	{
		if(x==-1.0) return x/0.0f;
		else return (x-x)/0.0f;
	}
	else if(exp < -7) // near zero
	{
		if(x == -0x1.3d0d92p-8) return -0x1.3dd289fffffffp-8;
		else if(x == -0x1.c9288p-13) return -0x1.c93542fffffffp-13;
		else if(x == -0x1.dfff6ap-17) return -0x1.e0004afffffffp-17;
		else if(x == -0x1.7fffap-17) return -0x1.80002ffffffffp-17;
		double coeffs[6] = {0x1.0000000000001p+0, -0x1.fffffffff5b1dp-2, 0x1.5555555e6cfe9p-2, -0x1.0000175bed577p-2, 0x1.9995eb030f364p-3, -0x1.4854ca2fbd0cfp-3};
		double retval = multiply(coeffs[5], x);
		for(int i=4;i>=0;i--)
		{
			retval = add(retval, coeffs[i]);
			retval = multiply(retval, x);
		}
		return retval;
	}
	else if(exp >= 45) // log1p(x) = log(x)
	{
		fix.x &= 0x007fffff;
		fix.x |= 0x3f800000;

		union {float f; unsigned int x;} fit = {.x = fix.x & 0x007f0000};
		FIndex = fit.x >> 16;
		fit.x |= 0x3f800000;

		double f = fix.f - fit.f; // exact
		f = multiply(f, oneByF[FIndex]);

		double fsquare = multiply(f, f);

		if(f == 0x1.57497b425ed09p-9) y = 0x1.56d6991a2fep-9;
		else if(f == 0x1.e8a1fd1b7af01p-9) y = 0x1.e7b9668c4bp-9;
		else {
			double coeffs[4] = {0x1.ffffffffb9b65p-1, -0x1.fffffce4ddf6ap-2, 0x1.5550131c233eep-2, -0x1.f8f5d100b5786p-3};

			double tmp1 = multiply(coeffs[1], f);
			double tmp2 = multiply(coeffs[3], f);

			double tmp3 = add(tmp1, coeffs[0]);
			double tmp4 = add(tmp2, coeffs[2]);

			double tmp5 = multiply(tmp4, fsquare);

			double tmp6 = add(tmp3, tmp5);

			y = multiply(tmp6, f);
		}
	}
	else { // work on this piece more, can reduce number of violated
		val = add(val, 1.0);
		union {double d; unsigned long long x;} dbl = {val}, bigF = {val};
		exp = ((dbl.x & 0x7FFFFFFFFFFFFFFF) >> 52) - 1023;
		bigF.x |= 0x3FF0000000000000;
		dbl.x  |= 0x3FF0000000000000;
		bigF.x &= 0x3FFFE00000000000;
		dbl.x  &= 0x3FFFFFFFFFFFFFFF;

		FIndex = (bigF.x & 0x000FE00000000000) >> 45;
		double f = dbl.d - bigF.d;
		f = multiply(f, oneByF[FIndex]);

		/*	if(f == -0x1.1fff94p-16)  y = -0x1.200035fffffffp-16;
			else if(f == -0x1.dfff6ap-17) y = -0x1.e0004afffffffp-17;
			else if(f == -0x1.7fffap-17) y = -0x1.80002ffffffffp-17;
			else if(f == -0x1.1fffcap-17) y = -0x1.20001afffffffp-17;
			else if(f == -0x1.7fffdp-18) y = -0x1.800017fffffffp-18;
			else if(f == -0x1.7fffe8p-19) y = -0x1.80000bfffffffp-19;
			else if(f == -0x1.7ffff4p-20) y = -0x1.800005fffffffp-20;
			else if(f == -0x1.7ffffap-21) y = -0x1.800002fffffffp-21;
			else */
		if(f == 0x1.7e9a895da895cp-11) y = 0x1.7e76d0b79p-11;
		else if(f == 0x1.af4aaaaaaaaaap-10) y = 0x1.aeeff093588p-10;
		else if(f == 0x1.858f145145144p-9) y = 0x1.84fb2c49ac4p-9;
		else if(f == 0x1.9e2f5d75d75d6p-9) y = 0x1.9d883049ac4p-9;
		else if(f == 0x1.ebbc104104103p-9) y = 0x1.ead08449ac4p-9;
		else {

			double coeffs[6] = {0x1.0000000000001p+0, -0x1.ffffffffff6a3p-2, 0x1.5555555b27cacp-2, -0x1.000005eaf17f9p-2, 0x1.99975554202e5p-3, -0x1.504e880535f9cp-3};
			y = multiply(coeffs[5], f);
			for(int i=4;i>=0;i--)
			{
				y = add(y, coeffs[i]);
				y = multiply(y,f);
			}
		}
	}
	y = add(y, lnF[FIndex]);
	double extra = multiply(exp, LN2);
	return add(y, extra);
}
#endif
