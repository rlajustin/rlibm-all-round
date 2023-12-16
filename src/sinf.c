#ifndef SINF_C
#define SINF_C

#include "constants.h"
#include "mathhelper.h"
#include "math.h"
#include <stdint.h>

#define PI_OVER_256 0x1.921fb54442d18p-7

typedef unsigned __int128 uint128_t;

double rlibm_sinf(float x) {
	union{float f; unsigned int x;} fX = {.f=x};
	uint32_t b = fX.x<<1;
	if (b < 0xff000000) {
		int k;
		long long a;
		int s = ((fX.x>>23)&0xff) - 150;
		uint64_t m = (fX.x&0x7FFFFF)|1<<23;
		double z, z2;
		if (b < 0x9d000000) {
			if(__builtin_expect(b == 0x7b9eab2e, 0)) return 0x1.9df5f18p-4*__builtin_copysign(1.0, x);
			if (b < 0x79921fb6) {
				if (b < 0x73d12ed2) return x*0x1.ffffffffffffep-1;
				z = x;
				z2 = multiply(x,x); // should be exact
				//y=0x1.0000000000011p+0 x^(1) + -0x1.55555558b5e8cp-3 x^(3) + 0x1.111300f42eec9p-7 x^(5)
				double temp = multiply(z2, 0x1.111300f42eec9p-7);
				temp = add(temp, -0x1.55555558b5e8cp-3);
				temp = multiply(z2, temp);
				temp = add(temp, 0x1.0000000000011p+0);
				return multiply(z, temp);
			}
			uint64_t p0 = m*0x441529fc27;
			uint64_t p1 = m*0xa2f9836e4e; p1+=(p0>>40);
			k = (p1>>(33-s));
			a = p1<<(31+s)|((p0<<24)>>(33-s));
		} else {
			if(__builtin_expect(b == 0xc88d9d80, 0)) return -0x1.d32b798p-1*__builtin_copysign(1.0, x);
			if(__builtin_expect(b == 0xcf524856, 0)) return -0x1.ff57018p-1*__builtin_copysign(1.0, x);
			if(__builtin_expect(b == 0xe6487e0c, 0)) return 0x1.2875078p-2*__builtin_copysign(1.0, x);
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
		k += (fX.x>>31)<<8;
		z = a*0x1p-64;
		z2 = multiply(z, z);
		double sinpiK = sinpiKBy256[k&511];
		double cospiK = sinpiKBy256[(k+128)&511];
		//y=0x1.0000000000001p+0 x^(0) + -0x1.3bd3cc9c37339p-14 x^(2) + 0x1.03c38b9f9839fp-30 x^(4)
		double cospiZ = multiply(z2, 0x1.03c38b9f9839fp-30);
		cospiZ = add(cospiZ, -0x1.3bd3cc9c37339p-14);
		cospiZ = multiply(cospiZ, z2);
		cospiZ = add(cospiZ, 0x1.0000000000001p+0);
		//y=0x1.921fb54442d34p-7 x^(1) + -0x1.4abbce7220104p-22 x^(3) + 0x1.46e011c763afap-39 x^(5)
		double sinpiZ = multiply(z2, 0x1.46e011c763afap-39);
		sinpiZ = add(sinpiZ, -0x1.4abbce7220104p-22);
		sinpiZ = multiply(sinpiZ, z2);
		sinpiZ = add(sinpiZ, 0x1.921fb54442d34p-7);
		sinpiZ = multiply(sinpiZ, z);
		return add(multiply(sinpiK, cospiZ), multiply(cospiK, sinpiZ));
		//return fma(sinpiK, cospiZ, cospiK*sinpiZ);
	} else { 
		return 0.0f/0.0f;
	}
}

#endif
