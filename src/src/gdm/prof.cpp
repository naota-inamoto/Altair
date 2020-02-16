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
#include <gdm/pict.h>
#include <dev/advcapp.h>
#include <dev/shell.h>
#include <interp/interp.h>
#include <cui/class.h>
#include <cui/member.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

void AGmForEachClass1(
		AGmNode *class_node, ReflectFunc func, AGmNode *data1, AGmNode *data2) {
	(*func)(class_node, data1, data2);
	AGmLink *l;
	for (l = AGm::next_outlink(class_node, NULL, AGm::NmSubClass());
			l; l = AGm::next_outlink(class_node, l, AGm::NmSubClass())) {
		AGmForEachClass1(l->arc->dst, func, data1, data2);
	}
}
void AGmForEachClass(
		AGmNode *root_class, ReflectFunc func, AGmNode *data1, AGmNode *data2) {
	AGmNode *alobj = AUiClassShell::NameToClass(root_class, "AlObject");
	AGmForEachClass1(alobj, func, data1, data2);
}
void AGmForEachMember(
		AGmNode *class_node, ReflectFunc func, AGmNode *data1, AGmNode *data2) {
	AGmLink *l;
	for (l = AGm::next_outlink(class_node, NULL);
			l; l = AGm::next_outlink(class_node, l)) {
				if (l->arc->attr == AGm::NmSubClass()) continue;
		(*func)(l->arc->dst, data1, data2);
	}
}
int AGmIsStatic(AGmNode *func_dcl) {
	return ((func_dcl->HI & AUI_STATIC) != 0);
}
int AGmIsFunc(AGmNode *func_dcl) {
	return ((func_dcl->HI & AUI_FUNC) != 0);
}
int AGmFuncDclToClassAndFunc(AGmNode *func_dcl, AGmNode*& class_node, AGmArc*& func_arc) {
	AGmLink *l;
	for (l = AGm::next_inlink(func_dcl, NULL);
			l; l = AGm::next_inlink(func_dcl, l)) {
		if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) break;
	}
	if (l == NULL) return 1;
	class_node = l->arc->src;
	func_arc = l->arc;
	return 0;
}

static void clearFuncProf(AGmNode *func_dcl, AGmNode* name, AGmNode*) {
	AGmNode *prof_root = AGm::dst_node2(AGm::RootGlobal(), AGm::NmProf());
	if (prof_root) {
		AGm::set_dst_node(prof_root, name, NULL);
		if (AGm::dst_node(prof_root) == NULL)
			AGm::set_dst_node(AGm::RootGlobal(), AGm::NmProf(), NULL);
	}

	if (!AGmIsFunc(func_dcl)) return;
	AGmNode *prof = AGm::dst_node2(func_dcl, AGm::NmProf());
	if (prof == NULL) return;
	AGm::set_dst_node(prof, name, NULL);
}
static void setFuncProf(AGmNode *func_dcl, AGmNode* name, AGmNode*) {
	if (!AGmIsFunc(func_dcl)) return;
	AGmNode *prof = AGm::dst_node2(func_dcl, AGm::NmProf());
	if (prof == NULL) {
		AGm::CreateArc(func_dcl, prof = AGm::Cons(NULL), AGm::NmProf());
	}
	AGm::set_dst_node(prof, name, AGm::Cons(NULL));

	AGmNode *prof_root = AGm::dst_node2(AGm::RootGlobal(), AGm::NmProf());
	if (prof_root == NULL) {
		AGm::CreateArc(AGm::RootGlobal(), prof_root = AGm::Cons(NULL), AGm::NmProf());
	}
	AGm::set_dst_node(prof_root, name, AGm::Cons(NULL));
}
static void clearClassProf(AGmNode *class_node, AGmNode* name, AGmNode*) {
	AGmForEachMember(class_node, clearFuncProf, name, NULL);
}
static void setClassProf(AGmNode *class_node, AGmNode* name, AGmNode*) {
	AGmForEachMember(class_node, setFuncProf, name, NULL);
}
static void setSpecifiedFuncProf(
		AGmNode *class_node, AGmNode *name, AGmNode *func_name) {
	AGmLink *l;
	for (l = AGm::next_outlink(class_node, NULL);
			l; l = AGm::next_outlink(class_node, l)) {
		AGmNode *attr = l->arc->attr;
		if (!AGm::IsString(attr)) continue;
		if (attr == AGm::NmSubClass()) continue;
		if (attr->a.s.v->Compare(func_name->a.s.v->Str()) != 0) continue;
		AGmNode *func_dcl = l->arc->dst;
		setFuncProf(func_dcl, name, NULL);
	}
}
static void resultFuncProf(AGmNode *func_dcl, AGmNode* name, AGmNode* res) {
	if (!AGmIsFunc(func_dcl)) return;

	AGmNode *class_node; AGmArc *func_arc;
	if (AGmFuncDclToClassAndFunc(func_dcl, class_node, func_arc)) return;

	AGmNode *prof = AGm::dst_node2(func_dcl, AGm::NmProf());
	if (prof == NULL) return;
	prof = AGm::dst_node(prof, name);
	if (prof == NULL) return;
	AGmNode *count = AGm::dst_node(prof, "$count");
	AGmNode *total = AGm::dst_node(prof, "$total");
	if (count == NULL || total == NULL) return;
	AGmNode *average = AGm::Integer(total->a.i.v/count->a.i.v);

	AGmNode *n = AGm::List(class_node->H, func_arc->attr, count, total, average);
	AGm::CreateArc(res, n);
}
static void resultClassProf(AGmNode *class_node, AGmNode* name, AGmNode* res) {
	AGmForEachMember(class_node, resultFuncProf, name, res);
}

