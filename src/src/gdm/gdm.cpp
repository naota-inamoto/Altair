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
#include <interp/prcsmgr.h>
#include <dev/advcvw.h>
#include <dev/advwnd.h>
#include <dev/bitmap.h>
#include <dev/ocx.h>
#include <cui/cui.h>

#include <sql/sql.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#if defined(_DEBUG) || defined(_LINUX_DEBUG)
#define _LEAK_CHECK
//#define	_DEBUG_ALLOC_FREE
//#define	_DEBUG_REDUCE_UNIQUE_STR_TBL
//#define	_CHECK_ALLOC_FREE
//#define	DEBUG_LEAK_00
#endif

#if	_WINDOWS && _64BIT
AGmNode *AGm::Integer(long long v, AGmNode *t) {
#else
AGmNode *AGm::Integer(long v, AGmNode *t) {
#endif
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_INTEGER;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.i.v = v; n->a.i.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_INTEGER, n);
#endif
	return n;
}
AGmNode *AGm::Real(double v) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_REAL;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.r.v = v;
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_REAL, n);
#endif
	return n;
}
AGmNode *AGm::Cons(AGmNode *h, AGmNode *t) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_LIST;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.l.h = Ref(h); n->a.l.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_LIST, n);
#endif
	return n;
}
#if	_WINDOWS && _64BIT
AGmNode *AGm::Array(long long size) {
#else
AGmNode *AGm::Array(long size) {
#endif
	if (size <= 0 || 16000 < size) return NULL;
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_ARRAY;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.a.v = new AGmNode*[size];
	if (n->a.a.v == NULL) {
		Free((AGmCell*)n);
		return NULL;
	}
	for (long i = 0; i < size; i++) n->a.a.v[i] = NULL;
	n->a.a.size = size;
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_ARRAY, n);
#endif
	return n;
}
AGmNode *AGm::Object(AGmNode *name, AGmNode *id) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_OBJECT;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.o.class_name = Ref(name); n->a.o.class_id = Ref(id);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_OBJECT, n);
#endif
	return n;
}
AGmNode *AGm::DstItr(AGmNode *node) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_DST_ITR;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.itr.node = Ref(node); n->a.itr.link = NULL;
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_DST_ITR, n);
#endif
	return n;
}
AGmNode *AGm::SrcItr(AGmNode *node) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_SRC_ITR;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.itr.node = Ref(node); n->a.itr.link = NULL;
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_SRC_ITR, n);
#endif
	return n;
}
AGmNode *AGm::String(const char* str, AGmNode *t) {
	AGmString *s = new AGmString(str);
	AGmNode *n = String(s, t);
	return n;
}
AGmNode *AGm::String(AGmString *v, AGmNode *t) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_STRING;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.s.v = v; n->a.s.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_STRING, n);
#endif
	return n;
}
AGmNode *AGm::Matrix(AGmMatrix *v, AGmNode *t) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_MATRIX;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.m.v = v; n->a.m.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_MATRIX, n);
#endif
	return n;
}
AGmNode *AGm::File(AGmFile *v, AGmNode *t) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_FILE;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.f.v = v; n->a.f.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_FILE, n);
#endif
	return n;
}
AGmNode *AGm::Window(ADvWindow *v, AGmNode *t) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_WINDOW;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.w.v = v; n->a.w.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_WINDOW, n);
#endif
	return n;
}
AGmNode *AGm::Bitmap(ADvBitmap *v, AGmNode *t) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_BITMAP;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.b.v = v; n->a.b.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_BITMAP, n);
#endif
	return n;
}
AGmNode *AGm::PtrCell(void *ptr, AGmNode *t) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_PTR;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.p.ptr = ptr; n->a.p.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_PTR, n);
#endif
	return n;
}
AGmNode *AGm::Binary(AGmBinary *bin, AGmNode *t) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_BINARY;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.bin.v = bin; n->a.bin.t = Ref(t);
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_BINARY, n);
#endif
	return n;
}
AGmNode* AGm::BigInt(long sign, AGmBinary* bin) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_BIGINT;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.bi.sign = sign; n->a.bi.bin = bin;
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_BIGINT, n);
#endif
	return n;
}
#if	_WINDOWS && _64BIT
AGmNode* AGm::GFp(long long v, long long mod) {
#else
AGmNode* AGm::GFp(long v, long mod) {
#endif
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_GFP;
	n->flag = 0; n->count = 0; n->body = NULL;
	v = v % mod; if (v < 0) v = v + mod;
	n->a.gp.v = v % mod; n->a.gp.mod = mod;
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_GFP, n);
#endif
	return n;
}
AGmNode *AGm::Ocx(ADvOcx *ocx) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_OCX;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.ocx.v = ocx;
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_OCX, n);
#endif
	return n;
}
AGmNode *AGm::Variant(COleVariant *var, int byref) {
	AGmNode *n = (AGmNode*)Alloc();
	n->type = AGM_NODE_VARIANT;
	n->flag = 0; n->count = 0; n->body = NULL;
	n->a.var.v = var;
	n->a.var.byref = byref;
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_NODE_VARIANT, n);
#endif
	return n;
}

int AGm::IsInteger(const AGmNode *n) {
	return (n && n->type == AGM_NODE_INTEGER);
}
int AGm::IsReal(const AGmNode *n) {
	return (n && n->type == AGM_NODE_REAL);
}
int AGm::IsList(const AGmNode *n) {
	return (n == NULL || n->type == AGM_NODE_LIST);
}
int AGm::IsArray(const AGmNode *n) {
	return (n && n->type == AGM_NODE_ARRAY);
}
int AGm::IsObject(const AGmNode *n) {
	return (n && n->type == AGM_NODE_OBJECT);
}
int AGm::IsDstItr(const AGmNode *n) {
	return (n && n->type == AGM_NODE_DST_ITR);
}
int AGm::IsSrcItr(const AGmNode *n) {
	return (n && n->type == AGM_NODE_SRC_ITR);
}
int AGm::IsString(const AGmNode *n) {
	return (n && n->type == AGM_NODE_STRING && n->a.s.v);
}
int AGm::IsMatrix(const AGmNode *n) {
	return (n && n->type == AGM_NODE_MATRIX && n->a.m.v);
}
int AGm::IsFile(const AGmNode *n) {
	return (n && n->type == AGM_NODE_FILE && n->a.f.v);
}
int AGm::IsWindow(const AGmNode *n) {
	return (n && n->type == AGM_NODE_WINDOW && n->a.w.v);
}
int AGm::IsBitmap(const AGmNode *n) {
	return (n && n->type == AGM_NODE_BITMAP && n->a.b.v);
}
int AGm::IsPtrCell(const AGmNode *n) {
	return (n && n->type == AGM_NODE_PTR);
}
int AGm::IsBinary(const AGmNode *n) {
	return (n && n->type == AGM_NODE_BINARY);
}
int AGm::IsBigInt(const AGmNode *n) {
	return (n && n->type == AGM_NODE_BIGINT);
}
int AGm::IsGFp(const AGmNode* n) {
	return (n && n->type == AGM_NODE_GFP);
}
int AGm::IsNumber(const AGmNode *n) {
	return (IsInteger(n) || IsBigInt(n) || IsReal(n) || IsGFp(n));
}
int AGm::IsOcx(const AGmNode *n) {
	return (n && n->type == AGM_NODE_OCX);
}
int AGm::IsVariant(const AGmNode *n) {
	return (n && n->type == AGM_NODE_VARIANT);
}
int AGm::IsNum(const AGmNode *n, double& r) {
	if (n == NULL) return 0;
	if (IsInteger(n) && n->a.i.t == NULL) {
		r = (double)n->a.i.v;
		return 1;
	}
	if (IsBigInt(n)) {
		AGmNode *nn = BigInteger::ToReal((AGmNode*)n);
		r = nn->a.r.v;
		RefUnref(nn);
		return 1;
	}
	if (IsReal(n)) {
		r = n->a.r.v;
		return 1;
	}
	return 0;
}
int AGm::IsStringList(const AGmNode* n) {
	const AGmNode *l = n;
	for (;;) {
		if (!l) return 1;
		if (!IsList(l)) return 0;
		if (!IsString(l->H)) return 0;
		l = l->T;
	}
}

AGmNode *AGm::Ref(AGmNode *n) {
	if (n == NULL)
		return NULL;
	if (++n->count == 0) {
#ifdef DEBUG_LEAK_00
DumpList("ref_cnt_overflow = ", n, 10, (AGmFile*)NULL, 1);
#endif
		throw new AlException(REF_COUNT_OVERFLOW, AL_STR("reference count overflow."));
	}
	return n;
}
int AGm::Unref(AGmNode *n) {
	int err = 0;
	if (n == NULL)
		return 0;
//#if defined(_DEBUG) || defined(_LINUX_DEBUG)
if (n->count == 0) {
DumpList("try to unref, but ref-count is zero: ", n, 10, (AGmFile*)NULL, 1);
return 1;
}
//#endif
	if (--n->count == 0) {
		err = DeleteNode(n);
	}
	return err;
}

void AGm::Unref(AGmNode *n1, AGmNode *n2) {
	Unref(n1); Unref(n2);
}
void AGm::Unref(AGmNode *n1, AGmNode *n2, AGmNode *n3) {
	Unref(n1); Unref(n2); Unref(n3);
}
void AGm::Unref(AGmNode *n1, AGmNode *n2, AGmNode *n3, AGmNode *n4) {
	Unref(n1); Unref(n2); Unref(n3); Unref(n4);
}
void AGm::Unref(AGmNode *n1, AGmNode *n2, AGmNode *n3, AGmNode *n4,
			AGmNode *n5, AGmNode *n6, AGmNode *n7, AGmNode *n8) {
	Unref(n1); Unref(n2); Unref(n3); Unref(n4);
	Unref(n5); Unref(n6); Unref(n7); Unref(n8);
}

void AGm::RefUnref(AGmNode *n1) {
	Ref(n1); Unref(n1);
}
void AGm::RefUnref(AGmNode *n1, AGmNode *n2, AGmNode *n3, AGmNode *n4, AGmNode *n5, AGmNode *n6) {
	RefUnref(n1);
	RefUnref(n2);
	RefUnref(n3);
	RefUnref(n4);
	RefUnref(n5);
	RefUnref(n6);
}

