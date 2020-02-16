/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AUI_SYSFUNC_H
#define _AUI_SYSFUNC_H

// arithmetic functions
#define AIP_SysFuncSqrt 			0
#define AIP_SysFuncSin				1
#define AIP_SysFuncCos				2
#define AIP_SysFuncTan				3
#define AIP_SysFuncASin 			4
#define AIP_SysFuncACos 			5
#define AIP_SysFuncATan 			6
#define AIP_SysFuncExp				7
#define AIP_SysFuncLog				8
#define AIP_SysFuncMax				9
#define AIP_SysFuncMin				10
#define AIP_SysFuncAbs				11
#define AIP_SysFuncMathMisc 		12

// list manipulation functions
#define AIP_SysFuncCons 			13
#define AIP_SysFuncAppendL			14
#define AIP_SysFuncList2			15
#define AIP_SysFuncList3			16
#define AIP_SysFuncList4			17
#define AIP_SysFuncList5			18
#define AIP_SysFuncList6			19
#define AIP_SysFuncList7			20
#define AIP_SysFuncList8			21
#define AIP_SysFuncScanList 		22
#define AIP_SysFuncPrintList		23
#define AIP_SysFuncListElem 		24
#define AIP_SysFuncSetListElem		25
#define AIP_SysFuncListMisc 		26

// array data manipulation functions
#define AIP_SysFuncCreateArray		27
#define AIP_SysFuncArrayLength		28

// graph data manipulation functions
#define AIP_SysFuncDstItr			29
#define AIP_SysFuncSrcItr			30
#define AIP_SysFuncNext 			31
#define AIP_SysFuncNextA			32
#define AIP_SysFuncNextA2			33
#define AIP_SysFuncPrev 			34
#define AIP_SysFuncPrevA			35
#define AIP_SysFuncPrevA2			36
#define AIP_SysFuncCompo			37
#define AIP_SysFuncInsert			38
#define AIP_SysFuncAppend			39
#define AIP_SysFuncRemove			40
#define AIP_SysFuncGetArcA			41
#define AIP_SysFuncSetArcA			42
#define AIP_SysFuncCount			43
#define AIP_SysFuncCount2			44
#define AIP_SysFuncIndexPos 		45
#define AIP_SysFuncSearch			46
#define AIP_SysFuncCreateArc		47
#define AIP_SysFuncCreateAssocArc	48
#define AIP_SysFuncCreateOrderedArc 49		
#define AIP_SysFuncDstNode			50
#define AIP_SysFuncDstNodeNoCase	51
#define AIP_SysFuncSrcNode			52
#define AIP_SysFuncSetDstNode		53
#define AIP_SysFuncNextDstNode		54
#define AIP_SysFuncPrevDstNode		55
#define AIP_SysFuncNextSrcNode		56
#define AIP_SysFuncPrevSrcNode		57
#define AIP_SysFuncInsertBefore 	58
#define AIP_SysFuncAppendAfter		59
#define AIP_SysFuncRemoveFrom		60
#define AIP_SysFuncCopy 			61
#define AIP_SysFuncDstItr2			62
#define AIP_SysFuncArcDst			63
#define AIP_SysFuncSrcItr2			64
#define AIP_SysFuncArcSrc			65
#define AIP_SysFuncGraphMisc		66

// PolyPoly data manipulation functions
#define AIP_SysFuncCreatePolyPoly	67
#define AIP_SysFuncCreateNextPoly	68
#define AIP_SysFuncAddPointToPoly	69
#define AIP_SysFuncAddRectToPoly	70
#define AIP_SysFuncAddCurveToPoly	71
#define AIP_SysFuncIntersection 	72
#define AIP_SysFuncUnion			73

// Matrix data manipulation funcitons
#define AIP_SysFuncUnitTM			74
#define AIP_SysFuncTranslateTM		75
#define AIP_SysFuncScaleTM			76
#define AIP_SysFuncRotateTM 		77
#define AIP_SysFuncTransform		78
#define AIP_SysFuncInverse			79
#define AIP_SysFuncTransformP		80
#define AIP_SysFuncInverseP 		81
#define AIP_SysFuncTransformRect	82

// graphics functions
#define AIP_SysFuncPrint			83
#define AIP_SysFuncAsk				84
#define AIP_SysFuncMenu 			85
#define AIP_SysFuncReadFileName 	86
#define AIP_SysFuncWriteFileName	87
#define AIP_SysFuncGetColor 		88
#define AIP_SysFuncGetFont			89
#define AIP_SysFuncMessageText		90
#define AIP_SysFuncInputByCEdit 	91

#define AIP_SysFuncCreateWindow 	92
#define AIP_SysFuncUserEdit 		93
#define AIP_SysFuncSetWindowArg 	94
#define AIP_SysFuncSetDrawProc		95
#define AIP_SysFuncSetEventProc 	96
#define AIP_SysFuncSetMoveCtrlProc	97
#define AIP_SysFuncSetDestroyProc	98
#define AIP_SysFuncSetActivateProc	99
#define AIP_SysFuncSetTextInsProc	100
#define AIP_SysFuncSetTitle 		101
#define AIP_SysFuncGetTitle 		102
#define AIP_SysFuncSetMenu			103
#define AIP_SysFuncGetWndRect		104
#define AIP_SysFuncSetWndRect		105
#define AIP_SysFuncResizeWnd		106
#define AIP_SysFuncMoveWnd			107
#define AIP_SysFuncShowWnd			108
#define AIP_SysFuncHideWnd			109
#define AIP_SysFuncDestroyWnd		110
								
