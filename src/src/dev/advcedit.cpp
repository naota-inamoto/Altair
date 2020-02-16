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
#include <gdm/error.h>
#include <gdm/pict.h>
#include <gdm/vmkey.h>
#include <dev/advcvw.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <interp/interp.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

// Edit on CView
#ifdef	_WINDOWS
IMPLEMENT_DYNAMIC(ADvCEdit, CRichEditCtrl)

ADvCEdit::ADvCEdit() {
	void *value;
	AGm::GetValue(NULL, AGM_VM_KEY_ROOT_CLASS, value);
	_aw = NULL;
	_root_class = AGm::Ref((AGmNode*)value);
}
ADvCEdit::~ADvCEdit() {
	AGm::Unref(_root_class);
}

int ADvCEdit::Create(DWORD style, const RECT& rect ,CWnd* parent, UINT nID) {
	_disable_newline = 0;
	if (CRichEditCtrl::Create(style, rect, parent, nID) == FALSE) return 1;
	return 0;
}

BOOL ADvCEdit::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style |= WS_CLIPSIBLINGS;
	return CRichEditCtrl::PreCreateWindow(cs);
}

LRESULT ADvCEdit::WindowProc(UINT msg, WPARAM w, LPARAM l) {
	LRESULT result = 0;
	HDROP hDrop; int n; char path[256];
	switch (msg) {
	case WM_KEYDOWN:
		switch (w) {
		case 0x08: // backspace
			if (_disable_newline) {
				GetSel(from, to);
				if (from > LineIndex())
					result = CRichEditCtrl::WindowProc(msg, w, l);
				break;
			}
		case 0x0d: // newline
			if (_disable_newline) break;
		default:
			result = CRichEditCtrl::WindowProc(msg, w, l);
			break;
		}
		GetParent()->SendMessage(msg, w, l);
		GetSel(from, to);
		break;
	case WM_CHAR:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		result = CRichEditCtrl::WindowProc(msg, w, l);
		GetParent()->SendMessage(msg, w, l);
		GetSel(from, to);
		if (w == 0x0d) {
			SetSel(from, from);
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MOUSEMOVE:
		result = CRichEditCtrl::WindowProc(msg, w, l);
		{
			CWnd *parent = GetParent();
			CPoint p(LOWORD(l), HIWORD(l));
			MapWindowPoints(parent, &p, 1);
			LPARAM l2 = MAKELPARAM(p.x, p.y);
			parent->SendMessage(msg, w, l2);
		}
		GetSel(from, to);
		break;
	case WM_DROPFILES:
		hDrop = (HDROP)w;
		n = DragQueryFile(hDrop, 0xFFFFFFFF, path, 255);
		if (n > 0) {
			n = DragQueryFile(hDrop, 0, path, 255);
			path[n] = 0;
		}
		DragFinish(hDrop);
		SetSel(from, from); CRichEditCtrl::ReplaceSel(path);
		break;
	default:
		result = CRichEditCtrl::WindowProc(msg, w, l);
		break;	
	}
	return result;
}
void ADvCEdit::DisableNewline(int flag) {
	_disable_newline = flag;
}
long ADvCEdit::GetSelText(char* buf, long buf_max) {
	CString line = CRichEditCtrl::GetSelText();
	long n = line.GetLength();
	if (n > buf_max) n = buf_max;
	art_strncpy(buf, buf_max + 1, line.GetBuffer(), n);
	return n;
}
void ADvCEdit::ReplaceSel(const char* str) {
	CRichEditCtrl::ReplaceSel(str);
	ADvWindow *aw = GetWindow();
	AGmNode **value;
	if (aw && aw->TextInsProg() && AIpProcess::SearchVar(aw->Arg(), AGm::NmEvent(), value) == 0) {
		int lno = GetLineCount();
		if (lno > 0) lno--;
		int len = LineLength();
		AGm::Set(*value, AGm::List(AGm::Integer(lno), AGm::Integer(len)));	
		AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->TextInsProg()));
	}
}

#endif	// _WINDOWS

#ifdef	_LINUX

ADvCEdit::ADvCEdit() {
	from_pos = 0;
	to_pos = 0;
	shift_code = 0;
	multi_byte_input_mode = 0;
	void *value;
	AGm::GetValue(NULL, AGM_VM_KEY_ROOT_CLASS, value);
	_aw = NULL;
	_root_class = AGm::Ref((AGmNode*)value);
}
ADvCEdit::~ADvCEdit() {
	AGm::Unref(_root_class);
}

