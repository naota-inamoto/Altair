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
#include <gdm/matrix.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <gdm/vmkey.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/advcedit.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/ocx.h>
#include <dev/mainfrm.h>

#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_LINUX_DEBUG
//#define	_CWND_LEAK_CHECK
#ifdef	_CWND_LEAK_CHECK
static int cwnd_count = 0;
#endif
#endif

//#define GTK_DEBUG
//#define WND_DESTROY_DEBUG

AGmNode *EvalCallback(AGmNode *root_class, AGmNode *arg, AGmNode *obj, AGmNode* func) {
	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	AIpProcess *process = new AIpProcess(pm, root_class, arg, func, obj) ;
	try {
		while (process->Execute() == 0);

		if (process->IsException() && process->ReturnProcess() == NULL) {
			AGmNode *val = process->Val();
			if (val) {
				AGmNode *msg = AGm::dst_node(val, "msg");
				AGmNode *fstk = AGm::dst_node(val, "stack_frame");
				AGmNode *pos = AGm::dst_node(val, "pos");
				ADvWindow::Print("Exception not catched: ");
				if (AGm::IsString(msg)) ADvWindow::Print(msg->a.s.v->Str());
				ADvWindow::Print("\n");
				ADvWindow::Print("StackTrace:\n");
				process->StackTrace(fstk);
				ADvWindow::Print("SourceCode:\n");
				process->ErrorSource(fstk, pos);
			}
		}

	} catch (AlException *e) {
		AGm::MaxHeapNum(AGm::MaxHeapNum() + 1);

		char s[256];
		art_sprintf_2(s, 256, "FATAL:\n\terrorCode = %d\n\t%s\n", e->ErrorCode(), e->Message());
		ADvWindow::Print(s);
		ADvWindow::Print("StackTrace:\n");
		process->StackTrace();
		ADvWindow::Print("SourceCode:\n");
		process->ErrorSource();
		delete e;

#if defined(_WINDOWS) && defined(_MFC_VER)
	} catch (CMemoryException *e) {
		try {
			char s[256];
			art_strcpy(s, 256, "CRITLCAL(CMemoryException), application ends: message = ");
#if	_WINDOWS && _64BIT
			long long n = strlen(s);
#else
			long n = strlen(s);
#endif
			e->GetErrorMessage(&s[n], (UINT)(255 - n));
			art_strcat(s, 256, "\n");
			ADvWindow::Print(s);
			ADvWindow::Print("StackTrace:\n");
			process->StackTrace();
			ADvWindow::Print("SourceCode:\n");
			process->ErrorSource();
			e->ReportError();
			e->Delete();

			delete process;

			::ExitProcess((UINT)-1);

		} catch (CMemoryException *e2) {
			e2->Delete();
			delete process;

			char s[256];
			art_strcpy(s, 256, "CRITLCAL(CMemoryException), application ends: message = ");
#if	_WINDOWS && _64BIT
			long long n = strlen(s);
#else
			long n = strlen(s);
#endif
			e->GetErrorMessage(&s[n], (UINT)(255 - n));
			art_strcat(s, 256, "\n");
			ADvWindow::Print(s);
			e->ReportError();

			::ExitProcess((UINT)-1);
		}

	} catch (CException *e) {
		char s[256];
		art_strcpy(s, 256, "CRITLCAL(CException): message = ");
#if	_WINDOWS && _64BIT
		long long n = strlen(s);
#else
		long n = strlen(s);
#endif		
		e->GetErrorMessage(&s[n], (UINT)(255 - n));
		art_strcat(s, 256, "\n");
		ADvWindow::Print(s);
		ADvWindow::Print("StackTrace:\n");
		process->StackTrace();
		ADvWindow::Print("SourceCode:\n");
		process->ErrorSource();
		e->ReportError();
		e->Delete();

		delete process;
		return NULL;
#endif
	}

	AGmNode *r = AGm::Ref(AGm::List(process->Val(), AGm::Integer(process->IsException())));
	delete process;
	return r;
}

AGmNode *EvalCallbackV(AGmNode *root_class, AGmNode* arg, AGmNode *obj, AGmNode* func_dcl) {
	AGmNode *vtbl = AGm::dst_node2(func_dcl, AGm::NmVtbl());
	ASSERT(vtbl);
	AGmNode *class_id = obj->a.o.class_id;
	ASSERT(class_id);
	AGmNode *func = AGm::dst_node2(vtbl, class_id);
	if (func == NULL) {
		return NULL;
	}
	return EvalCallback(root_class, arg, obj, func);
}

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIChildWnd constructor/destructor etc.

ADvCMDIChildWnd::ADvCMDIChildWnd() {
#ifdef	_LINUX
	_cwnd_type |= ADV_WNDTYPE_ADVCMDICHILD;
	_cview = NULL;
#endif
}

ADvCMDIChildWnd::~ADvCMDIChildWnd() {
#ifdef	_LINUX
	if (_cview) {
		_cview->DestroyWindow();
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// ADvCView constructor/destructor etc.

ADvCView::ADvCView() {
	AGm::SetValue(NULL, AGM_VM_KEY_CVIEW, this);
	_node = NULL;
	_event = NULL;
	_destroying = 0;
#ifdef	_WINDOWS
	_callback_tbl = AGm::Ref(AGm::Cons(NULL));
#endif

	void *value;
	AGm::GetValue(NULL, AGM_VM_KEY_ROOT_CLASS, value);
	_root_class = AGm::Ref((AGmNode*)value);

#ifdef	_LINUX
	_cedit = NULL;
	surface = NULL;
#endif
}

ADvCView::~ADvCView() {
#ifdef	_WINDOWS
	AGm::Unref(_event, _callback_tbl, _root_class);
#endif
}

#ifdef	_WINDOWS
void ADvCView::OnDestroy() {
	_destroying = 1;

	void *value;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
	AGmNode* n = (AGmNode*)value;
	ADvWindow *aw;
	CFrameWnd *frm = GetParentFrame();
	if (!frm || !frm->IsKindOf(RUNTIME_CLASS(ADvCFrameWnd))) {
		if (n && (aw = n->a.w.v) != NULL && _root_class && aw->DestroyProg()) {
			//TRACE0("--- start Destroy callback ---\n");
			AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->DestroyProg()));
			//TRACE0("--- end Destroy callback ---\n");
		}
	}

	// make its variable NULL, if output window
	AGm::GetValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW, value);
	if (value && value == n) {
		AGm::RemoveValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW);
	}

	if (n) {
		AGm::RemoveValue(this, AGM_VM_KEY_WINDOW_NODE);
		ADvWindow *aw = n->a.w.v;
		if (aw) {
			n->a.w.v = NULL;
			aw->View(NULL);
			delete aw;
		}
		AGm::ReleaseNode(n);
	}
	CView::OnDestroy();
}
#endif

double ADvFilePrintScale = 4.0;

void ADvCView::OnDraw(CDC *pDC) {
	void *value;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
	AGmNode* n = (AGmNode*)value;
	ADvWindow *aw;
	if (n == NULL) return;
	if ((aw = n->a.w.v) == NULL) return;

	if (aw->Type() != ADV_WINDOW_NORMAL) return;

	if (aw->QuickDraw()) {
		aw->EndQuickDraw();
	}
	aw->PaintDC(pDC);
	aw->StartDraw();
#ifdef	_LINUX
	CRgn *rgn = aw->UpdateRgn();
	if (rgn) delete rgn;
	rgn = new CRgn();
	rgn->CreateRectRgn(0, 0, 10000, 10000);
	//GetUpdateRgn(rgn);
	aw->UpdateRgn(rgn);
#endif
#ifdef	_WINDOWS
	if (pDC->IsPrinting()) {
		CRgn *rgn = aw->UpdateRgn();
		if (rgn) delete rgn;
		rgn = new CRgn();
		rgn->CreateRectRgn(0, 0, 10000, 10000);
		aw->UpdateRgn(rgn);
		AGmMatrix *m = new AGmMatrix(3, 3);
		m->Scale(ADvFilePrintScale, ADvFilePrintScale);
		aw->PushTM(AGm::List(AGm::Matrix(m)));
	}
#endif

	ADvWndProc f = aw->WndDrawProc();
	if (f) {
		int result = (*f)(aw, NULL);
		if (result == 0 || result != -1) {
#ifdef	_WINDOWS
			if (pDC->IsPrinting()) {
				aw->PopTM();
			}
#endif
			aw->EndDraw();
			aw->PaintDC(NULL);
			return;
		}
	}

	if (_root_class && aw->DrawProg()) {
		//TRACE0("--- start Draw callback ---\n");
		AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->DrawProg()));
		//TRACE0("--- end Draw callback ---\n");
	}
#ifdef	_WINDOWS
	if (pDC->IsPrinting()) {
		aw->PopTM();
	}
#endif
	aw->EndDraw();
	aw->PaintDC(NULL);
}

int ADvCView::EventProcess() {
	AGmNode *n = _node;
	ADvWindow *aw;
	if (n == NULL || (aw = n->a.w.v) == NULL) {
		return 1;
	}
	ADvWndProc f = aw->WndPickProc();
	if (f) {
		int result = (*f)(aw, _event);
		if (result == 0) {
			return 0;
		}
		if (result != -1) {
			return result;
		}
	}
	switch (aw->Type()) {
	case ADV_WINDOW_NORMAL:
		{
			AGmNode **value;
			if (AIpProcess::SearchVar(aw->Arg(), AGm::NmEvent(), value) == 0) {
				AGm::Set(*value, _event);	
			} else {
				return 1;
			}
			if (_root_class && aw->EventProg()) {
				//TRACE0("--- start Event callback ---\n");
				AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->EventProg()));
				//TRACE0("--- end Event callback ---\n");
			}
			return 0;
		}
	case ADV_WINDOW_EDIT:
		{
			AGmNode **value;
			if (AIpProcess::SearchVar(aw->Arg(), AGm::NmEvent(), value) == 0) {
				AGm::Set(*value, _event);	
			} else {
				return 1;
			}
			if (_root_class && aw->EventProg()) {
				//TRACE0("--- start edit Event callback ---\n");
				AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->EventProg()));
				//TRACE0("--- end edit Event callback ---\n");
			}
			return 1;
		}
	default:
		return 1;
	}
}

