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
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <interp/basefunc.h>
#include <interp/prog_if.h>
#include <dev/advcapp.h>
#include <dev/advwnd.h>
#include <dev/shell.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/body.h>
#include <altair/strres.h>

int AIpProcess::EvalGraphProg() {
	AGmNode *cmd, *arg1, *arg2, *arg3, *arg4;
	GetArg(cmd, arg1, arg2, arg3, arg4);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("GraphProg()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	int protect;
	if (strcmp(s, "runtime_class") == 0) {
		if (IsString(arg1)) {
			val = Ref(AUiClassShell::NameToClass(RootClass(), arg1));
		}

	} else if (strcmp(s, "regist_class") == 0) {
		int ng = 1;
		if (IsString(arg1) && IsString(arg2) && AUiClassShell::NameToClass(RootClass(), arg2) == NULL) {
			AGmNode *base_class, *sub_class;
			base_class = AUiClassShell::NameToClass(RootClass(), arg1);
			if (base_class) {
				sub_class = Cons(UniqueStr(arg2));
				CreateArc(base_class, sub_class, NmSubClass());
				AUiVtblUpdate = 0;
				ng = 0;
			}
		}
		if (ng) val = Ref(Integer(1));
	
	} else if (strcmp(s, "unregist_class") == 0) {
		if (!IsString(arg1)) {
			val = Ref(Integer(1));
		} else {
			AGmNode *_class = AUiClassShell::NameToClass(RootClass(), arg1);
			if (_class == NULL) {
				val = Ref(Integer(2));
			} else if (arg2 == NULL && Refered(_class, AUiClassShell::ReferedFromOutside)) {
				val = Ref(Integer(3));
			} else if (arg2 == NULL && dst_node2(_class, NmSubClass())) {
				val = Ref(Integer(4));
			} else {
				AGmLink *l = next_inlink(_class, NULL, NmSubClass());
				if (l == NULL) {
					val = Ref(Integer(5));
				} else {
					if (arg2 == NULL) DeleteLoopRef(l->arc->dst);
					DeleteArc(l->arc);
					AUiVtblUpdate = 0;
				}
			}
		}

	} else if (strcmp(s, "regist_var") == 0) {
		int ng = 1;
		if (IsString(arg1) && IsString(arg2) && IsString(arg3)) {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			AGmNode *type = AUiClassShell::NameToType(RootClass(), arg2);
			if (class_node && type && next_outlink(class_node, NULL, arg3) == NULL) {
				long info = AUI_PUBLIC;
				AGmNode *var_dcl = List(Integer(info), type);
				CreateArc(class_node, var_dcl, UniqueStr(arg3));
				ng = 0;

				AGmNode *public_var = dst_node(class_node, UniqueStr("public_var"));
				if (public_var == NULL) {
					long info = AUI_PUBLIC | AUI_FUNC;
					AGmNode *type = AUiClassShell::NameToType(RootClass(), UniqueStr("void"));
					public_var = List(Integer(info), type, NULL, NULL);
					CreateArc(class_node, public_var, UniqueStr("public_var"));
					AGpProgIF *pif = new AGpProgIF;
					pif->Prog(&public_var->TTH);
					pif->Code_i_n(AIP_EvalBlock, (AGmNode*)NULL);
					delete pif;
				}
				AGpProgIF *pif = new AGpProgIF;
				pif->Prog(&public_var->TTH->TH);
				while (pif->More()) pif->Next();
				pif->Code_i_n(AIP_EvalThisVar, UniqueStr(arg3));
				pif->Code_i(AIP_EvalSemicolon);
				delete pif; 
			}
		}
		if (ng) val = Ref(Integer(1));

	} else if (strcmp(s, "unregist_var") == 0) {
		int ng = 1;
		if (IsString(arg1) && IsString(arg2)) {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			AGmLink *member_link;
			if (class_node && (member_link = next_outlink(class_node, NULL, arg2)) != NULL) {
				AGmNode *member_node = member_link->arc->dst;
				long info = (long)member_node->HI;
				if (info == AUI_PUBLIC) {
					DeleteArc(member_link->arc);
					ng = 0;

					AGmNode *public_var = dst_node(class_node, UniqueStr("public_var"));
					if (public_var) {
						AGpProgIF *pif = new AGpProgIF;
						pif->Prog(&public_var->TTH->TH);
						while (pif->More()) {
							AGmNode *name = pif->MemberVar();
							if (name == NULL) { pif->Next(); continue; }
							if (name->a.s.v->Compare(arg2->a.s.v) != 0) { pif->Next(); continue; }
							pif->Code_Del();
							pif->Code_Del();
						}
						delete pif; 
					}
				}
			}
		}
		if (ng) val = Ref(Integer(1));

	} else if (strcmp(s, "add_to_arg_dcl") == 0) {
		if (IsList(arg1) && IsString(arg2) && IsString(arg3)) {
			AGmNode *type = AUiClassShell::NameToType(RootClass(), arg2);
			if (type) {
				if (arg1 == NULL) {
					val = Ref(List(UniqueStr(arg3), type));
				} else {
					val = Ref(Nconc(arg1, List(UniqueStr(arg3), type)));	
				}
			}
		}

	} else if (strcmp(s, "regist_sfunc") == 0) {
		int ng = 1;
		if (IsString(arg1) && IsString(arg2) && IsString(arg3)) {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			AGmNode *type = AUiClassShell::NameToType(RootClass(), arg2);
			if (class_node && type) {
				AGmNode *func_dcl = AUiBodyShell::find_class_func(
						class_node, class_node, arg3, arg4, protect, 0, 1);
				if (func_dcl == NULL) {
					long info = AUI_PUBLIC | AUI_STATIC | AUI_FUNC;
					func_dcl = List(Integer(info), type, NULL, arg4);
					CreateArc(class_node, func_dcl, UniqueStr(arg3));
					ng = 0;

					AGmNode *public_func = dst_node(class_node, UniqueStr("public_func"));
					if (public_func == NULL) {
						long info = AUI_PUBLIC | AUI_FUNC;
						AGmNode *type = AUiClassShell::NameToType(RootClass(), UniqueStr("void"));
						public_func = List(Integer(info), type, NULL, NULL);
						CreateArc(class_node, public_func, UniqueStr("public_func"));
						AGpProgIF *pif = new AGpProgIF;
						pif->Prog(&public_func->TTH);
						pif->Code_i_n(AIP_EvalBlock, (AGmNode*)NULL);
						delete pif;
					}
					AGpProgIF *pif = new AGpProgIF;
					pif->Prog(&public_func->TTH->TH);
					while (pif->More()) pif->Next();
					AGmNode *type_stack = NULL;
					AGmNode *ls;
					for (ls = arg4; ls; ls = ls->TT) {
						Push(type_stack, ls->TH);
					}
					while (type_stack) {
						pif->Code_i(AIP_EvalPush);
						pif->Code_i_n(AIP_EvalConstNull, (AGmNode*)NULL);
						pif->Code_i_n(AIP_EvalCast, type_stack->H);
						Pop(type_stack);
					}
					AGmNode **p = pif->Prog();
					pif->Code_i(AIP_EvalClassFunc);
					AGmArc *arc = CreateArc(*p, func_dcl, NmMember());
					arc->incl_assoc |= AGM_ARC_ASSOC;
					pif->Code_i(AIP_EvalSemicolon);
					delete pif; 
				}	
			}
		}
		if (ng) val = Ref(Integer(1));

	} else if (strcmp(s, "unregist_sfunc") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			val = Ref(Integer(1));
		} else {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			if (class_node == NULL) {
				val = Ref(Integer(2));
			} else {
				AGmNode *func_dcl = AUiBodyShell::find_class_func(
						class_node, class_node, arg3, arg4, protect);
				if (func_dcl == NULL) {
					val = Ref(Integer(3));
				} else if (Refered(func_dcl, AUiMemberShell::ReferedFromOutside)) {
					val = Ref(Integer(4));
				} else {
					AGmLink *l = find_outlink(class_node, func_dcl);
					DeleteArc(l->arc);

					AGmNode *public_func = dst_node(class_node, UniqueStr("public_func"));
					if (public_func) {
						AGpProgIF *pif = new AGpProgIF;
						pif->Prog(&public_func->TTH->TH);
						AGmNode *member_func;
						while (pif->More()) {
							member_func = pif->MemberSFunc();
							if (member_func == NULL) { pif->Next(); continue; }
							if (member_func != func_dcl) { pif->Next(); continue; }
							AGmNode *ls;
							for (ls = func_dcl->TTTH; ls; ls = ls->TT) {
								pif->Code_Del();
								pif->Code_Del();
								pif->Code_Del();
							}
							pif->Code_Del();
							pif->Code_Del();
						}
						delete pif; 
					}
				}
			}
		}

	} else if (strcmp(s, "regist_vfunc") == 0) {
		int ng = 1;
		if (IsString(arg1) && IsString(arg2) && IsString(arg3)) {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			AGmNode *type = AUiClassShell::NameToType(RootClass(), arg2);
			if (class_node && type) {
				AGmNode *func_dcl = AUiBodyShell::find_obj_func(
						class_node, class_node, class_node, arg3, arg4, protect, 0, 1);
				if (func_dcl == NULL) {
					long info = AUI_PUBLIC | AUI_FUNC;
					func_dcl = List(Integer(info), type, NULL, arg4);
					CreateArc(class_node, func_dcl, UniqueStr(arg3));
					AUiVtblUpdate = 0;
					ng = 0;

					AGmNode *public_func = dst_node(class_node, UniqueStr("public_func"));
					if (public_func == NULL) {
						long info = AUI_PUBLIC | AUI_FUNC;
						AGmNode *type = AUiClassShell::NameToType(RootClass(), UniqueStr("void"));
						public_func = List(Integer(info), type, NULL, NULL);
						CreateArc(class_node, public_func, UniqueStr("public_func"));
						AGpProgIF *pif = new AGpProgIF;
						pif->Prog(&public_func->TTH);
						pif->Code_i_n(AIP_EvalBlock, (AGmNode*)NULL);
						delete pif;
					}
					AGpProgIF *pif = new AGpProgIF;
					pif->Prog(&public_func->TTH->TH);
					while (pif->More()) pif->Next();
					AGmNode *type_stack = NULL;
					AGmNode *ls;
					for (ls = arg4; ls; ls = ls->TT) {
						Push(type_stack, ls->TH);
					}
					while (type_stack) {
						pif->Code_i(AIP_EvalPush);
						pif->Code_i_n(AIP_EvalConstNull, (AGmNode*)NULL);
						pif->Code_i_n(AIP_EvalCast, type_stack->H);
						Pop(type_stack);
					}
					AGmNode **p = pif->Prog();
					pif->Code_i(AIP_EvalThisFunc);
					AGmArc *arc = CreateArc(*p, func_dcl, NmVtbl());
					arc->incl_assoc |= AGM_ARC_ASSOC;
					arc = CreateArc(*p, class_node, NmClass());
					arc->incl_assoc |= AGM_ARC_ASSOC;
					arc = CreateArc(*p, class_node, NmThis());
					arc->incl_assoc |= AGM_ARC_ASSOC;
					pif->Code_i(AIP_EvalSemicolon);
					delete pif; 
				}	
			}
		}
		if (ng) val = Ref(Integer(1));

	} else if (strcmp(s, "unregist_vfunc") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			val = Ref(Integer(1));
		} else {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			if (class_node == NULL) {
				val = Ref(Integer(2));
			} else {
				AGmNode *func_dcl = AUiBodyShell::find_obj_func(
						class_node, class_node, class_node, arg3, arg4, protect);
				if (func_dcl == NULL) {
					val = Ref(Integer(3));
				} else if (Refered(func_dcl, AUiMemberShell::ReferedFromOutside)) {
					val = Ref(Integer(4));
				} else {
					AGmLink *l = find_outlink(class_node, func_dcl);
					DeleteArc(l->arc);
					AUiVtblUpdate = 0;

					AGmNode *public_func = dst_node(class_node, UniqueStr("public_func"));
					if (public_func) {
						AGpProgIF *pif = new AGpProgIF;
						pif->Prog(&public_func->TTH->TH);
						AGmNode *member_func;
						while (pif->More()) {
							member_func = pif->MemberSFunc();
							if (member_func == NULL) { pif->Next(); continue; }
							if (member_func != func_dcl) { pif->Next(); continue; }
							AGmNode *ls;
							for (ls = func_dcl->TTTH; ls; ls = ls->TT) {
								pif->Code_Del();
								pif->Code_Del();
								pif->Code_Del();
							}
							pif->Code_Del();
							pif->Code_Del();
						}
						delete pif; 
					}
				}
			}
		}

	} else if (strcmp(s, "var_list") == 0) {
		val = Ref(Cons(NULL));
		AGmNode *class_node, *public_var;
		if (IsString(arg1) && (class_node = AUiClassShell::NameToClass(RootClass(), arg1)) != NULL) {
			for (; class_node; class_node = src_node(class_node, NmSubClass())) {
				if ((public_var = dst_node(class_node, UniqueStr("public_var"))) != NULL
						&& (public_var->HI & AUI_FUNC)) {
					AGpProgIF *pif = new AGpProgIF;
					pif->Prog(&public_var->TTH);
					for (; pif->More(); pif->Next()) {
						AGmNode *name = pif->MemberVar();
						if (name) {
							AGmNode *var_dcl = NULL;
							AGmNode *_class;
							for (_class = class_node;
									_class;
									_class = src_node(_class, NmSubClass())) {
								var_dcl = dst_node(_class, name);
								if (var_dcl) break;
							}
							AGmNode *type = Ref(AUiClassShell::TypeToName(var_dcl->TH));
							ASSERT(type);
							AGmString *s = new AGmString(type->a.s.v->Str());
							Unref(type);
							s->Append(" ");
							s->Append(name);
							AGmNode *n = String(s);
							CreateArc(val, n);
							CreateArc(n, var_dcl);
						}	
					}
					delete pif;
				}
			}
		}

	} else if (strcmp(s, "sfunc_list") == 0) {
		val = Ref(Cons(NULL));
		AGmNode *class_node, *public_sfunc;
		if (IsString(arg1) && (class_node = AUiClassShell::NameToClass(RootClass(), arg1)) != NULL) {
			for (; class_node; class_node = src_node(class_node, NmSubClass())) {
				if ((public_sfunc = dst_node(class_node, UniqueStr("public_func"))) != NULL
						&& (public_sfunc->HI & AUI_FUNC)) {
					AGpProgIF *pif = new AGpProgIF;
					pif->Prog(&public_sfunc->TTH);
					for (; pif->More(); pif->Next()) {
						AGmNode *func_dcl = pif->MemberSFunc();
						if (func_dcl) {
							AGmArc *a = AUiClassShell::ClassOfMember(func_dcl);
							ASSERT(a);
							AGmString *s = new AGmString(512);
							AUiMemberShell::_member_printf1(s, a->src, func_dcl);
							AUiMemberShell::_member_printf2(s, a->src, func_dcl);
							AGmNode *n = String(s);
							CreateArc(val, n);
							CreateArc(n, func_dcl);
						}	
					}
					delete pif;
				}
			}
		}

	} else if (strcmp(s, "vfunc_list") == 0) {
		val = Ref(Cons(NULL));
		AGmNode *class_node, *public_vfunc;
		if (IsString(arg1) && (class_node = AUiClassShell::NameToClass(RootClass(), arg1)) != NULL) {
			for (; class_node; class_node = src_node(class_node, NmSubClass())) {
				if ((public_vfunc = dst_node(class_node, UniqueStr("public_func"))) != NULL
						&& (public_vfunc->HI & AUI_FUNC)) {
					AGpProgIF *pif = new AGpProgIF;
					pif->Prog(&public_vfunc->TTH);
					for (; pif->More(); pif->Next()) {
						AGmNode *func_dcl = pif->MemberVFunc();
						if (func_dcl) {
							AGmArc *a = AUiClassShell::ClassOfMember(func_dcl);
							ASSERT(a);
							AGmString *s = new AGmString(16);
							AUiMemberShell::_member_printf1(s, a->src, func_dcl);
							AUiMemberShell::_member_printf2(s, a->src, func_dcl);
							AGmNode *n = String(s);
							CreateArc(val, n);
							CreateArc(n, func_dcl);
						}	
					}
					delete pif;
				}
			}
		}

	} else if (strcmp(s, "type_to_name") == 0) {
		val = Ref(AUiClassShell::TypeToName(arg1));

	} else if (strcmp(s, "class_name_of") == 0) {
		if (IsObject(arg1)) {
			val = Ref(arg1->a.o.class_name);
		}

	} else if (strcmp(s, "pif_create") == 0) {
		if (IsString(arg1) && IsString(arg3)) {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			if (class_node) {
				AGmNode *func_dcl;
				if ((func_dcl = AUiBodyShell::find_class_func(
						class_node, class_node,
						arg3, arg4, protect)) != NULL) {
					AUiBodyShell *bs = new AUiBodyShell(
						NULL, RootClass(), class_node, func_dcl);
					bs->Reset();
					val = Ref(PtrCell(bs));
				} else if ((func_dcl = AUiBodyShell::find_obj_func(
						class_node, class_node, class_node,
						arg3, arg4, protect)) != NULL) {
					AUiBodyShell *bs = new AUiBodyShell(
						NULL, RootClass(), class_node, func_dcl);
					bs->Reset();
					val = Ref(PtrCell(bs));
				} else {
					Unref(cmd, arg1, arg2, arg3, arg4);
					return interp_error("GraphProg() pif_create");
				}
			}
		}

	} else if (strcmp(s, "pif_destroy") == 0) {
		int ng = 1;
		if (IsPtrCell(arg1)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			arg1->a.p.ptr = NULL;
			if (bs) delete bs;
			ng = 0;
		}
		if (ng) val = Ref(Integer(1));

	} else if (strcmp(s, "pif_clear") == 0) {
		int ng = 1;
		if (IsPtrCell(arg1)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			if (bs) {
				AGmNode *func_dcl = bs->Member();
				Set(func_dcl->TTH, NULL);
				bs->Reset();
				ng = 0;
			}
		}
		if (ng) val = Ref(Integer(1));

	} else if (strcmp(s, "pif_insert") == 0) {
		int ng = 1;
		if (IsPtrCell(arg1) && IsString(arg2)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			if (bs->InsertLine(arg2->a.s.v->Str()) == 0) {
				ng = 0;
			}
		}
		if (ng) val = Ref(Integer(1));

	} else if (strcmp(s, "pif_reset") == 0) {
		if (IsPtrCell(arg1) && IsInteger(arg2)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			if (bs) {
				bs->Reset((long)arg2->a.i.v);
			}
		}

	} else if (strcmp(s, "pif_more") == 0) {
		if (IsPtrCell(arg1)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			if (bs && bs->More()) {
				val = Ref(Integer(1));
			}
		}

	} else if (strcmp(s, "pif_last") == 0) {
		if (IsPtrCell(arg1)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			if (bs->IsLast()) {
				val = Ref(Integer(1));
			}
		}
	} else if (strcmp(s, "pif_next") == 0) {
		if (IsPtrCell(arg1)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			if (bs) bs->Next();
		}

	} else if (strcmp(s, "pif_lno") == 0) {
		Unref(val); val = NULL;
		if (IsPtrCell(arg1)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			if (bs) {
				val = Ref(Integer(bs->Lno()));
			}
		}

	} else if (strcmp(s, "pif_string") == 0) {
		if (IsPtrCell(arg1)) {
			AUiBodyShell *bs = (AUiBodyShell*)arg1->a.p.ptr;
			if (bs) {
				AGmString *s = bs->GetLineString();
				val = Ref(s ? String(s->Str()) : NULL);
			}
		}
	} else if (strcmp(s, "run") == 0) {
		if (IsString(arg1) && IsString(arg3)) {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			if (class_node) {
				AGmNode *func_dcl = AUiBodyShell::find_class_func(
						class_node, class_node, arg3, NULL, protect);
				if (func_dcl) {
					if (AUiVtblUpdate == 0) {
						AUiVtblUpdate = 1;
						ADvBeginWaitCursor();
						AUiMemberShell::CleanVtbl(RootClass());
						AUiMemberShell::UpdateVtbl(RootClass(), RootClass());
						ADvEndWaitCursor();
					}
					AIpProcessMgr *pm = AIpProcessMgr::Instance();
					pm->CreateProcess(RootClass(), NULL, func_dcl);
				} else {
					ADvWindow::Print(AL_S151);
					val = Ref(Integer(3));
				}
			} else {
				ADvWindow::Print(AL_S079);
				val = Ref(Integer(2));
			}
		} else {
			ADvWindow::Print(AL_S080);
			val = Ref(Integer(1));
		}

	} else if (strcmp(s, "unique_str") == 0) {
		if (IsString(arg1)) {
			val = Ref(UniqueStr(arg1));
		}

	} else if (strcmp(s, "run2") == 0) {
		int ng = 1;
		if (IsString(arg1) && IsString(arg3)) {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			if (class_node) {
				AGmNode *func_dcl = AUiBodyShell::find_class_func(
						class_node, class_node, arg3, NULL, protect, 1);
				if (func_dcl) {
					if (AUiVtblUpdate == 0) {
						AUiVtblUpdate = 1;
						ADvBeginWaitCursor();
						AUiMemberShell::CleanVtbl(RootClass());
						AUiMemberShell::UpdateVtbl(RootClass(), RootClass());
						ADvEndWaitCursor();
					}
					ng = 0;
					AIpProcessMgr *pm = AIpProcessMgr::Instance();
					AIpProcess *process = pm->CreateProcess(RootClass(), arg4, func_dcl, NULL);
					process->ReturnProcess(this);
					Unref(cmd, arg1, arg2, arg3, arg4);
					return AIP_PROCESS;
				}
			}
		}
		if (IsObject(arg1) && IsString(arg3)) {
			AGmNode *cls = arg1->a.o.class_id;
			AGmNode *func_dcl = AUiBodyShell::find_obj_func(
				cls, cls, cls, arg3, arg2, protect);
			if (func_dcl) {
				AGmNode *vtbl = dst_node2(func_dcl, NmVtbl());
				AGmNode *func = NULL;
				if (vtbl) func = dst_node2(vtbl, cls);
				if (func == NULL) {
					AUiMemberShell::CleanVtbl(RootClass());
					AUiMemberShell::UpdateVtbl(RootClass(), RootClass());
					vtbl = dst_node2(func_dcl, NmVtbl());
					ASSERT(vtbl);
					func = dst_node2(vtbl, cls);
					ASSERT(func);
				}

				ng = 0;
				AIpProcessMgr *pm = AIpProcessMgr::Instance();
				AIpProcess *process = pm->CreateProcess(RootClass(), arg4, func, arg1);
				process->ReturnProcess(this);
				Unref(cmd, arg1, arg2, arg3, arg4);
				return AIP_PROCESS;
			}
		}
		if (ng) val = Ref(Integer(0x80000000));

	} else if (strcmp(s, "new") == 0) {
		if (IsString(arg1)) {
			AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), arg1);
			if (class_node) {
				val = Ref(Object(class_node->H, class_node));
			}
		}

	} else if (strcmp(s, "vtbl") == 0) {
		if (AUiVtblUpdate == 0) {
			AUiVtblUpdate = 1;
			ADvBeginWaitCursor();
			AUiMemberShell::CleanVtbl(RootClass());
			AUiMemberShell::UpdateVtbl(RootClass(), RootClass());
			ADvEndWaitCursor();
		}

	} else if (strcmp(s, "method_name") == 0) {
		if (func_stack) {
			AGmNode *func = func_stack->H->TTTTTH;
			AGmArc *a = AUiClassShell::ClassOfMember(func);
			val = Ref(a->attr);
		}

	} else if (strcmp(s, "stack_frame") == 0) {
		val = Ref(func_stack);

	} else if (strcmp(s, "arg_dcl") == 0) {
		if (func_stack) {
			AGmNode *func = func_stack->H->TTTTTH;
			val = Ref(func->TTTH);
		}

	} else if (strcmp(s, "arg_list") == 0) {
		if (arg_base) {
			val = Ref(arg_base);
		}

	} else if (strcmp(s, "encode") == 0) {
		if (arg1 && IsList(arg1) && arg2 && IsList(arg2)) {
			AGmNode *arg_dcl = GraphCopy(arg1);
			AGmNode *arg_list = GraphCopy(arg2);
			AGmNode *n;
			for (n = arg_dcl; n; n = n->TT) {
				Set(n->TH, AUiClassShell::TypeToName(n->TH));
			}
			for (n = arg_list; n; n = n->T) {
				Set(n->H->TTH, NULL);
			}
			val = Ref(List(arg_dcl, arg_list));
		}

	} else if (strcmp(s, "decode") == 0) {
		if (arg1 && IsList(arg1)) {
			AGmNode *arg_dcl = GraphCopy(arg1);
			AGmNode *n;
			for (n = arg_dcl; n; n = n->TT) {
				Set(n->TH, AUiClassShell::NameToType(RootClass(), n->TH));
			}
			val = Ref(arg_dcl);
		}

	} else {
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("GraphProg()");
	}
	Unref(cmd, arg1, arg2, arg3, arg4);
	return 0;
}
