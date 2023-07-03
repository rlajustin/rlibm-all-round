#include "interval.h"

#define LO 0x0
#define HI 0x100000000

using namespace std;

int main(int argc, char** argv)
{
	FILE* fp = fopen("oracle_log1p.bin", "a");
	Float fl;
	float f;
	double result;
	mpfr_t mval;
	mpfr_init2(mval, 200);
	for(unsigned long i=LO;i<HI;i++)
	{
		fl.x = i;
		f=fl.f;
		result=MPFR34Log2(f);
		fwrite(&result,sizeof(double), 1, fp);
	}
	fclose(fp);
	return 0;
}
