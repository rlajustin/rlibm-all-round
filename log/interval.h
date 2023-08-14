#ifndef _INTERVAL_H
#define _INTERVAL_H

#include <math.h>
#include <mpfr.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "helper.h"
#include "constants.h"

#define LN2HIGH 0.69314718055994528622676398299518041312694549560546875
#define ONEBYLN2 1.44269504088896338700465094007

typedef struct IInterval{
        double lo;
        double hi;
} Interval;

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

#endif
