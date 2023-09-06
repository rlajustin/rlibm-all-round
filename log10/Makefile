SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

# run on local machine
# SOPLEX_INCLUDE=/Users/justinkim/rlibm/polygen/build/src
# SOPLEX_LIB=/Users/justinkim/rlibm/polygen/build/lib/libsoplex.a

all:

test:
	g++ -o test test.c -lmpfr -lgmp

roundingtest:
	g++ -o roundingtest roundingtest.cpp -lmpfr -lgmp

oracle:
	g++ -o oracle oracle.cpp -lmpfr -lgmp

interval:
	g++ -o interval interval.cpp -lmpfr -lgmp

sorting:
	g++ -o sorting sorting.cpp -lmpfr -lgmp

generate:
	g++ -o generate generate.cpp -lmpfr -lgmp -lm

piecewise: rlibm-prog.h piecewise-small.cpp
	g++ -O3 -I$(SOPLEX_INCLUDE) -std=c++11 piecewise-small.cpp $(SOPLEX_LIB) -o piecewise -lgmp -lz -lm -ggdb3

polygen:
	g++ -O2 -I$(SOPLEX_INCLUDE) -std=c++11 polygen.cpp $(SOPLEX_LIB) -o polygen -lgmp -lz -lm

tmp:
	gcc -std=c11 -frounding-math -O3 tmp.c -lm -o tmp
