/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_ADVAPP_H
#define _ADV_ADVAPP_H

/////////////////////////////////////////////////////////////////////////////
// ADvCWinApp:

class AIpProcess;

#ifdef	_WINDOWS
class AL_EXT_CLASS ADvCWinApp : public CWinApp {
	DECLARE_DYNAMIC(ADvCWinApp)
public:
	ADvCWinApp();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	virtual LRESULT ProcessWndProcException(CException*, const MSG*);

	virtual BOOL OnIdle(LONG);

	void CommandLine(const char*);

public:
	void ExitApplication() { CWinApp::OnAppExit(); }
	void CreateWnd() { CWinApp::OnFileNew(); }
};

#endif	// _WINDOWS

#ifdef	_LINUX

extern int adv_wnd_no;

class CFrameWnd;

class AL_EXT_CLASS ADvCWinApp {
public:
	ADvCWinApp();

	virtual int InitInstance();
	virtual int ExitInstance();
	virtual int Run();

	virtual int OnIdle();

	void CommandLine(const char*);

	void ExitApplication();
	void CreateWnd();

public:
	int argc;
	char** argv;
	CFrameWnd *m_pMainWnd;
};

/////////////////////////////////////////////////////////////////////////////
// ADvPostMessage, ADvMessage

void ADvPostMessage(long hwnd, long msg, long wParam, long lParam);

class ADvMessage {
public:
	static void Initialize();
	static void Finalize();
	ADvMessage(long hwnd, long msg, long wParam, long lParam);
	static void Put(ADvMessage *msg);
	static ADvMessage *Get();
	long hwnd, msg, wParam, lParam;

private:
	ADvMessage *next, *prev;
	static ADvMessage *root;
};
#endif	// _LINUX

extern int ADvAutoLayout;
extern int ADvX;
extern int ADvY;
extern int ADvWidth;
extern int ADvHeight;
extern long ADvWndStyle;
extern AGmNode *ADvFrames;
extern int ADvExiting;

AL_EXT_API void ADvBeginWaitCursor();
AL_EXT_API void ADvEndWaitCursor();
ADvCWinApp *ADvGetApp();

void ADvInitFrames();
void ADvDestroyFrames(int exe=1);
void ADvDestroyDocTmpls();

void ADvDoEvents();

extern int time_slice;
int OnIdleProc(int time_slice);

AL_EXT_DATA extern AGmNode* ADvCmdLine;
AL_EXT_DATA extern int ADvShowWnd;
extern int ADvExitCode;
extern int ADvAltairExe;

void FinalizeGdiCache(int exe);

class CPen;
class CBrush;
class CFont;

void ADvPutCPen(CPen* pen,
		char *style, long width, long color);
void ADvPutCBrush(CBrush *brush,
		char *style, long color, char *pattern);
void ADvPutCFont(CFont *font,
		char *name, long height, long width, long color,
		double h_scale, double w_scale, int escape, int orient,
		long font_bold, long font_italic, long font_underline);
CPen* ADvGetCPen(
		char *style, long width, long color);
CBrush* ADvGetCBrush(
		char *style, long color, char *pattern);
CFont* ADvGetCFont(
		char *name, long height, long width, long color,
		double h_scale, double w_scale, int escape, int orient,
		long font_bold, long font_italic, long font_underline);

class AGmFile;
extern AGmFile *AGmOutputFile;
extern int AGmOutputFileMaxSize;
extern int AGmOutputGui;

void AGmOutput(const char*);

time_t get_time();
void get_localtime(time_t ltime, long& year, long &month, long &day,
	long& hour, long& minute, long& second, long& wday);
time_t get_mktime(long year, long month, long day,
	long hour, long minute, long second);
void get_gmtime(time_t ltime, long& year, long &month, long &day,
		long& hour, long& minute, long& second, long& wday);

void QuickSort(AGmNode*&, AGmNode*, int, AGmNode* attr=NULL);
void QuickSortResult(AGmNode*, AGmNode*, int);

#endif	/* _ADV_ADVAPP_H */

