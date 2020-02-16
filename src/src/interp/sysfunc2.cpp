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
#include <gdm/matrix.h>
#include <gdm/file.h>
#include <gdm/vmkey.h>
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/shell.h>
#include <dev/advcedit.h>
#include <dev/bitmap.h>
#include <dev/olectrl.h>
#include <dev/mainfrm.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/cui.h>
#include <altair/strres.h>

#ifdef _WINDOWS
#include <altair/resource.h>
#endif

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

// PolyPoly manipulation function
int AIpProcess::EvalCreatePolyPoly() {
	Unref(val); val = Ref(CreatePolyPoly());
	return 0;
}
int AIpProcess::EvalCreateNextPoly() {
	AGmNode *polypoly;
	GetArg(polypoly);
	CreateNextPoly(polypoly);
	Unref(polypoly);
	return 0;
}
int AIpProcess::EvalAddPointToPoly() {
	AGmNode *polypoly, *x, *y; double rx, ry;
	GetArg(polypoly, x, y);
	if (!IsNum(x, rx) || !IsNum(y, ry)) {
		Unref(polypoly, x, y);
		return interp_error("AddPointToPoly()");
	}
	AddPointToPoly(polypoly, rx, ry);
	Unref(polypoly, x, y);
	return 0;
}
int AIpProcess::EvalAddRectToPoly() {
	AGmNode *polypoly, *l, *t, *r, *b; double rl, rt, rr, rb;
	GetArg(polypoly, l, t, r, b);
	if (!IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb)) {
		Unref(polypoly, l, t, r, b);
		return interp_error("AddRectToPoly()");
	}
	AddRectToPoly(polypoly, rl, rt, rr, rb);
	Unref(polypoly, l, t, r, b);
	return 0;
}
int AIpProcess::EvalAddCurveToPoly() {
	AGmNode *polypoly, *cx1, *cy1, *cx2, *cy2, *ex, *ey;
	double rcx1, rcy1, rcx2, rcy2, rex, rey;
	GetArg(polypoly, cx1, cy1, cx2, cy2, ex, ey);
	if (!IsNum(cx1, rcx1) || !IsNum(cy1, rcy1) || !IsNum(cx2, rcx2) || !IsNum(cy2, rcy2) ||
			!IsNum(ex, rex) || !IsNum(ey, rey)) {
		Unref(polypoly, cx1, cy1, cx2, cy2, ex, ey);
		return interp_error("AddCurveToPoly()");
	}
	AddCurveToPoly(polypoly, rcx1, rcy1, rcx2, rcy2, rex, rey);
	Unref(polypoly, cx1, cy1, cx2, cy2, ex, ey);
	return 0;
}
int AIpProcess::EvalIntersection() {
	AGmNode *poly1, *poly2;
	GetArg(poly1, poly2);
	if (poly1 == NULL) {
		Unref(val); val = Ref(poly2);
	} else if (poly2 == NULL) {
		Unref(val); val = Ref(poly1);
	} else {
		double l1, t1, r1, b1, l2, t2, r2, b2, l, t, r, b;
		GetPolyPolygonBox(poly1, l1, t1, r1, b1);
		GetPolyPolygonBox(poly2, l2, t2, r2, b2);
		l = max(l1, l2);
		t = max(t1, t2);
		r = min(r1, r2);
		b = min(b1, b2);
		if (l >= r || t >= b) {
			l = t = r = b = 0;
		}
		AGmNode *poly = CreatePolyPoly();
		AddRectToPoly(poly, l, t, r, b);
		Unref(val); val = Ref(poly);
	}
	Unref(poly1, poly2);
	return 0;
}
int AIpProcess::EvalUnion() {
	AGmNode *poly1, *poly2;
	GetArg(poly1, poly2);
	if (poly1 == NULL) {
		Unref(val); val = Ref(poly2);
	} else if (poly2 == NULL) {
		Unref(val); val = Ref(poly1);
	} else {
		double l1, t1, r1, b1, l2, t2, r2, b2, l, t, r, b;
		GetPolyPolygonBox(poly1, l1, t1, r1, b1);
		GetPolyPolygonBox(poly2, l2, t2, r2, b2);
		l = min(l1, l2);
		t = min(t1, t2);
		r = max(r1, r2);
		b = max(b1, b2);
		AGmNode *poly = CreatePolyPoly();
		AddRectToPoly(poly, l, t, r, b);
		Unref(val); val = Ref(poly);
	}
	Unref(poly1, poly2);
	return 0;
}

// Matrix data manipulation function
int AIpProcess::EvalUnitTM() {
	AGmMatrix *m = new AGmMatrix(3, 3);
	if (m->Id()) {
		delete m;
		return interp_error("UnitTM()");
	}
	Unref(val); val = Ref(Matrix(m));
	return 0;
}
int AIpProcess::EvalTranslateTM() {
	AGmNode *dx, *dy; double rdx, rdy;
	GetArg(dx, dy);
	if (!IsNum(dx, rdx) || !IsNum(dy, rdy)) {
		Unref(dx, dy);
		return interp_error("TranslateTM()");
	}
	AGmMatrix *m = new AGmMatrix(3, 3);
	if (m->Translate(rdx, rdy)) {
		delete m;
		Unref(dx, dy);
		return interp_error("TranslateTM()");
	}
	Unref(val); val = Ref(Matrix(m));
	Unref(dx, dy);
	return 0;
}
int AIpProcess::EvalScaleTM() {
	AGmNode *sx, *sy; double rsx, rsy;
	GetArg(sx, sy);
	if (!IsNum(sx, rsx) || !IsNum(sy, rsy)) {
		Unref(sx, sy);
		return interp_error("ScaleTM()");
	}
	AGmMatrix *m = new AGmMatrix(3, 3);
	if (m->Scale(rsx, rsy)) {
		delete m;
		Unref(sx, sy);
		return interp_error("TranslateTM()");
	}
	Unref(val); val = Ref(Matrix(m));
	Unref(sx, sy);
	return 0;
}
int AIpProcess::EvalRotateTM() {
	AGmNode *angle; double r_angle;
	GetArg(angle);
	if (!IsNum(angle, r_angle)) {
		Unref(angle);
		return interp_error("RotateTM()");
	}
	AGmMatrix *m = new AGmMatrix(3, 3);
	if (m->Rotate(r_angle)) {
		delete m;
		Unref(angle);
		return interp_error("TranslateTM()");
	}
	Unref(val); val = Ref(Matrix(m));
	Unref(angle);
	return 0;
}
int AIpProcess::EvalTransformXY() {
	AGmNode *tm, *x, *y; double rx, ry;
	GetArg(tm, x, y);
	if ((tm && !IsMatrix(tm)) || !IsNum(x, rx) || !IsNum(y, ry)) {
		Unref(tm, x, y);
		return interp_error("TransformXY()");
	}
	if (tm) {
		tm->a.m.v->Transform(rx, ry, rx, ry);
	}
	Unref(val); val = Ref(List(Real(rx), Real(ry)));
	Unref(tm, x, y);
	return 0;
}
int AIpProcess::EvalInverseXY() {
	AGmNode *tm, *x, *y; double rx, ry;
	GetArg(tm, x, y);
	if ((tm && !IsMatrix(tm)) || !IsNum(x, rx) || !IsNum(y, ry)) {
		Unref(tm, x, y);
		return interp_error("InverseXY()");
	}
	if (tm) {
		tm->a.m.v->Inverse(rx, ry, rx, ry);
	}
	Unref(val); val = Ref(List(Real(rx), Real(ry)));
	Unref(tm, x, y);
	return 0;
}
int AIpProcess::EvalTransformPoly() {
	AGmNode *tm, *polypoly;
	GetArg(tm, polypoly);
	if (tm && !IsMatrix(tm)) {
		Unref(tm, polypoly);
		return interp_error("TransformPoly()");
	}
	AGmNode *polypoly2;
	if (tm == NULL) {
		polypoly2 = polypoly;
	} else {
		tm->a.m.v->Transform(polypoly, polypoly2);
	}
	Ref(polypoly2);
	Unref(val); val = polypoly2;
	Unref(tm, polypoly);
	return 0;
}
int AIpProcess::EvalInversePoly() {
	AGmNode *tm, *polypoly;
	GetArg(tm, polypoly);
	if (!IsMatrix(tm)) {
		Unref(tm, polypoly);
		return interp_error("InversePoly()");
	}
	AGmNode *polypoly2;
	if (tm == NULL) {
		polypoly2 = polypoly;
	} else {
		tm->a.m.v->Inverse(polypoly, polypoly2);
	}
	Ref(polypoly2);
	Unref(val); val = polypoly2;
	Unref(tm, polypoly);
	return 0;
}
int AIpProcess::EvalTransformRect() {
	AGmNode *mat, *l, *t, *r, *b; double rl, rt, rr, rb;
	GetArg(mat, l, t, r, b);
	if ((mat && !IsMatrix(mat)) || !IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb)) {
		Unref(mat, l, t, r, b);
		return interp_error("TransformRect()");
	}
	double l1 = rl, t1 = rt, r1 = rr, b1 = rb;
	double l2 = rl, t2 = rb, r2 = rr, b2 = rt;
	if (mat) {
		mat->a.m.v->Transform(l1, t1, l1, t1);
		mat->a.m.v->Transform(r1, b1, r1, b1);
		mat->a.m.v->Transform(l2, b2, l2, b2);
		mat->a.m.v->Transform(r2, t2, r2, t2);
	}
	rl = min(min(l1, r1), min(l2, r2));
	rt = min(min(t1, b1), min(t2, b2));
	rr = max(max(l1, r1), max(l2, r2));
	rb = max(max(t1, b1), max(t2, b2));
	Unref(val); val = Ref(List(rl, rt, rr, rb));
	Unref(mat, l, t, r, b);
	return 0;
}

