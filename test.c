#ifndef TEST_CPP
#define TEST_CPP

#include <string>
#include <iostream>
#include "stdlib.h"
#include "log1p.h"

#define LO 0x7f800000
#define HI 0x100000000

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("usage: %s <oracle file>\n", argv[0]);
		exit(0);
	}

	FILE* fp = fopen(argv[1], "r");
	fseek(fp, LO*sizeof(double), SEEK_SET);

	double oracle, rlibm;
	for(unsigned long i=LO;i<HI;i++)
	{
		union {float f; unsigned int x;} f;
		f.x = i;
		fread(&oracle, sizeof(double), 1, fp);
		rlibm = rlibm34_log1p(f.f);
		//printf("input: %.20e, oracle: %.20e, rlibm: %.20e\n", f.f, oracle, rlibm);
		//printf("hex: %llx, %llx\n", dd.x, d.x);
		if(oracle!=rlibm && oracle==oracle)
		{
			printf("last working index: %lx\noracle: %.20e, rlibm: %.20e\n", i-1, oracle, rlibm);
			break;
		}
		
		if(i%0x100000==0) printf("prog: %ld/%ld\n", i/0x100000,0x100000000/0x100000);
		//printf("in: %x, oracle: %lx, out: %lx, same: %d\n", f.x, oracle.x, d.x, eq);
	}
}

#endif
