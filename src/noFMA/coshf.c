#ifndef COSHF_C
#define COSHF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

double rlibm34_coshf(float x)
{
	union{float f; unsigned int x;} fx = {x};
	fx.x &= 0x7fffffff;

	if(fx.x <= 0x39ffffff)
	{
		if(fx.x == 0) return 1.0;
		if(fx.x <= 0x39B504F3) return 0x1.0000008p+0;
		return 0x1.0000018p+0;
	}

	if(fx.x >= 0x42b2d4fd)
	{
		if(fx.x > 0x7f800000) return 0.0/0.0;
		if(fx.x == 0x7f800000) return 1.0/0.0;
		return 0x1.ffffff8p+127;
	}

	double xp = fx.f * CONST64BYLN2;
	int N = (int)xp;

	double prod = multiply(N, LN2BY64);

	int N2 = N % 64;
	if(N2 < 0) N2 += 64;
	int N1 = N-N2;
	int I = N1/64;

	double R = add(fx.f, -prod);
	double R2 = multiply(R, R);

	double sinhHigh = sinhH[I];
	double coshHigh = coshH[I];
	double sinhMid = sinhM[N2];
	double coshMid = coshM[N2];

	double tmp1 = multiply(sinhHigh, coshMid);
	double tmp2 = multiply(coshHigh, sinhMid);
	double tmp3 = multiply(sinhHigh, sinhMid);
	double tmp4 = multiply(coshHigh, coshMid); // can this be optimized more?

	double sinhHM = add(tmp1, tmp2);
	double coshHM = add(tmp3, tmp4);

	double sinhL;

	if(R == 0x1.e556c2f234p-10) sinhL = 0x1.e556d40428ae2p-10;
	else if(R == 0x1.d2fd8bccdp-9) sinhL = 0x1.d2fdcb470f5ccp-9;
	else if(R == 0x1.28e70f8382p-7) sinhL = 0x1.28e818c6b68a5p-7;
	else
	{
		sinhL = 0x1.112483e02ff29p-7;
		sinhL = multiply(sinhL, R2);
		sinhL = add(sinhL, 0x1.5555554cae2abp-3);
		sinhL = multiply(sinhL, R2);
		sinhL = add(sinhL, 0x1.000000000000bp+0);
		sinhL = multiply(sinhL, R);
	}

	double tmp5 = multiply(sinhHM, sinhL);

	double coshL;
	if(R == 0x1.6345051cde58p-11) coshL = 0x1.000003da11003p+0;
	else if(R == 0x1.d2fd8bccdp-9) coshL = 0x1.000069c999dc6p+0;
	else if(R == 0x1.28e70f8382p-7) coshL = 0x1.0002afd7d4c13p+0;
	else
	{
		coshL = 0x1.55580f6ebd602p-5;
		coshL = multiply(coshL, R2);
		coshL = add(coshL, 0x1.fffffffdbe3eap-2);
		coshL = multiply(coshL, R2);
		coshL = add(coshL, 0x1.0000000000002p+0);
	}

	double tmp6 = multiply(coshHM, coshL);
	return add(tmp5, tmp6);
}
#endif
