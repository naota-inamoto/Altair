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
#include <gdm/vmkey.h>
#include <gdm/debug.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/basefunc.h>
#include <interp/prog_if.h>
#include <dev/shell.h>
#include <cui/cui.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/body.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

// ---------- process in error case ----------
#define FAIL_E_STR(s)	{ AGmString::CreateString(_e_str, s); return 1; }
#define FAIL			return 1;

// ==================== input ====================
int AUiBodyShell::line_scanf(AUiAG* a) {
	if (a->IsEnd()) {
		FAIL_E_STR(AL_S175);
	} else if (a->MatchString("{")) {
		if (!a->IsEnd())
			FAIL_E_STR(AL_S033);
		AGmNode **prog = a->Tail();
		a->Code_i_n(AIP_EvalBlock, (AGmNode*)NULL);
		Push(ctrl_stack, Cons(*prog));
		Push(loc_base, NULL);
		a->Ctrl(AUI_CTRL_BLOCK);	
	} else if (a->MatchIdent("if")) {
		if (!a->MatchString("("))
			FAIL_E_STR(AL_S016);
		if (exp_scanf(a)) FAIL;
		if (!a->MatchString(")"))
			FAIL_E_STR(AL_S017);
		if (!a->MatchString("{"))
			FAIL_E_STR(AL_S018);
		if (!a->IsEnd())
			FAIL_E_STR(AL_S015);
		AGmNode **prog = a->Tail();
		a->Code_i_n_n(AIP_EvalBranch, (AGmNode*)NULL, (AGmNode*)NULL);
		a->CtrlTail(&(*prog)->a.i.t);
		Push(ctrl_stack, Cons(*prog));
		Push(loc_base, NULL);
		a->Ctrl(AUI_CTRL_BRANCH);	
	} else if (a->MatchIdent("loop")) {
		if (!a->MatchString("{"))
			FAIL_E_STR(AL_S019);
		if (!a->IsEnd())
			FAIL_E_STR(AL_S020);
		AGmNode **prog = a->Tail();
		a->Code_i_n(AIP_EvalLoop, (AGmNode*)NULL);
		Push(ctrl_stack, Cons(*prog));
		Push(loc_base, NULL);
		a->Ctrl(AUI_CTRL_LOOP); 
	} else if (a->MatchIdent("try")) {
		if (!a->MatchString("{"))
			FAIL_E_STR(AL_S400);
		if (!a->IsEnd())
			FAIL_E_STR(AL_S401);
		AGmNode **prog = a->Tail();
		a->Code_i_n_n(AIP_EvalTry, (AGmNode*)NULL, (AGmNode*)NULL);
		a->CtrlTail(&(*prog)->a.i.t);
		Push(ctrl_stack, Cons(*prog));
		Push(loc_base, NULL);
		a->Ctrl(AUI_CTRL_TRY);
	} else if (a->MatchString("}")) {
		if (ctrl_stack == NULL)
			FAIL_E_STR(AL_S029);				
		if (ctrl_stack->HHI == AIP_EvalBlock) {
			if (!a->IsEnd())
				FAIL_E_STR(AL_S032);
			AGmNode **prog = &ctrl_stack->HH;
			a->Tail(&(*prog)->a.i.t->T);
			Pop(ctrl_stack);
			Pop(loc_base);
			a->Ctrl(AUI_CTRL_BLOCK2);	
		} else if (ctrl_stack->HHI == AIP_EvalBranch) {
			if (a->MatchIdent("else")) {
				if (!a->MatchString("{"))
					FAIL_E_STR(AL_S013);
				if (!a->IsEnd())
					FAIL_E_STR(AL_S014);
				AGmNode **prog = &ctrl_stack->HH;
				a->Tail(&(*prog)->a.i.t->TH);
				a->Ctrl(AUI_CTRL_BRANCH2);	
			} else {
				if (!a->IsEnd())
					FAIL_E_STR(AL_S031);
				AGmNode **prog = &ctrl_stack->HH;
				a->Tail(&(*prog)->a.i.t->TT);
				Pop(ctrl_stack);
				Pop(loc_base);
				a->Ctrl(AUI_CTRL_BRANCH3);	
			}
		} else if (ctrl_stack->HHI == AIP_EvalLoop) {
			if (!a->IsEnd())
				FAIL_E_STR(AL_S031);
			AGmNode **prog = &ctrl_stack->HH;
			a->Tail(&(*prog)->a.i.t->T);
			Pop(ctrl_stack);
			Pop(loc_base);
			a->Ctrl(AUI_CTRL_LOOP2);	
		} else if (ctrl_stack->HHI == AIP_EvalTry) {
			if (a->MatchIdent("catch")) {
				if (!a->MatchString("("))
					FAIL_E_STR(AL_S402);
				if (!a->MatchIdent("AlException"))
					FAIL_E_STR(AL_S403);
				if (!a->MatchIdent("e"))
					FAIL_E_STR(AL_S404);
				if (!a->MatchString(")"))
					FAIL_E_STR(AL_S405);
				if (!a->MatchString("{"))
					FAIL_E_STR(AL_S406);
				AGmNode **prog = &ctrl_stack->HH;
				a->Tail(&(*prog)->a.i.t->TH);
				a->Ctrl(AUI_CTRL_TRY2); 
			} else {
				if (!a->IsEnd())
					FAIL_E_STR(AL_S407);
				AGmNode **prog = &ctrl_stack->HH;
				a->Tail(&(*prog)->a.i.t->TT);
				Pop(ctrl_stack);
				Pop(loc_base);
				a->Ctrl(AUI_CTRL_TRY3); 
			}
		} else {
			FAIL_E_STR(AL_S030);
		}
	} else if (a->MatchIdent("break")) {
		if (!a->MatchString(";"))
			FAIL_E_STR(AL_S009);
		if (!a->IsEnd())
			FAIL_E_STR(AL_S010);
		a->Code_i(AIP_EvalBreak);
	} else if (a->MatchIdent("continue")) {
		if (!a->MatchString(";"))
			FAIL_E_STR(AL_S011);
		if (!a->IsEnd())
			FAIL_E_STR(AL_S012);
		a->Code_i(AIP_EvalCont);
	} else if (a->MatchIdent("return")) {
		AGmNode *type = _member->TH;
		if (type->I != AUI_TYPE_VOID) {
			if (exp_scanf(a))
				FAIL_E_STR(AL_S026);
		}
		if (!a->MatchString(";"))
			FAIL_E_STR(AL_S025);
		if (!a->IsEnd())
			FAIL_E_STR(AL_S027);
		a->Code_i(AIP_EvalRet);
	} else if (a->MatchIdent("throw")) {
		if (exp_scanf(a))
			FAIL_E_STR(AL_S408);
		if (!a->MatchString(";"))
			FAIL_E_STR(AL_S409);
		if (!a->IsEnd())
			FAIL_E_STR(AL_S410);
		a->Code_i(AIP_EvalThrow);
	} else if (a->MatchIdent("var")) {
		if (loc_base == NULL)
			FAIL_E_STR(AL_S130);
		AGmNode *type, *name;
		AGmString *s;
		if (a->ReadIdent(s))
			FAIL_E_STR(AL_S131);
		type = AUiClassShell::NameToType(_root_class, s);
		if (type == NULL) {
			char err[256];
			art_sprintf_1(err, 256, AL_S036, s->Str());
			delete s;
			FAIL_E_STR(err);
		}
		if (type->I == AUI_TYPE_VOID) {
			delete s;
			FAIL_E_STR(AL_S050);
		}
		delete s;
		AGmNode *ls = NULL;
		for (;;) {
			if (a->ReadIdent(s)) {
				Unref(ls); RefUnref(type);
				FAIL_E_STR(AL_S132);
			}
			if (AUiProgramMgr::IsReserveWord(s)) {
				delete s;
				Unref(ls); RefUnref(type);
				FAIL_E_STR(AL_S204);
			}
			name = UniqueStr(s);
			delete s;
			if (ls == NULL) {
				ls = Ref(Cons(name));
			} else {
				AGmNode *l;
				for (l = ls; l->T; l = l->T) ;
				Set(l->T, Cons(name));
			}
			if (!a->MatchString(",")) break;
		}
		if (!a->MatchString(";")) {
			Unref(ls); RefUnref(type);
			FAIL_E_STR(AL_S129);
		}
		AGmNode *list = Cons(type, ls);
		Unref(ls);
		a->Code_i_n(AIP_EvalLocVarDcl, list);
	} else if (a->MatchString("//")) {
		AGmString *s;
		if (a->ReadLine(s)) {
			FAIL_E_STR(AL_S084);
		}
		a->Code_i_n(AIP_EvalComment, String(s));

	} else if (a->MatchIdent("para")) {
		AUiAG *a2 = new AUiAG(a);
		if (exp_scanf(a2)) {
			delete a2;
			FAIL_E_STR(AL_S022);
		}
		if (!a2->MatchString(";")) {
			delete a2;
			FAIL_E_STR(AL_S023);
		}
		if (!a2->IsEnd()) {
			delete a2;
			FAIL_E_STR(AL_S024);
		}
		AGmNode *head = a2->Head(), *n = NULL;
		AGpProgIF *pif = new AGpProgIF;
		pif->Prog(&head);
		while (pif->More() && pif->Prog() != a2->Tail()) {
			n = *pif->Prog();
			pif->Next();
		}
		delete pif;
		ASSERT(n);
		ASSERT(n->type == AGM_NODE_INTEGER);
		if (n->a.i.v == AIP_EvalClassFunc) {
			n->a.i.v = AIP_EvalParaClassFunc;
		} else if (n->a.i.v == AIP_EvalObjFunc) {
			n->a.i.v = AIP_EvalParaObjFunc;
		} else {
			delete a2;
			FAIL_E_STR(AL_S022);
		}
		Set(*a->Tail(), a2->Head()); a->Tail(a2->Tail());
		a->Code_i(AIP_EvalSemicolon);
		delete a2;
	} else {
		if (exp_scanf(a)) FAIL;
		if (!a->MatchString(";"))
			FAIL_E_STR(AL_S160);
		a->Code_i(AIP_EvalSemicolon);
	}
	return 0;
}

