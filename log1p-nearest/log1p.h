#ifndef LOG1P_H
#define LOG1P_H

#include "constants.h"

double roundto34bit(double d)
{
	union {double d; unsigned long long x;} val, ret;
	val.d=d;
	int exp = ((val.x&0x7FF0000000000000) >> 52) - 1023;
	int bits = 1+11+24;
	if(exp<-126) //subnormal
	{
		bits += (exp+126);
	}
	if(bits<0) return 0.0;

	ret.x=(val.x&(0xFFFFFFFFFFFFFFFF<<(64-bits)));
	ret.x |= (1L<<(63-bits));

	return ret.d;
}

double poly_eval(double x){
	for(int i=0;i<15;i++)
	{
		if(x==special[i].x) return special[i].y;
	}
	double res = 0.0;
	double* poly = polynegative;
	if(x>0.0) poly = polypositive;
	for(int i=6;i>0;i--)
	{
		res += poly[i];
		res *= x;
	}
	res += poly[0];
	return res;

}

double rlibm34_log1p(float x)
{
	union {float f; unsigned int i;} F = {x};
	unsigned int ix = F.i;
	int exp = ((ix & 0x7fffffff) >> 23) - 127;
	if(ix == 0x7f800000 || x==0.0 || (ix<<1) > 0xff000000) return x;	// positive infinity, log1p(x)=x, NaN
	else if(ix <= 0x33b504f3)
	{
		union {double d; unsigned long long x;} dbl = {x};
		dbl.x--;
		return roundto34bit(dbl.d);
	}
	else if(ix <= 0xb3b504f2 && ix >= 0x80000000)
	{
		union {double d; unsigned long long x;} dbl = {x};
		dbl.x++;
		return roundto34bit(dbl.d);
	}
	else if(ix >= 0xbf800000)
	{
		if(x==-1.0) return x/0.0f;
		else return (x-x)/0.0f;
	}
	else if(exp < -7) return roundto34bit(poly_eval((double)x));
	double val = (double)x + 1.0;
	union {double d; unsigned long long x;} dbl = {val}, bigF = {val};
	exp = ((dbl.x & 0x7FFFFFFFFFFFFFFF) >> 52) - 1023;
	bigF.x |= 0x3FF0000000000000;
	bigF.x &= 0x3FFFE00000000000;
	dbl.x  |= 0x3FF0000000000000;
	dbl.x  &= 0x3FFFFFFFFFFFFFFF;
	int ind = (bigF.x & 0x000FE00000000000) >> 45;
	val = (dbl.d-bigF.d) * oneByF[ind];
	double retval=poly_eval(val);
	return roundto34bit(retval + lnF[ind] + (exp * LN2HIGH));
}

#endif
