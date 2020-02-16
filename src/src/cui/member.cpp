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
#include <gdm/file.h>
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <dev/advcapp.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <cui/cui.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/body.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

int AUiVtblUpdate = 0;

AUiMemberShell::AUiMemberShell(ADvWindow *aw, AGmNode *root_class, AGmNode *class2, int for_ui)
: ADvEditShell(aw) {
	_for_ui = for_ui;
	_root_class = Ref(root_class);
	_class = Ref(class2);
	_member = NULL;
}
AUiMemberShell::~AUiMemberShell() {
	if (_for_ui)
		RemoveValue(_class, AGM_VM_KEY_MEMBER_EDITOR);
	Unref(_class, _root_class);
}

int AUiMemberShell::Reset(long lno) {
	AGmLink *l;
	for (l = next_outlink(_class, NULL);
			l; l = next_outlink(_class, l)) {
		if (l->arc->attr != NmSubClass()) break;	
	}
	int status;
	if (l) {
		status = 0;
		_member = l->arc->dst;
	} else {
		status = 1;
	}
	int flag = ADvEditShell::Reset(lno);
	_status |= status;
	return flag;
}
int AUiMemberShell::More() {
	return (_status != 2);
}
int AUiMemberShell::IsLast() {
	return (_status == 1);
}
int AUiMemberShell::Next() {
	if (_status == 2) return 0;
	if (_status == 1) {
		_status = 2; return 0;
	}
	AGmLink *l = find_outlink(_class, _member);
	for (l = next_outlink(_class, l);
			l; l = next_outlink(_class, l)) {
		if (l->arc->attr != NmSubClass()) break;	
	}
	if (l) {
		_member = l->arc->dst;
	} else {
		_status = 1;
	}
	_lno ++;
	return 0;
}

