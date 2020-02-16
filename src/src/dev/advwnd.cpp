/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#ifdef	_WINDOWS
#include <afxpriv.h>
#endif
#ifdef	_LINUX
#include <gdk/gdkcairo.h>
#include <cairo/cairo.h>
#endif

#include <gdm/gdm.h>
#include <gdm/string.h>
#include <gdm/file.h>
#include <gdm/matrix.h>
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
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <dev/bitmap.h>
#include <dev/olectrl.h>
#include <altair/strres.h>

#include <altair/resource.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
//#define	DRAW_ATTRIB_STACK_CHECK
#endif

#define INT_LIMIT(x)	(int)( x < -32700 ? -32700 : x > 32700 ? 32700 : x )
#define R4E5(x) 		( (x) >= 0 ? (x) + 0.5 : (x) - 0.5 )

#ifdef	_LINUX

static const double dashes[] = { 6, 18 };

CDC::CDC(ADvCView *cview) {
	_cview = cview;
	if (cview->surface == NULL) {
		CFrameWnd *parent = (CFrameWnd*)cview->_parent;
		cview->surface = gdk_window_create_similar_surface(
			gtk_widget_get_window(cview->m_pWidget), CAIRO_CONTENT_COLOR,
			parent->_width, parent->_height);
	}
	gc = cairo_create(cview->surface);
	font = pango_font_description_from_string(DEFAULT_FONT_NAME);
	context = gtk_widget_create_pango_context(cview->m_pWidget);

	// pen
	cpen = NULL;
	pen_color = -1;
	pen_width = 0;
	// set brush
	cbrush = NULL;
	fill_color = -1;
	fill = FALSE;
	// set font
	cfont = NULL;
	font_color = -1;

	x0 = y0 = 0;
}
CDC::~CDC() {
	cairo_destroy(gc);
	pango_font_description_free(font);
	g_object_unref(context);
}

void CDC::FillBackground() {
	GtkAllocation allocation;
	gtk_widget_get_allocation(_cview->m_pWidget, &allocation);
	cairo_set_source_rgb(gc, 1.0, 1.0, 1.0);
	cairo_rectangle(gc, 0, 0, allocation.width, allocation.height);
	cairo_fill(gc);
}

void CDC::SetTextColor(long color) {
	font_color = color;
}	
void CDC::SetBkColor(long color) {
	back_color = color;
}

void CDC::SetROP2(int op) {
	switch (op) {
	case R2_COPYPEN:
		cairo_set_operator(gc, CAIRO_OPERATOR_OVER);
		break;
	case R2_XORPEN:
		cairo_set_operator(gc, CAIRO_OPERATOR_XOR);
		break;
	}
}

CGdiObject* CDC::SelectObject(CGdiObject *pObject) {
	PangoFontDescription *desc;
	if (pObject == NULL) return NULL;
	switch (pObject->type) {
	case ADV_GDI_TYPE_PEN:
		cpen = (CPen*)pObject;
		pen_color = cpen->color;
		pen_width = cpen->width;
		break;
	case ADV_GDI_TYPE_BRUSH:
		cbrush = (CBrush*)pObject;
		fill_color = cbrush->color;
		fill = TRUE;
		break;
	case ADV_GDI_TYPE_FONT:
		cfont = (CFont*)pObject;
		desc = pango_font_description_from_string(cfont->font_name);
		if (desc) {
			if (font) {
				pango_font_description_free(font);
			}
			font = desc;
		}
		break;
	default:
		break;
	}
	return NULL;
}
CGdiObject* CDC::SelectStockObject(int obj) {
	switch (obj) {
	case NULL_PEN:
		cpen = NULL;
		pen_color = -1;
		break;
	case NULL_BRUSH:
		cbrush = NULL;
		fill_color = -1;
		fill = FALSE;
		break;
	default:
		break;
	}
	return NULL;
}
int CDC::SelectClipRgn(CRgn *rgn) {
	cairo_reset_clip(gc);
	if (rgn && rgn->rect && rgn->rect->width != 0 && rgn->rect->height != 0) {
		cairo_rectangle(gc, rgn->rect->x, rgn->rect->y, rgn->rect->width, rgn->rect->height);
		cairo_clip(gc);
	}
	return 0;
}

void CDC::MoveTo(int x1, int y1) {
	x0 = x1; y0 = y1;
}
void CDC::LineTo(int x1, int y1) {
	if (pen_color >= 0) {
		cairo_set_source_rgb(gc, AL_RED(pen_color), AL_GREEN(pen_color), AL_BLUE(pen_color));
		if (cpen && cpen->style == PS_DOT) {
			cairo_set_dash(gc, dashes, 1, 0.5);
		} else {
		}
		cairo_new_path(gc);
		cairo_move_to(gc, x0, y0);
		cairo_line_to(gc, x1, y1);
		cairo_close_path(gc);
		cairo_stroke(gc);
		if (cpen && cpen->style == PS_DOT) {
			cairo_set_dash(gc, NULL, 0, 0);
		} else {
		}
	}
	x0 = x1; y0 = y1;
}

void CDC::SetPixel(int x, int y, long color) {
	if (color >= 0) {
		cairo_set_source_rgb(gc, AL_RED(color), AL_GREEN(color), AL_BLUE(color));
		cairo_new_path(gc);
		cairo_move_to(gc, x, y);
		cairo_line_to(gc, x, y);
		cairo_close_path(gc);
		cairo_stroke(gc);
	}
}
void CDC::PolyDraw(GdkPoint *points, BYTE* types, int npoints) {

	if (pen_color >= 0 && npoints > 0) {
		cairo_set_source_rgb(gc, AL_RED(pen_color), AL_GREEN(pen_color), AL_BLUE(pen_color));
		cairo_set_line_width(gc, pen_width ? pen_width : 1);
		if (cpen && cpen->style == PS_DOT) {
			cairo_set_dash(gc, dashes, 1, 0.5);
		} else {
		}
		cairo_new_path(gc);
		int i;
		for (i = 0; i < npoints; i++) {
			switch (types[i]) {
			case PT_MOVETO:
				cairo_move_to(gc, points[i].x, points[i].y);
				break;
			case PT_LINETO:
				cairo_line_to(gc, points[i].x, points[i].y);
				break;
			}
		}
		cairo_close_path(gc);
		cairo_stroke(gc);
		if (cpen && cpen->style == PS_DOT) {
			cairo_set_dash(gc, NULL, 0, 0);
		} else {
		}
	}
}
void CDC::Polygon(GdkPoint *points, int npoints) {
	if (fill && fill_color >= 0) {
		cairo_set_source_rgb(gc, AL_RED(fill_color), AL_GREEN(fill_color), AL_BLUE(fill_color));
		cairo_new_path(gc);
		int i;
		for (i = 0; i < npoints; i++) {
			if (i == 0) {
				cairo_move_to(gc, points[i].x, points[i].y);
			} else {
				cairo_line_to(gc, points[i].x, points[i].y);
			}
		}
		cairo_close_path(gc);
		cairo_fill(gc);
	}
	if (pen_color >= 0) {
		cairo_set_source_rgb(gc, AL_RED(pen_color), AL_GREEN(pen_color), AL_BLUE(pen_color));
		cairo_set_line_width(gc, pen_width ? pen_width : 1);
		if (cpen && cpen->style == PS_DOT) {
			cairo_set_dash(gc, dashes, 1, 0.5);
		} else {
		}
		cairo_new_path(gc);
		int i;
		for (i = 0; i < npoints; i++) {
			if (i == 0) {
				cairo_move_to(gc, points[i].x, points[i].y);
			} else {
				cairo_line_to(gc, points[i].x, points[i].y);
			}
		}
		cairo_close_path(gc);
		cairo_stroke(gc);
		if (cpen && cpen->style == PS_DOT) {
			cairo_set_dash(gc, NULL, 0, 0);
		} else {
		}
	}
}
void CDC::PolyPolygon(GdkPoint *points, int *npolypoints, int npoly) {
	int i;
	for (i = 0; i < npoly; i++) {
		int npoints = *npolypoints++;
		Polygon(points, npoints);
		points += npoints;
	}
}

void CDC::GetTextExtent(const char *str, int len, long& width, long& height) {
	PangoRectangle rect;
	PangoLayout *layout = pango_layout_new(context);
	pango_layout_set_text(layout, str, len);
	pango_layout_set_font_description(layout, font);
	pango_layout_get_pixel_extents(layout, &rect, NULL);
	width = rect.width;
	height = rect. height;
	g_object_unref(layout);
}
void CDC::TextOut(int x, int y, const char *str, int len) {
	if (font_color >= 0) {
		PangoLayout *layout = pango_layout_new(context);
		pango_layout_set_text(layout, str, len);
		pango_layout_set_font_description(layout, font);
		pango_cairo_update_layout(gc, layout);
		cairo_move_to(gc, x, y);
		cairo_set_source_rgb(gc, AL_RED(font_color), AL_GREEN(font_color), AL_BLUE(font_color));
		pango_cairo_show_layout(gc, layout);
		g_object_unref(layout);
	}
}

CGdiObject::CGdiObject(int type) {
	this->type = type;
}
CGdiObject::~CGdiObject() {
}

CPen::CPen(int style, int width, long color)
: CGdiObject(ADV_GDI_TYPE_PEN) {
	if (width == 0) width = 1;
	this->style = style;
	this->width = width;
	this->color = color;
}
CPen::~CPen() {
}

CBrush::CBrush(long color)
: CGdiObject(ADV_GDI_TYPE_BRUSH) {
	this->color = color;
}
CBrush::~CBrush() {
}

CFont::CFont()
: CGdiObject(ADV_GDI_TYPE_FONT) {
}
CFont::~CFont() {
}
int CFont::CreateFont(
		int height, int width,
		int escapement, int orientation, int weight,
		int italic, int underline, int strikeout,
		int charset, int out_precision, int clip_prcision,
		int quality, int pitch_and_family, char *font_name) {

	this->height = height;
	this->width = width;
	this->escapement = escapement;
	this->orientation = orientation;
	this->weight = weight;
	this->italic = italic;
	this->underline = underline;
	this->strikeout = strikeout;
	this->charset = charset;
	this->out_precision = out_precision;
	this->clip_prcision = clip_prcision;
	this->quality = quality;
	this->pitch_and_family = pitch_and_family;
	this->font_name = font_name;

	return 1;
}

CRgn::CRgn() {
	rect = NULL;
}
CRgn::~CRgn() {
	if (rect) g_free(rect);
}

int CRgn::CreateRectRgn(int left, int top, int right, int bottom) {
	rect = (GdkRectangle*)g_malloc(sizeof(GdkRectangle));
	rect->x = left;
	rect->y = top;
	rect->width = right - left + 1;
	rect->height = bottom - top + 1;
	return 1;
}
int CRgn::CreatePolyPolygonRgn(GdkPoint *pts, int* npts, int npoly, int flag) {
	int left = 32000, top = 32000, right = -32000, bottom = -32000;
	int i, j;
	for (i = 0; i < npoly; i++) {
		int n = *npts++;
		for (j = 0; j < n; j++) {
			left   = min(left,	 pts[j].x);
			top    = min(top,	 pts[j].y);
			right  = max(right,  pts[j].x);
			bottom = max(bottom, pts[j].y);
		}
		pts += n;
	}
	rect = (GdkRectangle*)g_malloc(sizeof(GdkRectangle));
	rect->x = left;
	rect->y = top;
	rect->width = right - left + 1;
	rect->height = bottom - top + 1;
	return 1;
}
int CRgn::CombineRgn(CRgn *rgn1, CRgn *rgn2, int op) {
	GdkRectangle d;
	if (gdk_rectangle_intersect(rgn1->rect, rgn2->rect, &d)) {
		rect->x = d.x; rect->y = d.y;
		rect->width = d.width; rect->height = d.height;
	} else {
		rect->x = rect->y = rect->width = rect->height = 0;
	}
	return 0;
}
int CRgn::CopyRgn(CRgn *rgn2) {
	rect->x = rgn2->rect->x; rect->y = rgn2->rect->y;
	rect->width = rgn2->rect->width; rect->height = rgn2->rect->height;
	return 0;
}

