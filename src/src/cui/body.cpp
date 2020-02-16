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
#include <dev/advwnd.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <cui/cui.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/body.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
// #define	_DEBUG_BODY_EDITOR
#endif

AUiBodyShell::AUiBodyShell(ADvWindow *aw, AGmNode *root_class, AGmNode *class2, AGmNode *member, int for_ui)
: ADvEditShell(aw) {
	_for_ui = for_ui;
	_root_class = Ref(root_class);
	_class = Ref(class2);
	_member = Ref(member);
	exp = exp_stack = loc_base = ctrl_stack = NULL;
}
AUiBodyShell::~AUiBodyShell() {
	if (_for_ui)
		RemoveValue(_member, AGM_VM_KEY_BODY_EDITOR);
	Unref(exp, exp_stack, loc_base, ctrl_stack);
	Unref(_member, _class, _root_class);
}
void AUiBodyShell::CreateBody() {
	if (_member->TTH == NULL) {
		Set(_member->TTH, Integer(AIP_EvalBlock, Cons(NULL)));
	}
}

void AUiBodyShell::Update(int curr_lno) {
	if (_edit == NULL) return;
	int lno = _edit->GetLineCount() - 1;
	_edit->SetSel(0, _edit->LineIndex(lno) + _edit->LineLength(lno));
	_edit->ReplaceSel("");
	AGmString *ss = new AGmString(4096);
	for (Reset(); More(); /* Next() */) {
		AGmString *s = GetLineStringNext();
		ASSERT(s);
		ss->Append(s->Str());
		ss->Append(newline);
	}
	_edit->ReplaceSel(ss->Str());

	lno = _edit->GetLineCount() - 1;
	if (curr_lno < 0) curr_lno = 0;
	if (curr_lno > lno) curr_lno = lno;
	int pos = _edit->LineIndex(curr_lno);
	_edit->SetSel(pos, pos);
	delete ss;
}

int AUiBodyShell::Reset(long lno) {
	Unref(exp, exp_stack, loc_base, ctrl_stack);
	exp = exp_stack = loc_base = ctrl_stack = NULL;
	prog = &_member->TTH;
	return ADvEditShell::Reset(lno);
}
int AUiBodyShell::More() {
	return (_status != 2);
}
int AUiBodyShell::IsLast() {
	return (_status == 1);
}			 
int AUiBodyShell::Next() {
	if (_status == 2) return 0;
	if (_status == 1) {
		_status = 2; return 0;
	}
	_create_text = 0;
	line_printf(_l_str);
	return 0;
}

