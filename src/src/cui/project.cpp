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
#include <gdm/vmkey.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <dev/advcapp.h>
#include <dev/advwnd.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/cui.h>
#include <cui/project.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

AUiProjectShell::AUiProjectShell(ADvWindow *aw) : ADvEditShell(aw) {
}
AUiProjectShell::~AUiProjectShell() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_PROJ_EDITOR, value);
	if (value)
		RemoveValue(NULL, AGM_VM_KEY_PROJ_EDITOR);
}

AGmNode* AUiProjectShell::AddProject(AGmString *dir, AGmString *name,
			AGmNode *root_class, AGmNode *lib_list, AGmNode *class_list) {
	AGmNode *rp = AUiProgramMgr::Instance()->ProjectList();
	AGmNode *proj_info = List(root_class, lib_list, class_list);
	CreateArc(rp, proj_info, List(String(dir->Str()), String(name->Str()) ));
	void *value;
	GetValue(NULL, AGM_VM_KEY_PROJ_EDITOR, value);
	if (value) {
		((ADvWindow*)value)->Shell()->Update();
	}
	return proj_info;
}
int AUiProjectShell::RemoveProject(AGmNode *project) {
	AGmLink *l = find_outlink(AUiProgramMgr::Instance()->ProjectList(), project);
	if (l == NULL) return 1;

	ADvBeginWaitCursor();

// close decendant windows
	AGmNode *root_class = l->arc->dst->H;
	void *value;
	GetValue(root_class, AGM_VM_KEY_CLASS_EDITOR, value);
	if (value) {
		AUiProgramMgr::CloseWindow((ADvWindow*)value);
	}

// destroy project
	AUiMemberShell::CleanVtbl(root_class);
	AUiVtblUpdate = 0;
	
	// unregister class registration
	AGmNode *class_list = l->arc->dst->TTH;
	AGmLink *l2;
	for (l2 = next_outlink(class_list, NULL);
			l2; l2 = next_outlink(class_list, l2)) {
		AGmNode *_class = l2->arc->dst;
		AGmNode *_base_class = src_node2(_class, NmSubClass());
		if (_class && _base_class) {
			AGmLink *l3 = find_outlink(_base_class, _class);
			DeleteArc(l3->arc);
		}
	}

	// unregister root class
	DeleteArc(l->arc);

	GetValue(NULL, AGM_VM_KEY_PROJ_EDITOR, value);
	if (value) {
		((ADvWindow*)value)->Shell()->Update();
	}

	// garbage collection
	GC();

	ADvEndWaitCursor();
	return 0;
}
AGmLink* AUiProjectShell::SearchProject(AGmString *dir, AGmString *name) {
	AGmNode *rp = AUiProgramMgr::Instance()->ProjectList();
	AGmLink *l;
	for (l = next_outlink(rp, NULL);
			l; l = next_outlink(rp, l)) {
		if (l->arc->attr->H->a.s.v->Compare(dir)) continue;
		if (l->arc->attr->TH->a.s.v->Compare(name)) continue;
		return l;
	}
	return NULL;
}
AGmLink* AUiProjectShell::SearchProject(AGmNode *root_class) {
	AGmNode *rp = AUiProgramMgr::Instance()->ProjectList();
	AGmLink *l;
	for (l = next_outlink(rp, NULL);
			l; l = next_outlink(rp, l)) {
		if (l->arc->dst->H != root_class) continue;
		return l;
	}
	return NULL;
}

int AUiProjectShell::Reset(long lno) {
	AGmLink *l = next_outlink(AUiProgramMgr::Instance()->ProjectList(), NULL);
	int status;
	if (l) {
		status = 0;
		_project = l->arc->dst;
	} else {
		status = 1;
	}
	int flag = ADvEditShell::Reset(lno);
	_status |= status;
	return flag;
}
int AUiProjectShell::More() {
	return (_status != 2);
}
int AUiProjectShell::IsLast() {
	return (_status == 1);
}
int AUiProjectShell::Next() {
	if (_status == 2) return 0;
	if (_status == 1) {
		_status = 2; return 0;
	}
	AGmNode *rp = AUiProgramMgr::Instance()->ProjectList();
	AGmLink *l = find_outlink(rp, _project);
	l = next_outlink(rp, l);
	if (l) {
		_project = l->arc->dst;
	} else {
		_status = 1;
	}
	_lno ++;
	return 0;
}