// graphics function
int AIpProcess::EvalPrint() {
	AGmNode *arg1;
	GetArg(arg1);
	if (arg1 == NULL) {
		ADvWindow::Print("null");
		Unref(arg1); return 0;
	}
	char s[256]; int count = 0;
	AGmString *ss; AGmFile *f;
	switch (arg1->type) {
	case AGM_NODE_INTEGER:
		art_sprintf_1(s, 256, "%ld", arg1->a.i.v);
		ADvWindow::Print(s);
		break;
	case AGM_NODE_REAL:
		art_sprintf_1(s, 256, "%.16g", arg1->a.r.v);
		ADvWindow::Print(s);
		break;
	case AGM_NODE_STRING:
		ADvWindow::Print(arg1->a.s.v->Str());
		break;
	case AGM_NODE_LIST:
		ss = new AGmString(10); f = new AGmFile();
		f->Create(ss, AGM_FILE_STR_WRITE);
		f->WriteList(arg1, count);
		ADvWindow::Print(ss->Str());
		delete f; delete ss;
		break;
	default:
		ADvWindow::Print("other");
		break;
	}
	Unref(arg1);
	return 0;
}
int AIpProcess::EvalAsk() {
	AGmNode *title, *text;
	GetArg(title, text);
	if (!IsString(title) || !IsString(text)) {
		Unref(title, text);
		return interp_error("Ask()");
	}
	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	ASSERT(pm);
	pm->AddLocalLoopProcess(this);
	char *buf = ADvWindow::Ask(title->a.s.v->Str(), text->a.s.v->Str());
	pm->RemoveLocalLoopProcess(this);
	Unref(val); val = Ref(String(buf));
	Unref(title, text);
	return 0;
}
int AIpProcess::EvalMenu() {
	AGmNode *wnd, *evnt, *menu;
	GetArg(wnd, evnt, menu);
	if (!IsWindow(wnd)) {
		Unref(wnd, evnt, menu);
		return interp_error("Menu()");
	}
	short id = 0; AGmNode *n = NULL;
	ADvMenuInfo *menu_info = ADvWindow::_CreateMenuInfo(menu, id);

	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	ASSERT(pm);
	pm->AddLocalLoopProcess(this);

	id = (short)ADvWindow::MenuSelect(wnd->a.w.v, evnt, menu_info);

	pm->RemoveLocalLoopProcess(this);

	if (id >= 0)
		n = ADvWindow::_SearchMenuItem(menu, menu_info, id);
	ADvWindow::_DeleteMenuInfo(menu_info);
	Unref(val); val = Ref(n);
	Unref(wnd, evnt, menu);
	return 0;
}
int AIpProcess::EvalGetReadFileName() {
	AGmNode *filter, *opt;
	GetArg(filter, opt);
	if (filter && !IsString(filter)) {
		Unref(filter, opt);
		return 1;
	}
	AGmString *dir, *name;
	char *title = NULL, *init_file = NULL, *def_ext = NULL, *init_dir = NULL;
	if (opt && IsList(opt)) {
		if (IsString(opt->H)) title = (char*)opt->H->a.s.v->Str();
		if (opt->T && IsList(opt->T)) {
			if (IsString(opt->TH))	init_file = (char*)opt->TH->a.s.v->Str();
			if (opt->TT && IsList(opt->TT)) {
				if (IsString(opt->TTH)) def_ext = (char*)opt->TTH->a.s.v->Str();
				if (opt->TTT && IsList(opt->TTT)) {
					if (IsString(opt->TTTH)) init_dir = (char*)opt->TTTH->a.s.v->Str();
				}
			}
		}
	}
	if (AIpProcess::GetFileName(1, dir, name,
			(filter ? filter->a.s.v->Str() : NULL),
			title, init_file, def_ext, init_dir)) {
		Unref(val); val = NULL;
	} else {
		Unref(val); val = Ref(List(String(dir), String(name)));
	}
	Unref(filter, opt);
	return 0;
}
int AIpProcess::EvalGetWriteFileName() {
	AGmNode *filter, *opt;
	GetArg(filter, opt);
	if (filter && !IsString(filter)) {
		Unref(filter, opt);
		return 1;
	}
	AGmString *dir, *name;
	char *title = NULL, *init_file = NULL, *def_ext = NULL, *init_dir = NULL;
	if (opt && IsList(opt)) {
		if (IsString(opt->H)) title = (char*)opt->H->a.s.v->Str();
		if (opt->T && IsList(opt->T)) {
			if (IsString(opt->TH))	init_file = (char*)opt->TH->a.s.v->Str();
			if (opt->TT && IsList(opt->TT)) {
				if (IsString(opt->TTH)) def_ext = (char*)opt->TTH->a.s.v->Str();
				if (opt->TTT && IsList(opt->TTT)) {
					if (IsString(opt->TTTH)) init_dir = (char*)opt->TTTH->a.s.v->Str();
				}
			}
		}
	}
	if (AIpProcess::GetFileName(0, dir, name,
			(filter ? filter->a.s.v->Str() : NULL),
			title, init_file, def_ext, init_dir)) {
		Unref(val); val = NULL;
	} else {
		Unref(val); val = Ref(List(String(dir), String(name)));
	}
	Unref(filter, opt);
	return 0;
}
int AIpProcess::GetFileName(int is_open, AGmString*& dir, AGmString*& name, const char* filter,
		const char *title, const char *initial_file, const char *def_ext, const char *initial_dir) {
	AGmString *dir_save = NULL;
	if (AGmFile::GetDir(dir_save)) dir_save = NULL;

	AGmString *init_dir;
	if (initial_dir) {
		init_dir = new AGmString(initial_dir);
	} else {
		if (AGmFile::GetDir(init_dir)) {
			init_dir = new AGmString(".");
		}
	}
#ifdef	_WINDOWS
	if (init_dir) init_dir->ChangeFileDelimiter('/', '\\');
	CWinApp *app = AfxGetApp();
	CWnd *pMainWnd = app->m_pMainWnd;
	app->m_pMainWnd = NULL;
	CFileDialog *dialog = new CFileDialog(is_open, NULL, initial_file,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(filter ? filter : "All Files (*.*)|*.*||"));
	dialog->m_ofn.lpstrTitle = title;
	dialog->m_ofn.lpstrDefExt = def_ext;
	dialog->m_ofn.lpstrInitialDir = init_dir->Str();
	INT_PTR ok = dialog->DoModal();
	app->m_pMainWnd = pMainWnd;
	delete init_dir;
	if (ok != IDOK) {
		if (dir_save) { AGmFile::ChDir(dir_save->Str()); delete dir_save; }
		delete dialog; return 1;
	}
	CString cs = dialog->GetPathName();
	delete dialog;
	AGmString *s = new AGmString(cs.GetBuffer());
	s->ChangeFileDelimiter('\\', '/');
	if (dir_save) { AGmFile::ChDir(dir_save->Str()); delete dir_save; }
#endif
#ifdef	_LINUX
	AGmFile::ChDir(init_dir->Str());
	if (title == NULL) {
		if (is_open) {
			title = "open filename ?";
		} else {
			title = "save filename ?";
		}
	}
	gchar *c = g_locale_to_utf8(title, -1, NULL, NULL, NULL);
	GtkWindow *parent = GTK_WINDOW(ADvGetApp()->m_pMainWnd->m_pToplevelWidget);
	GtkWidget *w = NULL;
	GtkFileChooser *chooser = NULL;
	gchar *filename = NULL;
	if (is_open) {
		w = gtk_file_chooser_dialog_new(
			c, parent, GTK_FILE_CHOOSER_ACTION_OPEN,
			AL_STR("Cancel"), GTK_RESPONSE_CANCEL, AL_STR("OK"), GTK_RESPONSE_ACCEPT, NULL);
		gint res = gtk_dialog_run(GTK_DIALOG(w));
		if (res == GTK_RESPONSE_CANCEL) {
			gtk_widget_destroy(w);
			if (dir_save) { AGmFile::ChDir(dir_save->Str()); delete dir_save; }
			g_free(c);
			return 1;
		}
		chooser = GTK_FILE_CHOOSER(w);
		filename = gtk_file_chooser_get_filename(chooser);
	} else {
		w = gtk_file_chooser_dialog_new(
			c, parent, GTK_FILE_CHOOSER_ACTION_SAVE,
			AL_STR("Cancel"), GTK_RESPONSE_CANCEL, AL_STR("OK"), GTK_RESPONSE_ACCEPT, NULL);
		chooser = GTK_FILE_CHOOSER(w);
		gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
		if (initial_file == NULL) {
			gtk_file_chooser_set_current_name(chooser, AL_STR("Untitled"));
		} else {
			gtk_file_chooser_set_current_name(chooser, initial_file);
		}
		gint res = gtk_dialog_run(GTK_DIALOG(w));
		if (res == GTK_RESPONSE_CANCEL) {
			gtk_widget_destroy(w);
			if (dir_save) { AGmFile::ChDir(dir_save->Str()); delete dir_save; }
			g_free(c);
			return 1;
		}
		filename = gtk_file_chooser_get_filename(chooser);
	}
	gtk_widget_destroy(w);
	g_free(c);
	delete init_dir;
	AGmString *s = new AGmString(filename);
	g_free(filename);
	if (dir_save) { AGmFile::ChDir(dir_save->Str()); delete dir_save; }
#endif
	dir = s->DirOfPath("/");
	name = s->NameOfPath("/");
	delete s;
	return 0;
}
#ifdef	_LINUX
static gint color_response_id;
static GdkRGBA gcolor;
static void color_response(GtkDialog *w, gint response_id, gpointer user_data) {
	color_response_id = response_id;
	if (response_id == GTK_RESPONSE_OK) {
		gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w), &gcolor);
	}
	gtk_widget_destroy(GTK_WIDGET(w));
}
#endif
int AIpProcess::EvalGetColor() {
#ifdef	_WINDOWS
	CWinApp *app = AfxGetApp();
	CWnd *pMainWnd = app->m_pMainWnd;
	app->m_pMainWnd = NULL;
	CColorDialog *dialog = new CColorDialog();
	INT_PTR ok = dialog->DoModal();
	app->m_pMainWnd = pMainWnd;
	if (ok != IDOK) { delete dialog; Unref(val); val = NULL; return 1; }
	COLORREF color = dialog->GetColor();
	delete dialog;
#endif
#ifdef	_LINUX
	GtkWindow *parent = GTK_WINDOW(ADvGetApp()->m_pMainWnd->m_pToplevelWidget);
	GtkWidget *w = gtk_color_chooser_dialog_new("color ?", parent);
	g_signal_connect(w, "response", G_CALLBACK(color_response), NULL);
	gtk_dialog_run(GTK_DIALOG(w));
	if (color_response_id != GTK_RESPONSE_OK) {
		Unref(val); val = NULL; return 1;
	}
	long color = (((long)(gcolor.red*255))) + (((long)(gcolor.green*255))<<8) + (((long)(gcolor.blue*255))<<16);
#endif
	Unref(val); val = Ref(Integer(color));
	return 0;
}
#ifdef	_LINUX
static gint font_response_id;
static PangoStyle font_style;
static PangoWeight font_weight;
static char *font_string;
#ifdef	_LINUX_DEBUG
static const char *font_family;
static const char *file_name;
#endif
static void font_response(GtkDialog *w, gint response_id, gpointer user_data) {
	font_response_id = response_id;
	if (response_id == GTK_RESPONSE_OK) {
		PangoFontDescription *font_desc = gtk_font_chooser_get_font_desc(GTK_FONT_CHOOSER(w));
		font_style = pango_font_description_get_style(font_desc);
		font_weight = pango_font_description_get_weight(font_desc);
		font_string = pango_font_description_to_string(font_desc);
#ifdef	_LINUX_DEBUG
		font_family = pango_font_description_get_family(font_desc);
		printf("font_family = %s\n", font_family);
		file_name = pango_font_description_to_filename(font_desc);
		printf("filename = %s\n", file_name);
#endif
		pango_font_description_free(font_desc);
	}
	gtk_widget_destroy(GTK_WIDGET(w));
}
#endif
int AIpProcess::EvalGetFont() {
	char s[256];
	long height, color, bold, italic, underline;
#ifdef	_WINDOWS
	CWinApp *app = AfxGetApp();
	CWnd *pMainWnd = app->m_pMainWnd;
	app->m_pMainWnd = NULL;
	CFontDialog *dialog = new CFontDialog();
	INT_PTR ok = dialog->DoModal();
	app->m_pMainWnd = pMainWnd;
	if (ok != IDOK) { delete dialog; Unref(val); val = NULL; return 1; }
	CString cs = dialog->GetFaceName();
	art_strcpy(s, 256, cs);
	height = dialog->GetSize()/10;
	color = dialog->GetColor();
	bold = dialog->IsBold();
	italic = dialog->IsItalic();
	underline = dialog->IsUnderline();
	delete dialog;
#endif
#ifdef	_LINUX
	GtkWindow *parent = GTK_WINDOW(ADvGetApp()->m_pMainWnd->m_pToplevelWidget);
	GtkWidget *w = gtk_font_chooser_dialog_new("font ?", parent);
	g_signal_connect(w, "response", G_CALLBACK(font_response), NULL);
	gtk_dialog_run(GTK_DIALOG(w));
	if (font_response_id != GTK_RESPONSE_OK) {
		Unref(val); val = NULL; return 1;
	}
	strcpy(s, font_string);
	height = 14;
	color = 0;
	bold = font_weight == PANGO_WEIGHT_BOLD ? 1 : 0;
	italic = PANGO_STYLE_ITALIC ? 1 : 0;
	underline = 0;
#endif
	Unref(val);
	val = Ref(List(String(s), Integer(height), Integer(0), Integer(color),
			Integer(bold), Integer(italic), Integer(underline)));
	return 0;
}
int AIpProcess::EvalMessageText() {
	AGmNode *wnd, *text;
	GetArg(wnd, text);
	if (!IsWindow(wnd) || !IsString(text)) {
		Unref(wnd, text);
		return interp_error("MessageText()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->View()) {
		CFrameWnd *ch = wnd->a.w.v->View()->GetParentFrame();
		if (ch) {
			CFrameWnd *fr = ch->GetParentFrame();
			if (fr) {
				fr->SetMessageText(text->a.s.v->Str());
			} else {
				ch->SetMessageText(text->a.s.v->Str());
			}
		}
	}
	if (aw->Frame()) {
		aw->Frame()->SetMessageText(text->a.s.v->Str());
	}
	Unref(wnd, text);
	return 0;
}
int AIpProcess::EvalInputByCEdit() {
	AGmNode *wnd, *l, *t, *r, *b, *text; double rl = 0, rt = 0, rr = 0, rb = 0;
	GetArg(wnd, l, t, r, b, text);
	if (!IsWindow(wnd) || !IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb) ||
				!IsString(text)) {
		Unref(wnd, l, t, r, b, text);
		interp_error("InputByCEdit()");
	}

#ifdef	_WINDOWS
	ADvWindow *aw = wnd->a.w.v;
	int err = 1;
	ADvSingleLineCEdit *edit = new ADvSingleLineCEdit();
	long id = (long)GetUniqueID();
	DWORD style = ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER;
	RECT rect;
	rect.left	= (int)rl;
	rect.top	= (int)rt;
	rect.right	= (int)rr;
	rect.bottom = (int)rb;
	if (aw->View()) {
		err = edit->Create(style, rect, aw->View(), (UINT)id);
	} else if (aw->OleCtrl()) {
		err = edit->Create(style, rect, aw->OleCtrl(), (UINT)id);
	}
	if (err) {
		delete edit;
		ReleaseUniqueID(id);
		Unref(wnd, l, t, r, b, text);
		return interp_error("InputByCEdit()");
	}
	char *s = ADvCEdit::toCEditString(text->a.s.v->Str());
	edit->ReplaceSel(s);
	delete s;
	edit->LocalLoop();
	char line[256]; int cc;
	cc = edit->GetLine(0, line, 256);
	line[cc] = 0;
	Unref(val); val = Ref(String(line));
	delete edit;
	ReleaseUniqueID(id);
#endif
#ifdef	_LINUX
	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	ASSERT(pm);
	pm->AddLocalLoopProcess(this);

	char *buff = ADvWindow::Ask("", text->a.s.v->Str());
	Unref(val); val = Ref(String(buff));

	pm->RemoveLocalLoopProcess(this);
#endif

	Unref(wnd, l, t, r, b, text);
	return 0;
}

