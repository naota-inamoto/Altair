/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AIP_INTERP_H
#define _AIP_INTERP_H

// ----- return value of Execute
#define AIP_EXIT	-1
#define AIP_NEXT	-2
#define AIP_PROCESS -3

class CMenu;
class AIpProcessMgr;

class AL_EXT_CLASS AIpProcess : public AGmPict {
public:
	AIpProcess(AIpProcessMgr*, AGmNode *root_class, AGmNode *arg, AGmNode *prog, AGmNode *this_p=NULL);
	virtual ~AIpProcess();

	virtual int Execute();
	virtual int ExecuteException(AIpProcess*);

public:
	AGmNode* RootClass();
	AGmLink* Link();
protected:
	AIpProcessMgr *_prcsmgr;
	AGmNode *_root_class;
	AGmLink *_link;

public:
	AGmNode *Val();
	void	Val(AGmNode*);
	int 	IsException();
	void	ReturnProcess(AIpProcess *p);
	AIpProcess* ReturnProcess();

	int 	IsWaiting();
	void	Wait();
	void	Notify();
	AGmNode*	notify_cells;
protected:
	AGmNode *val, *val_stack;
	AGmNode *this_ptr, *prog;
	AGmNode *arg_base, *loc_base;
	AGmNode *ctrl_stack, *func_stack, *func_stack_ls;
	AGmNode *this_ptr_ls, *arg_base_ls, *loc_base_ls, *ctrl_stack_ls;
	int exception;
	AIpProcess *return_process;

	int waiting;

public:
	int ParaLock();
	void ParaLock(int);
protected:
	int para_lock;

public:
	static int InitFuncTbl();
	static void ReleaseFuncTbl();
protected:
	static int	_init_tbl;
	static struct	_AIpFuncTbl *evalFuncTbl;
	static struct	_AIpFuncTbl *retFuncTbl;
	static struct	_AIpFuncTbl *sysFuncTbl;
	static char**	sysFuncNameTbl;
	static int* sysFuncNumArgTbl;

protected:
// control
	int EvalBlock();
	int RetBlock();

	int EvalLocVarDcl();

	int EvalBranch();
	int RetBranch();

	int EvalLoop();
	int RetLoop();
	int EvalBreak();
	int EvalCont();

// exception handling
	int EvalTry();
	int RetTry();
	int EvalThrow();

// compare operatopn
	int EvalEQ();
	int EvalNE();
	int EvalLT();
	int EvalLE();
	int EvalGE();
	int EvalGT();

// +,-,*,/,% operatopn	
	int EvalAdd();
	int EvalSub();
	int EvalMul();
	int EvalDiv();
	int EvalMod();

// logical operation
	int EvalOr();
	int EvalXor();
	int EvalAnd();

// shift operation
	int EvalSL();
	int EvalSR();

// 1 term operation
	int EvalChgSign();
	int EvalInverse();

// 2 term operation argument check
	void	GetArg();
	AGmNode *arg1, *arg2;

// read
	int EvalConst();
	int EvalConstFunc();
	int EvalConstVtbl();
	int EvalNew();
	int EvalThis();
	int EvalLoc();
	int EvalArg();
	int EvalThisVar();
	int EvalClassVar();
	int EvalObjVar();
	int EvalHead();
	int EvalTail();
	int EvalArray();
	int EvalMatrix();
	int EvalCast();
	int EvalRuntime();

// write
	int EvalSetLoc();
	int EvalSetArg();	
	int EvalSetThisVar();
	int EvalSetClassVar();
	int EvalSetObjVar();
	int EvalSetHead();
	int EvalSetTail();
	int EvalSetArray();
	int EvalSetMatrix();

public:
// search variable
	static int SearchVar(AGmNode*, AGmNode*, AGmNode**&);

	static long 	SearchSysFunc(const char *);
	static char*	SysFuncName(long index);
	static int		SysFuncNumArg(long index);

protected:
// function call
	int EvalPush();
	int EvalPop();
	int EvalClassFunc();
	int EvalThisFunc();
	int EvalObjFunc();
	int EvalObjFunc2();
	int EvalClassFuncP();
	int EvalThisFuncP();
	int EvalObjFuncP();
	int EvalRet();
	int EvalSysFunc();
// parallel processing
	int EvalParaClassFunc();
	int EvalParaObjFunc();

// debug
public:
	int interp_error(const char *);
	void StackTrace();
	void StackTrace(AGmNode *func_stack);
	void StackTrace(AGmNode *func_stack, AGmFile *f);
	void ErrorSource();
	void ErrorSource(AGmNode *func_stack, AGmNode *prog);
	void ErrorSource(AGmNode *func_stack, AGmNode *prog, AGmFile *f);
	void _BaseFuncName(long func_no);

protected:
// arithmetic operation
	int EvalSqrt();
	int EvalSin();
	int EvalCos();
	int EvalTan();
	int EvalASin();
	int EvalACos();
	int EvalATan();
	int EvalExp();
	int EvalLog();
	int EvalMax();
	int EvalMin();
	int EvalAbs();
	int EvalMathMisc();

// list manipulation operation
	int EvalCons();
	int EvalAppendL();
	int EvalList2();
	int EvalList3();
	int EvalList4();
	int EvalList5();
	int EvalList6();
	int EvalList7();
	int EvalList8();
	int EvalScanList();
	int EvalPrintList();
	int EvalListElem();
	int EvalSetListElem();
	int EvalListMisc();
	
// array manipulation opration
	int EvalCreateArray();
	int EvalArrayLength();

// graph manipulation operation
	int EvalDstItr();
	int EvalSrcItr();
	int EvalNext();
	int EvalNextA();
	int EvalNextA2();
	int EvalPrev();
	int EvalPrevA();
	int EvalPrevA2();
	int EvalCompo();
	int EvalInsert();
	int EvalAppend();
	int EvalRemove();
	int EvalGetArcA();
	int EvalSetArcA();
	int EvalCount();
	int EvalCount2();
	int EvalIndexPos();
	int EvalSearch();

