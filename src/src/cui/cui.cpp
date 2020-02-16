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
#include <interp/basefunc.h>
#include <interp/interp.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/advwnd.h>
#include <dev/shell.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/body.h>
#include <cui/cui.h>
#include <cui/project.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_LINUX
extern AGmNode *split_str(AGmString *s, int d);

AGmNode *adv_split_str(char *s, int d) {
	AGmString *ss = new AGmString(s);
	AGmNode *ret = split_str(ss, d);
	delete ss;
	return ret;
}
#endif

AUiProgramMgr::AUiProgramMgr() {
	SetValue(NULL, AGM_VM_KEY_PROGRAM_MGR, this);
	_lib_search_path = Ref(Cons(NULL));
	_lib_list = Ref(Cons(NULL));
	_proj_list = Ref(Cons(NULL));
	CreateArc(_lib_search_path, String("."));
	AGmFile *f = new AGmFile;
#ifdef	_WINDOWS
	if (f->Create("altair.ini", AGM_FILE_READ)) { delete f; return; }
#endif
#ifdef	_LINUX
	char **argv = ADvGetApp()->argv, *cmd = NULL;
	char *path = NULL, filename[256];
	AGmNode *ls; AGmLink *l;

	ls = Ref(adv_split_str(argv[0], '/'));
	for (l = prev_outlink(ls, NULL); l; l = prev_outlink(ls, l)) {
		cmd = (char*)l->arc->dst->a.s.v->Str();
		break;
	}
	Unref(ls);
	if (cmd == NULL) { delete f; return; }

	path = getenv("PATH");
	if (path == NULL) { delete f; return; }
	ls = Ref(adv_split_str(path, ':'));
	for (l = next_outlink(ls, NULL); l; l = next_outlink(ls, l)) {
		sprintf(filename, "%s/%s", l->arc->dst->a.s.v->Str(), "altair.ini");
		if (AGmFile::DoesExist(filename)) {
			break;
		}
	}
	Unref(ls);
	if (l == NULL) { delete f; return; }
	if (f->Create(filename, AGM_FILE_READ)) { delete f; return; }
#endif
	AGmString *s;
	for (;;) {
		if (f->ReadIdent(s)) break;
		if (s->Compare("LIBDIR") == 0) {
			delete s;
			for (;;) {
				if (f->ReadQString(s)) break;
				CreateArc(_lib_search_path, String(s));
			}
			if (f->ReadIdent(s)) {
				// expected 'END_LIBDIR'
			} else {
				// s must be 'END_LIBDIR'
				delete s;
			}
		} else if (s->Compare("CURRDIR") == 0) {
			delete s;
			if (f->ReadQString(s) == 0) {
				AGmFile::ChDir(s->Str());
				delete s;
			}
		} else {
			delete s;
		}
	}
	delete f;
}
AUiProgramMgr::~AUiProgramMgr() {
	Unref(_lib_search_path, _proj_list,_lib_list);
	_lib_search_path = _proj_list = _lib_list = NULL;
	RemoveValue(NULL, AGM_VM_KEY_PROGRAM_MGR);
}
AUiProgramMgr *AUiProgramMgr::Instance() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_PROGRAM_MGR, value);
	return (AUiProgramMgr*)value;
}
AGmNode *AUiProgramMgr::LibSearchPath() { return _lib_search_path; }
AGmNode *AUiProgramMgr::LibList() { return _lib_list; }
AGmNode *AUiProgramMgr::ProjectList() { return _proj_list; }

AGmNode *AUiProgramMgr::CreateCommonClasses() {
	AGmNode *root = Cons(UniqueStr("AlObject"));
	AGmNode *excep = Cons(UniqueStr("AlException"));
	CreateArc(root, excep, NmSubClass());
	AGmNode *msg = List(
		Integer((long)AUI_PUBLIC), Integer(AUI_TYPE_STRING));
	AGmNode *fstk = List(
		Integer((long)AUI_PUBLIC), Integer(AUI_TYPE_LIST));
	AGmNode *pos = List(
		Integer((long)AUI_PUBLIC), Integer(AUI_TYPE_INTEGER));
	CreateArc(excep, msg, UniqueStr("msg"));
	CreateArc(excep, fstk, UniqueStr("stack_frame"));
	CreateArc(excep, pos, UniqueStr("pos"));

	AGmNode *v;
	AGmNode *ex = List(
		Integer((long)AUI_PUBLIC), v = Integer(AUI_TYPE_CLASS));
	AGmArc *a = CreateArc(v, excep, NmType());
	a->incl_assoc |= AGM_ARC_ASSOC;
	CreateArc(excep, ex, UniqueStr("exception"));

	return root;
}

