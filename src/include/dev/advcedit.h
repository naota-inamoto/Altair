/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_ADVCEDIT_H
#define _ADV_ADVCEDIT_H

#define ADV_MAX_LINE_LENGTH 2048

#ifdef	_WINDOWS
// Edit on CView
class ADvCEdit : public CRichEditCtrl {
	DECLARE_DYNAMIC(ADvCEdit)
public:
	ADvCEdit();
	~ADvCEdit();
	int Create(DWORD, const RECT&, CWnd*, UINT);

	virtual BOOL PreCreateWindow(CREATESTRUCT&);
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);

	void DisableNewline(int);

	long GetSelText(char*, long);

	void ReplaceSel(const char*);	

	static char* toCEditString(const char*);
	static char* fromCEditString(const char*);

	void SetWindow(ADvWindow*);
	ADvWindow *GetWindow();
	void GCmark();

protected:
	long from, to;
	int _disable_newline;
	ADvWindow* _aw;
	AGmNode *_root_class;
};

// Edit which exits by CR
class ADvSingleLineCEdit : public CEdit {
	DECLARE_DYNAMIC(ADvSingleLineCEdit)
public:
	ADvSingleLineCEdit();
	int Create(DWORD, const RECT&, CWnd*, UINT);
	~ADvSingleLineCEdit();

	virtual BOOL PreCreateWindow(CREATESTRUCT&);
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);
	void LocalLoop();

protected:
	int _exit_loop;
};
#endif	// _WINDOWS

#ifdef	_LINUX

#define ES_AUTOHSCROLL		0x0001
#define DRAG_ACCEPT_FILES	0x0002

// Edit on CView
class ADvCEdit : public CWnd {
public:
	ADvCEdit();
	~ADvCEdit();

	int Create(long style, CWnd *parent);
	virtual void DestroyWindow();

	void GetSel(long&, long&);
	void SetSel(long, long);
	void ReplaceSel(const char*, int no_cb=0);

	int GetLineCount();
	int LineIndex(int lno = -1);
	int LineLength(int lno = -1);
	long GetSelText(char*, long, int all=0);

	long LineFromChar(long pos);

	void Cut();
	void Copy();
	void Paste();
	void Clear();

	static char* toCEditString(const char*);
	static char* fromCEditString(const char*);

	void SetLast();
	void JumpToBottom();

	static void char_conversion_error(GError*, const char*);

	void SetWindow(ADvWindow*);
	ADvWindow *GetWindow();
	void GCmark();

	GtkScrolledWindow *scrolled_window;
	GtkTextView *text_view;
	GtkTextBuffer *text_buffer;
	long from_pos, to_pos;

	int shift_code;
	int multi_byte_input_mode;

	ADvWindow *_aw;
	AGmNode *_root_class;
};
#endif

#endif	/* _ADV_ADVCEDIT_H */

