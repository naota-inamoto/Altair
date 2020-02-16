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
#include <gdm/big_int.h>
#include <gdm/debug.h>
#include <net/crypt.h>
#include <symbol/pro_lisp.h>
#include <math/almath.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

//#define	_DEBUG_PM_REP1
//#define	_DEBUG_PM_REP2
//#define	_DEBUG_PM_REP3
//#define	_DEBUG_PM_REP4

AGmNode* AGmProlog::ToInternalPatturnMatchAndReplace(AGmNode* g, AGmNode* tree, AGmNode* obj_db, AGmNode* syn_db) {
#ifdef	_DEBUG_PM_REP1
DumpGraph("Enter: ToInternalPatturnMatchAndReplace: g = ", g, 10, 1000, NULL, 1);
//DumpGraph("obj_db = ", obj_db, 10, 100, NULL, 1);
#endif
	AGmNode *rel, *obj; long idx = 1;
	rel = List(Integer(PRO_PM_REP), p->pm_rep, NULL);
	CreateArc(g, rel);
	obj = ToInternalArg(g, tree->H, obj_db, syn_db);
	if (IsString(obj)) return obj;
	CreateArc(rel, obj, Integer(idx++));
	AGmNode *ls;
	for (ls = tree->TH->T; ls; ls = ls->T) {
		obj = ToInternalArg(g, ls->HTH, obj_db, syn_db);
		if (IsString(obj)) return obj;
		CreateArc(rel, obj, Integer(idx++));
	}

	CreateArc(rel, obj_db, p->obj_db_str);

	AGmNode *bace;
	CreateArc(rel, bace = Cons(p->bace), p->bace);

	AGmNode *tree2; idx = 1;
	for (tree2 = tree->TTHT; tree2; tree2 = tree2->T) {
		AGmNode *before = tree2->HT->H->T;
		AGmNode *after = tree2->HT->TH->T;
		AGmNode *common = tree2->HT->TTH->THT;
		AGmNode *tail = tree2->HT->TTTH->THT;

		AGmNode *before_and_after = NULL;
		AGmNode *err = BACTs(before, after, common, tail, obj_db, syn_db, before_and_after);
		if (IsString(err)) return err;

		CreateArc(bace, before_and_after, Integer(idx++));
	}

	AGmNode *fwh, *tree3, *tree4, *g2; idx = 1;
	CreateArc(bace, fwh = Cons(p->p_final), p->p_final);
	for (tree3 = tree->TTTHT->HT; tree3; tree3 = tree3->T) {
		CreateArc(fwh, g2 = Cons(NULL), Integer(idx++));
		for (tree4 = tree3->H; tree4; tree4 = tree4->TT) {
			if (tree4->THT) {
				AGmNode *err = ToInternalGoals(g2, tree4->THT, obj_db, syn_db);
				if(IsString(err)) {
					return err;
				}
			} else {
				break;
			}
		}
	}

#ifdef	_DEBUG_PM_REP1
DumpGraph("Leave: ToInternalPatturnMatchAndReplace: g = ", g, 20, 1000, NULL, 1);
#endif

	return rel;
}
AGmNode* AGmProlog::BACTs(AGmNode* before, AGmNode* after, AGmNode* common, AGmNode* tail,
		AGmNode* obj_db, AGmNode* syn_db, AGmNode*& before_and_after) {
	AGmNode *err;

	AGmNode *b_nodes = Cons(p->b_nodes), *b_arcs = Cons(p->b_arcs);
	AGmNode *a_nodes = Cons(p->a_nodes), *a_arcs = Cons(p->a_arcs);
	AGmNode *b_stat = Cons(p->blk_grph);
	AGmNode *a_stat = Cons(p->blk_grph);
	int invariant = 0;
	AGmNode *ls;

	status = 0;
	err = PmAndRep(NULL, NULL, b_nodes, b_arcs, b_stat, before->H, obj_db, syn_db, invariant);
	if(IsString(err)) { RefUnref(b_nodes, b_arcs, a_nodes, a_arcs, b_stat, a_stat); return err; }
	for (ls = before->THT; ls; ls = ls->T) {
		err = PmAndRep(NULL, NULL, b_nodes, b_arcs, b_stat, ls->HTH, obj_db, syn_db, invariant);
		if(IsString(err)) { RefUnref(b_nodes, b_arcs, a_nodes, a_arcs, b_stat, a_stat); return err; }
	}

	status = 1;
	err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, a_stat, after->H, obj_db, syn_db, invariant);
	if(IsString(err)) { RefUnref(b_nodes, b_arcs, a_nodes, a_arcs, b_stat, a_stat); return err; }
	for (ls = after->THT; ls; ls = ls->T) {
		err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, a_stat, ls->HTH, obj_db, syn_db, invariant);
		if(IsString(err)) { RefUnref(b_nodes, b_arcs, a_nodes, a_arcs, b_stat, a_stat); return err; }
	}

	before_and_after = Cons(NULL);
		CreateArc(before_and_after, b_stat, minus_one);
		CreateArc(before_and_after, a_stat, zero);
		CreateArc(before_and_after, Integer(invariant), one);
		CreateArc(before_and_after, b_nodes, two);
		CreateArc(before_and_after, b_arcs, three);
		CreateArc(before_and_after, a_nodes, four);
		CreateArc(before_and_after, a_arcs, five);

	AGmNode *tree, *tree2, *gls, *g;
	int idx;

	status = 2;
	idx = 1;
	CreateArc(before_and_after, gls = Cons(p->cwh), p->cwh);
	for (tree = common; tree; tree = tree->T) {
		CreateArc(gls, g = Cons(NULL), Integer(idx++));
		for (tree2 = tree->H; tree2; tree2 = tree2->TT) {
			if (tree2->THT) {
				err = ToInternalGoals(g, tree2->THT, obj_db, syn_db);
				if(IsString(err)) {
					RefUnref(b_nodes, b_arcs, a_nodes, a_arcs);
					RefUnref(before_and_after);
					return err;
				}
			} else {
				break;
			}
		}
	}

	status = 3;
	idx = 1;
	CreateArc(before_and_after, gls = Cons(p->twh), p->twh);
	for (tree = tail; tree; tree = tree->T) {
		CreateArc(gls, g = Cons(NULL), Integer(idx++));
		for (tree2 = tree->H; tree2; tree2 = tree2->TT) {
			if (tree2->THT) {
				err = ToInternalGoals(g, tree2->THT, obj_db, syn_db);
				if(IsString(err)) {
					RefUnref(b_nodes, b_arcs, a_nodes, a_arcs);
					RefUnref(before_and_after);
					return err;
				}
			} else {
				break;
			}
		}
	}

	return err;
}
AGmNode* AGmProlog::PmAndRep(
	AGmNode* b_nodes, AGmNode *b_arcs, AGmNode* a_nodes, AGmNode* a_arcs, AGmNode* stat,
	AGmNode* tree, AGmNode* obj_db, AGmNode *syn_db, int& invariant) {

	long num; AGmNode *nn, *err, *tree2;

#ifdef	_DEBUG_PM_REP2
DumpList("Enter::PmAndRep: tree = ", tree, 80, 200);
#endif

	if (tree->H == p->invariant) {  // invariant
		invariant = 1;

	} else if (tree->H == p->opt_grph) { // opt_grph
		num = (int)tree->THI;
		nn = Ref(List(p->opt_grph, Integer(num), Integer(0)));
		err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree->TTH, obj_db, syn_db, invariant);
		if (IsString(err)) { Unref(nn); return err; }
		CreateArc(stat, nn);
		Unref(nn);

	} else if (tree->H == p->blk_grph) { // blk_grph
		num = (int)tree->THI;
		nn = Ref(List(p->blk_grph, Integer(num)));
		err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree->TTH, obj_db, syn_db, invariant);
		if (IsString(err)) { Unref(nn); return err; }
		for (tree2 = tree->TTTHT; tree2; tree2 = tree2->T) {
			err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree2->HTH, obj_db, syn_db, invariant);
			if (IsString(err)) { Unref(nn); return err; }
		}
		if (tree->TTTTHI != num) {
			Unref(nn); return String("level mismatch '[[[' and ']]]'");
		}
		CreateArc(stat, nn);
		Unref(nn);

	} else if (tree->H == p->rpt_grph) { // rpt_grph
		num = (int)tree->THI;
		nn = Ref(List(p->rpt_grph, Integer(num), Integer(0)));
		err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree->TTH, obj_db, syn_db, invariant);
		if (IsString(err)) { Unref(nn); return err; }
		for (tree2 = tree->TTTHT; tree2; tree2 = tree2->T) {
			err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree2->HTH, obj_db, syn_db, invariant);
			if (IsString(err)) { Unref(nn); return err; }
		}
		if (tree->TTTTHI != num) {
			Unref(nn); return String("level mismatch '<<<' and '>>>'");
		}
		CreateArc(stat, nn);
		Unref(nn);

	} else if (tree->H == p->chc_grph) { // chc_grph
		num = (int)tree->THI;
		nn = Ref(List(p->chc_grph, Integer(num), Integer(0)));
		err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree->TTH, obj_db, syn_db, invariant);
		if (IsString(err)) { Unref(nn); return err; }
		for (tree2 = tree->TTTHT; tree2; tree2 = tree2->T) {
			err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree2->HTH, obj_db, syn_db, invariant);
			if (IsString(err)) { Unref(nn); return err; }
		}
		if (tree->TTTTHI != num) {
			Unref(nn); return String("level mismatch '[%' and ']%'");
		}
		CreateArc(stat, nn);
		Unref(nn);

	} else if (tree->H == p->chc_before) { // chc_before
		AGmNode *obj_db2 = GraphCopy(obj_db), *syn_db2 = GraphCopy(syn_db), *g;
		num = (int)tree->THI;
		nn = Ref(List(p->chc_before, Integer(num), obj_db2, syn_db2));
		CreateArc(nn, g = Cons(NULL));
		err = ToInternalGoals(g, tree->TT, obj_db2, syn_db2);
		if (IsString(err)) { Unref(nn, g); return err; }
		CreateArc(stat, nn);
		Unref(nn);

	} else if (tree->H == p->chc_after) { // chc_after
		num = (int)tree->THI;
		nn = Ref(List(p->chc_after, Integer(num)));
		err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree->TTH, obj_db, syn_db, invariant);
		if (IsString(err)) { Unref(nn); return err; }
		for (tree2 = tree->TTTHT; tree2; tree2 = tree2->T) {
			err = PmAndRep(b_nodes, b_arcs, a_nodes, a_arcs, nn, tree2->HTH, obj_db, syn_db, invariant);
			if (IsString(err)) { Unref(nn); return err; }
		}
		CreateArc(stat, nn);
		Unref(nn);

	} else if (tree->H == p->sub_graph) { // sub_graph
		return AGmProlog::sub_graph_stat(
			b_nodes, b_arcs, a_nodes, a_arcs, stat, tree, obj_db);

	} else if (tree->H == p->node) {
		// node_only
		AGmNode *sn = NULL, *sv = NULL;
		AGmNode *s, *d, *a; int ai;
		sn = tree->TH;
		if (tree->TTHT != NULL) {
			sv = tree->TTHT->HTH;
			if (sv) {
				sv = eval_exp(sv, obj_db);
				ASSERT(sv != NULL);
			} else {
				sv = p->wc_e;
			}
		} else {
			sv = p->wc_e;
		}
		s = List(sn, sv, Integer(0), p->undefined, Integer(exec_stack->level), Integer(0));
		d = NULL;
		a = NULL;
		ai = 0;
		AGmNode* err = add_s_d_a(b_nodes, b_arcs, a_nodes, a_arcs, s, d, a, ai, obj_db);
		if(IsString(err)) return err;
		AGmNode *nn;
		CreateArc(stat, nn = Cons(p->node));
		CreateArc(nn, s, p->grph_src);

	} else {
		ASSERT(0);
	}

	return NULL;
}