void ADvCView::GCmark() {
	AGm::_GCmark(_root_class);
	AGm::_GCmark(_node);
	AGm::_GCmark(_event);
#ifdef	_WINDOWS
	AGm::_GCmark(_callback_tbl);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIFrameWnd constructor/destructor etc.

ADvCMDIFrameWnd::ADvCMDIFrameWnd() {
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIFrameWnd::ADvCMDIFrameWnd(): this = %lx\n", (long)this);
#endif
#endif
	_adv_wnd = NULL;

	ADvInitFrames();
	AGmNode *mdi_frames = AGm::dst_node(ADvFrames, AGm::UniqueStr("MDIFrame"));
	AGmNode *n;
	AGm::CreateArc(mdi_frames, n = AGm::PtrCell(this));
#ifdef	_WINDOWS
	void* tmpl;
	AGm::GetValue(NULL, AGM_VM_KEY_DOC_TMPL, tmpl);
	AGm::Set(n->a.p.t, AGm::PtrCell(tmpl));
	AGm::RemoveValue(NULL, AGM_VM_KEY_DOC_TMPL);
#endif
	m_style = ADvWndStyle;

	_menu = NULL;
	void *value;
	AGm::GetValue(NULL, AGM_VM_KEY_ROOT_CLASS, value);
	_root_class = AGm::Ref((AGmNode*)value);

#ifdef	_LINUX
	_cwnd_type |= ADV_WNDTYPE_ADVCMDIFRAME;
	_children = AGm::Ref(AGm::Cons(NULL));
#endif
}
ADvCMDIFrameWnd::~ADvCMDIFrameWnd() {
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIFrameWnd::~ADvCMDIFrameWnd(): this = %lx\n", (long)this);
#endif
#endif
	AGmNode *mdi_frames = AGm::dst_node(ADvFrames, AGm::UniqueStr("MDIFrame"));
	AGmLink* l;
	for (l = AGm::next_outlink(mdi_frames, NULL);
			l; l = AGm::next_outlink(mdi_frames, l)) {
		AGmNode *n = l->arc->dst;
		if (n->a.p.ptr == this) {
#ifdef	_WINDOWS
			AGm::CreateArc(AGm::DocTmpl(), n->a.p.t);
#endif
			l = l->prev;
			AGm::DeleteArc(l->next->arc);
			break;
		}
	}

#ifdef	_LINUX
	if (_cmenu) {
		_cmenu->DestroyWindow();
		_cmenu = NULL;
	}
	if (_cstatusbar) {
		_cstatusbar->DestroyWindow();
		_cstatusbar = NULL;
	}
	AGm::Unref(_children);
#endif
}

void ADvCMDIFrameWnd::OnDestroy() {
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIFrameWnd::OnDestroy(): this = %lx\n", (long)this);
#endif
#endif
#ifdef	_WINDOWS
		AIpProcess::DestroyMenu(_menu);
#endif
	AGm::Unref(_menu, _root_class);

	void *value;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
	AGmNode* n = (AGmNode*)value;
	if (n) {
		AGm::RemoveValue(this, AGM_VM_KEY_WINDOW_NODE);
		ADvWindow *aw = n->a.w.v;
		if (aw) {
			n->a.w.v = NULL;
			aw->Frame(NULL);
			delete aw;
		}
		AGm::ReleaseNode(n);
	}
#ifdef	_WINDOWS
	CMDIFrameWnd::OnDestroy();
#endif
#ifdef	_LINUX
	DestroyWindow();
#endif
}

void ADvCMDIFrameWnd::Menu(AGmNode* menu) {
	AGm::Ref(menu); AGm::Unref(_menu); _menu = menu;
}

void ADvCMDIFrameWnd::GCmark() {
	AGm::_GCmark(_menu);
	AGm::_GCmark(_root_class);

#ifdef	_LINUX
	AGm::_GCmark(_children);
	if (_cmenu) _cmenu->GCmark();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// ADvCFrameWnd constructor/destructor etc.

ADvCFrameWnd::ADvCFrameWnd() {
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCFrameWnd::ADvCFrameWnd(): this = %lx\n", (long)this);
#endif
#endif
	_cview0 = NULL;
	_adv_wnd = NULL;

	ADvInitFrames();
	AGmNode *sdi_frames = AGm::dst_node(ADvFrames,
			AGm::UniqueStr("SDIFrame"));
	AGmNode *n;
	AGm::CreateArc(sdi_frames, n = AGm::PtrCell(this));
#ifdef	_WINDOWS
	void* tmpl;
	AGm::GetValue(NULL, AGM_VM_KEY_DOC_TMPL, tmpl);
	AGm::Set(n->a.p.t, AGm::PtrCell(tmpl));
	AGm::RemoveValue(NULL, AGM_VM_KEY_DOC_TMPL);
#endif
	m_style = ADvWndStyle;

	_menu = NULL;
	void *value;
	AGm::GetValue(NULL, AGM_VM_KEY_ROOT_CLASS, value);
	_root_class = AGm::Ref((AGmNode*)value);

#ifdef	_LINUX
	_cwnd_type |= ADV_WNDTYPE_ADVCFRAME;
#endif
}
ADvCFrameWnd::~ADvCFrameWnd() {
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCFrameWnd::~ADvCFrameWnd(): this = %lx\n", (long)this);
#endif
#endif
	AGmNode *sdi_frames = AGm::dst_node(ADvFrames, AGm::UniqueStr("SDIFrame"));
	AGmLink* l;
	for (l = AGm::next_outlink(sdi_frames, NULL);
			l; l = AGm::next_outlink(sdi_frames, l)) {
		AGmNode *n = l->arc->dst;
		if (n->a.p.ptr == this) {
#ifdef	_WINDOWS
			AGm::CreateArc(AGm::DocTmpl(), n->a.p.t);
#endif
			l = l->prev;
			AGm::DeleteArc(l->next->arc);
			break;
		}
	}
}

void ADvCFrameWnd::OnDestroy() {
#ifdef	_LINUX
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCFrameWnd::OnDestroy(): this = %lx\n", (long)this);
#endif
#endif
#ifdef	_WINDOWS
		AIpProcess::DestroyMenu(_menu);
#endif
	AGm::Unref(_menu, _root_class);
#ifdef	_LINUX
	if (_cmenu) {
		_cmenu->DestroyWindow();
		_cmenu = NULL;
	}
	if (_cview) {
		_cview->DestroyWindow();
		_cview = NULL;
	}
	if (_cstatusbar) {
		_cstatusbar->DestroyWindow();
		_cstatusbar = NULL;
	}
#endif

#ifdef	_WINDOWS
	CFrameWnd::OnDestroy();
#endif
#ifdef	_LINUX
	DestroyWindow();
#endif
}

void ADvCFrameWnd::Menu(AGmNode* menu) {
	AGm::Ref(menu); AGm::Unref(_menu); _menu = menu;
}

void ADvCFrameWnd::GCmark() {
	AGm::_GCmark(_menu);
	AGm::_GCmark(_root_class);

#ifdef	_LINUX
	AGm::_GCmark(_children);
	if (_cmenu) _cmenu->GCmark();
#endif
}

#ifdef	_WINDOWS
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ADvCMDIChildWnd

IMPLEMENT_DYNCREATE(ADvCMDIChildWnd, CMDIChildWnd)

BOOL ADvCMDIChildWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (ADvWndStyle & 0x0001) {
		cs.style &= ~WS_SYSMENU;
	}
	if (ADvAutoLayout == 0) {
		cs.x = ADvX;
		cs.y = ADvY;
		cs.cx = ADvWidth;
		cs.cy = ADvHeight;
	}
	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIChildWnd check

#ifdef _DEBUG
void ADvCMDIChildWnd::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void ADvCMDIChildWnd::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif /* _DEBUG */

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIChildWnd message handler

void ADvCMDIChildWnd::OnUpdateFrameMenu(BOOL active, CWnd* activeWnd, HMENU hMenu) {
	CWnd *w = GetParentFrame();
	if (w && w->IsKindOf(RUNTIME_CLASS(ADvCMDIFrameWnd))) {
		if (((ADvCMDIFrameWnd*)w)->m_style & 0x00024)
			return;
	}
	CMDIChildWnd::OnUpdateFrameMenu(active, activeWnd, hMenu);
}

/////////////////////////////////////////////////////////////////////////////
// ADvCView

IMPLEMENT_DYNCREATE(ADvCView, CView)

BEGIN_MESSAGE_MAP(ADvCView, CView)
	//{{AFX_MSG_MAP(ADvCView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// standard print command
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

BOOL ADvCView::Create(LPCSTR className, LPCSTR windowName,
				DWORD style, const RECT& r, CWnd* parentWnd,
				UINT nID, CCreateContext* context) {
	style |= WS_CLIPCHILDREN;
	return CView::Create(className, windowName,
				style, r, parentWnd, nID, context);
}

int ADvCView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	DragAcceptFiles();
	return CView::OnCreate(lpCreateStruct);
}

void ADvCView::OnInitialUpdate() {
	int x = ADvX, y = ADvY, w = ADvWidth, h = ADvHeight;
	CWnd *ch = GetParentFrame();
	if (ADvAutoLayout) {
		int width, height;
		if (ch->IsKindOf(RUNTIME_CLASS(ADvCFrameWnd))) {
			width = 800; height = 600;
		} else {
			CWnd *fr = ch->GetParentFrame();
			if (fr == NULL) return;
			CRect r; fr->GetClientRect(&r);
			width = r.Width(); height = r.Height();
		}
		w = width * 4 / 7 ;
		h = height * 3 / 4;
		x += 40; y += 30;
		if (x + w > width) x = 0; if (y + h > height) y = 0;
		ADvX = x; ADvY = y; ADvWidth = w; ADvHeight = h;
	}
	if (ch) ch->MoveWindow(x, y, w, h);
}

/////////////////////////////////////////////////////////////////////////////
// ADvCView print

BOOL ADvCView::OnPreparePrinting(CPrintInfo *pInfo) {
	// default setting before print
	return DoPreparePrinting(pInfo);
}

void ADvCView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
}

