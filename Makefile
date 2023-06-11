all: main

oracle:
	g++ -o oracle oracle.cpp -lmpfr -lgmp

interval:
	g++ -o interval interval.cpp -lmpfr -lgmp