AGmNode* AGmProlog::add_s_d_a(
		AGmNode* nodes_0, AGmNode* arcs_0,
		AGmNode* nodes, AGmNode* arcs,
		AGmNode*& s, AGmNode*& d, AGmNode* a,
		int ai, AGmNode* obj_db, AGmNode** arc_a) {
	AGmNode *sn = s->H;
	AGmNode *src = NULL, *dst = NULL;
	if (sn) {
		AGmNode *n = NULL;
		if (nodes_0) {
			n = dst_node2(nodes_0, sn);
		}
		if (n == NULL) {
			AGmNode *v = dst_node2(nodes, sn);
			if (v == NULL) {
				CreateArc(nodes, s, sn);
				AGmNode* obj = dst_node2(obj_db, sn);
				if (obj) {
					set_dst_node(s, p->eq, obj);
				}
				src = s;
			} else {
				src = v;
			}
		} else {
			AGmNode *v = dst_node2(nodes, sn);
			if (v == NULL) {
				CreateArc(nodes, n, sn);
				AGmNode* obj = dst_node2(obj_db, sn);
				if (obj) {
					set_dst_node(n, p->eq, obj);
				}
			}
			src = n;
			src->TTHI = 1;
		}
	} else {
		CreateArc(nodes, s, NULL);
		src = s;
	}

	if (d) {
		AGmNode *dn = d->H;
		if (dn) {
			AGmNode *n = NULL;
			if (nodes_0) {
				n = dst_node2(nodes_0, dn);
			}
			if (n == NULL) {
				AGmNode *v = dst_node2(nodes, dn);
				if (v == NULL) {
					CreateArc(nodes, d, dn);
					AGmNode* obj = dst_node2(obj_db, dn);
					if (obj) {
						set_dst_node(d, p->eq, obj);
					}
					dst = d;
				} else {
					dst = v;
				}
			} else {
				AGmNode *v = dst_node2(nodes, dn);
				if (v == NULL) {
					CreateArc(nodes, n, dn);
					AGmNode* obj = dst_node2(obj_db, dn);
					if (obj) {
						set_dst_node(n, p->eq, obj);
					}
				}
				dst = n;
				dst->TTHI = 1;
			}
		} else {
			CreateArc(nodes, d, NULL);
			dst = d;
		}
	}

	if (a) {
		AGmNode *an = a->H, *av = a->TH;
		AGmNode *arc = List(an, av, Integer(ai),
			Integer(0), p->undefined, Integer(exec_stack->level), Integer(0));
		CreateArc(arcs, arc, an);
		if (arc_a) *arc_a = arc;
		if (an) {
			if (arcs_0) {
				AGmNode *vvv = dst_node2(arcs_0, an);
				if (vvv) {
					vvv->TTTHI = 1;
					arc->TTTHI = 1;
					CreateArc(arc, vvv, p->comm_arc);
				}
			}
		}
	}

	RefUnref(s, d, a);

	s = src; d = dst;

	return NULL;
}

AGmNode *AGmProlog::sub_graph_stat(
		AGmNode* b_nodes, AGmNode *b_arcs, AGmNode* a_nodes, AGmNode* a_arcs, AGmNode* stat,
		AGmNode* tree, AGmNode* obj_db, AGmNode* ns, AGmNode* as) {
	AGmNode *sn = NULL, *sv = NULL;
	AGmNode *dn = NULL, *dv = NULL;
	AGmNode *an = NULL, *av = NULL;
	AGmNode *s, *d, *a; int ai=0;

	AGmNode *src = tree->TH;
	if (src->H == p->node) {
		sn = src->TH;
		if (sn) {
			AGmNode *obj = dst_node2(obj_db, sn);
			if (obj == NULL) {
				CreateArc(obj_db, List((AGmNode*)NULL, (AGmNode*)NULL), sn);
			}
		}
		if (src->TTHT != NULL) {
			sv = src->TTHT->HTH;
			if (sv) {
				sv = eval_exp(sv, obj_db);
				if (p->error) return p->error;
				ASSERT(sv != NULL);
			}
		} else {
			sv = p->wc_e;
		}
	} else {
		sn = NULL;
		sv = src;
		if (sv) {
			sv = eval_exp(sv, obj_db);
			if (p->error) return p->error;
			ASSERT(sv != NULL);
		} else {
			sv = p->wc_e;
		}
	}
	AGmNode *dst =tree->TTTH;
	if (dst->H == p->node) {
		dn = dst->TH;
		if (dn) {
			AGmNode *obj = dst_node2(obj_db, dn);
			if (obj == NULL) {
				CreateArc(obj_db, List((AGmNode*)NULL, (AGmNode*)NULL), dn);
			}
		}
		if (dst->TTHT != NULL) {
			dv = dst->TTHT->HTH;
			if (dv) {
				dv = eval_exp(dv, obj_db);
				if (p->error) return p->error;
				ASSERT(dv != NULL);
			}
		} else {
			dv = p->wc_e;
		}
	} else {
		dn = NULL;
		dv = dst;
		if (dv) {
			dv = eval_exp(dv, obj_db);
			if (p->error) return p->error;
			ASSERT(dv != NULL);
		} else {
			dv = p->wc_e;
		}
	}

	AGmNode *attr = tree->TTH;
	AGmNode* ah = attr->H;
	if (ah == p->rel_1_1) {
		ai = REL_1_1;
	} else if (ah == p->rel_1_n) {
		ai = REL_1_N;
	} else if (ah == p->inv_rel_1_1) {
		ai = INV_REL_1_1;
	} else if (ah == p->inv_rel_1_n) {
		ai = INV_REL_1_N;
	} else {
		ASSERT(0);
	}
	if (attr->T) {
		AGmNode *arc = attr->TH;
		if (arc->H == p->arc) {
			an = arc->TH;
			if (an) {
				AGmNode *obj = dst_node2(obj_db, an);
				if (obj == NULL) {
					CreateArc(obj_db, List((AGmNode*)NULL, (AGmNode*)NULL), an);
				}
			}
			if (arc->TTHT != NULL) {
				av = arc->TTHT->HTH;
				if (av) {
					av = eval_exp(av, obj_db);
					if (p->error) return p->error;
					ASSERT(av != NULL);
				}
			} else {
				av = p->wc_e;
			}
		} else {
			an = NULL;
			av = arc;
			if (av) {
				av = eval_exp(av, obj_db);
				if (p->error) return p->error;
				ASSERT(av != NULL);
			} else {
				av = p->wc_e;
			}
		}
	}
	
	s = List(sn, sv, Integer(0), p->undefined, Integer(exec_stack->level), Integer(0));
	d = List(dn, dv, Integer(0), p->undefined, Integer(exec_stack->level), Integer(0));
	a = List(an, av);
	AGmNode *arc = NULL;
	AGmNode* err = add_s_d_a(b_nodes, b_arcs, a_nodes, a_arcs, s, d, a, ai, obj_db, &arc);
	if(IsString(err)) return err;
	AGmNode *nn;
	CreateArc(stat, nn = Cons(p->sub_graph));
	CreateArc(nn, s, p->grph_src);
	CreateArc(nn, d, p->grph_dst);
	CreateArc(nn, arc, p->arc);
	if (ns) {
		CreateArc(ns, s);
		CreateArc(ns, d);
	}
	if (as)
		CreateArc(as, arc);
	return NULL;
}