/* call by reference value 'a' is attribute data of attribute grammer */
// generate grammer data	(whether allow 'void' type)
// combining grammer data	((type-id, class/functype), LHS-attr, LHS-assignment-pos)
int AUiBodyShell::exp_scanf(AUiAG* a) {
	return cond_scanf(a);
}

int AUiBodyShell::cond_scanf(AUiAG* a) {
	if (oror_scanf(a)) FAIL;
	if (a->MatchString("?")) {
		AUiAG *a2 = new AUiAG(a);
		if (cond_scanf(a2)) { delete a2; FAIL; }
		if (!a->MatchString(":")) {
			AGmString::CreateString(_e_str, AL_S008);
			delete a2; FAIL;
		}
		AUiAG *a3 = new AUiAG(a);
		if (cond_scanf(a3)) { delete a2; delete a3; FAIL; }
		a->Code_i_n_n(AIP_EvalCond, a2->Head(), a3->Head());
		delete a2; delete a3;
	}
	return 0;
}

int AUiBodyShell::oror_scanf(AUiAG* a) {
	if (andand_scanf(a)) FAIL;
	if (a->MatchString("||")) {
		AUiAG *a2 = new AUiAG(a);
		if (oror_scanf(a2)) { delete a2; FAIL; }
		a->Code_i_n_n(AIP_EvalOrOr, (AGmNode*)NULL, a2->Head());
		delete a2;
	}
	return 0;
}

