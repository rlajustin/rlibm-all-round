#ifndef EXP2FMA_C
#define EXP2FMA_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

double rlibm34_exp2f(float x)
{
	union{float f; unsigned int x;} fx = {x};
	if((fx.x & 0x7FFFFFFF) == 0) return 1.0;
	if(fx.x <= 0x3438aa3a)
	{
		if(fx.x <= 0x33b8aa3a)
		{
			return 0x1.0000008p+0;
		}
		return 0x1.0000018p+0;
	}
	if(fx.x >= 0x43000000 && fx.x <= 0xb3b8aa3b)
	{
		if(fx.x < 0x80000000)
		{
			if(fx.x < 0x7f800000) return 0x1.ffffff8p+127;
			if(fx.x == 0x7f800000) return 1.0/0.0;
			return 0.0/0.0;
		}
		if(fx.x <= 0xb338aa3b) return 0x1.ffffff8p-1;
		return 0x1.fffffe8p-1;
	}

	if(fx.x >= 0xc3160001)
	{
		if(fx.x == 0xff800000) return 0.0;
		if(fx.x < 0xff800000) return 0x1p-151;
		else return 0.0/0.0;
	}

	double xp = x * 64; // should be exact
	int N = (int)xp;
	int N2 = N % 64;
	if(N2<0) N2 += 64;
	int N1 = N-N2;
	float prod = N*0x1p-6; // should be exact

	int M = N1/64;
	int J = N2;

	double R = add(x, -prod);

	union{double d; unsigned long x;} ret;
	if(R==0.0 && N2==0)
	{
		ret.d = 1.0;
    		ret.x += ((unsigned long)M << 52);    
    		return ret.d;
	}

	double y = 0.0;
	if(R == 0x1.853a6ep-9) y = 0x1.0087090000001p+0;
	else
	{
		double coeffs[6] = {0x1p+0, 0x1.62e42fefa37f8p-1, 0x1.ebfbdff82a128p-3, 0x1.c6b08be2f0fcbp-5, 0x1.3b2b4de5d8155p-7, 0x1.61f78463e59f5p-10};
		double Rsquare = multiply(R, R);
		double temp1 = rndz_fma(R, coeffs[1], coeffs[0]);
		double temp2 = rndz_fma(R, coeffs[5], coeffs[4]);
		double temp3 = rndz_fma(R, coeffs[3], coeffs[2]);
		double temp4 = rndz_fma(Rsquare, temp2, temp3);
		y = rndz_fma(Rsquare, temp4, temp1);    
	}

	ret.d = exp2JBy64[J];
	ret.x += ((unsigned long)M << 52);
	y = multiply(y, ret.d);
	return y;
}
#endif
