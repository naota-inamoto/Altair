/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_OLECTRL_H
#define _ADV_OLECTRL_H

#ifdef	_WINDOWS

class AL_EXT_CLASS ADvCOleControlModule : public COleControlModule {
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

class AL_EXT_CLASS ADvCOleControl : public COleControl {
	DECLARE_DYNAMIC(ADvCOleControl)
public:
	ADvCOleControl();
	virtual ~ADvCOleControl();
protected:
	ADvWindow *_ole_wnd;

protected:
	virtual void OnDraw(
				CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual BOOL PreCreateWindow(CREATESTRUCT&);
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);
	virtual int CheckEvent(UINT, WPARAM, LPARAM);
	virtual int EventProcess();
	virtual BOOL OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*);
protected:
	AGmNode* _event;

public:
	virtual int SetMethod(AGmNode* /*method*/, AGmNode* /*obj*/, AGmNode* /*func*/) { return 1; }
	virtual void CustomEvent(AGmNode* /*event*/, AGmNode* /*params*/) {}
protected:
	AGmNode* _method_tbl;

public:
	AGmNode *CallbackTbl() { return _callback_tbl; }
protected:
	AGmNode *_callback_tbl;
	AGmNode *_root_class;

public:
	void GCmark();

// message map
	//{{AFX_MSG(ADvCOleControl)
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern const GUID IID_IAGmNode;

class IAGmNode : public IUnknown {
public:
	IAGmNode();
	virtual ~IAGmNode();

	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID, LPVOID*);

	STDMETHOD(GetNode)(AGmNode**);
	STDMETHOD(SetNode)(AGmNode*);

protected:
	ULONG	m_refCount;
	AGmNode* m_node;
};

AL_EXT_API IAGmNode* AGmNodeToVariant(AGmNode*, VARIANT*);
AL_EXT_API AGmNode* VariantToAGmNode(const VARIANT*, char* name1, char* name2);

#endif	// _WINDOWS

#endif	/* _ADV_OLECTRL_H */
