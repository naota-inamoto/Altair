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
#include <gdm/error.h>
#include <gdm/pict.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <interp/interp.h>
#include <cui/cui.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/cui.h>
#include <cui/project.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#define HIER_CHILD			0
#define HIER_LAST_CHILD 	1
#define HIER_BROTHER		2
#define HIER_ELDER_BROTHER	3

AUiClassShell::AUiClassShell(ADvWindow *aw, AGmNode *root_class, int for_ui)
: ADvEditShell(aw) {
	_for_ui = for_ui;
	_root_class = Ref(root_class);
	_class_stack = NULL;
	_create_pos = HIER_CHILD;
}
AUiClassShell::~AUiClassShell() {
	if (_for_ui)
		RemoveValue(_root_class, AGM_VM_KEY_CLASS_EDITOR);
	Unref(_class_stack, _root_class);
}

int AUiClassShell::Reset(long lno) {
	Unref(_class_stack); _class_stack = NULL;
	Push(_class_stack, _root_class);
	return ADvEditShell::Reset(lno);
}
int AUiClassShell::More() {
	return (_class_stack != NULL);
}
int  AUiClassShell::IsLast() {
	return (_class_stack != NULL && _status == 1);
}
int AUiClassShell::Next() {
	if (_class_stack == NULL) return 0;
	if (_status == 1) { Pop(_class_stack); return 0; }
	AGmNode *_class = _class_stack->H;
	AGmLink *l = next_outlink2(_class, NULL, NmSubClass());
	if (l) { /* ----- child ----- */
		if (_depth < _limit_depth) _lno ++;
		_depth ++;
		return Push(_class_stack, l->arc->dst);
	}
	for (;;) {
		if (_class_stack->T == NULL) { /* ----- last ----- */
			if (_depth < _limit_depth) _lno ++;
			_status = 1;
			return 0;
		}
		AGmNode *_super = _class_stack->TH;
		l = find_outlink(_super, _class);
		l = next_outlink2(_super, l, NmSubClass());
		if (l) { /* ----- brother ----- */
			if (_depth < _limit_depth) _lno ++;
			Set(_class_stack->H, l->arc->dst);
			return 0;
		}
		Pop(_class_stack); /* ----- parent ----- */
		_depth --;
		_class = _class_stack->H;
	}
}

