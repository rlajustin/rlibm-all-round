#include "mpfr.h"
#include "stdio.h"
#include "fenv.h"
#include "stdlib.h"
#include "math.h"

int main()
{
	fesetround(FE_TOWARDZERO);
	FILE* fp = fopen("data.txt", "w");
/*
	mpfr_t x, dx;
	mpfr_init2(x, 400);
	mpfr_init2(dx, 52);
	mpfr_set_d(x, 2.0, MPFR_RNDN);
	mpfr_log10(x, x, MPFR_RNDZ);

	double dHIGH = mpfr_get_d(x, MPFR_RNDZ);
	mpfr_set_d(dx, dHIGH, MPFR_RNDZ);
	if(mpfr_cmp_d(dx, dHIGH)!=0.0)
	{
		printf("bad\n");
		return 0;
	}
	mpfr_sub(x, x, dx, MPFR_RNDZ);
	double dLOW = mpfr_get_d(x, MPFR_RNDZ);

	fprintf(fp, "%a, %a\n%.30e, %.30e\n", dHIGH, dLOW, dHIGH, dLOW);

*/

	mpfr_t x;
	mpfr_init2(x, 200);
	mpfr_set_d(x, 1.0, MPFR_RNDZ);
	mpfr_exp(x, x, MPFR_RNDZ);
	mpfr_log2(x, x, MPFR_RNDZ);
	double d = mpfr_get_d(x, MPFR_RNDZ);
	printf("%a\n", d);
	mpfr_d_div(x, 1.0, x, MPFR_RNDZ);
	d = mpfr_get_d(x, MPFR_RNDZ);
	printf("%a\n", d);

/*
	mpfr_t x;
	mpfr_init2(x, 200);
	for(int i=0;i<64;i++)
	{
		double d = (double)i/64;
		mpfr_set_d(x, d, MPFR_RNDZ);
		mpfr_exp2(x, x, MPFR_RNDZ);
		double ret = mpfr_get_d(x, MPFR_RNDZ);
		fprintf(fp, "%a,\n", ret);
	}
	fclose(fp);
*/
}