void CRgn::GetRgnBox(GdkRectangle *r) {
	if (rect) {
		r->x = rect->x; r->y = rect->y;
		r->width = rect->width; r->height = rect->height;
	} else {
		r->x = r->y = r->width = r->height = 0;
	}
}
int CRgn::RectInRegion(GdkRectangle *r) {
	GdkRectangle d;
	if (gdk_rectangle_intersect(rect, r, &d)) {
		return 1;
	} else {
		return 0;
	}
}

#endif

ADvWindow::ADvWindow() {
	_window_type = ADV_WINDOW_NONE;
	_cframe = NULL;
	_cview = NULL;
	_ole_ctrl = NULL;

	_wnd_draw_proc = _wnd_pick_proc = NULL;
	w_arg =
	w_event_prog = w_draw_prog = w_move_prog = w_destroy_prog =
	w_activate_prog = w_text_ins_prog = _loop_params = NULL;

	_cEdit = NULL;
	_shell = NULL;

	_update_rgn = NULL;
	_dc = NULL;
	_dc_for_quick_draw = 0;

	_tm = NULL;

#ifdef	_LINUX
	_damaged = 0;
	_in_local_loop = 0;
	disable_event_handling = 0;
#endif

	StartDraw();
}
AGmNode* ADvWindow::Create(AGmNode *opt) {
	ADvWndStyle = (long)((opt && IsList(opt) && IsInteger(opt->H)) ? opt->HI : 0);
	int x, y, width, height;
	double rx, ry, rw, rh;
	AGmNode *geom = NULL;
	if (opt && opt->T && opt->TT)
		geom = opt->TTH;
	if (geom && IsNum(geom->H, rx) &&
		geom->T && IsNum(geom->TH, ry) &&
		geom->TT && IsNum(geom->TTH, rw) &&
		geom->TTT && IsNum(geom->TTTH, rh)) {
		ADvAutoLayout = 0;
		x = ADvX; ADvX = (int)rx;
		y = ADvY; ADvY = (int)ry;
		width = ADvWidth; ADvWidth = (int)rw;
		height = ADvHeight; ADvHeight = (int)rh;
	} else {
		ADvAutoLayout = 1;
		x = ADvX; y = ADvY;
		width = ADvWidth; height = ADvHeight;
	}
	int mdi_frame = 0, created = 0;
	ADvCFrameWnd *adv_cframe = NULL;
	if (opt && IsInteger(opt->H)) {
		if (opt->HI & 0x8000) { // MDIFrame
			mdi_frame = 1;
#ifdef	_WINDOWS
			CMultiDocTemplate *tmpl = new CMultiDocTemplate(
				IDR_MDICHILD,
				RUNTIME_CLASS(ADvCDocument),
				RUNTIME_CLASS(ADvCMDIChildWnd),
				RUNTIME_CLASS(ADvCView));
			AGm::SetValue(NULL, AGM_VM_KEY_DOC_TMPL, tmpl);
			_cframe = new ADvCMDIFrameWnd();
			_cframe->DocTmpl(tmpl);
			if (!_cframe->LoadFrame(IDR_MDICHILD))
				return NULL;
			_cframe->ShowWindow(SW_SHOW);
			_cframe->UpdateWindow();
#endif	// _WINDOWS
#ifdef	_LINUX
			_cframe = new ADvCMDIFrameWnd;
			if (ADvAutoLayout) {
				int x = ADvX, y = ADvY, w = ADvWidth, h = ADvHeight;
				int width = 800, height = 600;
				w = width * 4 / 7 ;
				h = height * 3 / 4;
				x += 40; y += 30;
				if (x + w > 1000) x = 10; if (y + h > 700) y = 30;
				ADvX = x; ADvY = y; ADvWidth = w; ADvHeight = h;
			}
			_cframe->Create("", 0, ADvX, ADvY, 240, 80, NULL);
			_cframe->ShowWindow(SW_SHOW);
#endif
			_cframe->SetADvWindow(this);
			created = 1;
		} else if (opt->HI & 0x4000) { // MDIChild
			if (opt->T == NULL || !IsWindow(opt->TH)) {
				return NULL;
			}
			ADvWindow *aw = opt->TH->a.w.v;
			if (aw->Frame() == NULL) return NULL;
#ifdef	_WINDOWS
			CMultiDocTemplate *tmpl = aw->Frame()->DocTmpl();
			CWinApp *app = AfxGetApp();
			CWnd *pMainWnd = app->m_pMainWnd;
			app->m_pMainWnd = aw->Frame();
			CDocument *pDoc = tmpl->OpenDocumentFile(NULL);
			app->m_pMainWnd = pMainWnd;
			if (pDoc == NULL) return NULL;
#endif	// _WINDOWS
#ifdef	_LINUX
			ADvCWinApp *app = ADvGetApp();
			if (app == NULL) return NULL;
			CFrameWnd *pMainWnd = app->m_pMainWnd;
			app->m_pMainWnd = aw->Frame();
			app->CreateWnd();
			app->m_pMainWnd = pMainWnd;
#endif
			created = 1;
		} else if (opt->HI & 0x2000) { // SDI
#ifdef	_WINDOWS
			CSingleDocTemplate *tmpl = new CSingleDocTemplate(
				IDR_SDIFRAME,
				RUNTIME_CLASS(ADvCDocument),
				RUNTIME_CLASS(ADvCFrameWnd),
				RUNTIME_CLASS(ADvCView));
			AGm::SetValue(NULL, AGM_VM_KEY_DOC_TMPL, tmpl);
			CWinApp *app = AfxGetApp();
			CWnd *pMainWnd = app->m_pMainWnd;
			app->m_pMainWnd = NULL;
			CDocument *pDoc = tmpl->OpenDocumentFile(NULL);
			adv_cframe = (ADvCFrameWnd*)app->m_pMainWnd;
			app->m_pMainWnd = pMainWnd;
			if (adv_cframe == NULL) {
				adv_cframe->ShowWindow(SW_SHOW);
				adv_cframe->UpdateWindow();
			}
			if (pDoc == NULL) {
				delete tmpl;
				return NULL;
			}
#endif	// _WINDOWS
#ifdef	_LINUX
			adv_cframe = new ADvCFrameWnd;
			if (ADvAutoLayout) {
				int x = ADvX, y = ADvY, w = ADvWidth, h = ADvHeight;
				int width = 800, height = 600;
				w = width * 4 / 7 ;
				h = height * 3 / 4;
				x += 40; y += 30;
				if (x + w > 1000) x = 10; if (y + h > 700) y = 30;
				ADvX = x; ADvY = y; ADvWidth = w; ADvHeight = h;
			}
			adv_cframe->Create("", 0, ADvX, ADvY, ADvWidth, ADvHeight, NULL);
			adv_cframe->ShowWindow(SW_SHOW);
#endif
			adv_cframe->SetADvWindow(this);
			created = 1;
		}
	}
	if (created == 0) {
		ADvCWinApp *app = ADvGetApp();
		if (app) {
			app->CreateWnd();
		} else {
			return NULL;
		}
	}
	if (ADvAutoLayout == 0) {
		ADvX = x; ADvY = y;
		ADvWidth = width; ADvHeight = height;
	}
	AGmNode *node = Window(this);
	if (mdi_frame) {
		SetValue(_cframe, AGM_VM_KEY_WINDOW_NODE, node);
		CreateArc(RootWindowNode(), node);
		_window_type = ADV_WINDOW_MDI;
	} else {
		void *value;
		GetValue(NULL, AGM_VM_KEY_CVIEW, value);
		RemoveValue(NULL, AGM_VM_KEY_CVIEW);
		_cview = (ADvCView*)value;
		SetValue(_cview, AGM_VM_KEY_WINDOW_NODE, node);
		CreateArc(RootWindowNode(), node);
		_window_type = ADV_WINDOW_NORMAL;
		if (adv_cframe) adv_cframe->_cview0 = _cview;
	}

	return node;
}

AGmNode *ADvWindow::CreateEdit(ADvEditShell *shell, AGmNode **node, AGmNode *opt, int drop_file) {
#ifdef	_WINDOWS
	AGmNode *n = Create(opt);
	if (n == NULL) return NULL;
	if (node) *node = n;
	_cEdit = new ADvCEdit();
	DWORD style = ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
		WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_HSCROLL | WS_VSCROLL;
	RECT rect; _cview->GetClientRect(&rect);
	_cEdit_nID = GetUniqueID();
	if (_cEdit->Create(style, rect, _cview, (UINT)_cEdit_nID)) {
		if (shell) delete shell;
		delete _cEdit; _cEdit = NULL;
		ReleaseUniqueID(_cEdit_nID);
		return NULL;
	}
	if (drop_file) _cEdit->DragAcceptFiles();

	// font
	CFont* font = ADvGetCFont(
		DEFAULT_FONT_NAME, DEFAULT_FONT_HEIGHT, DEFAULT_FONT_WIDTH,
		GetSysColor(COLOR_WINDOWTEXT), 1.0, 1.0, 0, 0, 0, 0, 0);
	if (font == NULL) {
		font = new CFont();
		font->CreateFont(
			DEFAULT_FONT_HEIGHT, DEFAULT_FONT_WIDTH,
			0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
#ifdef	AL_LANG_JAPANESE
			SHIFTJIS_CHARSET,
#else
			DEFAULT_CHARSET,
#endif
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			DEFAULT_FONT_NAME);
		ADvPutCFont(font,
			DEFAULT_FONT_NAME, DEFAULT_FONT_HEIGHT, DEFAULT_FONT_WIDTH,
			GetSysColor(COLOR_WINDOWTEXT), 1.0, 1.0, 0, 0, 0, 0, 0);
	}
	_cEdit->SetFont(font);

#ifdef	AL_LANG_JAPANESE
	LRESULT flag = _cEdit->SendMessage(EM_GETLANGOPTIONS, 0, 0);
	flag &= ~ IMF_DUALFONT;
	_cEdit->SendMessage(EM_SETLANGOPTIONS, 0, flag);
#endif

	unsigned int width = 16;
	_cEdit->SendMessage(EM_SETTABSTOPS, 1, (LPARAM)&width);

	if (opt && IsList(opt) && IsInteger(opt->H) && (opt->HI & 0x0040) != 0) {
		// line warp (user defined window)
		_cEdit->SendMessage(EM_SETTARGETDEVICE, 0, 0);
	} else {
		// (altair class/member/body window)
		_cEdit->DisableNewline(1);
	}

#endif	// _WINDOWS
#ifdef	_LINUX
	int x, y, width, height;
	double rx, ry, rw, rh;
	AGmNode *geom = NULL;
	if (opt && opt->T && opt->TT)
		geom = opt->TTH;
	if (geom && IsNum(geom->H, rx) &&
		geom->T && IsNum(geom->TH, ry) &&
		geom->TT && IsNum(geom->TTH, rw) &&
		geom->TTT && IsNum(geom->TTTH, rh)) {
		ADvAutoLayout = 0;
		x = ADvX; ADvX = (int)rx;
		y = ADvY; ADvY = (int)ry;
		width = ADvWidth; ADvWidth = (int)rw;
		height = ADvHeight; ADvHeight = (int)rh;
	} else {
		ADvAutoLayout = 1;
		x = ADvX; y = ADvY;
		width = ADvWidth; height = ADvHeight;
	}

	ADvCWinApp *app = ADvGetApp();
	if (app == NULL || app->m_pMainWnd == NULL) return NULL;

	if (ADvAutoLayout) {
		int x = ADvX, y = ADvY, w = ADvWidth, h = ADvHeight;
		int width = 800, height = 600;
		w = width * 4 / 7 ;
		h = height * 3 / 4;
		x += 40; y += 30;
		if (x + w > 1000) x = 10; if (y + h > 700) y = 30;
		ADvX = x; ADvY = y; ADvWidth = w; ADvHeight = h;
	}

	ADvCFrameWnd* adv_cframe = NULL;
	long style = 0;
	if (drop_file) style = DRAG_ACCEPT_FILES;
	if (opt && IsInteger(opt->H) && (opt->HI & 0x2000) != 0) {
		adv_cframe = new ADvCFrameWnd;
		adv_cframe->CreateEdit("", style, ADvX, ADvY, ADvWidth, ADvHeight, NULL);
		_cEdit = adv_cframe->_cview->_cedit;
		adv_cframe->ShowWindow(SW_SHOW);
		adv_cframe->SetADvWindow(this);
	} else {
		ADvCMDIChildWnd *wnd = new ADvCMDIChildWnd;
		char title[256];
		sprintf(title, "Altair%d", adv_wnd_no++);
		int x1 = ADvX, y1 = ADvY, w = ADvWidth, h = ADvHeight;
		if (opt && IsList(opt) && IsInteger(opt->H) && (opt->HI & 0x0040) != 0) {
		} else {
			style |= ES_AUTOHSCROLL;
		}
		wnd->CreateEdit(title, style, x1, y1, w, h, app->m_pMainWnd);
		_cEdit = wnd->_cview->_cedit;
		wnd->ShowWindow(SW_SHOW);
	}

	if (ADvAutoLayout == 0) {
		ADvX = x; ADvY = y;
		ADvWidth = width; ADvHeight = height;
	}

	AGmNode *n = Window(this);
	if (node) *node = n;
	
	void *value;
	GetValue(NULL, AGM_VM_KEY_CVIEW, value);
	RemoveValue(NULL, AGM_VM_KEY_CVIEW);
	_cview = (ADvCView*)value;
	SetValue(_cview, AGM_VM_KEY_WINDOW_NODE, n);
	CreateArc(RootWindowNode(), n);
	if (adv_cframe) adv_cframe->_cview0 = _cview;
#endif
	_wnd_pick_proc = ADvWindow::DefaultPickProc;
	_shell = shell;
	if (_shell) {
		_shell->EditCtrl(_cEdit);
		_shell->Update();
	}

	_window_type = ADV_WINDOW_EDIT;
	_cEdit->SetWindow(this);

	return n;
}
int ADvWindow::CreateControl(ADvCOleControl* ctrl) {
	_ole_ctrl = ctrl;
	AGmNode *node = Window(this);
	SetValue(_ole_ctrl, AGM_VM_KEY_WINDOW_NODE, node);
	CreateArc(RootWindowNode(), node);
	_window_type = ADV_WINDOW_OCX;
	return 0;
}
ADvWindow::~ADvWindow() {
#ifdef	_WINDOWS
	if (_update_rgn) delete _update_rgn;
	if (_cEdit) {
		ReleaseUniqueID(_cEdit_nID);
		delete	_cEdit;
		if (_shell) delete _shell;
	}
#endif
#ifdef	_LINUX
	if (_cEdit) {
		if (_shell) delete _shell;
	}
#endif
	if (_cview) {
		void *value;
		GetValue(_cview, AGM_VM_KEY_WINDOW_NODE, value);
		AGmNode* node = (AGmNode*)value;
		if (node) {
			node->a.w.v = NULL;
			RemoveValue(_cview, AGM_VM_KEY_WINDOW_NODE);
			CFrameWnd* wnd = _cview->GetParentFrame();
			if (wnd) {
				wnd->DestroyWindow();
			}
		}
		_cview = NULL;
	}
	if (_cframe) {
		void *value;
		GetValue(_cframe, AGM_VM_KEY_WINDOW_NODE, value);
		AGmNode* node = (AGmNode*)value;
		if (node) {
			node->a.w.v = NULL;
			RemoveValue(_cframe, AGM_VM_KEY_WINDOW_NODE);
			_cframe->DestroyWindow();
		}
		_cframe = NULL;
	}

	Unref(_tm);
	Unref(w_arg, w_draw_prog, w_move_prog, w_event_prog, w_destroy_prog, w_activate_prog);
}

