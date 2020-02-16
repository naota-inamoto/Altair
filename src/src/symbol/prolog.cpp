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

#include <symbol/pro_lisp.h>

#include <altair/alcrt.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#define GVAR p->env->TH

int AGmProlog::debug_pred;
int AGmProlog::debug_dump;
int AGmProlog::debug_syn;
int AGmProlog::debug_syn2;
int AGmProlog::debug_syn3;
AGmNode* AGmProlog::debug_syntax_id;
AGmNode* AGmProlog::debug_syntax_id2;
AGmNode* AGmProlog::debug_syntax_id3;

int AGmProlog::max_call_count = 1024;
AGmProlog::AGmProlog(AGmProLisp *p) {
	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmProlog());
	if (n == NULL) {
		CreateArc(root, n = Cons(NULL), NmProlog());
	}
	CreateArc(n, PtrCell(this));

	this->p = p;
	id0 = grmr0 = ptree0 = NULL;
	idxs2 = NULL;

	pred = Ref(Cons(NULL));
	manip = Ref(Cons(NULL));
	tree_conv = 0;

	for_stack = Ref(Cons(NULL));
	itr = NULL;

	gen_status = Ref(Cons(NULL));

	call_count = 0;
	pred_stack = NULL;

	exec_stack = NULL;
	PushExecStack(new AGmExecStack(OP_DUMMY));
}
AGmProlog::~AGmProlog() {
	Unref(pred, manip);
	Unref(for_stack, itr);
	Unref(gen_status);
	Unref(pred_stack);

	FreeExecStack();

	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmProlog());
	for (AGmLink *l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		if (l->arc->dst->a.p.ptr == this) {
			DeleteArc(l->arc); break;
		}
	}
}

AGmNode *AGmProlog::PredManip() {
	return List(pred, manip);
}
void AGmProlog::PredManip(AGmNode* pm) {
	if (pm && IsList(pm) && pm->H && pm->T && IsList(pm->T) && pm->TH) {
		Set(pred, pm->H); Set(manip, pm->TH);
	}
	RefUnref(pm);
}
void AGmProlog::Clear() {
	Set(pred, Cons(NULL));
	Set(manip, Cons(NULL));

	Set(for_stack, Cons(NULL));

	call_count = 0;

	FreeExecStack();
	PushExecStack(new AGmExecStack(OP_DUMMY));
}
void AGmProlog::Back() {
	call_count = 0;

	while (exec_stack) {
		AGmExecStack *next = exec_stack->next;
		DeleteArcs(exec_stack->arcs);
		delete exec_stack;
		exec_stack = next;
	}
	PushExecStack(new AGmExecStack(OP_DUMMY));
}
void AGmProlog::TreeConv(AGmNode* id0, AGmNode* grmr0) {
	this->id0 = id0;
	this->grmr0 = grmr0;
	tree_conv = 1;
}

void AGmProlog::MaxCallCount(int max_count) {
	max_call_count = max_count;
}
int AGmProlog::MaxCallCount() {
	return max_call_count;
}
void AGmProlog::CallCount(int count) {
	call_count = count;
}
int AGmProlog::CallCount() {
	return call_count;
}

void AGmProlog::GCmark() {
	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmProlog());
	if (n == NULL) return;
	for (AGmLink *l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		AGmProlog* p = (AGmProlog*)l->arc->dst->a.p.ptr;
		p->GC_mark();
	}
}
void AGmProlog::GC_mark() {
	_GCmark(pred);
	_GCmark(manip);
	_GCmark(for_stack);
	_GCmark(itr);
	_GCmark(gen_status);
	_GCmark(pred_stack);

	_GCmark(idxs2);
}

AGmNode* AGmProlog::ToInternal(AGmNode* tree) {
	if (tree->T == NULL) return NULL;
	for (tree = tree->THT; tree; tree = tree->T) {
		AGmNode* g = Cons(NULL);
		AGmNode* obj_db = Cons(NULL);
		AGmNode* syn_db = Cons(NULL);
		if (tree->HH == p->rule) {
			AGmNode *head = ToInternalRule(g, tree->H, obj_db, syn_db);
			if (IsString(head)) { RefUnref(g, obj_db, syn_db); return head; }
			for (AGmLink* l = next_outlink(g, NULL); l; l = next_outlink(g, l)) {
				if (l->arc->dst != head) continue;
				DeleteArc(l->arc); break;
			}
		} else if (tree->HH == p->query) {
			AGmNode *err = ToInternalQuery(g, tree->H, obj_db, syn_db);
			if (err) { RefUnref(g, obj_db, syn_db); return err; }
		} else {
			return String("syntax id is not rule/query.");
		}
		RefUnref(obj_db, syn_db);
	}
	return NULL;
}

AGmNode* AGmProlog::ToInternalRule(AGmNode* g, AGmNode* tree, AGmNode* obj_db, AGmNode* syn_db) {
	if (tree->THH != p->pred_goal) return String("head must be pred.");
	AGmNode* head = ToInternalPred(g, tree->THT, obj_db, syn_db); // head
	if (IsString(head)) return head;
	AGmNode *body = ToInternalGoals(g, tree->TTHT, obj_db, syn_db); // body
	if (IsString(body)) return body;
	AGmNode *n = dst_node2(pred, head->TH);
	if (n == NULL) CreateArc(pred, n = Cons(NULL), head->TH);
	CreateArc(n, List(head, g, obj_db));
	return head;
}
AGmNode* AGmProlog::ToInternalQuery(AGmNode* g, AGmNode* tree, AGmNode* obj_db, AGmNode* syn_db) {
	AGmNode *body = ToInternalGoals(g, tree->T, obj_db, syn_db);
	if (IsString(body)) return body;
	CreateArc(manip, g);
	return NULL;
}

AGmNode* AGmProlog::ToInternalGoals(AGmNode* g, AGmNode* body, AGmNode* obj_db, AGmNode* syn_db) {
	AGmNode *err;
	if (body == NULL) return g;
	body = body->H;
	if (body->H == p->goals) {
		if (body->THH == p->pred_goal) {
			err = ToInternalPred(g, body->THT, obj_db, syn_db);
		} else if (body->THH == p->pred_not) {
			err = ToInternalNotPred(g, body->THT, obj_db, syn_db);
		} else if (body->THH == p->next_if_false) {
			err = ToInternalNextIfFalse(g, body->THT, obj_db, syn_db);
		} else if (body->THH == p->pm_rep) {
			err = ToInternalPatturnMatchAndReplace(g, body->THT, obj_db, syn_db);
		} else {
			err = ToInternalFunc(g, body->THT, obj_db, syn_db);
		}
		if (IsString(err)) return err;
		for (AGmNode *ls = body->TTHT; ls; ls = ls->T) {
			if (ls->HTHH == p->pred_goal) {
				err = ToInternalPred(g, ls->HTHT, obj_db, syn_db);
			} else if (ls->HTHH == p->pred_not) {
				err = ToInternalNotPred(g, ls->HTHT, obj_db, syn_db);
			} else if (ls->HTHH == p->next_if_false) {
				err = ToInternalNextIfFalse(g, ls->HTHT, obj_db, syn_db);
			} else if (ls->HTHH == p->pm_rep) {
				err = ToInternalPatturnMatchAndReplace(g, ls->HTHT, obj_db, syn_db);
			} else {
				err = ToInternalFunc(g, ls->HTHT, obj_db, syn_db);
			}
			if (IsString(err)) return err;
		}
	} else {
		if (body->H == p->pred_goal) {
			err = ToInternalPred(g, body->T, obj_db, syn_db);
		} else if (body->H == p->pred_not) {
			err = ToInternalNotPred(g, body->T, obj_db, syn_db);
		} else if (body->H == p->next_if_false) {
			err = ToInternalNextIfFalse(g, body->T, obj_db, syn_db);
		} else if (body->H == p->pm_rep) {
			err = ToInternalPatturnMatchAndReplace(g, body->T, obj_db, syn_db);
		} else {
			err = ToInternalFunc(g, body->T, obj_db, syn_db);
		}
		if (IsString(err)) return err;
	}
	return g;
}

AGmNode* AGmProlog::ToInternalPred(AGmNode* g, AGmNode* pred, AGmNode* obj_db, AGmNode* syn_db) {
	AGmNode *rel, *obj; long idx = 1;
	AGmNode *cut = pred->HT;
	if (cut == NULL) {
		rel = List(Integer(PRO_PRED), pred->TH, NULL);
	} else {
		rel = List(Integer(PRO_PRED), pred->TH, one);
	}
	CreateArc(g, rel);
	obj = ToInternalArg(g, pred->TTH, obj_db, syn_db);
	if (IsString(obj)) return obj;
	CreateArc(rel, obj, Integer(idx++));
	for (AGmNode *ls = pred->TTTH->T; ls; ls = ls->T) {
		obj = ToInternalArg(g, ls->HTH, obj_db, syn_db);
		if (IsString(obj)) return obj;
		CreateArc(rel, obj, Integer(idx++));
	}
	return rel;
}
AGmNode* AGmProlog::ToInternalNotPred(AGmNode* g, AGmNode* body, AGmNode* obj_db, AGmNode* syn_db) {
	AGmNode *rel1 = List(Integer(PRO_NOT_PRED), p->pred_not);
	CreateArc(g, rel1);
	AGmNode *err = ToInternalGoals(g, body, obj_db, syn_db);
	if (IsString(err)) return err;
	AGmNode *rel2 = List(Integer(PRO_NOT_PRED), p->pred_not);
	CreateArc(g, rel2);
	AGmLink *l1 = find_outlink(g, rel1);
	AGmLink *l2 = find_outlink(g, rel2);
	for (AGmLink *l = l1; l != l2; l = l->next) {
		l->arc->dst->HI |= PRO_NOT_PRED;
	}
	return rel1;
}
AGmNode* AGmProlog::ToInternalNextIfFalse(AGmNode* g, AGmNode* body, AGmNode* obj_db, AGmNode* syn_db) {
	AGmNode *rel1 = List(Integer(PRO_NEXT_IF_F), p->next_if_false);
	CreateArc(g, rel1);
	AGmNode *err = ToInternalGoals(g, body, obj_db, syn_db);
	if (IsString(err)) return err;
	AGmNode *rel2 = List(Integer(PRO_NEXT_IF_F), p->next_if_false);
	CreateArc(g, rel2);
	AGmLink *l1 = find_outlink(g, rel1);
	AGmLink *l2 = find_outlink(g, rel2);
	for (AGmLink *l = l1; l != l2; l = l->next) {
		l->arc->dst->HI |= PRO_NEXT_IF_F;
	}
	return rel1;
}

AGmNode* AGmProlog::ToInternalFunc(AGmNode* g, AGmNode* func, AGmNode* obj_db, AGmNode* syn_db) {
	AGmNode *rel = List(Integer(PRO_FUNC), func->THH);
	CreateArc(g, rel);
	AGmNode *obj;
	obj = ToInternalArg(g, func->H, obj_db, syn_db);
	if (IsString(obj)) return obj;
	CreateArc(rel, obj, one);
	grph = 0;
	obj = ToInternalArg(g, func->TTH, obj_db, syn_db);
	if (IsString(obj)) return obj;
	if (grph && func->THH == p->eq) Set(rel->TH, p->grph_eq);
	CreateArc(rel, obj, two);
	return rel;
}
AGmNode* AGmProlog::ToInternalArg(AGmNode* g, AGmNode* arg, AGmNode* obj_db, AGmNode* syn_db, int syn) {
	AGmNode *id = arg->H;
	if (syn == 0 && id == p->null) {
		return List(one, (AGmNode*)NULL);

	} else if ((syn == 0 && id == p->const_i) || (syn == 0 && id == p->const_s)) {
		return List(one, arg->TH);

	} else if (syn == 0 && id == p->wc_e) {
		return List((AGmNode*)NULL, p->wc_e);

	} else if (syn == 0 && id == p->var) {
		AGmNode *name = arg->TH;
		AGmNode *n = dst_node2(obj_db, name);
		if (n == NULL) {
			n = List((AGmNode*)NULL, (AGmNode*)NULL);
			set_dst_node(obj_db, name, n);
		}
		return n;

	} else if (syn == 0 && id == p->syn_gen) {
		AGmNode *name = p->syn_gen_str;
		AGmNode *n = dst_node2(obj_db, name);
		if (n == NULL) {
			AGmNode *rel;
			CreateArc(g, rel = List(Integer(PRO_SYN), p->syn_gen));
			CreateArc(rel, n = List((AGmNode*)NULL, (AGmNode*)NULL));
			set_dst_node(obj_db, name, n);
		}
		return n;

	} else if (syn == 0 && id == p->syn_tree) {
		AGmNode *name = p->syn_tree_str;
		AGmNode *n = dst_node2(obj_db, name);
		if (n == NULL) {
			AGmNode *rel;
			CreateArc(g, rel = List(Integer(PRO_SYN), p->syn_tree));
			CreateArc(rel, n = List((AGmNode*)NULL, (AGmNode*)NULL));
			set_dst_node(obj_db, name, n);
		}
		return n;

	} else if (syn == 0 && id == p->count) {
		AGmNode *name = p->count_str;
		AGmNode *n = dst_node2(obj_db, name);
		if (n == NULL) {
			AGmNode *rel;
			CreateArc(g, rel = List(Integer(PRO_FUNC), p->count));
			CreateArc(rel, n = List((AGmNode*)NULL, (AGmNode*)NULL));
			set_dst_node(obj_db, name, n);
		}
		return n;

	} else if (syn == 0 && id == p->syn_ret) {
		AGmNode *name = p->syn_ret_str;
		AGmNode *n = dst_node2(obj_db, name);
		if (n == NULL) {
			AGmNode *rel;
			CreateArc(g, rel = List(Integer(PRO_SYN), p->syn_ret));
			CreateArc(rel, n = List((AGmNode*)NULL, (AGmNode*)NULL));
			set_dst_node(obj_db, name, n);
		}
		return n;

	} else if (id == p->syn_var) {
		AGmNode *n, *idxs = CreateIdxs(arg);
		AGmNode *rel = dst_node2(syn_db, idxs);
		if (syn == 0) {
			if (rel == NULL) {
				CreateArc(g, rel = List(Integer(PRO_SYN), p->syn_var));
				CreateArc(rel, List(one, idxs, zero, zero, zero), one);
				CreateArc(rel, n = List((AGmNode*)NULL, (AGmNode*)NULL));
				set_dst_node(syn_db, idxs, rel);
			} else {
				n = dst_node2(rel, (AGmNode*)NULL);
				ASSERT(n);
			}
		} else {
			if (rel == NULL) {
				n = List(one, idxs, zero, zero, zero);
			} else {
				n = dst_node(rel, 1);
				ASSERT(n);
				switch (syn) {
				case 1: // if
					Set(n->TTH, one); break;
				case 2: // switch
					Set(n->TTTH, one); break;
				case 3: // for, inv_for
					Set(n->TTTTH, one); break;
				}
			}
		}
		RefUnref(idxs);
		return n;

	} else if ((syn == 0 && id == p->add_1) || (syn == 0 && id == p->mul_1)) {
		AGmNode *rel, *obj1, *obj2, *res;
		obj1 = ToInternalArg(g, arg->TH, obj_db, syn_db);
		if (IsString(obj1)) return obj1;
		for (AGmNode *ls = arg->TTHT; ls; ls = ls->T) {
			CreateArc(g, rel = List(Integer(PRO_FUNC), ls->HTHH));
			CreateArc(rel, obj1, one);
			obj2 = ToInternalArg(g, ls->HTTH, obj_db, syn_db);
			if (IsString(obj2)) return obj2;
			CreateArc(rel, obj2, two);
			CreateArc(rel, res = List((AGmNode*)NULL, (AGmNode*)NULL));
			obj1 = res;
		}
		return obj1;

	} else if (syn == 0 && id == p->neg) {
		AGmNode *rel, *obj, *res;
		CreateArc(g, rel = List(Integer(PRO_FUNC), p->neg));
		obj = ToInternalArg(g, arg->TH, obj_db, syn_db);
		if (IsString(obj)) return obj;
		CreateArc(rel, obj, one);
		CreateArc(rel, res = List((AGmNode*)NULL, (AGmNode*)NULL));
		return res;

	} else if (syn == 0 && id == p->braket) {
		return ToInternalArg(g, arg->TH, obj_db, syn_db);

	} else if (syn == 0 && id == p->ls_1) {
		AGmNode *n = p->ToListFromListExp2(arg);
		if (n != p->undefined) {
			return List(one, n);
		}

		AGmNode *rel, *obj, *res; long idx = 1;
		CreateArc(g, rel = List(Integer(PRO_FUNC), p->list));

		AGmNode *e1 = arg->TH;
		AGmNode *idid = e1->H;
		if (idid == p->wc_e) {
			obj = List((AGmNode*)NULL, (AGmNode*)NULL);
		} else if (idid == p->t || idid == p->wc_t) {
			return String("... appeard in list.head.");
		} else {
			obj = ToInternalArg(g, e1, obj_db, syn_db);
			if (IsString(obj)) return obj;
		}
		CreateArc(rel, obj, Integer(idx++));

		for (AGmNode *ls = arg->TTHT; ls; ls = ls->T) {
			AGmNode *en = ls->HTH;
			AGmNode *idid = en->H;
			if (idid == p->wc_e) {
				obj = List((AGmNode*)NULL, (AGmNode*)NULL);
			} else if (idid == p->t || idid == p->wc_t) {
				return String("... appeard in list.tail.");
			} else {
				obj = ToInternalArg(g, en, obj_db, syn_db);
				if (IsString(obj)) return obj;
			}
			CreateArc(rel, obj, Integer(idx++));
		}
		CreateArc(rel, res = List((AGmNode*)NULL, (AGmNode*)NULL));
		return res;

	} else if (syn == 0 && id == p->grph) {
		AGmNode *rel, *src = ToInternalArg(g, arg->TH, obj_db, syn_db);
		if (IsString(src)) return src;
		for (AGmNode *ls = arg->TTHT; ls; ls = ls->T) {
			AGmNode *cut = NULL;
			if (arg->HT) cut = one;
			CreateArc(g, rel = List(Integer(PRO_FUNC), p->grph));
			CreateArc(rel, src, one);
			AGmNode *attr = ToInternalArg(g, ls->H->TH, obj_db, syn_db);
			if (IsString(attr)) return attr;
			CreateArc(rel, attr, two);
			if (ls->HH == p->null_a) {
				Set(rel->TH, p->null_a);
			} else {
				AGmNode *dst = ToInternalArg(g, ls->H->TTH, obj_db, syn_db);
				if (IsString(dst)) return dst;
				CreateArc(rel, dst, three);
				if (dst->H) {
					AGmNode *dst2 = ShallowCopy(dst);
					Set(dst->H, NULL); Set(dst->TH, NULL);
					CreateArc(g, rel = List(Integer(PRO_FUNC), p->grph_eq));
					CreateArc(rel, dst2, one);
					CreateArc(rel, dst, two);
				}
			}
		}
		grph = 1;
		return src;

	} else if (syn == 0 && id == p->func_exp) {
		AGmNode *rel, *obj, *res; long idx = 1;
		AGmNode *a1 = arg->TTHT;
		if (arg->TH == p->if_str) {
			if (a1) a1 = a1->H;
			if (a1 == NULL || a1->H == p->arg_1) return String("num arg of if must be one.");
			rel = List(Integer(PRO_SYN), arg->TH);
			CreateArc(g, rel);
			obj = ToInternalArg(g, a1, obj_db, syn_db, 1);
			if (IsString(obj)) return obj;
			CreateArc(rel, obj, one);
		} else if (arg->TH == p->switch_str) {
			if (a1) a1 = a1->H;
			if (a1 == NULL || a1->H == p->arg_1) return String("num arg of switch must be one.");
			rel = List(Integer(PRO_SYN), arg->TH);
			CreateArc(g, rel);
			obj = ToInternalArg(g, a1, obj_db, syn_db, 2);
			if (IsString(obj)) return obj;
			CreateArc(rel, obj, one);
		} else if (arg->TH == p->for_str) {
			if (a1) a1 = a1->H;
			if (a1 == NULL || a1->H == p->arg_1) return String("num arg of for must be one.");
			rel = List(Integer(PRO_SYN), arg->TH);
			CreateArc(g, rel);
			obj = ToInternalArg(g, a1, obj_db, syn_db, 3);
			if (IsString(obj)) return obj;
			CreateArc(rel, obj, one);
		} else if (arg->TH == p->inv_for_str) {
			if (a1) a1 = a1->H;
			if (a1 == NULL || a1->H == p->arg_1) return String("num arg of inv_for must be one.");
			rel = List(Integer(PRO_SYN), arg->TH);
			CreateArc(g, rel);
			obj = ToInternalArg(g, a1, obj_db, syn_db, 3);
			if (IsString(obj)) return obj;
			CreateArc(rel, obj, one);
		} else {
			rel = List(Integer(PRO_FUNC), arg->TH);
			CreateArc(g, rel);
			if (a1) {
				a1 = a1->H;
				if (a1 && IsList(a1) && a1->H == p->arg_1) {
					obj = ToInternalArg(g, a1->TH, obj_db, syn_db);
					if (IsString(obj)) return obj;
					CreateArc(rel, obj, Integer(idx++));
					for (AGmNode *an = a1->TTHT; an; an = an->T) {
						obj = ToInternalArg(g, an->HTH, obj_db, syn_db);
						if (IsString(obj)) return obj;
						CreateArc(rel, obj, Integer(idx++));
					}
				} else {
					obj = ToInternalArg(g, a1, obj_db, syn_db);
					if (IsString(obj)) return obj;
					CreateArc(rel, obj, one);
				}
			}
		}
		CreateArc(rel, res = List((AGmNode*)NULL, (AGmNode*)NULL));
		return res;

	} else {
		char s[256];
		if (syn == 0) {
			art_sprintf_1(s, 256, "%s not supported.", id->a.s.v->Str());
		} else {
			art_sprintf_1(s, 256, "%s appeared arg of if,for,inv_for,switch.", id->a.s.v->Str());
		}
		return String(s);
	}
}

