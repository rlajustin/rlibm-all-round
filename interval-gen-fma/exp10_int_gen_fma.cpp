#ifndef _EXP10_INTERVAL_FMA
#define _EXP10_INTERVAL_FMA
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

#include "interval1.h"

using namespace std;

bool ComputeSpecialCase(float x, float& res)
{
	Float fx;
	fx.f = x;
	if((fx.x & 0x7FFFFFFF) == 0) return true; // +-0, return 1.0

	if(fx.x <= 0x335E5BD7) // x small
	{
		if(fx.x <= 0x32DE5BD8) return true; // return 0x1.0000008p+0
		return true; // return 0x1.0000018p+0
	}

	if(fx.x >= 0x421A209B && fx.x <= 0xB2DE5BD9)
	{
		if(fx.x < 0x80000000) {
			if(fx.x <= 0x7f800000) return true; // return 0x1.ffffff8p+127 (inf - 1)
			if(fx.x == 0x7f800000) return true; //return 1.0/0.0
			return true; //return 0.0/0.0
		}

		if(fx.x <= 0xB25E5BD8) return true; //return 0x1.ffffff8p-1
		return true; // return 0x1.fffffe8p-1
	}

	if(fx.x >= 0xC2349E36)
	{
		if(fx.x == 0xff800000) return true; //return 0.0
		else return true; //return 0x1p-151
		//handle NaN case
	}

	switch(fx.x) {
		case 0x00000000: return true; // 1.0
		case 0x80000000: return true; // 1.0
		case 0x3f800000: return true; // 10.0
		case 0x40000000: return true; // 100.0
		case 0x40400000: return true; // 1000.0
		case 0x40800000: return true; // 10000.0
		case 0x40a00000: return true; // 100000.0
		case 0x40c00000: return true; // 1000000.0
		case 0x40e00000: return true; // 10000000.0
		case 0x41000000: return true; // 100000000.0
		case 0x41100000: return true; // 1000000000.0
		case 0x41200000: return true; // 10000000000.0
	}

	return false;
}

double RangeReduction(float x) {
	double xp = x * LG10X64;
	int N = (int)xp;
	int N2 = N % 64;
	if(N2<0) N2 += 64;
	int N1 = N-N2;

	int M = N1/64;
	int J = N2;

	double R = x - N*ONEBY64LG10;
	return R;
}

double OutputCompensation1(float x, double yp) {
	double xp = x * LG10X64;
	int N = (int)xp;
	int N2 = N % 64;
	if(N2<0) N2 += 64;
	int N1 = N-N2;

	int M = N1/64;
	int J = N2;

	double R = x - N*ONEBY64LG10;
	Double dY = {exp2JBy64[J]};
	dY.x += ((uint64_t)M<<52);
	yp *= dY.d;
	return yp;
}

void GuessInitialLbUb1(float x, double roundingLb, double roundingUb,
		double xp, double& lb, double& ub) {
	Double tempYp;
	tempYp.d = exp10(xp);
	double tempY = OutputCompensation1(x, tempYp.d);

	//printf("%.10e, %.20e, %.20e\n", xp, tempY, exp(x));
	//printf("%.20e\n%.20e\n%.20e\n", roundingLb, tempY, roundingUb);

	if (tempY < roundingLb) {
		do {
			if (tempYp.d >= 0.0) tempYp.x++;
			else tempYp.x--;

			tempY = OutputCompensation1(x, tempYp.d);
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

			tempY = OutputCompensation1(x, tempYp.d);
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
