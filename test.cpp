#ifndef TEST_CPP
#define TEST_CPP

#include <string>
#include <iostream>
#include "helper.h"
#include "stdlib.h"
#include "log1p.h"

#define ul unsigned long

using namespace std;

int main(int argc, char** argv)
{
	if(argc != 1)
	{
		printf("usage: %s\n", argv[0]);
		exit(0);
	}

	for(unsigned int i=0x80000001;i<0xbf800000;i++)
	//for(unsigned int i=0x1;i<0x3f800000;i++)
	{
		Float f;
		f.x = i;
		double oracle = MPFR34Log1p(f.f);
		Double d, dd;
		d.d = (double)f.f;
		if(i>0x80000000) d.x++;
		else d.x--;
		double rlibm = roundto34bit(d.d);
		d.d=rlibm;
		dd.d = oracle;
		//printf("input: %.20e, oracle: %.20e, rlibm: %.20e\n", f.f, oracle, rlibm);
		//printf("hex: %llx, %llx\n", dd.x, d.x);
		if(oracle!=rlibm)
		{
			printf("last working index: %x\n", i-1);
			break;
		}
		
		if(i%0x10000==0) printf("prog: %d/%d\n", (i-0x80000000)/0x10000,0x3f800000/0x10000);
		//printf("in: %x, oracle: %lx, out: %lx, same: %d\n", f.x, oracle.x, d.x, eq);
	}
}

#endif
