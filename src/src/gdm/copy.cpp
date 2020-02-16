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
#include <gdm/pict.h>
#include <dev/advwnd.h>
#include <dev/bitmap.h>
#include <dev/ocx.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

long AGm::nid;
AGmNode** AGm::ctbl;

void AGm::AttachNid(AGmNode *n) {	
	while (n) {
		AGmNodeBody *nb = n->body;
		if (nb == NULL) {
			nb = n->body = (AGmNodeBody*)Alloc();
			nb->type = AGM_BODY_OF_NODE;
			nb->flag = 0;
			nb->nid = 0;
			nb ->in = nb->out = NULL;
		}
		if (nb->nid != 0) break;
		nb->nid = ++ nid;

		AGmNode *t = 0;
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
		case AGM_NODE_BIGINT:
		case AGM_NODE_GFP:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case AGM_NODE_LIST:
			AttachNid(n->a.l.h);
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
				AttachNid(n->a.a.v[i]);
			t = NULL;
			break;
		default:	ASSERT(0);
		}
		AGmLink *l, *outlink = n->body->out;
		if (outlink) {
			for (l = outlink->next; l != outlink; l = l->next) {
				ASSERT(l);
				if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
					AttachNid(l->arc->attr);
					AttachNid(l->arc->dst);
				} else {
					if ((l->arc->dst->flag & AGM_FLAG_SELF) != 0) {
						AttachNid(l->arc->attr);
					}
				}
			}
		}
		n = t;
	}
}

void AGm::DetachNid(AGmNode *n) {
	while (n) {
		AGmNodeBody *nb = n->body;
		if (nb == NULL) break;
		if (nb->in == NULL && nb->out == NULL) {
			Free((AGmCell*)nb);
			n->body = NULL;
		} else {
			if (nb->nid == 0) break;
			nb->nid = 0;
		}

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
		case AGM_NODE_BIGINT:
		case AGM_NODE_GFP:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case AGM_NODE_LIST:
			DetachNid(n->a.l.h);
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
				DetachNid(n->a.a.v[i]);
			t = NULL;
			break;
		default:	ASSERT(0);
		}
		if (n->body) {
			AGmLink *l, *outlink = n->body->out;
			if (outlink) {
				for (l = outlink->next; l != outlink; l = l->next) {
					ASSERT(l);
					if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
						DetachNid(l->arc->attr);
						DetachNid(l->arc->dst);
					} else {
						if ((l->arc->dst->flag & AGM_FLAG_SELF) != 0) {
							DetachNid(l->arc->attr);
						}
					}
				}
			}
		}
		n = t;
	}
}

AGmNode* AGm::GraphCopy(AGmNode *n, int us) {
	nid = 0;
	MarkNode(n, AGM_FLAG_SELF);
	AttachNid(n);

	ctbl = new AGmNode*[nid];

	nid = 0;
	_GraphCopyNode(n, us);
	nid = 0;
	_GraphCopyArc(n);
	AGmNode *n1 = ctbl[n->body->nid-1];

	delete ctbl;

	DetachNid(n);
	UnmarkNode(n, AGM_FLAG_SELF);
	return n1;
}