// ------------------------------------------
AGmArc	*AGm::CreateArc(AGmNode *src, AGmNode *dst,
		AGmNode *attr, AGmLink *out, AGmLink *in,
		AGmLink**new_out, AGmLink**new_in) {
	if (src->body == NULL) {
		AGmNodeBody *nb = src->body = (AGmNodeBody*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_BODY_OF_NODE, nb);
#endif
		nb->type = AGM_BODY_OF_NODE;
		nb->flag = 0;
		nb->nid = 0;
		nb->in = nb->out = NULL;
	}
	if (src->body->out == NULL) {
		AGmLink *l = src->body->out = (AGmLink*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_DMY_OUT_LINK, l);
#endif
		l->type = AGM_DMY_OUT_LINK;
		l->flag = 0;
		l->next = l->prev = l;
	}
	if (dst->body == NULL) {
		AGmNodeBody *nb = dst->body = (AGmNodeBody*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_BODY_OF_NODE, nb);
#endif
		nb->type = AGM_BODY_OF_NODE;
		nb->flag = 0;
		nb->nid = 0;
		nb->in = nb->out = NULL;
	}
	if (dst->body->in == NULL) {
		AGmLink *l = dst->body->in = (AGmLink*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_DMY_IN_LINK, l);
#endif
		l->type = AGM_DMY_IN_LINK;
		l->flag = 0;
		l->next = l->prev = l;
	}
	AGmArc	*a =  (AGmArc*) Alloc();
	AGmLink *ol = (AGmLink*)Alloc();
	AGmLink *il = (AGmLink*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
DebugAlloc(AGM_ARC, a);
DebugAlloc(AGM_OUT_LINK, ol);
DebugAlloc(AGM_IN_LINK, il);
#endif
	a->type = AGM_ARC; a->flag = 0;
	ol->type = AGM_OUT_LINK; ol->flag = 0;
	il->type = AGM_IN_LINK; il->flag = 0;
	if (out == NULL) out = src->body->out;
	if (in == NULL) in = dst->body->in;
	AGM_INSERT(ol, out); ol->arc = a; ol->count = 1;
	AGM_INSERT(il, in);  il->arc = a; il->count = 1;
	a->src = src; a->dst = dst;
	a->attr = Ref(attr); a->incl_assoc = 0;
	Ref(dst);
	if (new_out) *new_out = ol;
	if (new_in) *new_in = il;
	return a;
}

// ------------------------------------------
AGmArc	*AGm::ReplaceArcSrc(AGmArc *a, AGmNode *new_src, AGmLink *src_link) {
	if (new_src->body == NULL) {
		AGmNodeBody *nb = new_src->body = (AGmNodeBody*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
	DebugAlloc(AGM_BODY_OF_NODE, nb);
#endif
		nb->type = AGM_BODY_OF_NODE;
		nb->flag = 0;
		nb->nid = 0;
		nb->in = nb->out = NULL;
	}
	if (new_src->body->out == NULL) {
		AGmLink *l = new_src->body->out = (AGmLink*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
	DebugAlloc(AGM_DMY_OUT_LINK, l);
#endif
		l->type = AGM_DMY_OUT_LINK;
		l->flag = 0;
		l->next = l->prev = l;
	}
	AGmLink *outlink = a->src->body->out;
	AGmLink *l;
	for (l = outlink->next; l != outlink; l = l->next)
		if (l->arc == a)
			break;
	ASSERT(l && l->arc == a);
	if (src_link == NULL)
		src_link = new_src->body->out;
	AGM_REMOVE(l); AGM_INSERT(l, src_link);
	a->src = new_src;
	return a;
}

AGmArc	*AGm::ReplaceArcDst(AGmArc *a, AGmNode *new_dst, AGmLink *dst_link) {
	if (new_dst->body == NULL) {
		AGmNodeBody *nb = new_dst->body = (AGmNodeBody*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
	DebugAlloc(AGM_BODY_OF_NODE, nb);
#endif
		nb->type = AGM_BODY_OF_NODE;
		nb->flag = 0;
		nb->nid = 0;
		nb->in = nb->out = NULL;
	}
	if (new_dst->body->in == NULL) {
		AGmLink *l = new_dst->body->in = (AGmLink*)Alloc();
#ifdef	_DEBUG_ALLOC_FREE
	DebugAlloc(AGM_DMY_IN_LINK, l);
#endif
		l->type = AGM_DMY_IN_LINK;
		l->flag = 0;
		l->next = l->prev = l;
	}
	AGmLink *inlink = a->dst->body->in;
	AGmLink *l;
	for (l= inlink->next; l != inlink; l = l->next)
		if (l->arc == a)
			break;
	ASSERT(l && l->arc == a);
	if (dst_link == NULL)
		dst_link = new_dst->body->in;
	AGM_REMOVE(l); AGM_INSERT(l, dst_link);
	AGmNode *dst = a->dst;
	a->dst = Ref(new_dst); Unref(dst);
	return a;
}

// ------------------------------------------
int AGm::DeleteArc(AGmArc *a, int unref_dst) {
	int err = 0;
	AGmLink *outlink = a->src->body->out;
	AGmLink *inlink  = a->dst->body->in;
	AGmLink *l;
	for (l = outlink->next; l != outlink; l = l->next) {
		if (l->arc == a) {
			AGM_REMOVE(l);
			if (--l->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l);
#endif
				Free((AGmCell*)l);
			} else {
				l->next = l->prev = NULL;
			}
			break;
		}
	}
	for (l = inlink->next; l != inlink; l = l->next) {
		if (l->arc == a) {
			AGM_REMOVE(l);
			if (--l->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l);
#endif
				Free((AGmCell*)l);
			} else {
				l->next = l->prev = NULL;
			}
			break;
		}
	}
	err = Unref(a->attr);
	if (err) return err;  // for avoiding application bug
	if (unref_dst) {
		err = Unref(a->dst);
		if (err) return err;  // for avoiding application bug
	}
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(a);
#endif
	Free((AGmCell*)a);
	return 0;
}

// ------------------------------------------
int AGm::DeleteNode(AGmNode *n) {
	int err;
	while (n) {
		if (n->flag & AGM_FLAG_DESTROY) break;
		n->flag |= AGM_FLAG_DESTROY;
		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
			t = n->a.i.t;	break;
		case AGM_NODE_GFP:
			t = NULL;		break;
		case AGM_NODE_REAL:
			t = NULL;		break;
		case AGM_NODE_LIST:
			err = Unref(n->a.l.h);
			if (err)        // for avoiding application bug
				return err; // for avoiding application bug
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++)
				Unref(n->a.a.v[i]);
			delete [] n->a.a.v;
			t = NULL;
			break;
		case AGM_NODE_OBJECT:
			Unref(n->a.o.class_name, n->a.o.class_id);
			t = NULL;
			break;
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
			Unref(n->a.itr.node);
			AGmLink *l;
			l = n->a.itr.link;
			if (l && --l->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l);
#endif
				Free((AGmCell*)l);
			}
			t = NULL;
			break;
		case AGM_NODE_STRING:
			AGmString *s;
			s = n->a.s.v; if (s) delete s; t = n->a.s.t;
			break;
		case AGM_NODE_MATRIX:
			AGmMatrix *m;
			m = n->a.m.v; if (m) delete m; t = n->a.m.t;
			break;
		case AGM_NODE_FILE:
			AGmFile *f;
			f = n->a.f.v; if (f) delete f; t = n->a.f.t;
			break;
		case AGM_NODE_WINDOW:
			ADvWindow *w;
			w = n->a.w.v; n->a.w.v = NULL; if (w) delete w; t = n->a.w.t;
			break;
		case AGM_NODE_BITMAP:
			ADvBitmap *b;
			b = n->a.b.v; if (b) delete b; t = n->a.b.t;
			break;
		case AGM_NODE_BINARY:
			AGmBinary *bin;
			bin = n->a.bin.v; if (bin) delete bin; t = n->a.bin.t;
			break;
		case AGM_NODE_BIGINT:
			bin = n->a.bi.bin; if (bin) delete bin; t = NULL;
			break;
		case AGM_NODE_PTR:
			t = n->a.p.t;
			break;
		case AGM_NODE_OCX:
			ADvOcx *o;
			o = n->a.ocx.v; n->a.ocx.v = NULL; if (o) delete o; t = NULL;
			break;
		case AGM_NODE_VARIANT:
			COleVariant *v;
			v = n->a.var.v; if (v) delete v; t = NULL;
			break;
		case AGM_NODE_SQL_CONN:
			if (n->a.conn.impl) {
				delete n->a.conn.impl;
				n->a.conn.impl = NULL;
			}
			t = NULL;
			break;
		case AGM_NODE_SQL_STAT:
			t = NULL;
			break;
		case AGM_BODY_OF_NODE: // for avoiding application bug
		case AGM_DMY_OUT_LINK: // for avoiding application bug
		case AGM_DMY_IN_LINK:  // for avoiding application bug
		case AGM_OUT_LINK:     // for avoiding application bug
		case AGM_IN_LINK:      // for avoiding application bug
		case AGM_ARC:          // for avoiding application bug
			return 0;          // for avoiding application bug
		default: 
			ASSERT(0);
			return 1;
		}
		if (n->body) {
			AGmLink *l;
			l = n->body->out;
			if (l) {
				while (l->next != l) {
					err = DeleteArc(l->next->arc);
					if (err) return err; // for avoiding application bug
				}
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l);
#endif
				Free((AGmCell*)l); // dummy inlink
			}
			l = n->body->in;
			if (l) {
				while (l != l->next) {
					err = DeleteArc(l->next->arc);
					if (err) return err; // for avoiding application bug
				}
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l);
#endif
				Free((AGmCell*)l); // dummy outlink
			}
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(n->body);
#endif
			Free((AGmCell*)n->body); // node body
		}
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(n);
#endif
		Free((AGmCell*)n); // node
		if (t && --t->count != 0) break;
		n = t;
	}
	return 0;
}

// ------------------------------------------
void AGm::DeleteNodeSelf(AGmNode *n) {
	// functions for the case of file read failure
	if (n == NULL) return;
	switch (n->type) {
	case AGM_NODE_ARRAY:
		delete [] n->a.a.v;
		break;
	case AGM_NODE_STRING:
		AGmString *s;
		s = n->a.s.v; if (s) delete s;
		break;
	case AGM_NODE_MATRIX:
		AGmMatrix *m;
		m = n->a.m.v; if (m) delete m;
		break;
	case AGM_NODE_FILE:
		AGmFile *f;
		f = n->a.f.v; if (f) delete f;
		break;
	case AGM_NODE_WINDOW:
		ADvWindow *w;
		w = n->a.w.v; n->a.w.v = NULL; if (w) delete w;
		break;
	case AGM_NODE_BITMAP:
		ADvBitmap *b;
		b = n->a.b.v; if (b) delete b;
		break;
	case AGM_NODE_BINARY:
		AGmBinary *bin;
		bin = n->a.bin.v; if (bin) delete bin;
		break;
	case AGM_NODE_BIGINT:
		bin = n->a.bi.bin; if (bin) delete bin;
		break;
	case AGM_NODE_OCX:
		ADvOcx *o;
		o = n->a.ocx.v; n->a.ocx.v = NULL; if (o) delete o;
		break;
	case AGM_NODE_VARIANT:
		COleVariant *v;
		v = n->a.var.v; if (v) delete v;
		break;
	case AGM_NODE_SQL_CONN:
		if (n->a.conn.impl) {
			delete n->a.conn.impl;
			n->a.conn.impl = NULL;
		}
		break;
	case AGM_NODE_SQL_STAT:
		break;
	default:
		break;
	}
	if (n->body) {
		AGmLink *l;
		l = n->body->out;
		if (l) {
			while (l != l->next)
				DeleteArc(l->next->arc, 0);
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l);
#endif
			Free((AGmCell*)l); // dummy inlink
		}
		l = n->body->in;
		if (l) {
			while (l != l->next)
				DeleteArc(l->next->arc, 0);
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l);
#endif
			Free((AGmCell*)l); // dummy outlink
		}
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(n->body);
#endif
		Free((AGmCell*)n->body); // node body
	}
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(n);
#endif
	Free((AGmCell*)n); // node
}

// ------------------------------------------
void AGm::ReleaseNode(AGmNode *n) {
	n->flag |= AGM_FLAG_DESTROY;
	switch (n->type) {
	case AGM_NODE_INTEGER:
	case AGM_NODE_GFP:
	case AGM_NODE_REAL:
	case AGM_NODE_LIST:
	case AGM_NODE_ARRAY:
	case AGM_NODE_OBJECT:
	case AGM_NODE_DST_ITR:
	case AGM_NODE_SRC_ITR:
	case AGM_NODE_PTR:
		break;
	case AGM_NODE_STRING:
		AGmString *s;
		s = n->a.s.v; n->a.s.v = NULL; if (s) delete s;
		break;
	case AGM_NODE_MATRIX:
		AGmMatrix *m;
		m = n->a.m.v; n->a.m.v = NULL; if (m) delete m;
		break;
	case AGM_NODE_FILE:
		AGmFile *f;
		f = n->a.f.v; n->a.f.v = NULL; if (f) delete f;
		break;
	case AGM_NODE_WINDOW:
		ADvWindow *w;
		w = n->a.w.v; n->a.w.v = NULL; if (w) delete w;
		break;
	case AGM_NODE_BITMAP:
		ADvBitmap *b;
		b = n->a.b.v; n->a.b.v = NULL; if (b) delete b;
		break;
	case AGM_NODE_BINARY:
		AGmBinary *bin;
		bin = n->a.bin.v; n->a.bin.v = NULL; if (bin) delete bin;
		break;
	case AGM_NODE_BIGINT:
		bin = n->a.bi.bin; n->a.bi.bin = NULL; if (bin) delete bin;
		break;
	case AGM_NODE_OCX:
		ADvOcx *o;
		o = n->a.ocx.v; n->a.ocx.v = NULL; if (o) delete o;
		break;
	case AGM_NODE_VARIANT:
		COleVariant *v;
		v = n->a.var.v; n->a.var.v = NULL; if (v) delete v;
		break;
	case AGM_NODE_SQL_CONN:
		if (n->a.conn.impl) {
			delete n->a.conn.impl;
			n->a.conn.impl = NULL;
		}
		break;
	case AGM_NODE_SQL_STAT:
		break;
	default:
		ASSERT(0);
		return;
	}
	if (n->body) {
		AGmLink *l;
		l = n->body->out;
		if (l) {
			while (l != l->next)
				DeleteArc(l->next->arc);
		}
		l = n->body->in;
		if (l) {
			while (l != l->next)
				DeleteArc(l->next->arc);
		}
	}
	n->flag &= ~AGM_FLAG_DESTROY;
	if (n->count == 0)
		DeleteNode(n);
}

