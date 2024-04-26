# rlibm-all-round

[The RLIBM Project](https://github.com/rutgers-apl/The-RLIBM-Project) contains functions that produce correctly-rounded results for all rounding modes with just a single implementation. However, this implementation must be executed in the mode in which it was generated, and the machine rounding mode needs to be changed twice per function call. The solution is to generate the implementation using the round-to-zero (FE_TOWARDZERO) rounding mode. Then, we can use error-free transformations to approximate the rounding error of every floating-point operation in the implementation, and simulate round-to-zero regardless of the machine's actual rounding mode.

## Generation

The generation process is nearly identical to the standard RLIBM approach. There are some differing constants, which are now generated with RNDZ rather than RNDN (done for consistency, but likely doesn't matter in the end). Range reduction, output compensation, and polynomial generation are all also computed with RNDZ, so it matches the generated implementation when it simulates RNDZ. 

To run the oracle or interval generators, you can use the make rule `make <function>`. To make all of the functions, you can use `make all`.

## Testing

The finished implmentations are all found in `src`.