int AUiBodyShell::andand_scanf(AUiAG* a) {
	if (comp_scanf(a)) FAIL;
	if (a->MatchString("&&")) {
		AUiAG *a2 = new AUiAG(a);
		if (andand_scanf(a2)) { delete a2; FAIL; }
		a->Code_i_n_n(AIP_EvalAndAnd, a2->Head(), (AGmNode*)NULL);
		delete a2;
	}
	return 0;
}

int AUiBodyShell::comp_scanf(AUiAG* a) {
	if (assign_scanf(a)) FAIL;
	for (;;) {
		int op = 0;
		if (a->MatchString("=="))		 { op = 1;
		} else if (a->MatchString("!=")) { op = 2;
		} else if (a->MatchString("<=")) { op = 3;
		} else if (a->MatchString("<"))  { op = 4;
		} else if (a->MatchString(">=")) { op = 5;
		} else if (a->MatchString(">"))  { op = 6;
		} else break;
		a->Code_i(AIP_EvalPush);
		if (assign_scanf(a)) FAIL;
		switch (op) {
		case 1: a->Code_i(AIP_EvalEQ);	break;
		case 2: a->Code_i(AIP_EvalNE);	break;
		case 3: a->Code_i(AIP_EvalLE);	break;
		case 4: a->Code_i(AIP_EvalLT);	break;
		case 5: a->Code_i(AIP_EvalGE);	break;
		case 6: a->Code_i(AIP_EvalGT);	break;
		}
		break;
	}
	return 0;
}
int AUiBodyShell::assign_scanf(AUiAG* a) {
	if (add_scanf(a)) FAIL;
	if (a->MatchString("=")) {
		if (a->IsChar('=')) { a->Unput(2); return 0; }
		long lhs = a->Lhs();
		AGmNode **prog, *data = NULL;
		AGmNode *prog1 = NULL, *prog2 = NULL; // for property set with arguments (OLE autometion)
		switch (lhs) {
		case AUI_LHS_LOC:
		case AUI_LHS_ARG:
		case AUI_LHS_THIS_VAR:
		case AUI_LHS_OBJ_VAR:
			prog = a->LhsPos();
			data = Ref((*prog)->a.i.t->H);
			Unref(*prog); *prog = NULL;
			a->Tail(prog);
			break;
		case AUI_LHS_VARIANT:
			prog = a->LhsPos2();
			prog1 = *prog;
			prog2 = *a->LhsPos();
			Ref(*prog); *prog = NULL;
			a->Tail(prog);
			break;
		case AUI_LHS_CLASS_VAR:
			prog = a->LhsPos();
			data = Ref( dst_node2(*prog, NmMember()) );
			Unref(*prog); *prog = NULL;
			a->Tail(prog);
			break;
		case AUI_LHS_LIST_HEAD:
		case AUI_LHS_LIST_TAIL:
		case AUI_LHS_MATRIX:
		case AUI_LHS_ARRAY:
			prog = a->LhsPos();
			data = NULL;
			Unref(*prog); *prog = NULL;
			a->Tail(prog);
			break;
		default:
			FAIL_E_STR(AL_S164);
		}
		switch (lhs) {
		case AUI_LHS_OBJ_VAR:
		case AUI_LHS_LIST_HEAD:
		case AUI_LHS_LIST_TAIL:
		case AUI_LHS_MATRIX:
		case AUI_LHS_ARRAY:
		case AUI_LHS_VARIANT:
			a->Code_i(AIP_EvalPush);
			break;
		default:
			break;
		}
		if (assign_scanf(a)) { Unref(data); FAIL; }
		switch (lhs) {
		case AUI_LHS_LOC:
			a->Code_i_n(AIP_EvalSetLoc, data);
			break;
		case AUI_LHS_ARG:
			a->Code_i_n(AIP_EvalSetArg, data);
			break;
		case AUI_LHS_THIS_VAR:
			a->Code_i_n(AIP_EvalSetThisVar, data);
			break;
		case AUI_LHS_CLASS_VAR:
			AGmNode **prog; AGmArc *arc;
			prog = a->Tail();
			a->Code_i(AIP_EvalSetClassVar);
			arc = CreateArc(*prog, data, NmMember());
			arc->incl_assoc |= AGM_ARC_ASSOC;
			break;
		case AUI_LHS_OBJ_VAR:
			a->Code_i_n(AIP_EvalSetObjVar, data);
			break;
		case AUI_LHS_LIST_HEAD:
			a->Code_i(AIP_EvalSetHead);
			break;
		case AUI_LHS_LIST_TAIL:
			a->Code_i(AIP_EvalSetTail);
			break;
		case AUI_LHS_MATRIX:
			a->Code_i(AIP_EvalSetMatrix);
			break;
		case AUI_LHS_ARRAY:
			a->Code_i(AIP_EvalSetArray);
			break;
		case AUI_LHS_VARIANT:
			prog = a->Tail();
			*prog = prog1;
			prog2->a.i.v = AIP_EvalOcxSetProp;
			prog2->a.i.t->T->a.i.v += 1;
			prog = &prog2->a.i.t->T->a.i.t;
			ASSERT(*prog == NULL);
			a->Tail(prog);
			break;
		}
		Unref(data);
	}
	return 0;
}
int AUiBodyShell::add_scanf(AUiAG* a) {
	if (mul_scanf(a)) FAIL;
	for (;;) {
		int op = 0;
		if (a->MatchString("+")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			op = 1;
		} else if (a->MatchString("-")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			op = 2;
		} else break;
		a->Code_i(AIP_EvalPush);
		if (mul_scanf(a)) FAIL;
		switch (op) {
		case 1: a->Code_i(AIP_EvalAdd); break;
		case 2: a->Code_i(AIP_EvalSub); break;
		}
	}
	return 0;
}
int AUiBodyShell::mul_scanf(AUiAG* a) {
	if (or_scanf(a)) FAIL;
	for (;;) {
		int op = 0;
		if (a->MatchString("*")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			op = 1;
		} else if (a->MatchString("/")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			op = 2;
		} else if (a->MatchString("%")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			op = 3;
		} else break;
		a->Code_i(AIP_EvalPush);
		if (or_scanf(a)) FAIL;
		switch (op) {
		case 1: a->Code_i(AIP_EvalMul); break;
		case 2: a->Code_i(AIP_EvalDiv); break;
		case 3: a->Code_i(AIP_EvalMod); break;
		}
	}
	return 0;
}
int AUiBodyShell::or_scanf(AUiAG* a) {
	if (xor_scanf(a)) FAIL;
	for (;;) {
		if (a->MatchString("|")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			if (a->IsChar('|')) { a->Unput(2); return 0; }
		} else break;
		a->Code_i(AIP_EvalPush);
		if (xor_scanf(a)) FAIL;
		a->Code_i(AIP_EvalOr);
	}
	return 0;
}
int AUiBodyShell::xor_scanf(AUiAG* a) {
	if (and_scanf(a)) FAIL;
	for (;;) {
		if (a->MatchString("^")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
		} else break;
		a->Code_i(AIP_EvalPush);
		if (and_scanf(a)) FAIL;
		a->Code_i(AIP_EvalXor);
	}
	return 0;
}
int AUiBodyShell::and_scanf(AUiAG* a) {
	if (shift_scanf(a)) FAIL;
	for (;;) {
		if (a->MatchString("&")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			if (a->IsChar('&')) { a->Unput(2); return 0; }
		} else break;
		a->Code_i(AIP_EvalPush);
		if (shift_scanf(a)) FAIL;
		a->Code_i(AIP_EvalAnd);
	}
	return 0;
}
int AUiBodyShell::shift_scanf(AUiAG* a) {
	if (unary_scanf(a)) FAIL;
	for (;;) {
		int op = 0;
		if (a->MatchString("<<")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			op = 1;
		} else if (a->MatchString(">>")) {
			if (a->IsChar('=')) { a->Unput(2); return 0; }
			op = 2;
		} else break;
		a->Code_i(AIP_EvalPush);
		if (unary_scanf(a)) FAIL;
		switch (op) {
		case 1: a->Code_i(AIP_EvalSL);	break;
		case 2: a->Code_i(AIP_EvalSR);	break;
		}
		return 0;
	}
	return 0;
}
int AUiBodyShell::unary_scanf(AUiAG* a) {
	// constant
	AGmNode *v = NULL;
	if (a->MatchString("0x")) {
		if (a->ReadHexa(v) == 0)	{
			a->Code_i_n(AIP_EvalConstHexa, v);
			a->RetType(AUI_TYPE_INTEGER);
			return 0;
		}
	} else if (a->ReadReal(v) == 0) {
		a->Code_i_n(AIP_EvalConstReal, v);
		a->RetType(AUI_TYPE_REAL);
		return 0;
	} else if (a->ReadInteger(v) == 0) {
		a->Code_i_n(AIP_EvalConstInt, v);
		a->RetType(AUI_TYPE_INTEGER);
		return 0;
	} else if (a->ReadQChar(v) == 0) {
		a->Code_i_n(AIP_EvalConstQChar, v);
		a->RetType(AUI_TYPE_INTEGER);
		return 0;
	} else if (a->ReadQString(v) == 0) {
		a->Code_i_n(AIP_EvalConstString, v);
		a->RetType(AUI_TYPE_STRING);
		return 0;
	}

#if	_WINDOWS && _64BIT
	long long pos = a->CurrentPoint();
#else
	long pos = a->CurrentPoint();
#endif
	int op = 0;
	AGmString *s; AGmNode *type = NULL;
	if (a->MatchString("-")) { op = 1;
	} else if (a->MatchString("~")) { op = 2;
	} else if (a->MatchString("(")) { op = 3;
		if (!a->ReadIdent(s)) {
			type = AUiClassShell::NameToType(_root_class, s);
			delete s;
			if (type) {
				if (a->MatchString(")")) {
				} else {
					FAIL_E_STR(AL_S066);
				}
			} else {
				a->Back(pos);
				op = 0;
			}
		} else {
			a->Back(pos);
			op = 0;
		}
	}
	if (obj_scanf(a)) FAIL;
	if (a->MatchString("[")) {
		a->Code_i(AIP_EvalPush);
		if (exp_scanf(a)) FAIL;
		if (a->MatchString("][")) { // Matrix
			a->Code_i(AIP_EvalPush);
			if (exp_scanf(a)) FAIL;
			a->RetType(AUI_TYPE_REAL, NULL, AUI_LHS_MATRIX, a->Tail());
			a->Code_i(AIP_EvalMatrix);
			if (!a->MatchString("]")) FAIL;
		} else { // Ptr Array
			a->RetType(AUI_TYPE_LIST, NULL, AUI_LHS_ARRAY, a->Tail());
			a->Code_i(AIP_EvalArray);
			if (!a->MatchString("]")) FAIL;
		}
	}
	switch (op) {
	case 1: a->Code_i(AIP_EvalChgSign); a->Lhs(0);	break;
	case 2: a->Code_i(AIP_EvalInverse); a->Lhs(0);	break;
	case 3: a->Code_i_n(AIP_EvalCast, type); a->RetType(type);	break;
	default: break;
	}
	return 0;
}
int AUiBodyShell::obj_scanf(AUiAG* a) {
	if (prim_scanf(a)) FAIL;
	for (;;) {
		if (a->MatchString(".")) {
		} else break;
		if (mem_scanf(a)) FAIL;
	}
	return 0;
}
int AUiBodyShell::mem_scanf(AUiAG* a) {
	int protect;
	char err[256];
	AGmNode *obj_type = a->Type();
	if (obj_type->I == AUI_TYPE_LIST) {
		if (a->MatchIdent("head")) {
			a->RetType(obj_type, AUI_LHS_LIST_HEAD, a->Tail());
			a->Code_i(AIP_EvalHead);
			return 0;
		}
		if (a->MatchIdent("tail")) {
			a->RetType(obj_type, AUI_LHS_LIST_TAIL, a->Tail());
			a->Code_i(AIP_EvalTail);
			return 0;
		}
		FAIL_E_STR(AL_S004);
	}
	AGmString *s; AGmNode *n;
	AGmNode *arg_dcl2 = NULL;
	if (obj_type->I == AUI_TYPE_VARIANT) {
		// OLE automation property/method
		if (!a->ReadIdent(s)) {
			n = Ref(String(s));
			a->LhsPos2(a->Tail());
			if (!a->MatchString("(")) {
			} else {
				if (a->MatchString(")")) {
				} else {
					if (arg_scanf(a, arg_dcl2)) { Unref(n); FAIL; }
					if (!a->MatchString(")")) {
						Unref(n, arg_dcl2);
						FAIL_E_STR(AL_S149);
					}
				}
			}
			a->RetType(AUI_TYPE_VARIANT, NULL, AUI_LHS_VARIANT, a->Tail());
			a->Code_i_n(AIP_EvalOcxInvoke, UniqueStr(n));
			a->Code_i(ListLength(arg_dcl2)/2);
			Unref(n, arg_dcl2);
			return 0;
		}
		FAIL_E_STR(AL_S002);
	}
	if (obj_type->I != AUI_TYPE_CLASS)
		FAIL_E_STR(AL_S003);
	if (!a->ReadIdent(s)) {
		n = Ref(String(s));
		AGmNode *ocn = dst_node2(obj_type, NmType());
		AGmNode *var_dcl = find_obj_var(ocn, ocn, _class, n, protect);
		if (var_dcl) { // object variable
			if (protect) {
				art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
				Unref(n);
				FAIL_E_STR(err);
			}
			a->RetType(var_dcl->TH, AUI_LHS_OBJ_VAR, a->Tail());
			a->Code_i_n(AIP_EvalObjVar, UniqueStr(n));
			Unref(n);
			return 0;
		}
		if (a->MatchString("(")) {
			if (a->MatchString(")")) {
			} else {
				if (arg_scanf(a, arg_dcl2)) { Unref(n); FAIL; }
				if (!a->MatchString(")")) {
					Unref(n, arg_dcl2);
					FAIL_E_STR(AL_S149);
				}
			}
			AGmNode *ocn = dst_node2(obj_type, NmType());
			AGmNode *func_dcl = find_obj_func(ocn, ocn, _class, n, arg_dcl2, protect);
			if (func_dcl) { // object function
				if (protect) {
					art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
					Unref(n, arg_dcl2);
					FAIL_E_STR(err);
				}
				Unref(n, arg_dcl2);
				AGmNode **prog = a->Tail();
				a->Code_i(AIP_EvalObjFunc);
				AGmArc *arc = CreateArc(*prog, func_dcl, NmVtbl());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				arc = CreateArc(*prog, ocn, NmClass());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				arc = CreateArc(*prog, _class, NmThis());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				a->RetType(func_dcl->TH);
				return 0;
			}
			art_sprintf_1(err, 256, AL_S038, n->a.s.v->Str());
			Unref(n, arg_dcl2); 
			FAIL_E_STR(err);
		}
		art_sprintf_1(err, 256, AL_S034, n->a.s.v->Str());
		Unref(n);	
		FAIL_E_STR(err);
	}
	if (a->MatchString("*")) {
		AUiAG *a2 = new AUiAG(a);
		if (exp_scanf(a2)) { delete a2; FAIL; }
		if (!a->MatchString("(")) {
			delete a2;
			FAIL_E_STR(AL_S150);
		}
		AGmNode *arg_dcl2 = NULL;
		if (a->MatchString(")")) {
		} else {
			if (arg_scanf(a, arg_dcl2)) { delete a2; FAIL; }
			if (!a->MatchString(")")) {
				Unref(arg_dcl2); delete a2;
				FAIL_E_STR(AL_S149);
			}
		}
		AGmNode *func_type = a2->Type();
		if (IsObjectFuncType(func_type, arg_dcl2)) { // object function by function pointer
			a->Code_i(AIP_EvalPush);
				Set(*a->Tail(), a2->Head()); a->Tail(a2->Tail());
			AGmNode **prog = a->Tail();
			a->Code_i(AIP_EvalObjFuncP);
			AGmArc *arc = CreateArc(*prog, dst_node2(func_type, NmType()), NmType());
			arc->incl_assoc |= AGM_ARC_ASSOC;
			a->RetType(FuncRetType(func_type));
			Unref(arg_dcl2);
			delete a2;
			return 0;
		}
		delete a2;
	}
	FAIL_E_STR(AL_S002);
}
int AUiBodyShell::prim_scanf(AUiAG* a) {
	int protect;
	char err[256];
	AGmString *s; AGmNode *n; AGmNode *func_type;
	if (a->MatchIdent("new")) {
		if (a->ReadIdent(s)) {
			FAIL_E_STR(AL_S021);
		}
		n = AUiClassShell::NameToClass(_root_class, s);
		if (n == NULL) {
			art_sprintf_1(err, 256, AL_S035, s->Str());
			AGmString::CreateString(_e_str, err);
			delete s; return 1;
		}
		delete s;
		AGmNode **prog = a->Tail();
		a->Code_i(AIP_EvalNew);
		AGmArc *arc = CreateArc(*prog, n, NmClass());
		arc->incl_assoc |= AGM_ARC_ASSOC;
		a->RetType(AUI_TYPE_CLASS, n);
		return 0;	
	} else if (a->MatchIdent("this")) {
		if (_member->HI & AIP_STATIC) {
			FAIL_E_STR(AL_S206);
		}
		a->Code_i(AIP_EvalThis);
		a->RetType(AUI_TYPE_CLASS, _class);
		return 0;
	} else if (a->MatchIdent("runtime")) {
		if (a->ReadIdent(s)) {
			FAIL_E_STR(AL_S028);
		}
		n = AUiClassShell::NameToClass(_root_class, s);
		if (n == NULL) {
			art_sprintf_1(err, 256, AL_S035, s->Str());
			AGmString::CreateString(_e_str, err);
			delete s; return 1;
		}
		delete s;
		AGmNode **prog = a->Tail();
		a->Code_i(AIP_EvalRuntime);
		AGmArc *arc = CreateArc(*prog, n, NmClass());
		arc->incl_assoc |= AGM_ARC_ASSOC;
		a->RetType(AUI_TYPE_CLASS, n);
		return 0;	
	} else if (a->MatchString("(")) {
		if (exp_scanf(a)) FAIL;
		if (!a->MatchString(")"))
			FAIL_E_STR(AL_S140);
		return 0;
	}
	if (!a->ReadIdent(s)) {
		n = Ref(String(s));
		AGmNode *type;
		type = find_loc_var(loc_base, n);
		if (type) { // local variable
			a->RetType(type, AUI_LHS_LOC, a->Tail());
			a->Code_i_n(AIP_EvalLoc, UniqueStr(n));
			Unref(n);
			return 0;
		}
		AGmNode *arg_dcl = _member->TTTH;
		type = find_arg_var(arg_dcl, n);
		if (type) { // argument
			a->RetType(type, AUI_LHS_ARG, a->Tail());
			a->Code_i_n(AIP_EvalArg, UniqueStr(n));
			Unref(n);
			return 0;
		}
		AGmNode *var_dcl;
		var_dcl = find_class_var(_class, _class, n, protect);
		if (var_dcl) { // class variable
			if (protect) {
				art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
				Unref(n);
				FAIL_E_STR(err);
			}
			Unref(n);
			a->RetType(var_dcl->TH, AUI_LHS_CLASS_VAR, a->Tail());
			AGmNode **prog = a->Tail();
			a->Code_i(AIP_EvalClassVar);
			AGmArc *arc = CreateArc(*prog, var_dcl, NmMember());
			arc->incl_assoc |= AGM_ARC_ASSOC;
			return 0;
		}
		var_dcl = find_obj_var(_class, _class, _class, n, protect);
		if (var_dcl) { // object variable
			if (_member->HI & AIP_STATIC) {
				Unref(n);
				FAIL_E_STR(AL_S208);
			}
			if (protect) {
				art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
				Unref(n);
				FAIL_E_STR(err);
			}
			a->RetType(var_dcl->TH, AUI_LHS_THIS_VAR, a->Tail());
			a->Code_i_n(AIP_EvalThisVar, UniqueStr(n));
			Unref(n);
			return 0;
		}
		AGmNode *arg_dcl2 = NULL;
		if (a->MatchString("(")) {
			if (a->MatchString(")")) {
			} else {
				if (arg_scanf(a, arg_dcl2)) { Unref(n, arg_dcl2); FAIL; }
				if (!a->MatchString(")")) {
					Unref(n, arg_dcl2);
					FAIL_E_STR(AL_S148);
				}
			}
			AGmNode *func_dcl;
			func_dcl = find_class_func(_class, _class, n, arg_dcl2, protect);
			if (func_dcl) { // class function
				if (protect) {
					art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
					Unref(n, arg_dcl2);
					FAIL_E_STR(err);
				}
				Unref(n, arg_dcl2);
				AGmNode **prog = a->Tail();
				a->Code_i(AIP_EvalClassFunc);
				AGmArc *arc = CreateArc(*prog, func_dcl, NmMember());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				a->RetType(func_dcl->TH);
				return 0;
			}
			func_dcl = find_obj_func(_class, _class, _class, n, arg_dcl2, protect);
			if (func_dcl) { // object function
				if (protect) {
					art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
					Unref(n, arg_dcl2);
					FAIL_E_STR(err);
				}
				Unref(n, arg_dcl2);
				if (_member->HI & AIP_STATIC) FAIL_E_STR(AL_S207);
				AGmNode **prog = a->Tail();
				a->Code_i(AIP_EvalThisFunc);
				AGmArc *arc = CreateArc(*prog, func_dcl, NmVtbl());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				arc = CreateArc(*prog, _class, NmClass());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				arc = CreateArc(*prog, _class, NmThis());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				a->RetType(func_dcl->TH);
				return 0;
			}
			long index = AIpProcess::SearchSysFunc(n->a.s.v->Str());
			if (index >= 0) { // system function
				AGmNode *l; int narg;
				for (l = arg_dcl2, narg = 0; l; l = l->TT, narg++)	;
				if (narg != AIpProcess::SysFuncNumArg(index)) {
					art_sprintf_1(err, 256, AL_S087, n->a.s.v->Str());
					Unref(n, arg_dcl2);
					FAIL_E_STR(err);
				}
				Unref(n, arg_dcl2);
				a->Code_i_i(AIP_EvalSysFunc, index);
				a->RetType(AUI_TYPE_CLASS, _root_class);
				return 0;
			}
			art_sprintf_1(err, 256, AL_S142, n->a.s.v->Str());
			Unref(n, arg_dcl2);
			FAIL_E_STR(err);
		}
		AGmNode *cn = AUiClassShell::NameToClass(_root_class, n);
		if (cn) {
			Unref(n);
			if (!a->MatchString("::"))
				FAIL_E_STR(AL_S081);
			if (a->ReadIdent(s))
				FAIL_E_STR(AL_S005);
			n = Ref(String(s));
			AGmNode *var_dcl;
			var_dcl = find_class_var(cn, _class, n, protect);
			if (var_dcl) { // class variable with class specifier
				if (protect) {
					art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
					Unref(n);
					FAIL_E_STR(err);
				}
				Unref(n);
				a->RetType(var_dcl->TH, AUI_LHS_CLASS_VAR, a->Tail());
				AGmNode **prog = a->Tail();
				a->Code_i(AIP_EvalClassVar);
				AGmArc *arc = CreateArc(*prog, var_dcl, NmMember());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				return 0;
			}
			AGmNode *arg_dcl2 = NULL;
			if (a->MatchString("(")) {
				if (a->MatchString(")")) {
				} else {
					if (arg_scanf(a, arg_dcl2)) { Unref(n); FAIL; }
					if (!a->MatchString(")")) {
						Unref(n, arg_dcl2);
						FAIL_E_STR(AL_S149);
					}
				}
				AGmNode *func_dcl;
				func_dcl = find_class_func(cn, _class, n, arg_dcl2, protect, 0, 1);
				if (func_dcl) { // class function with class specifier
					if (protect) {
						art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
						Unref(n, arg_dcl2);
						FAIL_E_STR(err);
					}
					Unref(n, arg_dcl2);
					AGmNode **prog = a->Tail();
					a->Code_i(AIP_EvalClassFunc);
					AGmArc *arc = CreateArc(*prog, func_dcl, NmMember());
					arc->incl_assoc |= AGM_ARC_ASSOC;
					a->RetType(func_dcl->TH);
					return 0;
				}
				func_dcl = find_obj_func(cn, cn, _class, n, arg_dcl2, protect);
				if (func_dcl) { // object function with class specifier
					if (protect) {
						art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
						Unref(n, arg_dcl2);
						FAIL_E_STR(err);
					}
					Unref(n, arg_dcl2);
					if (_member->HI & AIP_STATIC) FAIL_E_STR(AL_S208);
					AGmNode **prog = a->Tail();
					a->Code_i(AIP_EvalObjFunc2);
					AGmArc *arc = CreateArc(*prog, func_dcl, NmVtbl());
					arc->incl_assoc |= AGM_ARC_ASSOC;
					arc = CreateArc(*prog, cn, NmClass());
					arc->incl_assoc |= AGM_ARC_ASSOC;
					arc = CreateArc(*prog, _class, NmThis());
					arc->incl_assoc |= AGM_ARC_ASSOC;
					a->RetType(func_dcl->TH);
					return 0;
				}
				art_sprintf_1(err, 256, AL_S142, n->a.s.v->Str());
				Unref(n, arg_dcl2);
				FAIL_E_STR(err);
			}
			AGmNode *func_dcl;
			func_dcl = find_class_func(cn, _class, n, NULL, protect, 1, 1);
			if (func_dcl) { // function pointer of class function
				if (protect) {
					art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
					Unref(n);
					FAIL_E_STR(err);
				}
				Unref(n);
				AGmNode **prog = a->Tail();
				a->Code_i(AIP_EvalConstFunc);
				AGmArc *arc = CreateArc(*prog, func_dcl, NmMember());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				a->RetType(AUI_TYPE_CLASS_FUNC);
				return 0;
			}
			func_dcl = find_obj_func(cn, cn, _class, n, NULL, protect, 1, 1);
			if (func_dcl) { // function pointer of object function
				if (protect) {
					art_sprintf_1(err, 256, AL_S209, n->a.s.v->Str());
					Unref(n);
					FAIL_E_STR(err);
				}
				Unref(n);
				AGmNode **prog = a->Tail();
				a->Code_i(AIP_EvalConstVtbl);
				AGmArc *arc = CreateArc(*prog, func_dcl, NmMember());
				arc->incl_assoc |= AGM_ARC_ASSOC;
				a->RetType(AUI_TYPE_OBJ_FUNC);
				return 0;
			}
			art_sprintf_2(err, 256, AL_S039, cn->H->a.s.v->Str(), n->a.s.v->Str());
			Unref(n);
			FAIL_E_STR(err);	
		} else {
			if (n->a.s.v->Compare("null") == 0) {
				Unref(n);
				a->Code_i_n(AIP_EvalConstNull, (AGmNode*)NULL);
				a->RetType(AUI_TYPE_LIST);
				return 0;
			}
			art_sprintf_1(err, 256, AL_S037, n->a.s.v->Str());
			Unref(n);
			FAIL_E_STR(err);
		}
	}
	if (a->MatchString("*")) {
		AUiAG *a2 = new AUiAG(a);
		if (exp_scanf(a2)) { delete a2; FAIL; }
		if (!a->MatchString("("))
			FAIL_E_STR(AL_S150);
		AGmNode *arg_dcl2;
		if (arg_scanf(a, arg_dcl2)) { delete a2; FAIL; }
		if (!a->MatchString(")")) { Unref(arg_dcl2); delete a2; FAIL; }
		func_type = a2->Type();
		if (IsClassFuncType(func_type, arg_dcl2)) { // class function of function pointer
			Set(*a->Tail(), a2->Head()); a->Tail(a2->Tail());
			AGmNode **prog = a->Tail(); 							
			a->Code_i(AIP_EvalClassFuncP);
			AGmArc *arc = CreateArc(*prog, dst_node2(func_type, NmType()), NmType());
			arc->incl_assoc |= AGM_ARC_ASSOC;
			a->RetType(FuncRetType(func_type));
			Unref(arg_dcl2);
			delete a2;
			return 0;
		}
		func_type = a2->Type();
		if (IsObjectFuncType(func_type, arg_dcl2)) { // object function of funtion pointer
			Set(*a->Tail(), a2->Head()); a->Tail(a2->Tail());
			AGmNode **prog = a->Tail(); 							
			a->Code_i(AIP_EvalThisFuncP);
			AGmArc *arc = CreateArc(*prog, dst_node2(func_type, NmType()), NmType());
			arc->incl_assoc |= AGM_ARC_ASSOC;
			a->RetType(FuncRetType(func_type));
			Unref(arg_dcl2);
			delete a2;
			return 0;
		}
		delete a2;
		Unref(arg_dcl2);
		FAIL_E_STR(AL_S176);
	}

	// constant
	AGmNode *v = NULL;
	if (a->MatchString("0x")) {
		if (a->ReadHexa(v) == 0)	{
			a->Code_i_n(AIP_EvalConstHexa, v);
			a->RetType(AUI_TYPE_INTEGER);
			return 0;
		}
	} else if (a->ReadReal(v) == 0) {
		a->Code_i_n(AIP_EvalConstReal, v);
		a->RetType(AUI_TYPE_REAL);
		return 0;
	} else if (a->ReadInteger(v) == 0) {
		a->Code_i_n(AIP_EvalConstInt, v);
		a->RetType(AUI_TYPE_INTEGER);
		return 0;
	} else if (a->ReadQChar(v) == 0) {
		a->Code_i_n(AIP_EvalConstQChar, v);
		a->RetType(AUI_TYPE_INTEGER);
		return 0;
	} else if (a->ReadQString(v) == 0) {
		a->Code_i_n(AIP_EvalConstString, v);
		a->RetType(AUI_TYPE_STRING);
		return 0;
	}

	FAIL_E_STR(AL_S178);
}