AGmString* AUiBodyShell::GetLineString() {
	AGmString::DeleteString(_l_str);
	if (_status == 0) {
		long lno = _lno;
		_create_text = 1;
		line_printf(_l_str);
		Reset(lno);
	} else if (_status == 1) {
		AGmString::CreateString(_l_str, "END");
	}
	return _l_str;
}
AGmString* AUiBodyShell::GetLineStringNext() {
	AGmString::DeleteString(_l_str);
	if (_status == 0) {
		_create_text = 1;
		line_printf(_l_str);
	} else if (_status == 1) {
		AGmString::CreateString(_l_str, "END");
		_status = 2;
	}
	return _l_str;
}
int AUiBodyShell::_PreInsertLine(const char *s) {
	_num_insert_line = 1;
	AGmString::DeleteString(_e_str);
	AGmFile::CreateStrReadFile(_file, _line, s);
	if (_status == 0 && _insert_pos == AUI_SHELL_INSERT_LOWER) {
		_create_text = 0;
		AGmString *s;
		line_printf(s);
	}
	_a = new AUiAG(_file);
	int flag = line_scanf(_a);
	if (flag) {
		if (_lno > 0 && _depth < _limit_depth) _lno --;
		Reset(_lno);
	} else {
		switch (_a->Ctrl()) {
		case AUI_CTRL_BLOCK:
		case AUI_CTRL_LOOP:
			_num_insert_line = 2;
			break;
		case AUI_CTRL_BRANCH:
		case AUI_CTRL_TRY:
			_num_insert_line = 3;
			break;
		default:
			break;
		}
	}
	return flag;
}
int AUiBodyShell::_InsertLine(const char*) {
#ifdef	_DEBUG_BODY_EDITOR
TRACE0("InsertLine\n");
DumpList2(_a->Head());
#endif
	switch (_a->Ctrl()) {
	case AUI_CTRL_BLOCK2:
	case AUI_CTRL_LOOP2:
	case AUI_CTRL_BRANCH3:
	case AUI_CTRL_TRY3:
		break;
	default:
		Set(*_a->Tail(), *prog);
		Set(*prog, _a->Head());
		break;
	}
	Reset(_lno);
	return 0;
}
void AUiBodyShell::_PostInsertLine(const char*) {
	delete _a;
	AGmFile::DeleteStrReadFile(_file, _line);
}
int AUiBodyShell::_PreReplaceLine(const char *s) {
	line_no = _lno;
	AGmString::DeleteString(_e_str);
	AGmFile::CreateStrReadFile(_file, _line, s);
	_a = new AUiAG(_file);
	_end = (_file->MatchIdent("END") && _file->IsEnd());
	if (_end) return 0;
	int flag = line_scanf(_a);
	if (flag == 0) {
		Reset(line_no);
		_create_text = 0;
		AGmString *s;
		line_printf(s);
		if (_a->Ctrl() != ctrl_type) {
			AGmString::CreateString(_e_str, AL_S086);
			flag = 1;
		}
	}
	return flag;
}
int AUiBodyShell::_ReplaceLine(const char*) {
	if (_end) return 0;
#ifdef	_DEBUG_BODY_EDITOR
TRACE0("ReplaceLineLine\n");
DumpList2(_a->Head());
#endif
	switch (ctrl_type)	{
	case AUI_CTRL_BLOCK:
	case AUI_CTRL_BLOCK2:
	case AUI_CTRL_LOOP:
	case AUI_CTRL_LOOP2:
	case AUI_CTRL_BRANCH2:
	case AUI_CTRL_BRANCH3:
	case AUI_CTRL_TRY:
	case AUI_CTRL_TRY2:
	case AUI_CTRL_TRY3:
		break;
	case AUI_CTRL_BRANCH:
		Set(*_a->CtrlTail(), *ctrl_tail);
		Set(*_prog, _a->Head());
		break;
	default:
		Set(*_a->Tail(), *prog);
		Set(*_prog, _a->Head());
		break;
	}
	return 0;
}
void AUiBodyShell::_PostReplaceLine(const char*) {
	delete _a;
	AGmFile::DeleteStrReadFile(_file, _line);
	Reset(line_no);
}
int AUiBodyShell::_PreDeleteLine() {
	AGmString::DeleteString(_e_str);
	_num_delete_line = 0;
	int depth = _depth;
	AGmString *s;
	_create_text = 0;
	for (int i = line_to-line_from+1; i > 0; i--) {
		line_printf(s);
		if (_depth < depth) {
			AGmString::CreateString(_e_str, 
			AL_S162);
			return 1;
		}
		_num_delete_line ++;
	}
	while (_depth > depth) {
		line_printf(s);
		_num_delete_line ++;		
	}
	Reset(line_from);
	return 0;
}
int AUiBodyShell::_DeleteLine() {
	AGmNode **p1 = prog;
	AGmString *s;
	_create_text = 0;
	for (int i = 0; i < _num_delete_line; i++) {
		line_printf(s);
	}
	Set(*p1, *prog);
	return 0;
}

