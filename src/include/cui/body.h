/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AUI_BODY_H
#define _AUI_BODY_H

class AUiAG;

class AL_EXT_CLASS AUiBodyShell : public ADvEditShell {
public:
	AUiBodyShell(ADvWindow*, AGmNode *root_class, AGmNode *class2, AGmNode *member, int for_ui=0);
	virtual ~AUiBodyShell();
	void CreateBody();

	virtual void	Update(int curr_lno = 0);

	virtual int 	Reset(long lno = 0);
	virtual int 	More();
	virtual int 	IsLast();
	virtual int 	Next();

	virtual AGmString* GetLineString();
	virtual AGmString* GetLineStringNext();

	virtual int NumInsertLine() { return _num_insert_line; }

protected:
	virtual int 	_PreInsertLine(const char*);
	virtual int 	_InsertLine(const char*);
	virtual void	_PostInsertLine(const char*);

	virtual int 	_PreReplaceLine(const char*);
	virtual int 	_ReplaceLine(const char*);
	virtual void	_PostReplaceLine(const char*);

	virtual int 	_PreDeleteLine();
	virtual int 	_DeleteLine();

public:
	static int IsSameType(AGmNode*, AGmNode*);
	static int IsDerived(AGmNode *this_class, AGmNode *abst_cl);
	static int IsProtected(long info,
			AGmNode *abst_cl, AGmNode *obj_cl, AGmNode *this_cl);
	static void Overload(
			AGmNode *func_dcl, AGmNode *arg_dcl2, AGmNode*& candidate);

	static int	IsClassFuncType(AGmNode *func_type, AGmNode *arg_dcl2);
	static int	IsObjectFuncType(AGmNode *func_type, AGmNode *arg_dcl2);
	static AGmNode* FuncRetType(AGmNode *func_type);

	static int _IsDerived(AGmNode *this_class, AGmNode *abst_cl);

	static AGmNode* find_loc_var(
			AGmNode *loc_base, AGmNode *name);
	static AGmNode* find_arg_var(
			AGmNode *arg_dcl, AGmNode *name);

	static AGmNode* find_class_var(
			AGmNode *abst_cl, AGmNode *this_cl,
			AGmNode *name, int&);
	static AGmNode* find_obj_var(
			AGmNode *abst_cl, AGmNode *obj_cl, AGmNode *this_cl,
			AGmNode *name, int&);
	static AGmNode* find_class_func(
			AGmNode *abst_cl, AGmNode *this_cl,
			AGmNode *name, AGmNode* arg_dcl2, int&,
			int no_proto = 0, int no_recursive = 0);
	static AGmNode* find_obj_func(
			AGmNode *abst_cl, AGmNode *obj_cl, AGmNode *this_cl,
			AGmNode *name, AGmNode *arg_dcl2, int&,
			int no_proto = 0, int no_recursive = 0);
protected:
// body_in.cpp
	int line_scanf(AUiAG*);
	int exp_scanf(AUiAG*);
	int cond_scanf(AUiAG*);
	int oror_scanf(AUiAG*);
	int andand_scanf(AUiAG*);
	int comp_scanf(AUiAG*);
	int assign_scanf(AUiAG*);
	int add_scanf(AUiAG*);
	int mul_scanf(AUiAG*);
	int or_scanf(AUiAG*);
	int xor_scanf(AUiAG*);
	int and_scanf(AUiAG*);
	int shift_scanf(AUiAG*);
	int unary_scanf(AUiAG*);
	int obj_scanf(AUiAG*);
	int mem_scanf(AUiAG*);
	int prim_scanf(AUiAG*);
	int arg_scanf(AUiAG*, AGmNode *&arg_dcl2);

// body_out.cpp
	int line_printf(AGmString*&);
	int indent(AGmString*&);

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
	int Menu(ADvWindow*, AGmNode*);

public:
	AGmNode* Class() { return _class; }
	AGmNode* Member() { return _member; }
	AGmNode** Prog() { return prog; }

protected:
	AGmNode *_root_class, *_class, *_member;
	AGmFile *_file; AGmString *_line; AUiAG *_a;
	AGmNode *exp, *exp_stack; int _create_text;
	AGmNode **prog, *loc_base, *ctrl_stack;
	int _end;
	int _num_insert_line;
	long ctrl_type; AGmNode **_prog, **ctrl_tail;
	long line_no;

