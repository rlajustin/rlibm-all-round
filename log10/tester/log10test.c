#ifndef LOG2TEST_C
#define LOG2TEST_C
#include <stdio.h>
#include "../log10.c"
#include <x86intrin.h>
#include <stdlib.h>
#include <fenv.h>
#include <string.h>


void RunTest(FILE* fp, FILE* oracle) {
	unsigned long count = 0;
	unsigned long totalTime = 0;
	unsigned long someCount = 0;
	unsigned long t1, t2;
	unsigned int dummy;

	float x, res, oracleval;
	double oval;
	union {float f; unsigned long x;} xbase;

	for(count = 0x0; count < 0x100000000; count+=1)
	{
		xbase.x = count;
		x = xbase.f;

		do {
			t1 = __rdtscp(&dummy);
			res = rlibm34_log10(x);
			t2 = __rdtscp(&dummy);
		} while(t2 <= t1);

		fread(&oval, sizeof(double), 1, oracle);
		oracleval=(float)oval;
		totalTime += (t2-t1);
		if(res != oracleval && oracleval == oracleval)
		{
			printf("wrong rlibm: %.20e, oracle: %.20e\n", res, oracleval);
			printf("wrontg input: %lx\n", count);
			someCount++;
		}
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
	RunTest(fp, oracle);

	fclose(oracle);
	fclose(fp);
	return 0;
}

#endif