int AUiProgramMgr::LoadLibrary(AGmNode *root_class, AGmString *lib_name, AGmNode** lib) {
	char err[256]; AGmLink *l;
	for (l = next_outlink(LibList(), NULL);
			l; l = next_outlink(LibList(), l)) {
		// case that the library is already loaded
		if (l->arc->attr->H->a.s.v->CompareI(lib_name) == 0) {
			AGmNode *base_class = AUiClassShell::NameToClass(root_class, l->arc->attr->TH->a.s.v);
			if (base_class == NULL) {
				art_sprintf_1(err, 256, AL_S115, l->arc->attr->TH->a.s.v->Str());
				ADvWindow::Print(err);
				return 1;
			}
			if (find_outlink(base_class, l->arc->dst) == NULL)
				CreateArc(base_class, l->arc->dst, NmSubClass());
			if (lib) {
				*lib = l->arc->dst;
			}
			return 0;
		}
	}
	// case of newlyloading a library
	// load a library if it is found in search path
	AGmString *s, *path;
	for (;;) {
	// project directory
		l = AUiProjectShell::SearchProject(root_class);
		ASSERT(l);
		path = l->arc->attr->H->a.s.v;
		s = new AGmString(path);
		s->Append("/"); s->Append(lib_name);
		if (AGmFile::DoesExist(s->Str()))
			break;
		delete s;
		s = NULL;
	// path specified by altair.ini
		AGmNode *lib_ls = AUiProgramMgr::Instance()->LibSearchPath();
		for (l = next_outlink(lib_ls, NULL);
				l; l = next_outlink(lib_ls, l)) {
			s = new AGmString(l->arc->dst->a.s.v);
			s->Append("/"); s->Append(lib_name);
			if (AGmFile::DoesExist(s->Str()))
				break;
			delete s;
			s = NULL;
		}
		if (s)
			break;
		art_sprintf_1(err, 256, AL_S125, lib_name->Str());
		ADvWindow::Print(err);
		return 1;
	}
	
	AGmFile *f = new AGmFile;
	if (f->Create(s, AGM_FILE_READ)) {
		art_sprintf_1(err, 256, AL_S104, s->Str());
		ADvWindow::Print(err);
		delete f; delete s; return 1;
	}
	delete s;
	if (_LoadLib(f, NULL, root_class, lib)) {
		delete f; return 1;
	}
	ASSERT(lib && *lib);
	AGmNode *base_class = src_node(*lib, NmSubClass());
	ASSERT(base_class);
	CreateArc(LibList(), *lib, List(String(lib_name->Str()), base_class->H));
	delete f;
	return 0;
}

