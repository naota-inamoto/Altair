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
#include <gdm/vmkey.h>
#include <gdm/pict.h>
#include <gdm/error.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/ocx.h>
#include <dev/olectrl.h>
#include <cui/cui.h>

#include <sql/sql.h>

#include <net/socket.h>
#include <net/crypt.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_WINDOWS
static BOOL bInit;
BOOL ADvCOleControlModule::InitInstance() {
	bInit =COleControlModule::InitInstance();
	if (bInit) {
		AGm::RefMemorySystem();
		ADvSocket::Initialize();
		AfxEnableControlContainer();
	}
	return bInit;
}
int ADvCOleControlModule::ExitInstance() {
	if (bInit) {
		FinalizeGdiCache(0);
		AGmPNumMgr::Finalize();
		ADvSocket::Finalize();
		ADvSQLFinalize();
		AGm::UnrefMemorySystem();
	}
	return COleControlModule::ExitInstance();
}

IMPLEMENT_DYNAMIC(ADvCOleControl, COleControl)

BEGIN_MESSAGE_MAP(ADvCOleControl, COleControl)
	//{{AFX_MSG_MAP(ADvCOleControl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static int num_alctrl = 0;

ADvCOleControl::ADvCOleControl()
{
	num_alctrl++;
	
	_ole_wnd = new ADvWindow();
	_ole_wnd->CreateControl(this);
	_ole_wnd->Arg(AGm::Cons(AGm::List(AGm::NmEvent(), NULL, NULL)));
	_event = NULL;
	_method_tbl = AGm::Ref(AGm::Cons(NULL));
	_callback_tbl = AGm::Ref(AGm::Cons(NULL));
	_root_class = NULL;
}

ADvCOleControl::~ADvCOleControl()
{
	AGm::Unref(_event, _method_tbl, _callback_tbl, _root_class);
}

BOOL ADvCOleControl::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style |= WS_CLIPSIBLINGS;
	return COleControl::PreCreateWindow(cs);
}

int ADvCOleControl::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	DragAcceptFiles();
	return COleControl::OnCreate(lpCreateStruct);
}

void ADvCOleControl::OnDestroy() {
	if (--num_alctrl == 0)
		ADvDestroyFrames(0);

	void *value;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
	AGmNode* n = (AGmNode*)value;
	if (n) {
		AGm::RemoveValue(this, AGM_VM_KEY_WINDOW_NODE);
		ADvWindow *aw = n->a.w.v;
		if (aw) {
			n->a.w.v = NULL;
			aw->OleCtrl(NULL);
			delete aw;
		}
		AGm::ReleaseNode(n);
	}
	COleControl::OnDestroy();
}

void ADvCOleControl::OnDraw(
			CDC* pdc, const CRect& /*rcBounds*/, const CRect& /*rcInvalid*/)
{
	ADvWindow *aw = _ole_wnd;
	if (aw->QuickDraw()) {
		aw->EndQuickDraw();
	}
	if (_root_class && aw->DrawProg()) {
		aw->PaintDC(pdc);
		aw->StartDraw();
		AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->DrawProg()));
		aw->EndDraw();
		aw->PaintDC(NULL);
	}
}

#define _SHIFT	( (GetKeyState(VK_SHIFT)   & 0x8000) ? MODIF_SHIFT	 : 0 )
#define _CTRL	( (GetKeyState(VK_CONTROL) & 0x8000) ? MODIF_CONTROL : 0 )
#define _ALT	( (GetKeyState(VK_MENU)    & 0x8000) ? MODIF_ALT	 : 0 )

