#include <stdio.h>
#include <stdlib.h>

#define ul unsigned long

typedef union {
	double d;
	unsigned long x;
} Double;

using namespace std;

void WriteIntervals(FILE* source, FILE* dest, int N, unsigned long i) 
{
	ul counter = 0;
	fseek(source, 0, SEEK_SET);
	double d[3];
	unsigned long front = (0xFFFFFFFFFFFFFFFF>>(64-N));
	union {double d; unsigned long x;} in;
	int flag = -1;
	double last;
	fread(d, sizeof(double), 3, source);
 	while(!feof(source)) {
		in.d = d[0];
		if((in.x & front) == i)
		{
			counter++;
			fwrite(d, sizeof(double), 3, dest);
			if(flag==-1)
			{
				printf("first val: %lx\n", in.x);
				flag=1;
			}
			last = d[0];
		}
		fread(d, sizeof(double), 3, source);
	}
	in.d = last;
	printf("last val: %lx\n", in.x);
	printf("count: %lu\n", counter);
}

int main(int argc, char** argv)
{

	if(argc!=4)
	{
		printf("usage: %s <source> <destination> <N>\n", argv[0]);
		return 0;
	}

	int N = atoi(argv[3]);
	FILE* source = fopen(argv[1], "r");
	FILE* dest = fopen(argv[2], "w"); 

	if(dest == NULL)
	{
		printf("huh?\n");
		exit(0);
	}

	/*
	double first, last;
	fseek(source, 3*sizeof(double), SEEK_SET);
	fread(&first, sizeof(double), 1, source);
	fseek(source, -3*sizeof(double), SEEK_END);	
	fread(&last, sizeof(double), 1, source);
	fseek(source, 0, SEEK_SET);

	Double tmp1 = {first}, tmp2 = {last};

	unsigned long bitsSame = 0, sigBits = 0, lo = tmp1.x, hi = tmp2.x;

	while ( (lo & (0x1lu << (63lu - bitsSame))) ==
		(hi & (0x1lu << (63lu - bitsSame)))) {
		sigBits |= lo & (0x1lu << (63lu - bitsSame));
		bitsSame++;
	}
	*/

	for(unsigned long i=0;i<(0x1<<N);i++)
	{
		printf("writing index %lu/%lu\n", i, 0x1lu<<N);
		WriteIntervals(source, dest, N, i);
	}

	fclose(source);
	fclose(dest);

	return 0;
}