AGmNode* AGmProlog::eval_exp(AGmNode* exp, AGmNode* obj_db) {
	AGmNode *id = exp->H;
	if (id == p->null) {
		return NULL;
	} else if (id == p->const_i || id == p->const_s) {
		return exp->TH;

	} else if (id == p->var) {
		AGmNode *name = exp->TH;
		AGmNode *obj = dst_node2(obj_db, name);
		if (obj == NULL) {
			obj = List((AGmNode*)NULL, (AGmNode*)NULL);
			CreateArc(obj_db, obj, name);
			return List(p->var, name);
		}
		if (obj->H) {
			return obj->TH;
		} else {
			return List(p->var, name);
		}

	} else if (id == p->ls_1) {
		AGmNode *list = NULL, *tail = NULL;
		AGmNode *e1 = exp->TH;
		AGmNode *idid = e1->H;

		if (idid == p->wc_e) {
			list = Nconc(list, Cons(p->wc_e));
		} else if (idid == p->wc_t) {
			list = Nconc(list, p->wc_t);
		} else if (idid == p->t) {
			list = Nconc(list, tail = eval_exp(e1->TH, obj_db));
		} else {
			list = Nconc(list, Cons(eval_exp(e1, obj_db)));
		}
		if (tail != p->wc_e && tail != p->wc_t && tail != p->t && !IsList(tail)) {
			Unref(list);
			return error("list tail is not list(1): tail = ", tail);
		}

		AGmNode *ls;
		for (ls = exp->TTHT; ls; ls = ls->T) {
			AGmNode *en = ls->HTH, *tail = NULL;
			AGmNode *idid = en->H;
			if (idid == p->wc_e) {
				list = Nconc(list, Cons(p->wc_e));
			} else if (idid == p->wc_t) {
				list = Nconc(list, p->wc_t);
			} else if (idid == p->t) {
				list = Nconc(list, tail = eval_exp(en->TH, obj_db));
			} else {
				list = Nconc(list, Cons(eval_exp(en, obj_db)));
			}
			if (tail != p->wc_e && tail != p->wc_t && tail != p->t && !IsList(tail)) {
				Unref(list);
				return error("list tail is not list(2): tail = ", tail);
			}
		}
		if (list) list->count--;
		return list;

	} else {
		return error("no spport expression identifier: id = ", id);
	}
}

int AGmProlog::unify(AGmNode* dst, AGmNode* n, AGmNode* obj_db) {
	AGmNode *v = dst->TH;
	if (v && IsList(v) && v->T == NULL && v->HH == p->var && n && IsList(n) && n->T == NULL) {
		Set2(&dst->TH, n->H, exec_stack->arcs);
		Set2(&dst->TTTH, n->H, exec_stack->arcs);
		AGmNode *nm = v->HH;
		AGmNode *obj = dst_node2(obj_db, nm);
		if (obj) {
			Set_H_TH(obj, n->H, exec_stack->arcs);
		}
		return 0;
	}
	if (v == p->wc_e) {
		Set2(&dst->TH, n, exec_stack->arcs);
		Set2(&dst->TTTH, n, exec_stack->arcs);
		return 0;
	}
	if (n == NULL || !IsList(n)) {
		AGmNode *obj = NULL;
		if (v && IsList(v) && v->H == p->var) {
			obj = dst_node2(obj_db, v->TH);
			if (obj) {
				if (obj->H) {
					v = obj->TH;
				} else {
					Set_H_TH(obj, n, exec_stack->arcs);
					Set2(&dst->TH, n, exec_stack->arcs);
					return 0;
				}
			}
		}
		if (Compare(v, n) == 0) {
			Set2(&dst->TTTH, n, exec_stack->arcs);
			return 0;
		} else {
			return 1;
		}
	}
	if (v == NULL || !IsList(v)) {
		return 1;
	}
	if (unify_sub(&dst->TH, n, obj_db)) {
		return 1;
	} else {
		Set2(&dst->TTTH, n, exec_stack->arcs);
		return 0;
	}
}
int AGmProlog::unify_sub(AGmNode** d, AGmNode* n, AGmNode* obj_db) {
	AGmNode **ls1, *ls2;
	for (ls1 = d, ls2 = n; *ls1 && ls2; ls1 = &(*ls1)->T, ls2 = ls2->T) {
		AGmNode *v1 = (*ls1)->H;
		AGmNode *v2 = ls2->H;
		if (v1 == p->wc_e) {
			Set2(&(*ls1)->H, v2, exec_stack->arcs);
			continue;
		}
		if (v2 == NULL || !IsList(v2)) {
			AGmNode *obj = NULL;
			if (v1 && IsList(v1) && v1->H == p->syn_ret_str) {
				obj = dst_node2(obj_db, v1->TH);
				if (obj) {
					if (obj->H) {
						v1 = obj->TH;
					} else {
						Set_H_TH(obj, v2, exec_stack->arcs);
						Set2(&(*ls1)->H, v2, exec_stack->arcs);
						continue;
					}
				}
			}
			if (Compare(v1, v2) == 0) {
				Set2(&(*ls1)->H, v2, exec_stack->arcs);
				continue;
			} else {
				return 1;
			}
		}
		if (v1 == NULL || !IsList(v1)) {
			return 1;
		}
		if (unify_sub(&(*ls1)->H, v2, obj_db)) {
			return 1;
		}
	}
	if (*ls1 || ls2) return 1;
	return 0;
}
int AGmProlog::unify2(AGmNode* attr, AGmNode* av, AGmNode* obj_db) {
	if (attr == NULL || !IsList(attr)) {
		AGmNode *obj = NULL;
		if (av && IsList(av) && av->H == p->syn_ret_str) {
			obj = dst_node2(obj_db, av->TH);
			if (obj) {
				if (obj->H) {
					av = obj->TH;
				} else {
					Set_H_TH(obj, attr, exec_stack->arcs);
					return 0;
				}
			}
		}
		if (Compare(attr, av) == 0) {
			return 0;
		} else {
			return 1;
		}
	}
	if (av == NULL || !IsList(av)) {
		return 1;
	}
	if (unify_sub2(attr, av, obj_db)) {
		return 1;
	} else {
		return 0;
	}
}
int AGmProlog::unify_sub2(AGmNode* attr, AGmNode* av, AGmNode* obj_db) {
	AGmNode *ls1, *ls2;
	for (ls1 = attr, ls2 = av; ls1 && ls2; ls1 = ls1->T, ls2 = ls2->T) {
		AGmNode *attr1 = ls1->H;
		AGmNode *av2 = ls2->H;
		if (attr1 == NULL || !IsList(attr1)) {
			AGmNode *obj = NULL;
			if (av2 && IsList(av2) && av2->H == p->syn_ret_str) {
				obj = dst_node2(obj_db, av2->TH);
				if (obj) {
					if (obj->H) {
						av2 = obj->TH;
					} else {
						Set_H_TH(obj, attr1, exec_stack->arcs);
						continue;
					}
				}
			}
			if (Compare(attr1, av2) == 0) {
				continue;
			} else {
				return 1;
			}
		}
		if (av2 == NULL || !IsList(av2)) {
			return 1;
		}
		if (unify2(ls1->H, av2, obj_db)) {
			return 1;
		}
	}
	if (ls1 || ls2) return 1;
	return 0;
}
AGmNode* AGmProlog::eval(AGmNode* n, AGmNode* obj_db) {
	if (n == NULL)
		return NULL;
	if (!IsList(n)) {
		return ListCopy(n, 1);
	}
	if (n->H == p->syn_ret_str && n->T && n->TT == NULL) {
		AGmNode *obj = dst_node2(obj_db, n->TH);
		if (obj && obj->H) {
			return obj->TH;
		}
	}
	AGmNode *ls, *list = NULL;
	for (ls = n; ls; ls = ls->T) {
		list = Nconc(list, Cons(eval(ls->H, obj_db)));
	}
	if (list) list->count--;
	return list;
}

AGmNode* AGmProlog::ApplyPmAndRep(AGmNode* g1, AGmLink*& rl1) {
if (debug_dump) {
DebugDump("Before ApplyPmAndRep", g1, rl1->arc->dst);
}
	AGmGdb *gdb = new AGmGdb();

	AGmNode *error = NULL;
	gdb->Init(p, this, g1, rl1);
	gdb->RepBeforeAfter();
	error = gdb->err;
	if (IsString(error)) {
		delete gdb;
		return error;
	}
	gdb->Final();

if (debug_dump) {
DebugDump("After ApplyPmAndRep", gdb->g1, (gdb->rl2 ? gdb->rl2->arc->dst: NULL));
}
	rl1 = gdb->rl1;
	error = gdb->err;
	delete gdb;
	return error;
}

AGmGdb::AGmGdb() {
	p = NULL;
	prolog = NULL;
	g1 = NULL;
	rl1 = NULL;
	rl2 = NULL;
	rel = NULL;
	obj_db = NULL;
	bace = NULL;
	err = NULL;
	l1 = NULL;

	match = 0;
	no_rep = 0;
	to_be_modified = 0;
	no_more_rep = 0;

	before_and_after = NULL;
	b_stat = NULL;
	a_stat = NULL;
	invariant = NULL;
	b_nodes = NULL;
	b_arcs = NULL;
	a_nodes = NULL;
	a_arcs = NULL;
	cwh = NULL;
	twh = NULL;

	arc = NULL;
	an = NULL;
	av = NULL;
	ai = -1;
	src = NULL;
	dst = NULL;
	n1 = NULL;
	n = NULL;
	lnk = NULL;
}
AGmGdb::~AGmGdb() {
}

