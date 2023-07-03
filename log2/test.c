#ifndef TEST_CPP
#define TEST_CPP

#include <string>
#include <iostream>
#include "stdlib.h"
#include "log2.c"
#include <cstring>
#include <fenv.h>

#define LO 0x0
#define HI 0x100000000

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("usage: %s <oracle file> <zero|up|down|nearest>\n", argv[0]);
		exit(0);
	}

	FILE* fp = fopen(argv[1], "r");
	fseek(fp, LO*sizeof(double), SEEK_SET);
	
	int original = fegetround();
	char* roundmode = argv[2];

	if(strcmp(roundmode,"zero")==0)
	{
		fesetround(FE_TOWARDZERO);
	}
	else if(strcmp(roundmode,"up")==0)
	{
		fesetround(FE_UPWARD);
	}
	else if(strcmp(roundmode,"down")==0)
	{
		fesetround(FE_DOWNWARD);
	}
	else if(strcmp(roundmode,"nearest")==0)
	{
		fesetround(FE_TONEAREST);
	}

	float oracle, rlibm;
	double tmp;
	for(unsigned long i=LO;i<HI;i++)
	{
		union {float f; unsigned int x;} f;
		f.x = i;
		fread(&tmp, sizeof(double), 1, fp);
		oracle = (float)tmp;
		rlibm = (float)rlibm34_log2(f.f);
		if(oracle!=rlibm && oracle==oracle)
		{
			printf("input: %.20e, oracle: %.20e, rlibm: %.20e\n", f.f, oracle, rlibm);
		}
		
		//if(i%0x100000==0) printf("prog: %ld/%ld\n", i/0x100000,0x100000000/0x100000);
		//printf("in: %x, oracle: %lx, out: %lx, same: %d\n", f.x, oracle.x, d.x, eq);
	}
	fesetround(original);

	return 0;
}

#endif
