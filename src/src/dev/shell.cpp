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
#include <dev/advwnd.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_WINDOWS
char newline[] = { 0x0d, 0x0a, 0x00};
#endif
#ifdef	_LINUX
char newline[] = { 0x0a, 0x00};
#endif

ADvEditShell::ADvEditShell(ADvWindow *aw) {
	_aw = aw;
	_edit = NULL;
	_l_str = NULL; _e_str = NULL;
	_limit_depth = 30000;
	_insert_pos = AUI_SHELL_INSERT_LOWER;
	_insert_mode = 0;
	_status = 0;
}
ADvEditShell::~ADvEditShell() {
	AGmString::DeleteString(_l_str);
	AGmString::DeleteString(_e_str);
}
AGmNode *ADvEditShell::ADvWndToNode(ADvWindow *aw) {
	return _ADvWndToNode(RootWindowNode(), aw);
}
AGmNode *ADvEditShell::_ADvWndToNode(AGmNode *n, ADvWindow *aw) {
	if (IsWindow(n) && n->a.w.v == aw) return n;
	AGmLink *l;
	for (l = next_outlink(n, NULL);
			l; l = next_outlink(n, l)) {
		AGmNode *n2 = _ADvWndToNode(l->arc->dst, aw);
		if (n2) return n2;
	}
	return NULL;
}