AGmString* AUiMemberShell::GetLineString() {
	AGmString::DeleteString(_l_str);
	if (_status == 0) {
		member_printf(_l_str, _class, _member);
	} else if (_status == 1) {
		AGmString::CreateString(_l_str, "END");
	}
	return _l_str;
}
int AUiMemberShell::_PreInsertLine(const char* s) {
	AGmString::DeleteString(_e_str);
	AGmFile::CreateStrReadFile(_file, _line, s);
	if (member_scanf(_file, _name1, _member1)) return 1;
	return 0;
}
int AUiMemberShell::_InsertLine(const char*) {
	if (SameProtoMember()) return 1;
	if (same_proto_member) {
		AGmString::CreateString(_e_str, AL_S169);
		return 1;
	}
	AGmLink *l = NULL;
	if (_status == 0) {
		l = find_outlink(_class, _member);
		if (_insert_pos == AUI_SHELL_INSERT_LOWER)
			if (l) l = l->next;
	}
	CreateArc(_class, _member1, UniqueStr(_name1), l);
	AUiVtblUpdate = 0;
	if (_status == 0) {
		_member = _member1;
		if (_depth < _limit_depth) _lno ++;
	} else {
		_member = _member1;
		_status = 0;
	}
	return 0;
}
void AUiMemberShell::_PostInsertLine(const char*) {
	AGmFile::DeleteStrReadFile(_file, _line, _name1, _member1);
}
int AUiMemberShell::_PreReplaceLine(const char *s) {
	AGmString::DeleteString(_e_str);
	AGmFile::CreateStrReadFile(_file, _line, s);
	_end = (_file->MatchIdent("END") && _file->IsEnd());
	if (_end) { _name1 = _member1 = NULL; return 0; }
	if (member_scanf(_file, _name1, _member1)) return 1;
	if (SameProtoMember()) return 1;
	if (same_proto_member && same_proto_member != _member) {
		AGmString::CreateString(_e_str,
			AL_S170);
		return 1;
	}
	if (same_proto_member != _member && Refered(_member, AUiMemberShell::ReferedFromOutside)) {
		AGmString::CreateString(_e_str,
			AL_S054);
		return 1;
	}
	return 0;
}
int AUiMemberShell::_ReplaceLine(const char*) {
	if (_end) return 0;
	AGmLink *l = find_outlink(_class, _member);
	ASSERT(l);
	Set(l->arc->attr, UniqueStr(_name1));
	long info = (long)_member->HI;
	long info1 = (long)_member1->HI;
	if ((info & AUI_FUNC) && (info1 & AUI_FUNC)) {
		Set(_member1->TTH, _member->TTH);
	}
	set_dst_node2(_member, NmVtbl(), NULL);

	AGmLink *ll;
	for (ll = next_inlink(_member, NULL);
			ll; ll = next_inlink(_member, ll)) {
		ll = ll->prev; ReplaceArcDst(ll->next->arc, _member1);
	}

	ReplaceArcDst(l->arc, _member1);
	AUiVtblUpdate = 0;
	_member = _member1;
	return 0;
}
void AUiMemberShell::_PostReplaceLine(const char*) {
	AGmFile::DeleteStrReadFile(_file, _line, _name1, _member1);
}
int AUiMemberShell::_PreDeleteLine() {
	AGmString::DeleteString(_e_str);
	if (_status != 0) {
		AGmString::CreateString(_e_str, AL_S121);
		return 1;
	}
	if (Refered(_member, AUiMemberShell::ReferedFromOutside)) {
		AGmString::CreateString(_e_str,
		AL_S052);
		return 1;
	}
	return 0;
}
int AUiMemberShell::_DeleteLine() {
	AGmLink *l = find_outlink(_class, _member);
	ASSERT(l);
	DeleteArc(l->arc);
	AUiVtblUpdate = 0;
	return 0;
}
int AUiMemberShell::_PreOpenEditor() {
	if (_status != 0) return 1;
	return 0;
}
int AUiMemberShell::_OpenEditor() {
	void *value;
	GetValue(_member, AGM_VM_KEY_BODY_EDITOR, value);
	if (value != NULL) return ((ADvWindow*)value)->Raise();
	
	long info = (long)_member->HI;
	if ((info & AUI_FUNC) == 0)
		return 0;

	ADvWindow *aw = new ADvWindow();
	SetValue(_member, AGM_VM_KEY_BODY_EDITOR, aw);
	AUiBodyShell *shell = new AUiBodyShell(aw, _root_class, _class, _member, 1);
	shell->CreateBody();
	if (aw->CreateEdit(shell, NULL, NULL, 1) == NULL) { delete shell; delete aw; return 1; }
	AGmString *ss = new AGmString(_class->H->a.s.v); ss->Append("::");
	_member_printf2(ss, _class, _member);
	char s[256];
	art_sprintf_1(s, 256, AL_T_BODY, ss->Str());
	delete ss;
	aw->SetTitle(s);
	aw->SetFocus();

	// make body window to be a child of member windiow
	AGmNode *n_body = ADvWndToNode(aw);
	ASSERT(n_body);
	AGmLink *l = find_outlink(RootWindowNode(), n_body);
	ASSERT(l);
	AGmNode *n_member = ADvWndToNode(ADvWnd());
	ASSERT(n_member);
	ReplaceArcSrc(l->arc, n_member);
	return 0;
}


int AUiMemberShell::Up(int& l_from, int& l_to) {
	AGmString::DeleteString(_e_str);
	if (_status != 0) {
		AGmString::CreateString(_e_str, AL_S121);
		return 1;
	}
	AGmLink *l1 = find_outlink(_class, _member);
	ASSERT(l1);
	AGmLink *l2 = prev_outlink3(_class, l1, NmSubClass());
	if (l2 == NULL) {
		AGmString::CreateString(_e_str, AL_S060);
		return 1;
	}
	l_from = (int)(_lno - 1);
	l_to = (int)(_lno);
	AGM_REMOVE(l1); AGM_INSERT(l1, l2);
	return 0;
}
int AUiMemberShell::Down(int& l_from, int& l_to) {
	AGmString::DeleteString(_e_str);
	if (_status != 0) {
		AGmString::CreateString(_e_str, AL_S121);
		return 1;
	}
	AGmLink *l1 = find_outlink(_class, _member);
	ASSERT(l1);
	AGmLink *l2 = next_outlink3(_class, l1, NmSubClass());
	if (l2 == NULL) {
		AGmString::CreateString(_e_str, AL_S057);
		return 1;
	}
	l_from = (int)(_lno);
	l_to = (int)(_lno +1);
	AGM_REMOVE(l2); AGM_INSERT(l2, l1);
	return 0;
}