void ADvCView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
}

/////////////////////////////////////////////////////////////////////////////
// ADvCView check

#ifdef _DEBUG
void ADvCView::AssertValid() const {
	CView::AssertValid();
}

void ADvCView::Dump(CDumpContext& dc) const {
	CView::Dump(dc);
}
#endif /* _DEBUG */

/////////////////////////////////////////////////////////////////////////////
// ADvCView message handler

LRESULT ADvCView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	void *value;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
	AGmNode* n = (AGmNode*)value;
	ADvWindow *aw;
	if (n == NULL || (aw = n->a.w.v) == NULL)	{
		return CView::WindowProc(message, wParam, lParam);
	}

	if (message == WM_LBUTTONDOWN) {
		SendMessageToDescendants(WM_ALTAIR+0);
	}
	
	if (message == WM_PAINT) {
		if (_root_class && aw->MoveCtrlProg()) {
			//TRACE0("--- start MoveControl callback ---\n");
			AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->MoveCtrlProg()));
			//TRACE0("--- end MoveControl callback ---\n");
		}
	}

	switch (aw->Type()) {
	case ADV_WINDOW_NORMAL:
		if (message == WM_PAINT) { // get update area
			CRgn *rgn = aw->UpdateRgn();
			if (rgn) delete rgn;
			rgn = new CRgn();
			rgn->CreateRectRgn(0, 0, 1, 1);
			GetUpdateRgn(rgn);
			aw->UpdateRgn(rgn);
		}
		if (message == WM_LBUTTONDOWN) {
			SetFocus();
		}
		break;
	case ADV_WINDOW_EDIT:
		if (message == WM_SIZE) aw->FitSize();
		break;
	default:
		break;
	}

	_node = n;
	AGm::Ref(n); // in order to destroy window after EventProcess
	int flag = CheckEvent(message, wParam, lParam);
	if (flag == -1) return 0;
	if (flag == 1) {
		LRESULT result = CView::WindowProc(message, wParam, lParam);
		AGm::Unref(n); _node = NULL;
		return result;
	}
	if (EventProcess()) {
		if (!_destroying) {
			LRESULT result = CView::WindowProc(message, wParam, lParam);
			AGm::Unref(n); _node = NULL;
			return result;
		}
	}
	AGm::Unref(n); _node = NULL;
	return 0;
}

#define _SHIFT	( (GetKeyState(VK_SHIFT)   & 0x8000) ? MODIF_SHIFT	 : 0 )
#define _CTRL	( (GetKeyState(VK_CONTROL) & 0x8000) ? MODIF_CONTROL : 0 )
#define _ALT	( (GetKeyState(VK_MENU)    & 0x8000) ? MODIF_ALT	 : 0 )

int ADvCView::CheckEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	AGmNode *n = _node;
	ADvWindow *aw;
	if (n == NULL) return NULL;
	if ((aw = n->a.w.v) == NULL) return NULL;
	switch (aw->Type()) {
	case ADV_WINDOW_NORMAL:
	case ADV_WINDOW_EDIT: {
		long msg;
		long x = (short)LOWORD(lParam);
		long y = (short)HIWORD(lParam);
#if	_WINDOWS && _64BIT
		long long code = wParam;
#else
		long code = wParam;
#endif
		long modif = _SHIFT | _CTRL | _ALT;
		HDROP hDrop; POINT pt; int i, n, len; char path[256]; AGmNode *files = NULL;
		switch (message) {
		case WM_LBUTTONDOWN:
			msg = ADvLeftPress; 		break;
		case WM_LBUTTONUP:
			msg = ADvLeftRelease;		break;
		case WM_LBUTTONDBLCLK:
			msg = ADvLeftDoubleClick;	break;
		case WM_RBUTTONDOWN:
			msg = ADvRightPress;		break;
		case WM_RBUTTONUP:
			msg = ADvRightRelease;		break;
		case WM_RBUTTONDBLCLK:
			msg = ADvRightDoubleClick;	break;
		case WM_MOUSEMOVE:
			msg = ADvMouseMove; 		break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			msg = ADvKeyPress;		break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			msg = ADvKeyRelease;	break;
		case WM_CHAR:
			{
				msg = ADvKeyChar;
				int rep_cnt = lParam & 0xFFFF;
				int i;
				for (i = 0; i < rep_cnt; i++) {
					AGm::Set(_event, AGm::List(msg, x, y, code, modif));
					EventProcess();
				}
				return -1;
			}
		case WM_TIMER:
			msg = ADvTimerEvent;	break;
		case WM_DROPFILES:
			hDrop = (HDROP)wParam;
			DragQueryPoint(hDrop, &pt);
			x = pt.x; y = pt.y;
			n = DragQueryFile(hDrop, 0xFFFFFFFF, path, 255);
			files = AGm::Cons(NULL);
			for (i = 0; i < n; i++) {
				len = DragQueryFile(hDrop, i, path, 255);
				path[len] = 0;
				AGm::CreateArc(files, AGm::String(path), AGm::Integer(i+1));
			}
			DragFinish(hDrop);
			msg = ADvDropEvent; 	break;
		case WM_SIZE:
			msg = ADvSize;			break;
		default:	return 1;
		}
		if (files == NULL) {
			AGm::Set(_event, AGm::List(msg, x, y, code, modif));
		} else {
			AGm::Set(_event, AGm::List(AGm::Integer(msg), AGm::Integer(x), AGm::Integer(y), files, AGm::Integer(modif)));
		}
		return 0;
	}
	default:
		return 1;
	}
}

BOOL ADvCView::OnCmdMsg(UINT nID, int nCode,
		void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo) {
	for (;;) {
		if (nCode != CN_EVENT) break;
		AFX_EVENT *pEvent = (AFX_EVENT*)pExtra;
		if (pEvent->m_eventKind != AFX_EVENT::event) break;
		DISPID memid = pEvent->m_dispid;
		DISPPARAMS* params = pEvent->m_pDispParams;

		AGmNode *idx, *tbl, *info;
		idx = AGm::Ref(AGm::Integer((long)nID));
		tbl = AGm::dst_node(CallbackTbl(), idx);
		AGm::Unref(idx);
		if (tbl == NULL) return TRUE;
		idx = AGm::Ref(AGm::Integer((long)memid));
		info = AGm::dst_node(tbl, idx);
		AGm::Unref(idx);
		if (info == NULL) return TRUE;
		
		void *value;
		AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
		AGmNode* n = (AGmNode*)value;
		ADvWindow *aw;
		if (n && (aw = n->a.w.v) != NULL) {
			AGmNode *arg = AGm::Array(params->cArgs);
			for (unsigned i = 0; i < params->cArgs; i++) {
				AGm::Set(arg->a.a.v[i] ,ADvOcx::FromVariant(
					params->rgvarg[params->cArgs - i - 1]));
			}
			AGmNode *w_arg = aw->Arg();
			AGmNode *w_arg2;
			w_arg2 = AGm::Cons(AGm::List(AGm::NmO(), info->H, NULL), w_arg);
			w_arg2 = AGm::Cons(AGm::List(AGm::NmArg(), arg, NULL), w_arg2);
			aw->Arg(w_arg2);
			//TRACE0("--- start OCX callback ---\n");
			AGm::Unref(EvalCallbackV(_root_class, aw->Arg(), info->TH, info->TTH));
			//TRACE0("--- end OCX callback ---\n");
			aw->Arg(w_arg);
		}

		return TRUE;
	}
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

/////////////////////////////////////////////////////////////////////////////
// ADvCDocument

IMPLEMENT_DYNCREATE(ADvCDocument, CDocument)

/////////////////////////////////////////////////////////////////////////////
// ADvCDocument constructor/destuctor

ADvCDocument::ADvCDocument() {
}
ADvCDocument::~ADvCDocument() {
}

/////////////////////////////////////////////////////////////////////////////
// ADvCDocument check

#ifdef _DEBUG
void ADvCDocument::AssertValid() const {
	CDocument::AssertValid();
}
void ADvCDocument::Dump(CDumpContext& dc) const {
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// id array for control bar initialization
static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,			// status line indicator
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ADvCMDIFrameWnd

IMPLEMENT_DYNAMIC(ADvCMDIFrameWnd, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(ADvCMDIFrameWnd, CMDIFrameWnd)
	//{{AFX_MSG_MAP(ADvCMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL ADvCMDIFrameWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (m_style & 0x0001) {
		cs.style &= ~WS_SYSMENU;
	}
	if (m_style & 0x0002) {
		cs.style &= ~WS_CAPTION;
		cs.style |= WS_POPUP;
		cs.x = 0;
		cs.y = 0;
		cs.cx = 640;
		cs.cy = 400;
	}
	if (ADvAutoLayout == 0) {
		cs.x = ADvX;
		cs.y = ADvY;
		cs.cx = ADvWidth;
		cs.cy = ADvHeight;
	}
	return CMDIFrameWnd::PreCreateWindow(cs);
}

int ADvCMDIFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (m_style & 0x0004) {
		SetMenu(NULL);
	}
	if ((m_style & 0x0008) == 0) {
		CStatusBar *pStatusBar = new CStatusBar;
		if (!pStatusBar->Create(this) ||
			!pStatusBar->SetIndicators(indicators,
			sizeof(indicators)/sizeof(UINT)))
			return -1;
		pStatusBar->m_bAutoDelete = TRUE;
	}
	return 0;
}

