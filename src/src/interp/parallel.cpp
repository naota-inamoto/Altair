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
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <dev/advwnd.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

int AIpProcess::EvalParaClassFunc() {
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

	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	pm->CreateProcess(RootClass(), vars, func);
	prog = prog->a.i.t;
	return 0;
}
int AIpProcess::EvalParaObjFunc() {
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
		art_sprintf_1(s, 100, "ParaObjFunc() %s", a->attr->a.s.v->Str());
		return interp_error(s);
	}
	AGmNode* class_id = obj->a.o.class_id;
	AGmNode *func = dst_node2(vtbl, class_id);
	if (func == NULL) {
		RefUnref(vars);
		return 1;
	}

	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	pm->CreateProcess(RootClass(), vars, func, obj);
	prog = prog->a.i.t;
	return 0;
}

int AIpProcess::EvalNextProcess() {
	return AIP_NEXT;
}
int AIpProcess::EvalExitProcess() {
	return AIP_EXIT;
}

int AIpProcess::EvalWaitProcess() {
	AGmNode *wait;
	GetArg(wait);
	if (wait == NULL) {
		Unref(val); val = Ref(PtrCell(this));
		CreateArc(notify_cells, val);
	} else if (IsPtrCell(wait)) {
		AIpProcess *p = (AIpProcess*)wait->a.p.ptr;
		if (p) p->Wait();
	} else {
		Unref(wait);
		return interp_error("EvalWait()");
	}
	Unref(wait);
	return 0;
}
int AIpProcess::EvalNotifyProcess() {
	AGmNode *wait;
	GetArg(wait);
	if (IsPtrCell(wait)) {
		AIpProcess *p = (AIpProcess*)wait->a.p.ptr;
		if(p) p->Notify();
	} else {
		Unref(wait);
		return interp_error("EvalNotify()");
	}
	Unref(wait);
	return 0;
}