LRESULT ADvCOleControl::WindowProc(UINT msg, WPARAM w, LPARAM l) {
	if (msg == WM_LBUTTONDOWN) {
		SendMessageToDescendants(WM_ALTAIR+0);
	}
	
	ADvWindow *aw = _ole_wnd;
	if (msg == WM_PAINT) {
		if (_root_class && aw->MoveCtrlProg()) {
			AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->MoveCtrlProg()));
		}
	}

	switch (aw->Type()) {
	case ADV_WINDOW_OCX:
		if (msg == WM_PAINT) { // get update region
			CRgn *rgn = aw->UpdateRgn();
			if (rgn) delete rgn;
			rgn = new CRgn();
			rgn->CreateRectRgn(0, 0, 1, 1);
			GetUpdateRgn(rgn);
			aw->UpdateRgn(rgn);
		}
		break;
	default:
		break;
	}

	if (_root_class) {
		int flag = CheckEvent(msg, w, l);
		if (flag == -1) return 0;
		if (flag == 1) {
			return COleControl::WindowProc(msg, w, l);
		}
		if (EventProcess()) {
			return COleControl::WindowProc(msg, w, l);
		}
		return 0;
	}
	return COleControl::WindowProc(msg, w, l);
}
int ADvCOleControl::CheckEvent(UINT msg, WPARAM w, LPARAM l) {
	ADvWindow *aw = _ole_wnd;
	switch (aw->Type()) {
	case ADV_WINDOW_OCX: {
		long m;
		long x = (short)LOWORD(l);
		long y = (short)HIWORD(l);
		long code = (long)w;
		long modif = _SHIFT | _CTRL | _ALT;
		HDROP hDrop; POINT pt; int i, n, len; char path[256]; AGmNode *files = NULL;
		switch (msg) {
		case WM_LBUTTONDOWN:
			m = ADvLeftPress;			break;
		case WM_LBUTTONUP:
			m = ADvLeftRelease; 		break;
		case WM_LBUTTONDBLCLK:
			m = ADvLeftDoubleClick; 	break;
		case WM_RBUTTONDOWN:
			m = ADvRightPress;			break;
		case WM_RBUTTONUP:
			m = ADvRightRelease;		break;
		case WM_RBUTTONDBLCLK:
			m = ADvRightDoubleClick;	break;
		case WM_MOUSEMOVE:
			m = ADvMouseMove;			break;
		case WM_KEYDOWN:
			m = ADvKeyPress;		break;
		case WM_SYSKEYDOWN:
			m = ADvKeyPress;		break;
		case WM_KEYUP:
			m = ADvKeyRelease;		break;
		case WM_SYSKEYUP:
			m = ADvKeyRelease;		break;
		case WM_CHAR:
			{
				msg = ADvKeyChar;
				int rep_cnt = l & 0xFFFF;
				int i;
				for (i = 0; i < rep_cnt; i++) {
					AGm::Set(_event, AGm::List(msg, x, y, code, modif));
					EventProcess();
				}
				return -1;
			}
		case WM_TIMER:
			m = ADvTimerEvent;		break;
		case WM_DROPFILES:
			hDrop = (HDROP)w;
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
			m = ADvDropEvent;		break;
		case WM_SIZE:
			m = ADvSize;			break;
		default:	return 1;
		}
		if (files == NULL) {
			AGm::Set(_event, AGm::List(m, x, y, code, modif));
		} else {
			AGm::Set(_event, AGm::List(AGm::Integer(m), AGm::Integer(x), AGm::Integer(y), files, AGm::Integer(modif)));
		}
		return 0;
	}
	default:
		return 1;
	}
}
int ADvCOleControl::EventProcess() {
	ADvWindow *aw = _ole_wnd;
	switch (aw->Type()) {
	case ADV_WINDOW_OCX: {
		AGmNode **value;
		if (AIpProcess::SearchVar(aw->Arg(), AGm::NmEvent(), value) == 0) {
			AGm::Set(*value, _event);	
		} else {
			return 1;
		}
		if (_root_class && aw->EventProg()) {
			AGm::Unref(EvalCallback(_root_class, aw->Arg(), NULL, aw->EventProg()));
		}
		return 0;
	}
	default:
		return 1;
	}
}

BOOL ADvCOleControl::OnCmdMsg(UINT nID, int nCode,
		void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo) {
	for (;;) {
		if (nCode != CN_EVENT) break;
		AFX_EVENT *pEvent = (AFX_EVENT*)pExtra;
		if (pEvent->m_eventKind != AFX_EVENT::event) break;
		DISPID memid = pEvent->m_dispid;
		DISPPARAMS* params = pEvent->m_pDispParams;

		AGmNode *tbl = AGm::dst_node(CallbackTbl(), (long)nID);
		if (tbl == NULL) return TRUE;
		AGmNode *info = AGm::dst_node(tbl, (long)memid);
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
			AGm::Unref(EvalCallbackV(_root_class, aw->Arg(), info->TH, info->TTH));
			aw->Arg(w_arg);
		}

		return TRUE;
	}
	return COleControl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void ADvCOleControl::GCmark() {
	AGm::_GCmark(_event);
	AGm::_GCmark(_method_tbl);
	AGm::_GCmark(_callback_tbl);
	AGm::_GCmark(_root_class);
}

