/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_ADVWND_H
#define _ADV_ADVWND_H

#define ADV_WINDOW_NONE 	0
#define ADV_WINDOW_NORMAL	1
#define ADV_WINDOW_EDIT 	2
#define ADV_WINDOW_MDI		3
#define ADV_WINDOW_OCX		4

#ifdef	_WINDOWS
#define ADV_BG_COLOR	::GetSysColor(COLOR_WINDOW)
#define ADV_FG_COLOR	::GetSysColor(COLOR_WINDOWTEXT)
#endif

#ifdef	_LINUX
#define ADV_BG_COLOR	0xffffff
#define ADV_FG_COLOR	0x000000

#define R2_COPYPEN	0
#define R2_XORPEN	1

#define ADV_GDI_TYPE_PEN	1
#define ADV_GDI_TYPE_BRUSH	2
#define ADV_GDI_TYPE_FONT	3

#define NULL_PEN	1
#define NULL_BRUSH	2

#define PS_SOLID	1
#define PS_DOT		2

#define ALTERNATE	0
#define RGN_AND 	0
#define ERROR	-1

#define PT_MOVETO	0
#define PT_LINETO	1

#define	AL_RED(color)	(((double)(color & 0xff))/255.)
#define	AL_GREEN(color)	(((double)((color & 0xff00)>>8))/255.)
#define	AL_BLUE(color)	(((double)((color & 0xff0000)>>16))/255.)

class CGdiObject;
class CRgn;

class CDC {
public:
	CDC(ADvCView *cview);
	~CDC();

	void FillBackground();

	void SetTextColor(long color);
	void SetBkColor(long color);

	void SetROP2(int op);

	CGdiObject *SelectObject(CGdiObject *pObject);
	CGdiObject *SelectStockObject(int obj);
	int SelectClipRgn(CRgn *rgn);

	void MoveTo(int x, int y);
	void LineTo(int x, int y);

	void SetPixel(int x, int y, long color);
	void PolyDraw(GdkPoint *points, BYTE* types, int npoints);
	void Polygon(GdkPoint *points, int npoints);
	void PolyPolygon(GdkPoint *points, int *npolypoints, int npoly);

	void GetTextExtent(const char *str, int len, long& width, long& height);
	void TextOut(int x, int y, const char *str, int len);

public:
	ADvCView *_cview;
	cairo_t *gc;

	CPen *cpen;
	CBrush *cbrush;
	CFont *cfont;
	long pen_color;
	long pen_width;
	long fill_color;
	gint fill;
	long font_color;
	PangoFontDescription *font;
	PangoContext *context;
	long back_color;

	int x0, y0;
};

class CGdiObject {
public:
	CGdiObject(int type);
	~CGdiObject();

	int type;
};

class CPen : public CGdiObject {
public:
	CPen(int style, int width, long color);
	~CPen();

	int style;
	int width;
	long color;
};

class CBrush : public CGdiObject {
public:
	CBrush(long color);
	~CBrush();

	long color;
};

class CFont : public CGdiObject {
public:
	CFont();
	~CFont();
	int CreateFont(
		int height, int width,
		int escapement, int orientation, int weight,
		int italic, int underline, int strikeout,
		int charset, int out_precision, int clip_prcision,
		int quality, int pitch_and_family, char *font_name);

	int height;
	int width;
	int escapement;
	int orientation;
	int weight;
	int italic;
	int underline;
	int strikeout;
	int charset;
	int out_precision;
	int clip_prcision;
	int quality;
	int pitch_and_family;
	char *font_name;
};

class CRgn {
public:
	CRgn();
	~CRgn();

	int CreateRectRgn(int left, int top, int right, int bottom);
	int CreatePolyPolygonRgn(GdkPoint *pts, int* npts, int npoly, int flag);
	int CombineRgn(CRgn *rgn1, CRgn *rgn2, int op);
	int CopyRgn(CRgn *rgn2);

	void GetRgnBox(GdkRectangle *rect);
	int RectInRegion(GdkRectangle *rect);

	GdkRectangle *rect;
};

#endif

class ADvWindow;
class ADvCView;
class ADvCEdit;
class ADvEditShell;
class ADvBitmap;
class ADvCOleControl;
class AIpProcess;
class ADvCMDIFrameWnd;
class ADvCFrameWnd;

typedef int (*ADvWndProc)(ADvWindow*, AGmNode*);

