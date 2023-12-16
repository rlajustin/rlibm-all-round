#include "stdio.h"

using namespace std;

int main(int argc, char** argv)
{
	FILE* fp = fopen(argv[1], "r");
	unsigned long x[] = {0x3dcf5597, 0x4371ade3, 0x4e58728e, 0x5f208d82, 0x6446cec0, 0x67a9242b, 0x73243f06};
	double val;
	for(int i=0;i<sizeof(x)/sizeof(unsigned long);i++)
	{
		fseek(fp, x[i]*sizeof(double), SEEK_SET);
		fread(&val, sizeof(double), 1, fp);
		printf("%a\n", val);
	}
	return 0;
}