int AUiMemberShell::member_scanf(AGmFile *f, AGmNode*& _name, AGmNode*& _member) {
	_name = _member = NULL;
	AGmString *s;
	long info = 0;	
	if (f->ReadString(s)) {
		AGmString::CreateString(_e_str, AL_S045);
		return 1;
	}
	if (s->Compare("public:") == 0) {
		info = AUI_PUBLIC;
	} else if (s->Compare("protected:") == 0) {
		info = AUI_PROTECTED;
	} else if (s->Compare("private:") == 0) {
		info = AUI_PRIVATE;
	} else if (s->Compare("funcType:") == 0) {
		info = AUI_FUNC_TYPE;
	} else {
		delete s;
		AGmString::CreateString(_e_str, AL_S045);
		return 1;
	}
	delete s;
	if (f->ReadIdent(s)) {
		AGmString::CreateString(_e_str, AL_S048);
		return 1;
	}
	if (s->Compare("static") == 0) {
		delete s;
		info |= AUI_STATIC;
		if (f->ReadIdent(s)) {
			AGmString::CreateString(_e_str, AL_S156);
			return 1;
		}
	}
	AGmNode *type = Ref(AUiClassShell::NameToType(_root_class, s)); 
	if (type == NULL) {
		char err[256];
		art_sprintf_1(err, 256, AL_S155, s->Str());
		delete s;
		AGmString::CreateString(_e_str, err);
		return 1;
	}
	delete s;
	if (f->ReadIdent(s)) {
		Unref(type);
		AGmString::CreateString(_e_str, AL_S124);
		return 1;
	}
	if (AUiProgramMgr::IsReserveWord(s)) {
		delete s;
		Unref(type);
		AGmString::CreateString(_e_str, AL_S205);
		return 1;
	}
	_name = Ref(String(s));
	AGmNode *arg_dcl = NULL;
	AGmNode **addr = &arg_dcl;
	if (f->MatchString("(")) {
		info |= AUI_FUNC;
		f->ReadSpace();
		if (!f->MatchString(")")) {
			for (;;) {
				AGmNode *arg_type, *arg_name;
				if (f->ReadIdent(s)) {
					Unref(type, _name, arg_dcl);
					AGmString::CreateString(_e_str, AL_S137);
					return 1;
				}
				arg_type = Ref(AUiClassShell::NameToType(_root_class, s));
				if (arg_type == NULL) {
					Unref(type, arg_dcl);
					char err[256];
					art_sprintf_1(err, 256, AL_S136, s->Str());
					delete s;
					AGmString::CreateString(_e_str, err);
					return 1;
				}
				delete s;
				if (f->ReadIdent(s)) {
					Unref(type, _name, arg_dcl, arg_type);
					AGmString::CreateString(_e_str, AL_S138);
					return 1;
				}
				if (AUiProgramMgr::IsReserveWord(s)) {
					delete s;
					Unref(type, _name, arg_dcl, arg_type);
					AGmString::CreateString(_e_str, AL_S203);
					return 1;
				}
				arg_name = UniqueStr(s);	
				delete s;
				AGmNode *n = List(arg_name, arg_type);
				Set(*addr, n);
				addr = &(*addr)->TT;
				Unref(arg_type);
				if (!f->MatchString(",")) break;
			}
			if (!f->MatchString(")")) {
				Unref(type, _name, arg_dcl);
				AGmString::CreateString(_e_str, AL_S001);
				return 1;
			}
		}
	}
	if (!f->MatchString(";")) {
		Unref(type, _name, arg_dcl);
		AGmString::CreateString(_e_str, AL_S006);
		return 1;
	}
	if (!f->IsEnd()) {
		Unref(type, _name, arg_dcl);
		AGmString::CreateString(_e_str, AL_S007);
		return 1;
	}
	AGmNode *_info = Ref(Integer(info));
	if (info & AUI_FUNC) {
		_member = Ref(List(_info, type, NULL, arg_dcl));
	} else {
		_member = Ref(List(_info, type));
	}
	Unref(type, arg_dcl, _info);
	return 0;
}
void AUiMemberShell::member_printf(AGmString*& s, AGmNode *_class, AGmNode *_member) {
	_member_printf0(s, _class, _member);
	_member_printf1(s, _class, _member);
	_member_printf2(s, _class, _member);
	s->Append(";");
}
void AUiMemberShell::_member_printf0(AGmString*& s, AGmNode*, AGmNode *_member) {
	s = new AGmString(32);
	long info = (long)_member->HI;
	if (info & AUI_PUBLIC) s->Append("public: ");	
	if (info & AUI_PROTECTED) s->Append("protected: ");
	if (info & AUI_PRIVATE) s->Append("private: ");
	if (info & AUI_FUNC_TYPE) s->Append("funcType: ");
	if (info & AUI_STATIC) s->Append("static ");
}
void AUiMemberShell::_member_printf1(AGmString*& s, AGmNode*, AGmNode *_member) {
	AGmNode *type = _member->TH;
	AGmNode *type_name = Ref(AUiClassShell::TypeToName(type));
	s->Append(type_name);
	Unref(type_name);
	s->Append(" ");
}
void AUiMemberShell::_member_printf2(AGmString*& s, AGmNode *_class, AGmNode *_member) {
	AGmLink *l = find_outlink(_class, _member);
	s->Append(l->arc->attr);
	long info = (long)_member->HI;
	if (info & AUI_FUNC) {
		AGmNode *arg_dcl = _member->TTTH;
		s->Append("(");
		int first = 1;
		while (arg_dcl) {
			AGmNode *arg_name = arg_dcl->H; arg_dcl = arg_dcl->T;
			AGmNode *arg_type = arg_dcl->H; arg_dcl = arg_dcl->T;
			if (first) {
				first = 0;
			} else {
				s->Append(", ");
			}
			AGmNode *type_name = Ref(AUiClassShell::TypeToName(arg_type));
			s->Append(type_name);
			Unref(type_name);
			s->Append(" ");
			s->Append(arg_name);
		}
		s->Append(")");
	}
}

