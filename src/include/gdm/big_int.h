/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_BIG_INT_H
#define _AGM_BIG_INT_H

#define TYPE_ERROR	1000

class BigInteger : public AGm {
public:
	static AGmNode* FromString(const char *str);
	static AGmString* ToString(long sign, AGmBinary* bin);
	static AGmNode* Add(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2);
	static AGmNode* Sub(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2);
	static AGmNode* Mul(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2);
	static AGmNode* Div(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2, AGmNode*& mod);
	static int Compare(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2);
	static AGmNode* Gcd(AGmBinary* bin1, AGmBinary* bin2);

	static AGmNode* ToInteger(AGmNode*);
	static AGmNode* ToBigInt(AGmNode*);
	static AGmNode* ToReal(AGmNode*);
	static AGmNode* ToString(AGmNode*);

	static AGmNode* Add(AGmNode*, AGmNode*);
	static AGmNode* Sub(AGmNode*, AGmNode*);
	static AGmNode* Neg(AGmNode*);
	static AGmNode* Mul(AGmNode*, AGmNode*);
	static AGmNode* Div(AGmNode*, AGmNode*);
	static AGmNode* Mod(AGmNode*, AGmNode*);
	static int Compare(AGmNode*, AGmNode*);
	static AGmNode* Gcd(AGmNode*, AGmNode*);
	static AGmNode* GcdEx(AGmNode*, AGmNode*);
#if	_WINDOWS && _64BIT
	static long long Gcd(long long, long long);
	static void GcdEx(long long, long long, long long&, long long&, long long&);
	static int BitCount(long long);
	static long long gfp_inverse(long long, long long);
#else
	static long Gcd(long, long);
	static void GcdEx(long, long, long&, long&, long&);
	static int BitCount(long);
	static long gfp_inverse(long, long);
#endif

	static AGmNode* GFpInverse(AGmNode*, AGmNode*);
	static AGmNode* ChineseRemainderTheorem(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode* factor(AGmNode*);
};

void _al_bi_from_str(const char* str, long *size_a, unsigned char *buf_a);
		// size of buf_a = strlen(str) / 2 + 1
void _al_bi_to_str(long size_a, unsigned char *buf_a, char *str);
		// size of str = size_a * 2 + 1

void _al_bi_from_double(double r, long *size_a, unsigned char **buf_a);

void _al_bi_mul(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b,
				long *size_c, unsigned char *buf_c);
		// size_a >= size_b, size_c == size_a + size_b
		// a * b -> c
void _al_bi_div(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b,
				long *size_c, unsigned char *buf_c, long *size_d, unsigned char *buf_d);
		// b != 0, size_a >= size_b
		// size_c == size_a - size_b + 1, size_d = size_b
		// a / b -> c, a % b -> d

unsigned char** _al_bi_alloc_b8(long size_a, unsigned char *buf_a);
void _al_bi_free_b8(unsigned char **buf_buf);

extern "C" void _al_bi_clear(long size_a, unsigned char *buf_a);
		// 0 -> a 
extern "C" void _al_bi_copy(long size_a, unsigned char *buf_a, unsigned char *buf_b);
		// size_a == size_b
		// a -> b
extern "C" void _al_bi_shiftL(long size_a, unsigned char *buf_a, unsigned char *buf_b);
		// size_a == size_b
		// a << 1 -> b
extern "C" void _al_bi_shiftR(long size_a, unsigned char *buf_a, unsigned char *buf_b);
		// size_a == size_b
		// a >> 1 -> b
extern "C" int _al_bi_add(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b);
		// size_a >= size_b
		// a + b -> a
		// if overflow, return 1
extern "C" int _al_bi_sub(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b);
		// size_a >= size_b
		// a - b -> a
		// if negative, return 1
extern "C" void _al_bi_neg(long size_a, unsigned char *buf_a, unsigned char *buf_b);
		// size_a == size_b
		// 0 - a -> b
extern "C" long _al_bi_cut(long size_a, unsigned char *buf_a);
		// needed length,
extern "C" int _al_bi_comp(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b);
		// if a > b, return 1
		// if a == b, return 0
		// if a < b, return -1

void _al_bi_gcd(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b,
			   long* size_c, unsigned char *buf_c);
		// greatest common divisor
		// size of buf_c == min(size_a, size_b)

#endif /* _AGM_BIG_INT_H */
