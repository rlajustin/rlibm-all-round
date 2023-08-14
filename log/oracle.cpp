#include "interval.h"

#define LO 0x0
#define HI 0x100000000

using namespace std;

int main(int argc, char** argv)
{
	if(argc!=2)
	{
		printf("usage: %s <oracle file>\n", argv[0]);
		exit(0);
	}

	FILE* fp = fopen(argv[1], "a");
	Float fl;
	float f;
	double result;
	mpfr_t mval;
	mpfr_init2(mval, 200);
	for(unsigned long i=LO;i<HI;i++)
	{
		fl.x = i;
		f=fl.f;
		result=MPFR34Log(f);
		fwrite(&result,sizeof(double), 1, fp);
	}
	fclose(fp);
	return 0;
}