int AUiBodyShell::IsSameType(AGmNode *type1, AGmNode *type2) {
	if (type1->I != type2->I) return 0;
	if (type1->I == AUI_TYPE_CLASS ||
			type1->I == AUI_TYPE_CLASS_FUNC ||
			type1->I == AUI_TYPE_OBJ_FUNC) {
		AGmNode *cn1 = dst_node2(type1, NmType());
		AGmNode *cn2 = dst_node2(type2, NmType());	
		return (cn1 == cn2);
	}
	return 1;
}
int AUiBodyShell::IsDerived(AGmNode *this_cl, AGmNode *abst_cl) {
	if (this_cl->I != abst_cl->I) return 0;
	if (this_cl->I == AUI_TYPE_CLASS) {
		AGmNode *this_cn = dst_node2(this_cl, NmType());
		AGmNode *abst_cn = dst_node2(abst_cl, NmType());	
		return _IsDerived(this_cn, abst_cn);
	}
	return 1;
}
int AUiBodyShell::_IsDerived(AGmNode *this_cl, AGmNode *abst_cl) {
	if (this_cl == abst_cl) return 1;
	AGmLink *l;
	for (l = next_inlink(this_cl, NULL, NmSubClass());
			l; l = next_inlink(this_cl, l, NmSubClass())) {
		if (_IsDerived(l->arc->src, abst_cl)) return 1;
	}
	return 0;
}
int AUiBodyShell::IsProtected(long info,
			AGmNode *abst_cl, AGmNode *obj_cl, AGmNode *this_cl) {
	if (info & AUI_PRIVATE) {
		if (obj_cl != abst_cl) return 1;
		if (this_cl != obj_cl) return 1;
		return 0;
	} else if (info & AUI_PROTECTED) {
		if (!_IsDerived(obj_cl, abst_cl)) return 1;
		if (!_IsDerived(this_cl, obj_cl)) return 1;
		return 0;
	} else	if (info & AUI_PUBLIC) {
		if (!_IsDerived(obj_cl, abst_cl)) return 1;
		return 0;
	} else {
		return 1;
	}
}
void AUiBodyShell::Overload(
			AGmNode *func_dcl, AGmNode *arg_dcl2, AGmNode*& candidate) {
	AGmNode *arg_dcl1 = func_dcl->TTTH;
	AGmNode *a1, *a2, *a3;

	// check whether arg_dcl is matched
	for (a1 = arg_dcl1, a2 = arg_dcl2;
			a1 && a2;
			a1 = a1->TT, a2 = a2->TT) {
		if (!IsDerived(a2->TH, a1->TH)) break;
	}
	if (a1 != NULL || a2 != NULL) return ;
	if (candidate == NULL) { candidate = func_dcl; return; }

	// define priority if there is one which is previously matched
	AGmNode *arg_dcl3 = candidate->TTTH;
	for (a1 = arg_dcl1, a2 = arg_dcl2, a3 = arg_dcl3;
			a1 && a2 && a3;
			a1 = a1->TT, a2 = a2->TT, a3 = a3->TT) {
		if (IsSameType(a1->TH, a3->TH)) continue;
		if (IsSameType(a3->TH, a2->TH)) break;
		if (IsSameType(a1->TH, a2->TH)) { candidate = func_dcl; break; }
	}
}

int AUiBodyShell::IsClassFuncType(AGmNode *func_type, AGmNode *arg_dcl2) {
	if (func_type->I != AUI_TYPE_CLASS_FUNC) return 0;
	AGmNode *candidate = NULL;
	Overload(dst_node2(func_type, NmType()), arg_dcl2, candidate);
	return (candidate != NULL);
}
int AUiBodyShell::IsObjectFuncType(AGmNode *func_type, AGmNode *arg_dcl2) {
	if (func_type->I != AUI_TYPE_OBJ_FUNC) return 0;
	if (func_type->I != AUI_TYPE_OBJ_FUNC) return 0;
	AGmNode *candidate = NULL;
	Overload(dst_node2(func_type, NmType()), arg_dcl2, candidate);
	return (candidate != NULL);
}
AGmNode *AUiBodyShell::FuncRetType(AGmNode *func_type) {
	return dst_node2(func_type, NmType())->TH;
}

AGmNode* AUiBodyShell::find_loc_var(
			AGmNode *loc_base, AGmNode *name) {
	AGmNode* ls;
	for (ls = loc_base; ls; ls = ls->T) {
		AGmNode *l;
		for (l = ls->H; l; l = l->T) {
			if (Compare(l->HH, name) == 0) return l->HTTH;
		}
	}
	return NULL;
}
AGmNode* AUiBodyShell::find_arg_var(
			AGmNode *arg_dcl, AGmNode *name) {
	AGmNode *l;
	for (l = arg_dcl; l; l = l->TT) {
		if (Compare(l->H, name) == 0) return l->TH;
	}
	return NULL;
}