int ADvCEdit::Create(long edit_style, CWnd *parent) {
	_parent = parent;

	// ---- scroll_view ----
	m_pWidget = gtk_scrolled_window_new(NULL, NULL);
	scrolled_window = GTK_SCROLLED_WINDOW(m_pWidget);
	gtk_widget_set_hexpand(m_pWidget, TRUE);
	gtk_widget_set_vexpand(m_pWidget, TRUE);
	gtk_container_add(GTK_CONTAINER(parent->m_pToplevelWidget), m_pWidget);
	gtk_scrolled_window_set_policy(
		scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_widget_show(m_pWidget);

	// ---- text view ----
	GtkWidget *tv_widget = gtk_text_view_new();
	text_view = GTK_TEXT_VIEW(tv_widget);
	gtk_container_add(GTK_CONTAINER(scrolled_window), tv_widget);

	// set num chars of tab
	gint pixels = -1;
	PangoLayout *layout = gtk_widget_create_pango_layout(tv_widget, "ABCD");
	if (layout != NULL) {
		pango_layout_get_pixel_size(layout, &pixels, NULL);
		g_object_unref(G_OBJECT(layout));
	}
	PangoTabArray *tab_array = pango_tab_array_new(1, TRUE);
	pango_tab_array_set_tab(tab_array, 0, PANGO_TAB_LEFT, pixels);
	gtk_text_view_set_tabs(text_view, tab_array);
	pango_tab_array_free(tab_array);

	GtkWrapMode wrap;
	if ((edit_style & ES_AUTOHSCROLL) == 0) {
		wrap = GTK_WRAP_WORD;
	} else {
		wrap = GTK_WRAP_NONE;
	}
	text_buffer = gtk_text_view_get_buffer(text_view);
	// ### TODO gtk_text_buffer_create_tag(text_buffer, DEFAULT_FONT_FAMILY, "font-desc", get_default_font(), NULL);

	gtk_text_view_set_wrap_mode(text_view, wrap);
	gtk_text_view_set_editable(text_view, TRUE);
	gtk_widget_show(tv_widget);
	
	gtk_widget_grab_focus(tv_widget);

	return 0;
}
void ADvCEdit::DestroyWindow() {
	if (_destroying) return;
	_destroying = 1;
	CWnd::DestroyWindow();
}

void ADvCEdit::GetSel(long& from, long& to) {
	from = from_pos;
	to = to_pos;
}
void ADvCEdit::SetSel(long from, long to) {
	from_pos = from;
	to_pos = to;
}
void ADvCEdit::ReplaceSel(const char *str, int no_cb) {
	long from, to;
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(text_buffer, &start, &end);
	gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
	gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
	g_free(c); c = c2;
	long i, n = strlen(c);
	for (i = 0, from = 0; i < n && i < from_pos; i++, from++) {
		//if (isFirstChar(c[i])) i++;
		int i2;
		if (isUTF8((unsigned char*)&c[i], i2)) i+= i2-1;
	}
	gtk_text_buffer_get_iter_at_offset(text_buffer, &start, from);
	if (from_pos < to_pos) {
		for (to = from; i < n && i < to_pos; i++, to++) {
			//if (isFirstChar(c[i])) i++;
			int i2;
			if (isUTF8((unsigned char*)&c[i], i2)) i += i2-1;
		}
		gtk_text_buffer_get_iter_at_offset(text_buffer, &end, to);
		gtk_text_buffer_delete(text_buffer, &start, &end);
	}
	g_free(c);
	gtk_text_buffer_get_iter_at_offset(text_buffer, &start, from);
	gsize bw = 0; GError *err = NULL;
	n = strlen(str);
	c = g_locale_to_utf8(str, n, NULL, &bw, &err);
	if (err == NULL) {
		gtk_text_buffer_insert(text_buffer, &start, c, bw);
		g_free(c);
		for (i = 0; i < n; i++, from_pos++) {
			//if (isFirstChar(str[i])) { i++; from_pos++; }
			int i2;
			if (isUTF8((unsigned char*)&c[i], i2)) { i += i2-1; from_pos += i2-1; }
		}
	} else {
		char_conversion_error(err, "insert text error: message = %s\n");
	}
	to_pos = from_pos;

	ADvWindow *aw = GetWindow();
	AGmNode **value;
	if (no_cb == 0 && aw && aw->TextInsProg() && AIpProcess::SearchVar(aw->Arg(), AGm::NmEvent(), value) == 0) {
		int lno = GetLineCount();
		if (lno > 0) lno--;
		int len = LineLength(lno);
		AGm::Set(*value, AGm::List(AGm::Integer(lno), AGm::Integer(len)));	
		AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->TextInsProg()));
	}
}