int AIpProcess::EvalCreateWindow() {
	AGmNode *opt;
	GetArg(opt);
	SetValue(NULL, AGM_VM_KEY_ROOT_CLASS, RootClass());
	ADvWindow *aw = new ADvWindow();
	Unref(val);
	if (opt && IsList(opt) && IsInteger(opt->H) && (opt->HI & 0x0040) != 0) {
		val = Ref(aw->CreateEdit(NULL, NULL, opt, 1));
	} else {
		val = Ref(aw->Create(opt));
	}
	RemoveValue(NULL, AGM_VM_KEY_ROOT_CLASS);
	aw->Arg(Cons(List(NmEvent(), NULL, NULL)));
	Unref(opt);
	return 0;
}

int AIpProcess::EvalUserEdit() {
	AGmNode *cmd, *aw, *arg1, *arg2;
	GetArg(cmd, aw, arg1, arg2);
	if (!IsString(cmd) || !IsWindow(aw) || aw->a.w.v == NULL || aw->a.w.v->EditCtrl() == NULL) {
		Unref(cmd, aw, arg1, arg2);
		return interp_error("UserEdit()");
	}
	ADvCEdit *edit = aw->a.w.v->EditCtrl();
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "get_sel") == 0) {
		long from, to;
		edit->GetSel(from, to);
		val = Ref(List(Integer(from), Integer(to)));
	} else if (strcmp(s, "set_sel") == 0) {
		if (!IsInteger(arg1) || !IsInteger(arg2)) {
			Unref(cmd, aw, arg1, arg2);
			return interp_error("UserEdit() set_sel");
		}
		edit->SetSel((int)arg1->a.i.v, (int)arg2->a.i.v);
	} else if (strcmp(s, "replace_sel") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, aw, arg1, arg2);
			return interp_error("UserEdit() replace_sel");
		}
#ifdef	_WINDOWS
		char *ss = ADvCEdit::toCEditString(arg1->a.s.v->Str());
		edit->SetFocus();
		edit->ReplaceSel(ss);
		delete [] ss;
#endif
#ifdef	_LINUX
		edit->SetLast();
		edit->ReplaceSel(arg1->a.s.v->Str());
		edit->JumpToBottom();
#endif
	} else if (strcmp(s, "line_count") == 0) {
		val = Ref(Integer(edit->GetLineCount()));
	} else if (strcmp(s, "line_index") == 0) {
		int lno = -1;
		if (IsInteger(arg1)) lno = (int)arg1->a.i.v;
		val = Ref(Integer(edit->LineIndex(lno)));
	} else if (strcmp(s, "line_length") == 0) {
		int lno = -1;
		if (IsInteger(arg1)) lno = (int)arg1->a.i.v;
		val = Ref(Integer(edit->LineLength(lno)));
	} else if (strcmp(s, "get_line") == 0) {
		if (!IsInteger(arg1)) {
			Unref(cmd, aw, arg1, arg2);
			return interp_error("UserEdit() get_line");
		}
		char str[ADV_MAX_LINE_LENGTH];
		long from = edit->LineIndex((int)arg1->a.i.v);
		long to = edit->LineIndex((int)(arg1->a.i.v + 1)) - 1;
		edit->SetSel(from, to);
		long n = edit->GetSelText(str, ADV_MAX_LINE_LENGTH - 1);
		str[n] = 0;
#ifdef	_WINDOWS
		char *st = ADvCEdit::fromCEditString(str);
		val = Ref(String(st));
		delete [] st;
#endif
#ifdef	_LINUX
		val = Ref(String(str));
#endif
	} else if (strcmp(s, "line_from_pos") == 0) {
		int pos = -1;
		if (IsInteger(arg1)) pos = (int)arg1->a.i.v;
		val = Ref(Integer(edit->LineFromChar(pos)));
	} else if (strcmp(s, "use_as_output") == 0) {
		void *value;
		GetValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW, value);
		if (value) ReleaseNode((AGmNode*)value);
		SetValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW, aw);
	} else {
		Unref(cmd, aw, arg1, arg2);
		return interp_error("UserEdit()");
	}
	Unref(cmd, aw, arg1, arg2);
	return 0;
}

