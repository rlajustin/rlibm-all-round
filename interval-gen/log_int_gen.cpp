#ifndef _LOG_INTERVAL
#define _LOG_INTERVAL
#include <iostream>
#include <fenv.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"
#include "helper.h"
#include "sorting.h"

#define LO 0x0
#define HI 0x100000000

using namespace std;

bool ComputeSpecialCase(float x, float& res)
{
	Float fx;
	fx.f = x;
	if (x == 0.0) {
		res = -1.0/0.0;
		return true;
	} else if (x == 1.0) {
		res = 0.0;
		return true;
	} else if (fx.x == 0x7F800000) {
		res = x;
		return true;
	} else if (fx.x > 0x7F800000) {
		fx.x = 0x7FFFFFFF;
		res = fx.f;
		return true;
	}

	return false;
}

double RangeReduction(float x) {
	Float fix, fit;

	int m = 0;
	fix.f = x;
	if(fix.x < 0x800000) {
		fix.f *= 8388608.0;
		m-=23;
	}
	m+=fix.x>>23;
	m-=127;
	fix.x &= 0x007FFFFF;
	fix.x |= 0x3F800000;

	fit.x = fix.x & 0x007F0000;
	int FIndex = fit.x >> 16;
	//printf("findex: %d\n", FIndex);
	fit.x |= 0x3F800000;
	double F = fit.f;
	double f = fix.f-F;
	return f*oneByF[FIndex];
}

double OutputCompensation(float x, double yp) {
	Float fix, fit;

	int m = 0;
	fix.f = x;
	if(fix.x < 0x800000) {
		fix.f *= 8388608.0;
		m-=23;
	}

	m+=fix.x>>23;
	m-=127;
	fix.x &= 0x007FFFFF;
	fix.x |= 0x3F800000;

	fit.x = fix.x & 0x007F0000;
	int FIndex = fit.x >> 16;
	return yp + lnF[FIndex] + m * LN2;
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
		printf("usage: %s <oracle file> <destination folder>\n",argv[0]);
		exit(0);
	}

	int original = fegetround();
	fesetround(FE_TOWARDZERO);

	OneIntervalFile(argv);

	fesetround(original);
	return 0;
}
#endif
