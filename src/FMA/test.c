#ifndef BASICTEST_C
#define BASICTEST_C
#include <stdio.h>
#include "log.c"
#include <x86intrin.h>
#include <stdlib.h>
#include <fenv.h>
#include <string.h>

#define LO 0x0
#define HI 0x100000000

void RunTest(FILE* fp, FILE* oracle) {
	unsigned long count = 0;
	unsigned long totalTime = 0;
	unsigned long someCount = 0;
	unsigned long t1, t2;
	unsigned int dummy;

	float x, res, oracleval;
	double oval, rval;
	union {float f; unsigned long x;} xbase;

	for(count = LO; count < HI; count+=1)
	{
		xbase.x = count;
		x = xbase.f;

		do {
			t1 = __rdtscp(&dummy);
			res = rlibm34_logf(x);
			t2 = __rdtscp(&dummy);
		} while(t2 <= t1);

		fread(&oval, sizeof(double), 1, oracle);
		oracleval=(float)oval;
		totalTime += (t2-t1);
		if(res != oracleval && oracleval == oracleval)
		{
			printf("wrong rlibm: %lx/%a, oracle: %a\n", count, res, oracleval);
			//printf("double form: %a\n", rlibm34_exp10f(x)); 
			someCount++;
		}
		if(someCount>10) exit(0);
	}

	fprintf(fp, "%lu, %lu\n", totalTime, someCount);
}

int main(int argc, char** argv)
{
	if(argc!=4)
	{
		printf("usage: %s <oraclefile> <logfile> <zero|up|down|nearest>\n", argv[0]);
		exit(0);
	}
	char* roundmode = argv[3];
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

	FILE* oracle = fopen(argv[1], "r");
	FILE* fp = fopen(argv[2], "w");
	fseek(oracle, LO*sizeof(double), SEEK_SET);
	RunTest(fp, oracle);

	fclose(oracle);
	fclose(fp);
	return 0;
}

#endif
