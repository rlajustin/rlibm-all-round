#ifndef LOGF_C
#define LOGF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"
#include "log.h"

double rlibm34_log1p(float x)
{
	union {float f; unsigned int x;} fix = {x};
	double val = (double)x;
	int exp = ((fix.x & 0x7FFFFFFF) >> 23) - 127;
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
	else if(exp < -7)
	{
		//poly eval

	}
	else if(exp >= 45)
	{
		val = val + 1.0;
	}

	union {double d; unsigned long long x;} dbl = {val}, bigF = {val};
	exp = ((dbl.x & 0x7FFFFFFFFFFFFFFF) >> 52) - 1023;
	bigF.x |= 0x3FF0000000000000;
	dbl.x  |= 0x3FF0000000000000;
	bigF.x &= 0x3FFFE00000000000;
	dbl.x  &= 0x3FFFFFFFFFFFFFFF;

	int FIndex = (bigF.x & 0x000FE00000000000) >> 45;
	double f = (dbl.d - bigF.d);
	f = multiply(f, oneByF[FIndex]);



	

	f = multiply(f, oneByF[FIndex]); // through testing, we find that this can be done normally and still work for all rounding modes
	

	return y;
}
#endif
