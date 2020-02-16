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
#include <gdm/matrix.h>
#include <gdm/file.h>
#include <gdm/vmkey.h>
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <dev/advcvw.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/shell.h>
#include <dev/advcedit.h>
#include <dev/bitmap.h>
#include <dev/olectrl.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/cui.h>
#include <net/crypt.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
//#define	SYS_FUNC_NAME_TRACE
extern int interp_trace;
#endif

int AIpProcess::EvalSysFunc() {
	long func_no = (long)prog->a.i.t->I;
	int (AIpProcess::*func)() = sysFuncTbl[func_no].func;
	prog = prog->a.i.t->a.i.t;
#ifdef	SYS_FUNC_NAME_TRACE
if (interp_trace) {
TRACE1("SysFunc: %s\n", sysFuncTbl[func_no].name);
}
#endif
	return (this->*func)();
}

long AIpProcess::SearchSysFunc(const char *s) {
	for (long i = 0; sysFuncNameTbl[i]; i++) {
		char *sf = sysFuncNameTbl[i];
		if (strcmp(sf, s) == 0)
			return i;
	}
	return -1;
}

char*	AIpProcess::SysFuncName(long index) {
	return sysFuncNameTbl[index];
}
int 	AIpProcess::SysFuncNumArg(long index) {
	return sysFuncNumArgTbl[index];
}