#define AIP_SysFuncPushClipping 	111
#define AIP_SysFuncPopClipping		112
#define AIP_SysFuncClipping 		113
#define AIP_SysFuncIsClipped		114
#define AIP_SysFuncPushBgColor		115
#define AIP_SysFuncPopBgColor		116
#define AIP_SysFuncPushColorFunc	117
#define AIP_SysFuncPopColorFunc 	118
#define AIP_SysFuncGetSysColor		119
#define AIP_SysFuncPushPen			120
#define AIP_SysFuncPopPen			121
#define AIP_SysFuncPushBrush		122
#define AIP_SysFuncPopBrush 		123
#define AIP_SysFuncPushFont 		124
#define AIP_SysFuncPopFont			125
#define AIP_SysFuncPushFontEx		126
#define AIP_SysFuncPushTM			127
#define AIP_SysFuncPopTM			128
#define AIP_SysFuncWindowTM 		129
#define AIP_SysFuncIsPrinting		130

#define AIP_SysFuncStartQuickDraw	131
#define AIP_SysFuncEndQuickDraw 	132

#define AIP_SysFuncPoint			133
#define AIP_SysFuncLine 			134
#define AIP_SysFuncRectangle		135
#define AIP_SysFuncEllipse			136
#define AIP_SysFuncPolyPolyline 	137
#define AIP_SysFuncPolyPolygon		138
#define AIP_SysFuncTextOut			139
#define AIP_SysFuncTextExtent		140
#define AIP_SysFuncTextOutFromTo	141
#define AIP_SysFuncTextExtentFromTo 142
#define AIP_SysFuncMTextOut 		143
#define AIP_SysFuncPie				144
#define AIP_SysFuncBitmap			145
#define AIP_SysFuncImage			146

#define AIP_SysFuncDamage			147
#define AIP_SysFuncDamageRectPoly	148
#define AIP_SysFuncIsDamaged		149
#define AIP_SysFuncRepair			150
#define AIP_SysFuncRedraw			151
#define AIP_SysFuncLocalLoop		152
#define AIP_SysFuncExitLoop 		153

#define AIP_SysFuncGetId			154
#define AIP_SysFuncReleaseId		155
#define AIP_SysFuncFilePrint		156

#define AIP_SysFuncGuiMisc			157

// file manipulation functions
#define AIP_SysFuncFileOpen 		158
#define AIP_SysFuncFileRead 		159
#define AIP_SysFuncFileWrite		160
#define AIP_SysFuncFileManip		161
#define AIP_SysFuncFileMatchStr 	162

// string manipulation functions
#define AIP_SysFuncStrLen			163
#define AIP_SysFuncGetChar			164
#define AIP_SysFuncSetChar			165
#define AIP_SysFuncSubStr			166
#define AIP_SysFuncMatchStr 		167
#define AIP_SysFuncTailStr			168
#define AIP_SysFuncInsertChar		169
#define AIP_SysFuncDeleteChar		170
#define AIP_SysFuncSearchStr		171
#define AIP_SysFuncReplaceStr		172
#define AIP_SysFuncAppendStr		173
#define AIP_SysFuncIsFirstChar		174
#define AIP_SysFuncIsNormalChar 	175
#define AIP_SysFuncStrMisc			176
// misc
#define AIP_SysFuncIsType			177
#define AIP_SysFuncAddrEq			178
#define AIP_SysFuncIsClass			179
#define AIP_SysFuncIsDerived		180
#define AIP_SysFuncNextProcess		181
#define AIP_SysFuncExitProcess		182
#define AIP_SysFuncWaitProcess		183
#define AIP_SysFuncNotifyProcess	184
#define AIP_SysFuncSetClipboard 	185
#define AIP_SysFuncGetClipboard 	186
#define AIP_SysFuncGC				187
#define AIP_SysFuncGraphProg		188
#define AIP_SysFuncExec 			189
#define AIP_SysFuncLibrary			190
#define AIP_SysFuncWndMessage		191
#define AIP_SysFuncScript			192
#define AIP_SysFuncXML				193
#define AIP_SysFuncSQL				194
#define AIP_SysFuncMisc 			195
#define AIP_SysFuncSocket			196
#define AIP_SysFuncUseDll			197
#define AIP_SysFuncCrypt			198
#define AIP_SysFuncProf 			199
#define AIP_SysFuncSolid			200

#define AIP_SysFuncJpAna			201
#define AIP_SysFuncSymbolManip		202

// OCX
#define AIP_SysFuncOcx				203
#define AIP_SysFuncOcxSetMethod 	204
#define AIP_SysFuncOcxInvokeArg 	205
#define AIP_SysFuncOcxFireEvent 	206
#define AIP_SysFuncRootClass		207
#define AIP_SysFuncOleMisc			208

#define AIP_SysFuncEnd				209
#define AIP_NUM_SYSFUNC 			210

#endif	/* _AUI_SYSFUNC_H */
