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
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/basefunc.h>
#include <dev/shell.h>
#include <cui/class.h>
#include <cui/body.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

/* member variable 'exp' has following information */
// exp->H ... string
// exp->TH ... priority of expression
// exp->TTH ... LHS attribute

// ==================== output ====================
int AUiBodyShell::indent(AGmString*& text) {
	if (text->Length() > 0) return 0;
	for (int i = 0; i < _depth; i++)
		text->Append("\t");
	return 0;
}

int AUiBodyShell::line_printf(AGmString*& text) {
	_prog = prog; ctrl_type = 0; ctrl_tail = NULL;
	if (_create_text) {
		text = new AGmString(512);
	}
	for (;;) {
		if (*prog == NULL) {
			if (ctrl_stack == NULL) {
				_status = 1;
				return 1;
			}
			prog = &ctrl_stack->HH;
			switch ((*prog)->I) {
			case	AIP_EvalTry:
				_depth --;
				if (ctrl_stack->HTI) {
					if (_create_text) {
						indent(text); text->Append("} catch (AlException e) {");
					}
					ctrl_stack->HTI = 0;
					prog = &(*prog)->a.i.t->TH;
					_depth ++;
					Pop(loc_base);
					Push(loc_base, NULL);

					AGmNode *type = AUiClassShell::NameToType(_root_class, "AlException");
					AGmNode *name = UniqueStr("e");
					Push(loc_base->H, List(name, (AGmNode*)NULL, type));

					ctrl_type = AUI_CTRL_TRY2;
				} else {
					if (_create_text) {
						indent(text); text->Append("}");
					}
					Pop(ctrl_stack);
					Pop(loc_base);
					ctrl_type = AUI_CTRL_TRY3;
					prog = &(*prog)->a.i.t->TT;
				}
				break;
			case	AIP_EvalBlock:
				_depth --;
				if (_create_text) {
					indent(text); text->Append("}");
				}
				Pop(ctrl_stack);
				Pop(loc_base);
				ctrl_type = AUI_CTRL_BLOCK2;
				prog = &(*prog)->a.i.t->T;
				break;
			case	AIP_EvalBranch:
				_depth --;
				if (ctrl_stack->HTI) {
					if (_create_text) {
						indent(text); text->Append("} else {");
					}
					ctrl_stack->HTI = 0;
					prog = &(*prog)->a.i.t->TH;
					_depth ++;
					Pop(loc_base);
					Push(loc_base, NULL);
					ctrl_type = AUI_CTRL_BRANCH2;
				} else {
					if (_create_text) {
						indent(text); text->Append("}");
					}
					Pop(ctrl_stack);
					Pop(loc_base);
					ctrl_type = AUI_CTRL_BRANCH3;
					prog = &(*prog)->a.i.t->TT;
				}
				break;
			case	AIP_EvalCond: // treat as ctrl at output, although expression at input
				if (ctrl_stack->HTI) {
					if (_create_text) {
						ASSERT(exp);
						Push(exp_stack, exp);
					}
					ctrl_stack->HTI = 0;
					prog = &(*prog)->a.i.t->TH;
				} else {
					if (_create_text) {
						ASSERT(exp && exp_stack && exp_stack->T);
						AGmString *exp_str1 = exp_stack->THH->a.s.v;
						AGmString *exp_str2 = exp_stack->HH->a.s.v;
						AGmString *exp_str3 = exp->H->a.s.v;
						long pri_exp1 = (long)exp_stack->TH->THI;
						long pri_exp2 = (long)exp_stack->H->THI;
						long pri_exp3 = (long)exp->THI;
						long pri_op = AUI_PRI_COND;
						AGmString *s = new AGmString(512);
						if (pri_exp1 < pri_op)	s->Append("(");
						s->Append(exp_str1);
						if (pri_exp1 < pri_op)	s->Append(")");
						s->Append(" ? ");
						if (pri_exp2 < pri_op)	s->Append("(");
						s->Append(exp_str2);
						if (pri_exp2 < pri_op)	s->Append(")");
						s->Append(" : ");
						if (pri_exp3 < pri_op)	s->Append("(");
						s->Append(exp_str3);
						if (pri_exp3 < pri_op)	s->Append(")");
						Unref(exp); exp = Ref( List(s->Str(), pri_op) );
						delete s;
						Pop(exp_stack); Pop(exp_stack);
					}
					Pop(ctrl_stack);
					prog = &(*prog)->a.i.t->TT;
				}
				continue;
			case	AIP_EvalOrOr: // treat as ctrl at output, although expression at input
			case	AIP_EvalAndAnd: // treat as ctrl at output, although expression at input
				if (_create_text) {
					ASSERT(exp && exp_stack);
					AGmString *exp_str1 = exp_stack->HH->a.s.v;
					AGmString *exp_str2 = exp->H->a.s.v;
					const char *op_str = NULL;
					long pri_exp1 = (long)exp_stack->HTHI;
					long pri_exp2 = (long)exp->THI;
					long pri_op = 0;
					switch ((*prog)->I) {
					case	AIP_EvalOrOr:	op_str = " || ";	pri_op = AUI_PRI_OROR;		break;
					case	AIP_EvalAndAnd: op_str = " && ";	pri_op = AUI_PRI_ANDAND;	break;
					default: ASSERT(0);
					}
					AGmString *s = new AGmString(512);
					if (pri_exp1 < pri_op)	s->Append("(");
					s->Append(exp_str1);
					if (pri_exp1 < pri_op)	s->Append(")");
					s->Append(op_str);
					if (pri_exp2 < pri_op)	s->Append("(");
					s->Append(exp_str2);
					if (pri_exp2 < pri_op)	s->Append(")");
					Unref(exp); exp = Ref( List(s->Str(), pri_op) );
					delete s;
					Pop(exp_stack);
				}
				Pop(ctrl_stack);
				prog = &(*prog)->a.i.t->TT;
				continue;
			case	AIP_EvalLoop:
				_depth --;
				if (_create_text) {
					indent(text); text->Append("}");
				}
				Pop(ctrl_stack);
				Pop(loc_base);
				ctrl_type = AUI_CTRL_LOOP2;
				prog = &(*prog)->a.i.t->T;
				break;
			}
			break;
		}
		long op = (long)((*prog)->I);
		switch (op) {
		// control
		case	AIP_EvalBlock:
			if (_create_text) {
				ASSERT(exp == NULL && exp_stack == NULL);
				indent(text); text->Append("{");
			}
			Push(ctrl_stack, Cons(*prog));
			Push(loc_base, NULL);
			_depth ++;
			ctrl_type = AUI_CTRL_BLOCK;
			prog = &(*prog)->a.i.t->H;
			break;
		case	AIP_EvalLocVarDcl:
			if (_create_text) {
				ASSERT(exp == NULL && exp_stack == NULL);
				indent(text); text->Append("var ");
			}
			AGmNode *list, *type;
			list = (*prog)->a.i.t->H;
			type = list->H;
			if (_create_text) {
				AGmNode *n = AUiClassShell::TypeToName(type);
				text->Append(n);
				RefUnref(n);
			}
			int first;
			first = 1;
			for (list = list->T; list; list = list->T) {
				Push(loc_base->H, List(list->H, (AGmNode*)NULL, type));
				if (_create_text) {
					if (first == 1) {
						first = 0;
						text->Append(" ");
					} else {
						text->Append(", ");
					}
					text->Append(list->H);
				}
			}
			if (_create_text) text->Append(";");
			prog = &(*prog)->a.i.t->T;
			break;	
		case	AIP_EvalBranch:
			if (_create_text) {
				ASSERT(exp && exp_stack == NULL);
				indent(text); text->Append("if (");
				text->Append(exp->H); Unref(exp); exp = NULL;
				text->Append(") {");
			}
			Push(ctrl_stack, Cons(*prog, Integer(1)));
			Push(loc_base, NULL);
			_depth ++;
			ctrl_type = AUI_CTRL_BRANCH;
			ctrl_tail = &(*prog)->a.i.t;
			prog = &(*prog)->a.i.t->H;
			break;	
		case	AIP_EvalCond: // treat as ctrl at output, although expression at input
			if (_create_text) {
				ASSERT(exp);
				Push(exp_stack, exp);
			}
			Push(ctrl_stack, Cons(*prog, Integer(1)));
			prog = &(*prog)->a.i.t->H;
			continue;
		case	AIP_EvalOrOr: // treat as ctrl at output, although expression at input
			if (_create_text) {
				ASSERT(exp);
				Push(exp_stack, exp);
			}
			Push(ctrl_stack, Cons(*prog));
			prog = &(*prog)->a.i.t->TH;
			continue;
		case	AIP_EvalAndAnd: // treat as ctrl at output, although expression at input
			if (_create_text) {
				ASSERT(exp);
				Push(exp_stack, exp);
			}
			Push(ctrl_stack, Cons(*prog));
			prog = &(*prog)->a.i.t->H;
			continue;
		case	AIP_EvalLoop:
			if (_create_text) {
				ASSERT(exp == NULL && exp_stack == NULL);
				indent(text); text->Append("loop {");
			}
			Push(ctrl_stack,  Cons(*prog));
			Push(loc_base, NULL);
			_depth ++;
			ctrl_type = AUI_CTRL_LOOP;
			prog = &(*prog)->a.i.t->H;
			break;
		case	AIP_EvalBreak:
			if (_create_text) {
				ASSERT(exp == NULL && exp_stack == NULL);
				indent(text); text->Append("break;");
			}
			prog = &(*prog)->a.i.t;
			break;
		case	AIP_EvalCont:
			if (_create_text) {
				ASSERT(exp == NULL && exp_stack == NULL);
				indent(text); text->Append("continue;");
			}
			prog = &(*prog)->a.i.t;
			break;
		case	AIP_EvalTry:
			if (_create_text) {
				indent(text); text->Append("try {");
			}
			Push(ctrl_stack, Cons(*prog, Integer(1)));
			Push(loc_base, NULL);
			_depth ++;
			ctrl_type = AUI_CTRL_TRY;
			ctrl_tail = &(*prog)->a.i.t;
			prog = &(*prog)->a.i.t->H;
			break;
		case	AIP_EvalRet:
			if (_member->THI == AUI_TYPE_VOID) {
				if (_create_text) {
					//ASSERT(exp == NULL && exp_stack == NULL);
					indent(text); text->Append("return;");
				}
			} else {
				if (_create_text) {
					indent(text);
					//ASSERT(exp && exp_stack == NULL);
					text->Append("return ");
					if (exp && exp_stack == NULL) {
						text->Append(exp->H); Unref(exp); exp = NULL;
					} else {
						text->Append("null");
					}
					text->Append(";");
				}
			}
			prog = &(*prog)->a.i.t;
			break;
		case	AIP_EvalThrow:
			if (_create_text) {
				ASSERT(exp && exp_stack == NULL);
				indent(text); text->Append("throw ");
				text->Append(exp->H); Unref(exp); exp = NULL;
				text->Append(";");
			}
			prog = &(*prog)->a.i.t;
			break;
		case	AIP_EvalComment:
			if (_create_text) {
				ASSERT(exp_stack == NULL);
				indent(text);
				text->Append("// ");
				text->Append((*prog)->a.i.t->H->a.s.v);
			}
			prog = &(*prog)->a.i.t->T;
			break;
		case	AIP_EvalSemicolon:
			if (_create_text) {
				ASSERT(exp && exp_stack == NULL);
				indent(text); text->Append(exp->H); text->Append(";");
				Unref(exp); exp = NULL;
			}
			prog = &(*prog)->a.i.t;
			break;
		// operation with one operand	----------------------------------------
		case	AIP_EvalConstInt:
		case	AIP_EvalConstHexa:
		case	AIP_EvalConstQChar:
		case	AIP_EvalConstReal:
		case	AIP_EvalConstString:
		case	AIP_EvalConstVtbl:
		case	AIP_EvalConstFunc:
		case	AIP_EvalConstNull:
		case	AIP_EvalNew:
		case	AIP_EvalThis:
		case	AIP_EvalRuntime:
		case	AIP_EvalLoc:
		case	AIP_EvalArg:
		case	AIP_EvalThisVar:
		case	AIP_EvalClassVar:
			if (_create_text) {
				AGmString *s = new AGmString(512);
				AGmNode *n = ( (*prog)->a.i.t ? (*prog)->a.i.t->H : NULL );
				AGmArc *a;
				switch (op) {
				case	AIP_EvalConstInt:
					s->WriteInteger(n);
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalConstHexa:
					s->WriteString("0x");
					s->WriteHexa((long)n->a.i.v);
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalConstQChar:
					s->WriteQChar((char)n->a.i.v);
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalConstReal:
					s->WriteReal(n->a.r.v);
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalConstString:
					s->WriteQString(n->a.s.v);
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalConstVtbl:
				case	AIP_EvalConstFunc:
					n = dst_node2(*prog, NmMember());
					ASSERT(n);
					a = AUiClassShell::ClassOfMember(n);
					s->Append(a->src->H);
					s->Append("::");
					s->Append(a->attr); 
					prog = &(*prog)->a.i.t;
					break;
				case	AIP_EvalConstNull:
					s->Append("null");
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalNew:
					s->Append("new ");
					n = dst_node2(*prog, NmClass());
					ASSERT(n);
					s->Append(n->H);
					prog = &(*prog)->a.i.t;
					break;
				case	AIP_EvalThis:
					s->Append("this");
					prog = &(*prog)->a.i.t;
					break;
				case	AIP_EvalRuntime:
					s->Append("runtime ");
					n = dst_node2(*prog, NmClass());
					ASSERT(n);
					s->Append(n->H);
					prog = &(*prog)->a.i.t;
					break;
				case	AIP_EvalLoc:
				case	AIP_EvalArg:
				case	AIP_EvalThisVar:
					s->Append(n);
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalClassVar:
					n = dst_node2(*prog, NmMember());
					ASSERT(n);
					a = AUiClassShell::ClassOfMember(n);
					if (!_IsDerived(_class, a->src)) {
						s->Append(a->src->H);
						s->Append("::");
					}
					s->Append(a->attr); 
					prog = &(*prog)->a.i.t;
					break;
				default:
					ASSERT(0);
				}
				Unref(exp); exp = Ref( List(s->Str(), AUI_PRI_PRIM) );
				delete s;
			} else {
				switch (op) {
				case	AIP_EvalConstInt:
				case	AIP_EvalConstHexa:
				case	AIP_EvalConstQChar:
				case	AIP_EvalConstReal:
				case	AIP_EvalConstString:
				case	AIP_EvalConstNull:
				case	AIP_EvalLoc:
				case	AIP_EvalArg:
				case	AIP_EvalThisVar:
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalConstVtbl:
				case	AIP_EvalConstFunc:
				case	AIP_EvalNew:
				case	AIP_EvalThis:
				case	AIP_EvalRuntime:
				case	AIP_EvalClassVar:
					prog = &(*prog)->a.i.t;
					break;
				default:
					ASSERT(0);
				}
			}
			continue;
		// Push ----------------------------------------
		case	AIP_EvalPush:
			if (_create_text)
				Push(exp_stack, exp);
			prog = &(*prog)->a.i.t;
			continue;
		// operation with two operands	----------------------------------------
		case	AIP_EvalEQ:
		case	AIP_EvalNE:
		case	AIP_EvalLT:
		case	AIP_EvalLE:
		case	AIP_EvalGE:
		case	AIP_EvalGT:
		case	AIP_EvalAdd:
		case	AIP_EvalSub:
		case	AIP_EvalMul:
		case	AIP_EvalDiv:
		case	AIP_EvalMod:
		case	AIP_EvalAnd:
		case	AIP_EvalOr:
		case	AIP_EvalXor:
		case	AIP_EvalSL:
		case	AIP_EvalSR: 
			if (_create_text) {
				ASSERT(exp && exp_stack);
				AGmString *exp_str1 = exp_stack->HH->a.s.v;
				AGmString *exp_str2 = exp->H->a.s.v;
				const char *op_str = NULL;
				long pri_exp1 = (long)exp_stack->HTHI;
				long pri_exp2 = (long)exp->THI;
				long pri_op = 0;
				switch (op) {
				case	AIP_EvalEQ: op_str = " == ";	pri_op = AUI_PRI_COMP;	break;
				case	AIP_EvalNE: op_str = " != ";	pri_op = AUI_PRI_COMP;	break;
				case	AIP_EvalLT: op_str = " < "; 	pri_op = AUI_PRI_COMP;	break;
				case	AIP_EvalLE: op_str = " <= ";	pri_op = AUI_PRI_COMP;	break;
				case	AIP_EvalGE: op_str = " >= ";	pri_op = AUI_PRI_COMP;	break;
				case	AIP_EvalGT: op_str = " > "; 	pri_op = AUI_PRI_COMP;	break;
				case	AIP_EvalAdd:	op_str = " + "; pri_op = AUI_PRI_ADD;	break;
				case	AIP_EvalSub:	op_str = " - "; pri_op = AUI_PRI_ADD;	break;
				case	AIP_EvalMul:	op_str = " * "; pri_op = AUI_PRI_MUL;	break;
				case	AIP_EvalDiv:	op_str = " / "; pri_op = AUI_PRI_MUL;	break;
				case	AIP_EvalMod:	op_str = " % "; pri_op = AUI_PRI_MUL;	break;
				case	AIP_EvalAnd:	op_str = " & "; pri_op = AUI_PRI_AND;	break;
				case	AIP_EvalOr: 	op_str = " | "; pri_op = AUI_PRI_OR;	break;
				case	AIP_EvalXor:	op_str = " ^ "; pri_op = AUI_PRI_XOR;	break;
				case	AIP_EvalSL: op_str = " << ";	pri_op = AUI_PRI_SHIFT; break;
				case	AIP_EvalSR: op_str = " >> ";	pri_op = AUI_PRI_SHIFT; break;
				default: ASSERT(0);
				}
				AGmString *s = new AGmString(512);
				if (pri_exp1 < pri_op)	s->Append("(");
				s->Append(exp_str1);
				if (pri_exp1 < pri_op)	s->Append(")");
				s->Append(op_str);
				if (pri_exp2 <= pri_op) s->Append("(");
				s->Append(exp_str2);
				if (pri_exp2 <= pri_op) s->Append(")");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
				Pop(exp_stack);
			}
			prog = &(*prog)->a.i.t;
			continue;
		case	AIP_EvalSetObjVar:
			if (_create_text) {
				ASSERT(exp && exp_stack);
				AGmString *s = new AGmString(512);
				AGmString *exp_str1 = exp_stack->HH->a.s.v;
				AGmString *name = (*prog)->a.i.t->H->a.s.v;
				AGmString *exp_str2 = exp->H->a.s.v;
				long pri_exp2 = (long)exp->THI;
				long pri_op = AUI_PRI_ASSIGN;
				s->Append(exp_str1);
				s->Append(".");
				s->Append(name);
				s->Append(" = ");
				if (pri_exp2 < pri_op)	s->Append("(");
				s->Append(exp_str2);
				if (pri_exp2 < pri_op)	s->Append(")");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
				Pop(exp_stack);
			}
			prog = &(*prog)->a.i.t->T;
			continue;							
		case	AIP_EvalSetHead:
			if (_create_text) {
				ASSERT(exp && exp_stack);
				AGmString *s = new AGmString(512);
				AGmString *exp_str1 = exp_stack->HH->a.s.v;
				AGmString *exp_str2 = exp->H->a.s.v;
				long pri_exp2 = (long)exp->THI;
				long pri_op = AUI_PRI_ASSIGN;
				s->Append(exp_str1);
				s->Append(".head = ");
				if (pri_exp2 < pri_op)	s->Append("(");
				s->Append(exp_str2);
				if (pri_exp2 < pri_op)	s->Append(")");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
				Pop(exp_stack);
			}
			prog = &(*prog)->a.i.t;
			continue;							
		case	AIP_EvalSetTail:
			if (_create_text) {
				ASSERT(exp && exp_stack);
				AGmString *s = new AGmString(512);
				AGmString *exp_str1 = exp_stack->HH->a.s.v;
				AGmString *exp_str2 = exp->H->a.s.v;
				long pri_exp2 = (long)exp->THI;
				long pri_op = AUI_PRI_ASSIGN;
				s->Append(exp_str1);
				s->Append(".tail = ");
				if (pri_exp2 < pri_op)	s->Append("(");
				s->Append(exp_str2);
				if (pri_exp2 < pri_op)	s->Append(")");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
				Pop(exp_stack);
			}
			prog = &(*prog)->a.i.t;
			continue;							
		case	AIP_EvalArray:
			if (_create_text) {
				ASSERT(exp && exp_stack);
				AGmString *s = new AGmString(512);
				AGmString *exp_str1 = exp_stack->HH->a.s.v;
				AGmString *exp_str2 = exp->H->a.s.v;
				s->Append(exp_str1);
				s->Append("[");
				s->Append(exp_str2);
				s->Append("]");
				long pri_op = AUI_PRI_PRIM;
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
				Pop(exp_stack);
			}
			prog = &(*prog)->a.i.t;
			continue;

		// operation with one operand	 (Pre)	----------------------------------------
		case	AIP_EvalSetLoc:
		case	AIP_EvalSetArg:
		case	AIP_EvalSetThisVar:
		case	AIP_EvalSetClassVar:
		case	AIP_EvalChgSign:
		case	AIP_EvalInverse:
		case	AIP_EvalCast:
			if (_create_text) {
				ASSERT(exp);
				AGmString *s = new AGmString(512);
				AGmString *exp_str = exp->H->a.s.v;
				long pri_exp = (long)exp->THI;
				long pri_op = 0;
				AGmNode *n;
				AGmArc *a;
				switch (op) {
				case	AIP_EvalSetLoc:
				case	AIP_EvalSetArg:
				case	AIP_EvalSetThisVar:
					n = (*prog)->a.i.t->H;
					s->Append(n);
					s->Append(" = ");
					pri_op = AUI_PRI_ASSIGN;
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalSetClassVar:
					n = dst_node2(*prog, NmMember());
					ASSERT(n);
					a = AUiClassShell::ClassOfMember(n);
					if (!_IsDerived(_class, a->src)) {
						s->Append(a->src->H);
						s->Append("::");
					}
					s->Append(a->attr); 
					s->Append(" = ");
					pri_op = AUI_PRI_ASSIGN;
					prog = &(*prog)->a.i.t;
					break;
				case	AIP_EvalChgSign:
					s->Append(" - ");
					pri_op = AUI_PRI_PRIM;
					prog = &(*prog)->a.i.t;
					break;
				case	AIP_EvalInverse:
					s->Append(" ~ ");
					pri_op = AUI_PRI_PRIM;
					prog = &(*prog)->a.i.t;
					break;
				case	AIP_EvalCast:
					n = (*prog)->a.i.t->H;
					AGmNode *type_name;
					type_name = Ref(AUiClassShell::TypeToName(n));
					ASSERT(type_name);
					s->Append("(");
					s->Append(type_name->a.s.v);
					s->Append(")");
					Unref(type_name);
					pri_op = AUI_PRI_PRIM;
					prog = &(*prog)->a.i.t->T;
					break;
				default:
					ASSERT(0);
				}
				if (pri_exp < pri_op)	s->Append("(");
				s->Append(exp_str);
				if (pri_exp < pri_op)	s->Append(")");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
			} else {
				switch (op) {
				case	AIP_EvalSetLoc:
				case	AIP_EvalSetArg:
				case	AIP_EvalSetThisVar:
				case	AIP_EvalCast:
					prog = &(*prog)->a.i.t->T;
					break;
				case	AIP_EvalSetClassVar:
				case	AIP_EvalChgSign:
				case	AIP_EvalInverse:
					prog = &(*prog)->a.i.t;
					break;
				default:
					ASSERT(0);
				}
			}
			continue;
		// operation with one operand	 (Post) ----------------------------------------
		case	AIP_EvalHead:
			if (_create_text) {
				ASSERT(exp);
				AGmString *s = new AGmString(512);
				AGmString *exp_str = exp->H->a.s.v;
				long pri_exp = (long)exp->THI;
				long pri_op = AUI_PRI_PRIM;
				if (pri_exp < pri_op) s->Append("(");
				s->Append(exp_str);
				if (pri_exp < pri_op) s->Append(")");
				s->Append(".head");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
			}
			prog = &(*prog)->a.i.t;
			continue;
		case	AIP_EvalTail:
			if (_create_text) {
				ASSERT(exp);
				AGmString *s = new AGmString(512);
				AGmString *exp_str = exp->H->a.s.v;
				long pri_exp = (long)exp->THI;
				long pri_op = AUI_PRI_PRIM;
				if (pri_exp < pri_op) s->Append("(");
				s->Append(exp_str);
				if (pri_exp < pri_op) s->Append(")");
				s->Append(".tail");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
			}
			prog = &(*prog)->a.i.t;
			continue;
		case	AIP_EvalObjVar:
			if (_create_text) {
				ASSERT(exp);
				AGmString *s = new AGmString(512);
				AGmString *exp_str = exp->H->a.s.v;
				long pri_exp = (long)exp->THI;
				long pri_op = AUI_PRI_PRIM;
				if (pri_exp < pri_op) s->Append("(");
				s->Append(exp_str);
				if (pri_exp < pri_op) s->Append(")");
				s->Append(".");
				AGmNode *n = (*prog)->a.i.t->H;
				s->Append(n);
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
			}
			prog = &(*prog)->a.i.t->T;
			continue;
		// operation with three operands
		case	AIP_EvalSetArray:
			if (_create_text) {
				ASSERT(exp && exp_stack && exp_stack->T);
				AGmString *s = new AGmString(512);
				AGmString *exp_str1 = exp_stack->THH->a.s.v;
				AGmString *exp_str2 = exp_stack->HH->a.s.v;
				AGmString *exp_str3 = exp->H->a.s.v;
				long pri_exp3 = (long)exp->THI;
				long pri_op = AUI_PRI_ASSIGN;
				s->Append(exp_str1);
				s->Append("[");
				s->Append(exp_str2);
				s->Append("] = ");
				if (pri_exp3 < pri_op)	s->Append("(");
				s->Append(exp_str3);
				if (pri_exp3 < pri_op)	s->Append(")");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
				Pop(exp_stack); Pop(exp_stack);
			}
			prog = &(*prog)->a.i.t;
			continue;
		case	AIP_EvalMatrix:
			if (_create_text) {
				ASSERT(exp && exp_stack && exp_stack->T);
				AGmString *s = new AGmString(512);
				AGmString *exp_str1 = exp_stack->THH->a.s.v;
				AGmString *exp_str2 = exp_stack->HH->a.s.v;
				AGmString *exp_str3 = exp->H->a.s.v;
				s->Append(exp_str1);
				s->Append("[");
				s->Append(exp_str2);
				s->Append("][");
				s->Append(exp_str3);
				s->Append("]");
				long pri_op = AUI_PRI_PRIM;
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
				Pop(exp_stack); Pop(exp_stack);
			}
			prog = &(*prog)->a.i.t;
			continue;
		// operation with four operands
		case	AIP_EvalSetMatrix:
			if (_create_text) {
				ASSERT(exp && exp_stack && exp_stack->T && exp_stack->TT);
				AGmString *s = new AGmString(512);
				AGmString *exp_str1 = exp_stack->TTHH->a.s.v;
				AGmString *exp_str2 = exp_stack->THH->a.s.v;
				AGmString *exp_str3 = exp_stack->HH->a.s.v;
				AGmString *exp_str4 = exp->H->a.s.v;
				long pri_exp4 = (long)exp->THI;
				long pri_op = AUI_PRI_ASSIGN;
				s->Append(exp_str1);
				s->Append("[");
				s->Append(exp_str2);
				s->Append("][");
				s->Append(exp_str3);
				s->Append("] = ");
				if (pri_exp4 < pri_op)	s->Append("(");
				s->Append(exp_str4);
				if (pri_exp4 < pri_op)	s->Append(")");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
				Pop(exp_stack); Pop(exp_stack); Pop(exp_stack);
			}
			prog = &(*prog)->a.i.t;
			continue;
		// function call
		case	AIP_EvalClassFunc:
		case	AIP_EvalThisFunc:
		case	AIP_EvalObjFunc:
		case	AIP_EvalObjFunc2:
		case	AIP_EvalClassFuncP:
		case	AIP_EvalThisFuncP:
		case	AIP_EvalObjFuncP:
		case	AIP_EvalParaClassFunc:
		case	AIP_EvalParaObjFunc:
			if (_create_text) {
				AGmString *s = new AGmString(512);
				AGmString *func = new AGmString(512);
				AGmString *args = new AGmString(512);
				AGmNode *func_dcl = NULL, *ls;
				AGmArc *a;	int first = 1;
				switch(op) {
				case	AIP_EvalClassFunc:
					func_dcl = dst_node2(*prog, NmMember());
					break;
				case	AIP_EvalParaClassFunc:
					s->Append("para ");
					func_dcl = dst_node2(*prog, NmMember());
					break;
				case	AIP_EvalThisFunc:
				case	AIP_EvalObjFunc:
				case	AIP_EvalObjFunc2:
					func_dcl = dst_node2(*prog, NmVtbl());
					break;
				case	AIP_EvalParaObjFunc:
					s->Append("para ");
					func_dcl = dst_node2(*prog, NmVtbl());
					break;
				case	AIP_EvalClassFuncP:
				case	AIP_EvalThisFuncP:
				case	AIP_EvalObjFuncP:
					func_dcl = dst_node2(*prog, NmType());
					break;
				default:
					ASSERT(0);
				}
				ASSERT(func_dcl);
				switch (op) {
				case	AIP_EvalClassFunc:
				case	AIP_EvalParaClassFunc:
					a = AUiClassShell::ClassOfMember(func_dcl);
					if (!_IsDerived(_class, a->src)) {
						s->Append(a->src->H);
						s->Append("::");
					}
					func->Append(a->attr);
					break;
				case	AIP_EvalThisFunc:
				case	AIP_EvalObjFunc:
				case	AIP_EvalParaObjFunc:
					a = AUiClassShell::ClassOfMember(func_dcl);
					func->Append(a->attr);
					break;
				case	AIP_EvalObjFunc2:
					a = AUiClassShell::ClassOfMember(func_dcl);
					func->Append(a->src->H);
					func->Append("::");
					func->Append(a->attr);
					break;
				case	AIP_EvalClassFuncP:
				case	AIP_EvalThisFuncP:
				case	AIP_EvalObjFuncP:
					ASSERT(exp);
					func->Append("*");
					func->Append(exp->H);
					Unref(exp); exp = NULL;
					if (exp_stack) {
						exp = Ref(exp_stack->H); Pop(exp_stack);
					}
					break;
				default:
					ASSERT(0);
				}
				args->Append("(");
				for (ls = func_dcl->TTTH; ls; ls = ls->TT) {
					if (first) {
						first = 0;
					} else {
						args->Append(", ");
					}
					ASSERT(exp);
					args->Append(exp->H);
					Unref(exp); exp = NULL;
					if (exp_stack) {
						exp = Ref(exp_stack->H); Pop(exp_stack);
					}
				}
				args->Append(")");
				AGmString *exp_str;
				long pri_exp;
				long pri_op = AUI_PRI_PRIM;
				switch (op) {
				case	AIP_EvalObjFunc:
				case	AIP_EvalObjFuncP:
				case	AIP_EvalParaObjFunc:
					ASSERT(exp);
					exp_str = exp->H->a.s.v;
					pri_exp = (long)exp->THI;
					if (pri_exp < pri_op)	s->Append("(");
					s->Append(exp_str);
					if (pri_exp < pri_op)	s->Append(")");
					s->Append(".");
					break;
				default:
					break;
				}
				s->Append(func); s->Append(args);
				delete func; delete args;
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
			}
			prog = &(*prog)->a.i.t;
			continue;
		case	AIP_EvalSysFunc:
			if (_create_text) {
				int first = 1;
				long index = (long)((*prog)->a.i.t->I);
				AGmString *s = new AGmString(512);
				s->Append(AIpProcess::SysFuncName(index));
				s->Append("(");
				int n = AIpProcess::SysFuncNumArg(index);
				for (int i = 0; i < n; i++) {
					if (first) {
						first = 0;
					} else {
						s->Append(", ");
					}
					ASSERT(exp);
					s->Append(exp->H);
					Unref(exp); exp = NULL;
					if (exp_stack) {
						exp = Ref(exp_stack->H); Pop(exp_stack);
					}
				}
				s->Append(")");
				long pri_op = AUI_PRI_PRIM;
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete s;
			}
			prog = &(*prog)->a.i.t->a.i.t;
			continue;
		case	AIP_EvalOcxInvoke:
			if (_create_text) {
				int first = 1;
				AGmString *func_args = new AGmString(512);
				func_args->Append((*prog)->a.i.t->H->a.s.v->Str());
				func_args->Append("(");
				int n = (int)(*prog)->a.i.t->T->a.i.v;
				for (int i = 0; i < n; i++) {
					if (first) {
						first = 0;
					} else {
						func_args->Append(", ");
					}
					ASSERT(exp);
					func_args->Append(exp->H);
					Unref(exp); exp = NULL;
					if (exp_stack) {
						exp = Ref(exp_stack->H); Pop(exp_stack);
					}
				}
				func_args->Append(")");

				AGmString *s = new AGmString(512);
				ASSERT(exp);
				AGmString *exp_str = exp->H->a.s.v;
				long pri_op = AUI_PRI_PRIM;
				long pri_exp = (long)exp->THI;
				if (pri_exp < pri_op)	s->Append("(");
				s->Append(exp_str);
				if (pri_exp < pri_op)	s->Append(")");
				s->Append(".");
				s->Append(func_args);
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete func_args; delete s;
			}
			prog = &(*prog)->a.i.t->T->a.i.t;
			continue;
		case	AIP_EvalOcxSetProp:
			if (_create_text) {
				int first = 1;
				AGmString *func_args = new AGmString(512);
				func_args->Append((*prog)->a.i.t->H->a.s.v);
				func_args->Append("(");
				int n = (int)(*prog)->a.i.t->T->a.i.v - 1;
				for (int i = 0; i < n; i++) {
					if (first) {
						first = 0;
					} else {
						func_args->Append(", ");
					}
					ASSERT(exp);
					func_args->Append(exp->H);
					Unref(exp); exp = NULL;
					if (exp_stack) {
						exp = Ref(exp_stack->H); Pop(exp_stack);
					}
				}
				func_args->Append(")");

				ASSERT(exp);
				AGmString *value = new AGmString(exp->H->a.s.v);
				long pri_value = (long)exp->THI;
				Unref(exp); exp = NULL;
				if (exp_stack) {
					exp = Ref(exp_stack->H); Pop(exp_stack);
				}

				AGmString *s = new AGmString(512);
				ASSERT(exp);
				AGmString *exp_str = exp->H->a.s.v;
				long pri_op = AUI_PRI_PRIM;
				long pri_exp = (long)exp->THI;
				pri_op = AUI_PRI_ASSIGN;
				if (pri_exp < pri_op)	s->Append("(");
				s->Append(exp_str);
				if (pri_exp < pri_op)	s->Append(")");
				s->Append(".");
				s->Append(func_args);
				s->Append(" = ");
				if (pri_value < pri_op) s->Append("(");
				s->Append(value);
				if (pri_value < pri_op) s->Append(")");
				Unref(exp); exp = Ref( List(s->Str(), pri_op) );
				delete func_args; delete value; delete s;
			}
			prog = &(*prog)->a.i.t->T->a.i.t;
			continue;

		// not implemented
		case	AIP_EvalPop:
		default:
			ASSERT(0);
		}
		break;
	}
	if (*prog == NULL && ctrl_stack == NULL) {
		_status = 1;
	}
	if (_depth < _limit_depth) _lno ++;
	return 0;
}