	int EvalCreateArc();
	int EvalCreateAssocArc();
	int EvalCreateOrderedArc();
	int EvalDstNode();
	int EvalDstNodeNoCase();
	int EvalSrcNode();
	int EvalSetDstNode();
	int EvalNextDstNode();
	int EvalPrevDstNode();
	int EvalNextSrcNode();
	int EvalPrevSrcNode();
	int EvalInsertBefore();
	int EvalAppendAfter();
	int EvalRemoveFrom();

	int EvalCopy();

	int EvalDstItr2();
	int EvalArcDst();
	int EvalSrcItr2();
	int EvalArcSrc();

	int EvalGraphMisc();

// PolyPoly data manipulation operation
	int EvalCreatePolyPoly();
	int EvalCreateNextPoly();
	int EvalAddPointToPoly();
	int EvalAddRectToPoly();
	int EvalAddCurveToPoly();
	int EvalIntersection();
	int EvalUnion();

// Matrix data manipulation operation
	int EvalUnitTM();
	int EvalTranslateTM();
	int EvalScaleTM();
	int EvalRotateTM();
	int EvalTransformXY();
	int EvalInverseXY();
	int EvalTransformPoly();
	int EvalInversePoly();
	int EvalTransformRect();

// graphics operation
	int EvalPrint();
	int EvalAsk();
	int EvalMenu();
	int EvalGetReadFileName();
	int EvalGetWriteFileName();
public:
	static int GetFileName(int is_open, AGmString*&, AGmString*&, const char* filter = NULL,
		const char *title = NULL, const char *initial_file = NULL,
		const char *def_ext = NULL, const char *inital_dir = NULL);
protected:
	int EvalGetColor();
	int EvalGetFont();
	int EvalMessageText();
	int EvalInputByCEdit();

	int EvalCreateWindow();
	int EvalUserEdit();
	int EvalSetWindowArg();
	int EvalSetDrawProc();
	int EvalSetEventProc();
	int EvalSetMoveCtrlProc();
	int EvalSetDestroyProc();
	int EvalSetActivateProc();
	int EvalSetTextInsProc();
	int EvalSetTitle();
	int EvalGetTitle();
	int EvalSetMenu();
public:
	int CreateMenu(CMenu*, AGmNode*);
#ifdef	_WINDOWS
	static AGmNode *SearchMenu(AGmNode*, long);
	static void DestroyMenu(AGmNode*);
#endif

protected:
	int EvalGetWndRect();
	int EvalSetWndRect();
	int EvalResizeWnd();
	int EvalMoveWnd();
	int EvalShowWnd();
	int EvalHideWnd();
	int EvalDestroyWnd();

	int EvalPushClipping();
	int EvalPopClipping();
	int EvalClipping();
	int EvalIsClipped();
	int EvalPushBgColor();
	int EvalPopBgColor();
	int EvalPushColorFunc();
	int EvalPopColorFunc();
	int EvalGetSysColor();
	int EvalPushPen();
	int EvalPopPen();
	int EvalPushBrush();
	int EvalPopBrush();
	int EvalPushFont();
	int EvalPushFontEx();
	int EvalPopFont();
	int EvalPushTM();
	int EvalPopTM();
	int EvalWindowTM();
	int EvalIsPrinting();

	int EvalStartQuickDraw();
	int EvalEndQuickDraw();

	int EvalPoint();
	int EvalLine();
	int EvalRectangle();
	int EvalEllipse();
	int EvalPolyPolyline();
	int EvalPolyPolygon();
	int EvalTextOut();
	int EvalTextExtent();
	int EvalTextOutFromTo();
	int EvalTextExtentFromTo();
	int EvalMTextOut();
	int EvalPie();
	int EvalBitmap();
	int EvalImage();

	int EvalDamage();
	int EvalDamageRectPoly();
	int EvalIsDamaged();
	int EvalRepair();
	int EvalRedraw();

	int EvalLocalLoop();
	int EvalExitLoop();

	int EvalGetId();
	int EvalReleaseId();