// ------------------------------------------
// make a graph to be a tree by deleting inclusion
// whose target are shared.
// delete all assotiation.
void AGm::DeleteLoopRef(AGmNode *n) {
	_DeleteLoopRef(n);
	UnmarkNode(n, AGM_FLAG_MARK);
}
void AGm::_DeleteLoopRef(AGmNode *n) {
	while (n) {
		if (n->flag & AGM_FLAG_MARK) break;
		n->flag |= AGM_FLAG_MARK;
		AGmNode *n1, *t = NULL, **addr = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_STRING:
		case AGM_NODE_MATRIX:
		case AGM_NODE_FILE:
		case AGM_NODE_WINDOW:
		case AGM_NODE_BITMAP:
		case AGM_NODE_PTR:
		case AGM_NODE_BINARY:
			t = n->a.i.t;	addr = &n->a.i.t;	break;
		case AGM_NODE_REAL:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_BIGINT:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case AGM_NODE_LIST:
			n1 = n->a.l.h;
			if (n1 && (n1->flag & AGM_FLAG_MARK)) {
				n->a.l.h = NULL; Unref(n1);
			} else {
				_DeleteLoopRef(n1);
			}
			t = n->a.l.t; addr = &n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++) {
				n1 = n->a.a.v[i];
				if (n1 && (n1->flag & AGM_FLAG_MARK)) {
					n->a.a.v[i] = NULL; Unref(n1);
				} else {
					_DeleteLoopRef(n1);
				}
			}
			t = NULL;
			break;
		default:
			ASSERT(0);
			return;
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			n1 = l->arc->attr;
			if (n1 && (n1->flag & AGM_FLAG_MARK)) {
				l->arc->attr = NULL; Unref(n1);
			} else {
				_DeleteLoopRef(n1);
			}
			n1 = l->arc->dst;
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
				if (n1 && (n1->flag & AGM_FLAG_MARK)) {
					l = l->prev;
					DeleteArc(l->next->arc);
				} else {
					_DeleteLoopRef(n1);
				}
			} else {
				l = l->prev;
				DeleteArc(l->next->arc);
			}
		}
		if (t && (t->flag & AGM_FLAG_MARK)) {
			*addr = NULL; Unref(t);
			break;
		}
		n = t;
	}
}
int AGm::Refered(AGmNode *n, int(*func_p)(AGmLink*)) {
	if (func_p == NULL) func_p = AGm::ReferedFromOutside;
	MarkNode(n, AGM_FLAG_SELF);
	int result = _Refered(n, func_p);
	UnmarkNode(n, AGM_FLAG_MARK);
	UnmarkNode(n, AGM_FLAG_SELF);
	return result;
}
int AGm::_Refered(AGmNode *n, int(*func_p)(AGmLink*)) {
	while (n) {
		if (n->flag & AGM_FLAG_MARK) break;
		n->flag |= AGM_FLAG_MARK;
		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_STRING:
		case AGM_NODE_MATRIX:
		case AGM_NODE_FILE:
		case AGM_NODE_WINDOW:
		case AGM_NODE_BITMAP:
		case AGM_NODE_PTR:
		case AGM_NODE_BINARY:
			t = n->a.i.t;	break;
		case AGM_NODE_REAL:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_BIGINT:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case AGM_NODE_LIST:
			if (_Refered(n->a.l.h, func_p)) return 1;
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++)
				if (_Refered(n->a.a.v[i], func_p)) return 1;
			t = NULL;
			break;
		default: ASSERT(0);
		}
		AGmLink *l;
		for (l = next_inlink(n, NULL);
				l; l = next_inlink(n, l)) {
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) { // input inclusion
			} else { // input association
				if ((*func_p)(l)) return 1;
			}
		}
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) { // output inclusion
				if (_Refered(l->arc->attr, func_p)) return 1;
				if (_Refered(l->arc->dst, func_p)) return 1;
			} else { // output association
			}
		}
		n = t;
	}
	return 0;
}
int AGm::ReferedFromOutside(AGmLink *l) {
	if ((l->arc->src->flag & AGM_FLAG_SELF) == 0) {
		return 1;
	}
	return 0;
}
int AGm::Refer(AGmNode *n, int(*func_p)(AGmLink*)) {
	if (func_p == NULL) func_p = AGm::ReferToOutside;
	MarkNode(n, AGM_FLAG_SELF);
	int result = _Refer(n, func_p);
	UnmarkNode(n, AGM_FLAG_MARK);
	UnmarkNode(n, AGM_FLAG_SELF);
	return result;
}
int AGm::_Refer(AGmNode *n,int(*func_p)(AGmLink*)) {
	while (n) {
		if (n->flag & AGM_FLAG_MARK) break;
		n->flag |= AGM_FLAG_MARK;
		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_STRING:
		case AGM_NODE_MATRIX:
		case AGM_NODE_FILE:
		case AGM_NODE_WINDOW:
		case AGM_NODE_BITMAP:
		case AGM_NODE_PTR:
		case AGM_NODE_BINARY:
			t = n->a.i.t;	break;
		case AGM_NODE_REAL:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_BIGINT:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case AGM_NODE_LIST:
			if (_Refer(n->a.l.h, func_p)) return 1;
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++)
				if (_Refer(n->a.a.v[i], func_p)) return 1;
			t = NULL;
			break;
		default: ASSERT(0);
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) { // output inclusion
				if (_Refer(l->arc->attr, func_p)) return 1;
				if (_Refer(l->arc->dst, func_p)) return 1;
			} else { // output association
				if ((*func_p)(l)) return 1;
			}
		}
		n = t;
	}
	return 0;
}
int AGm::ReferToOutside(AGmLink *l) {
	if ((l->arc->dst->flag & AGM_FLAG_SELF) == 0) {
		return 1;
	}
	return 0;
}
void AGm::MarkNode(AGmNode *n, int flag) {
	while (n) {
		if (n->flag & flag) break;
		n->flag |= flag;
		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_STRING:
		case AGM_NODE_MATRIX:
		case AGM_NODE_FILE:
		case AGM_NODE_WINDOW:
		case AGM_NODE_BITMAP:
		case AGM_NODE_PTR:
		case AGM_NODE_BINARY:
			t = n->a.i.t;	break;
		case AGM_NODE_REAL:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_BIGINT:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case AGM_NODE_LIST:
			MarkNode(n->a.l.h, flag);
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++)
				MarkNode(n->a.a.v[i], flag);
			t = NULL;
			break;
		default: ASSERT(0);
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			MarkNode(l->arc->attr, flag);
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
				MarkNode(l->arc->dst, flag);
			} else {
			}
		}
		n = t;
	}
}
void AGm::UnmarkNode(AGmNode *n, int flag) {
	while (n) {
		if ((n->flag & flag) == 0) break;
		n->flag &= ~flag;
		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_STRING:
		case AGM_NODE_MATRIX:
		case AGM_NODE_FILE:
		case AGM_NODE_WINDOW:
		case AGM_NODE_BITMAP:
		case AGM_NODE_PTR:
		case AGM_NODE_BINARY:
			t = n->a.i.t;	break;
		case AGM_NODE_REAL:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_BIGINT:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case AGM_NODE_LIST:
			UnmarkNode(n->a.l.h, flag);
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++)
				UnmarkNode(n->a.a.v[i], flag);
			t = NULL;
			break;
		default: ASSERT(0);
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			UnmarkNode(l->arc->attr, flag);
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
				UnmarkNode(l->arc->dst, flag);
			} else {
			}
		}
		n = t;
	}
}

int AGm::Height(AGmNode* n, AGmNode* a) {
	int h = HeightMark(n, n, a);
	HeightUnmark(n, n, a);
	return h;
}
int AGm::HeightMark(AGmNode* n0, AGmNode* n, AGmNode* a) {
	if (n->flag & AGM_FLAG_MARK) return 0;
	n->flag |= AGM_FLAG_MARK;

	AGmLink *l;
	int h = 0;
	for (l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		if (Compare(l->arc->attr, a) != 0) continue;
		if (l->arc->dst == n0) return -1;
		int h2 = HeightMark(n0, l->arc->dst, a);
		if (h2 < 0) return -1;
		if (h2 > h) h = h2;
	}
	return h + 1;
}
void AGm::HeightUnmark(AGmNode* n0, AGmNode* n, AGmNode* a) {
	if ((n->flag & AGM_FLAG_MARK) == 0) return;
	n->flag &= ~AGM_FLAG_MARK;

	AGmLink *l;
	for (l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		if (Compare(l->arc->attr, a) != 0) continue;
		if (l->arc->dst == n0) return;
		HeightUnmark(n0, l->arc->dst, a);
	}
}

// ------------------------------------------
int AGm::Push(AGmNode *&stack, AGmNode *v) {
	AGmNode *n = Cons(v, stack);
	Unref(stack);
	stack = Ref(n);
	return 0;
}
void AGm::Pop(AGmNode *&stack) {
	ASSERT(stack);
	AGmNode *n = stack;
	stack = Ref(n->a.l.t);
	Unref(n);
}
void AGm::Set(AGmNode *&addr, AGmNode *v) {
	Ref(v);
	Unref(addr);
	addr = v;
}
void AGm::Ins(AGmNode **addr, AGmNode *ls) {
	if (addr == NULL || ls == NULL || !IsList(ls) || ls->T) return;
	Set(ls->T, *addr);
	Set(*addr, ls);
}
AGmNode* AGm::Nconc(AGmNode *x, AGmNode *y) {
	if (x == NULL) return Ref(y);
	AGmNode *l;
	for (l = x; l->T; l = l->T) ;
	Set(l->T, y);
	return x;
}
AGmNode* AGm::Append(AGmNode *x, AGmNode* y) {
	if (x == NULL) return y;
	if (IsList(x)) return Cons(x->H, Append(x->T, y));
	return Cons(x, y); // error case.
}
AGmNode* AGm::Reverse(AGmNode *x, AGmNode *y) {
	if (x == NULL) return y;
	if (IsList(x)) return Reverse(x->T, Cons(x->H, y));
	return Cons(x, y); // error case.
}
int AGm::Length(AGmNode* x) {
	int i = 0;
	for (AGmNode *l = x; l && IsList(l); l = l->T) i++;
	return i;
}
int AGm::Member(AGmNode* x, AGmNode* y) {
	for (AGmNode *l = y; l && IsList(l); l = l->T) {
		if (Compare(l->H, x) == 0) return 1;
	}
	return 0;
}

// ------------------------------------------
AGmNode*	AGm::itr_next(AGmNode *itr) {
	if (itr == NULL) return NULL;
	AGmNode *node1 = itr->a.itr.node, *node2;
	if (node1 == NULL) return NULL;
	AGmLink *l1 = itr->a.itr.link, *l2;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		l2 = next_outlink(node1, l1);
		node2 = ( l2 ? l2->arc->dst : NULL );
		break;	
	case AGM_NODE_SRC_ITR:
		l2 = next_inlink(node1, l1);
		node2 = (l2 ? l2->arc->src : NULL );
		break;
	default:
		return NULL;
	}
	if (l1) {
		if (-- l1->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l1);
#endif
			Free((AGmCell*)l1);
		}
	}
	if (l2) l2->count++;
	itr->a.itr.link = l2;
	return node2;
}
AGmNode*	AGm::itr_next(AGmNode *itr, AGmNode *attr) {
	if (itr == NULL) return NULL;
	AGmNode *node1 = itr->a.itr.node, *node2;
	if (node1 == NULL) return NULL;
	AGmLink *l1 = itr->a.itr.link, *l2;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		l2 = next_outlink(node1, l1, attr);
		node2 = ( l2 ? l2->arc->dst : NULL );
		break;	
	case AGM_NODE_SRC_ITR:
		l2 = next_inlink(node1, l1, attr);
		node2 = ( l2 ? l2->arc->src : NULL);
		break;
	default:
		return NULL;
	}
	if (l1) {
		if (-- l1->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l1);
#endif
			Free((AGmCell*)l1);
		}
	}
	if (l2) l2->count++;
	itr->a.itr.link = l2;
	return node2;
}
AGmNode*	AGm::itr_next2(AGmNode *itr, AGmNode *addr) {
	if (itr == NULL) return NULL;
	AGmNode *node1 = itr->a.itr.node, *node2;
	if (node1 == NULL) return NULL;
	AGmLink *l1 = itr->a.itr.link, *l2;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		l2 = next_outlink2(node1, l1, addr);
		node2 = ( l2 ? l2->arc->dst : NULL );
		break;	
	case AGM_NODE_SRC_ITR:
		l2 = next_inlink2(node1, l1, addr);
		node2 = ( l2 ? l2->arc->src : NULL );
		break;
	default:
		return NULL;
	}
	if (l1) {
		if (-- l1->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l1);
#endif
			Free((AGmCell*)l1);
		}
	}
	if (l2) l2->count++;
	itr->a.itr.link = l2;
	return node2;
}
AGmNode*	AGm::itr_prev(AGmNode *itr) {
	if (itr == NULL) return NULL;
	AGmNode *node1 = itr->a.itr.node, *node2;
	if (node1 == NULL) return NULL;
	AGmLink *l1 = itr->a.itr.link, *l2;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		l2 = prev_outlink(node1, l1);
		node2 = ( l2 ? l2->arc->dst : NULL );
		break;	
	case AGM_NODE_SRC_ITR:
		l2 = prev_inlink(node1, l1);
		node2 = ( l2 ? l2->arc->src : NULL );
		break;
	default:
		return NULL;
	}
	if (l1) {
		if (-- l1->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l1);
#endif
			Free((AGmCell*)l1);
		}
	}
	if (l2) l2->count++;
	itr->a.itr.link = l2;
	return node2;
}
AGmNode*	AGm::itr_prev(AGmNode *itr, AGmNode *attr) {
	if (itr == NULL) return NULL;
	AGmNode *node1 = itr->a.itr.node, *node2;
	if (node1 == NULL) return NULL;
	AGmLink *l1 = itr->a.itr.link, *l2;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		l2 = prev_outlink(node1, l1, attr);
		node2 = ( l2 ? l2->arc->dst : NULL );
		break;	
	case AGM_NODE_SRC_ITR:
		l2 = prev_inlink(node1, l1, attr);
		node2 = ( l2 ? l2->arc->src : NULL );
		break;
	default:
		return NULL;
	}
	if (l1) {
		if (-- l1->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l1);
#endif
			Free((AGmCell*)l1);
		}
	}
	if (l2) l2->count++;
	itr->a.itr.link = l2;
	return node2;
}
AGmNode*	AGm::itr_prev2(AGmNode *itr, AGmNode *addr) {
	if (itr == NULL) return NULL;
	AGmNode *node1 = itr->a.itr.node, *node2;
	if (node1 == NULL) return NULL;
	AGmLink *l1 = itr->a.itr.link, *l2;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		l2 = prev_outlink2(node1, l1, addr);
		node2 = ( l2 ? l2->arc->dst : NULL );
		break;	
	case AGM_NODE_SRC_ITR:
		l2 = prev_inlink2(node1, l1, addr);
		node2 = ( l2 ? l2->arc->src : NULL );
		break;
	default:
		return NULL;
	}
	if (l1) {
		if (-- l1->count == 0) {
#ifdef	_DEBUG_ALLOC_FREE
DebugFree(l1);
#endif
			Free((AGmCell*)l1);
		}
	}
	if (l2) l2->count++;
	itr->a.itr.link = l2;
	return node2;
}