AGmNode *AGmProlog::CreateIdxs(AGmNode* arg) {
	AGmNode *idxs = NULL;
	for (AGmNode *ls = arg->THT; ls; ls = ls->T) idxs = Nconc(idxs, Cons(ls->HTH));
	if (idxs) idxs->count--;
	return idxs;
}

AGmNode* AGmProlog::Execute() {
#ifdef PROF_TIME
	struct timeb start; ftime(&start);
#endif
	AGmNode *err = NULL;
	for (AGmLink *l = next_outlink(manip, NULL); l; l = next_outlink(manip, l)) {
		AGmNode *arcs = NULL;
		AGmNode *g1 = l->arc->dst;
		AGmNode *g2; AGmLink *rl2; AGmNode *def = NULL;
		AGmLink* rl1 = next_outlink(g1, NULL);
		if (rl1 == NULL) {
			break;
		}
		CopyGraph(g1, rl1, g2, rl2);
		if (tree_conv && id0) {
			ptree0 = Cons(id0, ListCopy(grmr0, 1));
			AGmLink *l, *l2, *l3 = NULL;
			for (l = next_outlink(g2, NULL); l; l = l2) {
				l2 = next_outlink(g2, l);
				AGmNode *r1 = l->arc->dst;
				if (r1->HI != PRO_SYN || r1->TH != p->syn_ret) continue;
				err = ApplySynFunc(g2, l, l3, def, arcs);
				if (err) { RefUnref(g2, def, arcs); return err; } // error.
				break;
			}
			if (rl2 == l) rl2 = l3;
		}

		for (;;) {
			err = ExecuteGen(g2, rl2, def, arcs);
			if (IsString(err)) { RefUnref(def); break; } // error.
			if (err == three) { RefUnref(def); break; } // break_if_found
			if (err == two) { RefUnref(def); break; } // fail: try next repetiton
			if (err == one) { RefUnref(def); break; } // fail.
			err = ExecuteDef(g2, rl2, def, arcs);
			if (IsString(err)) break; // error.
			if (err == three) break; // break_if_found
			if (err == two) break; // fail: try next repetiton
			if (err == one) break; // fail.
			if (rl2 == NULL) break; // success.

			resolve_count = 0;
			err = Execute(g2, rl2);
if (debug_pred) {
char msg[256];
if (err == NULL) {
art_sprintf_1(msg, 256, "manip [SUCCESS] call_count = %d\n", call_count);
ADvWindow::Print(msg);
} else if (err == one) {
art_sprintf_1(msg, 256, "manip [FAIL] call_count = %d\n", call_count);
ADvWindow::Print(msg);
} else if (err == two) {
art_sprintf_1(msg, 256, "manip [FAIL(try next repetiton)] call_count = %d\n", call_count);
ADvWindow::Print(msg);
} else if (err == three) {
art_sprintf_1(msg, 256, "manip [SUCCESS(break_if_found)] call_count = %d\n", call_count);
ADvWindow::Print(msg);
} else {
art_sprintf_1(msg, 256, "manip [ERROR] call_count = %d\n", call_count);
ADvWindow::Print(msg);
}
}
			break;
		}
		RefUnref(g2, arcs);
		if (IsString(err)) break;
		if (tree_conv) {
			if (err == NULL) break;
			if (id0) { RefUnref(ptree0); ptree0 = NULL; }
		} else {
			if (err) break;
		}
	}
#ifdef PROF_TIME
	struct timeb end; ftime(&end);
	char s[256];
	art_sprintf_1(s, 256, "time = %d misc\n",
		((long)(end.time - start.time) * 1000 + (long)(end.millitm - start.millitm)));
	ADvWindow::Print(s);
#endif

if (debug_dump) {
if (err == NULL) {
ADvWindow::Print("[SUCCESS]\n");
} else if (err == one) {
ADvWindow::Print("[FAIL]\n");
} else if (err == two) {
ADvWindow::Print("[FAIL(try next repetiton)]\n");
} else if (err == three) {
ADvWindow::Print("[SUCCESS(break_if_found)]\n");
} else {
ADvWindow::Print("[ERROR]\n");
}
}
	if (IsString(err) && p->error) { RefUnref(err); return p->error; }
	return err;
}

AGmNode* AGmProlog::Execute(AGmNode* g1, AGmLink* rl1) {
	for (;;) {
		ADvDoEvents(); if (ADvExiting) return error("exit request by user.", NULL);

		if (rl1 == NULL) {
			if (resolve_count == 0) return error("lack of information for resolving.", NULL);
			rl1 = next_outlink(g1, NULL);
			resolve_count = 0;
		}
		if (rl1 == NULL)
			return NULL; // success.

		AGmNode *r1 = rl1->arc->dst, *err = NULL;
		AGmLink *rl2 = NULL; AGmNode *def = NULL;
		switch (r1->HI) {
		case PRO_PRED:
			err = PushPred(g1, rl1);
			if (IsString(err)) return err; // error.
			if (err == three) return err; // break_if_found
			if (err == two) return err; // fail: try next repetiton
			if (err == one) break; // fail.
			if (err == minus_one) {
				rl1 = next_outlink(g1, rl1); continue; // deferment.
			}
			err = ExecutePred(g1, rl1);
			if (IsString(err)) return err; // error.
			if (err == three) return err; // break_if_found
			if (err == two) return err; // fail: try next repetiton
			break;
		case PRO_NOT_PRED:
			err = ExecuteNotPred(g1, rl1);
			if (IsString(err)) return err; // error.
			break;
		case PRO_NEXT_IF_F:
			err = ExecuteNextIfFalse(g1, rl1);
			if (IsString(err)) return err; // error.
			if (err == three) return err; // break_if_found
			if (err == two) return err; // fail: try next repetiton
			break;
		case PRO_PM_REP:
			err = ApplyPmAndRep(g1, rl1);
			if (IsString(err)) return err; // error.
			break;
		case PRO_FUNC:
		case PRO_SYN:
			switch (r1->HI) {
			case PRO_FUNC: err = ApplyFunc(g1, rl1, rl2, def, exec_stack->arcs); break;
			case PRO_SYN:  err = ApplySynFunc(g1, rl1, rl2, def, exec_stack->arcs); break;
			}
			if (IsString(err)) { RefUnref(def); return err; } // error.
			if (err == three) { RefUnref(def); return err; } // break_if_found
			if (err == two) { RefUnref(def); return err; } // fail: try next repetiton
			if (err == one) { RefUnref(def); break; } // fail.
			if (err == minus_one) {
				rl1 = next_outlink(g1, rl1); continue; // deferment.
			}
			rl1 = rl2;
			err = ExecuteDef(g1, rl1, def, exec_stack->arcs);
			if (IsString(err)) return err; // error.
			if (err == three) return err; // break_if_found
			if (err == two) return err; // fail: try next repetiton
			break;
		default:
			return error("unknown code.", NULL);
		}
		if (err == NULL) continue;

		// back
		while (exec_stack && exec_stack->op != OP_DUMMY && exec_stack->op != OP_CUTTED) {
DebugPred(0, "next:    ", exec_stack->rl1->arc->dst, exec_stack->pred_count);
			DeleteArcs(exec_stack->arcs);
			if (err == zero) {
				PopExecStack(); call_count--;
if (exec_stack && exec_stack->op != OP_DUMMY && exec_stack->op != OP_CUTTED) {
DebugPred(-1, "back(3): ", exec_stack->rl1->arc->dst, exec_stack->pred_count);
}
				DeleteArcs(exec_stack->arcs);
			}
			if (exec_stack == NULL || exec_stack->op == OP_DUMMY || exec_stack->op == OP_CUTTED) break;
			g1 = exec_stack->g1; rl1 = exec_stack->rl1;
			err = ExecutePred(g1, rl1);
			if (IsString(err)) return err; // error.
			if (err == three) return err; // break_if_found
			if (err == two) return err; // fail: try next repetiton
			if (err == NULL) break;
		}
		if (exec_stack == NULL || exec_stack->op == OP_DUMMY || exec_stack->op == OP_CUTTED) return one; // fail.
	}
}

AGmNode* AGmProlog::PushPred(AGmNode* g1, AGmLink* rl1) {
	AGmNode *r1 = rl1->arc->dst;

	AGmNode *rules = dst_node2(pred, r1->TH);
	if (rules == NULL) return error("not found predicate(1).", r1->TH);
	AGmLink *l = next_outlink(rules, NULL);
	if (l == NULL) return error("not found predicate(2).", r1->TH);

	if (r1->TTH) Cut();
	if (++call_count > max_call_count) return error("stack overflow.", NULL);
	PushExecStack(new AGmExecStack(OP_PRED, g1, rl1, rules, l));

	return NULL;
}

AGmNode* AGmProlog::ExecutePred(AGmNode*& g1, AGmLink*& rl1) {
	AGmNode *r1 = exec_stack->rl1->arc->dst;
	AGmNode *g2, *err; AGmLink *rl2;
	AGmNode *def = NULL;

	AGmNode *rules = exec_stack->n;
	AGmLink *l = exec_stack->l;
	if (l == NULL) return zero; // no more rule.
	exec_stack->l = next_outlink(rules, l);

	AGmNode *rule = l->arc->dst;
	exec_stack->pred_count++;

DebugPred(1, "apply:   ", r1, exec_stack->pred_count);
	err = ApplyPred(g1, exec_stack->rl1, rule, g2, rl2, def, exec_stack->arcs);
	if (IsString(err)) {
		RefUnref(def);
		return err; // error.
	}
	if (err == one) {
DebugPred(-1, "back(0): ", r1, exec_stack->pred_count);
		RefUnref(def);
		return one;	// fail.
	}
	err = ExecuteGen(g2, rl2, def, exec_stack->arcs);
	if (IsString(err)) {
		RefUnref(g2, def);
		return err; // error.
	}
	if (err == three) {
		RefUnref(g2, def);
		return err; // break_if_found
	}
	if (err == two) {
		RefUnref(g2, def);
		return err; // fail: try next repetiton
	}
	if (err == one) {
DebugPred(-1, "back(1): ", r1, exec_stack->pred_count);
		RefUnref(g2, def);
		return one; // fail.
	}
	err = ExecuteDef(g2, rl2, def, exec_stack->arcs);
	if (IsString(err)) {
		RefUnref(g2);
		return err; // error.
	}
	if (err == three) {
		RefUnref(g2);
		return err; // break_if_found
	}
	if (err == two) {
		RefUnref(g2);
		return err; // fail: try next repetiton
	}
	if (err == one) {
DebugPred(-1, "back(2): ", r1, exec_stack->pred_count);
		RefUnref(g2);
		return one; // fail.
	}

	g1 = g2; rl1 = rl2;

	return NULL;
}