// arithmetic function
int AIpProcess::EvalSqrt() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("Sqrt()");
	}
	Unref(val); val = Ref(Real(sqrt(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalSin() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("Sin()");
	}
	Unref(val); val = Ref(Real(sin(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalCos() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("Cos()");
	}
	Unref(val); val = Ref(Real(cos(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalTan() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("Tan()");
	}
	Unref(val); val = Ref(Real(tan(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalASin() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("ASin()");
	}
	Unref(val); val = Ref(Real(asin(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalACos() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("ACos()");
	}
	Unref(val); val = Ref(Real(acos(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalATan() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("ATan()");
	}
	Unref(val); val = Ref(Real(atan(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalExp() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("Exp()");
	}
	Unref(val); val = Ref(Real(exp(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalLog() {
	AGmNode *x; double r;
	GetArg(x);
	if (!IsNum(x, r)) {
		Unref(x);
		return interp_error("Log()");
	}
	Unref(val); val = Ref(Real(log(r)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalMax() {
	AGmNode *x, *y; double rx, ry;
	GetArg(x, y);
	if (!IsNum(x, rx) || !IsNum(y, ry)) {
		Unref(x, y);
		return interp_error("Max()");
	}
	Unref(val); val = Ref(Real(max(rx, ry)));
	Unref(x, y);
	return 0;
}
int AIpProcess::EvalMin() {
	AGmNode *x, *y; double rx, ry;
	GetArg(x, y);
	if (!IsNum(x, rx) || !IsNum(y, ry)) {
		Unref(x, y);
		return interp_error("Min()");
	}
	Unref(val); val = Ref(Real(min(rx, ry)));
	Unref(x, y);
	return 0;
}
int AIpProcess::EvalAbs() {
	AGmNode *x; double rx;
	GetArg(x);
	if (!IsNum(x, rx)) {
		Unref(x);
		return interp_error("Abs()");
	}
	Unref(val); val = Ref(Real(fabs(rx)));
	Unref(x);
	return 0;
}
int AIpProcess::EvalMathMisc() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("MathMisc()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "is_p_num") == 0) {
		if (IsInteger(arg1) && arg1->a.i.v > 0) {
			AGmPNumMgr *mgr = AGmPNumMgr::GetInstance();
			if (mgr->is_p_num((unsigned long)arg1->a.i.v)) {
				val = Ref(Integer(1));
			}
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("MathMisc() is_p_num");
		}
	} else if (strcmp(s, "get_p_num") == 0) {
		if (IsInteger(arg1) && arg1->a.i.v > 0) {
			AGmPNumMgr *mgr = AGmPNumMgr::GetInstance();
			val = Ref(Integer(mgr->get_p_num_greater_than((unsigned long)arg1->a.i.v)));
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("MathMisc() get_p_num");
		}

	} else if (strcmp(s, "gcd") == 0) {
		if ((IsInteger(arg1) || IsBigInt(arg1)) && (IsInteger(arg2) || IsBigInt(arg2))) {
			val = Ref(BigInteger::Gcd(arg1, arg2));
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("MathMisc() gcd");
		}
	} else if (strcmp(s, "lcm") == 0) {
		if ((IsInteger(arg1) || IsBigInt(arg1)) && (IsInteger(arg2) || IsBigInt(arg2))) {
			AGmNode *gcd = Ref(BigInteger::Gcd(arg1, arg2));
			AGmNode *tmp = Ref(BigInteger::Div(arg1, gcd));
			val = Ref(BigInteger::Mul(tmp, arg2));
			Unref(gcd, tmp);
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("MathMisc() lcm");
		}

	} else if (strcmp(s, "Zp") == 0) {
		if ((IsInteger(arg1) || IsBigInt(arg1)) && IsInteger(arg2)) {
			AGmNode *v = BigInteger::Mod(arg1, arg2);
			val = Ref(GFp(v->a.i.v, arg2->a.i.v));
			RefUnref(v);
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("MathMisc() Zp");
		}

	} else if (strcmp(s, "format_num") == 0) {
		char buf[256]; int err = 1;
		if (IsString(arg1)) {
			if (IsInteger(arg2)) {
				art_sprintf_1(buf, 256, arg1->a.s.v->Str(), arg2->a.i.v);
				val = Ref(String(buf)); err = 0;
			} else if (IsReal(arg2)) {
				art_sprintf_1(buf, 256, arg1->a.s.v->Str(), arg2->a.r.v);
				val = Ref(String(buf)); err = 0;
			}
		}
		if (err) {
			Unref(cmd, arg1, arg2);
			return interp_error("MathMisc() format_num");
		}
	} else if (strcmp(s, "gcd_ex") == 0) {
		val = Ref(BigInteger::GcdEx(arg1, arg2));
	} else if (strcmp(s, "to_big_int") == 0) {
		if (IsInteger(arg1)) {
			val = Ref(BigInteger::ToBigInt(arg1));
		}
	} else if (strcmp(s, "inv_mod_m") == 0) {
		val = Ref(BigInteger::GFpInverse(arg1, arg2));
	} else if (strcmp(s, "crt") == 0) {
		if (arg1 && IsList(arg1) && arg1->T && IsList(arg1->T)
				&& arg2 && IsList(arg2) && arg2->T && IsList(arg2->T)) {
			val = Ref(BigInteger::ChineseRemainderTheorem(arg1->H, arg1->TH, arg2->H, arg2->TH));
		}
	} else if (strcmp(s, "factor") == 0) {
		val = Ref(BigInteger::factor(arg1));
	}

	Unref(cmd, arg1, arg2);
	return 0;
}

// list data manipulation function
int AIpProcess::EvalCons() {
	AGmNode *h, *t;
	GetArg(h, t);
	Unref(val); val = Ref(Cons(h, t));
	Unref(h, t);
	return 0;
}
int AIpProcess::EvalAppendL() {
	AGmNode *list, *elem, *list2;
	GetArg(list, elem);
	if (list == NULL) {
		list2 = Ref(Cons(elem));
	} else {
		AGmNode *l;
		for (l = list; l->T; l = l->T) ;
		Set(l->T, Cons(elem));
		list2 = Ref(list);
	}
	Unref(val); val = list2;
	Unref(list, elem);
	return 0;
}
int AIpProcess::EvalList2() {
	AGmNode *l1, *l2;
	GetArg(l1, l2);
	Unref(val);
	val = Ref(List(l1, l2));
	Unref(l1, l2);
	return 0;
}
int AIpProcess::EvalList3() {
	AGmNode *l1, *l2, *l3;
	GetArg(l1, l2, l3);
	Unref(val);
	val = Ref(List(l1, l2, l3));
	Unref(l1, l2, l3);
	return 0;
}
int AIpProcess::EvalList4() {
	AGmNode *l1, *l2, *l3, *l4;
	GetArg(l1, l2, l3, l4);
	Unref(val);
	val = Ref(List(l1, l2, l3, l4));
	Unref(l1, l2, l3, l4);
	return 0;
}
int AIpProcess::EvalList5() {
	AGmNode *l1, *l2, *l3, *l4, *l5;
	GetArg(l1, l2, l3, l4, l5);
	Unref(val);
	val = Ref(List(l1, l2, l3, l4, l5));
	Unref(l1, l2, l3, l4, l5);
	return 0;
}
int AIpProcess::EvalList6() {
	AGmNode *l1, *l2, *l3, *l4, *l5, *l6;
	GetArg(l1, l2, l3, l4, l5, l6);
	Unref(val);
	val = Ref(List(l1, l2, l3, l4, l5, l6));
	Unref(l1, l2, l3, l4, l5, l6);
	return 0;
}
int AIpProcess::EvalList7() {
	AGmNode *l1, *l2, *l3, *l4, *l5, *l6, *l7;
	GetArg(l1, l2, l3, l4, l5, l6, l7);
	Unref(val);
	val = Ref(List(l1, l2, l3, l4, l5, l6, l7));
	Unref(l1, l2, l3, l4, l5, l6, l7);
	return 0;
}
int AIpProcess::EvalList8() {
	AGmNode *l1, *l2, *l3, *l4, *l5, *l6, *l7, *l8;
	GetArg(l1, l2, l3, l4, l5, l6, l7, l8);
	Unref(val);
	val = Ref(List(l1, l2, l3, l4, l5, l6, l7, l8));
	Unref(l1, l2, l3, l4, l5, l6, l7, l8);
	return 0;
}
int AIpProcess::EvalScanList() {
	AGmNode *arg1, *n = NULL;
	GetArg(arg1);
	if (!IsString(arg1)) {
		Unref(arg1);
		return interp_error("ScanList()");
	}
	AGmFile *f = new AGmFile();
	f->Create(arg1->a.s.v, AGM_FILE_STR_READ);
	f->ReadList(n);
	delete f;
	Unref(val); val = Ref(n);
	Unref(arg1);
	return 0;
}
int AIpProcess::EvalPrintList() {
	AGmNode *arg1; int count = 0;
	GetArg(arg1);
	if (!IsList(arg1)) {
		Unref(arg1);
		return interp_error("PrintList()");
	}
	AGmString *s = new AGmString(32);
	AGmFile *f = new AGmFile();
	f->Create(s, AGM_FILE_STR_WRITE);
	f->WriteList(arg1, count);
	delete f;
	Unref(val); val = Ref(String(s));
	Unref(arg1);
	return 0;
}
int AIpProcess::EvalListElem() {
	AGmNode *list, *index;
	GetArg(list, index);
	if (!IsInteger(index) || index->a.i.v <= 0) {
		Unref(list, index);
		return interp_error("ListElem()");
	}
	Unref(val); val = NULL;
	AGmNode *ls; int i;
	for (ls = list, i = 1; ls && IsList(ls); ls = ls->T, i++) {
		if (i == index->a.i.v) break;
	}
	if (ls) val = Ref(ls->H);
	Unref(list, index);
	return 0;
}
int AIpProcess::EvalSetListElem() {
	AGmNode *list, *index, *elem;
	GetArg(list, index, elem);
	if (!IsInteger(index) || index->a.i.v <= 0) {
		Unref(list, index);
		return interp_error("ListElem()");
	}
	AGmNode *ls; int i;
	for (ls = list, i = 1; ls && IsList(ls); ls = ls->T, i++) {
		if (i == index->a.i.v) break;
		if (ls->T == NULL) Set(ls->T, Cons(NULL));	
	}
	Set(ls->H, elem);
	Unref(val); val = Ref(elem);
	Unref(list, index, elem);
	return 0;
}

extern int dump_list_max_len;
int AIpProcess::EvalListMisc() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("ListMisc()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;

	if (strcmp(s, "copy") == 0) {
		val = Ref(ListCopy(arg1));

	} else if (strcmp(s, "shallow_copy") == 0) {
		val = Ref(ShallowCopy(arg1));

	} else if (strcmp(s, "append") == 0) {
		if (IsList(arg1) && IsList(arg2)) {
			val = Ref(Append(arg1, arg2));
		}
	} else if (strcmp(s, "reverse") == 0) {
		if (IsList(arg1)) {
			val = Ref(Reverse(arg1));
		}
	} else if (strcmp(s, "member") == 0) {
		if (IsList(arg2)) {
			if (Member(arg1, arg2)) {
				val = Ref(Integer(1));
			}
		}
	} else if (strcmp(s, "length") == 0) {
		val = Ref(Integer(Length(arg1)));

	} else if (strcmp(s, "dump") == 0) {
		if (arg1 && IsInteger(arg2)) {
			DumpList(arg1, (int)arg2->a.i.v);
		}
		if (arg1 && arg2 && IsList(arg2) && IsInteger(arg2->H) && arg2->T && IsFile(arg2->TH)) {
			DumpList(arg1, (int)arg2->H->a.i.v, arg2->TH->a.f.v);
		}
	} else if (strcmp(s, "dump_max_len") == 0) {
		if (IsInteger(arg1)) {
			dump_list_max_len = (int)arg1->a.i.v;
		}
	} else {
		Unref(cmd, arg1, arg2);
		return interp_error("ListMisc()");
	}
	Unref(cmd, arg1, arg2);
	return 0;
}

// array data manipulation function
int AIpProcess::EvalCreateArray() {
	AGmNode *size;
	GetArg(size);
	if (!IsInteger(size) || size->a.i.v <= 0 || size->a.i.v > 16000) {
		Unref(size);
		return interp_error("CreateArray()");
	}
	Unref(val); val = Ref(Array(size->a.i.v));
	Unref(size);
	return 0;
}
int AIpProcess::EvalArrayLength() {
	AGmNode *ary;
	GetArg(ary);
	if (!IsArray(ary)) {
		Unref(ary);
		return interp_error("ArrayLength()");
	}
	Unref(val); val = Ref(Integer(ary->a.a.size));
	Unref(ary);
	return 0;
}

// graph data manipulation function
int AIpProcess::EvalDstItr() {
	AGmNode *src;
	GetArg(src);
	Unref(val); val = Ref(DstItr(src));
	Unref(src);
	return 0;
}
int AIpProcess::EvalSrcItr() {
	AGmNode *dst;
	GetArg(dst);
	Unref(val); val = Ref(SrcItr(dst));
	Unref(dst);
	return 0;
}
int AIpProcess::EvalNext() {
	AGmNode *itr;
	GetArg(itr);
	Unref(val); val = Ref(itr_next(itr));
	Unref(itr);
	return 0;
}
int AIpProcess::EvalNextA() {
	AGmNode *itr, *attr;
	GetArg(itr, attr);
	Unref(val); val = Ref(itr_next(itr, attr));
	Unref(itr, attr);
	return 0;
}
int AIpProcess::EvalNextA2() {
	AGmNode *itr, *attr;
	GetArg(itr, attr);
	Unref(val); val = Ref(itr_next2(itr, attr));
	Unref(itr, attr);
	return 0;
}
int AIpProcess::EvalPrev() {
	AGmNode *itr;
	GetArg(itr);
	Unref(val); val = Ref(itr_prev(itr));
	Unref(itr);
	return 0;
}
int AIpProcess::EvalPrevA() {
	AGmNode *itr, *attr;
	GetArg(itr, attr);
	Unref(val); val = Ref(itr_prev(itr, attr));
	Unref(itr, attr);
	return 0;
}
int AIpProcess::EvalPrevA2() {
	AGmNode *itr, *attr;
	GetArg(itr, attr);
	Unref(val); val = Ref(itr_prev2(itr, attr));
	Unref(itr, attr);
	return 0;
}
int AIpProcess::EvalCompo() {
	AGmNode *itr, *n;
	GetArg(itr);
	if (itr_component(itr, n)) {
		Unref(itr);
		return interp_error("Component()");
	}
	Unref(val); val = Ref(n);
	Unref(itr);
	return 0;
}
int AIpProcess::EvalInsert() {
	AGmNode *itr, *obj, *attr;
	GetArg(itr, obj, attr);
	if (itr_insert(itr, obj, attr)) {
		Unref(itr, obj, attr);
		return interp_error("Insert()");
	}
	Unref(itr, obj, attr);
	return 0;
}
int AIpProcess::EvalAppend() {
	AGmNode *itr, *obj, *attr;
	GetArg(itr, obj, attr);
	if (itr_append(itr, obj, attr)) {
		Unref(itr, obj, attr);
		return interp_error("Insert()");
	}
	Unref(itr, obj, attr);
	return 0;
}
int AIpProcess::EvalRemove() {
	AGmNode *itr;
	GetArg(itr);
	if (itr_remove(itr)) {
		Unref(itr);
		return interp_error("Remove()");
	}
	Unref(itr);
	return 0;
}
int AIpProcess::EvalGetArcA() {
	AGmNode *itr;
	GetArg(itr);
	if (!IsDstItr(itr) && !IsSrcItr(itr)) {
		Unref(itr);
		return interp_error("GetArcA()");
	}
	AGmLink *l = itr->a.itr.link;
	if (l == NULL || l->arc == NULL) {
		Unref(itr);
		return interp_error("GetArcA()");
	}
	Unref(val);
	if (l) {
		val = Ref(l->arc->attr);
	} else {
		val = NULL;
	}
	Unref(itr);
	return 0;
}
int AIpProcess::EvalSetArcA() {
	AGmNode *itr, *attr;
	GetArg(itr, attr);
	if (!IsDstItr(itr) && !IsSrcItr(itr)) {
		Unref(itr, attr);
		return interp_error("SetArcA()");
	}
	AGmLink *l = itr->a.itr.link;
	if (l == NULL || l->arc == NULL) {
		Unref(itr, attr);
		return interp_error("SetArcA()");
	}
	Set(l->arc->attr, attr);
	Unref(itr, attr);
	return 0;
}
int AIpProcess::EvalCount() {
	AGmNode *itr;
	GetArg(itr);
	long count = 0;
	if (IsDstItr(itr)) {
		AGmNode *n = itr->a.itr.node;
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			count++;
		}
	} else if (IsSrcItr(itr)) {
		AGmNode *n = itr->a.itr.node;
		AGmLink *l;
		for (l = next_inlink(n, NULL);
				l; l = next_inlink(n, l)) {
			count++;
		}
	} else {
		Unref(itr);
		return interp_error("Count()");
	}
	Unref(val); val = Ref(Integer(count));
	Unref(itr);
	return 0;
}
int AIpProcess::EvalCount2() {
	AGmNode *itr, *attr;
	GetArg(itr, attr);
	long count = 0;
	if (IsDstItr(itr)) {
		AGmNode *n = itr->a.itr.node;
		AGmLink *l;
		for (l = next_outlink(n, NULL, attr);
				l; l = next_outlink(n, l, attr)) {
			count++;
		}
	} else if (IsSrcItr(itr)) {
		AGmNode *n = itr->a.itr.node;
		AGmLink *l;
		for (l = next_inlink(n, NULL, attr);
				l; l = next_inlink(n, l, attr)) {
			count++;
		}
	} else {
		Unref(itr, attr);
		return interp_error("Count()");
	}
	Unref(val); val = Ref(Integer(count));
	Unref(itr, attr);
	return 0;
}
int AIpProcess::EvalIndexPos() {
	AGmNode *itr, *index, *attr;
	GetArg(itr, index, attr);
	if (!IsInteger(index)) {
		Unref(itr, index, attr);
		return interp_error("IndexPos()");
	}
	if (!IsDstItr(itr) && !IsSrcItr(itr)) {
		Unref(itr, index, attr);
		return interp_error("IndexPos()");
	}
	long idx = (long)index->a.i.v;
	for (;;) {
		itr_next(itr, attr);
		if (itr->a.itr.link == NULL) break;
		if (idx == 0) break;
		idx--;
	}
	Unref(itr, index, attr);
	return 0;
}
int AIpProcess::EvalSearch() {
	AGmNode *itr, *obj, *attr;
	GetArg(itr, obj, attr);
	if (!IsDstItr(itr) && !IsSrcItr(itr)) {
		Unref(itr, obj, attr);
		return interp_error("Search()");
	}
	long idx = 0;
	for (;;) {
		itr_next(itr, attr);
		AGmNode *n;
		itr_component(itr, n);
		if (n == NULL) {
			idx = -1; break;
		}
		if (n == obj) break;
		idx++;
	}
	Unref(val); val = Ref(Integer(idx));
	Unref(itr, obj, attr);
	return 0;
}
int AIpProcess::EvalCreateArc() {
	AGmNode *src, *dst, *attr;
	GetArg(src, dst, attr);
	if (src == NULL) {
		Unref(src, dst, attr);
		return interp_error("CreateArc()");
	}
	if (dst == NULL) {
		Unref(src, dst, attr);
		return 0;
	}
	CreateArc(src, dst, attr);
	Unref(src, dst, attr);
	return 0;
}
int AIpProcess::EvalCreateAssocArc() {
	AGmNode *src, *dst, *attr;
	GetArg(src, dst, attr);
	if (src == NULL) {
		Unref(src, dst, attr);
		return interp_error("CreateAssocArc()");
	}
	if (dst == NULL) {
		Unref(src, dst, attr);
		return 0;
	}
	AGmArc *a = CreateArc(src, dst, attr);
	a->incl_assoc |= AGM_ARC_ASSOC;
	Unref(src, dst, attr);
	return 0;
}
int AIpProcess::EvalCreateOrderedArc() {
	AGmNode *src, *dst, *attr, *parent;
	GetArg(src, dst, attr, parent);
	if (src == NULL || dst == NULL ||
			parent == NULL || find_outlink(parent, src) == NULL) {
		Unref(src, dst, attr, parent);
		return interp_error("CreateOrderedArc()");
	}
	AGmLink *l2 = NULL;
	AGmLink *l;
	for (l = prev_outlink(parent, NULL);
			l; l = prev_outlink(parent, l)) {
		if (l->arc->dst == src) break;
		AGmLink *l3 = find_inlink(l->arc->dst, dst);
		if (l3) l2 = l3;
	}
	CreateArc(src, dst, attr, NULL, l2);
	Unref(src, dst, attr, parent);
	return 0;
}
int AIpProcess::EvalDstNode() {
	AGmNode *src, *attr;
	GetArg(src, attr);
	Unref(val); val = Ref(dst_node(src, attr));
	Unref(src, attr);
	return 0;
}
int AIpProcess::EvalDstNodeNoCase() {
	AGmNode *src, *attr;
	GetArg(src, attr);
	Unref(val); val = NULL;
	if (IsString(attr)) val = Ref(dst_node(src, attr->a.s.v->Str(), 1));
	Unref(src, attr);
	return 0;
}
int AIpProcess::EvalSrcNode() {
	AGmNode *dst, *attr;
	GetArg(dst, attr);
	Unref(val); val = Ref(src_node(dst, attr));
	Unref(dst, attr);
	return 0;
} 
int AIpProcess::EvalSetDstNode() {
	AGmNode *src, *attr, *value;
	GetArg(src, attr, value);
	if (src == NULL) {
		Unref(src, attr, value);
		return interp_error("SetDstNode()");
	}
	set_dst_node(src, attr, value);
	Unref(src, attr, value);
	return 0;
}

int AIpProcess::EvalNextDstNode() {
	AGmNode *n, *curr, *attr;
	GetArg(n, curr, attr);
	if (n == NULL) {
		Unref(n, curr, attr);
		return interp_error("NextDstNode()");
	}
	AGmLink *l = find_outlink(n, curr);
	if (l == NULL) {
		Unref(n, curr, attr);
		return interp_error("NextDstNode()");
	}
	l = next_outlink(n, l, attr);
	Unref(val);
	if (l) {
		val = Ref(l->arc->dst);
	} else {
		val = NULL;
	}
	Unref(n, curr, attr);
	return 0;
}
int AIpProcess::EvalPrevDstNode() {
	AGmNode *n, *curr, *attr;
	GetArg(n, curr, attr);
	if (n == NULL) {
		Unref(n, curr, attr);
		return interp_error("PrevDstNode()");
	}
	AGmLink *l = find_outlink(n, curr);
	if (l == NULL) {
		Unref(n, curr, attr);
		return interp_error("PrevDstNode()");
	}
	l = prev_outlink(n, l, attr);
	Unref(val);
	if (l) {
		val = Ref(l->arc->dst);
	} else {
		val = NULL;
	}
	Unref(n, curr, attr);
	return 0;
}
int AIpProcess::EvalNextSrcNode() {
	AGmNode *n, *curr, *attr;
	GetArg(n, curr, attr);
	AGmLink *l = find_inlink(curr, n);
	if (l == NULL) {
		Unref(n, curr, attr);
		return interp_error("NextSrcNode()");
	}
	l = next_inlink(n, l, attr);
	Unref(val);
	if (l) {
		val = Ref(l->arc->src);
	} else {
		val = NULL;
	}
	Unref(n, curr, attr);
	return 0;
}
int AIpProcess::EvalPrevSrcNode() {
	AGmNode *n, *curr, *attr;
	GetArg(n, curr, attr);
	AGmLink *l = find_inlink(curr, n);
	if (l == NULL) {
		Unref(n, curr, attr);
		return interp_error("PrevSrcNode()");
	}
	l = prev_inlink(n, l, attr);
	Unref(val);
	if (l) {
		val = Ref(l->arc->src);
	} else {
		val = NULL;
	}
	Unref(n, curr, attr);
	return 0;
}
int AIpProcess::EvalInsertBefore() {
	AGmNode *n, *pos, *obj, *attr;
	GetArg(n, pos, obj, attr);
	if (n == NULL || pos == NULL || obj == NULL) {
		Unref(n, pos, obj, attr);
		return interp_error("InsertBefore()");
	}
	AGmLink *l;
	for (l = next_outlink(n, NULL);
			l; l = next_outlink(n, l)) {
		if (l->arc->dst != pos) continue;
		CreateArc(n, obj, attr, l);
		Unref(n, pos, obj, attr);
		return 0;
	}
	Unref(n, pos, obj, attr);
	return interp_error("InsertBefore()");
}
int AIpProcess::EvalAppendAfter() {
	AGmNode *n, *pos, *obj, *attr;
	GetArg(n, pos, obj, attr);
	if (n == NULL || pos == NULL || obj == NULL) {
		Unref(n, pos, obj, attr);
		return interp_error("AppendAfter()");
	}
	AGmLink *l;
	for (l = prev_outlink(n, NULL);
			l; l = prev_outlink(n, l)) {
		if (l->arc->dst != pos) continue;
		CreateArc(n, obj, attr, l->next);
		Unref(n, pos, obj, attr);
		return 0;
	}
	Unref(n, pos, obj, attr);
	return interp_error("AppendAfter()");
}
int AIpProcess::EvalRemoveFrom() {
	AGmNode *n, *obj, *attr;
	GetArg(n, obj, attr);
	if (n == NULL || obj == NULL) {
		Unref(n, obj, attr);
		return interp_error("RemoveFrom()");
	}
	AGmLink *l;
	for (l = next_outlink(n, NULL, attr);
			l; l = next_outlink(n, l, attr)) {
		if (l->arc->dst != obj) continue;
		DeleteArc(l->arc);
		Unref(n, obj, attr);
		return 0;
	}
	Unref(n, obj, attr);
	return interp_error("RemoveFrom()");
}
int AIpProcess::EvalCopy() {
	AGmNode *obj;
	GetArg(obj);
	Unref(val); val = NULL;
	if (obj == NULL) {
		Unref(obj);
		return 0;
	}
	AGmNode *n = GraphCopy(obj);
	MakeStrUnique(n);
	val = Ref(n);
	Unref(obj);
	return 0;
}

int AIpProcess::EvalDstItr2() {
	AGmNode *src, *dst;
	GetArg(src, dst);
	Unref(val); val = NULL;
	AGmLink *l = find_outlink(src, dst);
	if (l) {
		AGmNode *itr = Ref(DstItr(src));
		l->count++;
		itr->a.itr.link = l;
		val = itr;
	}
	Unref(src, dst);
	return 0;
}
int AIpProcess::EvalArcDst() {
	AGmNode *itr, *newDst;
	GetArg(itr, newDst);
	if (!(IsDstItr(itr) || IsSrcItr(itr)) || !itr->a.itr.link || !newDst) {
		Unref(itr, newDst);
		return interp_error("ArcDst()");
	}
	AGmLink *l = itr->a.itr.link;
	if (IsSrcItr(itr))
		itr_prev(itr);
	ReplaceArcDst(l->arc, newDst);
	Unref(itr, newDst);
	return 0;
}
int AIpProcess::EvalSrcItr2() {
	AGmNode *src, *dst;
	GetArg(src, dst);
	Unref(val); val = NULL;
	AGmLink *l = find_inlink(src, dst);
	if (l) {
		AGmNode *itr = Ref(SrcItr(src));
		l->count++;
		itr->a.itr.link = l;
		val = itr;
	}
	Unref(src, dst);
	return 0;
}
int AIpProcess::EvalArcSrc() {
	AGmNode *itr, *newSrc;
	GetArg(itr, newSrc);
	if (!(IsDstItr(itr) || IsSrcItr(itr)) || !itr->a.itr.link || !newSrc) {
		Unref(itr, newSrc);
		return interp_error("ArcSrc()");
	}
	AGmLink *l = itr->a.itr.link;
	if (IsDstItr(itr))
		itr_prev(itr);
	ReplaceArcSrc(l->arc, newSrc);
	Unref(itr, newSrc);
	return 0;
}

int AIpProcess::EvalGraphMisc() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("GraphMisc()");
	}
	Unref(val); val = NULL;
	const char *s = cmd->a.s.v->Str();

	if (strcmp(s, "mark") == 0) {
		MarkNode(arg1, AGM_FLAG_USER_MARK);
	} else if (strcmp(s, "unmark") == 0) {
		UnmarkNode(arg1, AGM_FLAG_USER_MARK);
	} else if (strcmp(s, "is_marked") == 0) {
		if (arg1 && (arg1->flag & AGM_FLAG_USER_MARK)) {
			val = Ref(Integer(1));
		}

	} else if (strcmp(s, "height") == 0) {
		if (arg1) {
			val = Ref(Integer(Height(arg1, arg2)));
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("GraphMisc() height");
		}

	} else if (strcmp(s, "dump") == 0) {
		if (IsInteger(arg2)) {
			DumpGraph(arg1, (int)arg2->a.i.v);
		}
		if (IsList(arg1) && arg2 && IsList(arg2) && IsInteger(arg2->H) && arg2->T && IsFile(arg2->TH)) {
			DumpGraph(arg1, (int)arg2->H->a.i.v, arg2->TH->a.f.v);
		}
	} else if (strcmp(s, "dump_src") == 0) {
		if (IsInteger(arg2)) {
			DumpGraphSrc(arg1, (int)arg2->a.i.v);
		}
		if (IsList(arg1) && arg2 && IsList(arg2) && IsInteger(arg2->H) && arg2->T && IsFile(arg2->TH)) {
			DumpGraphSrc(arg1, (int)arg2->H->a.i.v, arg2->TH->a.f.v);
		}

	} else if (strcmp(s, "sort") == 0) {
		if (arg1 == NULL) {
			Unref(cmd, arg1, arg2);
			return interp_error("GraphMisc() sort: 1st argument is null.");
		}
		if (!IsString(arg2)) {
			Unref(cmd, arg1, arg2);
			return interp_error("GraphMisc() sort: 2nd argument is not string.");
		}
		val = Ref(Sort(arg1, arg2->a.s.v->Compare("desc") == 0 ? 1 : 0));
	} else if (strcmp(s, "max_attr") == 0) {
		if (arg1 == NULL) {
			Unref(cmd, arg1, arg2);
			return interp_error("GraphMisc() max_attr: 1st argument is null.");
		}
		val = Ref(MaxAttr(arg1));
	} else if (strcmp(s, "min_attr") == 0) {
		if (arg1 == NULL) {
			Unref(cmd, arg1, arg2);
			return interp_error("GraphMisc() min_attr: 1st argument is null.");
		}
		val = Ref(MinAttr(arg1));

	} else {
		Unref(cmd, arg1, arg2);
		return interp_error("GraphMisc()");
	}
	Unref(cmd, arg1, arg2);
	return 0;
}

//----------------------------------------------------------
// obtain arguments
int AIpProcess::GetArg(AGmNode*& arg1) {
	arg1 = Ref(val);
	AGmNode *n = Ref(val_stack->H);
	Unref(val); val = n;
	Pop(val_stack);
	return 0;
}
int AIpProcess::GetArg(AGmNode*& arg1, AGmNode*& arg2) {
	arg1 = Ref(val);
	arg2 = Ref(val_stack->H);
	Pop(val_stack);
	AGmNode *n = Ref(val_stack->H);
	Unref(val); val = n;
	Pop(val_stack);
	return 0;
}
int AIpProcess::GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3) {
	arg1 = Ref(val);
	arg2 = Ref(val_stack->H);
	Pop(val_stack);
	arg3 = Ref(val_stack->H);
	Pop(val_stack);
	AGmNode *n = Ref(val_stack->H);
	Unref(val); val = n;
	Pop(val_stack);
	return 0;
}
int AIpProcess::GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4) {
	arg1 = Ref(val);
	arg2 = Ref(val_stack->H);
	Pop(val_stack);
	arg3 = Ref(val_stack->H);
	Pop(val_stack);
	arg4 = Ref(val_stack->H);
	Pop(val_stack);
	AGmNode *n = Ref(val_stack->H);
	Unref(val); val = n;
	Pop(val_stack);
	return 0;
}
int AIpProcess::GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4,
			AGmNode*& arg5) {
	arg1 = Ref(val);
	arg2 = Ref(val_stack->H);
	Pop(val_stack);
	arg3 = Ref(val_stack->H);
	Pop(val_stack);
	arg4 = Ref(val_stack->H);
	Pop(val_stack);
	arg5 = Ref(val_stack->H);
	Pop(val_stack);
	AGmNode *n = Ref(val_stack->H);
	Unref(val); val = n;
	Pop(val_stack);
	return 0;
}
int AIpProcess::GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4,
			AGmNode*& arg5, AGmNode*& arg6) {
	arg1 = Ref(val);
	arg2 = Ref(val_stack->H);
	Pop(val_stack);
	arg3 = Ref(val_stack->H);
	Pop(val_stack);
	arg4 = Ref(val_stack->H);
	Pop(val_stack);
	arg5 = Ref(val_stack->H);
	Pop(val_stack);
	arg6 = Ref(val_stack->H);
	Pop(val_stack);
	AGmNode *n = Ref(val_stack->H);
	Unref(val); val = n;
	Pop(val_stack);
	return 0;
}
int AIpProcess::GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4,
			AGmNode*& arg5, AGmNode*& arg6, AGmNode*& arg7) {
	arg1 = Ref(val);
	arg2 = Ref(val_stack->H);
	Pop(val_stack);
	arg3 = Ref(val_stack->H);
	Pop(val_stack);
	arg4 = Ref(val_stack->H);
	Pop(val_stack);
	arg5 = Ref(val_stack->H);
	Pop(val_stack);
	arg6 = Ref(val_stack->H);
	Pop(val_stack);
	arg7 = Ref(val_stack->H);
	Pop(val_stack);
	AGmNode *n = Ref(val_stack->H);
	Unref(val); val = n;
	Pop(val_stack);
	return 0;
}
int AIpProcess::GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4,
			AGmNode*& arg5, AGmNode*& arg6, AGmNode*& arg7, AGmNode*& arg8) {
	arg1 = Ref(val);
	arg2 = Ref(val_stack->H);
	Pop(val_stack);
	arg3 = Ref(val_stack->H);
	Pop(val_stack);
	arg4 = Ref(val_stack->H);
	Pop(val_stack);
	arg5 = Ref(val_stack->H);
	Pop(val_stack);
	arg6 = Ref(val_stack->H);
	Pop(val_stack);
	arg7 = Ref(val_stack->H);
	Pop(val_stack);
	arg8 = Ref(val_stack->H);
	Pop(val_stack);
	AGmNode *n = Ref(val_stack->H);
	Unref(val); val = n;
	Pop(val_stack);
	return 0;
}
