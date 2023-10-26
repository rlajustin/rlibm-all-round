#ifndef _INTERVAL
#define _INTERVAL
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

void CalcRedIntErrorMsg1(float input, double roundingLb, double roundingUb,
		double guessLbP1, double guessUbP1,
		double guessLbP2, double guessUbP2,
		double tempResult) {
	printf("Initial guess for reduced interval results in a value outside of rounding interval\n");
	printf("Diagnostics:");
	printf("Input x = %.100e\n", input);
	printf("Rounding interval:\n");
	printf("lb      = %.100e\n", roundingLb);
	printf("ub      = %.100e\n", roundingUb);
	printf("Initial guess:\n");
	printf("lb (P1) = %.100e\n", guessLbP1);
	printf("ub (P1) = %.100e\n", guessUbP1);
	printf("lb (P2) = %.100e\n", guessLbP2);
	printf("ub (P2) = %.100e\n", guessUbP2);
	printf("output  = %.100e\n", tempResult);
	exit(0);
}

void CalculateInterval(double x, double& lb, double& ub) {

	unsigned long binary = doubleTo34Bit(x);

	if((binary & 0x1UL) == 0)
	{
		printf("ci wrong\n");
		exit(0);
	}

	if((binary & 0x1FE0000000UL) == 0x1FE0000000UL)
	{
		printf("error 2\n");
		exit(0);
	}
	//x positive
	if((binary & 0x200000000UL) == 0) {
		unsigned long ubBinary = binary + 1UL;
		Double dx;
		dx.d = bin34ToDouble(ubBinary);
		dx.x--;
		ub = dx.d;

		unsigned long lbBinary = binary - 1UL;
		dx.d = bin34ToDouble(lbBinary);
		dx.x++;
		lb = dx.d;
	} else {
		unsigned long lbBinary = binary + 1UL;
		Double dx;
		dx.d = bin34ToDouble(lbBinary);
		dx.x--;
		lb = dx.d;

		unsigned long ubBinary = binary - 1UL;
		dx.d = bin34ToDouble(ubBinary);
		dx.x++;
		ub = dx.d;
	}
}