AGmNode* AGmProlog::ExecuteNotPred(AGmNode* g, AGmLink*& rl) {
	AGmNode *g1 = Cons(NULL);
	AGmLink *rl1 = next_outlink(g, rl);
	DeleteArc2(rl, NULL, exec_stack->arcs);
	for (;;) {
		rl = rl1; rl1 = next_outlink(g, rl);
		if (rl->arc->dst->HI == PRO_NOT_PRED) {
			DeleteArc2(rl, NULL, exec_stack->arcs); rl = next_outlink(g, rl1); break;
		}
		ReplaceArcSrc2(rl, g1, NULL, exec_stack->arcs);
	}

	for (AGmLink *l = next_outlink(g1, NULL); l; l = next_outlink(g1, l)) {
		Set2(&l->arc->dst->H, Integer(l->arc->dst->HI ^ PRO_NOT_PRED), exec_stack->arcs);
	}

	rl1 = next_outlink(g1, NULL);

	AGmProlog* prolog = new AGmProlog(p);
	prolog->PredManip(PredManip());
	AGmNode* err = prolog->Execute(g1, rl1);
	if (err == NULL) {
		prolog->Back();
	}
	delete prolog;
	RefUnref(g1);

	if (IsString(err)) {
if (debug_dump) {
DumpList("NOT causes error, ", NULL);
}
		return err; // error.
	}
	if (err == NULL) {
if (debug_dump) {
DumpList("NOT Fail, ", NULL);
}
		return one; // success -> fail. (pred_not)
	}
if (debug_dump) {
DumpList("NOT Success, ", NULL);
}
	resolve_count++;
	return NULL; // fail -> success. (pred_not)
}
AGmNode* AGmProlog::ExecuteNextIfFalse(AGmNode* g, AGmLink*& rl) {
	AGmNode *g1 = Cons(NULL);
	AGmLink *rl1 = next_outlink(g, rl);
	DeleteArc2(rl, NULL, exec_stack->arcs);
	for (;;) {
		rl = rl1; rl1 = next_outlink(g, rl);
		if (rl->arc->dst->HI == PRO_NEXT_IF_F) {
			DeleteArc2(rl, NULL, exec_stack->arcs); rl = next_outlink(g, rl1); break;
		}
		ReplaceArcSrc2(rl, g1, NULL, exec_stack->arcs);
	}

	for (AGmLink *l = next_outlink(g1, NULL); l; l = next_outlink(g1, l)) {
		Set2(&l->arc->dst->H, Integer(l->arc->dst->HI ^ PRO_NEXT_IF_F), exec_stack->arcs);
	}

	rl1 = next_outlink(g1, NULL);

	AGmProlog* prolog = new AGmProlog(p);
	prolog->PredManip(PredManip());
	AGmNode* err = prolog->Execute(g1, rl1);
	delete prolog;
	RefUnref(g1);

	if (IsString(err)) {
if (debug_dump) {
DumpList("next_if_false causes error: error = ", err);
}
		return err; // error. (next_if_false)
	}
	if (err == one || err == two) {
if (debug_dump) {
DumpList("next_if_false Fail(try next repetiton), ", NULL);
}
		return two; // fail: try next repetiton (next_if_false)
	}
if (debug_dump) {
DumpList("next_if_false Success, ", NULL);
}
	resolve_count++;
	return NULL; // success. (next_if_false)
}

void AGmProlog::DebugPred(int inc, const char* s, AGmNode* r1, int pred_count) {
	char msg[256];
	if (debug_pred) {
		char format[128];
		art_strcpy(format, 128, s);
		if (tree_conv) {
			AGmNode *id;
			if (id0 == NULL) {
				id = p->syntax_id_val;
			} else {
				id = id0;
			}
			art_strcat(format, 128, "AAA this = 0x%x, call_count = %d, syntax_id = %s, pred = %s(%d)\n");
			art_sprintf_5(msg, 256, format, this, call_count, id->a.s.v->Str(), r1->TH->a.s.v->Str(), pred_count);
		} else {
			art_strcat(format, 128, "AAA this = 0x%x, call_count = %d, pred = %s(%d)\n");
			art_sprintf_4(msg, 256, format, this, call_count, r1->TH->a.s.v->Str(), pred_count);	
		}
		ADvWindow::Print(msg);
	}
	if (inc > 0) {
		art_sprintf_2(msg, 256, "%s(%d)", r1->TH->a.s.v->Str(), pred_count);
		Push(pred_stack, String(msg));
	} else if (inc < 0) {
		Pop(pred_stack);
	}
}

void AGmProlog::Cut() {
	AGmExecStack* stack = exec_stack;
	exec_stack = exec_stack->next;
	FreeExecStack();
	stack->op = OP_CUTTED;
	PushExecStack(stack);
}

// -------------------------------------------------------------
void AGmProlog::PushExecStack(AGmExecStack* stack) {
	stack->next = exec_stack;
	exec_stack = stack;
}
void AGmProlog::PopExecStack() {
	AGmExecStack *stack = exec_stack;
	exec_stack = exec_stack->next;
	delete stack;
}
void AGmProlog::FreeExecStack() {
	while (exec_stack) {
		call_count--;
		AGmExecStack *next = exec_stack->next;
		delete exec_stack;
		exec_stack = next;
	}
}

AGmExecStack::AGmExecStack(int op, AGmNode* g1, AGmLink* rl1,
	AGmNode* n, AGmLink* l) {
	this->op = op;
	this->g1 = g1; this->rl1 = rl1; this->n = n; this->l = l;
	pred_count = 0;
	arcs = NULL;
	obj_db = NULL;
	level = 0;
	save_stat = NULL;
	save_l1 = NULL;
}
AGmExecStack::~AGmExecStack() {
	RefUnref(arcs);
}
AGmExecStack *AGmExecStack::FindExecStack(int level) {
	AGmExecStack *stack = this;
	while (stack && stack->level > level) {
		stack = stack->next;
	}
	return stack;
}

void AGmExecStack::make_exist(AGmProlog *pr, AGmNode* arc_var, int exist) {
if (AGmProlog::debug_syn2) {
AGmNode *e = Ref(Integer(exist));
DumpList("AGmExecStack::make_exist: exist = ", e, 10, 10);
Unref(e);
}
	set_dst_node(arc_var, pr->p->grph_eq, Integer(exist));
}
AGmNode* AGmExecStack::arc_exist_info(AGmProlog* pr, AGmNode* an) {
if (AGmProlog::debug_syn2) {
DumpList("AGmExecStack::arc_exist_info: name = ", an, 10, 10);
}
	AGmExecStack* stack; AGmString *s;
	for (stack = pr->exec_stack; stack; stack = stack->next) {
		AGmNode *obj_db = stack->obj_db;
		if (obj_db == NULL) continue;
		AGmNode *arc_var = dst_node2(obj_db, an);
		if (arc_var == NULL) {
			s = new AGmString("can't find arc_var in obj_db. name = ");
			s->Append(an->a.s.v->Str());
			return String(s);
		}
		AGmNode *exist = dst_node2(arc_var, pr->p->grph_eq);
		if (exist == NULL) {
			s = new AGmString("arc_var does not have exist-info. name = ");
			s->Append(an->a.s.v->Str());
			return String(s);
		}
if (AGmProlog::debug_syn2) {
DumpList("AGmExecStack::arc_exist_info: exist = ", exist, 10, 10);
}
		return exist;
	}
	s = new AGmString("can't find exec_stack which has obj_db information. name = ");
	s->Append(an->a.s.v->Str());
	return String(s);
}

// -------------------------------------------------------------

void AGmProlog::DeleteArcs(AGmNode*& arcs) {
	char msg[256];
if (debug_syn) {
art_sprintf_2(msg, 256, "this = 0x%lx, ### Enter AGmProlog::DeleteArcs(): arcs = %lx\n", AL_PTR(this), AL_PTR(arcs));
ADvWindow::Print(msg);
}
	AGmNode *ls;
	if (tree_conv) {
		if (id0 == NULL) {
			// tree -> graph
			for (ls = arcs; ls; ls = ls->T) {
				if (ls->HH == BACK_SET_H_TH) { // back of Set_H_TH
					AGmNode *obj = ls->H->TH;
					AGmNode *h = ls->H->TTH;
					AGmNode *th = ls->H->TTTH;
					AGmNode *v = ls->H->TTTTH;
					ASSERT(obj->TH == v);
					Set(obj->H, h); Set(obj->TH, th);
				} else if (ls->HH == BACK_SET2) { // Set2
					AGmNode **addr = (AGmNode**)ls->H->TH->a.p.ptr;
					AGmNode *old_v = ls->H->TTH;
					AGmNode *new_v = ls->H->TTTH;
					ASSERT(*addr == new_v);
					Set(*addr, old_v);
				} else if (ls->HH == BACK_REP_ARC_DST) {
					AGmArc *a = (AGmArc*)ls->H->TH->a.p.ptr;
					AGmNode *oldDst = ls->H->TTH;
					AGmNode *oldSrc = ls->H->TTTH;
					AGmNode *newDst = ls->H->TTTTH;
					ASSERT(a->src == oldSrc);
					ASSERT(a->dst == newDst);
					ReplaceArcDst(a, oldDst);
				} else if (ls->HH == BACK_REP_ARC_SRC || ls->HH == BACK_DELETE_ARC) {
					AGmArc *a = (AGmArc*)ls->H->TH->a.p.ptr;
					AGmNode *oldSrc = ls->H->TTH;
					AGmNode *dstPrev = ls->H->TTTH;
					AGmNode *dstNext = ls->H->TTTTH;
					AGmNode *newSrc = ls->H->TTTTTH;
					ASSERT(a->src == newSrc);
					AGmLink *outlink = oldSrc->body->out, *lnk;
					if (dstPrev) {
						for (lnk = outlink->next ; lnk != outlink; lnk = lnk->next) {
							if (lnk->arc->dst == dstPrev) {
								if (dstNext) {
									ASSERT(lnk->next != outlink);
									if (lnk->next->arc->dst == dstNext) {
										break;
									}
								} else {
									break;
								}
							}
						}
					} else if (dstNext) {
						for (lnk = outlink->next ; lnk != outlink; lnk = lnk->next) {
							if (lnk->arc->dst == dstNext) {
								lnk = lnk->prev; break;
							}
						}
					} else {
						lnk = outlink;
					}
					ReplaceArcSrc(a, oldSrc, lnk->next);
				} else if (ls->HH == BACK_CREATE_ARC) {
					AGmArc *a = (AGmArc*)ls->HTTH->a.p.ptr;
					DeleteArc(a);
				} else if (ls->HH == BACK_CREATE_ARC2) {
					AGmNode *src = (AGmNode*)ls->H->TH;
					AGmNode *dst = (AGmNode*)ls->H->TTH;
					AGmLink *l;
					for (l = src->body->out->next; l != src->body->out; l = l->next) {
						if (l->arc->dst == dst) {
							DeleteArc(l->arc);
							break;
						}
					}
				} else {
					ASSERT(0);
				}
			}
		} else {
			// graph -> tree
			for (ls = arcs; ls; ls = ls->T) {
				if (ls->HH == BACK_SET_H_TH) { // back of Set_H_TH
					AGmNode *obj = ls->H->TH;
					AGmNode *h = ls->H->TTH;
					AGmNode *th = ls->H->TTTH;
					AGmNode *v = ls->H->TTTTH;
					ASSERT(obj->TH == v);
					Set(obj->H, h); Set(obj->TH, th);
				} else if (ls->HH == BACK_SET2) { // Set2
					AGmNode **addr = (AGmNode**)ls->H->TH->a.p.ptr;
					AGmNode *old_v = ls->H->TTH;
					AGmNode *new_v = ls->H->TTTH;
					ASSERT(*addr == new_v);
					Set(*addr, old_v);
				} else if (ls->HH == BACK_REP_ARC_DST) {
					AGmArc *a = (AGmArc*)ls->H->TH->a.p.ptr;
					AGmNode *oldDst = ls->H->TTH;
					AGmNode *oldSrc = ls->H->TTTH;
					AGmNode *newDst = ls->H->TTTTH;
					ASSERT(a->src == oldSrc);
					ASSERT(a->dst == newDst);
					ReplaceArcDst(a, oldDst);
				} else if (ls->HH == BACK_REP_ARC_SRC || ls->HH == BACK_DELETE_ARC) {
					AGmArc *a = (AGmArc*)ls->H->TH->a.p.ptr;
					AGmNode *oldSrc = ls->H->TTH;
					AGmNode *dstPrev = ls->H->TTTH;
					AGmNode *dstNext = ls->H->TTTTH;
					AGmNode *newSrc = ls->H->TTTTTH;
					ASSERT(a->src == newSrc);
					AGmLink *outlink = oldSrc->body->out, *lnk;
					if (dstPrev) {
						for (lnk = outlink->next ; lnk != outlink; lnk = lnk->next) {
							if (lnk->arc->dst == dstPrev) {
								if (dstNext) {
									ASSERT(lnk->next != outlink);
									if (lnk->next->arc->dst == dstNext) {
										break;
									}
								} else {
									break;
								}
							}
						}
					} else if (dstNext) {
						for (lnk = outlink->next ; lnk != outlink; lnk = lnk->next) {
							if (lnk->arc->dst == dstNext) {
								lnk = lnk->prev; break;
							}
						}
					} else {
						lnk = outlink;
					}
					ReplaceArcSrc(a, oldSrc, lnk->next);
				} else if (ls->HH == BACK_CREATE_ARC) {
					AGmArc *a = (AGmArc*)ls->HTTH->a.p.ptr;
					DeleteArc(a);
				} else if (ls->HH == BACK_FOR_INDEX) {
if (debug_syn2) {
art_sprintf_2(msg, 256, "AAA back for index: this = 0x%lx, call_count = %d\n", AL_PTR(this), call_count);
ADvWindow::Print(msg);
}
					AGmNode *for_idx = ls->HTH;
					for_idx->HI--;
				} else if (ls->HH == BACK_DELAYED) {
					for (AGmNode *ls2 = ls->HTH; ls2; ls2 = ls2->T) {
						ls2->H->TTTHI--;
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA back_delayed_val: this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls2->H), call_count); 
ADvWindow::Print(msg);
DumpList("AAA back_delayed_val = ", ls2->H, 20, 20);
}
					}
				} else if (ls->HH == BACK_GEN_STATUS) {
					AGmNode *src = ls->HTH;
					AGmNode *dst = ls->HTTH;
					for (AGmLink *l = next_outlink(src, NULL); l; l = next_outlink(src, l)) {
						if (l->arc->dst == dst) {
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA back gen_status: this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(l->arc->dst), call_count);
ADvWindow::Print(msg);
DumpList("AAA ls = ", l->arc->dst, 20, 20);
}
							DeleteArc(l->arc); break;
						}
					}
				} else if (ls->HH == BACK_DEC_OF_DELAYD) {
					AGmNode *delay_for = ls->HTH;
					for (AGmNode *ls2 = delay_for; ls2; ls2 = ls2->T) {
						ls2->H->TTTHI++;
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA back_dec_of_delayed_val: this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls2->H), call_count);
ADvWindow::Print(msg);
DumpList("ls = ", ls2->H, 20, 20);
}
					}
				} else {
					ASSERT(0);
				}
			}
		}
	} else {
		// general
		for (ls = arcs; ls; ls = ls->T) {
			if (ls->HH == BACK_SET_H_TH) { // back of Set_H_TH
				AGmNode *obj = ls->H->TH;
				AGmNode *h = ls->H->TTH;
				AGmNode *th = ls->H->TTTH;
				AGmNode *v = ls->H->TTTTH;
				ASSERT(obj->TH == v);
				Set(obj->H, h); Set(obj->TH, th);
			} else if (ls->HH == BACK_SET2) { // Set2
				AGmNode **addr = (AGmNode**)ls->H->TH->a.p.ptr;
				AGmNode *old_v = ls->H->TTH;
				AGmNode *new_v = ls->H->TTTH;
				ASSERT(*addr == new_v);
				Set(*addr, old_v);
			} else if (ls->HH == BACK_REP_ARC_DST) {
				AGmArc *a = (AGmArc*)ls->H->TH->a.p.ptr;
				AGmNode *oldDst = ls->H->TTH;
				AGmNode *oldSrc = ls->H->TTTH;
				AGmNode *newDst = ls->H->TTTTH;
				ASSERT(a->src == oldSrc);
				ASSERT(a->dst == newDst);
				ReplaceArcDst(a, oldDst);
			} else if (ls->HH == BACK_REP_ARC_SRC || ls->HH == BACK_DELETE_ARC) {
				AGmArc *a = (AGmArc*)ls->H->TH->a.p.ptr;
				AGmNode *oldSrc = ls->H->TTH;
				AGmNode *dstPrev = ls->H->TTTH;
				AGmNode *dstNext = ls->H->TTTTH;
				AGmNode *newSrc = ls->H->TTTTTH;
				ASSERT(a->src == newSrc);
				AGmLink *outlink = oldSrc->body->out, *lnk;
				if (dstPrev) {
					for (lnk = outlink->next ; lnk != outlink; lnk = lnk->next) {
						if (lnk->arc->dst == dstPrev) {
							if (dstNext) {
								ASSERT(lnk->next != outlink);
								if (lnk->next->arc->dst == dstNext) {
									break;
								}
							} else {
								break;
							}
						}
					}
				} else if (dstNext) {
					for (lnk = outlink->next ; lnk != outlink; lnk = lnk->next) {
						if (lnk->arc->dst == dstNext) {
							lnk = lnk->prev; break;
						}
					}
				} else {
					lnk = outlink;
				}
				ReplaceArcSrc(a, oldSrc, lnk->next);
			} else if (ls->HH == BACK_CREATE_ARC) {
				AGmArc *a = (AGmArc*)ls->HTTH->a.p.ptr;
				DeleteArc(a);
			} else {
				ASSERT(0);
			}
		}
	}
	RefUnref(arcs); arcs = NULL;
if (debug_syn) {
art_sprintf_2(msg, 256, "this = 0x%lx, ### Leave AGmProlog::DeleteArcs(): arcs = %lx\n", AL_PTR(this), AL_PTR(arcs));
ADvWindow::Print(msg);
}
}

