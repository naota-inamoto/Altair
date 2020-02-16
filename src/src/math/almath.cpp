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
#include <gdm/file.h>
#include <gdm/debug.h>
#include <gdm/big_int.h>
#include <interp/interp.h>
#include <math/almath.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#if defined(_DEBUG) || defined(_LINUX_DEBUG)
//#define DEBUG_ADD
//#define DEBUG_SUB
//#define DEBUG_NEG
//#define DEBUG_COMP
//#define DEBUG_MUL
//#define DEBUG_DIVMOD
//#define DEBUG_COMMON
//#define DEBUG_COMMON2
//#define DEBUG_NORMALIZE
//#define DEBUG_NORMALIZE2
//#define DEBUG_NORMALIZE3
#endif

int AGmMath::error(const char* s1, const char* s2, AGmNode*& ret) {
	char msg[256];
	art_sprintf_2(msg, 256, "%s, in 'math'-'%s' function", s1, s2);
	ret = String(msg); return 1;
}

int AGmMath::InterpretAndExecution(int argc, AGmNode *arg_ls, AGmNode*& ret) {
	if (argc < 1) return error("arguments required", "", ret);
	if (!IsString(arg_ls->H)) return error("1st argument must be command string", "", ret);
	const char *s = arg_ls->H->a.s.v->Str();
	if (strcmp(s, "add_poly") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH, *b = arg_ls->TTH;
		ret = add_poly(a, b);
	} else if (strcmp(s, "sub_poly") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH, *b = arg_ls->TTH;
		ret = sub_poly(a, b);
	} else if (strcmp(s, "mul_poly") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH, *b = arg_ls->TTH;
		ret = mul_poly(a, b);
	} else if (strcmp(s, "mul_poly_by_term") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH, *b = arg_ls->TTH;
		ret = mul_poly_by_term(a, b);
	} else if (strcmp(s, "divmod_poly") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH, *b = arg_ls->TTH;
		ret = divmod_poly(a, b);
	} else if (strcmp(s, "divmod_poly2") == 0) {
		if (argc != 6) return error("just 5 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH, *b = arg_ls->TTH;
		AGmNode *vf = arg_ls->TTTH, *v = arg_ls->TTTTH, *l = arg_ls->TTTTTH;
		ret = divmod_poly2(a, b, vf, v, l);
	} else if (strcmp(s, "get_first_var") == 0) {
		if (argc != 2) return error("just 1 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH;
		ret = get_first_var(a);
	} else if (strcmp(s, "degree_poly") == 0) {
		if (argc != 5) return error("just 4 sub arguments required", s, ret);
		AGmNode *tl = arg_ls->TH, *vf = arg_ls->TTH, *v = arg_ls->TTTH, *l = arg_ls->TTTTH;
		ret = degree_poly(tl, vf, v, l);
	} else if (strcmp(s, "coef_poly") == 0) {
		if (argc != 6) return error("just 2 sub arguments required", s, ret);
		AGmNode *tl = arg_ls->TH, *vf = arg_ls->TTH, *v = arg_ls->TTTH, *l = arg_ls->TTTTH, *d = arg_ls->TTTTTH;
		ret = coef_poly(tl, vf, v, l, d);
	} else if (strcmp(s, "normalize") == 0) {
		if (argc != 2) return error("just 1 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH;
		ret = normalize(a);
	} else if (strcmp(s, "normalize2") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *a = arg_ls->TH, *b = arg_ls->TTH;
		ret = normalize2(a, b);
	} else if (strcmp(s, "normalize3_var") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *vl1 = arg_ls->TH, *vl2 = arg_ls->TTH;
		ret = normalize3_var(vl1, vl2);
	} else if (strcmp(s, "normalize3_func") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *fl1 = arg_ls->TH, *fl2 = arg_ls->TTH;
		ret = normalize3_func(fl1, fl2);
	} else if (strcmp(s, "to_gfp_poly") == 0) {
		if (argc != 3) return error("just 2 sub arguments required", s, ret);
		AGmNode *tl = arg_ls->TH, *mod = arg_ls->TTH;
		if (!IsInteger(mod)) return error("2nd arguments must be integer", s, ret);
		ret = to_gfp_poly(tl, (unsigned long)mod->a.i.v);
	} else if (strcmp(s, "from_gfp_poly") == 0) {
		if (argc != 2) return error("just 1 sub arguments required", s, ret);
		AGmNode *tl = arg_ls->TH;
		ret = from_gfp_poly(tl);
	} else {
		return error("unknown commands", "", ret);
	}
	return 0;
}

int AGmMath::is_zero(AGmNode* a) {
	return ((IsInteger(a) && a->a.i.v == 0) || (IsGFp(a) && a->a.gp.v == 0));
}
int AGmMath::is_one(AGmNode* a) {
	return ((IsInteger(a) && a->a.i.v == 1) || (IsGFp(a) && a->a.gp.v == 1));
}
int AGmMath::is_plus(AGmNode* a) {
	if (IsInteger(a)) {
		return (a->a.i.v > 0);
	} else if (IsBigInt(a)) {
		return (a->a.bi.sign > 0);
	} else {
		return 0;
	}
}
int AGmMath::is_minus(AGmNode* a) {
	if (IsInteger(a)) {
		return (a->a.i.v < 0);
	} else if (IsBigInt(a)) {
		return (a->a.bi.sign < 0);
	} else {
		return 0;
	}
}

AGmNode* AGmMath::add_poly(AGmNode* tl1, AGmNode* tl2) {
#ifdef DEBUG_ADD
DumpList("add_poly: ---------------------------------------- ", NULL);
DumpList("add_poly: tl1 = ", tl1);
DumpList("add_poly: tl2 = ", tl2);
#endif
	if (tl1 == NULL) {
#ifdef DEBUG_ADD
DumpList("add_poly: return(1) = ", tl2);
#endif
		return tl2;
	}
	if (tl2 == NULL) {
#ifdef DEBUG_ADD
DumpList("add_poly: return(2) = ", tl1);
#endif
		return tl1;
	}
	AGmNode *t1 = tl1->H, *tl1t = tl1->T;
	AGmNode *t2 = tl2->H, *tl2t = tl2->T;
	if (is_zero(t1->HH)) {
		if (is_zero(t2->HH)) {
			AGmNode *ret = add_poly(tl1t, tl2t);
#ifdef DEBUG_ADD
DumpList("add_poly: return(3) = ", ret);
#endif
			return ret;
		} else {
			AGmNode *ret = add_poly(tl1t, tl2);
#ifdef DEBUG_ADD
DumpList("add_poly: return(4) = ", ret);
#endif
			return ret;
		}
	} else {
		if (is_zero(t2->HH)) {
			AGmNode *ret = add_poly(tl1, tl2t);
#ifdef DEBUG_ADD
DumpList("add_poly: return(5) = ", ret);
#endif
			return ret;
		} else {
			int r = term_comp(t1, t2);
			if (r > 0) {
				AGmNode *ret = Cons(t1, add_poly(tl1t, tl2));
#ifdef DEBUG_ADD
DumpList("add_poly: return(6) = ", ret);
#endif
				return ret;
			} else if (r < 0) {
				AGmNode *ret = Cons(t2, add_poly(tl1, tl2t));
#ifdef DEBUG_ADD
DumpList("add_poly: return(7) = ", ret);
#endif
				return ret;
			} else {
				AGmNode *s = add_term(t1, t2);
				if (s == NULL) {
					AGmNode *ret = add_poly(tl1t, tl2t);
#ifdef DEBUG_ADD
DumpList("add_poly: return(8) = ", ret);
#endif
					return ret;
				} else {
					AGmNode *ret = Cons(s, add_poly(tl1t, tl2t));
#ifdef DEBUG_ADD
DumpList("add_poly: return(9) = ", ret);
#endif
					return ret;
				}
			}
		}
	}
}
AGmNode* AGmMath::add_term(AGmNode* t1, AGmNode* t2) {
#ifdef DEBUG_ADD
DumpList("add_term: ---------------------------------------- ", NULL);
DumpList("add_term: t1 = ", t1);
DumpList("add_term: t2 = ", t2);
#endif
	AGmNode *c11 = t1->HH, *c12 = t1->HTH, *vl = t1->TH, *fl = t1->TTH;
	AGmNode *c21 = t2->HH, *c22 = t2->HTH;
	if (is_one(c12) && is_one(c22)) {
		AGmNode *c = BigInteger::Add(c11, c21);
		if (is_zero(c)) {
			RefUnref(c);
#ifdef DEBUG_ADD
DumpList("add_term: return(1) = ", NULL);
#endif
			return NULL;
		}
		AGmNode *ret = List(List(c, one), vl, fl);
#ifdef DEBUG_ADD
DumpList("add_term: return(2) = ", ret);
#endif
		return ret;
	} else {
		AGmNode *tmp1 = BigInteger::Mul(c11, c22);
		AGmNode *tmp2 = BigInteger::Mul(c21, c12);
		AGmNode *c1 = BigInteger::Add(tmp1, tmp2);
		RefUnref(tmp1, tmp2);
		if (is_zero(c1)) {
			RefUnref(c1);
#ifdef DEBUG_ADD
DumpList("add_term: return(3) = ", NULL);
#endif
			return NULL;
		}
		AGmNode *c2 = BigInteger::Mul(c12, c22);
		AGmNode *gcd = BigInteger::Gcd(c1, c2);
		if (is_one(gcd)) {
			RefUnref(gcd);
			AGmNode *ret = List(List(c1, c2), vl, fl);
#ifdef DEBUG_ADD
DumpList("add_term: return(4) = ", ret);
#endif
			return ret;
		} else {
			tmp1 = c1; tmp2 = c2;
			c1 = BigInteger::Div(tmp1, gcd);
			c2 = BigInteger::Div(tmp2, gcd);
			RefUnref(tmp1, tmp2, gcd);
			AGmNode *ret = List(List(c1, c2), vl, fl);
#ifdef DEBUG_ADD
DumpList("add_term: return(5) = ", ret);
#endif
			return ret;
		}
	}
}
AGmNode* AGmMath::sub_poly(AGmNode* tl1, AGmNode* tl2) {
#ifdef DEBUG_SUB
DumpList("sub_poly: ---------------------------------------- ", NULL);
DumpList("sub_poly: tl1 = ", tl1);
DumpList("sub_poly: tl2 = ", tl2);
#endif
	if (tl1 == NULL) {
		AGmNode *ret = neg_poly(tl2);
#ifdef DEBUG_SUB
DumpList("sub_poly: return(1) = ", ret);
#endif
		return ret;
	}
	if (tl2 == NULL) {
#ifdef DEBUG_SUB
DumpList("sub_poly: return(2) = ", tl1);
#endif
		return tl1;
	}
	AGmNode *t1 = tl1->H, *tl1t = tl1->T;
	AGmNode *t2 = tl2->H, *tl2t = tl2->T;
	if (is_zero(t1->HH)) {
		if (is_zero(t2->HH)) {
			AGmNode *ret = sub_poly(tl1t, tl2t);
#ifdef DEBUG_SUB
DumpList("sub_poly: return(3) = ", ret);
#endif
			return ret;
		} else {
			AGmNode *ret = sub_poly(tl1t, tl2);
#ifdef DEBUG_SUB
DumpList("sub_poly: return(4) = ", ret);
#endif
			return ret;
		}
	} else {
		if (is_zero(t2->HH)) {
			AGmNode *ret = sub_poly(tl1, tl2t);
#ifdef DEBUG_SUB
DumpList("sub_poly: return(5) = ", ret);
#endif
			return ret;
		} else {
			int r = term_comp(t1, t2);
			if (r > 0) {
				AGmNode *ret = Cons(t1, sub_poly(tl1t, tl2));
#ifdef DEBUG_SUB
DumpList("sub_poly: return(6) = ", ret);
#endif
				return ret;
			} else if (r < 0) {
				AGmNode *ret = Cons(neg_term(t2), sub_poly(tl1, tl2t));
#ifdef DEBUG_SUB
DumpList("sub_poly: return(7) = ", ret);
#endif
				return ret;
			} else {
				AGmNode *s = sub_term(t1, t2);
				if (s == NULL) {
					AGmNode *ret = sub_poly(tl1t, tl2t);
#ifdef DEBUG_SUB
DumpList("sub_poly: return(8) = ", ret);
#endif
					return ret;
				} else {
					AGmNode *ret =  Cons(s, sub_poly(tl1t, tl2t));
#ifdef DEBUG_SUB
DumpList("sub_poly: return(9) = ", ret);
#endif
					return ret;
				}
			}
		}
	}
}
AGmNode* AGmMath::sub_term(AGmNode* t1, AGmNode* t2) {
#ifdef DEBUG_SUB
DumpList("sub_term: ---------------------------------------- ", NULL);
DumpList("sub_term: t1 = ", t1);
DumpList("sub_term: t2 = ", t2);
#endif
	AGmNode *c11 = t1->HH, *c12 = t1->HTH, *vl = t1->TH, *fl = t1->TTH;
	AGmNode *c21 = t2->HH, *c22 = t2->HTH;
	if (is_one(c12) && is_one(c22)) {
		AGmNode *c = BigInteger::Sub(c11, c21);
		if (is_zero(c)) {
			RefUnref(c);
#ifdef DEBUG_SUB
DumpList("sub_term: return(1) = ", NULL);
#endif
			return NULL;
		}
		AGmNode *ret = List(List(c, one), vl, fl);
#ifdef DEBUG_SUB
DumpList("sub_term: return(2) = ", ret);
#endif
		return ret;
	} else {
		AGmNode *tmp1 = BigInteger::Mul(c11, c22);
		AGmNode *tmp2 = BigInteger::Mul(c21, c12);
		AGmNode *c1 = BigInteger::Sub(tmp1, tmp2);
		RefUnref(tmp1, tmp2);
		if (is_zero(c1)) {
			RefUnref(c1);
#ifdef DEBUG_SUB
DumpList("sub_term: return(3) = ", NULL);
#endif
			return NULL;
		}
		AGmNode *c2 = BigInteger::Mul(c12, c22);
		AGmNode *gcd = BigInteger::Gcd(c1, c2);
		if (is_one(gcd)) {
			RefUnref(gcd);
			AGmNode *ret = List(List(c1, c2), vl, fl);
#ifdef DEBUG_SUB
DumpList("sub_term: return(4) = ", ret);
#endif
			return ret;
		} else {
			tmp1 = c1; tmp2 = c2;
			c1 = BigInteger::Div(tmp1, gcd);
			c2 = BigInteger::Div(tmp2, gcd);
			RefUnref(tmp1, tmp2, gcd);
			AGmNode *ret = List(List(c1, c2), vl, fl);
#ifdef DEBUG_SUB
DumpList("sub_term: return(5) = ", ret);
#endif
			return ret;
		}
	}
}
AGmNode* AGmMath::neg_poly(AGmNode* tl) {
#ifdef DEBUG_NEG
DumpList("neg_poly: ---------------------------------------- ", NULL);
DumpList("neg_poly: tl = ", tl);
#endif
	if (tl == NULL) return NULL;
	AGmNode *t = tl->H, *tlt = tl->T;
	return Cons(neg_term(t), neg_poly(tlt));
}
AGmNode* AGmMath::neg_term(AGmNode* t) {
#ifdef DEBUG_NEG
DumpList("neg_term: ---------------------------------------- ", NULL);
DumpList("neg_term: t = ", t);
#endif
	AGmNode *c1 = t->HH, *c2 = t->HTH, *vl = t->TH, *fl = t->TTH;
	AGmNode *neg_c1 = BigInteger::Neg(c1);
	return List(List(neg_c1, c2), vl, fl);
}

int AGmMath::term_comp(AGmNode* t1, AGmNode* t2) {
#ifdef DEBUG_COMP
DumpList("term_comp: ---------------------------------------- ", NULL);
DumpList("term_comp: t1 = ", t1);
DumpList("term_comp: t2 = ", t2);
#endif
	AGmNode *vl1 = t1->TH, *fl1 = t1->TTH;
	AGmNode *vl2 = t2->TH, *fl2 = t2->TTH;
	int r = term_comp_by_var(vl1, vl2);
	if (r) {
#ifdef DEBUG_COMP
TRACE1("term_comp: return(v) = %d\n", r);
#endif
		return r;
	}
	r = term_comp_by_func(fl1, fl2);
#ifdef DEBUG_COMP
TRACE1("term_comp: return(f) = %d\n", r);
#endif
	return r;
}
int AGmMath::term_comp_by_var(AGmNode* vl1, AGmNode* vl2) {
	if (vl1 == NULL && vl2 == NULL) return 0;
	if (vl1 == NULL) return -1;
	if (vl2 == NULL) return 1;
	AGmNode *n1 = vl1->HTH, *il1 = vl1->HTTH, *d1 = vl1->H->TTTH, *vl1t = vl1->T;
	AGmNode *n2 = vl2->HTH, *il2 = vl2->HTTH, *d2 = vl2->H->TTTH, *vl2t = vl2->T;
	int r = Compare(n1, n2);
	if (r < 0) return 1;
	if (r > 0) return -1;
	r = Compare(il1, il2);
	if (r > 0) return 1;
	if (r < 0) return -1;
	r = Compare(d1, d2);
	if (r > 0) return 1;
	if (r < 0) return -1;
	return term_comp_by_var(vl1t, vl2t);
}
int AGmMath::term_comp_by_func(AGmNode* fl1, AGmNode* fl2) {
	if (fl1 == NULL && fl2 == NULL) return 0;
	if (fl1 == NULL) return -1;
	if (fl2 == NULL) return 1;
	AGmNode *n1 = fl1->HTH, *al1 = fl1->HTTH, *d1 = fl1->H->TTTH, *fl1t = fl1->T;
	AGmNode *n2 = fl2->HTH, *al2 = fl2->HTTH, *d2 = fl2->H->TTTH, *fl2t = fl2->T;
	int r = Compare(n1, n2);
	if (r < 0) return 1;
	if (r > 0) return -1;
	r = Compare(al1, al2);
	if (r < 0) return 1;
	if (r > 0) return -1;
	r = Compare(d1, d2);
	if (r > 0) return 1;
	if (r < 0) return -1;
	return term_comp_by_func(fl1t, fl2t);
}

AGmNode* AGmMath::mul_poly(AGmNode* tl1, AGmNode* tl2) {
#ifdef DEBUG_MUL
DumpList("mul_poly: ---------------------------------------- ", NULL);
DumpList("mul_poly: tl1 = ", tl1);
DumpList("mul_poly: tl2 = ", tl2);
#endif
	if (tl1 == NULL || tl2 == NULL) {
#ifdef DEBUG_MUL
DumpList("mul_poly: return(1) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *t2 = tl2->H, *tl2t = tl2->T;
	AGmNode *tmp1 = mul_poly_by_term(tl1, t2);
	AGmNode *tmp2 = mul_poly(tl1, tl2t);
	AGmNode *ret = add_poly(tmp1, tmp2);
	Ref(ret);
	RefUnref(tmp1, tmp2);
	if (ret) ret->count--;
#ifdef DEBUG_MUL
DumpList("mul_poly: return(2) = ", ret);
#endif
	return ret;
}
AGmNode* AGmMath::mul_poly_by_term(AGmNode* tl1, AGmNode* t2) {
#ifdef DEBUG_MUL
DumpList("mul_poly_by_term: ---------------------------------------- ", NULL);
DumpList("mul_poly_by_term: tl1 = ", tl1);
DumpList("mul_poly_by_term: t2 = ", t2);
#endif
	if (tl1 == NULL) {
#ifdef DEBUG_MUL
DumpList("mul_poly_by_term: return(1) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *t1 = tl1->H, *tl1t = tl1->T;
	AGmNode *n_t = mul_term(t1, t2);
	AGmNode *n_tl = mul_poly_by_term(tl1t, t2);
	if (n_t == NULL) {
#ifdef DEBUG_MUL
DumpList("mul_poly_by_term: return(2) = ", n_tl);
#endif
		return n_tl;
	}
	AGmNode *tmp = Cons(n_t);
	AGmNode *ret =add_poly(tmp, n_tl);
	Ref(ret);
	RefUnref(tmp);
	if (ret) ret->count--;
#ifdef DEBUG_MUL
DumpList("mul_poly_by_term: return(2) = ", ret);
#endif
	return ret;
}
AGmNode* AGmMath::mul_term(AGmNode* t1, AGmNode* t2) {
#ifdef DEBUG_MUL
DumpList("mul_term: ---------------------------------------- ", NULL);
DumpList("mul_term: t1 = ", t1);
DumpList("mul_term: t2 = ", t2);
#endif
	AGmNode *c11 = t1->HH, *c12 = t1->HTH, *vl1 = t1->TH, *fl1 = t1->TTH;
	AGmNode *c21 = t2->HH, *c22 = t2->HTH, *vl2 = t2->TH, *fl2 = t2->TTH;
	AGmNode *vl3 = mul_term_by_var(vl1, vl2);
	AGmNode *fl3 = mul_term_by_func(fl1, fl2);
	if (is_one(c12) && is_one(c22)) {
		AGmNode *c = BigInteger::Mul(c11, c21);
		if (is_zero(c)) {
			RefUnref(c);
#ifdef DEBUG_MUL
DumpList("mul_term: return(1) = ", NULL);
#endif
			RefUnref(vl3, fl3);
			return NULL;
		}
		AGmNode *ret =  List(List(c, one), vl3, fl3);
#ifdef DEBUG_MUL
DumpList("mul_term: return(2) = ", ret);
#endif
		return ret;
	} else {
		AGmNode *c1 = BigInteger::Mul(c11, c21);
		AGmNode *c2 = BigInteger::Mul(c12, c22);
		AGmNode *gcd = BigInteger::Gcd(c1, c2);
		if (is_one(gcd)) {
			RefUnref(gcd);
			AGmNode *ret = List(List(c1, c2), vl3, fl3);
#ifdef DEBUG_MUL
DumpList("mul_term: return(4) = ", ret);
#endif
			return ret;
		} else {
			AGmNode *tmp1 = c1, *tmp2 = c2;
			c1 = BigInteger::Div(tmp1, gcd);
			c2 = BigInteger::Div(tmp2, gcd);
			RefUnref(tmp1, tmp2, gcd);
			AGmNode *ret = List(List(c1, c2), vl3, fl3);
#ifdef DEBUG_MUL
DumpList("mul_term: return(5) = ", ret);
#endif
			return ret;
		}
	}
}
AGmNode* AGmMath::mul_term_by_var(AGmNode* vl1, AGmNode* vl2) {
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: ---------------------------------------- ", NULL);
DumpList("mul_term_by_var: vl1 = ", vl1);
DumpList("mul_term_by_var: vl2 = ", vl2);
#endif
	if (vl1 == NULL) {
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: return(1) = ", vl2);
#endif
		return vl2;
	}
	if (vl2 == NULL) {
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: return(2) = ", vl1);
#endif
		return vl1;
	}
	AGmNode *v1 = vl1->H, *vl1t = vl1->T;
	AGmNode *v2 = vl2->H, *vl2t = vl2->T;
	AGmNode *n1 = v1->TH, *il1 = v1->TTH, *d1 = v1->TTTH;
	AGmNode *n2 = v2->TH, *il2 = v2->TTH, *d2 = v2->TTTH;
	int r = Compare(n1, n2);
	if (r < 0) {
		AGmNode *ret = Cons(v1, mul_term_by_var(vl1t, vl2 ));
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: return(3) = ", ret);
#endif
		return ret;
	}
	if (r > 0) {
		AGmNode *ret = Cons(v2, mul_term_by_var(vl1 , vl2t));
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: return(4) = ", ret);
#endif
		return ret;
	}
	r = Compare(il1, il2);
	if (r > 0) {
		AGmNode *ret = Cons(v1, mul_term_by_var(vl1t, vl2 ));
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: return(5) = ", ret);
#endif
		return ret;
	}
	if (r < 0) {
		AGmNode *ret = Cons(v2, mul_term_by_var(vl1 , vl2t));
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: return(6) = ", ret);
#endif
		return ret;
	}
	AGmNode *d = BigInteger::Add(d1, d2);
	if (is_zero(d)) {
		RefUnref(d);
		AGmNode *ret = mul_term_by_var(vl1t, vl2t);
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: return(7) = ", ret);
#endif
		return ret;
	}
	AGmNode *ret = Cons(List(String("var"), n1, il1, d), mul_term_by_var(vl1t, vl2t));
#ifdef DEBUG_MUL
DumpList("mul_term_by_var: return(8) = ", ret);
#endif
	return ret;
}
AGmNode* AGmMath::mul_term_by_func(AGmNode* fl1, AGmNode* fl2) {
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: ---------------------------------------- ", NULL);
DumpList("mul_term_by_func: fl1 = ", fl1);
DumpList("mul_term_by_func: fl2 = ", fl2);
#endif
	if (fl1 == NULL) {
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: return(1) = ", fl2);
#endif
		return fl2;
	}
	if (fl2 == NULL) {
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: return(2) = ", fl1);
#endif
		return fl1;
	}
	AGmNode *f1 = fl1->H, *fl1t = fl1->T;
	AGmNode *f2 = fl2->H, *fl2t = fl2->T;
	AGmNode *n1 = f1->TH, *al1 = f1->TTH, *d1 = f1->TTTH;
	AGmNode *n2 = f2->TH, *al2 = f2->TTH, *d2 = f2->TTTH;
	int r = Compare(n1, n2);
	if (r < 0) {
		AGmNode *ret = Cons(f1, mul_term_by_var(fl1t, fl2 ));
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: return(3) = ", ret);
#endif
		return ret;
	}
	if (r > 0) {
		AGmNode *ret = Cons(f2, mul_term_by_var(fl1 , fl2t));
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: return(4) = ", ret);
#endif
		return ret;
	}
	r = Compare(al1, al2);
	if (r > 0) {
		AGmNode *ret = Cons(f1, mul_term_by_var(fl1t, fl2 ));
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: return(5) = ", ret);
#endif
		return ret;
	}
	if (r < 0) {
		AGmNode *ret = Cons(f2, mul_term_by_var(fl1 , fl2t));
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: return(6) = ", ret);
#endif
		return ret;
	}
	AGmNode *d = BigInteger::Add(d1, d2);
	if (is_zero(d)) {
		RefUnref(d);
		AGmNode *ret = mul_term_by_func(fl1t, fl2t);
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: return(7) = ", ret);
#endif
		return ret;
	}
	AGmNode *ret = Cons(List(String("func"), n1, al1, d), mul_term_by_var(fl1t, fl2t));
#ifdef DEBUG_MUL
DumpList("mul_term_by_func: return(8) = ", ret);
#endif
	return ret;
}

AGmNode *AGmMath::divmod_poly(AGmNode* tl1, AGmNode* tl2) {
	if (tl1 == NULL) return List((AGmNode*)NULL, (AGmNode*)NULL);
	if (tl2 == NULL) return String("divide by zero.");
	AGmNode *var = get_first_var(tl2);
	if (var == NULL) {
		AGmNode *c = tl2->HH;
		AGmNode *c1 = c->H, *c2 = c->TH;
		AGmNode *ic;
		if (is_plus(c1) == 0) {
			c1 = BigInteger::Neg(c1);
			c2 = BigInteger::Neg(c2);
		}
		ic = List(List(List(c2, c1), NULL, NULL));
		AGmNode *ret = List(mul_poly(tl1, ic), NULL);
		RefUnref(ic);
		return ret;
	}
	AGmNode *vf = var->H, *n = var->TH, *l = var->TTH;
	AGmNode *ret = divmod_poly2(tl1, tl2, vf, n, l);
	RefUnref(var);
	return ret;
}
AGmNode *AGmMath::divmod_poly2(AGmNode* tl1, AGmNode* tl2, AGmNode* vf, AGmNode* n, AGmNode *l) {
	AGmNode *d1 = degree_poly(tl1, vf, n, l);
	AGmNode *d2 = degree_poly(tl2, vf, n, l);
	if (Compare(d2, d1) > 0) {
		return List((AGmNode*)NULL, tl1);
	}
	AGmNode *k2 = coef_poly(tl2, vf, n, l, d2);
	AGmNode *ret = divmod_poly_by_term(tl1, tl2, k2, NULL, vf, n, l, d1, d2);
	RefUnref(k2);
	return ret;
}
AGmNode *AGmMath::divmod_poly_by_term(AGmNode* tl1, AGmNode* tl2,
		AGmNode* k2, AGmNode* q, AGmNode* vf, AGmNode* n, AGmNode *l, AGmNode *d1, AGmNode *d2) {
#ifdef DEBUG_DIVMOD
DumpList("divmod_poly_by_term: ---------------------------------------- ", NULL);
DumpList("divmod_poly_by_term: tl1 = ", tl1);
DumpList("divmod_poly_by_term: tl2 = ", tl2);
DumpList("divmod_poly_by_term: k2 = ", k2);
DumpList("divmod_poly_by_term: q = ", q);
DumpList("divmod_poly_by_term: vf = ", vf);
DumpList("divmod_poly_by_term: n = ", n);
DumpList("divmod_poly_by_term: l = ", l);
DumpList("divmod_poly_by_term: d1 = ", d1);
DumpList("divmod_poly_by_term: d2 = ", d2);
#endif
	AGmNode *k1 = coef_poly(tl1, vf, n, l, d1);
	AGmNode *kk12 = normalize2(k1, k2);
	AGmNode *kk1 = kk12->H, *kk2 = kk12->TH;
#ifdef DEBUG_DIVMOD
DumpList("divmod_poly_by_term: k1 = ", k1);
DumpList("divmod_poly_by_term: kk1 = ", kk1);
DumpList("divmod_poly_by_term: kk2 = ", kk2);
#endif
	if (Length(kk2) > 1 || kk2->HTH || kk2->HTTH) {
		RefUnref(k1, kk12);
		AGmNode *ret = List(q, tl1);
#ifdef DEBUG_DIVMOD
DumpList("divmod_poly_by_term: return(1) = ", ret);
#endif
		return ret;
	}
	AGmNode *c1 = kk2->HHH, *c2 = kk2->HHTH;
	AGmNode *v = div_term(c1, c2, vf, n, l, d1, d2);
	AGmNode *vv = mul_poly_by_term(kk1, v);
	AGmNode *qq = add_poly(q, vv);
	AGmNode *diff = mul_poly(tl2, vv);
	AGmNode *rr = sub_poly(tl1, diff);
#ifdef DEBUG_DIVMOD
DumpList("divmod_poly_by_term: v = ", v);
DumpList("divmod_poly_by_term: vv = ", vv);
DumpList("divmod_poly_by_term: qq = ", qq);
DumpList("divmod_poly_by_term: diff = ", diff);
DumpList("divmod_poly_by_term: rr = ", rr);
#endif
	RefUnref(k1);
	RefUnref(kk12);
	RefUnref(v);
	RefUnref(diff);
	if (q)
		RefUnref(vv);
	if (rr == NULL) {
		AGmNode *ret = List(qq, NULL);
#ifdef DEBUG_DIVMOD
DumpList("divmod_poly_by_term: return(2) = ", ret);
#endif
		return ret;
	}
	AGmNode *d3 = degree_poly(rr, vf, n, l);
#ifdef DEBUG_DIVMOD
DumpList("divmod_poly_by_term: d3 = ", d3);
#endif
	if (Compare(d3, d2) < 0) {
		AGmNode *ret = List(qq, rr);
#ifdef DEBUG_DIVMOD
DumpList("divmod_poly_by_term: return(3) = ", ret);
#endif
		return ret;
	}
	AGmNode *ret = divmod_poly_by_term(rr, tl2, k2, qq, vf, n, l, d3, d2);
	RefUnref(qq, rr);
#ifdef DEBUG_DIVMOD
DumpList("divmod_poly_by_term: return(4) = ", ret);
#endif
	return ret;
}
AGmNode *AGmMath::div_term(AGmNode* c1, AGmNode* c2, AGmNode* vf, AGmNode *n, AGmNode* l, AGmNode *d1, AGmNode *d2) {
	AGmNode *d = BigInteger::Sub(d1, d2);
	if (IsGFp(c1)) {
		ASSERT(is_one(c2));
#if	_WINDOWS && _64BIT
		unsigned long long mod = c1->a.gp.mod;
		unsigned long long inv_c1 = BigInteger::gfp_inverse(c1->a.gp.v, mod);
#else
		unsigned long mod = c1->a.gp.mod;
		unsigned long inv_c1 = BigInteger::gfp_inverse(c1->a.gp.v, mod);
#endif

		AGmNode *c = GFp(inv_c1, mod);
		if (is_zero(d)) {
			RefUnref(d); return List(List(c, one), NULL, NULL);
		} else {
			if (vf->a.s.v->Compare("var") == 0) {
				return List(List(c, one), Cons(List(vf, n, l, d)), NULL);
			} else {
				return List(List(c, one), NULL, Cons(List(vf, n, l, d)));
			}
		}		
	}
	if (is_plus(c1)) {
		if (is_zero(d)) {
			RefUnref(d); return List(List(c2, c1), NULL, NULL);
		} else {
			if (vf->a.s.v->Compare("var") == 0) {
				return List(List(c2, c1), Cons(List(vf, n, l, d)), NULL);
			} else {
				return List(List(c2, c1), NULL, Cons(List(vf, n, l, d)));
			}
		}
	} else {
		AGmNode *nc1 = BigInteger::Neg(c1);
		AGmNode *nc2 = BigInteger::Neg(c2);
		if (is_zero(d)) {
			RefUnref(d); return List(List(nc2, nc1), NULL, NULL);
		} else {
			if (vf->a.s.v->Compare("var") == 0) {
				return List(List(nc2, nc1), Cons(List(vf, n, l, d)), NULL);
			} else {
				return List(List(nc2, nc1), NULL, Cons(List(vf, n, l, d)));
			}
		}
	}
}
AGmNode *AGmMath::get_first_var(AGmNode* tl) {
	if (tl == NULL) return NULL;
	AGmNode *t = tl->H;
	AGmNode *vl = t->TH, *fl = t->TTH;
	if (vl) return List(vl->HH, vl->HTH, vl->HTTH);
	if (fl) return List(fl->HH, fl->HTH, fl->HTTH);
	return NULL;
}

AGmNode *AGmMath::degree_poly(AGmNode* tl, AGmNode* vf, AGmNode* n, AGmNode* l) {
	if (vf->a.s.v->Compare("var") == 0) {
		return degree_poly_var(tl, n, l);
	} else {
		return degree_poly_func(tl, n, l);
	}
}
AGmNode *AGmMath::degree_poly_var(AGmNode* tl, AGmNode* n, AGmNode* il) {
	if (tl == NULL) return zero;
	AGmNode *vl = tl->HTH, *tlt = tl->T;
	AGmNode *d1 = degree_poly_var2(vl, n, il);
	AGmNode *d2 = degree_poly_var(tlt, n, il);
	if (Compare(d1, d2) >= 0) {
		return d1;
	} else {
		return d2;
	}
}
AGmNode *AGmMath::degree_poly_var2(AGmNode* vl, AGmNode* n, AGmNode* il) {
	if (vl == NULL) return zero;
	AGmNode *v = vl->H, *vlt = vl->T;
	AGmNode *n2 = v->TH, *il2 = v->TTH, *d2 = v->TTTH;
	if (Compare(n, n2) == 0 && Compare(il, il2) == 0) return d2;
	return degree_poly_var2(vlt, n, il);
}
AGmNode *AGmMath::degree_poly_func(AGmNode* tl, AGmNode* n, AGmNode* al) {
	if (tl == NULL) return zero;
	AGmNode *fl = tl->HTTH, *tlt = tl->T;
	AGmNode *d1 = degree_poly_func2(fl, n, al);
	AGmNode *d2 = degree_poly_func(tlt, n, al);
	if (Compare(d1, d2) >= 0) {
		return d1;
	} else {
		return d2;
	}
}
AGmNode *AGmMath::degree_poly_func2(AGmNode* fl, AGmNode* n, AGmNode* al) {
	if (fl == NULL) return zero;
	AGmNode *f = fl->H, *flt = fl->T;
	AGmNode *n2 = f->TH, *al2 = f->TTH, *d2 = f->TTTH;
	if (Compare(n, n2) == 0 && Compare(al, al2) == 0) return d2;
	return degree_poly_func2(flt, n, al);
}
AGmNode *AGmMath::coef_poly(AGmNode* tl, AGmNode* vf, AGmNode* n, AGmNode* l, AGmNode* d) {
	if (vf->a.s.v->Compare("var") == 0) {
		return coef_poly_var(tl, n, l, d);
	} else {
		return coef_poly_func(tl, n, l, d);
	}
}
AGmNode *AGmMath::coef_poly_var(AGmNode* tl, AGmNode* n, AGmNode* il, AGmNode* d) {
	if (tl == NULL) return NULL;
	AGmNode *t = tl->H, *tlt = tl->T;
	AGmNode *c1 = t->H, *vl1 = t->TH, *fl1 = t->TTH;
	if (IsInteger(d) && d->a.i.v == 0) {
		int ok = 1;
		AGmNode *vl2 = coef_poly_var2(vl1, ok, NULL, n, il, d);
		if (ok) {
			AGmNode *tmp1 = coef_poly_var(tlt, n, il, d);
			AGmNode *tmp2 = List(c1, vl2, fl1);
			AGmNode *ret = sort_poly_insert(tmp1, tmp2);
			Ref(ret);
			RefUnref(tmp1, tmp2);
			if (ret) ret->count--;
			return ret;
		} else {
			RefUnref(vl2); return coef_poly_var(tlt, n, il, d);
		}
	}
	int ok = 0;
	AGmNode *vl2 = coef_poly_var2(vl1, ok, NULL, n, il, d);
	if (ok) {
		AGmNode *tmp1 = coef_poly_var(tlt, n, il, d);
		AGmNode *tmp2 = List(c1, vl2, fl1);
		AGmNode *ret = sort_poly_insert(tmp1, tmp2);
		Ref(ret);
		RefUnref(tmp1, tmp2);
		if (ret) ret->count--;
		return ret;
	} else {
		RefUnref(vl2); return coef_poly_var(tlt, n, il, d);
	}
}
AGmNode *AGmMath::coef_poly_var2(AGmNode* vl, int& ok, AGmNode* ls, AGmNode* n, AGmNode* il, AGmNode* d) {
	if (vl == NULL) return ls;
	AGmNode *v = vl->H, *vlt = vl->T;
	AGmNode *n2 = v->TH, *il2 = v->TTH, *d2 = v->TTTH;
	if (IsInteger(d) && d->a.i.v == 0) {
		if (Compare(n, n2) == 0 && Compare(il, il2) == 0) {
			ok = 0; return coef_poly_var2(vlt, ok, ls, n, il, d);
		} else {
			int first = (ls == NULL);
			AGmNode *tmp = Nconc(ls, Cons(v));
			AGmNode *ret = coef_poly_var2(vlt, ok, tmp, n, il, d);
			Ref(ret);
			if (first) Unref(tmp);
			if (ret) ret->count--;
			return ret;
		}
	}
	if (Compare(n, n2) == 0 && Compare(il, il2) == 0 && Compare(d, d2) == 0) {
		ok = 1; return coef_poly_var2(vlt, ok, ls, n, il, d);
	} else {
		int first = (ls == NULL);
		AGmNode *tmp = Nconc(ls, Cons(v));
		AGmNode *ret = coef_poly_var2(vlt, ok, tmp, n, il, d);
		Ref(ret);
		if (first) Unref(tmp);
		if (ret) ret->count--;
		return ret;
	}
}
AGmNode *AGmMath::coef_poly_func(AGmNode* tl, AGmNode* n, AGmNode* al, AGmNode* d) {
	if (tl == NULL) return NULL;
	AGmNode *t = tl->H, *tlt = tl->T;
	AGmNode *c1 = t->H, *vl1 = t->TH, *fl1 = t->TTH;
	if (IsInteger(d) && d->a.i.v == 0) {
		int ok = 1;
		AGmNode *fl2 = coef_poly_func2(fl1, ok, NULL, n, al, d);
		if (ok) {
			AGmNode *tmp1 = coef_poly_func(tlt, n, al, d);
			AGmNode *tmp2 = List(c1, vl1, fl2);
			AGmNode *ret = sort_poly_insert(tmp1, tmp2);
			Ref(ret);
			RefUnref(tmp1, tmp2);
			if (ret) ret->count--;
			return ret;
		} else {
			RefUnref(fl2); return coef_poly_func(tlt, n, al, d);
		}
	}
	int ok = 0;
	AGmNode *fl2 = coef_poly_func2(fl1, ok, NULL, n, al, d);
	if (ok) {
		AGmNode *tmp1 = coef_poly_func(tlt, n, al, d);
		AGmNode *tmp2 = List(c1, vl1, fl2);
		AGmNode *ret = sort_poly_insert(tmp1, tmp2);
		Ref(ret);
		RefUnref(tmp1, tmp2);
		if (ret) ret->count--;
		return ret;
	} else {
		RefUnref(fl2); return coef_poly_func(tlt, n, al, d);
	}
}
AGmNode *AGmMath::coef_poly_func2(AGmNode* fl, int& ok, AGmNode* ls, AGmNode* n, AGmNode* al, AGmNode* d) {
	if (fl == NULL) return ls;
	AGmNode *f = fl->H, *flt = fl->T;
	AGmNode *n2 = f->TH, *al2 = f->TTH, *d2 = f->TTTH;
	if (IsInteger(d) && d->a.i.v == 0) {
		if (Compare(n, n2) == 0 && Compare(al, al2) == 0) {
			ok = 0; return coef_poly_func2(flt, ok, ls, n, al, d);
		} else {
			int first = (ls == NULL);
			AGmNode *tmp = Nconc(ls, Cons(f));
			AGmNode *ret = coef_poly_func2(flt, ok, tmp, n, al, d);
			Ref(ret);
			if (first) Unref(tmp);
			if (ret) ret->count--;
			return ret;
		}
	}
	if (Compare(n, n2) == 0 && Compare(al, al2) == 0 && Compare(d, d2) == 0) {
		ok = 1; return coef_poly_func2(flt, ok, ls, n, al, d);
	} else {
		int first = (ls == NULL);
		AGmNode *tmp = Nconc(ls, Cons(f));
		AGmNode *ret = coef_poly_func2(flt, ok, tmp, n, al, d);
		Ref(ret);
		if (first) Unref(tmp);
		if (ret) ret->count--;
		return ret;
	}
}
AGmNode *AGmMath::sort_poly_insert(AGmNode* ls, AGmNode* e) {
	if (ls == NULL) return Cons(e);
	AGmNode *h = ls->H, *t = ls->T;
	AGmNode *vl1 = h->TH, *fl1 = h->TTH;
	AGmNode *vl2 = e->TH, *fl2 = e->TTH;
	int r = sort_poly_comp(vl1, vl2, 0);
	if (r > 0) return Cons(h, sort_poly_insert(t, e));
	if (r < 0) return Cons(e, ls);
	r = sort_poly_comp(fl1, fl2, 1);
	if (r > 0) return Cons(h, sort_poly_insert(t, e));
	return Cons(e, ls);
}
int AGmMath::sort_poly_comp(AGmNode* e1, AGmNode* e2, int func) {
	if (e1 == NULL && e2 == NULL) return 0;
	if (e2 == NULL) return 1;
	if (e1 == NULL) return -1;
	AGmNode *v1 = e1->H, *vl1t = e1->T;
	AGmNode *n1 = v1->TH, *l1 = v1->TTH, *d1= v1->TTTH;
	AGmNode *v2 = e2->H, *vl2t = e2->T;
	AGmNode *n2 = v2->TH, *l2 = v2->TTH, *d2= v2->TTTH;
	int r = Compare(n1, n2);
	if (r < 0) return 1;
	if (r > 0) return -1;
	r = Compare(l1, l2);
	if (r > 0) {
		if (func == 0) return 1;
		else return -1;
	}
	if (r < 0) {
		if (func == 0) return -1;
		return 1;
	}
	r = Compare(d1, d2);
	if (r > 0) return 1;
	if (r < 0) return -1;
	return sort_poly_comp(vl1t, vl2t, func);
}
AGmNode *AGmMath::common_num(AGmNode* tl) {
#ifdef DEBUG_COMMON
DumpList("common_num: tl = ", tl);
#endif
	if (tl == NULL) {
#ifdef DEBUG_COMMON
DumpList("common_num: return(1) = ", one);
#endif
		return one;
	}
	AGmNode *c = tl->HHH, *tlt = tl->T;
	if (IsGFp(c)) return GFp(1, c->a.gp.mod);
	AGmNode *ret = common_num2(c, tlt);
	if (is_minus(c)) {
		AGmNode *tmp = ret;
		ret = BigInteger::Neg(tmp);
		RefUnref(tmp);
	}
#ifdef DEBUG_COMMON
DumpList("common_num: return(2) = ", ret);
#endif
	return ret;
}
AGmNode *AGmMath::common_num2(AGmNode* c1, AGmNode* tl) {
#ifdef DEBUG_COMMON2
DumpList("common_num2: c1 = ", c1);
DumpList("common_num2: tl = ", tl);
#endif
	if (tl == NULL) {
		AGmNode *ret = c1;
		if (is_minus(c1)) {
			ret = BigInteger::Neg(c1);
		}
#ifdef DEBUG_COMMON2
DumpList("common_num2: return(1) = ", ret);
#endif
		return ret;
	}
	if (is_one(c1)) {
#ifdef DEBUG_COMMON2
DumpList("common_num2: return(2) = ", one);
#endif
		return c1;
	}
	AGmNode *c2 = tl->HHH, *tlt = tl->T;
	if (IsGFp(c2)) return GFp(1, c2->a.gp.mod);
	AGmNode *c = BigInteger::Gcd(c1, c2);
	if (is_one(c)) {
#ifdef DEBUG_COMMON2
DumpList("common_num2: return(3) = ", c);
#endif
		return c;
	}
	AGmNode *ret = common_num2(c, tlt);
#ifdef DEBUG_COMMON2
DumpList("common_num2: return(4) = ", ret);
#endif
	Ref(ret);
	RefUnref(c);
	if (ret) ret->count--;
	return ret;
}
AGmNode *AGmMath::common_var(AGmNode* tl) {
#ifdef DEBUG_COMMON
DumpList("common_var: tl = ", tl);
#endif
	if (tl == NULL) {
#ifdef DEBUG_COMMON
DumpList("common_var: return(1) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *vl = tl->HTH, *tlt = tl->T;
	AGmNode *ret = common_var2(vl, tlt);
#ifdef DEBUG_COMMON
DumpList("common_var: return(2) = ", ret);
#endif
	return ret;
}
AGmNode *AGmMath::common_var2(AGmNode* vl1, AGmNode* tl) {
#ifdef DEBUG_COMMON2
DumpList("common_var2: vl1 = ", vl1);
DumpList("common_var2: tl = ", tl);
#endif
	if (tl == NULL) {
#ifdef DEBUG_COMMON2
DumpList("common_var2: return(1) = ", vl1);
#endif
		return vl1;
	}
	AGmNode *vl2 = tl->HTH, *tlt = tl->T;
	AGmNode *l = common_var3(vl1, vl2);
	if (l == NULL) {
#ifdef DEBUG_COMMON2
DumpList("common_var2: return(2) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *ret = common_var2(l, tlt);
#ifdef DEBUG_COMMON2
DumpList("common_var2: return(3) = ", ret);
#endif
	Ref(ret);
	RefUnref(l);
	if (ret) ret->count--;
	return ret;
}
AGmNode *AGmMath::common_var3(AGmNode* vl1, AGmNode* vl2) {
#ifdef DEBUG_COMMON2
DumpList("common_var3: vl1 = ", vl1);
DumpList("common_var3: vl2 = ", vl2);
#endif
	if (vl1 == NULL || vl2 == NULL) {
#ifdef DEBUG_COMMON2
DumpList("common_var3: return(1) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *v1 = vl1->H, *vl1t = vl1->T;
	AGmNode *n1 = v1->TH, *il1 = v1->TTH, *d1 = v1->TTTH;
	AGmNode *v2 = vl2->H, *vl2t = vl2->T;
	AGmNode *n2 = v2->TH, *il2 = v2->TTH, *d2 = v2->TTTH;
	int r = Compare(n1, n2);
	if (r < 0) {
		AGmNode *ret = common_var3(vl1t, vl2);
#ifdef DEBUG_COMMON2
DumpList("common_var3: return(2) = ", ret);
#endif
		return ret;
	}
	if (r > 0) {
		AGmNode *ret = common_var3(vl1 , vl2t);
#ifdef DEBUG_COMMON2
DumpList("common_var3: return(3) = ", ret);
#endif
		return ret;
	}
	r = Compare(il1, il2);
	if (r > 0) {
		AGmNode *ret = common_var3(vl1t, vl2);
#ifdef DEBUG_COMMON2
DumpList("common_var3: return(4) = ", ret);
#endif
		return ret;
	}
	if (r < 0) {
		AGmNode *ret = common_var3(vl1 , vl2t);
#ifdef DEBUG_COMMON2
DumpList("common_var3: return(5) = ", ret);
#endif
		return ret;
	}
	if (Compare(d1, d2) > 0) {
		AGmNode *ret = Cons(v2, common_var3(vl1t, vl2t));
#ifdef DEBUG_COMMON2
DumpList("common_var3: return(6) = ", ret);
#endif
		return ret;
	}
	AGmNode *ret = Cons(v1, common_var3(vl1t, vl2t));
#ifdef DEBUG_COMMON2
DumpList("common_var3: return(7) = ", ret);
#endif
	return ret;
}
AGmNode *AGmMath::common_func(AGmNode* tl) {
#ifdef DEBUG_COMMON
DumpList("common_func: tl = ", tl);
#endif
	if (tl == NULL) {
#ifdef DEBUG_COMMON
DumpList("common_func: return(1) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *fl = tl->HTTH, *tlt = tl->T;
	AGmNode *ret = common_func2(fl, tlt);
#ifdef DEBUG_COMMON
DumpList("common_func: return(2) = ", ret);
#endif
	return ret;
}
AGmNode *AGmMath::common_func2(AGmNode* fl1, AGmNode* tl) {
#ifdef DEBUG_COMMON2
DumpList("common_func2: fl1 = ", fl1);
DumpList("common_func2: tl = ", tl);
#endif
	if (tl == NULL) {
#ifdef DEBUG_COMMON2
DumpList("common_func2: return(1) = ", fl1);
#endif
		return fl1;
	}
	AGmNode *fl2 = tl->HTTH, *tlt = tl->T;
	AGmNode *l = common_func3(fl1, fl2);
	if (l == NULL) {
#ifdef DEBUG_COMMON2
DumpList("common_func2: return(2) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *ret = common_func2(l, tlt);
#ifdef DEBUG_COMMON2
DumpList("common_func2: return(3) = ", ret);
#endif
	Ref(ret);
	RefUnref(l);
	if (ret) ret->count--;
	return ret;
}
AGmNode *AGmMath::common_func3(AGmNode* fl1, AGmNode* fl2) {
#ifdef DEBUG_COMMON2
DumpList("common_func3: fl1 = ", fl1);
DumpList("common_func3: fl2 = ", fl2);
#endif
	if (fl1 == NULL || fl2 == NULL) {
#ifdef DEBUG_COMMON2
DumpList("common_func3: return(1) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *f1 = fl1->H, *fl1t = fl1->T;
	AGmNode *n1 = f1->TH, *al1 = f1->TTH, *d1 = f1->TTTH;
	AGmNode *f2 = fl2->H, *fl2t = fl2->T;
	AGmNode *n2 = f2->TH, *al2 = f2->TTH, *d2 = f2->TTTH;
	int r = Compare(n1, n2);
	if (r < 0) {
		AGmNode *ret = common_func3(fl1t, fl2);
#ifdef DEBUG_COMMON2
DumpList("common_func3: return(2) = ", ret);
#endif
		return ret;
	}
	if (r > 0) {
		AGmNode *ret = common_func3(fl1 , fl2t);
#ifdef DEBUG_COMMON2
DumpList("common_func3: return(3) = ", ret);
#endif
		return ret;
	}
	r = Compare(al1, al2);
	if (r > 0) {
		AGmNode *ret = common_func3(fl1t, fl2);
#ifdef DEBUG_COMMON2
DumpList("common_func3: return(4) = ", ret);
#endif
		return ret;
	}
	if (r < 0) {
		AGmNode *ret = common_func3(fl1 , fl2t);
#ifdef DEBUG_COMMON2
DumpList("common_func3: return(5) = ", ret);
#endif
		return ret;
	}
	if (Compare(d1, d2) > 0) {
		AGmNode *ret = Cons(f2, common_func3(fl1t, fl2t));
#ifdef DEBUG_COMMON2
DumpList("common_func3: return(6) = ", ret);
#endif
		return ret;
	}
	AGmNode *ret = Cons(f1, common_func3(fl1t, fl2t));
#ifdef DEBUG_COMMON2
DumpList("common_func3: return(7) = ", ret);
#endif
	return ret;
}

AGmNode *AGmMath::normalize(AGmNode* tl){
#ifdef DEBUG_NORMALIZE
DumpList("normalize: ---------------------------------------- ", NULL);
DumpList("normalize: tl = ", tl);
#endif
	AGmNode *c = common_num(tl);
#ifdef DEBUG_NORMALIZE
DumpList("normalize: c = ", c);
#endif
	AGmNode *vl = common_var(tl);
#ifdef DEBUG_NORMALIZE
DumpList("normalize: vl = ", vl);
#endif
	AGmNode *fl = common_func(tl);
#ifdef DEBUG_NORMALIZE
DumpList("normalize: fl = ", fl);
#endif
	AGmNode *ret = normalize3(tl, c, vl, fl);
#ifdef DEBUG_NORMALIZE
DumpList("normaliz: ret = ", ret);
#endif
	RefUnref(c, vl, fl);
	return ret;
}
AGmNode *AGmMath::normalize2(AGmNode* tl1, AGmNode* tl2){
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: ---------------------------------------- ", NULL);
DumpList("normalize2: tl1 = ", tl1);
DumpList("normalize2: tl2 = ", tl2);
#endif
	AGmNode *c2 = common_num(tl2);
	int sign = is_minus(c2);
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: c2 = ", c2);
#endif
	AGmNode *vl2 = common_var(tl2);
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: vl2 = ", vl2);
#endif
	AGmNode *fl2 = common_func(tl2);
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: fl2 = ", fl2);
#endif
	AGmNode *c1 = common_num2(c2, tl1);
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: c1 = ", c1);
#endif
	AGmNode *vl1 = common_var2(vl2, tl1);
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: vl1 = ", vl1);
#endif
	AGmNode *fl1 = common_func2(fl2, tl1);
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: fl1 = ", fl1);
#endif
	if (sign) {
		AGmNode *tmp = c1;
		c1 = BigInteger::Neg(tmp);
		RefUnref(tmp);
	}
	AGmNode *ret1 = normalize3(tl1, c1, vl1, fl1);
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: ret1 = ", ret1);
#endif
	AGmNode *ret2 = normalize3(tl2, c1, vl1, fl1);
#ifdef DEBUG_NORMALIZE2
DumpList("normalize2: ret2 = ", ret2);
#endif
	AGmNode *ret = List(ret1, ret2);
	RefUnref(c1, vl1, fl1);
	RefUnref(c2, vl2, fl2);
	return ret;
}
AGmNode *AGmMath::normalize3(AGmNode* tl, AGmNode* c2, AGmNode* vl2, AGmNode* fl2) {
#ifdef DEBUG_NORMALIZE3
DumpList("normalize3: ---------------------------------------- ", NULL);
DumpList("normalize3: tl = ", tl);
DumpList("normalize3: c2 = ", c2);
DumpList("normalize3: vl2 = ", vl2);
DumpList("normalize3: fl2 = ", fl2);
#endif
	if (tl == NULL) {
#ifdef DEBUG_NORMALIZE3
DumpList("normalize3: return(1) = ", NULL);
#endif
		return NULL;
	}
	AGmNode *c1 = tl->HHH, *c = tl->HHTH, *vl1 = tl->HTH, *fl1 = tl->HTTH, *tlt = tl->T;
	AGmNode *cc = BigInteger::Div(c1, c2);
#ifdef DEBUG_NORMALIZE3
DumpList("normalize3: cc = ", cc);
#endif
	AGmNode *vv = normalize3_var(vl1, vl2);
#ifdef DEBUG_NORMALIZE3
DumpList("normalize3: vv = ", vv);
#endif
	AGmNode *ff = normalize3_func(fl1, fl2);
#ifdef DEBUG_NORMALIZE3
DumpList("normalize3: ff = ", ff);
#endif
	AGmNode *tt = normalize3(tlt, c2, vl2, fl2);
#ifdef DEBUG_NORMALIZE3
DumpList("normalize3: tt = ", tt);
#endif
	AGmNode *tmp = List(List(cc, c), vv, ff);
	AGmNode *ret = sort_poly_insert(tt, tmp);
	Ref(ret);
	RefUnref(cc, tmp); RefUnref(tt);
	if (ret) ret->count--;
#ifdef DEBUG_NORMALIZE3
DumpList("normalize3: return(2) = ", ret);
#endif
	return ret;
}
AGmNode *AGmMath::normalize3_var(AGmNode* vl1, AGmNode* vl2) {
	if (vl1 == NULL) return NULL;
	if (vl2 == NULL) return vl1;
	AGmNode *v1 = vl1->H, *vl1t = vl1->T;
	AGmNode *n1 = v1->TH, *il1 = v1->TTH, *d1 = v1->TTTH;
	AGmNode *v2 = vl2->H, *vl2t = vl2->T;
	AGmNode *n2 = v2->TH, *il2 = v2->TTH, *d2 = v2->TTTH;
	int r = Compare(n1, n2);
	if (r < 0) return Cons(v1, normalize3_var(vl1t, vl2));
	if (r > 0) return normalize3_var(vl1 , vl2t);
	r = Compare(il1, il2);
	if (r > 0) return Cons(v1, normalize3_var(vl1t, vl2));
	if (r < 0) return normalize3_var(vl1 , vl2t);
	if (Compare(d1, d2) == 0) return normalize3_var(vl1t, vl2t);
	AGmNode *d = BigInteger::Sub(d1, d2);
	return Cons(List(v1->H, n1, il1, d), normalize3_var(vl1t, vl2t));
}
AGmNode *AGmMath::normalize3_func(AGmNode* fl1, AGmNode* fl2) {
	if (fl1 == NULL) return NULL;
	if (fl2 == NULL) return fl1;
	AGmNode *f1 = fl1->H, *fl1t = fl1->T;
	AGmNode *n1 = f1->TH, *il1 = f1->TTH, *d1 = f1->TTTH;
	AGmNode *f2 = fl2->H, *fl2t = fl2->T;
	AGmNode *n2 = f2->TH, *il2 = f2->TTH, *d2 = f2->TTTH;
	int r = Compare(n1, n2);
	if (r < 0) return Cons(f1, normalize3_func(fl1t, fl2));
	if (r > 0) return normalize3_func(fl1 , fl2t);
	r = Compare(il1, il2);
	if (r > 0) return Cons(f1, normalize3_func(fl1t, fl2));
	if (r < 0) return normalize3_func(fl1 , fl2t);
	if (Compare(d1, d2) == 0) return normalize3_func(fl1t, fl2t);
	AGmNode *d = BigInteger::Sub(d1, d2);
	return Cons(List(f1->H, n1, il1, d), normalize3_func(fl1t, fl2t));
}
AGmNode *AGmMath::to_gfp_poly(AGmNode* tl, unsigned long mod) {
	if (tl == NULL) return NULL;
	AGmNode *c1 = tl->HHH, *c2 = tl->HHTH, *vl = tl->HTH, *fl = tl->HTTH, *tlt = tl->T;
	AGmNode *c = c1;
	if (IsInteger(c1)) c = GFp(c1->a.i.v, mod);
	if (is_zero(c)) {
		RefUnref(c);
		return to_gfp_poly(tlt, mod);
	}
	return Cons(List(List(c, c2), vl, fl), to_gfp_poly(tlt, mod));
}
AGmNode *AGmMath::from_gfp_poly(AGmNode* tl) {
	if (tl == NULL) return NULL;
	AGmNode *c1 = tl->HHH, *c2 = tl->HHTH, *vl = tl->HTH, *fl = tl->HTTH, *tlt = tl->T;
	AGmNode *c = c1;
	if (IsGFp(c1)) c = Integer(c1->a.i.v);
	return Cons(List(List(c, c2), vl, fl), from_gfp_poly(tlt));
}
