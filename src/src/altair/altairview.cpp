/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

// AltairView.cpp : class definition of CAltairView
//

#include "stdafx.h"
#include "altair.h"

#include "altairdoc.h"
#include "altairview.h"

#ifdef	_WINDOWS

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAltairView

IMPLEMENT_DYNCREATE(CAltairView, CView)

BEGIN_MESSAGE_MAP(CAltairView, CView)
	//{{AFX_MSG_MAP(CAltairView)
	ON_COMMAND(ID_PROJ_WND, OnProjWnd)
	ON_COMMAND(ID_HELP_ONLINE_MANUAL, OnOnlineManual)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
	// standard print command
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAltairView constructor/destructor

CAltairView::CAltairView()
{
}

CAltairView::~CAltairView()
{
}

BOOL CAltairView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAltairView draw

void CAltairView::OnDraw(CDC* pDC)
{
	ADvCView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CAltairView print

BOOL CAltairView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// prepare default print
	return DoPreparePrinting(pInfo);
}

void CAltairView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CAltairView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CAltairView check

#ifdef _DEBUG
void CAltairView::AssertValid() const
{
	CView::AssertValid();
}

void CAltairView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAltairDoc* CAltairView::GetDocument() // inilne if non-debug version
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAltairDoc)));
	return (CAltairDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAltairView message handler

void CAltairView::OnProjWnd() 
{
	AUiProgramMgr::OpenProjectEditor();
}

extern "C" AL_EXT_API int open_url(const char*);

void CAltairView::OnOnlineManual() {
	open_url("http://www5f.biglobe.ne.jp/~inamoto/altair/");
}

void CAltairView::OnAppExit() 
{
	AUiProgramMgr::Exit();
}

void CAltairView::OnFileClose() 
{
	void *value;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
	AGm::ReleaseNode((AGmNode*)value);
}

#endif	// _WINDOWS