const GUID IID_IAGmNode = {
	0xcbe7bc04, 0x1f9e, 0x11d0,
	{ 0xb3, 0x4e, 0x44, 0x45, 0x53, 0x54, 0, 0 }
};

IAGmNode::IAGmNode() {
	m_refCount = 0;
	m_node = NULL;
}
IAGmNode::~IAGmNode() {
	SetNode(NULL);
}

STDMETHODIMP_(ULONG) IAGmNode::AddRef() {
	return ++m_refCount;
}

STDMETHODIMP_(ULONG) IAGmNode::Release() {
	if (--m_refCount == 0) {
		delete this;
		return 0;
	}
	return m_refCount;
}

STDMETHODIMP IAGmNode::QueryInterface(REFIID iid, LPVOID* ppvObj) {
	if (iid == IID_IUnknown || iid == IID_IAGmNode) {
		*ppvObj = this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP IAGmNode::GetNode(AGmNode** node) {
	if (node == NULL)
		return S_OK;
	if (m_node) {
		AGmNode *root = AGm::Root();
		AGmNode *id = AGm::UniqueStr("IAGmNode");
		AGmLink *l;
		for (l = AGm::next_outlink2(root, NULL, id);
				l; l = AGm::next_outlink2(root, l, id)) {
			if (l->arc->dst == m_node)
				break;
		}
		if (l == NULL)
			m_node = NULL;
	}
	*node = m_node;
	return NOERROR;
}

STDMETHODIMP IAGmNode::SetNode(AGmNode* node) {
	AGm::Ref(node);
	AGmNode *root = AGm::Root();
	if (m_node) {
		AGmNode *id = AGm::UniqueStr("IAGmNode");
		AGmLink *l;
		for (l = AGm::next_outlink2(root, NULL, id);
				l; l = AGm::next_outlink2(root, l, id)) {
			if (l->arc->dst == m_node)
				break;
		}
		if (l) {
			AGm::DeleteArc(l->arc);
		}
	}
	m_node = node;
	if (m_node) {
		AGm::CreateArc(root, m_node, AGm::UniqueStr("IAGmNode"));
	}
	AGm::Unref(node);
	return NOERROR;
}

IAGmNode* AGmNodeToVariant(AGmNode* n, VARIANT* v) {
	IAGmNode *pNode = new IAGmNode;
	pNode->SetNode(n);
	pNode->AddRef();
	V_VT(v) = VT_UNKNOWN;
	V_UNKNOWN(v) = pNode;
	return pNode;
}

AGmNode* VariantToAGmNode(const VARIANT* v, char* name1, char* name2) {
	AGmNode *n = NULL;
	char err[256];
	HRESULT hr;
	IUnknown *punk;
	IAGmNode *pNode;
	switch(V_VT(v)) {
	case VT_ERROR: // optional argument
		if (v->scode != DISP_E_PARAMNOTFOUND) {
			art_sprintf_2(err, 256, "%s: %s is not optional arg", name1, name2);
			AfxThrowOleDispatchException(1, err);
		}
		n = NULL;
		break;
	case VT_UNKNOWN:
			punk = V_UNKNOWN(v);
			hr = punk->QueryInterface(IID_IAGmNode, (void**)&pNode);
			if (FAILED(hr)) {
				art_sprintf_2(err, 256, "%s: %s is not IAGmNode", name1, name2);
				AfxThrowOleDispatchException(1, err);
			}
			pNode->GetNode(&n);
			punk->Release();
			break;
	case VT_VARIANT|VT_BYREF:
		VARIANT *var;
		var = V_VARIANTREF(v);
		switch (V_VT(var)) {
		case VT_UNKNOWN:
				punk = V_UNKNOWN(var);
				hr = punk->QueryInterface(IID_IAGmNode, (void**)&pNode);
				if (FAILED(hr)) {
					art_sprintf_2(err, 256, "%s: %s is not IAGmNode", name1, name2);
					AfxThrowOleDispatchException(1, err);
				}
				pNode->GetNode(&n);
				punk->Release();
				break;
		default:
			art_sprintf_2(err, 256, "%s: %s is not IUnknown", name1, name2);
			AfxThrowOleDispatchException(1, err);
		}
		break;
	default:
		art_sprintf_2(err, 256, "%s: %s is not IUnknown", name1, name2);
		AfxThrowOleDispatchException(1, err);
	}
	return n;
}
#endif	// _WINDOWS
