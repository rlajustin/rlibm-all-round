#ifndef EXPF_C
#define EXPF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

double rlibm34_expf(float x)
{
	union{float f; unsigned int x;} fx = {x};
	if((fx.x & 0x7FFFFFFF) == 0) return 1.0;
	if(fx.x <= 0x33ffffff)
	{
		if(fx.x <= 0x337fffff)
		{
			return 0x1.0000008p+0;
		}
		return 0x1.0000018p+0;
	}
	if(fx.x >= 0x42b17218 && fx.x <= 0xb3800000)
	{
		if(fx.x < 0x80000000)
		{
			if(fx.x < 0x7f800000) return 0x1.ffffff8p+127;
			if(fx.x == 0x7f800000) return 1.0/0.0;
			return 0.0/0.0;
		}
		if(fx.x <= 0xb3000000) return 0x1.ffffff8p-1;
		return 0x1.fffffe8p-1;
	}

	if(fx.x >= 0xc2cff1b5)
	{
		if(fx.x == 0xff800000) return 0.0;
		if(fx.x < 0xff800000) return 0x1p-151;
		else return 0.0/0.0;
	}

	double xp = multiply(x, LGEX64); 
	int N = (int)xp;
	int N2 = N % 64;
	if(N2<0) N2 += 64;
	int N1 = N-N2;
	double prod = multiply(N, ONEBY64LGE); // this might not be necessary 

	int M = N1/64;
	int J = N2;

	double R = add(x, -prod);
	double Rsquare = multiply(R, R);
	double y = 0.0;
	if(R < -0x1.9e76cap-24) {
		if(R < -0x1.53946p-9) {
			if(R == -0x1.ce4276e268p-8) y = 0x1.fc66bbd0ab89dp-1;
			else {
				double coeffs[5] = {0x1.ffffffffffc2ep-1, 0x1.ffffffff3215bp-1, 0x1.fffffe0508f91p-2, 0x1.55530be3af495p-3, 0x1.52d18b16588fep-5};
				double temp1 = fma(R, coeffs[1], coeffs[0]);
				double temp2 = fma(R, coeffs[3], coeffs[2]);
				double temp3 = fma(Rsquare, coeffs[4], temp2);
				y = fma(Rsquare, temp3, temp1);
			}
		} else {
			double coeffs[5] = {0x1.0000000000009p+0, 0x1.000000000ba3bp+0, 0x1.000000fd8c7d5p-1, 0x1.556358fe7da3bp-3, 0x1.8e59dc3b920d7p-5};
			double temp1 = fma(R, coeffs[1], coeffs[0]);
			double temp2 = fma(R, coeffs[3], coeffs[2]);
			double temp3 = fma(Rsquare, coeffs[4], temp2);
			y = fma(Rsquare, temp3, temp1);
		}
	} else {
		if(R < 0x1.9f7c90df442p-9) {
			double coeffs[5] = {0x1.0000000000001p+0, 0x1.fffffffff386p-1, 0x1.00000068f3298p-1, 0x1.55510820fe6fap-3, 0x1.63296850123d3p-5};
			double temp1 = fma(R, coeffs[1], coeffs[0]);
			double temp2 = fma(R, coeffs[3], coeffs[2]);
			double temp3 = fma(Rsquare, coeffs[4], temp2);
			y = fma(Rsquare, temp3, temp1);
		} else {
			if(R == 0x1.d2fd8bccdp-9) y = 0x1.00e9e8af3d63fp+0;
			else if(R == 0x1.28e70f8382p-7) y = 0x1.02548009622e1p+0;
			else if(R == 0x1.351a404c04bp-7) y =0x1.026d214ba10d1p+0;
			else {
				double coeffs[5] = {0x1.00000000005c3p+0, 0x1.fffffffe243c4p-1, 0x1.000001d61a412p-1, 0x1.5551d81e28975p-3, 0x1.587d4dcc17f78p-5};
				double temp1 = fma(R, coeffs[1], coeffs[0]);
				double temp2 = fma(R, coeffs[3], coeffs[2]);
				double temp3 = fma(Rsquare, coeffs[4], temp2);
				y = fma(Rsquare, temp3, temp1);
			}
		}
	}

	union {double d; unsigned long x;} ret = {exp2JBy64[J]};
	ret.x += ((unsigned long)M << 52);
	y = multiply(y, ret.d);
	return y;
}
#endif
