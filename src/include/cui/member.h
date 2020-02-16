/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AUI_MEMBER_H
#define _AUI_MEMBER_H

AL_EXT_DATA extern int AUiVtblUpdate;

class AL_EXT_CLASS AUiMemberShell : public ADvEditShell {
public:
	AUiMemberShell(ADvWindow*, AGmNode *root_class, AGmNode *class2, int for_ui=0);
	virtual ~AUiMemberShell();

	virtual int 	Reset(long lno = 0);
	virtual int 	More();
	virtual int 	IsLast();
	virtual int 	Next();

	virtual AGmString* GetLineString();

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

protected:
	int member_scanf(AGmFile*, AGmNode*& name, AGmNode*& _member);
public:
	static void member_printf(AGmString*&, AGmNode *_class, AGmNode *_member);
	static void _member_printf0(AGmString*&, AGmNode *_class, AGmNode *_member);
	static void _member_printf1(AGmString*&, AGmNode *_class, AGmNode *_member);
	static void _member_printf2(AGmString*&, AGmNode *_class, AGmNode *_member);
protected:
	int UniquenessCheck();
	int SameProtoMember();
	AGmNode*	same_proto_member;

public:
	static void update_vfunc_tbl(AGmNode *root_class, AGmNode *_class, AGmNode *_member);
	static void _update_vfunc_tbl1(AGmNode *tbl, AGmNode *func_dcl2, AGmNode *def_cl,
				AGmNode *name, AGmNode *arg_dcl2);
	static void _update_vfunc_tbl2(AGmNode *&tbl, AGmNode *&func_dcl2, AGmNode *def_cl,
				AGmNode *name, AGmNode *arg_dcl2);	
	static void CleanVtbl(AGmNode*);
	static void UpdateVtbl(AGmNode*, AGmNode*);
	static int	ReferedFromOutside(AGmLink*);

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
	AGmNode* Class() { return _class; }
	AGmNode* Member() { return _member; }

protected:
	AGmNode *_root_class;
	AGmNode *_class, *_member;
	AGmFile *_file; AGmString *_line;
	AGmNode *_name1, *_member1;
	int _end;
	int _for_ui;

// for garbage collection
public:
	void GCmark();
};

#define AUI_PUBLIC		AIP_PUBLIC
#define AUI_PROTECTED	AIP_PROTECTED
#define AUI_PRIVATE 	AIP_PRIVATE
#define AUI_FUNC_TYPE	AIP_FUNC_TYPE
#define AUI_STATIC		AIP_STATIC
#define AUI_FUNC		AIP_FUNC

#endif	/* _AUI_MEMBER_H */

