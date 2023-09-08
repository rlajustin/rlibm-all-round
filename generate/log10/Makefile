SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

all:

oracle:
	g++ -o -I../../include oracle oracle.cpp -lmpfr -lgmp

interval:
	g++ -o -I../../include interval interval.cpp -lmpfr -lgmp

log10test:
	gcc -std=c11 -frounding-math -O3 log10test.c -lm -o log10test