int AUiProgramMgr::New() {
	AGmString *dir, *name;
	if (AIpProcess::GetFileName(0, dir, name, "Altair Project (*.apr)|*.apr||")) return 1;

	if (AUiProjectShell::SearchProject(dir, name)) {
		ADvWindow::Print(AL_S173);
		delete dir; delete name; return 1;
	}	

	AGmNode *root = CreateCommonClasses();
	AGmNode *lib_list = Cons(NULL);
	AGmNode *class_list = Cons(NULL);
	AGmNode *proj = AUiProjectShell::AddProject(dir, name, root, lib_list, class_list);
	Save(root);
	AUiProjectShell::OpenClassEditor(proj);

	delete dir; delete name;
	return 0;
}
int AUiProgramMgr::Open() {
	AGmString *dir, *name;
	if (AIpProcess::GetFileName(1, dir, name, "Altair Project (*.apr)|*.apr||")) return 1;
	if (AUiProjectShell::SearchProject(dir, name)) {
		ADvWindow::Print(AL_S154);
		delete dir; delete name; return 1;
	}
	int r = _Open(dir, name);
	delete dir; delete name; return r;
}
int AUiProgramMgr::_Open(AGmString *dir, AGmString *name) {
	AGmString *s = new AGmString(dir); s->Append("/"); s->Append(name);
	AGmNode *proj;
	if (AGmFile::DoesExist(s->Str())) {
		AGmNode *root = CreateCommonClasses();
		AGmNode *lib_list = Cons(NULL);
		AGmNode *class_list = Cons(NULL);
		proj = AUiProjectShell::AddProject(dir, name, root, lib_list, class_list);
		ADvBeginWaitCursor();
		if (__Open(s, proj)) {
			ADvEndWaitCursor();
			AUiProjectShell::RemoveProject(proj);
			delete s; return 1;
		}
		MakeStrUnique(root);
		ADvEndWaitCursor();
	} else {
		ADvWindow::Print(AL_S108);
		delete s; return 1;
	}
	AUiProjectShell::OpenClassEditor(proj);

	delete s;
	return 0;
}
int AUiProgramMgr::__Open(AGmString *path, AGmNode* proj, int inet) {
	char msg[256], err[256];
	AGmFile *f = new AGmFile;
	if (inet == 0) {
		if (f->Create(path, AGM_FILE_READ)) {
			art_sprintf_1(msg, 256, AL_S104, path->Str());
			ADvWindow::Print(msg);
			delete f; return 1;
		}
	} else {
		if (f->Create(path, AGM_FILE_STR_READ)) {
			ADvWindow::Print(AL_S104);
			delete f; return 1;
		}
	}
	AGmString *s;
// loading referred libraries
	for (;;) {
		if (f->ReadString(s)) {
			art_sprintf_1(msg, 256, AL_S106, path->Str());
			ADvWindow::Print(msg);
			delete f; return 1;
		}
		if (s->Compare("$END_LIB") == 0) {
			delete s; break;
		}
		art_sprintf_1(msg, 256, AL_S126, s->Str());
		ADvWindow::Print(msg);
		AGmNode *lib = NULL;
		if (AUiProgramMgr::Instance()->LoadLibrary(proj->H, s, &lib)) {
			delete f; delete s; return 1;
		}
		CreateArc(proj->TH, lib, String(s->Str()));
		delete s;
	}
// load main parts
	ADvWindow::Print(AL_S114);
	for (;;) {
		if (f->ReadString(s)) {
			art_sprintf_1(msg, 256, AL_S106, path->Str());
			ADvWindow::Print(msg);
			delete f; return 1;
		}
		if (s->Compare("$END_CLASS") == 0) {
			delete s; break;
		}
		AGmNode *lib = NULL;
		if (s->Compare("TEXT") != 0) {
			ADvWindow::Print(AL_S111);
			delete s; delete f; return 1;
		}
		delete s;
		if (f->ReadIdent(s)) {
			ADvWindow::Print(AL_S117);
			delete f; return 1;
		}
		AGmNode *root = AUiClassShell::NameToClass(proj->H, s);
		if (root == NULL) {
			art_sprintf_1(err, 256, AL_S115, s->Str());
			ADvWindow::Print(err);
			delete s; delete f; return 1;
		}
		delete s;
		if (_LoadLibText(f, NULL, proj->H, root, AIP_STEP_CLASS, &lib)) {
			ADvWindow::Print(AL_S111);
			delete f; return 1;
		}
		CreateArc(proj->TTH, lib, lib->H);
	}
	for (;;) {
		if (f->ReadString(s)) {
			art_sprintf_1(msg, 256, AL_S106, path->Str());
			ADvWindow::Print(msg);
			delete f; return 1;
		}
		if (s->Compare("$END_MEMBER") == 0) {
			delete s; break;
		}
		AGmNode *lib = NULL;
		if (s->Compare("TEXT") != 0) {
			ADvWindow::Print(AL_S111);
			delete s; delete f; return 1;
		}
		delete s;
		if (f->ReadIdent(s)) {
			ADvWindow::Print(AL_S117);
			delete f; return 1;
		}
		AGmNode *root = AUiClassShell::NameToClass(proj->H, s);
		if (root == NULL) {
			art_sprintf_1(err, 256, AL_S115, s->Str());
			ADvWindow::Print(err);
			delete s; delete f; return 1;
		}
		delete s;
		if (_LoadLibText(f, NULL, proj->H, root, AIP_STEP_MEMBER, &lib)) {
			ADvWindow::Print(AL_S111);
			delete f; return 1;
		}
	}
	for (;;) {
		if (f->ReadString(s)) {
			art_sprintf_1(msg, 256, AL_S106, path->Str());
			ADvWindow::Print(msg);
			delete f; return 1;
		}
		if (s->Compare("$END_BODY") == 0) {
			delete s; break;
		}
		AGmNode *lib = NULL;
		if (s->Compare("TEXT") != 0) {
			ADvWindow::Print(AL_S111);
			delete s; delete f; return 1;
		}
		delete s;
		if (f->ReadIdent(s)) {
			ADvWindow::Print(AL_S117);
			delete f; return 1;
		}
		AGmNode *root = AUiClassShell::NameToClass(proj->H, s);
		if (root == NULL) {
			art_sprintf_1(err, 256, AL_S115, s->Str());
			ADvWindow::Print(err);
			delete s; delete f; return 1;
		}
		delete s;
		if (_LoadLibText(f, NULL, proj->H, root, AIP_STEP_BODY, &lib)) {
			ADvWindow::Print(AL_S111);
			delete f; return 1;
		}
	}
	delete f;
	ADvWindow::Print(AL_S112);
	return 0;
}
int AUiProgramMgr::Save(AGmNode *root_class) {
	AGmLink *proj = AUiProjectShell::SearchProject(root_class);
	ASSERT(proj);
	AGmString *s = new AGmString(proj->arc->attr->H->a.s.v);
	s->Append("/"); s->Append(proj->arc->attr->TH->a.s.v);

	ADvBeginWaitCursor();
	if (_Save(s, proj->arc->dst)) {
		ADvEndWaitCursor();
		delete s;
		return 1;
	}
	ADvEndWaitCursor();

	delete s;
	return 0;
}
int AUiProgramMgr::SaveAs(AGmNode *root_class) {
	AGmLink *proj = AUiProjectShell::SearchProject(root_class);
	ASSERT(proj);
	AGmString *dir, *name, *s;
	if (AIpProcess::GetFileName(0, dir, name, "Altair Project (*.apr)|*.apr||")) return 1;

	if (AUiProjectShell::SearchProject(dir, name)) {
		ADvWindow::Print(AL_S173);
		delete dir; delete name; return 1;
	}	
	s = new AGmString(dir); s->Append("/"); s->Append(name);
	delete dir; delete name;

	ADvBeginWaitCursor();
	if (_Save(s, proj->arc->dst)) {
		ADvEndWaitCursor();
		delete s; return 1;
	}
	ADvEndWaitCursor();

	delete s;
	return 0;
}
int AUiProgramMgr::_Save(AGmString *path, AGmNode *proj) {
	AGmFile *f = new AGmFile;
	if (f->Create(path, AGM_FILE_WRITE)) {
		ADvWindow::Print(AL_S107);
		delete f; return 1;
	}
	AGmLink *l;
// save referred libraries
	for (l = next_outlink(proj->TH, NULL);
			l; l = next_outlink(proj->TH, l)) {
		f->WriteString(l->arc->attr->a.s.v);
		f->WriteString("\n");
	}
	f->WriteString("$END_LIB\n");

// save main parts
	for (l = next_outlink(proj->TTH, NULL);
			l; l = next_outlink(proj->TTH, l)) {
			AGmNode *sub_class = AUiClassShell::NameToClass(proj->H, l->arc->attr);
			ASSERT(sub_class);
			AGmNode *base_class = src_node(sub_class, NmSubClass());
			ASSERT(base_class);
		f->WriteString("TEXT\n");
		f->WriteString(base_class->H->a.s.v);
		f->WriteString("\n");
		_MakeLibText(f, proj->H, sub_class, AIP_STEP_CLASS);
	}
	f->WriteString("$END_CLASS\n");

	for (l = next_outlink(proj->TTH, NULL);
			l; l = next_outlink(proj->TTH, l)) {
			AGmNode *sub_class = AUiClassShell::NameToClass(proj->H, l->arc->attr);
			ASSERT(sub_class);
			AGmNode *base_class = src_node(sub_class, NmSubClass());
			ASSERT(base_class);
		f->WriteString("TEXT\n");
		f->WriteString(base_class->H->a.s.v);
		f->WriteString("\n");
		_MakeLibText(f, proj->H, sub_class, AIP_STEP_MEMBER);
	}
	f->WriteString("$END_MEMBER\n");

	for (l = next_outlink(proj->TTH, NULL);
			l; l = next_outlink(proj->TTH, l)) {
			AGmNode *sub_class = AUiClassShell::NameToClass(proj->H, l->arc->attr);
			ASSERT(sub_class);
			AGmNode *base_class = src_node(sub_class, NmSubClass());
			ASSERT(base_class);
		f->WriteString("TEXT\n");
		f->WriteString(base_class->H->a.s.v);
		f->WriteString("\n");
		_MakeLibText(f, proj->H, sub_class, AIP_STEP_BODY);
	}
	f->WriteString("$END_BODY\n");

	delete f;
	return 0;
}

