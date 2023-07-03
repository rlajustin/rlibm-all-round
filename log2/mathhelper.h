#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <stdbool.h>
#include <cmath>

typedef union {
	double d;
	unsigned long long x;
} Double;

double multiply(double a, double b)
{
	double x = a*b;
	Double retval;
	retval.d = x;
	double compare = fma(a,b,-x);
	if(compare<0) retval.x--;
	return retval.d;
}

double add(double a, double b)
{
	double x = a+b;
	Double retval;
	retval.d = x;
	double xtimes2 = retval.d*2.0;
	double xby2 = retval.d/2.0;
	if(a>b) // make sure b>a
	{
		double tmp = a;
		a=b;
		b=tmp;
	}
	// perform subtractions that are exactly representable by Sterbenz's lemma
	bool compare = false; // false means x<=a+b and we are done, true means x>a+b and we are not.
	if(b<0) // both a and b are negative
	{
		compare=(x-a>b);
	}
	else // honestly not sure if some of this is legal, TODO fix if I run into an error
	{
		if(b<xtimes2) // we know b>xby2 because b>a, so this means x-b is exact by Sterbenz
		{
			compare = (x-b>a);
		}
		else // by algebraic manipulation, we know that a+b is exactly representable by Sterbenz
		{
			compare = false;
		}
	}
	if(compare) retval.x--;
	return retval.d;
}






#endif