void AGmGdb::Init(AGmProLisp* p, AGmProlog* prolog, AGmNode* g1, AGmLink* rl1) {
	this->p = p;
	this->prolog = prolog;
	this->g1 = g1;
	this->rl1 = rl1;
	rel = rl1->arc->dst;
	prolog->PushExecStack(new AGmExecStack(OP_NOP));
	obj_db = dst_node2(rel, p->obj_db_str);
	prolog->exec_stack->obj_db = obj_db;
	bace = dst_node2(rel, p->bace);
	ASSERT(bace != NULL);
}
void AGmGdb::Final() {
	AGmNode *final = dst_node2(bace, p->p_final); // ### TODO TODO TODO
#if	_DEBUG_PM_REP4
DumpGraph("final = ", final, 10, 1000);
#endif
	rl2 = next_outlink(g1, rl1);
	if (to_be_modified) {
		prolog->DeleteArc2(rl1, NULL, prolog->exec_stack->arcs); prolog->resolve_count++;
		rl1 = rl2;
		// ### Is it better to insert Cut() ???
	} else {
		err = prolog->error("no match subgraph.", p->pm_rep);
	}
	prolog->PopExecStack();
}
void AGmGdb::RepBeforeAfter() {
	AGmLink* l; long idx = 1;
	to_be_modified = 0;
	// repeat before-after-pairs
	for (l = next_outlink(bace, NULL, idx++); l; l = next_outlink(bace, l, idx++)) {
		before_and_after = l->arc->dst;
		b_stat = dst_node2(before_and_after, minus_one);
		a_stat = dst_node2(before_and_after, zero);
		invariant = dst_node2(before_and_after, one);
		b_nodes = dst_node2(before_and_after, two);
		b_arcs = dst_node2(before_and_after, three);
		a_nodes = dst_node2(before_and_after, four);
		a_arcs = dst_node2(before_and_after, five);
		cwh = dst_node2(before_and_after, p->cwh);
		twh = dst_node2(before_and_after, p->twh);
#ifdef	_DEBUG_PM_REP3
		DumpGraph("before_struct = ", b_stat, 10, 1000, NULL, 1);
		DumpGraph("after_struct = ", a_stat, 10, 1000, NULL, 1);
		DumpGraph("invariant = ", invariant, 10, 1000, NULL, 1);
		DumpGraph("b_nodes = ", b_nodes, 10, 1000, NULL, 1);
		DumpGraph("b_arcs = ", b_arcs, 10, 1000, NULL, 1);
		DumpGraph("a_nodes = ", a_nodes, 10, 1000, NULL, 1);
		DumpGraph("a_arcs = ", a_arcs, 10, 1000, NULL, 1);
		DumpGraph("cwh = ", cwh, 10, 1000, NULL, 1);
		DumpGraph("twh = ", twh, 10, 1000, NULL, 1);
#endif
		AGmNode *b_stat0 = b_stat;
		l1 = NULL; int first = 1;
		for (;;) {
			l1 = NULL;
			PatternMatch(first);
			if (IsString(err)) return;
			if (match || no_rep) {
				// evaluate common condition
				// if not common, error.
				EvaluateCommonCondition();
				if (IsString(err)) return;
				if (ret == NULL || ret == three) {
					AGmLink *ll;
					for (ll = next_outlink(obj_db, NULL); ll; ll = next_outlink(obj_db, ll)) {
						AGmNode *var = ll->arc->dst;
						if (var->H == NULL) {
							continue;
						}
						AGmLink *lll;
						for (lll = next_inlink2(var, NULL, p->eq); lll; lll = next_inlink2(var, lll, p->eq)) {
							AGmNode *node = lll->arc->src;
							prolog->Set2(&node->TH, var->TH, prolog->exec_stack->arcs);
							prolog->Set2(&node->TTTH, var->TH, prolog->exec_stack->arcs);
						}
					}
					to_be_modified = 1;
					StartModify(b_stat0, a_stat);
					ModifyCreateNode(a_nodes);
					if (IsString(err)) return;
					ModifyCreateArc(a_arcs);
					if (IsString(err)) return;
					if (invariant->a.i.v == 0) {
						ModifyDeleteArc(b_nodes);
						if (IsString(err)) return;
					}
					Tail();
					if (IsString(err)) return;
				} else {
					char msg[256];
					art_sprintf_1(msg, 256, "no more common condition at %ld th pair.", idx);
					err = prolog->error(msg, p->pm_rep);
					return;
				}
			}
			NextRepetition();
			if (no_more_rep) break;
			first = 0;
		}
		if (to_be_modified) break;
	}
}
void AGmGdb::PatternMatch(int first) {
	// repetition on arc
	for (l1 = next_outlink(b_stat, NULL); l1; l1 = next_outlink(b_stat, l1)) {
		AGmLink *ll;
		for (ll = next_outlink(obj_db, NULL); ll; ll = next_outlink(obj_db, ll)) {
			AGmNode *var = ll->arc->dst;
			if (var->H == NULL) {
				continue;
			}
			AGmLink *lll;
			for (lll = next_inlink2(var, NULL, p->eq); lll; lll = next_inlink2(var, lll, p->eq)) {
				AGmNode *node = lll->arc->src;
				prolog->Set2(&node->TH, var->TH, prolog->exec_stack->arcs);
				prolog->Set2(&node->TTTH, var->TH, prolog->exec_stack->arcs);
			}
		}
		AGmNode *s = l1->arc->dst;
		if (s->H == p->opt_grph) {
			if (++prolog->call_count > prolog->max_call_count) {
				err = prolog->error("stack overflow.", NULL);
				return;
			}
			prolog->PushExecStack(new AGmExecStack(OP_OPT_GRPH));
			prolog->exec_stack->level = s->THI;
			prolog->exec_stack->save_stat = b_stat;
			prolog->exec_stack->save_l1 = l1;
			
			PatternMatch(1);
			if (IsString(err)) return;
			if (match || no_rep) {
				s->TTHI = 1;
			}
			continue;
		} else if (s->H == p->blk_grph) {
			if (++prolog->call_count > prolog->max_call_count) {
				err = prolog->error("stack overflow.", NULL);
				return;
			}
			prolog->PushExecStack(new AGmExecStack(OP_BLK_GRPH));
			prolog->exec_stack->level = s->THI;
			prolog->exec_stack->save_stat = b_stat;
			prolog->exec_stack->save_l1 = l1;
			AGmLink *l2;
			for (l2= next_outlink(s, NULL); l2; l2 = next_outlink(s, l2)) {
				prolog->exec_stack->save_stat2 = l1->arc->dst;
				prolog->exec_stack->save_l2 = l2;
			}
			if (IsString(err)) return;
			if (match || no_rep) continue;
			return;
		} else if (s->H == p->rpt_grph) {
			if (first) {
				if (++prolog->call_count > prolog->max_call_count) {
					err = prolog->error("stack overflow.", NULL);
					return;
				}
				prolog->PushExecStack(new AGmExecStack(OP_RPT_GRPH));
				prolog->exec_stack->level = s->THI;
				prolog->exec_stack->save_stat = b_stat;
				prolog->exec_stack->save_l1 = l1;
			}
			PatternMatch(1);
			if (IsString(err)) return;
			if (no_rep == 1) {
				s->TTHI = 1;
			}
			if (match || no_rep) continue;
			return;
		} else if (s->H == p->chc_grph) {
			if (++prolog->call_count > prolog->max_call_count) {
				err = prolog->error("stack overflow.", NULL);
				return;
			}
			prolog->PushExecStack(new AGmExecStack(OP_CHC_GRPH));
			prolog->exec_stack->level = s->THI;
			prolog->exec_stack->save_stat = b_stat;
			prolog->exec_stack->save_l1 = l1;
			AGmLink *l2;
			for (l2 = next_outlink(s, NULL); l2; l2 = next_outlink(s, l2)) {
				AGmNode *ss = l2->arc->dst;
				ASSERT(ss->H == p->chc_before);
				l1 = next_outlink(ss, NULL);
				EvaluateChoiceCondition();
				if (IsString(err)) return;
				if (ret == NULL) {
					s->TTHI = ss->THI;
					break;
				}
				break;
			}
			if (l2 == NULL) {
				err = prolog->error("choice selection not found.", s);
				return;
			}
			continue;
		} else if (s->H == p->sub_graph) {
			arc = dst_node2(s, p->arc);
			an = arc->H;
			av = arc->TH;
			ai = (long)arc->TTHI;
			src = dst_node2(s, p->grph_src);
			dst = dst_node2(s, p->grph_dst);
			brk = 0;
			switch (ai) {
			case REL_1_1:
				n1 = src->TTTH;
				PatternMatchRel11();
				if (IsString(err)) return;
				if (brk) break;
				continue;
			case REL_1_N:
				n1 = src->TTTH;
				PatternMatchRel1N();
				if (IsString(err)) return;
				if (brk) break;
				continue;
			case INV_REL_1_1:
				n1 = dst->TTTH;
				PatternMatchInvRel11();
				if (IsString(err)) return;
				if (brk) break;
				continue;
			case INV_REL_1_N:
				n1 = dst->TTTH;
				PatternMatchInvRel1N();
				if (IsString(err)) return;
				if (brk) break;
				continue;
			default:
				ASSERT(0);
				break;
			}
		} else if (s->H == p->node) {
			src = dst_node2(s, p->grph_src);
			n1 = src->TTTH;
			match = 1;
			no_rep = 0;
			continue;
		} else {
			ASSERT(0);
		}
	}
}

