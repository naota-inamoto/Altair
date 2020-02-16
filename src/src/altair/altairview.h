/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef __AFX_ALTAIRVIEW_H__
#define __AFX_ALTAIRVIEW_H__

#ifdef	_WINDOWS

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAltairView : public CView
{
protected: // for serialize
	CAltairView();
	DECLARE_DYNCREATE(CAltairView)

// attribute
public:
	CAltairDoc* GetDocument();

// operation
public:

// override
	//{{AFX_VIRTUAL(CAltairView)
	public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// implementation
public:
	virtual ~CAltairView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// message map
protected:
	//{{AFX_MSG(CAltairView)
	afx_msg void OnProjWnd();
	afx_msg void OnOnlineManual();
	afx_msg void OnAppExit();
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline CAltairDoc* CAltairView::GetDocument()
   { return (CAltairDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif	//	_WINDOWS

#endif	// __AFX_ALTAIRVIEW_H__
