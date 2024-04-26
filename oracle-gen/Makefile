FLAGS = -lmpfr -lgmp

INCLUDE = -I../include

FUNCTIONS = log log2 log10 log1p exp exp2 exp10 sinh cosh sin cos tan asin acos atan

all: $(FUNCTIONS)

$(FUNCTIONS): %: %_oracle.cpp
	g++ $(INCLUDE) $^ -o $@ $(FLAGS)

clean:
	rm $(FUNCTIONS) 2> /dev/null