void AGmProlog::Set_H_TH(AGmNode* obj, AGmNode* value, AGmNode*& arcs) {
	arcs = Cons(List(BACK_SET_H_TH, obj, obj->H, obj->TH, value), arcs);
	Set(obj->H, one); Set(obj->TH, value);
}
void AGmProlog::Set2(AGmNode** addr, AGmNode* value, AGmNode*& arcs) {
	arcs = Cons(List(BACK_SET2, PtrCell(addr), *addr, value), arcs);
	Set(*addr, value);
}
void AGmProlog::ReplaceArcDst2(AGmArc* a, AGmNode* newDst, AGmNode*& arcs) {
	arcs = Cons(List(BACK_REP_ARC_DST, PtrCell(a), a->dst, a->src, newDst), arcs);
	ReplaceArcDst(a, newDst);
}
void AGmProlog::ReplaceArcSrc2(AGmLink* lnk, AGmNode* newSrc, AGmLink* l, AGmNode*& arcs) {
	AGmArc *a = lnk->arc;
	AGmNode *oldSrc = a->src;
	AGmLink *outlink = oldSrc->body->out;
	AGmNode *dstPrev = (lnk->prev == outlink ? NULL : lnk->prev->arc->dst);
	AGmNode *dstNext = (lnk->next == outlink ? NULL : lnk->next->arc->dst);
	arcs = Cons(List(BACK_REP_ARC_SRC, PtrCell(a), oldSrc, dstPrev, dstNext, newSrc), arcs);
	ReplaceArcSrc(a, newSrc, l);
}
void AGmProlog::DeleteArc2(AGmLink* lnk, AGmNode* rule, AGmNode*& arcs) {
	AGmNode *obj = Cons(NULL, NULL);
	AGmArc *a = lnk->arc;
	AGmNode *oldSrc = a->src;
	AGmLink *outlink = oldSrc->body->out;
	AGmNode *dstPrev = (lnk->prev == outlink ? NULL : lnk->prev->arc->dst);
	AGmNode *dstNext = (lnk->next == outlink ? NULL : lnk->next->arc->dst);
	arcs = Cons(List(BACK_DELETE_ARC, PtrCell(a), oldSrc, dstPrev, dstNext, obj, rule), arcs);
	ReplaceArcSrc(a, obj);
}
void AGmProlog::CreateArc2(AGmNode* src, AGmNode* dst, AGmNode* attr, AGmNode*& arcs) {
	AGmArc* a = CreateArc(src, dst, attr);
	arcs = Cons(List(BACK_CREATE_ARC, src, PtrCell(a)), arcs);
}

AGmNode *AGmProlog::ExecuteDef(AGmNode* g1, AGmLink*& rl2, AGmNode*& def, AGmNode*& arcs) {
	AGmNode *cell, *o, *r, *err = NULL;
	while (def) {
		cell = def;
		o = cell->H; cell->H = NULL;
		def = cell->T; cell->T = NULL; if (def) def->count--;
		RefUnref(cell); AGmLink *l, *l2, *rl1, *rl3 = NULL;
		for (l = next_inlink(o, NULL); l; l = l2) {
			l2 = next_inlink(o, l);
			r = l->arc->src;
			rl1 = find_outlink(g1, r);
			if (rl1 == NULL) continue;
			//ASSERT(rl1);
			//ASSERT(rl1->arc->src == g1);
			//ASSERT(rl1->arc->dst == r);
			switch (r->HI) {
			case PRO_FUNC:
				err = ApplyFunc(g1, rl1, rl3, def, arcs);
				break;
			case PRO_SYN:  err = ApplySynFunc(g1, rl1, rl3, def, arcs);
				break;
			default:
				continue;
			}
			if (IsString(err)) { Unref(o); RefUnref(def); return err; } // error.
			if (err == three) { Unref(o); RefUnref(def); return err; } // break_if_found
			if (err == two) { Unref(o); RefUnref(def); return err; } // fail: try next repetiton
			if (err == one) { Unref(o); RefUnref(def); return err; } // fail.
			if (err == minus_one) continue; // deferment.
			if (rl1 == rl2) rl2 = rl3;
		}
		Unref(o);
	}
	return NULL;
}

AGmNode* AGmProlog::ExecuteGen(AGmNode* g, AGmLink*& rl, AGmNode*& def, AGmNode*& arcs) {
	if (tree_conv == 0) return NULL;
	AGmNode *err; AGmLink *l, *l2, *l3;
	for (l = next_outlink(g, NULL); l; l = l2) {
		l2 = next_outlink(g, l);
		AGmNode *r1 = l->arc->dst;

		if (r1->HI == PRO_NOT_PRED) {
			for (;;) {
				r1 = l2->arc->dst;
				l2 = next_outlink(g, l2);
				if (r1->HI == PRO_NOT_PRED) {
					r1 = l2->arc->dst;
					l2 = next_outlink(g, l2);
					break;
				}
			}
			if (l2 == NULL) break;
		}

		if (r1->TH != p->eq && r1->TH != p->grph_eq) continue;
		err = ApplyFunc(g, l, l3, def, arcs);
		if (IsString(err)) return err; // error.
		if (err == three) return err; // break_if_found
		if (err == two) return err; // fail: try next repetiton
		if (err == one) return one; // fail.
		if (err == minus_one) continue; // deferment.
		if (rl == l) rl = l2;
	}
	int count;
	for (count = 0;; count = 0) {
		for (l = next_outlink(g, NULL); l; l = l2) {
			l2 = next_outlink(g, l);
			AGmNode *r1 = l->arc->dst;

			if (r1->HI == PRO_NOT_PRED) {
				for (;;) {
					r1 = l2->arc->dst;
					l2 = next_outlink(g, l2);
					if (r1->HI == PRO_NOT_PRED) {
						r1 = l2->arc->dst;
						l2 = next_outlink(g, l2);
						break;
					}
				}
				if (l2 == NULL) break;
			}

			if (r1->TH != p->switch_str) continue;
			err = ApplySynFunc(g, l, l3, def, arcs);
			if (IsString(err)) return err; // error.
			if (err == one) return one; // fail.
			if (err == minus_one) continue; // deferment.
			count++;
			if (rl == l) rl = l2;
		}
		if (count == 0) break;
	}
	for (count = 0;; count = 0) {
		for (l = next_outlink(g, NULL); l; l = l2) {
			l2 = next_outlink(g, l);
			AGmNode *r1 = l->arc->dst;

			if (r1->HI == PRO_NOT_PRED) {
				for (;;) {
					r1 = l2->arc->dst;
					l2 = next_outlink(g, l2);
					if (r1->HI == PRO_NOT_PRED) {
						r1 = l2->arc->dst;
						l2 = next_outlink(g, l2);
						break;
					}
				}
				if (l2 == NULL) break;
			}

			if (r1->TH != p->switch_str && r1->H != p->for_str && r1->H != p->if_str) continue;
			err = ApplySynFunc(g, l, l3, def, arcs);
			if (IsString(err)) return err; // error.
			if (err == one) return one; // fail.
			if (err == minus_one) continue; // deferment.
			count++;
			if (rl == l) rl = l2;
		}
		if (count == 0) break;
	}
	return NULL;
}

void AGmProlog::DebugDump(const char*s, AGmNode* g, AGmNode* r) {
	char str[256];
	art_sprintf_3(str, 256, "==== this = 0x%lx ============ %s, call_count = %d\n", AL_PTR(this), s, call_count);
	ADvWindow::Print(str);
	DumpGraph(g, 10, 1000, NULL, 1, r);
}

AGmNode* AGmProlog::ApplyPred(
		AGmNode* g1, AGmLink* rl1, AGmNode* rule, AGmNode*& g2, AGmLink*& rl2, AGmNode*& def, AGmNode*& arcs) {
	AGmNode *head, *body, *rule2;
	AGmNode *r1 = rl1->arc->dst;

	// graph copy
	rule2 = GraphCopy(rule, 1);

	head = rule2->H; body = rule2->TH;
	CopyGraph(g1, rl1, g2, rl2);

if (debug_dump) {
DebugDump("Before ApplyPred", g2, rl2->arc->dst);
}

	// assignment of arguments and attach relations to the corresponding node
	AGmLink *l1, *l2, *l3; AGmNode *o1, *o2;
	AGmNode *r2 = rl2->arc->dst;
	for (l1 = next_outlink(r2, NULL), l2 = next_outlink(head, NULL);
			l1 && l2; l1 = next_outlink(r2, l1), l2 = next_outlink(head, l2)) {
		o1 = l1->arc->dst; o2 = l2->arc->dst;
		ASSERT(Compare(l1->arc->attr, l2->arc->attr) == 0);
		if (o1->H && o2->H == NULL) {
			Set_H_TH(o2, o1->TH, arcs); def = Cons(o2, def);
		} else if (o1->H == NULL && o2->H) {
			if (tree_conv && id0 && o1->TH) {
				// graph -> tree
				AGmNode *err = gen_DelayVal(o1->TH, o2->TH, arcs, r1->TH);
				if (err) { RefUnref(rule2, g2); return err; }
			}
			def = Cons(o2, def);
		} else if (o1->H && o2->H) {
			if (Compare(o1->TH, o2->TH) != 0) break;
		} else if (o1->H == NULL && o2->H == NULL) {
			if (o1->TH == p->wc_e && o2->TH == NULL) {
				Set2(&o2->TH, o1->TH, arcs); def = Cons(o2, def);
			} else if (o2->TH == p->wc_e && o1->TH == NULL) {
				def = Cons(o2, def);
			} else {
				Set2(&o2->TH, o1->TH, arcs); // delay_val
			}
		}
		// reconnect 'input arc to o1' to o2
		for (l3 = next_inlink(o1, NULL); l3; l3 = next_inlink(o1, l3)) {
			if (l3->arc->src == r2) continue;
			l3 = l3->prev;
			ReplaceArcDst2(l3->next->arc, o2, arcs);
		}
	}
	if (l1 && l2) { RefUnref(rule2, g2); return one; } // fail.
	if (l1 || l2) { RefUnref(rule2, g2); return error("number of argments mismatch.", r1->TH); }

	// replace subgraph
	l1 = rl2; rl2 = NULL;
	for (l2 = next_outlink(body, NULL); l2; l2 = next_outlink(body, l2)) {
		if (rl2 == NULL) rl2 = l2;
		l2 = l2->prev;
		ReplaceArcSrc2(l2->next, g2, l1, arcs);
	}
	DeleteArc2(l1, rule2, arcs); resolve_count++;

if (debug_dump) {
DebugDump("After ApplyPred", g2, (rl2 ? rl2->arc->dst: NULL));
}

	return NULL;
}

void AGmProlog::CopyGraph(AGmNode* g1, AGmLink* rl1, AGmNode*& g2, AGmLink*& rl2) {
	AGmNode *r1 = rl1->arc->dst;
	AGmLink *rl, *ol, *lnk = NULL; AGmNode *r, *o;
#if	_WINDOWS && _64BIT
	NodeBody(g1); g1->body->nid = (long long)ListCopy(g1, 1);
	for (rl = next_outlink(g1, NULL); rl; rl = next_outlink(g1, rl)) {
		r = rl->arc->dst;
		NodeBody(r); r->body->nid = (long long)ListCopy(r, 1);
		CreateArc((AGmNode*)g1->body->nid, (AGmNode*)r->body->nid, rl->arc->attr, NULL, NULL, &lnk);
		if (r == r1) rl2 = lnk;
		for (ol = next_outlink(r, NULL); ol; ol = next_outlink(r, ol)) {
			o = ol->arc->dst;
			CreateArc((AGmNode*)r->body->nid, o, ol->arc->attr);
		}
	}
#else
	NodeBody(g1); g1->body->nid = (long)ListCopy(g1, 1);
	for (rl = next_outlink(g1, NULL); rl; rl = next_outlink(g1, rl)) {
		r = rl->arc->dst;
		NodeBody(r); r->body->nid = (long)ListCopy(r, 1);
		CreateArc((AGmNode*)g1->body->nid, (AGmNode*)r->body->nid, rl->arc->attr, NULL, NULL, &lnk);
		if (r == r1) rl2 = lnk;
		for (ol = next_outlink(r, NULL); ol; ol = next_outlink(r, ol)) {
			o = ol->arc->dst;
			CreateArc((AGmNode*)r->body->nid, o, ol->arc->attr);
		}
	}
#endif
	g2 = (AGmNode*)g1->body->nid;
	ASSERT((AGmNode*)r1->body->nid == rl2->arc->dst);
	g1->body->nid = 0;
	for (rl = next_outlink(g1, NULL); rl; rl = next_outlink(g1, rl)) {
		r = rl->arc->dst; r->body->nid = 0;
	}
}

void AGmProlog::NodeBody(AGmNode* n) {
	if (n->body == NULL) {
		AGmNodeBody *nb = n->body = (AGmNodeBody*)Alloc();
		nb->type = AGM_BODY_OF_NODE;
		nb->flag = 0;
		nb->nid = 0;
		nb->in = nb->out = NULL;
	}
}

