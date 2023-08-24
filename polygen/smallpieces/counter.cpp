#include <stdio.h>
#include <stdlib.h>
#include <set>

#define ul unsigned long

using namespace std;

int main(int argc, char** argv)
{

	if(argc!=2)
	{
		printf("usage: %s <source>\n", argv[0]);
		return 0;
	}

	FILE* source = fopen(argv[1], "r");

	double d[3];

	set<double> s;

	fread(d, sizeof(double), 3, source);
	while(!feof(source))
	{
		if(d[0]==0x1.fbfffffffffffp-8) printf("index of num: %lu\n", ftell(source)/(3*sizeof(double)));
		if(s.find(d[0])!=s.end())
		{
			printf("found %a\n", d[0]);
			printf("repeat index: %lu\n", ftell(source)/(3*sizeof(double)));
		}
		s.insert(d[0]);
		fread(d, sizeof(double), 3, source);
	}
	fclose(source);

	return 0;
}
