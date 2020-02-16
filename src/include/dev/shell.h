/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_EDIT_ITR_H
#define _ADV_EDIT_ITR_H

class AGmString;
class ADvWindow;
class ADvCEdit;

extern char newline[];

class ADvEditShell : public AGm {
protected:
	ADvEditShell(ADvWindow *aw);
public:
	virtual ~ADvEditShell();

	ADvWindow* ADvWnd() { return _aw; }
	static AGmNode* ADvWndToNode(ADvWindow*);
protected:
	static AGmNode* _ADvWndToNode(AGmNode*, ADvWindow*);

public:
	void	EditCtrl(ADvCEdit *edit) { _edit = edit; }
	virtual void	Update(int curr_lno = 0);
	virtual void	Print();

	virtual int 	Reset(long lno = 0);
	virtual int 	More() { return 0; }
	virtual int 	IsLast() { return 1; }
	virtual int 	Next() { return 0; }
	
	virtual AGmString* GetLineString() { return _l_str; }
	virtual AGmString* GetErrorString() { return _e_str; }

	virtual int InsertLine(const char*);
	virtual int NumInsertLine() { return 1; }
	virtual int NumOverWriteLine() { return 0; }
	virtual int ReplaceLine(const char*);
	virtual int DeleteLine();
	virtual int OpenEditor();	

protected:
	virtual int 	_PreInsertLine(const char*) { return 1; }
	virtual int 	_InsertLine(const char*) { return 1; }
	virtual void	_PostInsertLine(const char*) {}

	virtual int 	_PreReplaceLine(const char*) { return 1; }
	virtual int 	_ReplaceLine(const char*) { return 1; }
	virtual void	_PostReplaceLine(const char*) {}

	virtual int 	_PreDeleteLine() { return 1; }
	virtual int 	_DeleteLine() { return 1; }
	virtual void	_PostDeleteLine() {}

	virtual int 	_PreOpenEditor() { return 1; }
	virtual int 	_OpenEditor() { return 1; }
	virtual void	_PostOpenEditor() {}

public:
	long	Lno() { return _lno; }
	void	LimitDepth(int);

	virtual int LeftPress(ADvWindow*, AGmNode*);
	virtual int LeftRelease(ADvWindow*, AGmNode*);
	virtual int LeftDoubleClick(ADvWindow*, AGmNode*);
	virtual int RightPress(ADvWindow*, AGmNode*);
	virtual int RightRelease(ADvWindow*, AGmNode*);
	virtual int RightDoubleClick(ADvWindow*, AGmNode*);
	virtual int MouseMove(ADvWindow*, AGmNode*);
	virtual int KeyPressFunc(ADvWindow*, AGmNode*);
	virtual int KeyReleaseFunc(ADvWindow*, AGmNode*);
	virtual int KeyChar(ADvWindow*, AGmNode*);

protected:
	ADvWindow *_aw;
	ADvCEdit *_edit;
	long	_lno;
	AGmString *_l_str, *_e_str;
	int _status, _depth, _limit_depth;
	int _insert_pos;
	int _insert_mode;

// for garbage collection
public:
	virtual void GCmark() {}
};

#define AUI_SHELL_INSERT_LOWER	0
#define AUI_SHELL_INSERT_UPPER	1

#endif	/* _ADV_EDIT_ITR_H */