AGmString* AUiProjectShell::GetLineString() {
	AGmString::DeleteString(_l_str);
	if (_status == 0) {
		AGmLink *l = find_outlink(AUiProgramMgr::Instance()->ProjectList(), _project);
		ASSERT(l);
		_l_str = new AGmString(l->arc->attr->TH->a.s.v);
		_l_str->Append("\t");
		_l_str->Append(l->arc->attr->H->a.s.v);
	} else if (_status == 1) {
		AGmString::CreateString(_l_str, "END");
	}
	return _l_str;
}

int AUiProjectShell::_PreDeleteLine() {
	AGmString::DeleteString(_e_str);
	if (_status != 0) {
		AGmString::CreateString(_e_str, AL_S113);
		return 1;
	}
	return 0;
}
int AUiProjectShell::_DeleteLine() {
	return RemoveProject(_project);
}

int AUiProjectShell::_PreOpenEditor() {
	if (_status != 0) return 1;
	return 0;
}
int AUiProjectShell::_OpenEditor() {
	return OpenClassEditor(_project);
}

int AUiProjectShell::OpenClassEditor(AGmNode *proj) {
	AGmLink *l = find_outlink(AUiProgramMgr::Instance()->ProjectList(), proj);
	ASSERT(l);
	AGmString *name = l->arc->attr->TH->a.s.v;
	AGmNode *root_class = proj->H;
	void *value;
	GetValue(root_class, AGM_VM_KEY_CLASS_EDITOR, value);
	if (value != NULL) return ((ADvWindow*)value)->Raise();

	ADvWindow *aw = new ADvWindow();
	SetValue(root_class, AGM_VM_KEY_CLASS_EDITOR, aw);
	AUiClassShell *shell = new AUiClassShell(aw, root_class, 1);
	if (aw->CreateEdit(shell) == NULL) { delete shell; delete aw; return 1; }
	char s[256];
	art_sprintf_1(s, 256, AL_T_CLASS, name->Str());
	aw->SetTitle(s);
	aw->SetFocus();

	// make class window to be a child of project window
	AGmNode *n_class = ADvWndToNode(aw);
	ASSERT(n_class);
	l = find_outlink(RootWindowNode(), n_class);
	ASSERT(l);
	GetValue(NULL, AGM_VM_KEY_PROJ_EDITOR, value);
	if (value == NULL) {
		AUiProgramMgr::OpenProjectEditor();
		GetValue(NULL, AGM_VM_KEY_PROJ_EDITOR, value);
		ASSERT(value);
	}
	AGmNode *n_proj = ADvWndToNode((ADvWindow*)value);
	ASSERT(n_proj);
	ReplaceArcSrc(l->arc, n_proj);
	return 0;
}

int AUiProjectShell::Up(int& l_from, int& l_to) {
	AGmString::DeleteString(_e_str);
	if (_status != 0) {
		AGmString::CreateString(_e_str, AL_S113);
		return 1;
	}
	AGmNode *rp = AUiProgramMgr::Instance()->ProjectList();
	AGmLink *l1 = find_outlink(rp, _project);
	ASSERT(l1);
	AGmLink *l2 = prev_outlink(rp, l1);
	if (l2 == NULL) {
		AGmString::CreateString(_e_str, AL_S059);
		return 1;
	}
	l_from = (int)(_lno - 1);
	l_to = (int)(_lno);
	AGM_REMOVE(l1); AGM_INSERT(l1, l2);
	return 0;
}
int AUiProjectShell::Down(int& l_from, int& l_to) {
	AGmString::DeleteString(_e_str);
	if (_status != 0) {
		AGmString::CreateString(_e_str, AL_S113);
		return 1;
	}
	AGmNode *rp = AUiProgramMgr::Instance()->ProjectList();
	AGmLink *l1 = find_outlink(rp, _project);
	ASSERT(l1);
	AGmLink *l2 = next_outlink(rp, l1);
	if (l2 == NULL) {
		AGmString::CreateString(_e_str, AL_S056);
		return 1;
	}
	l_from = (int)(_lno);
	l_to = (int)(_lno +1);
	AGM_REMOVE(l2); AGM_INSERT(l2, l1);
	return 0;
}

