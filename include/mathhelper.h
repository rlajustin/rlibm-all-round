#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "int_floats.h"

/*
   A double value rnd(x) for any rounding mode for any real number x must be adjusted to obtain rndz(x) when rnd(x) != rndz(x). 
   rnd(x) != rndz(x) when x cannot be exactly represented as a double and the given rnd function results in a 
   rounding away from zero.
 */

/*
static inline double multiply(double a, double b) { // NEW
//	   In the case that x = a*b, rounding away from zero results in |a*b| < |rnd(a*b)|.
//	   In other words, rnd(x) needs to be adjusted to obtain rndz(x) if and only if 
//	   |a*b| - |rnd(a*b)| = |a|*|b| - |rnd(a*b)| < 0.
	//Set retval.d = rnd(a*b);
	Double retval = { .d = a*b};
	//Set compare.d = |a|*|b|- rnd(|a*b|)
	Double compare = { .d = __builtin_fma(__builtin_fabs(a),__builtin_fabs(b),-__builtin_fabs(retval.d))};
	//If compare.d is negative (including negative zero), that implies round away from zero occurred and vice versa.
	//Subtract 2^l (where l is the exponent of the LSB of rnd(x)) from |rnd(x)| in the case that compare.d is negative.
	retval.x -= (compare.x > 0x8000000000000000);
	return retval.d;
}

static inline double add(double a, double b) { // NEW
//	   In the case that x = a+b (assuming a>=b), rounding away from zero results in different behaviors depending
//	   on whether or not the signs of a and b are equal.
//	   In the case that a and b have the same sign, rounding away from zero will result in rnd(a+b) = a+b+e where
//	   e = (-1)^s*2^l, with s=1 if a<0 and 0 otherwise and l being the exponent of the LSB of a.
//	   Note that in this case a, b, and e all share the same sign.
//	   In such a case, |rnd(a+b)-a| = |a+b+e-a| = |b+e| > |b| (since b and have the same sign). 
//	   In other words, when sign(a) = sign(b), |rnd(a+b)-a| > |b| indicates rounding away from zero occurred.
//	   In the case that a and b have different signs, rounding away from zero will result in rnd(a+b) = a+b+e where
//	   e = (-1)^s*2^l, with s=1 if a<0 and 0 otherwise and l being the exponent of the LSB of a.
//	   Note that in this case b's sign differs from the sign of a and consequently that of e.
//	   In such a case, |rnd(a+b)-a| = |a+b+e-a| = |b+e| < |b| (since b and e have different signs). 
//	   In other words, when sign(a) != sign(b) |rnd(a+b)-a| < |b| indicates rounding away from zero occurred.
	//Set retval.d, dA.d, dB.d as rnd(a+b), a, and b respectively.
	Double retval = { .d = a+b};
	Double dA = { .d = a};
	Double dB = { .d = b};
	Double tempdA = { .d = a};
	//Set cs to be 1 if a and b's signs are the same and 0 other wise.
	int cs = ((dA.x^dB.x)<0x8000000000000000);
	//Swap dA.d and dB.d in the case that |dA.d|<|dB.d| (and keep them as they are in the case that |dA.d|>=|dB.d|). 
	unsigned long long ca = ~(0xffffffffffffffff+(__builtin_fabs(dA.d)>=__builtin_fabs(dB.d)));
	dA.x = (ca&dA.x)|(~ca&dB.x);
	dB.x = (ca&dB.x)|(~ca&tempdA.x);
	//Adjust the value of retval.x if either one of these conditions holds true.
	//1. a and b have the same sign and |rnd(a+b)-a| > |b|.
	//2. a and b have different signs and |rnd(a+b)-a| < |b|.
	retval.x -= ((cs&(__builtin_fabs(retval.d-dA.d)>__builtin_fabs(dB.d)))|(!cs&(__builtin_fabs(retval.d-dA.d)<__builtin_fabs(dB.d))));
	return retval.d;
}
*/

static inline double multiply(double a, double b) // OLD
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

static inline double add(double a, double b) // OLD
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

/*
static inline double add(double a, double b) {		//ALT OLD
	union{double d; unsigned long x;} ba = {.d=a}, bb = {.d=b}, retval = {.d=a+b};
	int alb = ((ba.x & 0x7FFFFFFFFFFFFFFF) < (bb.x & 0x7FFFFFFFFFFFFFFF));
	unsigned long mask = ba.x ^ bb.x;
	ba.x = ba.x ^ (mask * alb);
	bb.x = bb.x ^ (mask * alb);
	ba.d = retval.d - ba.d;
	bb.d -= ba.d;
	retval.x -= ((bb.d*retval.d)<0.0);
	return retval.d;
}

static inline double multiply(double a, double b) {	//ALT OLD
	double x = a*b;
	union{double d; unsigned long x;} retval;
	retval.d = x;
	union{double d; unsigned long x;} compare = {.d=__builtin_fma(a,b,-x)};
	retval.x -= ((compare.d*retval.d)<0.0);
	return retval.d;
}
*/

static inline double divide(double a, double b) // a/b
{
	Double retval = {.d = a/b};
	Double compare = {.d = __builtin_fma(retval.d, -b, a)};
	// If compare is negative, retval should be decremented
	retval.d -= (compare.x >= 0x8000000000000000);
	return retval.d;
}

static inline double rndz_sqrt(double a)
{
	//ASSERT a is positive
	Double retval = {.d = __builtin_sqrt(a)};
	Double compare = {.d = __builtin_fma(retval.d, -retval.d, a)};
	// If compare is negative, then retval should be decremented, as it currently stores an overestimate for sqrt(a)
	retval.x -= (compare.x >= 0x8000000000000000);
	return retval.d;
}

static inline double rndz_fma(double a, double b, double c)
{
	Double retval = {.d = __builtin_fma(a,b,c)};
	Double compare = {.d = __builtin_fma(-a,b,retval.d)};
	retval.x -= (((retval.d < 0.0) && (compare.d < c))||((retval.d >= 0.0) && (compare.d > c)));
	return retval.d;
}
#endif