int AIpProcess::EvalProf() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd) || !IsString(arg1)) {
		Unref(cmd, arg1, arg2);
		return interp_error("EvalProf()");
	}
	const char *s = cmd->a.s.v->Str();
#if	_WINDOWS && _64BIT
	long long opt = 0;
#else
	long opt = 0;
#endif
	char *filename = NULL; AGmNode *tail;

	if (strcmp(s, "clear") == 0) {
		AGmForEachClass(_root_class, clearClassProf, arg1, NULL);

	} else if (strcmp(s, "set") == 0) {
		if (arg2 == NULL) {
			AGmForEachClass(_root_class, setClassProf, arg1, NULL);
		} else {
			AGmLink *l;
			for (l = next_outlink(arg2, NULL); l; l = next_outlink(arg2, l)) {
				AGmNode *class_name, *func_name, *class_node;
				AGmNode *n = l->arc->dst, *t;
				if (IsString(n)) {
					class_name = n;
					class_node = AUiClassShell::NameToClass(_root_class, class_name);
					if (class_node) {
						AGmForEachMember(class_node, setFuncProf, arg1, NULL);
					}
				} else if (Param(n, class_name, t) == 0
						&& Param(t, func_name, t) == 0) {
					class_node = AUiClassShell::NameToClass(_root_class, class_name);
					if (class_node) {
						setSpecifiedFuncProf(class_node, arg1, func_name);
					}
				}
			}
		}
	
	} else if (strcmp(s, "start") == 0) {
		if (Param(arg2, opt, tail) == 0 && Param(tail, filename, tail) == 0) {
			AGmNode *prof_root = AGm::dst_node2(AGm::RootGlobal(), NmProf());
			if (prof_root) {
				AGmNode *prof = AGm::dst_node(prof_root, arg1);
				if (prof && prof->H == NULL) {
					AGmNode *n = List(Integer(opt), NULL);
					if ((opt & 0x02) && filename) {
						AGmFile *f = new AGmFile();
						if (f->Create(filename, AGM_FILE_APPEND) == 0) {
							Set(n->TH, File(f));
						} else {
							delete f;
						}
					}
					Set(prof->H, n);
				}
			}
		}

	} else if (strcmp(s, "stop") == 0) {
		AGmNode *prof_root = AGm::dst_node2(AGm::RootGlobal(), NmProf());
		if (prof_root) {
			AGmNode *prof = AGm::dst_node(prof_root, arg1);
			if (prof && prof->H) Set(prof->H, NULL);
		}

	} else if (strcmp(s, "result") == 0) {
		if (arg2 == NULL || IsInteger(arg2)) {
			AGmNode *res = Cons(NULL);
			AGmForEachClass(_root_class, resultClassProf, arg1, res);
			if (arg2) {
				AGmNode *root = NULL;
				AGmLink *l;
				for (l = next_outlink(res, NULL);
						l; l = next_outlink(res, l)) {
					QuickSort(root, l->arc->dst, (int)arg2->a.i.v);
				}
				RefUnref(res); res = Cons(NULL);
				QuickSortResult(res, root, 1);
				Unref(root);
			}
			Unref(val); val = Ref(res);
		}
	} else {
		Unref(cmd, arg1, arg2);
		return interp_error("EvalProf()");
	}
	Unref(cmd, arg1, arg2);
	return 0;
}