int AIpProcess::EvalSetWindowArg() {
	AGmNode *wnd, *name, *value;
	GetArg(wnd, name, value);
	if (!IsWindow(wnd) || !IsString(name)) {
		Unref(wnd, name, value);
		return interp_error("SetWindowArg()");
	}
	ADvWindow *aw = wnd->a.w.v;
	AGmNode *w_arg = aw->Arg();
	AGmNode *u_name = UniqueStr(name);
	AGmNode **addr_v;
	if (SearchVar(w_arg, u_name, addr_v) == 0) {
		Set(*addr_v, value);
	} else {
		w_arg = Cons(List(u_name, value, NULL), w_arg);
		aw->Arg(w_arg);
	}
	Unref(wnd, name, value);
	return 0;
}
int AIpProcess::EvalSetDrawProc() {
	AGmNode *wnd, *proc;
	GetArg(wnd, proc);
	if (!IsWindow(wnd)) {
		Unref(wnd, proc);
		return interp_error("SetDrawProc()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (!IsList(proc) || !IsList(proc->T) ||
			!IsList(proc->TT) || !IsList(proc->TTT) ||
			!(proc->HI & AIP_STATIC) ) {
		Unref(wnd, proc);
		return interp_error("SetDrawProc()");
	}
	aw->DrawProg(proc);
	Unref(wnd, proc);
	return 0;
}
int AIpProcess::EvalSetEventProc() {
	AGmNode *wnd, *proc;
	GetArg(wnd, proc);
	if (!IsWindow(wnd)) {
		Unref(wnd, proc);
		return interp_error("SetEventProc()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (!IsList(proc) || !IsList(proc->T) ||
			!IsList(proc->TT) || !IsList(proc->TTT) ||
			!(proc->HI & AIP_STATIC) ) {
		Unref(wnd, proc);
		return interp_error("SetEventProc()");
	}
	aw->EventProg(proc);
	Unref(wnd, proc);
	return 0;
}
int AIpProcess::EvalSetMoveCtrlProc() {
	AGmNode *wnd, *proc;
	GetArg(wnd, proc);
	if (!IsWindow(wnd)) {
		Unref(wnd, proc);
		return interp_error("SetMoveCtrlProc()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (!IsList(proc) || !IsList(proc->T) ||
			!IsList(proc->TT) || !IsList(proc->TTT) ||
			!(proc->HI & AIP_STATIC) ) {
		Unref(wnd, proc);
		return interp_error("SetMoveCtrlProc()");
	}
	aw->MoveCtrlProg(proc);
	Unref(wnd, proc);
	return 0;
}
int AIpProcess::EvalSetDestroyProc() {
	AGmNode *wnd, *proc;
	GetArg(wnd, proc);
	if (!IsWindow(wnd)) {
		Unref(wnd, proc);
		return interp_error("SetDestroyProc()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (!IsList(proc) || !IsList(proc->T) ||
			!IsList(proc->TT) || !IsList(proc->TTT) ||
			!(proc->HI & AIP_STATIC) ) {
		Unref(wnd, proc);
		return interp_error("SetDestroyProc()");
	}
	aw->DestroyProg(proc);
	Unref(wnd, proc);
	return 0;
}
int AIpProcess::EvalSetActivateProc() {
	AGmNode *wnd, *proc;
	GetArg(wnd, proc);
	if (!IsWindow(wnd)) {
		Unref(wnd, proc);
		return interp_error("SetActivateProc()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (!IsList(proc) || !IsList(proc->T) ||
			!IsList(proc->TT) || !IsList(proc->TTT) ||
			!(proc->HI & AIP_STATIC) ) {
		Unref(wnd, proc);
		return interp_error("SetActivateProc()");
	}
	aw->ActivateProg(proc);
	Unref(wnd, proc);
	return 0;
}
int AIpProcess::EvalSetTextInsProc() {
	AGmNode *wnd, *proc;
	GetArg(wnd, proc);
	if (!IsWindow(wnd)) {
		Unref(wnd, proc);
		return interp_error("SetTextInsProc()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->Type() != ADV_WINDOW_EDIT) {
		Unref(wnd, proc);
		return interp_error("SetTextInsProc()");
	}
	if (!IsList(proc) || !IsList(proc->T) ||
			!IsList(proc->TT) || !IsList(proc->TTT) ||
			!(proc->HI & AIP_STATIC) ) {
		Unref(wnd, proc);
		return interp_error("SetTextInsProc()");
	}
	aw->TextInsProg(proc);
	Unref(wnd, proc);
	return 0;
}

int AIpProcess::EvalSetTitle() {
	AGmNode *wnd, *title;
	GetArg(wnd, title);
	if (!IsWindow(wnd) || !IsString(title)) {
		Unref(wnd, title);
		return interp_error("SetTitle()");
	}
	ADvWindow *aw = wnd->a.w.v;
	aw->SetTitle(title->a.s.v->Str());
	Unref(wnd, title);
	return 0;
}
int AIpProcess::EvalGetTitle() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("GetTitle()");
	}
	ADvWindow *aw = wnd->a.w.v;
	Unref(val); val = Ref(String(aw->GetTitle()));
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalSetMenu() {
	AGmNode *wnd, *menu;
	GetArg(wnd, menu);
	if (!IsWindow(wnd) || menu == NULL) {
		Unref(wnd, menu);
		return interp_error("SetMenu()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->View()) { // SDI frame window
		CFrameWnd *frm = aw->View()->GetParentFrame();
		ADvCFrameWnd *frame = (ADvCFrameWnd*)frm;
#ifdef	_WINDOWS
		if (!frm || !frm->IsKindOf(RUNTIME_CLASS(ADvCFrameWnd))) {
			Unref(wnd, menu);
			return interp_error("SetMenu()");
		}
		CMenu *m = new CMenu;
		m->CreateMenu();
		CreateArc(menu, PtrCell(m), UniqueStr("CMenu"));
		if (CreateMenu(m, menu)) {
			DestroyMenu(menu);
			Unref(wnd, menu);
			return interp_error("SetMenu()");
		}
		frame->m_bAutoMenuEnable = FALSE;
		frame->SetMenu(m);
		AGmNode *n = frame->Menu();
		if (n) DestroyMenu(n);
		frame->Menu(menu);
#endif
#ifdef	_LINUX
		if (!frm || !(frm->_cwnd_type & ADV_WNDTYPE_ADVCFRAME)) {
			Unref(wnd, menu);
			return interp_error("SetMenu()");
		}
		CMenu *m = frame->_cmenu;
		if (CreateMenu(m, menu)) {
			Unref(wnd, menu);
			return interp_error("SetMenu()");
		}
		frame->Menu(menu);
#endif

	} else if (aw->Frame()) { // MDI frame window
		ADvCMDIFrameWnd* frame = aw->Frame();
#ifdef	_WINDOWS
		CMenu *m = new CMenu;
		m->CreateMenu();
		CreateArc(menu, PtrCell(m), UniqueStr("CMenu"));
		if (CreateMenu(m, menu)) {
			DestroyMenu(menu);
			Unref(wnd, menu);
			return interp_error("SetMenu()");
		}
		frame->m_bAutoMenuEnable = FALSE;
		frame->SetMenu(m);
		AGmNode *n = frame->Menu();
		if (n) DestroyMenu(n);
		frame->Menu(menu);
#endif
#ifdef	_LINUX
		CMenu *m = frame->_cmenu;
		if (CreateMenu(m, menu)) {
			Unref(wnd, menu);
			return interp_error("SetMenu()");
		}
		frame->Menu(menu);
#endif

	} else {
		Unref(wnd, menu);
		return interp_error("SetMenu()");
	}
	Unref(wnd, menu);
	return 0;
}
#ifdef	_LINUX
void menu_cb(GtkMenuItem* w, gpointer gdata) {
	AGmNode *n2 = (AGmNode*)gdata;
	AGmNode *ptr_cell = AGm::dst_node(n2, AGm::UniqueStr("RootClass"));
	AGmNode *_root_class = (AGmNode*)ptr_cell->a.p.ptr;
	AGm::Unref(EvalCallbackV(_root_class, NULL, n2->TTH, n2->TTTH));
}
#endif
int AIpProcess::CreateMenu(CMenu *m1, AGmNode *n0) {
	AGmNode *n1 = dst_node(n0, UniqueStr("menu"));
	if (n1 == NULL) return 1;
	AGmLink *l;
	for (l = next_outlink(n1, NULL);
			l; l = next_outlink(n1, l)) {
		AGmNode *n2 = l->arc->dst;
		if (!IsString(n2->H) || !n2->T || !n2->TT || !n2->TTT)
			return 1;
#ifdef	_WINDOWS
		long nID = (long)GetUniqueID();
		CreateArc(n2, Integer(nID), UniqueStr("nID"));
		if (n2->TH == NULL) {
			m1->AppendMenu(MF_STRING, nID, n2->H->a.s.v->Str());
		} else {
			CMenu *m2 = new CMenu;
			m2->CreatePopupMenu();
			CreateArc(n2->TH, PtrCell(m2), UniqueStr("CMenu"));
			m1->AppendMenu(MF_POPUP, (int)m2->m_hMenu, n2->H->a.s.v->Str());
			if (CreateMenu(m2, n2->TH))
				return 1;
		}
#endif
#ifdef	_LINUX
		CreateArc(n2, PtrCell(_root_class), UniqueStr("RootClass"));
		if (n2->TH == NULL) {
			m1->AppendMenu(MF_STRING, n2, n2->H->a.s.v->Str(),
				(GCallback)menu_cb);
		} else {
			CMenu *m2 = new CMenu;
			m1->AppendMenu(MF_POPUP, m2, n2->H->a.s.v->Str());
			if (CreateMenu(m2, n2->TH))
				return 1;
		}
#endif
	}
	return 0;
}
#ifdef	_WINDOWS
AGmNode *AIpProcess::SearchMenu(AGmNode * n0, long nID) {
	AGmNode *n1 = dst_node(n0, UniqueStr("menu"));
	if (n1 == NULL) return NULL;
	AGmLink *l;
	for (l = next_outlink(n1, NULL);
			l; l = next_outlink(n1, l)) {
		AGmNode *n2 = l->arc->dst;
		AGmNode *n3 = dst_node(n2, UniqueStr("nID"));
		if (n3 && n3->a.i.v == nID) return n2;
		if (n2->TH) {
			AGmNode *n3 = SearchMenu(n2->TH, nID);
			if (n3) return n3;
		}
	}
	return NULL;
}
void AIpProcess::DestroyMenu(AGmNode *n0) {
	AGmNode *n1;
	n1 = dst_node(n0, UniqueStr("CMenu"));
	if (n1) {
		CMenu *m = (CMenu*)n1->a.p.ptr;
		delete m;
	}
	n1 = dst_node(n0, UniqueStr("menu"));
	if (n1 == NULL) return;
	AGmLink *l;
	for (l = next_outlink(n1, NULL);
			l; l = next_outlink(n1, l)) {
		AGmNode *n2 = l->arc->dst;
		AGmNode *n3 = dst_node(n2, UniqueStr("nID"));
		if (n3) ReleaseUniqueID(n3->a.i.v);
		if (n2->TH) {
			DestroyMenu(n2->TH);
		}
	}
}
#endif

int AIpProcess::EvalGetWndRect() {
	AGmNode *wnd, *opt;
	GetArg(wnd, opt);
	if (!IsWindow(wnd) || !IsInteger(opt)) {
		Unref(wnd, opt);
		return interp_error("GetWndRect()");
	}
	ADvWindow *aw = wnd->a.w.v;
#ifdef	_WINDOWS
	if (aw->View() && opt->a.i.v == 0) {
		CWnd *cvw = aw->View();
		CWnd *chw = cvw->GetParentFrame(), *frw = NULL;
		if (chw)
			frw = chw->GetParentFrame();
		if (frw == NULL) {
			if (chw) { // SDI Frame
				CRect r;
				chw->GetWindowRect(&r);
				Unref(val);
#if	_WINDOWS && _64BIT
				val = Ref(List((long long)r.left, (long long)r.top, (long long)r.Width(), (long long)r.Height()));
#else
				val = Ref(List((long)r.left, (long)r.top, (long)r.Width(), (long)r.Height()));
#endif
				Unref(wnd, opt);
				return 0;
			} else {
				Unref(wnd, opt);
				return interp_error("GetWndRect(): fail to GetParentFrame()");
			}
		}
		CRect cr, fr; // MDI Child
		chw->GetWindowRect(&cr);
		frw->GetWindowRect(&fr);
		CPoint p(fr.left, fr.top);
		frw->ScreenToClient(&p);
		long left = cr.left - fr.left + p.x - GetSystemMetrics(SM_CXFRAME)/2;
		long top = cr.top - fr.top + p.y - GetSystemMetrics(SM_CYFRAME)/2;
		Unref(val);
#if	_WINDOWS && _64BIT
		val = Ref(List(left, top, (long long)cr.Width(), (long long)cr.Height()));
#else
		val = Ref(List(left, top, (long)cr.Width(), (long)cr.Height()));
#endif
		Unref(wnd, opt);
		return 0;
	}
	if (aw->View() && opt->a.i.v == 1) { // Client
		CWnd *cvw = aw->View();
		CRect r;
		cvw->GetClientRect(&r);
		Unref(val);
#if	_WINDOWS && _64BIT
		val = Ref(List((long long)0, (long long)0,(long long)r.Width(), (long long)r.Height()));
#else
		val = Ref(List((long)0, (long)0,(long)r.Width(), (long)r.Height()));
#endif
		Unref(wnd, opt);
		return 0;
	}
	if (aw->Frame() && opt->a.i.v == 0) { // MDI Frame
		CRect r;
		aw->Frame()->GetWindowRect(&r);
		Unref(val);
#if	_WINDOWS && _64BIT
		val = Ref(List((long long)r.left, (long long)r.top, (long long)r.Width(), (long long)r.Height()));
#else
		val = Ref(List((long)r.left, (long)r.top, (long)r.Width(), (long)r.Height()));
#endif
		Unref(wnd, opt);
		return 0;
	}
#endif
#ifdef	_LINUX
	if (aw->View()) {
		ADvCView *cvw = aw->View();
		CFrameWnd *chw = cvw->GetParentFrame();
		if (chw == NULL || chw->m_pToplevelWidget == NULL) {
			Unref(wnd, opt);
			return interp_error("GetWndRect(): fail to get ParentFrame()");
		}
		int l, t, w, h;
		gdk_window_get_position(gtk_widget_get_window(chw->m_pToplevelWidget), &l, &t);
		gtk_window_get_size(GTK_WINDOW(chw->m_pToplevelWidget), &w, &h);
		Unref(val);
		val = Ref(List((long)l, (long)t, (long)w, (long)h));
		Unref(wnd, opt);
		return 0;
	}
	if (aw->Frame()) {
		CFrameWnd *frw = aw->Frame();
		if (frw == NULL || frw->m_pToplevelWidget == NULL) {
			Unref(wnd, opt);
			return interp_error("GetWndRect(): fail to get FrameWnd()");
		}
		int l, t, w, h;
		gdk_window_get_position(gtk_widget_get_window(frw->m_pToplevelWidget), &l, &t);
		gtk_window_get_size(GTK_WINDOW(frw->m_pToplevelWidget), &w, &h);
		Unref(val);
		val = Ref(List((long)l, (long)t, (long)w, (long)h));
		Unref(wnd, opt);
		return 0;
	}
#endif
	Unref(wnd, opt);
	return interp_error("GetWndRect()");
}
int AIpProcess::EvalSetWndRect() {
	AGmNode *wnd, *l, *t, *w, *h; double rl, rt, rw, rh;
	GetArg(wnd, l, t, w, h);
	if (!IsWindow(wnd) ||
			!IsNum(l, rl) || !IsNum(t, rt) ||
			!IsNum(w, rw) || !IsNum(h, rh)) {
		Unref(wnd, l, t, w, h);
		return interp_error("SetWndRect()");
	}
	ADvWindow *aw = wnd->a.w.v;
#ifdef	_WINDOWS
	if (aw->View()) {
		CWnd *cvw = aw->View();
		CWnd *chw = cvw->GetParentFrame();
		if (chw == NULL) {
			Unref(wnd, l, t, w, h);
			return interp_error("SetWndRect(): fail to GetParentFrame()");
		}
		chw->MoveWindow((int)rl, (int)rt, (int)rw, (int)rh);
		Unref(wnd, l, t, w, h);
		return 0;
	}
	if (aw->Frame()) {
		aw->Frame()->MoveWindow((int)rl, (int)rt, (int)rw, (int)rh);
		Unref(wnd, l, t, w, h);
		return 0;
	}
#endif
#ifdef	_LINUX
	if (aw->View()) {
		ADvCView *cvw = aw->View();
		CFrameWnd *chw = cvw->GetParentFrame();
		if (chw == NULL || chw->m_pToplevelWidget == NULL) {
			Unref(wnd, l, t, w, h);
			return interp_error("SetWndRect(): fail to get ParentFrame()");
		}
		gdk_window_move_resize(gtk_widget_get_window(chw->m_pToplevelWidget), (int)rl, (int)rt, (int)rw, (int)rh);
		Unref(wnd, l, t, w, h);
		return 0;
	}
	if (aw->Frame()) {
		CFrameWnd *frw = aw->Frame();
		if (frw == NULL || frw->m_pToplevelWidget == NULL) {
			Unref(wnd, l, t, w, h);
			return interp_error("SetWndRect(): fail to get FrameWnd()");
		}
		gdk_window_move_resize(gtk_widget_get_window(frw->m_pToplevelWidget), (int)rl, (int)rt, (int)rw, (int)rh);
		Unref(wnd, l, t, w, h);
		return 0;
	}
#endif
	Unref(wnd, l, t, w, h);
	return interp_error("SetWndRect()");
}
int AIpProcess::EvalResizeWnd() {
	AGmNode *wnd, *w, *h; double rw, rh;
	GetArg(wnd, w, h);
	if (!IsWindow(wnd) || !IsNum(w, rw) || !IsNum(h, rh)) {
		Unref(wnd, w, h);
		return interp_error("ResizeWnd()");
	}
	ADvWindow *aw = wnd->a.w.v;
#ifdef	_WINDOWS
	if (aw->View()) {
		CWnd *cvw = aw->View();
		CWnd *chw = cvw->GetParentFrame(), *frw = NULL;
		if (chw)
			frw = chw->GetParentFrame();
		if (frw == NULL) {
			if (chw) { // SDI
				CRect r;
				chw->GetWindowRect(&r);
				chw->MoveWindow((int)r.left, (int)r.top, (int)rw, (int)rh);
				Unref(wnd, w, h);
				return 0;
			} else {
				Unref(wnd, w, h);
				return interp_error("ResizeWnd(): fail to GetParentFrame()");
			}
		}
		CRect cr, fr; // MDI
		chw->GetWindowRect(&cr);
		frw->GetWindowRect(&fr);
		CPoint p(fr.left, fr.top);
		frw->ScreenToClient(&p);
		long left = cr.left - fr.left + p.x - GetSystemMetrics(SM_CXFRAME)/2;
		long top = cr.top - fr.top + p.y - GetSystemMetrics(SM_CYFRAME)/2;
		chw->MoveWindow((int)left, (int)top, (int)rw, (int)rh);
		Unref(wnd, w, h);
		return 0;
	}
	if (aw->Frame()) {
		CRect r;
		aw->Frame()->GetWindowRect(&r);
		aw->Frame()->MoveWindow(r.left, r.top, (int)rw, (int)rh);
		Unref(wnd, w, h);
		return 0;
	}
#endif
#ifdef	_LINUX
	if (aw->View()) {
		ADvCView *cvw = aw->View();
		CFrameWnd *chw = cvw->GetParentFrame();
		if (chw == NULL || chw->m_pToplevelWidget == NULL) {
			Unref(wnd, w, h);
			return interp_error("ResizeWnd(): fail to get ParentFrame()");
		}
		gdk_window_resize(gtk_widget_get_window(chw->m_pToplevelWidget), (int)rw, (int)rh);
		Unref(wnd, w, h);
		return 0;
	}
	if (aw->Frame()) {
		CFrameWnd *frw = aw->Frame();
		if (frw == NULL || frw->m_pToplevelWidget == NULL) {
			Unref(wnd, w, h);
			return interp_error("ResizeWnd(): fail to get FrameWnd()");
		}
		gdk_window_resize(gtk_widget_get_window(frw->m_pToplevelWidget), (int)rw, (int)rh);
		Unref(wnd, w, h);
		return 0;
	}
#endif
	Unref(wnd, w, h);
	return interp_error("ResizeWnd()");
}
int AIpProcess::EvalMoveWnd() {
	AGmNode *wnd, *l, *t; double rl, rt;
	GetArg(wnd, l, t);
	if (!IsWindow(wnd) || !IsNum(l, rl) || !IsNum(t, rt)) {
		Unref(wnd, l, t);
		return interp_error("MoveWnd()");
	}
	ADvWindow *aw = wnd->a.w.v;
#ifdef	_WINDOWS
	if (aw->View()) {
		CWnd *cvw = aw->View();
		CWnd *chw = cvw->GetParentFrame();
		CRect cr;
		chw->GetWindowRect(&cr);
		chw->MoveWindow((int)rl, (int)rt, cr.Width(), cr.Height());
		Unref(wnd, l, t);
		return 0;
	}
	if (aw->Frame()) {
		CRect r;
		aw->Frame()->GetWindowRect(&r);
		aw->Frame()->MoveWindow((int)rl, (int)rt, r.Width(), r.Height());
		Unref(wnd, l, t);
		return 0;
	}
#endif
#ifdef	_LINUX
	if (aw->View()) {
		ADvCView *cvw = aw->View();
		CFrameWnd *chw = cvw->GetParentFrame();
		if (chw == NULL || chw->m_pToplevelWidget == NULL) {
			Unref(wnd, l, t);
			return interp_error("MoveWnd(): fail to get ParentFrame()");
		}
		gdk_window_move(gtk_widget_get_window(chw->m_pToplevelWidget), (int)rl, (int)rt);
		Unref(wnd, l, t);
		return 0;
	}
	if (aw->Frame()) {
		CFrameWnd *frw = aw->Frame();
		if (frw == NULL || frw->m_pToplevelWidget == NULL) {
			Unref(wnd, l, t);
			return interp_error("MoveWnd(): fail to get FrameWnd()");
		}
		gdk_window_move(gtk_widget_get_window(frw->m_pToplevelWidget), (int)rt, (int)rt);
		Unref(wnd, l, t);
		return 0;
	}
#endif
	Unref(wnd, l, t);
	return interp_error("MoveWnd()");
}
int AIpProcess::EvalShowWnd() {
	AGmNode *wnd;
	GetArg(wnd);
	if (wnd == NULL && ADvGetApp()) {
		ADvGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
		return 0;
	}
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("ShowWnd()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->View()) {
		aw->View()->GetParentFrame()->ShowWindow(SW_SHOW);
		Unref(wnd);
		return 0;
	}
	if (aw->Frame()) {
		aw->Frame()->ShowWindow(SW_SHOW);
		Unref(wnd);
		return 0;
	}
	Unref(wnd);
	return interp_error("ShowWnd()");
}
int AIpProcess::EvalHideWnd() {
	AGmNode *wnd;
	GetArg(wnd);
	if (wnd == NULL && ADvGetApp()) {
		ADvGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
		return 0;
	}
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("HideWnd()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->View()) {
		aw->View()->GetParentFrame()->ShowWindow(SW_HIDE);
		Unref(wnd);
		return 0;
	}
	if (aw->Frame()) {
		aw->Frame()->ShowWindow(SW_HIDE);
		Unref(wnd);
		return 0;
	}
	Unref(wnd);
	return interp_error("HideWnd()");
}
int AIpProcess::EvalDestroyWnd() {
	AGmNode *wnd;
	GetArg(wnd);
	if (wnd == NULL) {
		void *value;
		GetValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW, value);
		if (value)
			ReleaseNode((AGmNode*)value);
		return 0;
	}
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("DestroyWnd()");
	}
	ReleaseNode(wnd);
	Unref(wnd);
	return 0;
}

int AIpProcess::EvalPushClipping() {
	AGmNode *wnd, *polypoly;
	GetArg(wnd, polypoly);
	if (!IsWindow(wnd)) {
		Unref(wnd, polypoly);
		return interp_error("PushClipping()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PushClipping(List(polypoly))) {
		Unref(wnd, polypoly);
		return interp_error("PushClipping()");
	}
	Unref(wnd, polypoly);
	return 0;
}
int AIpProcess::EvalPopClipping() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("PopClipping()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PopClipping()) {
		Unref(wnd);
		return interp_error("PopClipping()");
	}
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalClipping() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("TM()");
	}
	ADvWindow *aw = wnd->a.w.v;
	Unref(val); val = Ref(aw->Clipping());
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalIsClipped() {
	AGmNode *wnd, *m, *l, *t, *r, *b; double rl, rt, rr, rb;
	GetArg(wnd, m, l, t, r, b);
	if (!IsWindow(wnd) || (m && !IsMatrix(m)) ||
			!IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb)) {
		Unref(wnd, m, l, t, r, b);
		return interp_error("IsClipped()");
	}
	if (m) {
		double l1, t1, r1, b1, l2, t2, r2, b2;
		m->a.m.v->Transform(rl, rt, l1, t1);
		m->a.m.v->Transform(rr, rb, r1, b1);
		m->a.m.v->Transform(rl, rb, l2, b2);
		m->a.m.v->Transform(rr, rt, r2, t2);
		rl = min(min(l1, r1), min(l2, r2));
		rt = min(min(t1, b1), min(t2, b2));
		rr = max(max(l1, r1), max(l2, r2));
		rb = max(max(t1, b1), max(t2, b2));
	}
	ADvWindow *aw = wnd->a.w.v;
	AGmNode* clip = aw->Clipping();
	double _l = 0, _t = 0, _r = 0, _b = 0;
	if (clip) {
		AGmNode *n; AGmLink *l;
		n = dst_node(clip);
		l = next_outlink(n, NULL);
		_l = l->arc->dst->H->a.r.v;
		_t = l->arc->dst->TH->a.r.v;
		l = next_outlink(n, l);
		l = next_outlink(n, l);
		_r = l->arc->dst->H->a.r.v;
		_b = l->arc->dst->TH->a.r.v;
	}
	if (clip && (rl > _r || rt > _b || rr < _l || rb < _t)) {
		Unref(val); val = Ref(Integer(1));
	} else {
		Unref(val); val = NULL;
	}
	Unref(wnd, m, l, t, r, b);
	return 0;
}
int AIpProcess::EvalPushBgColor() {
	AGmNode *wnd, *color;
	GetArg(wnd, color);
	if (!IsWindow(wnd)) {
		Unref(wnd, color);
		return interp_error("PushBgColor()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PushBgColor(List(color))) {
		Unref(wnd, color);
		return interp_error("PushBgColor()");
	}
	Unref(wnd, color);
	return 0;
}
int AIpProcess::EvalPopBgColor() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("PopBgColor()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PopBgColor()) {
		Unref(wnd);
		return interp_error("PopBgColor()");
	}
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalPushColorFunc() {
	AGmNode *wnd, *func;
	GetArg(wnd, func);
	if (!IsWindow(wnd)) {
		Unref(wnd, func);
		return interp_error("PushColorFunc()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PushColorFunc(List(func))) {
		Unref(wnd, func);
		return interp_error("PushColorFunc()");
	}
	Unref(wnd, func);
	return 0;
}
int AIpProcess::EvalPopColorFunc() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("PopColorFunc()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PopColorFunc()) {
		Unref(wnd);
		return interp_error("PopColorFunc()");
	}
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalGetSysColor() {
	AGmNode *id;
	GetArg(id);
	if (!IsInteger(id)) {
		Unref(id);
		return interp_error("GetSysColor()");
	}
	long color;
	if (id->a.i.v == 0) {
		color = ADV_BG_COLOR;
	} else if (id->a.i.v == 1) {
		color = ADV_FG_COLOR;
	} else {
		Unref(id);
		return interp_error("GetSysColor()");
	}
	Unref(val); val = Ref(Integer(color));
	Unref(id);
	return 0;
}
int AIpProcess::EvalPushPen() {
	AGmNode *wnd, *style, *width, *color;
	GetArg(wnd, style, width, color);
	if (!IsWindow(wnd)) {
		Unref(wnd, style, width, color);
		return interp_error("PushPen()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PushPen(List(style, width, color))) {
		Unref(wnd, style, width, color);
		return interp_error("PushPen()");
	}
	Unref(wnd, style, width, color);
	return 0;
}
int AIpProcess::EvalPopPen() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("PopPen()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PopPen()) {
		Unref(wnd);
		return interp_error("PopPen()");
	}
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalPushBrush() {
	AGmNode *wnd, *style, *color, *pattern;
	GetArg(wnd, style, color, pattern);
	if (!IsWindow(wnd)) {
		Unref(wnd, style, color, pattern);
		return interp_error("PushBrush()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PushBrush(List(style, color, pattern))) {
		Unref(wnd, style, color, pattern);
		return interp_error("PushBrush()");
	}
	Unref(wnd, style, color, pattern);
	return 0;
}
int AIpProcess::EvalPopBrush() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("PopBrush()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PopBrush()) {
		Unref(wnd);
		return interp_error("PopBrush()");
	}
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalPushFont() {
	AGmNode *wnd, *name, *height, *width, *color;
	GetArg(wnd, name, height, width, color);
	if (!IsWindow(wnd)) {
		Unref(wnd, name, height, width, color);
		return interp_error("PushFont()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PushFont(List(name, height, width, color))) {
		Unref(wnd, name, height, width, color);
		return interp_error("PushFont()");
	}
	Unref(wnd, name, height, width, color);
	return 0;
}
int AIpProcess::EvalPushFontEx() {
	AGmNode *wnd, *font;
	GetArg(wnd, font);
	if (!IsWindow(wnd)) {
		Unref(wnd, font);
		return interp_error("PushFontEx()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PushFont(font)) {
		Unref(wnd, font);
		return interp_error("PushFontEx()");
	}
	Unref(wnd, font);
	return 0;
}
int AIpProcess::EvalPopFont() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("PopFont()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PopFont()) {
		Unref(wnd);
		return interp_error("PopFont()");
	}
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalPushTM() {
	AGmNode *wnd, *matrix;
	GetArg(wnd, matrix);
	if (!IsWindow(wnd) || (matrix && !IsMatrix(matrix))) {
		Unref(wnd, matrix);
		return interp_error("PushTM()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PushTM(matrix ? List(matrix): NULL)) {
		Unref(wnd, matrix);
		return interp_error("PushTM()");
	}
	Unref(wnd, matrix);
	return 0;
}
int AIpProcess::EvalPopTM() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("PopTM()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PopTM()) {
		Unref(wnd);
		return interp_error("PopTM()");
	}
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalWindowTM() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("WindowTM()");
	}
	ADvWindow *aw = wnd->a.w.v;
	Unref(val); val = Ref(aw->TM());
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalIsPrinting() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("IsPrinting()");
	}
	Unref(val); val = NULL;
#ifdef	_WINDOWS
	ADvWindow *aw = wnd->a.w.v;
	CDC *dc = aw->PaintDC();
	if (dc && dc->IsPrinting())
		val = Ref(Integer(1));
#endif
#ifdef	_LINUX
	// TODO_LINUX is_printing
#endif
	Unref(wnd);
	return 0;
}

int AIpProcess::EvalStartQuickDraw() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("StartQuickDraw()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->StartQuickDraw()) {
		Unref(wnd);
		return interp_error("StartQuickDraw()");
	}
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalEndQuickDraw() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("EndQuickDraw()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->EndQuickDraw()) {
		Unref(wnd);
		return interp_error("EndQuickDraw()");
	}
	Unref(wnd);
	return 0;
}

int AIpProcess::EvalPoint() {
	AGmNode *wnd, *x, *y, *color; double rx, ry;
	GetArg(wnd, x, y, color);
	if (!IsWindow(wnd) || !IsNum(x, rx) || !IsNum(y, ry) || !IsInteger(color)) {
		Unref(wnd, x, y, color);
		return interp_error("Point()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->Point(rx, ry, (long)color->a.i.v)) {
		Unref(wnd, x, y, color);
		return interp_error("Point()");
	}
	Unref(wnd, x, y, color);
	return 0;
}
int AIpProcess::EvalLine() {
	AGmNode *wnd, *x1, *y1, *x2, *y2; double rx1, ry1, rx2, ry2;
	GetArg(wnd, x1, y1, x2, y2);
	if (!IsWindow(wnd) || !IsNum(x1, rx1) || !IsNum(y1, ry1) || !IsNum(x2, rx2) || !IsNum(y2, ry2)) {
		Unref(wnd, x1, y1, x2, y2);
		return interp_error("Line()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->Line(rx1, ry1, rx2, ry2)) {
		Unref(wnd, x1, y1, x2, y2);
		return interp_error("Line()");
	}
	Unref(wnd, x1, y1, x2, y2);
	return 0;
}
int AIpProcess::EvalRectangle() {
	AGmNode *wnd, *l, *t, *r, *b; double rl, rt, rr, rb;
	GetArg(wnd, l, t, r, b);
	if (!IsWindow(wnd) || !IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb)) {
		Unref(wnd, l, t, r, b);
		return interp_error("Rectangle()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->Rectangle(rl, rt, rr, rb)) {
		Unref(wnd, l, t, r, b);
		return interp_error("Rectangle()");
	}
	Unref(wnd, l, t, r, b);
	return 0;
}
int AIpProcess::EvalEllipse() {
	AGmNode *wnd, *l, *t, *r, *b; double rl, rt, rr, rb;
	GetArg(wnd, l, t, r, b);
	if (!IsWindow(wnd) || !IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb)) {
		Unref(wnd, l, t, r, b);
		return interp_error("Ellipse()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->Ellipse(rl, rt, rr, rb)) {
		Unref(wnd, l, t, r, b);
		return interp_error("Ellipse()");
	}
	Unref(wnd, l, t, r, b);
	return 0;
}
int AIpProcess::EvalPolyPolyline() {
	AGmNode *wnd, *polypoly;
	GetArg(wnd, polypoly);
	if (!IsWindow(wnd)) {
		Unref(wnd, polypoly);
		return interp_error("PolyPolyline()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PolyPolyline(polypoly)) {
		Unref(wnd, polypoly);
		return interp_error("PolyPolyline()");
	}
	Unref(wnd, polypoly);
	return 0;
}
int AIpProcess::EvalPolyPolygon() {
	AGmNode *wnd, *polypoly;
	GetArg(wnd, polypoly);
	if (!IsWindow(wnd) || polypoly == NULL) {
		Unref(wnd, polypoly);
		return interp_error("PolyPolygon()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->PolyPolygon(polypoly)) {
		Unref(wnd, polypoly);
		return interp_error("PolyPolygon()");
	}
	Unref(wnd, polypoly);
	return 0;
}
int AIpProcess::EvalTextOut() {
	AGmNode *wnd, *x, *y, *text; double rx, ry;
	GetArg(wnd, x, y, text);
	if (!IsWindow(wnd) || !IsNum(x, rx) || !IsNum(y, ry) || !IsString(text)) {
		Unref(wnd, x, y, text);
		return interp_error("TextOut()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->TextOut(rx, ry, text->a.s.v->Str())) {
		Unref(wnd, x, y, text);
		return interp_error("TextOut()");
	}
	Unref(wnd, x, y, text);
	return 0;
}
int AIpProcess::EvalTextExtent() {
	AGmNode *wnd, *text;
	GetArg(wnd, text);
	if (!IsWindow(wnd) || !IsString(text)) {
		Unref(wnd, text);
		return interp_error("TextExtent()");
	}
	ADvWindow *aw = wnd->a.w.v;
#if	_WINDOWS && _64BIT
	long long w, h;
#else
	long w, h;
#endif
	if (aw->TextExtent(text->a.s.v->Str(), w, h)) {
		Unref(wnd, text);
		return interp_error("TextExtent()");
	}
	Unref(val); val = Ref(List(w, h));
	Unref(wnd, text);
	return 0;
}
int AIpProcess::EvalTextOutFromTo() {
	AGmNode *wnd, *x, *y, *text, *from, *to; double rx, ry;
	GetArg(wnd, x, y, text, from, to);
	if (!IsWindow(wnd) || !IsNum(x, rx) || !IsNum(y, ry) || !IsString(text) || !IsInteger(from) || !IsInteger(to)) {
		Unref(wnd, x, y, text, from, to);
		return interp_error("TextOutFromTo()");
	}
	if (from->a.i.v > to->a.i.v) {
		Unref(wnd, text, from, to);
		return interp_error("TextOutFromTo() from is greater than to");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->TextOutFromTo(rx, ry, text->a.s.v->Str(), from->a.i.v, to->a.i.v)) {
		Unref(wnd, x, y, text, from, to);
		return interp_error("TextOutFromTo()");
	}
	Unref(wnd, x, y, text, from, to);
	return 0;
}
int AIpProcess::EvalTextExtentFromTo() {
	AGmNode *wnd, *text, *from, *to;
	GetArg(wnd, text, from, to);
	if (!IsWindow(wnd) || !IsString(text) || !IsInteger(from) || !IsInteger(to)) {
		Unref(wnd, text, from, to);
		return interp_error("TextExtentFromTo()");
	}
	if (from->a.i.v > to->a.i.v) {
		Unref(wnd, text, from, to);
		return interp_error("TextExtentFromTo() from is greater than to");
	}
	ADvWindow *aw = wnd->a.w.v;
#if	_WINDOWS && _64BIT
	long long w, h;
#else
	long w, h;
#endif
	if (aw->TextExtentFromTo(text->a.s.v->Str(), from->a.i.v, to->a.i.v, w, h)) {
		Unref(wnd, text, from, to);
		return interp_error("TextExtentFromTo()");
	}
	Unref(val); val = Ref(List(w, h));
	Unref(wnd, text, from, to);
	return 0;
}
int AIpProcess::EvalMTextOut() {
	AGmNode *wnd, *l, *t, *r, *b, *text; double rl, rt, rr, rb;
	GetArg(wnd, l, t, r, b, text);
	if (!IsWindow(wnd) ||
			!IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb) ||
			!IsString(text)) {
		Unref(wnd, l, t, r, b, text);
		return interp_error("MTextOut()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->ExtTextOut(rl, rt, rr, rb, text->a.s.v->Str())) {
		Unref(wnd, r, t, r, b, text);
		return interp_error("MTextOut()");
	}
	Unref(wnd, l, t, r, b, text);
	return 0;
}
int AIpProcess::EvalPie() {
	AGmNode *wnd, *l, *t, *r, *b; double rl, rt, rr, rb;
	AGmNode *sx, *sy, *ex, *ey; double rsx, rsy, rex, rey;
	GetArg(wnd, l, t, r, b);
	GetArg(sx, sy, ex, ey);
	if (!IsWindow(wnd) || !IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb) ||
		!IsNum(sx, rsx) || !IsNum(sy, rsy) || !IsNum(ex, rex) || !IsNum(ey, rey)) {
		Unref(wnd, l, t, r, b);
		Unref(sx, sy, ex, ey);
		return interp_error("Pie()");
	}
	ADvWindow *aw = wnd->a.w.v;
	if (aw->Pie(rl, rt, rr, rb, rsx, rsy, rex, rey)) {
		Unref(wnd, l, t, r, b);
		Unref(sx, sy, ex, ey);
		return interp_error("Pie()");
	}
	Unref(wnd, l, t, r, b);
	Unref(sx, sy, ex, ey);
	return 0;
}
int AIpProcess::EvalBitmap() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("Bitmap()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "create") == 0) {
		ADvBitmap *bitmap = new ADvBitmap();
		val = Ref(Bitmap(bitmap));
	} else if (strcmp(s, "set_file") == 0) {
		if (IsBitmap(arg1) && IsString(arg2)) {
			arg1->a.b.v->SetFileName(arg2->a.s.v->Str());
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	} else if (strcmp(s, "get_file") == 0) {
		if (IsBitmap(arg1)) {
			AGmString *name = arg1->a.b.v->FileName();
			if (name) {
				val = Ref(String(name->Str()));
			}
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	} else if (strcmp(s, "load") == 0) {
		if (IsBitmap(arg1) && IsString(arg2)) {
			if (arg1->a.b.v->Load(arg2->a.s.v->Str())) {
				val = Ref(Integer(1));
			}
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	} else if (strcmp(s, "width") == 0) {
		if (IsBitmap(arg1)) {
			val = Ref(Integer(arg1->a.b.v->Width()));
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	} else if (strcmp(s, "height") == 0) {
		if (IsBitmap(arg1)) {
			val = Ref(Integer(arg1->a.b.v->Height()));
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	} else if (strcmp(s, "save_bmp") == 0) {
		if (IsBitmap(arg1) && IsString(arg2)) {
			AGmNode *err = arg1->a.b.v->SaveToBMP(arg2->a.s.v->Str());
			if (err) val = Ref(err);
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	} else if (strcmp(s, "save_png") == 0) {
		if (IsBitmap(arg1) && IsString(arg2)) {
			AGmNode *err = arg1->a.b.v->SaveToPNG(arg2->a.s.v->Str());
			if (err) val = Ref(err);
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	} else if (strcmp(s, "save_gif") == 0) {
		if (IsBitmap(arg1) && IsString(arg2)) {
			AGmNode *err = arg1->a.b.v->SaveToGIF(arg2->a.s.v->Str());
			if (err) val = Ref(err);
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	} else if (strcmp(s, "save_jpeg") == 0) {
		if (IsBitmap(arg1) && IsString(arg2)) {
			AGmNode *err = arg1->a.b.v->SaveToJPEG(arg2->a.s.v->Str());
			if (err) val = Ref(err);
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Bitmap()");
		}
	}
	Unref(cmd, arg1, arg2);
	return 0;
}
int AIpProcess::EvalImage() {
	AGmNode *wnd, *x, *y, *bitmap, *w, *h; double rx, ry, rw, rh;
	GetArg(wnd, x, y, bitmap, w, h);
	if (!IsWindow(wnd) || !IsNum(x, rx) || !IsNum(y, ry) || !IsBitmap(bitmap) ||
			!IsNum(w, rw) || !IsNum(h, rh)) {
		Unref(wnd, x, y, bitmap, w, h);
		return interp_error("Image()");
	}
	ADvWindow *aw = wnd->a.w.v;
	rw = max(0, rw); rh = max(0, rh);
	if (aw->Image(rx, ry, bitmap->a.b.v, rw, rh)) {
		Unref(wnd, x, y, bitmap, w, h);
		return interp_error("Image()");
	}
	Unref(wnd, x, y, bitmap, w, h);
	return 0;
}

int AIpProcess::EvalDamage() {
	AGmNode *wnd, *polypoly;
	GetArg(wnd, polypoly);
	if (!IsWindow(wnd)) {
		Unref(wnd, polypoly);
		return interp_error("Damage()");
	}
	ADvWindow *aw = wnd->a.w.v;
	aw->Damage(polypoly);
	Unref(wnd, polypoly);
	return 0;
}
int AIpProcess::EvalDamageRectPoly() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("DamageRectPoly()");
	}
	ADvWindow *aw = wnd->a.w.v;
	AGmNode *polypoly = CreatePolyPoly();
#ifdef	_WINDOWS
	RECT r;
	CRgn *rgn = aw->UpdateRgn();
	if (rgn) {
		rgn->GetRgnBox(&r);
	} else {
		r.left = r.top = - 32000; r.right = r.bottom = 32000;
	}
	AddRectToPoly(polypoly, r.left, r.top, r.right, r.bottom);
#endif
#ifdef	_LINUX
	GdkRectangle r;
	CRgn *rgn = aw->UpdateRgn();
	if (rgn) {
		rgn->GetRgnBox(&r);
	} else {
		r.x = r.y = - 32000; r.width = r.height = 64000;
	}
	AddRectToPoly(polypoly, r.x, r.y, r.x + r.width, r.y + r.height);
#endif
	Unref(val); val = Ref(polypoly);
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalIsDamaged() {
	AGmNode *wnd, *m, *l, *t, *r, *b; double rl, rt, rr, rb;
	GetArg(wnd, m, l, t, r, b);
	if (!IsWindow(wnd) || (m && !IsMatrix(m)) ||
			!IsNum(l, rl) || !IsNum(t, rt) || !IsNum(r, rr) || !IsNum(b, rb)) {
		Unref(wnd, m, l, t, r, b);
		return interp_error("IsDamaged()");
	}
	if (m) {
		double l1, t1, r1, b1, l2, t2, r2, b2;
		m->a.m.v->Transform(rl, rt, l1, t1);
		m->a.m.v->Transform(rr, rb, r1, b1);
		m->a.m.v->Transform(rl, rb, l2, b2);
		m->a.m.v->Transform(rr, rt, r2, t2);
		rl = min(min(l1, r1), min(l2, r2));
		rt = min(min(t1, b1), min(t2, b2));
		rr = max(max(l1, r1), max(l2, r2));
		rb = max(max(t1, b1), max(t2, b2));
	}
	ADvWindow *aw = wnd->a.w.v;
#ifdef	_WINDOWS
	RECT rect;
	rect.left	= (int)rl;
	rect.top	= (int)rt;
	rect.right	= (int)(rr + 1.99);
	rect.bottom = (int)(rb + 1.99);
#endif
#ifdef	_LINUX
	GdkRectangle rect;
	rect.x	 = (int)rl;
	rect.y	  = (int)rt;
	rect.width	= (int)(rr - rl + 1.99);
	rect.height = (int)(rb - rt + 1.99);
#endif
	CRgn *rgn = aw->UpdateRgn();
	if (rgn == NULL || (rgn && rgn->RectInRegion(&rect))) {
		Unref(val); val = Ref(Integer(1));
	} else {
		Unref(val); val = NULL;
	}
	Unref(wnd, m, l, t, r, b);
	return 0;
}
int AIpProcess::EvalRepair() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("Repair()");
	}
	wnd->a.w.v->Repair();
	Unref(wnd);
	return 0;
}
int AIpProcess::EvalRedraw() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("Redraw()");
	}
	wnd->a.w.v->Redraw();
	Unref(wnd);
	return 0;
}

int AIpProcess::EvalLocalLoop() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("LocalLoop()");
	}
	ADvWindow *aw = wnd->a.w.v;

	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	ASSERT(pm);
	pm->AddLocalLoopProcess(this);
	aw->LocalLoop();
	pm->RemoveLocalLoopProcess(this);

	Unref(wnd);
	return 0;
}
int AIpProcess::EvalExitLoop() {
	AGmNode *wnd;
	GetArg(wnd);
	if (!IsWindow(wnd)) {
		Unref(wnd);
		return interp_error("ExitLoop()");
	}
	ADvWindow *aw = wnd->a.w.v;
	aw->ExitLoop();
	Unref(wnd);
	return 0;
}

int AIpProcess::EvalFilePrint() {
	AGmNode *cmd, *arg;
	GetArg(cmd, arg);
	if (!IsString(cmd)) {
		Unref(cmd, arg);
		return interp_error("FilePrint()");
	}
	int err = 0;
#ifdef	_WINDOWS
	const char *s = cmd->a.s.v->Str();
	if (strcmp(s, "preview") == 0) {
		if (!IsWindow(arg) || !arg->a.w.v->View()) {
			err = 1;
		} else {
			arg->a.w.v->View()->FilePrintPreview();
		}
	} else if (strcmp(s, "print") == 0) {
		if (!IsWindow(arg) || !arg->a.w.v->View()) {
			err = 1;
		} else {
			arg->a.w.v->View()->FilePrint();
		}
	} else if (strcmp(s, "scale") == 0) {
		double scale = 4.0;
		if (arg == NULL || IsNum(arg, scale)) {
			ADvFilePrintScale = scale;
			Unref(val); val = Ref(Real(scale));
		} else {
			err = 1;
		}
	} else {
		err = 1;
	}
#endif
#ifdef	_LINUX
	// TODO_LINUX print
#endif
	Unref(cmd, arg);
	if (err) {
		return interp_error("FilePrint()");
	}
	return 0;
}

#ifdef	_WINDOWS
// -------- CInputDlg --------
class CInputDlg : public CDialog {
	DECLARE_DYNAMIC(CInputDlg)
public:
	CInputDlg(CWnd* pParent = NULL);
	virtual ~CInputDlg();
	enum { IDD = IDD_INPUTDLG1 };
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	CString title;
	CString text;
};

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)
CInputDlg::CInputDlg(CWnd* pParent) : CDialog(CInputDlg::IDD, pParent) {
	title = ""; text = "";
}
CInputDlg::~CInputDlg() {
}
BOOL CInputDlg::OnInitDialog() {
	SetWindowText(title);
	SetDlgItemText(IDC_EDIT1, text);
	return TRUE;
}
void CInputDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, text);
}
BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
END_MESSAGE_MAP()

// -------- CComboDlg --------
class CComboDlg : public CDialog {
	DECLARE_DYNAMIC(CComboDlg)
public:
	CComboDlg(CWnd* pParent = NULL);
	virtual ~CComboDlg();
	enum { IDD = IDD_COMBODLG1 };
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV supprot
	DECLARE_MESSAGE_MAP()
public:
	CString GetText();
public:
	CString title;
	CString text;
	AGmNode *items;
};

IMPLEMENT_DYNAMIC(CComboDlg, CDialog)
CComboDlg::CComboDlg(CWnd* pParent) : CDialog(CComboDlg::IDD, pParent) {
	title = ""; text = ""; items = NULL;
}
CComboDlg::~CComboDlg() {
}
BOOL CComboDlg::OnInitDialog() {
	SetWindowText(title);
	CComboBox *combo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if (items) {
		AGmLink *l;
		for (l = AGm::next_outlink(items, NULL); l; l = AGm::next_outlink(items, l)) {
			AGmNode *n = l->arc->dst;
			combo->AddString(n->a.s.v->Str());
		}
	}
	return TRUE;
}
void CComboDlg::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO1, text);
}
BEGIN_MESSAGE_MAP(CComboDlg, CDialog)
END_MESSAGE_MAP()

#endif

#ifdef	_LINUX

static GtkEntry* input_entry = NULL;
static GtkWidget* input_combo = NULL;
static gchar *input_text;

static gint button1_flag;
static gint button1_clicked(GtkWidget *w, GtkWidget *dlg) {
	if (input_entry) {
		gchar *c = (gchar*)gtk_editable_get_chars(GTK_EDITABLE(input_entry), 0, -1);
		input_text = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
		g_free(c);
	} else if (input_combo) {
		gchar *c = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(input_combo));
		input_text = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
		g_free(c);
	}
	button1_flag = 1;
	gtk_widget_destroy(dlg);
	return FALSE;
}
static gint button2_clicked(GtkWidget *w, GtkWidget *dlg) {
	input_text = NULL;
	gtk_widget_destroy(dlg);
	return FALSE;
}
static gint close_dlg(GtkWidget *w, GtkWidget *dlg) {
	gtk_main_quit();
	gtk_grab_remove(dlg);
	return FALSE;
}

char* input_dialog(const char* title, const char* init_text) {
	button1_flag = 0;

	gchar *c;

	GtkWidget *dlg = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(GTK_WIDGET(dlg), "destroy",
		 G_CALLBACK(close_dlg), dlg);
	gtk_container_set_border_width(GTK_CONTAINER(dlg), 15);

	c = g_locale_to_utf8(title, -1, NULL, NULL, NULL);
	gtk_window_set_title(GTK_WINDOW(dlg), c);
	g_free(c);

	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget *entry = gtk_entry_new();
	input_entry = GTK_ENTRY(entry);

	c = g_locale_to_utf8(init_text, -1, NULL, NULL, NULL);
	gtk_entry_set_text(input_entry, c);
	g_free(c);

	gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
	gtk_widget_show(entry);

	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	GtkWidget *button1 = gtk_button_new_with_label("OK");
	gtk_box_pack_start(GTK_BOX(hbox), button1, FALSE, FALSE, 0);
	g_signal_connect(GTK_WIDGET(button1), "clicked",
		G_CALLBACK(button1_clicked), dlg);
	gtk_widget_show(button1);

	GtkWidget *button2 = gtk_button_new_with_label("Cancel");
	gtk_box_pack_start(GTK_BOX(hbox), button2, FALSE, FALSE, 0);
	g_signal_connect(GTK_WIDGET(button2), "clicked",
		G_CALLBACK(button2_clicked), dlg);
	gtk_widget_show(button2);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	gtk_container_add(GTK_CONTAINER(dlg), vbox);
	gtk_widget_show(vbox);
	gtk_window_set_position(GTK_WINDOW(dlg), GTK_WIN_POS_CENTER);
	gtk_grab_add(dlg);
	gtk_widget_show(dlg);
	gtk_main();

	if (button1_flag == 0) return NULL;

	input_entry = NULL;
	return input_text;
}

char* combo_dialog(const char* title, AGmNode* items) {
	button1_flag = 0;

	gchar *c;

	GtkWidget *dlg = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(GTK_WIDGET(dlg), "destroy",
		 G_CALLBACK(close_dlg), dlg);
	gtk_container_set_border_width(GTK_CONTAINER(dlg), 15);

	c = g_locale_to_utf8(title, -1, NULL, NULL, NULL);
	gtk_window_set_title(GTK_WINDOW(dlg), c);
	g_free(c);

	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	input_combo = gtk_combo_box_text_new();

	if (items) {
		AGmLink *l;
		for (l = AGm::next_outlink(items, NULL); l; l = AGm::next_outlink(items, l)) {
			AGmNode *n = l->arc->dst;
			if (AGm::IsString(n)) {
				gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(input_combo), n->a.s.v->Str());
			}
		}
	}

	gtk_box_pack_start(GTK_BOX(vbox), input_combo, FALSE, FALSE, 0);
	gtk_widget_show(input_combo);

	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	GtkWidget *button1 = gtk_button_new_with_label("OK");
	gtk_box_pack_start(GTK_BOX(hbox), button1, FALSE, FALSE, 0);
	g_signal_connect(GTK_WIDGET(button1), "clicked",
		G_CALLBACK(button1_clicked), dlg);
	gtk_widget_show(button1);

	GtkWidget *button2= gtk_button_new_with_label("Cancel");
	gtk_box_pack_start(GTK_BOX(hbox), button2, FALSE, FALSE, 0);
	g_signal_connect(GTK_WIDGET(button2), "clicked",
		G_CALLBACK(button2_clicked), dlg);
	gtk_widget_show(button2);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	gtk_container_add(GTK_CONTAINER(dlg), vbox);
	gtk_widget_show(vbox);
	gtk_window_set_position(GTK_WINDOW(dlg), GTK_WIN_POS_CENTER);
	gtk_grab_add(dlg);
	gtk_widget_show(dlg);
	gtk_main();

	if (button1_flag == 0) return NULL;

	input_combo = NULL;
	return input_text;
}

int MessageBox(void*, const char* text, const char* caption, int type) {
	button1_flag = 0;

	gchar *c;

	GtkWidget *dlg = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(GTK_WIDGET(dlg), "destroy",
		 G_CALLBACK(close_dlg), dlg);
	gtk_container_set_border_width(GTK_CONTAINER(dlg), 15);

	c = g_locale_to_utf8(caption, -1, NULL, NULL, NULL);
	gtk_window_set_title(GTK_WINDOW(dlg), c);
	g_free(c);

	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	c = g_locale_to_utf8(text, -1, NULL, NULL, NULL);
	GtkWidget *label = gtk_label_new(c);
	g_free(c);

	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
	gtk_widget_show(label);

	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	GtkWidget *button1;
	if (type == MB_OK || type == MB_OKCANCEL) {
		button1 = gtk_button_new_with_label("OK");
	}
	if (type == MB_YESNO) {
		button1 = gtk_button_new_with_label("Yes");
	}
	gtk_box_pack_start(GTK_BOX(hbox), button1, FALSE, FALSE, 0);
	g_signal_connect(GTK_WIDGET(button1), "clicked",
		G_CALLBACK(button1_clicked), dlg);
	gtk_widget_show(button1);

	GtkWidget *button2;
	if (type == MB_OKCANCEL || type == MB_YESNO) {
		if (type == MB_OKCANCEL) {
			button2 = gtk_button_new_with_label("Cancel");
		}
		if (type == MB_YESNO) {
			button2 = gtk_button_new_with_label("No");
		}
		gtk_box_pack_start(GTK_BOX(hbox), button2, FALSE, FALSE, 0);
		g_signal_connect(GTK_WIDGET(button2), "clicked",
			G_CALLBACK(button2_clicked), dlg);
		gtk_widget_show(button2);
	}

	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	gtk_container_add(GTK_CONTAINER(dlg), vbox);
	gtk_widget_show(vbox);
	gtk_window_set_position(GTK_WINDOW(dlg), GTK_WIN_POS_CENTER);
	gtk_grab_add(dlg);
	gtk_widget_show(dlg);
	gtk_main();

	return button1_flag;
}
#endif

int AIpProcess::EvalGuiMisc() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("GuiMisc()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "point_to_dot") == 0) {
#ifdef	_WINDOWS
		double size;
		if (!IsNum(arg1, size)) {
			Unref(cmd, arg1, arg2);
			return interp_error("GuiMisc() point_to_dot");
		}
		int iPointSize = (int)(size * 10);	// round off to one decimal place
		HDC hDC = ::GetDC(NULL);
		POINT pt;
		pt.y = ::GetDeviceCaps(hDC, LOGPIXELSY) * iPointSize;
		double d = (double)pt.y;
		d /= 720;
		pt.y = (int)(d + 0.5);
		::DPtoLP(hDC, &pt, 1);
		ReleaseDC(NULL, hDC);
		val = Ref(Integer(abs(pt.y)));
#endif
#ifdef	_LINUX
	// TODO_LINUX point_to_dot
#endif

	} else if (strcmp(s, "raise") == 0) {
		if (!IsWindow(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("GuiMisc() raise");
		}
		ADvWindow *aw = arg1->a.w.v;
		if (aw->View()) {
			CFrameWnd *w = aw->View()->GetParentFrame();
			if (w) w->BringWindowToTop();
		} else if (aw->Frame()) {
			aw->Frame()->BringWindowToTop();
		}

	} else if (strcmp(s, "unload") == 0) {
		if (!( arg2 == NULL ||
				(IsList(arg2)
				 && arg2->T && IsList(arg2->T)
				 && arg2->TT && IsList(arg2->TT)) ) ) {
			Unref(cmd, arg1, arg2);
			return interp_error("GuiMisc() unload");
		}
		ADvCWinApp *app = ADvGetApp();
		if (app)
			((ADvCMainFrame*)app->m_pMainWnd)->SetUnloadCallback(arg2);
		
	} else if (strcmp(s, "timer") == 0) {
		if (!(arg1 && IsList(arg1) && arg1->T && IsList(arg1)
				 && IsInteger(arg1->H) && IsInteger(arg1->TH))) {
			Unref(cmd, arg1, arg2);
			return interp_error("GuiMisc() timer");
		}
		if (!(arg2 == NULL ||
				(IsList(arg2) && arg2->T && IsList(arg2->T) && arg2->TT && IsList(arg2->TT)))) {
			Unref(cmd, arg1, arg2);
			return interp_error("GuiMisc() timer");
		}
		ADvCWinApp *app = ADvGetApp();
		if (app)
			((ADvCMainFrame*)app->m_pMainWnd)->SetTimerCallback((long)arg1->HI, (long)arg1->THI, arg2);

	} else if (strcmp(s, "ok_dlg") == 0) {
		if (IsString(arg1) && IsString(arg2)) {
			::MessageBox(NULL, arg2->a.s.v->Str(), arg1->a.s.v->Str(), MB_OK);
		}
	} else if (strcmp(s, "ok_cancel_dlg") == 0) {
		if (IsString(arg1) && IsString(arg2)) {
			int ret = ::MessageBox(NULL, arg2->a.s.v->Str(), arg1->a.s.v->Str(), MB_OKCANCEL);
			if (ret == IDOK) val = Ref(Integer(1));
		}
	} else if (strcmp(s, "yes_no_dlg") == 0) {
		if (IsString(arg1) && IsString(arg2)) {
			int ret = ::MessageBox(NULL, arg2->a.s.v->Str(), arg1->a.s.v->Str(), MB_YESNO);
			if (ret == IDYES) val = Ref(Integer(1));
		}

	} else if (strcmp(s, "input_dlg") == 0) {
		if (IsString(arg1) && IsString(arg2)) {
#ifdef _WINDOWS
			CWinApp *app = AfxGetApp();
			CWnd *pMainWnd = app->m_pMainWnd;
			app->m_pMainWnd = NULL;
			CInputDlg *dlg = new CInputDlg();
			dlg->title = arg1->a.s.v->Str();
			dlg->text = arg2->a.s.v->Str();
			INT_PTR ok = dlg->DoModal();
			app->m_pMainWnd = pMainWnd;
			if (ok == IDOK) {
				val = Ref(String(dlg->text));
			}
			delete dlg;
#endif
#ifdef _LINUX
			gchar *cp = input_dialog(arg1->a.s.v->Str(), arg2->a.s.v->Str());
			if (cp) {
				val = Ref(String(cp)); g_free(cp);
			}
#endif
		}
	} else if (strcmp(s, "combo_dlg") == 0) {
		if (IsString(arg1) && arg2) {
#ifdef _WINDOWS
			CWinApp *app = AfxGetApp();
			CWnd *pMainWnd = app->m_pMainWnd;
			app->m_pMainWnd = NULL;
			CComboDlg *dlg = new CComboDlg();
			dlg->title = arg1->a.s.v->Str();
			dlg->items = arg2;
			INT_PTR ok = dlg->DoModal();
			app->m_pMainWnd = pMainWnd;
			if (ok == IDOK) {
				val = Ref(String(dlg->text));
			}
			delete dlg;
#endif
#ifdef _LINUX
			gchar *cp = combo_dialog(arg1->a.s.v->Str(), arg2);
			if (cp) {
				val = Ref(String(cp)); g_free(cp);
			}
#endif
		}

	} else {
		Unref(cmd, arg1, arg2);
		return interp_error("GuiMisc()");
	}
	Unref(cmd, arg1, arg2);
	return 0;
}

