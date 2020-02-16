/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AUI_CLASS_H
#define _AUI_CLASS_H

class AL_EXT_CLASS AUiClassShell : public ADvEditShell {
public:
	AUiClassShell(ADvWindow*, AGmNode *root, int for_ui=0);
	virtual ~AUiClassShell();

	virtual int 	Reset(long lno = 0);
	virtual int 	More();
	virtual int 	IsLast();
	virtual int 	Next();

	virtual AGmString* GetLineString();

	virtual int 	NumOverWriteLine() { return _num_overwrite_line; }

protected:
	virtual int 	_PreInsertLine(const char*);
	virtual int 	_InsertLine(const char*);
	virtual void	_PostInsertLine(const char*);

	virtual int 	_PreReplaceLine(const char*);
	virtual int 	_ReplaceLine(const char*);
	virtual void	_PostReplaceLine(const char*);

	virtual int 	_PreDeleteLine();
	virtual int 	_DeleteLine();

	virtual int 	_PreOpenEditor();
	virtual int 	_OpenEditor();

public:
	int Up(int&, int&);
	int Down(int&, int&);
	int Left(int&, int&);
	int Right(int&, int&);

public:
	static AGmNode* NameToClass(AGmNode* node, const char* name);
	static AGmNode* NameToClass(AGmNode* node, const AGmString* name);
	static AGmNode* NameToClass(AGmNode* node, const AGmNode* name);
	static AGmNode* NameToClassFuncType(AGmNode* node, const char* name);
	static AGmNode* NameToClassFuncType(AGmNode* node, const AGmString* name);
	static AGmNode* NameToClassFuncType(AGmNode* node, const AGmNode* name);
	static AGmNode* NameToObjFuncType(AGmNode* node, const char* name);
	static AGmNode* NameToObjFuncType(AGmNode* node, const AGmString* name);
	static AGmNode* NameToObjFuncType(AGmNode* node, const AGmNode* name);
	static AGmNode* TypeToName(AGmNode* type);
	static AGmNode* NameToType(AGmNode* node, const AGmString* type);
	static AGmNode* NameToType(AGmNode* node, const AGmNode* type);
	static AGmNode* NameToType(AGmNode* node, const char* type);
	static int	LinkClassObj(AGmNode* root_class, AGmNode*);
	static int	_LinkClassObj(AGmNode* root_class, AGmNode*);
	static int	NumDescendant(AGmNode*);
	static AGmArc* ClassOfMember(AGmNode*);
	static	int ReferedFromOutside(AGmLink*);
	static	int ReferToOutside(AGmLink*);

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

public:
	AGmNode* Class();
	void CreateAsLastChild();

// for garbage collection
public:
	void GCmark();

protected:
	AGmNode *_root_class, *_class_stack;
	int _create_pos;

	AGmFile *_file; AGmString *_line;
	AGmNode *_name;

	int _num_overwrite_line;
	int _num_delete_line;
	int _for_ui;
};

#define AUI_TYPE_INTEGER	AIP_TYPE_INTEGER
#define AUI_TYPE_REAL		AIP_TYPE_REAL
#define AUI_TYPE_LIST		AIP_TYPE_LIST
#define AUI_TYPE_ARRAY		AIP_TYPE_ARRAY
#define AUI_TYPE_DST_ITR	AIP_TYPE_DST_ITR
#define AUI_TYPE_SRC_ITR	AIP_TYPE_SRC_ITR
#define AUI_TYPE_STRING 	AIP_TYPE_STRING
#define AUI_TYPE_MATRIX 	AIP_TYPE_MATRIX
#define AUI_TYPE_FILE		AIP_TYPE_FILE
#define AUI_TYPE_WINDOW 	AIP_TYPE_WINDOW
#define AUI_TYPE_VARIANT	AIP_TYPE_VARIANT
#define AUI_TYPE_VOID		AIP_TYPE_VOID
#define AUI_TYPE_CLASS		AIP_TYPE_CLASS
#define AUI_TYPE_CLASS_FUNC AIP_TYPE_CLASS_FUNC
#define AUI_TYPE_OBJ_FUNC	AIP_TYPE_OBJ_FUNC

typedef void (*ReflectFunc)(AGmNode* target, AGmNode* data1, AGmNode *data2);
void AGmForEachClass(AGmNode *root_class,
	ReflectFunc func, AGmNode *data1, AGmNode *data2);
void AGmForEachMember(AGmNode *class_node,
	ReflectFunc func, AGmNode *data1, AGmNode *data2);
int AGmIsStatic(AGmNode *func_dcl);
int AGmIsFunc(AGmNode *func_dcl);
int AGmFuncDclToClassAndFunc(AGmNode *func_dcl, AGmNode*& class_node, AGmArc*& func_arc);

void AGmEnterProf(AGmNode *func_stack, AGmNode *func_dcl);
void AGmLeaveProf(AGmNode *func_stack, AGmNode *func_dcl);

#endif	/* _AUI_CLASS_H */