void AUiProgramMgr::Exit() {
	ADvCWinApp *app = ADvGetApp();
	if (app)
		app->ExitApplication();
}

// --------------------------------------------------------------------------------
// make a library
int AUiProgramMgr::MakeLibText(AGmNode *root_class, AGmNode *root) {	
	if (root == NULL) {
		ADvWindow::Print(AL_S076);
		return 1;
	}
	if (root == root_class) {
		ADvWindow::Print(AL_S127);
		return 1;
	}
	AGmNode *base_class = src_node(root, NmSubClass());
	ASSERT(base_class);
	AGmString *dir, *name, *s;
	if (AIpProcess::GetFileName(0, dir, name, "Altair Lib (*.alb)|*.alb||")) return 1;
	s = new AGmString(dir); s->Append("/"); s->Append(name);
	char msg[256];
	art_sprintf_2(msg, 256, AL_S089, name->Str(), root->H->a.s.v->Str());
	ADvWindow::Print(msg);

	ADvBeginWaitCursor();
	AGmFile *f = new AGmFile;
	if (f->Create(s, AGM_FILE_WRITE)) {
		ADvEndWaitCursor();
		ADvWindow::Print(AL_S109);
		delete f; delete s; delete dir; delete name; return 1;
	}
	delete s;
	f->WriteString("TEXT\n");
	f->WriteString(base_class->H->a.s.v);
	f->WriteString("\n");
	_MakeLibText(f, root_class, root, AIP_STEP_CLASS);
	_MakeLibText(f, root_class, root, AIP_STEP_MEMBER);
	_MakeLibText(f, root_class, root, AIP_STEP_BODY);
	delete f;
	ADvEndWaitCursor();

	art_sprintf_2(msg, 256, AL_S088, name->Str(), root->H->a.s.v->Str());
	ADvWindow::Print(msg);
	delete dir; delete name;
	return 0;
}
int AUiProgramMgr::_MakeLibText(AGmFile* f, AGmNode *root_class, AGmNode* n, int step) {
	if (f->WriteString("class ") || f->WriteString(n->H->a.s.v) || f->WriteString("\n")) return 1;
	if (step == AIP_STEP_MEMBER || step == AIP_STEP_BODY) {
		// {{{{{{{{ Member
		AUiMemberShell *ms = new AUiMemberShell((ADvWindow*)NULL, root_class, n);
		for (ms->Reset(); ms->More(); ms->Next()) {
			AGmString *s = ms->GetLineString();
			if (s->Compare("END") == 0) continue;
			if (f->WriteString("member\n")) { delete ms; return 1; }
			if (f->WriteString(s) || f->WriteString("\n")) { delete ms; return 1; }
			if (step == AIP_STEP_BODY) {
				// {{{{{{{{ Body
				AGmNode *member = ms->Member();
				if (member->HI & AUI_FUNC && member->TTH) {
					if (f->WriteString("body\n")) { delete ms; return 1; }
					AUiBodyShell *bs = new AUiBodyShell(
						(ADvWindow*)NULL, root_class, n, member);
					for (bs->Reset(); bs->More(); bs->Next()) {
						AGmString *s = bs->GetLineString();
						if (s->Compare("END") == 0) continue;
						if (f->WriteString(s) || f->WriteString("\n")) {
							delete bs; delete ms; return 1;
						}
					}
					delete bs;
					if (f->WriteString("end_body\n")) { delete ms; return 1; }
				}
				// }}}}}}}} Body
			}
		}
		delete ms;
		// }}}}}}}} Member
	}
	AGmLink *l;
	for (l = next_outlink2(n, NULL, NmSubClass());
			l; l = next_outlink2(n, l, NmSubClass())) {
		if (_MakeLibText(f, root_class, l->arc->dst, step)) return 1;
	}
	if (f->WriteString("end_class\n")) return 1;
	return 0;
}
int AUiProgramMgr::MakeLibGraph(AGmNode *root_class, AGmNode *root) {
	if (root == NULL) {
		ADvWindow::Print(AL_S076);
		return 1;
	}
	if (root == root_class) {
		ADvWindow::Print(AL_S127);
		return 1;
	}
	AGmNode *base_class = src_node(root, NmSubClass());
	ASSERT(base_class);
	AGmString *dir, *name, *s;
	if (AIpProcess::GetFileName(0, dir, name, "Altair Files (*.alb)|*.alb||")) return 1;
	s = new AGmString(dir); s->Append("/"); s->Append(name);
	char msg[256];
	art_sprintf_2(msg, 256, AL_S083, name->Str(), root->H->a.s.v->Str());
	ADvWindow::Print(msg);

	ADvBeginWaitCursor();
	AGmFile *f = new AGmFile;
	if (f->Create(s, AGM_FILE_WRITE)) {
		ADvEndWaitCursor();
		ADvWindow::Print(AL_S109);
		delete f; delete s; delete dir; delete name; return 1;
	}
	if (f->WriteString("GRAPH\n") ||	
		f->WriteString(base_class->H->a.s.v) ||
		f->WriteString("\n") ||
		f->WriteGraph(root)) {
		ADvEndWaitCursor();
		ADvWindow::Print(AL_S110);
		delete f; delete s; delete dir; delete name; return 1;
	}
	delete f;
	ADvEndWaitCursor();

	art_sprintf_2(msg, 256, AL_S082, name->Str(), root->H->a.s.v->Str());
	ADvWindow::Print(msg);
	delete s; delete dir; delete name;
	return 0;
}
int AUiProgramMgr::MakeLibText2(AGmNode *root_class, AGmNode *classes) {
	if (classes == NULL) return 0;
	AGmString *dir, *name, *s;
	if (AIpProcess::GetFileName(0, dir, name, "Altair Lib (*.alb)|*.alb||")) return 1;
	s = new AGmString(dir); s->Append("/"); s->Append(name);
	char msg[256];
	art_sprintf_1(msg, 256, AL_S201, name->Str());
	ADvWindow::Print(msg);

	ADvBeginWaitCursor();
	AGmFile *f = new AGmFile;
	if (f->Create(s, AGM_FILE_WRITE)) {
		ADvEndWaitCursor();
		ADvWindow::Print(AL_S109);
		delete f; delete s; delete dir; delete name; return 1;
	}
	delete s;
	f->WriteString("TEXT2\n");

	AGmLink *l;
	for (l = next_outlink(classes, NULL);
			l; l = next_outlink(classes, l)) {
		AGmNode *root = l->arc->dst;
		AGmNode *base_class = src_node(root, NmSubClass());
		ASSERT(base_class);
		f->WriteString(base_class->H->a.s.v);
		f->WriteString("\n");
		_MakeLibText(f, root_class, root, AIP_STEP_CLASS);
	}
	f->WriteString("$END_CLASS\n");

	for (l = next_outlink(classes, NULL);
			l; l = next_outlink(classes, l)) {
		AGmNode *root = l->arc->dst;
		AGmNode *base_class = src_node(root, NmSubClass());
		ASSERT(base_class);
		f->WriteString(base_class->H->a.s.v);
		f->WriteString("\n");
		_MakeLibText(f, root_class, root, AIP_STEP_MEMBER);
	}
	f->WriteString("$END_MEMBER\n");

	for (l = next_outlink(classes, NULL);
			l; l = next_outlink(classes, l)) {
		AGmNode *root = l->arc->dst;
		AGmNode *base_class = src_node(root, NmSubClass());
		ASSERT(base_class);
		f->WriteString(base_class->H->a.s.v);
		f->WriteString("\n");
		_MakeLibText(f, root_class, root, AIP_STEP_BODY);
	}
	f->WriteString("$END_BODY\n");

	delete f;
	ADvEndWaitCursor();

	art_sprintf_1(msg, 256, AL_S200, name->Str());
	ADvWindow::Print(msg);
	delete dir; delete name;
	return 0;
}