#define ID_NEW		100
#define ID_OPEN 	110
#define ID_DELETE	120
#define ID_EXIT 	130
#define ID_UP		200
#define ID_DOWN 	210
#define ID_REDRAW	300
#define ID_CLOSE	310
#define ID_MARK_OUTPUT	400
#define ID_OPEN2		410
#define ID_GC			420
#ifdef	_DEBUG
#define ID_EVAL_RET_FUNC_NAME_TRACE 450
#endif

static ADvMenuInfo file_menu[] = {
	{ AL_M_NEW, 	ID_NEW, 	NULL},
	{ AL_M_OPEN,	ID_OPEN,	NULL},
	{ AL_M_CLOSE,	ID_DELETE,	NULL},
	{ AL_M_EXIT,	ID_EXIT,	NULL},
	{ NULL, 		0,			NULL}
};
static ADvMenuInfo edit_menu[] = {
	{ AL_M_UP,		ID_UP,		NULL},
	{ AL_M_DOWN,	ID_DOWN,	NULL},
	{ NULL, 		0,			NULL}
};
static ADvMenuInfo window_menu[] = {
	{ AL_M_REDRAW,	ID_REDRAW,	NULL},
	{ AL_M_CLOSE,	ID_CLOSE,	NULL},
	{ NULL, 	0,			NULL}
};
static ADvMenuInfo misc_menu[] = {
	{ AL_M_MARK_OUTPUT, 		ID_MARK_OUTPUT, NULL},
	{ AL_M_OPEN_WITH_OUTPUT,	ID_OPEN2,		NULL},
	{ AL_M_GC,					ID_GC,			NULL},
#ifdef	_DEBUG
	{ "EvalRetFuncNameTrace",	ID_EVAL_RET_FUNC_NAME_TRACE, NULL},
#endif
	{ NULL, 					0,				NULL}
};
static ADvMenuInfo class_menu[] = {
	{ AL_M_FILE,	0,		file_menu},
	{ AL_M_EDIT,	0,		edit_menu},
	{ AL_M_WINDOW,	0,		window_menu},
	{ AL_M_MISC,	0,		misc_menu},
	{ NULL, 		0,		NULL}
};

