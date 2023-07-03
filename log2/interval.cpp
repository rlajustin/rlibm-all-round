#ifndef _INTERVAL
#define _INTERVAL
#include "interval.h"
#include <iostream>
#include <fenv.h>

#define LO 0x0
#define HI 0x100000000

using namespace std;

bool ComputeSpecialCase(float x, float& res)
{
	Float fx;
    	fx.f = x;
    	if (x == 0.0) {
        	res = -1.0/0.0;
        	return true;
    	} else if (fx.x == 0x7F800000) {
        	res = x;
        	return true;
    	} else if (fx.x > 0x7F800000) {
        	fx.x = 0x7FFFFFFF;
     		res = fx.f;
       		return true;
   	 }

   	int exp;
    	float remainder = frexpf(fx.f, &exp);

    	if (remainder == 0.5f || remainder == -0.5f) {
		res = (exp - 1);
		return true;
	}

	return false;
}

double RangeReduction(float x) {
	Float fix, fit;

	int m = 0;
	fix.f = x;
	if(fix.x < 0x800000) {
		fix.f *= 8388608.0;
		m-=23;
	}
	m+=fix.x>>23;
	m-=127;
	fix.x &= 0x007FFFFF;
	fix.x |= 0x3F800000;

	fit.x = fix.x & 0x007F0000;
	int FIndex = fit.x >> 16;
	//printf("findex: %d\n", FIndex);
	fit.x |= 0x3F800000;
	double F = fit.f;
	double f = fix.f-F;
	return f*oneByF[FIndex];
}

double OutputCompensation(float x, double yp) {
	Float fix, fit;

	int m = 0;
	fix.f = x;
	if(fix.x < 0x800000) {
		fix.f *= 8388608.0;
		m-=23;
	}

	m+=fix.x>>23;
	m-=127;
	fix.x &= 0x007FFFFF;
	fix.x |= 0x3F800000;

	fit.x = fix.x & 0x007F0000;
	int FIndex = fit.x >> 16;
	
	return yp + logbase2[FIndex] + m;
}

void GuessInitialLbUb(float x, double roundingLb, double roundingUb,
                                   double xp, double& lb, double& ub) {
    Double tempYp;
    tempYp.d = log1p(xp)*ONEBYLN2;
    double tempY = OutputCompensation(x, tempYp.d);

   
    if (tempY < roundingLb) {
        do {
            if (tempYp.d >= 0.0) tempYp.x++;
            else tempYp.x--;

            tempY = OutputCompensation(x, tempYp.d);
        } while (tempY < roundingLb);
        
        if (tempY > roundingUb) {
            printf("Error during GuessInitialLbUb: lb > ub.\n");
            printf("x = %.100e\n", x);
            exit(0);
        }
        lb = tempYp.d;
        ub = tempYp.d;
        return;
    }
    
    if (tempY > roundingUb) {
        do {
            if (tempYp.d >= 0.0) tempYp.x--;
            else tempYp.x++;

            tempY = OutputCompensation(x, tempYp.d);
        } while (tempY > roundingUb);
        
        if (tempY < roundingLb) {
            printf("Error during GuessInitialLbUb: lb > ub.\n");
            printf("x = %.100e\n", x);
            exit(0);
        }
        lb = tempYp.d;
        ub = tempYp.d;
        return;
    }
    
    lb = tempYp.d;
    ub = tempYp.d;
    return;
}

void CalcRedIntErrorMsg1(float input, double roundingLb, double roundingUb,
                         double guessLb, double guessUb, double tempResult) {
    printf("Initial guess resulted in a value outside of rounding interval\n");
    printf("Diagnostics:");
    printf("Input x = %.100e\n", input);
    printf("Rounding interval:\n");
    printf("lb      = %.100e\n", roundingLb);
    printf("ub      = %.100e\n", roundingUb);
    printf("Initial guess:\n");
    printf("lb      = %.100e\n", guessLb);
    printf("ub      = %.100e\n", guessUb);
    printf("output  = %.100e\n", tempResult);
    exit(0);
}

