#ifndef _SINH_INTERVAL
#define _SINH_INTERVAL
#include <iostream>
#include <fenv.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "constants.h"
#include "helper.h"
#include "sorting.h"

#define LO 0x39e89768
#define HI 0x42b2d4fd

#include "interval2.h"

using namespace std;

bool ComputeSpecialCase(float x, float& res)
{
	Float fx;
	fx.f = x;

	fx.x &= 0x7FFFFFFF;

	if(fx.x <= 0x39e89768) return true;
	if(fx.x >= 0x42b2d4fd) return true;

	return false;
}

double RangeReduction(float x) {
	double xp = x * CONST64BYLN2;
	int N = (int)xp;
	return x - N * LN2BY64;
}

double OutputCompensation2(float x, double sinhp, double coshp) {
	double xp = x * CONST64BYLN2;
	int N = (int)xp;
	int N2 = N % 64;
	if(N2 < 0) N2 += 64;
	int N1 = N-N2;
	int I = N1 >> 6;
	double sinhHM = sinhH[I] * coshM[N2] + coshH[I] * sinhM[N2];
	double coshHM = sinhH[I] * sinhM[N2] + coshH[I] * coshH[N2];

	return coshHM * sinhp + sinhHM * coshp;
}

bool GuessInitialLbUb2(float x, double totalLB, double totalUB,
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

	double M1 = totalLB / (sinhHM * A + coshHM * B);
	double M2 = totalUB / (sinhHM * A + coshHM * B);

	coshLB = A * M1;
	sinhLB = B * M1;

	if(sinhHM == 0)
	{
		dx.x = 0xFFEFFFFFFFFFFFFF; // -infty+1
		coshLB = dx.d;
	}
	if(coshHM == 0)
	{
		dx.x = 0xFFEFFFFFFFFFFFFF;
		sinhLB = dx.d;
	}

	unsigned long step = 0x1000000000000;
	while(step > 0)
	{
		dx1.d = coshLB;
		dx2.d = sinhLB;

		if(sinhHM != 0)
		{
			if(dx1.d >= 0) dx1.x -= step;
			else dx1.x += step;
		}

		if(coshHM != 0)
		{
			if(dx2.d >= 0) dx2.x -= step;
			else dx2.x += step;
		}

		double recon = sinhHM * dx1.d + coshHM * dx2.d;

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

	double recon = sinhHM * coshLB + coshHM * sinhLB;

	while(recon < totalLB)
	{
		if(sinhHM != 0)
		{
			dx.d = coshLB;
			if(dx.d >= 0) dx.x++;
			else dx.x--;
			coshLB = dx.d;
		}
		if(coshHM != 0)
		{
			dx.d = sinhLB;
			if(dx.d >= 0) dx.x++;
			else dx.x--;
			sinhLB = dx.d;
		}
		recon = sinhHM * coshLB + coshHM * sinhLB;
	}

	coshUB = A * M2;
	sinhUB = B * M2;

	// If SHM == 0, then cosh(R) can be any value.
	if (sinhHM == 0) {
		dx.x = 0x7FEFFFFFFFFFFFFF;
		coshUB = dx.d;
	}

	// If CHM == 0, then sinh(R) can be any value.
	if (coshHM == 0) {
		dx.x = 0x7FEFFFFFFFFFFFFF;
		sinhUB = dx.d;
	}


	step = 0x1000000000000;
	while (step > 0) {
		dx1.d = coshUB;
		dx2.d = sinhUB;

		if (sinhHM != 0) {
			if (dx1.d >= 0) dx1.x += step;
			else dx1.x -= step;
		}

		if (coshHM != 0) {
			if (dx2.d >= 0) dx2.x += step;
			else dx2.x -= step;
		}

		double recon = sinhHM * dx1.d + coshHM * dx2.d;

		if (recon <= totalUB) {
			coshUB = dx1.d;
			sinhUB = dx2.d;
		} else if (step > 0) {
			step /= 2;
		}
	}

	recon = sinhHM * coshUB + coshHM * sinhUB;

	while (recon > totalUB) {
		if (sinhHM != 0) {
			dx.d = coshUB;
			if (dx.d >= 0) dx.x--;
			else dx.x++;
			coshUB = dx.d;
		}
		if (coshHM != 0) {
			dx.d = sinhUB;
			if (dx.d >= 0) dx.x--;
			else dx.x++;
			sinhUB = dx.d;
		}
		recon = sinhHM * coshUB + coshHM * sinhUB;
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