AGmNode* AGmProlog::ApplyFunc(AGmNode* g, AGmLink* rl1, AGmLink*& rl2, AGmNode*& def, AGmNode*& arcs) {

if (debug_dump) {
DebugDump("Before ApplyFunc", g, rl1->arc->dst);
}

	AGmNode *r1 = rl1->arc->dst;
	AGmNode *id = r1->TH;
	AGmNode *o1 = dst_node(r1, 1);
	AGmNode *o2 = dst_node(r1, 2);
	AGmNode *o3 = dst_node(r1, 3);
	AGmNode *res = dst_node2(r1, (AGmNode*)NULL);
	if (id == p->add) {
		AGmString *s1, *s2, *s3;
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (o1->THI + o2->THI != res->THI) return one; // fail.
					} else if (IsString(o1->TH) && IsString(o2->TH) && IsString(res->TH)) {
						s1 = o1->TH->a.s.v; s2 = o2->TH->a.s.v; s3 = res->TH->a.s.v;
						if (s1->Length() + s2->Length() != s3->Length()) return one; // fail.
						if (s3->StartsWith(s1) == 0) return one; // fail.
						if (s3->EndsWith(s2) == 0) return one; // fail.
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					if (IsInteger(o1->TH) && IsInteger(o2->TH)) { // Integer + Integer
						if (tree_conv && id0) delay = Ref(res->TH);
						Set_H_TH(res, value = Integer(o1->THI + o2->THI), arcs); def = Cons(res, def);
					} else if (IsString(o1->TH) && IsString(o2->TH)) { // String + String
						s1 = new AGmString(o1->TH->a.s.v->Str());
						s1->WriteString(o2->TH->a.s.v->Str());
						if (tree_conv && id0) delay = Ref(res->TH);
						Set_H_TH(res, value = String(s1), arcs); def = Cons(res, def);
					} else if (IsString(o1->TH) && IsInteger(o2->TH)) { // String + Integer
						s1 = new AGmString(o1->TH->a.s.v->Str());
						s1->WriteInteger(o2->THI);
						if (tree_conv && id0) delay = Ref(res->TH);
						Set_H_TH(res, value = String(s1), arcs); def = Cons(res, def);
					} else if (IsString(o1->TH) && IsList(o2->TH)) { // String + List
						s1 = new AGmString(o1->TH->a.s.v->Str());
						int count = 0;
						s1->WriteList(o2->TH, count);
						if (tree_conv && id0) delay = Ref(res->TH);
						Set_H_TH(res, value = String(s1), arcs); def = Cons(res, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				}
			} else {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(res->TH)) {
						if (tree_conv && id0) delay = Ref(o2->TH);
						Set_H_TH(o2, value = Integer(res->THI - o1->THI), arcs); def = Cons(o2, def);
					} else if (IsString(o1->TH) && IsString(res->TH)) {
						s1 = o1->TH->a.s.v; s3 = res->TH->a.s.v;
						if (s3->StartsWith(s1) == 0) return one; // fail.
						if (tree_conv && id0) delay = Ref(o2->TH);
						Set_H_TH(o2, value = String(s3->SubString(s1->Length(), s3->Length())), arcs); def = Cons(o2, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					return minus_one; // deferment.
				}
			}
		} else {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (tree_conv && id0) delay = Ref(o1->TH);
						Set_H_TH(o1, value = Integer(res->THI - o2->THI), arcs); def = Cons(o1, def);
					} else if (IsString(o2->TH) && IsString(res->TH)) {
						s2 = o2->TH->a.s.v; s3 = res->TH->a.s.v;
						if (s3->EndsWith(s2) == 0) return one; // fail.
						if (tree_conv && id0) delay = Ref(o1->TH);
						Set_H_TH(o1, value = String(s3->SubString(0, s3->Length() - s2->Length())), arcs); def = Cons(o1, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					return minus_one; // deferment.
				}
			} else {
				return minus_one; // deferment.
			}
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->sub) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (o1->THI - o2->THI != res->THI) return one; // fail.
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					if (IsInteger(o1->TH) && IsInteger(o2->TH)) {
						if (tree_conv && id0) delay = Ref(res->TH);
						Set_H_TH(res, value = Integer(o1->THI - o2->THI), arcs); def = Cons(res, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				}
			} else {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(res->TH)) {
						if (tree_conv && id0) delay = Ref(o2->TH);
						Set_H_TH(o2, value = Integer(o1->THI - res->THI), arcs); def = Cons(o2, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					return minus_one; // deferment.
				}
			}
		} else {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (tree_conv && id0) delay = Ref(o1->TH);
						Set_H_TH(o1, value = Integer(o2->THI + res->THI), arcs); def = Cons(o1, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					return minus_one; // deferment.
				}
			} else {
				return minus_one; // deferment.
			}
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->mul) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (o1->THI * o2->THI != res->THI) return one; // fail.
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					if (IsInteger(o1->TH) && IsInteger(o2->TH)) {
						if (tree_conv && id0) delay = Ref(res->TH);
						Set_H_TH(res, value = Integer(o1->THI * o2->THI), arcs); def = Cons(res, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				}
			} else {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(res->TH)) {
						if (o1->THI == 0 || res->THI / o1->THI == 0) return one; // fail.	
						if (tree_conv && id0) delay = Ref(o2->TH);
						Set_H_TH(o2, value = Integer(res->THI / o1->THI), arcs); def = Cons(o2, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					return minus_one; // deferment.
				}
			}
		} else {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (o2->THI == 0 || res->THI / o2->THI == 0) return one; // fail.	
						if (tree_conv && id0) delay = Ref(o1->TH);
						Set_H_TH(o1, value = Integer(res->THI / o2->THI), arcs); def = Cons(o1, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					return minus_one; // deferment.
				}
			} else {
				return minus_one; // deferment.
			}
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->div) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (o2->THI == 0) return one; // fail.
						if (o1->THI / o2->THI != res->TTHI) return one; // fail.
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					if (IsInteger(o1->TH) && IsInteger(o2->TH)) {
						if (o2->THI == 0) return one; // fail.
						if (tree_conv && id0) delay = Ref(res->TH);
						Set_H_TH(res, value = Integer(o1->THI / o2->THI), arcs); def = Cons(res, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				}
			} else {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(res->TH)) {
						if (res->THI == 0 || o1->THI / res->THI == 0) return one; // fail.	
						if (tree_conv && id0) delay = Ref(o2->TH);
						Set_H_TH(o2, value = Integer(o1->THI / res->THI), arcs); def = Cons(o2, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					return minus_one; // deferment.
				}
			}
		} else {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (o2->THI == 0) return one; // fail.	
						if (tree_conv && id0) delay = Ref(o1->TH);
						Set_H_TH(o1, value = Integer(o2->THI * res->THI), arcs); def = Cons(o1, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					return minus_one; // deferment.
				}
			} else {
				return minus_one; // deferment.
			}
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->mod) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (o2->H) {
				if (res->H) {
					if (IsInteger(o1->TH) && IsInteger(o2->TH) && IsInteger(res->TH)) {
						if (o2->THI == 0) return one; // fail.
						if (o1->THI % o2->THI != res->THI) return one; // fail.
					} else {
						return one; // fail. error("type illegal.", id);
					}
				} else {
					if (IsInteger(o1->TH) && IsInteger(o2->TH)) {
						if (o2->THI == 0) return one; // fail.
						if (tree_conv && id0) delay = Ref(res->TH);
						Set_H_TH(res, value = Integer(o1->THI % o2->THI), arcs); def = Cons(res, def);
					} else {
						return one; // fail. error("type illegal.", id);
					}
				}
			} else {
				return minus_one; // deferment.
			}
		} else {
			return minus_one; // deferment.
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->neg) {
		if (!o1 || !res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (res->H) {
				if (IsInteger(o1->TH) && IsInteger(res->TH)) {
					if (o1->THI != - res->THI) return one; // fail.
				} else {
					return one; // fail. error("type illegal.", id);
				}
			} else {
				if (IsInteger(o1->TH) && IsInteger(o2->TH)) {
					if (tree_conv && id0) delay = Ref(res->TH);
					Set_H_TH(res, value = Integer(- o1->THI), arcs); def = Cons(res, def);
				} else {
					return one; // fail. error("type illegal.", id);
				}
			}
		} else {
			if (res->H) {
				if (IsInteger(res->TH)) {
					if (tree_conv && id0) delay = Ref(o1->TH);
					Set_H_TH(o1, value = Integer(- res->THI), arcs); def = Cons(o1, def);
				} else {
					return one; // fail. error("type illegal.", id);
				}
			} else {
				return minus_one; // deferment.
			}
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->eq || id == p->last_eq) {
		if (!o1 || !o2) return error("number of arguments mismatch", id);
		if (id == p->last_eq) {
			int cnt = out_count(g);
			if (cnt > 1) return minus_one; // deferment.
		}
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (o2->H) {
				if (Compare(o1->TH, o2->TH) != 0) return one; // fail.
			} else {
				if (tree_conv && id0) delay = Ref(o2->TH);
				Set_H_TH(o2, value = o1->TH, arcs); def = Cons(o2, def);
			}
		} else {
			if (o2->H) {
				if (tree_conv && id0) delay = Ref(o1->TH);
				Set_H_TH(o1, value = o2->TH, arcs); def = Cons(o1, def);
			} else {
				if (o1->TH == p->wc_e && o2->TH == NULL) {
					Set2(&o2->TH, o1->TH, arcs); def = Cons(o2, def);
				}
				if (o2->TH == p->wc_e && o1->TH == NULL) {
					Set2(&o1->TH, o2->TH, arcs); def = Cons(o1, def);
				}
				return minus_one; // deferment.
			}
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->ne) {
		if (!o1 || !o2) return error("number of arguments mismatch", id);
		if (o1->H) {
			if (o2->H) {
				if (Compare(o1->TH, o2->TH) == 0) return one; // fail.
			} else {
				return minus_one; // deferment.
			}
		} else {
			return minus_one; // deferment.
		}
	} else if (id == p->gt) {
		if (!o1 || !o2) return error("number of arguments mismatch", id);
		if (o1->H) {
			if (o2->H) {
				if (Compare(o1->TH, o2->TH) <= 0) return one; // fail.
			} else {
				return minus_one; // deferment.
			}
		} else {
			return minus_one; // deferment.
		}
	} else if (id == p->ge) {
		if (!o1 || !o2) return error("number of arguments mismatch", id);
		if (o1->H) {
			if (o2->H) {
				if (Compare(o1->TH, o2->TH) < 0) return one; // fail.
			} else {
				return minus_one; // deferment.
			}
		} else {
			return minus_one; // deferment.
		}
	} else if (id == p->lt) {
		if (!o1 || !o2) return error("number of arguments mismatch", id);
		if (o1->H) {
			if (o2->H) {
				if (Compare(o1->TH, o2->TH) >= 0) return one; // fail.
			} else {
				return minus_one; // deferment.
			}
		} else {
			return minus_one; // deferment.
		}
	} else if (id == p->le) {
		if (!o1 || !o2) return error("number of arguments mismatch", id);
		if (o1->H) {
			if (o2->H) {
				if (Compare(o1->TH, o2->TH) > 0) return one; // fail.
			} else {
				return minus_one; // deferment.
			}
		} else {
			return minus_one; // deferment.
		}
	} else if (id == p->cons) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		AGmNode *ls;
		AGmNode *delay = NULL, *value = NULL;
		AGmNode *delay2 = NULL, *value2 = NULL;
		if (o1->H) {
			if (o2->H) {
				if (res->H) {
					ls = res->TH;
					if (ls && IsList(ls)) {
						if (Compare(ls->H, o1->TH) != 0) return one;
						if (Compare(ls->T, o2->TH) != 0) return one;
					} else {
						return one; // fail.
					}
				} else {
					if (tree_conv && id0) delay = Ref(res->TH);
					Set_H_TH(res, value = Cons(o1->TH, o2->TH), arcs); def = Cons(res, def);
				}
			} else {
				if (res->H) {
					ls = res->TH;
					if (ls && IsList(ls)) {
						if (Compare(ls->H, o1->TH) != 0) return one;
						if (tree_conv && id0) delay = Ref(o2->TH);
						Set_H_TH(o2, value = ls->T, arcs); def = Cons(o2, def);
					} else {
						return one; // fail.
					}
				} else {
					return minus_one; // deferment.
				}
			}
		} else {
			if (o2->H) {
				if (res->H) {
					ls = res->TH;
					if (ls && IsList(ls)) {
						if (Compare(ls->T, o2->TH) != 0) return one;
						if (tree_conv && id0) delay = Ref(o1->TH);
						Set_H_TH(o1, value = ls->H, arcs); def = Cons(o1, def);
					} else {
						return one; // fail.
					}
				} else {
					return minus_one; // deferment.
				}
			} else {
				if (res->H) {
					ls = res->TH;
					if (ls && IsList(ls)) {
						if (tree_conv && id0) delay = Ref(o1->TH);
						Set_H_TH(o1, value = ls->H, arcs); def = Cons(o1, def);
						if (tree_conv && id0) delay2 = Ref(o2->TH);
						Set_H_TH(o2, value2 = ls->T, arcs); def = Cons(o2, def);
					} else {
						return one; // fail.
					}
				} else {
					return minus_one; // deferment.
				}
			}
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
		if (delay2) {
			delay2->count--;
			AGmNode *err = gen_DelayVal(delay2, value2, arcs, id);
			RefUnref(delay2);
			if (err) return err;
		}
	} else if (id == p->list) {
		if (!res) return error("number of arguments mismatch", id);
		if (res->H) {
			if (!IsList(res->TH)) return one; // fail. (type illegal.)
			AGmNode *ls = res->TH;
			for (long i = 1;; i++) {
				o1 = dst_node(r1, i);
				if (o1 == NULL) break;
				if (ls == NULL) return one; // fail. error ("list length mismatch.", id);
				if (o1->H) {
					if (Compare(ls->H, o1->TH) != 0) return one; // fail.
				} else {
					AGmNode *delay = NULL, *value = NULL;
					if (tree_conv && id0) delay = Ref(o1->TH);
					Set_H_TH(o1, value = ls->H, arcs); def = Cons(o1, def);
					if (delay) {
						delay->count--;
						AGmNode *err = gen_DelayVal(delay, value, arcs, id);
						RefUnref(delay);
						if (err) return err;
					}
				}
				ls = ls->T;
			}
		} else {
			AGmNode *ls = NULL;
			AGmNode *delay = NULL, *value = NULL;
			for (long i = 1;; i++) {
				o1 = dst_node(r1, i);
				if (o1 == NULL) break;
				if (o1->H) {
					ls = Nconc(ls, Cons(o1->TH));
				} else {
					Unref(ls); return minus_one; // deferment.
				}
			}
			if (tree_conv && id0) delay = Ref(res->TH);
			Set_H_TH(res, value = ls, arcs); Unref(ls); def = Cons(res, def);
			if (delay) {
				delay->count--;
				AGmNode *err = gen_DelayVal(delay, value, arcs, id);
				RefUnref(delay);
				if (err) return err;
			}
		}
	} else if (id == p->append) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (o2->H) {
				AGmNode *n = Append(o1->TH, o2->TH);
				if (res->H) {
					int flag = Compare(n, res->TH);
					RefUnref(n);
					if (flag) return one; // fail.
				} else {
					if (tree_conv && id0) delay = Ref(res->TH);
					Set_H_TH(res, value = n, arcs); def = Cons(res, def);
				}
			} else {
				return minus_one; // deferment.
			}
		} else {
			return minus_one; // deferment.
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->member) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (o1->H) {
			if (o2->H) {
				AGmNode *n = (Member(o1->TH, o2->TH) ? one : NULL);
				if (res->H) {
					int flag = Compare(n, res->TH);
					RefUnref(n);
					if (flag) return one; // fail.
				} else {
					if (tree_conv && id0) delay = Ref(res->TH);
					Set_H_TH(res, value = n, arcs); def = Cons(res, def);
				}
			} else {
				return minus_one; // deferment.
			}
		} else {
			return minus_one; // deferment.
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->grph) {
		if (tree_conv == 0) return error("graph notation appeared in not tree conversion.", id);
		if (!o1 || !o2 || !o3) return error("number of arguments mismatch", id);
		if (o1->H == NULL || o2->H == NULL) return minus_one; // deferment.
		if (o1->TH == NULL) return one; // fail.
		AGmNode *n = dst_node(o1->TH, o2->TH);
		if (id0 == NULL) { // tree -> graph (AttrGrmr)
			if (o3->H) {
				if (n) {
					if (Compare(n, o3->TH) != 0) return one; // fail.
				} else {
					if (o3->TH) {
						AGmArc *arc = CreateArc(o1->TH, o3->TH, o2->TH);
						arcs = Cons(List(BACK_CREATE_ARC, o1->TH, PtrCell(arc)), arcs);
					}
				}
			} else {
				if (n) {
					Set_H_TH(o3, n, arcs); def = Cons(o3, def);
				} else {
					if (o3->TH != p->wc_e) return minus_one; // deferment.
				}
			}
		} else { // graph -> tree (InvAttrGrmr)
			if (n == NULL) return one; // fail.
			if (o3->H) {
				if (Compare(n, o3->TH) != 0) return one; // fail.
			} else {
				if (o3->TH) {
					AGmNode *err = gen_DelayVal(o3->TH, n, arcs, id);
					if (err) return err;
				}
				Set_H_TH(o3, n, arcs); def = Cons(o3, def);
			}
		}
	} else if (id == p->null_a) {
		if (tree_conv == 0) return error("null arc notation appeared in not tree conversion.", id);
		if (!o1 || !o2) return error("number of arguments mismatch", id);
		if (o1->H == NULL || o2->H == NULL) return minus_one; // deferment.
		if (o1->TH == NULL) return one; // fail.
		AGmNode *n = dst_node(o1->TH, o2->TH);
		if (id0 == NULL) { // tree -> graph (AttrGrmr)
			if (n) return one; // fail.
		} else { // graph -> tree (InvAttrGrmr)
			if (n) return one; // fail.
		}
	} else if (id == p->grph_eq) {
		if (tree_conv == 0) return error("graph_equal notation appeared in not tree conversion.", id);
		if (!o1 || !o2) return error("number of arguments mismatch", id);
		if (id0 == NULL) { // tree -> graph (AttrGrmr)
			if (o1->H) {
				if (o2->H) {
					if (Compare(o1->TH, o2->TH) != 0) return one; // fail.
				} else {
					Set_H_TH(o2, o1->TH, arcs); def = Cons(o2, def);
				}
			} else {
				if (o2->H) {
					Set_H_TH(o1, o2->TH, arcs); def = Cons(o1, def);
				} else {
					return minus_one; // deferment.
				}
			}
		} else { // graph -> tree (InvAttrGrmr)
			if (o2->H == NULL) return minus_one; // deferment.
			if (o1->H) {
				if (Compare(o1->TH, o2->TH) != 0) return one; // fail.
			} else {
				if (o1->TH) {
					AGmNode *err = gen_DelayVal(o1->TH, o2->TH, arcs, id);
					if (err) return err;
				}
				Set_H_TH(o1, o2->TH, arcs); def = Cons(o1, def);
			}
		}

	} else if (id == p->dest_node) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL || o2->H == NULL) return minus_one; // deferment.
		if (o1->TH == NULL) return one; // fail.
		AGmNode *n = dst_node(o1->TH, o2->TH);
		AGmNode *delay = NULL, *value = NULL;
		if (res->H) {
			if (Compare(n, res->TH) != 0) return one; // fail.
		} else {
			if (tree_conv && id0) delay = Ref(res->TH);
			Set_H_TH(res, value = n, arcs); def = Cons(res, def);
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->sorc_node) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL || o2->H == NULL) return minus_one; // deferment.
		if (o1->TH == NULL) return one; // fail.
		AGmNode *n = src_node(o1->TH, o2->TH);
		AGmNode *delay = NULL, *value = NULL;
		if (res->H) {
			if (Compare(n, res->TH) != 0) return one; // fail.
		} else {
			if (tree_conv && id0) delay = Ref(res->TH);
			Set_H_TH(res, value = n, arcs); def = Cons(res, def);
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	
	} else if (id == p->must_be) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL || o2->H == NULL) return minus_one; // deferment.
		if (Compare(o1->TH, o2->TH) != 0) {
			AGmString *s = new AGmString(64);
			AGmFile *f = new AGmFile();
			f->Create(s, AGM_FILE_STR_WRITE);
			f->WriteString("MUST_BE = ");
			DumpGraph(o2->TH, 10, 100, f);
			f->WriteString("GRAPH = ");
			DumpGraph(o1->TH, 10, 100, f);
			AGmNode *err = error(s->Str(), id);
			delete f; delete s;
			return err;
		}
		if (res->H) {
			if (Compare(res->TH, one) != 0) return one; // fail.
		} else {
			Set_H_TH(res, one, arcs); def = Cons(res, def);
		}
	} else if (id == p->dump) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL || o2->H == NULL) return minus_one; // deferment.
		AGmString *s = new AGmString(64);
		AGmFile *f = new AGmFile();
		f->Create(s, AGM_FILE_STR_WRITE);
		DumpGraph(o1->TH, 20, 100, f, (o2->TH ? 1 : 0));
		delete f;
		if (res->H) {
			if (IsString(res->TH)) {
				int flag = s->Compare(res->TH->a.s.v);
				delete s;
				if (flag) return one; // fail.
			} else {
				delete s;
				return one; // fail. error("type illegal.", id);
			}
		} else {
			Set_H_TH(res, String(s), arcs); def = Cons(res, def);
		}
	} else if (id == p->print) {
		if (!o1 || !res) return error("number of arguments mismatch", id);
		if (res->H == NULL) return minus_one; // deferment.
		if (!IsInteger(res->TH)) {
			return error("print return value must be integer.", id);
		}
		int count;
		switch (res->THI) {
		case -1: // always fail.
			break;
		case 0: // execute last and fail.
		case 1: // execute last and success.
			count = out_count(g);
			if (count != 1) return minus_one; // deferment.
			break;
		case 2: // always success.
			break;
		}
		long i;
		for (i = 1;; i++) {
			o1 = dst_node(r1, i);
			if (o1 == NULL) break;
			if (o1->H == NULL) return minus_one; // deferment.
		}
		for (i = 1;; i++) {
			o1 = dst_node(r1, i);
			if (o1 == NULL) break;
			if (IsString(o1->TH)) {
				ADvWindow::Print(o1->TH->a.s.v->Str());
			} else {
				AGmString *s = new AGmString(64);
				AGmFile *f = new AGmFile();
				f->Create(s, AGM_FILE_STR_WRITE);
				DumpList(o1->TH, 100, f);
				ADvWindow::Print(s->Str());
				delete f; delete s;
			}
		}
		switch (res->THI) {
		case -1: // always fail.
		case 0: // execute last and fail.
			return one; // fail.
		case 1: // execute last and success.
		case 2: // always success.
			break;
		}

	} else if (id == p->var) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL || res->H == NULL) return minus_one; // deferment.
		if (!IsInteger(res->TH) || res->THI != 1) return one; // fail.
		if (!IsString(o1->TH)) return error("1st argument must be string", id);
		AGmNode *ls;
		AGmNode *delay = NULL, *value = NULL;
		for (ls = GVAR; ls; ls = ls->T) if (ls->HH == o1->TH) break;
		if (ls) {
			if (o2->H) {
				// verify
				if (Compare(ls->HTH, o2->TH) != 0) return one; // fail.
			} else {
				// get
				if (tree_conv && id0) delay = Ref(o2->TH);
				Set_H_TH(o2, value = ls->HTH, arcs); def = Cons(o2, def);
			}
		} else {
			if (o2->H == NULL) return minus_one; // deferment.
			// set
			Push(GVAR, List(o1->TH, o2->TH)); def = Cons(o1, def);
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}
	} else if (id == p->undef) {
		if (!o1 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL || res->H == NULL) return minus_one; // deferment.
		if (!IsInteger(res->TH)) return one; // fail.
		AGmNode *ls, *prev_ls = NULL;
		for (ls = GVAR; ls; prev_ls = ls, ls = ls->T) if (ls->HH == o1->TH) break;
		if (ls == NULL) {
			if (res->THI == 1) return one; // fail.
		} else {
			if (prev_ls == NULL) {
				Set2(&GVAR, ls->T, arcs);
			} else {
				Set2(&prev_ls->T, ls->T, arcs);
			}
		}
	
	} else if (id == p->count) {
		if (!res) return error("number of arguments mismatch", id);
		AGmNode *delay = NULL, *value = NULL;
		if (res->H) {
			if (!IsInteger(res->TH) || res->THI != call_count) return one; // fail.
		} else {
			if (tree_conv && id0) delay = Ref(res->TH);
			Set_H_TH(res, value = Integer(call_count), arcs); def = Cons(res, def);
		}
		if (delay) {
			delay->count--;
			AGmNode *err = gen_DelayVal(delay, value, arcs, id);
			RefUnref(delay);
			if (err) return err;
		}

	} else if (id == p->seq) {
		AGmNode *o4 = dst_node(r1, 4);
		if (!o1 || !o2 || !o3 || !o4 || !res) return error("number of arguments mismatch", id);
		if (o2->H == NULL || o3->H == NULL || o4->H == NULL || res->H == NULL) return minus_one; // deferment.
		if (res->TH) return error("result must be null", id);
		if (!IsInteger(o3->TH)) return error("3rd variable must be integer", id);
		if (!IsInteger(o4->TH)) return error("4th variable must be integer", id);
		if (o1->H == NULL) {
			Set2(&o1->H, one, arcs);
			Set2(&o1->TH, Integer(o3->THI), arcs);
		} else {
			// overwrite o1
			Set2(&o1->TH, Integer(o1->THI + o4->THI), arcs);
		}

	} else if (id == p->glb_seq) {
		AGmNode *o4 = dst_node(r1, 4);
		AGmNode *o5 = dst_node(r1, 5);
		if (!o1 || !o2 || !o3 || !o4 || !o5 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL || o3->H == NULL || o4->H == NULL || o5->H == NULL || res->H == NULL) return minus_one; // deferment.
		if (!IsString(o1->TH)) return error("1st variable must be string", id);
		if (res->TH) return error("result must be null", id);
		if (!IsInteger(o4->TH)) return error("4th variable must be integer", id);
		if (!IsInteger(o5->TH)) return error("5th variable must be integer", id);

		AGmNode *ls, *num_rep;
		for (ls = GVAR; ls; ls = ls->T) if (ls->HH == o1->TH) break;
		if (ls) {
			num_rep = ls->HTH;
			num_rep->a.i.v += o5->THI;
		} else {
			Push(GVAR, List(o1->TH, num_rep = Integer(o4->THI)));
		}
		if (o2->H == NULL) {
			Set2(&o2->H, one, arcs);
			Set2(&o2->TH, num_rep, arcs);
		} else {
			// overwrite o1
			Set2(&o2->TH, num_rep, arcs);
		}

	} else if (id == p->is_type) {
		if (!o1 || !o2 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL || o2->H == NULL || res->H == NULL) return minus_one; // deferment.
		if (res->TH) return error("result must be null", id);
		if (!IsString(o2->TH)) return error("2nd variable must be string", id);
		const char* s = o2->TH->a.s.v->Str();
		if (strcmp(s, "integer") == 0) {
			if (!IsInteger(o1->TH)) return one; // fail.
		} else if (strcmp(s, "real") == 0) {
			if (!IsReal(o1->TH)) return one; // fail.
		} else if (strcmp(s, "string") == 0) {
			if (!IsString(o1->TH)) return one; // fail.
		} else if (strcmp(s, "list") == 0) {
			if (!IsList(o1->TH)) return one; // fail.
		} else {
			char msg[256];
			art_sprintf_1(msg, 256, "unkown type '%s'", s);
			return error(msg, id);
		}

	} else if (id == p->if_e) {
		if (!o1 || !res) return error("number of arguments mismatch", id);
		if (res->H == NULL) return minus_one; // deferment.
		if (res->TH) return error("result must be null", id);
		if (o1->H) {
			// success.
		} else {
			return one; // fail.
		}

	} else if (id == p->if_ne) {
		if (!o1 || !res) return error("number of arguments mismatch", id);
		if (res->H == NULL) return minus_one; // deferment.
		if (res->TH) return error("result must be null", id);
		if (o1->H) {
			return one; // fail.
		} else {
			// success.
		}

	} else if (id == p->exist) {
		if (!o1 || !res) return error("number of arguments mismatch", id);
		if (o1->H) return error("1st variable must be undefined", id);
		if (res->H == NULL) return minus_one; // deferment.
		if (res->TH) return error("result must be null", id);
		exec_stack->make_exist(this, o1, 1);

	} else if (id == p->not_exist) {
		if (!o1 || !res) return error("number of arguments mismatch", id);
		if (o1->H) return error("1st variable must be undefined", id);
		if (res->H == NULL) return minus_one; // deferment.
		if (res->TH) return error("result must be null", id);
		exec_stack->make_exist(this, o1, 0);

	} else if (id == p->brk_if_found) {
		if (!o1 || !res) return error("number of arguments mismatch", id);
		if (o1->H == NULL) return minus_one; // deferment.
		if (res->H == NULL) return minus_one; // deferment.
		if (res->TH) return error("result must be null", id);
		if (out_count(g) > 1) return minus_one; // deferment.
		return three; // break;

	} else {
		if (!res) return error("number of arguments mismatch", id);
		AGmNode *arg_ls = NULL;
		for (long i = 1;; i++) {
			o1 = dst_node(r1, i);
			if (o1 == NULL) break;
			if (o1->H == NULL) { Unref(arg_ls); return minus_one; } // deferment.
			arg_ls = Nconc(arg_ls, Cons(o1->TH));
		}
		AGmNode *value = p->call(id, arg_ls); Unref(arg_ls);
		if (p->error) {
			RefUnref(value); return error("", NULL);
		}
		if (res->H) {
			if (Compare(res->TH, value) != 0) { RefUnref(value); return one; } // fail.
		} else {
			Set_H_TH(res, value, arcs); def = Cons(res, def);
		}
	}

	rl2 = next_outlink(g, rl1);
	DeleteArc2(rl1, NULL, arcs); resolve_count++;

