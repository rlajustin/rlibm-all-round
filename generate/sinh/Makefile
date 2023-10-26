SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

all:

oracle:
	g++ -I../../include oracle.cpp -o oracle -lmpfr -lgmp

interval:
	g++ -I../../include interval.cpp -o interval -lmpfr -lgmp

sinhtest:
	gcc -std=c11 -frounding-math -O3 sinhtest.c -lm -o sinhtest -I../../src -I../../include
