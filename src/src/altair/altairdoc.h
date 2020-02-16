/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef __AFX_ALTAIRDOC_H__
#define __AFX_ALTAIRDOC_H__

#ifdef	_WINDOWS

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAltairDoc : public CDocument
{
protected: // for serialize
	CAltairDoc();
	DECLARE_DYNCREATE(CAltairDoc)

// attribute
public:
	AGmString *m_dir;
	AGmString *m_name;

// operation
public:
	virtual BOOL SaveModified();

// override
	//{{AFX_VIRTUAL(CAltairDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// implememtation
public:
	virtual ~CAltairDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// generated message map functions
protected:
	//{{AFX_MSG(CAltairDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAltairDoc)
	afx_msg short Open(LPCTSTR dir, LPCTSTR name);
	afx_msg short MakeGraphLibrary(LPCTSTR className, LPCTSTR fileName);
	afx_msg short Run(LPCTSTR className, LPCTSTR methodName, LPCTSTR cmdLine, short sync);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif

#endif	// __AFX_ALTAIRDOC_H__