void AGmEnterProf(AGmNode *func_stack, AGmNode *func_dcl) {
	AGmNode *prof_root = AGm::dst_node2(AGm::RootGlobal(), AGm::NmProf());
	AGmNode *prof = AGm::dst_node2(func_dcl, AGm::NmProf());
	if (prof_root && prof) {
		AGmLink *l;
		for (l = AGm::next_outlink(prof_root, NULL);
				l; l = AGm::next_outlink(prof_root, l)) {
			AGmNode *name = l->arc->attr;
			AGmNode *n = AGm::dst_node(prof, name);
			if (n == NULL) continue;
			if (l->arc->dst->H == NULL) continue;
			AGmNode *opt = l->arc->dst->HH;
			AGmNode *file = l->arc->dst->HTH;
			if (opt->a.i.v & 0x01) {

				struct timeb buf;
				ftime(&buf);
				long start_sec = (long)buf.time;
				long start_msec = (long)buf.millitm;

				AGm::CreateArc(n,
					AGm::List(AGm::Integer(start_sec), AGm::Integer(start_msec)),
					func_stack);
			}
			if ((opt->a.i.v & 0x02) && file) {
				long year, month, day, hour, minute, second, wday;
				get_localtime(get_time(), year, month, day, hour, minute, second, wday);
				AGmNode *class_node; AGmArc *func_arc;
				if (AGmFuncDclToClassAndFunc(func_dcl, class_node, func_arc) == 0) {
					char s[256];
					art_sprintf_8(s, 256, "%.4ld/%.2ld/%.2ld %.2ld:%.2ld:%.2ld Enter %s::%s\n",
						year, month, day, hour, minute, second,
						class_node->H->a.s.v->Str(), func_arc->attr->a.s.v->Str());
					file->a.f.v->WriteString(s); file->a.f.v->Flush();
				}
			}
			if ((opt->a.i.v & 0x02) && file == NULL && AGmOutputFile) {
				long year, month, day, hour, minute, second, wday;
				get_localtime(get_time(), year, month, day, hour, minute, second, wday);
				AGmNode *class_node; AGmArc *func_arc;
				if (AGmFuncDclToClassAndFunc(func_dcl, class_node, func_arc) == 0) {
					char s[256];
					art_sprintf_8(s, 256, "%.4ld/%.2ld/%.2ld %.2ld:%.2ld:%.2ld Enter %s::%s\n",
						year, month, day, hour, minute, second,
						class_node->H->a.s.v->Str(), func_arc->attr->a.s.v->Str());
					AGmOutput(s);
				}
			}
		}
	}
}
void AGmLeaveProf(AGmNode *func_stack, AGmNode *func_dcl) {
	AGmNode *prof_root = AGm::dst_node2(AGm::RootGlobal(), AGm::NmProf());
	AGmNode *prof = AGm::dst_node2(func_dcl, AGm::NmProf());
	if (prof_root && prof) {
		AGmLink *l;
		for (l = AGm::next_outlink(prof_root, NULL);
				l; l = AGm::next_outlink(prof_root, l)) {
			AGmNode *name = l->arc->attr;
			AGmNode *n = AGm::dst_node(prof, name);
			if (n == NULL) continue;
			if (l->arc->dst->H == NULL) continue;
			AGmNode *opt = l->arc->dst->HH;
			AGmNode *file = l->arc->dst->HTH;

			if (opt->a.i.v & 0x01) {
				AGmNode *n2 = AGm::dst_node2(n, func_stack);
				if (n2) {
					long start_sec = (long)n2->HI;
					long start_msec = (long)n2->THI;

					struct timeb buf;
					ftime(&buf);
					long end_sec = (long)buf.time;
					long end_msec = (long)buf.millitm;

					AGmNode *count = AGm::dst_node(n, "$count");
					if (count == NULL)
						AGm::CreateArc(n, count = AGm::Integer(0), AGm::String("$count"));
					AGmNode *total = AGm::dst_node(n, "$total");
					if (total == NULL)
						AGm::CreateArc(n, total = AGm::Integer(0), AGm::String("$total"));

					count->a.i.v += 1;
					total->a.i.v +=
						(end_sec - start_sec) * 1000 + (end_msec - start_msec);

					AGmLink *lnk = AGm::find_inlink(n, n2);
					AGm::DeleteArc(lnk->arc);
				}
			}
			if ((opt->a.i.v & 0x02) && file) {
				long year, month, day, hour, minute, second, wday;
				get_localtime(get_time(), year, month, day, hour, minute, second, wday);
				AGmNode *class_node; AGmArc *func_arc;
				if (AGmFuncDclToClassAndFunc(func_dcl, class_node, func_arc) == 0) {
					char s[256];
					art_sprintf_8(s, 256, "%.4ld/%.2ld/%.2ld %.2ld:%.2ld:%.2ld Leave %s::%s\n",
						year, month, day, hour, minute, second,
						class_node->H->a.s.v->Str(), func_arc->attr->a.s.v->Str());
					file->a.f.v->WriteString(s); file->a.f.v->Flush();
				}
			}
			if ((opt->a.i.v & 0x02) && file == NULL && AGmOutputFile) {
				long year, month, day, hour, minute, second, wday;
				get_localtime(get_time(), year, month, day, hour, minute, second, wday);
				AGmNode *class_node; AGmArc *func_arc;
				if (AGmFuncDclToClassAndFunc(func_dcl, class_node, func_arc) == 0) {
					char s[256];
					art_sprintf_8(s, 256, "%.4ld/%.2ld/%.2ld %.2ld:%.2ld:%.2ld Leave %s::%s\n",
						year, month, day, hour, minute, second,
						class_node->H->a.s.v->Str(), func_arc->attr->a.s.v->Str());
						AGmOutput(s);
				}
			}
		}
	}
}