AGmNode* AUiBodyShell::find_class_var(
			AGmNode *abst_cl, AGmNode *this_cl,
			AGmNode *name, int& protect) {
	AGmLink *ol;
	for (ol = next_outlink(abst_cl, NULL, name);
			ol; ol = next_outlink(abst_cl, ol, name)) {
		AGmNode *var_dcl = ol->arc->dst;
		long info = (long)var_dcl->HI;
		if ((info & AUI_STATIC) == 0) continue;
		if (info & AUI_FUNC) continue;
		protect = IsProtected(info, abst_cl, abst_cl, this_cl);
		return var_dcl;
	}
	AGmLink *il;
	for (il = next_inlink(abst_cl, NULL, NmSubClass());
			il; il = next_inlink(abst_cl, il, NmSubClass())) {
		AGmNode *var_dcl = find_class_var(il->arc->src, this_cl, name, protect);
		if (var_dcl) return var_dcl;
	}
	return NULL;
}
AGmNode* AUiBodyShell::find_obj_var(
			AGmNode *abst_cl, AGmNode *obj_cl, AGmNode *this_cl,
			AGmNode *name, int& protect) {
	AGmLink *ol;
	for (ol = next_outlink(abst_cl, NULL, name);
			ol; ol = next_outlink(abst_cl, ol, name)) {
		AGmNode *var_dcl = ol->arc->dst;
		long info = (long)var_dcl->HI;
		if (info & AUI_STATIC) continue;
		if (info & AUI_FUNC) continue;
		protect = IsProtected(info, abst_cl, obj_cl, this_cl);
		return var_dcl;
	}
	AGmLink *il;
	for (il = next_inlink(abst_cl, NULL, NmSubClass());
			il; il = next_inlink(abst_cl, il, NmSubClass())) {
		AGmNode *var_dcl = find_obj_var(il->arc->src, obj_cl, this_cl, name, protect);
		if (var_dcl) return var_dcl;
	}
	return NULL;
}
AGmNode* AUiBodyShell::find_class_func(
			AGmNode *abst_cl, AGmNode *this_cl,
			AGmNode *name, AGmNode *arg_dcl2, int& protect,
			int no_proto, int no_recursive) {
	AGmNode *func_dcl, *candidate = NULL;
	AGmLink *ol;
	for (ol = next_outlink(abst_cl, NULL, name);
			ol; ol = next_outlink(abst_cl, ol, name)) {
		func_dcl = ol->arc->dst;
		long info = (long)func_dcl->HI;
		if ((info & AUI_STATIC) == 0) continue;
		if ((info & AUI_FUNC) == 0) continue;
		protect = IsProtected(info, abst_cl, abst_cl, this_cl);
		if (no_proto) return func_dcl;
		Overload(func_dcl, arg_dcl2, candidate);
	}
	if (candidate) return candidate;
	if (no_recursive) return NULL;
	AGmLink *il;
	for (il = next_inlink(abst_cl, NULL, NmSubClass());
			il; il = next_inlink(abst_cl, il, NmSubClass())) {
		func_dcl = find_class_func(il->arc->src, this_cl, name, arg_dcl2, protect);
		if (func_dcl) return func_dcl;
	}
	return NULL;
}
AGmNode* AUiBodyShell::find_obj_func(
			AGmNode *abst_cl, AGmNode *obj_cl, AGmNode *this_cl,
			AGmNode *name, AGmNode *arg_dcl2, int& protect,
			int no_proto, int no_recursive) {
	AGmNode *func_dcl, *candidate = NULL;
	AGmLink *ol;
	for (ol = next_outlink(abst_cl, NULL, name);
			ol; ol = next_outlink(abst_cl, ol, name)) {
		func_dcl = ol->arc->dst;
		long info = (long)func_dcl->HI;
		if (info & AUI_STATIC) continue;
		if ((info & AUI_FUNC) == 0) continue;
		protect = IsProtected(info, abst_cl, obj_cl, this_cl);
		if (no_proto) return func_dcl;
		Overload(func_dcl, arg_dcl2, candidate);
	}
	if (candidate) return candidate;
	if (no_recursive) return NULL;
	AGmLink *il;
	for (il = next_inlink(abst_cl, NULL, NmSubClass());
			il; il = next_inlink(abst_cl, il, NmSubClass())) {
		func_dcl = find_obj_func(il->arc->src, obj_cl, this_cl, name, arg_dcl2, protect);
		if (func_dcl) return func_dcl;
	}
	return NULL;
}