void ADvWindow::UpdateRgn(CRgn* rgn) { _update_rgn = rgn; }
CRgn* ADvWindow::UpdateRgn() { return _update_rgn; }

AGmNode* ADvWindow::Clipping() { if (_clip_chg) _ChangeClipping(); return _clip; }
long ADvWindow::BgColor() { return _bg_color; }
char ADvWindow::ColorFunc() { return _color_func; }
char* ADvWindow::PenStyle() { return _pen_style; }
char ADvWindow::PenWidth() { return _pen_width; }
long ADvWindow::PenColor() { return _pen_color; }
char* ADvWindow::BrushStyle() { return _brush_style; }
long ADvWindow::BrushColor() { return _brush_color; }
char* ADvWindow::BrushPattern() { return _brush_pattern; }
char* ADvWindow::FontName() { return _font_name; }
char ADvWindow::FontHeight() { return _font_height; }
char ADvWindow::FontWidth() { return _font_width; }
long ADvWindow::FontColor() { return _font_color; }
char ADvWindow::FontBold() { return _font_bold; }
char ADvWindow::FontItalic() { return _font_italic; }
char ADvWindow::FontUnderline() { return _font_underline; }
AGmNode* ADvWindow::TM() { return _tm; }

short ADvWindow::Type() const { return _window_type; }
void ADvWindow::Type(short type) { _window_type = type; }

ADvCMDIFrameWnd* ADvWindow::Frame() { return _cframe; }
void ADvWindow::Frame(ADvCMDIFrameWnd *cframe) { _cframe = cframe; }
ADvCView* ADvWindow::View() { return _cview; }
void ADvWindow::View(ADvCView* cview) { _cview = cview; }
ADvCOleControl* ADvWindow::OleCtrl() { return _ole_ctrl; }
void ADvWindow::OleCtrl(ADvCOleControl* ctrl) { _ole_ctrl = ctrl; }
int ADvWindow::QuickDraw() { return _dc_for_quick_draw; }

CDC* ADvWindow::PaintDC() { return _dc; }
void ADvWindow::PaintDC(CDC *dc) { _dc = dc; }

ADvWndProc ADvWindow::WndDrawProc() { return _wnd_draw_proc; }
void ADvWindow::WndDrawProc(ADvWndProc proc) { _wnd_draw_proc = proc; } 
ADvWndProc ADvWindow::WndPickProc() { return _wnd_pick_proc; }
void ADvWindow::WndPickProc(ADvWndProc proc) { _wnd_pick_proc = proc; } 

AGmNode* ADvWindow::Arg() { return w_arg; }
void ADvWindow::Arg(AGmNode *arg) { Set(w_arg, arg); }
AGmNode* ADvWindow::EventProg() { return w_event_prog; }
void ADvWindow::EventProg(AGmNode* event_prog) { Set(w_event_prog, event_prog); }
AGmNode* ADvWindow::DrawProg() { return w_draw_prog; }
void ADvWindow::DrawProg(AGmNode *draw_prog) { Set(w_draw_prog, draw_prog); }
AGmNode* ADvWindow::MoveCtrlProg() { return w_move_prog; }
void ADvWindow::MoveCtrlProg(AGmNode *move_prog) { Set(w_move_prog, move_prog); }
AGmNode* ADvWindow::DestroyProg() { return w_destroy_prog; }
void ADvWindow::DestroyProg(AGmNode *destroy_prog) { Set(w_destroy_prog, destroy_prog); }
AGmNode* ADvWindow::ActivateProg() { return w_activate_prog; }
void ADvWindow::ActivateProg(AGmNode *activate_prog) { Set(w_activate_prog, activate_prog); }
AGmNode* ADvWindow::TextInsProg() { return w_text_ins_prog; }
void ADvWindow::TextInsProg(AGmNode *text_ins_prog) { Set(w_text_ins_prog, text_ins_prog); }

ADvCEdit* ADvWindow::EditCtrl() { return _cEdit; }
ADvEditShell* ADvWindow::Shell() { return _shell; }

AGmNode* ADvWindow::LoopParams() { return _loop_params; }
void ADvWindow::LoopParams(AGmNode *params) { _loop_params = params; }

int ADvWindow::FitSize() {
#ifdef	_WINDOWS
	if (_cEdit) {
		if (_cview == NULL) return 1;
		RECT r;
		_cview->GetClientRect(&r);
		_cEdit->MoveWindow(&r, TRUE);
	}
#endif
	return 0;
}
int ADvWindow::SetTitle(const char *text) {
#ifdef	_WINDOWS
	if (_cview) {
		CDocument *doc = _cview->GetDocument();
		doc->SetTitle(text);
		return 0;
	} else if (_cframe) {
		_cframe->SetWindowText(text);
		return 0;
	} else {
		return 1;
	}
#endif
#ifdef	_LINUX
	CFrameWnd *frw;
	if (_cview) {
		frw = _cview->GetParentFrame();
	} else if (_cframe) {
		frw = _cframe;
	}
	if (frw == NULL) return 0;
	gchar *c = g_locale_to_utf8(text, -1, NULL, NULL, NULL);	
	gtk_window_set_title(GTK_WINDOW(frw->m_pToplevelWidget), c);
	g_free(c);
	return 1;
#endif
}
AGmString *ADvWindow::GetTitle() {
#ifdef	_WINDOWS
	CFrameWnd *frm = NULL;
	if (_cview) {
		frm = _cview->GetParentFrame();
	} else if (_cframe) {
		frm = _cframe;
	}
	if (frm) {
		char s[256];
		frm->GetWindowText(s, 256);
		return new AGmString(s);
	} else {
		return new AGmString("");
	}
#endif
#ifdef	_LINUX
	CFrameWnd *frm = NULL;
	if (_cview) {
		frm = _cview->GetParentFrame();
	} else if (_cframe) {
		frm = _cframe;
	}
	if (frm) {
		const gchar *c = gtk_window_get_title(GTK_WINDOW(frm->m_pToplevelWidget));
		gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
		AGmString *ret = new AGmString(c2);
		g_free(c2);
		return ret;
	} else {
		return new AGmString("");
	}
#endif
}
int ADvWindow::Raise() {
	if (_cview) {
		CFrameWnd *cw = _cview->GetParentFrame();
		if (cw == NULL) return 1;
		if (cw->IsIconic()) cw->ShowWindow(SW_RESTORE);
		cw->BringWindowToTop();
	} else {
		return 1;
	}
	return 0;
}
int ADvWindow::SetFocus() {
#ifdef	_WINDOWS
	if (_cEdit) {
		_cEdit->SetFocus();
		return 0;
	}
	if (_cview) {
		_cview->SetFocus();
		return 0;
	}
	return 1;
#endif
#ifdef	_LINUX
	// TODO_LINUX SetFocus
	return 0;
#endif
}

#ifdef	_WINDOWS
int ADvWindow::Damage(AGmNode *polypoly) {
	Ref(polypoly);
	CRgn *rgn = NULL;
	LPPOINT pts; LPINT npts; int npoly;
	if (polypoly) {
		if (_PolygonData(polypoly, pts, npts, npoly)) {
			Unref(polypoly); return 1;
		}
		// extra 1 pixcel al left and top
		// extra 2 pixcel al right and buttom
		int l1 = -1, l2 = -1, t1 = -1, t2 = -1;
		int r1 = -1, r2 = -1, b1 = -1, b2 = -1;
		for (int i = 0; i < *npts; i++) {
			if (l1 == -1) {
				l1 = i;
			} else if (l2 == -1) {
				l2 = i;
			} else if (pts[l1].x > pts[l2].x) {
				if (pts[i].x < pts[l1].x) l1 = i;
			} else {
				if (pts[i].x < pts[l2].x) l2 = i;
			}
			if (t1 == -1) {
				t1 = i;
			} else if (t2 == -1) {
				t2 = i;
			} else if (pts[t1].y > pts[t2].y) {
				if (pts[i].y < pts[t1].y) t1 = i;
			} else {
				if (pts[i].y < pts[t2].y) t2 = i;
			}
			if (r1 == -1) {
				r1 = i;
			} else if (r2 == -1) {
				r2 = i;
			} else if (pts[r1].x < pts[r2].x) {
				if (pts[i].x > pts[r1].x) r1 = i;
			} else {
				if (pts[i].x > pts[r2].x) r2 = i;
			}
			if (b1 == -1) {
				b1 = i;
			} else if (b2 == -1) {
				b2 = i;
			} else if (pts[b1].y < pts[b2].y) {
				if (pts[i].y > pts[b1].y) b1 = i;
			} else {
				if (pts[i].y > pts[b2].y) b2 = i;
			}
		}
		pts[l1].x-=1; pts[l2].x-=1, pts[t1].y-=1; pts[t2].y-=1;
		pts[r1].x+=2; pts[r2].x+=2, pts[b1].y+=2; pts[b2].y+=2;

		rgn = new CRgn;
		int flag = rgn->CreatePolyPolygonRgn(pts, npts, npoly, ALTERNATE);
		delete [] pts; delete [] npts;
		if (flag == 0) {
			delete rgn; Unref(polypoly); return 1;
		}
	}
	Unref(polypoly);
	if (_cview)
		_cview->InvalidateRgn(rgn);
	if (_ole_ctrl)
		_ole_ctrl->InvalidateRgn(rgn);
	delete rgn;
	return 0;
}
#endif
#ifdef	_LINUX
int ADvWindow::Damage(AGmNode *) {
	if (_cview) {
		_cview->InvalidateRect(NULL);
	}
	return 0;
}
#endif