	int line_from, line_to;
	int _num_delete_line;
	int _for_ui;

// for garbage collection
public:
	void GCmark();
};

// ---------- attribute data of attribute grammer ----------
class AUiAG : public AGm {
public:
	AUiAG(AGmFile *f);
	AUiAG(AUiAG *a);
	~AUiAG();

	AGmNode* Head() { return head; }
	AGmNode** Tail() { return p; }
	void Tail(AGmNode** tail) { p = tail; }
	AGmNode* Type() { return type; }
	long Lhs() { return lhs; }
	void Lhs(long kind) { lhs = kind; }
	AGmNode** LhsPos() { return lhs_pos; }
	AGmNode** LhsPos2() { return lhs_pos2; }
	void LhsPos2(AGmNode **pos) { lhs_pos2 = pos; } 
	long Ctrl() { return ctrl; }
	void Ctrl(long c) { ctrl = c; }
	AGmNode** CtrlTail() { return ctrl_tail; }
	void CtrlTail(AGmNode **t) { ctrl_tail = t; }

	int RetType(AGmNode *type, long lhs=0, AGmNode **ptr=NULL);
	int RetType(long type_id, AGmNode *class_id=NULL, long lhs=0, AGmNode **ptr=NULL);
	int Code_i(long i);
	int Code_i_i(long i, long i2);
	int Code_i_n(long i, AGmNode *n);
	int Code_i_n_n(long i, AGmNode *n, AGmNode *n2);

	int ReadIdent(AGmString*&);
	int ReadString(AGmString*&);
	int ReadLine(AGmString*&);
	int ReadMatrix(AGmMatrix*&);

	int ReadHexa(AGmNode*&);
	int ReadInteger(AGmNode*&);
	int ReadReal(AGmNode*&);
	int ReadQChar(AGmNode*&);
	int ReadQString(AGmNode*&);

	int MatchIdent(const char* s);
	int MatchString(const char* s);
	int IsChar(char c);
	int IsEnd();

	void Unput(long d);
#if	_WINDOWS && _64BIT
	long long CurrentPoint();
	void Back(long long);
#else
	long CurrentPoint();
	void Back(long);
#endif

private:
	AGmFile *f;
	AGmNode *head, **p;
	AGmNode *type; long lhs; AGmNode **lhs_pos, **lhs_pos2;
	long ctrl; AGmNode **ctrl_tail;
};

#define AUI_LHS_LOC 		(long)10
#define AUI_LHS_ARG 		(long)20
#define AUI_LHS_CLASS_VAR	(long)30
#define AUI_LHS_THIS_VAR	(long)40
#define AUI_LHS_OBJ_VAR 	(long)50
#define AUI_LHS_LIST_HEAD	(long)60
#define AUI_LHS_LIST_TAIL	(long)70
#define AUI_LHS_MATRIX		(long)80
#define AUI_LHS_ARRAY		(long)90
#define AUI_LHS_VARIANT 	(long)100

#define AUI_CTRL_BLOCK		(long)200
#define AUI_CTRL_BLOCK2 	(long)201
#define AUI_CTRL_LOOP		(long)210
#define AUI_CTRL_LOOP2		(long)211
#define AUI_CTRL_BRANCH 	(long)220
#define AUI_CTRL_BRANCH2	(long)221
#define AUI_CTRL_BRANCH3	(long)222
#define AUI_CTRL_TRY		(long)223
#define AUI_CTRL_TRY2		(long)224
#define AUI_CTRL_TRY3		(long)225

#define AUI_PRI_COND	(long)400
#define AUI_PRI_OROR	(long)410
#define AUI_PRI_ANDAND	(long)420

#define AUI_PRI_COMP	(long)500
#define AUI_PRI_ASSIGN	(long)510
#define AUI_PRI_ADD 	(long)520
#define AUI_PRI_MUL 	(long)530
#define AUI_PRI_OR		(long)540
#define AUI_PRI_XOR 	(long)550
#define AUI_PRI_AND 	(long)560
#define AUI_PRI_SHIFT	(long)570
#define AUI_PRI_PRIM	(long)580

#endif	/* _AUI_BODY_H */

