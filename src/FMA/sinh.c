#ifndef SINHF_C
#define SINHF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

double rlibm34_sinhf(float x)
{
	union{float f; unsigned int x;} fx = {x};
	unsigned long sign = (fx.x & 0x80000000) == 0 ? 0x0 : 0x8000000000000000;
	fx.x &= 0x7fffffff;

	if(fx.x == 0) return x;

	if(fx.x <= 0x39e89768)
	{
		union{double d; unsigned long x;} dx = {(double)fx.f};
		long exp = (dx.x & 0x7ff0000000000000UL) >> 52UL;
		exp -= 1023L;
		long mantissaCount = exp + 149L;
		if(mantissaCount > 23) mantissaCount = 23;
		mantissaCount += 2L;
		unsigned long shiftAmount = (52L - mantissaCount);
		unsigned long sticky = 1UL << shiftAmount;
		dx.x |= sticky;
		dx.x |= sign;
		return dx.d;
	}

	if(fx.x >= 0x42b2d4fd)
	{
		if(fx.x > 0x7f800000) return 0.0/0.0;
		if(fx.x == 0x7f800000)
		{
			if(x > 0.0f) return 1.0/0.0;
			else return -1.0/0.0;
		}
		if(x > 0.0f) return 0x1.ffffff8p+127;
		else return -0x1.ffffff8p+127;
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

	if(R == 0x1.d2fd8bccdp-9)
	{
		sinhL = 0x1.d2fdcb470f5ccp-9;
		coshL = 0x1.000069c999dc6p+0;
	}
	else if(R == 0x1.ff884078ecp-8)
	{
		sinhL = 0x1.ff8993ae28dadp-8;
		coshL = 0x1.0001fe7897ce1p+0;
	}
	else if(R == 0x1.113e28d466p-7)
	{
		sinhL = 0x1.113ef7cf95d0dp-7;
		coshL = 0x1.000246c8ff9fp+0;
	}
	else if(R == 0x1.28e70f8382p-7)
	{
		sinhL = 0x1.28e818c6b68a5p-7;
		coshL = 0x1.0002afd7d4c13p+0;
	}
	else if(R == 0x1.322aa79dbdp-7)
	{
		sinhL = 0x1.322bca80046ecp-7;
		coshL = 0x1.0002db71c422cp+0;
	}
	else
	{
		

		sinhL = fma(0x1.111812e6199c3p-7, R2, 0x1.5555555481c53p-3);
		sinhL = fma(sinhL, R2, 0x1.fffffffffffffp-1);
		sinhL = multiply(sinhL, R);

		coshL = fma(0x1.5557df50b52d8p-5, R2, 0x1.ffffffff1a5c1p-2);
		coshL = fma(sinhL, R2, 0x1.0000000000001p+0);
	}
	double tmp5 = multiply(coshHM, sinhL);
	double tmp6 = multiply(sinhHM, coshL);
	union{double d; unsigned long x;} dx;
	dx.d = add(tmp5, tmp6);
	dx.x |= sign;
	return dx.d;
}
#endif
