/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

// Altair.cpp : define functions of application class
//

#include "stdafx.h"
#include "altair.h"

#include "dev/mainfrm.h"
#include "altairdoc.h"
#include "altairview.h"

#include <gdm/string.h>

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef	_WINDOWS

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAltairApp constructor

CAltairApp::CAltairApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// CAltairApp

BEGIN_MESSAGE_MAP(CAltairApp, CWinApp)
	//{{AFX_MSG_MAP(CAltairApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// standard file document command
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// standartd print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// unique CAltairApp object

CAltairApp theApp;

// this id was generated to be unique statistically
// this id can be changed if you want to use a specific id

static const CLSID clsid =
{ 0x56ef30d1, 0x48f1, 0x11d2, { 0x86, 0xb0, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

/////////////////////////////////////////////////////////////////////////////
// CAltairApp initialization

BOOL CAltairApp::InitInstance()
{
	if (!ADvCWinApp::InitInstance())
		return FALSE;

//#ifdef _AFXDLL
//	Enable3dControls(); 		// case that shared MFC DLL is used
//#else
//	Enable3dControlsStatic();	// case that static MFC library is used
//#endif

	// change registry keys which is stored
	// change them to appropriate ones
	// shch as company name and department name
	SetRegistryKey(_T("Altair Applications"));

	// register document template
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_ALTAIRTYPE,
		RUNTIME_CLASS(CAltairDoc),
		RUNTIME_CLASS(ADvCMDIChildWnd), // MDI child frame
		RUNTIME_CLASS(CAltairView));
	AddDocTemplate(pDocTemplate);

	// connect COleTemplateServer to document template
	// COleTemplateServer creates a new document
	// instead of request to OLE container,
	// using information specified by document template
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);
	// register OLE server factory if executed
	// make OLE library available, in order to be enable for other applications to create the object
	COleTemplateServer::RegisterAll();

	// create main MDI frame window
	ADvCMainFrame* pMainFrame = new ADvCMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// analize command line of standard shell command shch as DDE, fil open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// comfirm whether executed as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// main window can not be displayed
		// when application is executed by /Embedding and /Automation
		return TRUE;
	}
	// update system registry
	// when system registry is broken and application starts up as stand alone.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	if (m_lpCmdLine[0] != '\0')
		CommandLine(m_lpCmdLine);

	// display and update, since main window is initialized
	if (ADvShowWnd) {
		pMainFrame->ShowWindow(SW_SHOW);
		pMainFrame->UpdateWindow();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for version information of application

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// dialog data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generate override functions of virtual functions
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// // support of DDX/DDV
	//}}AFX_VIRTUAL

// implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// no message handler
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// no message handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// application command for execting dialog
void CAltairApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAltairApp command

#endif	// _WINDOWS

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef	_LINUX
int main(int argc, char **argv) {
	CAltairApp *app = new CAltairApp(argc, argv);
	if (!app->InitInstance()) {
		return app->ExitInstance();
	} else {
		return app->Run();
	}
}

CAltairApp::CAltairApp(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;

	m_lpCmdLine = new AGmString(64);
	for (int i = 1; i < argc; i++) {
		AGmString *s1 = new AGmString(argv[i]);
		if (s1->Search(" ", 0) >= 0) {
			AGmString *s2 = new AGmString(16);
			s2->WriteQString(s1);
			delete s1; s1 = s2;
		}
		m_lpCmdLine->Append(s1);
		m_lpCmdLine->Append(" ");
		delete s1;
	}
}

int CAltairApp::InitInstance() {
	if (!ADvCWinApp::InitInstance())
		return 0;

	ADvCMainFrame* pMainFrame = new ADvCMainFrame;
	if (!pMainFrame->Create("altair",
			0, 20, 30, 240, 80, NULL)) {
		pMainFrame->DestroyWindow();
		return 0;
	}
	m_pMainWnd = pMainFrame;

	CommandLine(m_lpCmdLine->Str());
	delete m_lpCmdLine;

	if (ADvShowWnd) {
		pMainFrame->ShowWindow(SW_SHOW);
	}

	return 1;
}
#endif