int ADvCEdit::GetLineCount() {
	guint i, lines = 1;
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(text_buffer, &start, &end);
	gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
	gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
	g_free(c); c = c2;
	guint row_len = strlen(c);
	for (i = 0; i < row_len; i++) {
		//if (isFirstChar(c[i])) {
		//	i++; continue;
		//}
		int i2;
		if (isUTF8((unsigned char*)&c[i], i2)) {
			i += i2-1; continue;
		}
		if (c[i] == '\n') lines++;
	}
	g_free(c);
	return lines;
}
int ADvCEdit::LineIndex(int lno) {
	guint i, lines = 0;
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(text_buffer, &start, &end);
	gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
	gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
	g_free(c); c = c2;
	if (lno == -1) lno = LineFromChar(-1);
	guint row_len = strlen(c);
	for (i = 0; i < row_len; i++) {
		//if (isFirstChar(c[i])) {
		//	i++; continue;
		//}
		int i2;
		if (isUTF8((unsigned char*)&c[i], i2)) {
			i += i2-1; continue;
		}
		if (c[i] == '\n') lines++;
		if (lines >= lno) {
			if (c[i] == '\n') i++;
			break;
		}
	}
	g_free(c);
	return i;
}
int ADvCEdit::LineLength(int lno) {
	guint i, lines = 0;
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(text_buffer, &start, &end);
	gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
	gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
	g_free(c); c = c2;
	if (lno == -1) lno = LineFromChar(-1);
	guint row_len = strlen(c);
	for (i = 0; i < row_len; i++) {
		//if (isFirstChar(c[i])) {
		//	i++; continue;
		//}
		int i2;
		if (isUTF8((unsigned char*)&c[i], i2)) {
			i += i2-1; continue;
		}
		if (c[i] == '\n') lines++;
		if (lines >= lno) {
			if (c[i] == '\n') i++;
			break;
		}
	}
	guint from = i;
	for (; i < row_len; i++) {
		//if (isFirstChar(c[i])) {
		//	i++; continue;
		//}
		int i2;
		if (isUTF8((unsigned char*)&c[i], i2)) {
			i += i2-1; continue;
		}
		if (c[i] == '\n') lines++;
		if (lines >= lno+1) break;
	}
	g_free(c);
	guint to = i;
	guint length = to - from;
	return length;
}
long ADvCEdit::GetSelText(char* buf, long buf_max, int all) {
	gchar *c = NULL;
	GtkTextIter start, end;
	if (all == 0) {
		gtk_text_buffer_get_bounds(text_buffer, &start, &end);
		gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
		gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
		g_free(c); c = c2;
		long from, to, i, n = strlen(c);
		for (i = 0, from = 0; i < n && i < from_pos; i++, from++) {
			//if (isFirstChar(c[i])) i++;
			int i2;
			if (isUTF8((unsigned char*)&c[i], i2)) i += i2-1;
		}
		for (to = from; i < n && i < to_pos; i++, to++) {
			//if (isFirstChar(c[i])) i++;
			int i2;
			if (isUTF8((unsigned char*)&c[i], i2)) i += i2-1;
		}
		g_free(c);
		gtk_text_buffer_get_iter_at_offset(text_buffer, &start, from);
		gtk_text_buffer_get_iter_at_offset(text_buffer, &end, to);
	} else {
		gtk_text_buffer_get_bounds(text_buffer, &start, &end);
	}
	c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
	gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
	g_free(c);
	guint length = strlen(c2);
	if (length > buf_max) {
		length = buf_max;
	}
	strncpy(buf, c2, length);
	g_free(c2);
	return length;
}

long ADvCEdit::LineFromChar(long pos) {
	guint i, lines = 0;
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(text_buffer, &start, &end);
	gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
	gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
	g_free(c); c = c2;
	guint row_len = strlen(c);
	if (pos == -1) pos = from_pos;
	for (i = 0; i < row_len; i++) {
		if (c[i] == '\n') lines++;
		if (i >= pos) {
			if (c[i] == '\n') --lines;
			break;
		}
	}
	g_free(c);
	return lines;
}

