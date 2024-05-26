#include <stdio.h>

int main(int argc, char** argv)
{
	FILE* fp = fopen(argv[1], "r");

	double c = 0x1.789107b0e4883p-9;
	double d[3] = {0,0,0};
	long ctr = 0;
	while(!feof(fp))
	{
		fread(d, sizeof(double), 3, fp);
		if(d[0]==c) printf("%ld\n", ctr);
		ctr++;
	}

	return 0;
}
