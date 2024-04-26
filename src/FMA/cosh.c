#ifndef COSHFMA_C
#define COSHFMA_C

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

	double sinhL, coshL;

	if(R == 0x1.0f876cp-10) {
		sinhL = 0x1.000008ffffffep+0;
		coshL = 0x1.0000080000001p+0;
	}
	else if(R == 0x1.d2fd8bccdp-9) {
		sinhL = 0x1.d2fdcb470f5ccp-9;
		coshL = 0x1.000069c999dc6p+0;
	}
	else if(R == 0x1.28e70f8382p-7) {
		sinhL = 0x1.28e818c6b68a5p-7;
		coshL = 0x1.0002afd7d4c13p+0;
	}
	else if(R == 0x1.322aa79dbdp-7) {
		sinhL = 0x1.322bca80046ebp-7;
		coshL = 0x1.0002db71c422bp+0;
	}
	else
	{		
		sinhL = fma(0x1.112bece702313p-7, R2, 0x1.5555554bae1a1p-3);
		sinhL = fma(sinhL, R2, 0x1.0000000000009p+0);
		sinhL = multiply(sinhL, R);

		coshL = fma(0x1.555c87c50aaecp-5, R2, 0x1.fffffffb0633ap-2); 
		coshL = fma(coshL, R2, 0x1.0000000000009p+0); 
	}

	double tmp5 = multiply(sinhHM, sinhL);
	double tmp6 = multiply(coshHM, coshL);
	return add(tmp5, tmp6);
}
#endif
