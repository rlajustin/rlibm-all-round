#ifndef EXP10FMA_C
#define EXP10FMA_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

double rlibm34_exp10f(float x)
{
	union{float f; unsigned int x;} fx = {x};
	if((fx.x & 0x7FFFFFFF) == 0) return 1.0;
	if(fx.x <= 0x335e5bd7)
	{
		if(fx.x <= 0x32de5bd8)
		{
			return 0x1.0000008p+0;
		}
		return 0x1.0000018p+0;
	}
	if(fx.x >= 0x421a209b && fx.x <= 0xb2de5bd9)
	{
		if(fx.x < 0x80000000)
		{
			if(fx.x < 0x7f800000) return 0x1.ffffff8p+127;
			if(fx.x == 0x7f800000) return 1.0/0.0;
			return 0.0/0.0;
		}
		if(fx.x <= 0xb25e5bd8) return 0x1.ffffff8p-1;
		return 0x1.fffffe8p-1;
	}

	if(fx.x >= 0xc2349e36)
	{
		if(fx.x == 0xff800000) return 0.0;
		if(fx.x < 0xff800000) return 0x1p-151;
		else return 0.0/0.0;	
	}


	switch(fx.x) {
		case 0x00000000:
		case 0x80000000: return 1.0;
		case 0x3f800000: return 10.0;
		case 0x40000000: return 100.0;
		case 0x40400000: return 1000.0;
		case 0x40800000: return 10000.0;
		case 0x40a00000: return 100000.0;
		case 0x40c00000: return 1000000.0;
		case 0x40e00000: return 10000000.0;
		case 0x41000000: return 100000000.0;
		case 0x41100000: return 1000000000.0;
		case 0x41200000: return 10000000000.0;
	}

	double xp = multiply(x, LG10X64); 
	int N = (int)xp;
	int N2 = N % 64;
	if(N2<0) N2 += 64;
	int N1 = N-N2;
	double prod = multiply(N, ONEBY64LG10); // this might not be necessary 

	int M = N1/64;
	int J = N2;

	double R = add(x, -prod);
	double y = 0.0;
	double Rsquare = multiply(R, R);
	if(R < -0x1.572ca4p-25)
	{
		if(R < -0x1.22f212p-10)
		{
			if(R == -0x1.0c34ef4922p-9) y = 0x1.fd97e23938224p-1;
			else if(R == -0x1.f0ed19f05p-10) y = 0x1.fdc523acfb722p-1;
			else if(R ==-0x1.898cb8p-10) y = 0x1.fe3bb00000001p-1;
			else {
				double coeffs[5] = {0x1.0000000000124p+0, 0x1.26bb1bbb7605ep+1, 0x1.53524c68fc134p+1, 0x1.046fbf6e35159p+1, 0x1.2a5e2e24829b3p+0};
				double temp1 = fma(R, coeffs[1], coeffs[0]);
				double temp2 = fma(R, coeffs[3], coeffs[2]);
				double temp3 = fma(Rsquare, coeffs[4], temp2);
				y = fma(Rsquare, temp3, temp1);
			}
		}
		else
		{
			double coeffs[5] = {0x1.0000000000003p+0, 0x1.26bb1bbb616fap+1, 0x1.53524daf0e007p+1, 0x1.04799246512e5p+1, 0x1.54f893243166fp+0};
			double temp1 = fma(R, coeffs[1], coeffs[0]);
			double temp2 = fma(R, coeffs[3], coeffs[2]);
			double temp3 = fma(Rsquare, coeffs[4], temp2);
			y = fma(Rsquare, temp3, temp1);
		}
	}
	else
	{
		if(R < 0x1.1852a3f57acp-10)
		{
			if(R == -0x1.4d89c6p-25) y = 0x1.fffffdp-1;
			else {
				double coeffs[5] = {0x1.0000000000001p+0, 0x1.26bb1bbb51addp+1, 0x1.53524ad73643ep+1, 0x1.0481c91c0fbd2p+1, 0x1.aa6cedd409623p-1};
				double temp1 = fma(R, coeffs[1], coeffs[0]);
				double temp2 = fma(R, coeffs[3], coeffs[2]);
				double temp3 = fma(Rsquare, coeffs[4], temp2);
				y = fma(Rsquare, temp3, temp1);
			}
		}
		else
		{
			if(R == 0x1.fb29438864818p-10) y = 0x1.012498c03e2e1p+0;
			else if(R == 0x1.30dae3b6dap-9) y = 0x1.015fea9f330fdp+0;
			else if(R == 0x1.4c203a2d14p-9) y = 0x1.017f7d3455327p+0;
			else if(R == 0x1.7dd5a004ecp-9) y = 0x1.01b9140100aaap+0;
			else if(R == 0x1.d924c938acp-9) y = 0x1.0222fe9de751bp+0;
			else
			{
				double coeffs[5] = {0x1.00000000002e1p+0, 0x1.26bb1bbaacf1fp+1, 0x1.53524e3b361e3p+1, 0x1.046e1ee34d69p+1, 0x1.2e69330585091p+0};
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