	int EvalFilePrint();

	int EvalGuiMisc();

// file manipulation operation
	int EvalFileOpen();
	int EvalFileRead();
	int EvalFileWrite();
	int EvalFileManip();
	int EvalFileMatchStr();

protected:
// string manipulation operation
	int EvalStrLen();
	int EvalGetChar();
	int EvalSetChar();
	int EvalSubStr();
	int EvalMatchStr();
	int EvalTailStr();
	int EvalInsertChar();
	int EvalDeleteChar();
	int EvalSearchStr();
	int EvalReplaceStr();
	int EvalAppendStr();
	int EvalIsFirstChar();
	int EvalIsNormalChar();
	int EvalStrMisc();

// dynamic type check operation
	int EvalIsType();
	int EvalAddrEq();
	int EvalIsClass();
	int EvalIsDerived();
	int _EvalIsDerived(AGmNode *obj_cl, AGmNode *class_id);

// GP interface
	int EvalGraphProg();

// parallel processing
	int EvalNextProcess();
	int EvalExitProcess();

	int EvalWaitProcess();
	int EvalNotifyProcess();

// clip board
	int EvalSetToClipboard();
	int EvalGetFromClipboard();

// garbage collection
	int EvalGC();

// application execution
	int EvalExec();

// library management
	int EvalLibrary();

// window message
	int EvalWndMessage();

// Altair script
	int EvalScript();
public:
	void EvalScript(AGmNode*, AGmNode*);

protected:
// XML
	int EvalXML();

// sql
	int EvalSQL();

// Socket
	int EvalSocket();

// encription
	int EvalCrypt();

// dynamic link library load
	int EvalUseDll();

// Profiling
	int EvalProf();

// Solid Modeling
	int EvalSolid();

// Japanese Natural Language Manipulation
	int EvalJpAna();

// Symbolic Manipulation
	int EvalSymbolManip();

// misc
	int EvalMisc();

// OCX
	int EvalOcx();
	int EvalOcxInvoke();
	int EvalOcxSetProp();
	int EvalOcxSetMethod();
	int EvalOcxInvokeArg();
	int EvalOcxFireEvent();
	int EvalRootClass();
	int EvalOleMisc();

// argument evaluation
	int GetArg(AGmNode*& arg1);
	int GetArg(AGmNode*& arg1, AGmNode*& arg2);
	int GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3);
	int GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4);
	int GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4,
					AGmNode*& arg5);
	int GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4,
					AGmNode*& arg5, AGmNode*& arg6);
	int GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4,
					AGmNode*& arg5, AGmNode*& arg6, AGmNode*& arg7);
	int GetArg(AGmNode*& arg1, AGmNode*& arg2, AGmNode*& arg3, AGmNode*& arg4,
					AGmNode*& arg5, AGmNode*& arg6, AGmNode*& arg7, AGmNode*& arg8);

// call Listener Function
public:
	AGmNode* CallListener(AGmNode *obj, AGmNode *func_dcl, AGmNode *args);

// for garbage collection
public:
	void GCmark();

#ifdef	_DEBUG
	static	int eval_ret_func_name_trace;
#endif

};

typedef struct	_AIpFuncTbl {
	int (AIpProcess::*func)();
#ifdef _DEBUG
	char *name;
#endif
} AIpFuncTbl; 

#define AIP_TYPE_INTEGER	(long)AGM_NODE_INTEGER
#define AIP_TYPE_REAL		(long)AGM_NODE_REAL
#define AIP_TYPE_LIST		(long)AGM_NODE_LIST
#define AIP_TYPE_ARRAY		(long)AGM_NODE_ARRAY
#define AIP_TYPE_DST_ITR	(long)AGM_NODE_DST_ITR
#define AIP_TYPE_SRC_ITR	(long)AGM_NODE_SRC_ITR
#define AIP_TYPE_STRING 	(long)AGM_NODE_STRING
#define AIP_TYPE_MATRIX 	(long)AGM_NODE_MATRIX
#define AIP_TYPE_FILE		(long)AGM_NODE_FILE
#define AIP_TYPE_WINDOW 	(long)AGM_NODE_WINDOW
#define AIP_TYPE_VARIANT	(long)AGM_NODE_VARIANT
#define AIP_TYPE_VOID		(long)100
#define AIP_TYPE_CLASS		(long)101
#define AIP_TYPE_CLASS_FUNC (long)102
#define AIP_TYPE_OBJ_FUNC	(long)103

#define AIP_PUBLIC		0x01
#define AIP_PROTECTED	0x02
#define AIP_PRIVATE 	0x04
#define AIP_FUNC_TYPE	0x08
#define AIP_STATIC		0x10
#define AIP_FUNC		0x20

#define AIP_STEP_CLASS	1
#define AIP_STEP_MEMBER 2
#define AIP_STEP_BODY	3

AL_EXT_API int AIpEvalClassFunc(AGmNode *root_class,
	AGmNode *class_name, AGmNode *func_name, AGmNode *args = NULL);

#endif	/* _AIP_INTERP_H */

