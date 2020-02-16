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
#include <gdm/error.h>
#include <gdm/vmkey.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/advcedit.h>
#include <dev/mainfrm.h>
#include <cui/cui.h>
#include <altair/strres.h>

#include <altair/resource.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

//#define	WND_DESTROY_DEBUG

extern "C" AL_EXT_API int open_url(const char*);

/////////////////////////////////////////////////////////////////////////////
// ADvCMainFrame constructor/destructor

ADvCMainFrame::ADvCMainFrame()
{
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMainFrame::ADvCMainFrame(): this = %lx\n", (long)this);
#endif
#endif
	_msg_callback_tbl = AGm::Ref(AGm::Cons(NULL));
	_wParam = NULL;
	_lParam = NULL;
	_unload_callback = NULL;
	_timer_callback_tbl = AGm::Ref(AGm::Cons(NULL));

#ifdef	_LINUX
	_registered_msg = AGm::Ref(AGm::Cons(NULL));

	_cwnd_type |= ADV_WNDTYPE_ADVCMAINFRAME;
	_cmenu = NULL;
	_cstatusbar = NULL;
	_children = AGm::Ref(AGm::Cons(NULL));
#endif
}

ADvCMainFrame::~ADvCMainFrame()
{
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMainFrame::~ADvCMainFrame(): this = %lx\n", (long)this);
#endif
#endif
	AGm::Unref(_msg_callback_tbl, _wParam, _lParam,
		_unload_callback, _timer_callback_tbl);

#ifdef	_LINUX
	AGm::Unref(_registered_msg);

	if (_cmenu) delete _cmenu;
	if (_cstatusbar) delete _cstatusbar;
	AGm::Unref(_children);
	ADvGetApp()->m_pMainWnd = NULL;
#endif
}

void ADvCMainFrame::OnDestroy() {
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMainFrame::OnDestroy(): this = %lx\n", (long)this);
#endif
#endif
	if (_unload_callback) {
		//TRACE0("--- start AltairFrameWnd Destroy callback ---\n");
		AGm::Unref(EvalCallbackV(_unload_callback->H,
			NULL, _unload_callback->TH, _unload_callback->TTH));
		//TRACE0("--- end AltairFrameWnd Destroy callback ---\n");
	}
	ADvExiting = 1;
#ifdef	_WINDOWS
	CMDIFrameWnd::OnDestroy();
#endif
#ifdef	_LINUX
	DestroyWindow();
#endif
}

void ADvCMainFrame::SetMsgCallback(long msg, AGmNode *root_obj_func) {
	AGmNode *idx = AGm::Ref(AGm::Integer((unsigned long)msg));
	AGm::set_dst_node(_msg_callback_tbl, idx, root_obj_func);
	AGm::Unref(idx);
}

void ADvCMainFrame::SetUnloadCallback(AGmNode *root_obj_func) {
	AGm::Set(_unload_callback, root_obj_func);
}

#ifdef	_LINUX
static gint main_frame_timer_func(gpointer id) {
	ADvCMainFrame *frw = (ADvCMainFrame*)ADvGetApp()->m_pMainWnd;
	AGmNode *root_obj_func = AGm::dst_node(frw->_timer_callback_tbl, (long)id);
	if (root_obj_func == NULL) {
		return FALSE;
	}
	//TRACE0("--- start Timer callback ---\n");
	AGm::Unref(EvalCallbackV(root_obj_func->H,
		NULL, root_obj_func->TH, root_obj_func->TTH));
	//TRACE0("--- end Timer callback ---\n");
	return TRUE;
}
#endif
unsigned int ADvCMainFrame::SetTimerCallback(long id, long period, AGmNode *root_obj_func) {
	unsigned int r = 0;
	AGmNode *idx = AGm::Ref(AGm::Integer(id));
#ifdef	_WINDOWS
	if (root_obj_func) {
		r = (unsigned int)SetTimer((unsigned int)id, (unsigned int)period, NULL);
	} else {
		r = (unsigned int)KillTimer((unsigned int)id);
	}
#endif
#ifdef	_LINUX
	if (root_obj_func) {
		gint handler_id = g_timeout_add(period, main_frame_timer_func, (gpointer)id);
		AGm::Set(root_obj_func->TTT, AGm::Cons(AGm::Integer((long)handler_id)));
	} else {
		AGmNode *n = AGm::dst_node(_timer_callback_tbl, idx);
		if (n) {
			gint handler_id = n->TTTHI;
			g_source_remove(handler_id);
		} else {
			r = 1;
		}
	}
#endif
	AGm::set_dst_node(_timer_callback_tbl, idx, root_obj_func);
	AGm::Unref(idx);
	return r;
}

