# rlibm-log1p-all

## log2

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
make polygen
./polygen <interval file>
```
To test implementation use test
```
make test
./test <oracle file> <zero|up|down|nearest>
```

### Changes from standard RLIBM implementation
To generate the polynomial, we change very little from the RLIBM-all implementation of log2, except that we run interval and polynomial generation in the round-to-zero mode.

The library implementation of this function simulates floating-point operations in the round-to-zero rounding mode, regardless of the machine's rounding mode. This is achieved by looking at the sign of the approximate error of every floating point operation, and decrementing the double value toward zero if necessary.
