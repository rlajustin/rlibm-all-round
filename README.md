# rlibm-all-round

This repo contains functions for logf, log2f, and log10f that are correctly rounded for all rounding modes.

## Generation

To run the oracle generator use
```
make oracle
./oracle <oracle file>
```
To run the interval generator use
```
make interval
./interval <oracle file> <interval file>
```
To run polynomial generator use
```
make piecewise
./piecewise <interval file>
```

### Changes from standard RLIBM implementation
To generate the polynomial, we change very little from the RLIBM-ALL approach, except that we run interval and polynomial generation in the round-to-zero mode.

The library implementation of these functions simulates floating-point operations in the round-to-zero rounding mode, regardless of the machine's rounding mode. This is achieved by looking at the sign of the approximate error of every floating point operation, and decrementing the double value toward zero if necessary.

Values for constants were generated in round-to-zero using the MPFR oracle.