#ifdef	_WINDOWS
int ADvWindow::_PolygonData(AGmNode* polypoly,
		LPPOINT& pts, LPINT& npts, int &npoly) {
#endif
#ifdef	_LINUX
int ADvWindow::_PolygonData(AGmNode* polypoly,
		GdkPoint* &pts, int* &npts, int &npoly) {
#endif
	int n = 0;
	npoly = 0;
	AGmLink *ll;
	for (ll = next_outlink(polypoly, NULL);
			ll; ll = next_outlink(polypoly, ll)) {
		AGmNode *poly = ll->arc->dst;
		AGmLink *l;
		for (l = next_outlink(poly, NULL);
				l; l = next_outlink(poly, l)) {
			n++;
		}
		n++;
		npoly++;
	}
#ifdef	_WINDOWS
	pts = new POINT[n];
#endif
#ifdef	_LINUX
	pts = new GdkPoint[n];
#endif
	npts = new int[npoly];
	n = 0;
	npoly = 0;
	for (ll = next_outlink(polypoly, NULL);
			ll; ll = next_outlink(polypoly, ll)) {
		AGmNode *poly = ll->arc->dst;
		int start_n = n;
		npts[npoly] = 0;
		AGmLink *l;
		for (l = next_outlink(poly, NULL);
				l; l = next_outlink(poly, l)) {
#if	_WINDOWS && _64BIT
			long long x, y;
#else
			long x, y;
#endif
			AGmNode *tail;
			if (Param(l->arc->dst, x, y, tail)) {	
				delete [] pts; delete [] npts; return 1;
			}
			x = INT_LIMIT(x);
			y = INT_LIMIT(y);
			pts[n].x = (int)R4E5(x);
			pts[n].y = (int)R4E5(y);
			npts[npoly]++;
			n++;
		}
		pts[n].x = pts[start_n].x;
		pts[n].y = pts[start_n].y;
		npts[npoly]++;
		n++;
		npoly++;
	}
	return 0; 
}

void ADvWindow::Repair() {
#ifdef	_WINDOWS
	if (_cview)
		_cview->UpdateWindow();
	if (_ole_ctrl)
		_ole_ctrl->UpdateWindow();
#endif
#ifdef	_LINUX
	if (_cview) {
		_cview->InvalidateRect(NULL);
	}
#endif
}
void ADvWindow::Redraw() {
#ifdef	_WINDOWS
	if (_cview)
		_cview->InvalidateRect(NULL);
	if (_ole_ctrl)
		_ole_ctrl->InvalidateRect(NULL);
#endif
#ifdef	_LINUX
	if (_cview) {
		_cview->InvalidateRect(NULL);
	}

#endif
}

void ADvWindow::StartDraw() {
	_clip_stack = _bg_color_stack = _color_func_stack
		= _pen_stack = _brush_stack = _font_stack = _tm_stack = NULL;

	_bg_color = ADV_BG_COLOR;
	_bg_color_chg = 1;

	_color_func = ADvColorCopy;
	_color_func_chg = 0;

	_clip_chg = 1;
	_clip_all = 0;
	_clip = NULL;

	_pen_style = AL_STR("Solid");
	_pen_width = 0;
	_pen_color = ADV_FG_COLOR;
	_pen_chg = 1;
	_pen_selected = 0;
	_pen = NULL;

	_brush_style = AL_STR("Solid");
	_brush_pattern = NULL;
	_brush_color = ADV_BG_COLOR;
	_brush_chg = 1;
	_brush_selected = 0;
	_brush = NULL;
	
	_font_name = DEFAULT_FONT_NAME;
	_font_height = DEFAULT_FONT_HEIGHT;
	_font_width = DEFAULT_FONT_WIDTH;
	_font_color = ADV_FG_COLOR;
	_font_bold = 0;
	_font_italic = 0;
	_font_underline = 0;
	_font_chg = 1;
	_font_selected = 0;
	_h_scale = 1.0;
	_w_scale = 1.0;
	_escape = 0;
	_orient = 0;
	_font = NULL;

	Unref(_tm); _tm = NULL;
}

void ADvWindow::EndDraw() {
#ifdef	DRAW_ATTRIB_STACK_CHECK
	if (_clip_stack)	   TRACE0("ADvWindow::EndDraw() _clip_stack is not empty.\n");
	if (_bg_color_stack)   TRACE0("ADvWindow::EndDraw() _bg_color_stack is not empty.\n");
	if (_color_func_stack) TRACE0("ADvWindow::EndDraw() _color_func_stack is not empty.\n");
	if (_pen_stack) 	   TRACE0("ADvWindow::EndDraw() _pen_stack is not empty.\n");
	if (_brush_stack)	   TRACE0("ADvWindow::EndDraw() _brush_stack is not empty.\n");
	if (_font_stack)	   TRACE0("ADvWindow::EndDraw() _font_stack is not empty.\n");
	if (_tm_stack)		   TRACE0("ADvWindow::EndDraw() _tm_stack is not empty.\n");
#endif
	Unref(_tm); _tm = NULL;
	
	Unref(_clip_stack, _bg_color_stack, _color_func_stack,
		_pen_stack, _brush_stack, _font_stack, _tm_stack);

	_clip_stack = _bg_color_stack = _color_func_stack
		= _pen_stack = _brush_stack = _font_stack = _tm_stack = NULL;

	if (_pen_selected)	 _dc->SelectObject(_original_pen);
	if (_brush_selected) _dc->SelectObject(_original_brush);
	if (_font_selected)  _dc->SelectObject(_original_font);

	if (_pen && _pen_chg == 0 &&
			ADvGetCPen(_pen_style, _pen_width, _pen_color) == NULL)
		delete _pen;
	if (_brush && _brush_chg == 0 &&
			ADvGetCBrush(_brush_style, _brush_color, _brush_pattern) == NULL)
		delete _brush;
	if (_font && _font_chg == 0 &&
			ADvGetCFont(_font_name, _font_height, _font_width, _font_color,
			_h_scale, _w_scale, _escape, _orient,
			_font_bold, _font_italic, _font_underline) == NULL)
		delete _font;
}

// drawing attribute
int ADvWindow::PushClipping(AGmNode *info) {
	AGmNode *clip;	AGmNode *tail;
	if (info == NULL) {
		clip = NULL;
	} else {
		if (Param(info, clip, tail)) {
			return 1;
		}
	}
	_clip = clip; _clip_chg = 1;
	Push(_clip_stack, info);
	return 0;
}
int ADvWindow::PopClipping() {
	AGmNode *clip;	AGmNode *tail;
	if (_clip_stack) Pop(_clip_stack);
	if (_clip_stack == NULL || _clip_stack->H == NULL) { // default
		clip = NULL;
	} else {
		if (Param(_clip_stack->H, clip, tail)) return 1;
	}
	_clip = clip; _clip_chg = 1;
	return 0;
}

int ADvWindow::PushBgColor(AGmNode *info) {
#if	_WINDOWS && _64BIT
	long long bg_color;
#else
	long bg_color;
#endif
	AGmNode *tail;
	if (info == NULL) {
		bg_color = ADV_BG_COLOR;
	} else {
		if (Param(info, bg_color, tail)) {
			return 1;
		}
	}
	if (bg_color != _bg_color) {
		_bg_color = (long)bg_color;
		_bg_color_chg = 1;
	}
	Push(_bg_color_stack, info);
	return 0;
} 
int ADvWindow::PopBgColor() {
#if	_WINDOWS && _64BIT
	long long bg_color;
#else
	long bg_color;
#endif
	AGmNode *tail;
	if (_bg_color_stack) Pop(_bg_color_stack);
	if (_bg_color_stack == NULL || _bg_color_stack->H == NULL) { // default
		bg_color = ADV_BG_COLOR;
	} else {
		if (Param(_bg_color_stack->H, bg_color, tail)) return 1;
	}
	if (bg_color != _bg_color) {
		_bg_color = (long)bg_color;
		_bg_color_chg = 1;
	}
	return 0;
} 

int ADvWindow::PushColorFunc(AGmNode *info) {
#if	_WINDOWS && _64BIT
	long long color_func;
#else
	long color_func;
#endif
	AGmNode *tail;
	if (info == NULL) {
		color_func = ADvColorCopy;
	} else {
		if (Param(info, color_func, tail)) {
			return 1;
		}
	}
	if (color_func != _color_func) {
		_color_func = (char)color_func;
		_color_func_chg = 1;
	}
	Push(_color_func_stack, info);
	return 0;
} 
int ADvWindow::PopColorFunc() {
#if	_WINDOWS && _64BIT
	long long color_func;
#else
	long color_func;
#endif
	AGmNode *tail;
	if (_color_func_stack) Pop(_color_func_stack);
	if (_color_func_stack == NULL || _color_func_stack->H == NULL) { // default
		color_func = ADvColorCopy;
	} else {
		if (Param(_color_func_stack->H, color_func, tail)) return 1;
	}
	if (color_func != _color_func) {
		_color_func = (char)color_func;
		_color_func_chg = 1;
	}
	return 0;
} 
int ADvWindow::PushPen(AGmNode *info) {
	char *style;
#if	_WINDOWS && _64BIT
	long long width, color;
#else
	long width, color;
#endif
	AGmNode *tail;
	if (info == NULL) {
		style = AL_STR("Solid");
		width = 0;
		color = ADV_FG_COLOR;
	} else {
		if (Param(info, style, tail) || Param(tail, width, color, tail)) {
			return 1;
		}
	}
	if (strcmp(style, _pen_style) != 0 ||	
			width != _pen_width || color != _pen_color) {

		if (_pen && _pen_chg == 0) {
			ADvPutCPen(_pen, _pen_style, _pen_width, _pen_color);
			_pen = NULL;
		}

		_pen_style = style;
		_pen_width = (char)width;
		_pen_color = (long)color;
		_pen_chg = 1;		
	}
	Push(_pen_stack, info);
	return 0;
}	
int ADvWindow::PopPen() {
	char *style;
#if	_WINDOWS && _64BIT
	long long width, color;
#else
	long width, color;
#endif
	AGmNode *tail;
	if (_pen_stack) Pop(_pen_stack);
	if (_pen_stack == NULL || _pen_stack->H == NULL) { // default
		style = AL_STR("Solid");
		width = 0;
		color = ADV_FG_COLOR;
	} else {
		if (Param(_pen_stack->H, style, tail) || Param(tail, width, color, tail)) return 1;
	}
	if (strcmp(style, _pen_style) != 0 ||	
			width != _pen_width || color != _pen_color) {

		if (_pen && _pen_chg == 0) {
			ADvPutCPen(_pen, _pen_style, _pen_width, _pen_color);
			_pen = NULL;
		}

		_pen_style = style;
		_pen_width = (char)width;
		_pen_color = (long)color;
		_pen_chg = 1;		
	}
	return 0;
}	

int ADvWindow::PushBrush(AGmNode *info) {
	char *style;
#if	_WINDOWS && _64BIT
	long long color;
#else
	long color;
#endif
	char *pattern; AGmNode *tail;
	if (info == NULL) {
		style = AL_STR("Solid");
		color = ADV_BG_COLOR;
		pattern = NULL;
	} else {
		if (Param(info, style, tail) || Param(tail, color, tail)) {
			return 1;
		}
		AGmBinary *bin;
		if (Param(tail, bin, tail) == 0
				&& bin->Length() == 8) {
			pattern = (char*)bin->Buffer();
		} else {
			pattern = NULL;
		}
	}
	if (strcmp(style, _brush_style) != 0 ||
			color != _brush_color ||
			AGmString::str_cmp(pattern , _brush_pattern, 8) ) {

		if (_brush && _brush_chg == 0) {
			ADvPutCBrush(_brush, _brush_style, _brush_color, _brush_pattern);
			_brush = NULL;
		}

		_brush_style = style;
		_brush_color = (long)color;
		_brush_pattern = pattern;
		_brush_chg = 1;
	}
	Push(_brush_stack, info);
	return 0;
}
int ADvWindow::PopBrush() {
	char *style;
#if	_WINDOWS && _64BIT
	long long color;
#else
	long color;
#endif
	char *pattern; AGmNode *tail;
	if (_brush_stack) Pop(_brush_stack);
	if (_brush_stack == NULL || _brush_stack == NULL) { // default
		style = AL_STR("Solid");
		color = ADV_BG_COLOR;
		pattern = NULL;
	} else {
		if (Param(_brush_stack->H, style, tail) || Param(tail, color, tail)) return 1;
		if (tail == NULL) {
			pattern = NULL;
		} else {
			if (Param(tail, pattern, tail)) return 1;
		}
	}
	if (strcmp(style, _brush_style) != 0 ||
			color != _brush_color ||
			AGmString::str_cmp(pattern , _brush_pattern, 8) ) {

		if (_brush && _brush_chg == 0) {
			ADvPutCBrush(_brush, _brush_style, _brush_color, _brush_pattern);
			_brush = NULL;
		}

		_brush_style = style;
		_brush_color = (long)color;
		_brush_pattern = pattern;
		_brush_chg = 1;
	}
	return 0;
}

#define PI 3.14159265359

int ADvWindow::PushFont(AGmNode *info) {
	char *name;
#if	_WINDOWS && _64BIT
	long long height, width, color;
	long long font_bold, font_italic, font_underline;
#else
	long height, width, color;
	long font_bold, font_italic, font_underline;
#endif
	AGmNode *tail;
	double h_scale, w_scale; int escape, orient;
	if (info == NULL) {
		name = DEFAULT_FONT_NAME;
		height = DEFAULT_FONT_HEIGHT;
		width = DEFAULT_FONT_WIDTH;
		color = ADV_FG_COLOR;
		h_scale = w_scale = 1.0;
		escape = orient = 0;
		font_bold = font_italic = font_underline = 0;
	} else {
		if (Param(info, name, tail) || Param(tail, height, width, color, tail)) {
			return 1;
		}
		double t, tx, ty, sx, sy, dx, dy;
		tx = sy = 1.0;
		ty = sx = dx = dy = 0.0;
		if (_tm) {
			_tm->a.m.v->Transform(tx, ty, tx, ty);
			_tm->a.m.v->Transform(sx, sy, sx, sy);
			_tm->a.m.v->Transform(dx, dy, dx, dy);
		}
		tx -= dx; ty -= dy;
		sx -= dx; sy -= dy;
		t = sqrt(tx*tx + ty*ty);
		w_scale = t;
		h_scale = (sy*tx - sx*ty)/t;
		if (h_scale < 0) {
			orient = 1800;
			h_scale = -h_scale;
		} else {
			orient = 0;
		}
		escape = (int)(acos(tx/t)*1800/PI);
		if (ty < 0) {
			escape = 3600-escape;
		}
		if (tail) {
			if (Param(tail, font_bold, font_italic, font_underline, tail)) {
				return 1;
			}
		} else {
			font_bold = font_italic = font_underline = 0;
		}
	}
	if (strcmp(name, _font_name) != 0 ||
			_font_height != height ||
			_font_width != width ||
			_font_color != color ||
			_h_scale != h_scale ||
			_w_scale != w_scale ||
			_escape != escape ||
			_orient != orient ||
			_font_bold != font_bold ||
			_font_italic != font_italic ||
			_font_underline != font_underline) {

		if (_font && _font_chg == 0) {
			ADvPutCFont(_font, _font_name, _font_height, _font_width,
				_font_color, _h_scale, _w_scale, _escape, _orient,
				_font_bold, _font_italic, _font_underline);
			_font = NULL;
		}

		_font_name = name;
		_font_height = (char)height;
		_font_width = (char)width;
		_font_color = (long)color;
		_font_chg = 1;
		_h_scale = h_scale;
		_w_scale = w_scale;
		_escape = escape;
		_orient = orient;
		_font_bold = (char)font_bold;
		_font_italic = (char)font_italic;
		_font_underline = (char)font_underline;
	}
	Push(_font_stack, info);
	return 0;
}
AGmNode* ADvWindow::GetFontInfo() {
	AGmNode *info = List(String(_font_name), Integer(_font_height), Integer(_font_width),
		Integer(_font_color), Integer(_font_bold), Integer(_font_italic), Integer(_font_underline));
	return info;
}
int ADvWindow::PopFont() {
	char *name;
#if	_WINDOWS && _64BIT
	long long height, width, color;
	long long font_bold, font_italic, font_underline;
#else
	long height, width, color;
	long font_bold, font_italic, font_underline;
#endif
	AGmNode *tail;
	double h_scale, w_scale; int escape, orient;
	if (_font_stack) Pop(_font_stack);
	if (_font_stack == NULL || _font_stack->H == NULL) { // default
		name = DEFAULT_FONT_NAME;
		height = DEFAULT_FONT_HEIGHT;
		width = DEFAULT_FONT_WIDTH;
		color = ADV_FG_COLOR;
		h_scale = w_scale = 1.0;
		escape = orient = 0;
		font_bold = font_italic = font_underline = 0;
	} else {
		if (Param(_font_stack->H, name, tail) || Param(tail, height, width, color, tail)) return 1;
		double t, tx, ty, sx, sy, dx, dy;
		tx = sy = 1.0;
		ty = sx = dx = dy = 0.0;
		if (_tm) {
			_tm->a.m.v->Transform(tx, ty, tx, ty);
			_tm->a.m.v->Transform(sx, sy, sx, sy);
			_tm->a.m.v->Transform(dx, dy, dx, dy);
		}
		tx -= dx; ty -= dy;
		sx -= dx; sy -= dy;
		t = sqrt(tx*tx + ty*ty);
		w_scale = t;
		h_scale = (sy*tx - sx*ty)/t;
		if (h_scale < 0) {
			orient = 1800;
			h_scale = -h_scale;
		} else {
			orient = 0;
		}
		escape = (int)(acos(tx/t)*1800/PI);
		if (ty < 0) {
			escape = 3600-escape;
		}
		if (tail) {
			if (Param(tail, font_bold, font_italic, font_underline, tail)) return 1;
		} else {
			font_bold = font_italic = font_underline = 0;
		}
	}
	if (strcmp(name, _font_name) != 0 ||
			_font_height != height ||
			_font_width != width ||
			_font_color != color ||
			_h_scale != h_scale ||
			_w_scale != w_scale ||
			_escape != escape ||
			_orient != orient ||
			_font_bold != font_bold ||
			_font_italic != font_italic ||
			_font_underline != font_underline) {

		if (_font && _font_chg == 0) {
			ADvPutCFont(_font, _font_name, _font_height, _font_width,
				_font_color, _h_scale, _w_scale, _escape, _orient,
				_font_bold, _font_italic, _font_underline);
			_font = NULL;
		}

		_font_name = name;
		_font_height = (char)height;
		_font_width = (char)width;
		_font_color = (long)color;
		_font_chg = 1;
		_h_scale = h_scale;
		_w_scale = w_scale;
		_escape = escape;
		_orient = orient;
		_font_bold = (char)font_bold;
		_font_italic = (char)font_italic;
		_font_underline = (char)font_underline;
	}
	return 0;
}

int ADvWindow::PushTM(AGmNode *info) {
	AGmMatrix* tm; AGmNode *tail;
	if (info == NULL) {
		Unref(_tm); _tm = NULL;
	} else {
		if (Param(info, tm, tail)) {
			return 1;
		}
		Set(_tm, info->H);
	}
	Push(_tm_stack, info);
	return 0;
}
int ADvWindow::PopTM() {
	AGmMatrix* tm; AGmNode *tail;
	if (_tm_stack) Pop(_tm_stack);
	if (_tm_stack == NULL || _tm_stack->H == NULL) {
		Unref(_tm); _tm = NULL;
	} else {
		if (Param(_tm_stack->H, tm, tail)) return 1;
		Set(_tm, _tm_stack->HH);
	}
	return 0;
}

int ADvWindow::_ChangeClipping() {
	if (_dc == NULL) return 1;
	_clip_all = 0;
	CRgn *rgn = NULL;
	CRgn *rgn2 = UpdateRgn();
#ifdef	_WINDOWS
	LPPOINT pts; LPINT npts; int npoly;
#endif
#ifdef	_LINUX
	GdkPoint *pts; int* npts; int npoly;
#endif
	int flag;
	if (_clip) {
		if (_PolygonData(_clip, pts, npts, npoly)) return 1;
		if (pts[0].x == 0 && pts[0].y == 0 && pts[1].x == 0 && pts[1].y == 0 &&
				pts[2].x == 0 && pts[2].y == 0 && pts[3].x == 0 && pts[3].y == 0) {
			_clip_all = 1;
			delete [] pts; delete [] npts;
			_clip_chg = 0;
			return 0;
		}
		rgn = new CRgn;
		flag = rgn->CreatePolyPolygonRgn(pts, npts, npoly, ALTERNATE);
		delete [] pts; delete [] npts;
		if (flag == 0) {
			delete rgn; return 1;
		}
		if (_dc_for_quick_draw == 0 && rgn2) {
			rgn->CombineRgn(rgn, rgn2, RGN_AND);
		}
	}
	if (rgn == NULL && _dc_for_quick_draw == 0 && rgn2) {
		rgn = new CRgn();
		rgn->CreateRectRgn(0, 0, 1, 1);
		rgn->CopyRgn(rgn2);
	}
#ifdef	_WINDOWS
	if (_dc->IsKindOf(RUNTIME_CLASS(CPreviewDC))) {
		// codes for avoid bug of MFC
		ASSERT(_dc->m_hDC);
		if (_dc->m_hAttribDC)
			::SelectClipRgn(_dc->m_hAttribDC, (HRGN)rgn->GetSafeHandle());
		if (_dc->m_hDC != _dc->m_hAttribDC) {
			CPoint oldVpOrg = _dc->SetViewportOrg(0,0) ;
			CPoint sizeTopLeft;
			::GetViewportOrgEx(_dc->m_hDC, &sizeTopLeft);
			_dc->SetViewportOrg( oldVpOrg ) ;
			RECT r1 ;
			::GetClipBox(_dc->m_hAttribDC, &r1) ;
			_dc->LPtoDP(&r1);
			CPoint	p1(r1.left, r1.top), p2(r1.right, r1.bottom) ;
			((CPreviewDC*)_dc)->PrinterDPtoScreenDP(&p1) ;
			((CPreviewDC*)_dc)->PrinterDPtoScreenDP(&p2) ;
			HRGN rgn1;
			rgn1 = ::CreateRectRgn(
				p1.x + sizeTopLeft.x, p1.y + sizeTopLeft.y, 
				p2.x + sizeTopLeft.x, p2.y + sizeTopLeft.y ) ;
			::SelectClipRgn(_dc->m_hDC, rgn1);
			::DeleteObject(rgn1);
		}
	} else
#endif
	if (_dc->SelectClipRgn(rgn) == ERROR) {
		if (rgn) delete rgn;
		return 1;
	}
	if (rgn) delete rgn;
	_clip_chg = 0;
	return 0;
}
int ADvWindow::_ChangeBgColor() {
	if (_dc == NULL) return 1;
	_dc->SetBkColor(_bg_color);
	_bg_color_chg = 0;	
	return 0;
}
int ADvWindow::_ChangeColorFunc() {
	if (_dc == NULL) return 1;
	switch (_color_func)	{
	case ADvColorCopy:	_dc->SetROP2(R2_COPYPEN);	break;
	case ADvColorXor:	_dc->SetROP2(R2_XORPEN);	break;
	default:	break;
	}
	_color_func_chg = 0;
	return 0;
}
int ADvWindow::_ChangePen() {
	if (_dc == NULL) return 1;
	CPen *pen = ADvGetCPen(_pen_style, _pen_width, _pen_color);
	if (pen == NULL) {
		if (strcmp(_pen_style, "Solid") == 0) {
			pen = new CPen(PS_SOLID, (int)_pen_width, _pen_color);
		} else if (strcmp(_pen_style, "Dot") == 0) {
			pen = new CPen(PS_DOT, (int)_pen_width, _pen_color);
		} else { // "Null"
			if (_pen_selected) {
				_dc->SelectStockObject(NULL_PEN);
			} else {
				_original_pen = (CPen*)_dc->SelectStockObject(NULL_PEN);
				_pen_selected = 1;
			}
			_pen = NULL;
			_pen_chg = 0;
			return 0;
		}
	}
	if (_pen_selected) {
		_dc->SelectObject(pen);
	} else {
		_original_pen = (CPen*)_dc->SelectObject(pen);
		_pen_selected = 1;
	}
	_pen = pen;
	_pen_chg = 0;
	return 0;
}
int ADvWindow::_ChangeBrush() {
	if (_dc == NULL) return 1;
	CBrush *brush = ADvGetCBrush(_brush_style, _brush_color, _brush_pattern);
	if (brush == NULL) {
		if (strcmp(_brush_style, "Solid") == 0) {
			brush = new CBrush(_brush_color);
		} else { // "Null"
			if (_brush_selected) {
				_dc->SelectStockObject(NULL_BRUSH);
			} else {
				_original_brush = (CBrush*)_dc->SelectStockObject(NULL_BRUSH);
				_brush_selected = 1;
			}
			_brush = NULL;
			_brush_chg = 0;
			return 0;
		}
	}
	if (_brush_selected) {
		_dc->SelectObject(brush);
	} else {
		_original_brush = (CBrush*)_dc->SelectObject(brush);
		_brush_selected = 1;
	}
	_brush = brush;
	_brush_chg = 0;
	return 0;
}
int ADvWindow::_ChangeFont() {
	if (_dc == NULL) return 1;
	_dc->SetTextColor(_font_color);
	CFont *font = ADvGetCFont(
		_font_name, _font_height, _font_width, _font_color,
		_h_scale, _w_scale, _escape, _orient,
		_font_bold, _font_italic, _font_underline);
	if (font == NULL) {
		font = new CFont();
		int flag = font->CreateFont(
			(int)(_font_height * _h_scale),
			(int)(_font_width * _w_scale),
			-_escape, /* print direction */
			_orient, /* orientation of character */
			(_font_bold ? FW_BOLD : FW_NORMAL),
			_font_italic,
			_font_underline,
			FALSE, /* not strikeout */
#ifdef	AL_LANG_JAPANESE
			SHIFTJIS_CHARSET,
#else
			DEFAULT_CHARSET,
#endif
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			_font_name);
		if (flag == 0) return 1;
	}
	if (_font_selected) {
		_dc->SelectObject(font);
	} else {
		_original_font = (CFont*)_dc->SelectObject(font);
		_font_selected = 1;
	}
	_font = font;
	_font_chg = 0;
	return 0;
}

// for immediate redraw
int ADvWindow::StartQuickDraw() {
	if (_dc_for_quick_draw || _dc != NULL) return 0;
#ifdef	_WINDOWS
	if (_cview) {	
		_dc = new CClientDC(_cview);
	} else if (_ole_ctrl) {
		_dc = new CClientDC(_ole_ctrl);
	} else {
		return 1;
	}
#endif
#ifdef	_LINUX
	if (_cview) {
		_dc = new CDC(_cview);
	} else {
		return 1;
	}
#endif
	_dc_for_quick_draw = 1;
	StartDraw();
	return 0;
}
int ADvWindow::EndQuickDraw() { 
	if (_dc_for_quick_draw == 0) return 1;
	EndDraw();
	delete _dc;
	_dc = NULL;
	_dc_for_quick_draw = 0;
#ifdef	_LINUX
	GtkAllocation allocation;
	gtk_widget_get_allocation(_cview->m_pWidget, &allocation);
	GdkRectangle rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = allocation.width;
	rect.height = allocation.height;
	gdk_window_invalidate_rect(gtk_widget_get_window(_cview->m_pWidget), &rect, FALSE);
#endif
	return 0;
}

// draw functions
int ADvWindow::Point(double x, double y, long color) {
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_tm) {
		if (_tm->a.m.v->Transform(x, y, x, y)) return 1;
	}
	x = INT_LIMIT(x); y = INT_LIMIT(y);
	_dc->SetPixel((int)R4E5(x), (int)R4E5(y), color);
	return 0;
}
int ADvWindow::Line(double x1, double y1, double x2, double y2) {
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_pen_chg) if (_ChangePen()) return 1;
	if (_tm) {
		if (_tm->a.m.v->Transform(x1, y1, x1, y1)) return 1;
		if (_tm->a.m.v->Transform(x2, y2, x2, y2)) return 1;
	}
	x1 = INT_LIMIT(x1); y1 = INT_LIMIT(y1);
	x2 = INT_LIMIT(x2); y2 = INT_LIMIT(y2);
	_dc->MoveTo((int)R4E5(x1), (int)R4E5(y1));
	_dc->LineTo((int)R4E5(x2), (int)R4E5(y2));
	return 0;
}	  
int ADvWindow::Rectangle(double left, double top, double right, double bottom) {
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_bg_color_chg) if (_ChangeBgColor()) return 1;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_pen_chg) if (_ChangePen()) return 1;
	if (_brush_chg) if (_ChangeBrush()) return 1;
	AGmNode *polypoly = Ref(CreatePolyPoly());
	AddRectToPoly(polypoly, left, top, right, bottom);
	int flag = PolyPolygon(polypoly);
	Unref(polypoly);
	return flag;
}		
static const double p1 = 0.89657547;
static const double p2 = 0.70710678;
static const double p3 = 0.51763809;
static const double p4 = 0.26794919;
int ADvWindow::Ellipse(double left, double top, double right, double bottom) {
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_bg_color_chg) if (_ChangeBgColor()) return 1;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_pen_chg) if (_ChangePen()) return 1;
	if (_brush_chg) if (_ChangeBrush()) return 1;
	AGmNode *polypoly = Ref(CreatePolyPoly());
	double	x  = (right + left) * 0.5;
	double	y  = (bottom + top) * 0.5;
	double	rx = (right - left) * 0.5;
	double	ry = (bottom - top) * 0.5;
	double	px1 = p1 * rx, py1 = p1 * ry;
	double	px2 = p2 * rx, py2 = p2 * ry;
	double	px3 = p3 * rx, py3 = p3 * ry;
	double	px4 = p4 * rx, py4 = p4 * ry;
	AddPointToPoly(polypoly,							 x+rx,	y);
	AddCurveToPoly(polypoly, x+rx,	y+py4, x+px1, y+py3, x+px2, y+py2);
	AddCurveToPoly(polypoly, x+px3, y+py1, x+px4, y+ry,  x, 	y+ry);
	AddCurveToPoly(polypoly, x-px4, y+ry,  x-px3, y+py1, x-px2, y+py2);
	AddCurveToPoly(polypoly, x-px1, y+py3, x-rx,  y+py4, x-rx,	y);
	AddCurveToPoly(polypoly, x-rx,	y-py4, x-px1, y-py3, x-px2, y-py2);
	AddCurveToPoly(polypoly, x-px3, y-py1, x-px4, y-ry,  x, 	y-ry);
	AddCurveToPoly(polypoly, x+px4, y-ry,  x+px3, y-py1, x+px2, y-py2);
	AddCurveToPoly(polypoly, x+px1, y-py3, x+rx,  y-py4, x+rx,	y);
	int flag = PolyPolygon(polypoly);
	Unref(polypoly);
	return flag;
}		
int ADvWindow::PolyPolyline(AGmNode *polypoly) { // assume that polypoly is reffered
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_pen_chg) if (_ChangePen()) return 1;
	AGmNode *polypoly2;
	if (_tm == NULL) {
		polypoly2 = polypoly;
	} else {
		if (_tm->a.m.v->Transform(polypoly, polypoly2)) return 1;
	}
