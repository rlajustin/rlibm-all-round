SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

all:

oracle:
	g++ -o -I../../include oracle oracle.cpp -lmpfr -lgmp

interval:
	g++ -o -I../../include interval interval.cpp -lmpfr -lgmp

log2test:
	gcc -std=c11 -frounding-math -O3 log2test.c -lm -o log2test