AGmString* AUiClassShell::GetLineString() {
	AGmString::DeleteString(_l_str);
	if (_class_stack == NULL) return NULL;
	AGmNode *_class = _class_stack->H;
	if (_status == 0) {
		if (!IsList(_class)) return NULL;
		if (!IsString(_class->H)) return NULL;
		
		char s[256]; s[0] = 0;
		for (int i = 0; i < _depth; i++) art_strcat(s, 256, "\t");
		art_strcat(s, 256, _class->H->a.s.v->Str());
		AGmString::CreateString(_l_str, s);
	} else {
		AGmString::CreateString(_l_str, "END");
	}
	return _l_str;
}
int AUiClassShell::_PreInsertLine(const char* s) {
	_num_overwrite_line = 0;
	AGmString::DeleteString(_e_str);
	AGmFile::CreateStrReadFile(_file, _line, s);
	switch (_create_pos) {
	case HIER_CHILD:
		if (_class_stack == NULL) {
			AGmString::CreateString(_e_str, AL_S116);
			return 1;
		}
		break;
	case HIER_BROTHER:
		if (_class_stack == NULL || _class_stack->T == NULL) {
			AGmString::CreateString(_e_str, AL_S116);
			return 1;
		}
		_num_overwrite_line = NumDescendant(_class_stack->H);
		break;
	case HIER_ELDER_BROTHER:
		if (_class_stack == NULL || _class_stack->T == NULL) {
			AGmString::CreateString(_e_str, AL_S116);
			return 1;
		}
		break;
	}

	if (_file->ReadIdent(_name)) {
		AGmString::CreateString(_e_str, AL_S078);
		return 1;
	}
	Ref(_name);
	if (!_file->IsEnd()) {
		AGmString::CreateString(_e_str, AL_S177);
		return 1;
	}
	AGmNode *type = NameToType(_root_class, _name);
	if (type) {
		RefUnref(type);
		AGmString::CreateString(_e_str, AL_S168);
		return 1;
	}
	return 0;
}
int AUiClassShell::_InsertLine(const char*) {
	AGmNode *n = NULL; AGmLink *l = NULL;
	switch (_create_pos) {
	case HIER_CHILD:
		n = _class_stack->H;
		ASSERT(n);
		if (n->body) l = n->body->out;
		if (l && !IsLast()) l = l->next;
		break;
	case HIER_LAST_CHILD:
		n = _class_stack->H;
		ASSERT(n);
		if (n->body) l = n->body->out;
		break;
	case HIER_BROTHER:
		n = _class_stack->TH;
		ASSERT(n);
		l = find_outlink(n, _class_stack->H);
		ASSERT(l);
		l = l->next;
		break;
	case HIER_ELDER_BROTHER:
		n = _class_stack->TH;
		ASSERT(n);
		l = find_outlink(n, _class_stack->H);
		ASSERT(l);
		break;
	default:
		ASSERT(0);
	}

	ASSERT(n);
	AGmNode *n2 = Cons(UniqueStr(_name), NULL);
	CreateArc(n, n2, NmSubClass(), l);
	AUiVtblUpdate = 0;

	switch (_create_pos) {
	case HIER_CHILD:
		Push(_class_stack, n2);
		if (IsLast()) _status = 0;
		_depth ++;
		if (_depth < _limit_depth) _lno ++;
		break;
	case HIER_BROTHER:
		Set(_class_stack->H, n2);
		if (_depth < _limit_depth) _lno ++;
		break;
	case HIER_ELDER_BROTHER:
		Set(_class_stack->H, n2);
	default:
		break;
	}
	return 0;
}
void AUiClassShell::_PostInsertLine(const char*) {
	AGmFile::DeleteStrReadFile(_file, _line, _name);
}
int AUiClassShell::_PreReplaceLine(const char* s) {
	AGmString::DeleteString(_e_str);
	AGmFile::CreateStrReadFile(_file, _line, s);

	if (_file->ReadIdent(_name)) {
		AGmString::CreateString(_e_str, AL_S078);
		return 1;
	}
	Ref(_name);
	if (!_file->IsEnd()) {
		AGmString::CreateString(_e_str, AL_S177);
		return 1;
	}
	AGmNode *n = NULL;
	if (_class_stack) n = _class_stack->H;
	AGmNode *n2 = Ref(NameToType(_root_class, _name));
	if (n2 && dst_node2(n2, NmType()) != n) {
		Unref(n2);
		AGmString::CreateString(_e_str, AL_S168);
		return 1;
	}

	if (n == _root_class && n->H->a.s.v->Compare(_name->a.s.v) != 0) {
		Unref(n2);
		AGmString::CreateString(_e_str, AL_S180);
		return 1;
	}
	AGmLink *proj_lnk = AUiProjectShell::SearchProject(_root_class);
	ASSERT(proj_lnk);
	AGmNode *proj = proj_lnk->arc->dst;
	AGmLink *l;
	for (l = next_outlink(proj->TTH, NULL);
			l; l = next_outlink(proj->TTH, l)) {
		AGmNode *sub_class = AUiClassShell::NameToClass(proj->H, l->arc->attr);
		ASSERT(sub_class);
		if (sub_class == n && n->H->a.s.v->Compare(_name->a.s.v) != 0) {
			Unref(n2);
			AGmString::CreateString(_e_str, AL_S181);
			return 1;
		}
	}

	Unref(n2);
	return 0;
}
int AUiClassShell::_ReplaceLine(const char*) {
	if (_name->a.s.v->Compare("END") == 0) return 0;
	AGmNode *n = NULL;
	if (_class_stack) n = _class_stack->H;
	if (n == NULL || !IsList(n)) {
		return 1;
	}
	Set(n->H, UniqueStr(_name));
	return 0;
}
void AUiClassShell::_PostReplaceLine(const char*) {
	AGmFile::DeleteStrReadFile(_file, _line, _name);
}
int AUiClassShell::_PreDeleteLine() {
	AGmString::DeleteString(_e_str);
	if (_status != 0) {
		AGmString::CreateString(_e_str, AL_S077);
		return 1;
	}
	if (_class_stack->T == NULL) {
		AGmString::CreateString(_e_str, AL_S128);
		return 1;
	}
	AGmNode *n = _class_stack->H;
	if (Refered(n, AUiClassShell::ReferedFromOutside)) {
		AGmString::CreateString(_e_str,
		AL_S052);
		return 1;
	}
	return 0;
}
int AUiClassShell::_DeleteLine() {
	AGmLink *l = find_outlink(_class_stack->TH, _class_stack->H);
	ASSERT(l);
	_num_delete_line = NumDescendant(l->arc->dst);
	DeleteArc(l->arc);
	AUiVtblUpdate = 0;
	return 0;
}