int AGm::itr_component(AGmNode *itr, AGmNode*& n) {
	if (itr == NULL) return 1;
	AGmLink *l = itr->a.itr.link;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		n = ( l ? l->arc->dst : NULL );
		break;
	case AGM_NODE_SRC_ITR:
		n = ( l ? l->arc->src : NULL );
		break;
	default:
		return 1;
	}
	return 0;
}
int AGm::itr_insert(AGmNode *itr, AGmNode *obj, AGmNode *attr) {
	if (itr == NULL || obj == NULL) return 1;
	AGmNode *node = itr->a.itr.node;
	AGmLink *l = itr->a.itr.link;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		CreateArc(node, obj, attr, l, NULL);
		break;
	case AGM_NODE_SRC_ITR:
		CreateArc(obj, node, attr, NULL, l);
		break;
	default:
		return 1;
	}
	return 0;
}
int AGm::itr_append(AGmNode *itr, AGmNode *obj, AGmNode *attr) {
	if (itr == NULL || obj == NULL) return 1;
	AGmNode *node = itr->a.itr.node;
	AGmLink *l = itr->a.itr.link;
	switch (itr->type) {
	case AGM_NODE_DST_ITR:
		l = next_outlink(node, l);
		CreateArc(node, obj, attr, l, NULL);
		break;
	case AGM_NODE_SRC_ITR:
		l = next_inlink(node, l);
		CreateArc(obj, node, attr, NULL, l);
		break;
	default:
		return 1;
	}
	return 0;
}
int AGm::itr_remove(AGmNode *itr) {
	if (itr == NULL) return 1;
	AGmLink *l = itr->a.itr.link;
	if (l == NULL) return 1;
	itr_prev(itr); DeleteArc(l->arc);
	return 0;
}

AGmLink *AGm::find_outlink(AGmNode *src, AGmNode *dst) {
	AGmLink *l, *l1;
	if (src->body == NULL || (l1 = src->body->out) == NULL) return NULL;
	for (l = l1->next; l != l1; l = l->next)
		if (l->arc->dst == dst)
		return l;
	return NULL;
}
AGmLink *AGm::find_outlink(AGmNode *src, AGmNode *dst, AGmNode *attr) {
	AGmLink *l, *l1;
	if (src->body == NULL || (l1 = src->body->out) == NULL) return NULL;
	for (l = l1->next; l != l1; l = l->next)
		if (l->arc->dst == dst && Compare(l->arc->attr, attr) == 0)
		return l;
	return NULL;
}
AGmLink *AGm::find_inlink(AGmNode *src, AGmNode *dst) {
	AGmLink *l, *l1;
	if (dst->body == NULL || (l1 = dst->body->in) == NULL) return NULL;
	for (l = l1->next; l != l1; l = l->next)
		if (l->arc->src == src)
		return l;
	return NULL;
}
AGmLink *AGm::find_inlink(AGmNode *src, AGmNode *dst, AGmNode *attr) {
	AGmLink *l, *l1;
	if (dst->body == NULL || (l1 = dst->body->in) == NULL) return NULL;
	for (l = l1->next; l != l1; l = l->next)
		if (l->arc->src == src && Compare(l->arc->attr, attr) == 0)
		return l;
	return NULL;
}

AGmLink *AGm::next_outlink(AGmNode *src, AGmLink *link) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l = link->next;
	return ( (l && l != outlink) ? l : NULL );	
}
AGmLink *AGm::next_outlink(AGmNode *src, AGmLink *link, AGmNode *attr) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->next; l && l != outlink; l = l->next) {	
		if (Compare(l->arc->attr, attr) == 0) return l;
	}
	return NULL;
}
AGmLink *AGm::next_outlink2(AGmNode *src, AGmLink *link, AGmNode *addr) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->next; l && l != outlink; l = l->next) {	
		if (l->arc->attr == addr) return l;
	}
	return NULL;
}
AGmLink *AGm::next_outlink3(AGmNode *src, AGmLink *link, AGmNode *addr) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->next; l && l != outlink; l = l->next) {	
		if (l->arc->attr != addr) return l;
	}
	return NULL;
}
AGmLink *AGm::next_inlink(AGmNode *dst, AGmLink *link) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l = link->next;
	return ( (l && l != inlink) ? l : NULL );
}
AGmLink *AGm::next_inlink(AGmNode *dst, AGmLink *link, AGmNode *attr) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->next; l && l != inlink; l = l->next) {	
		if (Compare(l->arc->attr, attr) == 0) return l;
	}
	return NULL;
}
AGmLink *AGm::next_inlink2(AGmNode *dst, AGmLink *link, AGmNode *addr) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->next; l && l != inlink; l = l->next) {	
		if (l->arc->attr == addr) return l;
	}
	return NULL;
}
AGmLink *AGm::next_inlink3(AGmNode *dst, AGmLink *link, AGmNode *addr) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->next; l && l != inlink; l = l->next) {	
		if (l->arc->attr != addr) return l;
	}
	return NULL;
}
AGmLink *AGm::prev_outlink(AGmNode *src, AGmLink *link) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l = link->prev;
	return ( (l && l != outlink) ? l : NULL );
}
AGmLink *AGm::prev_outlink(AGmNode *src, AGmLink *link, AGmNode *attr) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->prev; l && l != outlink; l = l->prev) {	
		if (Compare(l->arc->attr, attr) == 0) return l;
	}
	return NULL;
}
AGmLink *AGm::prev_outlink2(AGmNode *src, AGmLink *link, AGmNode *addr) {
	if (src == NULL || src->body == NULL || src->body->out == NULL ) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->prev; l && l != outlink; l = l->prev) {	
		if (l->arc->attr == addr) return l;
	}
	return NULL;
}
AGmLink *AGm::prev_outlink3(AGmNode *src, AGmLink *link, AGmNode *addr) {
	if (src == NULL || src->body == NULL || src->body->out == NULL ) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->prev; l && l != outlink; l = l->prev) {	
		if (l->arc->attr != addr) return l;
	}
	return NULL;
}
AGmLink *AGm::prev_inlink(AGmNode *dst, AGmLink *link) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l = link->prev;
	return ( (l && l != inlink) ? l : NULL);
}
AGmLink *AGm::prev_inlink(AGmNode *dst, AGmLink *link, AGmNode *attr) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->prev; l && l != inlink; l = l->prev) {	
		if (Compare(l->arc->attr, attr) == 0) return l;
	}
	return NULL;
}
AGmLink *AGm::prev_inlink2(AGmNode *dst, AGmLink *link, AGmNode *addr) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->prev; l && l != inlink; l = l->prev) {	
		if (l->arc->attr == addr) return l;
	}
	return NULL;
}
AGmLink *AGm::prev_inlink3(AGmNode *dst, AGmLink *link, AGmNode *addr) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->prev; l && l != inlink; l = l->prev) {	
		if (l->arc->attr != addr) return l;
	}
	return NULL;
}
#if	_WINDOWS && _64BIT
AGmLink* AGm::next_outlink(AGmNode* src, AGmLink* link, long long idx) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->next; l && l != outlink; l = l->next) {	
		if (IsInteger(l->arc->attr) && l->arc->attr->a.i.v == idx) return l;
	}
	return NULL;
}
AGmLink* AGm::next_inlink(AGmNode* dst, AGmLink* link, long long idx) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->next; l && l != inlink; l = l->next) {	
		if (IsInteger(l->arc->attr) && l->arc->attr->a.i.v == idx) return l;
	}
	return NULL;
}
AGmLink* AGm::prev_outlink(AGmNode* src, AGmLink* link, long long idx) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->prev; l && l != outlink; l = l->prev) {	
		if (IsInteger(l->arc->attr) && l->arc->attr->a.i.v == idx) return l;
	}
	return NULL;
}
AGmLink* AGm::prev_inlink(AGmNode* dst, AGmLink* link, long long idx) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->prev; l && l != inlink; l = l->prev) {	
		if (IsInteger(l->arc->attr) && l->arc->attr->a.i.v == idx) return l;
	}
	return NULL;
}
#else
AGmLink* AGm::next_outlink(AGmNode* src, AGmLink* link, long idx) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->next; l && l != outlink; l = l->next) {	
		if (IsInteger(l->arc->attr) && l->arc->attr->a.i.v == idx) return l;
	}
	return NULL;
}
AGmLink* AGm::next_inlink(AGmNode* dst, AGmLink* link, long idx) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->next; l && l != inlink; l = l->next) {	
		if (IsInteger(l->arc->attr) && l->arc->attr->a.i.v == idx) return l;
	}
	return NULL;
}
AGmLink* AGm::prev_outlink(AGmNode* src, AGmLink* link, long idx) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->prev; l && l != outlink; l = l->prev) {	
		if (IsInteger(l->arc->attr) && l->arc->attr->a.i.v == idx) return l;
	}
	return NULL;
}
AGmLink* AGm::prev_inlink(AGmNode* dst, AGmLink* link, long idx) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->prev; l && l != inlink; l = l->prev) {	
		if (IsInteger(l->arc->attr) && l->arc->attr->a.i.v == idx) return l;
	}
	return NULL;
}
#endif
AGmLink* AGm::next_outlink(AGmNode* src, AGmLink* link, char* str) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->next; l && l != outlink; l = l->next) {	
		if (IsString(l->arc->attr) && l->arc->attr->a.s.v->Compare(str) == 0) return l;
	}
	return NULL;
}
AGmLink* AGm::next_inlink(AGmNode* dst, AGmLink* link, char* str) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->next; l && l != inlink; l = l->next) {	
		if (IsString(l->arc->attr) && l->arc->attr->a.s.v->Compare(str) == 0) return l;
	}
	return NULL;
}
AGmLink* AGm::prev_outlink(AGmNode* src, AGmLink* link, char* str) {
	if (src == NULL || src->body == NULL || src->body->out == NULL) return NULL;
	AGmLink *outlink = src->body->out;
	if (link == NULL) link = outlink;
	AGmLink *l;
	for (l = link->prev; l && l != outlink; l = l->prev) {	
		if (IsString(l->arc->attr) && l->arc->attr->a.s.v->Compare(str) == 0) return l;
	}
	return NULL;
}
AGmLink* AGm::prev_inlink(AGmNode* dst, AGmLink* link, char* str) {
	if (dst == NULL || dst->body == NULL || dst->body->in == NULL) return NULL;
	AGmLink *inlink = dst->body->in;
	if (link == NULL) link = inlink;
	AGmLink *l;
	for (l = link->prev; l && l != inlink; l = l->prev) {	
		if (IsString(l->arc->attr) && l->arc->attr->a.s.v->Compare(str) == 0) return l;
	}
	return NULL;
}

