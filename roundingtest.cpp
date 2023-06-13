#ifndef TEST_CPP
#define TEST_CPP

#include <string>
#include <iostream>
#include "helper.h"
#include "stdlib.h"
#include "log1p.h"
#include <fenv.h>
#pragma STDC FENV_ACCESS ON

using namespace std;

float FromMPFR(mpfr_t _mval) {
    double retVal = mpfr_get_d(_mval, MPFR_RNDN);

    // If double value is 0, then return 0
    if (retVal == 0) return 0.0;
    // If retVal is NaN, then ther esult is NaN
    if (retVal != retVal) {
        return retVal;
    }

    // Determine if mval rounds to 0
    if (mpfr_cmp_d(_mval, pow(2, -150)) <= 0 && mpfr_cmp_d(_mval, -pow(2, -150)) >= 0) {
        return 0;
    }

    long exp;
    double fr = mpfr_get_d_2exp(&exp, _mval, MPFR_RNDN);
    fr *= 2;
    exp--;

    // Determine if mval is denormalized value and return the result accordingly
    if (mpfr_cmp_d(_mval, 0.0) > 0) {
        if (mpfr_cmp_d(_mval, 1.5 * pow(2, -149)) < 0) return pow(2, -149);
        if (mpfr_cmp_d(_mval, pow(2, -148)) < 0) return pow(2, -148);

    } else {
        if (mpfr_cmp_d(_mval, -1.5 * pow(2, -149)) > 0) return -pow(2, -149);
        if (mpfr_cmp_d(_mval, -pow(2, -148)) > 0) return -pow(2, -148);
    }

    if (exp >= -148 && exp <= -127) {
        int prec = 150 + exp;
        mpfr_t r;
        mpfr_init2(r, prec);
        mpfr_set(r, _mval, MPFR_RNDN);
        retVal = mpfr_get_d(r, MPFR_RNDN);
        mpfr_clear(r);
        return retVal;
    } else {
        // mval is normalized value, so round it normally
        mpfr_t r;
        mpfr_init2(r, 24);
        mpfr_set(r, _mval, MPFR_RNDN);
        retVal = mpfr_get_d(r, MPFR_RNDN);
        mpfr_clear(r);
        return retVal;
    }
}

float MPFRLog1p(float f)
{
	mpfr_t x;
	mpfr_init2(x,200);
	mpfr_set_flt(x,f,MPFR_RNDN);
	mpfr_log1p(x,x,MPFR_RNDN);
	return FromMPFR(x);
}

int main(int argc, char** argv)
{
	if(argc != 1)
	{
		printf("usage: %s\n", argv[0]);
		exit(0);
	}

	int original = fegetround();
	fesetround(FE_TONEAREST);

	for(unsigned long i=0;i<0x100000000;i++)
	{
		Float f;
		f.x = i;
		float rounded34 = MPFR34Log1p(f.f);
		float oracle = MPFRLog1p(f.f);
		if(oracle!=rounded34)
		{
			printf("last working index: %ld\n", i-1);
			fesetround(original);
			return 0;
		}
		if(i%0x100000==0) printf("progress: %ld/%ld\n", i/0x100000, 0x100000/0x100000000);
	}
	fesetround(original);
	return 0;
}

#endif
