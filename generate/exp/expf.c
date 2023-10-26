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

	double y = 0.0;
	if(R < -0x1.53946p-9)
	{
		if(R == -0x1.ce4276e268p-8) y = 0x1.fc66bbd0ab89dp-1;
		else {
			double coeffs[5] = {0x1.ffffffffffc34p-1, 0x1.ffffffff3376dp-1, 0x1.fffffe08899e9p-2, 0x1.55530f8f55263p-3, 0x1.52d4272158fd6p-5};
			y = coeffs[4];
			for(int i=3;i>=0;i--)
			{
				y = multiply(y, R);
				y += coeffs[i];
			}
		}
	}
	else if(R < -0x1.9e76cap-24)
	{

		double coeffs[5] = {0x1.0000000000009p+0, 0x1.000000000b7e5p+0, 0x1.000000f50b983p-1, 0x1.55624ae242295p-3, 0x1.862dbe3b9e16dp-5};
		y = coeffs[4];
		for(int i=3;i>=0;i--)
		{
			y = multiply(y, R);
			y += coeffs[i];
		}
	}
	else if(R < 0x1.46135p-9)
	{

		double coeffs[5] = {0x1.ffffffffffffep-1, 0x1.fffffffff543fp-1, 0x1.0000007eaf3f4p-1, 0x1.554eebe1de7ap-3, 0x1.6e6da050b3d8cp-5};
		y = coeffs[4];
		for(int i=3;i>=0;i--)
		{
			y = multiply(y, R);
			y += coeffs[i];
		}
	}
	else
	{
		if(R == 0x1.d2fd8bccdp-9) y = 0x1.00e9e8af3d63fp+0;
		else if(R == 0x1.ff884078ecp-8) y = 0x1.0201880c45f6dp+0;
		else if(R == 0x1.28e70f8382p-7) y = 0x1.02548009622e1p+0;
		else if(R == 0x1.322aa79dbdp-7) y = 0x1.02673306c42b7p+0;
		else
		{
			double coeffs[5] = {0x1.000000000012ap+0, 0x1.ffffffff76e79p-1, 0x1.000000bc8d309p-1, 0x1.55536da15121dp-3, 0x1.57a7129e5eab7p-5};
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
