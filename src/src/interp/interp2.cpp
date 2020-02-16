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
#include <gdm/matrix.h>
#include <gdm/file.h>
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <interp/basefunc.h>
#include <interp/sysfunc.h>
#include <interp/prog_if.h>
#include <dev/advwnd.h>
#include <dev/shell.h>
#include <cui/class.h>
#include <cui/body.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

int AIpProcess::_init_tbl = 0;
AIpFuncTbl	*AIpProcess::evalFuncTbl;
AIpFuncTbl	*AIpProcess::retFuncTbl;
AIpFuncTbl	*AIpProcess::sysFuncTbl;
char**		AIpProcess::sysFuncNameTbl;
int*		AIpProcess::sysFuncNumArgTbl;

int AIpProcess::InitFuncTbl() {
	if (_init_tbl) return 0;
	evalFuncTbl = new AIpFuncTbl[AIP_NUM_EVAL];

	evalFuncTbl[AIP_EvalBlock].func 	  = &AIpProcess::EvalBlock;
	evalFuncTbl[AIP_EvalLocVarDcl].func   = &AIpProcess::EvalLocVarDcl;
	evalFuncTbl[AIP_EvalBranch].func	  = &AIpProcess::EvalBranch;
	evalFuncTbl[AIP_EvalLoop].func		  = &AIpProcess::EvalLoop;
	evalFuncTbl[AIP_EvalBreak].func 	  = &AIpProcess::EvalBreak;
	evalFuncTbl[AIP_EvalCont].func		  = &AIpProcess::EvalCont;
	evalFuncTbl[AIP_EvalCond].func		  = &AIpProcess::EvalBranch;
	evalFuncTbl[AIP_EvalOrOr].func		  = &AIpProcess::EvalBranch;
	evalFuncTbl[AIP_EvalAndAnd].func	  = &AIpProcess::EvalBranch;
	evalFuncTbl[AIP_EvalTry].func		  = &AIpProcess::EvalTry;
	evalFuncTbl[AIP_EvalThrow].func 	  = &AIpProcess::EvalThrow;

	evalFuncTbl[AIP_EvalEQ].func		  = &AIpProcess::EvalEQ;
	evalFuncTbl[AIP_EvalNE].func		  = &AIpProcess::EvalNE;
	evalFuncTbl[AIP_EvalLT].func		  = &AIpProcess::EvalLT;
	evalFuncTbl[AIP_EvalLE].func		  = &AIpProcess::EvalLE;
	evalFuncTbl[AIP_EvalGE].func		  = &AIpProcess::EvalGE;
	evalFuncTbl[AIP_EvalGT].func		  = &AIpProcess::EvalGT;

	evalFuncTbl[AIP_EvalAdd].func		  = &AIpProcess::EvalAdd;
	evalFuncTbl[AIP_EvalSub].func		  = &AIpProcess::EvalSub;
	evalFuncTbl[AIP_EvalMul].func		  = &AIpProcess::EvalMul;
	evalFuncTbl[AIP_EvalDiv].func		  = &AIpProcess::EvalDiv;
	evalFuncTbl[AIP_EvalMod].func		  = &AIpProcess::EvalMod;

	evalFuncTbl[AIP_EvalOr].func		  = &AIpProcess::EvalOr;
	evalFuncTbl[AIP_EvalXor].func		  = &AIpProcess::EvalXor;
	evalFuncTbl[AIP_EvalAnd].func		  = &AIpProcess::EvalAnd;
	evalFuncTbl[AIP_EvalSL].func		  = &AIpProcess::EvalSL;
	evalFuncTbl[AIP_EvalSR].func		  = &AIpProcess::EvalSR;

	evalFuncTbl[AIP_EvalChgSign].func	  = &AIpProcess::EvalChgSign;
	evalFuncTbl[AIP_EvalInverse].func	  = &AIpProcess::EvalInverse;

	evalFuncTbl[AIP_EvalConstInt].func	  = &AIpProcess::EvalConst;
	evalFuncTbl[AIP_EvalConstHexa].func   = &AIpProcess::EvalConst;
	evalFuncTbl[AIP_EvalConstQChar].func  = &AIpProcess::EvalConst;
	evalFuncTbl[AIP_EvalConstReal].func   = &AIpProcess::EvalConst;
	evalFuncTbl[AIP_EvalConstString].func = &AIpProcess::EvalConst;
	evalFuncTbl[AIP_EvalConstVtbl].func   = &AIpProcess::EvalConstVtbl;
	evalFuncTbl[AIP_EvalConstFunc].func   = &AIpProcess::EvalConstFunc;
	evalFuncTbl[AIP_EvalConstNull].func   = &AIpProcess::EvalConst;

	evalFuncTbl[AIP_EvalNew].func		  = &AIpProcess::EvalNew;
	evalFuncTbl[AIP_EvalThis].func		  = &AIpProcess::EvalThis;
	evalFuncTbl[AIP_EvalLoc].func		  = &AIpProcess::EvalLoc;
	evalFuncTbl[AIP_EvalLoc].func		  = &AIpProcess::EvalLoc;
	evalFuncTbl[AIP_EvalArg].func		  = &AIpProcess::EvalArg;
	evalFuncTbl[AIP_EvalThisVar].func	  = &AIpProcess::EvalThisVar;
	evalFuncTbl[AIP_EvalClassVar].func	  = &AIpProcess::EvalClassVar;
	evalFuncTbl[AIP_EvalObjVar].func	  = &AIpProcess::EvalObjVar;
	evalFuncTbl[AIP_EvalHead].func		  = &AIpProcess::EvalHead;
	evalFuncTbl[AIP_EvalTail].func		  = &AIpProcess::EvalTail;
	evalFuncTbl[AIP_EvalArray].func 	  = &AIpProcess::EvalArray;
	evalFuncTbl[AIP_EvalMatrix].func	  = &AIpProcess::EvalMatrix;
	evalFuncTbl[AIP_EvalCast].func		  = &AIpProcess::EvalCast;
	evalFuncTbl[AIP_EvalRuntime].func	  = &AIpProcess::EvalRuntime;

	evalFuncTbl[AIP_EvalSetLoc].func	  = &AIpProcess::EvalSetLoc;
	evalFuncTbl[AIP_EvalSetArg].func	  = &AIpProcess::EvalSetArg;
	evalFuncTbl[AIP_EvalSetThisVar].func  = &AIpProcess::EvalSetThisVar;
	evalFuncTbl[AIP_EvalSetClassVar].func = &AIpProcess::EvalSetClassVar;
	evalFuncTbl[AIP_EvalSetObjVar].func   = &AIpProcess::EvalSetObjVar;
	evalFuncTbl[AIP_EvalSetHead].func	  = &AIpProcess::EvalSetHead;
	evalFuncTbl[AIP_EvalSetTail].func	  = &AIpProcess::EvalSetTail;
	evalFuncTbl[AIP_EvalSetArray].func	  = &AIpProcess::EvalSetArray;
	evalFuncTbl[AIP_EvalSetMatrix].func   = &AIpProcess::EvalSetMatrix;

	evalFuncTbl[AIP_EvalPush].func		  = &AIpProcess::EvalPush;
	evalFuncTbl[AIP_EvalPop].func		  = &AIpProcess::EvalPop;

	evalFuncTbl[AIP_EvalClassFunc].func   = &AIpProcess::EvalClassFunc;
	evalFuncTbl[AIP_EvalThisFunc].func	  = &AIpProcess::EvalThisFunc;
	evalFuncTbl[AIP_EvalObjFunc].func	  = &AIpProcess::EvalObjFunc;
	evalFuncTbl[AIP_EvalObjFunc2].func	  = &AIpProcess::EvalObjFunc2;
	evalFuncTbl[AIP_EvalClassFuncP].func  = &AIpProcess::EvalClassFuncP;
	evalFuncTbl[AIP_EvalThisFuncP].func   = &AIpProcess::EvalThisFuncP;
	evalFuncTbl[AIP_EvalObjFuncP].func	  = &AIpProcess::EvalObjFuncP;

	evalFuncTbl[AIP_EvalRet].func		  = &AIpProcess::EvalRet;
	evalFuncTbl[AIP_EvalSysFunc].func	  = &AIpProcess::EvalSysFunc;

	evalFuncTbl[AIP_EvalParaClassFunc].func   = &AIpProcess::EvalParaClassFunc;
	evalFuncTbl[AIP_EvalParaObjFunc].func	  = &AIpProcess::EvalParaObjFunc;

	evalFuncTbl[AIP_EvalOcxInvoke].func   = &AIpProcess::EvalOcxInvoke;
	evalFuncTbl[AIP_EvalOcxSetProp].func  = &AIpProcess::EvalOcxSetProp;

#ifdef	_DEBUG
	evalFuncTbl[AIP_EvalBlock].name 	  = "EvalBlock";
	evalFuncTbl[AIP_EvalLocVarDcl].name   = "EvalLocVarDcl";
	evalFuncTbl[AIP_EvalBranch].name	  = "EvalBranch";
	evalFuncTbl[AIP_EvalLoop].name		  = "EvalLoop";
	evalFuncTbl[AIP_EvalBreak].name 	  = "EvalBreak";
	evalFuncTbl[AIP_EvalCont].name		  = "EvalCont";
	evalFuncTbl[AIP_EvalCond].name		  = "EvalBranch";
	evalFuncTbl[AIP_EvalOrOr].name		  = "EvalBranch";
	evalFuncTbl[AIP_EvalAndAnd].name	  = "EvalBranch";
	evalFuncTbl[AIP_EvalTry].name		  = "EvalTry";
	evalFuncTbl[AIP_EvalThrow].name 	  = "EvalThrow";

	evalFuncTbl[AIP_EvalEQ].name		  = "EvalEQ";
	evalFuncTbl[AIP_EvalNE].name		  = "EvalNE";
	evalFuncTbl[AIP_EvalLT].name		  = "EvalLT";
	evalFuncTbl[AIP_EvalLE].name		  = "EvalLE";
	evalFuncTbl[AIP_EvalGE].name		  = "EvalGE";
	evalFuncTbl[AIP_EvalGT].name		  = "EvalGT";

	evalFuncTbl[AIP_EvalAdd].name		  = "EvalAdd";
	evalFuncTbl[AIP_EvalSub].name		  = "EvalSub";
	evalFuncTbl[AIP_EvalMul].name		  = "EvalMul";
	evalFuncTbl[AIP_EvalDiv].name		  = "EvalDiv";
	evalFuncTbl[AIP_EvalMod].name		  = "EvalMod";

	evalFuncTbl[AIP_EvalOr].name		  = "EvalOr";
	evalFuncTbl[AIP_EvalXor].name		  = "EvalXor";
	evalFuncTbl[AIP_EvalAnd].name		  = "EvalAnd";
	evalFuncTbl[AIP_EvalSL].name		  = "EvalSL";
	evalFuncTbl[AIP_EvalSR].name		  = "EvalSR";

	evalFuncTbl[AIP_EvalChgSign].name	  = "EvalChgSign";
	evalFuncTbl[AIP_EvalInverse].name	  = "EvalInverse";

	evalFuncTbl[AIP_EvalConstInt].name	  = "EvalConst";
	evalFuncTbl[AIP_EvalConstHexa].name   = "EvalConst";
	evalFuncTbl[AIP_EvalConstQChar].name  = "EvalConst";
	evalFuncTbl[AIP_EvalConstReal].name   = "EvalConst";
	evalFuncTbl[AIP_EvalConstString].name = "EvalConst";
	evalFuncTbl[AIP_EvalConstVtbl].name   = "EvalConstVtbl";
	evalFuncTbl[AIP_EvalConstFunc].name   = "EvalConstFunc";
	evalFuncTbl[AIP_EvalConstNull].name   = "EvalConst";

	evalFuncTbl[AIP_EvalNew].name		  = "EvalNew";
	evalFuncTbl[AIP_EvalThis].name		  = "EvalThis";
	evalFuncTbl[AIP_EvalLoc].name		  = "EvalLoc";
	evalFuncTbl[AIP_EvalLoc].name		  = "EvalLoc";
	evalFuncTbl[AIP_EvalArg].name		  = "EvalArg";
	evalFuncTbl[AIP_EvalThisVar].name	  = "EvalThisVar";
	evalFuncTbl[AIP_EvalClassVar].name	  = "EvalClassVar";
	evalFuncTbl[AIP_EvalObjVar].name	  = "EvalObjVar";
	evalFuncTbl[AIP_EvalHead].name		  = "EvalHead";
	evalFuncTbl[AIP_EvalTail].name		  = "EvalTail";
	evalFuncTbl[AIP_EvalArray].name 	  = "EvalArray";
	evalFuncTbl[AIP_EvalMatrix].name	  = "EvalMatrix";
	evalFuncTbl[AIP_EvalCast].name		  = "EvalCast";
	evalFuncTbl[AIP_EvalRuntime].name	  = "EvalRuntime";

	evalFuncTbl[AIP_EvalSetLoc].name	  = "EvalSetLoc";
	evalFuncTbl[AIP_EvalSetArg].name	  = "EvalSetArg";
	evalFuncTbl[AIP_EvalSetThisVar].name  = "EvalSetThisVar";
	evalFuncTbl[AIP_EvalSetClassVar].name = "EvalSetClassVar";
	evalFuncTbl[AIP_EvalSetObjVar].name   = "EvalSetObjVar";
	evalFuncTbl[AIP_EvalSetHead].name	  = "EvalSetHead";
	evalFuncTbl[AIP_EvalSetTail].name	  = "EvalSetTail";
	evalFuncTbl[AIP_EvalSetArray].name	  = "EvalSetArray";
	evalFuncTbl[AIP_EvalSetMatrix].name   = "EvalSetMatrix";

	evalFuncTbl[AIP_EvalPush].name		  = "EvalPush";
	evalFuncTbl[AIP_EvalPop].name		  = "EvalPop";

	evalFuncTbl[AIP_EvalClassFunc].name   = "EvalClassFunc";
	evalFuncTbl[AIP_EvalThisFunc].name	  = "EvalThisFunc";
	evalFuncTbl[AIP_EvalObjFunc].name	  = "EvalObjFunc";
	evalFuncTbl[AIP_EvalObjFunc2].name	  = "EvalObjFunc2";
	evalFuncTbl[AIP_EvalClassFuncP].name  = "EvalClassFuncP";
	evalFuncTbl[AIP_EvalThisFuncP].name   = "EvalThisFuncP";
	evalFuncTbl[AIP_EvalObjFuncP].name	  = "EvalObjFuncP";

	evalFuncTbl[AIP_EvalRet].name		  = "EvalRet";
	evalFuncTbl[AIP_EvalSysFunc].name	  = "EvalSysFunc";

	evalFuncTbl[AIP_EvalParaClassFunc].name   = "EvalParaClassFunc";
	evalFuncTbl[AIP_EvalParaObjFunc].name	  = "EvalParaObjFunc";

	evalFuncTbl[AIP_EvalOcxInvoke].name   = "EvalOcxInvoke";
	evalFuncTbl[AIP_EvalOcxSetProp].name  = "EvalOcxSetProp";
#endif
	
	retFuncTbl = new AIpFuncTbl[AIP_NUM_RET];

	retFuncTbl[AIP_EvalBlock].func	 = &AIpProcess::RetBlock;
	retFuncTbl[AIP_EvalBranch].func  = &AIpProcess::RetBranch;
	retFuncTbl[AIP_EvalLoop].func	 = &AIpProcess::RetLoop;
	retFuncTbl[AIP_EvalCond].func	 = &AIpProcess::RetBranch;
	retFuncTbl[AIP_EvalOrOr].func	 = &AIpProcess::RetBranch;
	retFuncTbl[AIP_EvalAndAnd].func  = &AIpProcess::RetBranch;
	retFuncTbl[AIP_EvalTry].func	 = &AIpProcess::RetTry;

#ifdef	_DEBUG
	retFuncTbl[AIP_EvalBlock].name	 = "RetBlock";
	retFuncTbl[AIP_EvalBranch].name  = "RetBranch";
	retFuncTbl[AIP_EvalLoop].name	 = "RetLoop";
	retFuncTbl[AIP_EvalCond].name	 = "RetBranch";
	retFuncTbl[AIP_EvalOrOr].name	 = "RetBranch";
	retFuncTbl[AIP_EvalAndAnd].name  = "RetBranch";
	retFuncTbl[AIP_EvalTry].name	 = "RetTry";
#endif

	sysFuncTbl = new AIpFuncTbl[AIP_NUM_SYSFUNC];
	sysFuncNameTbl = new char*[AIP_NUM_SYSFUNC];
	sysFuncNumArgTbl = new int[AIP_NUM_SYSFUNC];

// arithmetic functions
	sysFuncNameTbl[AIP_SysFuncSqrt] = AL_STR("al_sqrt");
	sysFuncNumArgTbl[AIP_SysFuncSqrt] = 1;
	sysFuncTbl[AIP_SysFuncSqrt].func = &AIpProcess::EvalSqrt;
	sysFuncNameTbl[AIP_SysFuncSin] = AL_STR("al_sin");
	sysFuncNumArgTbl[AIP_SysFuncSin] = 1;
	sysFuncTbl[AIP_SysFuncSin].func = &AIpProcess::EvalSin;
	sysFuncNameTbl[AIP_SysFuncCos] = AL_STR("al_cos");
	sysFuncNumArgTbl[AIP_SysFuncCos] = 1;
	sysFuncTbl[AIP_SysFuncCos].func = &AIpProcess::EvalCos;
	sysFuncNameTbl[AIP_SysFuncTan] = AL_STR("al_tan");
	sysFuncNumArgTbl[AIP_SysFuncTan] = 1;
	sysFuncTbl[AIP_SysFuncTan].func = &AIpProcess::EvalTan;
	sysFuncNameTbl[AIP_SysFuncASin] = AL_STR("al_asin");
	sysFuncNumArgTbl[AIP_SysFuncASin] = 1;
	sysFuncTbl[AIP_SysFuncASin].func = &AIpProcess::EvalASin;
	sysFuncNameTbl[AIP_SysFuncACos] = AL_STR("al_acos");
	sysFuncNumArgTbl[AIP_SysFuncACos] = 1;
	sysFuncTbl[AIP_SysFuncACos].func = &AIpProcess::EvalACos;
	sysFuncNameTbl[AIP_SysFuncATan] = AL_STR("al_atan");
	sysFuncNumArgTbl[AIP_SysFuncATan] = 1;
	sysFuncTbl[AIP_SysFuncATan].func = &AIpProcess::EvalATan;
	sysFuncNameTbl[AIP_SysFuncExp] = AL_STR("al_exp");
	sysFuncNumArgTbl[AIP_SysFuncExp] = 1;
	sysFuncTbl[AIP_SysFuncExp].func = &AIpProcess::EvalExp;
	sysFuncNameTbl[AIP_SysFuncLog] = AL_STR("al_log");
	sysFuncNumArgTbl[AIP_SysFuncLog] = 1;
	sysFuncTbl[AIP_SysFuncLog].func = &AIpProcess::EvalLog;
	sysFuncNameTbl[AIP_SysFuncMax] = AL_STR("al_max");
	sysFuncNumArgTbl[AIP_SysFuncMax] = 2;
	sysFuncTbl[AIP_SysFuncMax].func = &AIpProcess::EvalMax;
	sysFuncNameTbl[AIP_SysFuncMin] = AL_STR("al_min");
	sysFuncNumArgTbl[AIP_SysFuncMin] = 2;
	sysFuncTbl[AIP_SysFuncMin].func = &AIpProcess::EvalMin;
	sysFuncNameTbl[AIP_SysFuncAbs] = AL_STR("al_abs");
	sysFuncNumArgTbl[AIP_SysFuncAbs] = 1;
	sysFuncTbl[AIP_SysFuncAbs].func = &AIpProcess::EvalAbs;
	sysFuncNameTbl[AIP_SysFuncMathMisc] = AL_STR("al_math_misc");
	sysFuncNumArgTbl[AIP_SysFuncMathMisc] = 3;
	sysFuncTbl[AIP_SysFuncMathMisc].func = &AIpProcess::EvalMathMisc;

// list data manipulation functions
	sysFuncNameTbl[AIP_SysFuncCons] = AL_STR("al_cons");
	sysFuncNumArgTbl[AIP_SysFuncCons] = 2;
	sysFuncTbl[AIP_SysFuncCons].func = &AIpProcess::EvalCons;
	sysFuncNameTbl[AIP_SysFuncAppendL] = AL_STR("al_append_list");
	sysFuncNumArgTbl[AIP_SysFuncAppendL] = 2;
	sysFuncTbl[AIP_SysFuncAppendL].func = &AIpProcess::EvalAppendL;
	sysFuncNameTbl[AIP_SysFuncList2] = AL_STR("al_list2");
	sysFuncNumArgTbl[AIP_SysFuncList2] = 2;
	sysFuncTbl[AIP_SysFuncList2].func = &AIpProcess::EvalList2;
	sysFuncNameTbl[AIP_SysFuncList3] = AL_STR("al_list3");
	sysFuncNumArgTbl[AIP_SysFuncList3] = 3;
	sysFuncTbl[AIP_SysFuncList3].func = &AIpProcess::EvalList3;
	sysFuncNameTbl[AIP_SysFuncList4] = AL_STR("al_list4");
	sysFuncNumArgTbl[AIP_SysFuncList4] = 4;
	sysFuncTbl[AIP_SysFuncList4].func = &AIpProcess::EvalList4;
	sysFuncNameTbl[AIP_SysFuncList5] = AL_STR("al_list5");
	sysFuncNumArgTbl[AIP_SysFuncList5] = 5;
	sysFuncTbl[AIP_SysFuncList5].func = &AIpProcess::EvalList5;
	sysFuncNameTbl[AIP_SysFuncList6] = AL_STR("al_list6");
	sysFuncNumArgTbl[AIP_SysFuncList6] = 6;
	sysFuncTbl[AIP_SysFuncList6].func = &AIpProcess::EvalList6;
	sysFuncNameTbl[AIP_SysFuncList7] = AL_STR("al_list7");
	sysFuncNumArgTbl[AIP_SysFuncList7] = 7;
	sysFuncTbl[AIP_SysFuncList7].func = &AIpProcess::EvalList7;
	sysFuncNameTbl[AIP_SysFuncList8] = AL_STR("al_list8");
	sysFuncNumArgTbl[AIP_SysFuncList8] = 8;
	sysFuncTbl[AIP_SysFuncList8].func = &AIpProcess::EvalList8;
	sysFuncNameTbl[AIP_SysFuncScanList] = AL_STR("al_scan_list");
	sysFuncNumArgTbl[AIP_SysFuncScanList] = 1;
	sysFuncTbl[AIP_SysFuncScanList].func = &AIpProcess::EvalScanList;
	sysFuncNameTbl[AIP_SysFuncPrintList] = AL_STR("al_print_list");
	sysFuncNumArgTbl[AIP_SysFuncPrintList] = 1;
	sysFuncTbl[AIP_SysFuncPrintList].func = &AIpProcess::EvalPrintList;
	sysFuncNameTbl[AIP_SysFuncListElem] = AL_STR("al_list_elem");
	sysFuncNumArgTbl[AIP_SysFuncListElem] = 2;
	sysFuncTbl[AIP_SysFuncListElem].func = &AIpProcess::EvalListElem;
	sysFuncNameTbl[AIP_SysFuncSetListElem] = AL_STR("al_set_list_elem");
	sysFuncNumArgTbl[AIP_SysFuncSetListElem] = 3;
	sysFuncTbl[AIP_SysFuncSetListElem].func = &AIpProcess::EvalSetListElem;
	sysFuncNameTbl[AIP_SysFuncListMisc] = AL_STR("al_list_misc");
	sysFuncNumArgTbl[AIP_SysFuncListMisc] = 3;
	sysFuncTbl[AIP_SysFuncListMisc].func = &AIpProcess::EvalListMisc;

// array data manipulation functions
	sysFuncNameTbl[AIP_SysFuncCreateArray] = AL_STR("al_create_array");
	sysFuncNumArgTbl[AIP_SysFuncCreateArray] = 1;
	sysFuncTbl[AIP_SysFuncCreateArray].func = &AIpProcess::EvalCreateArray;
	sysFuncNameTbl[AIP_SysFuncArrayLength] = AL_STR("al_array_length");
	sysFuncNumArgTbl[AIP_SysFuncArrayLength] = 1;
	sysFuncTbl[AIP_SysFuncArrayLength].func = &AIpProcess::EvalArrayLength;

// graph data manipulation functions
	sysFuncNameTbl[AIP_SysFuncDstItr] = AL_STR("al_dst_itr");
	sysFuncNumArgTbl[AIP_SysFuncDstItr] = 1;
	sysFuncTbl[AIP_SysFuncDstItr].func = &AIpProcess::EvalDstItr;
	sysFuncNameTbl[AIP_SysFuncSrcItr] = AL_STR("al_src_itr");
	sysFuncNumArgTbl[AIP_SysFuncSrcItr] = 1;
	sysFuncTbl[AIP_SysFuncSrcItr].func = &AIpProcess::EvalSrcItr;
	sysFuncNameTbl[AIP_SysFuncNext] = AL_STR("al_next");
	sysFuncNumArgTbl[AIP_SysFuncNext] = 1;
	sysFuncTbl[AIP_SysFuncNext].func = &AIpProcess::EvalNext;
	sysFuncNameTbl[AIP_SysFuncNextA] = AL_STR("al_next_a");
	sysFuncNumArgTbl[AIP_SysFuncNextA] = 2;
	sysFuncTbl[AIP_SysFuncNextA].func = &AIpProcess::EvalNextA;
	sysFuncNameTbl[AIP_SysFuncNextA2] = AL_STR("al_next_a2");
	sysFuncNumArgTbl[AIP_SysFuncNextA2] = 2;
	sysFuncTbl[AIP_SysFuncNextA2].func = &AIpProcess::EvalNextA2;
	sysFuncNameTbl[AIP_SysFuncPrev] = AL_STR("al_prev");
	sysFuncNumArgTbl[AIP_SysFuncPrev] = 1;
	sysFuncTbl[AIP_SysFuncPrev].func = &AIpProcess::EvalPrev;
	sysFuncNameTbl[AIP_SysFuncPrevA] = AL_STR("al_prev_a");
	sysFuncNumArgTbl[AIP_SysFuncPrevA] = 2;
	sysFuncTbl[AIP_SysFuncPrevA].func = &AIpProcess::EvalPrevA;
	sysFuncNameTbl[AIP_SysFuncPrevA2] = AL_STR("al_prev_a2");
	sysFuncNumArgTbl[AIP_SysFuncPrevA2] = 2;
	sysFuncTbl[AIP_SysFuncPrevA2].func = &AIpProcess::EvalPrevA2;
	sysFuncNameTbl[AIP_SysFuncCompo] = AL_STR("al_component");
	sysFuncNumArgTbl[AIP_SysFuncCompo] = 1;
	sysFuncTbl[AIP_SysFuncCompo].func = &AIpProcess::EvalCompo;
	sysFuncNameTbl[AIP_SysFuncInsert] = AL_STR("al_insert");
	sysFuncNumArgTbl[AIP_SysFuncInsert] = 3;
	sysFuncTbl[AIP_SysFuncInsert].func = &AIpProcess::EvalInsert;
	sysFuncNameTbl[AIP_SysFuncAppend] = AL_STR("al_append");
	sysFuncNumArgTbl[AIP_SysFuncAppend] = 3;
	sysFuncTbl[AIP_SysFuncAppend].func = &AIpProcess::EvalAppend;
	sysFuncNameTbl[AIP_SysFuncRemove] = AL_STR("al_remove");
	sysFuncNumArgTbl[AIP_SysFuncRemove] = 1;
	sysFuncTbl[AIP_SysFuncRemove].func = &AIpProcess::EvalRemove;
	sysFuncNameTbl[AIP_SysFuncGetArcA] = AL_STR("al_arc_a");
	sysFuncNumArgTbl[AIP_SysFuncGetArcA] = 1;
	sysFuncTbl[AIP_SysFuncGetArcA].func = &AIpProcess::EvalGetArcA;
	sysFuncNameTbl[AIP_SysFuncSetArcA] = AL_STR("al_set_arc_a");
	sysFuncNumArgTbl[AIP_SysFuncSetArcA] = 2;
	sysFuncTbl[AIP_SysFuncSetArcA].func = &AIpProcess::EvalSetArcA;
	sysFuncNameTbl[AIP_SysFuncCount] = AL_STR("al_count");
	sysFuncNumArgTbl[AIP_SysFuncCount] = 1;
	sysFuncTbl[AIP_SysFuncCount].func = &AIpProcess::EvalCount;
	sysFuncNameTbl[AIP_SysFuncCount2] = AL_STR("al_count2");
	sysFuncNumArgTbl[AIP_SysFuncCount2] = 2;
	sysFuncTbl[AIP_SysFuncCount2].func = &AIpProcess::EvalCount2;
	sysFuncNameTbl[AIP_SysFuncIndexPos] = AL_STR("al_index_pos");
	sysFuncNumArgTbl[AIP_SysFuncIndexPos] = 3;
	sysFuncTbl[AIP_SysFuncIndexPos].func = &AIpProcess::EvalIndexPos;
	sysFuncNameTbl[AIP_SysFuncSearch] = AL_STR("al_search");
	sysFuncNumArgTbl[AIP_SysFuncSearch] = 3;
	sysFuncTbl[AIP_SysFuncSearch].func = &AIpProcess::EvalSearch;

	sysFuncNameTbl[AIP_SysFuncCreateArc] = AL_STR("al_create_arc");
	sysFuncNumArgTbl[AIP_SysFuncCreateArc] = 3;
	sysFuncTbl[AIP_SysFuncCreateArc].func = &AIpProcess::EvalCreateArc;
	sysFuncNameTbl[AIP_SysFuncCreateAssocArc] = AL_STR("al_create_assoc_arc");
	sysFuncNumArgTbl[AIP_SysFuncCreateAssocArc] = 3;
	sysFuncTbl[AIP_SysFuncCreateAssocArc].func = &AIpProcess::EvalCreateAssocArc;
	sysFuncNameTbl[AIP_SysFuncCreateOrderedArc] = AL_STR("al_create_ordered_arc");
	sysFuncNumArgTbl[AIP_SysFuncCreateOrderedArc] = 4;
	sysFuncTbl[AIP_SysFuncCreateOrderedArc].func = &AIpProcess::EvalCreateOrderedArc;
	sysFuncNameTbl[AIP_SysFuncDstNode] = AL_STR("al_dst_node");
	sysFuncNumArgTbl[AIP_SysFuncDstNode] = 2;
	sysFuncTbl[AIP_SysFuncDstNode].func = &AIpProcess::EvalDstNode;
	sysFuncNameTbl[AIP_SysFuncDstNodeNoCase] = AL_STR("al_dst_node_i");
	sysFuncNumArgTbl[AIP_SysFuncDstNodeNoCase] = 2;
	sysFuncTbl[AIP_SysFuncDstNodeNoCase].func = &AIpProcess::EvalDstNodeNoCase;
	sysFuncNameTbl[AIP_SysFuncSrcNode] = AL_STR("al_src_node");
	sysFuncNumArgTbl[AIP_SysFuncSrcNode] = 2;
	sysFuncTbl[AIP_SysFuncSrcNode].func = &AIpProcess::EvalSrcNode;
	sysFuncNameTbl[AIP_SysFuncSetDstNode] = AL_STR("al_set_dst_node");
	sysFuncNumArgTbl[AIP_SysFuncSetDstNode] = 3;
	sysFuncTbl[AIP_SysFuncSetDstNode].func = &AIpProcess::EvalSetDstNode;
	sysFuncNameTbl[AIP_SysFuncNextDstNode] = AL_STR("al_next_dst_node");
	sysFuncNumArgTbl[AIP_SysFuncNextDstNode] = 3;
	sysFuncTbl[AIP_SysFuncNextDstNode].func = &AIpProcess::EvalNextDstNode;
	sysFuncNameTbl[AIP_SysFuncPrevDstNode] = AL_STR("al_prev_dst_node");
	sysFuncNumArgTbl[AIP_SysFuncPrevDstNode] = 3;
	sysFuncTbl[AIP_SysFuncPrevDstNode].func = &AIpProcess::EvalPrevDstNode;
	sysFuncNameTbl[AIP_SysFuncNextSrcNode] = AL_STR("al_next_src_node");
	sysFuncNumArgTbl[AIP_SysFuncNextSrcNode] = 3;
	sysFuncTbl[AIP_SysFuncNextSrcNode].func = &AIpProcess::EvalNextSrcNode;
	sysFuncNameTbl[AIP_SysFuncPrevSrcNode] = AL_STR("al_prev_src_node");
	sysFuncNumArgTbl[AIP_SysFuncPrevSrcNode] = 3;
	sysFuncTbl[AIP_SysFuncPrevSrcNode].func = &AIpProcess::EvalPrevSrcNode;
	sysFuncNameTbl[AIP_SysFuncInsertBefore] = AL_STR("al_insert_before");
	sysFuncNumArgTbl[AIP_SysFuncInsertBefore] = 4;
	sysFuncTbl[AIP_SysFuncInsertBefore].func = &AIpProcess::EvalInsertBefore;
	sysFuncNameTbl[AIP_SysFuncAppendAfter] = AL_STR("al_append_after");
	sysFuncNumArgTbl[AIP_SysFuncAppendAfter] = 4;
	sysFuncTbl[AIP_SysFuncAppendAfter].func = &AIpProcess::EvalAppendAfter;
	sysFuncNameTbl[AIP_SysFuncRemoveFrom] = AL_STR("al_remove_from");
	sysFuncNumArgTbl[AIP_SysFuncRemoveFrom] = 3;
	sysFuncTbl[AIP_SysFuncRemoveFrom].func = &AIpProcess::EvalRemoveFrom;
	sysFuncNameTbl[AIP_SysFuncCopy] = AL_STR("al_copy");
	sysFuncNumArgTbl[AIP_SysFuncCopy] = 1;
	sysFuncTbl[AIP_SysFuncCopy].func = &AIpProcess::EvalCopy;

	sysFuncNameTbl[AIP_SysFuncDstItr2] = AL_STR("al_dst_itr2");
	sysFuncNumArgTbl[AIP_SysFuncDstItr2] = 2;
	sysFuncTbl[AIP_SysFuncDstItr2].func = &AIpProcess::EvalDstItr2;
	sysFuncNameTbl[AIP_SysFuncArcDst] = AL_STR("al_arc_dst");
	sysFuncNumArgTbl[AIP_SysFuncArcDst] = 2;
	sysFuncTbl[AIP_SysFuncArcDst].func = &AIpProcess::EvalArcDst;
	sysFuncNameTbl[AIP_SysFuncSrcItr2] = AL_STR("al_src_itr2");
	sysFuncNumArgTbl[AIP_SysFuncSrcItr2] = 2;
	sysFuncTbl[AIP_SysFuncSrcItr2].func = &AIpProcess::EvalSrcItr2;
	sysFuncNameTbl[AIP_SysFuncArcSrc] = AL_STR("al_arc_src");
	sysFuncNumArgTbl[AIP_SysFuncArcSrc] = 2;
	sysFuncTbl[AIP_SysFuncArcSrc].func = &AIpProcess::EvalArcSrc;

	sysFuncNameTbl[AIP_SysFuncGraphMisc] = AL_STR("al_graph_misc");
	sysFuncNumArgTbl[AIP_SysFuncGraphMisc] = 3;
	sysFuncTbl[AIP_SysFuncGraphMisc].func = &AIpProcess::EvalGraphMisc;

// PolyPoly data manipulation functions
	sysFuncNameTbl[AIP_SysFuncCreatePolyPoly] = AL_STR("al_create_poly_poly");
	sysFuncNumArgTbl[AIP_SysFuncCreatePolyPoly] = 0;
	sysFuncTbl[AIP_SysFuncCreatePolyPoly].func = &AIpProcess::EvalCreatePolyPoly;
	sysFuncNameTbl[AIP_SysFuncCreateNextPoly] = AL_STR("al_create_next_poly");
	sysFuncNumArgTbl[AIP_SysFuncCreateNextPoly] = 1;
	sysFuncTbl[AIP_SysFuncCreateNextPoly].func = &AIpProcess::EvalCreateNextPoly;
	sysFuncNameTbl[AIP_SysFuncAddPointToPoly] = AL_STR("al_add_point_to_poly");
	sysFuncNumArgTbl[AIP_SysFuncAddPointToPoly] = 3;
	sysFuncTbl[AIP_SysFuncAddPointToPoly].func = &AIpProcess::EvalAddPointToPoly;
	sysFuncNameTbl[AIP_SysFuncAddRectToPoly] = AL_STR("al_add_rect_to_poly");
	sysFuncNumArgTbl[AIP_SysFuncAddRectToPoly] = 5;
	sysFuncTbl[AIP_SysFuncAddRectToPoly].func = &AIpProcess::EvalAddRectToPoly;
	sysFuncNameTbl[AIP_SysFuncAddCurveToPoly] = AL_STR("al_add_curve_to_poly");
	sysFuncNumArgTbl[AIP_SysFuncAddCurveToPoly] = 7;
	sysFuncTbl[AIP_SysFuncAddCurveToPoly].func = &AIpProcess::EvalAddCurveToPoly;
	sysFuncNameTbl[AIP_SysFuncIntersection] = AL_STR("al_intersection");
	sysFuncNumArgTbl[AIP_SysFuncIntersection] = 2;
	sysFuncTbl[AIP_SysFuncIntersection].func = &AIpProcess::EvalIntersection;
	sysFuncNameTbl[AIP_SysFuncUnion] = AL_STR("al_union");
	sysFuncNumArgTbl[AIP_SysFuncUnion] = 2;
	sysFuncTbl[AIP_SysFuncUnion].func = &AIpProcess::EvalUnion;

// Matrix data manipulation functions
	sysFuncNameTbl[AIP_SysFuncUnitTM] = AL_STR("al_unit_tm");
	sysFuncNumArgTbl[AIP_SysFuncUnitTM] = 0;
	sysFuncTbl[AIP_SysFuncUnitTM].func = &AIpProcess::EvalUnitTM;
	sysFuncNameTbl[AIP_SysFuncTranslateTM] = AL_STR("al_translate_tm");
	sysFuncNumArgTbl[AIP_SysFuncTranslateTM] = 2;
	sysFuncTbl[AIP_SysFuncTranslateTM].func = &AIpProcess::EvalTranslateTM;
	sysFuncNameTbl[AIP_SysFuncScaleTM] = AL_STR("al_scale_tm");
	sysFuncNumArgTbl[AIP_SysFuncScaleTM] = 2;
	sysFuncTbl[AIP_SysFuncScaleTM].func = &AIpProcess::EvalScaleTM;
	sysFuncNameTbl[AIP_SysFuncRotateTM] = AL_STR("al_rotate_tm");
	sysFuncNumArgTbl[AIP_SysFuncRotateTM] = 1;
	sysFuncTbl[AIP_SysFuncRotateTM].func = &AIpProcess::EvalRotateTM;
	sysFuncNameTbl[AIP_SysFuncTransform] = AL_STR("al_transform_xy");
	sysFuncNumArgTbl[AIP_SysFuncTransform] = 3;
	sysFuncTbl[AIP_SysFuncTransform].func = &AIpProcess::EvalTransformXY;
	sysFuncNameTbl[AIP_SysFuncInverse] = AL_STR("al_inverse_xy");
	sysFuncNumArgTbl[AIP_SysFuncInverse] = 3;
	sysFuncTbl[AIP_SysFuncInverse].func = &AIpProcess::EvalInverseXY;
	sysFuncNameTbl[AIP_SysFuncTransformP] = AL_STR("al_transform_poly");
	sysFuncNumArgTbl[AIP_SysFuncTransformP] = 2;
	sysFuncTbl[AIP_SysFuncTransformP].func = &AIpProcess::EvalTransformPoly;
	sysFuncNameTbl[AIP_SysFuncInverseP] = AL_STR("al_inverse_poly");
	sysFuncNumArgTbl[AIP_SysFuncInverseP] = 2;
	sysFuncTbl[AIP_SysFuncInverseP].func = &AIpProcess::EvalInversePoly;
	sysFuncNameTbl[AIP_SysFuncTransformRect] = AL_STR("al_transform_rect");
	sysFuncNumArgTbl[AIP_SysFuncTransformRect] = 5;
	sysFuncTbl[AIP_SysFuncTransformRect].func = &AIpProcess::EvalTransformRect;

// graphics functions
	sysFuncNameTbl[AIP_SysFuncPrint] = AL_STR("al_print");
	sysFuncNumArgTbl[AIP_SysFuncPrint] = 1;
	sysFuncTbl[AIP_SysFuncPrint].func = &AIpProcess::EvalPrint;
	sysFuncNameTbl[AIP_SysFuncAsk] = AL_STR("al_ask");
	sysFuncNumArgTbl[AIP_SysFuncAsk] = 2;
	sysFuncTbl[AIP_SysFuncAsk].func = &AIpProcess::EvalAsk;
	sysFuncNameTbl[AIP_SysFuncMenu] = AL_STR("al_menu_select");
	sysFuncNumArgTbl[AIP_SysFuncMenu] = 3;
	sysFuncTbl[AIP_SysFuncMenu].func = &AIpProcess::EvalMenu;
	sysFuncNameTbl[AIP_SysFuncReadFileName] = AL_STR("al_get_read_filename");
	sysFuncNumArgTbl[AIP_SysFuncReadFileName] = 2;
	sysFuncTbl[AIP_SysFuncReadFileName].func = &AIpProcess::EvalGetReadFileName;
	sysFuncNameTbl[AIP_SysFuncWriteFileName] = AL_STR("al_get_write_filename");
	sysFuncNumArgTbl[AIP_SysFuncWriteFileName] = 2;
	sysFuncTbl[AIP_SysFuncWriteFileName].func = &AIpProcess::EvalGetWriteFileName;
	sysFuncNameTbl[AIP_SysFuncGetColor] = AL_STR("al_get_color");
	sysFuncNumArgTbl[AIP_SysFuncGetColor] = 0;
	sysFuncTbl[AIP_SysFuncGetColor].func = &AIpProcess::EvalGetColor;
	sysFuncNameTbl[AIP_SysFuncGetFont] = AL_STR("al_get_font");
	sysFuncNumArgTbl[AIP_SysFuncGetFont] = 0;
	sysFuncTbl[AIP_SysFuncGetFont].func = &AIpProcess::EvalGetFont;
	sysFuncNameTbl[AIP_SysFuncMessageText] = AL_STR("al_message_text");
	sysFuncNumArgTbl[AIP_SysFuncMessageText] = 2;
	sysFuncTbl[AIP_SysFuncMessageText].func = &AIpProcess::EvalMessageText;
	sysFuncNameTbl[AIP_SysFuncInputByCEdit] = AL_STR("al_input_by_cedit");
	sysFuncNumArgTbl[AIP_SysFuncInputByCEdit] = 6;
	sysFuncTbl[AIP_SysFuncInputByCEdit].func = &AIpProcess::EvalInputByCEdit;

	sysFuncNameTbl[AIP_SysFuncCreateWindow] = AL_STR("al_create_window");
	sysFuncNumArgTbl[AIP_SysFuncCreateWindow] = 1;
	sysFuncTbl[AIP_SysFuncCreateWindow].func = &AIpProcess::EvalCreateWindow;
	sysFuncNameTbl[AIP_SysFuncUserEdit] = AL_STR("al_user_edit");
	sysFuncNumArgTbl[AIP_SysFuncUserEdit] = 4;
	sysFuncTbl[AIP_SysFuncUserEdit].func = &AIpProcess::EvalUserEdit;

	sysFuncNameTbl[AIP_SysFuncSetWindowArg] = AL_STR("al_set_window_arg");
	sysFuncNumArgTbl[AIP_SysFuncSetWindowArg] = 3;
	sysFuncTbl[AIP_SysFuncSetWindowArg].func = &AIpProcess::EvalSetWindowArg;
	sysFuncNameTbl[AIP_SysFuncSetDrawProc] = AL_STR("al_set_draw_proc");
	sysFuncNumArgTbl[AIP_SysFuncSetDrawProc] = 2;
	sysFuncTbl[AIP_SysFuncSetDrawProc].func = &AIpProcess::EvalSetDrawProc;
	sysFuncNameTbl[AIP_SysFuncSetEventProc] = AL_STR("al_set_event_proc");
	sysFuncNumArgTbl[AIP_SysFuncSetEventProc] = 2;
	sysFuncTbl[AIP_SysFuncSetEventProc].func = &AIpProcess::EvalSetEventProc;
	sysFuncNameTbl[AIP_SysFuncSetMoveCtrlProc] = AL_STR("al_set_move_ctrl_proc");
	sysFuncNumArgTbl[AIP_SysFuncSetMoveCtrlProc] = 2;
	sysFuncTbl[AIP_SysFuncSetMoveCtrlProc].func = &AIpProcess::EvalSetMoveCtrlProc;
	sysFuncNameTbl[AIP_SysFuncSetDestroyProc] = AL_STR("al_set_destroy_proc");
	sysFuncNumArgTbl[AIP_SysFuncSetDestroyProc] = 2;
	sysFuncTbl[AIP_SysFuncSetDestroyProc].func = &AIpProcess::EvalSetDestroyProc;
	sysFuncNameTbl[AIP_SysFuncSetActivateProc] = AL_STR("al_set_activate_proc");
	sysFuncNumArgTbl[AIP_SysFuncSetActivateProc] = 2;
	sysFuncTbl[AIP_SysFuncSetActivateProc].func = &AIpProcess::EvalSetActivateProc;
	sysFuncNameTbl[AIP_SysFuncSetTextInsProc] = AL_STR("al_set_text_ins_proc");
	sysFuncNumArgTbl[AIP_SysFuncSetTextInsProc] = 2;
	sysFuncTbl[AIP_SysFuncSetTextInsProc].func = &AIpProcess::EvalSetTextInsProc;

	sysFuncNameTbl[AIP_SysFuncSetTitle] = AL_STR("al_set_title");
	sysFuncNumArgTbl[AIP_SysFuncSetTitle] = 2;
	sysFuncTbl[AIP_SysFuncSetTitle].func = &AIpProcess::EvalSetTitle;
	sysFuncNameTbl[AIP_SysFuncGetTitle] = AL_STR("al_get_title");
	sysFuncNumArgTbl[AIP_SysFuncGetTitle] = 1;
	sysFuncTbl[AIP_SysFuncGetTitle].func = &AIpProcess::EvalGetTitle;
	sysFuncNameTbl[AIP_SysFuncSetMenu] = AL_STR("al_set_menu");
	sysFuncNumArgTbl[AIP_SysFuncSetMenu] = 2;
	sysFuncTbl[AIP_SysFuncSetMenu].func = &AIpProcess::EvalSetMenu;
	sysFuncNameTbl[AIP_SysFuncGetWndRect] = AL_STR("al_get_window_rect");
	sysFuncNumArgTbl[AIP_SysFuncGetWndRect] = 2;
	sysFuncTbl[AIP_SysFuncGetWndRect].func = &AIpProcess::EvalGetWndRect;
	sysFuncNameTbl[AIP_SysFuncSetWndRect] = AL_STR("al_set_window_rect");
	sysFuncNumArgTbl[AIP_SysFuncSetWndRect] = 5;
	sysFuncTbl[AIP_SysFuncSetWndRect].func = &AIpProcess::EvalSetWndRect;
	sysFuncNameTbl[AIP_SysFuncResizeWnd] = AL_STR("al_resize_window");
	sysFuncNumArgTbl[AIP_SysFuncResizeWnd] = 3;
	sysFuncTbl[AIP_SysFuncResizeWnd].func = &AIpProcess::EvalResizeWnd;
	sysFuncNameTbl[AIP_SysFuncMoveWnd] = AL_STR("al_move_window");
	sysFuncNumArgTbl[AIP_SysFuncMoveWnd] = 3;
	sysFuncTbl[AIP_SysFuncMoveWnd].func = &AIpProcess::EvalMoveWnd;
	sysFuncNameTbl[AIP_SysFuncShowWnd] = AL_STR("al_show_window");
	sysFuncNumArgTbl[AIP_SysFuncShowWnd] = 1;
	sysFuncTbl[AIP_SysFuncShowWnd].func = &AIpProcess::EvalShowWnd;
	sysFuncNameTbl[AIP_SysFuncHideWnd] = AL_STR("al_hide_window");
	sysFuncNumArgTbl[AIP_SysFuncHideWnd] = 1;
	sysFuncTbl[AIP_SysFuncHideWnd].func = &AIpProcess::EvalHideWnd;
	sysFuncNameTbl[AIP_SysFuncDestroyWnd] = AL_STR("al_destroy_window");
	sysFuncNumArgTbl[AIP_SysFuncDestroyWnd] = 1;
	sysFuncTbl[AIP_SysFuncDestroyWnd].func = &AIpProcess::EvalDestroyWnd;

	sysFuncNameTbl[AIP_SysFuncPushClipping] = AL_STR("al_push_clipping");
	sysFuncNumArgTbl[AIP_SysFuncPushClipping] = 2;
	sysFuncTbl[AIP_SysFuncPushClipping].func = &AIpProcess::EvalPushClipping;
	sysFuncNameTbl[AIP_SysFuncPopClipping] = AL_STR("al_pop_clipping");
	sysFuncNumArgTbl[AIP_SysFuncPopClipping] = 1;
	sysFuncTbl[AIP_SysFuncPopClipping].func = &AIpProcess::EvalPopClipping;
	sysFuncNameTbl[AIP_SysFuncClipping] = AL_STR("al_clipping");
	sysFuncNumArgTbl[AIP_SysFuncClipping] = 1;
	sysFuncTbl[AIP_SysFuncClipping].func = &AIpProcess::EvalClipping;
	sysFuncNameTbl[AIP_SysFuncIsClipped] = AL_STR("al_is_clipped");
	sysFuncNumArgTbl[AIP_SysFuncIsClipped] = 6;
	sysFuncTbl[AIP_SysFuncIsClipped].func = &AIpProcess::EvalIsClipped;
	sysFuncNameTbl[AIP_SysFuncPushBgColor] = AL_STR("al_push_bg_color");
	sysFuncNumArgTbl[AIP_SysFuncPushBgColor] = 2;
	sysFuncTbl[AIP_SysFuncPushBgColor].func = &AIpProcess::EvalPushBgColor;
	sysFuncNameTbl[AIP_SysFuncPopBgColor] = AL_STR("al_pop_bg_color");
	sysFuncNumArgTbl[AIP_SysFuncPopBgColor] = 1;
	sysFuncTbl[AIP_SysFuncPopBgColor].func = &AIpProcess::EvalPopBgColor;
	sysFuncNameTbl[AIP_SysFuncPushColorFunc] = AL_STR("al_push_color_func");
	sysFuncNumArgTbl[AIP_SysFuncPushColorFunc] = 2;
	sysFuncTbl[AIP_SysFuncPushColorFunc].func = &AIpProcess::EvalPushColorFunc;
	sysFuncNameTbl[AIP_SysFuncPopColorFunc] = AL_STR("al_pop_color_func");
	sysFuncNumArgTbl[AIP_SysFuncPopColorFunc] = 1;
	sysFuncTbl[AIP_SysFuncPopColorFunc].func = &AIpProcess::EvalPopColorFunc;
	sysFuncNameTbl[AIP_SysFuncGetSysColor] = AL_STR("al_get_sys_color");
	sysFuncNumArgTbl[AIP_SysFuncGetSysColor] = 1;
	sysFuncTbl[AIP_SysFuncGetSysColor].func = &AIpProcess::EvalGetSysColor;
	sysFuncNameTbl[AIP_SysFuncPushPen] = AL_STR("al_push_pen");
	sysFuncNumArgTbl[AIP_SysFuncPushPen] = 4;
	sysFuncTbl[AIP_SysFuncPushPen].func = &AIpProcess::EvalPushPen;
	sysFuncNameTbl[AIP_SysFuncPopPen] = AL_STR("al_pop_pen");
	sysFuncNumArgTbl[AIP_SysFuncPopPen] = 1;
	sysFuncTbl[AIP_SysFuncPopPen].func = &AIpProcess::EvalPopPen;
	sysFuncNameTbl[AIP_SysFuncPushBrush] = AL_STR("al_push_brush");
	sysFuncNumArgTbl[AIP_SysFuncPushBrush] = 4;
	sysFuncTbl[AIP_SysFuncPushBrush].func = &AIpProcess::EvalPushBrush;
	sysFuncNameTbl[AIP_SysFuncPopBrush] = AL_STR("al_pop_brush");
	sysFuncNumArgTbl[AIP_SysFuncPopBrush] = 1;
	sysFuncTbl[AIP_SysFuncPopBrush].func = &AIpProcess::EvalPopBrush;
	sysFuncNameTbl[AIP_SysFuncPushFont] = AL_STR("al_push_font");
	sysFuncNumArgTbl[AIP_SysFuncPushFont] = 5;
	sysFuncTbl[AIP_SysFuncPushFont].func = &AIpProcess::EvalPushFont;
	sysFuncNameTbl[AIP_SysFuncPushFontEx] = AL_STR("al_push_font_ex");
	sysFuncNumArgTbl[AIP_SysFuncPushFontEx] = 2;
	sysFuncTbl[AIP_SysFuncPushFontEx].func = &AIpProcess::EvalPushFontEx;
	sysFuncNameTbl[AIP_SysFuncPopFont] = AL_STR("al_pop_font");
	sysFuncNumArgTbl[AIP_SysFuncPopFont] = 1;
	sysFuncTbl[AIP_SysFuncPopFont].func = &AIpProcess::EvalPopFont;
	sysFuncNameTbl[AIP_SysFuncPushTM] = AL_STR("al_push_tm");
	sysFuncNumArgTbl[AIP_SysFuncPushTM] = 2;
	sysFuncTbl[AIP_SysFuncPushTM].func = &AIpProcess::EvalPushTM;
	sysFuncNameTbl[AIP_SysFuncPopTM] = AL_STR("al_pop_tm");
	sysFuncNumArgTbl[AIP_SysFuncPopTM] = 1;
	sysFuncTbl[AIP_SysFuncPopTM].func = &AIpProcess::EvalPopTM;
	sysFuncNameTbl[AIP_SysFuncWindowTM] = AL_STR("al_window_tm");
	sysFuncNumArgTbl[AIP_SysFuncWindowTM] = 1;
	sysFuncTbl[AIP_SysFuncWindowTM].func = &AIpProcess::EvalWindowTM;
	sysFuncNameTbl[AIP_SysFuncIsPrinting] = AL_STR("al_is_printing");
	sysFuncNumArgTbl[AIP_SysFuncIsPrinting] = 1;
	sysFuncTbl[AIP_SysFuncIsPrinting].func = &AIpProcess::EvalIsPrinting;

	sysFuncNameTbl[AIP_SysFuncStartQuickDraw] = AL_STR("al_start_quick_draw");
	sysFuncNumArgTbl[AIP_SysFuncStartQuickDraw] = 1;
	sysFuncTbl[AIP_SysFuncStartQuickDraw].func = &AIpProcess::EvalStartQuickDraw;
	sysFuncNameTbl[AIP_SysFuncEndQuickDraw] = AL_STR("al_end_quick_draw");
	sysFuncNumArgTbl[AIP_SysFuncEndQuickDraw] = 1;
	sysFuncTbl[AIP_SysFuncEndQuickDraw].func = &AIpProcess::EvalEndQuickDraw;

	sysFuncNameTbl[AIP_SysFuncPoint] = AL_STR("al_point");
	sysFuncNumArgTbl[AIP_SysFuncPoint] = 4;
	sysFuncTbl[AIP_SysFuncPoint].func = &AIpProcess::EvalPoint;
	sysFuncNameTbl[AIP_SysFuncLine] = AL_STR("al_line");
	sysFuncNumArgTbl[AIP_SysFuncLine] = 5;
	sysFuncTbl[AIP_SysFuncLine].func = &AIpProcess::EvalLine;
	sysFuncNameTbl[AIP_SysFuncRectangle] = AL_STR("al_rectangle");
	sysFuncNumArgTbl[AIP_SysFuncRectangle] = 5;
	sysFuncTbl[AIP_SysFuncRectangle].func = &AIpProcess::EvalRectangle;
	sysFuncNameTbl[AIP_SysFuncEllipse] = AL_STR("al_ellipse");
	sysFuncNumArgTbl[AIP_SysFuncEllipse] = 5;
	sysFuncTbl[AIP_SysFuncEllipse].func = &AIpProcess::EvalEllipse;
	sysFuncNameTbl[AIP_SysFuncPolyPolyline] = AL_STR("al_polyline");
	sysFuncNumArgTbl[AIP_SysFuncPolyPolyline] = 2;
	sysFuncTbl[AIP_SysFuncPolyPolyline].func = &AIpProcess::EvalPolyPolyline;
	sysFuncNameTbl[AIP_SysFuncPolyPolygon] = AL_STR("al_polygon");
	sysFuncNumArgTbl[AIP_SysFuncPolyPolygon] = 2;
	sysFuncTbl[AIP_SysFuncPolyPolygon].func = &AIpProcess::EvalPolyPolygon;
	sysFuncNameTbl[AIP_SysFuncTextOut] = AL_STR("al_text");
	sysFuncNumArgTbl[AIP_SysFuncTextOut] = 4;
	sysFuncTbl[AIP_SysFuncTextOut].func = &AIpProcess::EvalTextOut;
	sysFuncNameTbl[AIP_SysFuncTextExtent] = AL_STR("al_text_width");
	sysFuncNumArgTbl[AIP_SysFuncTextExtent] = 2;
	sysFuncTbl[AIP_SysFuncTextExtent].func = &AIpProcess::EvalTextExtent;
	sysFuncNameTbl[AIP_SysFuncTextOutFromTo] = AL_STR("al_text_from_to");
	sysFuncNumArgTbl[AIP_SysFuncTextOutFromTo] = 6;
	sysFuncTbl[AIP_SysFuncTextOutFromTo].func = &AIpProcess::EvalTextOutFromTo;
	sysFuncNameTbl[AIP_SysFuncTextExtentFromTo] = AL_STR("al_text_width_from_to");
	sysFuncNumArgTbl[AIP_SysFuncTextExtentFromTo] = 4;
	sysFuncTbl[AIP_SysFuncTextExtentFromTo].func = &AIpProcess::EvalTextExtentFromTo;
	sysFuncNameTbl[AIP_SysFuncMTextOut] = AL_STR("al_m_text");
	sysFuncNumArgTbl[AIP_SysFuncMTextOut] = 6;
	sysFuncTbl[AIP_SysFuncMTextOut].func = &AIpProcess::EvalMTextOut;
	sysFuncNameTbl[AIP_SysFuncPie] = AL_STR("al_pie");
	sysFuncNumArgTbl[AIP_SysFuncPie] = 9;
	sysFuncTbl[AIP_SysFuncPie].func = &AIpProcess::EvalPie;
	sysFuncNameTbl[AIP_SysFuncBitmap] = AL_STR("al_bitmap");
	sysFuncNumArgTbl[AIP_SysFuncBitmap] = 3;
	sysFuncTbl[AIP_SysFuncBitmap].func = &AIpProcess::EvalBitmap;
	sysFuncNameTbl[AIP_SysFuncImage] = AL_STR("al_image");
	sysFuncNumArgTbl[AIP_SysFuncImage] = 6;
	sysFuncTbl[AIP_SysFuncImage].func = &AIpProcess::EvalImage;

	sysFuncNameTbl[AIP_SysFuncDamage] = AL_STR("al_damage");
	sysFuncNumArgTbl[AIP_SysFuncDamage] = 2;
	sysFuncTbl[AIP_SysFuncDamage].func = &AIpProcess::EvalDamage;
	sysFuncNameTbl[AIP_SysFuncDamageRectPoly] = AL_STR("al_damage_rect_poly");
	sysFuncNumArgTbl[AIP_SysFuncDamageRectPoly] = 1;
	sysFuncTbl[AIP_SysFuncDamageRectPoly].func = &AIpProcess::EvalDamageRectPoly;
	sysFuncNameTbl[AIP_SysFuncIsDamaged] = AL_STR("al_is_damaged");
	sysFuncNumArgTbl[AIP_SysFuncIsDamaged] = 6;
	sysFuncTbl[AIP_SysFuncIsDamaged].func = &AIpProcess::EvalIsDamaged;
	sysFuncNameTbl[AIP_SysFuncRepair] = AL_STR("al_repair");
	sysFuncNumArgTbl[AIP_SysFuncRepair] = 1;
	sysFuncTbl[AIP_SysFuncRepair].func = &AIpProcess::EvalRepair;
	sysFuncNameTbl[AIP_SysFuncRedraw] = AL_STR("al_redraw");
	sysFuncNumArgTbl[AIP_SysFuncRedraw] = 1;
	sysFuncTbl[AIP_SysFuncRedraw].func = &AIpProcess::EvalRedraw;

	sysFuncNameTbl[AIP_SysFuncLocalLoop] = AL_STR("al_local_loop");
	sysFuncNumArgTbl[AIP_SysFuncLocalLoop] = 1;
	sysFuncTbl[AIP_SysFuncLocalLoop].func = &AIpProcess::EvalLocalLoop;
	sysFuncNameTbl[AIP_SysFuncExitLoop] = AL_STR("al_exit_loop");
	sysFuncNumArgTbl[AIP_SysFuncExitLoop] = 1;
	sysFuncTbl[AIP_SysFuncExitLoop].func = &AIpProcess::EvalExitLoop;

	sysFuncNameTbl[AIP_SysFuncFilePrint] = AL_STR("al_file_print");
	sysFuncNumArgTbl[AIP_SysFuncFilePrint] = 2;
	sysFuncTbl[AIP_SysFuncFilePrint].func = &AIpProcess::EvalFilePrint;

	sysFuncNameTbl[AIP_SysFuncGuiMisc] = AL_STR("al_gui_misc");
	sysFuncNumArgTbl[AIP_SysFuncGuiMisc] = 3;
	sysFuncTbl[AIP_SysFuncGuiMisc].func = &AIpProcess::EvalGuiMisc;

// file input/output functions
	sysFuncNameTbl[AIP_SysFuncFileOpen] = AL_STR("al_file_open");
	sysFuncNumArgTbl[AIP_SysFuncFileOpen] = 2;
	sysFuncTbl[AIP_SysFuncFileOpen].func = &AIpProcess::EvalFileOpen;
	sysFuncNameTbl[AIP_SysFuncFileRead] = AL_STR("al_file_read");
	sysFuncNumArgTbl[AIP_SysFuncFileRead] = 2;
	sysFuncTbl[AIP_SysFuncFileRead].func = &AIpProcess::EvalFileRead;
	sysFuncNameTbl[AIP_SysFuncFileWrite] = AL_STR("al_file_write");
	sysFuncNumArgTbl[AIP_SysFuncFileWrite] = 3;
	sysFuncTbl[AIP_SysFuncFileWrite].func = &AIpProcess::EvalFileWrite;
	sysFuncNameTbl[AIP_SysFuncFileManip] = AL_STR("al_file_manip");
	sysFuncNumArgTbl[AIP_SysFuncFileManip] = 3;
	sysFuncTbl[AIP_SysFuncFileManip].func = &AIpProcess::EvalFileManip;
	sysFuncNameTbl[AIP_SysFuncFileMatchStr] = AL_STR("al_file_match_str");
	sysFuncNumArgTbl[AIP_SysFuncFileMatchStr] = 2;
	sysFuncTbl[AIP_SysFuncFileMatchStr].func = &AIpProcess::EvalFileMatchStr;

// string manipulation functions
	sysFuncNameTbl[AIP_SysFuncStrLen] = AL_STR("al_strlen");
	sysFuncNumArgTbl[AIP_SysFuncStrLen] = 1;
	sysFuncTbl[AIP_SysFuncStrLen].func = &AIpProcess::EvalStrLen;
	sysFuncNameTbl[AIP_SysFuncGetChar] = AL_STR("al_get_char");
	sysFuncNumArgTbl[AIP_SysFuncGetChar] = 2;
	sysFuncTbl[AIP_SysFuncGetChar].func = &AIpProcess::EvalGetChar;
	sysFuncNameTbl[AIP_SysFuncSetChar] = AL_STR("al_set_char");
	sysFuncNumArgTbl[AIP_SysFuncSetChar] = 3;
	sysFuncTbl[AIP_SysFuncSetChar].func = &AIpProcess::EvalSetChar;
	sysFuncNameTbl[AIP_SysFuncSubStr] = AL_STR("al_substr");
	sysFuncNumArgTbl[AIP_SysFuncSubStr] = 3;
	sysFuncTbl[AIP_SysFuncSubStr].func = &AIpProcess::EvalSubStr;
	sysFuncNameTbl[AIP_SysFuncMatchStr] = AL_STR("al_match_str");
	sysFuncNumArgTbl[AIP_SysFuncMatchStr] = 3;
	sysFuncTbl[AIP_SysFuncMatchStr].func = &AIpProcess::EvalMatchStr;
	sysFuncNameTbl[AIP_SysFuncTailStr] = AL_STR("al_tail_str");
	sysFuncNumArgTbl[AIP_SysFuncTailStr] = 2;
	sysFuncTbl[AIP_SysFuncTailStr].func = &AIpProcess::EvalTailStr;
	sysFuncNameTbl[AIP_SysFuncInsertChar] = AL_STR("al_insert_char");
	sysFuncNumArgTbl[AIP_SysFuncInsertChar] = 3;
	sysFuncTbl[AIP_SysFuncInsertChar].func = &AIpProcess::EvalInsertChar;
	sysFuncNameTbl[AIP_SysFuncDeleteChar] = AL_STR("al_delete_char");
	sysFuncNumArgTbl[AIP_SysFuncDeleteChar] = 2;
	sysFuncTbl[AIP_SysFuncDeleteChar].func = &AIpProcess::EvalDeleteChar;
	sysFuncNameTbl[AIP_SysFuncSearchStr] = AL_STR("al_search_str");
	sysFuncNumArgTbl[AIP_SysFuncSearchStr] = 3;
	sysFuncTbl[AIP_SysFuncSearchStr].func = &AIpProcess::EvalSearchStr;
	sysFuncNameTbl[AIP_SysFuncReplaceStr] = AL_STR("al_replace_str");
	sysFuncNumArgTbl[AIP_SysFuncReplaceStr] = 4;
	sysFuncTbl[AIP_SysFuncReplaceStr].func = &AIpProcess::EvalReplaceStr;
	sysFuncNameTbl[AIP_SysFuncAppendStr] = AL_STR("al_append_str");
	sysFuncNumArgTbl[AIP_SysFuncAppendStr] = 2;
	sysFuncTbl[AIP_SysFuncAppendStr].func = &AIpProcess::EvalAppendStr;
	sysFuncNameTbl[AIP_SysFuncIsFirstChar] = AL_STR("al_is_first_char");
	sysFuncNumArgTbl[AIP_SysFuncIsFirstChar] = 1;
	sysFuncTbl[AIP_SysFuncIsFirstChar].func = &AIpProcess::EvalIsFirstChar;
	sysFuncNameTbl[AIP_SysFuncIsNormalChar] = AL_STR("al_is_normal_char");
	sysFuncNumArgTbl[AIP_SysFuncIsNormalChar] = 1;
	sysFuncTbl[AIP_SysFuncIsNormalChar].func = &AIpProcess::EvalIsNormalChar;
	sysFuncNameTbl[AIP_SysFuncStrMisc] = AL_STR("al_str_misc");
	sysFuncNumArgTbl[AIP_SysFuncStrMisc] = 3;
	sysFuncTbl[AIP_SysFuncStrMisc].func = &AIpProcess::EvalStrMisc;

// dynamic type check
	sysFuncNameTbl[AIP_SysFuncIsType] = AL_STR("al_is_type");
	sysFuncNumArgTbl[AIP_SysFuncIsType] = 2;
	sysFuncTbl[AIP_SysFuncIsType].func = &AIpProcess::EvalIsType;
	sysFuncNameTbl[AIP_SysFuncAddrEq] = AL_STR("al_addr_eq");
	sysFuncNumArgTbl[AIP_SysFuncAddrEq] = 2;
	sysFuncTbl[AIP_SysFuncAddrEq].func = &AIpProcess::EvalAddrEq;

	sysFuncNameTbl[AIP_SysFuncGetId] = AL_STR("al_get_id");
	sysFuncNumArgTbl[AIP_SysFuncGetId] = 0;
	sysFuncTbl[AIP_SysFuncGetId].func = &AIpProcess::EvalGetId;
	sysFuncNameTbl[AIP_SysFuncReleaseId] = AL_STR("al_release_id");
	sysFuncNumArgTbl[AIP_SysFuncReleaseId] = 1;
	sysFuncTbl[AIP_SysFuncReleaseId].func = &AIpProcess::EvalReleaseId;

	sysFuncNameTbl[AIP_SysFuncIsClass] = AL_STR("al_is_class");
	sysFuncNumArgTbl[AIP_SysFuncIsClass] = 2;
	sysFuncTbl[AIP_SysFuncIsClass].func = &AIpProcess::EvalIsClass;
	sysFuncNameTbl[AIP_SysFuncIsDerived] = AL_STR("al_is_derived");
	sysFuncNumArgTbl[AIP_SysFuncIsDerived] = 2;
	sysFuncTbl[AIP_SysFuncIsDerived].func = &AIpProcess::EvalIsDerived;

// parallel processing
	sysFuncNameTbl[AIP_SysFuncNextProcess] = AL_STR("al_next_process");
	sysFuncNumArgTbl[AIP_SysFuncNextProcess] = 0;
	sysFuncTbl[AIP_SysFuncNextProcess].func = &AIpProcess::EvalNextProcess;
	sysFuncNameTbl[AIP_SysFuncExitProcess] = AL_STR("al_exit_process");
	sysFuncNumArgTbl[AIP_SysFuncExitProcess] = 0;
	sysFuncTbl[AIP_SysFuncExitProcess].func = &AIpProcess::EvalExitProcess;

	sysFuncNameTbl[AIP_SysFuncWaitProcess] = AL_STR("al_wait");
	sysFuncNumArgTbl[AIP_SysFuncWaitProcess] = 1;
	sysFuncTbl[AIP_SysFuncWaitProcess].func = &AIpProcess::EvalWaitProcess;
	sysFuncNameTbl[AIP_SysFuncNotifyProcess] = AL_STR("al_notify");
	sysFuncNumArgTbl[AIP_SysFuncNotifyProcess] = 1;
	sysFuncTbl[AIP_SysFuncNotifyProcess].func = &AIpProcess::EvalNotifyProcess;

// clipboard
	sysFuncNameTbl[AIP_SysFuncSetClipboard] = AL_STR("al_set_clipboard");
	sysFuncNumArgTbl[AIP_SysFuncSetClipboard] = 1;
	sysFuncTbl[AIP_SysFuncSetClipboard].func = &AIpProcess::EvalSetToClipboard;
	sysFuncNameTbl[AIP_SysFuncGetClipboard] = AL_STR("al_get_clipboard");
	sysFuncNumArgTbl[AIP_SysFuncGetClipboard] = 0;
	sysFuncTbl[AIP_SysFuncGetClipboard].func = &AIpProcess::EvalGetFromClipboard;

// garbage collection
	sysFuncNameTbl[AIP_SysFuncGC] = AL_STR("al_gc");
	sysFuncNumArgTbl[AIP_SysFuncGC] = 1;
	sysFuncTbl[AIP_SysFuncGC].func = &AIpProcess::EvalGC;

// execution of application
	sysFuncNameTbl[AIP_SysFuncExec] = AL_STR("al_exec");
	sysFuncNumArgTbl[AIP_SysFuncExec] = 4;
	sysFuncTbl[AIP_SysFuncExec].func = &AIpProcess::EvalExec;

// Graph programming
	sysFuncNameTbl[AIP_SysFuncGraphProg] = AL_STR("al_gp");
	sysFuncNumArgTbl[AIP_SysFuncGraphProg] = 5;
	sysFuncTbl[AIP_SysFuncGraphProg].func = &AIpProcess::EvalGraphProg;

// library management
	sysFuncNameTbl[AIP_SysFuncLibrary] = AL_STR("al_library");
	sysFuncNumArgTbl[AIP_SysFuncLibrary] = 2;
	sysFuncTbl[AIP_SysFuncLibrary].func = &AIpProcess::EvalLibrary;

// window message
	sysFuncNameTbl[AIP_SysFuncWndMessage] = AL_STR("al_wnd_message");
	sysFuncNumArgTbl[AIP_SysFuncWndMessage] = 4;
	sysFuncTbl[AIP_SysFuncWndMessage].func = &AIpProcess::EvalWndMessage;

// Altair script
	sysFuncNameTbl[AIP_SysFuncScript] = AL_STR("al_script");
	sysFuncNumArgTbl[AIP_SysFuncScript] = 3;
	sysFuncTbl[AIP_SysFuncScript].func = &AIpProcess::EvalScript;

// XML
	sysFuncNameTbl[AIP_SysFuncXML] = AL_STR("al_xml");
	sysFuncNumArgTbl[AIP_SysFuncXML] = 4;
	sysFuncTbl[AIP_SysFuncXML].func = &AIpProcess::EvalXML;

// SQL
	sysFuncNameTbl[AIP_SysFuncSQL] = AL_STR("al_sql");
	sysFuncNumArgTbl[AIP_SysFuncSQL] = 5;
	sysFuncTbl[AIP_SysFuncSQL].func = &AIpProcess::EvalSQL;

// Socket
	sysFuncNameTbl[AIP_SysFuncSocket] = AL_STR("al_socket");
	sysFuncNumArgTbl[AIP_SysFuncSocket] = 4;
	sysFuncTbl[AIP_SysFuncSocket].func = &AIpProcess::EvalSocket;

// cryto graphy
	sysFuncNameTbl[AIP_SysFuncCrypt] = AL_STR("al_crypt");
	sysFuncNumArgTbl[AIP_SysFuncCrypt] = 4;
	sysFuncTbl[AIP_SysFuncCrypt].func = &AIpProcess::EvalCrypt;

// DLL
	sysFuncNameTbl[AIP_SysFuncUseDll] = AL_STR("al_dll");
	sysFuncNumArgTbl[AIP_SysFuncUseDll] = 4;
	sysFuncTbl[AIP_SysFuncUseDll].func = &AIpProcess::EvalUseDll;

// profile
	sysFuncNameTbl[AIP_SysFuncProf] = AL_STR("al_prof");
	sysFuncNumArgTbl[AIP_SysFuncProf] = 3;
	sysFuncTbl[AIP_SysFuncProf].func = &AIpProcess::EvalProf;

// solid modeling
	sysFuncNameTbl[AIP_SysFuncSolid] = AL_STR("al_solid");
	sysFuncNumArgTbl[AIP_SysFuncSolid] = 5;
	sysFuncTbl[AIP_SysFuncSolid].func = &AIpProcess::EvalSolid;

// Japanese Natural Language Manipulation
	sysFuncNameTbl[AIP_SysFuncJpAna] = AL_STR("al_jp_ana");
	sysFuncNumArgTbl[AIP_SysFuncJpAna] = 5;
	sysFuncTbl[AIP_SysFuncJpAna].func = &AIpProcess::EvalJpAna;

// Symbolic Manipulation
	sysFuncNameTbl[AIP_SysFuncSymbolManip] = AL_STR("al_symbol_manip");
	sysFuncNumArgTbl[AIP_SysFuncSymbolManip] = 5;
	sysFuncTbl[AIP_SysFuncSymbolManip].func = &AIpProcess::EvalSymbolManip;

// misc
	sysFuncNameTbl[AIP_SysFuncMisc] = AL_STR("al_misc");
	sysFuncNumArgTbl[AIP_SysFuncMisc] = 3;
	sysFuncTbl[AIP_SysFuncMisc].func = &AIpProcess::EvalMisc;

// OCX
	sysFuncNameTbl[AIP_SysFuncOcx] = AL_STR("al_ocx");
	sysFuncNumArgTbl[AIP_SysFuncOcx] = 4;
	sysFuncTbl[AIP_SysFuncOcx].func = &AIpProcess::EvalOcx;
	sysFuncNameTbl[AIP_SysFuncOcxSetMethod] = AL_STR("al_set_method");
	sysFuncNumArgTbl[AIP_SysFuncOcxSetMethod] = 4;
	sysFuncTbl[AIP_SysFuncOcxSetMethod].func = &AIpProcess::EvalOcxSetMethod;
	sysFuncNameTbl[AIP_SysFuncOcxInvokeArg] = AL_STR("al_arg");
	sysFuncNumArgTbl[AIP_SysFuncOcxInvokeArg] = 3;
	sysFuncTbl[AIP_SysFuncOcxInvokeArg].func = &AIpProcess::EvalOcxInvokeArg;
	sysFuncNameTbl[AIP_SysFuncOcxFireEvent] = AL_STR("al_fire_event");
	sysFuncNumArgTbl[AIP_SysFuncOcxFireEvent] = 3;
	sysFuncTbl[AIP_SysFuncOcxFireEvent].func = &AIpProcess::EvalOcxFireEvent;
	sysFuncNameTbl[AIP_SysFuncRootClass] = AL_STR("al_root_class");
	sysFuncNumArgTbl[AIP_SysFuncRootClass] = 0;
	sysFuncTbl[AIP_SysFuncRootClass].func = &AIpProcess::EvalRootClass;

	sysFuncNameTbl[AIP_SysFuncOleMisc] = AL_STR("al_ole_misc");
	sysFuncNumArgTbl[AIP_SysFuncOleMisc] = 3;
	sysFuncTbl[AIP_SysFuncOleMisc].func = &AIpProcess::EvalOleMisc;

	sysFuncNameTbl[AIP_SysFuncEnd] = NULL;
	sysFuncNumArgTbl[AIP_SysFuncEnd] = 0;
	sysFuncTbl[AIP_SysFuncEnd].func = NULL;

#ifdef	_DEBUG
	sysFuncTbl[AIP_SysFuncSqrt].name = "EvalSqrt";
	sysFuncTbl[AIP_SysFuncSin].name = "EvalSin";
	sysFuncTbl[AIP_SysFuncCos].name = "EvalCos";
	sysFuncTbl[AIP_SysFuncTan].name = "EvalTan";
	sysFuncTbl[AIP_SysFuncASin].name = "EvalASin";
	sysFuncTbl[AIP_SysFuncACos].name = "EvalACos";
	sysFuncTbl[AIP_SysFuncATan].name = "EvalATan";
	sysFuncTbl[AIP_SysFuncExp].name = "EvalExp";
	sysFuncTbl[AIP_SysFuncLog].name = "EvalLog";
	sysFuncTbl[AIP_SysFuncMax].name = "EvalMax";
	sysFuncTbl[AIP_SysFuncMin].name = "EvalMin";
	sysFuncTbl[AIP_SysFuncAbs].name = "EvalAbs";
	sysFuncTbl[AIP_SysFuncMathMisc].name = "EvalMathMisc";

	sysFuncTbl[AIP_SysFuncCons].name = "EvalCons";
	sysFuncTbl[AIP_SysFuncAppendL].name = "EvalAppendL";
	sysFuncTbl[AIP_SysFuncList2].name = "EvalList2";
	sysFuncTbl[AIP_SysFuncList3].name = "EvalList3";
	sysFuncTbl[AIP_SysFuncList4].name = "EvalList4";
	sysFuncTbl[AIP_SysFuncList5].name = "EvalList5";
	sysFuncTbl[AIP_SysFuncList6].name = "EvalList6";
	sysFuncTbl[AIP_SysFuncList7].name = "EvalList7";
	sysFuncTbl[AIP_SysFuncList8].name = "EvalList8";
	sysFuncTbl[AIP_SysFuncIsType].name = "EvalIsType";
	sysFuncTbl[AIP_SysFuncAddrEq].name = "EvalAddrEq";
	sysFuncTbl[AIP_SysFuncScanList].name = "EvalScanList";
	sysFuncTbl[AIP_SysFuncPrintList].name = "EvalPrintList";
	sysFuncTbl[AIP_SysFuncListMisc].name = "EvalListMisc";

	sysFuncTbl[AIP_SysFuncCreateArray].name = "EvalCreateArray";
	sysFuncTbl[AIP_SysFuncArrayLength].name = "EvalArrayLength";

	sysFuncTbl[AIP_SysFuncDstItr].name = "EvalDstItr";
	sysFuncTbl[AIP_SysFuncSrcItr].name = "EvalSrcItr";
	sysFuncTbl[AIP_SysFuncNext].name = "EvalNext";
	sysFuncTbl[AIP_SysFuncNextA].name = "EvalNextA";
	sysFuncTbl[AIP_SysFuncNextA2].name = "EvalNextA2";
	sysFuncTbl[AIP_SysFuncPrev].name = "EvalPrev";
	sysFuncTbl[AIP_SysFuncPrevA].name = "EvalPrevA";
	sysFuncTbl[AIP_SysFuncPrevA2].name = "EvalPrevA2";
	sysFuncTbl[AIP_SysFuncCompo].name = "EvalCompo";
	sysFuncTbl[AIP_SysFuncInsert].name = "EvalInsert";
	sysFuncTbl[AIP_SysFuncAppend].name = "EvalAppend";
	sysFuncTbl[AIP_SysFuncRemove].name = "EvalRemove";
	sysFuncTbl[AIP_SysFuncGetArcA].name = "EvalGetArcA";
	sysFuncTbl[AIP_SysFuncSetArcA].name = "EvalSetArcA";
	sysFuncTbl[AIP_SysFuncCount].name = "EvalCount";
	sysFuncTbl[AIP_SysFuncIndexPos].name = "EvalIndexPos";
	sysFuncTbl[AIP_SysFuncSearch].name = "EvalSearch";
	sysFuncTbl[AIP_SysFuncCreateArc].name = "EvalCreateArc";
	sysFuncTbl[AIP_SysFuncCreateAssocArc].name = "EvalCreateAssocArc";
	sysFuncTbl[AIP_SysFuncCreateOrderedArc].name = "EvalCreateOrderedArc";
	sysFuncTbl[AIP_SysFuncDstNode].name = "EvalDstNode";
	sysFuncTbl[AIP_SysFuncSrcNode].name = "EvalSrcNode";
	sysFuncTbl[AIP_SysFuncSetDstNode].name = "EvalSetDstNode";
	sysFuncTbl[AIP_SysFuncNextDstNode].name = "EvalNextDstNode";
	sysFuncTbl[AIP_SysFuncPrevDstNode].name = "EvalPrevDstNode";
	sysFuncTbl[AIP_SysFuncNextSrcNode].name = "EvalNextSrcNode";
	sysFuncTbl[AIP_SysFuncPrevSrcNode].name = "EvalPrevSrcNode";
	sysFuncTbl[AIP_SysFuncInsertBefore].name = "EvalInsertBefore";
	sysFuncTbl[AIP_SysFuncAppendAfter].name = "EvalAppendAfter";
	sysFuncTbl[AIP_SysFuncRemoveFrom].name = "EvalRemoveFrom";
	sysFuncTbl[AIP_SysFuncCopy].name = "EvalCopy";
	sysFuncTbl[AIP_SysFuncDstItr2].name = "EvalDstItr2";
	sysFuncTbl[AIP_SysFuncArcDst].name = "EvalArcDst";
	sysFuncTbl[AIP_SysFuncSrcItr2].name = "EvalSrcItr2";
	sysFuncTbl[AIP_SysFuncArcSrc].name = "EvalArcSrc";
	sysFuncTbl[AIP_SysFuncGraphMisc].name = "EvalGraphMisc";

	sysFuncTbl[AIP_SysFuncCreatePolyPoly].name = "EvalCreatePolyPoly";
	sysFuncTbl[AIP_SysFuncCreateNextPoly].name = "EvalCreateNextPoly";
	sysFuncTbl[AIP_SysFuncAddPointToPoly].name = "EvalAddPointToPoly";
	sysFuncTbl[AIP_SysFuncAddRectToPoly].name = "EvalAddRectToPoly";
	sysFuncTbl[AIP_SysFuncAddCurveToPoly].name = "EvalAddCurveToPoly";
	sysFuncTbl[AIP_SysFuncIntersection].name = "EvalIntersection";
	sysFuncTbl[AIP_SysFuncUnion].name = "EvalUnion";
	sysFuncTbl[AIP_SysFuncUnitTM].name = "EvalUnitTM";
	sysFuncTbl[AIP_SysFuncTranslateTM].name = "EvalTranslateTM";
	sysFuncTbl[AIP_SysFuncScaleTM].name = "EvalScaleTM";
	sysFuncTbl[AIP_SysFuncRotateTM].name = "EvalRotateTM";
	sysFuncTbl[AIP_SysFuncTransform].name = "EvalTransformXY";
	sysFuncTbl[AIP_SysFuncInverse].name = "EvalInverseXY";
	sysFuncTbl[AIP_SysFuncTransformP].name = "EvalTransformPoly";
	sysFuncTbl[AIP_SysFuncInverseP].name = "EvalInversePoly";
	sysFuncTbl[AIP_SysFuncTransformRect].name = "EvalTransformRect";

	sysFuncTbl[AIP_SysFuncPrint].name = "EvalPrint";
	sysFuncTbl[AIP_SysFuncAsk].name = "EvalAsk";
	sysFuncTbl[AIP_SysFuncMenu].name = "EvalMenu";
	sysFuncTbl[AIP_SysFuncReadFileName].name = "EvalGetReadFileName";
	sysFuncTbl[AIP_SysFuncWriteFileName].name = "EvalGetWriteFileName";
	sysFuncTbl[AIP_SysFuncGetColor].name = "EvalGetColor";
	sysFuncTbl[AIP_SysFuncGetFont].name = "EvalGetFont";
	sysFuncTbl[AIP_SysFuncMessageText].name = "EvalMessageText";
	sysFuncTbl[AIP_SysFuncInputByCEdit].name = "EvalInputByCEdit";
	sysFuncTbl[AIP_SysFuncCreateWindow].name = "EvalCreateWindow";
	sysFuncTbl[AIP_SysFuncSetWindowArg].name = "EvalSetWindowArg";
	sysFuncTbl[AIP_SysFuncSetDrawProc].name = "EvalSetDrawProc";
	sysFuncTbl[AIP_SysFuncSetEventProc].name = "EvalSetEventProc";
	sysFuncTbl[AIP_SysFuncSetMoveCtrlProc].name = "EvalSetMoveCtrlProc";
	sysFuncTbl[AIP_SysFuncSetDestroyProc].name = "EvalSetDestroyProc";
	sysFuncTbl[AIP_SysFuncSetActivateProc].name = "EvalSetActivateProc";
	sysFuncTbl[AIP_SysFuncSetTitle].name = "EvalSetTitle";
	sysFuncTbl[AIP_SysFuncSetMenu].name = "EvalSetMenu";
	sysFuncTbl[AIP_SysFuncGetWndRect].name = "EvalGetWndRect";
	sysFuncTbl[AIP_SysFuncSetWndRect].name = "EvalSetWndRect";
	sysFuncTbl[AIP_SysFuncResizeWnd].name = "EvalResizeWnd";
	sysFuncTbl[AIP_SysFuncMoveWnd].name = "EvalMoveWnd";
	sysFuncTbl[AIP_SysFuncShowWnd].name = "EvalShowWnd";
	sysFuncTbl[AIP_SysFuncHideWnd].name = "EvalHideWnd";
	sysFuncTbl[AIP_SysFuncDestroyWnd].name = "EvalDestroyWnd";
	sysFuncTbl[AIP_SysFuncPushClipping].name = "EvalPushClipping";
	sysFuncTbl[AIP_SysFuncPopClipping].name = "EvalPopClipping";
	sysFuncTbl[AIP_SysFuncClipping].name = "EvalClipping";
	sysFuncTbl[AIP_SysFuncIsClipped].name = "EvalIsClipped";
	sysFuncTbl[AIP_SysFuncPushBgColor].name = "EvalPushBgColor";
	sysFuncTbl[AIP_SysFuncPopBgColor].name = "EvalPopBgColor";
	sysFuncTbl[AIP_SysFuncPushColorFunc].name = "EvalPushColorFunc";
	sysFuncTbl[AIP_SysFuncPopColorFunc].name = "EvalPopColorFunc";
	sysFuncTbl[AIP_SysFuncGetSysColor].name = "EvalGetSysColor";
	sysFuncTbl[AIP_SysFuncPushPen].name = "EvalPushPen";
	sysFuncTbl[AIP_SysFuncPopPen].name = "EvalPopPen";
	sysFuncTbl[AIP_SysFuncPushBrush].name = "EvalPushBrush";
	sysFuncTbl[AIP_SysFuncPopBrush].name = "EvalPopBrush";
	sysFuncTbl[AIP_SysFuncPushFont].name = "EvalPushFont";
	sysFuncTbl[AIP_SysFuncPushFontEx].name = "EvalPushFontEx";
	sysFuncTbl[AIP_SysFuncPopFont].name = "EvalPopFont";
	sysFuncTbl[AIP_SysFuncPushTM].name = "EvalPushTM";
	sysFuncTbl[AIP_SysFuncPopTM].name = "EvalPopTM";
	sysFuncTbl[AIP_SysFuncWindowTM].name = "EvalWindowTM";
	sysFuncTbl[AIP_SysFuncIsPrinting].name = "EvalIsPrinting";
	sysFuncTbl[AIP_SysFuncStartQuickDraw].name = "EvalStartQuickDraw";
	sysFuncTbl[AIP_SysFuncEndQuickDraw].name = "EvalEndQuickDraw";
	sysFuncTbl[AIP_SysFuncLine].name = "EvalLine";
	sysFuncTbl[AIP_SysFuncRectangle].name = "EvalRectangle";
	sysFuncTbl[AIP_SysFuncEllipse].name = "EvalEllipse";
	sysFuncTbl[AIP_SysFuncPolyPolyline].name = "EvalPolyPolyline";
	sysFuncTbl[AIP_SysFuncPolyPolygon].name = "EvalPolyPolygon";
	sysFuncTbl[AIP_SysFuncTextOut].name = "EvalTextOut";
	sysFuncTbl[AIP_SysFuncTextExtent].name = "EvalTextExtent";
	sysFuncTbl[AIP_SysFuncTextOutFromTo].name = "EvalTextOutFromTo";
	sysFuncTbl[AIP_SysFuncTextExtentFromTo].name = "EvalTextExtentFromTo";
	sysFuncTbl[AIP_SysFuncMTextOut].name = "EvalMTextOut";
	sysFuncTbl[AIP_SysFuncPie].name = "EvalPie";
	sysFuncTbl[AIP_SysFuncBitmap].name = "EvalBitmap";
	sysFuncTbl[AIP_SysFuncImage].name = "EvalImage";
	sysFuncTbl[AIP_SysFuncDamage].name = "EvalDamage";
	sysFuncTbl[AIP_SysFuncDamageRectPoly].name = "EvalDamageRectPoly";
	sysFuncTbl[AIP_SysFuncIsDamaged].name = "EvalIsDamaged";
	sysFuncTbl[AIP_SysFuncRepair].name = "EvalRepair";
	sysFuncTbl[AIP_SysFuncRedraw].name = "EvalRedraw";
	sysFuncTbl[AIP_SysFuncLocalLoop].name = "EvalLocalLoop";
	sysFuncTbl[AIP_SysFuncExitLoop].name = "EvalExitLoop";
	sysFuncTbl[AIP_SysFuncGetId].name = "EvalGetId";
	sysFuncTbl[AIP_SysFuncReleaseId].name = "EvalReleaseId";
	sysFuncTbl[AIP_SysFuncFilePrint].name = "EvalFilePrint";
	sysFuncTbl[AIP_SysFuncGuiMisc].name = "EvalGuiMisc";

	sysFuncTbl[AIP_SysFuncFileOpen].name = "EvalFileOpen";
	sysFuncTbl[AIP_SysFuncFileRead].name = "EvalFileRead";
	sysFuncTbl[AIP_SysFuncFileWrite].name = "EvalFileWrite";
	sysFuncTbl[AIP_SysFuncFileManip].name = "EvalFileManip";
	sysFuncTbl[AIP_SysFuncFileMatchStr].name = "EvalFileMatchStr";

	sysFuncTbl[AIP_SysFuncStrLen].name = "EvalStrLen";
	sysFuncTbl[AIP_SysFuncGetChar].name = "EvalGetChar";
	sysFuncTbl[AIP_SysFuncSetChar].name = "EvalSetChar";
	sysFuncTbl[AIP_SysFuncSubStr].name = "EvalSubStr";
	sysFuncTbl[AIP_SysFuncMatchStr].name = "EvalMatchStr";
	sysFuncTbl[AIP_SysFuncTailStr].name = "EvalTailStr";
	sysFuncTbl[AIP_SysFuncInsertChar].name = "EvalInsertChar";
	sysFuncTbl[AIP_SysFuncDeleteChar].name = "EvalDeleteChar";
	sysFuncTbl[AIP_SysFuncSearchStr].name = "EvalSearchStr";
	sysFuncTbl[AIP_SysFuncReplaceStr].name = "EvalReplaceStr";
	sysFuncTbl[AIP_SysFuncAppendStr].name = "EvalAppendStr";
	sysFuncTbl[AIP_SysFuncIsFirstChar].name = "EvalIsFirstChar";
	sysFuncTbl[AIP_SysFuncIsNormalChar].name = "EvalIsNormalChar";
	sysFuncTbl[AIP_SysFuncStrMisc].name = "EvalStrMisc";

	sysFuncTbl[AIP_SysFuncIsClass].name = "EvalIsClass";
	sysFuncTbl[AIP_SysFuncIsDerived].name = "EvalIsDerived";
	sysFuncTbl[AIP_SysFuncSetClipboard].name = "EvalSetToClipboard";
	sysFuncTbl[AIP_SysFuncGetClipboard].name = "EvalGetFromClipboard";
	sysFuncTbl[AIP_SysFuncWndMessage].name = "EvalWndMessage";

	sysFuncTbl[AIP_SysFuncNextProcess].name = "EvalNextProcess";
	sysFuncTbl[AIP_SysFuncExitProcess].name = "EvalExitProcess";
	sysFuncTbl[AIP_SysFuncGraphProg].name = "EvalGraphProg";
	sysFuncTbl[AIP_SysFuncGC].name = "EvalGC";
	sysFuncTbl[AIP_SysFuncExec].name = "EvalExec";
	sysFuncTbl[AIP_SysFuncUseDll].name = "EvalUseDll";
	sysFuncTbl[AIP_SysFuncLibrary].name = "EvalLibrary";
	sysFuncTbl[AIP_SysFuncScript].name = "EvalScript";

	sysFuncTbl[AIP_SysFuncSocket].name = "EvalSocket";
	sysFuncTbl[AIP_SysFuncCrypt].name = "EvalCrypt";
	sysFuncTbl[AIP_SysFuncSQL].name = "EvalSQL";
	sysFuncTbl[AIP_SysFuncXML].name = "EvalXML";
	sysFuncTbl[AIP_SysFuncMisc].name = "EvalMisc";

	sysFuncTbl[AIP_SysFuncOcx].name = "EvalOcx";
	sysFuncTbl[AIP_SysFuncOcxSetMethod].name = "EvalOcxSetMethod";
	sysFuncTbl[AIP_SysFuncOcxInvokeArg].name = "EvalOcxInvokeArg";
	sysFuncTbl[AIP_SysFuncOcxFireEvent].name = "EvalOcxFireEvent";
	sysFuncTbl[AIP_SysFuncRootClass].name = "EvalRootClass";
	sysFuncTbl[AIP_SysFuncOleMisc].name = "EvalOleMisc";
#endif
	
	_init_tbl = 1;
	return 0;
}
void AIpProcess::ReleaseFuncTbl() {
	if (_init_tbl == 0) return;
	delete [] evalFuncTbl; delete [] retFuncTbl;
	delete [] sysFuncTbl; delete [] sysFuncNameTbl; delete [] sysFuncNumArgTbl;
	_init_tbl = 0;
}