AGmNode *AGm::dst_node(AGmNode *src) {
	AGmLink *l = next_outlink(src, NULL);
	return ( l ? l->arc->dst : NULL);
}
AGmNode *AGm::dst_node(AGmNode *src, AGmNode *attr) {
	AGmLink *l = next_outlink(src, NULL, attr);
	return ( l ? l->arc->dst : NULL);
}
AGmNode *AGm::dst_node(AGmNode *src, const char *attr, int ignore_case) {
	AGmLink *l;
	for (l = next_outlink(src, NULL); l; l = next_outlink(src, l)) {
		if (!IsString(l->arc->attr)) continue;
		if (ignore_case == 0) {
			if (strcmp(l->arc->attr->a.s.v->Str(), attr) == 0)
				return l->arc->dst;
		} else {
			if (l->arc->attr->a.s.v->CompareI(attr) == 0)
				return l->arc->dst;
		}
	}
	return NULL;
}
#if	_WINDOWS && _64BIT
AGmNode *AGm::dst_node(AGmNode *src, long long attr) {
#else
AGmNode *AGm::dst_node(AGmNode *src, long attr) {
#endif
	AGmLink *l;
	for (l = next_outlink(src, NULL); l; l = next_outlink(src, l)) {
		if (!IsInteger(l->arc->attr)) continue;
		if (l->arc->attr->a.i.v != attr) continue;
		return l->arc->dst;
	}
	return NULL;
}
AGmNode *AGm::dst_node2(AGmNode *src, AGmNode *addr) {
	AGmLink *l = next_outlink2(src, NULL, addr);
	return ( l ? l->arc->dst : NULL);
}
AGmNode *AGm::dst_node3(AGmNode *src, AGmNode *addr) {
	AGmLink *l = next_outlink3(src, NULL, addr);
	return ( l ? l->arc->dst : NULL);
}
AGmNode *AGm::src_node(AGmNode *dst) {
	AGmLink *l = next_inlink(dst, NULL);
	return ( l ? l->arc->src : NULL);
}
AGmNode *AGm::src_node(AGmNode *dst, AGmNode *attr) {
	AGmLink *l = next_inlink(dst, NULL, attr);
	return ( l ? l->arc->src : NULL);
}
AGmNode *AGm::src_node(AGmNode* dst, const char* attr, int ignore_case) {
	AGmLink *l;
	for (l = next_inlink(dst, NULL); l; l = next_inlink(dst, l)) {
		if (!IsString(l->arc->attr)) continue;
		if (ignore_case == 0) {
			if (strcmp(l->arc->attr->a.s.v->Str(), attr) == 0)
				return l->arc->src;
		} else {
			if (l->arc->attr->a.s.v->CompareI(attr) == 0)
				return l->arc->src;
		}
	}
	return NULL;
}
AGmNode *AGm::src_node2(AGmNode *dst, AGmNode *addr) {
	AGmLink *l = next_inlink2(dst, NULL, addr);
	return ( l ? l->arc->src : NULL);
}
AGmNode *AGm::src_node3(AGmNode *dst, AGmNode *addr) {
	AGmLink *l = next_inlink3(dst, NULL, addr);
	return ( l ? l->arc->src : NULL);
}
AGmArc* 	AGm::set_dst_node(AGmNode *src, AGmNode *new_dst) {
	AGmLink *l = next_outlink(src, NULL);
	if (l) {
		if (new_dst) {
			return ReplaceArcDst(l->arc, new_dst);
		} else {
			DeleteArc(l->arc);
			return NULL;
		}
	} else {
		if (new_dst) {
			return CreateArc(src, new_dst);
		} else {
			return NULL;
		}
	}
}
AGmArc* AGm::set_dst_node(AGmNode *src, AGmNode *attr, AGmNode *new_dst) {
	AGmLink *l = next_outlink(src, NULL, attr);
	if (l) {
		if (new_dst) {
			return ReplaceArcDst(l->arc, new_dst);
		} else {
			DeleteArc(l->arc);
			return NULL;
		}
	} else {
		if (new_dst) {
			return CreateArc(src, new_dst, attr);
		} else {
			return NULL;
		}
	}
}
AGmArc* AGm::set_dst_node(AGmNode *src, const char *attr0, AGmNode *new_dst) {
	AGmNode *attr = Ref(String(attr0));
	AGmArc *arc = set_dst_node(src, attr, new_dst);
	Unref(attr);
	return arc;
}
#if	_WINDOWS && _64BIT
AGmArc* AGm::set_dst_node(AGmNode *src, long long attr0, AGmNode *new_dst) {
#else
AGmArc* AGm::set_dst_node(AGmNode *src, long attr0, AGmNode *new_dst) {
#endif
	AGmNode *attr = Ref(Integer(attr0));
	AGmArc *arc = set_dst_node(src, attr, new_dst);
	Unref(attr);
	return arc;
}
AGmArc* AGm::set_dst_node2(AGmNode *src, AGmNode *addr, AGmNode *new_dst) {
	AGmLink *l = next_outlink2(src, NULL, addr);
	if (l) {
		if (new_dst) {
			return ReplaceArcDst(l->arc, new_dst);
		} else {
			DeleteArc(l->arc);
			return NULL;
		}
	} else {
		if (new_dst) {
			return CreateArc(src, new_dst, addr);
		} else {
			return NULL;
		}
	}
}
int AGm::out_count(AGmNode* n) {
	AGmLink *l; int count = 0;
	for (l = next_outlink(n, NULL); l; l = next_outlink(n, l)) count++;
	return count;
}
int AGm::out_count(AGmNode* n, AGmNode* a) {
	AGmLink *l; int count = 0;
	for (l = next_outlink(n, NULL, a); l; l = next_outlink(n, l, a)) count++;
	return count;
}
int AGm::in_count(AGmNode* n) {
	AGmLink *l; int count = 0;
	for (l = next_inlink(n, NULL); l; l = next_inlink(n, l)) count++;
	return count;
}
int AGm::in_count(AGmNode* n, AGmNode* a) {
	AGmLink *l; int count = 0;
	for (l = next_inlink(n, NULL, a); l; l = next_inlink(n, l, a)) count++;
	return count;
}

int AGm::Compare(AGmNode *n1, AGmNode *n2) {
	int flag;
	while (n1 && n2) {
		AGmNode *t1 = NULL, *t2 = NULL;
		if (n1->type > n2->type) return 1;
		if (n1->type < n2->type) return -1;
		switch (n1->type) {
		case AGM_NODE_INTEGER:
			if (n1->a.i.v > n2->a.i.v) return 1;
			if (n1->a.i.v < n2->a.i.v) return -1;
			t1 = n1->a.i.t; t2 = n2->a.i.t; break;
		case AGM_NODE_REAL:
			if (n1->a.r.v > n2->a.r.v) return 1;
			if (n1->a.r.v < n2->a.r.v) return -1;
			return 0;
		case AGM_NODE_LIST:
			flag = Compare(n1->a.l.h, n2->a.l.h);
			if (flag) return flag;
			t1 = n1->a.l.t; t2 = n2->a.l.t; break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n1->a.a.size;
			if (size < n2->a.a.size) return 1;
			if (size > n2->a.a.size) return -1;
			for (i = 0; i < size; i++) {
				flag = Compare(n1->a.a.v[i], n2->a.a.v[i]);
				if (flag) return flag;
			}
			return 0;
		case AGM_NODE_STRING:
			flag = n1->a.s.v->Compare(n2->a.s.v);
			if (flag) return flag;
			t1 = n1->a.s.t; t2 = n2->a.s.t; break;
		case AGM_NODE_MATRIX:
			if (n1->a.m.v->IsEqual(n2->a.m.v) == 0) return 0;
#ifdef _WINDOWS
			if ((unsigned long long)n1 < (unsigned long long)n2) return 1;
			if ((unsigned long long)n1 > (unsigned long long)n2) return -1;
#else
			if ((unsigned long)n1 < (unsigned long)n2) return 1;
			if ((unsigned long)n1 > (unsigned long)n2) return -1;
#endif
			return 0;
		case AGM_NODE_BINARY:
			flag = n1->a.bin.v->Compare(n2->a.bin.v);
			if (flag) return flag;
			t1 = n1->a.bin.t; t2 = n2->a.bin.t; break;
		case AGM_NODE_BIGINT:
			return BigInteger::Compare((long)n1->a.bi.sign, n1->a.bi.bin, (long)n2->a.bi.sign, n2->a.bi.bin);
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_FILE:
		case AGM_NODE_WINDOW:
		case AGM_NODE_BITMAP:
		case AGM_NODE_PTR:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			if ((unsigned long)n1 < (unsigned long)n2) return 1;
			if ((unsigned long)n1 > (unsigned long)n2) return -1;
			return 0;
		default:
			ASSERT(0);
		}
		n1 = t1; n2 = t2;
	}
	if (n1 == NULL && n2) return -1;
	if (n1 && n2 == NULL) return 1;
	return 0;
}

long AGm::ListLength(AGmNode *l) {
	long count;
	for (count = 0; l; l = l->T) count++;
	return count;
}

AGmNode *AGm::ListCopy(AGmNode *n, int us) {
	AGmNode *n2 = NULL, **addr = &n2;
	AGmNode *tmp;
	while (n) {
		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
			Set(*addr, Integer(n->a.i.v));
			t = n->a.i.t; addr = &(*addr)->a.i.t; break;
		case AGM_NODE_GFP:
			Set(*addr, GFp(n->a.gp.v, n->a.gp.mod));
			t = NULL; break;
		case AGM_NODE_REAL:
			Set(*addr, Real(n->a.r.v));
			t = NULL; break;
		case AGM_NODE_LIST:
			Set(*addr, Cons(ListCopy(n->a.l.h, us)));
			t = n->a.l.t; addr = &(*addr)->a.l.t; break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size; 
			Set(*addr, Array(size));
			for (i = 0; i < size; i++)
				Set((*addr)->a.a.v[i], ListCopy(n->a.a.v[i], us));
			t = NULL; break;
		case AGM_NODE_OBJECT:
			Set(*addr, Object(n->a.o.class_name, n->a.o.class_id));
			t = NULL; break;
		case AGM_NODE_DST_ITR:
			Set(*addr, DstItr(n->a.itr.node));
			t = NULL; break;
		case AGM_NODE_SRC_ITR:
			Set(*addr, SrcItr(n->a.itr.node));
			t = NULL; break;
		case AGM_NODE_STRING:
			if (us == 0) {
				AGmString *s = n->a.s.v;
				Set(*addr, String(s ? s->Str() : NULL));
			} else {
				Set(*addr, n);
			}
			t = n->a.s.t; addr = &(*addr)->a.s.t; break;
		case AGM_NODE_MATRIX:
			AGmMatrix *m;
			m = n->a.m.v;
			Set(*addr, Matrix(m ? m->Copy() : NULL));
			t = n->a.m.t; addr = &(*addr)->a.m.t; break;
		case AGM_NODE_FILE:
			Set(*addr, File(NULL));
			t = n->a.f.t; addr = &(*addr)->a.f.t; break;
		case AGM_NODE_WINDOW:
			Set(*addr, Window(NULL));
			t = n->a.w.t; addr = &(*addr)->a.w.t; break;
		case AGM_NODE_BITMAP:
			ADvBitmap *b;
			b = n->a.b.v;
			Set(*addr, Bitmap(b ? b->Copy() : NULL));
			t = n->a.b.t; addr = &(*addr)->a.w.t; break;
		case AGM_NODE_PTR:
			Set(*addr, PtrCell(NULL));
			t = n->a.p.t; addr = &(*addr)->a.p.t; break;
		case AGM_NODE_BINARY:
			AGmBinary *bin;
			bin = n->a.bin.v;
			Set(*addr, Binary(bin ? bin->Copy(): NULL));
			t = n->a.bin.t; addr = &(*addr)->a.bin.t; break;
		case AGM_NODE_BIGINT:
			bin = n->a.bi.bin;
			Set(*addr, BigInt((long)n->a.bi.sign, bin ? bin->Copy(): NULL));
			t = NULL; break;
		case AGM_NODE_OCX:
			Set(*addr, Ocx(NULL));
			t = NULL; break;
		case AGM_NODE_VARIANT:
			COleVariant *v;
			v = n->a.var.v; if (v) v = new COleVariant(*v);
			Set(*addr, Variant(v, n->a.var.byref));
			t = NULL; break;
		case AGM_NODE_SQL_CONN:
			Set(*addr, tmp = Integer(0));
			tmp->type = AGM_NODE_SQL_CONN;
#ifdef	_WINDOWS
			tmp->a.conn.impl->hdbc = NULL;
#endif
#ifdef	_LINUX
			tmp->a.conn.impl = NULL;
#endif
			t = NULL; break;
		case AGM_NODE_SQL_STAT:
			Set(*addr, tmp = Integer(0));
			tmp->type = AGM_NODE_SQL_STAT;
#ifdef	_WINDOWS
			tmp->a.stat.impl->hdbc = NULL;
			tmp->a.stat.impl->hstmt = NULL;
#endif
#ifdef	_LINUX
			tmp->a.stat.impl = NULL;
#endif
			t = NULL; break;
		default:
			ASSERT(0);
		}
		n = t;
	}
	if (n2) n2->count--;
	return n2;
}
AGmNode* AGm::ShallowCopy(AGmNode* n) {
	AGmNode *n2 = NULL, **addr = &n2;
	while (n && IsList(n))	{
		Set(*addr, Cons(n->H));
		n = n->T; addr = &(*addr)->T;
	}
	if (n2) n2->count--;
	return n2;
}

//#define	DEBUG_SORT

AGmNode* AGm::Sort(AGmNode* src, int desc) {
	int n = out_count(src);
	if (n <= 1) return src;
	AGmLink **l_src = new AGmLink*[n];
	AGmLink **l_dst = new AGmLink*[n];
	AGmLink *l; int idx1, idx2, len;
	for (l = next_outlink(src, NULL), idx1 = 0; l; l = next_outlink(src, NULL), idx1++) {
		AGM_REMOVE(l); l_src[idx1] = l; l->prev = l->next = l;
	}
#ifdef	DEBUG_SORT
int rep_count = 1;
#endif
	for (len = 1; len < n; len += len) {
#ifdef	DEBUG_SORT
AGmNode *count = Ref(List(Integer(rep_count++), Integer(len)));
DumpList("loop(1) (count, len) = ", count);
Unref(count);
int rep_count2 = 1;
#endif
		int first = 1, idx, e1, e2, lhs_empty = 1, rhs_empty = 1;
		AGmLink *lhs = NULL, *rhs = NULL, *lnk = NULL;
		for (idx = idx1 = 0, idx2 = idx1 + len, e1 = min(idx2, n), e2 = min(idx2 + len, n);
				idx1 < e1 && idx2 < e2;
				idx1 += len, idx2 = idx1 + len, e1 = min(idx2, n), e2 = min(idx2 + len, n)) {
#ifdef	DEBUG_SORT
AGmNode *count = Ref(List(Integer(rep_count2++), Integer(idx1), Integer(e1), Integer(idx2), Integer(e2)));
DumpList("(loop(2) (count, idx1, e1, idx2, e2) = ", count);
Unref(count);
#endif
			while (idx1 < e1 && idx2 < e2) {
				if (lhs_empty) {
					lhs_empty = 0;
					lhs = l_src[idx1];
				}
				if (rhs_empty) {
					rhs_empty = 0;
					rhs = l_src[idx2];
				}
				if (desc == 0) {
					if (Compare(lhs->arc->attr, rhs->arc->attr) <= 0) {
						if (first == 1) {
							first = 0;
							l_dst[idx++] = lhs;
						} else {
							l_dst[idx] = lhs;
							AGM_REMOVE(lhs);
							lnk = l_dst[idx-1]->next;
							AGM_INSERT(lhs, lnk);
							idx++;
						}
						idx1++;
						lhs_empty = 1;
#ifdef	DEBUG_SORT
AGmNode *i1 = Ref(Integer(idx1-1));
DumpList("idx1 = ", i1);
Unref(i1);
#endif
					} else {
						if (first == 1) {
							first = 0;
							l_dst[idx++] = rhs;
						} else {
							l_dst[idx] = rhs;
							AGM_REMOVE(rhs);
							lnk = l_dst[idx-1]->next;
							AGM_INSERT(rhs, lnk);
							idx++;
						}
						idx2++;
						rhs_empty = 1;
#ifdef	DEBUG_SORT
AGmNode *i2 = Ref(Integer(idx2-1));
DumpList("idx2 = ", i2);
Unref(i2);
#endif
					}
				} else {
					if (Compare(lhs->arc->attr, rhs->arc->attr) <= 0) {
						if (first == 1) {
							first = 0;
							l_dst[idx++] = rhs;
						} else {
							l_dst[idx] = rhs;
							AGM_REMOVE(rhs);
							lnk = l_dst[idx-1]->next;
							AGM_INSERT(rhs, lnk);
							idx++;
						}
						idx2++;
						rhs_empty = 1;
#ifdef	DEBUG_SORT
AGmNode *i2 = Ref(Integer(idx2-1));
DumpList("idx2 = ", i2);
Unref(i2);
#endif
					} else {
						if (first == 1) {
							first = 0;
							l_dst[idx++] = lhs;
						} else {
							l_dst[idx] = lhs;
							AGM_REMOVE(lhs);
							lnk = l_dst[idx-1]->next;
							AGM_INSERT(lhs, lnk);
							idx++;
						}
						idx1++;
						lhs_empty = 1;
#ifdef	DEBUG_SORT
AGmNode *i1 = Ref(Integer(idx1-1));
DumpList("idx1 = ", i1);
Unref(i1);
#endif
					}
				}
			}
			if (idx1 < e1) {
				for (; idx1 < e1; ) {
					if (lhs_empty) {
						lhs_empty = 0;
						lhs = l_src[idx1];
					}
#ifdef	DEBUG_SORT
AGmNode *i1 = Ref(Integer(idx1));
DumpList("reminder idx1 = ", i1);
Unref(i1);
#endif
					l_dst[idx] = lhs;
					AGM_REMOVE(lhs);
					lnk = l_dst[idx-1]->next;
					AGM_INSERT(lhs, lnk);
					idx++;
					idx1++;
					lhs_empty = 1;
				}
			} else if (idx2 < e2) {
				for (; idx2 < e2; ) {
					if (rhs_empty) {
						rhs_empty = 0;
						rhs = l_src[idx2];
					}
#ifdef	DEBUG_SORT
AGmNode *i2 = Ref(Integer(idx2));
DumpList("reminder idx2 = ", i2);
Unref(i2);
#endif
					l_dst[idx] = rhs;
					AGM_REMOVE(rhs);
					lnk = l_dst[idx-1]->next;
					AGM_INSERT(rhs, lnk);
					idx++;
					idx2++;
					rhs_empty = 1;
				}
			}
		}
		if (idx1 < e1) {
			for (; idx1 < e1; ) {
				if (lhs_empty) {
					lhs_empty = 0;
					lhs = l_src[idx1];
				}
#ifdef	DEBUG_SORT
AGmNode *i1 = Ref(Integer(idx1));
DumpList("reminder(2) idx1 = ", i1);
Unref(i1);
#endif
				l_dst[idx] = lhs;
				AGM_REMOVE(lhs);
				lnk = l_dst[idx-1]->next;
				AGM_INSERT(lhs, lnk);
				idx++;
				idx1++;
				lhs_empty = 1;
			}
		} else if (idx2 < e2) {
			for (; idx2 < e2; ) {
				if (rhs_empty) {
					rhs_empty = 0;
					rhs = l_src[idx2];
				}
#ifdef	DEBUG_SORT
AGmNode *i2 = Ref(Integer(idx2));
DumpList("reminder(2) idx2 = ", i2);
Unref(i2);
#endif
				l_dst[idx] = rhs;
				AGM_REMOVE(rhs);
				lnk = l_dst[idx-1]->next;
				AGM_INSERT(rhs, lnk);
				idx++;
				idx2++;
				rhs_empty = 1;
			}
		}
#ifdef	DEBUG_SORT
count = Ref(List(Integer(idx1), Integer(e1), Integer(idx2), Integer(e2)));
DumpList("end of loop(2) (idx1, e1, idx2, e2) = ", count);
Unref(count);
#endif
		AGmLink **tmp = l_src;
		l_src = l_dst;
		l_dst = tmp;
	}
	AGM_INSERT(src->body->out, l_src[0]);
	delete [] l_src;
	delete [] l_dst;
	return src;
}
AGmNode* AGm::MaxAttr(AGmNode* src) {
	AGmLink *l;
	AGmNode *attr = NULL;
	int first = 1;
	for (l = next_outlink(src, NULL); l; l = next_outlink(src, l)) {
		if (first) {
			first = 0;
			attr = l->arc->attr;
			continue;
		}
		if (Compare(l->arc->attr, attr) >= 0) {
			attr = l->arc->attr;
		}
	}
	return attr;
}
AGmNode* AGm::MinAttr(AGmNode* src) {
	AGmLink *l;
	AGmNode *attr = NULL;
	int first = 1;
	for (l = next_outlink(src, NULL); l; l = next_outlink(src, l)) {
		if (first) {
			first = 0;
			attr = l->arc->attr;
			continue;
		}
		if (Compare(l->arc->attr, attr) <= 0) {
			attr = l->arc->attr;
		}
	}
	return attr;
}

AGmNode* AGm::List(AGmNode* n1) {
	return Cons(n1);
}
AGmNode* AGm::List(AGmNode* n1, AGmNode* n2) {
	return Cons(n1, List(n2));
}
AGmNode* AGm::List(AGmNode* n1, AGmNode* n2, AGmNode* n3) {
	return Cons(n1, List(n2, n3));
}
AGmNode* AGm::List(AGmNode* n1, AGmNode* n2, AGmNode* n3, AGmNode* n4) {
	return Cons(n1, List(n2, n3, n4));
}
AGmNode* AGm::List(AGmNode* n1, AGmNode* n2, AGmNode* n3, AGmNode* n4, AGmNode* n5) {
	return Cons(n1, List(n2, n3, n4, n5));
}
AGmNode* AGm::List(AGmNode* n1, AGmNode* n2, AGmNode* n3, AGmNode* n4, AGmNode* n5, AGmNode* n6) {
	return Cons(n1, List(n2, n3, n4, n5, n6));
}
AGmNode* AGm::List(AGmNode* n1, AGmNode* n2, AGmNode* n3, AGmNode* n4,
			AGmNode* n5, AGmNode* n6, AGmNode *n7) {
	return Cons(n1, List(n2, n3, n4, n5, n6, n7));
}
AGmNode* AGm::List(AGmNode* n1, AGmNode* n2, AGmNode* n3, AGmNode* n4,
			AGmNode* n5, AGmNode* n6, AGmNode *n7, AGmNode *n8) {
	return Cons(n1, List(n2, n3, n4, n5, n6, n7, n8));
}

#if	_WINDOWS && _64BIT
AGmNode* AGm::List(long long i1, AGmNode* tail) {
	return Cons(Integer(i1), tail);
}
AGmNode* AGm::List(long long i1, long long i2, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, tail));
}
AGmNode* AGm::List(long long i1, long long i2, long long i3, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, i3, tail));
}
AGmNode* AGm::List(long long i1, long long i2, long long i3, long long i4, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, i3, i4, tail));
}
AGmNode* AGm::List(long long i1, long long i2, long long i3, long long i4, long long i5, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, i3, i4, i5, tail));
}
AGmNode* AGm::List(long long i1, long long i2, long long i3, long long i4, long long i5, long long i6, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, i3, i4, i5, i6, tail));
}
#else
AGmNode* AGm::List(long i1, AGmNode* tail) {
	return Cons(Integer(i1), tail);
}
AGmNode* AGm::List(long i1, long i2, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, tail));
}
AGmNode* AGm::List(long i1, long i2, long i3, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, i3, tail));
}
AGmNode* AGm::List(long i1, long i2, long i3, long i4, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, i3, i4, tail));
}
AGmNode* AGm::List(long i1, long i2, long i3, long i4, long i5, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, i3, i4, i5, tail));
}
AGmNode* AGm::List(long i1, long i2, long i3, long i4, long i5, long i6, AGmNode* tail) {
	return Cons(Integer(i1), List(i2, i3, i4, i5, i6, tail));
}
#endif