void ComputeReducedInterval(float input, double corrResult, FILE* file1, FILE* file2) {
	// For each input, determine if it's special case or not. If it is, then
	// we continue to the next input
	//printf("%x: ", fl.x);
	float specialCaseResult;
	if (ComputeSpecialCase(input, specialCaseResult)) return;

	// Compute rounding interval
	double roundingLb, roundingUb;
	CalculateInterval(corrResult, roundingLb, roundingUb);

	if (roundingLb > corrResult || roundingUb < corrResult) {
		printf("Rounding interval seems to be computed wrongly.\n");
		printf("x            = %.100e\n", input);
		printf("oracleResult = %.100e\n", corrResult);
		printf("roundingLb   = %.100e\n", roundingLb);
		printf("roundingUb   = %.100e\n\n", roundingUb);
		exit(0);
	}

	// Compute reduced input
	double reducedInput = RangeReduction(input);
	//printf("input: %.10e,reduced input: %.10e\noracle: %.10e,result: %.10e\n",input,reducedInput,corrResult, OutputCompensation(input, log1p(reducedInput)*ONEBYLN2));

	double guessLbP1, guessUbP1, guessLbP2, guessUbP2;

	bool useThis = GuessInitialLbUb(input,
					roundingLb, roundingUb,
					reducedInput,
					guessLbP1, guessUbP1,
					guessLbP2, guessUbP2);

	if (useThis) {
		// Since thiss is from user, let's do sanity check                                           
		if (guessLbP1 > guessUbP1) {
			printf("P1 interval is empty for :\n");
			printf("input = %.100e\n", input);
			printf("P1 Lb = %.100e\n", guessLbP1);
			printf("P1 Ub = %.100e\n", guessUbP1);
			printf("Aborting...\n");
			exit(0);
		}

		if (guessLbP2 > guessUbP2) {
			printf("P2 interval is empty for :\n");
			printf("input = %.100e\n", input);
			printf("P2 Lb = %.100e\n", guessLbP2);
			printf("P2 Ub = %.100e\n", guessUbP2);
			printf("Aborting...\n");
			exit(0);
		}
		// Save reduced input, lb, and ub to files.                                                  
		fwrite(&reducedInput, sizeof(double), 1, file1);
		fwrite(&guessLbP1, sizeof(double), 1, file1);
		fwrite(&guessUbP1, sizeof(double), 1, file1);
		fwrite(&reducedInput, sizeof(double), 1, file2);
		fwrite(&guessLbP2, sizeof(double), 1, file2);
		fwrite(&guessUbP2, sizeof(double), 1, file2);
		return;
	}

	double redIntLbP1, redIntUbP1, redIntLbP2, redIntUbP2, tempResult;

	// Check if we can lower the lower bound more
	tempResult = OutputCompensation(input, guessLbP1, guessLbP2);
	// If the initial guess puts us outside of rounding interval, there
	// is nothing more we can do
	if (tempResult < roundingLb || tempResult > roundingUb) {
		CalcRedIntErrorMsg1(input, roundingLb, roundingUb,
				guessLbP1, guessUbP1,
				guessLbP2, guessUbP2, tempResult);
	}



	// Otherwise, we keep lowering lb and see if we are still inside the
	// rounding interval
	unsigned long long step = 0x1000000000000;
	Double dx1, dx2;
	while (step > 0) {
		dx1.d = guessLbP1;
		if (dx1.d >= 0) dx1.x -= step;
		else dx1.x += step;

		dx2.d = guessLbP2;
		if (dx2.d >= 0) dx2.x -= step;
		else dx2.x += step;

		tempResult = OutputCompensation(input, dx1.d, dx2.d);
		if (tempResult >= roundingLb && tempResult <= roundingUb) {
			// It's safe to lower the lb
			guessLbP1 = dx1.d;
			guessLbP2 = dx2.d;
		} else {
			// Otherwise decrease the step by half
			step /= 2;
		}
	}

	// Finally, set redIntLbP1 and redIntLbP2
	redIntLbP1 = guessLbP1;
	redIntLbP2 = guessLbP2;

	tempResult = OutputCompensation(input, guessUbP1, guessUbP2);
	// If the initial guess puts us outside of rounding interval, there
	// is nothing more we can do
	if (tempResult < roundingLb || tempResult > roundingUb) {
		CalcRedIntErrorMsg1(input, roundingLb, roundingUb,
				guessLbP1, guessUbP1,
				guessLbP2, guessUbP2, tempResult);
	}

	// Otherwisse, we keep increasing ub's and see if we are still
	// inside the rounding interval
	step = 0x1000000000000;

	while (step > 0) {
		dx1.d = guessUbP1;
		if (dx1.d >= 0) dx1.x += step;
		else dx1.x -= step;

		dx2.d = guessUbP2;
		if (dx2.d >= 0) dx2.x += step;
		else dx2.x -= step;

		tempResult = OutputCompensation(input, dx1.d, dx2.d);
		if (tempResult >= roundingLb && tempResult <= roundingUb) {
			// It's safe to lower the lb
			guessUbP1 = dx1.d;
			guessUbP2 = dx2.d;
		} else {
			// Otherwise decrease the step by half
			step /= 2;
		}
	}

	// Finally, set redIntLbP1 and redIntLbP2
	redIntUbP1 = guessUbP1;
	redIntUbP2 = guessUbP2;

	fwrite(&reducedInput, sizeof(double), 1, file1);
	fwrite(&redIntLbP1, sizeof(double), 1, file1);
	fwrite(&redIntUbP1, sizeof(double), 1, file1);
	fwrite(&reducedInput, sizeof(double), 1, file2);
	fwrite(&redIntLbP2, sizeof(double), 1, file2);
	fwrite(&redIntUbP2, sizeof(double), 1, file2);
}

int main(int argc, char** argv)
{
	if(argc!=4)
	{
		printf("usage: %s <oracle file> <intervals1> <intervals2>\n",argv[0]);
		exit(0);
	}

	FILE* oraclefile = fopen(argv[1], "r");
	FILE* file1 = fopen(argv[2], "w");
	FILE* file2 = fopen(argv[3], "w");
	fseek(oraclefile, LO*sizeof(double), SEEK_SET);
	int original = fegetround();
	fesetround(FE_TOWARDZERO);
	for(unsigned long i=LO;i<HI;i++)
	{
		double oracle;
		fread(&oracle, sizeof(double), 1, oraclefile);
		Float f;
		f.x = i;
		ComputeReducedInterval(f.f, oracle, file1, file2);
		//if((i%0x1000000)==0) printf("%ld/%ld\n", i/0x1000000, 0x100000000/0x1000000);
	}
	fesetround(original);
	fclose(file1);
	fclose(file2);
	fclose(oraclefile);

	return 0;
}
#endif
