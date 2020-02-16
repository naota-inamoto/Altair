/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_ADVCVW_H
#define _ADV_ADVCVW_H

#define MODIF_SHIFT 	0x0001
#define MODIF_CONTROL	0x0002
#define MODIF_ALT		0x0004

class ADvWindow;

AL_EXT_API AGmNode *EvalCallback(AGmNode *root_class,
		AGmNode* arg, AGmNode *obj, AGmNode* func);
AGmNode *EvalCallbackV(AGmNode *root_class,
		AGmNode* arg, AGmNode *obj, AGmNode* func_dcl);

#ifdef	_WINDOWS

#define WM_ALTAIR	(WM_USER + 0x2000)

#ifdef	_WINDOWS
#ifdef	AL_LANG_JAPANESE
#define DEFAULT_FONT_NAME	"ÇlÇr ÉSÉVÉbÉN"
#endif
#ifdef	AL_LANG_ENGLISH
#define DEFAULT_FONT_NAME	"System"
#endif
#define DEFAULT_FONT_HEIGHT 12
#define DEFAULT_FONT_WIDTH	0
#endif

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIChildWnd:

class AL_EXT_CLASS ADvCMDIChildWnd : public CMDIChildWnd {
	DECLARE_DYNCREATE(ADvCMDIChildWnd)
public:
	ADvCMDIChildWnd();
	virtual ~ADvCMDIChildWnd();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT&);
	virtual void OnUpdateFrameMenu(BOOL, CWnd*, HMENU);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// ADvCView:

extern double ADvFilePrintScale;

class AL_EXT_CLASS ADvCView : public CView {
protected: // for serialization
	ADvCView();
	DECLARE_DYNCREATE(ADvCView)

protected:
	virtual BOOL Create(LPCSTR, LPCSTR,
		DWORD, const RECT&, CWnd*, UINT, CCreateContext* context = NULL);
	virtual void OnInitialUpdate();
public:
	virtual ~ADvCView();

protected:
	AGmNode *_node; // node which represents window
	int _destroying;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnDraw(CDC* pDC);
	virtual LRESULT WindowProc(UINT message, WPARAM, LPARAM);
	virtual int CheckEvent(UINT message, WPARAM, LPARAM);
	virtual int EventProcess();
	virtual BOOL OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*);
	AGmNode* _event;

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	AGmNode *CallbackTbl() { return _callback_tbl; }
	AGmNode *RootClass() { return _root_class; }
	void FilePrint() { CView::OnFilePrint(); }
	void FilePrintPreview() { CView::OnFilePrintPreview(); }
protected:
	AGmNode *_callback_tbl;
	AGmNode *_root_class;

// for garbage collection
public:
	void GCmark();

protected:
	//{{AFX_MSG(ADvCView)
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// ADvCDocument:

class ADvCDocument : public CDocument
{
protected: // for serialization
	ADvCDocument();
	DECLARE_DYNCREATE(ADvCDocument)

public:
	virtual ~ADvCDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIFrameWnd:

class ADvCMDIFrameWnd : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(ADvCMDIFrameWnd)
public:
	ADvCMDIFrameWnd();
	virtual ~ADvCMDIFrameWnd();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT&);
	virtual BOOL OnCommand(WPARAM, LPARAM);
	virtual void OnUpdateFrameMenu(HMENU);
	virtual void OnUpdateFrameTitle(BOOL);

public:
	CMultiDocTemplate *DocTmpl() { return m_pDocTmpl; }
	void DocTmpl(CMultiDocTemplate* tmpl) { m_pDocTmpl = tmpl; }
	void SetADvWindow(ADvWindow *w) { _adv_wnd = w; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	long m_style;
	AGmNode *Menu() { return _menu; }
	void Menu(AGmNode* menu);
protected:
	CMultiDocTemplate *m_pDocTmpl;
	AGmNode *_menu, *_root_class;
	ADvWindow *_adv_wnd;

// for garbage collection
public:
	void GCmark();

protected:
	//{{AFX_MSG(ADvCMDIFrameWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT, CWnd*, BOOL);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// ADvCFrameWnd:

class ADvCFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(ADvCFrameWnd)
public:
	ADvCFrameWnd();
	virtual ~ADvCFrameWnd();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT&);
	virtual BOOL OnCommand(WPARAM, LPARAM);
	virtual void OnUpdateFrameMenu(HMENU);

