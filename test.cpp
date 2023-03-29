#ifndef TEST_CPP
#define TEST_CPP

#include <string>
#include <iostream>
#include "roundtoodd.h"
#include "stdlib.h"

#define ul unsigned long

using namespace std;

double Oracle34(float f)
{
	mpfr_t x;
	mpfr_init2(x,250);

	if (f == 1.0/0.0) return 1.0/0.0;
	if (f == -1.0) return -1.0/0.0;

	int sticky = 0;
	// Set float value to mpfr. This should be exact
	mpfr_set_d(x, (double)f, MPFR_RNDN);
	
	int status = mpfr_log1p(x,x,MPFR_RNDZ);
	if (status != 0) sticky |= 0x1;

	return FromMPFRToFloat34Ro(x, sticky); 
}

ul round_to_sticky(ul x, bool minus)
{
	int sticky = 0;
	if(x&0x7fffffff>0) sticky = 1;
	x &= 0xffffffff00000000;
	if(sticky) x |= 0x40000000;
	return x;
}

int main(int argc, char** argv)
{
	if(argc != 1)
	{
		printf("usage: %s\n", argv[0]);
		exit(0);
	}

	//for(unsigned int i=0x0;i<0xbf800000;i++)
	for(unsigned int i=0x0;i<0x10;i++)
	{
		Float f;
		f.x = i;
		Double oracle;
		oracle.d = Oracle34(f.f);
		Double d;
		d.d = (double)f.f;
		double extra = (double)f.f * f.f;
		d.d -= extra;
		d.x = round_to_sticky(d.x, true);
		
		int eq = (d.x==oracle.x);

		/*if(eq==0)
		{
			printf("last working index: %x\n", i-1);
			break;
		}
		*/
		//if(i%0x10000==0) printf("prog: %d", i/0x10000);
		printf("in: %x, oracle: %lx, out: %lx, same: %d\n", f.x, oracle.x, d.x, eq);
	}
}

#endif