AGmNode* AGm::List(double r1, AGmNode* tail) {
	return Cons(Real(r1), tail);
}
AGmNode* AGm::List(double r1, double r2, AGmNode* tail) {
	return Cons(Real(r1), List(r2, tail));
}
AGmNode* AGm::List(double r1, double r2, double r3, AGmNode* tail) {
	return Cons(Real(r1), List(r2, r3, tail));
}
AGmNode* AGm::List(double r1, double r2, double r3, double r4, AGmNode* tail) {
	return Cons(Real(r1), List(r2, r3, r4, tail));
}

AGmNode* AGm::List(const char* s, AGmNode* tail) {
	return Cons(String(s), tail);
}

#if	_WINDOWS && _64BIT
AGmNode* AGm::List(const char *s, long long i, AGmNode *tail) {
	return Cons(String(s), List(i, tail));
}
AGmNode* AGm::List(const char *s, long long i1, long long i2, AGmNode *tail) {
	return Cons(String(s), List(i1, i2, tail));
}
AGmNode* AGm::List(const char *s, long long i1, long long i2, long long i3, AGmNode *tail) {
	return	Cons(String(s), List(i1, i2, i3, tail));
}
#else
AGmNode* AGm::List(const char *s, long i, AGmNode *tail) {
	return Cons(String(s), List(i, tail));
}
AGmNode* AGm::List(const char *s, long i1, long i2, AGmNode *tail) {
	return Cons(String(s), List(i1, i2, tail));
}
AGmNode* AGm::List(const char *s, long i1, long i2, long i3, AGmNode *tail) {
	return	Cons(String(s), List(i1, i2, i3, tail));
}
#endif

