#ifndef _COSH_INTERVAL
#define _COSH_INTERVAL
#include <iostream>
#include <fenv.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "constants.h"
#include "helper.h"
#include "sorting.h"
#include "interval.h"

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

	fx.x &= 0x7FFFFFFF;

	if(fx.x <= 0x39FFFFFF) return true;
	if(fx.x >= 0x42B2D4FD) return true;

	return false;
}

double RangeReduction(float x) {
	double xp = x * CONST64BYLN2;
	int N = (int)xp;
	return x - N * LN2BY64;
}

double OutputCompensation(float x, double sinhp, double coshp) {
	double xp = x * CONST64BYLN2;
	int N = (int)xp;
	int N2 = N % 64;
	if(N2 < 0) N2 += 64;
	int N1 = N-N2;
	int I = N1/64;
	double sinhHM = sinhH[I] * coshM[N2] + coshH[I] * sinhM[N2];
	double coshHM = sinhH[I] * sinhM[N2] + coshH[I] * coshH[N2];

	return sinhHM * sinhp + coshHM * coshp;
}

bool GuessInitialLbUb(float x, double totalLB, double totalUB,
			double R,
			double& sinhLB, double& sinhUB,
			double& coshLB, double& coshUB) {
	Double dx, dx1, dx2;

	double xp = x * CONST64BYLN2;
	int N = (int)xp;
	int N2 = N % 64;
	if(N < 0) N2 += 64;
	int N1 = N - N2;
	int I = N1 / 64;
	double sinhHM = sinhH[I] * coshM[N2] + coshH[I] * sinhM[N2];
	double coshHM = sinhH[I] * sinhM[N2] + coshH[I] * coshM[N2];

	double A = cosh(R);
	double B = sinh(R);

	double M1 = totalLB / (sinhHM * B + coshHM * A);
	coshLB = A * M1;
	sinhLB = B * M1;

	if(sinhHM == 0)
	{
		dx.x = 0xFFEFFFFFFFFFFFFF; // -infty+1
		sinhLB = dx.d;
	}
	if(coshHM == 0)
	{
		dx.x = 0xFFEFFFFFFFFFFFFF;
		coshLB = dx.d;
	}

	unsigned long step = 0x1000000000000;
	while(step > 0)
	{
		dx1.d = coshLB;
		dx2.d = sinhLB;

		if(coshHM != 0)
		{
			if(dx1.d >= 0) dx1.x -= step;
			else dx1.x += step;
		}

		if(sinhHM != 0)
		{
			if(dx2.d >= 0) dx2.x -= step;
			else dx2.x += step;
		}

		double recon = sinhHM * dx2.d + coshHM * dx1.d;

		if(recon >= totalLB)
		{
			coshLB = dx1.d;
			sinhLB = dx2.d;
		}
		else if(step > 0)
		{
			step /= 2;
		}
	}

	double recon = sinhHM * sinhLB + coshHM * coshLB;

	while(recon < totalLB)
	{
		if(coshHM != 0)
		{
			dx.d = coshLB;
			if(dx.d >= 0) dx.x++;
			else dx.x--;
			coshLB = dx.d;
		}
		if(sinhHM != 0)
		{
			dx.d = sinhLB;
			if(dx.d >= 0) dx.x++;
			else dx.x--;
			sinhLB = dx.d;
		}
		recon = sinhHM * sinhLB + coshHM * coshLB;
	}

	double M2 = totalUB / (sinhHM * B + coshHM * A);
	coshUB = A * M2;
	sinhUB = B * M2;

	// If SHM == 0, then cosh(R) can be any value.
	if (coshHM == 0) {
		dx.x = 0x7FEFFFFFFFFFFFFF;
		coshUB = dx.d;
	}

	// If CHM == 0, then sinh(R) can be any value.
	if (sinhHM == 0) {
		dx.x = 0x7FEFFFFFFFFFFFFF;
		sinhUB = dx.d;
	}


	step = 0x1000000000000;
	while (step > 0) {
		dx1.d = coshUB;
		dx2.d = sinhUB;

		if (coshHM != 0) {
			if (dx1.d >= 0) dx1.x += step;
			else dx1.x -= step;
		}

		if (sinhHM != 0) {
			if (dx2.d >= 0) dx2.x += step;
			else dx2.x -= step;
		}

		double recon = sinhHM * dx2.d + coshHM * dx1.d;

		if (recon <= totalUB) {
			coshUB = dx1.d;
			sinhUB = dx2.d;
		} else if (step > 0) {
			step /= 2;
		}
	}

	recon = sinhHM * sinhUB + coshHM * coshUB;

	while (recon > totalUB) {
		if (coshHM != 0) {
			dx.d = coshUB;
			if (dx.d >= 0) dx.x--;
			else dx.x++;
			coshUB = dx.d;
		}
		if (sinhHM != 0) {
			dx.d = sinhUB;
			if (dx.d >= 0) dx.x--;
			else dx.x++;
			sinhUB = dx.d;
		}
		recon = sinhHM * sinhUB + coshHM * coshUB;
	}
	return true;
}

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		printf("usage: %s <oracle file> <intervals folder>\n",argv[0]);
		exit(0);
	}

	int original = fegetround();
	fesetround(FE_TOWARDZERO);

	TwoIntervalFile(argv);

	fesetround(original);

	return 0;
}
#endif
