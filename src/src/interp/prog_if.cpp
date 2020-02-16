/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <interp/basefunc.h>
#include <interp/prog_if.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

AGpProgIF::AGpProgIF() {
	ctrl_stack = NULL;
	_end = 0;
}
AGpProgIF::~AGpProgIF() {
	Unref(ctrl_stack);
}

AGmNode** AGpProgIF::Prog() { return prog; }
void AGpProgIF::Prog(AGmNode **p) {
	prog = p;
	_end = (*prog == NULL);
}

int AGpProgIF::More() {
	return (_end == 0);
}
void AGpProgIF::Next() {
	if (*prog == NULL) {
		if (ctrl_stack == NULL) {
			_end = 1; return;
		}
		prog = &ctrl_stack->HH;
		switch ((*prog)->I) {
		case	AIP_EvalBlock:
			Pop(ctrl_stack);
			prog = &(*prog)->a.i.t->T;
			break;
		case	AIP_EvalBranch:
			if (ctrl_stack->HTI) {
				ctrl_stack->HTI = 0;
				prog = &(*prog)->a.i.t->TH;
			} else {
				Pop(ctrl_stack);
				prog = &(*prog)->a.i.t->TT;
			}
			break;
		case	AIP_EvalCond: // treat as ctrl at output, although expression at input
			if (ctrl_stack->HTI) {
				ctrl_stack->HTI = 0;
				prog = &(*prog)->a.i.t->TH;
			} else {
				Pop(ctrl_stack);
				prog = &(*prog)->a.i.t->TT;
			}
			break;
		case	AIP_EvalOrOr: // treat as ctrl at output, although expression at input
		case	AIP_EvalAndAnd: // treat as ctrl at output, although expression at input
			Pop(ctrl_stack);
			prog = &(*prog)->a.i.t->TT;
			break;
		case	AIP_EvalLoop:
			Pop(ctrl_stack);
			prog = &(*prog)->a.i.t->T;
			break;
		case	AIP_EvalTry:
			if (ctrl_stack->HTI) {
				ctrl_stack->HTI = 0;
				prog = &(*prog)->a.i.t->TH;
			} else {
				Pop(ctrl_stack);
				prog = &(*prog)->a.i.t->TT;
			}
			break;
		}
		return;
	}
	long op = (long)(*prog)->I;
	switch (op) {
	// control
	case	AIP_EvalBlock:
		Push(ctrl_stack, Cons(*prog));
		prog = &(*prog)->a.i.t->H;
		break;
	case	AIP_EvalLocVarDcl:
		prog = &(*prog)->a.i.t->T;
		break;	
	case	AIP_EvalBranch:
		Push(ctrl_stack, Cons(*prog, Integer(1)));
		prog = &(*prog)->a.i.t->H;
		break;	
	case	AIP_EvalCond: // treat as ctrl at output, although expression at input
		Push(ctrl_stack, Cons(*prog, Integer(1)));
		prog = &(*prog)->a.i.t->H;
		break;
	case	AIP_EvalOrOr: // treat as ctrl at output, although expression at input
		Push(ctrl_stack, Cons(*prog));
		prog = &(*prog)->a.i.t->TH;
		break;
	case	AIP_EvalAndAnd: // treat as ctrl at output, although expression at input
		Push(ctrl_stack, Cons(*prog));
		prog = &(*prog)->a.i.t->H;
		break;
	case	AIP_EvalLoop:
		Push(ctrl_stack,  Cons(*prog));
		prog = &(*prog)->a.i.t->H;
		break;
	case	AIP_EvalBreak:
	case	AIP_EvalCont:
	case	AIP_EvalRet:
	case	AIP_EvalThrow:
		prog = &(*prog)->a.i.t;
		break;	
	case	AIP_EvalTry:
		Push(ctrl_stack, Cons(*prog, Integer(1)));
		prog = &(*prog)->a.i.t->H;
		break;	
	case	AIP_EvalComment:
		prog = &(*prog)->a.i.t->T;
		break;
	case	AIP_EvalSemicolon:
		prog = &(*prog)->a.i.t;
		break;
	// operation with one operand	----------------------------------------
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
	// Push ----------------------------------------
	case	AIP_EvalPush:
		prog = &(*prog)->a.i.t;
		break;
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
		prog = &(*prog)->a.i.t;
		break;
	case	AIP_EvalSetObjVar:
		prog = &(*prog)->a.i.t->T;
		break;							
	case	AIP_EvalSetHead:
		prog = &(*prog)->a.i.t;
		break;							
	case	AIP_EvalSetTail:
		prog = &(*prog)->a.i.t;
		break;							
	case	AIP_EvalArray:
		prog = &(*prog)->a.i.t;
		break;							
	// operation with one operand	 (Pre)	----------------------------------------
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
	// operation with one operand	 (Post) ----------------------------------------
	case	AIP_EvalHead:
	case	AIP_EvalTail:
		prog = &(*prog)->a.i.t;
		break;
	case	AIP_EvalObjVar:
		prog = &(*prog)->a.i.t->T;
		break;
	// operation with three operands
	case	AIP_EvalSetArray:
	case	AIP_EvalMatrix:
		prog = &(*prog)->a.i.t;
		break;
	// operation with four operands
	case	AIP_EvalSetMatrix:
		prog = &(*prog)->a.i.t;
		break;
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
		prog = &(*prog)->a.i.t;
		break;
	case	AIP_EvalSysFunc:
		prog = &(*prog)->a.i.t->a.i.t;
		break;
	case	AIP_EvalOcxInvoke:
	case	AIP_EvalOcxSetProp:
		prog = &(*prog)->a.i.t->T->a.i.t;
		break;

	// not implemented
	case	AIP_EvalPop:
	default:
		ASSERT(0);
	}
}

AGmNode *AGpProgIF::MemberVar() {
	if (*prog && (*prog)->I == AIP_EvalThisVar) {
		return (*prog)->a.i.t->H;
	}
	return NULL;
}
AGmNode *AGpProgIF::MemberSFunc() {
	if (*prog && (*prog)->I == AIP_EvalClassFunc) {
		return dst_node(*prog, NmMember());
	}
	return NULL;
}
AGmNode *AGpProgIF::MemberVFunc() {
	if (*prog && (*prog)->I == AIP_EvalThisFunc) {
		return dst_node(*prog, NmVtbl());
	}
	return NULL;
}

int AGpProgIF::Code_i(long i) {
	AGmNode *l1 = Integer(i);
	Set(*prog, l1); prog = &l1->a.i.t;
	return 0;
}
int AGpProgIF::Code_i_i(long i, long i2) {
	AGmNode *l1 = Integer(i2);
	AGmNode *l2 = Integer(i, l1);
	Set(*prog, l2); prog = &l1->a.i.t;
	return 0;
}
int AGpProgIF::Code_i_n(long i, AGmNode *n) {
	AGmNode *l1 = Cons(n);
	AGmNode *l2 = Integer(i, l1);
	Set(*prog, l2); prog = &l1->a.l.t;
	return 0;
}
int AGpProgIF::Code_i_n_n(long i, AGmNode *n, AGmNode *n2) {
	AGmNode *l1 = Cons(n2);
	AGmNode *l2 = Cons(n, l1);
	AGmNode *l3 = Integer(i, l2);
	Set(*prog, l3); prog = &l1->a.l.t;
	return 0;
}
void AGpProgIF::Code_Del() {
	AGmNode **p = prog;
	Next();
	Set(*p, *prog);
	prog = p;
	if (*prog == NULL) _end = 1;
}