int AUiClassShell::_PreOpenEditor() {
	if (_class_stack == NULL) return 1;
	return 0;
}
int AUiClassShell::_OpenEditor() {
	AGmNode *class_node = _class_stack->H;
	void *value;
	GetValue(class_node, AGM_VM_KEY_MEMBER_EDITOR, value);
	if (value != NULL) return ((ADvWindow*)value)->Raise();

	ADvWindow *aw = new ADvWindow();
	SetValue(class_node, AGM_VM_KEY_MEMBER_EDITOR, aw);
	AUiMemberShell *shell = new AUiMemberShell(aw, _root_class, class_node, 1);
	if (aw->CreateEdit(shell) == NULL) { delete shell; delete aw; return 1; }
	char s[256];
	art_sprintf_1(s, 256, AL_T_MEMBER, class_node->H->a.s.v->Str());
	aw->SetTitle(s);
	aw->SetFocus();

	// make member window to be a child of class window
	AGmNode *n_member = ADvWndToNode(aw);
	ASSERT(n_member);
	AGmLink *l = find_outlink(RootWindowNode(), n_member);
	ASSERT(l);
	AGmNode *n_class = ADvWndToNode(ADvWnd());
	ASSERT(n_class);
	ReplaceArcSrc(l->arc, n_class);
	return 0;
}


int AUiClassShell::Up(int& l_from, int& l_to) {
	AGmString::DeleteString(_e_str);
	if (_class_stack == NULL || _class_stack->T == NULL) {
		AGmString::CreateString(_e_str, AL_S159);
		return 1;
	}
	AGmLink *l1 = find_outlink(_class_stack->TH, _class_stack->H);
	ASSERT(l1);
	AGmLink *l2 = prev_outlink2(_class_stack->TH, l1, NmSubClass());
	if (l2 == NULL) {
		AGmString::CreateString(_e_str, AL_S058);
		return 1;
	}
	l_from = (int)(_lno - NumDescendant(l2->arc->dst));
	l_to = (int)(_lno + NumDescendant(l1->arc->dst) -1);
	AGM_REMOVE(l1); AGM_INSERT(l1, l2);
	return 0;
}
int AUiClassShell::Down(int& l_from, int& l_to) {
	AGmString::DeleteString(_e_str);
	if (_class_stack == NULL || _class_stack->T == NULL) {
		AGmString::CreateString(_e_str, AL_S159);
		return 1;
	}
	AGmLink *l1 = find_outlink(_class_stack->TH, _class_stack->H);
	ASSERT(l1);
	AGmLink *l2 = next_outlink2(_class_stack->TH, l1, NmSubClass());
	if (l2 == NULL) {
		AGmString::CreateString(_e_str, AL_S055);
		return 1;
	}
	l_from = (int)(_lno);
	l_to = (int)(_lno + NumDescendant(l1->arc->dst) + NumDescendant(l2->arc->dst) -1);
	AGM_REMOVE(l2); AGM_INSERT(l2, l1);
	return 0;
}
int AUiClassShell::Left(int& l_from, int& l_to) {
	AGmString::DeleteString(_e_str);
	if (_class_stack == NULL || _class_stack->T == NULL || _class_stack->TT == NULL) {
		AGmString::CreateString(_e_str, AL_S159);
		return 1;
	}
	AGmNode *n = _class_stack->H;
	AGmNode *p = _class_stack->TH;
	AGmNode *pp = _class_stack->TTH;
	AGmLink *l = find_outlink(p, n);
	ASSERT(l);
	AGmLink *ll = find_outlink(pp, p);
	ASSERT(ll);

	// brother -> child
	AGmLink *bl = next_outlink2(p, l, NmSubClass());
	if (bl) {
		ReplaceArcSrc(bl->arc, n);
	}
	// parent -> grand parent
	ll = ll->next;
	AGM_REMOVE(l); AGM_INSERT(l, ll);
	l->arc->src = pp;
	AUiVtblUpdate = 0;

	l_from = (int)(_lno);
	l_to = (int)(_lno + NumDescendant(l->arc->dst) -1);
	return 0;
}
int AUiClassShell::Right(int& l_from, int& l_to) {
	AGmString::DeleteString(_e_str);
	if (_class_stack == NULL || _class_stack->T == NULL) {
		AGmString::CreateString(_e_str, AL_S159);
		return 1;
	}
	AGmNode *n = _class_stack->H;
	AGmNode *p = _class_stack->TH;
	AGmLink *l = find_outlink(p, n);
	ASSERT(l);
	if (l->prev == p->body->out) {
		AGmString::CreateString(_e_str, AL_S157);
		return 1;
	}

	// elder brother -> parent
	AGmNode *ebn = l->prev->arc->dst;
	ReplaceArcSrc(l->arc, ebn);
	AUiVtblUpdate = 0;

	l_from = (int)(_lno);
	l_to = (int)(_lno + NumDescendant(l->arc->dst) -1);
	return 0;
}