void ADvCMainFrame::GCmark() {
	AGm::_GCmark(_msg_callback_tbl);
	AGm::_GCmark(_wParam);
	AGm::_GCmark(_lParam);
	AGm::_GCmark(_unload_callback);
	AGm::_GCmark(_timer_callback_tbl);

#ifdef	_LINUX
	AGm::_GCmark(_registered_msg);
	AGm::_GCmark(_children);
	if (_cmenu) _cmenu->GCmark();
#endif
}

#ifdef	_WINDOWS
/////////////////////////////////////////////////////////////////////////////
// ADvCMainFrame

IMPLEMENT_DYNAMIC(ADvCMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(ADvCMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(ADvCMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
	ON_COMMAND(ID_PROJ_WND, OnProjWnd)
	ON_COMMAND(ID_HELP_ONLINE_MANUAL2, OnOnlineManual)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// id array for control initialization

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,		// status line indicator
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

int ADvCMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.Create(this) ||
	//	!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
	//	!m_wndToolBar.SetButtons(buttons,
	//	  sizeof(buttons)/sizeof(UINT)))
	//{
	//	return -1;	// return value is -1, if creation is failed
	//}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		return -1;	// return value is -1, if creation is failed
	}

	return 0;
}

void ADvCMainFrame::OnClose() {
	ADvDestroyFrames();

	// copy and modify CFrameWnd::OnClose() MFC codes

	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this)) return;
	CDocument* pDocument = GetActiveDocument();
	if (pDocument != NULL && !pDocument->CanCloseFrame(this)) {
		return;
	}
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this) {
		if (pDocument == NULL && !pApp->SaveAllModified())
			return;
		pApp->HideApplication();

		//pApp->CloseAllDocuments(FALSE); // commont out

		if (!AfxOleCanExitApp()) {
			AfxOleSetUserCtrl(FALSE);
			return;
		}
		if (!afxContextIsDLL && pApp->m_pMainWnd == NULL) {
			AfxPostQuitMessage(0);
			return;
		}
	}
	if (pDocument != NULL && pDocument->m_bAutoDelete) {
		BOOL bOtherFrame = FALSE;
		POSITION pos = pDocument->GetFirstViewPosition();
		while (pos != NULL) {
			CView* pView = pDocument->GetNextView(pos);
			ASSERT_VALID(pView);
			if (pView->GetParentFrame() != this) {
				bOtherFrame = TRUE;
				break;
			}
		}
		if (!bOtherFrame) {
			pDocument->OnCloseDocument();
			return;
		}
		pDocument->PreCloseFrame(this);
	}
	DestroyWindow();
}

