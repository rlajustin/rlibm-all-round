#ifndef LOG10FMA_C
#define LOG10FMA_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"

double rlibm34_log10f(float x)
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

	//if(f < 0x1.5ac30c30c30c2p-9) {

	if(f < 0x1.8eb111111111p-9) {
		if(f < 0x1.c2590b21642c6p-10) {

			if(f == 0x1.a17982f305e6p-11) y = 0x1.6a7836b621f32p-12;
			else if(f == 0x1.8027f6027f602p-10) y = 0x1.4d6ddb4b6a17ap-11;
			else {
				double coeffs[4] = {0x1.bcb7b15284951p-2, -0x1.bcb7b2da003e1p-3, 0x1.287e0e3e2221ep-3, -0x1.bf81e3a74053dp-4};

				double temp1 = rndz_fma(f, coeffs[2], coeffs[1]);
				double temp2 = rndz_fma(fsquare, coeffs[3], temp1);
				double temp3 = multiply(fsquare, temp2);
				y = rndz_fma(f, coeffs[0], temp3);
			}
		}
		else {
			if(f == 0x1.00acf914c1bacp-9) y = 0x1.bd748e5fbf7e7p-11;
			else if(f == 0x1.10dadadadadadp-9) y = 0x1.d9813cafb3be7p-11;
			else if(f == 0x1.241408e78356cp-9) y = 0x1.fad3d17d3f7e7p-11;
			else {

				double coeffs[4] = {0x1.bcb7b153c23a9p-2, -0x1.bcb7bebc83e78p-3, 0x1.288f9edbaca86p-3, -0x1.d0e3dc8ce8e1p-4};
				double temp1 = rndz_fma(f, coeffs[2], coeffs[1]);
				double temp2 = rndz_fma(fsquare, coeffs[3], temp1);
				double temp3 = multiply(fsquare, temp2);
				y = rndz_fma(f, coeffs[0], temp3);
			}
		}
	}
	else {
		if(f < 0x1.e4a7627627626p-9) {
			if(f == 0x1.bde34a2b10bf5p-9) y = 0x1.82a2d0cd72e4fp-10;
			else if(f == 0x1.d1e5e5e5e5e5ep-9) y = 0x1.93f4d597d9df4p-10;
			else if(f == 0x1.e0b56ad5ab56ap-9) y = 0x1.a0c640ef6a0f4p-10;
			else {
				double coeffs[3] = {0x1.bcb7b10d3c095p-2, -0x1.bcb5cab60e948p-3, 0x1.26410b002ffdep-3};
				double temp = multiply(fsquare, rndz_fma(f, coeffs[2], coeffs[1]));
				y = rndz_fma(f, coeffs[0], temp);
			}
		}
		else {
			if(f == 0x1.f95306eb3e452p-9) y = 0x1.b613383dffbp-10;
			else if(f == 0x1.95e4d9364d935p-8) y = 0x1.5f77b475329f4p-9;
			else {
				double coeffs[4] = {0x1.bcb7b15183498p-2, -0x1.bcb7ab796f857p-3, 0x1.28738d402a37cp-3, -0x1.b588aecf9f0b2p-4};
				double temp1 = rndz_fma(f, coeffs[2], coeffs[1]);
				double temp2 = rndz_fma(fsquare, coeffs[3], temp1);
				double temp3 = multiply(fsquare, temp2);
				y = rndz_fma(f, coeffs[0], temp3);
			}
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
