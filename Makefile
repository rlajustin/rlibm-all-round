all: main

main:
	g++ -o main main.cpp -L/home/jk1849/include -lmpfr -lgmp

test:
	g++ -o test test.cpp -L/home/jk1849/include -lmpfr -lgmp

clean:
	rm main test
