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
#include <gdm/big_int.h>
#include <gdm/matrix.h>
#include <gdm/file.h>
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <interp/basefunc.h>
#include <interp/sysfunc.h>
#include <interp/prog_if.h>
#include <dev/advwnd.h>
#include <dev/shell.h>
#include <cui/class.h>
#include <cui/body.h>

#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
//#define	_DEBUG_INTERP1
//#define	_DEBUG_INTERP2
int interp_trace = 0;
//#define	_DEBUG_STACK_TRACE
//#define	_DEBUG_PROCESS
int AIpProcess::eval_ret_func_name_trace = 0;
#endif

#ifdef	_DEBUG_PROCESS
int num_of_process = 0;
#endif

AIpProcess::AIpProcess(AIpProcessMgr* prcsmgr, AGmNode *root_class, AGmNode *arg, AGmNode *func, AGmNode *this_p) {
	ASSERT(prcsmgr);
	_prcsmgr = prcsmgr;
	_root_class = Ref(root_class);

	CreateArc(_prcsmgr->Processes(), PtrCell(this), NULL, NULL, NULL, &_link);

	val = val_stack =
	loc_base = ctrl_stack = func_stack = func_stack_ls =
	this_ptr_ls = arg_base_ls = loc_base_ls = ctrl_stack_ls = NULL;

	exception = 0;
	return_process = NULL;

	waiting = 0;

	Push(func_stack,
		List(NULL, NULL, arg, loc_base, ctrl_stack, func));

	this_ptr = this_p;
	prog = func->TTH;
	arg_base = arg;

	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

	para_lock = 0;

	notify_cells = Ref(Cons(NULL));
#ifdef	_DEBUG_PROCESS
	num_of_process++;
	TRACE1("new Process, #%d\n", num_of_process);
#endif
}

AIpProcess::~AIpProcess() {
	Unref(val, val_stack,
		func_stack, this_ptr_ls,
		arg_base_ls, loc_base_ls, ctrl_stack_ls, func_stack_ls);
	Unref(_root_class);

	_prcsmgr->RemoveLocalLoopProcess(this);
	DeleteArc(_link->arc);
	_prcsmgr->NextProcess(_link);

	AGmLink *l; AGmNode *cell;
	for (l = next_outlink(notify_cells, NULL); l; l = next_outlink(notify_cells, l)) {
		cell = l->arc->dst;
		ASSERT(IsPtrCell(cell));
		cell->a.p.ptr = NULL;
	}
	Unref(notify_cells);

	AGmNode *n = _prcsmgr->Processes();
	for (l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		AIpProcess *p = (AIpProcess*)l->arc->dst->a.p.ptr;
		if (p->return_process == this) {
			p->return_process = NULL; break;
		}
	}

#ifdef	_DEBUG_PROCESS
	num_of_process--;
	TRACE1("delete Process, #%d\n", num_of_process);
#endif
}

AGmNode* AIpProcess::RootClass() { return _root_class; }
AGmLink* AIpProcess::Link() { return _link; }
AGmNode *AIpProcess::Val() { return val; }
void AIpProcess::Val(AGmNode* v) { Unref(val); val = Ref(v); }
int AIpProcess::IsException() { return exception; }
int AIpProcess::ParaLock() { return para_lock; }
void AIpProcess::ParaLock(int v) { para_lock = v; }

AIpProcess* AIpProcess::ReturnProcess() {
	return return_process;
}
void AIpProcess::ReturnProcess(AIpProcess *p) {
	return_process = p;
	_prcsmgr->AddLocalLoopProcess(p);
}

int AIpProcess::IsWaiting() {
	return waiting;
}
void AIpProcess::Wait() {
	waiting = 1;
	_prcsmgr->AddLocalLoopProcess(this, 1);
}
void AIpProcess::Notify() {
	waiting = 0;
	_prcsmgr->RemoveLocalLoopProcess(this, 1);
}

int AIpProcess::ExecuteException(AIpProcess *process) {
	_prcsmgr->RemoveLocalLoopProcess(this);
	int excep = process->IsException();
	Unref(val); val = Ref(process->Val());
	if (excep && val) {
		AGmNode *n = dst_node(val, UniqueStr("stack_frame"));
		while (n->T) n = n->T;
		Set(n->T, func_stack);
		return EvalThrow();
	}
	return 0;
}
int AIpProcess::Execute() {
	for (;;) {
		if (prog == NULL) {
			if (ctrl_stack == NULL) {
				return EvalRet();
			}
			prog = ctrl_stack->HH;
			long func_no;
			func_no = (long)prog->I;
#ifdef	_DEBUG_INTERP2
if (interp_trace) {
TRACE0("\t\tret\t"); _BaseFuncName(func_no);
}
#endif
			int (AIpProcess::*func)() = retFuncTbl[func_no].func;
#ifdef	_DEBUG
			if (eval_ret_func_name_trace) {
				TRACE1("Ret: %s\n", retFuncTbl[func_no].name);
			}
#endif
			return (this->*func)();
		}
#ifdef	_DEBUG_INTERP1
	DumpList(prog, 10);
#endif
		switch (prog->I) {
		case AIP_EvalSemicolon:
			prog = prog->a.i.t;
			continue;
		case AIP_EvalComment:
			prog = prog->a.i.t->T;
			continue;
		default:
			long func_no;
			func_no = (long)prog->I;
#ifdef	_DEBUG_INTERP2
if (interp_trace) {
TRACE0("\t\teval\t"); _BaseFuncName(func_no);
}
#endif
			int (AIpProcess::*func)() = evalFuncTbl[func_no].func;
#ifdef	_DEBUG
			if (eval_ret_func_name_trace) {
				TRACE1("Eval: %s\n", evalFuncTbl[func_no].name);
			}
#endif
			int ret = (this->*func)();
			return ret;
		}
	}
}

// control
int AIpProcess::EvalBlock() {
	Push(ctrl_stack, Cons(prog)); ctrl_stack_ls->H = ctrl_stack;
	Push(loc_base, NULL); loc_base_ls->H = loc_base;
	prog = prog->a.i.t->H;
	return 0;	
}
int AIpProcess::RetBlock() {
	Pop(ctrl_stack); ctrl_stack_ls->H = ctrl_stack;
	Pop(loc_base); loc_base_ls->H = loc_base;	
	prog = prog->a.i.t->T;
	return 0;
}

int AIpProcess::EvalLocVarDcl() {
	AGmNode *list = prog->a.i.t->H;
	AGmNode *type = list->H;
	for (list = list->T; list; list = list->T) {
		Push(loc_base->H, List(list->H, (AGmNode*)NULL, type));
	}
	prog = prog->a.i.t->T;
	return 0;	
}

