#ifndef _INTERVAL_H
#define _INTERVAL_H

void OneIntervalFile(char** argv)
{
	//change logic to check for existence of sorted interval file?
	string func = argv[0];
	func = func.substr(2);
	FILE* oraclefile = fopen(argv[1], "r");
	
	string unsorted = argv[2];
	unsorted.append("intervals_");
	unsorted += func;
	string sorted = argv[2];
	sorted.append("sorted_");
	sorted += func;
	FILE* fp = fopen(unsorted.c_str(), "w");
	fseek(oraclefile, LO*sizeof(double), SEEK_SET);
	for(unsigned long i=LO;i<HI;i++)
	{
		double oracle;
		fread(&oracle, sizeof(double), 1, oraclefile);
		Float f;
		f.x = i;
		ComputeReducedInterval1(f.f,fp,oracle);
	}

	SortIntervalFile(fp, sorted);

	fclose(fp);
	fclose(oraclefile);
}

void TwoIntervalFile(char** argv)
{
	//check also for existing files
	string func = argv[0];
	func = func.substr(2);
	FILE* oraclefile = fopen(argv[2], "r");

	string unsorted1 = argv[2];
	unsorted1.append("intervals_");
	unsorted1 += func;
	string unsorted2 = unsorted1 + to_string(2);
	unsorted1 += to_string(1);

	string sorted1 = argv[2];
	sorted1.append("sorted_");
	sorted1 += func;
	string sorted2 = sorted1 + to_string(2);
	sorted1 += to_string(1);

	FILE* file1 = fopen(unsorted1.c_str(), "w");
	FILE* file2 = fopen(unsorted2.c_str(), "w");

	fseek(oraclefile, LO*sizeof(double), SEEK_SET);
	for(unsigned long i=LO;i<HI;i++)
	{
		double oracle;
		fread(&oracle, sizeof(double), 1, oraclefile);
		Float f;
		f.x = i;
		ComputeReducedInterval2(f.f, oracle, file1, file2);
	}

	SortIntervalFile(file1, sorted1);
	SortIntervalFile(file2, sorted2);

	fclose(file1);
	fclose(file2);
	fclose(oraclefile);

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

void CalcRedIntErrorMsg2(float input, double roundingLb, double roundingUb,
		double guessLbP1, double guessUbP1,
		double guessLbP2, double guessUbP2,
		double tempResult) {
	printf("Initial guess for reduced interval results in a value outside of rounding interval\n");
	printf("Diagnostics:");
	printf("Input x = %.100e\n", input);
	printf("Rounding interval:\n");
	printf("lb      = %.100e\n", roundingLb);
	printf("ub      = %.100e\n", roundingUb);
	printf("Initial guess:\n");
	printf("lb (P1) = %.100e\n", guessLbP1);
	printf("ub (P1) = %.100e\n", guessUbP1);
	printf("lb (P2) = %.100e\n", guessLbP2);
	printf("ub (P2) = %.100e\n", guessUbP2);
	printf("output  = %.100e\n", tempResult);
	exit(0);
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

void ComputeReducedInterval1(float input, FILE* file, double corrResult) {
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

void ComputeReducedInterval2(float input, double corrResult, FILE* file1, FILE* file2) {
	// For each input, determine if it's special case or not. If it is, then
	// we continue to the next input
	//printf("%x: ", fl.x);
	float specialCaseResult;
	if (ComputeSpecialCase(input, specialCaseResult)) return;

	// Compute rounding interval
	double roundingLb, roundingUb;
	CalculateInterval(corrResult, roundingLb, roundingUb);

	if (roundingLb > corrResult || roundingUb < corrResult) {
		printf("Rounding interval seems to be computed wrongly.\n");
		printf("x            = %.100e\n", input);
		printf("oracleResult = %.100e\n", corrResult);
		printf("roundingLb   = %.100e\n", roundingLb);
		printf("roundingUb   = %.100e\n\n", roundingUb);
		exit(0);
	}

	// Compute reduced input
	double reducedInput = RangeReduction(input);
	//printf("input: %.10e,reduced input: %.10e\noracle: %.10e,result: %.10e\n",input,reducedInput,corrResult, OutputCompensation(input, log1p(reducedInput)*ONEBYLN2));

	double guessLbP1, guessUbP1, guessLbP2, guessUbP2;

	bool useThis = GuessInitialLbUb(input,
					roundingLb, roundingUb,
					reducedInput,
					guessLbP1, guessUbP1,
					guessLbP2, guessUbP2);

	if (useThis) {
		// Since thiss is from user, let's do sanity check                                           
		if (guessLbP1 > guessUbP1) {
			printf("P1 interval is empty for :\n");
			printf("input = %.100e\n", input);
			printf("P1 Lb = %.100e\n", guessLbP1);
			printf("P1 Ub = %.100e\n", guessUbP1);
			printf("Aborting...\n");
			exit(0);
		}

		if (guessLbP2 > guessUbP2) {
			printf("P2 interval is empty for :\n");
			printf("input = %.100e\n", input);
			printf("P2 Lb = %.100e\n", guessLbP2);
			printf("P2 Ub = %.100e\n", guessUbP2);
			printf("Aborting...\n");
			exit(0);
		}
		// Save reduced input, lb, and ub to files.                                                  
		fwrite(&reducedInput, sizeof(double), 1, file1);
		fwrite(&guessLbP1, sizeof(double), 1, file1);
		fwrite(&guessUbP1, sizeof(double), 1, file1);
		fwrite(&reducedInput, sizeof(double), 1, file2);
		fwrite(&guessLbP2, sizeof(double), 1, file2);
		fwrite(&guessUbP2, sizeof(double), 1, file2);
		return;
	}

	double redIntLbP1, redIntUbP1, redIntLbP2, redIntUbP2, tempResult;

	// Check if we can lower the lower bound more
	tempResult = OutputCompensation(input, guessLbP1, guessLbP2);
	// If the initial guess puts us outside of rounding interval, there
	// is nothing more we can do
	if (tempResult < roundingLb || tempResult > roundingUb) {
		CalcRedIntErrorMsg2(input, roundingLb, roundingUb,
				guessLbP1, guessUbP1,
				guessLbP2, guessUbP2, tempResult);
	}



	// Otherwise, we keep lowering lb and see if we are still inside the
	// rounding interval
	unsigned long long step = 0x1000000000000;
	Double dx1, dx2;
	while (step > 0) {
		dx1.d = guessLbP1;
		if (dx1.d >= 0) dx1.x -= step;
		else dx1.x += step;

		dx2.d = guessLbP2;
		if (dx2.d >= 0) dx2.x -= step;
		else dx2.x += step;

		tempResult = OutputCompensation(input, dx1.d, dx2.d);
		if (tempResult >= roundingLb && tempResult <= roundingUb) {
			// It's safe to lower the lb
			guessLbP1 = dx1.d;
			guessLbP2 = dx2.d;
		} else {
			// Otherwise decrease the step by half
			step /= 2;
		}
	}

	// Finally, set redIntLbP1 and redIntLbP2
	redIntLbP1 = guessLbP1;
	redIntLbP2 = guessLbP2;

	tempResult = OutputCompensation(input, guessUbP1, guessUbP2);
	// If the initial guess puts us outside of rounding interval, there
	// is nothing more we can do
	if (tempResult < roundingLb || tempResult > roundingUb) {
		CalcRedIntErrorMsg2(input, roundingLb, roundingUb,
				guessLbP1, guessUbP1,
				guessLbP2, guessUbP2, tempResult);
	}

	// Otherwisse, we keep increasing ub's and see if we are still
	// inside the rounding interval
	step = 0x1000000000000;

	while (step > 0) {
		dx1.d = guessUbP1;
		if (dx1.d >= 0) dx1.x += step;
		else dx1.x -= step;

		dx2.d = guessUbP2;
		if (dx2.d >= 0) dx2.x += step;
		else dx2.x -= step;

		tempResult = OutputCompensation(input, dx1.d, dx2.d);
		if (tempResult >= roundingLb && tempResult <= roundingUb) {
			// It's safe to lower the lb
			guessUbP1 = dx1.d;
			guessUbP2 = dx2.d;
		} else {
			// Otherwise decrease the step by half
			step /= 2;
		}
	}

	// Finally, set redIntLbP1 and redIntLbP2
	redIntUbP1 = guessUbP1;
	redIntUbP2 = guessUbP2;

	fwrite(&reducedInput, sizeof(double), 1, file1);
	fwrite(&redIntLbP1, sizeof(double), 1, file1);
	fwrite(&redIntUbP1, sizeof(double), 1, file1);
	fwrite(&reducedInput, sizeof(double), 1, file2);
	fwrite(&redIntLbP2, sizeof(double), 1, file2);
	fwrite(&redIntUbP2, sizeof(double), 1, file2);
}

#endif