#ifdef	_WINDOWS
	LPPOINT pts; LPINT npts; int npoly;
#endif
#ifdef	_LINUX
	GdkPoint *pts; int* npts; int npoly;
#endif
	Ref(polypoly2);
	if (_PolygonData(polypoly2, pts, npts, npoly)) return 1;
	Unref(polypoly2);
	long j = 0;
	for (long i = 0; i < npoly; i++) {
		BYTE *types = new BYTE[npts[i]];
		int k;
		for (k = 0; k < npts[i]; k++) {
			if (k == 0 || k == npts[i] - 1) {
				types[k] = PT_MOVETO;
			} else {
				types[k] = PT_LINETO;
			}
		}
		_dc->PolyDraw(&pts[j], types, npts[i]);
		delete [] types;
		j += npts[i];
	}
	delete [] pts; delete [] npts;
	return 0;
}
int ADvWindow::PolyPolygon(AGmNode *polypoly) { // asume that polypoly is Refered
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_bg_color_chg) if (_ChangeBgColor()) return 1;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_pen_chg) if (_ChangePen()) return 1;
	if (_brush_chg) if (_ChangeBrush()) return 1;
	AGmNode *polypoly2;
	if (_tm == NULL) {
		polypoly2 = polypoly;
	} else {
		if (_tm->a.m.v->Transform(polypoly, polypoly2)) return 1;
	}