int AIpProcess::EvalBranch() {
	AGmNode *prog1;
	if (val) {
		prog1 = prog->a.i.t->H;
	} else {
		prog1 = prog->a.i.t->TH;
	}
	Push(ctrl_stack, Cons(prog)); ctrl_stack_ls->H = ctrl_stack;
	Push(loc_base, NULL); loc_base_ls->H = loc_base;	
	prog = prog1;
	return 0;	
}
int AIpProcess::RetBranch() {
	Pop(loc_base); loc_base_ls->H = loc_base;	
	Pop(ctrl_stack); ctrl_stack_ls->H = ctrl_stack;
	prog = prog->a.i.t->TT;
	return 0;
}

int AIpProcess::EvalLoop() {
	Push(ctrl_stack, Cons(prog)); ctrl_stack_ls->H = ctrl_stack;
	Push(loc_base, NULL); loc_base_ls->H = loc_base;	
	prog = prog->a.i.t->H;
	return 0;	
}
int AIpProcess::RetLoop() {
	Pop(loc_base); loc_base_ls->H = loc_base;	
	Push(loc_base, NULL); loc_base_ls->H = loc_base;	
	prog = prog->a.i.t->H;
	return 0;
}
int AIpProcess::EvalBreak() {
	for (;;) {
		if (ctrl_stack == NULL) return interp_error("Break()");
		if (ctrl_stack->HHI != AIP_EvalLoop) {
			Pop(loc_base); loc_base_ls->H = loc_base;	
			Pop(ctrl_stack); ctrl_stack_ls->H = ctrl_stack;
			continue;
		} else {
			prog = ctrl_stack->HHTT;
			Pop(loc_base); loc_base_ls->H = loc_base;	
			Pop(ctrl_stack); ctrl_stack_ls->H = ctrl_stack;
			return 0;
		}	
	}
}
int AIpProcess::EvalCont() {
	for (;;) {
		if (ctrl_stack == NULL) return interp_error("Cont()");
		if (ctrl_stack->HHI != AIP_EvalLoop) {
			Pop(loc_base); loc_base_ls->H = loc_base;	
			Pop(ctrl_stack); ctrl_stack_ls->H = ctrl_stack;
			continue;
		} else {
			prog = ctrl_stack->HHTH;
			return 0;
		}	
	}
}

int AIpProcess::EvalTry() {
	Push(ctrl_stack, List(prog, Integer(0))); ctrl_stack_ls->H = ctrl_stack;
	Push(loc_base, NULL); loc_base_ls->H = loc_base;	
	prog = prog->a.i.t->H;
	return 0;
}
int AIpProcess::RetTry() {
	Pop(loc_base); loc_base_ls->H = loc_base;	
	Pop(ctrl_stack); ctrl_stack_ls->H = ctrl_stack;
	prog = prog->a.i.t->TT;
	return 0;
}

int AIpProcess::EvalThrow() {
	if (val) {
		if (!dst_node(val, UniqueStr("stack_frame"))) {
			if (func_stack)
				CreateArc(val, func_stack, UniqueStr("stack_frame"));
		}
		if (!dst_node(val, UniqueStr("pos"))) {
			if (prog)
				CreateArc(val, prog, UniqueStr("pos"));
		}
	}

	for (;;) {
		if (func_stack == NULL) break;
		for (;;) {
			if (ctrl_stack == NULL) break;

			if (ctrl_stack->HHI == AIP_EvalTry && ctrl_stack->HTHI == 1) {
				Pop(ctrl_stack); ctrl_stack_ls->H = ctrl_stack;
			}

			if (ctrl_stack->HHI != AIP_EvalTry) {
				Pop(loc_base); loc_base_ls->H = loc_base;	
				Pop(ctrl_stack); ctrl_stack_ls->H = ctrl_stack;
				continue;
			} else {
				prog = ctrl_stack->HH->a.i.t->TH;
				ctrl_stack->HTHI = 1;

				Pop(loc_base); loc_base_ls->H = loc_base;	

				Push(loc_base, NULL); loc_base_ls->H = loc_base;

				AGmNode *type = AUiClassShell::NameToType(_root_class, "AlException");
				AGmNode *name = UniqueStr("e");
				Push(loc_base->H, List(name, val, type));

				exception = 0;
				return 0;
			}	
		}
		// return from function
		AGmNode *stack_data = func_stack->H;
		Pop(this_ptr_ls); this_ptr = stack_data->H;
		prog = stack_data->TH;
		Pop(arg_base_ls); arg_base = stack_data->TTH;
		Pop(loc_base_ls); loc_base = stack_data->TTTH;
		Pop(ctrl_stack_ls); ctrl_stack = stack_data->TTTTH;
		Pop(func_stack);
	}

	exception = 1;
	return AIP_EXIT;
}

