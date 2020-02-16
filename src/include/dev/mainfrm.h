/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_MAINFRM_H
#define _ADV_MAINFRM_H

#ifdef	_WINDOWS

extern void exitTimerFunc();

class AL_EXT_CLASS ADvCMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(ADvCMainFrame)
public:
	ADvCMainFrame();

// attribute
public:

// operation
public:

// implementaion
public:
	virtual ~ADvCMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);

protected:	// for controls
	CStatusBar	m_wndStatusBar;
	//CToolBar	m_wndToolBar;

public:
	void SetMsgCallback(long msg, AGmNode *root_obj_func);
	AGmNode *lParam() { return _lParam; }
	AGmNode *wParam() { return _wParam; }
	void SetUnloadCallback(AGmNode *root_obj_func);
	unsigned int SetTimerCallback(long id, long period, AGmNode *root_obj_func);
	void GCmark();
protected:
	AGmNode *_msg_callback_tbl;
	AGmNode *_wParam;
	AGmNode *_lParam;
	AGmNode *_unload_callback;
	AGmNode *_timer_callback_tbl;

// generate message map functions
protected:
	//{{AFX_MSG(ADvCMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
#ifdef	_64BIT
	afx_msg void OnTimer(UINT_PTR nIDEvent);
#else
	afx_msg void OnTimer(UINT nIDEvent);
#endif
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnEndSession(BOOL);
	afx_msg void OnProjWnd();
	afx_msg void OnOnlineManual();
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// _WINDOWS

#ifdef	_LINUX

class AL_EXT_CLASS ADvCMainFrame : public CMDIFrameWnd {
public:
	ADvCMainFrame();
	virtual ~ADvCMainFrame();

	int Create(const char *name,
		long style, long left, long top, long width, long height,
		CWnd *parent);
	virtual void OnDestroy();

	virtual void DestroyWindow();

	virtual void SetMessageText(const char *text);

	AGmNode *_registered_msg;
	void SetMsgCallback(long msg, AGmNode *root_obj_func);
	AGmNode *_msg_callback_tbl;
	AGmNode *ProcessMsg(ADvMessage*);
	AGmNode *lParam() { return _lParam; }
	AGmNode *wParam() { return _wParam; }
	AGmNode *_wParam;
	AGmNode *_lParam;

	void SetUnloadCallback(AGmNode *root_obj_func);
	AGmNode *_unload_callback;

	unsigned int SetTimerCallback(long id, long period, AGmNode *root_obj_func);
	AGmNode *_timer_callback_tbl;

	void GCmark();

	CMenu		*_cmenu;
	CStatusBar	*_cstatusbar;
};

#endif	// _LINUX

#endif	/* _ADV_MAINFRM_H */

