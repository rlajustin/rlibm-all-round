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
/*
	mpfr_t x;
	mpfr_init2(x, 200);
	mpfr_set_d(x, 2.0, MPFR_RNDZ);
	mpfr_log(x, x, MPFR_RNDZ);
	mpfr_d_div(x, 64.0, x, MPFR_RNDZ);
	printf("%a\n", mpfr_get_d(x, MPFR_RNDZ));
	printf("%.20e\n", mpfr_get_d(x, MPFR_RNDZ));

	mpfr_set_d(x, 2.0, MPFR_RNDZ);
	mpfr_log(x, x, MPFR_RNDZ);
	mpfr_div_d(x, x, 64.0, MPFR_RNDZ);
	printf("%a\n", mpfr_get_d(x, MPFR_RNDZ));
	printf("%.20e\n", mpfr_get_d(x, MPFR_RNDZ));
 */

/*	mpfr_t x;
	mpfr_init2(x, 200);
	fprintf(fp, "sinh:\n");
	for(int i=0;i<130;i++)
	{
		double d = (double)i;
		mpfr_set_d(x, 2.0, MPFR_RNDZ);
		mpfr_log(x, x, MPFR_RNDZ);
		mpfr_mul_d(x, x, d, MPFR_RNDZ);
		mpfr_sinh(x, x, MPFR_RNDZ);
		fprintf(fp, "%a,\n", mpfr_get_d(x, MPFR_RNDZ));
	}
	fprintf(fp, "cosh:\n");
	for(int i=0;i<130;i++)
	{
		double d = (double)i;
		mpfr_set_d(x, 2.0, MPFR_RNDZ);
		mpfr_log(x, x, MPFR_RNDZ);
		mpfr_mul_d(x, x, d, MPFR_RNDZ);
		mpfr_cosh(x, x, MPFR_RNDZ);
		fprintf(fp, "%a,\n", mpfr_get_d(x, MPFR_RNDZ));
	}
*/

/*
	mpfr_t x;
	mpfr_init2(x, 200);
	mpfr_const_pi(x, MPFR_RNDZ);
	printf("pi: %a\n", mpfr_get_d(x, MPFR_RNDZ));
	mpfr_d_div(x, 1.0, x, MPFR_RNDZ);
	printf("1/pi: %a\n", mpfr_get_d(x, MPFR_RNDZ));
	mpfr_const_pi(x, MPFR_RNDZ);
	mpfr_div_d(x, x, 256.0, MPFR_RNDZ);
	printf("pi/256: %a\n", mpfr_get_d(x, MPFR_RNDZ));
*/

	mpfr_t x;
	mpfr_init2(x, 300);
	for(int i=0;i<512;i++)
	{
		mpfr_const_pi(x, MPFR_RNDZ);
		mpfr_mul_d(x, x, (double)i, MPFR_RNDZ);
		mpfr_div_d(x, x, 256.0, MPFR_RNDZ);
		mpfr_sin(x, x, MPFR_RNDZ);
		fprintf(fp, "%a,\n", mpfr_get_d(x, MPFR_RNDZ));
	}

	fclose(fp);
}
