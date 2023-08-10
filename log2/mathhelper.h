#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <stdbool.h>
#include <math.h>

typedef union {
	double d;
	unsigned long long x;
} Double;

static inline double multiply(double a, double b)
{
	double x = a*b;
	Double retval;
	retval.d = x;
	Double compare = {fma(a,b,-x)};
	//handle when compare is positive or negative 0.0
	retval.x -= (((compare.x ^ retval.x) > 0x8000000000000000) &&
			((compare.x<<1)!=0x0));
	return retval.d;
}

static inline double add(double a, double b)
{
	union {double d; unsigned long x;} ba = {a}, bb = {b}, retval = {a+b};
	int alb = ((ba.x & 0x7FFFFFFFFFFFFFFF) < (bb.x & 0x7FFFFFFFFFFFFFFF));
	unsigned long mask = ba.x ^ bb.x;

	ba.x = ba.x ^ (mask * alb);
	bb.x = bb.x ^ (mask * alb); 

	ba.d = retval.d - ba.d;

	bb.d -= ba.d;

	retval.x -= (((bb.x ^ retval.x)>0x8000000000000000) &&
			((bb.x<<1)!=0x0));
	return retval.d;
}


#endif
