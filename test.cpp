#ifndef TEST_CPP
#define TEST_CPP

#include <string>
#include <iostream>
#include "helper.h"
#include "stdlib.h"

#define ul unsigned long

using namespace std;

double Oracle34(float f)
{
	mpfr_t x;
	mpfr_init2(x,200);
	mpfr_set_flt(x, f, MPFR_RNDN);
	int sticky = 0;
	int status = mpfr_log1p(x,x,MPFR_RNDZ);
	if (status != 0) sticky |= 0x1;
	return FromMPFRToFloat34Ro(x, sticky); 
}

double minus1Ulp(double d)
{
	Double dx;
	dx.d = d;
	dx.x--;
	unsigned long result = doubleTo34Bit(dx.d);
	return bin34ToDouble(result);
}

int main(int argc, char** argv)
{
	if(argc != 1)
	{
		printf("usage: %s\n", argv[0]);
		exit(0);
	}

	//for(unsigned int i=0x0;i<0xbf800000;i++)
	for(unsigned int i=0x2;i<0x3f800000;i++)
	{
		Float f;
		f.x = i;
		double oracle = Oracle34(f.f);
		double x = (double)f.f; //-(double)f.f*f.f;
		double rlibm = minus1Ulp(x);
		printf("oracle: %.20e, rlibm: %.20e\n", oracle, rlibm);
		printf("hmmge: %lx\n", doubleTo34Bit(oracle));
		if(oracle!=rlibm)
		{
			printf("last working index: %x\n", i-1);
			break;
		}
		
		if(i%0x10000==0) printf("prog: %d/%d\n", i/0x10000,0x3f800000/0x10000);
		//printf("in: %x, oracle: %lx, out: %lx, same: %d\n", f.x, oracle.x, d.x, eq);
	}
}

#endif