int AUiProjectShell::LeftPress(ADvWindow *aw, AGmNode *e) {
	if (e->TTTTHI != MODIF_CONTROL)
		return ADvEditShell::LeftPress(aw, e);
	ASSERT(_edit);
	long from, to, line_from, line_to;
	_edit->GetSel(from, to);
	line_from = _edit->LineFromChar(from);
	line_to = _edit->LineFromChar(to);
	if (line_from != line_to)
		return ADvEditShell::LeftPress(aw, e);
	return Menu(aw, e, line_from);
}
int AUiProjectShell::LeftRelease(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::LeftRelease(aw, e);
}
int AUiProjectShell::LeftDoubleClick(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::LeftDoubleClick(aw, e);
}
int AUiProjectShell::RightPress(ADvWindow *aw, AGmNode *e) {
	ASSERT(_edit);
	long from, to, line_from, line_to;
	_edit->GetSel(from, to);
	line_from = _edit->LineFromChar(from);
	line_to = _edit->LineFromChar(to);
	if (line_from != line_to)
		return ADvEditShell::RightPress(aw, e);
	return Menu(aw, e, line_from);
}
int AUiProjectShell::RightRelease(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::RightRelease(aw, e);
}
int AUiProjectShell::RightDoubleClick(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::RightDoubleClick(aw, e);
}
int AUiProjectShell::MouseMove(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::MouseMove(aw, e);
}
int AUiProjectShell::KeyPressFunc(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyPressFunc(aw, e);
}
int AUiProjectShell::KeyReleaseFunc(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyReleaseFunc(aw, e);
}
int AUiProjectShell::KeyChar(ADvWindow*, AGmNode*) {
	return -1;
}
int AUiProjectShell::Menu(ADvWindow *aw, AGmNode *e, long line_from) {
#ifdef	_WINDOWS
	POINT pt;
	_edit->SendMessage(EM_GETSCROLLPOS, 0, (LPARAM)&pt);
#endif
#ifdef	_LINUX
	GtkAdjustment *vbar = gtk_scrolled_window_get_vadjustment(_edit->scrolled_window);
	gdouble value = gtk_adjustment_get_value(vbar);
#endif
	int lno = _edit->LineFromChar(-1);
	Reset(lno);
	int l_from, l_to;
	long id = ADvWindow::MenuSelect(aw, e, class_menu);
	switch (id) {
	case ID_NEW:
		AUiProgramMgr::New();
		break;
	case ID_OPEN:
		AUiProgramMgr::Open();
		break;
	case ID_DELETE:
		if (DeleteLine()) {
			// case that Delete is failed
			AGmString* s = GetErrorString();
			if (s)
				ADvWindow::Print(s->Str());
			else
				ADvWindow::Print(AL_S040);
		} else {
			int begin, end;
			begin = _edit->LineIndex(lno);
			end = _edit->LineIndex(lno + 1);
			_edit->SetSel(begin, end);
			_edit->ReplaceSel("");
			Reset(lno);
		}
		Update(line_from - 4);
#ifdef	_WINDOWS
		_edit->SendMessage(EM_SETSCROLLPOS, 0, (LPARAM)&pt);
#endif
#ifdef	_LINUX
		ADvDoEvents();
		gtk_adjustment_set_value(vbar, value);
#endif
		break;
	case ID_EXIT:
		AUiProgramMgr::Exit();
		break;
	case ID_UP:
		if (Up(l_from, l_to)) {
			AGmString *s = GetErrorString();
			ADvWindow::Print(s->Str());
		} else {
			OverWriteLines(l_from, l_to);
		}
		break;
	case ID_DOWN:
		if (Down(l_from, l_to)) {
			AGmString *s = GetErrorString();
			ADvWindow::Print(s->Str());
		} else {
			OverWriteLines(l_from, l_to);
		}
		break;
	case ID_REDRAW:
		Update(line_from - 4);
#ifdef	_WINDOWS
		_edit->SendMessage(EM_SETSCROLLPOS, 0, (LPARAM)&pt);
#endif
#ifdef	_LINUX
		ADvDoEvents();
		gtk_adjustment_set_value(vbar, value);
#endif
		break;
	case ID_CLOSE:
		AUiProgramMgr::CloseWindow(aw);
		break;
	case ID_MARK_OUTPUT:
		ADvWindow::Print("====================\n");
		break;
	case ID_OPEN2:
		if (AUiProgramMgr::check_load) {
			AUiProgramMgr::check_load = 0;
			ADvWindow::Print(AL_S212);
		} else {
			AUiProgramMgr::check_load = 1;
			ADvWindow::Print(AL_S211);
		}
		break;
	case ID_GC:
#if	_WINDOWS && _64BIT
		long long num_new_free, num_free, num_use;
#else
		long num_new_free, num_free, num_use;
#endif
		AGm::GC(&num_new_free, &num_free, &num_use);
		ADvWindow::Print("----------------------------------------\n");
		char msg[256];
		art_sprintf_1(msg, 256, AL_GC_NEW_CELL, num_new_free);
		ADvWindow::Print(msg);
		art_sprintf_1(msg, 256, AL_GC_FREE_CELL, num_free);
		ADvWindow::Print(msg);
		art_sprintf_1(msg, 256, AL_GC_USE_CELL, num_use);
		ADvWindow::Print(msg);
		break;
#ifdef	_DEBUG
	case ID_EVAL_RET_FUNC_NAME_TRACE:
		if (AIpProcess::eval_ret_func_name_trace) {
			AIpProcess::eval_ret_func_name_trace = 0;
			ADvWindow::Print("eval_ret_func_trace off");
		} else {
			AIpProcess::eval_ret_func_name_trace = 1;
			ADvWindow::Print("eval_ret_func_trace on");
		}
		break;
#endif
	default:
		break;
	}
	return 0;
}

int AUiProjectShell::OverWriteLines(int l_from, int l_to) {
	Reset(l_from);
	int from, to;
	from = _edit->LineIndex(l_from);
	to = _edit->LineIndex(l_to+1);
	_edit->SetSel(from, to);
	_edit->ReplaceSel("");
	for (int i = l_from; i < l_to+1; i++) {
		AGmString *s = GetLineString();
		_edit->ReplaceSel(s->Str());
		_edit->ReplaceSel(newline);
		Next();
	}
	return 0;
}