void AGmGdb::PatternMatchRel11() {
	if (n1 == p->undefined) {
		err = prolog->error("rel_11 src_node is undefiend", p->pm_rep);
		return;
	}
	for (lnk = next_outlink(n1, NULL); lnk; lnk = next_outlink(n1, lnk)) {
		if (av == p->wc_e) {
			AGmNode* arc_var = dst_node2(obj_db, an);
			prolog->Set_H_TH(arc_var, lnk->arc->attr, prolog->exec_stack->arcs);
			prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
			break;
		}
		if (prolog->unify2(lnk->arc->attr, av, obj_db) == 0) {
			prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
			break;
		}
	}
	if (lnk == NULL) {
		match = 0;
		no_rep = 0;
		brk = 1;
		return;
	} else {
		n = lnk->arc->dst;
		if (prolog->unify(dst, n, obj_db)) {
if (prolog->debug_syn) {
DumpList(prolog, "### rel_11 dst_node_value mismatch: dst = ", dst, 20, 1000, NULL, 1);
DumpList(prolog, "### rel_11 dst_node_value mismatch: n = ", n, 20, 1000, NULL, 1);
}
			match = 0;
			no_rep = 0;
			brk = 1;
			return;
		} else {
			AGmNode* var = dst_node2(dst, p->eq);
			if (var) {
				prolog->Set_H_TH(var, n, prolog->exec_stack->arcs);
				prolog->Set2(&arc->TTTT->TTH, PtrCell(lnk), prolog->exec_stack->arcs);
			}
if (prolog->debug_syn) {
DumpList(prolog, "### rel_11 dst_node_value match: dst = ", dst, 20, 1000, NULL, 1);
DumpList(prolog, "### rel_11 dst_node_value match: n = ", n, 20, 1000, NULL, 1);
}
			match = 1;
			no_rep = 0;
			return;
		}
	}
}
void AGmGdb::PatternMatchRel1N() {
	if (prolog->exec_stack->op != OP_RPT_GRPH) {
		err = prolog->error("-/*/=> appeared without <<<n", p->pm_rep);
		return;
	}
	// first
	if (n1 == p->undefined) {
		err = prolog->error("for_all src_node is undefiend", p->pm_rep);
		return;
	}
	if (av == p->wc_e) {
		if (an == NULL) {
			err = prolog->error("wild card attr arc must have a variable name", p->pm_rep);
			return;
		}
		lnk = next_outlink(n1, NULL);
	} else {
		for (lnk = next_outlink(n1, NULL); lnk; lnk = next_outlink(n1, lnk)) {
			if (prolog->unify2(lnk->arc->attr, av, obj_db) == 0) {
				prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
				break;
			}
		}
	}
	if (lnk == NULL) {
		match = 0;
		no_rep = 1;
		return;
	} else {
		prolog->exec_stack->for_all = FOR_ALL;
		prolog->exec_stack->g1 = g1;
		prolog->exec_stack->rl1 = rl1;
		prolog->exec_stack->n = n1;
		prolog->exec_stack->l = lnk;
		if (av == p->wc_e) {
			AGmNode* arc_var = dst_node2(obj_db, an);
			prolog->Set_H_TH(arc_var, lnk->arc->attr, prolog->exec_stack->arcs);
			prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
if (prolog->debug_syn3) {
DumpList("## arc_link(4): ", arc, 20, NULL, 1);
}
		}
if (prolog->debug_syn) {
char msg[256];
art_sprintf_4(msg, 256, "this = 0x%lx, ### for_all push: exec_stack = %lx, level = %d, l1 = %lx\n",
AL_PTR(prolog), AL_PTR(prolog->exec_stack), prolog->exec_stack->level, AL_PTR(l1));
ADvWindow::Print(msg);
}
		n = lnk->arc->dst;
		if (prolog->unify(dst, n, obj_db)) {
if (prolog->debug_syn) {
DumpList(prolog, "### for_all dst_node_value mismatch(1): dst = ", dst, 20, 1000, NULL, 1);
DumpList(prolog, "### for_all dst_node_value mismatch(1): n = ", n, 20, 1000, NULL, 1);
}
			err = prolog->error("for_all dst_node_value mismatch(1)", p->pm_rep);
			return;
		} else {
			if (av == p->wc_e) {
				AGmNode* var = dst_node2(obj_db, an);
				if (var) {
					prolog->Set_H_TH(var, lnk->arc->attr, prolog->exec_stack->arcs);
					prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
				}
			}
			AGmNode* var = dst_node2(dst, p->eq);
			if (var) {
				prolog->Set_H_TH(var, n, prolog->exec_stack->arcs);
				prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
			}
if (prolog->debug_syn) {
DumpList(prolog, "### for_all dst_node_value match(1): dst = ", dst, 20, 1000, NULL, 1);
DumpList(prolog, "### for_all dst_node_value match(1): n = ", n, 20, 1000, NULL, 1);
}
			match = 1;
			no_rep = 0;
			return;
		}
	}
}
void AGmGdb::PatternMatchInvRel11() {
	if (n1 == p->undefined) {
		err = prolog->error("inv_rel_11 dst_node is undefiend", p->pm_rep);
		return;
	}
	for (lnk = next_inlink(n1, NULL); lnk; lnk = next_inlink(n1, lnk)) {
		if (av == p->wc_e) {
			AGmNode* arc_var = dst_node2(obj_db, an);
			prolog->Set_H_TH(arc_var, lnk->arc->attr, prolog->exec_stack->arcs);
			prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
if (prolog->debug_syn3) {
DumpList("## arc_link(8): ", arc, 20, NULL, 1);
}
			break;
		}
		if (prolog->unify2(lnk->arc->attr, av, obj_db) == 0) {
			prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
if (prolog->debug_syn3) {
DumpList("## arc_link(9): ", arc, 20, NULL, 1);
}
			break;
		}
	}
	if (lnk == NULL) {
		match = 0;
		no_rep = 0;
		brk = 1;
		return;
	} else {
		n = lnk->arc->src;
		if (prolog->unify(dst, n, obj_db)) {
if (prolog->debug_syn) {
DumpList(prolog, "### inv_rel_11 src_node_value mismatch: dst = ", dst, 20, 1000, NULL, 1);
DumpList(prolog, "### inv_rel_11 src_node_value mismatch: n = ", n, 20, 1000, NULL, 1);
}
			match = 0;
			no_rep = 0;
			brk = 1;
			return;
		} else {
			AGmNode* var = dst_node2(dst, p->eq);
			if (var) {
				prolog->Set_H_TH(var, n, prolog->exec_stack->arcs);
				prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
if (prolog->debug_syn3) {
DumpList("## arc_link(10): ", arc, 20, NULL, 1);
}
			}
if (prolog->debug_syn) {
DumpList(prolog, "### inv_rel_11 src_node_value match: dst = ", dst, 20, 1000, NULL, 1);
DumpList(prolog, "### inv_rel_11 src_node_value match: n = ", n, 20, 1000, NULL, 1);
}
			match = 1;
			no_rep = 0;
			return;
		}
	}
}
void AGmGdb::PatternMatchInvRel1N() {
	if (prolog->exec_stack->op != OP_RPT_GRPH) {
		err = prolog->error("<=/*/- appeared without <<<n", p->pm_rep);
		return;
	}
	// first
	if (n1 == p->undefined) {
		err = prolog->error("inv_for_all dst_node is undefiend", p->pm_rep);
		return;
	}
	if (av == p->wc_e) {
		if (an == NULL) {
			err = prolog->error("wild card attr arc must have a variable name", p->pm_rep);
			return;
		}
		lnk = next_inlink(n1, NULL);
	} else {
		for (lnk = next_inlink(n1, NULL); lnk; lnk = next_inlink(n1, lnk)) {
			if (prolog->unify2(lnk->arc->attr, av, obj_db) == 0) {
				prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
if (prolog->debug_syn3) {
DumpList("## arc_link(12): ", arc, 20, NULL, 1);
}
				break;
			}
		}
	}
	if (lnk == NULL) {
		match = 0;
		no_rep = 1;
		return;
	} else {
		prolog->exec_stack->for_all = INV_FOR_ALL;
		prolog->exec_stack->g1 = g1;
		prolog->exec_stack->rl1 = rl1;
		prolog->exec_stack->n = n1;
		prolog->exec_stack->l = lnk;
		if (av == p->wc_e) {
			AGmNode* arc_var = dst_node2(obj_db, an);
			prolog->Set_H_TH(arc_var, lnk->arc->attr, prolog->exec_stack->arcs);
			prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
if (prolog->debug_syn3) {
DumpList("## arc_link(13): ", arc, 20, NULL, 1);
}
		}
if (prolog->debug_syn) {
char msg[256];
art_sprintf_4(msg, 256, "this = 0x%lx, ### inv_for_all push: exec_stack = %lx, level = %d, l1 = %lx\n",
AL_PTR(prolog), AL_PTR(prolog->exec_stack), prolog->exec_stack->level, AL_PTR(l1));
ADvWindow::Print(msg);
}
		n = lnk->arc->src;
		if (prolog->unify(dst, n, obj_db)) {
if (prolog->debug_syn) {
DumpList(prolog, "### inv_for_all src_node_value mismatch(1): dst = ", dst, 20, 1000, NULL, 1);
DumpList(prolog, "### inv_for_all src_node_value mismatch(1): n = ", n, 20, 1000, NULL, 1);
}
			err = prolog->error("inv_for_all src_node_value mismatch(1)", p->pm_rep);
			return;
		} else {
			if (av == p->wc_e) {
				AGmNode* var = dst_node2(obj_db, an);
				if (var) {
					prolog->Set_H_TH(var, lnk->arc->attr, prolog->exec_stack->arcs);
					prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
					if (prolog->debug_syn3) {
						DumpList("## arc_link(14): ", arc, 20, NULL, 1);
					}
				}
			}
			AGmNode* var = dst_node2(dst, p->eq);
			if (var) {
				prolog->Set_H_TH(var, n, prolog->exec_stack->arcs);
				prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
				if (prolog->debug_syn3) {
					DumpList("## arc_link(15): ", arc, 20, NULL, 1);
				}
			}
			if (prolog->debug_syn) {
				DumpList(prolog, "### inv_for_all src_node_value match(1): dst = ", dst, 20, 1000, NULL, 1);
				DumpList(prolog, "### inv_for_all src_node_value match(1): n = ", n, 20, 1000, NULL, 1);
			}
		}
		match = 1;
		no_rep = 0;
		return;
	}
}

