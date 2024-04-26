#ifndef COSF_C
#define COSF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"
#include <stdint.h>

#define PI_OVER_256 0x1.921fb54442d18p-7

typedef unsigned __int128 uint128_t;

double rlibm_cosf(float x) {
	union{float f; unsigned int x;} fX = {.f=x};
	uint32_t b = fX.x<<1;
	if (b < 0xff000000) {
		int k;
		long long a;
		int s = ((fX.x>>23)&0xff) - 150;
		uint64_t m = (fX.x&0x7FFFFF)|1<<23;
		double z, z2;
		if (b < 0x9d000000) {
			if(__builtin_expect(b == 0x9ba56c4e, 0)) return 0x1.9b04988p-7;
			if (b < 0x78921fb6) {
				if (b < 0x73000002) return 0x1.ffffff8p-1;
				z = x;
				z2 = multiply(x, x); // probably exact anyway
				//y=0x1.0000000000002p+0 x^(0) + -0x1.00000000752fdp-1 x^(2) + 0x1.5557b658a7321p-5 x^(4)
				double temp = multiply(0x1.5557b658a7321p-5, z2);
				temp = add(temp, -0x1.00000000752fdp-1);
				temp = multiply(temp, z2);
				return add(temp, 0x1.0000000000002p+0);
			}
			uint64_t p0 = m*0x441529fc27;
			uint64_t p1 = m*0xa2f9836e4e; p1+=(p0>>40);
			k = (p1>>(33-s));
			a = p1<<(31+s)|((p0<<24)>>(33-s));
		} else {
			if(__builtin_expect(b == 0xb2455500, 0)) return 0x1.115d7d8p-1;
			if(__builtin_expect(b == 0xc22b9622, 0)) return 0x1.f0285d8p-1;
			if(__builtin_expect(b == 0xc2e072ec, 0)) return 0x1.b598ab8p-2;
			static const uint64_t ipi[] = {0xdb6295993c439041, 0xfc2757d1f534ddc0, 0xa2f9836e4e441529};
			uint128_t p1 = (uint128_t)m*ipi[0];
			uint128_t p2 = (uint128_t)m*ipi[1]; p2 += p1>>64;
			uint128_t p3 = (uint128_t)m*ipi[2]; p3 += p2>>64;
			uint64_t p3h = p3>>64, p3l = p3, p2l = p2, p1l = p1;
			if (s < 57) {
				k = (p3h<<(7+s))|(p3l>>(57-s));
				a = (p3l<<(7+s))|(p2l>>(57-s));
			} else if (s == 57) {
				k = p3l;
				a = p2l;
			} else {
				k = (p3l<<(s-57))|(p2l>>(121-s));
				a = (p2l<<(s-57))|(p1l>>(121-s));
			}
		}
		long sm = a>>63;
		k -= sm;
		z = a*0x1p-64;
		z2 = z*z;
		double sinpiK = sinpiKBy256[k&511];
		double cospiK = sinpiKBy256[(k+128)&511];
		//y=0x1.fffffffffffffp-1 x^(0) + -0x1.3bd3cc9bd931fp-14 x^(2) + 0x1.03c2222d9e424p-30 x^(4)
		double cospiZ = multiply(z2, 0x1.03c2222d9e424p-30);
		cospiZ = add(cospiZ, -0x1.3bd3cc9bd931fp-14);
		cospiZ = multiply(cospiZ, z2);
		cospiZ = add(cospiZ, 0x1.fffffffffffffp-1);
		//y=0x1.921fb54442d32p-7 x^(1) + -0x1.4abbce76e1cd1p-22 x^(3) + 0x1.472b6e29cca14p-39 x^(5)
		double sinpiZ = multiply(z2, 0x1.472b6e29cca14p-39);
		sinpiZ = add(sinpiZ, -0x1.4abbce76e1cd1p-22);
		sinpiZ = multiply(sinpiZ, z2);
		sinpiZ = add(sinpiZ, 0x1.921fb54442d32p-7);
		sinpiZ = multiply(sinpiZ, z);
		return add(multiply(cospiK, cospiZ), -multiply(sinpiK, sinpiZ));
		//return fma(cospiK, cospiZ, -sinpiK*sinpiZ);
	} else { 
		return 0.0f/0.0f;
	}
}
#endif