#ifdef	_64BIT
void ADvCMainFrame::OnTimer(UINT_PTR nIDEvent) {
	AGmNode *idx = AGm::Ref(AGm::Integer((unsigned long)nIDEvent));
#else
void ADvCMainFrame::OnTimer(UINT nIDEvent) {
	AGmNode *idx = AGm::Ref(AGm::Integer((long)nIDEvent));
#endif
	AGmNode *root_obj_func = AGm::dst_node(_timer_callback_tbl, idx);
	AGm::Unref(idx);
	if (root_obj_func == NULL) {
		CMDIFrameWnd::OnTimer(nIDEvent);
		return;
	}
	//TRACE0("--- start Timer callback ---\n");
	AGm::Unref(EvalCallbackV(root_obj_func->H,
		NULL, root_obj_func->TH, root_obj_func->TTH));
	//TRACE0("--- end Timer callback ---\n");
}

BOOL ADvCMainFrame::OnQueryEndSession() {
	ADvDestroyFrames();
	return TRUE;
}
void ADvCMainFrame::OnEndSession(BOOL b) {
	CMDIFrameWnd::OnEndSession(b);
}

LRESULT ADvCMainFrame::WindowProc(UINT msg, WPARAM w, LPARAM l) {
	if (msg == WM_USER + 211) {
		exitTimerFunc();
		return CMDIFrameWnd::WindowProc(msg, w, l);
	}
	AGmNode *idx = AGm::Ref(AGm::Integer((unsigned long)msg));
	AGmNode *root_obj_func = AGm::dst_node(_msg_callback_tbl, idx);
	AGm::Unref(idx);
	if (root_obj_func == NULL)
		return CMDIFrameWnd::WindowProc(msg, w, l);
	AGm::Unref(_wParam); _wParam = AGm::Ref(AGm::Integer(w));
	AGm::Unref(_lParam); _lParam = AGm::Ref(AGm::Integer(l));
	//TRACE0("--- start WndMsg callback ---\n");
	AGmNode *r = EvalCallbackV(root_obj_func->H,
		NULL, root_obj_func->TH, root_obj_func->TTH);
	//TRACE0("--- end WndMsg callback ---\n");
	int result = 0;
	if (AGm::IsInteger(r->H))
		result = (long)r->HI;
	AGm::Unref(r);
	return (LRESULT)result;
}

/////////////////////////////////////////////////////////////////////////////
// ADvCMainFrame check

#ifdef _DEBUG
void ADvCMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void ADvCMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ADvCMainFrame message handler

void ADvCMainFrame::OnProjWnd()
{
	AUiProgramMgr::OpenProjectEditor();
}

void ADvCMainFrame::OnOnlineManual() {
	open_url("http://www5f.biglobe.ne.jp/~inamoto/altair/");
}

void ADvCMainFrame::OnAppExit()
{
	AUiProgramMgr::Exit();
}
#endif

#ifdef	_LINUX

#define ID_EXIT 			1
#define ID_WINDOW			2
#define ID_STATUSBAR		3
#define	ID_ONLINE_MANUAL	4
#define ID_VERSION			5

static gint button_clicked(GtkWidget *w, GtkWidget *dlg) {
	gtk_widget_destroy(dlg);
	return FALSE;
}
static gint close_dlg(GtkWidget *w, GtkWidget *dlg) {
	gtk_main_quit();
	gtk_grab_remove(dlg);
	return FALSE;
}

static void main_menu_cb(GtkMenuItem* w, gpointer gdata) {
	long id = (long)gdata;
	switch (id) {
	case ID_EXIT:
		ADvGetApp()->ExitApplication();
		break;
	case ID_WINDOW:
		AUiProgramMgr::OpenProjectEditor();
		break;
	case ID_STATUSBAR:
		break;
	case ID_ONLINE_MANUAL:
		open_url("http://www5f.biglobe.ne.jp/~inamoto/altair/");
		break;
	case ID_VERSION:
		{
			GtkWidget *dlg = gtk_window_new(GTK_WINDOW_TOPLEVEL);
			g_signal_connect(GTK_WIDGET(dlg), "destroy",
				 G_CALLBACK(close_dlg), dlg);
			gtk_container_set_border_width(GTK_CONTAINER(dlg), 15);

			GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

			GtkWidget *label1 = gtk_label_new("Altair Version 0.44");
			gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
			gtk_widget_show(label1);

			GtkWidget *label2 = gtk_label_new("CopyRight (C) 1995-2020 Naota Inamoto");
			gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 0);
			gtk_widget_show(label2);

			GtkWidget *label3 = gtk_label_new("http://www5f.biglobe.ne.jp/~inamoto/altair/");
			gtk_box_pack_start(GTK_BOX(vbox), label3, FALSE, FALSE, 0);
			gtk_widget_show(label3);

			GtkWidget *label4 = gtk_label_new("mailto:altair1984jp@yahoo.co.jp");
			gtk_box_pack_start(GTK_BOX(vbox), label4, FALSE, FALSE, 0);
			gtk_widget_show(label4);

			GtkWidget *button = gtk_button_new_with_label("OK");
			gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
			g_signal_connect(GTK_BUTTON(button), "clicked",
				G_CALLBACK(button_clicked), dlg);
			gtk_widget_show(button);

			gtk_container_add(GTK_CONTAINER(dlg), vbox);
			gtk_widget_show(vbox);
			gtk_window_set_position(GTK_WINDOW(dlg), GTK_WIN_POS_CENTER);
			gtk_grab_add(dlg);
			gtk_widget_show(dlg);
			gtk_main();
		}
		break;
	}
}

static gint main_frame_destroy(GtkWidget *w, ADvCMainFrame *frame) {
#ifdef	WND_DESTROY_DEBUG
	printf("main_frame_destroy(...): frame = %lx\n", (long)frame);
#endif
	frame->OnDestroy();
	ADvGetApp()->ExitApplication();
	return FALSE;
}