if (debug_dump) {
DebugDump("After ApplyFunc", g, (rl2 ? rl2->arc->dst: NULL));
}

	return NULL;
}

AGmNode* AGmProlog::ApplySynFunc(AGmNode* g1, AGmLink* rl1, AGmLink*& rl2, AGmNode*& def, AGmNode*& arcs) {

if (debug_dump) {
DebugDump("Before ApplySynFunc", g1, rl1->arc->dst);
}
	char msg[256];

	AGmNode *r1 = rl1->arc->dst;
	AGmNode *id = r1->TH;
	AGmNode *o1 = dst_node(r1, 1);
	AGmNode *res = dst_node2(r1, (AGmNode*)NULL);
	AGmNode *idxs, *value, *er;
	int err;

	if (tree_conv == 0) return error("not in parse_tree <-> graph conversion", id); // error.

	if (id0 == NULL) {
		// tree -> graph
		if (id == p->syn_ret) {
			if (!res) return error("number of arguments mismatch", id);
			if (res->H == NULL) return minus_one; // deferment. 
			p->unset_loc(p->syn_ret_str); p->set_loc(p->syn_ret_str, res->TH);
		} else if (id == p->syn_gen) {
			if (!res) return error("number of arguments mismatch", id);
			if (res->H) {
				if ((id0 && res->TH == NULL) || (id0 == NULL && res->TH)) return one; // fail.
			} else {
				Set_H_TH(res, (id0 ? one : NULL), arcs); def = Cons(res, def);
			}
		} else if (id == p->syn_tree) {
			if (!res) return error("number of arguments mismatch", id);
			if (res->H) {
			} else {
				Set_H_TH(res, NULL, arcs); def = Cons(res, def);
			}
		} else if (id == p->syn_var) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			ASSERT(o1->H);
			idxs = o1->TH;
			err = SynVar(idxs);
			if (err == SYN_VAR_NO_DATA) return one; // fail.
			if (err == FOR_STACK_EMPTY) return minus_one; // deferment. 
			if (p->error) return error("", id);
			value = GetStackValue();
			if (res->H) {
				if (Compare(res->TH, value) != 0) return one; // fail.
			} else {
				Set_H_TH(res, value, arcs); def = Cons(res, def);
			}
		} else if (id == p->if_str) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			if (res->H == NULL) return minus_one; // deferment.
			ASSERT(o1->H);
			idxs = o1->TH;
			err = SynVar(idxs);
			if (err == SYN_VAR_NO_DATA) return one; // fail.
			if (err == FOR_STACK_EMPTY) return minus_one; // deferment. 
			if (p->error) return error("", id);
			value = GetStackValue();
			if (value && IsList(value) && value->T) {
				value = one;
			} else {
				value = NULL;
			}
			if (Compare(res->TH, value) != 0) return one; // fail.
		} else if (id == p->switch_str) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			ASSERT(o1->H);
			idxs = o1->TH;
			err = SynVar(idxs);
			if (err == SYN_VAR_NO_DATA) return one; // fail.
			if (err == FOR_STACK_EMPTY) return minus_one; // deferment. 
			if (p->error) return error("", id);
			value = GetSwitchId();
			if (IsInteger(value)) {
				err = (int)value->a.i.v; RefUnref(value);
				if (err == FOR_STACK_EMPTY) return minus_one; // deferment.
				ASSERT(0);
			}
			if (p->error) return error("", id);
			if (res->H) {
				if (Compare(res->TH, value) != 0) return one; // fail.
			} else {
				Set_H_TH(res, value, arcs); def = Cons(res, def);
			}
		} else if (id == p->for_str) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			if (res->H == NULL) return minus_one; // deferment.
			ASSERT(o1->H);
			idxs = o1->TH;
			err = SynVar(idxs);
			if (err == SYN_VAR_NO_DATA) return one; // fail.
			if (err == FOR_STACK_EMPTY) return minus_one; // deferment.
			if (p->error) return error("", id);
			value = GetParseTreeValue();
			if (value == NULL || !IsList(value)
				|| (value->H != p->str_repeat_0 && value->H != p->str_repeat_1)) return error("arg not {}*, {}+ .", id);
			value = value->T;
			int n = Length(value);
			AGmNode *for_idx = dst_node(for_stack, idxs);
			if (res->TH) {
				if (n == 0) return one; // fail.
				if (for_idx == NULL) {
					AGmNode *idx = Integer(0);
					set_dst_node(for_stack, idxs, idx);
					arcs = Cons(List(BACK_CREATE_ARC2, for_stack, idx), arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA (for-begin): index = %d, value = 0x%lx, call_count = %d\n", 0, AL_PTR(value), call_count);
ADvWindow::Print(msg);
DumpList("AAA idxs = ", idxs);
}
				} else if (for_idx->a.i.v < n - 1) {
					for_idx->a.i.v++;
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA (for+): index = %ld, value = 0x%lx, call_count = %d\n", for_idx->a.i.v, AL_PTR(value), call_count);
ADvWindow::Print(msg);
DumpList("AAA idxs = ", idxs);
}
				} else {
					return one; // fail.
				}
			} else {
				if (for_idx) {
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA (for-end): last_index = %ld, value = 0x%lx, call_count = %d\n", for_idx->a.i.v, AL_PTR(value), call_count);
ADvWindow::Print(msg);
DumpList("AAA idxs = ", idxs);
}
					set_dst_node(for_stack, idxs, NULL);
				}
			}
		} else if (id == p->inv_for_str) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			if (res->H == NULL) return minus_one; // deferment.
			ASSERT(o1->H);
			idxs = o1->TH;
			err = SynVar(idxs);
			if (err == SYN_VAR_NO_DATA) return one; // fail.
			if (err == FOR_STACK_EMPTY) return minus_one; // deferment. 
			if (p->error) return error("", id);
			value = GetParseTreeValue();
			if (value == NULL || !IsList(value)
				|| (value->H != p->str_repeat_0 && value->H != p->str_repeat_1)) return error("arg not {}*, {}+ .", id);
			value = value->T;
			int n = Length(value);
			AGmNode *for_idx = dst_node(for_stack, idxs);
			if (res->TH) {
				if (n == 0) return one; // fail.
				if (for_idx == NULL) {
					AGmNode *idx = Integer(n - 1);
					set_dst_node(for_stack, idxs, idx);
					arcs = Cons(List(BACK_CREATE_ARC2, for_stack, idx), arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA (inv_for-begin): index = %d, value = 0x%lx, call_count = %d\n", 0, AL_PTR(value), call_count);
ADvWindow::Print(msg);
DumpList("AAA idxs = ", idxs);
}
				} else if (for_idx->a.i.v > 0) {
					for_idx->a.i.v--;
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA (inv_for+): index = %ld, value = 0x%lx, call_count = %d\n", for_idx->a.i.v, AL_PTR(value), call_count);
ADvWindow::Print(msg);
DumpList("AAA idxs = ", idxs);
}
				} else {
					return one; // fail.
				}
			} else {
				if (for_idx) {
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA (inv_for-end): last_index = %ld, value = 0x%lx, call_count = %d\n", for_idx->a.i.v, AL_PTR(value), call_count);
ADvWindow::Print(msg);
DumpList("AAA idxs = ", idxs);
}
					set_dst_node(for_stack, idxs, NULL);
				}
			}
		} else {
			return error("unknown syntax function.", id);
		}
	} else {
		// graph -> tree
		if (id == p->syn_ret) {
			if (!res) return error("number of arguments mismatch", id);
			value = p->get_loc(p->syn_ret_str);
			if (value == p->undefined) return error("variable $$ have no value.", id);
			if (res->H) {
				if (Compare(res->TH, value) != 0) return one; // fail.
			} else {
				Set_H_TH(res, value, arcs); def = Cons(res, def);
			}
		} else if (id == p->syn_gen) {
			if (!res) return error("number of arguments mismatch", id);
			if (res->H) {
				if ((id0 && res->TH == NULL) || (id0 == NULL && res->TH)) return one; // fail.
			} else {
				Set_H_TH(res, (id0 ? one : NULL), arcs); def = Cons(res, def);
			}
		} else if (id == p->syn_tree) {
			if (!res) return error("number of arguments mismatch", id);
			if (res->H) return one; // fail.
			Set_H_TH(res, ptree0, arcs);
		} else if (id == p->syn_var) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			ASSERT(o1->H);
			idxs = o1->TH;
			ASSERT(IsInteger(o1->TTH) && IsInteger(o1->TTTH) && IsInteger(o1->TTTTH));
			int sv_if = (int)o1->TTHI, sv_switch = (int)o1->TTTHI, sv_for = (int)o1->TTTTHI;
			AGmNode *ls = dst_node(gen_status, idxs);
			if (ls) {
				if (sv_if && ls->H == NULL) return minus_one; // deferment.
				if (sv_switch && ls->TH == NULL) return minus_one; // deferment.
				if (sv_for && ls->TTH == NULL) return minus_one; // deferment.
			} else {
				if (sv_if || sv_switch || sv_for) return minus_one; // deferment.
			}
			AGmNode *delay_for = NULL;
			err = gen_SynVar(idxs, arcs, 0, &delay_for);
			er = gen_SynVarRet(err, id);
			if (er) { RefUnref(delay_for); return er; } // fail or deferment.
			if (res->H) {
				RefUnref(delay_for);
				if (res->TH == NULL) return sub_graph_null_error(idxs, p->syntax_id_stack, id);
				AGmNode *tree = p->GetSubParseTree(this->id, grmr, res->TH, call_count, 1);
				if (p->error) return error("", id); // error.
				if (tree == NULL) return one; // fail.
				Set2(addr, tree, arcs);
			} else {
if (debug_syn2) {
if (delay_for) DumpList("AAA delayed syn_var idxs = ", idxs);
}
				if (delay_for) arcs = Cons(List(BACK_DELAYED, delay_for), arcs);
				for (AGmNode *ls2 = delay_for; ls2; ls2 = ls2->T) {
					ls2->H->TTTHI++;
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA inc_delayed_val: this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls2->H), call_count); 
ADvWindow::Print(msg);
DumpList("AAA inc_delayed_val = ", ls2->H, 20, 20);
}
				}
				Set2(&res->TH,
					List(p->delay_val, this->id, grmr, PtrCell(addr), delay_for, idxs, p->syntax_id_stack),
					arcs);
			}
		} else if (id == p->if_str) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			if (res->H == NULL) return minus_one; // deferment. 
			ASSERT(o1->H);
			idxs = o1->TH;
			AGmNode *ls = dst_node(gen_status, idxs);
			if (ls == NULL) {
				ls = List((AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, Integer(0), (AGmNode*)NULL);
				arcs = Cons(List(BACK_GEN_STATUS, gen_status, ls), arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA apply (if): this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA if idxs = ", idxs);
}
				set_dst_node(gen_status, idxs, ls);
			}
			if (res->TH) {
				Set(ls->H, one);
				err = gen_SynVar(idxs, arcs, 1);
				er = gen_SynVarRet(err, id);
				if (er) return er; // fail or deferment.
			} else {
				Set(ls->H, zero);
				err = gen_SynVar(idxs, arcs, 1);
				er = gen_SynVarRet(err, id);
				if (er) return er; // fail or deferment.
			}
		} else if (id == p->switch_str) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			if (res->H == NULL) return minus_one;  // deferment.
			ASSERT(o1->H);
			idxs = o1->TH;
			AGmNode *ls = dst_node(gen_status, idxs);
			if (ls == NULL) {
				ls = List((AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, Integer(0), (AGmNode*)NULL);
				arcs = Cons(List(BACK_GEN_STATUS, gen_status, ls), arcs);
				set_dst_node(gen_status, idxs, ls);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA apply (switch): this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA switch idxs = ", idxs);
}
			}
			Set2(&ls->TH, res->TH, arcs);
			err = gen_SynVar(idxs, arcs, 2);
			er = gen_SynVarRet(err, id);
			if (er) return er; // fail or deferment.
		} else if (id == p->for_str) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			if (res->H == NULL) return minus_one;  // deferment.
			ASSERT(o1->H);
			idxs = o1->TH;
			AGmNode *ls = dst_node(gen_status, idxs);
			if (ls == NULL) {
				ls = List((AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, Integer(0), (AGmNode*)NULL);
				arcs = Cons(List(BACK_GEN_STATUS, gen_status, ls), arcs);
				set_dst_node(gen_status, idxs, ls);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA apply (for): this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA for idxs = ", idxs);
}
			}
			AGmNode *for_idx = ls->TTTTH;
			if (res->TH) {
				Set(ls->TTH, List(one, NULL));
				if (for_idx == NULL) {
					Set(ls->TTTTH, List(Integer(0), NULL, NULL));
if (debug_syn2) {
art_sprintf_4(msg, 256, "AAA (for-begin): index = %d, this = 0x%lx, addr = 0x%lx, call_count = %d\n", 0, AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA (for idxs) = ", idxs);
DumpList("AAA ls = ", ls, 20, 20);
}
				} else {
					for_idx->H->a.i.v++;
if (debug_syn2) {
art_sprintf_4(msg, 256, "AAA (for+): index = %ld, this = 0x%lx, addr = 0x%lx, call_count = %d\n", for_idx->H->a.i.v, AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA (for idxs) = ", idxs);
DumpList("AAA ls = ", ls, 20, 20);
}
				}
				err = gen_SynVar(idxs, arcs, 3);
				er = gen_SynVarRet(err, id);
				if (er) {
					if (for_idx) {
						for_idx->H->a.i.v--;
					} else {
						Set(ls->TTTTH, NULL);
					}
					return er; // fail or deferment.
				}
				if (for_idx) {
					arcs = Cons(List(BACK_FOR_INDEX, for_idx), arcs);
				}
			} else {
				if (ls->TTTHI > 0) {
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA wait (for_delayed_val): this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA (for idxs) = ", idxs);
DumpList("AAA wait (for_delayed_val) = ", ls, 20, 20);
}
					return minus_one; // deferment.
				}
				if (for_idx == NULL) {
					Set(ls->TTH, List(one, minus_one));
				} else {
					Set(ls->TTH, List(one, zero));
				}
				err = gen_SynVar(idxs, arcs, 3);
				er = gen_SynVarRet(err, id);
				if (er) return er; // fail or deferment.
if (debug_syn2) {
if (for_idx) {
art_sprintf_4(msg, 256, "AAA (for-end): last_index = %ld, this = 0x%lx, addr = 0x%lx, call_count = %d\n", for_idx->H->a.i.v, AL_PTR(this), AL_PTR(ls), call_count);
} else {
art_sprintf_3(msg, 256, "AAA (for-empty): this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
}
ADvWindow::Print(msg);
DumpList("AAA (for idxs) = ", idxs);
DumpList("AAA ls = ", ls, 20, 20);
}
			}
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA apply (for) [Success]: this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
}
		} else if (id == p->inv_for_str) {
			if (!o1 || !res) return error("number of arguments mismatch", id);
			if (res->H == NULL) return minus_one;  // deferment.
			ASSERT(o1->H);
			idxs = o1->TH;
			AGmNode *ls = dst_node(gen_status, idxs);
			if (ls == NULL) {
				ls = List((AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, Integer(0), (AGmNode*)NULL);
				arcs = Cons(List(BACK_GEN_STATUS, gen_status, ls), arcs);
				set_dst_node(gen_status, idxs, ls);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA apply (inv_for): this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA (inv_for idxs) = ", idxs);
}
			}
			AGmNode *for_idx = ls->TTTTH;
			if (res->TH) {
				Set(ls->TTH, List(minus_one, NULL));
				if (for_idx == NULL) {
					Set(ls->TTTTH, List(Integer(0), NULL, NULL));
if (debug_syn2) {
art_sprintf_4(msg, 256, "AAA (inv_for-begin): index = %d, this = 0x%lx, addr = 0x%lx, call_count = %d\n", 0, AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA (inv_for idxs) = ", idxs);
DumpList("AAA ls = ", ls, 20, 20);
}
				} else {
					for_idx->H->a.i.v++;
if (debug_syn2) {
art_sprintf_4(msg, 256, "AAA (inv_for+): index = %ld, this = 0x%lx, addr = 0x%lx, call_count = %d\n", for_idx->H->a.i.v, AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA (inv_for idxs) = ", idxs);
DumpList("AAA ls = ", ls, 20, 20);
}
				}
				err = gen_SynVar(idxs, arcs, 3);
				er = gen_SynVarRet(err, id);
				if (er) {
					if (for_idx) {
						for_idx->H->a.i.v--;
					} else {
						Set(ls->TTTTH, NULL);
					}
					return er; // fail or deferment.
				}
				if (for_idx) {
					arcs = Cons(List(BACK_FOR_INDEX, for_idx), arcs);
				}
			} else {
				if (ls->TTTHI > 0) {
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA wait (inv_for_delayed_val): this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA (inv_for idxs) = ", idxs);
DumpList("AAA wait (inv_for_delayed_val) = ", ls, 20, 20);
}
					return minus_one; // deferment.
				}
				if (for_idx == NULL) {
					Set(ls->TTH, List(minus_one, minus_one));
				} else {
					Set(ls->TTH, List(minus_one, zero));
				}
				err = gen_SynVar(idxs, arcs, 3);
				er = gen_SynVarRet(err, id);
				if (er) return er; // fail or deferment.
if (debug_syn2) {
if (for_idx) {
art_sprintf_4(msg, 256, "AAA (inv_for-end): last_index = %ld, this = 0x%lx, addr = 0x%lx, call_count = %d\n", for_idx->H->a.i.v, AL_PTR(this), AL_PTR(ls), call_count);
} else {
art_sprintf_3(msg, 256, "AAA (inv_for-empty): this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
}
ADvWindow::Print(msg);
DumpList("AAA (inv_for idxs) = ", idxs);
DumpList("AAA ls = ", ls, 20, 20);
}
			}
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA apply (inv_for) [Success]: this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
}
		} else {
			return error("unknown syntax function.", id);
		}
	}
	rl2 = next_outlink(g1, rl1);
	DeleteArc2(rl1, NULL, arcs); resolve_count++;

