/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AIP_BASEFUNC_H
#define _AIP_BASEFUNC_H

// control
#define AIP_EvalBlock		0
#define AIP_EvalLocVarDcl	1
#define AIP_EvalBranch		2
#define AIP_EvalLoop		3
#define AIP_EvalBreak		4
#define AIP_EvalCont		5
#define AIP_EvalSemicolon	6
#define AIP_EvalComment 	7
#define AIP_EvalCond		8
#define AIP_EvalOrOr		9
#define AIP_EvalAndAnd		10
#define AIP_EvalTry 		11

#define AIP_NUM_RET 		12

#define AIP_EvalThrow		12

// 2 term operation
	// compare
#define AIP_EvalEQ			13
#define AIP_EvalNE			14
#define AIP_EvalLT			15
#define AIP_EvalLE			16
#define AIP_EvalGE			17
#define AIP_EvalGT			18
	// +,-,*,/
#define AIP_EvalAdd 		19
#define AIP_EvalSub 		20
#define AIP_EvalMul 		21
#define AIP_EvalDiv 		22
#define AIP_EvalMod 		23
	// logic
#define AIP_EvalAnd 		24
#define AIP_EvalOr			25
#define AIP_EvalXor 		26
	// shift
#define AIP_EvalSL			27
#define AIP_EvalSR			28
// 1 term operation
#define AIP_EvalChgSign 	29
#define AIP_EvalPreInc		30
#define AIP_EvalPostInc 	31
#define AIP_EvalPreDec		32
#define AIP_EvalPostDec 	33
#define AIP_EvalInverse 	34
#define AIP_EvalNot 		35		
// read
#define AIP_EvalConstInt	36
#define AIP_EvalConstHexa	37
#define AIP_EvalConstQChar	38
#define AIP_EvalConstReal	39
#define AIP_EvalConstString 40
#define AIP_EvalConstVtbl	41
#define AIP_EvalConstFunc	42
#define AIP_EvalConstNull	43
#define AIP_EvalNew 		44
#define AIP_EvalThis		45
#define AIP_EvalLoc 		46
#define AIP_EvalArg 		47
#define AIP_EvalThisVar 	48
#define AIP_EvalClassVar	49
#define AIP_EvalObjVar		50
#define AIP_EvalHead		51	
#define AIP_EvalTail		52
#define AIP_EvalArray		53
#define AIP_EvalMatrix		54
#define AIP_EvalCast		55
#define AIP_EvalRuntime 	56	
// write
#define AIP_EvalSetLoc		57
#define AIP_EvalSetArg		58
#define AIP_EvalSetThisVar	59
#define AIP_EvalSetClassVar 60
#define AIP_EvalSetObjVar	61
#define AIP_EvalSetHead 	62
#define AIP_EvalSetTail 	63
#define AIP_EvalSetArray	64
#define AIP_EvalSetMatrix	65
// function call
#define AIP_EvalPush		66
#define AIP_EvalPop 		67
#define AIP_EvalClassFunc	68
#define AIP_EvalThisFunc	69
#define AIP_EvalObjFunc 	70
#define AIP_EvalObjFunc2	71
#define AIP_EvalClassFuncP	72
#define AIP_EvalThisFuncP	73
#define AIP_EvalObjFuncP	74
#define AIP_EvalRet 		75
#define AIP_EvalSysFunc 	76
// parallel preocessing
#define AIP_EvalParaClassFunc	77
#define AIP_EvalParaObjFunc 	78

// OCX
#define AIP_EvalOcxInvoke	79
#define AIP_EvalOcxSetProp	80

#define AIP_NUM_EVAL		81

#endif	/* _AIP_BASEFUNC_H */