// ---------- memory management ----------

long AGm::max_heap_num = 50000;
long AGm::curr_heap_num = 0;

long AGm::MaxHeapNum() { return max_heap_num; }
long AGm::CurrHeapNum() { return curr_heap_num; }
long AGm::HeapBlockSize() { return AGM_MEM_BLOCK_SIZE; }
int AGm::MaxHeapNum(long num) {
	if (num <= curr_heap_num) return 1;
	max_heap_num = num;
	return 0;
}

AGmCell *AGm::_mem_block_chain = NULL;
AGmCell *AGm::_mem_block = NULL;
unsigned short AGm::_mem_index;
AGmCell *AGm::_mem_free = NULL;
long AGm::_allocated_max_id = 0xA000;

#ifdef	_LEAK_CHECK
static long _num_alloc = 0;
static long _num_free = 0;
static long _num_uniqueID = 0;
#endif

AGmCell *AGm::Alloc() {
	AGmCell *cell = NULL;
	if (_mem_free) {
		cell = _mem_free;
		_mem_free = (AGmCell*)((AGmNode*)_mem_free)->a.l.t;
#ifdef	_LEAK_CHECK
	_num_free--;
#ifdef	_CHECK_ALLOC_FREE
	TRACE1("Alloc: %x\n", cell);
#endif
#endif
	} else if (_mem_block && _mem_index < AGM_MEM_BLOCK_SIZE) {
		cell = &_mem_block[_mem_index++];
#ifdef	_LEAK_CHECK
	_num_alloc++;
#ifdef	_CHECK_ALLOC_FREE
	TRACE1("Alloc(New): %x\n", cell);
#endif
#endif
	} else {
		if (curr_heap_num >= max_heap_num)
			throw new AlException(CELL_HEAP_OVERFLOW, AL_STR("cell heap overflow."));
		_mem_block = new AGmCell[AGM_MEM_BLOCK_SIZE];
		_mem_index = 1;

		curr_heap_num++;
		((AGmNode*)_mem_block)->a.l.t = (AGmNode*)_mem_block_chain;
		_mem_block_chain = _mem_block;
		cell = &_mem_block[_mem_index++];
#ifdef	_LEAK_CHECK
	_num_alloc++;
#ifdef	_CHECK_ALLOC_FREE
	TRACE1("Alloc(New): %x\n", cell);
#endif
#endif
	}
	return cell;
}
void AGm::Free(AGmCell *cell) {
	((AGmNode*)cell)->flag |= AGM_FLAG_FREE_CELL;
	((AGmNode*)cell)->a.l.t = (AGmNode*)_mem_free;
	_mem_free = cell;
#ifdef	_LEAK_CHECK
	_num_free++;
#ifdef	_CHECK_ALLOC_FREE
	TRACE1("Free: %x\n", cell);
#endif
#endif
}

long AGm::_mem_system_ref_count = 0;
void AGm::RefMemorySystem() {
	if (_mem_system_ref_count++ > 0)
		return;
	InitUniqueStrTbl();

	minus_one = Ref(Integer(-1));
	zero = Ref(Integer(0));
	one = Ref(Integer(1));
	two = Ref(Integer(2));
	three = Ref(Integer(3));
	four = Ref(Integer(4));
	five = Ref(Integer(5));
	six = Ref(Integer(6));
	seven = Ref(Integer(7));
	eight = Ref(Integer(8));
	nine = Ref(Integer(9));
	ten = Ref(Integer(10));

	AUiProgramMgr *pgm = AUiProgramMgr::Instance();
	if (pgm == NULL)
		new AUiProgramMgr();
	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	if (pm == NULL)
		new AIpProcessMgr();
}
void AGm::UnrefMemorySystem() {
	if (--_mem_system_ref_count == 0)
		ReleaseMemory();
}

void AGm::ReleaseMemory() {
	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	if (pm)
		delete pm;
	AUiProgramMgr *pgm = AUiProgramMgr::Instance();
	if (pgm)
		delete pgm;

	Unref(minus_one, zero, one, two, three);
	Unref(four, five, six);
	Unref(seven, eight, nine, ten);

	AGm::ReleaseClipboard();

	void* value;
	GetValue(NULL, AGM_VM_KEY_TREE_ROOT, value);
	RemoveValue(NULL, AGM_VM_KEY_TREE_ROOT);
	DeleteLoopRef((AGmNode*)value);
	Unref((AGmNode*)value);
	
	// string list whose addrese is used as ID
	Unref(_class, _subclass, _member); _class = _subclass = _member = NULL;
	Unref(_type, _vtbl, _this); _type = _vtbl = _this = NULL;
	Unref(_window, _global, _doc_tmpl); _window = _global = _doc_tmpl = NULL;
	Unref(_pen, _brush, _font, _pipeStr); _pen = _brush = _font =_pipeStr = NULL;
	Unref(_event, _o, _arg, _prof); _event = _o = _arg = _prof = NULL;
	Unref(_pro_lisp, _prolog, _game); _pro_lisp = _prolog = _game = NULL;
	// free list of UniqueStr
	Unref(_u_str_tbl); _u_str_tbl = NULL; _init_u_str_tbl = 0;
	// free list of UniqueID
	Unref(_free_id_list); _free_id_list = NULL;

// for static variables that are shared between dll and AP
#ifdef	_LEAK_CHECK
{
	AGmNode *l;
	for (l = _values; l; l = l->T)
		TRACE3(AL_STR("(key, ptr, value) = (%lx, %lx, %lx)\n"),
			l->HI,
			AL_PTR(l->H->a.i.t->a.p.ptr),
			AL_PTR(l->H->a.i.t->a.p.t->a.p.ptr));
}
#endif
	Unref(_values); _values = NULL;
#ifdef	_DEBUG_ALLOC_FREE
	DebugLeakCells();
#endif

// garbage collection
	GC();

// area for cells
	AGmNode* n = (AGmNode*)_mem_block_chain;
	while (n) {
		AGmNode *n1 = n->a.l.t;
		delete [] n;
		n = n1;
	}
	_mem_block_chain = NULL;

#ifdef	_LEAK_CHECK
	TRACE0(AL_STR("\n"));
	TRACE1(AL_STR("num alloc cell %d\n"), _num_alloc);
	TRACE1(AL_STR("num free  cell %d\n"), _num_free);
	TRACE1(AL_STR("num uniqueID %d\n"), _num_uniqueID);
#endif
}

long AGm::MemorySize(AGmNode *n) {
	long size = 0;
	_MemorySize(n, size);
	UnmarkNode(n, AGM_FLAG_MARK);
	return size;
}
void AGm::_MemorySize(AGmNode *n, long& size) {
	while (n) {
		if (n->flag & AGM_FLAG_MARK) return;
		n->flag |= AGM_FLAG_MARK;
		AGmNode *t;
		size += sizeof(AGmCell);
		switch (n->type) {
		case AGM_NODE_INTEGER:
			t = n->a.i.t; break;
		case AGM_NODE_BIGINT:
			size += sizeof(AGmBinary) + (n->a.bi.bin ? n->a.bi.bin->Length() : 0);
			t = NULL; break;
		case AGM_NODE_REAL:
			t = NULL; break;
		case AGM_NODE_STRING:
			size += sizeof(AGmString) + n->a.s.v->MaxLength();
			t = n->a.s.t; break;
		case AGM_NODE_LIST:
			_MemorySize(n->a.l.h, size);
			t = n->a.l.t; break;
		case AGM_NODE_BINARY:
			size += sizeof(AGmBinary) + (n->a.bin.v ? n->a.bin.v->Length() : 0);
			t = n->a.bin.t; break;
		case AGM_NODE_WINDOW:
			size += sizeof(ADvWindow);
			t = n->a.w.t; break;
		case AGM_NODE_MATRIX:
			size += sizeof(AGmMatrix);
			t = n->a.m.t; break;
		case AGM_NODE_FILE:
			size += sizeof(AGmFile);
			t = n->a.f.t; break;
		case AGM_NODE_BITMAP:
			size += sizeof(ADvBitmap);
			t = n->a.b.t; break;
		case AGM_NODE_PTR:
			t = n->a.p.t; break;
		default:
			t = NULL; break;
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			_MemorySize(l->arc->attr, size);
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
				_MemorySize(l->arc->dst, size);
			} else {
			}
		}
		n = t;
	}
}

// --------------- Unique ID management ---------------
AGmNode *AGm::_free_id_list = NULL;

#if	_WINDOWS && _64BIT
long long AGm::GetUniqueID() {
#else
long AGm::GetUniqueID() {
#endif
#ifdef	_LEAK_CHECK
	_num_uniqueID ++;
#endif
	AGmNode *n = _free_id_list;
	if (n) {
#if	_WINDOWS && _64BIT
		long long id = n->HI;
#else
		long id = n->HI;
#endif
		Pop(_free_id_list);
		return id;
	}
	return _allocated_max_id++;
}

#if	_WINDOWS && _64BIT
int AGm::ReleaseUniqueID(long long id) {
#else
int AGm::ReleaseUniqueID(long id) {
#endif
#ifdef	_LEAK_CHECK
	_num_uniqueID --;
#endif
	AGmNode *n = Integer(id);
	Push(_free_id_list, n);
	return 0;
}

// any
int AGm::Param(AGmNode* args,
		AGmNode*& arg1,
		AGmNode*& tail) {
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	arg1 = args->H;
	tail = args->T; return 0;
}

#define R4E5(f) ((f)>=0 ? (f)+0.5 : (f)-0.5)

// long
#if	_WINDOWS && _64BIT
int AGm::Param(AGmNode* args,
		long long& arg1,
		AGmNode*& tail) {
#else
int AGm::Param(AGmNode* args,
		long& arg1,
		AGmNode*& tail) {
#endif
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = (long)R4E5(arg->a.r.v);
	} else { return 1; }
	tail = args->T; return 0;
}
#if	_WINDOWS && _64BIT
int AGm::Param(AGmNode* args,
		long long& arg1, long long& arg2,
		AGmNode*& tail) {
#else
int AGm::Param(AGmNode* args,
		long& arg1, long& arg2,
		AGmNode*& tail) {
#endif
			if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = (long)R4E5(arg->a.r.v);
	} else { return 1; }
	return Param(args->T, arg2, tail);
}
#if	_WINDOWS && _64BIT
int AGm::Param(AGmNode* args,
		long long& arg1, long long& arg2, long long& arg3,
		AGmNode*& tail) {
#else
int AGm::Param(AGmNode* args,
		long& arg1, long& arg2, long& arg3,
		AGmNode*& tail) {
#endif
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = (long)R4E5(arg->a.r.v);
	} else { return 1; }
	return Param(args->T, arg2, arg3, tail);
}
#if	_WINDOWS && _64BIT
int AGm::Param(AGmNode* args,
		long long& arg1, long long& arg2, long long& arg3, long long& arg4,
		AGmNode*& tail) {
#else
int AGm::Param(AGmNode* args,
		long& arg1, long& arg2, long& arg3, long& arg4,
		AGmNode*& tail) {
#endif
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = (long)R4E5(arg->a.r.v);
	} else { return 1; }
	return Param(args->T, arg2, arg3, arg4, tail);
}
#if	_WINDOWS && _64BIT
int AGm::Param(AGmNode* args,
		long long& arg1, long long& arg2, long long& arg3, long long& arg4, long long& arg5,
		AGmNode*& tail) {
#else
int AGm::Param(AGmNode* args,
		long& arg1, long& arg2, long& arg3, long& arg4, long& arg5,
		AGmNode*& tail) {
#endif
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = (long)R4E5(arg->a.r.v);
	} else { return 1; }
	return Param(args->T, arg2, arg3, arg4, arg5, tail);
}
#if	_WINDOWS && _64BIT
int AGm::Param(AGmNode* args,
		long long& arg1, long long& arg2, long long& arg3, long long& arg4, long long& arg5, long long& arg6,
		AGmNode*& tail) {
#else
int AGm::Param(AGmNode* args,
		long& arg1, long& arg2, long& arg3, long& arg4, long& arg5, long& arg6,
		AGmNode*& tail) {
#endif
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = (long)R4E5(arg->a.r.v);
	} else { return 1; }
	return Param(args->T, arg2, arg3, arg4, arg5, arg6, tail);
}

// double
int AGm::Param(AGmNode* args,
		double& arg1,
		AGmNode*& tail) {
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = (double)arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = arg->a.r.v;
	} else { return 1; }
	tail = args->T; return 0;
}
int AGm::Param(AGmNode* args,
		double& arg1, double& arg2,
		AGmNode*& tail) {
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = (double)arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = arg->a.r.v;
	} else { return 1; }
	return Param(args->T, arg2, tail);
}
int AGm::Param(AGmNode* args,
		double& arg1, double& arg2, double& arg3,
		AGmNode*& tail) {
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = (double)arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = arg->a.r.v;
	} else { return 1; }
	return Param(args->T, arg2, arg3, tail);
}
int AGm::Param(AGmNode* args,
		double& arg1, double& arg2, double& arg3, double& arg4,
		AGmNode*& tail) {
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsInteger(arg)) { arg1 = (double)arg->a.i.v;
	} else if (IsReal(arg)) { arg1 = arg->a.r.v;
	} else { return 1; }
	return Param(args->T, arg2, arg3, arg4, tail);
}