int AUiMemberShell::SameProtoMember() {
	if ((_member1->HI & AUI_FUNC) == 0) {
		AGmLink *l;
		for (l = next_outlink(_class, NULL, _name1); l;) {
			same_proto_member = l->arc->dst;
			if (same_proto_member->HI & AUI_FUNC) {
				AGmString::CreateString(_e_str, AL_S171);
				return 1;
			} else {
				AUiBodyShell::IsSameType(same_proto_member->TH, _member1->TH);
				return 0;
			}
		}
		same_proto_member = NULL;
		return 0;
	} else {
		AGmLink *l;
		for (l = next_outlink(_class, NULL, _name1);
				l; l = next_outlink(_class, l, _name1)) {
			same_proto_member = l->arc->dst;
			if ((same_proto_member->HI & AUI_FUNC) == 0) {
				AGmString::CreateString(_e_str, AL_S172);
				return 1;
			}
			AGmNode *arg_dcl1 = same_proto_member->TTTH;
			AGmNode *arg_dcl2 = _member1->TTTH;
			AGmNode *a1, *a2;
			for (a1 = arg_dcl1, a2 = arg_dcl2;
					a1 && a2;
					a1 = a1->TT, a2 = a2->TT) {
				if (!AUiBodyShell::IsSameType(a1->TH, a2->TH)) break;
			}
			if (a1 == NULL && a2 == NULL) {
				AUiBodyShell::IsSameType(same_proto_member->TH, _member1->TH);
				return 0;
			}
		}
		same_proto_member = NULL;
		return 0;
	}
}