public:
	void SetADvWindow(ADvWindow *w) { _adv_wnd = w; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	long m_style;
	AGmNode *Menu() { return _menu; }
	void Menu(AGmNode* menu);
	ADvCView* _cview0;
protected:
	AGmNode *_menu, *_root_class;
	ADvWindow *_adv_wnd;

// for garbage collection
public:
	void GCmark();

protected:
	//{{AFX_MSG(ADvCFrameWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT, CWnd*, BOOL);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// _WINDOWS

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef	_LINUX

#ifdef	_LINUX
#ifdef	_MAC
#define DEFAULT_FONT_NAME	AL_STR("Hiragino Sans Thin 12")
#define DEFAULT_FONT_FAMILY	AL_STR("hiragino_sans_thin_12")
#define DEFAULT_FONT_HEIGHT 12
#define DEFAULT_FONT_WIDTH	0
#else
#define DEFAULT_FONT_NAME	AL_STR("Sans 12")
#define DEFAULT_FONT_FAMILY	AL_STR("sans_12")
#define DEFAULT_FONT_HEIGHT 12
#define DEFAULT_FONT_WIDTH	0
#endif

#define	DELAY_RESIZE_TIMEOUT	500
#define	DELAY_RESIZE_COUNT		2

#define FW_BOLD 	1
#define FW_NORMAL	0
// dummy
#define SHIFTJIS_CHARSET	0
#define DEFAULT_CHARSET 	0
#define OUT_DEFAULT_PRECIS	0
#define CLIP_DEFAULT_PRECIS 0
#define DEFAULT_QUALITY 	0
#define DEFAULT_PITCH		0
#define FF_DONTCARE 		0
#endif

PangoFontDescription* get_default_font();
PangoFontDescription* get_font_desc(gchar*);

class CFrameWnd;

#define SW_HIDE 	0
#define SW_SHOW 	1
#define SW_RESTORE	2

#define MF_STRING	0
#define MF_POPUP	1

#define ADV_WNDTYPE_CFRAME			0x0001
#define ADV_WNDTYPE_ADVCMDICHILD	0x0002
#define ADV_WNDTYPE_ADVCMDIFRAME	0x0004
#define ADV_WNDTYPE_ADVCFRAME		0x0008
#define ADV_WNDTYPE_ADVCMAINFRAME	0x0010

#define ADV_WNDTYPE_ADVCVIEW		0x0020
#define ADV_WNDTYPE_ADVEDITVIEW 	0x0040

class CMenu;
class ADvCEdit;
class ADvCView;
class CStatusBar;

/////////////////////////////////////////////////////////////////////////////
// CWnd:

class CWnd {
public:
	CWnd();
	virtual ~CWnd();

	virtual void DestroyWindow();

	virtual void SetCapture();
	virtual void ReleaseCapture();

	void InvalidateRect(GdkRectangle*);
	CFrameWnd *GetParentFrame();

	long	_cwnd_type;
	CWnd	*_parent;
	GtkWidget	*m_pToplevelWidget;
	GtkWidget	*m_pWidget;
	int _destroying;
	AGmNode *_children;
	int _capture;
};

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd:

class AL_EXT_CLASS CFrameWnd : public CWnd {
public:
	CFrameWnd();
	virtual ~CFrameWnd();

	virtual void DestroyWindow();

	int ShowWindow(int, int show_child=0);

	int IsIconic();
	void BringWindowToTop();

	virtual void SetMessageText(const char *text) {}

	int _visible, _actual_visible;

	CMenu		*_cmenu;
	ADvCView	*_cview;
	CStatusBar	*_cstatusbar;

	int	_width, _height;
	int _resize_timer_id;
	int _resize_timer_cnt;
};


/////////////////////////////////////////////////////////////////////////////
// CMDIFrameWnd:

class CMDIFrameWnd : public CFrameWnd {
public:
	CMDIFrameWnd();
	virtual ~CMDIFrameWnd();
};

/////////////////////////////////////////////////////////////////////////////
// CMenu:

class CMenu : public CWnd {
public:
	CMenu();
	virtual ~CMenu();

	int CreateMenu(CWnd *parent);
	int AppendMenu(long flag,
		void* item_id, const char* item_str, GCallback cb=NULL);

	virtual void DestroyWindow();

	int is_menu_bar;
	GtkWidget *menu_shell;

// for garbage collection
	void GCmark();
};

/////////////////////////////////////////////////////////////////////////////
// CStatusBar:

class CStatusBar : public CWnd {
public:
	CStatusBar();
	virtual ~CStatusBar();

	int Create(const char *name,
		long style, long left, long top, long width, long height,
		CWnd *parent);

	virtual void DestroyWindow();

	void SetWindowText(const char *text);
};

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIChildWnd:

class ADvCMDIChildWnd : public CFrameWnd {
public:
	ADvCMDIChildWnd();
	virtual ~ADvCMDIChildWnd();

	int Create(const char *name,
		long style, long left, long top, long width, long height, CWnd *parent);
	int CreateEdit(const char *name,
		long style, long left, long top, long width, long height, CWnd *parent);
	virtual void OnDestroy();

	virtual void DestroyWindow();

	ADvCView	*_cview;
};

/////////////////////////////////////////////////////////////////////////////
// ADvCView:

class CDC;

class ADvCView : public CWnd {
public:
	ADvCView();
	virtual ~ADvCView();

	int Create(CWnd *parent, long width, long height);
	int CreateEdit(long style, CWnd *parent);

	virtual void DestroyWindow();

	AGmNode *_node; // node which represents window

	void OnDraw(CDC* pDC);
	int EventProcess();
	AGmNode* _event;

	AGmNode *RootClass() { return _root_class; }
	AGmNode *_root_class;

	ADvCEdit *_cedit;

	cairo_surface_t *surface;

// for garbage collection
	void GCmark();
};

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIFrameWnd:

class ADvCMDIFrameWnd : public CMDIFrameWnd {
public:
	ADvCMDIFrameWnd();
	virtual ~ADvCMDIFrameWnd();

	int Create(const char *name,
		long style, long left, long top, long width, long height,
		CWnd *parent);
	virtual void OnDestroy();

	virtual void DestroyWindow();

	virtual void SetMessageText(const char *text);

	void SetADvWindow(ADvWindow *w) { _adv_wnd = w; }

	long m_style;
	AGmNode *Menu() { return _menu; }
	void Menu(AGmNode* menu);

	AGmNode *_menu, *_root_class;
	ADvWindow *_adv_wnd;

// for garbage collection
	void GCmark();
};

/////////////////////////////////////////////////////////////////////////////
// ADvCFrameWnd:

class ADvCFrameWnd : public CFrameWnd {
public:
	ADvCFrameWnd();
	virtual ~ADvCFrameWnd();

	int Create(const char *name, long style,
		long left, long top, long width, long height,
		CWnd *parent);
	int CreateEdit(const char *name, long style,
		long left, long top, long width, long height,
		CWnd *parent);
	virtual void OnDestroy();

	virtual void DestroyWindow();

	virtual void SetMessageText(const char *text);

	void SetADvWindow(ADvWindow *w) { _adv_wnd = w; }

	long m_style;
	AGmNode *Menu() { return _menu; }
	void Menu(AGmNode* menu);
	ADvCView* _cview0;

	AGmNode *_menu, *_root_class;
	ADvWindow *_adv_wnd;

// for garbage collection
	void GCmark();
};
#endif	// _LINUX

#endif	/* _ADV_ADVCVW_H */