#ifdef	_WINDOWS
	LPPOINT pts; LPINT npts; int npoly;
#endif
#ifdef	_LINUX
	GdkPoint *pts; int* npts; int npoly;
#endif
	Ref(polypoly2);
	if (_PolygonData(polypoly2, pts, npts, npoly)) return 1;
	Unref(polypoly2);
	long j = 0;
	for (long i = 0; i < npoly; i++) {
		_dc->Polygon(&pts[j], npts[i]);
		j += npts[i];
	}
	delete [] pts; delete [] npts;
	return 0;
}		
int ADvWindow::TextOut(double x, double y, const char *s) {
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_bg_color_chg) if (_ChangeBgColor()) return 1;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_font_chg) if (_ChangeFont()) return 1;
	_dc->SetTextColor(_font_color);
	if (_tm) {
		if (_tm->a.m.v->Transform(x, y, x, y))	return 1;
	}
	x = INT_LIMIT(x); y = INT_LIMIT(y);
	_dc->TextOut((int)R4E5(x), (int)R4E5(y), s, (int)strlen(s));
	return 0;
}
#if	_WINDOWS && _64BIT
int ADvWindow::TextExtent(const char *s, long long& width, long long& height) {
#else
int ADvWindow::TextExtent(const char *s, long& width, long& height) {
#endif
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_font_chg) if (_ChangeFont()) return 1;
#ifdef	_WINDOWS
	CSize size = _dc->GetTextExtent(s, (int)strlen(s));
	width = size.cx; height = size.cy;
#endif
#ifdef	_LINUX
	_dc->GetTextExtent(s, strlen(s), width, height);
#endif
	return 0;
}
#if	_WINDOWS && _64BIT
int ADvWindow::TextOutFromTo(double x, double y, const char *s, long long from, long long to) {
#else
int ADvWindow::TextOutFromTo(double x, double y, const char *s, long from, long to) {
#endif
	ASSERT(from <= to);
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_bg_color_chg) if (_ChangeBgColor()) return 1;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_font_chg) if (_ChangeFont()) return 1;
	_dc->SetTextColor(_font_color);
	if (_tm) {
		if (_tm->a.m.v->Transform(x, y, x, y))	return 1;
	}
	x = INT_LIMIT(x); y = INT_LIMIT(y);
	_dc->TextOut((int)R4E5(x), (int)R4E5(y), &s[from], (int)(to - from));
	return 0;
}
#if	_WINDOWS && _64BIT
int ADvWindow::TextExtentFromTo(const char *s, long long from, long long to, long long& width, long long& height) {
#else
int ADvWindow::TextExtentFromTo(const char *s, long from, long to, long& width, long& height) {
#endif
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_font_chg) if (_ChangeFont()) return 1;
	if (to < from || strlen(s) < (size_t)to) return 1;
#ifdef	_WINDOWS
	CSize size = _dc->GetTextExtent(&s[from], (int)(to - from));
	width = size.cx; height = size.cy;
#endif
#ifdef	_LINUX
	_dc->GetTextExtent(&s[from], to - from, width, height);
#endif
	return 0;
}
int ADvWindow::ExtTextOut(double left, double top, double right, double bottom, const char *s) {
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_bg_color_chg) if (_ChangeBgColor()) return 1;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_font_chg) if (_ChangeFont()) return 1;
	_dc->SetTextColor(_font_color);
	if (_tm) {
		if (_tm->a.m.v->Transform(left, top, left, top)) return 1;
		if (_tm->a.m.v->Transform(right, bottom, right, bottom)) return 1;
	}
	left = INT_LIMIT(left); top = INT_LIMIT(top);
	right = INT_LIMIT(right); bottom = INT_LIMIT(bottom);
	int width = (int)(right - left);
	char *ss = new char[strlen(s)+1];
	int idx = 0, i, j = 0, k;
	long w, h = 0;
	while (s[idx]) {
		k = 0;
		for (i = 0; s[i+idx]; i++) {
			ss[i] = s[i+idx]; ss[i+1] = 0; j = 1;
#ifdef	_WINDOWS
			if (isFirstChar(ss[i])) {
				i++; ss[i] = s[i+idx]; ss[i+1] = 0; j = 2;
			}
			CSize size = _dc->GetTextExtent(ss, (int)strlen(ss));
			w = size.cx; h = size.cy;
#endif
#ifdef	_LINUX
			int i2;
			if (isUTF8((unsigned char*)&s[i+idx], i2)) {
				art_strcpy(&ss[i], 8, (const char*)utf8);
				i += i2-1;
				j = i2;
			}
			_dc->GetTextExtent(ss, strlen(ss), w, h);
#endif
			if (w > width) {
				k = 1; i++; break;
			}
		}
		if (k) i -= j;
		ss[i] = 0;
		if (i == 0)
			break;
		_dc->TextOut((int)R4E5(left), (int)R4E5(top), ss, (int)strlen(ss));
		idx += i; top += h + 2;
	}
	delete [] ss;
	return 0;
}
int ADvWindow::Pie(double left, double top, double right, double bottom,
					double sx, double sy, double ex, double ey) {
	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL)
		return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;
	if (_bg_color_chg) if (_ChangeBgColor()) return 1;
	if (_color_func_chg) if (_ChangeColorFunc()) return 1;
	if (_pen_chg) if (_ChangePen()) return 1;
	if (_brush_chg) if (_ChangeBrush()) return 1;
	AGmNode *polypoly = Ref(CreatePolyPoly());
	double	x  = (right + left) * 0.5;
	double	y  = (bottom + top) * 0.5;
	double	rx = (right - left) * 0.5;
	double	ry = (bottom - top) * 0.5;
	double	sr, er, d1, d2;
	d1 = sx - x; d2 = sy - y;
	if (d1 == 0) {
		er = (d2 < 0 ? 1.5 : 0.5) * PI;
	} else {
		er = atan2( rx * d2, ry * d1);
		if (er < 0)
			er += 2 * PI;
	}
	d1 = ex - x; d2 = ey - y;
	if (d1 == 0) {
		sr = (d2 < 0 ? 1.5 : 0.5) * PI;
	} else {
		sr = atan2(rx * d2, ry * d1);
		if (sr < 0)
			sr += 2 * PI;
	}
	if (er <= sr)
		er += 2 * PI;
	AddPointToPoly(polypoly, x+rx*cos(sr), y+ry*sin(sr));
	double nr, rr, d;
	rr = sr;
	d = floor(sr / PI * 2) + 1;
	d = d * PI / 2 - sr;
	for (;;) {
		nr = rr + d;
		if (er < nr) nr = er;
		double dr = (nr - rr) / 3.0;
		double drc = cos(dr);
		double csx = rx * cos(rr+dr) / drc + x;
		double csy = ry * sin(rr+dr) / drc + y;
		double cex = rx * cos(nr-dr) / drc + x;
		double cey = ry * sin(nr-dr) / drc + y;
		AddCurveToPoly(polypoly, csx, csy, cex, cey, rx*cos(nr)+x, ry*sin(nr)+y);
		if (nr == er) break;
		rr += d;
		d = PI / 2;
	}
	AddPointToPoly(polypoly, x, y);
	int flag = PolyPolygon(polypoly);
	Unref(polypoly);
	return flag;
}

