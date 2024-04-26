FLAGS = -lmpfr -lgmp

INCLUDE = -I../include

FUNCTIONS = log log2 log10 log1p exp exp2 exp10 sinh cosh sin cos

#all: $(FUNCTIONS)

$(FUNCTIONS): % : %_int_gen_fma.cpp
	g++ $(INCLUDE) $^ -o $@ $(FLAGS)

clean:
	@rm -f $(FUNCTIONS)
