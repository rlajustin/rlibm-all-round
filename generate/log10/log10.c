#ifndef LOG10_C
#define LOG10_C

#include "../constants.h"
#include "../mathhelper.h"
#include "math.h"

double rlibm34_log10(float x)
{
	union {float f; unsigned int x;} fix = {x};
	int m = 0;

	if(fix.x < 0x800000 || fix.x >= 0x7F800000) {
		if((fix.x & 0x7FFFFFFF) == 0) { // log(0)= -infty
			fix.x = 0xFF800000;
			return fix.f;
		}
		if(fix.x > 0x7FFFFFFF) { // log(negative) = NaN
			return (x-x)/0.0;
		}
		if(fix.x >= 0x7F800000) {// log(NaN)=NaN, log
			return x;
		}
		fix.f *= 8388608.0;
		m-=23;
	}

	switch (fix.x) {
		case 0x3f800000 : return 0.0;
		case 0x41200000 : return 1.0;
		case 0x42c80000 : return 2.0;
		case 0x447a0000 : return 3.0;
		case 0x461c4000 : return 4.0;
		case 0x47c35000 : return 5.0;
		case 0x49742400 : return 6.0;
		case 0x4b189680 : return 7.0;
		case 0x4cbebc20 : return 8.0;
		case 0x4e6e6b28 : return 9.0;
		case 0x501502f9 : return 10.0;
	}

	m+= fix.x >> 23;
	m -= 127;
	fix.x &= 0x007fffff;
	fix.x |= 0x3f800000;

	union {float f; unsigned int x;} fit = {.x = fix.x & 0x007f0000};
	int FIndex = fit.x >> 16;
	fit.x |= 0x3f800000;

	double f = fix.f - fit.f; // exact
	f = multiply(f, oneByF[FIndex]);
	double fsquare = multiply(f, f);
	
	double y = 0.0;

	if(f < 0x1.5ac30c30c30c2p-9) {
		if(f < 0x1.5abffffffffffp-10) {
			if(f == 0x1.a17982f305e6p-11) y = 0x1.6a7836b621f32p-12;
			else {
				double coeffs[4] = {0x1.bcb7b1527ceccp-2, -0x1.bcb7b1b949169p-3, 0x1.28776aaea8942p-3, -0x1.a7acff226b581p-4};
				double tmp1 = multiply(coeffs[1], f);
				double tmp2 = multiply(coeffs[3], f);

				double tmp3 = add(tmp1, coeffs[0]);
				double tmp4 = add(tmp2, coeffs[2]);

				double tmp5 = multiply(tmp4, fsquare);

				double tmp6 = add(tmp3, tmp5);

				y = multiply(tmp6, f);
			}
		}
		else {
			if(f == 0x1.8027f6027f602p-10) y = 0x1.4d6ddb4b6a17ap-11;
			if(f == 0x1.b2ce38e38e38dp-10) y = 0x1.795abcd15756ep-11;
			if(f == 0x1.1fddb0d3224f2p-9)  y = 0x1.f386956539587p-11;
			else {
				double coeffs[4] = {0x1.bcb7b1515a119p-2, -0x1.bcb7a3c9a62c8p-3, 0x1.285ec2ca15fa9p-3, -0x1.96f602d3ab6p-4};
				double tmp1 = multiply(coeffs[1], f);
				double tmp2 = multiply(coeffs[3], f);

				double tmp3 = add(tmp1, coeffs[0]);
				double tmp4 = add(tmp2, coeffs[2]);

				double tmp5 = multiply(tmp4, fsquare);

				double tmp6 = add(tmp3, tmp5);

				y = multiply(tmp6, f);
			}
		}
	}
	else {
		if(f < 0x1.038e9aa180dbep-8) {
			if(f == 0x1.bde34a2b10bf5p-9) y = 0x1.82a2d0cd72e4fp-10;
			if(f == 0x1.e0b56ad5ab56ap-9) y = 0x1.a0c640ef6a0f4p-10;
			if(f == 0x1.f95306eb3e452p-9) y = 0x1.b613383dffbp-10;
			if(f == 0x1.fdf239662b9f8p-9) y = 0x1.ba12e06b7a7f4p-10;
			else {
				double coeffs[4] = {0x1.bcb7b151512cp-2, -0x1.bcb7a94c7460cp-3, 0x1.287059da087c6p-3, -0x1.b2b35216140bdp-4};
				double tmp1 = multiply(coeffs[1], f);
				double tmp2 = multiply(coeffs[3], f);

				double tmp3 = add(tmp1, coeffs[0]);
				double tmp4 = add(tmp2, coeffs[2]);

				double tmp5 = multiply(tmp4, fsquare);

				double tmp6 = add(tmp3, tmp5);

				y = multiply(tmp6, f);
			}
		}
		else {
			double coeffs[4] = {0x1.bcb7b150f58afp-2, -0x1.bcb7a900247fbp-3, 0x1.2871be7639e6ap-3, -0x1.b4abed1000d48p-4};
			double tmp1 = multiply(coeffs[1], f);
			double tmp2 = multiply(coeffs[3], f);

			double tmp3 = add(tmp1, coeffs[0]);
			double tmp4 = add(tmp2, coeffs[2]);

			double tmp5 = multiply(tmp4, fsquare);

			double tmp6 = add(tmp3, tmp5);

			y = multiply(tmp6, f);
		}
	}

	double extra = multiply(m, LOG102LOW);
	y = add(y, extra);
	double tmp = multiply(m, LOG102HIGH);
	y = add(y, log10F[FIndex]);
	y = add(y, tmp);
	
	return y;
}
#endif