#define ID_NEW			100
#define ID_OPEN 		110
#define ID_SAVE 		120
#define ID_SAVE_AS		130
#define ID_PRINT		140
#define ID_EXIT 		150
#define ID_INSERT	200
#define ID_APPEND	210
#define ID_CUT		220
#define ID_COPY 	230
#define ID_PASTE	240
#define ID_DELETE	250
#define ID_REDRAW		300
#define ID_PARENT		310
#define ID_CLOSE		320
#define ID_CLOSE_ALL	330
#define ID_MARK_OUTPUT	400

static ADvMenuInfo file_menu[] = {
	{ AL_M_NEW, 	ID_NEW, 	NULL},
	{ AL_M_OPEN,	ID_OPEN,	NULL},
	{ AL_M_SAVE,	ID_SAVE,	NULL},
	{ AL_M_SAVE_AS, ID_SAVE_AS, NULL},
	{ AL_M_PRINT,	ID_PRINT,	NULL},
	{ AL_M_EXIT,	ID_EXIT,	NULL},
	{ NULL, 		0,			NULL}
};
static ADvMenuInfo pos_menu[] = {
	{ AL_M_UPPER,	ID_INSERT,	NULL},
	{ AL_M_LOWER,	ID_APPEND,	NULL},
	{ NULL, 0,		NULL}
};
static ADvMenuInfo edit_menu[] = {
	{ AL_M_POS, 	0,			pos_menu},
	{ AL_M_CUT, 	ID_CUT, 	NULL },
	{ AL_M_COPY,	ID_COPY,	NULL},
	{ AL_M_PASTE,	ID_PASTE,	NULL},
	{ AL_M_DELETE,	ID_DELETE,	NULL},
	{ NULL, 		0,			NULL}
};
static ADvMenuInfo window_menu[] = {
	{ AL_M_REDRAW,		ID_REDRAW,		NULL},
	{ AL_M_PARENT,		ID_PARENT,		NULL},
	{ AL_M_CLOSE,		ID_CLOSE,		NULL},
	{ AL_M_CLOSE_ALL,	ID_CLOSE_ALL,	NULL},
	{ NULL, 			0,				NULL}
};
static ADvMenuInfo misc_menu[] = {
	{ AL_M_MARK_OUTPUT, ID_MARK_OUTPUT, NULL},
	{ NULL, 			0,				NULL}
};
static ADvMenuInfo body_menu[] = {
	{ AL_M_FILE,	0,	file_menu},
	{ AL_M_EDIT,	0,	edit_menu},
	{ AL_M_WINDOW,	0,	window_menu},
	{ AL_M_MISC,	0,	misc_menu},
	{ NULL, 		0,	NULL}
};