void ADvCMDIFrameWnd::OnClose() {

	if (_root_class && _adv_wnd->DestroyProg()) {
		//TRACE0("--- start MDIFrameWnd Close callback ---\n");
		AGmNode *ret = EvalCallback(_root_class, _adv_wnd->Arg(), NULL, _adv_wnd->DestroyProg());
		//TRACE0("--- end MDIFrameWnd Close callback ---\n");
		int flag = (ret->H == NULL);
		AGm::Unref(ret);
		if (flag) return;
	}

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

		// pApp->CloseAllDocuments(FALSE); // commont out

		if (!AfxOleCanExitApp()) {
			AfxOleSetUserCtrl(FALSE);
			return;
		}
		//if (!afxContextIsDLL && pApp->m_pMainWnd == NULL) {
		//	AfxPostQuitMessage(0);
		//	return;
		//}
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

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIFrameWnd check

#ifdef _DEBUG
void ADvCMDIFrameWnd::AssertValid() const {
	CMDIFrameWnd::AssertValid();
}
void ADvCMDIFrameWnd::Dump(CDumpContext& dc) const {
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIFrameWnd message handler

BOOL ADvCMDIFrameWnd::OnCommand(WPARAM w, LPARAM l) {
	UINT nID = (UINT)LOWORD(w);
	AGmNode *n = Menu();
	if (n) {
		AGmNode *n2 = AIpProcess::SearchMenu(n, nID);
		if (n2) {
			//TRACE0("--- start MDIFrameWnd Menu callback ---\n");
			AGm::Unref(EvalCallbackV(_root_class, NULL, n2->TTH, n2->TTTH));
			//TRACE0("--- end MDIFrameWnd Menu callback ---\n");
			return TRUE;
		}
	}
	return CMDIFrameWnd::OnCommand(w, l);
}

void ADvCMDIFrameWnd::OnUpdateFrameMenu(HMENU hMenu) {
	if (m_style & 0x0004)
		return;
	CMDIFrameWnd::OnUpdateFrameMenu(hMenu);
}
void ADvCMDIFrameWnd::OnUpdateFrameTitle(BOOL bAddToTitle) {
	if (m_style & 0x0010)
		return;
	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
}

void ADvCMDIFrameWnd::OnActivate(UINT nState, CWnd*, BOOL) {
	if (nState == WA_INACTIVE) return;
	if (_adv_wnd && _root_class && _adv_wnd->ActivateProg()) {
		//TRACE0("--- start MDIFrameWnd Activate callback ---\n");
		AGm::Unref(EvalCallback(_root_class, _adv_wnd->Arg(), NULL, _adv_wnd->ActivateProg()));
		//TRACE0("--- end MDIFrameWnd Activate callback ---\n");
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ADvCFrameWnd

IMPLEMENT_DYNCREATE(ADvCFrameWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(ADvCFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(ADvCFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL ADvCFrameWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (m_style & 0x0001) {
		cs.style &= ~WS_SYSMENU;
	}
	if (m_style & 0x0002) {
		cs.style &= ~WS_CAPTION;
		cs.style |= WS_POPUP;
		cs.x = 0;
		cs.y = 0;
		cs.cx = 640;
		cs.cy = 400;
	}
	if (ADvAutoLayout == 0) {
		cs.x = ADvX;
		cs.y = ADvY;
		cs.cx = ADvWidth;
		cs.cy = ADvHeight;
	}
	return CFrameWnd::PreCreateWindow(cs);
}

int ADvCFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (m_style & 0x0004) {
		SetMenu(NULL);
	}
	if ((m_style & 0x0008) == 0) {
		CStatusBar *pStatusBar = new CStatusBar;
		if (!pStatusBar->Create(this) ||
			!pStatusBar->SetIndicators(indicators,
			sizeof(indicators)/sizeof(UINT)))
			return -1;
		pStatusBar->m_bAutoDelete = TRUE;
	}
	return 0;
}

void ADvCFrameWnd::OnClose() {

	if (_root_class && _adv_wnd->DestroyProg()) {
		//TRACE0("--- start SDIFrameWnd Close callback ---\n");
		AGmNode *ret = EvalCallback(_root_class, _adv_wnd->Arg(), NULL, _adv_wnd->DestroyProg());
		//TRACE0("--- end SDIFrameWnd Close callback ---\n");
		int flag = (ret->H == NULL);
		AGm::Unref(ret);
		if (flag) return;
	}

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

		// pApp->CloseAllDocuments(FALSE); // commont out

		if (!AfxOleCanExitApp()) {
			AfxOleSetUserCtrl(FALSE);
			return;
		}
		//if (!afxContextIsDLL && pApp->m_pMainWnd == NULL) {
		//	AfxPostQuitMessage(0);
		//	return;
		//}
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

/////////////////////////////////////////////////////////////////////////////
// ADvCFrameWnd check

#ifdef _DEBUG
void ADvCFrameWnd::AssertValid() const {
	CFrameWnd::AssertValid();
}
void ADvCFrameWnd::Dump(CDumpContext& dc) const {
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ADvCFrameWnd message handler

BOOL ADvCFrameWnd::OnCommand(WPARAM w, LPARAM l) {
	UINT nID = (UINT)LOWORD(w);
	AGmNode *n = Menu();
	if (n) {
		AGmNode *n2 = AIpProcess::SearchMenu(n, nID);
		if (n2) {
			//TRACE0("--- start SDIFrameWnd Menu callback ---\n");
			AGm::Unref(EvalCallbackV(_root_class, NULL, n2->TTH, n2->TTTH));
			//TRACE0("--- end SDIFrameWnd Menu callback ---\n");
			return TRUE;
		}
	}
	return CFrameWnd::OnCommand(w, l);
}

void ADvCFrameWnd::OnUpdateFrameMenu(HMENU hMenu) {
	if (m_style & 0x0004)
		return;
	CFrameWnd::OnUpdateFrameMenu(hMenu);
}

void ADvCFrameWnd::OnActivate(UINT nState, CWnd*, BOOL) {
	if (nState == WA_INACTIVE) return;
	if (_adv_wnd && _root_class && _adv_wnd->ActivateProg()) {
		//TRACE0("--- start SDIFrameWnd Activate callback ---\n");
		AGm::Unref(EvalCallback(_root_class, _adv_wnd->Arg(), NULL, _adv_wnd->ActivateProg()));
		//TRACE0("--- end SDIFrameWnd Activate callback ---\n");
	}
}

#endif	// _WINDOWS

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef	_LINUX

/////////////////////////////////////////////////////////////////////////////
// CWnd:

CWnd::CWnd() {
#ifdef	WND_DESTROY_DEBUG
	printf("CWnd::CWnd(): this = %lx\n", (long)this);
#endif
	_cwnd_type = 0;
	_parent = NULL;
	m_pToplevelWidget = NULL;
	m_pWidget = NULL;
	_destroying = 0;
	_children = NULL;
	_capture = 0;
#ifdef	_CWND_LEAK_CHECK
	TRACE1(AL_STR("CWnd created. total count = %d\n"), ++cwnd_count);
#endif
}
CWnd::~CWnd() {
#ifdef	WND_DESTROY_DEBUG
	printf("CWnd::~CWnd(): this = %lx\n", (long)this);
#endif
#ifdef	_CWND_LEAK_CHECK
	TRACE1(AL_STR("CWnd destroyed. total count = %d\n"), --cwnd_count);
#endif
}

void CWnd::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("CWnd::DestroyWindow(): this = %lx\n", (long)this);
#endif
	if (_capture) {
		if (m_pToplevelWidget) {
			gtk_grab_remove(m_pToplevelWidget);
		} else if (m_pWidget) {
			gtk_grab_remove(m_pWidget);
		}
	}

	if ((_cwnd_type & 0x1f) != 0 && m_pToplevelWidget) {
#ifdef	WND_DESTROY_DEBUG
	printf("gtk_widget_destroy(m_pToplevelWidget): this = %lx\n", (long)m_pToplevelWidget);
#endif
		gtk_widget_destroy(m_pToplevelWidget);
		m_pToplevelWidget = NULL;
	}
	if (_parent && _parent->_children) {
		AGmLink *l;
		for (l = AGm::next_outlink(_parent->_children, NULL);
				l; l = AGm::next_outlink(_parent->_children, l)) {
			if (l->arc->dst->a.p.ptr == this) break;
		}
		if (l) {
			AGm::DeleteArc(l->arc);
		}
	}
	delete this;
}

void CWnd::SetCapture() {
	if (_capture) return;
	if (m_pToplevelWidget) {
		gtk_grab_add(m_pToplevelWidget);
	} else if (m_pWidget) {
		gtk_grab_add(m_pWidget);
	}
	_capture = 1;
}
void CWnd::ReleaseCapture() {
	if (_capture == 0) return;
	if (m_pToplevelWidget) {
		gtk_grab_remove(m_pToplevelWidget);
	} else if (m_pWidget) {
		gtk_grab_remove(m_pWidget);
	}
	_capture = 0;
}

void CWnd::InvalidateRect(GdkRectangle* area) {
	if (_cwnd_type == ADV_WNDTYPE_ADVCVIEW) {
		ADvCView *cview = (ADvCView*)this;

		GtkAllocation allocation;
		if (cview->surface) {
			cairo_surface_destroy(cview->surface);
		}
		gtk_widget_get_allocation(cview->m_pWidget, &allocation);
		cview->surface = gdk_window_create_similar_surface(
			gtk_widget_get_window(cview->m_pWidget), CAIRO_CONTENT_COLOR,
			allocation.width, allocation.height);

		CDC *pDC = new CDC(cview);
		pDC->FillBackground();
		cview->OnDraw(pDC);
		delete pDC;

		if (area) {
			gdk_window_invalidate_rect(gtk_widget_get_window(cview->m_pWidget), area, FALSE);
		} else {
			GtkAllocation allocation;
			gtk_widget_get_allocation(cview->m_pWidget, &allocation);
			GdkRectangle rect;
			rect.x = 0;
			rect.y = 0;
			rect.width = allocation.width;
			rect.height = allocation.height;
			gdk_window_invalidate_rect(gtk_widget_get_window(cview->m_pWidget), &rect, FALSE);
		}
	}
}

CFrameWnd *CWnd::GetParentFrame() {
	CWnd *w;
	for (w = this; w; w = w->_parent) {
		if (w->_cwnd_type & ADV_WNDTYPE_CFRAME)
			return (CFrameWnd*)w;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd:

CFrameWnd::CFrameWnd() {
#ifdef	WND_DESTROY_DEBUG
	printf("CFrameWnd::CFrameWnd(): this = %lx\n", (long)this);
#endif
	_cwnd_type |= ADV_WNDTYPE_CFRAME;
	_visible = _actual_visible = 0;
	_cmenu = NULL;
	_cview = NULL;
	_cstatusbar = NULL;
	_resize_timer_id = 0;
}
CFrameWnd::~CFrameWnd() {
#ifdef	WND_DESTROY_DEBUG
	printf("CFrameWnd::~CFrameWnd(): this = %lx\n", (long)this);
#endif
}

void CFrameWnd::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("CFrameWnd::DestroyWindow(): this = %lx\n", (long)this);
#endif
	if (_resize_timer_id) {
		g_source_remove(_resize_timer_id);
		_resize_timer_id = 0;
	}
	CWnd::DestroyWindow();
}

int CFrameWnd::ShowWindow(int show, int show_child) {
	if ((_cwnd_type & ADV_WNDTYPE_CFRAME) == 0) return 0;
	CFrameWnd * wnd = (CFrameWnd*)_parent;
	AGmLink *l;
	switch (show) {
	case SW_SHOW:
		if (show_child == 0) _visible = 1;
		if (wnd == NULL) {
			_actual_visible = 1;
		} else {
			if (wnd->_actual_visible && _visible) {
				_actual_visible = 1;
			} else {
				_actual_visible = 0;
			}
		}
		if (_actual_visible && m_pToplevelWidget) {
			gtk_widget_show(m_pToplevelWidget);
		}
		if (_children) for (l = AGm::next_outlink(_children, NULL);
				l; l = AGm::next_outlink(_children, l)) {
			CFrameWnd *wnd = (CFrameWnd*)l->arc->dst->a.p.ptr;
			wnd->ShowWindow(SW_SHOW, 1);
		}
		break;
	case SW_HIDE:
		if (show_child == 0) _visible = 0;
		if (wnd == NULL) {
			_actual_visible = 0;
		} else {
			if (wnd->_actual_visible && _visible) {
				_actual_visible = 1;
			} else {
				_actual_visible = 0;
			}
		}
		if (_actual_visible == 0 && m_pToplevelWidget) {
			gtk_widget_hide(m_pToplevelWidget);
		}
		if (_children) for (l = AGm::next_outlink(_children, NULL);
				l; l = AGm::next_outlink(_children, l)) {
			CFrameWnd *wnd = (CFrameWnd*)l->arc->dst->a.p.ptr;
			wnd->ShowWindow(SW_HIDE, 1);
		}
		break;
	case SW_RESTORE:
		// TODO_LINUX restore (from icom)
		break;
	}
	return 1;
}

int CFrameWnd::IsIconic() {
	// TODO_LINUX IsIconic
	return 0;
}
void CFrameWnd::BringWindowToTop() {
	if ((_cwnd_type & 0x1f) != 0) {
		gdk_window_raise(gtk_widget_get_window(m_pToplevelWidget));
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMDIFrameWnd:

CMDIFrameWnd::CMDIFrameWnd() {
#ifdef	WND_DESTROY_DEBUG
	printf("CMDIFrameWnd::CMDIFrameWnd(): this = %lx\n", (long)this);
#endif
}
CMDIFrameWnd::~CMDIFrameWnd() {
#ifdef	WND_DESTROY_DEBUG
	printf("CMDIFrameWnd::~CMDIFrameWnd(): this = %lx\n", (long)this);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CMenu:

CMenu::CMenu() {
#ifdef	WND_DESTROY_DEBUG
	printf("CMenu::CMenu(): this = %lx\n", (long)this);
#endif
	_children = AGm::Ref(AGm::Cons(NULL));
	is_menu_bar = 0;
}
CMenu::~CMenu() {
#ifdef	WND_DESTROY_DEBUG
	printf("CMenu::~CMenu(): this = %lx\n", (long)this);
#endif
	AGm::Unref(_children);
}

int CMenu::CreateMenu(CWnd *parent) {
	_parent = parent;

	is_menu_bar = 1;

	m_pWidget = gtk_menu_bar_new();
	menu_shell = m_pWidget;

	gtk_box_pack_start(GTK_BOX(_parent->m_pWidget), m_pWidget,
		FALSE, TRUE, 0);
	gtk_widget_show(m_pWidget);

	return 1;
}
int CMenu::AppendMenu(
		long flag, void* item_id, const char *item_str, GCallback cb) {
	if (item_str == NULL || strlen(item_str) == 0)
		return 0;

	gchar *c = g_locale_to_utf8(item_str, -1, NULL, NULL, NULL);
	GtkWidget *menuitem = gtk_menu_item_new_with_label(c);
	g_free(c);

	CMenu *popup;

	switch (flag) {
	case MF_STRING:
		g_signal_connect(GTK_MENU_ITEM(menuitem), "activate", G_CALLBACK(cb), (gpointer)item_id);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu_shell), menuitem);
		gtk_widget_show(menuitem);
		break;
	case MF_POPUP:
		gtk_menu_shell_append(GTK_MENU_SHELL(menu_shell), menuitem);
		gtk_widget_show(menuitem);

		popup = (CMenu*)item_id;
		popup->_parent = this;
		popup->m_pWidget = menuitem;
		popup->menu_shell = gtk_menu_new();
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), popup->menu_shell);
		AGm::CreateArc(_children, AGm::PtrCell(popup));
		break;
	default:
		return 0;
	}

	return 1;
}

void CMenu::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("CMenu::DestroyWindow(): this = %lx\n", (long)this);
#endif

	if (_destroying) return;
	_destroying = 1;
	for (;;) {
		AGmLink *l = AGm::next_outlink(_children, NULL);
		if (l == NULL) break;
		CMenu *w = (CMenu*)l->arc->dst->a.p.ptr;
		w->DestroyWindow();
	}
	CWnd::DestroyWindow();
}

void CMenu::GCmark() {
	AGm::_GCmark(_children);
	AGmLink *l;
	for (l = AGm::next_outlink(_children, NULL);
			l; l = AGm::next_outlink(_children, l)) {
		CMenu *w = (CMenu*)l->arc->dst->a.p.ptr;
		w->GCmark();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CStatusBar:

CStatusBar::CStatusBar() {
#ifdef	WND_DESTROY_DEBUG
	printf("CStatusBar::CStatusBar(): this = %lx\n", (long)this);
#endif
}
CStatusBar::~CStatusBar() {
#ifdef	WND_DESTROY_DEBUG
	printf("CStatusBar::~CStatusBar(): this = %lx\n", (long)this);
#endif
}

int CStatusBar::Create(const char *name,
		long style, long left, long top, long width, long height,
		CWnd *parent) {
#ifdef	WND_DESTROY_DEBUG
	printf("CStatusBar::Create(...): this = %lx\n", (long)this);
#endif
	_parent = parent;
	m_pWidget = gtk_label_new(name);
	ASSERT(m_pWidget != NULL);
	// ### TODO TODO TODO: gtk_misc_set_alignment(GTK_MISC(m_pWidget), 0.0/* left */, 1.0/* bottom */);
	gtk_container_add(GTK_CONTAINER(parent->m_pWidget), m_pWidget);
	gtk_widget_show(m_pWidget);
	return 1;
}

void CStatusBar::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("CStatusBar::DestroyWindow(): this = %lx\n", (long)this);
#endif
	if (_destroying) return;
	_destroying = 1;
	CWnd::DestroyWindow();
}

void CStatusBar::SetWindowText(const char *text) {
	gtk_label_set_label(GTK_LABEL(m_pWidget), text);
}

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIChildWnd

static gboolean delay_resize_cb(void *data) {
	ADvCFrameWnd *frame = (ADvCFrameWnd*)data;
	if (frame->_resize_timer_id == 0) return FALSE;
	int cnt = --(frame->_resize_timer_cnt);
	gtk_window_resize(GTK_WINDOW(frame->m_pToplevelWidget), frame->_width - cnt, frame->_height - cnt);
	if (cnt > 0) return TRUE;
	g_source_remove(frame->_resize_timer_id);
	frame->_resize_timer_id = 0;
	return FALSE;
}

static gint mdi_child_destroy(GtkWidget *w, ADvCMDIChildWnd *child) {
#ifdef	WND_DESTROY_DEBUG
	printf("mdi_child_destroy(...): child = %lx\n", (long)child);
#endif
	child->OnDestroy();
	return FALSE;
}

int ADvCMDIChildWnd::Create(const char *name,
		long style, long left, long top, long width, long height, CWnd *parent) {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIChildWnd::Create(...): this = %lx\n", (long)this);
#endif
	_parent = parent;
	_width = width; _height = height;

	// main window
	m_pToplevelWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gchar *c = g_locale_to_utf8(name, -1, NULL, NULL, NULL);	
	gtk_window_set_title(GTK_WINDOW(m_pToplevelWidget), c);
	g_free(c);
	gtk_window_move(GTK_WINDOW(m_pToplevelWidget), left, top);
	gtk_window_set_default_size(GTK_WINDOW(m_pToplevelWidget),
		width-DELAY_RESIZE_COUNT, height-DELAY_RESIZE_COUNT);
	g_signal_connect(GTK_WIDGET(m_pToplevelWidget), "destroy",
		G_CALLBACK(mdi_child_destroy), this);

	m_pWidget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_set_border_width(GTK_CONTAINER(m_pWidget), 0);
	gtk_container_add(GTK_CONTAINER(m_pToplevelWidget), m_pWidget);
	gtk_widget_show(m_pWidget);

	// cview
	_cview = new ADvCView;
	_cview->Create(this, width-DELAY_RESIZE_COUNT, height-DELAY_RESIZE_COUNT);
	
	AGm::CreateArc(parent->_children, AGm::PtrCell(this));

	ShowWindow(SW_SHOW);

	_resize_timer_id = g_timeout_add(DELAY_RESIZE_TIMEOUT, delay_resize_cb, (gpointer)this);
	_resize_timer_cnt = DELAY_RESIZE_COUNT;

	return 1;
}
int ADvCMDIChildWnd::CreateEdit(const char *name,
		long style, long left, long top, long width, long height, CWnd *parent) {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIChildWnd::CreateEdit(...): this = %lx\n", (long)this);
#endif
	_parent = parent;
	
	// main window
	m_pToplevelWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gchar *c = g_locale_to_utf8(name, -1, NULL, NULL, NULL);	
	gtk_window_set_title(GTK_WINDOW(m_pToplevelWidget), c);
	g_free(c);
	gtk_window_move(GTK_WINDOW(m_pToplevelWidget), left, top);
	gtk_window_set_default_size(GTK_WINDOW(m_pToplevelWidget), width, height);
	g_signal_connect(GTK_WIDGET(m_pToplevelWidget), "destroy",
		G_CALLBACK(mdi_child_destroy), this);

	// vbox
	m_pWidget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_set_border_width(GTK_CONTAINER(m_pWidget), 1);
	gtk_container_add(GTK_CONTAINER(m_pToplevelWidget), m_pWidget);
	gtk_widget_show(m_pWidget);

	_cview = new ADvCView;
	_cview->CreateEdit(style, this);

	AGm::CreateArc(parent->_children, AGm::PtrCell(this));

	ShowWindow(SW_SHOW);

	return 1;
}

void ADvCMDIChildWnd::OnDestroy() {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIChildWnd::OnDestroy(): this = %lx\n", (long)this);
#endif
	DestroyWindow();
}
void ADvCMDIChildWnd::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIChildWnd::DestroyWindow(): this = %lx\n", (long)this);
#endif
	if (_destroying) return;
	_destroying = 1;
	if (_cview) {
		_cview->DestroyWindow();
		_cview = NULL;
	}
	CFrameWnd::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// ADvCView

static gboolean cview_drag_motion (
			GtkWidget *widget, GdkDragContext *context,
			gint x, gint y, guint time, ADvCView *cvw) {
	gdk_drag_status(context, GDK_ACTION_COPY, time);
	return 0;
}

enum { TARGET_STRING };
static GtkTargetEntry target_table[] = {
  { AL_STR("STRING"),	  0, TARGET_STRING },
  { AL_STR("text/plain"), 0, TARGET_STRING }
};
static void cview_drag_data_received(
			GtkWidget *widget, GdkDragContext *context,
			gint x, gint y, GtkSelectionData   *data,
			guint info, guint time, ADvCView *cvw) {
#ifdef GTK_DEBUG
printf("DRAG_AND_DROP\n");
#endif
	if (gtk_selection_data_get_length(data) >= 0 &&
			gtk_selection_data_get_format(data) == 8) {
		void *value;
		AGm::GetValue(cvw, AGM_VM_KEY_WINDOW_NODE, value);
		AGmNode* n = (AGmNode*)value;
		if (AGm::IsWindow(n)) {
			gsize br, bw; GError *err = NULL;
			gchar *c = g_locale_from_utf8(
				(gchar*)gtk_selection_data_get_data(data),
				gtk_selection_data_get_length(data), &br, &bw, &err);
			if (err == NULL) {
				AGmString *name, *s = new AGmString(c); g_free(c);
				AGmFile *f = new AGmFile(); f->Create(s, AGM_FILE_STR_READ);
				AGmNode *files = AGm::Cons(NULL); int i = 0;
				for (;;) {
					f->MatchString("file://");
					if (f->ReadString(name)) break;
					AGm::CreateArc(files, AGm::String(name), AGm::Integer(++i));
				}
				delete f; delete s;
				cvw->_node = n;
				AGm::Ref(n); // in order to destroy window after EventProcess
				AGm::Set(cvw->_event, AGm::List(
					AGm::Integer(ADvDropEvent), AGm::Integer(x), AGm::Integer(y), files, AGm::zero));
				if (cvw->EventProcess()) {
					if (!cvw->_destroying) {
						AGm::Unref(n); cvw->_node = NULL; return;
					}
				}
				AGm::Unref(n); cvw->_node = NULL;
			} else {
				g_error_free(err);
			}
		}
	  gtk_drag_finish(context, TRUE, FALSE, time);
	  return;
	}
	gtk_drag_finish(context, FALSE, FALSE, time);
}

static gboolean view_draw(GtkWidget *w, cairo_t *cr, ADvCView *cvw) {
	if(cvw->surface == NULL) return FALSE;
	cairo_set_source_surface(cr, cvw->surface, 0, 0);
	cairo_paint(cr);
	return FALSE;
}

static gboolean view_event(GtkWidget *w, GdkEvent *e, ADvCView *cvw) {
	void *value;
	AGm::GetValue(cvw, AGM_VM_KEY_WINDOW_NODE, value);
	AGmNode* n = (AGmNode*)value;
	if (!AGm::IsWindow(n)) return FALSE;
	long msg = 0, x = 0, y = 0;
	long code = 0, modifier = 0;
	switch (e->type) {
	case GDK_BUTTON_PRESS: {
		GdkEventButton *evnt = (GdkEventButton*)e;
#ifdef GTK_DEBUG
printf("evnt->button = %d\n", evnt->button);
printf("evnt->state = %d\n", evnt->state);
#endif
		switch (evnt->button) {
		case 1: // left button
#ifdef GTK_DEBUG
printf("LEFT_BUTTON_PRESS\n");
#endif
			if (cvw->_cedit && (evnt->state & GDK_CONTROL_MASK) == 0) {
				GtkTextView *text_view = cvw->_cedit->text_view;
				GtkTextBuffer *text_buffer = cvw->_cedit->text_buffer;
				GtkTextIter start, end;
				gtk_text_buffer_get_iter_at_offset(text_buffer, &start, 0);
				gint x, y;
				GtkTextWindowType type = gtk_text_view_get_window_type(text_view, evnt->window);
				gtk_text_view_window_to_buffer_coords(
					text_view, type, (int)evnt->x, (int)evnt->y, &x, &y);
				gtk_text_view_get_iter_at_location(text_view, &end, x, y);
				gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
				gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
				long pos = strlen(c2); cvw->_cedit->SetSel(pos, pos);
				g_free(c2); g_free(c);
			}
			msg = ADvLeftPress; break;
		case 3: // right button
#ifdef GTK_DEBUG
printf("RIGHT_BUTTON_PRESS\n");
#endif
			msg = ADvRightPress; break;
		}
		x = (long)evnt->x;
		y = (long)evnt->y;
		if (evnt->state & GDK_SHIFT_MASK) {
			modifier |= MODIF_SHIFT;
		}
		if (evnt->state & GDK_CONTROL_MASK) {
			modifier |= MODIF_CONTROL;
		}
		if (evnt->state & GDK_MOD1_MASK) {
			modifier |= MODIF_ALT;
		}
	}	break;
	case GDK_BUTTON_RELEASE: {
		GdkEventButton *evnt = (GdkEventButton*)e;
		switch (evnt->button) {
		case 1: // left button
#ifdef GTK_DEBUG
printf("LEFT_BUTTON_RELEASE\n");
#endif
			if (cvw->_cedit && (evnt->state & GDK_CONTROL_MASK) == 0) {
				GtkTextView *text_view = cvw->_cedit->text_view;
				GtkTextBuffer *text_buffer = cvw->_cedit->text_buffer;
				GtkTextIter start, end;
				gtk_text_buffer_get_iter_at_offset(text_buffer, &start, 0);
				gint x, y;
				GtkTextWindowType type = gtk_text_view_get_window_type(text_view, evnt->window);
				gtk_text_view_window_to_buffer_coords(
					text_view, type, (int)evnt->x, (int)evnt->y, &x, &y);
				gtk_text_view_get_iter_at_location(text_view, &end, x, y);
				gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
				long pos, from, to;
				gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
				pos = strlen(c2); g_free(c2); g_free(c);
				cvw->_cedit->GetSel(from, to);
				if (to < pos) {
					cvw->_cedit->SetSel(to, pos);
				} else {
					cvw->_cedit->SetSel(pos, to);
				}
			}
			msg = ADvLeftRelease; break;
		case 3: // right button
#ifdef GTK_DEBUG
printf("RIGHT_BUTTON_RELEASE\n");
#endif
			msg = ADvRightRelease; break;
		}
		x = (long)evnt->x;
		y = (long)evnt->y;
		if (evnt->state & GDK_SHIFT_MASK) {
			modifier |= MODIF_SHIFT;
		}
		if (evnt->state & GDK_CONTROL_MASK) {
			modifier |= MODIF_CONTROL;
		}
		if (evnt->state & GDK_MOD1_MASK) {
			modifier |= MODIF_ALT;
		}
	}	break;
	case GDK_2BUTTON_PRESS: { // double click
		GdkEventButton *evnt = (GdkEventButton*)e;
		switch (evnt->button) {
		case 1: // left button
#ifdef GTK_DEBUG
printf("LEFT_DOUBLE_CLICK\n");
#endif
			msg = ADvLeftDoubleClick; break;
		case 3: // right button
#ifdef GTK_DEBUG
printf("RIGHT_DOUBLE_CLICK\n");
#endif
			msg = ADvRightDoubleClick; break;
		}
		x = (long)evnt->x;
		y = (long)evnt->y;
		if (evnt->state & GDK_SHIFT_MASK) {
			modifier |= MODIF_SHIFT;
		}
		if (evnt->state & GDK_CONTROL_MASK) {
			modifier |= MODIF_CONTROL;
		}
		if (evnt->state & GDK_MOD1_MASK) {
			modifier |= MODIF_ALT;
		}
	}	break;
	case GDK_MOTION_NOTIFY: {
#ifdef GTK_DEBUG
printf("MOUSE_MOVE\n");
#endif
		GdkEventMotion *evnt = (GdkEventMotion*)e;
		msg = ADvMouseMove;
		x = (long)evnt->x; y = (long)evnt->y;
		if (evnt->state & GDK_SHIFT_MASK) {
			modifier |= MODIF_SHIFT;
		}
		if (evnt->state & GDK_CONTROL_MASK) {
			modifier |= MODIF_CONTROL;
		}
		if (evnt->state & GDK_MOD1_MASK) {
			modifier |= MODIF_ALT;
		}
	}	break;
	case GDK_KEY_PRESS: {
#ifdef GTK_DEBUG
printf("KEY_PRESS\n");
#endif
		GdkEventKey *evnt = (GdkEventKey*)e;
		msg = ADvKeyPress;
		code = (evnt->keyval & 0xff);
		if (evnt->state & GDK_SHIFT_MASK) {
			modifier |= MODIF_SHIFT;
		}
		if (evnt->state & GDK_CONTROL_MASK) {
			modifier |= MODIF_CONTROL;
		}
		if (evnt->state & GDK_MOD1_MASK) {
			modifier |= MODIF_ALT;
		}
#ifdef	GTK_DEBUG
printf("code = %lx\n", code);
printf("modifier = %lx\n", modifier);
#endif
		if (cvw->_cedit) {
			if (code == 0xe1) { // shift
				cvw->_cedit->shift_code = 1;
			} else if (cvw->_cedit->shift_code && code == 0x20) { // scape
				if (cvw->_cedit->multi_byte_input_mode == 0) {
					cvw->_cedit->multi_byte_input_mode = 1;
				} else {
					cvw->_cedit->multi_byte_input_mode = 0;
				}
			}
			GtkTextBuffer *text_buffer = cvw->_cedit->text_buffer;
			GtkTextIter start, end;
			gtk_text_buffer_get_iter_at_offset(text_buffer, &start, 0);
			GtkTextMark *mark = gtk_text_buffer_get_mark(text_buffer, "insert");
			gtk_text_buffer_get_iter_at_mark(text_buffer, &end, mark);
			gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
			gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
			long pos = strlen(c2); cvw->_cedit->SetSel(pos, pos);
			g_free(c2); g_free(c);
		}
	}	break;
	case GDK_KEY_RELEASE: {
#ifdef GTK_DEBUG
printf("KEY_RELEASE\n");
#endif
		GdkEventKey *evnt = (GdkEventKey*)e;
		msg = ADvKeyRelease;
		code = (evnt->keyval & 0xff);
		if (evnt->state & GDK_SHIFT_MASK) {
			modifier |= MODIF_SHIFT;
		}
		if (evnt->state & GDK_CONTROL_MASK) {
			modifier |= MODIF_CONTROL;
		}
		if (evnt->state & GDK_MOD1_MASK) {
			modifier |= MODIF_ALT;
		}
		if (cvw->_cedit) {
			cvw->_cedit->shift_code = 0;
			GtkTextBuffer *text_buffer = cvw->_cedit->text_buffer;
			GtkTextIter start, end;
			gtk_text_buffer_get_iter_at_offset(text_buffer, &start, 0);
			GtkTextMark *mark = gtk_text_buffer_get_mark(text_buffer, "insert");
			gtk_text_buffer_get_iter_at_mark(text_buffer, &end, mark);
			gchar *c = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
			gchar *c2 = g_locale_from_utf8(c, -1, NULL, NULL, NULL);
			long pos = strlen(c2); cvw->_cedit->SetSel(pos, pos);
			g_free(c2); g_free(c);
		}
	}	break;
	case GDK_CONFIGURE: {
#ifdef GTK_DEBUG
printf("VIEW_RESIZE\n");
#endif
		if (cvw->_cedit == NULL) {
			GtkAllocation allocation;
			if (cvw->surface) {
				cairo_surface_destroy(cvw->surface);
			}
			gtk_widget_get_allocation(cvw->m_pWidget, &allocation);
			cvw->surface = gdk_window_create_similar_surface(
				gtk_widget_get_window(cvw->m_pWidget), CAIRO_CONTENT_COLOR,
				allocation.width, allocation.height);
			CDC *pDC = new CDC(cvw);
			pDC->FillBackground();
			cvw->OnDraw(pDC);
			delete pDC;
		}
		GdkEventExpose *evnt = (GdkEventExpose*)e;
		msg = ADvSize;
		x = evnt->area.width;
		y = evnt->area.height;
	}	break;
	default:
#ifdef GTK_DEBUG
printf("e->type = %d\n", e->type);
#endif
		return FALSE;
	}
	cvw->_node = n;
	AGm::Ref(n); // in order to destroy window after EventProcess
	AGm::Set(cvw->_event, AGm::List(msg, x, y, code, modifier));
	if (cvw->EventProcess()) {
		if (!cvw->_destroying) {
			AGm::Unref(n); cvw->_node = NULL;
			return FALSE;
		}
	}
	AGm::Unref(n); cvw->_node = NULL;
	return TRUE;
}

int ADvCView::Create(CWnd *parent, long width, long height) {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCView::Create(...): this = %lx\n", (long)this);
#endif
	_cwnd_type |= ADV_WNDTYPE_ADVCVIEW;
	_parent = parent;

	m_pWidget = gtk_drawing_area_new();
	gtk_widget_set_size_request(m_pWidget, width, height);

	gtk_box_pack_start(GTK_BOX(parent->m_pWidget), m_pWidget,
		TRUE, TRUE, 0);
	gtk_widget_show(m_pWidget);

	gtk_drag_dest_set(m_pWidget, GTK_DEST_DEFAULT_ALL,
		target_table, 2, (GdkDragAction)(GDK_ACTION_COPY | GDK_ACTION_MOVE));

	g_signal_connect(GTK_WIDGET(m_pWidget), "drag_motion",
		G_CALLBACK(cview_drag_motion), this);
	g_signal_connect(GTK_WIDGET(m_pWidget), "drag_data_received",
		G_CALLBACK(cview_drag_data_received), this);
	g_signal_connect(GTK_WIDGET(m_pWidget), "draw",
		G_CALLBACK (view_draw), this);
	g_signal_connect(GTK_WIDGET(m_pWidget), "configure-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(m_pWidget), "button-press-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(m_pWidget), "button-release-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(m_pWidget), "motion-notify-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(m_pWidget), "key-press-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(m_pWidget), "key-release-event",
		G_CALLBACK(view_event), this);

	gtk_widget_set_events(m_pWidget, GDK_ALL_EVENTS_MASK);
	
	gtk_widget_grab_focus(m_pWidget);

	return 1;
}

int ADvCView::CreateEdit(long style, CWnd *parent) {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCView::CreateEdit(...): this = %lx\n", (long)this);
#endif
	_parent = parent;

	// vbox
	m_pToplevelWidget = parent->m_pWidget;

	// text
	_cedit = new ADvCEdit();
	_cedit->Create(style, this);
	m_pWidget = _cedit->m_pWidget;

	g_signal_connect(GTK_WIDGET(_cedit->text_view), "configure-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(_cedit->text_view), "button-press-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(_cedit->text_view), "button-release-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(_cedit->text_view), "motion-notify-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(_cedit->text_view), "key-press-event",
		G_CALLBACK(view_event), this);
	g_signal_connect(GTK_WIDGET(_cedit->text_view), "key-release-event",
		G_CALLBACK(view_event), this);

	return 1;
}

void ADvCView::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCView::DestroyWindow(): this = %lx\n", (long)this);
#endif
	if (_destroying) return;
	_destroying = 1;

	void *value;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
	AGmNode* n = (AGmNode*)value;
	ADvWindow *aw;

	// make its variable NULL, if output window
	AGm::GetValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW, value);
	if (value && value == n) {
		AGm::RemoveValue(NULL, AGM_VM_KEY_OUTPUT_WINDOW);
	}

	if (n) {
		AGm::RemoveValue(this, AGM_VM_KEY_WINDOW_NODE);
		ADvWindow *aw = n->a.w.v;
		if (aw) {
			n->a.w.v = NULL;
			aw->View(NULL);
			delete aw;
		}
		AGm::ReleaseNode(n);
	}

	AGm::Unref(_event, _root_class);
	if (_cedit) {
		_cedit->DestroyWindow();
		_cedit = NULL;
	}

	if (surface) {
		cairo_surface_destroy(surface);
		surface = NULL;
	}
	CWnd::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// ADvCMDIFrameWnd

static gint mdi_frame_delete(GtkWidget*, GdkEvent*, ADvCMDIFrameWnd *mdi_frame) {
	AGmNode *rc = mdi_frame->_root_class;
	ADvWindow *aw = mdi_frame->_adv_wnd;
	if (rc && aw->DestroyProg()) {
		//TRACE0("--- start MDIFrameWnd Destroy callback ---\n");
		AGmNode *ret = EvalCallback(rc, aw->Arg(), NULL, aw->DestroyProg());
		//TRACE0("--- end MDIFrameWnd Destroy callback ---\n");
		int flag = (ret->H == NULL);
		AGm::Unref(ret);
		if (flag) return TRUE;
	}
	return FALSE;
}
static gint mdi_frame_destroy(GtkWidget*, ADvCMDIFrameWnd *mdi_frame) {
#ifdef	WND_DESTROY_DEBUG
	printf("mdi_frame_destroy(...): mdi_frame = %lx\n", (long)mdi_frame);
#endif
	mdi_frame->OnDestroy();
	return FALSE;
}

int ADvCMDIFrameWnd::Create(const char *name,
		long style, long left, long top, long width, long height, CWnd *parent) {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIFrameWnd::Create(...): this = %lx\n", (long)this);
#endif
	_parent = parent;
	_width = width; _height = height;

	// main window
	m_pToplevelWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gchar *c = g_locale_to_utf8(name, -1, NULL, NULL, NULL);	
	gtk_window_set_title(GTK_WINDOW(m_pToplevelWidget), c);
	g_free(c);
	gtk_window_move(GTK_WINDOW(m_pToplevelWidget), left, top);
	gtk_window_set_default_size(GTK_WINDOW(m_pToplevelWidget),
		width-DELAY_RESIZE_COUNT, height-DELAY_RESIZE_COUNT);
	g_signal_connect(GTK_WIDGET(m_pToplevelWidget), "delete_event",
		G_CALLBACK(mdi_frame_delete), this);
	g_signal_connect(GTK_WIDGET(m_pToplevelWidget), "destroy",
		G_CALLBACK(mdi_frame_destroy), this);

	m_pWidget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_set_border_width(GTK_CONTAINER(m_pWidget), 1);
	gtk_container_add(GTK_CONTAINER(m_pToplevelWidget), m_pWidget);
	gtk_widget_show(m_pWidget);

	// menu
	_cmenu = new CMenu;
	_cmenu->CreateMenu(this);

	// statusbar
	_cstatusbar = new CStatusBar;
	_cstatusbar->Create("Ready", 0, 0, 0, 0, 0, this);

	_resize_timer_id = g_timeout_add(DELAY_RESIZE_TIMEOUT, delay_resize_cb, (gpointer)this);
	_resize_timer_cnt = DELAY_RESIZE_COUNT;

	return 1;
}
void ADvCMDIFrameWnd::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCMDIFrameWnd::DestroyWindow(): this = %lx\n", (long)this);
#endif
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

void ADvCMDIFrameWnd::SetMessageText(const char *text) {
	if (_cstatusbar) {
		_cstatusbar->SetWindowText(text);
	}
}

/////////////////////////////////////////////////////////////////////////////
// ADvCFrameWnd

static gint sdi_frame_delete(GtkWidget*, GdkEvent*, ADvCFrameWnd *sdi_frame) {
	if (sdi_frame->_capture) return TRUE;

	AGmNode *rc = sdi_frame->_root_class;
	ADvWindow *aw = sdi_frame->_adv_wnd;
	if (rc && aw->DestroyProg()) {
		//TRACE0("--- start SDIFrameWnd Destroy callback ---\n");
		AGmNode *ret = EvalCallback(rc, aw->Arg(), NULL, aw->DestroyProg());
		//TRACE0("--- end SDIFrameWnd Destroy callback ---\n");
		int flag = (ret->H == NULL);
		AGm::Unref(ret);
		if (flag) return TRUE;
	}
	return FALSE;
}
static gint sdi_frame_destroy(GtkWidget*, ADvCFrameWnd *sdi_frame) {
#ifdef	WND_DESTROY_DEBUG
	printf("sdi_frame_destroy(...): sdi_frame = %lx\n", (long)sdi_frame);
#endif
	sdi_frame->OnDestroy();
	return FALSE;
}

int ADvCFrameWnd::Create(const char *name, long style,
		long left, long top, long width, long height, CWnd *parent) {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCFrameWnd::Create(...): this = %lx\n", (long)this);
#endif
	_parent = parent;
	_width = width; _height = height;

	m_pToplevelWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gchar *c = g_locale_to_utf8(name, -1, NULL, NULL, NULL);	
	gtk_window_set_title(GTK_WINDOW(m_pToplevelWidget), c);
	g_free(c);
	gtk_window_move(GTK_WINDOW(m_pToplevelWidget), left, top);
	gtk_window_set_default_size(GTK_WINDOW(m_pToplevelWidget),
		width-DELAY_RESIZE_COUNT, height-DELAY_RESIZE_COUNT);
	g_signal_connect(GTK_WIDGET(m_pToplevelWidget), "delete_event",
		G_CALLBACK(sdi_frame_delete), this);
	g_signal_connect(GTK_WIDGET(m_pToplevelWidget), "destroy",
		G_CALLBACK(sdi_frame_destroy), this);

	m_pWidget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_set_border_width(GTK_CONTAINER(m_pWidget), 1);
	gtk_container_add(GTK_CONTAINER(m_pToplevelWidget), m_pWidget);
	gtk_widget_show(m_pWidget);

	// menu
	_cmenu = new CMenu;
	_cmenu->CreateMenu(this);

	// cview
	_cview = new ADvCView;
	_cview->Create(this, width-DELAY_RESIZE_COUNT, height-DELAY_RESIZE_COUNT);
	
	// statusbar
	// _cstatusbar = new CStatusBar;
	// _cstatusbar->Create("Ready", 0, 0, 0, 0, 0, this);

	_resize_timer_id = g_timeout_add(DELAY_RESIZE_TIMEOUT, delay_resize_cb, (gpointer)this);
	_resize_timer_cnt = DELAY_RESIZE_COUNT;

	return 1;
}

int ADvCFrameWnd::CreateEdit(const char *name,
		long style, long left, long top, long width, long height, CWnd *parent) {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCFrameWnd::CreateEdit(...): this = %lx\n", (long)this);
#endif
	_parent = parent;
	
	// main window
	m_pToplevelWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gchar *c = g_locale_to_utf8(name, -1, NULL, NULL, NULL);	
	gtk_window_set_title(GTK_WINDOW(m_pToplevelWidget), c);
	g_free(c);
	gtk_window_move(GTK_WINDOW(m_pToplevelWidget), left, top);
	gtk_window_set_default_size(GTK_WINDOW(m_pToplevelWidget), width, height);
	g_signal_connect(GTK_WIDGET(m_pToplevelWidget), "delete_event",
		G_CALLBACK(sdi_frame_delete), this);
	g_signal_connect(GTK_WIDGET(m_pToplevelWidget), "destroy",
		G_CALLBACK(sdi_frame_destroy), this);

	m_pWidget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_set_border_width(GTK_CONTAINER(m_pWidget), 1);
	gtk_container_add(GTK_CONTAINER(m_pToplevelWidget), m_pWidget);
	gtk_widget_show(m_pWidget);

	// menu
	_cmenu = new CMenu;
	_cmenu->CreateMenu(this);

	// cview
	_cview = new ADvCView;
	_cview->CreateEdit(style, this);
	
	// statusbar
	// _cstatusbar = new CStatusBar;
	// _cstatusbar->Create("Ready", 0, 0, 0, 0, 0, this);

	return 1;
}

void ADvCFrameWnd::DestroyWindow() {
#ifdef	WND_DESTROY_DEBUG
	printf("ADvCFrameWnd::DestroyWindow(): this = %lx\n", (long)this);
#endif
	if (_destroying) return;
	_destroying = 1;
	if (_cmenu) {
		_cmenu->DestroyWindow();
		_cmenu = NULL;
	}
	if (_cview) {
		_cview->DestroyWindow();
		_cview = NULL;
	}
	if (_cstatusbar) {
		_cstatusbar->DestroyWindow();
		_cstatusbar = NULL;
	}
	CFrameWnd::DestroyWindow();
}

void ADvCFrameWnd::SetMessageText(const char *text) {
	if (_cstatusbar) {
		_cstatusbar->SetWindowText(text);
	}
}

static PangoFontDescription* default_font = NULL;
PangoFontDescription* get_default_font() {
	if (default_font) return default_font;
	PangoFontDescription* font = get_font_desc(DEFAULT_FONT_NAME);
	default_font = font;
	return font;
}

PangoFontDescription* get_font_desc(gchar* font_name) {
	return pango_font_description_from_string(font_name);
}

#endif	// _LINUX
