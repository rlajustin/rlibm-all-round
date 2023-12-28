//TODO MAKE THIS AUTOMATIC

#ifndef _LOG1P_INTERVAL
#define _LOG1P_INTERVAL
#include <iostream>
#include <fenv.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "constants.h"
#include "helper.h"

#define LO 0x0
#define HI 0x100000000

using namespace std;

typedef struct IInterval{
	double lo;
	double hi;
} Interval;

bool ComputeSpecialCase(float x, float& res)
{
	Float fx;
	fx.f = x;
	if (x == -1.0) {
		res = -1.0/0.0;
		return true;
	} else if (fx.x == 0x7F800000) { // x = infinity
		res = x;
		return true;
	} else if (fx.x == 0xbf800000) {
		res = 0xff800000;
		return true;
	} else if (fx.x <= 0x33b504f3) { // log1p(x)=x
		return true;
	} else if (fx.x >= 0x80000000 and fx.x <= 0xb3b504f2) { // log1p(x)=x
		return true;
	} else if (fx.x > 0xbf800000) { // x < -1.0
		fx.x = 0xFFFFFFFF;
		res = fx.f;
		return true;
	} else if (fx.x > 0x7F800000 and fx.x < 0x80000000) { // x is NaN
		fx.x = 0x7FFFFFFF;
		res = fx.f;
		return true;
	} else if ((fx.x & 0x7FFFFFFF) < 0x3c000000) { // directly computed
		return true;
	} else if (fx.x >= 0x56000000 and fx.x < 0x80000000) { // log1p(x)=log(x)
		return true;
	} /*else if ((fx.x & 0x7FFFFFFF) >= 0x3c000000) {
		return true;
	}*/
	return false;
}

double RangeReduction(float x) {
	Float fix, F;
	fix.f = x;
	
	int m = ((fix.x & 0x7fffffff) >> 23) - 127;
	if(m < -7) return x;

	fix.x &= 0x007fffff;
	fix.x |= 0x3f800000;

	F.x = fix.x & 0x007f0000;
	int FIndex = F.x >> 16;
	F.x |= 0x3f800000;
	double f = fix.f - F.f;
	return f * oneByF[FIndex];
	
}

double OutputCompensation(float x, double yp) {
	Float fix, fit;
	fix.f = x;

	int m = ((fix.x & 0x7fffffff) >> 23) - 127;

	if(m < -7) return yp;
	double round = (double)x;
	if(m < 45) round += 1.0;

	Double d, p1;
	p1.d = round;
	d.d = round;
	d.x |= 0x3ff0000000000000;
	d.x &= 0x3fffe00000000000;
	p1.x|= 0x3ff0000000000000;
	p1.x&= 0x3fffffffffffffff;
	double f = p1.d - d.d;

	p1.x &= 0x000fe00000000000;
	int findex = p1.x >> 45;

	d.d = (double)x + 1.0;
	m = ((d.x & 0x7fffffffffffffff) >> 52) - 1023;
	return yp + lnF[findex] + (m * LN2);
}

void GuessInitialLbUb(float x, double roundingLb, double roundingUb,
		double xp, double& lb, double& ub) {
	Double tempYp;
	tempYp.d = log1p(xp);
	double tempY = OutputCompensation(x, tempYp.d);


	if (tempY < roundingLb) {
		do {
			if (tempYp.d >= 0.0) tempYp.x++;
			else tempYp.x--;

			tempY = OutputCompensation(x, tempYp.d);
		} while (tempY < roundingLb);

		if (tempY > roundingUb) {
			printf("Error during GuessInitialLbUb: lb > ub.\n");
			printf("x = %.100e\n", x);
			exit(0);
		}
		lb = tempYp.d;
		ub = tempYp.d;
		return;
	}

	if (tempY > roundingUb) {
		do {
			if (tempYp.d >= 0.0) tempYp.x--;
			else tempYp.x++;

			tempY = OutputCompensation(x, tempYp.d);
		} while (tempY > roundingUb);

		if (tempY < roundingLb) {
			printf("Error during GuessInitialLbUb: lb > ub.\n");
			printf("x = %.100e\n", x);
			exit(0);
		}
		lb = tempYp.d;
		ub = tempYp.d;
		return;
	}

	lb = tempYp.d;
	ub = tempYp.d;
	return;
}

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		printf("usage: %s <oracle file> <interval folder>\n",argv[0]);
		exit(0);
	}

	int original = fegetround();
	fesetround(FE_TOWARDZERO);
	
	OneIntervalFile(argv);

	fesetround(original);

	return 0;
}
#endif
