/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/vmkey.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
//#define LOCAL_LOOP_DEBUG
#endif

AIpProcessMgr::AIpProcessMgr() {
	_processes = Ref(Cons(NULL));
	_curr_process_link = NULL;
	SetValue(NULL, AGM_VM_KEY_PROCESS_MGR, this);
	AIpProcess::InitFuncTbl();
}
AIpProcessMgr::~AIpProcessMgr() {
	AIpProcess::ReleaseFuncTbl();
	RemoveValue(NULL, AGM_VM_KEY_PROCESS_MGR);
	AGmLink *l;
	for (l = next_outlink(_processes, NULL);
			l; l = next_outlink(_processes, l)) {
		AIpProcess *p = (AIpProcess*)l->arc->dst->a.p.ptr;
		delete p;
	}
	Unref(_processes);
}
AIpProcessMgr *AIpProcessMgr::Instance() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_PROCESS_MGR, value);
	return (AIpProcessMgr*)value;
}

AIpProcess *AIpProcessMgr::CreateProcess(AGmNode *root_class, AGmNode *arg, AGmNode *func, AGmNode *this_p) {
	AIpProcess *p = new AIpProcess(this, root_class, arg, func, this_p);
	if (_curr_process_link == NULL)
		_curr_process_link = p->Link();
	return p;
}

int AIpProcessMgr::NextProcess(AGmLink *lnk) {
	if (lnk) {
		if (_curr_process_link != lnk) {
			return 0;
		}
		_curr_process_link = NULL;
	}

	// obtain next process
	AGmLink *l = next_outlink(_processes, _curr_process_link);
	if (l == NULL)
		l = next_outlink(_processes, NULL);
	if (l)
		_curr_process_link = l;

	return 0;
}

AIpProcess *AIpProcessMgr::Process() {
	if (_curr_process_link == NULL) {
		_curr_process_link = next_outlink(_processes, NULL);
	}
	if (_curr_process_link == NULL) {
		return NULL;
	}

	AGmLink *l = _curr_process_link;
	AGmNode *idx0 = Ref(Integer(0));
	AGmNode *idx1 = Ref(Integer(1));
	for (;;) {
		// search one not in LocalLooop
		if (dst_node(l->arc->dst, idx1) || dst_node(l->arc->dst, idx0) == NULL) {
			_curr_process_link = l;
			Unref(idx0, idx1);
			return (AIpProcess*)l->arc->dst->a.p.ptr;
		}
		// obtain next process
		l = next_outlink(_processes, l);
		if (l == NULL) {
			l = next_outlink(_processes, NULL);
		}
		if (l == _curr_process_link) {
			Unref(idx0, idx1);
			return NULL;
		}
	}
}

// for garbage collection
void AIpProcessMgr::GCmark() {
	_GCmark(_processes);
	AGmLink *l;
	for (l = next_outlink(_processes, NULL);
			l; l = next_outlink(_processes, l)) {
		AIpProcess *p = (AIpProcess*)l->arc->dst->a.p.ptr;
		p->GCmark();
	}
}

void AIpProcessMgr::AddLocalLoopProcess(AIpProcess* p, int wait_notify_flag) {
	AGmLink *l;
	for (l = next_outlink(_processes, NULL);
			l; l = next_outlink(_processes, l)) {
		if (l->arc->dst->a.p.ptr == p) {
#ifdef LOCAL_LOOP_DEBUG
TRACE1("process %x enter local loop\n", p);
#endif
			AGmNode *idx0 = Ref(Integer(0));
			AGmNode *idx1 = Ref(Integer(1));
			if (wait_notify_flag == 0) {
				set_dst_node(l->arc->dst, idx0, Cons(NULL));
			} else {
				if (dst_node(l->arc->dst, idx1)) {
					set_dst_node(l->arc->dst, idx1, NULL);
				} else {
					set_dst_node(l->arc->dst, idx0, Cons(NULL));
				}
			}
			Unref(idx0, idx1);
			break;
		}
	}
}
void AIpProcessMgr::RemoveLocalLoopProcess(AIpProcess* p, int wait_notify_flag) {
	AGmLink *l;
	for (l = next_outlink(_processes, NULL);
			l; l = next_outlink(_processes, l)) {
		if (l->arc->dst->a.p.ptr == p) {
#ifdef LOCAL_LOOP_DEBUG
TRACE1("process %x leave local loop\n", p);
#endif
			AGmNode *idx0 = Ref(Integer(0));
			AGmNode *idx1 = Ref(Integer(1));
			if (wait_notify_flag == 0) {
				set_dst_node(l->arc->dst, idx0, NULL);
			} else {
				if (dst_node(l->arc->dst, idx0)) {
					set_dst_node(l->arc->dst, idx0, NULL);
				} else {
					set_dst_node(l->arc->dst, idx1, Cons(NULL));
				}
			}
			Unref(idx0, idx1);
			break;
		}
	}
}
