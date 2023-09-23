SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

all:

oracle:
	g++ -I../../include oracle.cpp -o oracle -lmpfr -lgmp

interval:
	g++ -I../../include interval.cpp -o interval -lmpfr -lgmp

log2test:
	gcc -std=c11 -frounding-math -O3 log2test.c -lm -o log2test -I../../src -I../../include