void AGmGdb::NextRepetition() {
	char msg[256];
	no_more_rep = 1;
	for (; prolog->exec_stack->op != OP_NOP;) {
		switch (prolog->exec_stack->op) {
		case OP_RPT_GRPH:
			// Rel1N not first
			if (prolog->exec_stack->for_all == FOR_ALL) {
				if (prolog->debug_syn) {
					art_sprintf_3(msg, 256, "this = 0x%lx, ### for_all delete arcs: exec_stack = %lx, level = %d\n",
						AL_PTR(prolog), AL_PTR(prolog->exec_stack), prolog->exec_stack->level);
					ADvWindow::Print(msg);
				}
				prolog->DeleteArcs(prolog->exec_stack->arcs);
				if (prolog->debug_syn2) {
					DumpGraph(prolog, "after DeleteArcs(1): obj_db = ", obj_db, 10, 100, NULL, 1);
				}
				n1 = prolog->exec_stack->n;
				if (av == p->wc_e) {
					lnk = next_outlink(n1, prolog->exec_stack->l);
				} else {
					for (lnk = next_outlink(n1, prolog->exec_stack->l); lnk; lnk = next_outlink(n1, lnk)) {
						if (prolog->unify2(lnk->arc->attr, av, obj_db) == 0) {
							prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
							if (prolog->debug_syn3) {
								DumpList("## arc_link(5): ", arc, 20, NULL, 1);
							}
							break;
						}
					}
				}
				if (lnk) {
					prolog->exec_stack->l = lnk;
					n = lnk->arc->dst;
					if (prolog->unify(dst, n, obj_db)) {
						if (prolog->debug_syn) {
							DumpList(prolog, "### for_all dst_node_value mismatch(2): dst = ", dst, 20, 1000, NULL, 1);
							DumpList(prolog, "### for_all dst_node_value mismatch(2): n = ", n, 20, 1000, NULL, 1);
						}
						err = prolog->error("for_all dst_node_value mismatch(2)", p->pm_rep);
						return;
					} else {
						if (av == p->wc_e) {
							AGmNode* var = dst_node2(obj_db, an);
							prolog->Set_H_TH(var, lnk->arc->attr, prolog->exec_stack->arcs);
							prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
							if (prolog->debug_syn3) {
								DumpList("## arc_link(6): ", arc, 20, NULL, 1);
							}
						}
						AGmNode* var = dst_node2(dst, p->eq);
						if (var) {
							prolog->Set_H_TH(var, n, prolog->exec_stack->arcs);
							prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
							if (prolog->debug_syn3) {
								DumpList("## arc_link(7): ", arc, 20, NULL, 1);
							}
						}
						if (prolog->debug_syn) {
							DumpList(prolog, "### for_all dst_node_value match(2): dst = ", dst, 20, 1000, NULL, 1);
							DumpList(prolog, "### for_all dst_node_value match(2): n = ", n, 20, 1000, NULL, 1);
						}
						no_more_rep = 0;
						return;
					}
				} else {
					if (prolog->debug_syn) {
						art_sprintf_4(msg, 256, "this = 0x%lx, ### for_all pop(before): exec_stack = %lx, level = %d, l1 = %lx\n",
							AL_PTR(prolog), AL_PTR(prolog->exec_stack), prolog->exec_stack->level, AL_PTR(l1));
						ADvWindow::Print(msg);
					}
					prolog->DeleteArcs(prolog->exec_stack->arcs);
					if (prolog->debug_syn2) {
						DumpGraph(prolog, "after DeleteArcs(2): obj_db = ", obj_db, 10, 100, NULL, 1);
					}
					b_stat = prolog->exec_stack->save_stat;
					l1 = prolog->exec_stack->save_l1;
					prolog->PopExecStack(); prolog->call_count--;
					no_more_rep = 1;
					continue;
				}
			// InvRel1N not first
			} else if (prolog->exec_stack->for_all == INV_FOR_ALL) {
				if (prolog->debug_syn) {
					art_sprintf_3(msg, 256, "this = 0x%lx, ### inv_for_all delete arcs: exec_stack = %lx, level = %d\n",
						AL_PTR(prolog), AL_PTR(prolog->exec_stack), prolog->exec_stack->level);
					ADvWindow::Print(msg);
				}
				prolog->DeleteArcs(prolog->exec_stack->arcs);
				if (prolog->debug_syn2) {
					DumpGraph(prolog, "after DeleteArcs(3): obj_db = ", obj_db, 10, 100, NULL, 1);
				}
				n1 = prolog->exec_stack->n;
				if (av == p->wc_e) {
					lnk = next_inlink(n1, prolog->exec_stack->l);
				} else {
					for (lnk = next_inlink(n1, prolog->exec_stack->l); lnk; lnk = next_inlink(n1, lnk)) {
						if (prolog->unify2(lnk->arc->attr, av, obj_db) == 0) {
							prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
							if (prolog->debug_syn3) {
								DumpList("## arc_link(16): ", arc, 20, NULL, 1);
							}
							break;
						}
					}
				}
				if (lnk) {
					prolog->exec_stack->l = lnk;
					n = lnk->arc->src;
					if (prolog->unify(dst, n, obj_db)) {
						if (prolog->debug_syn) {
							DumpList(prolog, "### inv_for_all src_node_value mismatch(2): dst = ", dst, 20, 1000, NULL, 1);
							DumpList(prolog, "### inv_for_all src_node_value mismatch(2): n = ", n, 20, 1000, NULL, 1);
						}
						err = prolog->error("inv_for_all src_node_value mismatch(2)", p->pm_rep);
						return;
					} else {
						if (av == p->wc_e) {
							AGmNode* var = dst_node2(obj_db, an);
							prolog->Set_H_TH(var, lnk->arc->attr, prolog->exec_stack->arcs);
							prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
							if (prolog->debug_syn3) {
								DumpList("## arc_link(17): ", arc, 20, NULL, 1);
							}
						}
						AGmNode* var = dst_node2(dst, p->eq);
						if (var) {
							prolog->Set_H_TH(var, n, prolog->exec_stack->arcs);
							prolog->Set2(&arc->TTTTH, PtrCell(lnk), prolog->exec_stack->arcs);
							if (prolog->debug_syn3) {
								DumpList("## arc_link(18): ", arc, 20, NULL, 1);
							}
						}
						if (prolog->debug_syn) {
							DumpList(prolog, "### inv_for_all src_node_value match(2): dst = ", dst, 20, 1000, NULL, 1);
							DumpList(prolog, "### inv_for_all src_node_value match(2): n = ", n, 20, 1000, NULL, 1);
						}
						no_more_rep = 0;
						return;
					}
				} else {
					if (prolog->debug_syn) {
						art_sprintf_4(msg, 256, "this = 0x%lx, ### inv_for_all pop(before): exec_stack = %lx, level = %d, l1 = %lx\n",
							AL_PTR(prolog), AL_PTR(prolog->exec_stack), prolog->exec_stack->level, AL_PTR(l1));
						ADvWindow::Print(msg);
					}
					prolog->DeleteArcs(prolog->exec_stack->arcs);
					if (prolog->debug_syn2) {
						DumpGraph(prolog, "after DeleteArcs(4): obj_db = ", obj_db, 10, 100, NULL, 1);
					}
					b_stat = prolog->exec_stack->save_stat;
					l1 = prolog->exec_stack->save_l1;
					prolog->PopExecStack(); prolog->call_count--;
					prolog->PopExecStack();
					no_more_rep = 1;
					continue;
				}
			} else {
				ASSERT(0);
			}
		default:
			break;
		}
	}
}

AGmNode *AGmGdb::get_b_stat(AGmNode *b_stat, AGmNode* kind, int level) {
	AGmLink *l1;
	for (l1 = next_outlink(b_stat, NULL); l1; l1 = next_outlink(b_stat, NULL)) {
		AGmNode *bs = l1->arc->dst;
		if (bs->H == kind && bs->THI == level) {
			return bs;
		}
		bs = get_b_stat(bs, kind, level);
		if (bs) {
			return bs;
		}
	}
	return NULL;
}

AGmNode *AGmGdb::get_next_arc_name(AGmNode* stat) {
	AGmLink *l2;
	if (stat->H == p->blk_grph) {
		for (l2 = next_outlink(stat, NULL); l2; l2 = next_outlink(stat, l2)) {
			AGmNode *ss = l2->arc->dst;
			if (ss->H == p->sub_graph) {
				arc = dst_node2(ss, p->arc);
				return arc->H;
			}
		}
	}
	if (stat->H == p->rpt_grph) {
		for (l2 = next_outlink(stat, NULL); l2; l2 = next_outlink(stat, l2)) {
			AGmNode *ss = l2->arc->dst;
			if (ss->H == p->sub_graph) {
				arc = dst_node2(ss, p->arc);
				return arc->H;
			}
		}
	}
	if (stat->H == p->sub_graph) {
		arc = dst_node2(stat, p->arc);
		return arc->H;
	}
	return p->undefined;
}