if (debug_dump) {
DebugDump("Alter ApplySynFunc", g1, (rl2 ? rl1->arc->dst : NULL));
}

	return NULL;
}

AGmNode* AGmProlog::error(const char* s, AGmNode* id) {
	AGmString *err = NULL;
	if (p->error) {
		err = new AGmString(p->error->a.s.v);
		err->Append("\n");
	} else {
		err = new AGmString(64);
	}
	err->Append(s);
	if (id) {
		err->Append(" at ");
		err->Append(id->a.s.v);
	}

	AGmNode *ls;
	err->Append("\n");
	for (ls = pred_stack; ls; ls = ls->T) {
		err->Append("\t");
		err->Append(ls->H->a.s.v);
		err->Append("\n");
	}

	if (tree_conv) {
		err->Append("tree converion error: syntax_id = ");
		if (id0) {
			err->WriteString(id0->a.s.v);
		} else {
			err->WriteString(p->syntax_id_val->a.s.v);
		}
		err->Append("\n");
		for (ls = p->syntax_id_stack; ls; ls = ls->T) {
			err->Append("\t");
			err->Append(ls->H->a.s.v);
			err->Append("\n");
		}
	}
	Set(p->error, String(err)); return p->error;
}

AGmNode* AGmProlog::sub_graph_null_error(AGmNode* idxs, AGmNode* stack, AGmNode* id) {
	AGmString *str = new AGmString("tree compose error: source graph is null.");
	if (id) {
		str->Append(" at ");
		str->Append(id->a.s.v);
	}
	str->Append("\n");
	str->Append("syn_var idx = ");
	int count = 0;
	str->WriteList(idxs, count);
	str->Append("\n");
	for (AGmNode *ls = stack; ls; ls = ls->T) {
		str->Append("\t");
		str->Append(ls->H->a.s.v);
		str->Append("\n");
	}
	return String(str);
}

// ======== Read Parse Tree

int AGmProlog::SynVar(AGmNode *idxs) {
	int err;
	for (;;) {
		err = GetFirst(idxs);
		if (err) break;
		if (is_last_idx()) break;
		for (;;) {
			err = GetNext();
			if (err) break;
			if (is_last_idx()) break;	
		}
		break;
	}
	Unref(idxs2); idxs2 = NULL;
	return err;
}

int AGmProlog::GetFirst(AGmNode* idxs) {
	last_id = NULL;
	id = p->syntax_id_val;
	grmr = p->grammar_val;
	ptree = p->parse_tree_val;
	stack = p->comp_grmr_val_stack;
	Set(itr, DstItr(for_stack));
	init_idx(idxs);

if (debug_syn) {
char msg[256];
AGmString *ss; WriteSynVar(ss);
art_sprintf_3(msg, 256, "### ======== syntax_id = %s, variable = %s, call_count = %d\n", id->a.s.v->Str(), ss->Str(), call_count);
delete ss;
ADvWindow::Print(msg);
ADvWindow::Print("Enter GetFirst\n");
}

	int i, idx = next_idx(), len = Length(grmr);
	if (idx > len) return SYN_VAR_NO_DATA;

	ptree = ptree->T;
	for (i = 0; stack && i < len - idx; i++) stack = stack->T;
	if (stack) stack = stack->H;
	for (i = 1; i < idx; i++) { ptree = ptree->T; grmr = grmr->T; }
	ptree = ptree->H; grmr = grmr->H;
	if (IsList(grmr)) {
		id = grmr->H;
	} else {
		id = NULL;
	}
	if (id == p->str_braket_1) {
		grmr = grmr->T;
		GetSwitch();
		if (p->error) return 1;
	}
if (debug_syn) {
ADvWindow::Print("Leave GetFirst\n");
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
DumpList("## stack = ", stack, 8, 8);
}
	return 0;
}

int AGmProlog::GetNext() {
if (debug_syn) {
ADvWindow::Print("Enter GetNext\n");
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
DumpList("## stack = ", stack, 8, 8);
}
	AGmString *err;
	int i, idx, len;
	if (id == NULL) {
		WriteSynVar(err); err->Append(" target is regex.");
		p->Error(err->Str()); delete err; return 1;
	}
	
	if (stack == NULL) {
		WriteSynVar(err); err->Append(" unexpected switch. switch id '");
		err->Append(last_id ? last_id->a.s.v->Str() : ""); err->Append("' must be checked.");
		p->Error(err->Str()); delete err; return 1;
	}

	stack = stack->T;
	ptree = ptree->T;
	if (ptree == NULL) return SYN_VAR_NO_DATA;

	if (id == p->str_repeat_1 || id == p->str_repeat_0) {
		// for, inv_for
		AGmNode *count_n = itr_next(itr);
		if (count_n == NULL) return FOR_STACK_EMPTY;
		int i, count = (int)count_n->a.i.v;
		for (i = 0; i < count; i++) { stack = stack->T; ptree = ptree->T; }
		if (stack == NULL) {
			WriteSynVar(err); err->Append(" val_stack empty.");
			p->Error(err->Str()); delete err; return 1;
		}
		stack = stack->HT;
		grmr = grmr->T;
		ptree = ptree->H;
		GetSwitch();
		if (p->error) return 1;
		grmr = grmr->TH;
		ptree = ptree->T;
	} else if (id == p->str_braket_0) {
		grmr = grmr->T;
		ptree = ptree->H;
		GetSwitch();
		if (p->error) return 1;
		grmr = grmr->TH;
		ptree = ptree->T;
		stack = stack->HT;
	} else {
		if (grmr->T) {
			grmr = grmr->TH;
		} else {
			grmr = dst_node2(p->syntax_unit_ids, grmr->H);
		}
	}

	idx = next_idx(), len = Length(grmr);
	if (idx > len) return SYN_VAR_NO_DATA;

	for (i = 1; i < idx; i++) {
		if (stack == NULL) {
			WriteSynVar(err); err->Append(" value stack empty(1).");
			p->Error(err->Str()); delete err; return 1;
		}
		stack = stack->T; ptree = ptree->T; grmr = grmr->T;
	}
	if (stack == NULL) {
		WriteSynVar(err); err->Append(" value stack empty(2).");
		p->Error(err->Str()); delete err; return 1;
	}
	stack = stack->H; ptree = ptree->H; grmr = grmr->H;

	if (IsList(grmr)) {
		id = grmr->H;
	} else {
		id = NULL;
	}
	if (id == p->str_braket_1) {
		grmr = grmr->T;
		GetSwitch();
		if (p->error) return 1;
	}
if (debug_syn) {
ADvWindow::Print("Leave GetNext\n");
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
DumpList("## stack = ", stack, 8, 8);
}
	return 0;
}

void AGmProlog::GetSwitch() {
if (debug_syn) {
ADvWindow::Print("Enter GetSwitch\n");
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
}
	AGmNode *h = ptree->H;
	AGmNode *ls; AGmString *err;
	for (ls = grmr; ls; ls = ls->T) {
		if (ls->HH == h) break;
	}
	if (ls) {
		grmr = ls->H; id = grmr->H;
	} else {
		WriteSynVar(err);
		err->Append(" not found switch case literal: id(in tree) = '");
		err->Append(h->a.s.v); err->Append("'.");
		p->Error(err->Str()); delete err; return;
	}
if (debug_syn) {
char msg[256];
art_sprintf_1(msg, 256, "Leave GetSwitch: id = %s\n", (id ? id->a.s.v->Str() : "null"));
ADvWindow::Print(msg);
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
}
	last_id = id;
}

