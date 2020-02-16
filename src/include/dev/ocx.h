/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_OCX_H
#define _ADV_OCX_H

#ifdef	_WINDOWS

class COleControlSite;
class ADvWindow;
class ADvOcx;

class ADvOcxWnd : public CWnd {
public:
	ADvOcxWnd(ADvOcx*);
	~ADvOcxWnd();
	COleControlSite *GetControlSite() { return m_pCtrlSite; }
protected:
	virtual BOOL PreTranslateMessage(MSG*);
	virtual BOOL PreCreateWindow(CREATESTRUCT&);
	virtual BOOL OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*);
	ADvOcx *_ocx;
};

class AL_EXT_CLASS ADvOcx : public AGmPict {
public:
	ADvOcx(const char *progID, int visible, const char *subStgID);
	ADvOcx(ADvOcx&);
	int Create(ADvWindow *aw);
	virtual ~ADvOcx();

	AGmString *ProgID() { return _progID; }
	int Left() { return _left; }
	int Top() { return _top; }
	int Right() { return _right; }
	int Bottom() { return _bottom; }
	int Visible() { return _visible; }
	AGmString *SubStgID() { return _subStgID; }
	ADvWindow *Parent() { return _aw; }
	void Parent(ADvWindow *aw) { _aw = aw; }

	long nID() { return _nID; }
	ADvOcxWnd *Ctrl() { return _ctrl; }
	void Ctrl(ADvOcxWnd* ctrl) { _ctrl = ctrl; }

	const char *FileName() const;
	void FileName(const char *name);
	
	int Place(int left, int top, int right, int bottom);
	void Visible(int);

	int Mode() { return _mode; }
	void Mode(int mode) { _mode = mode; }

	IUnknown *GetUnknown();
	IDispatch *GetDispatch();
	ITypeInfo *GetClassInfo();
	BOOL RuntimeInvisible();

	void PropertyEditor();
	int Save(IStorage*);
	int Load(IStorage*);

	static int GetIDsOfNames(IDispatch*, const char*, DISPID*);
	static COleVariant* ToVariant(AGmNode*);
	static AGmNode *FromVariant(const VARIANT&);
	static COleVariant *LoadPicture(const char *dir, const char *name);
	static COleVariant *CreateFont(const char *name, int size,
		int bold, int italic, int underline, int strike);

	static AGmNode* ProgIDList();

protected:
	AGmString *_progID;
	int _left, _top, _right, _bottom, _visible;
	AGmString *_subStgID;
	ADvWindow *_aw;
	long _nID;
	ADvOcxWnd *_ctrl;
	AGmString *_fileName;
	int _mode;
};

#endif	// _WINDOWS

#ifdef	_LINUX
class ADvOcxWnd {
};

class ADvOcx : public AGmPict {
public:
	ADvOcx(ADvOcx&);
	ADvOcx(const char *progID, int visible, const char *subStgID);
	~ADvOcx();

	AGmString *ProgID() { return _progID; }
	int Visible() { return _visible; }
	AGmString *SubStgID() { return _subStgID; }
	int Mode() { return _mode; }
	void Mode(int mode) { _mode = mode; }

protected:
	AGmString *_progID;
	int _left, _top, _right, _bottom, _visible;
	AGmString *_subStgID;
	ADvWindow *_aw;
	long _nID;
	ADvOcxWnd *_ctrl;
	AGmString *_fileName;
	int _mode;
};

class COleVariant {
public:
	COleVariant(COleVariant&) {}
};
#endif

#endif	/* _ADV_OCX_H */