// load library
int AUiProgramMgr::LoadLib(AGmNode *root_class, int tmp) {
	AGmLink *proj = AUiProjectShell::SearchProject(root_class);
	ASSERT(proj);
	AGmString *dir, *name, *s;
	if (AIpProcess::GetFileName(1, dir, name, "Altair Lib (*.alb)|*.alb||")) return 1;

	AGmNode *rp = Instance()->LibList();	
	AGmLink *l;
	for (l = next_outlink(rp, NULL);
			l; l = next_outlink(rp, l)) {
		if (l->arc->attr->H->a.s.v->Compare(name) != 0) continue;
		ADvWindow::Print(AL_S174);
		ADvWindow::Print(AL_S153);
		AGmNode *lib;
		if (Instance()->LoadLibrary(proj->arc->dst->H, name, &lib)) return 1;
		CreateArc(proj->arc->dst->TH, lib, String(name->Str()));
		delete dir; delete name;
		return 0;
	}

	s = new AGmString(dir); s->Append("/"); s->Append(name);
	AGmFile *f = new AGmFile;
	if (AGmFile::DoesExist(s->Str())) {
		if (f->Create(s, AGM_FILE_READ)) {
			ADvWindow::Print(AL_S107);
			delete f; delete s; delete dir; delete name; return 1;
		}
	}
	delete s;

	ADvBeginWaitCursor();
	AGmNode *lib = NULL;
	if (_LoadLib(f, NULL, root_class, &lib)) {
		delete f; delete dir; delete name; return 1;				
	}
	ADvEndWaitCursor();

	if (tmp == 0) {
		ASSERT(lib);
		AGmNode *base_class = src_node(lib, NmSubClass());
		ASSERT(base_class);
		CreateArc(Instance()->LibList(), lib, List(String(name->Str()), base_class->H));
		CreateArc(proj->arc->dst->TH, lib, String(name->Str()));
	}
	
	delete f; delete dir; delete name;
	return 0;
}
int AUiProgramMgr::_LoadLib(AGmFile *f, AGmString *s, AGmNode *root_class, AGmNode **lib) {
	char err[256];
	if (s == NULL) {
		if (f->ReadIdent(s)) {
			ADvWindow::Print(AL_S111);
			return 1;
		}
	}
	if (s->Compare("TEXT") == 0) {
		delete s;
		if (f->ReadIdent(s)) {
			ADvWindow::Print(AL_S117);
			return 1;
		}
		AGmNode *root = AUiClassShell::NameToClass(root_class, s);
		if (root == NULL) {
			art_sprintf_1(err, 256, AL_S115, s->Str());
			ADvWindow::Print(err);
			delete s; return 1;
		}
		delete s;
		if (_LoadLibText(f, NULL, root_class, root, AIP_STEP_CLASS, lib)) {
			ADvWindow::Print(AL_S111);
			return 1;
		}
		if (_LoadLibText(f, NULL, root_class, root, AIP_STEP_MEMBER, lib)) {
			ADvWindow::Print(AL_S111);
			return 1;
		}
		if (_LoadLibText(f, NULL, root_class, root, AIP_STEP_BODY, lib)) {
			ADvWindow::Print(AL_S111);
			return 1;
		}
	} else if (s->Compare("GRAPH") == 0) {
		delete s;
		if (f->ReadIdent(s)) {
			delete s;
			ADvWindow::Print(AL_S117);
			return 1;
		}
		AGmNode *root = AUiClassShell::NameToClass(root_class, s);
		if (root == NULL) {
			art_sprintf_1(err, 256, AL_S115, s->Str());
			ADvWindow::Print(err);
			delete s; return 1;
		}
		delete s;
		if (_LoadLibGraph(f, root_class, root, lib)) {
			ADvWindow::Print(AL_S111);
			return 1;
		}
	} else if (s->Compare("TEXT2") == 0) {
		delete s;
		for (;;) {
			if (f->ReadString(s)) {
				ADvWindow::Print(AL_S117);
				return 1;
			}
			if (s->Compare("$END_CLASS") == 0) {
				delete s;
				break;
			}
			AGmNode *root = AUiClassShell::NameToClass(root_class, s);
			if (root == NULL) {
				art_sprintf_1(err, 256, AL_S115, s->Str());
				ADvWindow::Print(err);
				delete s; return 1;
			}
			delete s;
			if (_LoadLibText(f, NULL, root_class, root, AIP_STEP_CLASS, lib)) {
				ADvWindow::Print(AL_S111);
				return 1;
			}
		}
		for (;;) {
			if (f->ReadString(s)) {
				ADvWindow::Print(AL_S117);
				return 1;
			}
			if (s->Compare("$END_MEMBER") == 0) {
				delete s;
				break;
			}
			AGmNode *root = AUiClassShell::NameToClass(root_class, s);
			if (root == NULL) {
				art_sprintf_1(err, 256, AL_S115, s->Str());
				ADvWindow::Print(err);
				delete s; return 1;
			}
			delete s;
			if (_LoadLibText(f, NULL, root_class, root, AIP_STEP_MEMBER, lib)) {
				ADvWindow::Print(AL_S111);
				return 1;
			}
		}
		for (;;) {
			if (f->ReadString(s)) {
				ADvWindow::Print(AL_S117);
				return 1;
			}
			if (s->Compare("$END_BODY") == 0) {
				delete s;
				break;
			}
			AGmNode *root = AUiClassShell::NameToClass(root_class, s);
			if (root == NULL) {
				art_sprintf_1(err, 256, AL_S115, s->Str());
				ADvWindow::Print(err);
				delete s; return 1;
			}
			delete s;
			if (_LoadLibText(f, NULL, root_class, root, AIP_STEP_BODY, lib)) {
				ADvWindow::Print(AL_S111);
				return 1;
			}
		}
	} else {
		delete s;
		ADvWindow::Print(AL_S179);
		return 1;
	}
	return 0;
}

