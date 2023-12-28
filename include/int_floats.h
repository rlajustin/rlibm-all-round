#ifndef INT_FLOATS
#define INT_FLOATS

typedef union {
	float f;
	unsigned int x;
} Float;

typedef union {
	double d;
	unsigned long x;
} Double;

#endif