int AUiBodyShell::arg_scanf(AUiAG *a, AGmNode *&arg_dcl2) {
	AUiAG *a2 = new AUiAG(a);
	a2->Code_i(AIP_EvalPush);
	if (exp_scanf(a2)) { delete a2; FAIL; }
	if (a->MatchString(",")) {
		if (arg_scanf(a, arg_dcl2)) { delete a2; FAIL; }
	}
	Set(*a->Tail(), a2->Head()); a->Tail(a2->Tail());
	Push(arg_dcl2, a2->Type());
	Push(arg_dcl2, NULL);
	delete a2;
	return 0;
}

// ---------- attribute data of attribute grammer ----------
AUiAG::AUiAG(AGmFile *file) {
	f = file; head = NULL; p = &head;
	type = NULL; ctrl = 0;
}
AUiAG::AUiAG(AUiAG *a) {
	f = a->f; head = NULL; p = &head;
	type = NULL; ctrl = 0;
}
AUiAG::~AUiAG() {
	Unref(head, type);
}
int AUiAG::RetType(AGmNode *type, long lhs, AGmNode **lhs_pos) {
	Unref(this->type);
	this->type = Ref(type);
	this->lhs = lhs;
	this->lhs_pos = lhs_pos;
	return 0;
}
int AUiAG::RetType(long type_id, AGmNode *class_id, long lhs, AGmNode **lhs_pos) {
	Unref(this->type);
	this->type = Ref(Integer(type_id));
	if (class_id) {
		AGmArc *a = CreateArc(this->type, class_id, NmType());
		a->incl_assoc |= AGM_ARC_ASSOC;
	}
	this->lhs = lhs;
	this->lhs_pos = lhs_pos;
	return 0;
}