void ADvEditShell::Update(int curr_lno) {
	if (_edit == NULL) return;
	int lno = _edit->GetLineCount() - 1;
	int last_line_begin = _edit->LineIndex(lno);
	int last_line_length = _edit->LineLength(lno);
	_edit->SetSel(0,  last_line_begin + last_line_length);
	_edit->ReplaceSel("");
	AGmString *ss = new AGmString(4096);
	for (Reset(); More(); Next()) {
		AGmString *s = GetLineString();
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

void ADvEditShell::Print() {
	char ss[1024];
#ifdef	_WINDOWS
	CFileDialog *dialog = new CFileDialog(FALSE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files (*.*)|*.*||");
	INT_PTR ok = dialog->DoModal();
	if (ok != IDOK) { delete dialog; return; }
	CString cs = dialog->GetPathName();
	art_strcpy(ss, 1024, cs);
	delete dialog;
#endif
#ifdef	_LINUX
	GtkWindow *parent = GTK_WINDOW(ADvGetApp()->m_pMainWnd->m_pToplevelWidget);
	GtkWidget *w = gtk_file_chooser_dialog_new(
		AL_STR(""), parent, GTK_FILE_CHOOSER_ACTION_SAVE,
		AL_STR("Cancel"), GTK_RESPONSE_CANCEL, AL_STR("OK"), GTK_RESPONSE_ACCEPT, NULL);
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(w);
	gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
	gtk_file_chooser_set_current_name(chooser, AL_STR("out.txt"));
	gint res = gtk_dialog_run(GTK_DIALOG(w));
	if (res == GTK_RESPONSE_CANCEL) {
		gtk_widget_destroy(w);
		return;
	}
	gchar *filename = gtk_file_chooser_get_filename(chooser);
	gtk_widget_destroy(w);
	art_strcpy(ss, 1024, filename);
	g_free(filename);
#endif
	AGmFile *f = new AGmFile();
	if (f->Create(ss, AGM_FILE_WRITE)) {
		ADvWindow::Print(AL_S109);
		delete f; return;
	}
	AGmString *title = _aw->GetTitle();
	f->WriteString(title);
	f->WriteString("\n");
	delete title;
	for (Reset(); More(); Next()) {
		AGmString *s = GetLineString();
		ASSERT(s);
		char ss[8];
		art_sprintf_1(ss, 8, "%6d", _depth);
		f->WriteString(ss);
		f->WriteString(" ");
		f->WriteString(s);
		f->WriteString("\n");
	}
	delete f;
}

int ADvEditShell::Reset(long lno) {
	_lno = 0; _depth = 0;
	_status = 0;
	if (lno >=	0) {
		while (More() && _lno < lno)
			Next();
	} else {
		while (!IsLast())
			Next();
	}
	return 0;
}

int ADvEditShell::InsertLine(const char* s) {
	int flag = _PreInsertLine(s);
	if (flag == 0)
		flag |= _InsertLine(s);
	_PostInsertLine(s);
	return flag;
}
int ADvEditShell::ReplaceLine(const char* s) {
	int flag = _PreReplaceLine(s);
	if (flag == 0)
		flag |= _ReplaceLine(s);
	_PostReplaceLine(s);
	return flag;
}
int ADvEditShell::DeleteLine() {
	int flag = _PreDeleteLine();
	if (flag == 0)
		flag |= _DeleteLine();
	_PostDeleteLine();
	return flag;
}
int ADvEditShell::OpenEditor() {
	int flag = _PreOpenEditor();
	if (flag == 0)
		flag |= _OpenEditor();
	_PostOpenEditor();
	return flag;
}

void ADvEditShell::LimitDepth(int depth) {
	_limit_depth = depth;
	Update();
}

int ADvEditShell::LeftPress(ADvWindow*, AGmNode*) {
	return -1;
}
int ADvEditShell::LeftRelease(ADvWindow*, AGmNode*) {
	return -1;
}
int ADvEditShell::LeftDoubleClick(ADvWindow*, AGmNode*) {
	if (_edit == NULL)
		return 1;
	int lno = _edit->LineFromChar(-1);
	Reset(lno);
	OpenEditor();
	return -1;
}
int ADvEditShell::RightPress(ADvWindow*, AGmNode*) {
	return -1;
}
int ADvEditShell::RightRelease(ADvWindow*, AGmNode*) {
	return -1;
}
int ADvEditShell::RightDoubleClick(ADvWindow*, AGmNode*) {
	return -1;
}
int ADvEditShell::MouseMove(ADvWindow*, AGmNode*) {
	return -1;
}
int ADvEditShell::KeyPressFunc(ADvWindow*, AGmNode*) {
	return -1;
}
int ADvEditShell::KeyReleaseFunc(ADvWindow*, AGmNode*) {
	return -1;
}
int ADvEditShell::KeyChar(ADvWindow*, AGmNode *e) {
	if (e->TTTHI != 0x0d) return -1;
#ifdef _LINUX
	if (_edit->multi_byte_input_mode) return -1;
#endif
	char line[ADV_MAX_LINE_LENGTH];
	long lno = _edit->LineFromChar(-1);
	long line_begin = _edit->LineIndex();
	long line_length = _edit->LineLength();
	long line_end = line_begin + line_length;

	_edit->SetSel(line_begin, line_end);
	long n = _edit->GetSelText(line, ADV_MAX_LINE_LENGTH - 2);
	line[n] = 0; art_strcat(line, ADV_MAX_LINE_LENGTH, "\n");

	AGmString *s;
	switch (_insert_mode) {
	case 0: // normal mode
		Reset(lno);
		if (ReplaceLine(line)) {
			// case that Replace is failed
			s = GetErrorString();
			if (s)
				ADvWindow::Print(s->Str());
			else
				ADvWindow::Print(AL_S047);
			break;
		} else {
			// case that Replace is succeded
			s = GetLineString();
			ASSERT(s);
			_edit->SetSel(line_begin, line_end);
			_edit->ReplaceSel(s->Str());
			line_length = _edit->LineLength();
			line_end = line_begin + line_length;
		}
		if (IsLast()) {
			// generate an empty line before END
			_edit->SetSel(line_begin, line_begin);
			_edit->ReplaceSel(newline);
			_edit->SetSel(line_begin, line_begin);
		} else if (_insert_pos == AUI_SHELL_INSERT_LOWER) {
			// generate an empty line after a line
			_edit->SetSel(line_end, line_end);
			_edit->ReplaceSel(newline);
		} else if (_insert_pos == AUI_SHELL_INSERT_UPPER) {
			// generate an empty line before a line
			_edit->SetSel(line_begin, line_begin);
			_edit->ReplaceSel(newline);
			_edit->SetSel(line_begin, line_begin);
		}
		_insert_mode = 1;
		break;
	case 1: // insert mode
		if (line_length == 0) {
			// return to normal mode if an empty line
			_edit->SetSel(line_end, _edit->LineIndex(lno+1));
			_edit->ReplaceSel("");
			_edit->SetSel(line_end, line_end);
			_insert_mode = 0;
			break;
		}
		if (IsLast()) {
			// insert a line at last
			if (lno != _lno) {	
				ADvWindow::Print(AL_S063);
				char err[256];
				art_sprintf_2(err, 256, AL_S064, lno+1, _lno+1);
				ADvWindow::Print(err);
				break;
			}
			if (InsertLine(line)) {
				// case that Insert is failed
				s = GetErrorString();
				if (s)	
					ADvWindow::Print(s->Str());
				else
					ADvWindow::Print(AL_S044);
				break;
			} else {
				// case that Insert is succeded
				s = GetLineString();
				ASSERT(s);
				_edit->SetSel(line_begin, line_end);
				_edit->ReplaceSel(s->Str());
				line_end = line_begin + (int)s->Length();
				int i = NumInsertLine();
				if (i > 1) {
					while (--i > 0) {
						Next();
						s = GetLineString();
						ASSERT(s);
						_edit->ReplaceSel(newline);
						_edit->ReplaceSel(s->Str());
					}
					Reset(lno);
					_edit->SetSel(line_end, line_end);
				}
			}
		} else if (_insert_pos == AUI_SHELL_INSERT_LOWER) {
			// add a line after a line
			if (lno != _lno+1) {	
				ADvWindow::Print(AL_S063);
				char err[256];
				art_sprintf_2(err, 256, AL_S064, lno+1, _lno+2);
				ADvWindow::Print(err);
				break;
			}
			if (InsertLine(line)) {
				// case that Insert is failed
				s = GetErrorString();
				if (s)	
					ADvWindow::Print(s->Str());
				else
					ADvWindow::Print(AL_S044);
				break;
			} else {
				// case that Insert is succeded
				int i = NumOverWriteLine();
				if (i < 2) {
					s = GetLineString();
					ASSERT(s);
					_edit->SetSel(line_begin, line_end);
					_edit->ReplaceSel(s->Str());
					line_end = line_begin + (int)s->Length();
					int i = NumInsertLine();
					if (i > 1) {
						while (--i > 0) {
							Next();
							s = GetLineString();
							ASSERT(s);
							_edit->ReplaceSel(newline);
							_edit->ReplaceSel(s->Str());
						}
						Reset(lno-1);
						_edit->SetSel(line_end, line_end);
					}
				} else {
					Reset(lno);
					int begin = _edit->LineIndex(lno);
					int end = _edit->LineIndex(lno + i);
					_edit->SetSel(begin, end);
					_edit->ReplaceSel("");
					while (i-- > 0) {
						s = GetLineString();
						ASSERT(s);
						_edit->ReplaceSel(s->Str());
						_edit->ReplaceSel(newline);
						Next(); lno++;
					}
				}
			}
		} else if (_insert_pos == AUI_SHELL_INSERT_UPPER) {
			// insert a line before a line
			if (lno != _lno) {	
				ADvWindow::Print(AL_S063);
				char err[256];
				art_sprintf_2(err, 256, AL_S064, lno+1, _lno+1);
				ADvWindow::Print(err);
				break;
			}
			if (InsertLine(line)) {
				// case that Insert is failed
				s = GetErrorString();
				if (s)
					ADvWindow::Print(s->Str());
				else
					ADvWindow::Print(AL_S044);
				break;
			} else {
				// case that Insert is succeded
				s = GetLineString();
				ASSERT(s);
				_edit->SetSel(line_begin, line_end);
				_edit->ReplaceSel(s->Str());
				line_end = line_begin + (int)s->Length();
				int i = NumInsertLine();
				if (i > 1) {
					while (--i > 0) {
						Next();
						s = GetLineString();
						ASSERT(s);
						_edit->ReplaceSel(newline);
						_edit->ReplaceSel(s->Str());
					}
					Reset(lno);
					_edit->SetSel(line_end, line_end);
				}
			}
		}
		_insert_mode = 0;
		break;
	default:
		break;
	}
	return 0;
}
