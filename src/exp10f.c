#ifndef EXP10F_C
#define EXP10F_C

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
	if(R < -0x1.22f212p-10)
	{
		if(R == -0x1.0c34ef4922p-9) y = 0x1.fd97e23938224p-1;
		else if(R == -0x1.f0ed19f05p-10) y = 0x1.fdc523acfb722p-1;
		else if(R == -0x1.898cb8p-10) y = 0x1.fe3bb00000001p-1;
		else
		{
			double coeffs[5] = {0x1.0000000000127p+0, 0x1.26bb1bbb76d95p+1, 0x1.53524c6b38afdp+1, 0x1.046fc1e507509p+1, 0x1.2a600949b8f31p+0};
			y = coeffs[4];
			for(int i=3;i>=0;i--)
			{
				y = multiply(y, R);
				y += coeffs[i];
			}
		}
/*
		if(R == -0x1.f0ed19f05p-10) y = 0x1.fdc523acfb722p-1;
		else if(R == -0x1.e88c1ef9aaep-10) y = 0x1.fdcebd6e8c417p-1;
		else if(R == -0x1.59c1b4fde398p-10) y = 0x1.fe72896a7782p-1;
		else if(R == -0x1.5894ap-10) y = 0x1.fe73e30000001p-1;
		else
		{
			double coeffs[5] = {0x1.ffffffffff55dp-1, 0x1.26bb1bba461dcp+1, 0x1.535249f3af665p+1, 0x1.046d97e5dac4cp+1, 0x1.290267c007179p+0};
			y = coeffs[4];
			for(int i=3;i>=0;i--)
			{
				y = multiply(y, R);
				y += coeffs[i];
			}
		}

*/
	}
	else if(R < -0x1.572ca4p-25)
	{
		double coeffs[5] = {0x1.0000000000003p+0, 0x1.26bb1bbb616fap+1, 0x1.53524daf0e007p+1, 0x1.04799246512e5p+1, 0x1.54f893243166fp+0};
		y = coeffs[4];
		for(int i=3;i>=0;i--)
		{
			y = multiply(y, R);
			y += coeffs[i];
		}
	}
	else if(R < 0x1.1852a3f57acp-10)
	{
		double coeffs[5] = {0x1p+0, 0x1.26bb1bbb53241p+1, 0x1.53524ac2d98cfp+1, 0x1.04823ae01b606p+1, 0x1.a6f3d8e86206p-1};
		y = coeffs[4];
		for(int i=3;i>=0;i--)
		{
			y = multiply(y, R);
			y += coeffs[i];
		}
	}
	else
	{
		if (R == 0x1.30dae3b6dap-9) y = 0x1.015fea9f330fdp+0;
		else if (R == 0x1.4c203a2d14p-9) y = 0x1.017f7d3455327p+0;
		else if (R == 0x1.7dd5a004ecp-9) y = 0x1.01b9140100aaap+0;
		else if (R == 0x1.d924c938acp-9) y = 0x1.0222fe9de751bp+0;
		else
		{
			double coeffs[5] = {0x1.00000000001f7p+0, 0x1.26bb1bbadc784p+1, 0x1.53524dd2a02c3p+1, 0x1.046e7cc018b74p+1, 0x1.2e2e10fa1a98ap+0};
			y = coeffs[4];
			for(int i=3;i>=0;i--)
			{
				y = multiply(y, R);
				y += coeffs[i];
			}
		}
	}

	union {double d; unsigned long x;} ret = {exp2JBy64[J]};
	ret.x += ((unsigned long)M << 52);
	y = multiply(y, ret.d);
	return y;
}
#endif
