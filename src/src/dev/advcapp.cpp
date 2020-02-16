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
#include <gdm/vmkey.h>
#include <gdm/pict.h>
#include <gdm/error.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/cui.h>
#include <cui/project.h>
#include <dev/mainfrm.h>
#include <altair/strres.h>

#include <sql/sql.h>

#include <net/socket.h>
#include <net/crypt.h>

AGmNode* ADvCmdLine = NULL;
int ADvExitCode = 0;
int ADvShowWnd = 1;
int ADvAltairExe = 0;
int ADvAutoLayout = 1;
int ADvX = 0;
int ADvY = 0;
int ADvWidth = 450;
int ADvHeight = 400;
long ADvWndStyle;
AGmNode *ADvFrames = NULL;
int ADvExiting = 0;
int AGmOutputGui = 1;
struct timeb startup_time;

/////////////////////////////////////////////////////////////////////////////
// ADvCWinApp construction

static ADvCWinApp *theApp = NULL;

ADvCWinApp::ADvCWinApp() {
	theApp = this;
}

/////////////////////////////////////////////////////////////////////////////
void ADvDoEvents() {
#ifdef	_WINDOWS
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) { // WM_QUIT
			AfxPostQuitMessage(0);
			break;
		}
	}
#endif
#ifdef	_LINUX
	while (g_main_pending()) {
		g_main_iteration(TRUE);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

#define TIME_SLICE	500

int time_slice = TIME_SLICE;
int OnIdleProc(int time_slice2) {
	ADvDestroyDocTmpls();

#ifdef	_LINUX
	// process post_message
	ADvCMainFrame *frw = (ADvCMainFrame*)ADvGetApp()->m_pMainWnd;
	ADvMessage *msg = ADvMessage::Get();
	while (msg) {
		AGm::Unref(frw->ProcessMsg(msg));
		delete msg;
		msg = ADvMessage::Get();
	}
	// check pipe-connected child process exit
	AGmFile::CheckChildProcessExit();
#endif

	AIpProcessMgr *prcsmgr = AIpProcessMgr::Instance();
	AIpProcess *process = prcsmgr->Process();
	if (process == NULL) {
		AGm::ReduceUniqueStrTbl();
		return 0;
	}

	int flag = 0;
	long count;
	for (count = 0; count < time_slice2; count++) {

		try {
			flag = process->Execute() ;

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

			if (process->IsWaiting()) flag = AIP_PROCESS;

			if (flag == AIP_NEXT || flag == AIP_PROCESS || flag == AIP_EXIT) break;

			if (process->ParaLock()) count = 0;

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

			count = time_slice2; flag = AIP_EXIT;

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
				e->Delete();

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

			flag = AIP_EXIT;
			break;
#endif
		}

	}
	AIpProcess *proc;
	switch (flag) {
	case AIP_NEXT:
#ifdef	_LINUX
		// check pipe-connected child process exit
		AGmFile::CheckChildProcessExit();
#endif
	case AIP_PROCESS:
	case 0:
		prcsmgr->NextProcess();
		break;
	case AIP_EXIT:
		proc = process->ReturnProcess();
		if (proc) {
			proc->ExecuteException(process);
		}
		delete process;
		break;
	}
	return 1;
}

void ADvInitFrames() {
	if (ADvFrames) return;
	ADvFrames = AGm::Ref(AGm::Cons(NULL));
	AGm::CreateArc(ADvFrames, AGm::Cons(NULL), AGm::UniqueStr("MDIFrame"));
	AGm::CreateArc(ADvFrames, AGm::Cons(NULL), AGm::UniqueStr("SDIFrame"));
}
void ADvDestroyFrames(int exe) {
	if ((ADvAltairExe == 1 && exe == 1) || (ADvAltairExe == 0 && exe == 0)) {
		AUiProgramMgr::CloseAllWindows();
		ADvDestroyDocTmpls();
		AGm::Unref(ADvFrames); ADvFrames = NULL;
	}
}

void ADvDestroyDocTmpls() {
	AGmNode *n = AGm::DocTmpl();
	AGmLink *l;
	for (l = AGm::next_outlink(n, NULL);
			l; l = AGm::next_outlink(n, l)) {
#ifdef	_WINDOWS
		CDocTemplate *tmpl = (CDocTemplate*)l->arc->dst->a.p.ptr;
		delete tmpl;
#endif
		l = l->prev;
		AGm::DeleteArc(l->next->arc);
	}
}

#ifdef	_WINDOWS
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ADvCWinApp

IMPLEMENT_DYNAMIC(ADvCWinApp, CWinApp)

/////////////////////////////////////////////////////////////////////////////
// ADvCWinApp initialization

BOOL ADvCWinApp::InitInstance() {
	ftime(&startup_time);
	ADvAltairExe = 1;

	if (!CWinApp::InitInstance())
		return FALSE;
	if (!AfxOleInit()) {
		AfxMessageBox(AL_O1);
		return FALSE;
	}
	AfxEnableControlContainer();

	AGm::RefMemorySystem();
	ADvSocket::Initialize();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// ADvCWinApp finalization

int ADvCWinApp::ExitInstance() {
	AGm::Unref(ADvCmdLine); ADvCmdLine = NULL;
	if (AGmOutputFile) { delete AGmOutputFile; AGmOutputFile = NULL; }

	FinalizeGdiCache(1);
	AGmPNumMgr::Finalize();
	ADvSocket::Finalize();
	ADvSQLFinalize();
	AGm::UnrefMemorySystem();

	CWinApp::ExitInstance();
	return ADvExitCode;
}

/////////////////////////////////////////////////////////////////////////////
// ADvCWinApp exception handling
LRESULT ADvCWinApp::ProcessWndProcException(CException*, const MSG*) {
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// ADvCWinApp command

BOOL ADvCWinApp::OnIdle(LONG lCount) {
	BOOL result = CWinApp::OnIdle(lCount);
	if (result) return result;
	return OnIdleProc(TIME_SLICE);
}

ADvCWinApp *ADvGetApp() {
	CWinApp *app = AfxGetApp();
	if (app && app->IsKindOf(RUNTIME_CLASS(ADvCWinApp))) {
		return (ADvCWinApp*)app;
	}
	return NULL;
}

void ADvBeginWaitCursor() {
	AfxGetApp()->BeginWaitCursor();
}
void ADvEndWaitCursor() {
	AfxGetApp()->EndWaitCursor();
}

#endif	// _WINDOWS

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef	_LINUX

int ADvCWinApp::InitInstance() {
	ftime(&startup_time);
	ADvAltairExe = 1;
	AGm::RefMemorySystem();

	gtk_init(&argc, &argv);

	ADvMessage::Initialize();
	ADvSocket::Initialize();

	return 1;
}

int ADvCWinApp::ExitInstance() {
	AGm::Unref(ADvCmdLine); ADvCmdLine = NULL;
	if (AGmOutputFile) { delete AGmOutputFile; AGmOutputFile = NULL; }

	ADvMessage::Finalize();
	AGmPNumMgr::Finalize();
	ADvSocket::Finalize();
	ADvSQLFinalize();
	AGm::UnrefMemorySystem();

	return ADvExitCode;
}

int ADvCWinApp::Run() {
	for (;;) {
		int bIdle = TRUE;
		while (bIdle && !g_main_pending()) {
			if (!OnIdle()) bIdle = FALSE;
		}
		do {
			g_main_iteration(TRUE);
			if (ADvExiting) break;
		} while (g_main_pending());
		if (ADvExiting) break;
	}
	return ExitInstance();
}

void ADvCWinApp::ExitApplication() {
	ADvCMainFrame *frame = (ADvCMainFrame*)ADvGetApp()->m_pMainWnd;
	if (frame) {
		frame->DestroyWindow();
	}
}
int ADvCWinApp::OnIdle() {
	return OnIdleProc(TIME_SLICE);
}

int adv_wnd_no = 1;
void ADvCWinApp::CreateWnd() {
	ADvCMDIChildWnd *wnd = new ADvCMDIChildWnd;
	char title[256];
	sprintf(title, "Altair%d", adv_wnd_no++);
	int x = ADvX, y = ADvY, w = ADvWidth, h = ADvHeight;
	if (ADvAutoLayout) {
		int width = 800, height = 600;
		w = width * 4 / 7 ;
		h = height * 3 / 4;
		x += 40; y += 30;
		if (x + w > 1000) x = 10; if (y + h > 700) y = 30;
		ADvX = x; ADvY = y; ADvWidth = w; ADvHeight = h;
	}
	wnd->Create(title, 0, x, y, w, h, m_pMainWnd);
	wnd->ShowWindow(SW_SHOW);
}

ADvCWinApp *ADvGetApp() {
	return theApp;
}

void ADvBeginWaitCursor() {
	// TODO_LINUX BeginWaitCursor
}
void ADvEndWaitCursor() {
	// TODO_LINUX EndWaitCursor
}

/////////////////////////////////////////////////////////////////////////////
// ADvPostMessage, ADvMessage

void ADvPostMessage(long hwnd, long msg, long wParam, long lParam) {
	ADvMessage::Put(new ADvMessage(hwnd, msg, wParam, lParam));
}

ADvMessage *ADvMessage::root = NULL;
GMutex msg_mutex;

void ADvMessage::Initialize() {
	root = new ADvMessage(0, 0, 0, 0);
	root->next = root->prev = root;
	g_mutex_init(&msg_mutex);
}
void ADvMessage::Finalize() {
	for (ADvMessage *msg = Get(); msg; msg = Get()) {
		delete msg;
	}
	delete root;
	g_mutex_clear(&msg_mutex);
}
ADvMessage::ADvMessage(long hwnd, long msg, long wParam, long lParam) {
	this->hwnd = hwnd;
	this->msg = msg;
	this->wParam = wParam;
	this->lParam = lParam;
}
void ADvMessage::Put(ADvMessage *msg) {
	g_mutex_lock(&msg_mutex);
	msg->prev = root->prev; msg->next = root;
	root->prev->next = msg; root->prev = msg;
	g_mutex_unlock(&msg_mutex);
}
ADvMessage *ADvMessage::Get() {
	g_mutex_lock(&msg_mutex);
	ADvMessage *msg = root->next;
	if (msg == root) {
		g_mutex_unlock(&msg_mutex);
		return NULL;
	}
	msg->prev->next = msg->next; msg->next->prev = msg->prev;
	g_mutex_unlock(&msg_mutex);
	return msg;
}
#endif	// _LINUX

// caching resources for draw
class CachePen {
public:
	CPen *_pen;
	char *_style;
	long _width;
	long _color;

	CachePen(CPen* pen,
			char *style, long width, long color) {
		_pen = pen;
		_style = style;
		_width = width;
		_color = color;
	}
	~CachePen() {
		delete _pen;
	}
};
class CacheBrush {
public:
	CBrush *_brush;
	char *_style;
	long _width;
	long _color;
	char *_pattern;

	CacheBrush(CBrush *brush,
			char *style, long color, char *pattern) {
		_brush = brush;
		_style = style;
		_color = color;
		_pattern = pattern;
	}
	~CacheBrush() {
		delete _brush;
	}
};
class CacheFont {
public:
	CFont *_font;
	char *_name;
	long _height;
	long _width;
	long _color;
	double _h_scale;
	double _w_scale;
	int _escape;
	int _orient;
	long _font_bold;
	long _font_italic;
	long _font_underline;

	CacheFont(CFont *font,
			char *name, long height, long width, long color,
			double h_scale, double w_scale, int escape, int orient,
			long font_bold, long font_italic, long font_underline) {
		_font = font;
		_name = name;
		_height = height;
		_width = width;
		_color = color;
		_h_scale = h_scale;
		_w_scale = w_scale;
		_escape = escape;
		_orient = orient;
		_font_bold = font_bold;
		_font_italic = font_italic;
		_font_underline = font_underline;
	}
	~CacheFont() {
		delete _font;
	}
};

void FinalizeGdiCache(int exe) {

	if ((ADvAltairExe == 1 && exe == 1) || (ADvAltairExe == 0 && exe == 0)) {
		//	!!!!!!!!  consign Free-GDI-resource task to Windows-OS !!!!!!!!
#ifdef _DEBUG
		AGmLink *l;
		AGmNode *pens = AGm::dst_node2(AGm::RootGlobal(), AGm::NmPen());
		if (pens) {
			l = pens->body->out;
			while (l->next != l) {
				CachePen* cp = (CachePen*)l->next->arc->dst->a.p.ptr;
				delete cp;
				AGm::DeleteArc(l->next->arc);		
			}
		}
		AGmNode *brushs = AGm::dst_node2(AGm::RootGlobal(), AGm::NmBrush());
		if (brushs) {
			l = brushs->body->out;
			while (l->next != l) {
				CacheBrush* cb = (CacheBrush*)l->next->arc->dst->a.p.ptr;
				delete cb;
				AGm::DeleteArc(l->next->arc);
			}
		}
		AGmNode *fonts = AGm::dst_node2(AGm::RootGlobal(), AGm::NmFont());
		if (fonts) {
			l = fonts->body->out;
			while (l->next != l) {
				CacheFont* cf = (CacheFont*)l->next->arc->dst->a.p.ptr;
				delete cf;
				AGm::DeleteArc(l->next->arc);
			}
		}
#endif
	}
}

#if defined(_DEBUG) || defined(_LINUX_DEBUG)
int num_cache_pen = 0;
int num_cache_brush = 0;
int num_cache_font = 0;
#endif

void ADvPutCPen(CPen* pen,
		char *style, long width, long color) {
	if (ADvGetCPen(style, width, color))
		return;
	AGmNode *pens = AGm::dst_node2(AGm::RootGlobal(), AGm::NmPen());
	if (pens == NULL) {
		pens = AGm::Cons(NULL);
		AGm::CreateArc(AGm::RootGlobal(), pens, AGm::NmPen());
	}
	CachePen *cp = new CachePen(pen, style, width, color);
	AGm::CreateArc(pens, AGm::PtrCell(cp));
	TRACE1(AL_STR("cachePen count = %d\n"), ++num_cache_pen);
}
void ADvPutCBrush(CBrush *brush,
		char *style, long color, char *pattern) {
	if (ADvGetCBrush(style, color, pattern))
		return;
	AGmNode *brushs = AGm::dst_node2(AGm::RootGlobal(), AGm::NmBrush());
	if (brushs == NULL) {
		brushs = AGm::Cons(NULL);
		AGm::CreateArc(AGm::RootGlobal(), brushs, AGm::NmBrush());
	}
	CacheBrush *cb = new CacheBrush(brush, style, color, pattern);
	AGm::CreateArc(brushs, AGm::PtrCell(cb));
	TRACE1(AL_STR("cacheBrush count = %d\n"), ++num_cache_brush);
}
void ADvPutCFont(CFont *font,
		char *name, long height, long width, long color,
		double h_scale, double w_scale, int escape, int orient,
		long font_bold, long font_italic, long font_underline) {
	if (ADvGetCFont(name, height, width, color,
			h_scale, w_scale, escape, orient,
			font_bold, font_italic, font_underline))
		return;
	AGmNode *fonts = AGm::dst_node2(AGm::RootGlobal(), AGm::NmFont());
	if (fonts == NULL) {
		fonts = AGm::Cons(NULL);
		AGm::CreateArc(AGm::RootGlobal(), fonts, AGm::NmFont());
	}
	CacheFont *cf = new CacheFont(font, name, height, width, color,
		h_scale, w_scale, escape, orient,
		font_bold, font_italic, font_underline);;
	AGm::CreateArc(fonts, AGm::PtrCell(cf));
	TRACE1(AL_STR("cacheFont count = %d\n"), ++num_cache_font);
}

CPen* ADvGetCPen(
		char *style, long width, long color) {
	AGmNode *pens = AGm::dst_node2(AGm::RootGlobal(), AGm::NmPen());
	if (pens == NULL)
		return NULL;
	AGmLink *l;
	for (l = AGm::next_outlink(pens, NULL);
			l; l = AGm::next_outlink(pens, l)) {
		CachePen *cp = (CachePen*)l->arc->dst->a.p.ptr;
		if (strcmp(style, cp->_style) == 0 &&
				width == cp->_width && color == cp->_color) {
			return cp->_pen;
		}
	}
	return NULL;
}
CBrush* ADvGetCBrush(
		char *style, long color, char *pattern) {
	AGmNode *brushs = AGm::dst_node2(AGm::RootGlobal(), AGm::NmBrush());
	if (brushs == NULL)
		return NULL;
	AGmLink *l;
	for (l = AGm::next_outlink(brushs, NULL);
			l; l = AGm::next_outlink(brushs, l)) {
		CacheBrush *cb = (CacheBrush*)l->arc->dst->a.p.ptr;
		if (strcmp(style, cb->_style) == 0 &&
				color == cb->_color &&
				AGmString::str_cmp(pattern , cb->_pattern, 8) == 0) {
			return cb->_brush;
		}
	}
	return NULL;
}
CFont* ADvGetCFont(
		char *name, long height, long width, long color,
		double h_scale, double w_scale, int escape, int orient,
		long font_bold, long font_italic, long font_underline) {
	AGmNode *fonts = AGm::dst_node2(AGm::RootGlobal(), AGm::NmFont());
	if (fonts == NULL)
		return NULL;
	AGmLink *l;
	for (l = AGm::next_outlink(fonts, NULL);
			l; l = AGm::next_outlink(fonts, l)) {
		CacheFont *cf = (CacheFont*)l->arc->dst->a.p.ptr;
		if (strcmp(name, cf->_name) == 0 &&
				height == cf->_height &&
				width == cf->_width &&
				color == cf->_color &&
				h_scale == cf->_h_scale &&
				w_scale == cf->_w_scale &&
				escape == cf->_escape &&
				orient == cf->_orient &&
				font_bold == cf->_font_bold &&
				font_italic == cf->_font_italic &&
				font_underline == cf->_font_underline) {
			return cf->_font;
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ADvCWinApp::CommandLine(const char *cmd) {
	AGmString *s = new AGmString(cmd);
	AGmFile *f = new AGmFile;
	if (f->Create(s, AGM_FILE_STR_READ)) {
		delete f; delete s;
		return;
	}

	AGmString *curr, *dir, *file = NULL, *cls = NULL, *method = NULL;
	if (AGmFile::GetDir(curr) == 0) {
	} else {
		curr = new AGmString(".");
	}
	dir = new AGmString(curr);

	for (;;) {
		if (f->MatchIdent("-hide")) {
			ADvShowWnd = 0;
		} else if (f->MatchIdent("-dir")) {
			if (dir) delete dir;
			if (f->ReadString(dir) == 0) {
				AGmFile::ChDir(dir->Str());
			} else {
				dir = new AGmString(curr);
			}
		} else if (f->MatchIdent("-file")) {
			if (file) delete file;
			if (f->ReadString(file) == 0) {
				AUiProgramMgr::_Open(dir, file);
			} else {
				file = NULL;
			}
		} else if (f->MatchIdent("-class")) {
			if (cls) delete cls;
			if (f->ReadString(cls) == 0) {
			} else {
				cls = NULL;
			}
		} else if (f->MatchIdent("-method")) {
			if (method) delete method;
			if (f->ReadString(method) == 0) {
				if (file == NULL || cls == NULL) continue;

				AGmLink *l = AUiProjectShell::SearchProject(dir, file);
				if (l == NULL) continue;
				AGmNode *root_class = l->arc->dst->H;
				AGmNode *_class = AUiClassShell::NameToClass(root_class, cls);
				if (_class == NULL) continue;
				AGmNode *_member = AGm::dst_node(_class, method->Str());
				if (_member == NULL) continue;
				if ((_member->HI & AUI_PUBLIC) &&
						(_member->HI & AUI_STATIC) &&
						(_member->HI & AUI_FUNC) &&
						_member->TTTH == NULL)	{
					AUiProgramMgr::CloseAllWindows();
					if (AUiVtblUpdate == 0) {
						AUiVtblUpdate = 1;
						ADvBeginWaitCursor();
						AUiMemberShell::CleanVtbl(root_class);
						AUiMemberShell::UpdateVtbl(root_class, root_class);
						ADvEndWaitCursor();
					}
					AIpProcessMgr *pm = AIpProcessMgr::Instance();
					ASSERT(pm);
					pm->CreateProcess(root_class, NULL, _member);
				}
			} else {
				method = NULL;
			}
		} else {
			break;
		}
	}
	if (curr) delete curr;
	if (dir) delete dir;
	if (file) delete file;
	if (cls) delete cls;
	if (method) delete method;
	f->ReadSpace();
	if (f->ReadLine(ADvCmdLine) == 0) {
		AGm::Ref(ADvCmdLine);
	}
	delete f; delete s;
}

AGmFile *AGmOutputFile = NULL;
int AGmOutputFileMaxSize = 100 * 1024 * 1024;

void AGmOutput(const char* s) {
	if (AGmOutputFile == NULL) return;
	AGmString* filename = AGmOutputFile->Filename();
	struct stat buf;
	if (filename && AGmFile::GetStat(filename->Str(), &buf) == 0) {
		char fname[256];
		art_strcpy(fname, 256, filename->Str());
		if (buf.st_size + (signed)strlen(s) > AGmOutputFileMaxSize) {
			delete AGmOutputFile;
			char path[256];
			art_strcpy(path, 256, fname);
			art_strcat(path, 256, ".");
#if	_WINDOWS && _64BIT
			long long n = strlen(path);
#else
			long n = strlen(path);
#endif
			long year, month, day, hour, minute, second, wday;
			get_localtime(get_time(), year, month, day, hour, minute, second, wday);
			art_sprintf_6((char*)&path[n], 256 - n, "%.4ld%.2ld%.2ld%.2ld%.2ld%.2ld", year, month, day, hour, minute, second);

			int err = AGmFile::Rename(fname, path);
			ASSERT(err == 0);
			AGmOutputFile = new AGmFile();
			err = AGmOutputFile->Create(fname, AGM_FILE_WRITE);
			ASSERT(err == 0);
		}
	}
	AGmOutputFile->WriteString(s);
	AGmOutputFile->Flush();
}