AGmNode *AUiClassShell::NameToClass(AGmNode* n, const char* name) {
	ASSERT(n);
	ASSERT(n->type == AGM_NODE_LIST);
	ASSERT(n->H);
	ASSERT(n->H->type == AGM_NODE_STRING);
	ASSERT(n->H->a.s.v);
	if (n->H->a.s.v->Compare(name) == 0) return n;
	AGmLink *l;
	for (l = AGm::next_outlink(n, NULL, NmSubClass());
			l; l = AGm::next_outlink(n, l, NmSubClass())) {
		AGmNode *n2 = NameToClass(l->arc->dst, name);
		if (n2) return n2;
	}
	return NULL;
}
AGmNode *AUiClassShell::NameToClass(AGmNode* n, const AGmString* s) {
	return NameToClass(n, s->Str());
}
AGmNode *AUiClassShell::NameToClass(AGmNode* n, const AGmNode* name) {
	if (!IsString(name)) return NULL;
	if (name->a.s.v == NULL) return NULL;
	return NameToClass(n, name->a.s.v->Str());	
}
AGmNode *AUiClassShell::NameToClassFuncType(AGmNode* n, const char* name) {
	ASSERT(n);
	ASSERT(n->type == AGM_NODE_LIST);
	ASSERT(n->H);
	ASSERT(n->H->type == AGM_NODE_STRING);
	ASSERT(n->H->a.s.v);
	AGmLink *l;
	for (l = AGm::next_outlink(n, NULL);
			l; l = AGm::next_outlink(n, l)) {
		if (l->arc->attr == NmSubClass()) {
			AGmNode *n2 = NameToClassFuncType(l->arc->dst, name);
			if (n2) return n2;
		} else {
			AGmNode *s = l->arc->attr;
			AGmNode *n2 = l->arc->dst;
			if (!IsString(s)) continue;
			if (!(n2->HI & AUI_FUNC_TYPE)) continue;
			if (!(n2->HI & AUI_STATIC)) continue;
			if (s->a.s.v->Compare(name) == 0) return n2;
		}
	}
	return NULL;
}
AGmNode *AUiClassShell::NameToClassFuncType(AGmNode* n, const AGmString* s) {
	return NameToClassFuncType(n, s->Str());
}
AGmNode *AUiClassShell::NameToClassFuncType(AGmNode *n, const AGmNode* name) {
	if (!IsString(name)) return NULL;
	if (name->a.s.v == NULL) return NULL;
	return NameToClassFuncType(n, name->a.s.v->Str());	
}
AGmNode *AUiClassShell::NameToObjFuncType(AGmNode* n, const char* name) {
	ASSERT(n);
	ASSERT(n->type == AGM_NODE_LIST);
	ASSERT(n->H);
	ASSERT(n->H->type == AGM_NODE_STRING);
	ASSERT(n->H->a.s.v);
	AGmLink *l;
	for (l = AGm::next_outlink(n, NULL);
			l; l = AGm::next_outlink(n, l)) {
		if (l->arc->attr == NmSubClass()) {
			AGmNode *n2 = NameToObjFuncType(l->arc->dst, name);
			if (n2) return n2;
		} else {
			AGmNode *s = l->arc->attr;
			AGmNode *n2 = l->arc->dst;
			if (!IsString(s)) continue;
			if (!(n2->HI & AUI_FUNC_TYPE)) continue;
			if (n2->HI & AUI_STATIC) continue;
			if (s->a.s.v->Compare(name) == 0) return n2;
		}
	}
	return NULL;
}
AGmNode *AUiClassShell::NameToObjFuncType(AGmNode* n, const AGmString* s) {
	return NameToObjFuncType(n, s->Str());
}
AGmNode *AUiClassShell::NameToObjFuncType(AGmNode *n, const AGmNode* name) {
	if (!IsString(name)) return NULL;
	if (name->a.s.v == NULL) return NULL;
	return NameToObjFuncType(n, name->a.s.v->Str());	
}
AGmNode *AUiClassShell::TypeToName(AGmNode *type) {
	ASSERT(type);
	AGmNode *n;
	switch (type->I) {
	case AUI_TYPE_INTEGER:		return String("integer");
	case AUI_TYPE_REAL: 		return String("real");
	case AUI_TYPE_LIST: 		return String("list");
	case AUI_TYPE_ARRAY:		return String("array");
	case AUI_TYPE_DST_ITR:		return String("dstItr");
	case AUI_TYPE_SRC_ITR:		return String("srcItr");
	case AUI_TYPE_STRING:		return String("string");
	case AUI_TYPE_MATRIX:		return String("matrix");
	case AUI_TYPE_FILE: 		return String("file");
	case AUI_TYPE_WINDOW:		return String("window");
	case AUI_TYPE_VARIANT:		return String("variant");
	case AUI_TYPE_VOID: 		return String("void");
	case AUI_TYPE_CLASS:
		n = dst_node2(type, NmType());
		ASSERT(n);
		return String(n->H->a.s.v->Str());
	case AUI_TYPE_CLASS_FUNC:
	case AUI_TYPE_OBJ_FUNC:
		n = dst_node2(type, NmType());
		ASSERT(n);
		AGmLink *l;
		for (l = next_inlink(n, NULL);
				l; l = next_inlink(n, l)) {
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0)
				break;
		}
		ASSERT(l);
		return String(l->arc->attr->a.s.v->Str());
	default:
		ASSERT(0);
	}
	return String("");
}
AGmNode *AUiClassShell::NameToType(AGmNode* n, const AGmString *s) {
	if (s->Compare("integer") == 0) return Integer(AUI_TYPE_INTEGER);
	if (s->Compare("real")	  == 0) return Integer(AUI_TYPE_REAL);
	if (s->Compare("list")	  == 0) return Integer(AUI_TYPE_LIST);
	if (s->Compare("array")   == 0) return Integer(AUI_TYPE_ARRAY);
	if (s->Compare("dstItr")  == 0) return Integer(AUI_TYPE_DST_ITR);
	if (s->Compare("srcItr")  == 0) return Integer(AUI_TYPE_SRC_ITR);
	if (s->Compare("string")  == 0) return Integer(AUI_TYPE_STRING);
	if (s->Compare("matrix")  == 0) return Integer(AUI_TYPE_MATRIX);
	if (s->Compare("file")	  == 0) return Integer(AUI_TYPE_FILE);
	if (s->Compare("window")  == 0) return Integer(AUI_TYPE_WINDOW);
	if (s->Compare("variant") == 0) return Integer(AUI_TYPE_VARIANT);
	if (s->Compare("void") == 0)	return Integer(AUI_TYPE_VOID);
	AGmNode *n2, *n3; AGmArc *a;
	n2 = NameToClass(n, s);
	if (n2) {
		n3 = Integer(AUI_TYPE_CLASS);
		a = CreateArc(n3, n2, NmType());
		a->incl_assoc |= AGM_ARC_ASSOC;
		return n3;
	}
	n2 = NameToClassFuncType(n, s);
	if (n2) {
		n3 = Integer(AUI_TYPE_CLASS_FUNC);
		a = CreateArc(n3, n2, NmType());
		a->incl_assoc |= AGM_ARC_ASSOC;
		return n3;
	}
	n2 = NameToObjFuncType(n, s);
	if (n2) {
		n3 = Integer(AUI_TYPE_OBJ_FUNC);
		a = CreateArc(n3, n2, NmType());
		a->incl_assoc |= AGM_ARC_ASSOC;
		return n3;
	}
	return NULL;
}
AGmNode *AUiClassShell::NameToType(AGmNode *n, const AGmNode *name) {
	if (!IsString(name)) return NULL;
	return NameToType(n, name->a.s.v);
}
AGmNode *AUiClassShell::NameToType(AGmNode *n, const char *name) {
	AGmString *s = new AGmString(name);
	AGmNode *n1 = NameToType(n, s);
	delete s;
	return n1;
}
int AUiClassShell::LinkClassObj(AGmNode* root_class, AGmNode *n) {
	int err = _LinkClassObj(root_class, n);
	UnmarkNode(n, AGM_FLAG_MARK);
	return err;
}
int AUiClassShell::_LinkClassObj(AGmNode* root_class, AGmNode *n) {
	int err;
	while (n) {
		if (n->flag & AGM_FLAG_MARK) break;
		n->flag |= AGM_FLAG_MARK;
		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_STRING:
		case AGM_NODE_MATRIX:
		case AGM_NODE_FILE:
		case AGM_NODE_WINDOW:
		case AGM_NODE_BITMAP:
		case AGM_NODE_PTR:
		case AGM_NODE_BINARY:
			t = n->a.i.t;	break;
		case AGM_NODE_REAL:
		case AGM_NODE_BIGINT:
		case AGM_NODE_GFP:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;		break;
		case AGM_NODE_LIST:
			err = _LinkClassObj(root_class, n->a.l.h);
			if (err) return err;
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
#if	_WINDOWS && _64BIT
			long long i, size;
#else
			long i, size;
#endif
			size = n->a.a.size;
			for (i = 0; i < size; i++) {
				err = _LinkClassObj(root_class, n->a.a.v[i]);
				if (err) return err;
			}
			t = NULL;
			break;
		case AGM_NODE_OBJECT:
			AGmNode *cn;
			ASSERT(IsString(n->a.o.class_name));
			cn = NameToClass(root_class, n->a.o.class_name->a.s.v);
			if (cn == NULL) {
				char err[256];
				art_sprintf_1(err, 256, AL_S068, n->a.o.class_name->a.s.v->Str());
				ADvWindow::Print(err);
				return 1;
			}
			Ref(cn); Unref(n->a.o.class_id);
			n->a.o.class_id = cn;
			t = NULL;	break;
		default: ASSERT(0);
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			err = _LinkClassObj(root_class, l->arc->attr);
			if (err) return err;
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
				err = _LinkClassObj(root_class, l->arc->dst);
				if (err) return err;
			} else {
			}
		}
		n = t;
	}
	return 0;
}