void ComputeReducedInterval(float input, FILE* file, double corrResult) {
    // For each input, determine if it's special case or not. If it is, then
    // we continue to the next input
    float specialCaseResult;
    if (ComputeSpecialCase(input, specialCaseResult)) return;
    
    // Compute rounding interval
    double roundingLb, roundingUb;
    CalculateInterval(corrResult, roundingLb, roundingUb);
    
    // Compute reduced input
    double reducedInput = RangeReduction(input);
    //printf("input: %.10e,reduced input: %.10e\noracle: %.10e,result: %.10e\n",input,reducedInput,corrResult, OutputCompensation(input, log1p(reducedInput)*ONEBYLN2));
    // Get the initial guess for Lb and Ub
    double guessLb, guessUb;
    GuessInitialLbUb(input, roundingLb, roundingUb, reducedInput, guessLb, guessUb);
    //printf("oracle: %.10e, guesses: %.10e, %.10e\n", corrResult, guessLb, guessUb);
    
    // 6. In a while loop, keep increasing lb and ub using binary search
    //    method to find largest reduced interval
    double redIntLb, redIntUb, tempResult;
    bool lbIsSpecCase = false, ubIsSpecCase = false;

    
    // Check if we can lower the lower bound more
    tempResult = OutputCompensation(input, guessLb);
//    printf("guess: %.10e\n", tempResult);
    
    // If the initial guess puts us outside of rounding interval, there is
    // nothing more we can do
    if (tempResult < roundingLb || tempResult > roundingUb) {
        CalcRedIntErrorMsg1(input, roundingLb, roundingUb,
                            guessLb, guessUb, tempResult);
    }
    // Otherwise, we keep lowering lb and see if we are still inside the
    // rounding interval
    unsigned long long step = 0x1000000000000llu;
    while(step > 0) {
        Double dx;
        dx.d = guessLb;
	//printf("guess: %.10e\n", guessLb);
        if (dx.d >= 0) {
		if(dx.x < step) {
			dx.x = 0x8000000000000000 + step - dx.x;
		} else dx.x -= step;
	}
        else dx.x += step;
//	printf("yes\n");
        tempResult = OutputCompensation(input, dx.d);
//	printf("in %.10e, guess %.10e\n", input, dx.d);
//	printf("step: %llu, lb: %.10e, ub: %.10e\nguess: %.10e, pre: %.10e\n", step,roundingLb,roundingUb,tempResult,dx.d); 
        if (tempResult >= roundingLb && tempResult <= roundingUb) {
            // It's safe to lower the lb
            guessLb = dx.d;
        } else {
            // Otherwise decrease the step by half
            step /= 2;
        }
    }
    
        
    // Finally, set redIntLb
    redIntLb = guessLb;
    
    // Check if we can increase the upper bound more
    tempResult = OutputCompensation(input, guessUb);
    // If the initial guess puts us outside of rounding interval, there is
    // nothing more we can do
    if (tempResult < roundingLb || tempResult > roundingUb) {
        CalcRedIntErrorMsg1(input, roundingLb, roundingUb,
                            guessLb, guessUb, tempResult);
    }
    // Otherwise, we keep raising ub and see if we are still inside the
    // rounding interval
    step = 0x1000000000000llu;
    while(step > 0) {

        Double dx;
        dx.d = guessUb;
        if (dx.x < 0x8000000000000000) dx.x += step;
        else
	{
		//if negative about to go positive by increasing
		if(dx.x - step < 0x8000000000000000) {
			dx.x = 0x8000000000000000 - dx.x + step;
		} else dx.x -= step;
	}
        tempResult = OutputCompensation(input, dx.d);
	//printf("step: %llx, lb: %.10e, ub: %.10e\nguess: %.10e, pre: %.20e\n", step,roundingLb,roundingUb,tempResult,dx.d); 
        if (tempResult >= roundingLb && tempResult <= roundingUb) {
            // It's safe to lower the lb
            guessUb = dx.d;
	} else {
		// Otherwise decrease the step by half
            step /= 2;
	}
    }

    // Finally, set redIntLb
    redIntUb = guessUb;
    
    // Save reduced input, lb, and ub to files.
    fwrite(&reducedInput, sizeof(double), 1, file);
    fwrite(&redIntLb, sizeof(double), 1, file);
    fwrite(&redIntUb, sizeof(double), 1, file);
}

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		printf("usage: %s <oracle file> <interval file>\n",argv[0]);
		exit(0);
	}

	FILE* oraclefile = fopen(argv[1], "r");
	FILE* fp = fopen(argv[2], "w");
	fseek(oraclefile, LO*sizeof(double), SEEK_SET);
	int original = fegetround();
	fesetround(FE_TOWARDZERO);
	for(unsigned long i=LO;i<HI;i++)
	{
		double oracle;
		fread(&oracle, sizeof(double), 1, oraclefile);
		Float f;
		f.x = i;
		ComputeReducedInterval(f.f,fp,oracle);
	}
	fesetround(original);
	fclose(fp);
	fclose(oraclefile);

	return 0;
}
#endif