AGmNode *AGmProlog::GetStackValue() {
	if (p->error) return NULL;
if (debug_syn3) {
DumpGraph("#### value = ", stack, 8, 8, NULL, 1);
}
	return stack;
}

AGmNode *AGmProlog::GetParseTreeValue() {
	if (p->error) return NULL;
	return ptree;
}

AGmNode *AGmProlog::GetSwitchId() {
	if (id == p->str_repeat_0 || id == p->str_repeat_1) {
		// for, inv_for
		int i, count = 0;
		AGmNode *count_n = itr_next(itr);
		if (count_n) count = (int)count_n->a.i.v;
		for (i = 0; i < count; i++) ptree = ptree->T;
		grmr = grmr->T;
		if (ptree->T) {
			ptree = ptree->TH;
			GetSwitch();
			if (p->error) return NULL;
		} else {
			id = NULL;
		}
	} else if (id == p->str_braket_0) {
		grmr = grmr->T;
		if (ptree->T) {
			ptree = ptree->TH;
			GetSwitch();
			if (p->error) return NULL;
		} else {
			id = NULL;
		}
	}
if (debug_syn3) {
DumpList("#### id = ", id);
}
	return id;
}

// ======== Write Parse Tree

int AGmProlog::gen_SynVar(AGmNode *idxs, AGmNode*& arcs, int modif, AGmNode** delay_for) {
	int err;
	for (;;) {
		err = gen_GetFirst(idxs);
		if (err) break;
		if (is_last_idx()) break;
		for (;;) {
			err = gen_GetNext(arcs, delay_for);
			if (err) break;
			if (is_last_idx()) break;	
		}
		break;
	}
	if (err == 0 && modif == 2) err = gen_GetSwitch(arcs);
	if (err == 0 && modif > 0) err = gen_ModifTree(arcs, modif);
	Unref(idxs2); idxs2 = NULL;
	return err;
}

int AGmProlog::gen_ModifTree(AGmNode*& arcs,int modif) {
	char msg[256];

	AGmNode *ls = dst_node(gen_status, idxs0);
	if (ls && modif == 1) {
		if (ptree->H != p->str_braket_0) return NOT_MATCH_BRAKET_0;
		if (ls->H == one) {
		} else if (ls->H == zero) {
			Set2(&ptree->T, NULL, arcs);
		} else {
			p->Error("if not match {}? ."); return 1;
		}
	} else if (ls && modif == 3) {
		if (ptree->H != p->str_repeat_0 && ptree->H != p->str_repeat_1) return NOT_MATCH_REPEAT;
		AGmNode *count_n = ls->TTTTH;
		if (ls->TTHH == one && ls->TTH->TH == NULL) {
			AGmNode **ad, *n = count_n->TH;
			if (n == NULL) {
				Set(count_n->TH, ListCopy(ptree->T, 1));
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA ## (for-begin) this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA ## (for idxs) = ", idxs0);
DumpList("AAA ## ls = ", ls, 20, 20);
}
			} else {
				for (ad = &count_n->TTH; (*ad); ad = &(*ad)->T) ;
				Set2(ad, ptree->T, arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA ## (for+) this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA ## (for idxs) = ", idxs0);
DumpList("AAA ## ls = ", ls, 20, 20);
}
				Set2(&ptree->T, ListCopy(n, 1), arcs);
			}
		} else if (ls->TTHH == minus_one && ls->TTH->TH == NULL) {
			AGmNode **ad, *n = count_n->TH;
			if (n == NULL) {
				Set(count_n->TH, ListCopy(ptree->T, 1));
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA ## (inv_for-begin) this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA ## (inv_for idxs) = ", idxs0);
DumpList("AAA ## ls = ", ls, 20, 20);
}
			} else {
				for (ad = &ptree->T; (*ad); ad = &(*ad)->T) ;
				Set2(ad, count_n->TTH, arcs);
				Set2(&count_n->TTH, ptree->T, arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA ## (inv_for+) this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA ## (inv_for idxs) = ", idxs0);
DumpList("AAA ## ls = ", ls, 20, 20);
}
				Set2(&ptree->T, ListCopy(n, 1), arcs);
			}
		} else if (ls->TTHH == one && ls->TTH->TH == zero) {
			AGmNode **ad;
			for (ad = &count_n->TTH; (*ad); ad = &(*ad)->T) ;
			Set2(ad, ptree->T, arcs);

			Set2(&ptree->T, count_n->TTH, arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA ## (for-end) this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA ## (for idxs) = ", idxs0);
DumpList("AAA ## ls = ", ls, 20, 20);
}
			Set2(&ls->TTTTH, NULL, arcs);
		} else if (ls->TTHH == minus_one && ls->TTH->TH == zero) {
			AGmNode **ad;
			for (ad = &ptree->T; (*ad); ad = &(*ad)->T) ;
			Set2(ad, count_n->TTH, arcs);
			Set2(&count_n->TTH, ptree->T, arcs);

			Set2(&ptree->T, count_n->TTH, arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA ## (inv_for-end) this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA ## (inv_for idxs) = ", idxs0);
DumpList("AAA ## ls = ", ls, 20, 20);
}
			Set2(&ls->TTTTH, NULL, arcs);
		} else if (ls->TTHH == one && ls->TTH->TH == minus_one) {
			Set2(&ptree->T, NULL, arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA ## (for-empty) this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA ## (for idxs) = ", idxs0);
DumpList("AAA ## ls = ", ls, 20, 20);
}
			Set2(&ls->TTTTH, NULL, arcs);
		} else if (ls->TTHH == minus_one && ls->TTH->TH == minus_one) {
			Set2(&ptree->T, NULL, arcs);
if (debug_syn2) {
art_sprintf_3(msg, 256, "AAA ## (inv_for-empty) this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls), call_count);
ADvWindow::Print(msg);
DumpList("AAA ## (inv_for idxs) = ", idxs0);
DumpList("AAA ## ls = ", ls, 20, 20);
}
			Set2(&ls->TTTTH, NULL, arcs);
		} else {
			p->Error("for, inv_for not match {}* , {}+ ."); return 1;
		}
	}
	return 0;
}

int AGmProlog::gen_GetFirst(AGmNode* idxs) {
	char msg[256];

	addr = NULL;

	id = id0;
	grmr = grmr0;
	ptree = ptree0;
	Set(itr, DstItr(for_stack));
	init_idx(idxs);

if (debug_syn2) {
char msg[256];
AGmString *ss; WriteSynVar(ss);
art_sprintf_3(msg, 256, "### ======== syntax_id = %s, variable = %s, call_count = %d\n", id->a.s.v->Str(), ss->Str(), call_count);
delete ss;
ADvWindow::Print(msg);
ADvWindow::Print("Enter gen_GetFirst\n");
}
	ptree = ptree->T;

	int i, idx = next_idx(), len = Length(grmr);
	if (idx > len) {
if (debug_syn2) {
art_sprintf_1(msg, 256, "gen_GetFirst: syn_var_no_data. this = 0x%lx\n", AL_PTR(this));
ADvWindow::Print(msg);
DumpList("idxs2 = ", idxs2);
}
		return SYN_VAR_NO_DATA;
	}
	for (i = 1; i < idx; i++) { ptree = ptree->T; grmr = grmr->T; }

	addr = &ptree->H;
	ptree = ptree->H; grmr = grmr->H;

	if (IsList(grmr)) {
		id = grmr->H;
	} else {
		id = NULL;
	}

if (debug_syn2) {
ADvWindow::Print("Leave gen_GetFirst\n");
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
DumpList("## *addr = ", *addr, 8, 8);
}
	return 0;
}

int AGmProlog::gen_GetNext(AGmNode*& arcs, AGmNode** delay_for) {
	char msg[256];

if (debug_syn2) {
ADvWindow::Print("Enter gen_GetNext\n");
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
}
	if (id == p->str_braket_1) {
		grmr = grmr->T;
		int err = gen_GetSwitch(arcs);
		if (err) return err;
		if (p->error) return 1;
	}

	int i, idx, len;
	if (id == NULL) {
if (debug_syn2) {
art_sprintf_1(msg, 256, "gen_GetNext: target is regex. this = 0x%lx\n", AL_PTR(this));
ADvWindow::Print(msg);
}
		return TARGET_IS_REGEX;
	}
	AGmNode *ls = dst_node(gen_status, idxs2);
	if (id == p->str_repeat_1 || id == p->str_repeat_0) {
		// for, inv_for
		if (ls == NULL || ls->TTH == NULL) {
if (debug_syn2) {
art_sprintf_2(msg, 256, "gen_GetNext: not_yet_for_modif. this = 0x%lx, addr = 0x%lx\n", AL_PTR(this), AL_PTR(ls));
ADvWindow::Print(msg);
DumpList("idxs2 = ", idxs2);
DumpList("ls = ", ls);
}
			return NOT_YET_FOR_MODIF;
		}

		if (delay_for) *delay_for = Cons(ls, *delay_for);

		if (ls->TTTTH == NULL) {
if (debug_syn2) {
art_sprintf_2(msg, 256, "gen_GetNext: for_stack_empty. this = 0x%lx, addr = 0x%lx\n", AL_PTR(this), AL_PTR(ls));
ADvWindow::Print(msg);
DumpList("idxs2 = ", idxs2);
DumpList("ls = ", ls);
}
			return FOR_STACK_EMPTY;
		}
		if (ls->TTHH == one) {
		} else if (ls->TTHH == minus_one) {
		} else if (ls->TTH->TH == zero) {
if (debug_syn2) {
art_sprintf_2(msg, 256, "gen_GetNext: end_of_for. this = 0x%lx, addr = 0x%lx\n", AL_PTR(this), AL_PTR(ls));
ADvWindow::Print(msg);
DumpList("idxs2 = ", idxs2);
DumpList("ls = ", ls);
}
			return END_OF_FOR;
		} else {
			ASSERT(0);
		}
		grmr = grmr->T;
		int err = gen_GetSwitch(arcs);
		if (err) return err;
		if (p->error) return 1;
		if (grmr->T) {
			grmr = grmr->TH;
		} else {
			grmr = dst_node2(p->syntax_unit_ids, grmr->H);
		}
		ptree = ptree->TH;
	} else if (id == p->str_braket_0) {
		grmr = grmr->T;
		int err = gen_GetSwitch(arcs);
		if (err) return err;
		if (p->error) return 1;
		if (grmr->T) {
			grmr = grmr->TH;
		} else {
			grmr = dst_node2(p->syntax_unit_ids, grmr->H);
		}
		ptree = ptree->TH;
	} else {
		if (grmr->T) {
			grmr = grmr->TH;
		} else {
			grmr = dst_node2(p->syntax_unit_ids, grmr->H);
		}
		ptree = ptree->TH;
	}

	idx = next_idx(), len = Length(grmr);
	if (idx > len) return SYN_VAR_NO_DATA;
	for (i = 1; i < idx; i++) { ptree = ptree->T; grmr = grmr->T; }

	addr = &ptree->H;
	ptree = ptree->H; grmr = grmr->H;

	if (IsList(grmr)) {
		id = grmr->H;
	} else {
		id = NULL;
	}
if (debug_syn2) {
ADvWindow::Print("Leave gen_GetNext\n");
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
DumpList("## *addr = ", *addr, 8, 8);
}
	return 0;
}

int AGmProlog::gen_GetSwitch(AGmNode*& arcs) {
	char msg[256];

if (debug_syn2) {
ADvWindow::Print("Enter gen_GetSwitch\n");
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
}
	AGmString *err;
	if (ptree->T == NULL) {
if (debug_syn2) {
art_sprintf_1(msg, 256, "gen_GetSwitch: [ERROR] optional already cutted., this = 0x%lx\n", AL_PTR(this));
ADvWindow::Print(msg);
DumpList("idxs2 = ", idxs2);
DumpList("id = ", id);
DumpList("ptree = ", ptree, 8, 8);
}
		WriteSynVar(err); err->Append(" optional already cutted.");
		p->Error(err->Str()); delete err; return -100;
	}
	if (Length(grmr) == 1) {
		ASSERT(grmr->HH == ptree->THH);
		id = grmr->HH;
		grmr = grmr->H;
		ptree = ptree->TH;
	} else {
		AGmNode *ls = dst_node(gen_status, idxs2), *ls2, *ls3;
		if (ls == NULL || ls->TH == NULL) {
if (debug_syn2) {
art_sprintf_2(msg, 256, "gen_GetSwitch: not_yet_switch_id. this = 0x%lx, addr = 0x%lx\n", AL_PTR(this), AL_PTR(ls));
ADvWindow::Print(msg);
DumpList("idxs2 = ", idxs2);
DumpList("ls = ", ls);
}
			return NOT_YET_SWITCH_ID;
		}
		id = ls->TH;
		for (ls2 = grmr; ls2; ls2 = ls2->T) {
			if (!IsList(ls2->H)) continue;
			if (ls2->HH == id) break;
		}
		int done = 0;
		if (Length(ptree->T) == 1) {
			ls3 = ptree->T; done = 1;
		} else {
			for (ls3 = ptree->T; ls3; ls3 = ls3->T) {
				if (ls3->HH == id) break;
			}
		}
		if (!(ls2 && ls3)) {
if (debug_syn2) {
art_sprintf_2(msg, 256, "gen_GetSwitch: [ERROR] switch id not found., this = 0x%lx, addr = 0x%lx\n", AL_PTR(this), AL_PTR(ls));
ADvWindow::Print(msg);
DumpList("idxs2 = ", idxs2);
DumpList("ls = ", ls);
DumpList("id = ", id);
DumpList("ptree = ", ptree, 8, 8);
}
			WriteSynVar(err); err->Append(" switch id not found.");
			p->Error(err->Str()); delete err; return -100;
		}
		if (done == 0) {
			Set2(&ptree->T, Cons(ls3->H), arcs);
		}
		grmr = ls2->H;
		ptree = ptree->TH;
	}
if (debug_syn2) {
art_sprintf_1(msg, 256, "Leave gen_GetSwitch: id = %s\n", (id ? id->a.s.v->Str() : "null"));
ADvWindow::Print(msg);
}
if (debug_syn3) {
DumpList("## id = ", id);
DumpList("## grmr = ", grmr, 8, 8);
DumpList("## ptree = ", ptree, 8, 8);
}
	return 0;
}

AGmNode* AGmProlog::gen_SynVarRet(int err, AGmNode* id) {
	if (err == SYN_VAR_NO_DATA) return one; // fail.	
	if (err == FOR_STACK_EMPTY) return minus_one; // deferment. 
	if (err == NOT_YET_FOR_MODIF) return minus_one; // deferment.	
	if (err == END_OF_FOR) return one; // fail.
	if (err == NOT_YET_SWITCH_ID) return minus_one; // deferment.	
	if (err == TARGET_IS_REGEX) return one; // fail.
	if (err == NOT_MATCH_BRAKET_0) return one; // fail.
	if (err == NOT_MATCH_REPEAT) return one; // fail.
	if (p->error) return error("", id);
	return NULL;
}

AGmNode* AGmProlog::gen_DelayVal(AGmNode* delay, AGmNode* value, AGmNode*& arcs, AGmNode* id) {
	if (delay == NULL || !IsList(delay) || delay->H != p->delay_val) return NULL;

	if (value == NULL) {
		AGmNode *idxs = delay->TTTTT->H;
		AGmNode *stack = delay->TTTTT->TH;
		return sub_graph_null_error(idxs, stack, id);
	}

	AGmNode **ad = (AGmNode**)delay->TTTH->a.p.ptr;
	AGmNode *tree = p->GetSubParseTree(delay->TH, delay->TTH, value, call_count);
	if (p->error) return error("", id); // error.
	if (tree == NULL) return one; // fail.

	AGmNode *delay_for = delay->TTTTH;
	if (delay_for) arcs = Cons(List(BACK_DEC_OF_DELAYD, delay_for), arcs);
	for (AGmNode *ls2 = delay_for; ls2; ls2 = ls2->T) {
		ls2->H->TTTHI--;
if (debug_syn2) {
char msg[256];
art_sprintf_3(msg, 256, "AAA dec_delayed_val: this = 0x%lx, addr = 0x%lx, call_count = %d\n", AL_PTR(this), AL_PTR(ls2->H), call_count); 
ADvWindow::Print(msg);
DumpList("AAA dec_delayed_val = ", ls2->H, 20, 20);
}
	}
	Set2(ad, tree, arcs);
	return NULL;
}

// ======== Read & Write Parse Tree

void AGmProlog::init_idx(AGmNode* idxs) {
	this->idxs0 = this->idxs = idxs;
}

int AGmProlog::next_idx() {
	int idx = -1;
	if (idxs) {
		idx = (int)idxs->HI; idxs = idxs->T;
		idxs2 = Nconc(idxs2, Cons(Integer(idx)));
	}
	return idx;
}

int AGmProlog::is_last_idx() {
	return (idxs == NULL);
}

void AGmProlog::WriteSynVar(AGmString*& err) {
	err = new AGmString("$");
	AGmNode *ls;
	for (ls = idxs0; ls; ls = ls->T) {
		err->Append("["); err->WriteInteger(ls->HI); err->Append("]");
	}
}
