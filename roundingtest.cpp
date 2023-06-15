#ifndef TEST_CPP
#define TEST_CPP

#include <string>
#include <iostream>
#include "helper.h"
#include "stdlib.h"
#include "log1p.h"
#include <fenv.h>
#pragma STDC FENV_ACCESS ON

#define mpfr_roundmode MPFR_RNDD
#define roundmode FE_DOWNWARD


using namespace std;

float MPFRLog1p(float f)
{
	mpfr_t x;
	mpfr_init2(x,200);
	mpfr_set_flt(x,f, mpfr_roundmode);
	mpfr_log1p(x,x, mpfr_roundmode);
	float retval = mpfr_get_flt(x,mpfr_roundmode);
	mpfr_clear(x);
	return retval;
}

int main(int argc, char** argv)
{
	if(argc != 1)
	{
		printf("usage: %s\n", argv[0]);
		exit(0);
	}

	int original = fegetround();
	fesetround(roundmode);

	for(unsigned long i=0;i<0x100000000;i++)
	{
		Float f;
		f.x = i;
		float rounded34 = MPFR34Log1p(f.f);
		float oracle = MPFRLog1p(f.f);
		//printf("mpfr: %.20e, oracle: %.20e\n", rounded34, oracle);
		if(oracle!=rounded34)
		{
			printf("last working index: %ld\n", i-1);
			fesetround(original);
			return 0;
		}
		if(i%0x100000==0) printf("progress: %ld/%ld\n", i/0x100000, 0x100000000/0x100000);
	}
	fesetround(original);
	return 0;
}

#endif
