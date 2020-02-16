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
#include <interp/prcsmgr.h>
#include <net/crypt.h>

#include <symbol/parser_generator.h>
#include <symbol/pro_lisp.h>
#include <math/almath.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#if defined(_DEBUG) || defined(_LINUX_DEBUG)
//#define DEBUG_LEAK_CRITICAL
#endif

#define IF_DEBUG(level) if (debug_level >= level && (debug_func == NULL || dst_node(debug_func, func_name)))
#define GVAR	env->TH

int AGmProLisp::max_call_count = 1024;
int AGmProLisp::inv_attr_grmr_syntax_depth = 0;

int AGmProLisp::unq_str_seq = 0;

AGmProLisp::AGmProLisp() {
	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmProLisp());
	if (n == NULL) {
		CreateArc(root, n = Cons(NULL), NmProLisp());
	}
	CreateArc(n, PtrCell(this));

	SetConst();
	prog = Ref(Cons(NULL));
	pred = Ref(Cons(NULL));
	manip = Ref(Cons(NULL));
	env = Ref(List(prog, NULL, pred, manip));

	list_itr = NULL;
}
AGmProLisp::AGmProLisp(AGmNode* l) {
	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmProLisp());
	if (n == NULL) {
		CreateArc(root, n = Cons(NULL), NmProLisp());
	}
	CreateArc(n, PtrCell(this));

	SetConst();
	if (l && IsList(l->H) && l->T && l->TT && IsList(l->TT) && l->TTH && l->TTT && IsList(l->TTT) && l->TTTH) {
		prog = Ref(l->H);
		pred = Ref(l->TTH);
		manip = Ref(l->TTTH);
	} else {
		prog = NULL;
		pred = NULL;
		manip = NULL;
		error = Ref(String("env is illegal."));
	}
	env = Ref(l);

	list_itr = NULL;
}
void AGmProLisp::SetConst() {
	loc = NULL;
	stack = NULL;
	arg_ls_stack = NULL;
	call_count = 0;
	error = NULL;
	error_count = 0;
	error_stack_trace = NULL;
	debug_level = 0;
	debug_func = NULL;

	cont = Ref(UniqueStr("cont"));
	brk = Ref(UniqueStr("brk"));
	next = Ref(UniqueStr("next"));
	para_1 = Ref(UniqueStr("para_1"));
	cond_1 = Ref(UniqueStr("cond_1"));
	s_exp = Ref(UniqueStr("s_exp"));

	asgn_1 = Ref(UniqueStr("asgn_1"));
	or_1 = Ref(UniqueStr("or_1"));
	and_1 = Ref(UniqueStr("and_1"));
	cmp_1 = Ref(UniqueStr("cmp_1"));
	last_eq = Ref(UniqueStr("last_eq"));
	eq = Ref(UniqueStr("eq"));
	ne = Ref(UniqueStr("ne"));
	ge = Ref(UniqueStr("ge"));
	gt = Ref(UniqueStr("gt"));
	le = Ref(UniqueStr("le"));
	lt = Ref(UniqueStr("lt"));
	ls_1 = Ref(UniqueStr("ls_1"));
	ls_n = Ref(UniqueStr("ls_n"));
	wc_t = Ref(UniqueStr("wc_t"));
	wc_e = Ref(UniqueStr("wc_e"));
	t = Ref(UniqueStr("t"));
	add_1 = Ref(UniqueStr("add_1"));
	add = Ref(UniqueStr("add"));
	sub = Ref(UniqueStr("sub"));
	mul_1 = Ref(UniqueStr("mul_1"));
	mul = Ref(UniqueStr("mul"));
	div = Ref(UniqueStr("div"));
	mod = Ref(UniqueStr("mod"));
	null = Ref(UniqueStr("null"));
	const_i = Ref(UniqueStr("const_i"));
	const_s = Ref(UniqueStr("const_s"));
	neg = Ref(UniqueStr("neg"));
	l_not = Ref(UniqueStr("not"));
	var = Ref(UniqueStr("var"));
	braket = Ref(UniqueStr("braket"));
	func_exp = Ref(UniqueStr("func_exp"));
	arg_1 = Ref(UniqueStr("arg_1"));
	pre_inc = Ref(UniqueStr("pre_inc"));
	pre_dec = Ref(UniqueStr("pre_dec"));
	post_inc = Ref(UniqueStr("post_inc"));
	post_dec = Ref(UniqueStr("post_dec"));

	gcd = Ref(UniqueStr("gcd"));
	gcd_ex = Ref(UniqueStr("gcd_ex"));
	inv_mod_m = Ref(UniqueStr("inv_mod_m"));
	crt = Ref(UniqueStr("crt"));
	factor = Ref(UniqueStr("factor"));
	next_prime = Ref(UniqueStr("next_prime"));
	set_var = Ref(UniqueStr("set_var"));
	get_var = Ref(UniqueStr("get_var"));
	ifdef_var = Ref(UniqueStr("ifdef_var"));
	undef_var = Ref(UniqueStr("undef_var"));
	set_ary = Ref(UniqueStr("set_ary"));
	get_ary = Ref(UniqueStr("get_ary"));
	stop = Ref(UniqueStr("stop"));
	attr_grmr = Ref(UniqueStr("attr_grmr"));
	inv_attr_grmr = Ref(UniqueStr("inv_attr_grmr"));
	math = Ref(UniqueStr("math"));
	unq_str = Ref(UniqueStr("unq_str"));

	undefined = Ref(UniqueStr("undefined"));

	ag_prog = Ref(UniqueStr("ag_prog"));

	prog_2 = Ref(UniqueStr("prog_2"));
	block_2 = Ref(UniqueStr("block_2"));
	if_2 = Ref(UniqueStr("if_2"));
	while_2 = Ref(UniqueStr("while_2"));
	for_range_2 = Ref(UniqueStr("for_range_2"));
	for_list_2 = Ref(UniqueStr("for_list_2"));
	for_2 = Ref(UniqueStr("for_2"));
	inv_for_2 = Ref(UniqueStr("inv_for_2"));
	switch_2 = Ref(UniqueStr("switch_2"));
	exp_2 = Ref(UniqueStr("exp_2"));
	ret = Ref(UniqueStr("ret"));

	str_const = Ref(UniqueStr("const"));
	str_no_sp_const = Ref(UniqueStr("no_sp_const"));
	str_not_const = Ref(UniqueStr("not_const"));
	str_no_sp_not_const = Ref(UniqueStr("no_sp_not_const"));
	str_regex = Ref(UniqueStr("regex"));
	str_value = Ref(UniqueStr("$value"));
	str_not_nl = Ref(UniqueStr("not_nl"));
	str_eof = Ref(UniqueStr("EOF"));
	str_reg_ref = Ref(UniqueStr("$reg_ref"));
	str_redirect = Ref(UniqueStr("$redirect"));
	str_define = Ref(UniqueStr("$define"));
	str_undef = Ref(UniqueStr("$undef"));
	str_ifdef = Ref(UniqueStr("$ifdef"));
	str_ifndef = Ref(UniqueStr("$ifndef"));

	str_repeat_0 = Ref(UniqueStr("{}*"));
	str_repeat_1 = Ref(UniqueStr("{}+"));
	str_braket_0 = Ref(UniqueStr("{}?"));
	str_braket_1 = Ref(UniqueStr("{}"));

	syn_var = Ref(UniqueStr("syn_var"));
	syn_ret = Ref(UniqueStr("syn_ret"));
	syn_ret_str = Ref(UniqueStr("$$"));
	syn_loc = Ref(UniqueStr("syn_loc"));
	syn_gen = Ref(UniqueStr("syn_gen"));
	syn_gen_str = Ref(UniqueStr("$gen"));
	syn_tree = Ref(UniqueStr("syn_tree"));
	syn_tree_str = Ref(UniqueStr("$tree"));
	syn_1 = Ref(UniqueStr("$1"));
	syn_2 = Ref(UniqueStr("$2"));
	syn_3 = Ref(UniqueStr("$3"));
	syn_4 = Ref(UniqueStr("$4"));
	syn_5 = Ref(UniqueStr("$5"));
	syn_6 = Ref(UniqueStr("$6"));
	syn_7 = Ref(UniqueStr("$7"));
	syn_8 = Ref(UniqueStr("$8"));
	syn_9 = Ref(UniqueStr("$9"));

	count = Ref(UniqueStr("count"));
	count_str = Ref(UniqueStr("$count"));

	if_str = Ref(UniqueStr("if"));
	for_str = Ref(UniqueStr("for"));
	inv_for_str = Ref(UniqueStr("inv_for"));
	switch_str = Ref(UniqueStr("switch"));
	delay_val = Ref(UniqueStr("delay_val"));

	rule = Ref(UniqueStr("rule"));
	query = Ref(UniqueStr("query"));
	pred_goal = Ref(UniqueStr("pred_goal"));
	pred_not = Ref(UniqueStr("pred_not"));
	next_if_false = Ref(UniqueStr("next_if_false"));

	goals = Ref(UniqueStr("goals"));
	cons = Ref(UniqueStr("cons"));
	list = Ref(UniqueStr("list"));
	append = Ref(UniqueStr("append"));
	member = Ref(UniqueStr("member"));
	length = Ref(UniqueStr("length"));
	starts_with = Ref(UniqueStr("starts_with"));
	ends_with = Ref(UniqueStr("ends_with"));
	grph = Ref(UniqueStr("grph"));
	null_a = Ref(UniqueStr("null_a"));
	grph_eq = Ref(UniqueStr("grph_eq"));
	dest_node = Ref(UniqueStr("dst_node"));
	sorc_node = Ref(UniqueStr("src_node"));
	must_be = Ref(UniqueStr("must_be"));
	eval_script = Ref(UniqueStr("eval_script"));
	dump = Ref(UniqueStr("dump"));
	print = Ref(UniqueStr("write"));
	debug = Ref(UniqueStr("debug"));
	undef = Ref(UniqueStr("undef"));

	grph_src = Ref(UniqueStr("grph_src"));
	grph_dst = Ref(UniqueStr("grph_dst"));
	comm_arc = Ref(UniqueStr("comm_arc"));
	bace = Ref(UniqueStr("bace"));
	cwh = Ref(UniqueStr("cwh"));
	twh = Ref(UniqueStr("twh"));
	invariant= Ref(UniqueStr("invariant"));
	b_nodes = Ref(UniqueStr("b_nodes"));
	b_arcs = Ref(UniqueStr("b_arcs"));
	a_nodes = Ref(UniqueStr("a_nodes"));
	a_arcs = Ref(UniqueStr("a_arcs"));
	nodes = Ref(UniqueStr("nodes"));
	arcs = Ref(UniqueStr("arcs"));
	p_final = Ref(UniqueStr("final"));

	stat = Ref(UniqueStr("stat"));
	opt_grph = Ref(UniqueStr("opt_grph"));
	blk_grph = Ref(UniqueStr("blk_grph"));
	rpt_grph = Ref(UniqueStr("rpt_grph"));
	chc_grph = Ref(UniqueStr("chc_grph"));
	chc_before = Ref(UniqueStr("chc_before"));
	chc_after = Ref(UniqueStr("chc_after"));
	sub_grph = Ref(UniqueStr("sub_grph"));

	obj_db_str = Ref(UniqueStr("obj_db"));

	seq      = Ref(UniqueStr("sequence"));
	glb_seq  = Ref(UniqueStr("global_sequence"));
	is_type  = Ref(UniqueStr("is_type"));
	if_e     = Ref(UniqueStr("if_exist"));
	if_ne    = Ref(UniqueStr("if_not_exist"));
	exist    = Ref(UniqueStr("exist"));
	not_exist = Ref(UniqueStr("not_exist"));
	brk_if_found = Ref(UniqueStr("break_if_found"));

	pm_rep = Ref(UniqueStr("pm_rep"));
	rel_1_1 = Ref(UniqueStr("rel_1_1"));
	rel_1_n = Ref(UniqueStr("rel_1_n"));
	inv_rel_1_1 = Ref(UniqueStr("inv_rel_1_1"));
	inv_rel_1_n = Ref(UniqueStr("inv_rel_1_n"));

	sub_graph = Ref(UniqueStr("sub_graph"));
	node = Ref(UniqueStr("node"));
	arc = Ref(UniqueStr("arc"));

	// ("ls_1", ("wc_e"), ("{}*", ("ls_n", ("wc_t"))))
	any_list = Ref(List(ls_1, Cons(wc_e), List(str_repeat_0, List(ls_n, Cons(wc_t)))));

	comp_grmr_val_stack = NULL;
	attr_grmr_units = NULL;
	syntax_unit_ids = NULL;
	pred_manips = NULL;
	attr_grmr_depth = NULL;
	syntax_id_stack = NULL;
	ids = NULL;
	infinit_recursive_check = NULL;

	ag_unit = NULL;
	prolog = NULL;
}
AGmProLisp::~AGmProLisp() {
	Unref(env);
	Unref(prog);
	Unref(pred);
	Unref(manip);
	Unref(loc);
	Unref(stack);
	Unref(arg_ls_stack);
	Unref(error);
	Unref(error_stack_trace);
	Unref(debug_func);

	Unref(cont);
	Unref(brk);
	Unref(next);
	Unref(para_1);
	Unref(cond_1);
	Unref(s_exp);

	Unref(asgn_1);
	Unref(or_1);
	Unref(and_1);
	Unref(cmp_1);
	Unref(last_eq);
	Unref(eq);
	Unref(ne);
	Unref(ge);
	Unref(gt);
	Unref(le);
	Unref(lt);
	Unref(ls_1);
	Unref(ls_n);
	Unref(wc_t);
	Unref(wc_e);
	Unref(t);
	Unref(add_1);
	Unref(add);
	Unref(sub);
	Unref(mul_1);
	Unref(mul);
	Unref(div);
	Unref(mod);
	Unref(null);
	Unref(const_i);
	Unref(const_s);
	Unref(neg);
	Unref(l_not);
	Unref(var);
	Unref(braket);
	Unref(func_exp);
	Unref(arg_1);
	Unref(pre_inc);
	Unref(pre_dec);
	Unref(post_inc);
	Unref(post_dec);

	Unref(gcd);
	Unref(gcd_ex);
	Unref(inv_mod_m);
	Unref(crt);
	Unref(factor);
	Unref(next_prime);
	Unref(set_var);
	Unref(get_var);
	Unref(ifdef_var);
	Unref(undef_var);
	Unref(set_ary);
	Unref(get_ary);
	Unref(stop);
	Unref(attr_grmr);
	Unref(inv_attr_grmr);
	Unref(math);
	Unref(unq_str);

	Unref(undefined);

	Unref(ag_prog);

	Unref(prog_2);
	Unref(block_2);
	Unref(if_2);
	Unref(while_2);
	Unref(for_range_2);
	Unref(for_list_2);
	Unref(for_2);
	Unref(inv_for_2);
	Unref(switch_2);
	Unref(exp_2);
	Unref(ret);

	Unref(str_const);
	Unref(str_no_sp_const);
	Unref(str_not_const);
	Unref(str_no_sp_not_const);
	Unref(str_regex);
	Unref(str_value);
	Unref(str_not_nl);
	Unref(str_eof);
	Unref(str_reg_ref);
	Unref(str_redirect);
	Unref(str_define);
	Unref(str_undef);
	Unref(str_ifdef);
	Unref(str_ifndef);

	Unref(str_repeat_0);
	Unref(str_repeat_1);
	Unref(str_braket_0);
	Unref(str_braket_1);

	Unref(syn_var);
	Unref(syn_ret);
	Unref(syn_ret_str);
	Unref(syn_loc);
	Unref(syn_gen);
	Unref(syn_gen_str);
	Unref(syn_tree);
	Unref(syn_tree_str);
	Unref(syn_1);
	Unref(syn_2);
	Unref(syn_3);
	Unref(syn_4);
	Unref(syn_5);
	Unref(syn_6);
	Unref(syn_7);
	Unref(syn_8);
	Unref(syn_9);

	Unref(count);
	Unref(count_str);

	Unref(if_str);
	Unref(for_str);
	Unref(inv_for_str);
	Unref(switch_str);
	Unref(delay_val);

	Unref(rule);
	Unref(query);
	Unref(pred_goal);
	Unref(pred_not);
	Unref(next_if_false);
	Unref(goals);
	Unref(cons);
	Unref(list);
	Unref(append);
	Unref(member);
	Unref(length);
	Unref(starts_with);
	Unref(ends_with);
	Unref(grph);
	Unref(null_a);
	Unref(grph_eq);
	Unref(dest_node, sorc_node);
	Unref(must_be);
	Unref(eval_script);
	Unref(dump);
	Unref(print);
	Unref(debug);
	Unref(undef);

	Unref(pm_rep);
	Unref(rel_1_1, rel_1_n, inv_rel_1_1, inv_rel_1_n);
	Unref(sub_graph, node, arc);
	Unref(any_list);

	Unref(grph_src, grph_dst);
	Unref(comm_arc);
	Unref(bace, invariant);
	Unref(cwh, twh, p_final);

	Unref(opt_grph, blk_grph, rpt_grph, chc_grph);
	Unref(chc_before, chc_after, sub_grph, stat);

	Unref(obj_db_str);

	Unref(b_nodes, b_arcs, a_nodes, a_arcs);
	Unref(nodes, arcs);
	Unref(seq, glb_seq, is_type, brk_if_found);
	Unref(if_e, if_ne, exist, not_exist);

	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmProLisp());
	for (AGmLink *l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		if (l->arc->dst->a.p.ptr == this) {
			DeleteArc(l->arc); break;
		}
	}
}

