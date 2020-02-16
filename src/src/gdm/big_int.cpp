/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/string.h>
#include <gdm/big_int.h>

AGmNode* BigInteger::FromString(const char *str) {
	AGmNode *ret = NULL;
	long sign = 1;
	if (*str == '-') { str++; sign = -1; }
	int len = (int)strlen(str);
	long size = len / 2 + 2;
	unsigned char *buf = new unsigned char[size];
	_al_bi_from_str(str, &size, buf);
	if (size) {
		AGmBinary *b = new AGmBinary(size);
		_al_bi_copy(size, &buf[len / 2 + 2 - size], b->Buffer());
		ret = BigInt(sign, b);
	} else {
		ret = BigInt(0, NULL);
	}
	delete [] buf;
	return ret;
}
AGmString* BigInteger::ToString(long sign, AGmBinary *bin) {
	AGmString *ss = NULL;
	if (sign == 0) {
		ss = new AGmString("0");
	} else {
		long size = bin->Length();
		char *str = new char[size * 3 + 2];
		*str = '-';
		_al_bi_to_str(size, bin->Buffer(), &str[1]);
		if (sign == 1) {
			ss = new AGmString(&str[1]);
		} else {
			ss = new AGmString(&str[0]);
		}
		delete [] str;
	}
	return ss;
}
AGmNode* BigInteger::Add(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2) {
	if (sign2 == 0) return BigInt(sign1, bin1 ? bin1->Copy() : NULL);
	if (sign1 == 0) return BigInt(sign2, bin2 ? bin2->Copy() : NULL);

	AGmNode *ret = NULL;
	long size1 = bin1->Length();
	long size2 = bin2->Length();
	long size, sign; AGmBinary *bin;
	if (size2 > size1) {
		size = size1; size1 = size2; size2 = size;
		sign = sign1; sign1 = sign2; sign2 = sign;
		bin = bin1;   bin1 = bin2;	 bin2 = bin;
	}
	unsigned char *buf = new unsigned char[size1 + 1];
	_al_bi_copy(size1, bin1->Buffer(), &buf[1]);
	if (sign1 == sign2) {
		if (_al_bi_add(size1, &buf[1], size2, bin2->Buffer())) {
			buf[0] = 0x01;
			bin = new AGmBinary(size1 + 1);
			_al_bi_copy(size1 + 1, &buf[0], bin->Buffer());
			ret = BigInt(sign1, bin);
		} else {
			bin = new AGmBinary(size1);
			_al_bi_copy(size1, &buf[1], bin->Buffer());
			ret = BigInt(sign1, bin);
		}
	} else {
		if (_al_bi_sub(size1, &buf[1], size2, bin2->Buffer())) {
			sign1 = -sign1;
			_al_bi_neg(size1, &buf[1], &buf[1]);
		}
		long len = _al_bi_cut(size1, &buf[1]);
		if (len == 0) {
			ret = BigInt(0, NULL);
		} else {
			bin = new AGmBinary(len);
			_al_bi_copy(len, &buf[size1 - len + 1], bin->Buffer());
			ret = BigInt(sign1, bin);
		}
	}
	delete [] buf;
	return ret;
}
AGmNode* BigInteger::Sub(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2) {
	if (sign2 == 0) return BigInt(sign1, bin1 ? bin1->Copy() : NULL);
	if (sign1 == 0) return BigInt(-sign2, bin2->Copy());
	return Add(sign1, bin1, -sign2, bin2);
}
AGmNode* BigInteger::Mul(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2) {
	if (sign1 == 0 || sign2 == 0) return BigInt(0, NULL);

	long size1 = bin1->Length();
	long size2 = bin2->Length();
	long size, sign; AGmBinary *bin;
	unsigned char *buf = new unsigned char[size1 + size2];
	_al_bi_mul(size1, bin1->Buffer(), size2, bin2->Buffer(), &size, buf);
	sign = sign1 * sign2;
	bin = new AGmBinary(size);
	_al_bi_copy(size, &buf[size1 + size2 - size], bin->Buffer());
	AGmNode *ret = BigInt(sign, bin);
	delete [] buf;
	return ret;
}
AGmNode* BigInteger::Div(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2, AGmNode*& mod) {
	if (sign1 == 0) return BigInt(sign1, bin1 ? bin1->Copy() : NULL);
	if (sign2 == 0) return NULL; // divide by zero

	AGmNode *ret = NULL;
	long size1 = bin1->Length();
	long size2 = bin2->Length();
	if (size1 < size2) {
		ret = BigInt(0, NULL);
		mod = BigInt(sign1, bin1 ? bin1->Copy() : NULL);
	} else {
		long size_c, size_d;
		unsigned char *buf_c = new unsigned char[size1 - size2 + 1];
		unsigned char *buf_d = new unsigned char[size2];
		_al_bi_div(size1, bin1->Buffer(), size2, bin2->Buffer(), &size_c, buf_c, &size_d, buf_d);
		long sign; AGmBinary* bin;
		if (size_c == 0) {
			ret = BigInt(0, NULL);
		} else {
			sign = sign1 * sign2;
			bin = new AGmBinary(size_c);
			_al_bi_copy(size_c, &buf_c[size1 - size2 + 1 - size_c], bin->Buffer());
			ret = BigInt(sign, bin);
		}
		if (size_d == 0) {
			mod = BigInt(0, NULL);
		} else {
			sign = sign1;
			bin = new AGmBinary(size_d);
			_al_bi_copy(size_d, &buf_d[size2 - size_d], bin->Buffer());
			mod = BigInt(sign, bin);
		}
		delete [] buf_c; delete [] buf_d;
	}
	return ret;
}
int BigInteger::Compare(long sign1, AGmBinary* bin1, long sign2, AGmBinary* bin2) {
	if (sign1 < sign2) return -1;
	if (sign1 > sign2) return 1;
	if (sign1 == 0) return 0;
	int flag = _al_bi_comp(bin1->Length(), bin1->Buffer(), bin2->Length(), bin2->Buffer());
	if (sign1 == -1) {
		return -flag;
	} else {
		return flag;
	}
}
AGmNode* BigInteger::Gcd(AGmBinary* bin1, AGmBinary* bin2) {
	long size1 = bin1->Length();
	long size2 = bin2->Length();
	long len = (size1 < size2 ? size1 : size2);
	long size; AGmBinary *bin;
	unsigned char *buf = new unsigned char[len];
	_al_bi_gcd(size1, bin1->Buffer(), size2, bin2->Buffer(), &size, buf);
	bin = new AGmBinary(size);
	_al_bi_copy(size, buf, bin->Buffer());
	AGmNode *ret = BigInt(1, bin);
	delete [] buf;
	return ret;
}

AGmNode* BigInteger::ToInteger(AGmNode* n) {
	if (IsInteger(n)) {
		return Integer(n->a.i.v);
	} else if (IsReal(n)) {
		return Integer((long)n->a.r.v);
	} else if (IsBigInt(n)) {
		long sign = (long)n->a.bi.sign;
		if (sign == 0) return Integer(0);
		AGmBinary *bin = n->a.bi.bin;
		long v = 0, size = bin->Length();
		if (size > 4 || (size == 4 && (bin->Component(0) & 0x80) != 0)) return NULL;
		unsigned char *s = bin->Buffer();
		for (int i = 0; i < size; i++) {
			v <<= 8; v += s[i];
		}
		if (sign < 0) v = -v;
		return Integer(v);
	} else if (IsString(n)) {
		long v = atol(n->a.s.v->Str());
		return Integer(v);
	} else {
		return NULL;
	}
}
AGmNode* BigInteger::ToBigInt(AGmNode* n) {
	if (IsInteger(n)) {
#if	_WINDOWS && _64BIT
		long long v = n->a.i.v, sign = 0, size;
#else
		long v = n->a.i.v, sign = 0, size;
#endif
		AGmBinary *bin = NULL;
		if (v > 0) {
			sign = 1;
		} else if (v < 0) {
			sign = -1; v = -v;
		}
		if (sign) {
			if (v & 0xff000000) {
				size = 4;
			} else if (v & 0xff0000) {
				size = 3; v <<= 8;
			} else if (v & 0xff00) {
				size = 2; v <<= 16;
			} else {
				size = 1; v <<= 24;
			}
			bin = new AGmBinary((long)size);
			unsigned char *s = bin->Buffer();
			int i;
			for (i = 0; i < size; i++) {
				s[i] = (unsigned char)(v >> 24);
				v <<= 8;
			}
		}
		return BigInt((long)sign, bin);
	} else if (IsBigInt(n)) {
		return BigInt((long)n->a.bi.sign, n->a.bi.bin ? n->a.bi.bin->Copy() : NULL);
	} else if (IsReal(n)) {
		double r = n->a.r.v;
		if (r == 0) return BigInt(0, NULL);
		long sign = 1;
		if (r < 0) { sign = -1; r = -r; }
		long size; unsigned char* buf;
		_al_bi_from_double(r, &size, &buf);
		AGmBinary *bin = new AGmBinary(size);
		memcpy(bin->Buffer(), buf, size);
		delete [] buf;
		return BigInt(sign, bin);
	} else if (IsString(n)) {
		return FromString(n->a.s.v->Str());
	} else {
		return NULL;
	}
}
AGmNode* BigInteger::ToReal(AGmNode* n) {
	if (IsInteger(n)) {
		return Real((double)n->a.i.v);
	} else if (IsReal(n)) {
		return Real(n->a.r.v);
	} else if (IsBigInt(n)) {
		double r = 0;
		AGmString *s = ToString((long)n->a.bi.sign, n->a.bi.bin);
		art_sscanf_1(s->Str(), "%lg", &r);
		delete s;
		return Real(r);
	} else {
		return NULL;
	}
}
AGmNode* BigInteger::ToString(AGmNode* n) {
	char s[256];
	if (IsInteger(n)) {
		art_sprintf_1(s, 256, "%ld", n->a.i.v);
		return String(s);
	} else if (IsReal(n)) {
		art_sprintf_1(s, 256, "%lg", n->a.r.v);
		return String(s);
	} else if (IsBigInt(n)) {
		return String(ToString((long)n->a.bi.sign, n->a.bi.bin));
	} else if (IsGFp(n)) {
		art_sprintf_1(s, 256, "%ld", n->a.gp.v);
		return String(s);
	} else if (IsString(n)) {
		return String(n->a.s.v->Str());
	} else {
		return NULL;
	}
}