int AUiProgramMgr::check_load = 0;
int AUiProgramMgr::_LoadLibText(AGmFile *f, AGmString* s, AGmNode *root_class, AGmNode *root, int step, AGmNode** lib) {
	AUiVtblUpdate = 0;
	char line[4096];
	if (s == NULL) {
		f->ReadSpace();
		if (f->ReadLine(s)) return 1;
	}
	AGmFile *f2 = new AGmFile;
	if (f2->Create(s, AGM_FILE_STR_READ)) {
		delete f2; delete s; return 1;
	}
	if (f2->MatchIdent("class") == 0) {
		delete f2; delete s; return 1;
	}
	AUiClassShell *cs = new AUiClassShell((ADvWindow*)NULL, root_class);
	cs->CreateAsLastChild();
	for (cs->Reset(); cs->Class() != root; cs->Next()) ;
	AGmString *name;
	if (f2->ReadIdent(name)) {
		delete f2; delete s; delete cs; return 1;
	}
	delete f2; delete s;
	if (step == AIP_STEP_CLASS) {
		if (cs->InsertLine(name->Str())) {	// LineInsert (Class)
			ADvWindow::Print(cs->GetErrorString()->Str());
			delete name; delete cs; return 1;
		}
		if (lib && *lib == NULL) {
			*lib = AUiClassShell::NameToClass(root_class, name);
		}
	}
	AGmNode *cn = AUiClassShell::NameToClass(root_class, name);
	delete name;
	f->ReadSpace();
	if (f->ReadLine(s)) {
		delete cs; return 1;
	}
	if (check_load) {
		art_sprintf_1(line, 4096, "%s\n", s->Str());
		ADvWindow::Print(line);
	}
	if (step == AIP_STEP_MEMBER || step == AIP_STEP_BODY) {
		// {{{{{{{{ Member
		AUiMemberShell *ms = new AUiMemberShell(
			(ADvWindow*)NULL, root_class, cn);
		for (ms->Reset();; ms->Next()) {
			if (s->Compare("member") != 0) break;
			delete s;
			f->ReadSpace();
			if (f->ReadLine(s)) {
				delete ms; delete cs; return 1;
			}
			if (check_load) {
				art_sprintf_1(line, 4096, "%s\n", s->Str());
				ADvWindow::Print(line);
			}
			if (step == AIP_STEP_MEMBER) {
				if (ms->InsertLine(s->Str())) { // LineInsert (Member)
					delete s; delete ms; delete cs; return 1;
				}
			}
			delete s;
			f->ReadSpace();
			if (f->ReadLine(s)) {
				delete ms; delete cs; return 1;
			}
			if (check_load) {
				art_sprintf_1(line, 4096, "%s\n", s->Str());
				ADvWindow::Print(line);
			}
			if (step == AIP_STEP_BODY) {
				if (s->Compare("body") == 0) {
					delete s;
					// {{{{{{{{ Body
					AUiBodyShell *bs = new AUiBodyShell(
						(ADvWindow*)NULL, root_class, ms->Class(), ms->Member());
					for (bs->Reset();;) {
						f->ReadSpace();
						if (f->ReadLine(s)) {
							delete bs; delete ms; delete cs; return 1;
						}
						if (check_load) {
							art_sprintf_1(line, 4096, "%s\n", s->Str());
							ADvWindow::Print(line);
						}
						if (bs->InsertLine(s->Str()) == 0) { // LineInsert (Body)
							delete s; continue;
						}
						if (s->Compare("end_body") != 0) {
							delete s; delete bs; delete ms; delete cs; return 1;
						}
						delete s;
						break;
					}
					delete bs;
					// }}}}}}}} Body
					f->ReadSpace();
					if (f->ReadLine(s)) {
						delete ms; delete cs; return 1;
					}
					if (check_load) {
						art_sprintf_1(line, 4096, "%s\n", s->Str());
						ADvWindow::Print(line);
					}
				}
			}
		}
		delete ms;
		// }}}}}}}} Member
	}
	for (;;) {
		if (s->Compare("end_class") == 0) break;
		if (_LoadLibText(f, s, root_class, cn, step, lib)) {
			delete cs; return 1;
		}
		f->ReadSpace();
		if (f->ReadLine(s)) {
			delete cs; return 1;
		}
	}
	delete s;
	delete cs;
	return 0;
}
int AUiProgramMgr::_LoadLibGraph(AGmFile *f, AGmNode* /*root_class*/, AGmNode *root, AGmNode** lib) {
	AUiVtblUpdate = 0;
	AGmNode *root2;
	if (f->ReadGraph(root2)) return 1;
	MakeStrUnique(root2);
	CreateArc(root, root2, NmSubClass());
	if (lib) *lib = root2;
	return 0;
}