void AUiMemberShell::update_vfunc_tbl(AGmNode *root_class, AGmNode *_class, AGmNode *_member) {
	AGmLink *l = find_outlink(_class, _member);
	ASSERT(l);
	AGmNode *name = l->arc->attr;
	AGmNode *func_dcl = l->arc->dst;
	AGmNode *arg_dcl2 = func_dcl->TTTH;
	_update_vfunc_tbl1(NULL, NULL, root_class, name, arg_dcl2);
}
void AUiMemberShell::_update_vfunc_tbl1(AGmNode *tbl, AGmNode *func_dcl2,
			AGmNode *def_cl, AGmNode *name, AGmNode *arg_dcl2) {	
	_update_vfunc_tbl2(tbl, func_dcl2, def_cl, name, arg_dcl2);
	AGmLink *l;
	for (l = next_outlink2(def_cl, NULL, NmSubClass());
			l; l = next_outlink2(def_cl, l, NmSubClass())) {
		_update_vfunc_tbl1(tbl, func_dcl2, l->arc->dst, name, arg_dcl2);
	}
}
void AUiMemberShell::_update_vfunc_tbl2(AGmNode *&tbl, AGmNode *&func_dcl2,
			AGmNode *def_cl, AGmNode *name, AGmNode *arg_dcl2) {
	AGmNode *candidate = NULL;
	AGmLink *l;
	for (l = next_outlink(def_cl, NULL, name);
			l; l = next_outlink(def_cl, l, name)) {
		if (l->arc->attr == NmSubClass()) continue;
		AGmNode *func_dcl = l->arc->dst;
		long info = (long)func_dcl->HI;
		if (info & AUI_STATIC) continue;
		if (!(info & AUI_FUNC)) continue;
		AUiBodyShell::Overload(func_dcl, arg_dcl2, candidate);	
	}
	AGmArc *a;
	if (candidate) {
		func_dcl2 = candidate;
		if (tbl == NULL) {
			tbl = Cons(NULL);
		}
		a = set_dst_node2(func_dcl2, NmVtbl(), tbl);
		ASSERT(a);
		a->incl_assoc |= AGM_ARC_ASSOC;
	}
	if (tbl) {
		a = set_dst_node(tbl, def_cl, func_dcl2);
		ASSERT(a);
		a->incl_assoc |= AGM_ARC_ASSOC;
	}
}

void AUiMemberShell::CleanVtbl(AGmNode* n) {
	AGmLink *l;
	for (l = next_outlink(n, NULL);
			l; l = next_outlink(n, l)) {
		if (l->arc->attr != NmSubClass()) {
			set_dst_node(l->arc->dst, NmVtbl(), NULL);
		} else {
			CleanVtbl(l->arc->dst);
		}
	}
}
void AUiMemberShell::UpdateVtbl(AGmNode* root, AGmNode* n) {
	AGmLink *l;
	for (l = next_outlink(n, NULL);
			l; l = next_outlink(n, l)) {
		if (l->arc->attr != NmSubClass()) {
			if (dst_node(l->arc->dst, NmVtbl()) == NULL) {
				AGmNode *name = l->arc->attr;
				AGmNode *func_dcl = l->arc->dst;
				long info = (long)func_dcl->HI;
				if (!(info & AUI_STATIC) && (info & AUI_FUNC)) {
					AGmNode *arg_dcl2 = func_dcl->TTTH;
					_update_vfunc_tbl1(NULL, NULL, root, name, arg_dcl2);
				}
			}
		} else {
			UpdateVtbl(root, l->arc->dst);
		}
	}
}

int AUiMemberShell::ReferedFromOutside(AGmLink *l) {
	if ((l->arc->src->flag & AGM_FLAG_SELF) != 0)
		return 0;
	if (dst_node(l->arc->dst, NmVtbl()) == l->arc->src)
		return 0;
	return 1;
}

#define ID_NEW			100
#define ID_OPEN 		110
#define ID_SAVE 		120
#define ID_SAVE_AS		130
#define ID_PRINT		140
#define ID_EXIT 		150
#define ID_INSERT	200
#define ID_APPEND	210
#define ID_UP		220
#define ID_DOWN 	230
#define ID_DELETE	240
#define ID_REDRAW		300
#define ID_PARENT		310
#define ID_CLOSE		320
#define ID_CLOSE_ALL	330
#define ID_TEST_EXEC	400
#define ID_EXEC 		420
#define ID_MARK_OUTPUT	500