#ifdef _WINDOWS
#undef T
#include <atlimage.h>
#endif

int ADvWindow::Image(double x, double y, ADvBitmap *bitmap, double width, double height) {

	if (bitmap->Buffer() == NULL) return 0;

	if ((_cview == NULL && _ole_ctrl == NULL) || _dc == NULL) return 1;
	if (_clip_chg) if (_ChangeClipping()) return 1;
	if (_clip_all) return 0;

	if (width <= 0) width = bitmap->Width();
	if (height <= 0) height = bitmap->Height(); 
	double l, t, r, b;
	if (_tm) {
		_tm->a.m.v->Transform(x, y, l, t);
		_tm->a.m.v->Transform(x + width, y + height, r, b);
	} else {
		l = x; t = y;
		r = x + width; b = y + height;
	}

#ifdef	_WINDOWS
	bitmap->Buffer()->Draw(_dc->m_hDC, (int)l, (int)t, (int)(r-l), (int)(b-t));
#endif
#ifdef	_LINUX
	if (bitmap->Width() == (int)(r-l) && bitmap->Height() == (int)(b-t)) {
		gdk_cairo_set_source_pixbuf(_dc->gc, bitmap->Buffer(), l, t);
		cairo_paint(_dc->gc);
	} else {
		GdkPixbuf *pixbuf = gdk_pixbuf_scale_simple(bitmap->Buffer(), (int)(r-l), (int)(b-t), GDK_INTERP_BILINEAR);
		gdk_cairo_set_source_pixbuf(_dc->gc, pixbuf, l, t);
		cairo_paint(_dc->gc);
		g_object_unref(pixbuf);
	}
#endif

	return 0;
}

// callback
int ADvWindow::DefaultPickProc(ADvWindow *aw, AGmNode *e) {
	ADvEditShell *shell = aw->Shell();
	if (shell == NULL) return -1;
	int result = -1;
	switch (e->HI) {
	case ADvLeftPress:
		result = shell->LeftPress(aw, e); 		break;
	case ADvLeftRelease:
		result = shell->LeftRelease(aw, e);		break;
	case ADvLeftDoubleClick:
		result = shell->LeftDoubleClick(aw, e);	break;
	case ADvRightPress:
		result = shell->RightPress(aw, e);		break;
	case ADvRightRelease:
		result = shell->RightRelease(aw, e);		break;
	case ADvRightDoubleClick:
		result = shell->RightDoubleClick(aw, e);	break;
	case ADvMouseMove:
		result = shell->MouseMove(aw, e); 		break;
	case ADvKeyPress:
		result = shell->KeyPressFunc(aw, e);
#ifdef	_LINUX
		result = shell->KeyChar(aw, e);
#endif
		break;
	case ADvKeyChar:
		result = shell->KeyChar(aw, e);			break;
	case ADvKeyRelease:
		result = shell->KeyReleaseFunc(aw, e);	break;	
	default:	break;
	}
	return result;
}

// local event loop
void ADvWindow::LocalLoop() {
	_exit_loop = 0;
#ifdef	_WINDOWS
	ADvCWinApp *pApp = ADvGetApp();
	ASSERT(pApp);
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;
	MSG msgCur;
	for (;;) {
		while (bIdle && !::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE)) {
			if (!pApp->OnIdle(lIdleCount)) bIdle = FALSE;
		}
		do {
			if (!pApp->PumpMessage()) { // WM_QUIT
				AfxPostQuitMessage(0);
				return;
			}
			if (_exit_loop) return;
			if (pApp->IsIdleMessage(&msgCur)) {
				bIdle = TRUE; lIdleCount = 0;
			}
		} while (::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE));
	}
#endif
#ifdef	_LINUX
	ADvCWinApp *pApp = ADvGetApp();
	for (;;) {
		int bIdle = TRUE;
		while (bIdle && !g_main_pending()) {
			if (!pApp->OnIdle()) bIdle = FALSE;
		}
		do {
			g_main_iteration(TRUE);
			if (ADvExiting) {
				return;
			}
			if (_exit_loop) {
				return;
			}
		} while (g_main_pending());
		if (ADvExiting) break;
	}
#endif
}

void ADvWindow::ExitLoop() {
#ifdef	_LINUX
	if (Frame()) Frame()->ReleaseCapture();
	if (View()) View()->ReleaseCapture();
	if (EditCtrl()) EditCtrl()->ReleaseCapture();
#endif
	_exit_loop = 1;
}

// generic interface
int AGmDumpTrace = 1;
int ADvWindow::Print(const char* s1, ...) {

	AGmOutput(s1);

#if defined(_DEBUG) || defined(_LINUX_DEBUG)
	if (AGmDumpTrace) {
#if 0
		char buf[8192];
		va_list args;
		va_start(args, s1);
		art_vsprintf(buf, 8192, s1, args);
		va_end(args);
		TRACE0(buf);
#else
		TRACE1(AL_STR("%s"), s1);
#endif
	}
#endif

	ADvCWinApp* app = ADvGetApp();
	if (ADvShowWnd && app && app->m_pMainWnd && AGmOutputGui) {

		void *value;
		GetValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW, value);
		AGmNode *node = (AGmNode*)value;
		ADvWindow *aw;
		if (node == NULL) {

			AGmNode *opt;
			ADvCWinApp *app = ADvGetApp();
			if (app->m_pMainWnd) {
				opt = NULL;
			} else {
				GetValue(NULL, AGM_VM_KEY_OCX_DEBUG_MODE, value);
				if (value == NULL) return 1;
				opt = Ref(List(Integer(0x200c), (AGmNode*)NULL, (AGmNode*)NULL));
			}	
			aw = new ADvWindow();
			if (aw->CreateEdit(NULL, &node, opt) == NULL) {
				Unref(opt);
				delete aw; return 1;
			}
			Unref(opt);

			SetValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW, node);
			aw->SetTitle(AL_T_OUTPUT);
		} else {
			aw = node->a.w.v;
			if (ADvGetApp()) {
				aw->Raise();
			}
		}

		ADvCEdit *edit = aw->EditCtrl();
		if (edit == NULL) return 1;
#ifdef	_WINDOWS
		char *s2 = ADvCEdit::toCEditString(s1);
		edit->SetFocus();
		edit->ReplaceSel(s2);
		delete [] s2;
		edit->UpdateWindow();
#endif
#ifdef	_LINUX
		edit->SetLast();
		edit->ReplaceSel(s1);
		edit->JumpToBottom();
#endif
	}
	return 0;
}

