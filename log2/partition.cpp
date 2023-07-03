#include <string>
#include <queue>
#include <map>
#include "interval.h"

#define ul unsigned long

using namespace std;

bool ReadData(FILE* f, double dest[3]) {
    size_t s = fread(dest, sizeof(double), 3, f);
    if (s == 0) return false;
    if (s == 3) return true;
    printf("ReadData needs to read 3 * double data. Seems like we couldn't.\n");
    exit(0);
}

int main(int argc, char** argv)
{
	if(argc!=4)
	{
		printf("usage: %s <source> <dest1> <dest2>\n", argv[0]);
		return 0;
	}

	string source = argv[1];
	string dest1 = argv[2];
	string dest2 = argv[3];
	FILE* read = fopen(source.c_str(), "r");
	FILE* neg = fopen(dest1.c_str(), "w");
	FILE* pos = fopen(dest2.c_str(), "w");

	double data[3];
	
	while(ReadData(read, data))
	{
		if(data[0]<=0)
		{
			fwrite(data, sizeof(double), 3, neg);
		}
		else fwrite(data, sizeof(double), 3, pos);
	}
	fclose(read);
	fclose(neg);
	fclose(pos);

	return 0;
}