void AGmGdb::StartModify(AGmNode *b_stat, AGmNode *a_stat) {
if (prolog->debug_syn2) {
DumpGraph(prolog, "start modify: obj_db = ", obj_db, 10, 100, NULL, 1);
}
	// repetition on arc
	for (l1 = next_outlink(a_stat, NULL); l1; l1 = next_outlink(a_stat, l1)) {
		AGmNode *s = l1->arc->dst;
		if (s->H == p->opt_grph) {
			AGmNode *an = get_next_arc_name(s);
			AGmNode *var = dst_node2(obj_db, an);
			if (var && var->TH) {
				StartModify(b_stat, dst_node(s, (AGmNode*)NULL));
			}
		} else if (s->H == p->blk_grph) {
			AGmLink *l2;
			for (l2 = next_outlink(s, NULL); l2; l2 = next_outlink(s, l2)) {
				StartModify(b_stat, l2->arc->dst);
			}

		} else if (s->H == p->rpt_grph) {
			AGmNode *bs = get_b_stat(b_stat, s->H, s->THI);
			if (bs->TTI == 1) {
				AGmNode *ss = dst_node(s, (AGmNode*)NULL);
				ASSERT(ss->H == p->sub_graph);
				arc = dst_node2(ss, p->arc);
				ai = (long)arc->TTHI;
				switch (ai) {
				case REL_1_N:
					src = dst_node2(ss, p->grph_src);
					src->TTTTTHI = 1;
					break;
				case INV_REL_1_N:
					dst = dst_node2(ss, p->grph_dst);
					dst->TTTTTHI = 1;
					break;
				}
			} else {
				AGmLink *l2;
				for (l2 = next_outlink(s, NULL); l2; l2 = next_outlink(s, l2)) {
					StartModify(b_stat, l2->arc->dst);
				}
			}
		} else if (s->H == p->chc_grph) {
			AGmNode *bs = get_b_stat(b_stat, s->H, s->THI);
			AGmLink *l2; AGmNode *ss;
			for (l2 = next_outlink(s, NULL); l2; l2 = next_outlink(s, l2)) {
				ss = l2->arc->dst;
				if (ss->THI == bs->TTHI) break;
			}
			if (l2 == NULL) {
				err = prolog->error("choice after not found.", s);
				return;
			}
			StartModify(b_stat, ss);
		} else if (s->H == p->sub_graph) {
			arc = dst_node2(s, p->arc);
			src = dst_node2(s, p->grph_src);
			dst = dst_node2(s, p->grph_dst);
			arc->TTTTT->THI = 1;
			src->TTTTTHI = 1;
			dst->TTTTTHI = 1;
		} else if (s->H == p->node) {
			src = dst_node2(s, p->grph_src);
			src->TTTTTHI = 1;
		} else {
			ASSERT(0);
		}
	}
}
void AGmGdb::ModifyCreateNode(AGmNode *a_nodes) {
if (prolog->debug_syn) {
DumpList(prolog, "do modify subgraph (after: create_node): ", NULL);
}
	// do modify subgraph (after: create_node)
	AGmLink *l3;
	for (l3 = next_outlink(a_nodes, NULL); l3; l3 = next_outlink(a_nodes, l3)) {
		AGmNode *node = l3->arc->dst;
if (prolog->debug_syn2) {
DumpGraph(prolog, "node = ", node, 20, 1000, NULL, 1);
}
		if (node->TTTTTHI == 0) continue;
		if (node->TTTH == p->undefined) {
			AGmNode *name = node->H;
			AGmNode *v = dst_node2(obj_db, name);
			AGmExecStack *stack = prolog->exec_stack->FindExecStack(node->TTTTHI);
			if (stack == NULL) {
				err = prolog->error("after_create_node: exec_stack speficied level is NULL.", p->pm_rep);
				return;
			}
			AGmNode *n = prolog->eval(node->TH, obj_db);
			prolog->Set2(&node->TTTH, n, stack->arcs);
			AGmNode* var = dst_node2(node, p->eq);
			if (var) {
				prolog->Set_H_TH(var, n, stack->arcs);
			}
			if (n == p->wc_e) {
				err = prolog->error("after_create_node: src_node is wild card. ", p->pm_rep);
				return;
			}
if (prolog->debug_syn2) {
DumpList(prolog, "### created node = ", n, 20, 100, NULL, 1);
}
		}
	}
}
void AGmGdb::ModifyCreateArc(AGmNode *a_arcs) {
	char msg[256];
	if (prolog->debug_syn) {
DumpList(prolog, "do modify subgraph (after: create_arc): ", NULL);
}
	// do modify subgraph (after: create_arc)
	AGmLink *l4;
	for (l4 = next_outlink(a_arcs, NULL); l4; l4 = next_outlink(a_arcs, l4)) {
		AGmNode *arc = l4->arc->dst;
		if (arc->TTTTT->THI == 0) continue;
		AGmNode *an = arc->H;
		AGmNode *attr = arc->TH;
		long ai = (long)arc->TTHI;
		AGmNode *stat = src_node2(arc, p->arc);
		AGmNode *src = dst_node2(stat, p->grph_src);
		AGmNode *dst = dst_node2(stat, p->grph_dst);
		AGmNode* n1 = src->TTTH;
		AGmNode* n2 = dst->TTTH;
		AGmArc* a; AGmLink* o_lnk = NULL;
		AGmNode* arc_var;
if (prolog->debug_syn2) {
DumpGraph(prolog, "arc = ", arc, 20, 1000, NULL, 1);
}
		int common = (int)arc->TTTHI;
		AGmNode **arc_crrspnd = &arc->TTTTH;
		AGmExecStack *stack;
		switch (ai) {
		case REL_1_1:
			if (*arc_crrspnd != p->undefined) break;
			stack = prolog->exec_stack->FindExecStack(arc->TTTTTHI);
			if (stack == NULL) {
				err = prolog->error("after_create_arc(rel_1_1): exec_stack speficied level is NULL.", p->pm_rep);
				return;
			}
			if (n1 == p->undefined) {
				if (src->H) {
					AGmNode *obj = dst_node2(obj_db, src->H);
					if (obj && obj->H) {
						prolog->Set2(&src->TTTH, n1 = obj->TH, stack->arcs);
					} else {
						err = prolog->error("rel_11 create_arc: src_node is undefiend(2)", p->pm_rep);
						return;
					}
				} else {
					err = prolog->error("rel_11 create_arc: src_node is undefiend(1)", p->pm_rep);
					return;
				}
			}
			if (n2 == p->undefined) {
				err = prolog->error("rel_11 create_arc: dst_node undefined. ", p->pm_rep);
				return;
			}
			if (attr == p->wc_e) {
				AGmNode *var = dst_node2(obj_db, an);
				if (var && var->H) {
					attr = var->TH;
				} else {
					err = prolog->error("rel_11 create_arc: arc_attr is wild card(2). ", p->pm_rep);
					return;
				}
			}
			attr = prolog->eval(attr, obj_db);
			if (common == 0) {
				a = CreateArc(n1, n2, attr, NULL, NULL, &o_lnk);
				prolog->Set2(arc_crrspnd, PtrCell(o_lnk), stack->arcs);
if (prolog->debug_syn2) {
DumpList(prolog, "### rel_11: created arc->src = ", a->src, 20, 100, NULL, 1);
DumpList(prolog, "### rel_11: created arc->dst = ", a->dst, 20, 100, NULL, 1);
DumpList(prolog, "### rel_11: created arc->attr = ", a->attr, 20, 100, NULL, 1);
}
			} else {
				AGmNode *arc2 = dst_node2(arc, p->comm_arc);
				AGmNode **arc_crrspnd2 = &arc2->TTTTT->TH;
				if (!IsPtrCell(*arc_crrspnd2)) {
					err = prolog->error("rel_11 create_arc: share arc src is undefined. ", p->pm_rep);
					return;
				}
				a = CreateArc(n1, n2, attr, NULL, NULL, &o_lnk);
				prolog->Set2(arc_crrspnd, PtrCell(o_lnk), stack->arcs);
				// ### TODO AGmLink *l = (AGmLink*)((*arc_crrspnd2)->a.p.ptr);
				// ### TODO DeleteArc(l->arc);
if (prolog->debug_syn2) {
DumpList(prolog, "### rel_11: replaced arc->src = ", o_lnk->arc->src, 20, 100, NULL, 1);
DumpList(prolog, "### rel_11: replaced arc->dst = ", o_lnk->arc->dst, 20, 100, NULL, 1);
DumpList(prolog, "### rel_11: replaced arc->attr = ", o_lnk->arc->attr, 20, 100, NULL, 1);
}
			}
			break;
		case REL_1_N:
			if (*arc_crrspnd != p->undefined) break;
			stack = prolog->exec_stack->FindExecStack(arc->TTTTTHI);
			if (stack == NULL) {
				err = prolog->error("after_create_arc(rel_1_n): exec_stack speficied level is NULL.", p->pm_rep);
				return;
			}
			if (n1 == p->undefined) {
				err = prolog->error("for_all create_arc: src_node undefined. ", p->pm_rep);
				return;
			}
			if (n2 == p->undefined) {
if (prolog->debug_syn) {
art_sprintf_0(msg, 256, "for_all create_arc: number of repetition is zero (dst_node undefined).\n");
}
				break;
			}
			if (attr == p->wc_e) {
				AGmNode *var = dst_node2(obj_db, an);
				if (var && var->H) {
					attr = var->TH;
				} else {
if (prolog->debug_syn) {
art_sprintf_0(msg, 256, "for_all create_arc: number of repetition is zero (arc_attr is wild card).\n");
}
					break;
				}
			}
			attr = prolog->eval(attr, obj_db);
			a = CreateArc(n1, n2, attr, NULL, NULL, &o_lnk);
			prolog->Set2(arc_crrspnd, PtrCell(o_lnk), stack->arcs);
if (prolog->debug_syn2) {
DumpList(prolog, "### for_all: created arc->src = ", a->src, 20, 100, NULL, 1);
DumpList(prolog, "### for_all: created arc->dst = ", a->dst, 20, 100, NULL, 1);
DumpList(prolog, "### for_all: created arc->attr = ", a->attr, 20, 100, NULL, 1);
}
			break;
		case INV_REL_1_1:
			if (*arc_crrspnd != p->undefined) break;
			stack = prolog->exec_stack->FindExecStack(arc->TTTTTHI);
			if (stack == NULL) {
				err = prolog->error("after_create_arc(inv_rel_1_1): exec_stack speficied level is NULL.", p->pm_rep);
				return;
			}
			if (n2 == p->undefined) {
				if (dst->H) {
					AGmNode *obj = dst_node2(obj_db, dst->H);
					if (obj && obj->H) {
						prolog->Set2(&dst->TTTTH, n2 = obj->TH, stack->arcs);
					} else {
						err = prolog->error("inv_rel_11 create_arc: dst_node is undefiend(2)", p->pm_rep);
						return;
					}
				} else {
					err = prolog->error("inv_rel_11 create_arc: dst_node is undefiend(1)", p->pm_rep);
					return;
				}
			}
			if (n1 == p->undefined) {
				err = prolog->error("inv_rel_11 create_arc: src_node undefined. ", p->pm_rep);
				return;
			}
			if (attr == p->wc_e) {
				AGmNode *var = dst_node2(obj_db, an);
				if (var && var->H) {
					attr = var->TH;
				} else {
					err = prolog->error("inv_rel_11 create_arc: arc_attr is wild card(2). ", p->pm_rep);
					return;
				}
			}
			attr = prolog->eval(attr, obj_db);
			if (common == 0) {
				a = CreateArc(n2, n1, attr, NULL, NULL, &o_lnk);
				prolog->Set2(arc_crrspnd, PtrCell(o_lnk), stack->arcs);
				if (prolog->debug_syn2) {
					DumpList(prolog, "### inv_rel_11: created arc->dst = ", a->dst, 20, 100, NULL, 1);
					DumpList(prolog, "### inv_rel_11: created arc->src = ", a->src, 20, 100, NULL, 1);
					DumpList(prolog, "### inv_rel_11: created arc->attr = ", a->attr, 20, 100, NULL, 1);
				}
			} else {
				AGmNode *arc2 = dst_node2(arc, p->comm_arc);
				AGmNode **arc_crrspnd2 = &arc2->TTTTH;
				if (!IsPtrCell(*arc_crrspnd2)) {
					err = prolog->error("inv_rel_11 create_arc: share arc src is undefined. ", p->pm_rep);
					return;
				}
				a = CreateArc(n1, n2, attr, NULL, NULL, &o_lnk);
				prolog->Set2(arc_crrspnd, PtrCell(o_lnk), stack->arcs);
				// ### TODO AGmLink *l = (AGmLink*)((*arc_crrspnd2)->a.p.ptr);
				// ### TODO DeleteArc(l->arc);
				if (prolog->debug_syn2) {
					DumpList(prolog, "### inv_rel_11: replaced arc->dst = ", o_lnk->arc->dst, 20, 100, NULL, 1);
					DumpList(prolog, "### inv_rel_11: replaced arc->src = ", o_lnk->arc->src, 20, 100, NULL, 1);
					DumpList(prolog, "### inv_rel_11: replaced arc->attr = ", o_lnk->arc->attr, 20, 100, NULL, 1);
				}
			}
			break;
		case INV_REL_1_N:
			if (*arc_crrspnd != p->undefined) break;
			stack = prolog->exec_stack->FindExecStack(arc->TTTTTHI);
			if (stack == NULL) {
				err = prolog->error("after_create_arc(inv_rel_1_n): exec_stack speficied level is NULL.", p->pm_rep);
				return;
			}
			if (n2 == p->undefined) {
				err = prolog->error("inv_for_all create_arc: dst_node undefined. ", p->pm_rep);
				return;
			}
			if (n1 == p->undefined) {
				if (prolog->debug_syn) {
					art_sprintf_0(msg, 256, "inv_for_all create_arc: number of repetition is zero (src_node undefined).\n");
				}
				break;
			}
			if (attr == p->wc_e) {
				AGmNode *var = dst_node2(obj_db, an);
				if (var && var->H) {
					attr = var->TH;
				} else {
					if (prolog->debug_syn) {
						art_sprintf_0(msg, 256, "inv_for_all create_arc: number of repetition is zero (arc_attr is wild card).\n");
					}
					break;
				}
			}
			attr = prolog->eval(attr, obj_db);
			a = CreateArc(n2, n1, attr, NULL, NULL, &o_lnk);
			prolog->Set2(arc_crrspnd, PtrCell(o_lnk), stack->arcs);
			if (prolog->debug_syn2) {
				DumpList(prolog, "### inv_for_all: created arc->dst = ", a->dst, 20, 100, NULL, 1);
				DumpList(prolog, "### inv_for_all: created arc->src = ", a->src, 20, 100, NULL, 1);
				DumpList(prolog, "### inv_for_all: created arc->attr = ", a->attr, 20, 100, NULL, 1);
			}
			break;
		default:
			ASSERT(0);
			break;
		}
	}
}
void AGmGdb::ModifyDeleteArc(AGmNode *b_arcs) {
if (prolog->debug_syn) {
DumpList(prolog, "do modify subgraph (before: delete_arc): ", NULL);
}
	// do modify subgraph (before: delete_arc)
	AGmLink *l5;
	for (l5 = next_outlink(b_arcs, NULL); l5; l5 = next_outlink(b_arcs, l5)) {
		AGmNode *arc = l5->arc->dst;
		AGmNode *an = arc->H;
		AGmNode *common = arc->TTTH;
		AGmNode *ptr_cell = arc->TTTTH;
		if (common->a.s.v) continue;
		if (!IsPtrCell(ptr_cell)) {
			err = prolog->error("delete_arc: arc undefined. ", p->pm_rep);
			return;
		}
		AGmLink* o_lnk = (AGmLink*)ptr_cell->a.p.ptr;
if (prolog->debug_syn2) {
DumpList(prolog, "### deleting arc->src = ", o_lnk->arc->src, 20, 100, NULL, 1);
DumpList(prolog, "### deleting arc->dst = ", o_lnk->arc->dst, 20, 100, NULL, 1);
DumpList(prolog, "### deleting arc->attr = ", o_lnk->arc->attr, 20, 100, NULL, 1);
}
		prolog->DeleteArc2(o_lnk, NULL, prolog->exec_stack->arcs);
	}
}