int ADvCMainFrame::Create(const char *name,
		long style, long left, long top, long width, long height, CWnd *parent) {
	_parent = parent;
	
	// main window
	m_pToplevelWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gchar *c = g_locale_to_utf8(name, -1, NULL, NULL, NULL);	
	gtk_window_set_title(GTK_WINDOW(m_pToplevelWidget), c);
	g_free(c);
	gtk_window_move(GTK_WINDOW(m_pToplevelWidget), left, top);
	gtk_widget_set_size_request(m_pToplevelWidget, width, height);
	g_signal_connect(m_pToplevelWidget, "destroy",
		G_CALLBACK(main_frame_destroy), this);

	m_pWidget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(m_pToplevelWidget), m_pWidget);
	gtk_container_set_border_width(GTK_CONTAINER(m_pWidget), 1);
	if (ADvShowWnd) {
		gtk_widget_show(m_pWidget);

		// menu
		_cmenu = new CMenu;
		_cmenu->CreateMenu(this);
		CMenu *file    = new CMenu;
		CMenu *project = new CMenu;
		CMenu *view    = new CMenu;
		CMenu *window  = new CMenu;
		CMenu *help    = new CMenu;
		_cmenu->AppendMenu(MF_POPUP, file,	  AL_MM_FILE);
		_cmenu->AppendMenu(MF_POPUP, project, AL_MM_PROJECT);
		_cmenu->AppendMenu(MF_POPUP, view,	  AL_MM_VIEW);
		_cmenu->AppendMenu(MF_POPUP, window,  AL_MM_WINDOW);
		_cmenu->AppendMenu(MF_POPUP, help,	  AL_MM_HELP);
		file   ->AppendMenu(MF_STRING, (void*)ID_EXIT,	        AL_MM_EXIT,	   G_CALLBACK(main_menu_cb));
		project->AppendMenu(MF_STRING, (void*)ID_WINDOW,        AL_MM_WINDOW,  G_CALLBACK(main_menu_cb));
		help   ->AppendMenu(MF_STRING, (void*)ID_ONLINE_MANUAL, AL_MM_MANUAL,  G_CALLBACK(main_menu_cb));
		help   ->AppendMenu(MF_STRING, (void*)ID_VERSION,       AL_MM_VERSION, G_CALLBACK(main_menu_cb));

		// statusbar
		_cstatusbar = new CStatusBar;
		_cstatusbar->Create("Ready", 0, 0, 0, 0, 0, this);
	}

	return 1;
}
void ADvCMainFrame::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMainFrame::DestroyWindow(): this = %lx\n", (long)this);
#endif
	ADvDestroyFrames();
	if (_destroying) return;
	_destroying = 1;
	if (_cmenu) {
		_cmenu->DestroyWindow();
		_cmenu = NULL;
	}
	if (_cstatusbar) {
		_cstatusbar->DestroyWindow();
		_cstatusbar = NULL;
	}
	for (;;) {
		AGmLink *l = AGm::next_outlink(_children, NULL);
		if (l == NULL) break;
		ADvCMDIChildWnd *w = (ADvCMDIChildWnd*)l->arc->dst->a.p.ptr;
		w->DestroyWindow();
	}
	CMDIFrameWnd::DestroyWindow();
}

void ADvCMainFrame::SetMessageText(const char *text) {
	if (_cstatusbar) {
		_cstatusbar->SetWindowText(text);
	}
}

AGmNode *ADvCMainFrame::ProcessMsg(ADvMessage *msg) {
	AGmNode *idx = AGm::Ref(AGm::Integer(msg->msg));
	AGmNode *root_obj_func = AGm::dst_node(_msg_callback_tbl, idx);
	AGm::Unref(idx);
	if (root_obj_func == NULL) return NULL;
	AGm::Unref(_wParam); _wParam = AGm::Ref(AGm::Integer(msg->wParam));
	AGm::Unref(_lParam); _lParam = AGm::Ref(AGm::Integer(msg->lParam));
	//TRACE0("--- start WndMsg callback ---\n");
	AGmNode *ret = EvalCallbackV(root_obj_func->H,
		NULL, root_obj_func->TH, root_obj_func->TTH);
	//TRACE0("--- end WndMsg callback ---\n");
	AGmNode *val = AGm::Ref(ret->H);
	AGm::Unref(ret);
	return val;
}
#endif
