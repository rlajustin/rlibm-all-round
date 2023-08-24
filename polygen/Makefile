SOPLEX_INCLUDE=/common/home/jk1849/soplex-4.0.1/src
SOPLEX_LIB=/common/home/jk1849/soplex-4.0.1/build/lib/libsoplex.a

all: piecewise 

piecewise: rlibm-fast.h piecewise.cpp
	g++ -g -O2 -I$(SOPLEX_INCLUDE) -std=c++11 piecewise.cpp  $(SOPLEX_LIB) -o piecewise -lgmp -lz -lm	

clean:
	rm -f *~ piecewise

