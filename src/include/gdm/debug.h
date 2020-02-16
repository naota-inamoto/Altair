/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ALTAIR_DEBUG_H
#define _ALTAIR_DEBUG_H

long NumUsedCell(long* garbage=NULL);

void Dump(const char*, AGmFile* f=NULL, ...);

void DumpList(void*, const char*, AGmNode*, int depth=10, AGmFile* f=NULL, int hex=0);
void DumpList(const char*, AGmNode*, int depth=10, AGmFile* f=NULL, int hex=0);
void DumpList(AGmNode*, int depth=10, AGmFile* f=NULL);
void DumpList(void*, const char*, AGmNode*, int depth, int max_line, AGmFile* f=NULL, int hex=0);
void DumpList(const char*, AGmNode*, int depth, int max_line, AGmFile* f=NULL, int hex=0);
void DumpList(AGmNode*, int depth, int max_line, AGmFile* f=NULL);

void DumpGraph(void*, const char*, AGmNode*, int depth=10, AGmFile* f=NULL, int hex=0, AGmNode* r=NULL);
void DumpGraph(const char*, AGmNode*, int depth=10, AGmFile* f=NULL, int hex=0, AGmNode* r=NULL);
void DumpGraph(AGmNode*, int depth=10, AGmFile* f=NULL, int hex=0, AGmNode* r=NULL);
void DumpGraph(void*, const char*, AGmNode*, int depth, int max_line, AGmFile* f=NULL, int hex=0, AGmNode* r=NULL);
void DumpGraph(const char*, AGmNode*, int depth, int max_line, AGmFile* f=NULL, int hex=0, AGmNode* r=NULL);
void DumpGraph(AGmNode*, int depth, int max_line, AGmFile* f=NULL, int hex=0, AGmNode* r=NULL);
void DumpGraphSrc(const char*, AGmNode*, int depth=10, AGmFile* f=NULL, int hex=0);
void DumpGraphSrc(AGmNode*, int depth=10, AGmFile* f=NULL, int hex=0);
void DumpGraphSrc(const char*, AGmNode*, int depth, int max_line, AGmFile* f=NULL, int hex=0);
void DumpGraphSrc(AGmNode*, int depth, int max_line, AGmFile* f=NULL, int hex=0);
void print_node(AGmString*, AGmNode*, AGmFile*, int);

#if defined(_DEBUG) || defined(_LINUX_DEBUG)
void DebugAlloc(long, void*);
void DebugFree(void*);
void DebugLeakCells();
void DumpKind(AGmNode*);
void DumpKindOut(AGmNode*);

void CheckOutLink(const char* s, AGmNode* src);
void CheckInLink(const char* s, AGmNode* dst);
#endif

#endif /* _ALTAIR_DEBUG_H */
