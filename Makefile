SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

# run on local machine
# SOPLEX_INCLUDE=/Users/justinkim/rlibm/polygen/build/src
# SOPLEX_LIB=/Users/justinkim/rlibm/polygen/build/lib/libsoplex.a

all:

sorting:
	g++ -o sorting sorting.cpp -lmpfr -lgmp

generate:
	g++ -o generate generate.cpp -lmpfr -lgmp -lm