static ADvMenuInfo file_menu[] = {
	{ AL_M_NEW, 	ID_NEW, 	NULL},
	{ AL_M_OPEN,	ID_OPEN,	NULL},
	{ AL_M_SAVE,	ID_SAVE,	NULL},
	{ AL_M_SAVE_AS, ID_SAVE_AS, NULL},
	{ AL_M_PRINT,	ID_PRINT,	NULL},
	{ AL_M_EXIT,	ID_EXIT,		NULL},
	{ NULL, 		0,				NULL}
};
static ADvMenuInfo pos_menu[] = {
	{ AL_M_UPPER,	ID_INSERT,	NULL},
	{ AL_M_LOWER,	ID_APPEND,	NULL},
	{ NULL, 0,		NULL}
};
static ADvMenuInfo edit_menu[] = {
	{ AL_M_POS, 	0,			pos_menu},
	{ AL_M_UP,		ID_UP,		NULL},
	{ AL_M_DOWN,	ID_DOWN,	NULL},
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
static ADvMenuInfo exec_menu[] = {
	{ AL_M_TEST_EXEC,	ID_TEST_EXEC,	NULL},
	{ AL_M_EXEC,		ID_EXEC,		NULL},
	{ NULL, 			0,				NULL}
};
static ADvMenuInfo misc_menu[] = {
	{ AL_M_MARK_OUTPUT, ID_MARK_OUTPUT, NULL},
	{ NULL, 			0,				NULL}
};
static ADvMenuInfo member_menu[] = {
	{ AL_M_FILE,	0,	file_menu},
	{ AL_M_EDIT,	0,	edit_menu},
	{ AL_M_WINDOW,	0,	window_menu},
	{ AL_M_INTERP,	0,	exec_menu},
	{ AL_M_MISC,	0,	misc_menu},
	{ NULL, 		0,	NULL}
};

int AUiMemberShell::LeftPress(ADvWindow *aw, AGmNode *e) {
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
int AUiMemberShell::LeftRelease(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::LeftRelease(aw, e);
}
int AUiMemberShell::LeftDoubleClick(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::LeftDoubleClick(aw, e);
}
int AUiMemberShell::RightPress(ADvWindow *aw, AGmNode *e) {
	ASSERT(_edit);
	long from, to, line_from, line_to;
	_edit->GetSel(from, to);
	line_from = _edit->LineFromChar(from);
	line_to = _edit->LineFromChar(to);
	if (line_from != line_to)
		return ADvEditShell::RightPress(aw, e);
	return Menu(aw, e, line_from);
}
int AUiMemberShell::RightRelease(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::RightRelease(aw, e);
}
int AUiMemberShell::RightDoubleClick(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::RightDoubleClick(aw, e);
}
int AUiMemberShell::MouseMove(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::MouseMove(aw, e);
}
int AUiMemberShell::KeyPressFunc(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyPressFunc(aw, e);
}
int AUiMemberShell::KeyReleaseFunc(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyReleaseFunc(aw, e);
}
int AUiMemberShell::KeyChar(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyChar(aw, e);
}
int AUiMemberShell::Menu(ADvWindow *aw, AGmNode *e, long line_from) {
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
	AGmNode *n, *p;
	int l_from, l_to;
	long id = ADvWindow::MenuSelect(aw, e, member_menu);
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
			int end = _edit->LineIndex(lno + 1);
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
	case ID_TEST_EXEC:
		Reset(lno);
		if ((_member->HI & AUI_PUBLIC) &&
				(_member->HI & AUI_STATIC) &&
				(_member->HI & AUI_FUNC) &&
				_member->TTTH == NULL)	{
			if (AUiVtblUpdate == 0) {
				AUiVtblUpdate = 1;
				ADvBeginWaitCursor();
				AUiMemberShell::CleanVtbl(_root_class);
				AUiMemberShell::UpdateVtbl(_root_class, _root_class);
				ADvEndWaitCursor();
			}
			AIpProcessMgr *pm;
			pm = AIpProcessMgr::Instance();
			ASSERT(pm);
			pm->CreateProcess(_root_class, NULL, _member);
		} else {
			ADvWindow::Print(AL_S134);
		}
		break;
	case ID_EXEC:
		Reset(lno);
		if ((_member->HI & AUI_PUBLIC) &&
				(_member->HI & AUI_STATIC) &&
				(_member->HI & AUI_FUNC) &&
				_member->TTTH == NULL)	{
			if (AUiVtblUpdate == 0) {
				AUiVtblUpdate = 1;
				ADvBeginWaitCursor();
				AUiMemberShell::CleanVtbl(_root_class);
				AUiMemberShell::UpdateVtbl(_root_class, _root_class);
				ADvEndWaitCursor();
			}
			AUiProgramMgr::CloseAllWindows();
			AIpProcessMgr *pm;
			pm = AIpProcessMgr::Instance();
			ASSERT(pm);
			pm->CreateProcess(_root_class, NULL, _member);
		} else {
			ADvWindow::Print(AL_S135);
		}
		break;
	case ID_MARK_OUTPUT:
		ADvWindow::Print("====================\n");
		break;
	default:
		break;
	}
	return 0;
}

int AUiMemberShell::OverWriteLines(int l_from, int l_to) {
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

void AUiMemberShell::GCmark() {
}
