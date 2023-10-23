SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

all:

oracle:
	g++ -I../../include oracle.cpp -o oracle -lmpfr -lgmp

interval:
	g++ -I../../include interval.cpp -o interval -lmpfr -lgmp -ffp-contract=off

exp10test:
	gcc -std=c11 -march=native -frounding-math exp10test.c -lm -o exp10test -I../../src -I../../include