int AUiBodyShell::LeftPress(ADvWindow *aw, AGmNode *e) {
	if (e->TTTTHI != MODIF_CONTROL)
		return ADvEditShell::LeftPress(aw, e);
	return Menu(aw, e);
}
int AUiBodyShell::LeftRelease(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::LeftRelease(aw, e);
}
int AUiBodyShell::LeftDoubleClick(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::LeftDoubleClick(aw, e);
}
int AUiBodyShell::RightPress(ADvWindow *aw, AGmNode *e) {
	return Menu(aw, e);
}
int AUiBodyShell::RightRelease(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::RightRelease(aw, e);
}
int AUiBodyShell::RightDoubleClick(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::RightDoubleClick(aw, e);
}
int AUiBodyShell::MouseMove(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::MouseMove(aw, e);
}
int AUiBodyShell::KeyPressFunc(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyPressFunc(aw, e);
}
int AUiBodyShell::KeyReleaseFunc(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyReleaseFunc(aw, e);
}
int AUiBodyShell::KeyChar(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyChar(aw, e);
}
int AUiBodyShell::Menu(ADvWindow *aw, AGmNode *e) {
	long from, to;
	_edit->GetSel(from, to);
	line_from = _edit->LineFromChar(from);
	line_to = _edit->LineFromChar(to);
#ifdef	_WINDOWS
	if (line_from < line_to) {
		int from2 = _edit->LineIndex(line_to);
		char *buf = new char[to - from2 + 1];
		long n = _edit->GetSelText(buf, to - from2);
		if (n == 0) line_to--;
		delete buf;
	}
	POINT pt;
	_edit->SendMessage(EM_GETSCROLLPOS, 0, (LPARAM)&pt);
#endif
#ifdef	_LINUX
	GtkAdjustment *vbar = gtk_scrolled_window_get_vadjustment(_edit->scrolled_window);
	gdouble value = gtk_adjustment_get_value(vbar);
#endif
	int lno = line_from;
	Reset(lno);
	AGmNode *n, *p;
#if	_WINDOWS && _64BIT
	long long id = ADvWindow::MenuSelect(aw, e, body_menu);
#else
	long id = ADvWindow::MenuSelect(aw, e, body_menu);
#endif
	switch (id) {
	case ID_NEW:
		AUiProgramMgr::New();
		break;
	case ID_OPEN:
		AUiProgramMgr::Open();
		break;
	case ID_SAVE:
		AUiProgramMgr::Save(_root_class);
		break;
	case ID_SAVE_AS:
		AUiProgramMgr::SaveAs(_root_class);
		break;
	case ID_PRINT:
		Print();
		break;
	case ID_EXIT:
		AUiProgramMgr::Exit();
		break;
	case ID_INSERT:
		_insert_pos = AUI_SHELL_INSERT_UPPER;
		break;
	case ID_APPEND:
		_insert_pos = AUI_SHELL_INSERT_LOWER;
		break;
	case ID_CUT: {
			AGmString *ss = new AGmString(128);
			for (Reset(line_from); _lno <= line_to; Next()) {
				AGmString *s = GetLineString();
				if (s) { ss->Append(s); ss->Append("\n"); }
			}
			ss->WriteString("END\n");
			SetToClipboard(String(ss));

			Reset(lno);
			if (DeleteLine()) {
				// case that Delete is failed
				AGmString* s = GetErrorString();
				if (s)
					ADvWindow::Print(s->Str());
				else
					ADvWindow::Print(AL_S040);
			} else {
				int begin = _edit->LineIndex(lno);
				int end = _edit->LineIndex(lno + _num_delete_line);
				_edit->SetSel(begin, end);
				_edit->ReplaceSel("");
				Reset(lno);
			}
		}
		break;
	case ID_COPY: {
			AGmString *ss = new AGmString(128);
			for (Reset(line_from); _lno <= line_to; Next()) {
				AGmString *s = GetLineString();
				if (s) { ss->Append(s); ss->Append("\n"); }
			}
			ss->WriteString("END\n");
			SetToClipboard(String(ss));
		}
		break;
	case ID_PASTE: {
			AGmNode *n = GetFromClipboard();
			if (!IsString(n)) break;
			AGmFile *f = new AGmFile;
			if (f->Create(n->a.s.v, AGM_FILE_STR_READ)) {
				ADvWindow::Print(AL_S065);
				delete f; break;
			}
			AGmString *s;
			Reset(line_from);
			for (;;) {
				f->ReadSpace();
				if (f->ReadLine(s)) {
					ADvWindow::Print(AL_S091);
					break;
				}
				if (s->Compare("END") == 0)
					break;
				if (InsertLine(s->Str())) {
					ADvWindow::Print(AL_S090);
					break;
				}
				delete s;
			}
			delete s;
			delete f;
			Update(line_from - 4);
#ifdef	_WINDOWS
			_edit->SendMessage(EM_SETSCROLLPOS, 0, (LPARAM)&pt);
#endif
#ifdef	_LINUX
			ADvDoEvents();
			gtk_adjustment_set_value(vbar, value);
#endif
		}
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
			int begin = _edit->LineIndex(lno);
			int end = _edit->LineIndex(lno + _num_delete_line);
			_edit->SetSel(begin, end);
			_edit->ReplaceSel("");
			Reset(lno);
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
	case ID_PARENT:
		n = ADvWndToNode(ADvWnd());
		p = src_node(n);
		if (IsWindow(p) && p->a.w.v) {
			p->a.w.v->Raise();
		}
		break;
	case ID_CLOSE:
		AUiProgramMgr::CloseWindow(aw);
		break;
	case ID_CLOSE_ALL:
		AUiProgramMgr::CloseAllWindows();
		break;
	case ID_MARK_OUTPUT:
		ADvWindow::Print("====================\n");
		break;
	default:
		break;
	}
	return 0;
}

void AUiBodyShell::GCmark() {
	_GCmark(exp);
	_GCmark(exp_stack);
	_GCmark(loc_base);
	_GCmark(ctrl_stack);
}
