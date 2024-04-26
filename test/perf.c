/* Performance of mathematical functions.

   Copyright (c) 2022 Stéphane Glondu, Inria.

   This file is part of the CORE-MATH project
   (https://core-math.gitlabpages.inria.fr/).

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fenv.h>
#include <sys/mman.h>

#define SAMPLE_SIZE (sizeof(double))

typedef double function_under_test(double);

function_under_test rlibm_function, libc_function;

//define rlibm_function in file

int main(int argc, char** argv)
{
	int count = 1000000, repeat = 1;
	char* file = NULL;
	function_under_test* p_function_under_test = &rlibm_function;
	int show_rdtsc = 0, reference = 0;

	while(argc >= 2)
	{
		if(strcmp(argv[1], "--file") == 0)
		{
			argc--;
			argv++;
			file = argv[1];
			argc--;
			argv++;
		}
		else if(strcmp(argv[1], "--count") == 0)
		{
			argc--;
			argv++;
			count = atoi(argv[1]);
			argc--;
			argv++;
		}
		else if(strcmp(argv[1], "--repeat") == 0)
		{
			argc--;
			argv++;
			repeat = atoi(argv[1]);
			argc--;
			argv++;
		}
		else if(strcmp(argv[1], "--reference") == 0)
		{
			reference = 1;
			argc--;
			argv++;
		}
		else if(strcmp(argv[1], "--libc") == 0)
		{
			p_function_under_test = &libc_function;
			argc--;
			argv++;
		}
		else if(strcmp(argv[1], "--rdtsc") == 0)
		{
			show_rdtsc = 1;
			argc--;
			argv++;
		}
		else
		{
			fprintf(stderr, "Error unknown option %s\n", argv[1]);
			exit(1);
		}
	}

	if(reference)
	{
		int fd = open(file, O_CREAT | O_RDWR, 00644);
		if(fd<0)
		{
			perror("open");
			exit(3);
		}
		if(ftruncate(fd, count*SAMPLE_SIZE)<0)
		{
			perror("ftruncate");
			exit(3);
		}

		double* randoms = mmap(NULL, count * SAMPLE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if(randoms == MAP_FAILED)
		{
			perror("map");
			exit(3);
		}
		for(unsigned int i=0;i<count;i++)
		{
			randoms[i]=random_under_test(); // defined individually for each lib function to be the range of values
		}
		msync(randoms, count * SAMPLE_SIZE, MS_SYNC);
		munmap(randoms, count * SAMPLE_SIZE);
		close(fd);
	}
	else
	{
		int fd = open(file, O_RDONLY);
		if(fd<0)
		{
			perror("open");
			exit(3);
		}
		double* mmaped_randoms = mmap(NULL, count * SAMPLE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
		if(mmaped_randoms == MAP_FAILED)
		{
			perror("mmap");
			exit(3);
		}
		double* randoms = malloc(count * SAMPLE_SIZE);
		if(randoms == NULL)
		{
			perror("malloc");
			exit(3);
		}
		memcpy(randoms, mmaped_randoms, count * SAMPLE_SIZE);
		uint64_t start = __rdtsc();
		for(int r=0;r<repeat;r++)
		{
			for(int i=0;i<count;i++)
			{
				CALL_THROUGHPUT(i);
			}
		}
		uint64_t stop = __rdtsc();
		if(show_rdtsc)
		{
			printf(".3f\n", (double)(stop-start)/(repeat*count));
		}
		munmap(mmaped_randoms, count * sizeof(double));
		close(fd);
	}
	return 0;
}