AGmNode* BigInteger::Add(AGmNode* a, AGmNode* b) {
	AGmNode *ret = NULL;
	if (IsInteger(a)) {
		if (IsInteger(b)) {
			if (a->a.i.v == 0) {
				ret = Integer(b->a.i.v);
			} else if (b->a.i.v == 0) {
				ret = Integer(a->a.i.v);
			} else if (BitCount(a->a.i.v) < 31 && BitCount(b->a.i.v) < 31) {
				ret = Integer(a->a.i.v + b->a.i.v);
			} else {
				AGmNode *aa = ToBigInt(a);
				AGmNode *bb = ToBigInt(b);
				AGmNode *d = Add((long)aa->a.bi.sign, aa->a.bi.bin, (long)bb->a.bi.sign, bb->a.bi.bin);
				AGmNode *e = ToInteger(d);
				if (e) {
					ret = e; RefUnref(d, aa, bb);
				} else {
					ret = d; RefUnref(aa, bb);
				}
			}
		} else if (IsReal(b)) {
			ret = Real((double)a->a.i.v + b->a.r.v);
		} else if (IsBigInt(b)) {
			if (b->a.bi.sign == 0) {
				ret = Integer(a->a.i.v);
			} else {
				AGmNode *aa = ToBigInt(a);
				AGmNode *d = Add((long)aa->a.bi.sign, aa->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin);
				AGmNode *e = ToInteger(d);
				if (e) {
					ret = e; RefUnref(d, aa);
				} else {
					ret = d; RefUnref(aa);
				}
			}
		} else if (IsGFp(b)) {
			ret = GFp(a->a.i.v + b->a.gp.v, b->a.gp.mod);
		}
	} else if (IsReal(a)) {
		if (IsInteger(b)) {
			ret = Real(a->a.r.v + (double)b->a.i.v);
		} else if (IsReal(b)) {
			ret = Real(a->a.r.v + b->a.r.v);
		} else if (IsBigInt(b)) {
			AGmNode *bb = ToReal(b);
			AGmNode *d = Real(a->a.r.v + bb->a.r.v);
			RefUnref(bb);
			ret = d;
		} else if (IsGFp(b)) {
			AGmNode* i = ToInteger(a);
			ret = GFp(i->a.i.v + b->a.gp.v, b->a.gp.mod);
			RefUnref(i);
		}
	} else if (IsBigInt(a)) {
		if (IsInteger(b)) {
			if (a->a.bi.sign == 0) {
				ret = Integer(b->a.i.v);
			} else if (b->a.i.v == 0) {
				ret = BigInt((long)a->a.bi.sign, a->a.bi.bin ? a->a.bi.bin->Copy() : NULL);
			} else {
				AGmNode *bb = ToBigInt(b);
				AGmNode *d = Add((long)a->a.bi.sign, a->a.bi.bin, (long)bb->a.bi.sign, bb->a.bi.bin);
				AGmNode *e = ToInteger(d);
				if (e) {
					ret = e; RefUnref(d, bb);
				} else {
					ret = d; RefUnref(bb);
				}
			}
		} else if (IsReal(b)) {
			AGmNode *aa = ToReal(a);
			AGmNode *d = Real(aa->a.r.v + b->a.r.v);
			RefUnref(aa);
			ret = d;
		} else if (IsBigInt(b)) {
			if (b->a.bi.sign == 0) {
				ret = Integer(a->a.i.v);
			} else {
				AGmNode *d = Add((long)a->a.bi.sign, a->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin);
				AGmNode *e = ToInteger(d);
				if (e) {
					ret = e; RefUnref(d);
				} else {
					ret = d;
				}
			}
		} else if (IsGFp(b)) {
			AGmNode* d = Integer(b->a.gp.mod);
			AGmNode* m0 = Mod(a, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			ret = GFp(m->a.i.v + b->a.gp.v, b->a.gp.mod);
			RefUnref(d, m);
		}
	} else if (IsGFp(a)) {
		if (IsInteger(b)) {
			ret = GFp(a->a.gp.v + b->a.i.v, a->a.gp.mod);
		} else if (IsReal(b)) {
			AGmNode* i = ToInteger(b);
			ret = GFp(a->a.gp.v + i->a.i.v, a->a.gp.mod);
			RefUnref(i);
		} else if (IsBigInt(b)) {
			AGmNode* d = Integer(a->a.gp.mod);
			AGmNode* m0 = Mod(b, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			ret = GFp(a->a.gp.v + m->a.i.v, a->a.gp.mod);
			RefUnref(d, m);
		} else if (IsGFp(b)) {
			if (a->a.gp.mod == b->a.gp.mod) {
				ret = GFp(a->a.gp.v + b->a.gp.v, a->a.gp.mod);
			}
		}
	} else if (IsString(a)) {
		if (IsInteger(b) || IsBigInt(b) || IsReal(b)) {
			AGmNode *bb = ToString(b);
			AGmString *s = new AGmString(a->a.s.v->Str()); s->Append(bb->a.s.v->Str());
			RefUnref(bb);
			ret = String(s);
		} else if (IsString(b)) {
			AGmString *s = new AGmString(a->a.s.v->Str()); s->Append(b->a.s.v->Str());
			ret = String(s);
		} else if (b == NULL) {
			AGmString *s = new AGmString(a->a.s.v->Str()); s->Append("null");
			ret = String(s);
		}
	} else if (IsGFp(a) && IsGFp(b) && a->a.gp.mod == a->a.gp.mod) {
		ret = GFp((a->a.gp.v + b->a.gp.v) % a->a.gp.mod, a->a.gp.mod);
	}
	return ret;
}
AGmNode* BigInteger::Sub(AGmNode* a, AGmNode* b) {
	AGmNode *bb = Neg(b);
	AGmNode *ret = Add(a, bb);
	RefUnref(bb);
	return ret;
}
AGmNode* BigInteger::Neg(AGmNode* n) {
	if (IsInteger(n)) {
		return Integer(-n->a.i.v);
	} else if (IsReal(n)) {
		return Real(-n->a.r.v);
	} else if (IsBigInt(n)) {
		return BigInt((long)(-n->a.bi.sign), n->a.bi.bin ? n->a.bi.bin->Copy() : NULL);
	} else if (IsGFp(n)) {
		return GFp((n->a.gp.mod - n->a.gp.v) % n->a.gp.mod, n->a.gp.mod);
	} else {
		return NULL;
	}
}
AGmNode* BigInteger::Mul(AGmNode* a, AGmNode* b) {
	AGmNode *ret = NULL;
	if (IsInteger(a)) {
		if (a->a.i.v == 0) {
			ret = Integer(0);
		} else if (IsInteger(b)) {
			if (b->a.i.v == 0) {
				ret = Integer(0);
			} else if (BitCount(a->a.i.v) + BitCount(b->a.i.v) < 32) {
				ret = Integer(a->a.i.v * b->a.i.v);
			} else {
				AGmNode *aa = ToBigInt(a);
				AGmNode *bb = ToBigInt(b);
				ret = Mul((long)aa->a.bi.sign, aa->a.bi.bin, (long)bb->a.bi.sign, bb->a.bi.bin);
				RefUnref(aa, bb);
			}
		} else if (IsReal(b)) {
			ret = Real((double)a->a.i.v * b->a.r.v);
		} else if (IsBigInt(b)) {
			if (b->a.bi.sign == 0) {
				ret = Integer(0);
			} else {
				AGmNode *aa = ToBigInt(a);
				ret = Mul((long)aa->a.bi.sign, aa->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin);
				RefUnref(aa);
			}
		} else if (IsGFp(b)) {
			AGmNode* g = GFp(a->a.i.v, b->a.gp.mod);
			ret = GFp((g->a.gp.v * b->a.gp.v) % b->a.gp.mod, b->a.gp.mod);
			RefUnref(g);
		}
	} else if (IsReal(a)) {
		if (a->a.r.v == 0) {
			ret = Integer(0);
		} else if (IsInteger(b)) {
			if (b->a.i.v == 0) {
				ret = Integer(0);
			} else {
				ret = Real(a->a.r.v * (double)b->a.i.v);
			}
		} else if (IsReal(b)) {
			ret = Real(a->a.r.v * b->a.r.v);
		} else if (IsBigInt(b)) {
			AGmNode *bb = ToReal(b);
			ret = Real(a->a.r.v * bb->a.r.v);
			RefUnref(bb);
		} else if (IsGFp(b)) {
			AGmNode* i = ToInteger(a);
			AGmNode* g = GFp(i->a.i.v, b->a.gp.mod);
			ret = GFp((g->a.gp.v * b->a.gp.v) % b->a.gp.mod, b->a.gp.mod);
			RefUnref(i, g);
		}
	} else if (IsBigInt(a)) {
		if (a->a.bi.sign == 0) {
			ret = Integer(0);
		} else if (IsInteger(b)) {
			if (b->a.i.v == 0) {
				ret = Integer(0);
			} else {
				AGmNode *bb = ToBigInt(b);
				ret = Mul((long)a->a.bi.sign, a->a.bi.bin, (long)bb->a.bi.sign, bb->a.bi.bin);
				RefUnref(bb);
			}
		} else if (IsReal(b)) {
			if (b->a.r.v == 0) {
				ret = Integer(0);
			} else {
				AGmNode *aa = ToReal(a);
				ret = Real(aa->a.r.v * b->a.r.v);
				RefUnref(aa);
			}
		} else if (IsBigInt(b)) {
			if (b->a.bi.sign == 0) {
				ret = Integer(0);
			} else {
				ret = Mul((long)a->a.bi.sign, a->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin);
			}
		} else if (IsGFp(b)) {
			AGmNode* d = Integer(b->a.gp.mod);
			AGmNode* m0 = Mod(a, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			ret = GFp((m->a.i.v * b->a.gp.v) % b->a.gp.mod, b->a.gp.mod);
			RefUnref(d, m);
		}
	} else if (IsGFp(a)) {
		if (IsInteger(b)) {
			ret = GFp((a->a.i.v * b->a.i.v) % a->a.gp.mod, a->a.gp.mod);
		} else if (IsReal(b)) {
			AGmNode* i = ToInteger(b);
			ret = GFp((a->a.i.v * i->a.i.v) % a->a.gp.mod, a->a.gp.mod);
			RefUnref(i);
		} else if (IsBigInt(b)) {
			AGmNode* d = Integer(a->a.gp.mod);
			AGmNode* m0 = Mod(b, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			ret = GFp((a->a.gp.v * m->a.i.v) % a->a.gp.mod, a->a.gp.mod);
			RefUnref(d, m);
		} else if (IsGFp(b)) {
			if (a->a.gp.mod == a->a.gp.mod) {
				ret = GFp((a->a.gp.v * b->a.gp.v) % a->a.gp.mod, a->a.gp.mod);
			}
		}
	}
	return ret;
}
AGmNode* BigInteger::Div(AGmNode* a, AGmNode* b) {
	AGmNode *ret = NULL;
	if (IsInteger(a)) {
		if (a->a.i.v == 0) {
			ret = Integer(0);
		} else if (IsInteger(b)) {
			if (b->a.i.v != 0) {
				ret = Integer(a->a.i.v / b->a.i.v);
			}
		} else if (IsReal(b)) {
			if (b->a.r.v != 0) {
				ret = Real((double)a->a.i.v / b->a.r.v);
			}
		} else if (IsBigInt(b)) {
			if (b->a.bi.sign != 0) {
				AGmNode *aa = ToBigInt(a);
				AGmNode *r = NULL;
				AGmNode *q = Div((long)aa->a.bi.sign, aa->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin, r);
				AGmNode *qq = ToInteger(q);
				if (qq) {
					ret = qq; RefUnref(q, r, aa);
				} else {
					ret = q; RefUnref(r, aa);
				}
			}
		} else if (IsGFp(b)) {
			AGmNode* aa = GFp(a->a.i.v, b->a.gp.mod);
			ret = GFp((aa->a.gp.v * gfp_inverse(b->a.gp.v, b->a.gp.mod)), b->a.gp.mod);
			RefUnref(aa);
		}
	} else if (IsReal(a)) {
		if (a->a.r.v == 0) {
			ret = Integer(0);
		} else if (IsInteger(b)) {
			if (b->a.i.v != 0) {
				ret = Real(a->a.r.v / (double)b->a.i.v);
			}
		} else if (IsReal(b)) {
			if (b->a.r.v != 0) {
				ret = Real(a->a.r.v / b->a.r.v);
			}
		} else if (IsBigInt(b)) {
			if (b->a.bi.sign != 0) {
				AGmNode *bb = ToReal(b);
				ret = Real(a->a.r.v / bb->a.r.v);
				RefUnref(bb);
			}
		} else if (IsGFp(b)) {
			AGmNode* i = ToInteger(a);
			AGmNode* aa = GFp(i->a.i.v, b->a.gp.mod);
			ret = GFp((aa->a.gp.v * gfp_inverse(b->a.gp.v, b->a.gp.mod)), b->a.gp.mod);
			RefUnref(i, aa);
		}
	} else if (IsBigInt(a)) {
		if (a->a.bi.sign == 0) {
			ret = Integer(0);
		} else if (IsInteger(b)) {
			if (b->a.bi.sign != 0) {
				AGmNode *bb = ToBigInt(b);
				AGmNode *r = NULL;
				AGmNode *q = Div((long)a->a.bi.sign, a->a.bi.bin, (long)bb->a.bi.sign, bb->a.bi.bin, r);
				AGmNode *qq = ToInteger(q);
				if (qq) {
					ret = qq; RefUnref(q, r, bb);
				} else {
					ret = q; RefUnref(r, bb);
				}
			}
		} else if (IsReal(b)) {
			if (b->a.r.v != 0) {
				AGmNode *aa = ToReal(a);
				ret = Real(aa->a.r.v / b->a.r.v);
				RefUnref(aa);
			}
		} else if (IsBigInt(b)) {
			if (b->a.bi.sign != 0) {
				AGmNode *r = NULL;
				AGmNode *q = Div((long)a->a.bi.sign, a->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin, r);
				AGmNode *qq = ToInteger(q);
				if (qq) {
					ret = qq; RefUnref(q, r);
				} else {
					ret = q; RefUnref(r);
				}
			}
		} else if (IsGFp(b)) {
			AGmNode* d = Integer(b->a.gp.mod);
			AGmNode* m0 = Mod(a, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			ret = GFp((m->a.i.v * gfp_inverse(b->a.gp.v, b->a.gp.mod)), b->a.gp.mod);
			RefUnref(d, m);
		}
	} else if (IsGFp(a)) {
		if (IsInteger(b)) {
			ret = GFp((a->a.gp.v * gfp_inverse(b->a.i.v, a->a.gp.mod)), a->a.gp.mod);
		} else if (IsReal(b)) {
			AGmNode* i = ToInteger(b);
			ret = GFp((a->a.gp.v * gfp_inverse(i->a.i.v, a->a.gp.mod)), a->a.gp.mod);
			RefUnref(i);
		} else if (IsBigInt(b)) {
			AGmNode* d = Integer(a->a.gp.mod);
			AGmNode* m0 = Mod(b, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			ret = GFp((a->a.gp.v * gfp_inverse(m->a.i.v, a->a.gp.mod)), a->a.gp.mod);
			RefUnref(d, m);
		} else if (IsGFp(b)) {
			if (a->a.gp.mod == a->a.gp.mod) {
				ret = GFp((a->a.gp.v * gfp_inverse(b->a.gp.v, b->a.gp.mod)), a->a.gp.mod);
			}
		}
	}
	return ret;
}
AGmNode* BigInteger::Mod(AGmNode* a, AGmNode* b) {
	AGmNode *ret = NULL;
	if (IsInteger(a)) {
		if (IsInteger(b)) {
			if (b->a.i.v != 0) {
				ret = Integer(a->a.i.v % b->a.i.v);
			}
		} else if (IsBigInt(b)) {
			if (b->a.bi.sign != 0) {
				AGmNode *aa = ToBigInt(a);
				AGmNode *m = NULL;
				AGmNode *q = Div((long)aa->a.bi.sign, aa->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin, m);
				AGmNode *mm = ToInteger(m);
				if (mm) {
					ret = mm; RefUnref(q, m, aa);
				} else {
					ret = m; RefUnref(q, aa);
				}
			}
		} else if (IsGFp(b)) {
			AGmNode* aa = GFp(a->a.i.v, b->a.gp.mod);
			ret = GFp((aa->a.gp.v % (b->a.gp.v ? b->a.gp.v : b->a.gp.mod)), b->a.gp.mod);
			RefUnref(aa);
		}
	} else if (IsBigInt(a)) {
		if (IsInteger(b)) {
			if (b->a.i.v != 0) {
				AGmNode *bb = ToBigInt(b);
				AGmNode *m = NULL;
				AGmNode *q = Div((long)a->a.bi.sign, a->a.bi.bin, (long)bb->a.bi.sign, bb->a.bi.bin, m);
				AGmNode *mm = ToInteger(m);
				if (mm) {
					ret = mm; RefUnref(q, m, bb);
				} else {
					ret = m; RefUnref(q, bb);
				}
			}
		} else if (IsBigInt(b)) {
			AGmNode *m = NULL;
			AGmNode *q = Div((long)a->a.bi.sign, a->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin, m);
			AGmNode *mm = ToInteger(m);
			if (mm) {
				ret = mm; RefUnref(q, m);
			} else {
				ret = m; RefUnref(q);
			}
		} else if (IsGFp(b)) {
			AGmNode* d = Integer(b->a.gp.mod);
			AGmNode* m0 = Mod(a, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			ret = GFp(m->a.i.v, b->a.gp.mod);
			RefUnref(d, m);
		}
	} else if (IsGFp(a)) {
		if (IsInteger(b)) {
			AGmNode* aa = GFp(a->a.i.v, b->a.gp.mod);
			ret = GFp((a->a.gp.v % (b->a.i.v ? b->a.i.v : a->a.gp.mod)), a->a.gp.mod);
			RefUnref(aa);
		} else if (IsBigInt(b)) {
			AGmNode* d = Integer(a->a.gp.mod);
			AGmNode* m0 = Mod(b, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			ret = GFp(m->a.i.v, a->a.gp.mod);
			RefUnref(d, m);
		} else if (IsGFp(b)) {
			if (a->a.gp.mod == b->a.gp.mod) {
				ret = GFp((a->a.gp.v % (b->a.gp.v ? b->a.gp.v : a->a.gp.mod)), a->a.gp.mod);
			}
		}
	}
	return ret;
}
int BigInteger::Compare(AGmNode* a, AGmNode* b) {
	int flag = 0;
	if (IsInteger(a)) {
		if (IsInteger(b)) {
			if (a->a.i.v > b->a.i.v) flag = 1;
			if (a->a.i.v < b->a.i.v) flag = -1;
		} else if (IsBigInt(b)) {
			AGmNode *aa = ToBigInt(a);
			flag = Compare((long)aa->a.bi.sign, aa->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin);
			RefUnref(aa);
		} else if (IsReal(b)) {
			if ((double)a->a.i.v > b->a.r.v) flag = 1;
			if ((double)a->a.i.v < b->a.r.v) flag = -1;
		} else if (IsGFp(b)) {
			AGmNode* aa = GFp(a->a.i.v, b->a.gp.mod);
			if (aa->a.gp.v == b->a.gp.v) {
				flag = 0;
			} else if (aa->a.gp.v > b->a.gp.v) {
				flag = 1;
			} else {
				flag = -1;
			}
			RefUnref(aa);
		} else {
			flag = TYPE_ERROR;
		}
	} else if (IsReal(a)) {
		if (IsInteger(b)) {
			if (a->a.r.v > (double)b->a.i.v) flag = 1;
			if (a->a.r.v < (double)b->a.i.v) flag = -1;
		} else if (IsBigInt(b)) {
			AGmNode *bb = ToReal(b);
			if (a->a.r.v > bb->a.r.v) flag = 1;
			if (a->a.r.v < bb->a.r.v) flag = -1;
			RefUnref(bb);
		} else if (IsReal(b)) {
			if (a->a.r.v > b->a.r.v) flag = 1;
			if (a->a.r.v < b->a.r.v) flag = -1;
		} else if (IsGFp(b)) {
			AGmNode* i = ToInteger(a);
			AGmNode* aa = GFp(i->a.i.v, b->a.gp.mod);
			if (aa->a.gp.v == b->a.gp.v) {
				flag = 0;
			} else if (aa->a.gp.v > b->a.gp.v) {
				flag = 1;
			} else {
				flag = -1;
			}
			RefUnref(i, aa);
		} else {
			flag = TYPE_ERROR;
		}
	} else if (IsBigInt(a)) {
		if (IsInteger(b)) {
			AGmNode *bb = ToBigInt(b);
			flag = Compare((long)a->a.bi.sign, a->a.bi.bin, (long)bb->a.bi.sign, bb->a.bi.bin);
			RefUnref(bb);
		} else if (IsBigInt(b)) {
			flag = Compare((long)a->a.bi.sign, a->a.bi.bin, (long)b->a.bi.sign, b->a.bi.bin);
		} else if (IsReal(b)) {
			AGmNode *aa = ToReal(a);
			if (aa->a.r.v > b->a.r.v) flag = 1;
			if (aa->a.r.v < b->a.r.v) flag = -1;
			RefUnref(aa);
		} else if (IsGFp(b)) {
			AGmNode* d = Integer(b->a.gp.mod);
			AGmNode* m0 = Mod(a, b);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			if (m->a.gp.v == b->a.gp.v) {
				flag = 0;
			} else if (m->a.gp.v > b->a.gp.v) {
				flag = 1;
			} else {
				flag = -1;
			}
			RefUnref(d, m);
		} else {
			flag = TYPE_ERROR;
		}
	} else if (IsGFp(a)) {
		if (IsInteger(b)) {
			AGmNode* bb = GFp(b->a.i.v, a->a.gp.mod);
			if (a->a.gp.v == bb->a.gp.v) {
				flag = 0;
			} else if (a->a.gp.v > bb->a.gp.v) {
				flag = 1;
			} else {
				flag = -1;
			}
			RefUnref(bb);
		} else if (IsBigInt(b)) {
			AGmNode* d = Integer(a->a.gp.mod);
			AGmNode* m0 = Mod(a, d);
			AGmNode* m = ToInteger(m0); RefUnref(m0);
			if (m->a.gp.v == b->a.gp.v) {
				flag = 0;
			} else if (m->a.gp.v > b->a.gp.v) {
				flag = 1;
			} else {
				flag = -1;
			}
			RefUnref(d, m);
		} else if (IsReal(b)) {
			AGmNode* i = ToInteger(b);
			AGmNode* bb = GFp(i->a.i.v, b->a.gp.mod);
			if (a->a.gp.v == bb->a.gp.v) {
				flag = 0;
			} else if (a->a.gp.v > bb->a.gp.v) {
				flag = 1;
			} else {
				flag = -1;
			}
			RefUnref(i, bb);
		} else if (IsGFp(b)) {
			if (a->a.gp.v == b->a.gp.v) {
				flag = 0;
			} else if (a->a.gp.v > b->a.gp.v) {
				flag = 1;
			} else {
				flag = -1;
			}
		} else {
			flag = TYPE_ERROR;
		}
	} else {
		flag = TYPE_ERROR;
	}
	ASSERT(flag != TYPE_ERROR);
	return flag;
}
AGmNode* BigInteger::Gcd(AGmNode* a, AGmNode* b) {
	AGmNode *ret = NULL;
	if (IsInteger(a)) {
		if (IsInteger(b)) {
			if (a->a.i.v != 0 && b->a.i.v != 0) {
#if	_WINDOWS && _64BIT
				long long aa = a->a.i.v; if (aa < 0) aa = -aa;
				long long bb = b->a.i.v; if (bb < 0) bb = -bb;
#else
				long aa = a->a.i.v; if (aa < 0) aa = -aa;
				long bb = b->a.i.v; if (bb < 0) bb = -bb;
#endif
				ret = Integer(Gcd(aa, bb));
			}
		} else if (IsBigInt(b)) {
			if (a->a.i.v != 0 && b->a.bi.sign != 0) {
				AGmNode *aa = ToBigInt(a);
				AGmNode *g = Gcd(aa->a.bi.bin, b->a.bi.bin);
				AGmNode *gg = ToInteger(g);
				if (gg) {
					ret = gg; RefUnref(g, aa);
				} else {
					ret = g; RefUnref(aa);
				}
			}
		} else if (IsGFp(b)) {
			AGmNode* aa = GFp(a->a.i.v, b->a.gp.mod);
			if (Compare(aa, b) == 0) {
				ret = b;
			} else {
				ret = GFp(1, b->a.gp.mod);
			}
			RefUnref(aa);
		}
	} else if (IsBigInt(a)) {
		if (IsInteger(b)) {
			if (a->a.bi.sign != 0 && b->a.i.v != 0) {
				AGmNode *bb = ToBigInt(b);
				AGmNode *g = Gcd(a->a.bi.bin, bb->a.bi.bin);
				AGmNode *gg = ToInteger(g);
				if (gg) {
					ret = gg; RefUnref(g, bb);
				} else {
					ret = g; RefUnref(bb);
				}
			}
		} else if (IsBigInt(b)) {
			if (a->a.bi.sign != 0 && b->a.bi.sign != 0) {
				AGmNode *g = Gcd(a->a.bi.bin, b->a.bi.bin);
				AGmNode *gg = ToInteger(g);
				if (gg) {
					ret = gg; RefUnref(g);
				} else {
					ret = g; RefUnref(gg);
				}
			}
		} else if (IsGFp(b)) {
			AGmNode* d = Integer(b->a.gp.mod);
			AGmNode* m = Mod(a, d);
			if (Compare(m, b) == 0) {
				ret = b;
			} else {
				ret = GFp(1, b->a.gp.mod);
			}
			RefUnref(d, m);
		}
	} else if (IsGFp(a)) {
		if (IsInteger(b)) {
			AGmNode* bb = GFp(b->a.i.v, a->a.gp.mod);
			if (Compare(a, bb) == 0) {
				ret = a;
			} else {
				ret = GFp(1, a->a.gp.mod);
			}
			RefUnref(bb);
		} else if (IsBigInt(b)) {
			AGmNode* d = Integer(a->a.gp.mod);
			AGmNode* m = Mod(b, d);
			if (Compare(m, a) == 0) {
				ret = a;
			} else {
				ret = GFp(1, a->a.gp.mod);
			}
			RefUnref(d, m);
		} else if (IsGFp(b)) {
			if (Compare(a, b) == 0) {
				ret = b;
			} else {
				ret = GFp(1, b->a.gp.mod);
			}
		}
	}
	return ret;
}
AGmNode* BigInteger::GcdEx(AGmNode* a, AGmNode* b) {
	AGmNode *ret = NULL;
	if (IsInteger(a) && IsInteger(b)) {
#if	_WINDOWS && _64BIT
		long long aa = a->a.i.v;
		long long bb = b->a.i.v;
		if (aa && bb) {
			long long x, y, gcd;
#else
		long aa = a->a.i.v;
		long bb = b->a.i.v;
		if (aa && bb) {
			long x, y, gcd;
#endif
			GcdEx(aa, bb, x, y, gcd);
			ret = List(Integer(gcd), Integer(x), Integer(y));
		}
	} else if ((IsInteger(a) && IsBigInt(b)) || (IsBigInt(a) && IsInteger(b)) || (IsBigInt(a) && IsBigInt(b))) {
		AGmNode *aa = a, *bb = b;
		if ((IsInteger(a) && a->a.i.v < 0) || (IsBigInt(a) && a->a.bi.sign < 0)) {
			aa = Neg(a);
		}
		if ((IsInteger(b) && b->a.i.v < 0) || (IsBigInt(b) && b->a.bi.sign < 0)) {
			bb = Neg(b);
		}
		if (aa->a.bi.sign && bb->a.bi.sign) {
			AGmNode *r0, *r1, *x0, *y0, *x1, *y1;
			AGmNode *r2, *x2, *y2, *q;
			AGmNode *tmp;
			Ref(aa); Ref(bb);
			r0 = aa; r1 = bb;
			x0 = one; x1 = zero;
			y0 = zero; y1 = one;
			while (r1->a.bi.sign > 0) {
				// q = r0 / r1;
				q = Div(r0, r1);
				// r2 = r0 % r1;
				r2 = Mod(r0, r1);
				// x2 = x0 - q * x1;
				tmp = Mul(q, x1);
				x2 = Sub(x0, tmp);
				RefUnref(tmp);
				// y2 = y0 - q * y1;
				tmp = Mul(q, y1);
				y2 = Sub(y0, tmp);
				RefUnref(tmp);
				RefUnref(q);
				// r0 = r1; r1 = r2;
				RefUnref(r0); r0 = r1;
				r1 = r2;
				// x0 = x1; x1 = x2;
				RefUnref(x0); x0 = x1;
				x1 = x2;
				// y0 = y1; y1 = y2;
				RefUnref(y0); y0 = y1;
				y1 = y2;
			}
			//x = x0; y = y0; gcd = r0;
			ret = List(r0, x0, y0);
			RefUnref(r1, x1, y1);
			Unref(aa, bb);
		} else {
			RefUnref(aa, bb);
		}
	}
	return ret;
}
#if	_WINDOWS && _64BIT
long long BigInteger::Gcd(long long a, long long b) {
	for (;;) {
		long long mod = a % b;
		if (mod == 0) break;
		a = b; b = mod;
	}
	return b;
}
void BigInteger::GcdEx(long long a, long long b, long long& x, long long& y, long long& gcd) {
	long long r0, r1, r2, x0, y0, x1, y1, x2, y2, q;
	r0 = a; r1 = b;
	x0 = 1; x1 = 0;
	y0 = 0; y1 = 1;
	while (r1 > 0) {
		q = r0 / r1;
		r2 = r0 % r1;
		x2 = x0 - q * x1;
		y2 = y0 - q * y1;
		r0 = r1; r1 = r2;
		x0 = x1; x1 = x2;
		y0 = y1; y1 = y2;
	}
	x = x0; y = y0; gcd = r0;
}
#else
long BigInteger::Gcd(long a, long b) {
	for (;;) {
		long mod = a % b;
		if (mod == 0) break;
		a = b; b = mod;
	}
	return b;
}
void BigInteger::GcdEx(long a, long b, long& x, long& y, long& gcd) {
	long r0, r1, r2, x0, y0, x1, y1, x2, y2, q;
	r0 = a; r1 = b;
	x0 = 1; x1 = 0;
	y0 = 0; y1 = 1;
	while (r1 > 0) {
		q = r0 / r1;
		r2 = r0 % r1;
		x2 = x0 - q * x1;
		y2 = y0 - q * y1;
		r0 = r1; r1 = r2;
		x0 = x1; x1 = x2;
		y0 = y1; y1 = y2;
	}
	x = x0; y = y0; gcd = r0;
}
#endif

#if	_64BIT
#ifdef	_WINDOWS
int BigInteger::BitCount(long long v) {
	if (v < 0) v = -v;
	int count = 64;
	for (;;) {
		if (v & 0x8000000000000000) return count;
		count--;
		if (count == 0) return 0;
		v <<= 1;
	}
}
#else
int BigInteger::BitCount(long v) {
	if (v < 0) v = -v;
	int count = 64;
	for (;;) {
		if (v & 0x8000000000000000) return count;
		count--;
		if (count == 0) return 0;
		v <<= 1;
	}
}
#endif
#else
int BigInteger::BitCount(long v) {
	if (v < 0) v = -v;
	int count = 32;
	for (;;) {
		if (v & 0x80000000) return count;
		count--;
		if (count == 0) return 0;
		v <<= 1;
	}
}
#endif
AGmNode* BigInteger::GFpInverse(AGmNode* v, AGmNode* mod) {
	AGmNode *ret = NULL;
	if (IsInteger(v) && IsInteger(mod)) {
		ret = Integer(gfp_inverse(v->a.i.v, mod->a.i.v));
	} else if ((IsInteger(v) && IsBigInt(mod)) || (IsBigInt(v) && IsInteger(mod)) || (IsBigInt(v) && IsBigInt(mod))) {
		// GcdEx(v, mod, x, y, gcd);
		AGmNode *gcd_x_y = GcdEx(v, mod);
		if (gcd_x_y) {
			// return ((x + mod) % mod);
			AGmNode *tmp = Add(gcd_x_y->TH, mod);
			ret = Mod(tmp, mod);
			RefUnref(tmp);
			RefUnref(gcd_x_y);
		}
	}
	return ret;
}

#if	_WINDOWS && _64BIT
long long BigInteger::gfp_inverse(long long v, long long mod) {
	long long x, y, gcd;
	GcdEx(v, mod, x, y, gcd);
	return ((x + mod) % mod);
}
#else
long BigInteger::gfp_inverse(long v, long mod) {
	long x, y, gcd;
	GcdEx(v, mod, x, y, gcd);
	return ((x + mod) % mod);
}
#endif

AGmNode* BigInteger::ChineseRemainderTheorem(AGmNode* a, AGmNode* m, AGmNode* b, AGmNode* n) {
	// get x such that [x = a (mod m) x = b (mod n) ]
	AGmNode *ret = NULL;
	AGmNode *gcd_x_y = GcdEx(m, n);
	if (gcd_x_y) {
		AGmNode *gcd = gcd_x_y->H;
		AGmNode *x = gcd_x_y->TH;
		AGmNode *y = gcd_x_y->TTH;
		if (Compare(gcd, one) == 0) {
			// (a * n * y + b * m * x) % (m * n)
			AGmNode *tmp, *tmp1, *tmp2;

			tmp = Mul(a, n);
			tmp1 = Mul(tmp, y);
			RefUnref(tmp);
			tmp = Mul(b, m);
			tmp2 = Mul(tmp, x);
			RefUnref(tmp);
			tmp = Add(tmp1, tmp2);
			RefUnref(tmp1, tmp2);

			tmp1 = tmp;
			tmp2 = Mul(m, n);
			ret = Mod(tmp1, tmp2);
			RefUnref(tmp1, tmp2);
		}
		RefUnref(gcd_x_y);
	}
	return ret;
}

void _al_bi_from_str(const char* str, long *size_a, unsigned char *buf_a) {
		// size of buf_a = strlen(str) / 2 + 2
	int len = (int)strlen(str);
	long size = len / 2 + 2;
	_al_bi_clear(size, buf_a);
	unsigned char b8[1], b8_2[1];
	b8[0] = 0x0a;
	for (int i = 0; i < len; i++) {
		_al_bi_copy(size - 1, &buf_a[1], buf_a);
		_al_bi_mul(size - 1, buf_a, 1, b8, NULL, buf_a);
		b8_2[0] = (unsigned char)(str[i] - '0');
		_al_bi_add(size, buf_a, 1, b8_2);
	}
	*size_a = _al_bi_cut(size, buf_a);
}
void _al_bi_to_str(long size_a, unsigned char *buf_a, char *str) {
		// size of str = size_a * 3 + 1
	unsigned char *buf = new unsigned char[size_a];
	_al_bi_copy(size_a, buf_a, buf);
	int size = size_a * 3 + 1;
	int len = size;
	unsigned char b8[1], b8_2[1];
	b8[0] = 0x0a; str[--len] = 0;
	while (_al_bi_cut(size_a, buf)) {
		_al_bi_div(size_a, buf, 1, b8, NULL, buf, NULL, b8_2);
		str[--len] = (char)(b8_2[0] + '0');
	}
	for (int i = 0; i < size - len; i++) {
		str[i] = str[len + i];
	}
	delete [] buf;
}
void _al_bi_from_double(double r, long *size_a, unsigned char **buf_a) {
	if (r < 1) {
		size_a = 0; buf_a = NULL; return;
	}

	int num_digit = (int)ceil(log(r)/log((double)10)) + 2;
	char *str = new char[num_digit];
	art_sprintf_1(str, num_digit, "%.0lf", r);
	int len = (int)strlen(str);
	long size = len / 2 + 2;
	unsigned char *buf = new unsigned char[size];
	_al_bi_from_str(str, &size, buf);
	delete [] str;

	*size_a = size;
	*buf_a = new unsigned char[size];
	_al_bi_copy(size, &buf[len / 2 + 2 - size], *buf_a);
	delete [] buf;
}

void _al_bi_mul(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b,
				long *size_c, unsigned char *buf_c) {
		// size_a >= size_b, size_c == size_a + size_b
		// a * b -> c
	unsigned char **buf_buf = _al_bi_alloc_b8(size_b, buf_b);
	unsigned char *buf = new unsigned char[size_a];
	_al_bi_copy(size_a, buf_a, buf);
	_al_bi_clear(size_a + size_b, buf_c);
	for (int i = 0; i < size_a; i++) {
		int c = buf[size_a - i - 1];
		for (int j = 0; j < 8; j++) {
			if ((c & 0x01) != 0) {
				_al_bi_add(size_a + size_b - i, buf_c, size_b + 1, buf_buf[j]);
			}
			c >>= 1;
		}
	}
	if (size_c)
		*size_c = _al_bi_cut(size_a + size_b, buf_c);
	delete [] buf;
	_al_bi_free_b8(buf_buf);
}
void _al_bi_div(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b,
				long* size_c, unsigned char *buf_c, long *size_d, unsigned char *buf_d) {
		// b != 0, size_a >= size_b
		// size_c == size_a - size_b + 1, size_d = size_b
		// a / b -> c, a % b -> d
	unsigned char **buf_buf = _al_bi_alloc_b8(size_b, buf_b);
	unsigned char *buf = new unsigned char[size_a + 1];
	buf[0] = 0;
	_al_bi_copy(size_a, buf_a, &buf[1]);
	for (int i = 0; i < size_a - size_b + 1; i++) {
		int c = 0;
		for (int j = 7; j >= 0; j--) {
			c <<= 1;
			if (_al_bi_sub(size_b + 1, &buf[i], size_b + 1, buf_buf[j])) {
				_al_bi_add(size_b + 1, &buf[i], size_b + 1, buf_buf[j]);
			} else {
				c |= 0x01;
			}
		}
		if (buf_c)
			buf_c[i] = (unsigned char)c;
	}
	if (buf_c && size_c) {
		*size_c = _al_bi_cut(size_a - size_b + 1, buf_c);
	}
	if (buf_d) {
		_al_bi_copy(size_b, &buf[size_a - size_b + 1], buf_d);
	}
	if (size_d) {
		*size_d = _al_bi_cut(size_b, &buf[size_a - size_b + 1]);
	}
	delete [] buf;
	_al_bi_free_b8(buf_buf);
}

unsigned char** _al_bi_alloc_b8(long size_a, unsigned char *buf_a) {
	unsigned char **buf_buf = new unsigned char*[8];
	buf_buf[0] = new unsigned char[size_a + 1];
	buf_buf[0][0] = 0;
	_al_bi_copy(size_a, buf_a, &buf_buf[0][1]);
	for (int i = 1; i < 8; i++) {
		buf_buf[i] = new unsigned char[size_a + 1];
		_al_bi_shiftL(size_a + 1, buf_buf[i - 1], buf_buf[i]);	
	}
	return buf_buf;
}
void _al_bi_free_b8(unsigned char **buf_buf) {
	unsigned char *buf;
	for (int i = 0; i < 8; i++) {
		buf = buf_buf[i];
		delete [] buf;
	}
	delete [] buf_buf;
}

#if	_64BIT
#else
extern "C" void _al_bi_clear(long size_a, unsigned char *buf_a) {
		// 0 -> a 
#ifdef	_WINDOWS
	_asm {
		push	eax
		push	ecx
		push	edi
		mov 	ecx, size_a
		mov 	edi, buf_a
		sub 	al, al
	loop1:
		mov 	byte ptr [edi], al
		inc 	edi
		dec 	ecx
		jnz 	loop1
		pop 	edi
		pop 	ecx
		pop 	eax
	}
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -24(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v2 = (long)buf_a;	// -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -32(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rcx \n"
	"   push    %rdi \n"
#ifdef _MAC
	"   mov     -24(%rbp), %rcx \n"
	"   mov     -32(%rbp), %rdi \n"
#else
	"   mov     -16(%rbp), %rcx \n"
	"   mov     -8(%rbp), %rdi \n"
#endif
	"   sub     %al, %al \n"
	"loop1_1: \n"
	"   mov     %al, (%rdi) \n"
	"   inc     %rdi \n"
	"   dec     %rcx \n"
	"   jnz     loop1_1 \n"
	"   pop     %rdi \n"
	"   pop     %rcx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ecx \n"
	"   push    %edi \n"
#ifdef _MAC
	"   mov     -16(%ebp), %ecx \n"
	"   mov     -12(%ebp), %edi \n"
#else
	"   mov     -8(%ebp), %ecx \n"
	"   mov     -4(%ebp), %edi \n"
#endif
	"   sub     %al, %al \n"
	"loop1_1: \n"
	"   mov     %al, (%edi) \n"
	"   inc     %edi \n"
	"   dec     %ecx \n"
	"   jnz     loop1_1 \n"
	"   pop     %edi \n"
	"   pop     %ecx \n"
	"   pop     %eax \n"
#endif
	);
#endif
}
extern "C" void _al_bi_copy(long size_a, unsigned char *buf_a, unsigned char *buf_b) {
		// size_a == size_b
		// a -> b
#ifdef	_WINDOWS
	_asm {
		push	eax
		push	ecx
		push	esi
		push	edi
		mov 	ecx, size_a
		mov 	esi, buf_a
		mov 	edi, buf_b
	loop1:
		mov 	al, byte ptr [esi]
		mov 	byte ptr [edi], al
		inc 	esi
		inc 	edi
		dec 	ecx
		jnz 	loop1
		pop 	edi
		pop 	esi
		pop 	ecx
		pop 	eax
	}
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -20(%ebp) (Mac)   -12(%ebp) (Linux)
	                        // -32(%rbp) (Mac64) -24(%rbp) (Linux64)
	long v2 = (long)buf_a;	// -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -40(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v3 = (long)buf_b;	// -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -48(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rcx \n"
	"   push    %rsi \n"
	"   push    %rdi \n"
#ifdef _MAC
	"   mov     -32(%rbp), %rcx \n"
	"   mov     -40(%rbp), %rsi \n"
	"   mov     -48(%rbp), %rdi \n"
#else
	"   mov     -24(%rbp), %rcx \n"
	"   mov     -16(%rbp), %rsi \n"
	"   mov     -8(%rbp), %rdi \n"
#endif
	"loop2_1: \n"
	"   mov     (%rsi), %al \n"
	"   mov     %al, (%rdi) \n"
	"   inc     %rsi \n"
	"   inc     %rdi \n"
	"   dec     %rcx \n"
	"   jnz     loop2_1 \n"
	"   pop     %rdi \n"
	"   pop     %rsi \n"
	"   pop     %rcx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ecx \n"
	"   push    %esi \n"
	"   push    %edi \n"
#ifdef _MAC
	"   mov     -20(%ebp), %ecx \n"
	"   mov     -16(%ebp), %esi \n"
	"   mov     -12(%ebp), %edi \n"
#else
	"   mov     -12(%ebp), %ecx \n"
	"   mov     -8(%ebp), %esi \n"
	"   mov     -4(%ebp), %edi \n"
#endif
	"loop2_1: \n"
	"   mov     (%esi), %al \n"
	"   mov     %al, (%edi) \n"
	"   inc     %esi \n"
	"   inc     %edi \n"
	"   dec     %ecx \n"
	"   jnz     loop2_1 \n"
	"   pop     %edi \n"
	"   pop     %esi \n"
	"   pop     %ecx \n"
	"   pop     %eax \n"
#endif
	);
#endif
}
extern "C" void _al_bi_shiftL(long size_a, unsigned char *buf_a, unsigned char *buf_b) {
		// size_a == size_b
		// a << 1 -> b
#ifdef	_WINDOWS
	_asm {
		push	eax
		push	ecx
		push	esi
		push	edi
		mov 	ecx, size_a
		mov 	esi, buf_a
		add 	esi, ecx
		mov 	edi, buf_b
		add 	edi, ecx
		sub 	al, al
	loop1:
		dec 	esi
		dec 	edi
		mov 	al, byte ptr [esi]
		rcl 	al, 1
		mov 	byte ptr [edi], al
		dec 	ecx
		jnz 	loop1
		pop 	edi
		pop 	esi
		pop 	ecx
		pop 	eax
	}
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -20(%ebp) (Mac)   -12(%ebp) (Linux)
	                        // -32(%rbp) (Mac64) -24(%rbp) (Linux64)
	long v2 = (long)buf_a;  // -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -40(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v3 = (long)buf_b;  // -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -48(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rcx \n"
	"   push    %rsi \n"
	"   push    %rdi \n"
#ifdef _MAC
	"   mov     -32(%rbp), %rcx \n"
	"   mov     -40(%rbp), %rsi \n"
	"   mov     -48(%rbp), %rdi \n"
#else
	"   mov     -24(%rbp), %rcx \n"
	"   mov     -16(%rbp), %rsi \n"
	"   mov     -8(%rbp), %rdi \n"
#endif
	"   add     %rcx, %rsi \n"
	"   add     %rcx, %rdi \n"
	"   sub     %al, %al \n"
	"loop3_1: \n"
	"   dec     %rsi \n"
	"   dec     %rdi \n"
	"   mov     (%rsi), %al \n"
	"   rcl     %al \n"
	"   mov     %al, (%rdi) \n"
	"   dec     %rcx \n"
	"   jnz     loop3_1 \n"
	"   pop     %rdi \n"
	"   pop     %rsi \n"
	"   pop     %rcx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ecx \n"
	"   push    %esi \n"
	"   push    %edi \n"
#ifdef _MAC
	"   mov     -20(%ebp), %ecx \n"
	"   mov     -16(%ebp), %esi \n"
	"   mov     -12(%ebp), %edi \n"
#else
	"   mov     -8(%ebp), %ecx \n"
	"   mov     -16(%ebp), %esi \n"
	"   mov     -24(%ebp), %edi \n"
#endif
	"   add     %ecx, %esi \n"
	"   add     %ecx, %edi \n"
	"   sub     %al, %al \n"
	"loop3_1: \n"
	"   dec     %esi \n"
	"   dec     %edi \n"
	"   mov     (%esi), %al \n"
	"   rcl     %al \n"
	"   mov     %al, (%edi) \n"
	"   dec     %ecx \n"
	"   jnz     loop3_1 \n"
	"   pop     %edi \n"
	"   pop     %esi \n"
	"   pop     %ecx \n"
	"   pop     %eax \n"
#endif
	);
#endif
}
extern "C" void _al_bi_shiftR(long size_a, unsigned char *buf_a, unsigned char *buf_b) {
		// size_a == size_b
		// a >> 1 -> b
#ifdef	_WINDOWS
	_asm {
		push	eax
		push	ecx
		push	esi
		push	edi
		mov 	ecx, size_a
		mov 	esi, buf_a
		mov 	edi, buf_b
		sub 	al, al
	loop1:
		mov 	al, byte ptr [esi]
		rcr 	al, 1
		mov 	byte ptr [edi], al
		inc 	esi
		inc 	edi
		dec 	ecx
		jnz 	loop1
		pop 	edi
		pop 	esi
		pop 	ecx
		pop 	eax
	}
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -20(%ebp) (Mac)   -12(%ebp) (Linux)
	                        // -32(%rbp) (Mac64) -24(%rbp) (Linux64)
	long v2 = (long)buf_a;  // -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -40(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v3 = (long)buf_b;  // -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -48(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rcx \n"
	"   push    %rsi \n"
	"   push    %rdi \n"
#ifdef _MAC
	"   mov     -32(%rbp), %rcx \n"
	"   mov     -40(%rbp), %rsi \n"
	"   mov     -48(%rbp), %rdi \n"
#else
	"   mov     -24(%rbp), %rcx \n"
	"   mov     -16(%rbp), %rsi \n"
	"   mov     -8(%rbp), %rdi \n"
#endif
	"   sub     %al, %al \n"
	"loop4_1: \n"
	"   mov     (%rsi), %al \n"
	"   rcr     %al \n"
	"   mov     %al, (%rdi) \n"
	"   inc     %rsi \n"
	"   inc     %rdi \n"
	"   dec     %rcx \n"
	"   jnz     loop4_1 \n"
	"   pop     %rdi \n"
	"   pop     %rsi \n"
	"   pop     %rcx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ecx \n"
	"   push    %esi \n"
	"   push    %edi \n"
#ifdef _MAC
	"   mov     -20(%ebp), %ecx \n"
	"   mov     -16(%ebp), %esi \n"
	"   mov     -12(%ebp), %edi \n"
#else
	"   mov     -12(%ebp), %ecx \n"
	"   mov     -8(%ebp), %esi \n"
	"   mov     -4(%ebp), %edi \n"
#endif
	"   sub     %al, %al \n"
	"loop4_1: \n"
	"   mov     (%esi), %al \n"
	"   rcr     %al \n"
	"   mov     %al, (%edi) \n"
	"   inc     %esi \n"
	"   inc     %edi \n"
	"   dec     %ecx \n"
	"   jnz     loop4_1 \n"
	"   pop     %edi \n"
	"   pop     %esi \n"
	"   pop     %ecx \n"
	"   pop     %eax \n"
#endif
	);
#endif
}
extern "C" int _al_bi_add(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b) {
		// size_a >= size_b
		// a + b -> a
		// if overflow, return 1
#ifdef	_WINDOWS
	int ret_val = 0;
	_asm {
		push	eax
		push	ecx
		push	edx
		push	esi
		push	edi
		mov 	ecx, size_a
		mov 	edi, buf_a
		add 	edi, ecx
		mov 	edx, size_b
		mov 	esi, buf_b
		add 	esi, edx
		sub 	ecx, edx
		inc 	ecx
		sub 	al, al
	loop1:
		dec 	edi
		dec 	esi
		mov 	al, byte ptr [edi]
		adc 	al, byte ptr [esi]
		mov 	byte ptr [edi], al
		dec 	edx
		jnz 	loop1
		jnc 	skip2
	loop2:
		dec 	ecx
		jz		skip1
		dec 	edi
		mov 	al, byte ptr [edi]
		adc 	al, 0
		mov 	byte ptr [edi], al
		jnc 	skip2
		jmp 	loop2
	skip1:
		mov 	ret_val, 1
	skip2:
		pop 	edi
		pop 	esi
		pop 	edx
		pop 	ecx
		pop 	eax
	}
	return ret_val;
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -28(%ebp) (Mac)   -20(%ebp) (Linux)
	                        // -48(%rbp) (Mac64) -40(%rbp) (Linux64)
	long v2 = (long)buf_a;  // -24(%ebp) (Mac)   -16(%ebp) (Linux)
	                        // -56(%rbp) (Mac64) -32(%rbp) (Linux64)
	long v3 = (long)size_b; // -20(%ebp) (Mac)   -12(%ebp) (Linux)
	                        // -64(%rbp) (Mac64) -24(%rbp) (Linux64)
	long v4 = (long)buf_b;  // -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -72(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v5 = 0;            // -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -80(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rbx \n"
	"   push    %rcx \n"
	"   push    %rdx \n"
	"   push    %rsi \n"
	"   push    %rdi \n"
	"   sub     %rbx, %rbx \n"
#ifdef _MAC
	"   mov     -48(%rbp), %rcx \n"
	"   mov     -56(%rbp), %rdi \n"
	"   mov     -64(%rbp), %rdx \n"
	"   mov     -72(%rbp), %rsi \n"
#else
	"   mov     -40(%rbp), %rcx \n"
	"   mov     -32(%rbp), %rdi \n"
	"   mov     -24(%rbp), %rdx \n"
	"   mov     -16(%rbp), %rsi \n"
#endif
	"   add     %rcx, %rdi \n"
	"   add     %rdx, %rsi \n"
	"   sub     %rdx, %rcx \n"
	"   inc     %rcx \n"
	"   sub     %al, %al \n"
	"loop5_1: \n"
	"   dec     %rdi \n"
	"   dec     %rsi \n"
	"   mov     (%rdi), %al \n"
	"   adc     (%rsi), %al \n"
	"   mov     %al, (%rdi) \n"
	"   dec     %rdx \n"
	"   jnz     loop5_1 \n"
	"   jnc     skip5_2 \n"
	"loop5_2: \n"
	"   dec     %rcx \n"
	"   jz      skip5_1 \n"
	"   dec     %rdi \n"
	"   mov     (%rdi), %al \n"
	"   adc     %bl, %al \n"
	"   mov     %al, (%rdi) \n"
	"   jnc     skip5_2 \n"
	"   jmp     loop5_2 \n"
	"skip5_1: \n"
	"   sub     %rax, %rax \n"
	"   inc     %rax \n"
#ifdef _MAC
	"   mov     %rax, -80(%rbp) \n"
#else
	"   mov     %rax, -8(%rbp) \n"
#endif
	"skip5_2: \n"
	"   pop     %rdi \n"
	"   pop     %rsi \n"
	"   pop     %rdx \n"
	"   pop     %rcx \n"
	"   pop     %rbx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ebx \n"
	"   push    %ecx \n"
	"   push    %edx \n"
	"   push    %esi \n"
	"   push    %edi \n"
	"   sub     %ebx, %ebx \n"
#ifdef _MAC
	"   mov     -28(%ebp), %ecx \n"
	"   mov     -24(%ebp), %edi \n"
	"   mov     -20(%ebp), %edx \n"
	"   mov     -16(%ebp), %esi \n"
#else
	"   mov     -20(%ebp), %ecx \n"
	"   mov     -16(%ebp), %edi \n"
	"   mov     -12(%ebp), %edx \n"
	"   mov     -8(%ebp), %esi \n"
#endif
	"   add     %ecx, %edi \n"
	"   add     %edx, %esi \n"
	"   sub     %edx, %ecx \n"
	"   inc     %ecx \n"
	"   sub     %al, %al \n"
	"loop5_1: \n"
	"   dec     %edi \n"
	"   dec     %esi \n"
	"   mov     (%edi), %al \n"
	"   adc     (%esi), %al \n"
	"   mov     %al, (%edi) \n"
	"   dec     %edx \n"
	"   jnz     loop5_1 \n"
	"   jnc     skip5_2 \n"
	"loop5_2: \n"
	"   dec     %ecx \n"
	"   jz      skip5_1 \n"
	"   dec     %edi \n"
	"   mov     (%edi), %al \n"
	"   adc     %bl, %al \n"
	"   mov     %al, (%edi) \n"
	"   jnc     skip5_2 \n"
	"   jmp     loop5_2 \n"
	"skip5_1: \n"
	"   sub     %eax, %eax \n"
	"   inc     %eax \n"
#ifdef _MAC
	"   mov     %eax, -12(%ebp) \n"
#else
	"   mov     %eax, -4(%ebp) \n"
#endif
	"skip5_2: \n"
	"   pop     %edi \n"
	"   pop     %esi \n"
	"   pop     %edx \n"
	"   pop     %ecx \n"
	"   pop     %ebx \n"
	"   pop     %eax \n"
#endif
	);
	return v5;
#endif
}
extern "C" int _al_bi_sub(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b) {
		// size_a >= size_b
		// a - b -> a
		// if negative, return 1
#ifdef	_WINDOWS
	int ret_val = 0;
	_asm {
		push	eax
		push	ecx
		push	edx
		push	esi
		push	edi
		mov 	ecx, size_a
		mov 	edi, buf_a
		add 	edi, ecx
		mov 	edx, size_b
		mov 	esi, buf_b
		add 	esi, edx
		sub 	ecx, edx
		inc 	ecx
		sub 	al ,al
	loop1:
		dec 	edi
		dec 	esi
		mov 	al, byte ptr [edi]
		sbb 	al, byte ptr [esi]
		mov 	byte ptr [edi], al
		dec 	edx
		jnz 	loop1
		jnc 	skip2
	loop2:
		dec 	ecx
		jz		skip1
		dec 	edi
		mov 	al, byte ptr [edi]
		sbb 	al, 0
		mov 	byte ptr [edi], al
		jnc 	skip2
		jmp 	loop2
	skip1:
		mov 	ret_val, 1
	skip2:
		pop 	edi
		pop 	esi
		pop 	edx
		pop 	ecx
		pop 	eax
	}
	return ret_val;
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -28(%ebp) (Mac)   -20(%ebp) (Linux)
	                        // -48(%rbp) (Mac64) -40(%rbp) (Linux64)
	long v2 = (long)buf_a;  // -24(%ebp) (Mac)   -16(%ebp) (Linux)
	                        // -56(%rbp) (Mac64) -32(%rbp) (Linux64)
	long v3 = (long)size_b; // -20(%ebp) (Mac)   -12(%ebp) (Linux)
	                        // -64(%rbp) (Mac64) -24(%rbp) (Linux64)
	long v4 = (long)buf_b;  // -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -72(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v5 = 0;            // -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -80(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rbx \n"
	"   push    %rcx \n"
	"   push    %rdx \n"
	"   push    %rsi \n"
	"   push    %rdi \n"
	"   sub     %rbx, %rbx \n"
#ifdef _MAC
	"   mov     -48(%rbp), %rcx \n"
	"   mov     -56(%rbp), %rdi \n"
	"   mov     -64(%rbp), %rdx \n"
	"   mov     -72(%rbp), %rsi \n"
#else
	"   mov     -40(%rbp), %rcx \n"
	"   mov     -32(%rbp), %rdi \n"
	"   mov     -24(%rbp), %rdx \n"
	"   mov     -16(%rbp), %rsi \n"
#endif
	"   add     %rcx, %rdi \n"
	"   add     %rdx, %rsi \n"
	"   sub     %rdx, %rcx \n"
	"   inc     %rcx \n"
	"   sub     %al, %al \n"
	"loop6_1: \n"
	"   dec     %rdi \n"
	"   dec     %rsi \n"
	"   mov     (%rdi), %al \n"
	"   sbb     (%rsi), %al \n"
	"   mov     %al, (%rdi) \n"
	"   dec     %rdx \n"
	"   jnz     loop6_1 \n"
	"   jnc     skip6_2 \n"
	"loop6_2: \n"
	"   dec     %rcx \n"
	"   jz      skip6_1 \n"
	"   dec     %rdi \n"
	"   mov     (%rdi), %al \n"
	"   sbb     %bl, %al \n"
	"   mov     %al, (%rdi) \n"
	"   jnc     skip6_2 \n"
	"   jmp     loop6_2 \n"
	"skip6_1: \n"
	"   sub     %rax, %rax \n"
	"   inc     %rax \n"
#ifdef _MAC
	"   mov     %rax, -80(%rbp) \n"
#else
	"   mov     %rax, -8(%rbp) \n"
#endif
	"skip6_2: \n"
	"   pop     %rdi \n"
	"   pop     %rsi \n"
	"   pop     %rdx \n"
	"   pop     %rcx \n"
	"   pop     %rbx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ebx \n"
	"   push    %ecx \n"
	"   push    %edx \n"
	"   push    %esi \n"
	"   push    %edi \n"
	"   sub     %ebx, %ebx \n"
#ifdef _MAC
	"   mov     -28(%ebp), %ecx \n"
	"   mov     -24(%ebp), %edi \n"
	"   mov     -20(%ebp), %edx \n"
	"   mov     -16(%ebp), %esi \n"
#else
	"   mov     -20(%ebp), %ecx \n"
	"   mov     -16(%ebp), %edi \n"
	"   mov     -12(%ebp), %edx \n"
	"   mov     -8(%ebp), %esi \n"
#endif
	"   add     %ecx, %edi \n"
	"   add     %edx, %esi \n"
	"   sub     %edx, %ecx \n"
	"   inc     %ecx \n"
	"   sub     %al, %al \n"
	"loop6_1: \n"
	"   dec     %edi \n"
	"   dec     %esi \n"
	"   mov     (%edi), %al \n"
	"   sbb     (%esi), %al \n"
	"   mov     %al, (%edi) \n"
	"   dec     %edx \n"
	"   jnz     loop6_1 \n"
	"   jnc     skip6_2 \n"
	"loop6_2: \n"
	"   dec     %ecx \n"
	"   jz      skip6_1 \n"
	"   dec     %edi \n"
	"   mov     (%edi), %al \n"
	"   sbb     %bl, %al \n"
	"   mov     %al, (%edi) \n"
	"   jnc     skip6_2 \n"
	"   jmp     loop6_2 \n"
	"skip6_1: \n"
	"   sub     %eax, %eax \n"
	"   inc     %eax \n"
#ifdef _MAC
	"   mov     %eax, -12(%ebp) \n"
#else
	"   mov     %eax, -4(%ebp) \n"
#endif
	"skip6_2: \n"
	"   pop     %edi \n"
	"   pop     %esi \n"
	"   pop     %edx \n"
	"   pop     %ecx \n"
	"   pop     %ebx \n"
	"   pop     %eax \n"
#endif
	);
	return v5;
#endif
}
extern "C" void _al_bi_neg(long size_a, unsigned char *buf_a, unsigned char *buf_b) {
		// size_a == size_b
		// 0 - a -> b
#ifdef	_WINDOWS
	_asm {
		push	eax
		push	ecx
		push	esi
		push	edi
		mov 	ecx, size_a
		mov 	esi, buf_a
		add 	esi, ecx
		mov 	edi, buf_b
		add 	edi, ecx
		sub 	al, al
	loop1:
		dec 	esi
		dec 	edi
		mov 	al, 0
		sbb 	al, byte ptr [esi]
		mov 	byte ptr [edi], al
		dec 	ecx
		jnz 	loop1
		pop 	edi
		pop 	esi
		pop 	ecx
		pop 	eax
	}
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -20(%ebp) (Mac)   -12(%ebp) (Linux)
	                        // -32(%rbp) (Mac64) -24(%rbp) (Linux64)
	long v2 = (long)buf_a;  // -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -40(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v3 = (long)buf_b;  // -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -48(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rbx \n"
	"   push    %rcx \n"
	"   push    %rsi \n"
	"   push    %rdi \n"
	"   sub     %rbx, %rbx \n"
#ifdef _MAC
	"   mov     -32(%rbp), %rcx \n"
	"   mov     -40(%rbp), %rsi \n"
	"   mov     -48(%rbp), %rdi \n"
#else
	"   mov     -24(%rbp), %rcx \n"
	"   mov     -16(%rbp), %rsi \n"
	"   mov     -8(%rbp), %rdi \n"
#endif
	"   add     %rcx, %rsi \n"
	"   add     %rcx, %rdi \n"
	"   sub     %al, %al \n"
	"loop7_1: \n"
	"   dec     %rsi \n"
	"   dec     %rdi \n"
	"   mov     %bl, %al \n"
	"   sbb     (%rsi), %al \n"
	"   mov     %al, (%rdi) \n"
	"   dec     %rcx \n"
	"   jnz     loop7_1 \n"
	"   pop     %rdi \n"
	"   pop     %rsi \n"
	"   pop     %rcx \n"
	"   pop     %rbx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ebx \n"
	"   push    %ecx \n"
	"   push    %esi \n"
	"   push    %edi \n"
	"   sub     %ebx, %ebx \n"
#ifdef _MAC
	"   mov     -20(%ebp), %ecx \n"
	"   mov     -16(%ebp), %esi \n"
	"   mov     -12(%ebp), %edi \n"
#else
	"   mov     -12(%ebp), %ecx \n"
	"   mov     -8(%ebp), %esi \n"
	"   mov     -4(%ebp), %edi \n"
#endif
	"   add     %ecx, %esi \n"
	"   add     %ecx, %edi \n"
	"   sub     %al, %al \n"
	"loop7_1: \n"
	"   dec     %esi \n"
	"   dec     %edi \n"
	"   mov     %bl, %al \n"
	"   sbb     (%esi), %al \n"
	"   mov     %al, (%edi) \n"
	"   dec     %ecx \n"
	"   jnz     loop7_1 \n"
	"   pop     %edi \n"
	"   pop     %esi \n"
	"   pop     %ecx \n"
	"   pop     %ebx \n"
	"   pop     %eax \n"
#endif
	);
#endif
}
extern "C" long _al_bi_cut(long size_a, unsigned char *buf_a) {
#ifdef	_WINDOWS
	int ret_val;
	_asm {
		push	eax
		push	ecx
		push	esi
		mov 	ecx, size_a
		mov 	esi, buf_a
	loop1:
		mov 	al, byte ptr [esi]
		or		al, al
		jnz 	skip1
		inc 	esi
		dec 	ecx
		jnz 	loop1
	skip1:
		mov 	ret_val, ecx
		pop 	esi
		pop 	ecx
		pop 	eax
	}
	return ret_val;
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -20(%ebp) (Mac)   -12(%ebp) (Linux)
	                        // -40(%rbp) (Mac64) -24(%rbp) (Linux64)
	long v2 = (long)buf_a;  // -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -48(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v3;                // -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -56(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rcx \n"
	"   push    %rsi \n"
#ifdef _MAC
	"   mov     -40(%rbp), %rcx \n"
	"   mov     -48(%rbp), %rsi \n"
#else
	"   mov     -24(%rbp), %rcx \n"
	"   mov     -16(%rbp), %rsi \n"
#endif
	"loop8_1: \n"
	"   mov     (%rsi), %al \n"
	"   or      %al, %al \n"
	"   jnz     skip8_1 \n"
	"   inc     %rsi \n"
	"   dec     %rcx \n"
	"   jnz     loop8_1 \n"
	"skip8_1: \n"
#ifdef _MAC
	"   mov     %rcx, -56(%rbp) \n"
#else
	"   mov     %rcx, -8(%rbp) \n"
#endif
	"   pop     %rsi \n"
	"   pop     %rcx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ecx \n"
	"   push    %esi \n"
#ifdef _MAC
	"   mov     -20(%ebp), %ecx \n"
	"   mov     -16(%ebp), %esi \n"
#else
	"   mov     -12(%ebp), %ecx \n"
	"   mov     -8(%ebp), %esi \n"
#endif
	"loop8_1: \n"
	"   mov     (%esi), %al \n"
	"   or      %al, %al \n"
	"   jnz     skip8_1 \n"
	"   inc     %esi \n"
	"   dec     %ecx \n"
	"   jnz     loop8_1 \n"
	"skip8_1: \n"
#ifdef _MAC
	"   mov     %ecx, -12(%ebp) \n"
#else
	"   mov     %ecx, -4(%ebp) \n"
#endif
	"   pop     %esi \n"
	"   pop     %ecx \n"
	"   pop     %eax \n"
#endif
	);
	return v3;
#endif
}
extern "C" int _al_bi_comp(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b) {
		// if a > b, return 1
		// if a == b, return 0
		// if a < b, return -1
	if (size_a > size_b) return 1;
	if (size_a < size_b) return -1;
#ifdef	_WINDOWS
	int ret_val;
	_asm {
		push	eax
		push	ecx
		push	esi
		push	edi
		mov 	ecx, size_a
		mov 	edi, buf_a
		mov 	esi, buf_b
	loop1:
		mov 	al, byte ptr [edi]
		cmp 	al, byte ptr [esi]
		jc		skip1
		jnz 	skip2
		inc 	edi
		inc 	esi
		dec 	ecx
		jnz 	loop1
		mov 	ret_val, 0
		jmp 	skip3
	skip1:
		mov 	ret_val, -1
		jmp 	skip3
	skip2:
		mov 	ret_val, 1
	skip3:
		pop 	edi
		pop 	esi
		pop 	ecx
		pop 	eax
	}
	return ret_val;
#endif
#ifdef	_LINUX
	long v1 = (long)size_a; // -24(%ebp) (Mac)   -16(%ebp) (Linux)
	                        // -48(%rbp) (Mac64) -32(%rbp) (Linux64)
	long v2 = (long)buf_a;  // -20(%ebp) (Mac)   -12(%ebp) (Linux)
	                        // -56(%rbp) (Mac64) -24(%rbp) (Linux64)
	long v3 = (long)buf_b;	// -16(%ebp) (Mac)    -8(%ebp) (Linux)
	                        // -64(%rbp) (Mac64) -16(%rbp) (Linux64)
	long v4;                // -12(%ebp) (Mac)    -4(%ebp) (Linux)
	                        // -72(%rbp) (Mac64)  -8(%rbp) (Linux64)
	__asm__  (
#ifdef	_64BIT
	"   push    %rax \n"
	"   push    %rcx \n"
	"   push    %rsi \n"
	"   push    %rdi \n"
#ifdef _MAC
	"   mov     -48(%rbp), %rcx \n"
	"   mov     -56(%rbp), %rdi \n"
	"   mov     -64(%rbp), %rsi \n"
#else
	"   mov     -32(%rbp), %rcx \n"
	"   mov     -24(%rbp), %rdi \n"
	"   mov     -16(%rbp), %rsi \n"
#endif
	"loop9_1: \n"
	"   mov     (%rdi), %al \n"
	"   cmp     (%rsi), %al \n"
	"   jc      skip9_1 \n"
	"   jnz     skip9_2 \n"
	"   inc     %rdi \n"
	"   inc     %rsi \n"
	"   dec     %rcx \n"
	"   jnz     loop9_1 \n"
	"   sub     %rax, %rax \n"
	"   jmp     skip9_3 \n"
	"skip9_1: \n"
	"   sub     %rax, %rax \n"
	"   dec     %rax \n"
	"   jmp     skip9_3 \n"
	"skip9_2: \n"
	"   sub     %rax, %rax \n"
	"   inc     %rax \n"
	"skip9_3: \n"
#ifdef _MAC
	"   mov     %rax, -72(%rbp) \n"
#else
	"   mov     %rax, -8(%rbp) \n"
#endif
	"   pop     %rdi \n"
	"   pop     %rsi \n"
	"   pop     %rcx \n"
	"   pop     %rax \n"
#else
	"   push    %eax \n"
	"   push    %ecx \n"
	"   push    %esi \n"
	"   push    %edi \n"
#ifdef _MAC
	"   mov     -24(%ebp), %ecx \n"
	"   mov     -20(%ebp), %edi \n"
	"   mov     -16(%ebp), %esi \n"
#else
	"   mov     -16(%ebp), %ecx \n"
	"   mov     -12(%ebp), %edi \n"
	"   mov     -8(%ebp), %esi \n"
#endif
	"loop9_1: \n"
	"   mov     (%edi), %al \n"
	"   cmp     (%esi), %al \n"
	"   jc      skip9_1 \n"
	"   jnz     skip9_2 \n"
	"   inc     %edi \n"
	"   inc     %esi \n"
	"   dec     %ecx \n"
	"   jnz     loop9_1 \n"
	"   sub     %eax, %eax \n"
	"   jmp     skip9_3 \n"
	"skip9_1: \n"
	"   sub     %eax, %eax \n"
	"   dec     %eax \n"
	"   jmp     skip9_3 \n"
	"skip9_2: \n"
	"   sub     %eax, %eax \n"
	"   inc     %eax \n"
	"skip9_3: \n"
#ifdef _MAC
	"   mov     %eax, -12(%ebp) \n"
#else
	"   mov     %eax, -4(%ebp) \n"
#endif
	"   pop     %edi \n"
	"   pop     %esi \n"
	"   pop     %ecx \n"
	"   pop     %eax \n"
#endif
	);
	return v4;
#endif
}
#endif	// _WINDOWS && _64BIT

void _al_bi_gcd(long size_a, unsigned char *buf_a, long size_b, unsigned char *buf_b,
			   long* size_c, unsigned char *buf_c) {
		// greatest common divisor
		// size of buf_c == min(size_a, size_b)
	if (size_a < size_b) {
		long size; unsigned char *buf;
		size = size_a; size_a = size_b; size_b = size;
		buf = buf_a;   buf_a = buf_b;	buf_b = buf;
	}
	long size_aa = size_a, size_bb = size_b;
	unsigned char *buf_aa = new unsigned char[size_aa];
	unsigned char *buf_bb = new unsigned char[size_bb];
	_al_bi_copy(size_aa, buf_a, buf_aa);
	_al_bi_copy(size_bb, buf_b, buf_bb);
	for (;;) {
		long size_cc;
		_al_bi_div(size_aa, buf_aa, size_bb, buf_bb, NULL, NULL, &size_cc, buf_c);
		if (size_cc == 0) break;
		_al_bi_copy(size_bb, buf_bb, buf_aa);
		_al_bi_copy(size_cc, &buf_c[size_bb - size_cc], buf_bb);
		size_aa = size_bb; size_bb = size_cc;
	}
	_al_bi_copy(size_bb, buf_bb, buf_c);
	if (size_c) *size_c = size_bb;
	delete [] buf_bb;
	delete [] buf_aa;
}

AGmNode* BigInteger::factor(AGmNode* a) {
	AGmNode *ret = NULL, *ret1 = NULL, *ret2 = NULL;
	if (IsInteger(a)) {
#if	_WINDOWS && _64BIT
		long long n = a->a.i.v; if (n < 0) n = -n;
		long i = 1;
		for (;; i++) {
			if (n % i == 0) {
				ret1 = Nconc(ret1, Cons(Integer(i)));
				long long j = n / i;
				if (i < j) {
					ret2 = Cons(Integer(j), ret2);
				} else {
					break;
				}
			}
		}
#else
		long n = a->a.i.v; if (n < 0) n = -n;
		int i = 1;
		for (;; i++) {
			if (n % i == 0) {
				ret1 = Nconc(ret1, Cons(Integer(i)));
				long j = n / i;
				if (i < j) {
					ret2 = Cons(Integer(j), ret2);
				} else {
					break;
				}
			}
		}
		ret = Nconc(ret1, ret2);
		if (ret) ret->count--;
#endif
	} else if (IsBigInt(a)) {
		AGmNode *n = a;
		if (n->a.bi.sign < 0) {
			n = Neg(n);
		}
		AGmNode *i = one, *tmp;
		for (;; tmp = i, i = Add(tmp, one), RefUnref(tmp)) {
			AGmNode *mod = Mod(n, i);
			if (mod->a.bi.sign == 0) {
				ret1 = Nconc(ret1, Cons(i));
				AGmNode *j = Div(n, i);
				if (i < j) {
					ret2 = Cons(j, ret2);
				} else {
					break;
				}
			}
		}
		RefUnref(n);
		ret = Nconc(ret1, ret2);
		if (ret) ret->count--;
	}
	return ret;
}
