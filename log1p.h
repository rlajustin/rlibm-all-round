#ifndef LOG1P_H
#define LOG1P_H

#include "interval.h"

double polypositive[] = {};
double polynegative[] = {};

double roundto34bit(double d)
{
	Double val,abs;
	val.d=d;
	abs.x=val.x;
	abs.x&=0x7FFFFFFFFFFFFFFF;
	unsigned int exp = (val.x&0x7FF0000000000000) >> 52;
	exp-=1023;
	int bits = 1+11+24;
	if(exp<-126) //subnormal
	{
		bits += (exp+126);
	}
	if(bits<0) return 0.0;

	Double ret;
	ret.x=(val.x&(0xFFFFFFFFFFFFFFFF<<(64-bits)));
	ret.x |= (1L<<(63-bits));

	return ret.d;
}

double rlibm34_log1p(float x)
{
	Float fix, fit;
	fix.f = x;
	int m = 0;
	Double d;
	d.d=(double)x;
	if(x==0.0) return 0.0;
	
}

#endif
