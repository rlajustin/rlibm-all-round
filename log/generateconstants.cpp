#include "mpfr.h"
#include "stdio.h"
#include "fenv.h"
#include "stdlib.h"
#include "math.h"

int main()
{
	fesetround(FE_TOWARDZERO);
	FILE* fp = fopen("data.txt", "w");
	for(int i=0;i<128;i++)
	{
		int bin = i<<16;
		bin |= 0x3f800000;
		union{float f; unsigned int x;} fl;
		fl.x = bin;
		double d = log((double)fl.f);
		fprintf(fp, "%a,\n", d);
	}
	fclose(fp);
}
