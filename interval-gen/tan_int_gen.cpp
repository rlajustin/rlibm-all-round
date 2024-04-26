#ifndef _SIN_INTERVAL
#define _SIN_INTERVAL

#include <iostream>
#include <math.h>
#include <mpfr.h>
#include <fenv.h>

#include "constants.h"
#include "helper.h"
#include "mathhelper.h"

#pragma STDC FENV_ACCESS ON

using namespace std;

typedef unsigned __int128 uint128_t;

mpfr_t sinpi_val, cospi_val, pi;

bool SinGuessInitialLbUbFullRNDZ(double R, double sinK, double cosK, double sinR, double cosR, double totalLB, double totalUB, double& sinLB, double& sinUB, double& cosLB, double& cosUB) {
	Double dx, dx1, dx2;
	if (cosK == 0) {
		sinLB = -1.0e300;
		sinUB = 1.0e300;
	} else {
		sinLB = sinR;
		sinUB = sinR;
	}
	if (sinK == 0) {
		cosLB = -1.0e300;
		cosUB = 1.0e300;
	} else {
		cosLB = cosR;
		cosUB = cosR;
	}
	if (add(multiply(sinK, cosR), multiply(cosK, sinR)) == 0) {
		return true;
	}
	bool oob = false;
	if (add(multiply(sinK, cosR), multiply(cosK, sinR)) < totalLB) {
		oob = true;
	}
	if (add(multiply(sinK, cosR), multiply(cosK, sinR)) > totalUB) {
		oob = true;
	}
	if (oob) return false;
	if ((sinK >= 0) && (cosK >=0)) {
		unsigned long step = 0x100000000000;
		while (sinK != 0 && cosK != 0 && sinUB != 0 && step > 0) {
			dx1.d = cosUB;
			dx1.x += step;
			dx2.d = sinUB;
			if (sinUB > 0) {
				dx2.x += step;
			} else {
				dx2.x -= step;
			}
			if (add(multiply(sinK, dx1.d), multiply(cosK, dx2.d)) <= totalUB) {
				cosUB = dx1.d;
				sinUB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && step > 0) {
			dx1.d = cosUB;
			dx1.x += step;
			if (add(multiply(sinK, dx1.d), multiply(cosK, sinUB)) <= totalUB) {
				cosUB = dx1.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (cosK != 0 && sinUB != 0 && step > 0) {
			dx2.d = sinUB;
			if (sinUB > 0) {
				dx2.x += step;
			} else {
				dx2.x -= step;
			}
			if (add(multiply(sinK, cosUB), multiply(cosK, dx2.d)) <= totalUB) {
				sinUB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && cosLB != 0 && cosK != 0 && sinLB != 0 && step > 0) {
			dx1.d = cosLB;
			dx1.x -= step;
			dx2.d = sinLB;
			if (sinLB > 0) {
				dx2.x -= step;
			} else {
				dx2.x += step;
			}
			if (add(multiply(sinK, dx1.d), multiply(cosK, dx2.d)) >= totalLB) {
				cosLB = dx1.d;
				sinLB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && cosLB != 0 && step > 0) {
			dx1.d = cosLB;
			dx1.x -= step;
			if (add(multiply(sinK, dx1.d), multiply(cosK, sinLB)) >= totalLB) {
				cosLB = dx1.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (cosK != 0 && sinLB != 0 && step > 0) {
			dx2.d = sinLB;
			if (sinLB > 0) {
				dx2.x -= step;
			} else {
				dx2.x += step;
			}
			if (add(multiply(sinK, cosLB), multiply(cosK, dx2.d)) >= totalLB) {
				sinLB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
	}
	if ((sinK >= 0) && (cosK < 0)) {
		unsigned long step = 0x100000000000;
		while (sinK != 0 && cosK != 0 && sinLB != 0 && step > 0) {
			dx1.d = cosUB;
			dx1.x += step;
			dx2.d = sinLB;
			if (sinLB > 0) {
				dx2.x -= step;
			} else {
				dx2.x += step;
			}
			if (add(multiply(sinK, dx1.d), multiply(cosK, dx2.d)) <= totalUB) {
				cosUB = dx1.d;
				sinLB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && step > 0) {
			dx1.d = cosUB;
			dx1.x += step;
			if (add(multiply(sinK, dx1.d), multiply(cosK, sinLB)) <= totalUB) {
				cosUB = dx1.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (cosK != 0 && sinLB != 0 && step > 0) {
			dx2.d = sinLB;
			if (sinLB > 0) {
				dx2.x -= step;
			} else {
				dx2.x += step;
			}
			if (add(multiply(sinK, cosUB), multiply(cosK, dx2.d)) <= totalUB) {
				sinLB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && cosLB != 0 && cosK != 0 && sinUB != 0 && step > 0) {
			dx1.d = cosLB;
			dx1.x -= step;
			dx2.d = sinUB;
			if (sinUB > 0) {
				dx2.x += step;
			} else {
				dx2.x -= step;
			}
			if (add(multiply(sinK, dx1.d), multiply(cosK, dx2.d)) >= totalLB) {
				cosLB = dx1.d;
				sinUB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && cosLB != 0 && step > 0) {
			dx1.d = cosLB;
			dx1.x -= step;
			if (add(multiply(sinK, dx1.d), multiply(cosK, sinUB)) >= totalLB) {
				cosLB = dx1.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (cosK != 0 && sinUB != 0 && step > 0) {
			dx2.d = sinUB;
			if (sinUB > 0) {
				dx2.x += step;
			} else {
				dx2.x -= step;
			}
			if (add(multiply(sinK, cosLB), multiply(cosK, dx2.d)) >= totalLB) {
				sinUB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
	}
	if ((sinK < 0) && (cosK >= 0)) {
		unsigned long step = 0x100000000000;
		while (sinK != 0 && cosLB != 0 && cosK != 0 && sinUB != 0 && step > 0) {
			dx1.d = cosLB;
			dx1.x -= step;
			dx2.d = sinUB;
			if (sinUB > 0) {
				dx2.x += step;
			} else {
				dx2.x -= step;
			}
			if (add(multiply(sinK, dx1.d), multiply(cosK, dx2.d)) <= totalUB) {
				cosLB = dx1.d;
				sinUB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && cosLB != 0 && step > 0) {
			dx1.d = cosLB;
			dx1.x -= step;
			if (add(multiply(sinK, dx1.d), multiply(cosK, sinUB)) <= totalUB) {
				cosLB = dx1.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (cosK != 0 && sinUB != 0 && step > 0) {
			dx2.d = sinUB;
			if (sinUB > 0) {
				dx2.x += step;
			} else {
				dx2.x -= step;
			}
			if (add(multiply(sinK, cosLB), multiply(cosK, dx2.d)) <= totalUB) {
				sinUB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && cosK != 0 && sinLB != 0 && step > 0) {
			dx1.d = cosUB;
			dx1.x += step;
			dx2.d = sinLB;
			if (sinLB > 0) {
				dx2.x -= step;
			} else {
				dx2.x += step;
			}
			if (add(multiply(sinK, dx1.d), multiply(cosK, dx2.d)) >= totalLB) {
				cosUB = dx1.d;
				sinLB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && step > 0) {
			dx1.d = cosUB;
			dx1.x += step;
			if (add(multiply(sinK, dx1.d), multiply(cosK, sinLB)) >= totalLB) {
				cosUB = dx1.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (cosK != 0 && sinLB != 0 && step > 0) {
			dx2.d = sinLB;
			if (sinLB > 0) {
				dx2.x -= step;
			} else {
				dx2.x += step;
			}
			if (add(multiply(sinK, cosUB), multiply(cosK, dx2.d)) >= totalLB) {
				sinLB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
	}
	if ((sinK < 0) && (cosK < 0)) {
		unsigned long step = 0x100000000000;
		while (sinK != 0 && cosLB != 0 && cosK != 0 && sinLB != 0 && step > 0) {
			dx1.d = cosLB;
			dx1.x -= step;
			dx2.d = sinLB;
			if (sinLB > 0) {
				dx2.x -= step;
			} else {
				dx2.x += step;
			}
			if (add(multiply(sinK, dx1.d), multiply(cosK, dx2.d)) <= totalUB) {
				cosLB = dx1.d;
				sinLB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && cosLB != 0 && step > 0) {
			dx1.d = cosLB;
			dx1.x -= step;
			if (add(multiply(sinK, dx1.d), multiply(cosK, sinLB)) <= totalUB) {
				cosLB = dx1.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (cosK != 0 && sinLB != 0 && step > 0) {
			dx2.d = sinLB;
			if (sinLB > 0) {
				dx2.x -= step;
			} else {
				dx2.x += step;
			}
			if (add(multiply(sinK, cosLB), multiply(cosK, dx2.d)) <= totalUB) {
				sinLB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && cosK != 0 && sinUB != 0 && step > 0) {
			dx1.d = cosUB;
			dx1.x += step;
			dx2.d = sinUB;
			if (sinUB > 0) {
				dx2.x += step;
			} else {
				dx2.x -= step;
			}
			if (add(multiply(sinK, dx1.d), multiply(cosK, dx2.d)) >= totalLB) {
				cosUB = dx1.d;
				sinUB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (sinK != 0 && step > 0) {
			dx1.d = cosUB;
			dx1.x += step;
			if (add(multiply(sinK, dx1.d), multiply(cosK, sinUB)) >= totalLB) {
				cosUB = dx1.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
		step = 0x100000000000;
		while (cosK != 0 && sinUB != 0 && step > 0) {
			dx2.d = sinUB;
			if (sinUB > 0) {
				dx2.x += step;
			} else {
				dx2.x -= step;
			}
			if (add(multiply(sinK, cosUB), multiply(cosK, dx2.d)) >= totalLB) {
				sinUB = dx2.d;
			} else if (step > 0) {
				step /= 2;
			}
		}
	}
	return true;
}
int range_reduction_pi_256_full(float x, double* x_prime) {
	Float fX = {.f=x};
	uint32_t b = fX.x<<1;
	int K;
	long long a;
	int s = ((fX.x>>23)&0xff) - 150;
	uint64_t m = (fX.x&0x7FFFFF)|1<<23;
	if (b < 0x9d000000) {
		uint64_t p0 = m*0x441529fc27;
		uint64_t p1 = m*0xa2f9836e4e; p1+=(p0>>40);
		K = (p1>>(33-s));
		a = p1<<(31+s)|((p0<<24)>>(33-s));
	} else {
		static const uint64_t ipi[] = {0xdb6295993c439041, 0xfc2757d1f534ddc0, 0xa2f9836e4e441529};
		uint128_t p1 = (uint128_t)m*ipi[0];
		uint128_t p2 = (uint128_t)m*ipi[1]; p2 += p1>>64;
		uint128_t p3 = (uint128_t)m*ipi[2]; p3 += p2>>64;
		uint64_t p3h = p3>>64, p3l = p3, p2l = p2, p1l = p1;
		if (s < 57) {
			K = (p3h<<(7+s))|(p3l>>(57-s));
			a = (p3l<<(7+s))|(p2l>>(57-s));
		} else if (s == 57) {
			K = p3l;
			a = p2l;
		} else {
			K = (p3l<<(s-57))|(p2l>>(121-s));
			a = (p2l<<(s-57))|(p1l>>(121-s));
		}
	}
	long sm = a>>63;
	K -= sm;
	K += (fX.x>>31)<<8;
	x_prime[0] = a*0x1p-64;
	return K;
}

void CalculateInterval(double x, double& lb, double& ub) {

	unsigned long binary = doubleTo34Bit(x);

	if((binary & 0x1UL) == 0)
	{
		printf("ci wrong\n");
		exit(0);
	}

	if((binary & 0x1FE0000000UL) == 0x1FE0000000UL)
	{
		printf("error 2\n");
		exit(0);
	}
	//x positive
	if((binary & 0x200000000UL) == 0) {
		unsigned long ubBinary = binary + 1UL;
		Double dx;
		dx.d = bin34ToDouble(ubBinary);
		dx.x--;
		ub = dx.d;

		unsigned long lbBinary = binary - 1UL;
		dx.d = bin34ToDouble(lbBinary);
		dx.x++;
		lb = dx.d;
	} else {
		unsigned long lbBinary = binary + 1UL;
		Double dx;
		dx.d = bin34ToDouble(lbBinary);
		dx.x--;
		lb = dx.d;

		unsigned long ubBinary = binary - 1UL;
		dx.d = bin34ToDouble(ubBinary);
		dx.x++;
		ub = dx.d;
	}
}

int main(int argc, char** argv){
	if(argc!=5)
	{
		printf("usage: %s <interval file 1> <interval file 2> <oob intervals> <oracle file>", argv[0]);
		exit(0);
	}
	fesetround(FE_TOWARDZERO);
	mpfr_init2(sinpi_val, 600);
	mpfr_init2(cospi_val, 600);
	mpfr_init2(pi, 600);
	mpfr_const_pi(pi, MPFR_RNDN);
	FILE* fp_sin = fopen(argv[1], "w");
	FILE* fp_cos = fopen(argv[2], "w");
	FILE* fp_oob = fopen(argv[3], "w");
	FILE* fp_oracle = fopen(argv[4], "r");
	Float x;
	double y;

	unsigned long count;
	unsigned long out_of_bounds_count = 0;
	unsigned long wrong_xp_count = 0;
	for (count=0; count<0x3C490FDB; ++count) {
		fread(&y, sizeof(double), 1, fp_oracle); 
	}
	for (count=0x3C490FDB; count<0x80000000; ++count) {
		if(count % 0x100000 == 0){
			printf("Completed count = %lx\n", count);
		} 
		fread(&y, sizeof(double), 1, fp_oracle); 
		if (y == 0.0f || y == 1.0f || y == -1.0f) {
			continue;
		}
		x.x = count;
		if ((x.x & 0x7FFFFFFF) >= 0x7F800000) {
			continue;
		}  
		double totalLB, totalUB;
		CalculateInterval(y, totalLB, totalUB);

		double xp, sinpiLB, sinpiUB, cospiLB, cospiUB;
		double finalEval;
		double data_entry[3];

		int N = range_reduction_pi_256_full(x.f, &xp);
		if ((xp > 0.5) || (xp < -0.5)) {
			printf("Reduced input is out of bounds (%a) for count = %lx\n", xp, count);
			wrong_xp_count++;
		}
		double sinpiK = sinpiKBy256[N&511];
		double cospiK = sinpiKBy256[(N+128)&511];
		mpfr_set_d(sinpi_val, xp, MPFR_RNDN);
		mpfr_mul(sinpi_val, sinpi_val, pi, MPFR_RNDN);
		mpfr_div_d(sinpi_val, sinpi_val, 256, MPFR_RNDN);
		mpfr_sin(sinpi_val, sinpi_val, MPFR_RNDN);
		double sinpiR = mpfr_get_d(sinpi_val, MPFR_RNDN);
		mpfr_set_d(cospi_val, xp, MPFR_RNDN);
		mpfr_mul(cospi_val, cospi_val, pi, MPFR_RNDN);
		mpfr_div_d(cospi_val, cospi_val, 256, MPFR_RNDN);
		mpfr_cos(cospi_val, cospi_val, MPFR_RNDN);
		double cospiR = mpfr_get_d(cospi_val, MPFR_RNDN);

		bool check = SinGuessInitialLbUbFullRNDZ(xp, sinpiK, cospiK, sinpiR, cospiR, totalLB, totalUB, sinpiLB, sinpiUB, cospiLB, cospiUB);

		if (!check) {
			printf("OC result is out of bounds from onset for count = %lx with xp = %a and k = %d\n", count, xp, N);
			out_of_bounds_count++;

			data_entry[0] = xp;
			data_entry[1] = totalLB;
			data_entry[2] = totalUB;
			fwrite(&data_entry, sizeof(double), 3, fp_oob);
		} else {
			if (sinpiK >= 0 && cospiK >= 0) {
				finalEval = (sinpiK*cospiUB)+(cospiK*sinpiUB);
				if (finalEval > totalUB) {
					printf("Final evaluation greater than UB for count = %lx\n", count);
					break;
				}
				finalEval = (sinpiK*cospiLB)+(cospiK*sinpiLB);
				if (finalEval < totalLB) {
					printf("Final evaluation smaller than LB for count = %lx\n", count);
					break;
				}
			} else if (sinpiK >= 0 && cospiK < 0) {
				finalEval = (sinpiK*cospiUB)+(cospiK*sinpiLB);
				if (finalEval > totalUB) {
					printf("Final evaluation greater than UB for count = %lx\n", count);
					break;
				}
				finalEval = (sinpiK*cospiLB)+(cospiK*sinpiUB);
				if (finalEval < totalLB) {
					printf("Final evaluation smaller than LB for count = %lx\n", count);
					break;
				}
			} else if (sinpiK < 0 && cospiK >= 0) { 	
				finalEval = (sinpiK*cospiLB)+(cospiK*sinpiUB);
				if (finalEval > totalUB) {
					printf("Final evaluation greater than UB for count = %lx\n", count);
					break;
				}
				finalEval = (sinpiK*cospiUB)+(cospiK*sinpiLB);
				if (finalEval < totalLB) {
					printf("Final evaluation smaller than LB for count = %lx\n", count);
					break;
				}
			} else {
				finalEval = (sinpiK*cospiLB)+(cospiK*sinpiLB);
				if (finalEval > totalUB) {
					printf("Final evaluation greater than UB for count = %lx\n", count);
					break;
				}
				finalEval = (sinpiK*cospiUB)+(cospiK*sinpiUB);
				if (finalEval < totalLB) {
					printf("Final evaluation smaller than LB for count = %lx\n", count);
					break;
				}
			}

			data_entry[0] = xp;
			data_entry[1] = sinpiLB;
			data_entry[2] = sinpiUB;
			fwrite(&data_entry, sizeof(double), 3, fp_sin);

			data_entry[0] = xp;
			data_entry[1] = cospiLB;
			data_entry[2] = cospiUB;
			fwrite(&data_entry, sizeof(double), 3, fp_cos);
		}
	}
	printf("%ld inputs are out of bounds from onset\n", out_of_bounds_count);
	printf("%ld reduced inputs are out of bounds\n", wrong_xp_count);
	fclose(fp_sin);
	fclose(fp_cos);
	fclose(fp_oob);
	fclose(fp_oracle);
	mpfr_clears(sinpi_val, cospi_val, pi, (mpfr_ptr) 0);
	return 0;
}

#endif