void ADvCEdit::Cut() {
	Copy();
	Clear();
}
void ADvCEdit::Copy() {
	long len = to_pos - from_pos;
	if (len <= 0) return;
	char *buff = new char[len + 1];
	long n = GetSelText(buff, len);
	buff[n] = 0;

	GtkClipboard *clipboard = gtk_clipboard_get_for_display(gdk_display_get_default(), GDK_SELECTION_CLIPBOARD);
	if (clipboard) {
		gsize br, bw; GError *err = NULL;
		gchar *c = g_locale_to_utf8(buff, n, &br, &bw, &err);
		if (err == NULL) {
			gtk_clipboard_set_text(clipboard, c, bw);
			g_free(c);
		} else {
			char_conversion_error(err, "text copy error: message = %s\n");
		}
	}

	delete [] buff;
}
void ADvCEdit::Paste() {
	GtkClipboard *clipboard = gtk_clipboard_get_for_display(gdk_display_get_default(), GDK_SELECTION_CLIPBOARD);
	if (clipboard && gtk_clipboard_wait_is_text_available(clipboard)) {
		gchar *c = gtk_clipboard_wait_for_text(clipboard);
		long len = c ? strlen(c) : 0;
		if (c && len > 0) {
			gsize br, bw; GError *err = NULL;
			gchar *c2 = g_locale_from_utf8(c, -1, &br, &bw, &err);
			g_free(c);
			if (err == NULL) {
				Clear();
				ReplaceSel(c2, 1);
				g_free(c2);
			} else {
				char_conversion_error(err, "text paste error: message = %s\n");
			}
		}
	}
}
void ADvCEdit::Clear() {
	ReplaceSel("", 1);
}

void ADvCEdit::SetLast() {
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(text_buffer, &start, &end);
	gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
	gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
	g_free(c); c = c2;
	from_pos = strlen(c); to_pos = from_pos;
	g_free(c);
}

void ADvCEdit::JumpToBottom() {
	GtkAdjustment *vbar = gtk_scrolled_window_get_vadjustment(scrolled_window);
	gtk_adjustment_set_value(vbar, gtk_adjustment_get_upper(vbar));
}

void ADvCEdit::char_conversion_error(GError *err, const char* format) {
	GError *e = NULL;
	gchar *str = g_locale_from_utf8(err->message, -1, NULL, NULL, &e);
	char msg[512];
	art_sprintf_1(msg, 512, format, e == NULL ? str : "");
	if (e == NULL) {
		g_free(str);
	} else {
		g_error_free(e);
	}
	g_error_free(err);
	ADvWindow::Print(msg);
}
#endif

char *ADvCEdit::toCEditString(const char *s1) {
	char *cp, *s2 = new char[strlen(s1)*2+1];
	for (cp = s2; *s1; s1++) {
#ifdef	_WINDOWS
		if (isFirstChar(*s1)) {
			*cp++ = *s1++;
			*cp++ = *s1;
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8((unsigned char*)s1, i2)) {
			art_strcpy(cp, 8, (const char*)utf8);
			cp += i2; s1 += i2-1;
#endif
		} else if (*s1 == 0x0a) {
			*cp++ = 0x0d; *cp++ = 0x0a;
		} else {
			*cp++ = *s1;
		}
	}
	*cp = 0x00;
	return s2;
}
char *ADvCEdit::fromCEditString(const char *s1) {
	char *cp, *s2 = new char[strlen(s1)+1];
	for (cp = s2; *s1; s1++) {
#ifdef	_WINDOWS
		if (isFirstChar(*s1)) {
			*cp++ = *s1++;
			*cp++ = *s1;
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8((unsigned char*)s1, i2)) {
			art_strcpy(cp, 8, (const char*)utf8);
			cp += i2; s1 += i2-1;
#endif
		} else if (*s1 == 0x0d) {
		} else {
			*cp++ = *s1;
		}
	}
	*cp = 0x00;
	return s2;
}

void ADvCEdit::SetWindow(ADvWindow* aw) { _aw = aw; }
ADvWindow* ADvCEdit::GetWindow() { return _aw; }
void ADvCEdit::GCmark() {
	AGm::_GCmark(_root_class);
}

#ifdef _WINDOWS
// Edit which exits by CR
IMPLEMENT_DYNAMIC(ADvSingleLineCEdit, CEdit)

ADvSingleLineCEdit::ADvSingleLineCEdit() {
}
ADvSingleLineCEdit::~ADvSingleLineCEdit() {
}

int ADvSingleLineCEdit::Create(DWORD style, const RECT& rect ,CWnd* parent, UINT nID) {
	if (CEdit::Create(style, rect, parent, nID) == FALSE) return 1;
	SetFocus();
	return 0;
}

BOOL ADvSingleLineCEdit::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style |= WS_CLIPSIBLINGS;
	return CEdit::PreCreateWindow(cs);
}

LRESULT ADvSingleLineCEdit::WindowProc(UINT msg, WPARAM w, LPARAM l) {
	if (msg == WM_CHAR && w == 0x0d || msg == WM_KILLFOCUS || msg == WM_ALTAIR+0) {
		_exit_loop = 1;
		return 0;
	} else {
		return CEdit::WindowProc(msg, w, l);
	}
}

void ADvSingleLineCEdit::LocalLoop() {
	_exit_loop = 0;
	MSG msg;
	for (;;) {
		if (GetMessage(&msg, NULL, 0, 0) == 0)
			break; // WM_QUIT
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (_exit_loop) break;
	}
}
#endif	// _WINDOWS