// compare
int AIpProcess::EvalEQ() {
	GetArg();
	int flag;
	if (IsNumber(arg1) && IsNumber(arg2)) {
		flag = BigInteger::Compare(arg1, arg2);
	} else {
		flag = Compare(arg1, arg2);
	}
	if (flag == 0) val = Ref(Integer(1));
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalNE() {
	GetArg();
	int flag;
	if (IsNumber(arg1) && IsNumber(arg2)) {
		flag = BigInteger::Compare(arg1, arg2);
	} else {
		flag = Compare(arg1, arg2);
	}
	if (flag != 0) val = Ref(Integer(1));
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;	
}

int AIpProcess::EvalLT() {
	GetArg();
	int flag;
	if (IsNumber(arg1) && IsNumber(arg2)) {
		flag = BigInteger::Compare(arg1, arg2);
	} else {
		flag = Compare(arg1, arg2);
	}
	if (flag < 0) val = Ref(Integer(1));
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalLE() {
	GetArg();
	int flag;
	if (IsNumber(arg1) && IsNumber(arg2)) {
		flag = BigInteger::Compare(arg1, arg2);
	} else {
		flag = Compare(arg1, arg2);
	}
	if (flag <= 0) val = Ref(Integer(1));
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalGE() {
	GetArg();
	int flag;
	if (IsNumber(arg1) && IsNumber(arg2)) {
		flag = BigInteger::Compare(arg1, arg2);
	} else {
		flag = Compare(arg1, arg2);
	}
	if (flag >= 0) val = Ref(Integer(1));
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalGT() {
	GetArg();
	int flag;
	if (IsNumber(arg1) && IsNumber(arg2)) {
		flag = BigInteger::Compare(arg1, arg2);
	} else {
		flag = Compare(arg1, arg2);
	}
	if (flag > 0) val = Ref(Integer(1));
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;	
}

// +-*/
int AIpProcess::EvalAdd() {
	GetArg();
	if ((IsNumber(arg1) || IsString(arg1)) && IsNumber(arg2)) {
		val = Ref(BigInteger::Add(arg1, arg2));
	} else if (IsString(arg1) && IsString(arg2)) {
		AGmString *s = new AGmString(arg1->a.s.v);
		s->Append(arg2->a.s.v);
		val = Ref(String(s));
	} else if (IsMatrix(arg1) && IsMatrix(arg2)) {
		AGmMatrix *m = arg1->a.m.v->Add(arg2->a.m.v);
		val = Ref(Matrix(m));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("Add()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalSub() {
	GetArg();
	if (IsNumber(arg1) && IsNumber(arg2)) {
		val = Ref(BigInteger::Sub(arg1, arg2));
	} else if (IsMatrix(arg1) && IsMatrix(arg2)) {
		AGmMatrix *m = arg1->a.m.v->Sub(arg2->a.m.v);
		val = Ref(Matrix(m));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("Sub()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalMul() {
	GetArg();
	if (IsNumber(arg1) && IsNumber(arg2)) {
		val = Ref(BigInteger::Mul(arg1, arg2));
	} else if (IsMatrix(arg1) && IsMatrix(arg2)) {
		AGmMatrix *m = arg1->a.m.v->Mul(arg2->a.m.v);
		val = Ref(Matrix(m));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("Mul()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalDiv() {
	GetArg();
	if (IsNumber(arg1) && IsNumber(arg2)) {
		val = Ref(BigInteger::Div(arg1, arg2));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("Div()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalMod() {
	GetArg();
	if (IsNumber(arg1) && IsNumber(arg2)) {
		val = Ref(BigInteger::Mod(arg1, arg2));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("Mod()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}

// logical operation
int AIpProcess::EvalOr() {
	GetArg();
	if (IsInteger(arg1) && IsInteger(arg2)) {
		val = Ref(Integer(arg1->a.i.v | arg2->a.i.v));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("Or()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalXor() {
	GetArg();
	if (IsInteger(arg1) && IsInteger(arg2)) {
		val = Ref(Integer(arg1->a.i.v ^ arg2->a.i.v));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("Xor()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalAnd() {
	GetArg();
	if (IsInteger(arg1) && IsInteger(arg2)) {
		val = Ref(Integer(arg1->a.i.v & arg2->a.i.v));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("And()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}

// shift operation
int AIpProcess::EvalSL() {
	GetArg();
	if (IsInteger(arg1) && IsInteger(arg2)) {
		val = Ref(Integer(arg1->a.i.v << arg2->a.i.v));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("SL()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalSR() {
	GetArg();
	if (IsInteger(arg1) && IsInteger(arg2)) {
		val = Ref(Integer(arg1->a.i.v >> arg2->a.i.v));
	}
	if (val == NULL) {
		Unref(arg1, arg2);
		return interp_error("SR()");
	}
	Unref(arg1, arg2);
	prog = prog->a.i.t;
	return 0;
} 

// operation with one operand
int AIpProcess::EvalChgSign() {
#if	_WINDOWS && _64BIT
	long long i; double r; AGmMatrix *m;
#else
	long i; double r; AGmMatrix *m;
#endif
	if (val == NULL)
		return interp_error("ChgSign() null");
	switch (val->type) {
	case AGM_NODE_INTEGER:
		i = val->a.i.v;
		i = -i;
		Unref(val); val = Ref(Integer(i));
		break;	
	case AGM_NODE_REAL:
		r = val->a.r.v;
		r = -r;
		Unref(val); val = Ref(Real(r));
		break;	
	case AGM_NODE_MATRIX:
		m = val->a.m.v;
		m = m->ChangeSign();
		Unref(val); val = Ref(Matrix(m));
		break;	
	default:
		Unref(val); val = NULL;
		return interp_error("ChgSign()");
	}
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalInverse() {
#if	_WINDOWS && _64BIT
	long long i; AGmMatrix *m;
#else
	long i; AGmMatrix *m;
#endif
	if (val == NULL)
		return interp_error("Inverse() null");
	switch (val->type) {
	case AGM_NODE_INTEGER:
		i = val->a.i.v;
		i = ~i;
		Unref(val); val = Ref(Integer(i));
		break;	
	case AGM_NODE_MATRIX:
		m = val->a.m.v;
		m = m->Inverse();
		Unref(val); val = Ref(Matrix(m));
		break;	
	default:
		return interp_error("Inverse()");
	}
	prog = prog->a.i.t;
	return 0;
}

// make arguments match for operation with two operands
void AIpProcess::GetArg() {
	arg1 = Ref(val_stack->H); arg2 = Ref(val);
	Pop(val_stack);
	Unref(val); val = NULL;
}

// read
int AIpProcess::EvalConst() {
	Unref(val); val = Ref(prog->a.i.t->H);
	prog = prog->a.i.t->T;
	return 0;	
}
int AIpProcess::EvalConstFunc() {
	Unref(val); val = Ref(dst_node2(prog, NmMember()));
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalConstVtbl() {
	Unref(val); val = Ref(dst_node2(prog, NmMember()));
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalNew() {
	AGmNode *class_name, *class_id;
	class_id = dst_node2(prog, NmClass());
	ASSERT(class_id);
	class_name = class_id->H;
	Unref(val);
	val = Ref(Object(class_name, class_id));
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalThis() {
	Unref(val);
	val = Ref(this_ptr);
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalLoc() {
	AGmNode *name, **value;
	name = prog->a.i.t->H;
	AGmNode *ls;
	for (ls = loc_base ; ls; ls = ls->T) {
		if (SearchVar(ls->H, name, value) == 0) {
			Unref(val); val = Ref(*value);
			prog = prog->a.i.t->T;
			return 0;	
		}
	}
	char err[256];
	art_sprintf_1(err, 256, "Loc() %s", name->a.s.v->Str());
	return interp_error(err);
}
int AIpProcess::EvalArg() {
	AGmNode *name, **value;
	name = prog->a.i.t->H;
	if (SearchVar(arg_base, name, value) == 0) {
		Unref(val); val = Ref(*value);
		prog = prog->a.i.t->T;
		return 0;
	}
	char err[256];
	art_sprintf_1(err, 256, "Arg() %s", name->a.s.v->Str());
	return interp_error(err);
}
int AIpProcess::EvalThisVar() {
	AGmNode *name = prog->a.i.t->H;
	if (this_ptr == NULL) {
		char err[256];
		art_sprintf_1(err, 256, "ThisVar() %s", name->a.s.v->Str());
		interp_error(err);
	}
	AGmNode *n = Ref(dst_node2(this_ptr, name));
	Unref(val); val = n;
	prog = prog->a.i.t->T;
	return 0;	
}
int AIpProcess::EvalClassVar() {
	AGmNode *member = dst_node2(prog, NmMember());
	ASSERT(member);
	AGmNode *n = Ref(dst_node2(member, NULL));
	Unref(val); val = n;
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalObjVar() {
	AGmNode *name = prog->a.i.t->H;
	AGmNode *n = Ref(dst_node2(val, name));
	Unref(val); val = n;
	prog = prog->a.i.t->T;
	return 0;	
}
int AIpProcess::EvalHead() {
	if (val == NULL || !IsList(val)) {
		return interp_error("Head()");
	}
	AGmNode *n = Ref(val->H);
	Unref(val); val = n;
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalTail() {
	if (val == NULL || !IsList(val)) {
		return interp_error("Tail()");
	}
	AGmNode *n = Ref(val->T);
	Unref(val); val = n;
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalArray() {
	AGmNode *index = Ref(val);
	if (val_stack == NULL) {
		Unref(index); return interp_error("Array()");
	}
	AGmNode *array = Ref(val_stack->H);
	Pop(val_stack);
	if (!IsArray(array) || !IsInteger(index)) {
		Unref(array, index);
		return interp_error("Array()");
	}
	if (index->a.i.v < 0 || array->a.a.size <= index->a.i.v) {
		Unref(array, index);
		return interp_error("Array()");
	}
	Unref(val);
	val = Ref(array->a.a.v[index->a.i.v]);
	Unref(array, index);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalMatrix() {
	AGmNode *j = Ref(val);
	if (val_stack == NULL) {
		Unref(j); return interp_error("Matrix()");
	}
	AGmNode *i = Ref(val_stack->H);
	Pop(val_stack);
	if (val_stack == NULL) {
		Unref(i, j); return interp_error("Matrix()");
	}
	AGmNode *matrix = Ref(val_stack->H);
	Pop(val_stack);
	if (!IsMatrix(matrix) || !IsInteger(i) || !IsInteger(j)) {
		Unref(matrix, i, j);
		return interp_error("Matrix()");
	}
	if (i->a.i.v < 0 || matrix->a.m.v->NumRow() <= i->a.i.v ||
		j->a.i.v < 0 || matrix->a.m.v->NumColumn() <= j->a.i.v) {
		Unref(matrix, i, j);
		return interp_error("Matrix()");
	}
	Unref(val);
	val = Ref(Real(matrix->a.m.v->Component((long)i->a.i.v, (long)j->a.i.v)));
	if (val == NULL) {
		Unref(matrix, i, j);
		return interp_error("Matrix()");
	}
	Unref(matrix, i, j);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalCast() {
	AGmNode *n, *n2, *n3; int count = 0;
	AGmFile *f; AGmString *ss;
	char s[128];
	switch (prog->a.i.t->HI) {
	case AIP_TYPE_INTEGER:
		if (val) switch (val->type) {
		case AGM_NODE_REAL:
			n = val;
			n2 = BigInteger::ToBigInt(n);
			n3 = BigInteger::ToInteger(n2);
			if (n3) {
				val = Ref(n3); RefUnref(n2);
			} else {
				val = Ref(n2);
			}
			Unref(n);
			break;
		case AGM_NODE_STRING:
			n = val;
			f = new AGmFile();
			f->Create(n->a.s.v, AGM_FILE_STR_READ);
			if (f->_MatchString("0x")) {
#if	_WINDOWS && _64BIT
				long long i = 0;
#else
				long i = 0;
#endif
				f->ReadHexa(i);
				val = Ref(Integer(i));
			} else {
				AGmNode *i = NULL;
				f->ReadInteger(i);
				val = Ref(i);
			}
			delete f;
			Unref(n);
			break;
		default:
			break;
		}
		break;
	case AIP_TYPE_REAL:
		if (val) switch (val->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_BIGINT:
			n = val;
			val = Ref(BigInteger::ToReal(n));
			Unref(n);
			break;
		case AGM_NODE_STRING:
			n = val;
			val = Ref(Real(atof(n->a.s.v->Str())));
			Unref(n);
			break;
		default:
			break;
		}
		break;
	case AIP_TYPE_STRING:
		if (val) switch (val->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_BIGINT:
		case AGM_NODE_GFP:
			n = val;
			val = Ref(BigInteger::ToString(n));
			Unref(n);	
			break;
		case AGM_NODE_REAL:
			n = val;
			art_sprintf_1(s, 128, "%.16g", n->a.r.v);
			val = Ref(String(s));
			Unref(n);	
			break;
		case AGM_NODE_LIST:
			n = val;
			ss = new AGmString(32);
			f = new AGmFile();
			f->Create(ss, AGM_FILE_STR_WRITE);
			f->WriteList(val, count, 100);
			delete f;
			val = Ref(String(ss));
			Unref(n);
			break;
		default:
			break;
		} else {
			val = Ref(String("null"));
		}
		break;
	default:
		break;
	}
	prog = prog->a.i.t->T;
	return 0;
}
int AIpProcess::EvalRuntime() {
	AGmNode *class_id;
	class_id = dst_node2(prog, NmClass());
	ASSERT(class_id);
	Unref(val); val = Ref(class_id);
	prog = prog->a.i.t;
	return 0;
}

// write
int AIpProcess::EvalSetLoc() {
	AGmNode *name, **value;
	name = prog->a.i.t->H;
	AGmNode *ls;
	for (ls = loc_base ; ls; ls = ls->T) {
		if (SearchVar(ls->H, name, value) == 0) {
			Set(*value, val);
			prog = prog->a.i.t->T;
			return 0;	
		}
	}
	char err[256];
	art_sprintf_1(err, 256, "SetLoc() %s", name->a.s.v->Str());
	return interp_error(err);
}
int AIpProcess::EvalSetArg() {
	AGmNode *name, **value;
	name = prog->a.i.t->H;
	if (SearchVar(arg_base, name, value) == 0) {
		Set(*value, val);
		prog = prog->a.i.t->T;
		return 0;
	}	
	return interp_error("SetArg()");
}
int AIpProcess::EvalSetThisVar() {
	AGmNode *name = prog->a.i.t->H;
	if (this_ptr == NULL) return interp_error("ThisVar()");
	set_dst_node2(this_ptr, name, val);
	prog = prog->a.i.t->T;
	return 0;	
}
int AIpProcess::EvalSetClassVar() {
	AGmNode *member = dst_node2(prog, NmMember());
	ASSERT(member);
	set_dst_node2(member, NULL, val);
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalSetObjVar() {
	AGmNode *name = prog->a.i.t->H;
	if (val_stack == NULL) return interp_error("ObjVar()");
	AGmNode *obj = Ref(val_stack->H);
	Pop(val_stack);
	if (obj == NULL) return interp_error("ObjVar()");
	set_dst_node2(obj, name, val);
	Unref(obj);
	prog = prog->a.i.t->T;
	return 0;	
}
int AIpProcess::EvalSetHead() {
	if (val_stack == NULL) return interp_error("SetHead()");
	AGmNode *list = Ref(val_stack->H);
	Pop(val_stack);
	if (list == NULL || !IsList(list)) {
		Unref(list);
		return interp_error("SetHead()");
	}
	Set(list->H, val);
	Unref(list);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalSetTail() {
	if (val_stack == NULL) return interp_error("SetTail()");
	AGmNode *list = Ref(val_stack->H);
	Pop(val_stack);
	if (list == NULL || !IsList(list)) {
		Unref(list);
		return interp_error("SetTail()");
	}
	Set(list->T, val);
	Unref(list);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalSetArray() {
	if (val_stack == NULL) interp_error("SetArray()");
	AGmNode *index = Ref(val_stack->H);
	Pop(val_stack);
	if (val_stack == NULL) {
		Unref(index); return interp_error("SetArray()");	
	}
	AGmNode *array = Ref(val_stack->H);
	Pop(val_stack);
	if (!IsArray(array) || !IsInteger(index)) {
		Unref(array, index);
		return interp_error("SetArray()");
	}
	if (index->a.i.v < 0 || array->a.a.size <= index->a.i.v) {
		Unref(array, index);
		interp_error("SetArray()");
	}
	Set(array->a.a.v[index->a.i.v], val);
	Unref(array, index);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalSetMatrix() {
	if (val_stack == NULL) return interp_error("SetMatrix()");
	AGmNode *j = Ref(val_stack->H);
	Pop(val_stack);
	if (val_stack == NULL) {
		Unref(j); return interp_error("SetMatrix()");
	}
	AGmNode *i = Ref(val_stack->H);
	Pop(val_stack);
	if (val_stack == NULL) {
		Unref(i, j);
		return interp_error("SetMatrix()");
	}
	AGmNode *matrix = Ref(val_stack->H);
	Pop(val_stack);
	if (!IsMatrix(matrix) || !IsInteger(i) || !IsInteger(j)) {
		Unref(matrix, i, j);
		return interp_error("SetMatrix()");
	}
	if (i->a.i.v < 0 || matrix->a.m.v->NumRow() <= i->a.i.v ||
		j->a.i.v < 0 || matrix->a.m.v->NumColumn() <= j->a.i.v) {
		Unref(matrix, i, j);
		return interp_error("SetMatrix()");
	}
	if (IsInteger(val)) {
		matrix->a.m.v->Component((long)i->a.i.v, (long)j->a.i.v) = (double)val->a.i.v;
	} else if (IsReal(val)) {
		matrix->a.m.v->Component((long)i->a.i.v, (long)j->a.i.v) = val->a.r.v;
	} else {
		Unref(matrix, i, j);
		return interp_error("SetMatrix()");
	}
	Unref(matrix, i, j);
	prog = prog->a.i.t;
	return 0;
}

// search variable
int AIpProcess::SearchVar(AGmNode *vars, AGmNode *name, AGmNode **&value) {
	for (; vars; vars = vars->T) {
		AGmNode *var = vars->H;
		if (var->H == name) { value = &var->TH; return 0; }
	}
	return 1;	
}

// function call
int AIpProcess::EvalPush() {
	Push(val_stack, val);
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalPop() {
	if (val_stack == NULL) return interp_error("Pop()");
	Unref(val);
	val = Ref(val_stack->H);
	Pop(val_stack);
	prog = prog->a.i.t;
	return 0;	
}
int AIpProcess::EvalClassFunc() {
	AGmNode *func = dst_node2(prog, NmMember());
	ASSERT(func);
	AGmNode *arg_dcl = func->TTTH;

	AGmNode *l, *vars = NULL;
	for (l = arg_dcl; l; l = l->TT) {
		vars = Cons(List(l->H, val, l->TH), vars);		
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}

	Push(func_stack,
		List(this_ptr, prog->a.i.t, arg_base, loc_base, ctrl_stack, func));

	this_ptr = NULL;
	prog = func->TTH;
	arg_base = vars;
	loc_base = ctrl_stack = NULL;
	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

#ifdef	_DEBUG_STACK_TRACE
	TRACE0("\nEval\n");
	StackTrace();
#endif
	return 0;
}
int AIpProcess::EvalThisFunc() {
	AGmNode *func_dcl = dst_node2(prog, NmVtbl());
	ASSERT(func_dcl);
	AGmNode *arg_dcl = func_dcl->TTTH;
	AGmNode *vtbl = dst_node2(func_dcl, NmVtbl());
	ASSERT(vtbl);

	AGmNode *l, *vars = NULL;
	for (l = arg_dcl; l; l = l->TT) {
		vars = Cons(List(l->H, val, l->TH), vars);		
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}

	ASSERT(this_ptr);
	if (!IsObject(this_ptr)) {
		RefUnref(vars);
		return interp_error("ThisFunc()");
	}
	AGmNode* class_id = this_ptr->a.o.class_id;
	AGmNode *func = dst_node2(vtbl, class_id);
	if (func == NULL) {
		RefUnref(vars);
		char msg[256];
		art_sprintf_1(msg, 256, "vtbl error: class = %s,", class_id->H->a.s.v->Str());
		throw new AlException(VTBL_ERROR, msg);
	}

	Push(func_stack,
		List(this_ptr, prog->a.i.t, arg_base, loc_base, ctrl_stack, func));

	prog = func->TTH;
	arg_base = vars;
	loc_base = ctrl_stack = NULL;
	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

#ifdef	_DEBUG_STACK_TRACE
	TRACE0("\nEval\n");
	StackTrace();
#endif
	return 0;
}
int AIpProcess::EvalObjFunc() {
	AGmNode *func_dcl = dst_node2(prog, NmVtbl());
	ASSERT(func_dcl);
	AGmNode *arg_dcl = func_dcl->TTTH;
	AGmNode *vtbl = dst_node2(func_dcl, NmVtbl());
	ASSERT(vtbl);

	AGmNode *l, *vars = NULL;
	for (l = arg_dcl; l; l = l->TT) {
		vars = Cons(List(l->H, val, l->TH), vars);		
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}

	AGmNode *obj = val;

	if (!IsObject(obj)) {
		RefUnref(vars);
		char s[100];
		AGmLink *l;
		for (l = next_inlink(func_dcl, NULL);
				l; l = next_inlink(func_dcl, l)) {
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) break;
		}
		ASSERT(l);
		AGmArc *a = l->arc;
		art_sprintf_1(s, 100, "ObjFunc() %s", a->attr->a.s.v->Str());
		return interp_error(s);
	}
	AGmNode* class_id = obj->a.o.class_id;
	AGmNode *func = dst_node2(vtbl, class_id);
	if (func == NULL) {
		RefUnref(vars);
		char msg[256];
		art_sprintf_1(msg, 256, "vtbl error: class = %s,", class_id->H->a.s.v->Str());
		throw new AlException(VTBL_ERROR, msg);
	}

	Push(func_stack,
		List(this_ptr, prog->a.i.t, arg_base, loc_base, ctrl_stack, func));

	this_ptr = obj;
	prog = func->TTH;
	arg_base = vars;
	loc_base = ctrl_stack = NULL;
	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

#ifdef	_DEBUG_STACK_TRACE
	TRACE0("\nEval\n");
	StackTrace();
#endif
	return 0;
}
int AIpProcess::EvalObjFunc2() {
	AGmNode *func_dcl = dst_node2(prog, NmVtbl());
	ASSERT(func_dcl);
	AGmNode *arg_dcl = func_dcl->TTTH;

	AGmNode *l, *vars = NULL;
	for (l = arg_dcl; l; l = l->TT) {
		vars = Cons(List(l->H, val, l->TH), vars);		
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}

	AGmNode *func = func_dcl;

	Push(func_stack,
		List(this_ptr, prog->a.i.t, arg_base, loc_base, ctrl_stack, func));

	prog = func->TTH;
	arg_base = vars;
	loc_base = ctrl_stack = NULL;
	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

#ifdef	_DEBUG_STACK_TRACE
	TRACE0("\nEval\n");
	StackTrace();
#endif
	return 0;
}
int AIpProcess::EvalClassFuncP() {
	AGmNode *func = val;
	if (func == NULL) {
		return interp_error(AL_S202);
	}
	AGmNode *arg_dcl = func->TTTH;

	ASSERT(val_stack);
	Set(val, val_stack->H);
	Pop(val_stack);

	AGmNode *l, *vars = NULL;
	for (l = arg_dcl; l; l = l->TT) {
		vars = Cons(List(l->H, val, l->TH), vars);		
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}

	Push(func_stack,
		List(this_ptr, prog->a.i.t, arg_base, loc_base, ctrl_stack, func));

	this_ptr = NULL;
	prog = func->TTH;
	arg_base = vars;
	loc_base = ctrl_stack = NULL;
	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

#ifdef	_DEBUG_STACK_TRACE
	TRACE0("\nEval\n");
	StackTrace();
#endif
	return 0;
}
int AIpProcess::EvalThisFuncP() {
	AGmNode *func_dcl = val;
	ASSERT(func_dcl);
	AGmNode *arg_dcl = func_dcl->TTTH;
	AGmNode *vtbl = dst_node2(func_dcl, NmVtbl());
	ASSERT(vtbl);

	ASSERT(val_stack);
	Set(val, val_stack->H);
	Pop(val_stack);

	AGmNode *l, *vars = NULL;
	for (l = arg_dcl; l; l = l->TT) {
		vars = Cons(List(l->H, val, l->TH), vars);		
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}

	ASSERT(this_ptr);
	if (!IsObject(this_ptr)) {
		RefUnref(vars);
		return interp_error("ThisFuncP()");
	}
	AGmNode* class_id = this_ptr->a.o.class_id;
	AGmNode *func = dst_node2(vtbl, class_id);
	if (func == NULL) {
		RefUnref(vars);
		char msg[256];
		art_sprintf_1(msg, 256, "vtbl error: class = %s,", class_id->H->a.s.v->Str());
		throw new AlException(VTBL_ERROR, msg);
	}

	Push(func_stack,
		List(this_ptr, prog->a.i.t, arg_base, loc_base, ctrl_stack, func));

	prog = func->TTH;
	arg_base = vars;
	loc_base = ctrl_stack = NULL;
	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

#ifdef	_DEBUG_STACK_TRACE
	TRACE0("\nEval\n");
	StackTrace();
#endif
	return 0;
}
int AIpProcess::EvalObjFuncP() {
	AGmNode *func_dcl = val;
	ASSERT(func_dcl);
	AGmNode *arg_dcl = func_dcl->TTTH;
	AGmNode *vtbl = dst_node2(func_dcl, NmVtbl());
	ASSERT(vtbl);

	ASSERT(val_stack);
	Set(val, val_stack->H);
	Pop(val_stack);

	AGmNode *l, *vars = NULL;
	for (l = arg_dcl; l; l = l->TT) {
		vars = Cons(List(l->H, val, l->TH), vars);		
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}

	AGmNode *obj = val;

	if (!IsObject(obj)) {
		RefUnref(vars);
		return interp_error("ObjFuncP()");
	}
	AGmNode* class_id = obj->a.o.class_id;
	AGmNode *func = dst_node2(vtbl, class_id);
	if (func == NULL) {
		RefUnref(vars);
		char msg[256];
		art_sprintf_1(msg, 256, "vtbl error: class = %s,", class_id->H->a.s.v->Str());
		throw new AlException(VTBL_ERROR, msg);
	}

	Push(func_stack,
		List(this_ptr, prog->a.i.t, arg_base, loc_base, ctrl_stack, func));

	this_ptr = obj;
	prog = func->TTH;
	arg_base = vars;
	loc_base = ctrl_stack = NULL;
	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

#ifdef	_DEBUG_STACK_TRACE
	TRACE0("\nEval\n");
	StackTrace();
#endif
	return 0;
}
int AIpProcess::EvalRet() {
	if (func_stack == NULL) {
		return AIP_EXIT;
	}

	AGmNode *stack_data = func_stack->H;

	AGmNode *func = stack_data->TTTTTH;
	AGmLeaveProf(func_stack, func);

	Pop(this_ptr_ls); this_ptr = stack_data->H;
	prog = stack_data->TH;
	Pop(arg_base_ls); arg_base = stack_data->TTH;
	Pop(loc_base_ls); loc_base = stack_data->TTTH;
	Pop(ctrl_stack_ls); ctrl_stack = stack_data->TTTTH;
	Pop(func_stack);


#ifdef	_DEBUG_STACK_TRACE
	TRACE0("\nRet\n");
	StackTrace();
#endif
	return 0;	
}

AGmNode* AIpProcess::CallListener(AGmNode *obj, AGmNode *func_dcl, AGmNode *args) {
	ASSERT(obj);
	if (func_dcl == NULL) return 0;
	AGmNode *arg_dcl = func_dcl->TTTH;
	AGmNode *vtbl = dst_node2(func_dcl, NmVtbl());
	ASSERT(vtbl);

	Unref(val); val = NULL;
	AGmNode *l, *l2, *vars = NULL;
	for (l = arg_dcl, l2 = args; l && l2; l = l->TT, l2 = l2->T) {
		vars = Cons(List(l->H, l2->H, l->TH), vars);		
	}

	if (!IsObject(obj)) {
		RefUnref(vars); exception = 1;
		CreateArc(val = Ref(Cons(NULL)), String("tartget is not object."), String("msg"));
		return val;
	}
	AGmNode* class_id = obj->a.o.class_id;
	AGmNode *func = dst_node2(vtbl, class_id);
	if (func == NULL) {
		RefUnref(vars); exception = 1;
		CreateArc(val = Ref(Cons(NULL)), String("function not found."), String("msg"));
	}

	Push(func_stack_ls, func_stack); func_stack = NULL;

	Push(func_stack,
		List(this_ptr, prog, arg_base, loc_base, ctrl_stack, func));

	this_ptr = obj;
	prog = func->TTH;
	arg_base = vars;
	loc_base = ctrl_stack = NULL;
	Push(this_ptr_ls, this_ptr);
	Push(arg_base_ls, arg_base);
	Push(loc_base_ls, loc_base);
	Push(ctrl_stack_ls, ctrl_stack);

	AGmEnterProf(func_stack, func);

	while (Execute() == 0 && func_stack) ;

	AGmLeaveProf(func_stack, func);

	func_stack = func_stack_ls->H;
	Pop(func_stack_ls);

	return val;
}

int AIpProcess::interp_error(const char *s) {
	Unref(val); val = NULL;
	AGmNode *class_node = AUiClassShell::NameToClass(RootClass(), UniqueStr("AlException"));
	val = Ref(Object(class_node->H, class_node));
	AGmString *str = new AGmString("runtime: ");
	str->Append(s);
	CreateArc(val, String(str), UniqueStr("msg"));
	if (func_stack) CreateArc(val, func_stack, UniqueStr("stack_frame"));
	if (prog) CreateArc(val, prog, UniqueStr("pos"));
	return EvalThrow();
}
void AIpProcess::StackTrace() {
	StackTrace(func_stack);
}
void AIpProcess::StackTrace(AGmNode *func_stack) {
	AGmString *str = new AGmString(256);
	AGmFile *f = new AGmFile();
	f->Create(str, AGM_FILE_STR_WRITE);
	StackTrace(func_stack, f);
	ADvWindow::Print(str->Str());
	delete f; delete str;
}
void AIpProcess::StackTrace(AGmNode *func_stack, AGmFile *fi) {
	AGmNode *f;
	for (f = func_stack; f; f = f->T) {
		AGmNode *func = f->H->TTTTTH;
		AGmLink *l;
		for (l = next_inlink(func, NULL);
				l; l = next_inlink(func, l)) {
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) break;
		}
		ASSERT(l);
		AGmArc *a = l->arc;
#ifdef	_DEBUG_STACK_TRACE
		TRACE2("\t%s::%s\n", a->src->H->a.s.v->Str(), a->attr->a.s.v->Str());
#else
		char s[256];
		art_sprintf_2(s, 256, "\t%s::%s\n", a->src->H->a.s.v->Str(), a->attr->a.s.v->Str());
		fi->WriteString(s);
#endif
	}
#ifdef	_DEBUG_STACK_TRACE
	TRACE0("====================\n");
#endif
}
void AIpProcess::ErrorSource() {
	ErrorSource(func_stack, prog);
}
void AIpProcess::ErrorSource(AGmNode *func_stack, AGmNode *prog) {
	AGmString *str = new AGmString(256);
	AGmFile *f = new AGmFile();
	f->Create(str, AGM_FILE_STR_WRITE);
	ErrorSource(func_stack, prog, f);
	ADvWindow::Print(str->Str());
	delete f; delete str;
}
void AIpProcess::ErrorSource(AGmNode *func_stack, AGmNode *prog, AGmFile *f) {
	if (func_stack == NULL) return;
	AGmNode *func = func_stack->H->TTTTTH;
	AGmLink *l;
	for (l = next_inlink(func, NULL);
			l; l = next_inlink(func, l)) {
		if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) break;
	}
	ASSERT(l);
	AUiBodyShell *bs = new AUiBodyShell((ADvWindow*)NULL, RootClass(), l->arc->src, l->arc->dst);
	bs->Reset();
	AGmNode **p = bs->Prog();
	bs->Next();
	AGpProgIF *pif = new AGpProgIF();
	for (pif->Prog(p); pif->More(); pif->Next() ) {
		if (*pif->Prog() == prog)	{
			break;
		}
		if (pif->Prog() == bs->Prog()) {
			p = bs->Prog();
			bs->Next();
		}
	}
	delete pif;
	for (bs->Reset(); bs->Prog() != p; bs->Next()) ;
	for (int i = 0; i < 12 && bs->More(); i++, bs->Next()) {
		AGmString *s = bs->GetLineString();
		ASSERT(s);
		f->WriteString(s->Str());
		f->WriteString("\n");
	}
	delete bs;
}
#ifdef	_DEBUG_INTERP2
void AIpProcess::_BaseFuncName(long func_no) {
	switch (func_no) {
	case	AIP_EvalBlock:			TRACE0("Block\n");		break;
	case	AIP_EvalLocVarDcl:		TRACE0("LocVarDcl\n");	break;
	case	AIP_EvalBranch: 		TRACE0("Branch\n"); 	break;
	case	AIP_EvalLoop:			TRACE0("Loop\n");		break;
	case	AIP_EvalBreak:			TRACE0("Break\n");		break;
	case	AIP_EvalCont:			TRACE0("Cont\n");		break;
	case	AIP_EvalSemicolon:		TRACE0("SemiColon\n");	break;
	case	AIP_EvalComment:		TRACE0("Comment\n");	break;
	case	AIP_EvalCond:			TRACE0("Cond\n");		break;
	case	AIP_EvalOrOr:			TRACE0("OrOr\n");		break;
	case	AIP_EvalAndAnd: 		TRACE0("AndAnd\n"); 	break;
	case	AIP_EvalEQ: 			TRACE0("EQ\n"); 		break;
	case	AIP_EvalNE: 			TRACE0("NE\n"); 		break;
	case	AIP_EvalLT: 			TRACE0("LT\n"); 		break;
	case	AIP_EvalLE: 			TRACE0("LE\n"); 		break;
	case	AIP_EvalGE: 			TRACE0("GE\n"); 		break;
	case	AIP_EvalGT: 			TRACE0("GT\n"); 		break;
	case	AIP_EvalAdd:			TRACE0("Add\n");		break;	
	case	AIP_EvalSub:			TRACE0("Sub\n");		break;	
	case	AIP_EvalMul:			TRACE0("Mul\n");		break;	
	case	AIP_EvalDiv:			TRACE0("Div\n");		break;	
	case	AIP_EvalMod:			TRACE0("Mod\n");		break;	
	case	AIP_EvalAnd:			TRACE0("And\n");		break;	
	case	AIP_EvalOr: 			TRACE0("Or\n"); 		break;
	case	AIP_EvalXor:			TRACE0("Xor\n");		break;	
	case	AIP_EvalSL: 			TRACE0("SL\n"); 		break;
	case	AIP_EvalSR: 			TRACE0("SR\n"); 		break;
	case	AIP_EvalChgSign:		TRACE0("ChgSign\n");	break;		
	case	AIP_EvalPreInc: 		TRACE0("PreInc\n"); 	break;
	case	AIP_EvalPostInc:		TRACE0("PostInc\n");	break;	
	case	AIP_EvalPreDec: 		TRACE0("PreDec\n"); 	break;
	case	AIP_EvalPostDec:		TRACE0("PostDec\n");	break;		
	case	AIP_EvalInverse:		TRACE0("Inverse\n");	break;	
	case	AIP_EvalNot:			TRACE0("Not\n");		break;			
	case	AIP_EvalConstInt:		TRACE0("ConstInt\n");	break;
	case	AIP_EvalConstHexa:		TRACE0("ConstHexa\n");	break;
	case	AIP_EvalConstQChar: 	TRACE0("ConstQChar\n"); break;	
	case	AIP_EvalConstReal:		TRACE0("ConstReal\n");	break;
	case	AIP_EvalConstString:	TRACE0("ConstString\n");break;	
	case	AIP_EvalConstVtbl:		TRACE0("ConstVtbl\n");	break;
	case	AIP_EvalConstFunc:		TRACE0("ConstFunc\n");	break;
	case	AIP_EvalConstNull:		TRACE0("ConstNull\n");	break;
	case	AIP_EvalNew:			TRACE0("New\n");		break;
	case	AIP_EvalThis:			TRACE0("This\n");		break;

	case	AIP_EvalLoc:			TRACE0("Loc\n");			break;
	case	AIP_EvalArg:			TRACE0("Arg\n");			break;
	case	AIP_EvalThisVar:		TRACE0("ThisVar\n");		break;
	case	AIP_EvalClassVar:		TRACE0("ClassVar\n");	break;
	case	AIP_EvalObjVar: 		TRACE0("ObjVar\n"); 	break;
	case	AIP_EvalHead:			TRACE0("Head\n");		break;
	case	AIP_EvalTail:			TRACE0("Tail\n");		break;
	case	AIP_EvalArray:			TRACE0("Array\n");		break;
	case	AIP_EvalMatrix: 		TRACE0("Matrix\n"); 	break;
	case	AIP_EvalSetLoc: 		TRACE0("SetLoc\n"); 	break;
	case	AIP_EvalSetArg: 		TRACE0("SetArg\n"); 	break;
	case	AIP_EvalSetThisVar: 	TRACE0("SetThisVar\n"); break;
	case	AIP_EvalSetClassVar:	TRACE0("SetClassVar\n");	break;
	case	AIP_EvalSetObjVar:		TRACE0("SetObjVar\n");	break;
	case	AIP_EvalSetHead:		TRACE0("SetHead\n");		break;
	case	AIP_EvalSetTail:		TRACE0("SetTail\n");		break;
	case	AIP_EvalSetArray:		TRACE0("SetArray\n");	break;
	case	AIP_EvalSetMatrix:		TRACE0("SetMatrix\n");	break;
	case	AIP_EvalPush:			TRACE0("Push\n");		break;		
	case	AIP_EvalPop:			TRACE0("Pop\n");		break;
	case	AIP_EvalClassFunc:		TRACE0("ClassFunc\n");	break;
	case	AIP_EvalThisFunc:		TRACE0("ThisFunc\n");	break;
	case	AIP_EvalObjFunc:		TRACE0("ObjFunc\n");	break;
	case	AIP_EvalClassFuncP: 	TRACE0("ClassFuncP\n"); break;
	case	AIP_EvalThisFuncP:		TRACE0("ThisFuncP\n");	break;
	case	AIP_EvalObjFuncP:		TRACE0("ObjFuncP\n");	break;
	case	AIP_EvalRet:			TRACE0("Ret\n");		break;
	case	AIP_EvalSysFunc:		TRACE0("SysFunc\n");	break;
	}
}
#else
void AIpProcess::_BaseFuncName(long) {
}
#endif

// for garbage collection
void AIpProcess::GCmark() {
	_GCmark(_root_class);

	_GCmark(val);
	_GCmark(val_stack);
	_GCmark(this_ptr);
	_GCmark(prog);
	_GCmark(arg_base);
	_GCmark(loc_base);
	_GCmark(ctrl_stack);
	_GCmark(func_stack);

	_GCmark(this_ptr_ls);
	_GCmark(arg_base_ls);
	_GCmark(loc_base_ls);
	_GCmark(ctrl_stack_ls);
	_GCmark(func_stack_ls);

	_GCmark(notify_cells);
}

int AIpEvalClassFunc(AGmNode *root_class,
		AGmNode *class_name, AGmNode *func_name, AGmNode *args) {
	AGmNode *_class = AUiClassShell::NameToClass(
		root_class, class_name);
	if (_class == NULL)
		return 1;
	int protect;
	AGmNode *func_dcl = AUiBodyShell::find_class_func(
		_class, _class, func_name, NULL, protect, 1, 1);
	if (func_dcl == NULL || protect)
		return 2;
	AGm::Unref(EvalCallback(root_class, args, NULL, func_dcl));
	return 0;
}