ADvMenuInfo *ADvWindow::_CreateMenuInfo(AGmNode *menu, short& id) {
	if (menu == NULL) return NULL;
	AGmLink *l; short i; AGmNode *n;
	for (i = 0, l = AGm::next_outlink(menu, NULL);
		l; l = AGm::next_outlink(menu, l)) {
		i++;
	}
	if (i == 0) return NULL;
	ADvMenuInfo *info = new ADvMenuInfo[i + 1];
	for (i = 0, l = AGm::next_outlink(menu, NULL);
		l; l = AGm::next_outlink(menu, l)) {
		n = l->arc->dst;
		info[i].name = (char*)n->H->a.s.v->Str();
		info[i].id = ++id;
		info[i].submenu = _CreateMenuInfo(dst_node(n->TH, UniqueStr("menu")), id);
		i++;
	}
	info[i].name = NULL;
	info[i].id = -1;
	info[i].submenu = NULL;
	return info;
}
AGmNode *ADvWindow::_SearchMenuItem(AGmNode *menu, ADvMenuInfo *info, short id) {
	if (menu == NULL || info == NULL) return NULL;
	AGmLink *l; short i; AGmNode *n;
	for (i = 0, l = AGm::next_outlink(menu, NULL);
		l; l = AGm::next_outlink(menu, l)) {
		n = l->arc->dst;
		if (info[i].id == id) return n;
		n = _SearchMenuItem(dst_node(n->TH, UniqueStr("menu")), info[i].submenu, id);
		if (n != NULL) return n;
		i++;
	}
	return NULL;
}
void ADvWindow::_DeleteMenuInfo(ADvMenuInfo* info) {
	if (info == NULL) return;
	for (short i = 0; info[i].name; i++) {
		_DeleteMenuInfo(info[i].submenu);
	}
	delete [] info;
}

long ADvWindow::MenuSelect(ADvWindow *aw, AGmNode *e, ADvMenuInfo* m) {
	if (aw->View() == NULL) {
		return -1;
	}
	CWnd *fr = aw->View()->GetParentFrame();
	if (fr == NULL) {
		return -1;
	}
#if	_WINDOWS && _64BIT
	long long x, y;
	long long w, h;
#else
	long x, y;
	long w, h;
#endif
#ifdef	_WINDOWS
	CRect r;
	fr->GetWindowRect(&r);
	x = r.left + e->THI;
	y = r.top + e->TTHI;
#endif
#ifdef	_LINUX
	gint xx, yy;
	gdk_window_get_root_origin(gtk_widget_get_window(fr->m_pToplevelWidget), &xx, &yy);
	x = xx + e->THI;
	y = yy + e->TTHI;
#endif
	_MenuCalcSize(m, w, h);
	ADvWindow *menu = new ADvWindow();
	AGmNode *opt = Ref(List(Integer(0x200F), NULL, List(x, y, w, h)));
	if (menu->Create(opt) == NULL) {
		Unref(opt); delete menu; return -1;
	}
	Unref(opt);
	AGmLink *lnk;
	AGmNode *l = PtrCell(m, List(w, -1));
	CreateArc(RootGlobal(), l, NULL, NULL, NULL, &lnk);
	menu->LoopParams(l);
	menu->WndDrawProc(ADvWindow::_MenuDraw);
	menu->WndPickProc(ADvWindow::_MenuPick);
	menu->SetTitle("");
	
#ifdef	_WINDOWS
	menu->View()->SetCapture();
	HCURSOR org_cur = GetCursor();
	HCURSOR arrow_cur = LoadCursor(NULL, IDC_ARROW);
	SetCursor(arrow_cur);
	menu->LocalLoop();
	SetCursor(org_cur);
	DestroyCursor(arrow_cur);
	::ReleaseCapture();
#endif
#ifdef	_LINUX
	menu->View()->SetCapture();
	menu->LocalLoop();
#endif
	long id = (long)l->a.p.t->THI;
	DeleteArc(lnk->arc);

	menu->View()->GetParentFrame()->DestroyWindow();

	return id;
}

#define CHAR_WIDTH			11
#define MENU_LINE_HEIGHT	25
#define LEFT_RIGHT_MARGIN	10
#define TOP_BOTTOM_MARGIN	10
#define HEIGHT_ADJUST	11

#if	_WINDOWS && _64BIT
void ADvWindow::_MenuCalcSize(ADvMenuInfo* info, long long& w, long long& h) {
#else
void ADvWindow::_MenuCalcSize(ADvMenuInfo* info, long& w, long& h) {
#endif
	w = h = 0;
	for (int i = 0; info[i].name; i++) {
		w = max(w, (long)strlen(info[i].name));
		h ++;	
	}
	w = w * CHAR_WIDTH + LEFT_RIGHT_MARGIN; w = max(w, 100);
	h = h * MENU_LINE_HEIGHT + TOP_BOTTOM_MARGIN + HEIGHT_ADJUST;
}

int ADvWindow::_MenuDraw(ADvWindow* aw, AGmNode*) {
	ADvMenuInfo *info = (ADvMenuInfo*)aw->LoopParams()->a.p.ptr;
#if	_WINDOWS && _64BIT
	long long width = aw->LoopParams()->a.p.t->HI - LEFT_RIGHT_MARGIN;
	long long index = aw->LoopParams()->a.p.t->THI;
	long long x = LEFT_RIGHT_MARGIN/2, y = TOP_BOTTOM_MARGIN/2;
#else
	long width = aw->LoopParams()->a.p.t->HI - LEFT_RIGHT_MARGIN;
	long index = aw->LoopParams()->a.p.t->THI;
	long x = LEFT_RIGHT_MARGIN/2, y = TOP_BOTTOM_MARGIN/2;
#endif
	for (int i = 0; info[i].name; i++) {
#ifdef _WINDOWS
		aw->Rectangle((double)x, (double)y, (double)(x+width-10), (double)(y+MENU_LINE_HEIGHT));
		aw->TextOut((double)(x+10), (double)(y+5), info[i].name);
		if (i == index) {
			aw->PushColorFunc(List(ADvColorXor));
#if _64BIT
			aw->PushBrush(List("Solid", List((long long)0xffffff)));
#else
			aw->PushBrush(List("Solid", List((long)0xffffff)));
#endif
			aw->Rectangle((double)x, (double)y, (double)(x+width-10), (double)(y+MENU_LINE_HEIGHT));
			aw->PopBrush();
			aw->PopColorFunc();
		}
#endif
#ifdef _LINUX
		if (i != index) {
			aw->Rectangle((double)x, (double)y, (double)(x+width-10), (double)(y+MENU_LINE_HEIGHT));
			aw->TextOut((double)(x+10), (double)(y+5), info[i].name);
		} else {
			aw->PushBrush(List("Solid", List((long)ADV_FG_COLOR)));
			aw->Rectangle((double)x, (double)y, (double)(x+width-10), (double)(y+MENU_LINE_HEIGHT));
			aw->PopBrush();
			AGmNode *fi = aw->GetFontInfo();
			fi->TTTHI = ADV_BG_COLOR;
			aw->PushFont(fi);
			aw->TextOut((double)(x+10), (double)(y+5), info[i].name);
			aw->PopFont();
		}
#endif
		y += MENU_LINE_HEIGHT;
	}
	return 0;
}
int ADvWindow::_MenuPick(ADvWindow* aw, AGmNode *e) {
	ADvMenuInfo* info = (ADvMenuInfo*)aw->LoopParams()->a.p.ptr;
	AGmNode *param = aw->LoopParams()->a.p.t;
	long width = (long)(param->HI - LEFT_RIGHT_MARGIN);
	long last_index = (long)param->THI;
	long ex = (long)e->THI, ey = (long)e->TTHI;
	long x = LEFT_RIGHT_MARGIN/2, y = TOP_BOTTOM_MARGIN/2, index = -1;	
	for (int i = 0; info[i].name; i++) {
		if (x <= ex && ex <= x+width-10 &&
			y <= ey && ey <= y+MENU_LINE_HEIGHT) {
			index = i;	break;
		}
		y += MENU_LINE_HEIGHT;
	}
	CFrameWnd *w;
	switch (e->HI) {
	case ADvLeftPress:
	case ADvRightPress:
#ifdef	_LINUX
		if (aw->disable_event_handling) {
			break;
		}
		aw->disable_event_handling = 1;
#endif
		if (index != -1) {
			if (info[index].submenu) {
				param->THI = MenuSelect(aw, e, info[index].submenu);
			} else {
				param->THI = info[index].id;
			}
		} else {
			param->THI = -1;
		}
		aw->ExitLoop();

		break;
	case ADvMouseMove:
#ifdef	_LINUX
		if (aw->disable_event_handling) {
			break;
		}
#endif
		w = aw->View()->GetParentFrame();
		if (w) w->BringWindowToTop();
		if (index == last_index) break;
		param->THI = index;
		aw->View()->InvalidateRect(NULL);
		break;
	default:
		break;
	}
	return 0;
}

class ADvAskShell : public ADvEditShell {
public:
	ADvAskShell(ADvWindow *);
	virtual ~ADvAskShell() {}
	virtual int KeyChar(ADvWindow*, AGmNode*);
};
ADvAskShell::ADvAskShell(ADvWindow *aw) : ADvEditShell(aw) {
}
static char buf[4096];
int ADvAskShell::KeyChar(ADvWindow *aw, AGmNode *e) {
	if (e->TTTHI != 0x0d) return -1;
#ifdef _LINUX
	if (_edit->multi_byte_input_mode) return -1;
#endif
#ifdef	_WINDOWS
	long from = _edit->LineIndex();
	long to = from + _edit->LineLength();
	_edit->SetSel(from, to);
	long n = _edit->GetSelText(buf, 255);
#endif
#ifdef	_LINUX
	long n = _edit->GetSelText(buf, 255, 1);
#endif
	buf[n] = 0;
	aw->ExitLoop();
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
#endif
#ifdef	_LINUX
extern char* input_dialog(const char* title, const char* text);
#endif

char *ADvWindow::Ask(const char *title, const char *text) {
#ifdef _WINDOWS
	CWinApp *app = AfxGetApp();
	CWnd *pMainWnd = app->m_pMainWnd;
	app->m_pMainWnd = NULL;
	CInputDlg *dlg = new CInputDlg();
	dlg->title = title;
	dlg->text = text;
	INT_PTR ok = dlg->DoModal();
	app->m_pMainWnd = pMainWnd;
	if (ok == IDOK) {
		art_strcpy(buf, 4096, dlg->text);
	} else {
		art_strcpy(buf, 4096, text);
	}
	delete dlg;
#endif
#ifdef _LINUX
	char* cp = input_dialog(title, text);
	if (cp) {
		art_strcpy(buf, 4096, cp);
	} else {
		art_strcpy(buf, 4096, text);
	}
#endif
	return buf;
}

// for garbage collection
void ADvWindow::GCmark() {
	if (_cview || _ole_ctrl) {
		_GCmark(_clip_stack);
		_GCmark(_clip);
		_GCmark(_bg_color_stack);
		_GCmark(_color_func_stack);
		_GCmark(_pen_stack);
		_GCmark(_brush_stack);
		_GCmark(_font_stack);
		_GCmark(_tm_stack);
		_GCmark(_tm);
	}
	_GCmark(w_arg);
	_GCmark(w_event_prog);
	_GCmark(w_draw_prog);
	_GCmark(w_move_prog);
	_GCmark(w_destroy_prog);
	_GCmark(w_activate_prog);
	_GCmark(w_text_ins_prog);

	if (_shell) _shell->GCmark();
	if (_cview) {
		_cview->GCmark();
		CFrameWnd *frm = _cview->GetParentFrame();
#ifdef	_WINDOWS
		if (frm && frm->IsKindOf(RUNTIME_CLASS(ADvCFrameWnd))) {
			((ADvCFrameWnd*)frm)->GCmark();
		}
#endif
#ifdef	_LINUX
		if (frm && (frm->_cwnd_type & ADV_WNDTYPE_ADVCFRAME)) {
			((ADvCFrameWnd*)frm)->GCmark();
		}
#endif
	}
	if (_cframe)
		_cframe->GCmark();
#ifdef	_WINDOWS
	if (_ole_ctrl)
		_ole_ctrl->GCmark();
#endif
}
