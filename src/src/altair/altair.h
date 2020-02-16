/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef __AFX_ALTAIR_H__
#define __AFX_ALTAIR_H__

#include <altair/docvdef.h>

#ifdef	_WINDOWS

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class CAltairApp : public CWinApp
{
public:
	CAltairApp();

// override
	//{{AFX_VIRTUAL(CAltairApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// implementation
	COleTemplateServer m_server;

	//{{AFX_MSG(CAltairApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif	// _WINDOWS

#ifdef	_LINUX

class AGmString;

class CAltairApp : public CWinApp {
public:
	CAltairApp(int argc, char** argv);

	virtual int InitInstance();

	AGmString* m_lpCmdLine;
};
#endif

#endif	// __AFX_ALTAIR_H__