void AGmGdb::EvaluateChoiceCondition() {
	AGmProlog *pr = new AGmProlog(p);
	AGmNode* p_m = p->PredManip();
	AGmNode* pred = p_m->H;
	RefUnref(p_m);
	AGmNode *manip = Cons(NULL);
	CreateArc(manip, l1->arc->dst);
	pr->exec_stack->obj_db = obj_db;
	pr->PredManip(List(pred, manip));
	ret = pr->Execute();
	delete pr;
	if (IsString(ret)) {
		err = prolog->error("error at choice condition evaluation.", ret);
		return;
	}
}

void AGmGdb::EvaluateCommonCondition() {
if (prolog->debug_syn2) {
DumpGraph(prolog, "šššcwh = ", cwh, 20, 100, NULL, 1);
}
	ret = NULL;
	AGmLink *l2;
	for (l2 = next_outlink(cwh, NULL); l2; l2 = next_outlink(cwh, l2)) {
		AGmProlog *pr = new AGmProlog(p);
		AGmNode* p_m = p->PredManip();
		AGmNode* pred = p_m->H;
		RefUnref(p_m);
		AGmNode *manip = Cons(NULL);
		CreateArc(manip, l2->arc->dst);
		pr->exec_stack->obj_db = obj_db;
		pr->PredManip(List(pred, manip));
		ret = pr->Execute();
		delete pr;
		if (IsString(ret)) {
			err = prolog->error("error at common condition evaluation.", ret);
			return;
		}
		if (ret == two) { // try next repetition
			break;
		}
		if (ret == three) { // break if found
			break;
		}
		if (ret == NULL) break;
		ASSERT(ret == one);
	}
}

void AGmGdb::Tail() {
if (prolog->debug_syn) {
DumpList(prolog, "tail: ", NULL);
}
if (prolog->debug_syn2) {
DumpGraph(prolog, "obj_db = ", obj_db, 10, 100, NULL, 1);
DumpGraph(prolog, "ššštwh = ", twh, 20, 100, NULL, 1);
}
	ret = NULL;
	AGmLink *l2;
	for (l2 = next_outlink(twh, NULL); l2; l2 = next_outlink(twh, l2)) {
		AGmProlog *pr = new AGmProlog(p);
		AGmNode* p_m = p->PredManip();
		AGmNode* pred = p_m->H;
		RefUnref(p_m);
		AGmNode *manip = Cons(NULL);
		CreateArc(manip, l2->arc->dst);
		pr->exec_stack->obj_db = obj_db;
		pr->PredManip(List(pred, manip));
		ret = pr->Execute();
		delete pr;
		if (IsString(ret)) {
			err = prolog->error("error at tail.", ret);
			return;
		}
		if (ret == NULL) break;
		ASSERT(ret == one);
	}
}