int AUiClassShell::NumDescendant(AGmNode *n) {
	int num = 1;
	AGmLink *l;
	for (l = next_outlink(n, NULL, NmSubClass());
			l; l = next_outlink(n, l, NmSubClass())) {
		num += NumDescendant(l->arc->dst);
	}
	return num;
}
AGmArc* AUiClassShell::ClassOfMember(AGmNode* n) {
	AGmLink *l;
	for (l = next_inlink(n, NULL);
			l; l = next_inlink(n, l)) {
		if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) break;
	}
	ASSERT(l);
	return l->arc;
}

int AUiClassShell::ReferedFromOutside(AGmLink *l) {
	if ((l->arc->src->flag & AGM_FLAG_SELF) != 0)
		return 0;
	if (dst_node(l->arc->dst, NmVtbl()) == l->arc->src)
		return 0;
	return 1;
}
int AUiClassShell::ReferToOutside(AGmLink *l) {
	if ((l->arc->dst->flag & AGM_FLAG_SELF) != 0)
		return 0;
	if (l->arc->attr == NmVtbl())
		return 0;
	return 1;
}

#define ID_NEW			100
#define ID_OPEN 		110
#define ID_SAVE 		120
#define ID_SAVE_AS		130
#define ID_PRINT		140
#define ID_EXIT 		150
#define ID_ELDER_BROTHER	200
#define ID_CHILD			210
#define ID_BROTHER			220
#define ID_UP		230
#define ID_DOWN 	240
#define ID_LEFT 	250
#define ID_RIGHT	260
#define ID_DELETE	270
#define ID_REGIST_LIST	300
#define ID_REGIST		310
#define ID_UNREGIST 	320
#define ID_TEXT_LIB 	400
#define ID_GRAPH_LIB	410
#define ID_TEXT2_CLEAR	420
#define ID_TEXT2_LIST	430
#define ID_TEXT2_ADD	440
#define ID_TEXT2_CREATE 450
#define ID_LD_LIB		460
#define ID_LD_LIB_TMP	470
#define ID_REDRAW		500
#define ID_CLOSE		510
#define ID_CLOSE_ALL	520
#define ID_MARK_OUTPUT	600
#define ID_MEMORY_SIZE	610
#define ID_GC			620
#define ID_CLEAR_VTBL	630

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
	{ AL_M_ELDER,	ID_ELDER_BROTHER,	NULL},
	{ AL_M_BROTHER, ID_BROTHER, 		NULL},
	{ AL_M_CHILD,	ID_CHILD,			NULL},
	{ NULL, 		0,					NULL}
};
static ADvMenuInfo edit_menu[] = {
	{ AL_M_POS, 	0,			pos_menu},
	{ AL_M_UP,		ID_UP,		NULL},
	{ AL_M_DOWN,	ID_DOWN,	NULL},
	{ AL_M_LEFT,	ID_LEFT,	NULL},
	{ AL_M_RIGHT,	ID_RIGHT,	NULL},
	{ AL_M_DELETE,	ID_DELETE,	NULL},
	{ NULL, 		0,			NULL}
};
static ADvMenuInfo text2_lib_menu[] = {
	{ AL_M_TEXT2_CLEAR, ID_TEXT2_CLEAR },
	{ AL_M_TEXT2_LIST,	ID_TEXT2_LIST },
	{ AL_M_TEXT2_ADD,	ID_TEXT2_ADD },
	{ AL_M_TEXT2_CREATE,ID_TEXT2_CREATE },
	{ NULL, 			0,				NULL }
};
static ADvMenuInfo mk_lib_menu[] = {
	{ AL_M_MKLIB_TEXT,	ID_TEXT_LIB,	NULL},
	{ AL_M_MKLIB_GRAPH, ID_GRAPH_LIB,	NULL},
	{ AL_M_MKLIB_TEXT2, 0,				text2_lib_menu },
	{ NULL, 			0,				NULL}
};
static ADvMenuInfo regist_menu[] = {
	{ AL_M_REGIST_CLASS_LIST,	ID_REGIST_LIST, NULL},
	{ AL_M_REGIST_CLASS,		ID_REGIST,		NULL},
	{ AL_M_UNREGIST_CLASS,		ID_UNREGIST,	NULL},
	{ NULL, 					0,				NULL}
};
static ADvMenuInfo lib_menu[] = {
	{ AL_M_MKLIB,			0,				mk_lib_menu},
	{ AL_M_LOAD_LIB,		ID_LD_LIB,		NULL},
	{ AL_M_LOAD_LIB_TMP,	ID_LD_LIB_TMP,	NULL},
	{ NULL, 					0,		NULL}
};
static ADvMenuInfo window_menu[] = {
	{ AL_M_REDRAW,		ID_REDRAW,		NULL},
	{ AL_M_CLOSE,		ID_CLOSE,		NULL},
	{ AL_M_CLOSE_ALL,	ID_CLOSE_ALL,	NULL},
	{ NULL, 			0,				NULL}
};
static ADvMenuInfo misc_menu[] = {
	{ AL_M_MARK_OUTPUT, ID_MARK_OUTPUT, NULL},
	{ AL_M_MEMORY_SIZE, ID_MEMORY_SIZE, NULL},
	{ AL_M_GC,			ID_GC,			NULL},
	{ AL_M_CLEAR_VTBL,	ID_CLEAR_VTBL,	NULL},
	{ NULL, 			0,				NULL}
};
static ADvMenuInfo class_menu[] = {
	{ AL_M_FILE,	0,	file_menu},
	{ AL_M_EDIT,	0,	edit_menu},
	{ AL_M_CLASS,	0,	regist_menu},
	{ AL_M_LIB, 	0,	lib_menu},
	{ AL_M_WINDOW,	0,	window_menu},
	{ AL_M_MISC,	0,	misc_menu},
	{ NULL, 		0,	NULL}
};