void AGm::_GraphCopyNode(AGmNode *n, int us) {
	while (n) {
		if (n->body->nid != nid+1) break;
		nid++;
		AGmNode *t = NULL, *itr; AGmLink *itr_link;
		switch (n->type) {
		case AGM_NODE_INTEGER:
			if (us == 0) {
				ctbl[n->body->nid-1] = Integer(n->a.i.v);
			} else {
				ctbl[n->body->nid-1] = n;
			}
			t = n->a.i.t;
			break;
		case AGM_NODE_REAL:
			ctbl[n->body->nid-1] = Real(n->a.r.v);
			t = NULL;
			break;
		case AGM_NODE_LIST:
			ctbl[n->body->nid-1] = Cons(NULL);
			_GraphCopyNode(n->a.l.h, us);
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			ctbl[n->body->nid-1] = Array(size);
			for (i = 0; i < size; i++) {
				_GraphCopyNode(n->a.a.v[i], us);
			}
			t = NULL;
			break;
		case AGM_NODE_OBJECT:
			ctbl[n->body->nid-1] = Object(n->a.o.class_name, n->a.o.class_id);
			t = NULL;
			break;
		case AGM_NODE_DST_ITR:
			itr = DstItr(n->a.itr.node);
			ctbl[n->body->nid-1] = itr;

			itr_link = n->a.itr.link;
			if (itr_link) itr_link->count++;
			itr->a.itr.link = itr_link;

			t = NULL;
			break;
		case AGM_NODE_SRC_ITR:
			itr = SrcItr(n->a.itr.node);
			ctbl[n->body->nid-1] = itr;

			itr_link = n->a.itr.link;
			if (itr_link) itr_link->count++;
			itr->a.itr.link = itr_link;

			t = NULL;
			break;
		case AGM_NODE_STRING:
			if (us == 0) {
				AGmString *s;
				s = n->a.s.v;
				ctbl[n->body->nid-1] = String(s ? s->Str() : NULL);
			} else {
				ctbl[n->body->nid-1] = n;
			}
			t = n->a.s.t;
			break;
		case AGM_NODE_MATRIX:
			AGmMatrix *m;
			m = n->a.m.v;
			ctbl[n->body->nid-1] = Matrix(m ? m->Copy() : NULL);
			t = n->a.m.t;
			break;
		case AGM_NODE_FILE:
			ctbl[n->body->nid-1] = File(NULL);
			t = n->a.f.t;
			break;
		case AGM_NODE_WINDOW:
			ctbl[n->body->nid-1] = Window(NULL);
			t = n->a.w.t;
			break;
		case AGM_NODE_BITMAP:
			ADvBitmap *b;
			b = n->a.b.v;
			ctbl[n->body->nid-1] = Bitmap(b ? b->Copy() : NULL);
			t = n->a.b.t;
			break;
		case AGM_NODE_PTR:
			ctbl[n->body->nid-1] = PtrCell(n->a.p.ptr);
			t = n->a.p.t;
			break;
		case AGM_NODE_BINARY:
			AGmBinary *bin;
			bin = n->a.bin.v;
			ctbl[n->body->nid-1] = Binary(bin ? bin->Copy() : NULL);
			t = n->a.bin.t;
			break;
		case AGM_NODE_BIGINT:
			bin = n->a.bi.bin;
			ctbl[n->body->nid-1] = BigInt((long)n->a.bi.sign, bin ? bin->Copy() : NULL);
			t = NULL;
			break;
		case AGM_NODE_GFP:
			ctbl[n->body->nid-1] = GFp(n->a.gp.v, n->a.gp.mod);
			break;
		case AGM_NODE_OCX:
			ctbl[n->body->nid-1] = Ocx(
				n->a.ocx.v ? new ADvOcx(*n->a.ocx.v) : NULL);
			t = NULL;
			break;
		case AGM_NODE_VARIANT:
			ctbl[n->body->nid-1] = Variant(
				(n->a.var.v ? new COleVariant(*n->a.var.v) : NULL),
				n->a.var.byref);
			t = NULL;
			break;
		case AGM_NODE_SQL_CONN:
			ctbl[n->body->nid-1] = Integer(0);
			ctbl[n->body->nid-1]->a.conn.impl = NULL;
			break;
		case AGM_NODE_SQL_STAT:
			ctbl[n->body->nid-1] = Integer(0);
			ctbl[n->body->nid-1]->a.stat.impl = NULL;
			break;
		default:	ASSERT(0);
		}
		{
			AGmLink *l;
			for (l = next_outlink(n, NULL);
					l; l = next_outlink(n, l)) {
				if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
					_GraphCopyNode(l->arc->attr, us);
					_GraphCopyNode(l->arc->dst, us);
				} else {
					if ((l->arc->dst->flag & AGM_FLAG_SELF) != 0) {
						_GraphCopyNode(l->arc->attr, us);
					}
				}
			}
		}
		n = t;
	}
}

void AGm::_GraphCopyArc(AGmNode *n) {
	while (n) {
		if (n->body->nid != nid+1) break;
		nid++;
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
			if (n->a.i.t)
				Set(ctbl[n->body->nid-1]->a.i.t, ctbl[n->a.i.t->body->nid-1]);
			t = n->a.i.t;
			break;
		case AGM_NODE_REAL:
		case AGM_NODE_BIGINT:
		case AGM_NODE_GFP:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;
			break;
		case AGM_NODE_LIST:
			if (n->a.l.h)
				Set(ctbl[n->body->nid-1]->a.l.h, ctbl[n->a.l.h->body->nid-1]);
			if (n->a.l.t)
				Set(ctbl[n->body->nid-1]->a.l.t, ctbl[n->a.l.t->body->nid-1]);
			_GraphCopyArc(n->a.l.h);
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++) {
				if (n->a.a.v[i])
					Set(ctbl[n->body->nid-1]->a.a.v[i], ctbl[n->a.a.v[i]->body->nid-1]);
				_GraphCopyArc(n->a.a.v[i]);
			}
			t = NULL;
			break;
		default:	ASSERT(0);
		}
		{
			AGmLink *l;
			for (l = next_outlink(n, NULL);
					l; l = next_outlink(n, l)) {
				if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
					CreateArc(ctbl[n->body->nid-1], ctbl[l->arc->dst->body->nid-1],
						(l->arc->attr ? ctbl[l->arc->attr->body->nid-1] : NULL));
					_GraphCopyArc(l->arc->attr);
					_GraphCopyArc(l->arc->dst);
				} else {
					if ((l->arc->dst->flag & AGM_FLAG_SELF) != 0) {
						AGmArc *a = CreateArc(ctbl[n->body->nid-1], ctbl[l->arc->dst->body->nid-1],
							(l->arc->attr ? ctbl[l->arc->attr->body->nid-1] : NULL));
						a->incl_assoc |= AGM_ARC_ASSOC;
						_GraphCopyArc(l->arc->attr);
					}
				}
			}
		}
		n = t;
	}
}

