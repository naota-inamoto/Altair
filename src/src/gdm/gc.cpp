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
#include <gdm/matrix.h>
#include <gdm/file.h>
#include <gdm/debug.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/advwnd.h>
#include <dev/shell.h>
#include <dev/bitmap.h>
#include <dev/ocx.h>
#include <dev/mainfrm.h>
#include <cui/cui.h>

#include <sql/sql.h>
#include <net/socket.h>

#include <symbol/pro_lisp.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#if defined(_DEBUG) | defined(_LINUX_DEBUG)
//#define	_DEBUG_GC
//#define	DEBUG_LEAK_10
//#define	DEBUG_LEAK_20
#endif

#if	_WINDOWS && _64BIT
void AGm::GC(long long *num_new_free, long long *num_free, long long *num_use, int debug) {
#else
void AGm::GC(long *num_new_free, long *num_free, long *num_use, int debug) {
#endif
	// mark
	GCmark();
	// scan
	_GCscan(1, NULL, NULL, num_new_free, num_free, num_use, debug);
}

#if	_WINDOWS && _64BIT
void AGm::GCcyclicOnly(long long*num_new_free, long long*num_free, long long *num_use, int debug) {
#else
void AGm::GCcyclicOnly(long *num_new_free, long *num_free, long *num_use, int debug) {
#endif
	// mark
	GCmark();
	// mark zero-ref-count cell
	_GCscan(4, NULL, NULL, NULL, NULL, NULL, 0);

	// scan
	_GCscan(1, NULL, NULL, num_new_free, num_free, num_use, debug);
}

#if	_WINDOWS && _64BIT
void AGm::GCtestRefCount(long long *ref_count_zero, long long *ref_count_minus, long long *num_new_free,
						 long long *num_free, long long *num_use, int debug) {
#else
void AGm::GCtestRefCount(long *ref_count_zero, long *ref_count_minus, long *num_new_free,
						 long *num_free, long *num_use, int debug) {
#endif
	// mark
	GCmark();
	// scan
	_GCscan(2, ref_count_zero, ref_count_minus, num_new_free, num_free, num_use, debug);
}


#if	_WINDOWS && _64BIT
void AGm::GCtestCyclic(long long *not_cyclic, long long *cyclic, long long *num_new_free,
						 long long *num_free, long long *num_use, int debug) {
#else
void AGm::GCtestCyclic(long *not_cyclic, long *cyclic, long *num_new_free,
						 long *num_free, long *num_use, int debug) {
#endif
	// mark
	GCmark();
	// scan
	_GCscan(3, not_cyclic, cyclic, num_new_free, num_free, num_use, debug);
}

void AGm::GCmark() {
	_GCmark(Root());
	_GCmark(GetFromClipboard());
	_GCmark(GetFromClipboard2());

	_GCmark(minus_one); _GCmark(zero);
	_GCmark(one); _GCmark(two); _GCmark(three);
	_GCmark(four); _GCmark(five); _GCmark(six);
	_GCmark(seven); _GCmark(eight); _GCmark(nine);
	_GCmark(ten);

	_GCmark(_u_str_tbl);
	_GCmark(_free_id_list);
	_GCmark(_values);
	_GCmark(ADvCmdLine);
	_GCmark(ADvFrames);

	ADvCWinApp *app = ADvGetApp();
	if (app && app->m_pMainWnd)
		((ADvCMainFrame*)app->m_pMainWnd)->GCmark();

	AUiProgramMgr *prgm = AUiProgramMgr::Instance();
	if (prgm) prgm->GCmark();
	AIpProcessMgr *pcsm = AIpProcessMgr::Instance();
	if (pcsm) pcsm->GCmark();

	ADvSocket::GCmark();

	AGmProLisp::GCmark();
	AGmProlog::GCmark();
}

void AGm::_GCmark(AGmNode *n) {
	while (n) {
		if (n->flag & AGM_FLAG_MARK) break;
		n->flag |= AGM_FLAG_MARK;
		if (n->body) {
			n->body->flag |= AGM_FLAG_MARK;
			if (n->body->in) {
				n->body->in->flag |= AGM_FLAG_MARK;
			}
			if (n->body->out) {
				n->body->out->flag |= AGM_FLAG_MARK;
			}
		}
		AGmNode *t;
		switch (n->type) {
		case	AGM_NODE_INTEGER:
		case	AGM_NODE_STRING:
		case	AGM_NODE_MATRIX:
		case	AGM_NODE_BITMAP:
		case	AGM_NODE_PTR:
		case	AGM_NODE_BINARY:
			t = n->a.i.t;	break;
		case	AGM_NODE_REAL:
		case	AGM_NODE_BIGINT:
		case	AGM_NODE_GFP:
		case	AGM_NODE_OCX:
		case	AGM_NODE_VARIANT:
			t = NULL;		break;
		case	AGM_NODE_SQL_CONN:
			if (n->a.conn.impl)
				n->a.conn.impl->GCmark();
			t = NULL;		break;
		case	AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case	AGM_NODE_LIST:
			_GCmark(n->a.l.h);
			t = n->a.l.t;
			break;
		case	AGM_NODE_ARRAY:
			long i, size;
			size =(long) n->a.a.size;
			for (i = 0; i < size; i++)
				_GCmark(n->a.a.v[i]);
			t = NULL;
			break;
		case	AGM_NODE_OBJECT:
			_GCmark(n->a.o.class_name);
			_GCmark(n->a.o.class_id);
			t = NULL;
			break;
		case	AGM_NODE_DST_ITR:
		case	AGM_NODE_SRC_ITR:
			_GCmark(n->a.itr.node);
			t = NULL;
			break;
		case	AGM_NODE_FILE:
			if (n->a.f.v)	
				n->a.f.v->GCmark();
			t = n->a.i.t;	break;
		case	AGM_NODE_WINDOW:
			if (n->a.w.v)
				n->a.w.v->GCmark();
			t = n->a.w.t; break;
		default:
			t = NULL; break;
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			l->flag |= AGM_FLAG_MARK;
			AGmArc *a = l->arc;
			a->flag |= AGM_FLAG_MARK;
			AGmLink *il;
			for (il = a->dst->body->in->next;; il = il->next) {
				ASSERT(il != a->dst->body->in);
				if (il->arc == a) break;
			}
			il->flag |= AGM_FLAG_MARK;
			_GCmark(a->attr);
			_GCmark(a->dst);
		}
		n = t;
	}
}

void AGm::_GCmark2(AGmNode *n, int flag) {
	while (n) {
		if (n->flag & flag) {
			if (flag == AGM_FLAG_REF) {
				_GCmark2(n, AGM_FLAG_CYCLIC);
			}
			break;
		}
		n->flag |= flag;
		if (n->body) {
			n->body->flag |= flag;
			if (n->body->in) {
				n->body->in->flag |= flag;
			}
			if (n->body->out) {
				n->body->out->flag |= flag;
			}
		}
		AGmNode *t;
		switch (n->type) {
		case	AGM_NODE_INTEGER:
		case	AGM_NODE_STRING:
		case	AGM_NODE_MATRIX:
		case	AGM_NODE_BITMAP:
		case	AGM_NODE_PTR:
		case	AGM_NODE_BINARY:
			t = n->a.i.t;	break;
		case	AGM_NODE_REAL:
		case	AGM_NODE_BIGINT:
		case	AGM_NODE_GFP:
		case	AGM_NODE_OCX:
		case	AGM_NODE_VARIANT:
			t = NULL;		break;
		case	AGM_NODE_SQL_CONN:
		case	AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case	AGM_NODE_LIST:
			_GCmark2(n->a.l.h, flag);
			t = n->a.l.t;
			break;
		case	AGM_NODE_ARRAY:
			long i, size;
			size = (long)n->a.a.size;
			for (i = 0; i < size; i++)
				_GCmark2(n->a.a.v[i], flag);
			t = NULL;
			break;
		case	AGM_NODE_OBJECT:
			_GCmark2(n->a.o.class_name, flag);
			_GCmark2(n->a.o.class_id, flag);
			t = NULL;
			break;
		case	AGM_NODE_DST_ITR:
		case	AGM_NODE_SRC_ITR:
			_GCmark2(n->a.itr.node, flag);
			t = NULL;
			break;
		case	AGM_NODE_FILE:
			t = n->a.i.t;	break;
		case	AGM_NODE_WINDOW:
			t = n->a.w.t; break;
		default:
			t = NULL; break;
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			l->flag |= flag;
			AGmArc *a = l->arc;
			a->flag |= flag;
			AGmLink *il;
			for (il = a->dst->body->in->next;; il = il->next) {
				ASSERT(il != a->dst->body->in);
				if (il->arc == a) break;
			}
			il->flag |= flag;
			_GCmark2(a->attr, flag);
			_GCmark2(a->dst, flag);
		}
		n = t;
	}
}

#if	_WINDOWS && _64BIT
void AGm::_GCscan(int cmd, long long *a1, long long *a2, long long *num_new_free, long long *num_free, long long *num_use, int debug) {
#else
void AGm::_GCscan(int cmd, long *a1, long *a2, long *num_new_free, long *num_free, long *num_use, int debug) {
#endif
	if (a1) *a1 = 0;
	if (a2) *a2 = 0;
	if (num_new_free) *num_new_free = 0;
	if (num_free) *num_free = 0;
	if (num_use) *num_use = 0;

	if (_mem_block_chain == NULL) return;

	unsigned short i;
	AGmNode *block;
	for (i = 1; i < _mem_index; i++) {
		AGmNode *cell = (AGmNode*)&_mem_block_chain[i];
		// cell in use
		if ((cell->flag & AGM_FLAG_MARK) != 0) {
			if (cmd == 1 || cmd == 2) {
				cell->flag &= ~AGM_FLAG_MARK;
				if (num_use) (*num_use)++;
			}
			continue;
		}
		// cell in free area
		if ((cell->flag & AGM_FLAG_FREE_CELL) != 0) {
			if (cmd == 1 || cmd == 2) {
				if (num_free) (*num_free)++;
			}
			continue;
		}
		// cell to be recycled
		if (cmd == 1 || cmd == 2) {
			if (num_new_free) (*num_new_free)++;
			if (debug) TRACE2(AL_STR("recycle = 0x%lx, type = %d\n"), AL_PTR(cell), ((AGmNode*)cell)->type);
		}
		if (cmd == 1) {
			_Recycle(cell);
		} else if (cmd == 2) {
			if (AGM_NODE_TYPE_BEGIN <= cell->type && cell->type <= AGM_NODE_TYPE_END) {
				if (cell->count == 0 && a1)
					(*a1)++;
				if (cell->count < 0 && a2)
					(*a2)++;
#ifdef DEBUG_LEAK_10
if (cell->count == 0)
	DumpList("ref_cnt_zero: ", cell, 10, (AGmFile*)NULL, 1);
#endif
			}
#if	_DEBUG || _LINUX_DEBUG
DumpKindOut(cell);
#endif
		} else if (cmd == 3) {
			if (AGM_NODE_TYPE_BEGIN <= cell->type && cell->type <= AGM_NODE_TYPE_END) {
				_GCmark2(cell, AGM_FLAG_REF);
			}
		} else if (cmd == 4) {
			if (AGM_NODE_TYPE_BEGIN <= cell->type && cell->type <= AGM_NODE_TYPE_END) {
				if (cell->count == 0) {
					_GCmark(cell);
				}
			}
		}
	}

	for (block = ((AGmNode*)_mem_block_chain)->a.l.t;
			block; block = block->a.l.t) {
		for (i = 1; i < AGM_MEM_BLOCK_SIZE; i++) {
			AGmNode *cell = &block[i];
			// cell in use
			if ((cell->flag & AGM_FLAG_MARK) != 0) {
				if (cmd == 1 || cmd == 2) {
					cell->flag &= ~AGM_FLAG_MARK;
					if (num_use) (*num_use)++;
				}
				continue;
			}
			// cell in free area
			if ((cell->flag & AGM_FLAG_FREE_CELL) != 0) {
				if (cmd == 1 || cmd == 2) {
					if (num_free) (*num_free)++;
				}
				continue;
			}
			// cell to be recycled
			if (cmd == 1 || cmd == 2) {
				if (num_new_free) (*num_new_free)++;
				if (debug) TRACE2(AL_STR("recycle = 0x%lx, type = %d\n"), AL_PTR(cell), ((AGmNode*)cell)->type);
			}
			if (cmd == 1) {
				_Recycle(cell);
			} else if (cmd == 2) {
				if (AGM_NODE_TYPE_BEGIN <= cell->type && cell->type <= AGM_NODE_TYPE_END) {
					if (cell->count == 0 && a1)
						(*a1)++;
					if (cell->count < 0 && a2)
						(*a2)++;
#ifdef DEBUG_LEAK_10
if (cell->count == 0)
	DumpList("ref_cnt_zero: ", cell, 10, (AGmFile*)NULL, 1);
#endif
				}
			} else if (cmd == 3) {
				if (AGM_NODE_TYPE_BEGIN <= cell->type && cell->type <= AGM_NODE_TYPE_END) {
					_GCmark2(cell, AGM_FLAG_REF);
				}
			} else if (cmd == 4) {
				if (AGM_NODE_TYPE_BEGIN <= cell->type && cell->type <= AGM_NODE_TYPE_END) {
					if (cell->count == 0) {
						_GCmark(cell);
					}
				}
			}
		}
	}

	if (cmd == 3) {
		for (i = 1; i < _mem_index; i++) {
			AGmNode *cell = (AGmNode*)&_mem_block_chain[i];
			// cell in use
			if ((cell->flag & AGM_FLAG_MARK) != 0) {
				cell->flag &= ~AGM_FLAG_MARK;
				if (num_use) (*num_use)++;
				continue;
			}
			// cell in free area
			if ((cell->flag & AGM_FLAG_FREE_CELL) != 0) {
				if (num_free) (*num_free)++;
				continue;
			}
			// cell to be recycled
			if (num_new_free) (*num_new_free)++;
			if (debug) TRACE2(AL_STR("recycle = 0x%lx, type = %d\n"), AL_PTR(cell), ((AGmNode*)cell)->type);
			if (AGM_NODE_TYPE_BEGIN <= cell->type && cell->type <= AGM_NODE_TYPE_END) {
				if ((cell->flag & AGM_FLAG_CYCLIC) != 0) {
					if (a2) (*a2)++;
				} else {
					if (a1) (*a1)++;
				}
#ifdef DEBUG_LEAK_20
if ((cell->flag & AGM_FLAG_CYCLIC) != 0) {
	DumpList("cyclic: ", cell, 10, (AGmFile*)NULL, 1);
} else {
	//DumpList("garbage: ", cell, 10, (AGmFile*)NULL, 1);
	DumpGraph("garbage: ", cell, 10, (AGmFile*)NULL, 1);
}
#endif
			}
			cell->flag &= ~ (AGM_FLAG_REF | AGM_FLAG_CYCLIC);
		}

		for (block = ((AGmNode*)_mem_block_chain)->a.l.t;
				block; block = block->a.l.t) {
			for (i = 1; i < AGM_MEM_BLOCK_SIZE; i++) {
				AGmNode *cell = &block[i];
				// cell in use
				if ((cell->flag & AGM_FLAG_MARK) != 0) {
					cell->flag &= ~AGM_FLAG_MARK;
					if (num_use) (*num_use)++;
					continue;
				}
				// cell in free area
				if ((cell->flag & AGM_FLAG_FREE_CELL) != 0) {
					if (num_free) (*num_free)++;
					continue;
				}
				// cell to be recycled
				if (num_new_free) (*num_new_free)++;
				if (debug) TRACE2(AL_STR("recycle = 0x%lx, type = %d\n"), AL_PTR(cell), ((AGmNode*)cell)->type);
				if (AGM_NODE_TYPE_BEGIN <= cell->type && cell->type <= AGM_NODE_TYPE_END) {
					if ((cell->flag & AGM_FLAG_CYCLIC) != 0) {
						if (a2) (*a2)++;
					} else {
						if (a1) (*a1)++;
					}
#ifdef DEBUG_LEAK_20
if ((cell->flag & AGM_FLAG_CYCLIC) != 0) {
	DumpList("cyclic: ", cell, 10, (AGmFile*)NULL, 1);
} else {
	DumpList("garbage: ", cell, 10, (AGmFile*)NULL, 1);
}
#endif
				}
				cell->flag &= ~ (AGM_FLAG_REF | AGM_FLAG_CYCLIC);
			}
		}
	}
}

void AGm::_Recycle(AGmNode *n) {
#ifdef	_DEBUG_GC
	if (ADvExiting == 0) {
		TRACE2("(this, type) = (%x, %d): ", (long)n, (long)n->type);
		DumpKind(n);
	}
#endif
	switch (n->type) {
	case AGM_NODE_INTEGER:
		if (n->a.i.t) n->a.i.t->count--;
		break;
	case AGM_NODE_REAL:
		break;
	case AGM_NODE_LIST:
		if (n->a.l.h) n->a.l.h->count--;
		if (n->a.l.t) n->a.l.t->count--;
		break;
	case AGM_NODE_ARRAY:
		long i, size;
		size = (long)n->a.a.size;
		for (i = 0; i < size; i++)
			if (n->a.a.v[i]) n->a.a.v[i]->count--;
		break;
	case AGM_NODE_OBJECT:
		if (n->a.o.class_name) n->a.o.class_name->count--;
		if (n->a.o.class_id) n->a.o.class_id->count--;
		break;
	case AGM_NODE_DST_ITR:
	case AGM_NODE_SRC_ITR:
		if (n->a.itr.node) n->a.itr.node->count--;
		if (n->a.itr.link) n->a.itr.link->count--;
		break;
	case AGM_NODE_STRING:
		AGmString *s;
		s = n->a.s.v; if (s) delete s;
		if (n->a.s.t) n->a.s.t->count--;
		break;
	case AGM_NODE_MATRIX:
		AGmMatrix *m;
		m = n->a.m.v; if (m) delete m;
		if (n->a.m.t) n->a.m.t->count--;
		break;
	case AGM_NODE_FILE:
		AGmFile *f;
		f = n->a.f.v; if (f) delete f;
		if (n->a.f.t) n->a.f.t->count--;
		break;
	case AGM_NODE_WINDOW:
		ADvWindow *w;
		w = n->a.w.v; if (w) delete w;
		if (n->a.w.t) n->a.w.t->count--;
		break;
	case AGM_NODE_BITMAP:
		ADvBitmap *b;
		b = n->a.b.v; if (b) delete b;
		if (n->a.b.t) n->a.b.t->count--;
		break;
	case AGM_NODE_PTR:
		if (n->a.p.t) n->a.p.t->count--;
		break;
	case AGM_NODE_BINARY:
		AGmBinary *bin;
		bin = n->a.bin.v; if (bin) delete bin;
		if (n->a.bin.t) n->a.bin.t->count--;
		break;
	case AGM_NODE_BIGINT:
		bin = n->a.bi.bin; if (bin) delete bin;
		break;
	case AGM_NODE_OCX:
		ADvOcx *o;
		o = n->a.ocx.v; if (o) delete o;;
		break;
	case AGM_NODE_VARIANT:
		COleVariant *v;
		v = n->a.var.v; if (v) delete v;;
		break;
	case AGM_NODE_SQL_CONN:
		if (n->a.conn.impl) {
			delete n->a.conn.impl;
			n->a.conn.impl = NULL;
		}
		break;
	case AGM_NODE_GFP:
	case AGM_NODE_SQL_STAT:
		break;
	case AGM_BODY_OF_NODE:
		break;
	case AGM_DMY_OUT_LINK:
	case AGM_DMY_IN_LINK:
	case AGM_OUT_LINK:
	case AGM_IN_LINK:
		AGmLink *l;
		l = (AGmLink*)n;
		AGM_REMOVE(l);
		break;
	case AGM_ARC:
		AGmArc *a;
		a = (AGmArc*)n;
		a->dst->count--;
		if (a->attr) a->attr->count--;
		break;
	default:
		ASSERT(0);
		break;
	}
	Free((AGmCell*)n);
}