int AUiClassShell::LeftPress(ADvWindow *aw, AGmNode *e) {
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
int AUiClassShell::LeftRelease(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::LeftRelease(aw, e);
}
int AUiClassShell::LeftDoubleClick(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::LeftDoubleClick(aw, e);
}
int AUiClassShell::RightPress(ADvWindow *aw, AGmNode *e) {
	ASSERT(_edit);
	long from, to, line_from, line_to;
	_edit->GetSel(from, to);
	line_from = _edit->LineFromChar(from);
	line_to = _edit->LineFromChar(to);
	if (line_from != line_to)
		return ADvEditShell::RightPress(aw, e);
	return Menu(aw, e, line_from);
}
int AUiClassShell::RightRelease(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::RightRelease(aw, e);
}
int AUiClassShell::RightDoubleClick(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::RightDoubleClick(aw, e);
}
int AUiClassShell::MouseMove(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::MouseMove(aw, e);
}
int AUiClassShell::KeyPressFunc(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyPressFunc(aw, e);
}
int AUiClassShell::KeyReleaseFunc(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyReleaseFunc(aw, e);
}
int AUiClassShell::KeyChar(ADvWindow *aw, AGmNode *e) {
	return ADvEditShell::KeyChar(aw, e);
}
int AUiClassShell::Menu(ADvWindow *aw, AGmNode *e, long line_from) {
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
	AGmNode *root;
	if (_status == 0 && _class_stack) {
		root = _class_stack->H;
	} else {
		root = NULL;
	}
	int l_from, l_to;
	long id = ADvWindow::MenuSelect(aw, e, class_menu);
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
	case ID_ELDER_BROTHER:
		_create_pos = HIER_ELDER_BROTHER;
		_insert_pos = AUI_SHELL_INSERT_UPPER;
		break;
	case ID_CHILD:
		_create_pos = HIER_CHILD;
		_insert_pos = AUI_SHELL_INSERT_LOWER;
		break;
	case ID_BROTHER:
		_create_pos = HIER_BROTHER;
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
	case ID_LEFT:
		if (Left(l_from, l_to)) {
			AGmString *s = GetErrorString();
			ADvWindow::Print(s->Str());
		} else {
			OverWriteLines(l_from, l_to);
		}
		break;
	case ID_RIGHT:
		if (Right(l_from, l_to)) {
			AGmString *s = GetErrorString();
			ADvWindow::Print(s->Str());
		} else {
			OverWriteLines(l_from, l_to);
		}
		break;
	case ID_DELETE:
		if (root == NULL) break;
		char msg[256];
		AGmLink *proj, *l;
		proj = AUiProjectShell::SearchProject(_root_class);
		ASSERT(proj);
		l = next_outlink(proj->arc->dst->TTH, NULL, root->H);
		if (l) {
			art_sprintf_1(msg, 256, AL_S070, root->H->a.s.v->Str());
			ADvWindow::Print(msg);
			break;
		}
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
	case ID_REGIST_LIST:
		proj = AUiProjectShell::SearchProject(_root_class);
		ASSERT(proj);
		ADvWindow::Print(AL_S075);
		for (l = next_outlink(proj->arc->dst->TTH, NULL);
				l; l = next_outlink(proj->arc->dst->TTH, l)) {
			art_sprintf_1(msg, 256, "\t%s\n", l->arc->attr->a.s.v->Str());
			ADvWindow::Print(msg);
		}
		break;
	case ID_REGIST:
		if (root == NULL) break;
		proj = AUiProjectShell::SearchProject(_root_class);
		ASSERT(proj);
		l = next_outlink(proj->arc->dst->TTH, NULL, root->H);
		if (l) {
			art_sprintf_1(msg, 256, AL_S071, root->H->a.s.v->Str());
			ADvWindow::Print(msg);
			break;
		}
		CreateArc(proj->arc->dst->TTH, root, root->H);
		art_sprintf_1(msg, 256, AL_S067, root->H->a.s.v->Str());
		ADvWindow::Print(msg);
		break;
	case ID_UNREGIST:
		if (root == NULL) break;
		proj = AUiProjectShell::SearchProject(_root_class);
		ASSERT(proj);
		l = next_outlink(proj->arc->dst->TTH, NULL, root->H);
		if (l == NULL) {
			art_sprintf_1(msg, 256, AL_S072, root->H->a.s.v->Str());
			ADvWindow::Print(msg);
			break;
		}
		DeleteArc(l->arc);
		art_sprintf_1(msg, 256, AL_S069, root->H->a.s.v->Str());
		ADvWindow::Print(msg);
		break;
	case ID_TEXT_LIB:
		AUiProgramMgr::MakeLibText(_root_class, root);
		break;
	case ID_GRAPH_LIB:
		if (Refer(root, AUiClassShell::ReferToOutside)) {
			ADvWindow::Print(AL_S051);
			break;
		}
		AUiProgramMgr::MakeLibGraph(_root_class, root);
		break;
	case ID_TEXT2_CLEAR:
		proj = AUiProjectShell::SearchProject(_root_class);
		ASSERT(proj);
		set_dst_node(proj->arc->dst,  (AGmNode*)NULL, UniqueStr("text2_lib"));
		break;
	case ID_TEXT2_LIST:
		AGmNode *classes;
		proj = AUiProjectShell::SearchProject(_root_class);
		ASSERT(proj);
		classes = dst_node(proj->arc->dst, UniqueStr("text2_lib"));
		AGmLink *lnk;
		for (lnk = next_outlink(classes, NULL);
				lnk; lnk = next_outlink(classes, lnk)) {
			ADvWindow::Print("\t");
			ADvWindow::Print(lnk->arc->dst->H->a.s.v->Str());
			ADvWindow::Print("\n");
		}
		break;
	case ID_TEXT2_ADD:
		proj = AUiProjectShell::SearchProject(_root_class);
		ASSERT(proj);
		classes = dst_node(proj->arc->dst, UniqueStr("text2_lib"));
		if (classes == NULL) {
			classes = Cons(NULL);
			CreateArc(proj->arc->dst, classes, UniqueStr("text2_lib"));
		}
		CreateArc(classes, root);
		break;
	case ID_TEXT2_CREATE:
		proj = AUiProjectShell::SearchProject(_root_class);
		ASSERT(proj);
		classes = dst_node(proj->arc->dst, UniqueStr("text2_lib"));
		AUiProgramMgr::MakeLibText2(_root_class, classes);
		break;
	case ID_LD_LIB:
		AUiProgramMgr::LoadLib(_root_class);
		Update();	
		break;
	case ID_LD_LIB_TMP:
		AUiProgramMgr::LoadLib(_root_class, 1);
		Update();	
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
	case ID_CLOSE_ALL:
		AUiProgramMgr::CloseAllWindows();
		break;
	case ID_MARK_OUTPUT:
		ADvWindow::Print("====================\n");
		break;
	case ID_MEMORY_SIZE:
		long size;
		size = MemorySize(_root_class);
		art_sprintf_1(msg, 256, AL_S053, size);
		ADvWindow::Print(msg);
		break;
	case ID_GC:
#if	_WINDOWS && _64BIT
		long long num_new_free, num_free, num_use;
#else
		long num_new_free, num_free, num_use;
#endif
		AGm::GC(&num_new_free, &num_free, &num_use);
		ADvWindow::Print("----------------------------------------\n");
		art_sprintf_1(msg, 256, AL_GC_NEW_CELL, num_new_free);
		ADvWindow::Print(msg);
		art_sprintf_1(msg, 256, AL_GC_FREE_CELL, num_free);
		ADvWindow::Print(msg);
		art_sprintf_1(msg, 256, AL_GC_USE_CELL, num_use);
		ADvWindow::Print(msg);
		break;
	case ID_CLEAR_VTBL:
		AUiMemberShell::CleanVtbl(_root_class);
		AUiMemberShell::UpdateVtbl(_root_class, _root_class);
		AUiVtblUpdate = 0;
		break;
	default:
		break;
	}
	return 0;
}

int AUiClassShell::OverWriteLines(int l_from, int l_to) {
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

AGmNode *AUiClassShell::Class() {
	return _class_stack->H;
}
void AUiClassShell::CreateAsLastChild() {
	_create_pos = HIER_LAST_CHILD;
}

void AUiClassShell::GCmark() {
	_GCmark(_class_stack);
}


