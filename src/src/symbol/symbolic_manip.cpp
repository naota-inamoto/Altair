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
#include <interp/interp.h>
#include <interp/prcsmgr.h>

#include <symbol/parser_generator.h>
#include <symbol/pro_lisp.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

int AIpProcess::EvalSymbolManip() {
	AGmNode *cmd, *arg1, *arg2, *arg3, *arg4;
	GetArg(cmd, arg1, arg2, arg3, arg4);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("SymbolManip()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;

	if (strcmp(s, "compile_syntax") == 0) {
		if (IsFile(arg1)) {
			AGmParserGenerator *pg = new AGmParserGenerator();
			// opt
			if (arg4) {
				// debug
				AGmNode *debug = dst_node(arg4, "debug");
				if (IsInteger(debug)) pg->DebugCompile((int)debug->a.i.v);
			}
			val = Ref(pg->Compile(arg1->a.f.v));
			delete pg;
		} else {
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("SymbolManip() compile_syntax");
		}

	} else if (strcmp(s, "parse") == 0) {
		if (IsFile(arg1) && IsString(arg3)) {
			AGmParserGenerator *pg = new AGmParserGenerator(arg2);
			// opt
			if (arg4) {
				// debug
				AGmNode *debug = dst_node(arg4, "debug");
				if (debug && IsList(debug)) {
					pg->DebugUnit10(debug->H);
					if (debug->T && IsList(debug->T)) {
						pg->DebugUnit20(debug->TH);
						if (debug->TT && IsList(debug->TT)) {
							pg->DebugUnit30(debug->TTH);
						}
					}
				}
				// full
				AGmNode *full = dst_node(arg4, "full");
				if (full) pg->ParseFull(1);
			}
			AIpProcessMgr *pm = AIpProcessMgr::Instance();
			pm->AddLocalLoopProcess(this);
			val = Ref(pg->Parse(arg1->a.f.v, arg3->a.s.v->Str()));
			pm->RemoveLocalLoopProcess(this);
			delete pg;
		} else {
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("SymbolManip() parse");
		}
	} else if (strcmp(s, "parse_cut_depth") == 0) {
		if (IsInteger(arg1)) {
			AGmParserGenerator::ParseCutDepth((int)arg1->a.i.v);
		} else {
			val = Ref(Integer(AGmParserGenerator::ParseCutDepth()));
		}

	} else if (strcmp(s, "generate") == 0) {
		if (IsFile(arg1)) {
			AGmParserGenerator *pg = new AGmParserGenerator(arg2);
			// opt
			if (arg4) {
				// spacing
				pg->set_space(0);
			}
			val = Ref(pg->Generate(arg1->a.f.v, arg3));
			delete pg;
		} else {
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("SymbolManip() generate");
		}

	} else if (strcmp(s, "out_syntax_unit") == 0) {
		if (IsFile(arg1)) {
			AGmParserGenerator *pg = new AGmParserGenerator(arg2);
			// opt
			int out_skelton = 0;
			if (arg4) {
				// out_skelton
				AGmNode *out_skel = dst_node(arg4, "skelton");
				if (IsInteger(out_skel)) out_skelton = (int)out_skel->a.i.v;
			}
			val = Ref(pg->OutSyntaxUnit(arg1->a.f.v, out_skelton));
			delete pg;
		} else {
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("SymbolManip() out_syntax_unit");
		}

	} else if (strcmp(s, "plisp_load") == 0) {
		AGmProLisp *pl = NULL;
		if (arg1 == NULL) {
			pl = new AGmProLisp();
		} else {
			pl = new AGmProLisp(arg1);
		}
		val = Ref(pl->Load(arg2));
		delete pl;
	} else if (strcmp(s, "plisp_call") == 0) {
		AGmProLisp *pl = NULL;
		if (arg1 == NULL) {
			pl = new AGmProLisp();
		} else {
			pl = new AGmProLisp(arg1);
		}
		if (!IsString(arg2)) {
			delete pl;
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("SymbolManip() plisp_call: 3rd argument must be function name.");
		}
		// opt
		if (arg4) {
			// debug
			AGmNode *debug = dst_node(arg4, "debug");
			if (IsInteger(debug)) {
				pl->DebugLevel((int)debug->a.i.v);
			} else if (debug && IsList(debug) && IsInteger(debug->H) && debug->T && debug->TH) {
				pl->DebugLevel((int)debug->H->a.i.v);
				pl->DebugFunc(debug->TH);
			}
		}
		AGmNode *ret = Ref(pl->Call(arg2->a.s.v->Str(), arg3));
		Unref(val); val = ret;
		if (pl->Error()) {
			int flag = interp_error(pl->Error()->a.s.v->Str());
			delete pl;
			Unref(cmd, arg1, arg2, arg3, arg4);
			return flag;
		}
		delete pl;

	} else if (strcmp(s, "set_var") == 0) {
		if (IsString(arg2)) {
			if (arg1 == NULL) {
				val = Ref(List(Cons(NULL), NULL, Cons(NULL), Cons(NULL)));
			} else if (arg1 && IsList(arg1->H) && arg1->T && arg1->TT
					&& IsList(arg1->TT) && arg1->TTH && arg1->TTT && IsList(arg1->TTT) && arg1->TTTH) {
				val = Ref(arg1);
			}
			if (val) {
				AGmNode *name = Ref(UniqueStr(arg2));
				AGmNode *ls;
				for (ls = val->TH; ls; ls = ls->T) if (ls->HH == name) break;
				if (ls == NULL) {
					Push(val->TH, List(name, arg3));
				} else {
					Set(ls->HTH, arg3);
				}
				Unref(name);
			}
		}

	} else if (strcmp(s, "get_var") == 0) {
		if (arg1 && IsList(arg1->H) && arg1->T && arg1->TT
				&& IsList(arg1->TT) && arg1->TTH && arg1->TTT && IsList(arg1->TTT) && arg1->TTTH
				&& IsString(arg2)) {
			AGmNode *name = Ref(UniqueStr(arg2));
			AGmNode *ls;
			for (ls = arg1->TH; ls; ls = ls->T) if (ls->HH == name) break;
			if (ls) val = Ref(ls->HTH);
			Unref(name);
		}

	} else if (strcmp(s, "prolog_exec") == 0) {
		AGmProLisp *p = NULL;
		if (arg1 == NULL) {
			p = new AGmProLisp();
		} else {
			p = new AGmProLisp(arg1);
		}
		AGmProlog *pr = new AGmProlog(p);
		pr->PredManip(p->PredManip());
		// opt
		AGmProlog::debug_pred = 0;
		AGmProlog::debug_dump = 0;
		AGmProlog::debug_syn = 0;
		AGmProlog::debug_syn2 = 0;
		AGmProlog::debug_syn3 = 0;
		if (arg4) {
			// debug_dump
			if (dst_node(arg4, "debug_pred")) AGmProlog::debug_pred = 1;
			if (dst_node(arg4, "debug_dump")) AGmProlog::debug_dump = 1;
			if (dst_node(arg4, "debug_syn")) AGmProlog::debug_syn = 1;
			if (dst_node(arg4, "debug_syn2")) AGmProlog::debug_syn2 = 1;
			if (dst_node(arg4, "debug_syn3")) AGmProlog::debug_syn3 = 1;
		}
		AGmNode *ret = pr->Execute();
		if (ret) val = Ref(ret);
		delete pr;
		delete p;

	} else if (strcmp(s, "clear_query") == 0) {
		if (arg1 == NULL) {
			val = Ref(List(Cons(NULL), NULL, Cons(NULL), Cons(NULL)));
		} else if (arg1 && IsList(arg1->H) && arg1->T && arg1->TT
				&& IsList(arg1->TT) && arg1->TTH && arg1->TTT && IsList(arg1->TTT) && arg1->TTTH) {
			val = Ref(arg1);
		}
		if (val) {
			Set(val->TTTH, Cons(NULL)); // (prog, GVAR, pred, <manip>)
		}
	} else if (strcmp(s, "clear_pred") == 0) {
		if (arg1 == NULL) {
			val = Ref(List(Cons(NULL), NULL, Cons(NULL), Cons(NULL)));
		} else if (arg1 && IsList(arg1->H) && arg1->T && arg1->TT
				&& IsList(arg1->TT) && arg1->TTH && arg1->TTT && IsList(arg1->TTT) && arg1->TTTH) {
			val = Ref(arg1);
		}
		if (val) {
			Set(val->TTH, Cons(NULL)); // (prog, GVAR, <pred>, manip)
		}
	} else if (strcmp(s, "ls_pred") == 0) {
		if (arg1 && IsList(arg1->H) && arg1->T && arg1->TT
				&& IsList(arg1->TT) && arg1->TTH && arg1->TTT && IsList(arg1->TTT) && arg1->TTTH) {
			AGmString *str = new AGmString(64);
			AGmNode *g = arg1->TTH;
			AGmLink *l;
			for (l = next_outlink(g, NULL); l; l = next_outlink(g, l)) {
				str->Append(l->arc->attr->a.s.v->Str());
				str->Append("\n");
			}
			val = Ref(String(str));
		}

	} else if (strcmp(s, "plisp_max_call") == 0) {
		if (IsInteger(arg1)) {
			AGmProLisp::MaxCallCount((int)arg1->a.i.v);
		} else {
			val = Ref(Integer(AGmProLisp::MaxCallCount()));
		}

	} else if (strcmp(s, "prolog_max_call") == 0) {
		if (IsInteger(arg1)) {
			AGmProlog::MaxCallCount((int)arg1->a.i.v);
		} else {
			val = Ref(Integer(AGmProlog::MaxCallCount()));
		}

	} else if (strcmp(s, "inv_attr_grmr_syntax_depth") == 0) {
		val = Ref(Integer(AGmProLisp::InvAttrGrmrSyntaxDepth()));

	} else {
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("SymbolManip()");
	}
	Unref(cmd, arg1, arg2, arg3, arg4);
	return 0;
}
