//#include "PolyGenFromFile.h"
#include "PolyGenFromFileReorder.h"
#include "PolyGenSampling.cpp"
#include <vector>

//TODO GENERATE log2 WITH y=1.0 LINEAR TERM AND DEGS 2 AND 3
//TODO TRY DOING PIECEWISE BY USING BITS FROM THE OTHER SIDE

int main(int argc, char** argv)
{
	if(argc!=4)
	{
		printf("usage: %s <interval file> <logfile> <header file (output)>\n", argv[0]);
		exit(0);
	}

	FILE* interval = fopen(argv[1], "r");
	FILE* logfile = fopen(argv[2], "w");
	FILE* header = fopen(argv[3], "w");

	vector<int> power{1, 2, 3, 4, 5};

	GeneratePiecewiseFunction(interval, logfile, header, power, 8, "logcoeffs"); 
}