typedef struct	_ADvMenuInfo {
	char*	name;
	long	id;
	struct	_ADvMenuInfo*	submenu;
} ADvMenuInfo;

#if	_WINDOWS && _64BIT
#define ADvColorCopy	(long long)1
#define ADvColorXor 	(long long)2
#else
#define ADvColorCopy	(long)1
#define ADvColorXor 	(long)2
#endif

class AL_EXT_CLASS ADvWindow : public AGmPict {
public:
	 ADvWindow();
	AGmNode* Create(AGmNode *opt=NULL);
	AGmNode* CreateEdit(ADvEditShell *shell=NULL, AGmNode **n=NULL, AGmNode *opt=NULL, int drop_file=0);
	int CreateControl(ADvCOleControl*);
	virtual ~ADvWindow();

	int FitSize();
	int SetTitle(const char*);
	AGmString *GetTitle();
	int Raise();
	int SetFocus();

public:
	int 	Damage(AGmNode *polypoly);
	void	Repair();
	void	Redraw();
protected:
#ifdef	_WINDOWS
	int 	_PolygonData(AGmNode*, LPPOINT&, LPINT&, int&);
#endif
#ifdef	_LINUX
	int 	_PolygonData(AGmNode*, GdkPoint*&, int*&, int&);
#endif
public:
	void	UpdateRgn(CRgn* rgn);
	CRgn*	UpdateRgn();
protected:
	CRgn* _update_rgn;	
#ifdef	_LINUX
public:
	int _damaged, _damage_left, _damage_top, _damage_right, _damage_bottom;
	int _in_local_loop;
#endif

public:
	void  StartDraw();
	void  EndDraw();

public:
// draw attribute
	int PushClipping(AGmNode*); // (polypoly)
	int PopClipping();
	int PushBgColor(AGmNode*);	// (color)
	int PopBgColor();
	int PushColorFunc(AGmNode*); // (func)
	int PopColorFunc();
	int PushPen(AGmNode*); // (style, width, color)
	int PopPen();
	int PushBrush(AGmNode*); // (style, color, [pattern])
	int PopBrush();
	int PushFont(AGmNode*); // (name, height, width, color)
	int PopFont();
	int PushTM(AGmNode*); // (matrix)
	int PopTM();

	AGmNode* Clipping();
	long	BgColor();
	char	ColorFunc();
	char*	PenStyle();
	char	PenWidth();
	long	PenColor();
	char*	BrushStyle();
	long	BrushColor();
	char*	BrushPattern();
	char*	FontName();
	char	FontHeight();
	char	FontWidth();
	long	FontColor();
	char	FontBold();
	char	FontItalic();
	char	FontUnderline();
	AGmNode *TM();

protected:
	int _ChangeClipping();
	int _ChangeBgColor();
	int _ChangeColorFunc();
	int _ChangePen();
	int _ChangeBrush();
	int _ChangeFont();

	AGmNode*	_clip;
	char		_clip_chg;
	AGmNode*	_clip_stack;
	char		_clip_all;

	long		_bg_color;
	char		_bg_color_chg;
	AGmNode*	_bg_color_stack;

	char		_color_func;
	char		_color_func_chg;
	AGmNode*	_color_func_stack;

	char*		_pen_style;
	char		_pen_width;
	long		_pen_color;
	char		_pen_chg;
	AGmNode*	_pen_stack;
	char		_pen_selected;

	char*		_brush_style;
	long		_brush_color;
	char*		_brush_pattern;
	char		_brush_chg;
	AGmNode*	_brush_stack;
	char		_brush_selected;

	char*		_font_name;
	char		_font_height;
	char		_font_width;
	long		_font_color;
	char		_font_bold;
	char		_font_italic;
	char		_font_underline;
	char		_font_chg;
	double		_h_scale;
	double		_w_scale;
	int _escape;
	int _orient;
	AGmNode*	_font_stack;
	char		_font_selected;

	AGmNode*	_tm;
	AGmNode*	_tm_stack;

	CPen*		_pen;
	CPen*		_original_pen;
	CBrush* 	_brush;
	CBrush* 	_original_brush;
	CFont*		_font;
	CFont*		_original_font;

	AGmNode*	GetFontInfo();
public:
// for immediate draw
	int StartQuickDraw();
	int EndQuickDraw();

public:
// draw functions
	int Point(double x, double y, long color);
	int Line(double x1, double y1, double x2, double y2);
	int Rectangle(double left, double top, double right, double bottom);		
	int Ellipse(double left, double top, double right, double bottom);	