AGmNode *AGmProLisp::PredManip() {
	return List(pred, manip);
}

void AGmProLisp::DebugLevel(int debug_level) {
	this->debug_level = debug_level;
}
void AGmProLisp::DebugFunc(AGmNode* debug_func) {
	Set(this->debug_func, debug_func);
}

void AGmProLisp::MaxCallCount(int max_count) {
	max_call_count = max_count;
}
int AGmProLisp::MaxCallCount() {
	return max_call_count;
}

void AGmProLisp::GCmark() {
	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmProLisp());
	if (n == NULL) return;
	for (AGmLink *l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		AGmProLisp* p = (AGmProLisp*)l->arc->dst->a.p.ptr;
		p->GC_mark();
	}
}
void AGmProLisp::GC_mark() {
	_GCmark(env);
	_GCmark(prog);
	_GCmark(pred);
	_GCmark(manip);
	_GCmark(loc);
	_GCmark(stack);
	_GCmark(arg_ls_stack);
	_GCmark(error);
	_GCmark(error_stack_trace);
	_GCmark(debug_func);

	_GCmark(cont);
	_GCmark(brk);
	_GCmark(next);
	_GCmark(para_1);
	_GCmark(cond_1);
	_GCmark(s_exp);

	_GCmark(asgn_1);
	_GCmark(or_1);
	_GCmark(and_1);
	_GCmark(cmp_1);
	_GCmark(last_eq);
	_GCmark(eq);
	_GCmark(ne);
	_GCmark(ge);
	_GCmark(gt);
	_GCmark(le);
	_GCmark(lt);
	_GCmark(ls_1);
	_GCmark(ls_n);
	_GCmark(wc_t);
	_GCmark(wc_e);
	_GCmark(t);
	_GCmark(add_1);
	_GCmark(add);
	_GCmark(sub);
	_GCmark(mul_1);
	_GCmark(mul);
	_GCmark(div);
	_GCmark(mod);
	_GCmark(null);
	_GCmark(const_i);
	_GCmark(const_s);
	_GCmark(neg);
	_GCmark(l_not);
	_GCmark(var);
	_GCmark(braket);
	_GCmark(func_exp);
	_GCmark(arg_1);
	_GCmark(pre_inc);
	_GCmark(pre_dec);
	_GCmark(post_inc);
	_GCmark(post_dec);

	_GCmark(gcd);
	_GCmark(gcd_ex);
	_GCmark(inv_mod_m);
	_GCmark(crt);
	_GCmark(factor);
	_GCmark(next_prime);
	_GCmark(set_var);
	_GCmark(get_var);
	_GCmark(ifdef_var);
	_GCmark(undef_var);
	_GCmark(set_ary);
	_GCmark(get_ary);
	_GCmark(stop);
	_GCmark(attr_grmr);
	_GCmark(inv_attr_grmr);
	_GCmark(math);
	_GCmark(unq_str);

	_GCmark(undefined);

	_GCmark(ag_prog);

	_GCmark(prog_2);
	_GCmark(block_2);
	_GCmark(if_2);
	_GCmark(while_2);
	_GCmark(for_range_2);
	_GCmark(for_list_2);
	_GCmark(for_2);
	_GCmark(inv_for_2);
	_GCmark(switch_2);
	_GCmark(exp_2);
	_GCmark(ret);

	_GCmark(str_const);
	_GCmark(str_no_sp_const);
	_GCmark(str_not_const);
	_GCmark(str_no_sp_not_const);
	_GCmark(str_regex);
	_GCmark(str_value);
	_GCmark(str_not_nl);
	_GCmark(str_eof);
	_GCmark(str_reg_ref);
	_GCmark(str_redirect);
	_GCmark(str_define);
	_GCmark(str_undef);
	_GCmark(str_ifdef);
	_GCmark(str_ifndef);

	_GCmark(str_repeat_0);
	_GCmark(str_repeat_1);
	_GCmark(str_braket_0);
	_GCmark(str_braket_1);

	_GCmark(syn_var);
	_GCmark(syn_ret);
	_GCmark(syn_ret_str);
	_GCmark(syn_loc);
	_GCmark(syn_gen);
	_GCmark(syn_gen_str);
	_GCmark(syn_tree);
	_GCmark(syn_tree_str);
	_GCmark(syn_1);
	_GCmark(syn_2);
	_GCmark(syn_3);
	_GCmark(syn_4);
	_GCmark(syn_5);
	_GCmark(syn_6);
	_GCmark(syn_7);
	_GCmark(syn_8);
	_GCmark(syn_9);

	_GCmark(count);
	_GCmark(count_str);

	_GCmark(if_str);
	_GCmark(for_str);
	_GCmark(inv_for_str);
	_GCmark(switch_str);
	_GCmark(delay_val);

	_GCmark(rule);
	_GCmark(query);
	_GCmark(pred_goal);
	_GCmark(pred_not);
	_GCmark(next_if_false);
	_GCmark(goals);
	_GCmark(cons);
	_GCmark(list);
	_GCmark(append);
	_GCmark(member);
	_GCmark(length);
	_GCmark(starts_with);
	_GCmark(ends_with);
	_GCmark(grph);
	_GCmark(null_a);
	_GCmark(grph_eq);
	_GCmark(dest_node);
	_GCmark(sorc_node);
	_GCmark(must_be);
	_GCmark(eval_script);
	_GCmark(dump);
	_GCmark(print);
	_GCmark(debug);
	_GCmark(undef);

	_GCmark(pm_rep);
	_GCmark(rel_1_1);
	_GCmark(rel_1_n);
	_GCmark(inv_rel_1_1);
	_GCmark(inv_rel_1_n);
	_GCmark(sub_graph);
	_GCmark(node);
	_GCmark(arc);
	_GCmark(any_list);

	_GCmark(grph_src);
	_GCmark(grph_dst);
	_GCmark(comm_arc);
	_GCmark(bace);
	_GCmark(cwh);
	_GCmark(twh);
	_GCmark(invariant);
	_GCmark(b_nodes);
	_GCmark(b_arcs);
	_GCmark(a_nodes);
	_GCmark(a_arcs);
	_GCmark(nodes);
	_GCmark(arcs);
	_GCmark(p_final);

	_GCmark(stat);
	_GCmark(opt_grph);
	_GCmark(blk_grph);
	_GCmark(rpt_grph);
	_GCmark(chc_grph);
	_GCmark(chc_before);
	_GCmark(chc_after);
	_GCmark(sub_grph);

	_GCmark(obj_db_str);

	_GCmark(seq);
	_GCmark(glb_seq);

	_GCmark(is_type);
	_GCmark(if_e);
	_GCmark(if_ne);
	_GCmark(exist);
	_GCmark(not_exist);
	_GCmark(brk_if_found);

	_GCmark(comp_grmr_val_stack);
	_GCmark(attr_grmr_units);
	_GCmark(syntax_unit_ids);
	_GCmark(pred_manips);
	_GCmark(attr_grmr_depth);
	_GCmark(syntax_id_stack);
	_GCmark(ids);
	_GCmark(infinit_recursive_check);

	if (list_itr) list_itr->GCmark();
}

AGmNode* AGmProLisp::Error(const char* msg) {
#ifdef DEBUG_LEAK_CRITICAL
TRACE1("PLISP ERRROR: msg = %s\n", msg);
#endif

	if (error) return error;

	AGmString *s = new AGmString(msg);

	error = Ref(String(s));
	error_count = 1;

	// stack trace
	s = new AGmString(256);
	AGmNode *ls;
	for (ls = stack; ls; ls = ls->T) {
		s->Append("\t");
		s->Append(ls->HTH->a.s.v);
		s->Append("\n");
	}

	if (ag_unit) {
		AGmFile *f = new AGmFile();
		f->Create(s, AGM_FILE_STR_APPEND);
		f->WriteString("in Attribute Grammar: unit =\n");
		DumpList("syntax_id = ", syntax_id_val, 10, f);
		DumpList(ag_unit, 50, 20, f);
		delete f;
	}

	Set(error_stack_trace, String(s));

	return error;
}

AGmNode* AGmProLisp::Error() {
	return error;
}

AGmNode* AGmProLisp::Load(AGmNode* tree) {
	if (error) return String(error->a.s.v->Str());

	list_itr = new AGmListItr(tree);
	for (;;) {
		AGmNode *func = list_itr->Next("fd");
		if (func == NULL) break;
		set_dst_node(prog, func->TH, func->TT);
	}
	delete list_itr; list_itr = NULL;

	prolog = new AGmProlog(this);
	prolog->PredManip(PredManip());
	list_itr = new AGmListItr(tree);
	for (;;) {
		AGmNode *rule = list_itr->Next("rule");
		if (rule == NULL) break;
		AGmNode *g = Cons(NULL), *obj_db = Cons(NULL), *syn_db = Cons(NULL);
		AGmNode *head = prolog->ToInternalRule(g, rule, obj_db, syn_db);
		if (error) {
			RefUnref(g, obj_db, syn_db);
			RefUnref(head);
			delete list_itr;;
			delete prolog;
			return error;
		}
		if (IsString(head)) {
			RefUnref(obj_db, syn_db);
			delete list_itr;;
			delete prolog;
			return head;
		} else {
			for (AGmLink* l = next_outlink(g, NULL); l; l = next_outlink(g, l)) {
				if (l->arc->dst != head) continue;
				DeleteArc(l->arc); break;
			}
		}
		RefUnref(obj_db, syn_db);
	}
	delete list_itr; list_itr = NULL;
	list_itr = new AGmListItr(tree);
	for (;;) {
		AGmNode *query = list_itr->Next("query");
		if (query == NULL) break;
		AGmNode *g = Cons(NULL), *obj_db = Cons(NULL), *syn_db = Cons(NULL);
		AGmNode *err = prolog->ToInternalQuery(g, query, obj_db, syn_db);
		if (error) {
			RefUnref(g, obj_db, syn_db);
			delete list_itr;;
			delete prolog;
			return error;
		}
		if (IsString(err)) {
			RefUnref(obj_db, syn_db);
			delete list_itr;;
			delete prolog;
			return err;
		}
		RefUnref(obj_db, syn_db);
	}
	delete list_itr; list_itr = NULL;
	delete prolog;
	return List(prog, GVAR, pred, manip);
}
AGmNode* AGmProLisp::Call(const char* func_name, AGmNode* arg_ls) {
	if (error) return String(error->a.s.v->Str());
	call_count = 0;
	AGmNode *name = UniqueStr(func_name);
	AGmNode *ret = call(name, arg_ls);
	RefUnref(name);
IF_DEBUG(10) {
DumpList("CALL RESULT = \n", ret, 20);
}
	if (error && error_stack_trace) {
		error->a.s.v->Append("\n");
		error->a.s.v->Append(error_stack_trace->a.s.v);
	}

	return ret;
}

