#include "helper.h"

#define LO 0x0
#define HI 0x100000000

using namespace std;

mpfr_t x;

double ComputeOracleResult(float f)
{
	int status = mpfr_set_flt(x, f, MPFR_RNDN);
	if (status != 0) {
		printf("Something went wrong when setting float to mpfr\n");
		exit(0);
	}
	status = mpfr_exp(x,x,MPFR_RNDZ);
	int sticky = 0;
	if(status!=0) sticky = 0x1;
	return FromMPFRToFloat34Ro(x, sticky);
}

int main(int argc, char** argv)
{
	if(argc!=2)
	{
		printf("usage: %s <oracle file>\n", argv[0]);
		exit(0);
	}

	FILE* fp = fopen(argv[1], "w");
	mpfr_init2(x, 200);
	double result;
	for(unsigned long i=LO;i<HI;i++)
	{
		union{float f; unsigned long x;} fl = {.x=i};
		result = ComputeOracleResult(fl.f);
		fwrite(&result,sizeof(double), 1, fp);
	}
	fclose(fp);
	mpfr_clear(x);
	return 0;
}