	int PolyPolyline(AGmNode* polypoly);		
	int PolyPolygon(AGmNode* polypoly); 	
	int TextOut(double x, double y, const char*);
#if	_WINDOWS && _64BIT
	int TextExtent(const char*, long long& width, long long& height);
	int TextOutFromTo(double x, double y, const char*, long long from, long long to);
	int TextExtentFromTo(const char*, long long from, long long to, long long& width, long long& height);
#else
	int TextExtent(const char*, long& width, long& height);
	int TextOutFromTo(double x, double y, const char*, long from, long to);
	int TextExtentFromTo(const char*, long from, long to, long& width, long& height);
#endif
	int ExtTextOut(double l, double t, double r, double b, const char*);

	int Pie(double l, double t, double r, double b,
					double sx, double sy, double ex, double ey);

	int Image(double x, double y, ADvBitmap* bitmap,
		double width = 0, double height = 0);

public:
	short Type() const;
	void Type(short type);

	ADvCMDIFrameWnd* Frame();
	void Frame(ADvCMDIFrameWnd *cframe);
	ADvCView* View();
	void View(ADvCView* cview);
	ADvCOleControl* OleCtrl();
	void OleCtrl(ADvCOleControl* ctrl);
	int QuickDraw();
protected:
	short	_window_type;
	ADvCMDIFrameWnd*	_cframe;
	ADvCView*			_cview;
	ADvCOleControl* 	_ole_ctrl;
	int 	_dc_for_quick_draw;
public:
	CDC* PaintDC();
	void PaintDC(CDC *dc);
protected:
	CDC*	_dc;

// callback
public:
	ADvWndProc WndDrawProc();
	void WndDrawProc(ADvWndProc proc);
	ADvWndProc WndPickProc();
	void WndPickProc(ADvWndProc proc);

	AGmNode* Arg();
	void Arg(AGmNode*);
	AGmNode* EventProg();
	void EventProg(AGmNode*);
	AGmNode* DrawProg();
	void DrawProg(AGmNode *draw_prog);
	static int DefaultPickProc(ADvWindow*, AGmNode*);
	AGmNode* MoveCtrlProg();
	void MoveCtrlProg(AGmNode*);
	AGmNode* DestroyProg();
	void DestroyProg(AGmNode*);
	AGmNode* ActivateProg();
	void ActivateProg(AGmNode*);
	AGmNode* TextInsProg();
	void TextInsProg(AGmNode*);
protected:
	ADvWndProc	_wnd_draw_proc, _wnd_pick_proc;
	AGmNode 	*w_arg, *w_event_prog, *w_draw_prog;
	AGmNode 	*w_move_prog, *w_destroy_prog;
	AGmNode 	*w_activate_prog, *w_text_ins_prog;

// text edit control
public:
	ADvCEdit* EditCtrl();
	ADvEditShell* Shell();
protected:
	ADvCEdit *_cEdit;
#if	_WINDOWS && _64BIT
	long long	_cEdit_nID;
#else
	long	_cEdit_nID;
#endif
	ADvEditShell	*_shell;

// local event loop
public:
	void LocalLoop();
	void ExitLoop();
	AGmNode* LoopParams();
	void LoopParams(AGmNode *params);
protected:
	int _exit_loop;
	AGmNode* _loop_params;

// generic interface
public:
	static int Print(const char*, ...);
	static ADvMenuInfo *_CreateMenuInfo(AGmNode *menu, short& id);
	static AGmNode *_SearchMenuItem(AGmNode *menu, ADvMenuInfo *info, short id);
	static void _DeleteMenuInfo(ADvMenuInfo* info);
	static long MenuSelect(ADvWindow*, AGmNode*, ADvMenuInfo*);
	static char* Ask(const char *title, const char *text);
protected:
#if	_WINDOWS && _64BIT
	static void _MenuCalcSize(ADvMenuInfo*, long long&, long long&);
#else
	static void _MenuCalcSize(ADvMenuInfo*, long&, long&);
#endif
	static int _MenuDraw(ADvWindow*, AGmNode*);
	static int _MenuPick(ADvWindow*, AGmNode*);
#ifdef	_LINUX
public:
	int disable_event_handling;
#endif

// for garbage collection
public:
	void GCmark();
};					

extern int AGmDumpTrace;

#endif	/* _ADV_ADVWND_H */