// --------------------------------------------------------------------------------
void AUiProgramMgr::CloseWindow(ADvWindow *aw) {
	void *value;
	GetValue(aw->View(), AGM_VM_KEY_WINDOW_NODE, value);
	ReleaseNode((AGmNode*)value);
}

void AUiProgramMgr::CloseAllWindows() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW, value);
	if (value)
		RemoveValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW);
	AGmNode *root_window = RootWindowNode();
	for (;;) {
		AGmLink *l = next_outlink(root_window, NULL);
		if (l == NULL) break;
		ReleaseNode(l->arc->dst);
	}
}

int AUiProgramMgr::OpenProjectEditor() {
	void *value;
	GetValue(NULL, AGM_VM_KEY_PROJ_EDITOR, value);
	if (value != NULL) return ((ADvWindow*)value)->Raise();

	ADvWindow *aw = new ADvWindow();
	AUiProjectShell *shell = new AUiProjectShell(aw);
	SetValue(NULL, AGM_VM_KEY_PROJ_EDITOR, aw);
	if (aw->CreateEdit(shell) == NULL) { delete shell; delete aw; return 1; }
	aw->SetTitle(AL_T_PROJECT);
	aw->SetFocus();

	return 0;
}

int AUiProgramMgr::IsReserveWord(AGmString* s) {
	if (s->Compare("var") == 0
			|| s->Compare("new") == 0
			|| s->Compare("this") == 0

			|| s->Compare("integer") == 0
			|| s->Compare("real") == 0
			|| s->Compare("string") == 0
			|| s->Compare("list") == 0
			|| s->Compare("array") == 0
			|| s->Compare("matrix") == 0
			|| s->Compare("file") == 0
			|| s->Compare("window") == 0
			|| s->Compare("void") == 0
			|| s->Compare("variant") == 0
			|| s->Compare("runtime") == 0

			|| s->Compare("if") == 0
			|| s->Compare("else") == 0
			|| s->Compare("loop") == 0
			|| s->Compare("break") == 0
			|| s->Compare("continue") == 0
			|| s->Compare("return") == 0
			|| s->Compare("try") == 0
			|| s->Compare("catch") == 0
			|| s->Compare("para") == 0) {
		return 1;
	} else {
		return 0;
	}
}

// for garbage collection
void AUiProgramMgr::GCmark() {
	_GCmark(_lib_search_path);
	_GCmark(_lib_list);
	_GCmark(_proj_list);
}

