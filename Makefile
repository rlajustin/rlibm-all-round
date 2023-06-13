SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

# run on local machine
# SOPLEX_INCLUDE=/Users/justinkim/rlibm/polygen/build/src
# SOPLEX_LIB=/Users/justinkim/rlibm/polygen/build/lib/libsoplex.a

all:

test:
	g++ -o test test.cpp -lmpfr -lgmp

oracle:
	g++ -o oracle oracle.cpp -lmpfr -lgmp

interval:
	g++ -o interval interval.cpp -lmpfr -lgmp

sorting:
	g++ -o sorting sorting.cpp -lmpfr -lgmp

polygen:
	g++ -O2 -I$(SOPLEX_INCLUDE) -std=c++11 polygen.cpp $(SOPLEX_LIB) -o polygen -lgmp -lz -lm