// char*
int AGm::Param(AGmNode* args,
		char*& arg1,
		AGmNode*& tail) {
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { arg1 = NULL; tail = args->T; return 0;
	} else if (IsString(arg)) { arg1 = (char*)arg->a.s.v->Str();
	} else { return 1; }
	tail = args->T; return 0;
}

// AGmMatrix*
int AGm::Param(AGmNode* args,
		AGmMatrix*& arg1,
		AGmNode*& tail) {
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsMatrix(arg)) { arg1 = arg->a.m.v;
	} else { return 1; }
	tail = args->T; return 0;
}

// AGmBinary*
int AGm::Param(AGmNode* args,
		AGmBinary*& arg1,
		AGmNode*& tail) {
	if (args == NULL || !IsList(args)) {
		return 1;
	}
	AGmNode *arg = args->H;
	if (arg == NULL) { return 1;
	} else if (IsBinary(arg)) { arg1 = arg->a.bin.v;
	} else { return 1; }
	tail = args->T; return 0;
}

int AGm::_init_u_str_tbl = 0;

int AGm::InitUniqueStrTbl() {
	if (_init_u_str_tbl) return 0;
	_class = Ref(UniqueStr("$class"));
	_subclass = Ref(UniqueStr("$subclass"));
	_member = Ref(UniqueStr("$member"));
	_type = Ref(UniqueStr("$type"));
	_vtbl = Ref(UniqueStr("$vtbl"));
	_this = Ref(UniqueStr("$this"));
	_window = Ref(UniqueStr("$window"));
	_doc_tmpl = Ref(UniqueStr("$doc_tmpl"));
	_global = Ref(UniqueStr("$global"));
	_pen = Ref(UniqueStr("$pen"));
	_brush = Ref(UniqueStr("$brush"));
	_font = Ref(UniqueStr("$font"));
	_pipeStr = Ref(UniqueStr("$pipe"));
	_event = Ref(UniqueStr("event"));
	_o = Ref(UniqueStr("o"));
	_arg = Ref(UniqueStr("arg"));
	_prof = Ref(UniqueStr("$prof"));
	_pro_lisp = Ref(UniqueStr("$pro_lisp"));
	_prolog = Ref(UniqueStr("$prolog"));
	_game = Ref(UniqueStr("$game"));

	_init_u_str_tbl = 1;
	return 0;
}

AGmNode *AGm::_u_str_tbl = NULL;

AGmNode* AGm::UniqueStr(const char *key) {
	return SearchSortCell(_u_str_tbl, key);
}
AGmNode* AGm::UniqueStr(AGmString* s) {
	return UniqueStr(s->Str());
}
AGmNode* AGm::UniqueStr(AGmNode* n) {
	if (!IsString(n)) return NULL;
	AGmString *s = n->a.s.v;
	if (s == NULL) return NULL;
	return UniqueStr(s->Str());
}
AGmNode *AGm::SearchSortCell(AGmNode*& cell, const char* key) {
	if (cell == NULL) {
		AGmNode *n1 = String(key);
		Set(cell, List(n1, (AGmNode*)NULL, (AGmNode*)NULL));
		return n1;
	}
	int result = cell->H->a.s.v->Compare(key);
	if (result == 0) {
		return cell->H;
	} else if (result > 0) {
		return SearchSortCell(cell->TH, key);
	} else {
		return SearchSortCell(cell->TTH, key);
	}
}
void AGm::ReduceUniqueStrTbl() {
	ReduceSortCell(_u_str_tbl);
}
void AGm::ReduceSortCell(AGmNode*& cell) {
	if (cell->TH) ReduceSortCell(cell->TH);
	if (cell->TTH) ReduceSortCell(cell->TTH);
	if (cell->H->count == 1) {
#ifdef	_DEBUG_REDUCE_UNIQUE_STR_TBL
		TRACE1("free unique str[[[%s]]]\n", cell->H->a.s.v->Str());
#endif
		AGmNode *a = cell->TH;
		AGmNode *b = cell->TTH;
		if (a == NULL && b == NULL) {
			Set(cell, NULL); return;
		}
		if (a == NULL) {
			Set(cell, b); return;
		}
		if (b == NULL) {
			Set(cell, a); return;
		}
		AGmNode **a2 = &cell->TH;
		AGmNode **b1 = &cell->TTH;
		for (;;) {
			if ((*a2)->TTH == NULL) {
				Set((*a2)->TTH, b); Set(cell, a); return;
			}
			if ((*b1)->TH == NULL) {
				Set((*b1)->TH, a); Set(cell, b); return;
			}
			a2 = &(*a2)->TTH; b1 = &(*b1)->TH;
		}
	}
}

void AGm::MakeStrUnique(AGmNode *n) {
	_MakeStrUnique(n);
	UnmarkNode(n, AGM_FLAG_MARK);
}
void AGm::_MakeStrUnique(AGmNode *n) {
	while (n) {
		if (n->flag & AGM_FLAG_MARK) break;
		n->flag |= AGM_FLAG_MARK;
		AGmNode *t;
		switch (n->type) {
		default:
			t = NULL;	break;
		case	AGM_NODE_INTEGER:
		case	AGM_NODE_STRING:
		case	AGM_NODE_MATRIX:
		case	AGM_NODE_FILE:
		case	AGM_NODE_WINDOW:
		case	AGM_NODE_BITMAP:
		case	AGM_NODE_PTR:
		case	AGM_NODE_BINARY:
			t = n->a.i.t;	break;
		case	AGM_NODE_LIST:
			if (IsString(n->a.l.h) && n->a.l.h->a.s.t == NULL) {
				Set(n->a.l.h, UniqueStr(n->a.l.h));
			} else {
				_MakeStrUnique(n->a.l.h);
			}
			t = n->a.l.t;	break;
		case	AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++)
				_MakeStrUnique(n->a.a.v[i]);
			t = NULL;
			break;
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			AGmNode *n1;
			n1 = l->arc->attr;
			if (IsString(n1) && n1->a.s.t == NULL) {
				Set(l->arc->attr, UniqueStr(n1));
			} else {
				_MakeStrUnique(l->arc->attr);
			}
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
				_MakeStrUnique(l->arc->dst);
			} else {
			}
		}
		n = t;
	}
}

AGmNode *AGm::_values = NULL;
int AGm::SetValue(void* ptr, long key, void* value) {
	AGmNode *l = _values;
	for (;; l = l->T) {
		if (l == NULL) {
			Push(_values, Integer(key, PtrCell(ptr, PtrCell(value))));
			break;
		}
		if (l->HI != key) continue;
		if (l->H->a.i.t->a.p.ptr != ptr) continue;
		l->H->a.i.t->a.p.t->a.p.ptr = value;
		break;
	}
	return 0;
}
int AGm::GetValue(void* ptr, long key, void*& value) {
	AGmNode *l = _values;
	for (;; l = l->T) {
		if (l == NULL) { value = NULL; return 1; }
		if (l->HI != key) continue;
		if (l->H->a.i.t->a.p.ptr != ptr) continue;
		value = l->H->a.i.t->a.p.t->a.p.ptr;
		break;
	}
	return 0;
}
int AGm::RemoveValue(void* ptr, long key) {
	AGmNode *l = _values;
	AGmNode **addr = &_values;
	for (;; addr = &l->T, l = l->T) {
		if (l == NULL) {
			TRACE1(AL_STR("VmKey %x not found.\n"), key);
			return 1;
		}
		if (l->HI != key) continue;
		if (l->H->a.i.t->a.p.ptr != ptr) continue;
		Pop(*addr); 	
		break;
	}
	return 0;
}

AGmNode* AGm::Root() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_TREE_ROOT, value);
	if (value)
		return (AGmNode*)value;
	if (ADvExiting) return NULL;
	AGmNode *root = Ref(Cons(NULL));
	SetValue(NULL, AGM_VM_KEY_TREE_ROOT, root);
	return root;
}
AGmNode* AGm::RootWindowNode() {
	AGmNode *root = Root();
	AGmNode *root_window = dst_node2(root, NmWindow());
	if (root_window)
		return root_window;
	root_window = Cons(NULL);
	CreateArc(root, root_window, NmWindow());
	return root_window;
}
AGmNode* AGm::DocTmpl() {
	AGmNode *root = Root();
	AGmNode *doc_tmpl = dst_node2(root, NmDocTmpl());
	if (doc_tmpl)
		return doc_tmpl;
	doc_tmpl = Cons(NULL);
	CreateArc(root, doc_tmpl, NmDocTmpl());
	return doc_tmpl;
}
AGmNode* AGm::RootGlobal() {
	AGmNode *root = Root();
	AGmNode *root_global = dst_node2(root, NmGlobal());
	if (root_global)
		return root_global;
	root_global = Cons(NULL);
	CreateArc(root, root_global, NmGlobal());
	return root_global;
}

AGmNode *AGm::_class;
AGmNode *AGm::_subclass;
AGmNode *AGm::_member;
AGmNode *AGm::_type;
AGmNode *AGm::_vtbl;
AGmNode *AGm::_this;
AGmNode *AGm::_window;
AGmNode *AGm::_doc_tmpl;
AGmNode *AGm::_global;
AGmNode *AGm::_pen;
AGmNode *AGm::_brush;
AGmNode *AGm::_font;
AGmNode *AGm::_pipeStr;
AGmNode *AGm::_event;
AGmNode *AGm::_o;
AGmNode *AGm::_arg;
AGmNode *AGm::_prof;
AGmNode *AGm::_pro_lisp;
AGmNode *AGm::_prolog;
AGmNode *AGm::_game;

AGmNode *AGm::minus_one;
AGmNode *AGm::zero;
AGmNode *AGm::one;
AGmNode *AGm::two;
AGmNode *AGm::three;
AGmNode *AGm::four;
AGmNode *AGm::five;
AGmNode *AGm::six;
AGmNode *AGm::seven;
AGmNode *AGm::eight;
AGmNode *AGm::nine;
AGmNode *AGm::ten;

AGmNode* AGm::NmClass() { return _class; }
AGmNode* AGm::NmSubClass() { return _subclass; }
AGmNode* AGm::NmMember() { return _member; }
AGmNode* AGm::NmType() { return _type; }
AGmNode* AGm::NmVtbl() { return _vtbl; }
AGmNode* AGm::NmThis() { return _this; }
AGmNode* AGm::NmWindow() { return _window; }
AGmNode* AGm::NmDocTmpl() { return _doc_tmpl; }
AGmNode* AGm::NmGlobal() { return _global; }
AGmNode* AGm::NmPen() { return _pen; }
AGmNode* AGm::NmBrush() { return _brush; }
AGmNode* AGm::NmFont() { return _font; }
AGmNode* AGm::NmPipe() { return _pipeStr; }
AGmNode* AGm::NmEvent() { return _event; }
AGmNode* AGm::NmO() { return _o; }
AGmNode* AGm::NmArg() { return _arg; }
AGmNode* AGm::NmProf() { return _prof; }
AGmNode* AGm::NmProLisp() { return _pro_lisp; }
AGmNode* AGm::NmProlog() { return _prolog; }
AGmNode* AGm::NmGame() { return _game; }

void AGm::SetToClipboard(AGmNode* data) {
	Ref(data);

	void *value;
	GetValue(NULL, AGM_VM_KEY_CUT_BUFFER, value);
	Unref((AGmNode*)value);
	SetValue(NULL, AGM_VM_KEY_CUT_BUFFER, data);
}
AGmNode *AGm::GetFromClipboard() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_CUT_BUFFER, value);
	return (AGmNode*)value;
}
void AGm::ReleaseClipboard() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_CUT_BUFFER, value);
	if (value) {
		RemoveValue(NULL, AGM_VM_KEY_CUT_BUFFER);
		Unref((AGmNode*)value);
	}
}

void AGm::SetToClipboard2(AGmNode* data) {
	Ref(data);

	void *value;
	GetValue(NULL, AGM_VM_KEY_CUT_BUFFER2, value);
	Unref((AGmNode*)value);
	SetValue(NULL, AGM_VM_KEY_CUT_BUFFER2, data);
}
AGmNode *AGm::GetFromClipboard2() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_CUT_BUFFER2, value);
	return (AGmNode*)value;
}
void AGm::ReleaseClipboard2() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_CUT_BUFFER2, value);
	if (value) {
		RemoveValue(NULL, AGM_VM_KEY_CUT_BUFFER2);
		Unref((AGmNode*)value);
	}
}

