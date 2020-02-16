/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

// AltairDoc.cpp : class definition of CAltairDoc
//

#include "stdafx.h"
#include "altair.h"

#include "altairdoc.h"

#ifdef	_WINDOWS

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAltairDoc

IMPLEMENT_DYNCREATE(CAltairDoc, CDocument)

BEGIN_MESSAGE_MAP(CAltairDoc, CDocument)
	//{{AFX_MSG_MAP(CAltairDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAltairDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CAltairDoc)
	DISP_FUNCTION(CAltairDoc, "Open", Open, VT_I2, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAltairDoc, "MakeGraphLibrary", MakeGraphLibrary, VT_I2, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAltairDoc, "Run", Run, VT_I2, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

static const IID IID_IAltair =
{ 0x56ef30d3, 0x48f1, 0x11d2, { 0x86, 0xb0, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(CAltairDoc, CDocument)
	INTERFACE_PART(CAltairDoc, IID_IAltair, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAltairDoc constructor/destructor

CAltairDoc::CAltairDoc()
{
	EnableAutomation();

	AfxOleLockApp();

	m_dir = NULL;
	m_name = NULL;
}

CAltairDoc::~CAltairDoc()
{
	if (m_dir)
		delete m_dir;
	if (m_name)
		delete m_name;

	AfxOleUnlockApp();
}

BOOL CAltairDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL CAltairDoc::SaveModified()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAltairDoc serialization

void CAltairDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAltairDoc check

#ifdef _DEBUG
void CAltairDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAltairDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAltairDoc command

short CAltairDoc::Open(LPCTSTR dir, LPCTSTR name) 
{
	if (m_dir) delete m_dir;
	if (m_name) delete m_name;
	m_dir = new AGmString(dir);
	m_name = new AGmString(name);
	return (short)AUiProgramMgr::_Open(m_dir, m_name);
}

short CAltairDoc::MakeGraphLibrary(LPCTSTR className, LPCTSTR fileName) 
{
	if (m_dir == NULL || m_name == NULL)
		return 11;
	AGmLink *l = AUiProjectShell::SearchProject(m_dir, m_name);
	if (l == NULL) return 12;
	AGmNode *_root_class = l->arc->dst->H;
	AGmNode *_class = AUiClassShell::NameToClass(_root_class, className);
	if (_class == NULL) return 13;
	if (AGm::Refer(_class, AUiClassShell::ReferToOutside)) return 14;
	AGmNode *base_class = AGm::src_node(_class, AGm::NmSubClass());
	if (base_class == NULL) return 15;

	AGmFile *f = new AGmFile;
	if (f->Create(fileName, AGM_FILE_WRITE)) {
		delete f; return 1;
	}
	if (f->WriteString("GRAPH\n") ||	
		f->WriteString(base_class->H->a.s.v) ||
		f->WriteString("\n") ||
		f->WriteGraph(_class)) {
		delete f; return 1;
	}
	delete f; return 0;
}

short CAltairDoc::Run(LPCTSTR className, LPCTSTR methodName, LPCSTR cmdLine, short sync) 
{
	if (m_dir == NULL || m_name == NULL)
		return 11;
	AGmLink *l = AUiProjectShell::SearchProject(m_dir, m_name);
	if (l == NULL) return 12;
	AGmNode *_root_class = l->arc->dst->H;
	AGmNode *_class = AUiClassShell::NameToClass(_root_class, className);
	if (_class == NULL) return 13;
	AGmNode *method = AGm::Ref(AGm::String(methodName));
	AGmNode *_member = AGm::dst_node(_class, method);
	AGm::Unref(method);
	if (_member == NULL) return 14;
	if ((_member->HI & AUI_PUBLIC) &&
			(_member->HI & AUI_STATIC) &&
			(_member->HI & AUI_FUNC) &&
			_member->TTTH == NULL)	{
	} else {
		return 15;
	}

	AGm::Unref(ADvCmdLine);
	ADvCmdLine = AGm::Ref(AGm::String(cmdLine));

	if (AUiVtblUpdate == 0) {
			AUiVtblUpdate = 1;
			AUiMemberShell::CleanVtbl(_root_class);
			AUiMemberShell::UpdateVtbl(_root_class, _root_class);
	}
	AIpProcessMgr *pm = AIpProcessMgr::Instance();
	ASSERT(pm);
	if (sync == 0) { // asynchronous
		pm->CreateProcess(_root_class, NULL, _member);
	} else if (sync == 1) { // synchronous
		AGm::Unref(EvalCallback(_root_class, NULL, NULL, _member));
	} else {
		return 16;
	}
	return 0;
}
#endif	// _WINDOWS
