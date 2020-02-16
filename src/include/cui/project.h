/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AUI_PROJECT_H
#define _AUI_PROJECT_H

class AL_EXT_CLASS AUiProjectShell : public ADvEditShell {
public:
	AUiProjectShell(ADvWindow*);
	virtual ~AUiProjectShell();

public:
	static AGmNode* AddProject(AGmString *dir, AGmString *name,
						AGmNode *root_class, AGmNode *lib_list, AGmNode *class_list);
	static int		RemoveProject(AGmNode *project);
	static AGmLink* SearchProject(AGmString *dir, AGmString *name);
	static AGmLink* SearchProject(AGmNode *root_class);

public:
	virtual int 	Reset(long lno = 0);
	virtual int 	More();
	virtual int 	IsLast();
	virtual int 	Next();

	virtual AGmString* GetLineString();

protected:
	virtual int 	_PreDeleteLine();
	virtual int 	_DeleteLine();

	virtual int 	_PreOpenEditor();
	virtual int 	_OpenEditor();

public:
	static	int 	OpenClassEditor(AGmNode* project);

public:
	int Up(int&, int&);
	int Down(int&, int&);

public:
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
	int Menu(ADvWindow*, AGmNode*, long);

protected:
	int OverWriteLines(int l_from, int l_to);

protected:
	AGmNode *_project;
};

#endif	/* _AUI_PROJECT_H */