int AUiAG::Code_i(long i) {
	AGmNode *l1 = Integer(i);
	Set(*p, l1); p = &l1->a.i.t;
	return 0;
}
int AUiAG::Code_i_i(long i, long i2) {
	AGmNode *l1 = Integer(i2);
	AGmNode *l2 = Integer(i, l1);
	Set(*p, l2); p = &l1->a.i.t;
	return 0;
}
int AUiAG::Code_i_n(long i, AGmNode *n) {
	AGmNode *l1 = Cons(n);
	AGmNode *l2 = Integer(i, l1);
	Set(*p, l2); p = &l1->a.l.t;
	return 0;
}
int AUiAG::Code_i_n_n(long i, AGmNode *n, AGmNode *n2) {
	AGmNode *l1 = Cons(n2);
	AGmNode *l2 = Cons(n, l1);
	AGmNode *l3 = Integer(i, l2);
	Set(*p, l3); p = &l1->a.l.t;
	return 0;
}

int AUiAG::ReadIdent(AGmString*& s) {
	return f->ReadIdent(s);
}
int AUiAG::ReadString(AGmString*& s) {
	return f->ReadString(s);
}
int AUiAG::ReadLine(AGmString*& s) {
	f->ReadSpace();
	return f->ReadLine(s);
}
int AUiAG::ReadMatrix(AGmMatrix*& m) {
	return f->ReadMatrix(m);
}

int AUiAG::ReadInteger(AGmNode*& v) {
	return f->ReadInteger(v);
}
int AUiAG::ReadHexa(AGmNode*& v) {
	return f->ReadHexa(v);
}
int AUiAG::ReadReal(AGmNode*& v) {
	return f->ReadReal(v);
}
int AUiAG::ReadQChar(AGmNode*& v) {
	return f->ReadQChar(v); 
}
int AUiAG::ReadQString(AGmNode*& v) {
	return f->ReadQString(v);
}

int AUiAG::MatchIdent(const char* s) {
	return f->MatchIdent(s);
}
int AUiAG::MatchString(const char* s) {
	return f->MatchString(s);
}
int AUiAG::IsChar(char c) {
	return f->IsChar(c);
}
int AUiAG::IsEnd() {
	return f->IsEnd();
}
void AUiAG::Unput(long d) {
	f->Unput(d);
}
#if _WINDOWS && _64BIT
long long AUiAG::CurrentPoint() {
	return f->CurrentPoint();
}
void AUiAG::Back(long long pos) {
	f->Back(pos);
}
#else
long AUiAG::CurrentPoint() {
	return f->CurrentPoint();
}
void AUiAG::Back(long pos) {
	f->Back(pos);
}
#endif
