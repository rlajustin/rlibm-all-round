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

double smallvals(float f)
{
	Double val;
	Float fl;
	fl.f=f;
	fl.x--;
	val.d = (double)f;
	val.x--;
	int exp = (val.x&0x7FF0000000000000) >> 52;
	exp -= 1023;
	int precbits = 1+11+25;
	if(exp<-126)
	{
		precbits += (exp+126);
	}
	val.x &= (0xFFFFFFFFFFFFFFFF<<(64-precbits));
	return val.d;
}

/*
double rlibm_reduce_input(float x)
{
	Float fit;	
}
*/

#endif
