static inline float random_under_test()
{
	// sample between exp(-1) and exp(1)
	const double low = 0x1.78b56362cef38p-2, high = 0x1.5bf0a8b145769p+1;
	return (high-low) * ((double) rand()/(double) RAND_MAX) + low;
}
