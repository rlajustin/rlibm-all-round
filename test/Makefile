LIBM+=-lm

all:: perf

perf: perf.o $(FUNCTION_UNDER_TEST).o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBM) 