AGmNode* AGmProLisp::get_loc(AGmNode* name) {
	AGmNode* ls;
	for (ls = loc; ls; ls = ls->T) {
		if (ls->HH == name) {
			return ls->HTH;
		}
	}
	return undefined;
}
void AGmProLisp::set_loc(AGmNode* name, AGmNode* value) {
	Push(loc, List(name, value));
}
AGmNode* AGmProLisp::loc_save() {
	if (loc == NULL) return NULL;
	return loc->HH;
}
void AGmProLisp::pop_loc(AGmNode* name) {
	if (name == NULL) { Unref(loc); loc = NULL; return; }
	AGmNode* ls;
	for (ls = loc; ls; ls = ls->T) {
		if (ls->HH == name) {
			Set(loc, ls); break;
		}
	}
}
int AGmProLisp::unset_loc(AGmNode* name) {
	AGmNode *ls, *prev_ls;
	for (prev_ls = NULL, ls = loc; ls; prev_ls = ls, ls = ls->T) {
		if (ls->HH == name) break;
	}
	if (ls == NULL) return 1;
	if (prev_ls) {
		Set(prev_ls->T, ls->T);
	} else {
		Set(loc, ls->T);
	}
	return 0;
}

AGmNode* AGmProLisp::call(AGmNode* func_name, AGmNode *arg_ls) {
	ADvDoEvents(); if (ADvExiting) return Error("abort request from user.");
	if (call_count++ > max_call_count)
		return Error("stack overflow.");
	char msg[256];
IF_DEBUG(10) {
art_sprintf_1(msg, 256, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\nENTER %s\narg_ls = \n", func_name->a.s.v->Str());
DumpList(msg, arg_ls, 20);
}
	AGmNode *func = dst_node2(prog, func_name);
	if (func == NULL) {
		art_sprintf_1(msg, 256, "function '%s' not found.", func_name->a.s.v->Str());
		Error(msg); return NULL;
	}
	Push(stack, List(loc, func_name)); Unref(loc); loc = NULL;
	AGmNode *a1 = func->HT;
	if (a1) {
		AGmNode *name = a1->HTH;
		if (arg_ls == NULL) {
			art_sprintf_1(msg, 256, "number of arguments of function '%s' not match.", func_name->a.s.v->Str());
			Error(msg); return NULL;
		}
		set_loc(name, arg_ls->H); arg_ls = arg_ls->T;
		AGmNode *an;
		for (an = a1->H->TTHT; an; an = an->T) {
			AGmNode *name = an->HTH;
			if (arg_ls == NULL) {
				art_sprintf_1(msg, 256, "number of arguments of function '%s' not match.", func_name->a.s.v->Str());
				Error(msg); return NULL;
			}
			set_loc(name, arg_ls->H); arg_ls = arg_ls->T;
		}
	}
	if (arg_ls) {
		art_sprintf_1(msg, 256, "number of arguments of function '%s' not match.", func_name->a.s.v->Str());
		Error(msg); return NULL;
	}

#ifdef DEBUG_LEAK_CRITICAL
DumpList("plisp func call: func_name = ", func_name);
//DumpList("plisp func call: loc(arg_ls) = ", loc);
#endif

	int ok = 0;
	AGmNode *para = func->TH, *ret = NULL;
	if (para->H == para_1) {
		ret = eval_para(func_name, para, ok);
		if (error) return error_return(func_name);
		ASSERT(ret == NULL || ret->count >= 0);
	} else if (para->H == prog_2) {
		ret = eval_stat(func_name, para->TH, ok);
		if (error) return error_return(func_name);
		ASSERT(ret == NULL || ret->count >= 0);
	} else {
		Error("<para> or <prog> not found.");
		return error_return(func_name);
	}

	call_count--;
IF_DEBUG(10) {
art_sprintf_1(msg, 256, "LEAVE(ok) %s\nReturn Value = \n", func_name->a.s.v->Str());
DumpList(msg, ret, 20);
DumpList(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", NULL, 20);
}
	ASSERT(ret == NULL || ret->count >= 0);
	if (func_name == attr_grmr || func_name == inv_attr_grmr) {
		Ref(ret);
		Unref(loc); loc = Ref(stack->HH);
		Pop(stack);
		if (ret) ret->count--;
		ASSERT(ret == NULL || ret->count >= 0);
#ifdef DEBUG_LEAK_CRITICAL
DumpList("plisp func call return: func_name = ", func_name);
//DumpList("plisp func call return: loc(arg_ls) = ", loc);
#endif
		return ret;
	} else {
		//Ref(ret);
		AGmNode *ret_2 = ListCopy(ret, 1);
		RefUnref(ret);
		Unref(loc); loc = Ref(stack->HH);
		Pop(stack);
		//if (ret) ret->count--;
		ASSERT(ret_2 == NULL || ret_2->count >= 0);
#ifdef DEBUG_LEAK_CRITICAL
DumpList("plisp func call return: func_name = ", func_name);
//DumpList("plisp func call return: loc(arg_ls) = ", loc);
#endif
		return ret_2;
	}

}
AGmNode* AGmProLisp::error_return(AGmNode* func_name) {
	char msg[256];
	if (error_count == 2) {
IF_DEBUG(10) {
art_sprintf_1(msg, 256, "LEAVE(error) %s\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", func_name->a.s.v->Str());
DumpList(msg, NULL, 20);
}
		return NULL;
	}
	art_sprintf_1(msg, 256, " (in function '%s')", func_name->a.s.v->Str());
	error->a.s.v->Append(msg); error_count = 2;
IF_DEBUG(10) {
art_sprintf_2(msg, 256, "LEAVE(error) %s, error = %s\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", func_name->a.s.v->Str(), error->a.s.v->Str());
DumpList(msg, NULL, 20);
}
	return NULL;
}

AGmNode* AGmProLisp::eval_para(AGmNode* func_name, AGmNode* para, int& ok) {
	AGmNode *ret = NULL;
	AGmNode *ls;
	for (ls = para->THT; ls; ls = ls->T) {
		AGmNode *sent = ls->H;
		ret = eval_sent(func_name, sent, ok);
		if (ok || error) break;
		RefUnref(ret);
	}
	if (ok == 0 && error == NULL) {
		Error("no match clause.");
	}
	if (ok == RES_NEXT) ok = 0;
	return ret;
}
AGmNode* AGmProLisp::eval_sent(AGmNode* func_name, AGmNode* sent, int& ok) {
	AGmNode *cond1 = sent->TH;
	if (eval_cond1(func_name, cond1, ok)) return NULL;

	AGmNode *exp = sent->TTH;
	if (exp->H == s_exp) {
		AGmNode *cond = exp->TH, *value;
		if (cond->H == cond_1) {
			value = eval_exp(func_name, cond->TH, ok);
			if (error) return NULL;
			for (AGmNode *ls = cond->TTHT; ls; ls = ls->T) {
				RefUnref(value);
				if (value == NULL) {
					Error("clause after '->' is false."); return NULL;
				}
				value = eval_exp(func_name, ls->HTH, ok);
				if (ok || error) { RefUnref(value); return NULL; }
			}
		} else {
			value = eval_exp(func_name, cond, ok);
			if (ok || error) { RefUnref(value); return NULL; }
		}
		if (error == NULL) ok = RES_OK;
		return value;
	} else if (exp->H == para_1) {
		return eval_para(func_name, exp, ok);
	} else {
		AGmNode *cond2 = exp->THTH->TH;
		if (eval_cond2(func_name, cond2, ok)) {
			if (error == NULL) Error("clause after '->' is false.");
			return NULL;
		}
		return eval_para(func_name, exp->TTH, ok);
	}
}
int AGmProLisp::eval_cond1(AGmNode* func_name, AGmNode* cond, int& ok) {
IF_DEBUG(30) {
DumpList("eval_cond1: cond = \n", cond, 20);
}
	AGmNode *save = loc_save();
	if (cond->H == cond_1) {
		if (eval_cmp(func_name, cond->TH, 1, ok)) { pop_loc(save); return 1; }
		AGmNode *ls;
		for (ls = cond->TTHT; ls; ls = ls->T) {
			if (eval_cmp(func_name, ls->HTH, 1, ok)) { pop_loc(save); return 1; }
		}
	} else {
		if (eval_cmp(func_name, cond, 1, ok)) { pop_loc(save); return 1; }
	}
	return 0;
}
int AGmProLisp::eval_cond2(AGmNode* func_name, AGmNode* cond, int& ok) {
	if (cond == NULL) return 0;
IF_DEBUG(30) {
DumpList("eval_cond2: cond = \n", cond, 20);
}
	if (cond->H == cond_1) {
		if (eval_cmp(func_name, cond->TH, 0, ok)) return 1;
		AGmNode *ls;
		for (ls = cond->TTHT; ls; ls = ls->T) {
			if (eval_cmp(func_name, ls->HTH, 0, ok)) return 1;
		}
	} else {
		if (eval_cmp(func_name, cond, 0, ok)) return 1;
	}
	return 0;
}
int AGmProLisp::eval_cmp(AGmNode* func_name, AGmNode* cmp, int arith, int& ok) {
	AGmNode *id = cmp->H;
	if (id == cmp_1) {
		AGmNode *exp1 = cmp->TH;
		AGmNode *ls = cmp->TTHT->HT;
		AGmNode *op = ls->HH;
		AGmNode *exp2 = ls->TH;
		if (op == eq) {
			return unify(func_name, exp1, exp2, ok);
		} else if (arith) {
			int flag = eval_cmp_atom(func_name, exp1, exp2, ok);
			if (error) return 1;
			if (op == ne) {
				return !(flag != 0);
			} else if (op == ge) {
				return !(flag >= 0);
			} else if (op == gt) {
				return !(flag > 0);
			} else if (op == le) {
				return !(flag <= 0);
			} else if (op == lt) {
				return !(flag < 0);
			} else {
				Error("unknown compare operator."); return 1;
			}
		} else {
			Error("illegal compare clause."); return 1;
		}
	} else if (id == func_exp) {
		int flag = 0;
		AGmNode *val = eval_exp(func_name, cmp, ok);
		if (error) return 1;
		if (val == NULL) flag = 1;
		RefUnref(val); return flag;
	} else {
		// true;
		return 0;
	}
}
int AGmProLisp::unify(AGmNode* func_name, AGmNode* exp1, AGmNode* exp2, int& ok) {
	char msg[256];
	AGmNode *id1 = exp1->H;
	AGmNode *id2 = exp2->H;
	AGmNode *value, *value2;
	if (id1 == ls_1 && id2 != ls_1) {
		value2 = eval_exp(func_name, exp2, ok);
		if (error) return 1;
		int flag = unify_sub(func_name, value2, exp1);
		RefUnref(value2); return flag;
	}
	if (id1 != var) {
		return eval_cmp_atom(func_name, exp1, exp2, ok);
	}
	AGmNode *name = exp1->TH;
	value = get_loc(name);
	if (value == undefined) {
		value2 = eval_exp(func_name, exp2, ok);
		if (error) return 1;
		set_loc(name, value2);
IF_DEBUG(20) {
art_sprintf_1(msg, 256, "unify %s = \n", name->a.s.v->Str());
DumpList(msg, value2, 20);
}
		return 0;
	}
	if (id2 != ls_1) {
		value2 = eval_exp(func_name, exp2, ok);
		if (error) return 1;
		int flag = (Compare(value, value2) != 0);
		RefUnref(value2); return flag;
	}

	// list-unification
	return unify_sub(func_name, value, exp2);
}
int AGmProLisp::unify_sub(AGmNode* func_name, AGmNode* value, AGmNode* exp2) {
	char msg[256];
	AGmNode *e2 = exp2->TH;
	AGmNode *id2 = e2->H;
	if (id2 == wc_t || id2 == t) {
		Error("first element must not be wild card at list-unification.");
		return 1;
	} else if (id2 == wc_e) {
	} else if (id2 == null) {
		if (value == NULL || !IsList(value)) return 1;
		if (value->H) return 1;
	} else if (id2 == var) {
		if (value == NULL || !IsList(value)) return 1;
		AGmNode *name = e2->TH;
		AGmNode *val = get_loc(name);
		if (val == undefined) {
			set_loc(name, value->H);
IF_DEBUG(20) {
art_sprintf_1(msg, 256, "unify(e, ...) %s = \n", name->a.s.v->Str());
DumpList(msg, value->H, 20);
}
		} else {
			if (Compare(value->H, val) != 0) return 1;
		}
	} else if (id2 == const_i || id2 == const_s) {
		if (value == NULL || !IsList(value)) return 1;
		if (Compare(value->H, e2->TH) != 0) return 1;
	} else if (id2 == ls_1) {
		if (value == NULL || !IsList(value)) return 1;
		if (unify_sub(func_name, value->H, e2)) return 1;
	} else {
		art_sprintf_1(msg, 256, "'%s' can not be used at list-unification(1).", id2->a.s.v->Str());
		Error(msg);
		return 1;
	}
	if (value == NULL || !IsList(value)) return 1;
	value = value->T;
	AGmNode *ls;
	for (ls = exp2->TTHT; ls; ls = ls->T) {
		AGmNode *e2 = ls->HTH;
		id2 = e2->H;
		if (id2 == wc_t) {
			value = NULL; break;
		} else if (id2 == t) {
			AGmNode *ee = e2->TH;
			id2 = ee->H;
			if (id2 == var) {
				AGmNode *name = ee->TH;
				AGmNode *val = get_loc(name);
				if (val == undefined) {
					set_loc(name, value);
IF_DEBUG(20) {
art_sprintf_1(msg, 256, "unify(t...) %s = \n", name->a.s.v->Str());
DumpList(msg, value, 20);
}
				} else {
					if (Compare(value, val) != 0) return 1;
				}
			} else {
				art_sprintf_1(msg, 256, "'%s' can not be used as wild card tail at list-unification", id2->a.s.v->Str());
				Error(msg);
				return 1;
			}
			value = NULL; break;
		} else if (id2 == wc_e) {
		} else if (id2 == null) {
			if (value == NULL || !IsList(value)) return 1;
			if (value->H) return 1;
		} else if (id2 == var) {
			if (value == NULL || !IsList(value)) return 1;
			AGmNode *name = e2->TH;
			AGmNode *val = get_loc(name);
			if (val == undefined) {
				set_loc(name, value->H);
IF_DEBUG(20) {
art_sprintf_1(msg, 256, "unify(..., e, ...) %s = \n", name->a.s.v->Str());
DumpList(msg, value->H, 20);
}
			} else {
				if (Compare(value->H, val) != 0) return 1;
			}
		} else if (id2 == const_i || id2 == const_s) {
			if (value == NULL || !IsList(value)) return 1;
			if (Compare(value->H, e2->TH) != 0) return 1;
		} else if (id2 == ls_1) {
			if (value == NULL || !IsList(value)) return 1;
			if (unify_sub(func_name, value->H, e2)) return 1;
		} else {
			art_sprintf_1(msg, 256, "'%s' can not be used at list-unification(2).", id2->a.s.v->Str());
			Error(msg);
			return 1;
		}
		if (value == NULL || !IsList(value)) return 1;
		value = value->T;
	}
	if (value) return 1;
	return 0;
}
int AGmProLisp::eval_cmp_atom(AGmNode* func_name, AGmNode* exp1, AGmNode* exp2, int& ok) {
	AGmNode *val1 = eval_exp(func_name, exp1, ok);
	if (error) return 0;
	AGmNode *val2 = eval_exp(func_name, exp2, ok);
	if (error) { Unref(val1); return 0; }
	int flag = Compare(val1, val2);
	RefUnref(val1, val2); return flag;
}
AGmNode* AGmProLisp::eval_exp(AGmNode* func_name, AGmNode* exp, int& ok) {
	char msg[256];
	AGmNode *id = exp->H;
	if (id == null) {
		return NULL;
	} else if (id == const_i || id == const_s) {
		return exp->TH;

	} else if (id == var || id == syn_loc || id == syn_gen || id == syn_tree) {
		AGmNode *name = exp->TH;
		AGmNode *val = get_loc(name);
		if (val == undefined) {
#ifdef DEBUG_LEAK_CRITICAL
DumpList("eval_exp, var not found: name = ", name);
DumpList("eval_exp, var not found: val = ", val);
#endif
			art_sprintf_1(msg, 256, "value of variable '%s' not defined.", name->a.s.v->Str());
			Error(msg); return NULL;
		}
		return val;

	} else if (id == syn_ret) {
		AGmNode *val = get_loc(syn_ret_str);
		if (val == undefined) {
			art_sprintf_1(msg, 256, "value of variable '%s' not defined.", syn_ret_str->a.s.v->Str());
			Error(msg); return NULL;
		}
		return val;

	} else if (id == syn_var) {
		if (prolog == NULL) { Error("not in attr_grmr"); return NULL; }
		AGmNode *idxs = prolog->CreateIdxs(exp);
		int err = prolog->SynVar(idxs); RefUnref(idxs);
		if (err == SYN_VAR_NO_DATA) { Error("syn var no data."); return NULL; }
		if (err == FOR_STACK_EMPTY) { Error("for stack empty."); return NULL; }
		if (error) return NULL;
		return prolog->GetStackValue();

	} else if (id == pre_inc || id == pre_dec || id == post_inc || id == post_dec) {
		AGmNode *name = exp->THTH;
		AGmNode *val1 = get_loc(name), *val2;
		if (id == pre_inc || id == post_inc) {
			val2 = BigInteger::Add(val1, AGm::one);
		} else {
			val2 = BigInteger::Sub(val1, AGm::one);
		}
		if (val2 == NULL) { Error("not integer '++','--' operation"); return NULL; }
		if (id == pre_inc || id == pre_dec) {
			unset_loc(name); set_loc(name, val2); return val2;
		} else {
			val1 = ListCopy(val1, 1); unset_loc(name); set_loc(name, val2); return val1;
		}

	} else if (id == ls_1) {
		AGmNode *list = NULL, *check = NULL;
		AGmNode *e1 = exp->TH;
		AGmNode *idid = e1->H;

		if (idid == wc_e || idid == wc_t) {
			Error("wild card appears when composing list(1).");
		} else if (idid == t) {
			list = Nconc(list, check = eval_exp(func_name, e1->TH, ok));
		} else {
			list = Nconc(list, Cons(eval_exp(func_name, e1, ok)));
		}
		if (error == NULL && !IsList(check)) {
			Error("expression is not list at list tail wild card(1).");
		}
		if (error) { Unref(list); return NULL; }

		AGmNode *ls;
		for (ls = exp->TTHT; ls; ls = ls->T) {
			AGmNode *en = ls->HTH, *check = NULL;
			AGmNode *idid = en->H;
			if (idid == wc_e || idid == wc_t) {
				Error("wild card appears when composing list(2).");
			} else if (idid == t) {
				list = Nconc(list, check = eval_exp(func_name, en->TH, ok));
			} else {
				list = Nconc(list, Cons(eval_exp(func_name, en, ok)));
			}
			if (error == NULL && !IsList(check)) {
				Error("expression is not list at list tail wild card(2).");
			}
			if (error) { Unref(list); return NULL; }
		}
		if (list) list->count--;
		return list;

	} else if (id == add_1 || id == mul_1) {
		AGmNode *val_1 = eval_exp(func_name, exp->TH, ok);
		if (error) return NULL;
		if (val_1 == NULL) {
			Error("null value appeared in { +,-,*,/ } operation. (1)");
			RefUnref(val_1); return NULL;
		}
		AGmNode *ls;
		for (ls = exp->TTHT; ls; ls = ls->T) {
			AGmNode *val = NULL;
			AGmNode *val_n = eval_exp(func_name, ls->HTTH, ok);
			if (error) { RefUnref(val_1); return NULL; }
			if (val_n == NULL) {
				Error("null value appeared in { +,-,*,/ } operation. (2)");
				RefUnref(val_1, val_n); return NULL;
			}
			if (ls->HTHH == add) {
				val = BigInteger::Add(val_1, val_n);
				ASSERT(val);
			} else if (ls->HTHH == sub) {
				val = BigInteger::Sub(val_1, val_n);
				ASSERT(val);
			} else if (ls->HTHH == mul) {
				val = BigInteger::Mul(val_1, val_n);
				ASSERT(val);
			} else if (ls->HTHH == div) {
				val = BigInteger::Div(val_1, val_n);
				ASSERT(val);
			} else if (ls->HTHH == mod) {
				val = BigInteger::Mod(val_1, val_n);
				ASSERT(val);
			}
			if (val == NULL) {
				Error("error occured in { +,-,*,/,% } operation.");
				RefUnref(val_1, val_n); return NULL;
			}
			RefUnref(val_1, val_n);
			val_1 = val;
		}
		return val_1;

	} else if (id == neg) {
		AGmNode *val_1 = eval_exp(func_name, exp->TH, ok);
		if (error) return NULL;
		AGmNode *val = BigInteger::Neg(val_1);
		if (val == NULL) {
			Error("not integer value appeared in arithmetic operation.");
			RefUnref(val_1); return NULL;
		}
		RefUnref(val_1);
		return val;

	} else if (id == l_not) {
		AGmNode *value = eval_exp(func_name, exp->TH, ok);
		if (error) return NULL;
		if (value == NULL) return one;
		RefUnref(value); return NULL;
	
	} else if (id == braket) {
		return eval_exp(func_name, exp->TH, ok);

	} else if (id == cont) {
		ok = RES_CONTINUE; return NULL;
	} else if (id == brk) {
		ok = RES_BREAK; return NULL;
	} else if (id == next) {
		ok = RES_NEXT; return NULL;

	} else if (id == asgn_1) {
		AGmNode *value = eval_exp(func_name, exp->TTH, ok);
		if (error) return NULL;
		AGmNode *stack = NULL;
		for (AGmNode *ls = exp->THT; ls; ls = ls->T) Push(stack, ls->HTH);
		while (stack) {
			if (assign(stack->H, value, "assign: lhs is illegal.")) {
				RefUnref(value); Unref(stack); return NULL;
			}
			Pop(stack);
		}
		return value;

	} else if (id == or_1) {
		AGmNode *save = loc_save();
		AGmNode *val = eval_exp(func_name, exp->TH, ok);
		if (error) return NULL;
		if (val) return val;
		pop_loc(save);
		for (AGmNode *ls = exp->TTHT; ls; ls = ls->T) {
			val = eval_exp(func_name, ls->HTH, ok);
			if (error) return NULL;
			if (val) return val;
			pop_loc(save);
		}
		return NULL;

	} else if (id == and_1) {
		AGmNode *val = eval_exp(func_name, exp->TH, ok);
		if (error) return NULL;
		if (val == NULL) return NULL;
		RefUnref(val);
		for (AGmNode *ls = exp->TTHT; ls; ls = ls->T) {
			val = eval_exp(func_name, ls->HTH, ok);
			if (error) return NULL;
			if (val == NULL) return NULL;
			RefUnref(val);
		}
		return one;

	} else if (id == cmp_1) {
		AGmNode *exp1 = exp->TH;
		AGmNode *ls = exp->TTHT->HT;
		AGmNode *op = ls->HH;
		AGmNode *exp2 = ls->TH;
		if (op == eq) {
			int flag = unify(func_name, exp1, exp2, ok);
			if (flag || error) return NULL;
			return one;
		} else {
			int flag = eval_cmp_atom(func_name, exp1, exp2, ok);
			if (error) return NULL;
			if (op == ne) {
				if (flag != 0) return one;	
			} else if (op == ge) {
				if (flag >= 0) return one;
			} else if (op == gt) {
				if (flag > 0) return one;
			} else if (op == le) {
				if (flag <= 0) return one;
			} else if (op == lt) {
				if (flag < 0) return one;
			} else {
				Error("unknown compare operator.");
			}
		}
		return NULL;

	} else if (id == func_exp) {
		AGmNode *name = exp->TH;
		AGmNode *arg_ls = NULL; int argc = 0;
		AGmNode *a1 = exp->TTHT;

		AGmNode *ret = NULL;
		for (;;) {
			if (a1) {
				a1 = a1->H;
				if (a1 && IsList(a1) && a1->H == arg_1) {
					arg_ls = Nconc(arg_ls, Cons(eval_exp(func_name, a1->TH, ok))); argc++;
					if (error) { Unref(arg_ls); return NULL; }
					AGmNode *an;
					for (an = a1->TTHT; an; an = an->T) {
						arg_ls = Nconc(arg_ls, Cons(eval_exp(func_name, an->HTH, ok))); argc++;
						if (error) { Unref(arg_ls); return NULL; }
					}
				} else {
					arg_ls = Nconc(arg_ls, Cons(eval_exp(func_name, a1, ok))); argc++;
					if (error) { Unref(arg_ls); return NULL; }
				}
			}
			Push(arg_ls_stack, arg_ls);

			if (name == gcd) {
				// function: gcd
				AGmNode *arg1, *arg2;
				if (GetArg(argc, arg1, arg2, arg_ls, name)) break;
				AGmNode *val = BigInteger::Gcd(arg1, arg2);
				if (val == NULL) {
					ret = SysFuncError("error (%s).", name); break;
				}
				ret = val;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == gcd_ex) {
				// function: gcd_ex
				AGmNode *arg1, *arg2;
				if (GetArg(argc, arg1, arg2, arg_ls, name)) break;
				AGmNode *val = BigInteger::GcdEx(arg1, arg2);
				if (val == NULL) {
					ret = SysFuncError("error (%s).", name); break;
				}
				ret = val;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == inv_mod_m) {
				// function: inv_mod_m
				AGmNode *a, *mod;
				if (GetArg(argc, a, mod, arg_ls, name)) break;
				AGmNode *val = BigInteger::GFpInverse(a, mod);
				if (val == NULL) {
					ret = SysFuncError("error (%s).", name); break;
				}
				ret = val;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == crt) {
				// function: crt
				AGmNode *a, *m, *b, *n;
				if (GetArg(argc, a, m, b, n, arg_ls, name)) break;
				AGmNode *val = BigInteger::ChineseRemainderTheorem(a, m, b, n);
				if (val == NULL) {
					ret = SysFuncError("error (%s).", name); break;
				}
				ret = val;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == factor) {
				// function: factor
				AGmNode *a;
				if (GetArg(argc, a, arg_ls, name)) break;
				AGmNode *val = BigInteger::factor(a);
				if (val == NULL) {
					ret = SysFuncError("error (%s).", name); break;
				}
				ret = val;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == next_prime) {
				// function: next_prime
				AGmNode *a;
				if (GetArg(argc, a, arg_ls, name)) break;
				if (!IsInteger(a)) {
					ret = SysFuncError("1st argument must be integer (%s).", name); break;
				}
				AGmPNumMgr *mgr = AGmPNumMgr::GetInstance();
				long p = mgr->get_p_num_greater_than((unsigned long)a->a.i.v);
				ret = Integer(p);
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == set_var) {
				// function: set_var
				AGmNode *var_name, *value;
				if (GetArg(argc, var_name, value, arg_ls, name)) break;
				if (!IsString(var_name)) {
					ret = SysFuncError("variable name required (%s).", name); break;
				}
				AGmNode *ls;
				for (ls = GVAR; ls; ls = ls->T) if (ls->HH == var_name) break;
				if (ls == NULL) {
					Push(GVAR, List(var_name, value));
				} else {
					Set(ls->HTH, value);
				}
				ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == get_var) {
				// function: get_var
				AGmNode *var_name;
				if (GetArg(argc, var_name, arg_ls, name)) break;
				if (!IsString(var_name)) {
					ret = SysFuncError("variable name required (%s).", name); break;
				}
				AGmNode *ls;
				for (ls = GVAR; ls; ls = ls->T) if (ls->HH == var_name) break;
				if (ls == NULL) {
					ret = SysFuncError("variable '%s' not defined (%s).", var_name, name);
					break;
				}
				AGmNode *value = ls->HTH;
				ret = value;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == ifdef_var) {
				// function: ifdef_var
				AGmNode *var_name;
				if (GetArg(argc, var_name, arg_ls, name)) break;
				if (!IsString(var_name)) {
					ret = SysFuncError("variable name required (%s).", name); break;
				}
				AGmNode *ls;
				for (ls = GVAR; ls; ls = ls->T) if (ls->HH == var_name) break;
				if (ls == NULL) break;
				ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == undef_var) {
				// function: undef_var
				AGmNode *var_name;
				if (GetArg(argc, var_name, arg_ls, name)) break;
				if (!IsString(var_name)) {
					ret = SysFuncError("variable name required (%s).", name); break;
				}
				AGmNode *prev_ls = NULL, *ls;
				for (ls = GVAR; ls; prev_ls = ls, ls = ls->T) if (ls->HH == var_name) break;
				if (ls) {
					if (prev_ls == NULL) {
						Set(GVAR, ls->T);
					} else {
						Set(prev_ls->T, ls->T);
					}
				}
				ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == set_ary) {
				// function: set_ary
				AGmNode *ary, *idx_ls, *value;
				if (GetArg(argc, ary, idx_ls, value, arg_ls, name)) break;
				if (ary == NULL) {
					ret = SysFuncError("array is null (%s).", name); break;
				}
				if (idx_ls == NULL || !IsList(idx_ls)) {
					ret = SysFuncError("index value illegal (%s).", name); break;
				}
				AGmNode *ls, *n1, *n2 = NULL;
				for (n1 = ary, ls = idx_ls; ls; n1 = n2, ls = ls->T) {
					n2 = dst_node(n1, ls->H);
					if (n2 == NULL) {
						CreateArc(ary, n2 = Cons(NULL), ls->H);
					}
				}
				Set(n2->H, value);
				ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
#ifdef DEBUG_LEAK_CRITICAL
DumpList("set_ary: ary = ", ary, 10, (AGmFile*)NULL, 1);
DumpList("set_ary: idx_ls = ", idx_ls);
DumpList("set_ary: value = ", value);
#endif

				break;

			} else if (name == get_ary) {
				// function: get_ary
				AGmNode *ary, *idx_ls;
				if (GetArg(argc, ary, idx_ls, arg_ls, name)) break;
				if (ary == NULL) {
					ret = SysFuncError("array is null (%s).", name); break;
				}
				if (idx_ls == NULL || !IsList(idx_ls)) {
					ret = SysFuncError("index value illegal (%s).", name); break;
				}
				AGmNode *ls, *n1, *n2 = NULL;
				for (n1 = ary, ls = idx_ls; ls; n1 = n2, ls = ls->T) {
					n2 = dst_node(n1, ls->H);
					if (n2 == NULL) break;
				}
				AGmNode *value = NULL;
				if (n2) value = n2->H;
				ret = value;
				ASSERT(ret == NULL || ret->count >= 0);
#ifdef DEBUG_LEAK_CRITICAL
DumpList("get_ary: ary = ", ary, 10, (AGmFile*)NULL, 1);
DumpList("get_ary: idx_ls = ", idx_ls);
DumpList("get_ary: return = ", ret);
#endif
				break;

			} else if (name == dump) {
				// function: dump
				AGmNode *value, *addr_out;
				if (GetArg(argc, value, addr_out, arg_ls, name)) break;
				DumpGraph(value, 50, NULL, (addr_out ? 1 : 0));
				ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == print) {
				// function: write
				AGmNode *msg, *value;
				if (GetArg(argc, msg, value, arg_ls, name)) break;
				if (!IsString(msg)) {
					ret = SysFuncError("1st argument must be string. (%s).", name); break;
				}
				DumpList(msg->a.s.v->Str(), value, 50);
				ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == debug) {
				// function: debug
#if defined(_DEBUG) || defined(_LINUX_DEBUG)
				AGmNode *msg, *value;
				if (GetArg(argc, msg, value, arg_ls, name)) break;
				if (!IsString(msg)) {
					ret = SysFuncError("1st argument must be string. (%s).", name); break;
				}
				DumpList(msg->a.s.v->Str(), value, 50);
#endif
				ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == append) {
				// function: append
				AGmNode *ls, *elem;
				if (GetArg(argc, ls, elem, arg_ls, name)) break;
				if (!IsList(ls)) {
					ret = SysFuncError("1st argument must be list. (%s).", name); break;
				}
				ret = Append(ls, elem);
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == member) {
				// function: member
				AGmNode *elem, *ls;
				if (GetArg(argc, elem, ls, arg_ls, name)) break;
				if (!IsList(ls)) {
					ret = SysFuncError("1st argument must be list. (%s).", name); break;
				}
				ret = (Member(elem, ls) ? one : NULL);
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == length) {
				// function: length
				AGmNode *ls;
				if (GetArg(argc, ls, arg_ls, name)) break;
				if (!IsList(ls)) {
					ret = SysFuncError("1st argument must be list. (%s).", name); break;
				}
				ret = Integer(Length(ls));
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == starts_with) {
				// function: starts_with
				AGmNode *s1, *s2;
				if (GetArg(argc, s1, s2, arg_ls, name)) break;
				if (!IsString(s1)) {
					ret = SysFuncError("1st argument must be string. (%s).", name); break;
				}
				if (!IsString(s2)) {
					ret = SysFuncError("2nd argument must be string. (%s).", name); break;
				}
				if (s1->a.s.v->StartsWith(s2->a.s.v)) ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == ends_with) {
				// function: ends_with
				AGmNode *s1, *s2;
				if (GetArg(argc, s1, s2, arg_ls, name)) break;
				if (!IsString(s1)) {
					ret = SysFuncError("1st argument must be string. (%s).", name); break;
				}
				if (!IsString(s2)) {
					ret = SysFuncError("2nd argument must be string. (%s).", name); break;
				}
				if (s1->a.s.v->EndsWith(s2->a.s.v)) ret = one;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == eval_script) {
				// function: eval_script
				AGmNode *script, *args;
				if (GetArg(argc, script, args, arg_ls, name)) break;
				AIpProcessMgr* pm = AIpProcessMgr::Instance();
				AIpProcess* p = pm->Process();
				p->EvalScript(script, args);
				ret = p->Val();
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == stop) {
				// function: stop
				AGmNode *value;
				if (GetArg(argc, value, arg_ls, name)) break;
				AGmString *s = new AGmString(64);
				AGmFile *f = new AGmFile();
				f->Create(s, AGM_FILE_STR_WRITE);
				DumpList("message = ", value, 50, f);
				delete f;
				Error(s->Str());
				delete s;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == attr_grmr) {
				// function: attribute_grammar
				AGmNode *parse_tree, *attr_grmr_tree, *opt, *graph;
				if (GetArg(argc, parse_tree, attr_grmr_tree, opt, arg_ls, name)) break;
				graph = AttrGrmr(parse_tree, attr_grmr_tree, opt);
				ret = graph;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == inv_attr_grmr) {
				// function: attribute_grammar
				AGmNode *graph, *attr_grmr_tree, *syntax, *syntax_id, *opt, *parse_tree;
				if (GetArg(argc, graph, attr_grmr_tree, syntax, syntax_id, opt, arg_ls, name))
					break;
				AGmNode* syntax_id_u = Ref(UniqueStr(syntax_id));
				parse_tree = InvAttrGrmr(graph, attr_grmr_tree, syntax, syntax_id_u, opt);
				Unref(syntax_id_u);
				ret = parse_tree;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == math) {
				// function: math
				AGmNode *value = NULL;
				if (AGmMath::InterpretAndExecution(argc, arg_ls, value)) {
					Error(value->a.s.v->Str());
					RefUnref(value); break;
				}
				ret = value;
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else if (name == unq_str) {
				char id[128];
				art_sprintf_1(id, 128, "$$%d", ++unq_str_seq);
				ret = UniqueStr(id);
#ifdef DEBUG_LEAK_CRITICAL
DumpList("unq_str: id = ", ret, 10, (AGmFile*)NULL, 1);
#endif
				ASSERT(ret == NULL || ret->count >= 0);
				break;

			} else {
				ret = call(name, arg_ls);
				ASSERT(ret == NULL || ret->count >= 0);
				break;
			}
		}
		Ref(ret);
		Unref(arg_ls);
		Pop(arg_ls_stack);
		if (ret) ret->count--;
		return ret;

	} else {
		art_sprintf_1(msg, 256, "fatal: unknown expression identifier (%s).", id->a.s.v->Str());
		Error(msg); return NULL;
	}
}

int AGmProLisp::assign(AGmNode* exp, AGmNode* value, const char* msg) {
	AGmNode *name;
	if (exp->H == var || exp->H == syn_loc) {
		name = exp->TH;
	} else if (exp->H == syn_ret) {
		name = syn_ret_str;
	} else {
		Error(msg); return 1;
	}
	unset_loc(name); set_loc(name, value); return 0;
}

int AGmProLisp::GetArg(int argc, AGmNode* name) {
	char msg[256];
	if (argc != 0) {
		art_sprintf_1(msg, 256, "number of arguments of '%s' must be 0.", name->a.s.v->Str());
		Error(msg); return 1;
	}
	return 0;
}
int AGmProLisp::GetArg(int argc, AGmNode*& arg1, AGmNode* arg_ls, AGmNode* name) {
	char msg[256];
	if (argc != 1) {
		art_sprintf_1(msg, 256, "number of arguments of '%s' must be 1.", name->a.s.v->Str());
		Error(msg); return 1;
	}
	arg1 = arg_ls->H;
	return 0;
}
int AGmProLisp::GetArg(int argc, AGmNode*& arg1, AGmNode*& arg2, AGmNode* arg_ls, AGmNode* name) {
	char msg[256];
	if (argc != 2) {
		art_sprintf_1(msg, 256, "number of arguments of '%s' must be 2.", name->a.s.v->Str());
		Error(msg); return 1;
	}
	arg1 = arg_ls->H;
	arg2 = arg_ls->TH;
	return 0;
}
int AGmProLisp::GetArg(int argc, AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode* arg_ls, AGmNode* name) {
	char msg[256];
	if (argc != 3) {
		art_sprintf_1(msg, 256, "number of arguments of '%s' must be 3.", name->a.s.v->Str());
		Error(msg); return 1;
	}
	arg1 = arg_ls->H;
	arg2 = arg_ls->TH;
	arg3 = arg_ls->TTH;
	return 0;
}
int AGmProLisp::GetArg(
		int argc, AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4, AGmNode* arg_ls, AGmNode* name) {
	char msg[256];
	if (argc != 4) {
		art_sprintf_1(msg, 256, "number of arguments of '%s' must be 4.", name->a.s.v->Str());
		Error(msg); return 1;
	}
	arg1 = arg_ls->H;
	arg2 = arg_ls->TH;
	arg3 = arg_ls->TTH;
	arg4 = arg_ls->TTTH;
	return 0;
}
int AGmProLisp::GetArg(int argc,
		AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4, AGmNode*& arg5, AGmNode* arg_ls, AGmNode* name) {
	char msg[256];
	if (argc != 5) {
		art_sprintf_1(msg, 256, "number of arguments of '%s' must be 5.", name->a.s.v->Str());
		Error(msg); return 1;
	}
	arg1 = arg_ls->H;
	arg2 = arg_ls->TH;
	arg3 = arg_ls->TTH;
	arg4 = arg_ls->TTTH;
	arg5 = arg_ls->TTTTH;
	return 0;
}

AGmNode* AGmProLisp::SysFuncError(const char* format, AGmNode* name) {
	int len = (int)strlen(format) + 256;
	char *msg = new char[len];
	art_sprintf_1(msg, len, format, name->a.s.v->Str());
	Error(msg);
	delete [] msg;
	return NULL;
}
AGmNode* AGmProLisp::SysFuncError(const char* format, AGmNode* name1, AGmNode* name2) {
	int len = (int)strlen(format) + 256;
	char *msg = new char[len];
	art_sprintf_2(msg, len, format, name1->a.s.v->Str(), name2->a.s.v->Str());
	Error(msg);
	delete [] msg;
	return NULL;
}

AGmNode* AGmProLisp::eval_stat(AGmNode* func_name, AGmNode* stat, int& ok) {
	AGmNode *id = stat->H;
	if (id == block_2) {
		AGmNode *value = NULL;
		for (AGmNode *ls = stat->THT; ls; ls = ls->T) {
			RefUnref(value);
			value = eval_stat(func_name, ls->H, ok);
			if (error) return NULL;
			if (ok) return value;
		}
		return value;
	} else if (id == if_2) {
		AGmNode *cond = stat->TH, *value;
		if (cond->H == syn_var) {
			if (prolog == NULL) { Error("not in attr_grmr"); return NULL; }
			AGmNode *idxs = prolog->CreateIdxs(cond);
			int err = prolog->SynVar(idxs); RefUnref(idxs);
			if (err == SYN_VAR_NO_DATA) { Error("syn var no data."); return NULL; }
			if (err == FOR_STACK_EMPTY) { Error("for stack empty."); return NULL; }
			if (error) return NULL;
			value = prolog->GetStackValue();
			if (value == NULL || !IsList(value)) { Error("if arg is null."); return NULL; }
			value = value->T;
		} else {
			value = eval_exp(func_name, cond, ok);
			if (error) return NULL;
		}
		RefUnref(value);
		if (value) {
			return eval_stat(func_name, stat->TTH, ok);
		} else {
			AGmNode *stat2 = stat->TTTH->T;
			if (stat2 == NULL) return NULL;
			return eval_stat(func_name, stat2->HTH, ok);
		}
	} else if (id == for_2) { // syn_var
		AGmNode *exp = stat->TH;
		if (exp && IsList(exp) && exp->H == syn_var) {
			if (prolog == NULL) { Error("not in attr_grmr"); return NULL; }
			AGmNode *idxs = prolog->CreateIdxs(exp);
			int err = prolog->SynVar(idxs);
			if (err == SYN_VAR_NO_DATA) { RefUnref(idxs); Error("syn var no data."); return NULL; }
			if (err == FOR_STACK_EMPTY) { RefUnref(idxs); Error("for stack empty."); return NULL; }
			if (error) { RefUnref(idxs); return NULL; }
			AGmNode *pos = prolog->GetStackValue();
			if (pos == NULL || !IsList(pos)) { RefUnref(idxs); Error("for arg is null."); return NULL; }
			pos = pos->T;
			int n = Length(pos); AGmNode *value = NULL;
			for (int i = 0; i < n; i++) {
				set_dst_node(prolog->for_stack, idxs, Integer(i));
				RefUnref(value);
				value = eval_stat(func_name, stat->TTH, ok);
				if (error) { RefUnref(idxs); return NULL; }
			}
			set_dst_node(prolog->for_stack, idxs, NULL);
			RefUnref(idxs); return value;
		} else {
			error = Error("for () required syn var arg."); return NULL;
		}
	} else if (id == inv_for_2) { // syn_var
		AGmNode *exp = stat->TH;
		if (exp && IsList(exp) && exp->H == syn_var) {
			if (prolog == NULL) { Error("not in attr_grmr"); return NULL; }
			AGmNode *idxs = prolog->CreateIdxs(exp);
			int err = prolog->SynVar(idxs);
			if (err == SYN_VAR_NO_DATA) { RefUnref(idxs); Error("syn var no data."); return NULL; }
			if (err == FOR_STACK_EMPTY) { RefUnref(idxs); Error("for stack empty."); return NULL; }
			if (error) { RefUnref(idxs); return NULL; }
			AGmNode *pos = prolog->GetStackValue();
			if (pos == NULL || !IsList(pos)) { RefUnref(idxs); Error("inv_for arg is null."); return NULL; }
			pos = pos->T;
			int n = Length(pos); AGmNode *value = NULL;
			for (int i = n-1; i >= 0; i--) {
				set_dst_node(prolog->for_stack, idxs, Integer(i));
				RefUnref(value);
				value = eval_stat(func_name, stat->TTH, ok);
				if (error) { RefUnref(idxs); return NULL; }
			}
			set_dst_node(prolog->for_stack, idxs, NULL);
			RefUnref(idxs); return value;
		} else {
			Error("inv_for required syn var arg."); return NULL;
		}
	} else if (id == switch_2) {
		AGmNode *exp = stat->TH, *id;
		if (exp && IsList(exp) && exp->H == syn_var) {
			if (prolog == NULL) { Error("not in attr_grmr"); return NULL; }
			AGmNode *idxs = prolog->CreateIdxs(exp);
			int err = prolog->SynVar(idxs); RefUnref(idxs);
			if (err == SYN_VAR_NO_DATA) { Error("syn var no data."); return NULL; }
			if (err == FOR_STACK_EMPTY) { Error("for stack empty."); return NULL; }
			if (error) return NULL;
			id = prolog->GetSwitchId();
		} else {
			id = eval_exp(func_name, exp, ok);
		}
		AGmNode *ls, *value = NULL;
		for (ls = stat->TTHT; ls; ls = ls->T) {
			AGmNode *id2 = eval_exp(func_name, ls->HTH, ok);
			if (error) { RefUnref(id); return NULL; }
			int flag = Compare(id2, id);
			RefUnref(id2);
			if (flag == 0) break;
		}
		RefUnref(id);
		if (ls == NULL) return NULL;
		for (ls = ls->HTTH->T; ls; ls = ls->T) {
			RefUnref(value);
			value = eval_stat(func_name, ls->H, ok);
			if (ok || error) return NULL;
		}
		return value;
	} else if (id == for_range_2) {
		AGmNode *name = stat->TH;
		AGmNode *from = eval_exp(func_name, stat->TTH, ok);
		if (error) return NULL;
		AGmNode *to = eval_exp(func_name, stat->TTTH, ok);
		if (error) { RefUnref(from); return NULL; }
#ifdef DEBUG_LEAK_CRITICAL
DumpList("for_range: name = ", name);
DumpList("for_range: from = ", from);
DumpList("for_range: to = ", to);
#endif
		unset_loc(name); set_loc(name, from);
		AGmNode *value = NULL;
		for (;;) {
			AGmNode *index = get_loc(name);
#ifdef DEBUG_LEAK_CRITICAL
DumpList("for_range: index = ", index);
#endif
			int flag = BigInteger::Compare(index, to);
			if (flag == TYPE_ERROR) { RefUnref(from, to); return Error("for: range is illegal."); }
			if (flag > 0) break;
			RefUnref(value);
			value = eval_stat(func_name, stat->TTTTH, ok);
			if (error) return NULL;
			if (ok == RES_BREAK) { ok = 0; break; }
			if (ok == RES_CONTINUE) ok = 0;
			if (ok == RES_RETURN) break;
			AGmNode *next_index = BigInteger::Add(index, one);
#ifdef DEBUG_LEAK_CRITICAL
DumpList("for_range: next_index = ", next_index);
#endif
			set_loc(name, next_index);
		}
		RefUnref(from, to); return value;
	} else if (id == for_list_2) {
		AGmNode *name = stat->TH;
		AGmNode *list = eval_exp(func_name, stat->TTH, ok);
		if (error) return NULL;
		AGmNode *value = NULL;
		for (AGmNode *ls = list; ls; ls = ls->T) {
			if (!IsList(ls)) { RefUnref(list); return Error("for: list is illegal."); }
			unset_loc(name); set_loc(name, ls->H);
			RefUnref(value);
			value = eval_stat(func_name, stat->TTTH, ok);
			if (ok == RES_BREAK) { ok = 0; break; }
			if (ok == RES_CONTINUE) ok = 0;
			if (ok == RES_RETURN) break;
		}
		RefUnref(list); return value;
	} else if (id == while_2) {
		AGmNode *cond = stat->TH, *value = NULL;
		for (;;) {
			RefUnref(value);
			value = eval_exp(func_name, cond, ok);
			if (error) return NULL;
			if (value) {
				RefUnref(value);
				value = eval_stat(func_name, stat->TTH, ok);
				if (ok == RES_BREAK) { ok = 0; break; }
				if (ok == RES_CONTINUE) ok = 0;
				if (ok == RES_RETURN) break;
			} else {
				break;
			}
		}
		return value;

	} else if (id == exp_2) {
		return eval_exp(func_name, stat->TH, ok);

	} else if (id == ret) {
		AGmNode *value = NULL;
		if (stat->THT) {
			value = eval_exp(func_name, stat->TH->TH, ok);
			if (error) return NULL;
		}
		ok = RES_RETURN;
		return value;

	} else {
		char msg[256];
		art_sprintf_1(msg, 256, "unknown statement identifier (%s).", id->a.s.v->Str());
		Error(msg);
		return NULL;
	}
}

AGmNode* AGmProLisp::AttrGrmr(AGmNode* parse_tree, AGmNode* attr_grmr_tree, AGmNode* opt) {
	prolog = new AGmProlog(this);
	prolog->TreeConv();

	attr_grmr_units = Ref(Cons(NULL));
	syntax_unit_ids = Ref(Cons(NULL));
	pred_manips = Ref(Cons(NULL));
	syntax_id_stack = NULL;

	attr_grmr_depth = Ref(Integer(0));

	AGmProlog::debug_pred = 0;
	AGmProlog::debug_dump = 0;
	AGmProlog::debug_syn = 0;
	AGmProlog::debug_syn2 = 0;
	AGmProlog::debug_syn3 = 0;
	AGmProlog::debug_syntax_id = NULL;
	AGmProlog::debug_syntax_id2 = NULL;
	AGmProlog::debug_syntax_id3 = NULL;
	if (opt) {
		if (dst_node(opt, "debug_pred")) AGmProlog::debug_pred = 1;
		if (dst_node(opt, "debug_dump")) AGmProlog::debug_dump = 1;
		if (dst_node(opt, "debug_syn")) AGmProlog::debug_syn = 1;
		if (dst_node(opt, "debug_syn2")) AGmProlog::debug_syn2 = 1;
		if (dst_node(opt, "debug_syn3")) AGmProlog::debug_syn3 = 1;

		AGmNode *n = dst_node(opt, "debug_syntax_id");
		if (IsString(n)) AGmProlog::debug_syntax_id = n;
		n = dst_node(opt, "debug_syntax_id2");
		if (IsString(n)) AGmProlog::debug_syntax_id2 = n;
		n = dst_node(opt, "debug_syntax_id3");
		if (IsString(n)) AGmProlog::debug_syntax_id3 = n;
	}

	list_itr = new AGmListItr(attr_grmr_tree);
	for (;;) {
		AGmNode *n = list_itr->Next("syntax_unit");
		if (n == NULL) break;
		CreateArc(attr_grmr_units, n, n->TH);
		AGmNode *u = ToListFromListExp(n->TTH);
		if (u) CreateArc(syntax_unit_ids, u, n->TH);
	}
	delete list_itr; list_itr = NULL;

	AGmListCB *cb = new AGmAttrGrmrCB(this);
	list_itr = new AGmListItr(parse_tree, cb);
	list_itr->AttrGrmrCB();
	for (;;) {
		ADvDoEvents(); if (ADvExiting) break;
		AGmNode *ls = list_itr->Next();
		if (ls == NULL) break;
	}
	delete list_itr; list_itr = NULL;
	delete cb;

	AGmNode *graph = NULL;
	if (error == NULL) graph = get_loc(syn_ret_str);

	Unref(comp_grmr_val_stack); comp_grmr_val_stack = NULL;
	Unref(attr_grmr_units); attr_grmr_units = NULL;
	Unref(syntax_unit_ids); syntax_unit_ids = NULL;
	Unref(pred_manips); pred_manips = NULL;
	Unref(attr_grmr_depth); attr_grmr_depth = NULL;
	Unref(syntax_id_stack); syntax_id_stack = NULL;

	delete prolog; prolog = NULL;
	return graph;
}
AGmNode* AGmProLisp::InvAttrGrmr(AGmNode* graph, AGmNode* attr_grmr_tree, AGmNode* syntax, AGmNode* syntax_id, AGmNode* opt) {

	inv_attr_grmr_syntax_depth = 0;

	if (syntax == NULL || !IsList(syntax) || syntax->T == NULL || syntax->TH == NULL) {
		Error("compiled syntax illegal."); return NULL;
	}
	ids = Ref(syntax->TH);

	attr_grmr_units = Ref(Cons(NULL));
	syntax_unit_ids = Ref(Cons(NULL));
	pred_manips = Ref(Cons(NULL));
	infinit_recursive_check = Ref(Cons(NULL));
	syntax_id_stack = NULL;

	attr_grmr_depth = Ref(Integer(0));

	inv_ag_error_grph = 0;

	AGmProlog::debug_pred = 0;
	AGmProlog::debug_dump = 0;
	AGmProlog::debug_syn = 0;
	AGmProlog::debug_syn2 = 0;
	AGmProlog::debug_syn3 = 0;
	AGmProlog::debug_syntax_id = NULL;
	AGmProlog::debug_syntax_id2 = NULL;
	AGmProlog::debug_syntax_id3 = NULL;
	if (opt) {
		if (dst_node(opt, "debug_pred")) AGmProlog::debug_pred = 1;
		if (dst_node(opt, "debug_dump")) AGmProlog::debug_dump = 1;
		if (dst_node(opt, "debug_syn")) AGmProlog::debug_syn = 1;
		if (dst_node(opt, "debug_syn2")) AGmProlog::debug_syn2 = 1;
		if (dst_node(opt, "debug_syn3")) AGmProlog::debug_syn3 = 1;

		AGmNode *n = dst_node(opt, "debug_syntax_id");
		if (IsString(n)) AGmProlog::debug_syntax_id = n;
		n = dst_node(opt, "debug_syntax_id2");
		if (IsString(n)) AGmProlog::debug_syntax_id2 = n;
		n = dst_node(opt, "debug_syntax_id3");
		if (IsString(n)) AGmProlog::debug_syntax_id3 = n;
	}

	AGmListItr *itr;
	itr = new AGmListItr(attr_grmr_tree);
	for (;;) {
		AGmNode *n = itr->Next("syntax_unit");
		if (n == NULL) break;
		CreateArc(attr_grmr_units, n, n->TH);
		AGmNode *u = ToListFromListExp(n->TTH);
		if (u) CreateArc(syntax_unit_ids, u, n->TH);
	}
	delete itr;

	AGmNode *parse_tree = InvAttrGrmr2(graph, syntax_id, 0);
if (AGmProlog::debug_dump || AGmProlog::debug_syntax_id || AGmProlog::debug_syntax_id2 || AGmProlog::debug_syntax_id3) {
DumpList("================================ Init: Tree =\n", parse_tree, 200);
}
	if (parse_tree) {
		AGmNode *tree = Ref(ParseTreeNormalize(parse_tree));
		RefUnref(parse_tree); parse_tree = tree; parse_tree->count--;
if (AGmProlog::debug_dump || AGmProlog::debug_syntax_id || AGmProlog::debug_syntax_id2 || AGmProlog::debug_syntax_id3) {
DumpList("================================ After Normalize(1): Tree =\n", parse_tree, 200);
}
		AGmNode *grmr = dst_node(ids, syntax_id);
		if (grmr == NULL) {
			Error("syntax id incorrect.");
		} else {
			int err = ParseTreeCorrect(grmr, parse_tree);
			if (err && error == NULL) Error("constructed parse tree validation error.");
		}
if (AGmProlog::debug_dump || AGmProlog::debug_syntax_id || AGmProlog::debug_syntax_id2 || AGmProlog::debug_syntax_id3) {
DumpList("================================ After Correct: Tree =\n", parse_tree, 200);
}
		if (error == NULL) {
			tree = Ref(ParseTreeNormalize(parse_tree));
			RefUnref(parse_tree); parse_tree = tree; parse_tree->count--;
		}
if (AGmProlog::debug_dump || AGmProlog::debug_syntax_id || AGmProlog::debug_syntax_id2 || AGmProlog::debug_syntax_id3) {
DumpList("================================ After Normalize(2): Tree =\n", parse_tree, 200);
ADvWindow::Print("================================ \n");
}
	}

	Unref(attr_grmr_units); attr_grmr_units = NULL;
	Unref(syntax_unit_ids); syntax_unit_ids = NULL;
	Unref(pred_manips); pred_manips = NULL;
	Unref(ids); ids = NULL;
	Unref(syntax_id_stack); syntax_id_stack = NULL;
	Unref(infinit_recursive_check); infinit_recursive_check = NULL;
	Unref(attr_grmr_depth); attr_grmr_depth = NULL;

	if (error) { RefUnref(parse_tree); parse_tree = NULL; }

	return parse_tree;
}

AGmNode* AGmProLisp::InvAttrGrmr2(AGmNode* graph, AGmNode* syntax_id, int call_count) {
	syntax_id_val = syntax_id;
	AGmNode *attr_grmr_unit = dst_node2(attr_grmr_units, syntax_id);
	if (attr_grmr_unit == NULL) return NULL;
	ASSERT(syntax_id_val == attr_grmr_unit->TH);
	grammar_val = dst_node2(syntax_unit_ids, syntax_id_val);

	AGmNode *nn = dst_node(infinit_recursive_check, syntax_id);
	if (nn == NULL) {
		CreateArc(infinit_recursive_check, nn = Cons(NULL), syntax_id);
	}
	if (dst_node(nn, (long)graph)) return NULL;
	CreateArc(nn, zero, Integer((long)graph));

	AGmProlog *prolog = new AGmProlog(this);
	prolog->TreeConv(syntax_id_val, grammar_val);
	prolog->CallCount(call_count);

	Push(syntax_id_stack, syntax_id);

	attr_grmr_depth->a.i.v++;
	if (attr_grmr_depth->a.i.v > inv_attr_grmr_syntax_depth) {
		inv_attr_grmr_syntax_depth = (int)attr_grmr_depth->a.i.v;
	}

int save_debug_pred = AGmProlog::debug_pred;
int save_debug_dump = AGmProlog::debug_dump;
int save_debug_syn = AGmProlog::debug_syn;
int save_debug_syn2 = AGmProlog::debug_syn2;
int save_debug_syn3 = AGmProlog::debug_syn3;
if (AGmProlog::debug_syntax_id || AGmProlog::debug_syntax_id2 || AGmProlog::debug_syntax_id3) {
	if ((AGmProlog::debug_syntax_id && syntax_id->a.s.v->Compare(AGmProlog::debug_syntax_id->a.s.v) == 0)
			|| (AGmProlog::debug_syntax_id2 && syntax_id->a.s.v->Compare(AGmProlog::debug_syntax_id2->a.s.v) == 0)
			|| (AGmProlog::debug_syntax_id3 && syntax_id->a.s.v->Compare(AGmProlog::debug_syntax_id3->a.s.v) == 0)) {
		AGmProlog::debug_pred = 1;
		AGmProlog::debug_syn = 1;
		if (AGmProlog::debug_syntax_id2 && syntax_id->a.s.v->Compare(AGmProlog::debug_syntax_id2->a.s.v) == 0) {
			AGmProlog::debug_syn2 = 1;
			AGmProlog::debug_syn3 = 1;
		} else {
			AGmProlog::debug_syn2 = 0;
			AGmProlog::debug_syn3 = 0;
		}
		if (AGmProlog::debug_syntax_id3 && syntax_id->a.s.v->Compare(AGmProlog::debug_syntax_id3->a.s.v) == 0) {
			AGmProlog::debug_dump = 1;
		} else {
			AGmProlog::debug_dump = 0;
		}
	} else {
		AGmProlog::debug_pred = 0;
		AGmProlog::debug_dump = 0;
		AGmProlog::debug_syn = 0;
		AGmProlog::debug_syn2 = 0;
		AGmProlog::debug_syn3 = 0;
	}
}

if (AGmProlog::debug_syn) {
DumpList("<<<¡¡¡ depth = ", attr_grmr_depth);
DumpList("<<<¡¡¡ Begin: id = ", syntax_id);
}
if (AGmProlog::debug_syn) {
DumpGraph("<<<¡¡¡ graph = ", graph, 20, 20);
}

	ag_unit = grammar_val;
	AGmNode *n = attr_grmr_unit->TTTH;
	if (n->H == ag_prog) {
		AGmNode *err;
		AGmNode *pm = dst_node2(pred_manips, syntax_id);
		if (pm == NULL) {
			err = prolog->ToInternal(n);
			if (err) {
				Error(err->a.s.v->Str()); RefUnref(err);
				ag_unit = NULL; delete prolog; return NULL;
			}
			set_dst_node(pred_manips, syntax_id, pm = prolog->PredManip());
		}
		pm = GraphCopy(pm, 1);
		prolog->PredManip(pm);

		unset_loc(syn_ret_str); set_loc(syn_ret_str, graph);

		err = prolog->Execute();
		if (IsString(err)) {
			if (error == NULL) {
				Error(err->a.s.v->Str()); RefUnref(err);
			}
		} else if (IsInteger(err)) {
			if (err != one && error == NULL) Error("execution fail: no match suitable rule.");
		}
		if (error && inv_ag_error_grph == 0) {
			inv_ag_error_grph = 1;
			AGmFile *f = new AGmFile();
			f->Create(error->a.s.v, AGM_FILE_STR_APPEND);
			f->WriteString("\n");
			DumpGraph("graph = ", graph, 10, 10, f);
			delete f;
		}
		if (err) {
			RefUnref(prolog->ptree0); prolog->ptree0 = NULL;

if (AGmProlog::debug_syn) {
DumpList(">>>¡¡¡ depth = ", attr_grmr_depth);
DumpList(">>>¡¡¡ Fail: id = ", syntax_id);
}
		} else {
if (AGmProlog::debug_syn) {
DumpList(">>>¡¡¡ depth = ", attr_grmr_depth);
DumpList(">>>¡¡¡ Success: id = ", syntax_id);
}
if (AGmProlog::debug_syn) {
DumpList(">>>¡¡¡ After ptree0 = ", prolog->ptree0, 20, 20);
}
		}
	} else {
		RefUnref(prolog->ptree0); prolog->ptree0 = NULL;
	}
	ag_unit = NULL;

	set_dst_node(nn, (long)graph, NULL);

	Pop(syntax_id_stack);

	attr_grmr_depth->a.i.v--;

	AGmProlog::debug_pred = save_debug_pred;
	AGmProlog::debug_dump = save_debug_dump;
	AGmProlog::debug_syn = save_debug_syn;
	AGmProlog::debug_syn2 = save_debug_syn2;
	AGmProlog::debug_syn3 = save_debug_syn3;

	AGmNode *parse_tree = prolog->ptree0;
	delete prolog; return parse_tree;
}

AGmNode* AGmProLisp::ParseTreeNormalize(AGmNode* tree) {
	if (tree == NULL || IsString(tree)) return tree;
	for (AGmNode *ls = tree; ls; ls = ls->T) {
		if (IsString(ls->H) && ls->T && ls->TT == NULL
				&& ls->H != str_braket_0 && ls->H != str_repeat_0 && ls->H != str_repeat_1
				&& IsList(ls->TH) && (!IsString(ls->THH)
					|| (ls->THH != str_braket_0 && ls->THH != str_repeat_0 && ls->THH != str_repeat_1
						&& dst_node2(ids, ls->THH) == NULL)) ) {
			Set(ls->T, ParseTreeNormalize(ls->TH));
			for (; ls->T; ls = ls->T) ;
		} else if (IsList(ls->H) && ls->HH == str_braket_1 && Length(ls->H) == 2) {
			Set(ls->H, ParseTreeNormalize(ls->HTH));
		} else {
			Set(ls->H, ParseTreeNormalize(ls->H));
		}
	}
	return tree;
}

#ifdef DEBUG_PTREE
static int ptree_depth = 0;
#endif

int AGmProLisp::ParseTreeCorrect(AGmNode* grmr, AGmNode* tree) {
#ifdef DEBUG_PTREE
char msg[64];
art_sprintf_1(msg, 64, "Enter depth = %d\n", ++ptree_depth);
ADvWindow::Print(msg);
DumpList("grmr = ", grmr, 10, 10);
DumpList("tree = ", tree, 10, 10);
#endif

	if (IsString(grmr->H)) grmr = grmr->T;
	if (IsString(tree->H)) tree = tree->T;
	// tree ((...), ... , (...))
		// (...) = (xxx, (...), ...) | ({}, (...), ...) | ({}?, (...), ...) | ({}+, (...), ...) | ({}*, (...), ...)
	// grmr ((...), ... , (...))
		// (...) = (xxx, ((...), ...)) | ({}, (...), ...) | ({}?, (...), ...) | ({}+, (...), ...) | ({}*, (...), ...)

	for (; tree && grmr; tree = tree->T, grmr = grmr->T) {
		if (IsString(tree->H)) continue;
		if (IsString(grmr->H)) continue;
		AGmNode *tree2 = NULL, *grmr2 = NULL;

		for (; grmr;) {
			AGmNode *h = grmr->HH;
			if (h == str_const || h == str_no_sp_const || h == str_regex || h == str_value || h == str_reg_ref || h == str_eof
					|| h == str_not_const || h == str_no_sp_not_const || h == str_not_nl || h == str_redirect
					|| h == str_define || h == str_undef || h == str_ifdef || h == str_ifndef) {
				grmr = grmr->T; continue;
			}
			grmr2 = grmr->H;
			tree2 = tree->H;
			break;
		}
		ASSERT(tree2 && grmr && grmr2);

#ifdef DEBUG_PTREE
DumpList("grmr2 = ", grmr2, 10, 10);
DumpList("tree2 = ", tree2, 10, 10);
#endif
		if (tree2->H == str_braket_1) {
			ASSERT(grmr2->H == tree2->H);
			// tree2 = ({}, (...), ...)
			// grmr2 = ({}, (...), ...)

			AGmNode *ls;
			for (ls = grmr2->T; ls; ls = ls->T) {
#ifdef DEBUG_PTREE
DumpList("correct syntax_id(1) = ", ls->HH);
#endif
				AGmNode *sub_tree = GetSubParseTree(ls->HH, ls->H, NULL, 0);
				if (error) {
					AGmString *err = new AGmString("Correct Tree Error(1):\n");
					err->Append(error->a.s.v);
					Set(error, String(err));
					return 1;
				}
				if (sub_tree == NULL) continue;
				Set(tree->H, sub_tree);
				break;
			}
			if (ls == NULL) {
				AGmString *s = new AGmString("correct failed(1): syntax = ");
				int cnt = 0;
				s->WriteList(tree2, cnt);
				Error(s->Str()); delete s; return 1;
			}

		} else if (Length(tree2) == 1 && Length(grmr2) > 1 && dst_node(syntax_unit_ids, tree2->H)) {
			// tree2 = (xxx)
			// grmr2 = (xxx, ((...), ...))

			if (grmr2->H == str_braket_1) {
				for (grmr2 = grmr2->T; grmr2; grmr2 = grmr2->T) {
					if (IsList(grmr2->H) && tree2->H == grmr2->HH) {
						grmr2 = grmr2->H; break;
					}
				}
				ASSERT(grmr2);
#ifdef DEBUG_PTREE
DumpList("{} grmr2 = ", grmr2, 10, 10);
DumpList("{} tree2 = ", tree2, 10, 10);
#endif
			}
#ifdef DEBUG_PTREE
DumpList("correct syntax_id(2) = ", tree2->H);
#endif
			ASSERT(grmr2->H == tree2->H);
			AGmNode *sub_tree = GetSubParseTree(tree2->H, grmr2, NULL, 0);
			if (error) {
				AGmString *err = new AGmString("Correct Tree Error(2):\n");
				err->Append(error->a.s.v);
				Set(error, String(err));
				return 1;
			}
			if (sub_tree == NULL) {
				AGmString *s = new AGmString("correct failed(2): syntax = ");
				int cnt = 0;
				s->WriteList(tree2, cnt);
				Error(s->Str()); delete s; return 1;
			}
			Set(tree->H, sub_tree);

		} else if (tree2->H == str_braket_0) {
			ASSERT(grmr2->H == tree2->H);
			// tree2 = ({}?, (...))
			// grmr2 = ({}?, (...), ...)

			if (tree2->T == NULL) continue;
			tree2 = tree2->TH;

			for (grmr2 = grmr2->T; grmr2; grmr2 = grmr2->T) {
				if (!IsList(grmr2->H) || tree2->H != grmr2->HH) continue;
				if (tree2->H != str_braket_1 && tree2->H != str_braket_0
						&& tree2->H != str_repeat_1 && tree2->H != str_repeat_0) break;
				for (AGmNode *grmr3 = grmr2->HT; grmr3; grmr3 = grmr3->T) {
					if (IsList(grmr3->H) && tree2->H == grmr3->HH) break;
				}
			}
			ASSERT(grmr2);
			grmr2 = grmr2->H;

#ifdef DEBUG_PTREE
DumpList("{}? grmr2 = ", grmr2, 10, 10);
DumpList("{}? tree2 = ", tree2, 10, 10);
#endif
			int err = ParseTreeCorrect(grmr2, tree2);
			if (err) return err;

		} else if (tree2->H == str_repeat_0 || tree2->H == str_repeat_1) {
			ASSERT(grmr2->H == tree2->H);
			// tree2 = ({}*, (...), ...) | ({}+, (...), ...)
			// grmr2 = ({}*, (...), ...) | ({}+, (...), ...)
			
			for (tree2 = tree2->T; tree2; tree2 = tree2->T) {
				AGmNode *tree3 = tree2->H;
				AGmNode *grmr3;

				for (grmr3 = grmr2->T; grmr3; grmr3 = grmr3->T) {
					if (!IsList(grmr3->H) || tree3->H != grmr3->HH) continue;
					if (tree3->H != str_braket_1 && tree3->H != str_braket_0
							&& tree3->H != str_repeat_1 && tree3->H != str_repeat_0) break;
					for (AGmNode *grmr4 = grmr3->HT; grmr4; grmr4 = grmr4->T) {
						if (IsList(grmr4->H) && tree3->H == grmr4->HH) break;
					}
				}
				ASSERT(grmr3);
				grmr3 = grmr3->H;

#ifdef DEBUG_PTREE
DumpList("{}*, {}+ grmr3 = ", grmr3, 10, 10);
DumpList("{}*, {}+ tree3 = ", tree3, 10, 10);
#endif
				int err = ParseTreeCorrect(grmr3, tree3);
				if (err) return err;
			}

		} else {
			// tree2 = (xxx, (...), ...)
			// grmr2 = (xxx, ((...), ...)) | ({}, (...), ...)
			
#ifdef DEBUG_PTREE
DumpList("xxx grmr2 = ", grmr2, 10, 10);
DumpList("xxx tree2 = ", tree2, 10, 10);
#endif
			if (grmr2->H == str_braket_1) {
				for (grmr2 = grmr2->T; grmr2; grmr2 = grmr2->T) {
					if (IsList(grmr2->H) && tree2->H == grmr2->HH) {
						grmr2 = grmr2->H; break;
					}
				}
				ASSERT(grmr2);
#ifdef DEBUG_PTREE
DumpList("{} grmr2 = ", grmr2, 10, 10);
DumpList("{} tree2 = ", tree2, 10, 10);
#endif
			}
			
			int err = ParseTreeCorrect(grmr2, tree2);
			if (err) return err;
		}
	}
#ifdef DEBUG_PTREE
art_sprintf_1(msg, 64, "Leave depth = %d\n", ptree_depth--);
ADvWindow::Print(msg);
#endif
	return 0;
}

AGmNode* AGmProLisp::GetSubParseTree(AGmNode* id, AGmNode* grmr, AGmNode* value, int call_count, int para) {
	AGmNode *tree = NULL;
	if (id) {
		AGmNode *sw_id = grmr->H;
		if (IsString(sw_id)) {
			if (sw_id != str_braket_1) {
				tree = InvAttrGrmr2(value, sw_id, call_count);
				if (error) return NULL;
			} else {
				for (AGmNode *ls = grmr->T; ls; ls = ls->T) {
					ASSERT(IsString(ls->HH));
					if (para && dst_node2(attr_grmr_units, ls->HH) == NULL && Compare(value, ls->H) == 0) {
						tree = value; break;
					}
					tree = InvAttrGrmr2(value, ls->HH, call_count);
					if (error) return NULL;
					if (tree) break;
				}
			}
		} else {
			Error("syntex id is not string."); return NULL;
		}
	} else {
		if (IsString(value)) {
			tree = String(value->a.s.v->Str());
		} else {
			tree = value;
		}
	}

	return tree;
}

int AGmProLisp::InvAttrGrmrSyntaxDepth() {
	return inv_attr_grmr_syntax_depth;
}

AGmNode* AGmProLisp::ToListFromListExp(AGmNode* n) {
	if (n == NULL || !IsList(n) || !IsString(n->H)) return n;
	if (n->H == null && n->T == NULL) return NULL;
	if (n->H == const_s) return n->TH;
	if (n->H != ls_1) return n;
	AGmNode *n2 = Cons(ToListFromListExp(n->TH));
	for (AGmNode *t = n->TTHT; t; t = t->T) {
		n2 = Nconc(n2, Cons(ToListFromListExp(t->HTH)));
	}
	return n2;
}
AGmNode* AGmProLisp::ToListFromListExp2(AGmNode* n) {
	if (n == NULL || !IsList(n) || !IsString(n->H)) return n;
	if (n->H == null && n->T == NULL) return NULL;
	if (n->H == const_i || n->H == const_s) return n->TH;
	if (n->H != ls_1) return undefined;
	AGmNode *n1 = ToListFromListExp2(n->TH);
	if (n1 == undefined) return n1;
	AGmNode *n2 = Cons(n1);
	for (AGmNode *t = n->TTHT; t; t = t->T) {
		n1 = ToListFromListExp2(t->HTH);
		if (n1 == undefined) { RefUnref(n2); return n1; }
		n2 = Nconc(n2, Cons(n1));
	}
	return n2;
}

void AGmProLisp::GenerateGrammarCallback(AGmNode* unit) {
	if (unit->H == str_repeat_0 || unit->H == str_repeat_1 || unit->H == str_braket_0) {
		Push(comp_grmr_val_stack, unit);
		return;
	}
	attr_grmr_depth->a.i.v++; Push(syntax_id_stack, unit->H);
if (AGmProlog::debug_syn) {
DumpList("<<<¡¡¡ depth = ", attr_grmr_depth);
DumpList("<<<¡¡¡ BEGIN id = ", unit->H);
DumpList("<<<¡¡¡ ptree = ", unit, 20, 20);
}
}
void AGmProLisp::ComposeGrammarCallback(AGmNode* unit, int regex) {
	if (regex) {
		Push(comp_grmr_val_stack, unit); return;
	}
	if (unit->H == str_repeat_0 || unit->H == str_repeat_1 || unit->H == str_braket_0) {
		AGmNode *value = NULL;
		for (; comp_grmr_val_stack == NULL || comp_grmr_val_stack->H != unit;
				Pop(comp_grmr_val_stack)) {
			if (comp_grmr_val_stack == NULL) {
				Error("compose grammer stack empty(1).");
				RefUnref(value); return;
			}
			value = Cons(comp_grmr_val_stack->H, value);		
		}
		value = Cons(unit->H, value);
		Pop(comp_grmr_val_stack);
		Push(comp_grmr_val_stack, value);
		return;
	}
	AGmNode *attr_grmr_unit = dst_node2(attr_grmr_units, unit->H);
	if (attr_grmr_unit == NULL) {
		AGmNode *ls;
		AGmNode *value = NULL;
		for (ls = unit->T; ls; ls = ls->T) {
			if (comp_grmr_val_stack == NULL) {
				Error("compose grammer stack empty(2).");
				RefUnref(value); return;
			}
			value = Cons(comp_grmr_val_stack->H, value);		
			Pop(comp_grmr_val_stack);
		}
		value = Cons(unit->H, value);
		Push(comp_grmr_val_stack, value);
		attr_grmr_depth->a.i.v--; Pop(syntax_id_stack);
		return;
	}

	syntax_id_val = attr_grmr_unit->TH;
	ASSERT(syntax_id_val == unit->H);
	grammar_val = dst_node2(syntax_unit_ids, syntax_id_val);
	parse_tree_val = unit;

if (AGmProlog::debug_syn) {
DumpList(">>>ššš syntax_id = ", syntax_id_val);
DumpList(">>>ššš prolog->Clear(); ", NULL);
}
	prolog->Clear();

int save_debug_pred = AGmProlog::debug_pred;
int save_debug_dump = AGmProlog::debug_dump;
int save_debug_syn = AGmProlog::debug_syn;
int save_debug_syn2 = AGmProlog::debug_syn2;
int save_debug_syn3 = AGmProlog::debug_syn3;
if (AGmProlog::debug_syntax_id || AGmProlog::debug_syntax_id2 || AGmProlog::debug_syntax_id3) {
	if ((AGmProlog::debug_syntax_id && syntax_id_val->a.s.v->Compare(AGmProlog::debug_syntax_id->a.s.v) == 0)
			|| (AGmProlog::debug_syntax_id2 && syntax_id_val->a.s.v->Compare(AGmProlog::debug_syntax_id2->a.s.v) == 0)
			|| (AGmProlog::debug_syntax_id3 && syntax_id_val->a.s.v->Compare(AGmProlog::debug_syntax_id3->a.s.v) == 0)) {
		AGmProlog::debug_pred = 1;
		AGmProlog::debug_syn = 1;
		if (AGmProlog::debug_syntax_id2 && syntax_id_val->a.s.v->Compare(AGmProlog::debug_syntax_id2->a.s.v) == 0) {
			AGmProlog::debug_syn2 = 1;
			AGmProlog::debug_syn3 = 1;
		} else {
			AGmProlog::debug_syn2 = 0;
			AGmProlog::debug_syn3 = 0;
		}
		if (AGmProlog::debug_syntax_id3 && syntax_id_val->a.s.v->Compare(AGmProlog::debug_syntax_id3->a.s.v) == 0) {
			AGmProlog::debug_dump = 1;
		} else {
			AGmProlog::debug_dump = 0;
		}
	} else {
		AGmProlog::debug_pred = 0;
		AGmProlog::debug_dump = 0;
		AGmProlog::debug_syn = 0;
		AGmProlog::debug_syn2 = 0;
		AGmProlog::debug_syn3 = 0;
	}
}

	ag_unit = unit;
	AGmNode *n = attr_grmr_unit->TTTH;
	unset_loc(syn_ret_str);
	if (n->H == ag_prog) {
		AGmNode *err;
		AGmNode *pm = dst_node2(pred_manips, syntax_id_val);
		if (pm == NULL) {
			err = prolog->ToInternal(n);
			if (err) { Error(err->a.s.v->Str()); RefUnref(err); return; }
			set_dst_node(pred_manips, syntax_id_val, pm = prolog->PredManip());
		}
		pm = GraphCopy(pm, 1);
		prolog->PredManip(pm);
if (AGmProlog::debug_syn) {
DumpList(">>>ššš syntax_id = ", syntax_id_val);
DumpList(">>>ššš prolog->Execute(); Begin: ", NULL);
}
		err = prolog->Execute();
if (AGmProlog::debug_syn) {
DumpList(">>>ššš syntax_id = ", syntax_id_val);
DumpList(">>>ššš prolog->Execute(); End: err = ", err);
}
		if (IsString(err)) {
			if (error == NULL) Error(err->a.s.v->Str()); RefUnref(err);
		} else if (IsInteger(err)) {
			if (error == NULL) Error("no match rule.");
		}
	} else {
		unset_loc(syn_1); set_loc(syn_1, NULL);
		unset_loc(syn_2); set_loc(syn_2, NULL);
		unset_loc(syn_3); set_loc(syn_3, NULL);
		unset_loc(syn_4); set_loc(syn_4, NULL);
		unset_loc(syn_5); set_loc(syn_5, NULL);
		unset_loc(syn_6); set_loc(syn_6, NULL);
		unset_loc(syn_7); set_loc(syn_7, NULL);
		unset_loc(syn_8); set_loc(syn_8, NULL);
		unset_loc(syn_9); set_loc(syn_9, NULL);
		int ok = 0;
		RefUnref(eval_stat(unit->H, n, ok));
	}
	if (error) return;
	ag_unit = NULL;

	AGmNode *value = get_loc(syn_ret_str);
	AGmNode *ls;
	for (ls = unit->T; ls; ls = ls->T) {
		if (comp_grmr_val_stack == NULL) {
			Error("compose grammer stack empty(3).");
			RefUnref(value); return;
		}
		Pop(comp_grmr_val_stack);
	}
	Push(comp_grmr_val_stack, value);
if (AGmProlog::debug_syn) {
DumpList(">>>¡¡¡ depth = ", attr_grmr_depth);
DumpList(">>>¡¡¡ id = ", syntax_id_val);
#ifdef DUMP_ADDR
DumpGraph(">>>¡¡¡ value = ", value, 50, NULL, 1);
#else
DumpGraph(">>>¡¡¡ value = ", value, 50);
#endif
}
	attr_grmr_depth->a.i.v--; Pop(syntax_id_stack);

AGmProlog::debug_pred = save_debug_pred;
AGmProlog::debug_dump = save_debug_dump;
AGmProlog::debug_syn = save_debug_syn;
AGmProlog::debug_syn2 = save_debug_syn2;
AGmProlog::debug_syn3 = save_debug_syn3;
}

AGmListCB::AGmListCB() {}
AGmListCB::~AGmListCB() {}
void AGmListCB::GenerateGrammarCallback(AGmNode*) {}
void AGmListCB::ComposeGrammarCallback(AGmNode*, int) {}
AGmNode* AGmListCB::Error() { return NULL; }

AGmAttrGrmrCB::AGmAttrGrmrCB(AGmProLisp* cb) {
	this->cb = cb;
}
AGmAttrGrmrCB::~AGmAttrGrmrCB() {
}
void AGmAttrGrmrCB::GenerateGrammarCallback(AGmNode* ls) {
	cb->GenerateGrammarCallback(ls);
}
void AGmAttrGrmrCB::ComposeGrammarCallback(AGmNode* ls, int regex) {
	cb->ComposeGrammarCallback(ls, regex);
}
AGmNode* AGmAttrGrmrCB::Error() {
	return cb->Error();
}

AGmListItr::AGmListItr(AGmNode* tree, AGmListCB* cb) {
	root = tree; stack = NULL; this->cb = cb; attr_grmr = 0;
}
AGmListItr::~AGmListItr() {
	Unref(stack);
}
void AGmListItr::GCmark() {
	_GCmark(stack);
}

AGmNode* AGmListItr::Next() {
	if (root) {
		for (AGmNode *ls = root; ls && IsList(ls); ls = ls->H) {
			Push(stack, List(ls, ls->T));
			if (attr_grmr && cb) {
				cb->GenerateGrammarCallback(ls);
				if (cb->Error()) return NULL;
			}
		}
		root = NULL;
	}
	for (;;) {
		if (stack == NULL) return NULL;
		AGmNode *ls = stack->HH;
		AGmNode *curr = stack->HTH;
		if (curr == NULL || !IsList(curr)) {
			if (attr_grmr && cb) {
				cb->ComposeGrammarCallback(ls);
				if (cb->Error()) return NULL;
			}
			Pop(stack);
			return ls;
		}
		Set(stack->HTH, curr->T);
		if (IsString(curr->H)) {
			if (attr_grmr && cb) {
				cb->ComposeGrammarCallback(curr->H, 1);
				if (cb->Error()) return NULL;
			}
		}
		for (ls = curr->H; ls && IsList(ls); ls = ls->H) {
			Push(stack, List(ls, ls->T));
			if (attr_grmr && cb) {
				cb->GenerateGrammarCallback(ls);
				if (cb->Error()) return NULL;
			}
		}
	}
}
AGmNode* AGmListItr::Next(const char* s) {
	for (;;) {
		AGmNode *ls = Next();
		if (ls == NULL) return NULL;
		if (IsList(ls) && IsString(ls->H) && ls->H->a.s.v->Compare(s) == 0) return ls;
	}
}
AGmNode* AGmListItr::Next(AGmNode* id) {
	for (;;) {
		AGmNode *ls = Next();
		if (ls == NULL) return NULL;
		AGmNode *ls2 = ls;
		for (; ls2 && IsList(ls2); ls2 = ls2->T) {
			if (ls2->H && IsList(ls2->H) && IsString(ls2->HH) && ls2->HH == id && ls2->HT == NULL) return ls2;
		}
	}
}

