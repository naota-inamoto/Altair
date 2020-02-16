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
#include <gdm/file.h>
#include <gdm/debug.h>
#include <dev/advwnd.h>
#include <gdm/big_int.h>

#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_LINUX
#ifdef	_LINUX_DEBUG
#include <stdarg.h>
int trace(char *format, ...) {
	va_list args;
	va_start(args, format);
	int cc = vfprintf(stdout, format, args);
	fflush(stdout);
	va_end(args);
	return cc;
}
#endif
#endif

#if	_WINDOWS && _64BIT
long long NumUsedCell(long long* garbage) {
	long long ref_count_zero, ref_count_minus, num_new_free, num_free, num_use;
#else
long NumUsedCell(long* garbage) {
	long ref_count_zero, ref_count_minus, num_new_free, num_free, num_use;
#endif
	AGm::GCtestRefCount(&ref_count_zero, &ref_count_minus, &num_new_free, &num_free, &num_use);

	if (garbage) *garbage = num_new_free;
	return num_use;
}

// --------------------
void Dump(const char* format, AGmFile* f, ...) {
	char msg[4096];
	va_list args;
	va_start(args, f);
	art_vsprintf(msg, 4096, format, args);
	va_end(args);
	if (f) {
		f->WriteString(msg);
	} else {
		ADvWindow::Print(msg);
	}
}

// --------------------
static int num_line;
static int max_line;

// --------------------
int dump_list_max_len = 150;
static void _DumpList0(AGmNode*, int indent, int depth, AGmFile*);
static int _DumpList1(AGmString*, int count, AGmNode*, int depth);
static int _DumpList2(AGmString*, int count, AGmNode*, int depth);

void DumpList(void* ptr, const char* s, AGmNode* n, int depth, AGmFile* f, int hex) {
	if (f) {
		f->WriteString("this = 0x");
#if	_WINDOWS && _64BIT
		f->WriteHexa((long long)ptr);
#else
		f->WriteHexa((long)ptr);
#endif
		f->WriteString(", ");
	} else {
		char msg[256];
		art_sprintf_1(msg, 256, "this = 0x%lx, ", AL_PTR(ptr));
		ADvWindow::Print(msg);
	}
	DumpList(s, n, depth, f, hex);
}
void DumpList(const char* s, AGmNode* n, int depth, AGmFile* f, int hex) {
	if (f) {
		f->WriteString(s);
		if (hex) {
			f->WriteString("0x");
#if	_WINDOWS && _64BIT
			f->WriteHexa((long long)n);
#else
			f->WriteHexa((long)n);
#endif
			f->WriteString(" ");
		}
	} else {
		ADvWindow::Print(s);
		if (hex) {
			char hex[64];
			art_sprintf_1(hex, 64, "0x%lx ", AL_PTR(n));
			ADvWindow::Print(hex);
		}
	}
	DumpList(n, depth, f);
}
void DumpList(AGmNode *n, int depth, AGmFile* f) {
	num_line = 0; max_line = -1;
	_DumpList0(n, 0, depth, f);
	if (f) {
		f->WriteString("\n");
	} else {
		ADvWindow::Print("\n");
	}
}

void DumpList(void* ptr, const char* s, AGmNode* n, int depth, int max, AGmFile* f, int hex) {
	if (f) {
		f->WriteString("this = 0x");
#if	_WINDOWS && _64BIT
		f->WriteHexa((long long)ptr);
#else
		f->WriteHexa((long)ptr);
#endif
		f->WriteString(", ");
	} else {
		char msg[256];
		art_sprintf_1(msg, 256, "this = 0x%lx, ", AL_PTR(ptr));
		ADvWindow::Print(msg);
	}
	DumpList(s, n, depth, max, f, hex);
}
void DumpList(const char* s, AGmNode* n, int depth, int max, AGmFile* f, int hex) {
	if (f) {
		f->WriteString(s);
		if (hex) {
			f->WriteString("0x");
#if	_WINDOWS && _64BIT
			f->WriteHexa((long long)n);
#else
			f->WriteHexa((long)n);
#endif
			f->WriteString(" ");
		}
	} else {
		ADvWindow::Print(s);
		if (hex) {
			char hex[64];
			art_sprintf_1(hex, 64, "0x%lx ", AL_PTR(n));
			ADvWindow::Print(hex);
		}
	}
	DumpList(n, depth, max, f);
}
void DumpList(AGmNode* n, int depth, int max, AGmFile* f) {
	num_line = 0; max_line = max;
	_DumpList0(n, 0, depth, f);
	if (f) {
		f->WriteString("\n");
	} else {
		ADvWindow::Print("\n");
	}
}

static void _DumpList0(AGmNode *l, int indent, int depth, AGmFile* f) {
	char buf[256];
	AGmString* s = new AGmString(256);
	int i, count, first = 1;

	count = 0;
	for (i = 0; i < indent;  i++) { s->Append(' '); count += 2; }
	count = _DumpList1(s, count, l, depth);
	if (count <= dump_list_max_len) {
		if (f) {
			f->WriteString(s);
		} else {
			ADvWindow::Print(s->Str());
		}
		delete s; return;
	}
	if (l == NULL) return;

	if (indent > depth) {
		s->Copy(""); count = 0;
		for (i = 0; i < indent;  i++) { s->Append(' '); count += 2; }
		s->Append("...");
		if (f) {
			f->WriteString(s);
		} else {
			ADvWindow::Print(s->Str());
		}
		delete s; return;
	}
	AGmString *str;
	for (;;) {
		s->Copy(""); count = 0;
		for (i = 0; i < indent;  i++) { s->Append(' '); count += 2; }

		switch (l->type) {
		case AGM_NODE_INTEGER:
			art_sprintf_1(buf, 256, "%ld", l->a.i.v);
			s->Append(buf); l = NULL; break;
		case AGM_NODE_GFP:
			art_sprintf_2(buf, 256, "%lu(mod %lu)", l->a.gp.v, l->a.gp.mod);
			s->Append(buf); l = NULL; break;
		case AGM_NODE_REAL:
			art_sprintf_1(buf, 256, "%g", l->a.r.v);
			s->Append(buf); l = NULL; break;
		case AGM_NODE_BIGINT:
			str = BigInteger::ToString((long)l->a.bi.sign, l->a.bi.bin);
			s->Append(str); delete str; l = NULL; break;
		case AGM_NODE_STRING:
			s->WriteQString(l->a.s.v); l = NULL; break;
		case AGM_NODE_LIST:
			if (first) {
				first = 0;
				s->Append("(\n");
			} else {
				s->Copy(",\n");
			}
			if (f) {
				f->WriteString(s);
			} else {
				ADvWindow::Print(s->Str());
			}
			if (max_line > 0 && ++num_line > max_line) { delete s; return; }
			_DumpList0(l->a.l.h, indent + 1, depth, f);
			l = l->a.l.t;
			if (l == NULL) {
				if (f) {
					f->WriteString("\n");
				} else {
					ADvWindow::Print("\n");
				}
				s->Copy(""); count = 0;
				for (i = 0; i < indent;  i++) { s->Append(' '); count += 2; }
				s->Append(")");
			}
			if (max_line > 0 && ++num_line > max_line) { delete s; return; }
			break;
		case AGM_NODE_ARRAY:
			s->Append("array"); l = NULL; break;
		case AGM_NODE_OBJECT:
			s->Append("object"); l = NULL; break;
		case AGM_NODE_DST_ITR:
			s->Append("dst_itr"); l = NULL; break;
		case AGM_NODE_SRC_ITR:
			s->Append("src_itr"); l = NULL; break;
		case AGM_NODE_MATRIX:
			s->Append("matrix"); l = NULL; break;
		case AGM_NODE_FILE:
			s->Append("file"); l = NULL; break;
		case AGM_NODE_WINDOW:
			s->Append("window"); l = NULL; break;
		case AGM_NODE_BITMAP:
			s->Append("bitmap"); l = NULL; break;
		case AGM_NODE_PTR:
			s->Append("pointer"); l = NULL; break;
		case AGM_NODE_BINARY:
			s->Append("binary"); l = NULL; break;
		case AGM_NODE_OCX:
			s->Append("ocx"); l = NULL; break;
		case AGM_NODE_VARIANT:
			s->Append("variant"); l = NULL; break;
		case AGM_NODE_SQL_CONN:
			s->Append("connection"); l = NULL; break;
		case AGM_NODE_SQL_STAT:
			s->Append("statement"); l = NULL; break;
		default:
			s->Append("unknown"); l = NULL; break;
		}
		if (l == NULL) break;
	}
	if (f) {
		f->WriteString(s);
	} else {
		ADvWindow::Print(s->Str());
	}
	delete s;
}
static int _DumpList1(AGmString *s, int count, AGmNode *l, int depth) {
	char buf[256];
	if (l == NULL) { s->Append("null"); return count + 4; }
	AGmString *str;
	switch (l->type) {
	case AGM_NODE_INTEGER:
		count += art_sprintf_1(buf, 256, "%ld", l->a.i.v);
		s->Append(buf); break;
	case AGM_NODE_GFP:
		count += art_sprintf_2(buf, 256, "%lu(mod %lu)", l->a.gp.v, l->a.gp.mod);
		s->Append(buf); break;
	case AGM_NODE_REAL:
		count += art_sprintf_1(buf, 256, "%g", l->a.r.v);
		s->Append(buf); break;
	case AGM_NODE_STRING:
		str = new AGmString(64); str->WriteQString(l->a.s.v); s->Append(str);
		count += str->Length(); delete str;
		break;
	case AGM_NODE_BIGINT:
		str = BigInteger::ToString((long)l->a.bi.sign, l->a.bi.bin);
		count += str->Length(); s->Append(str); delete str;
		break;
	case AGM_NODE_LIST:
		s->Append("("); count += 1;
		count = _DumpList1(s, count, l->a.l.h, depth);
		if (count > dump_list_max_len) break;
		count = _DumpList2(s, count, l->a.l.t, depth);
		break;
	case AGM_NODE_ARRAY:
		s->Append("array"); count += 5;
		break;
	case AGM_NODE_OBJECT:
		s->Append("object"); count += 6;
		break;
	case AGM_NODE_DST_ITR:
		s->Append("dst_itr"); count += 7;
		break;
	case AGM_NODE_SRC_ITR:
		s->Append("src_itr"); count += 7;
		break;
	case AGM_NODE_MATRIX:
		s->Append("matrix"); count += 6;
		break;
	case AGM_NODE_FILE:
		s->Append("file"); count += 4;
		break;
	case AGM_NODE_WINDOW:
		s->Append("window"); count += 6;
		break;
	case AGM_NODE_BITMAP:
		s->Append("bitmap"); count += 6;
		break;
	case AGM_NODE_PTR:
		s->Append("pointer"); count += 7;
		break;
	case AGM_NODE_BINARY:
		s->Append("binary"); count += 6;
		break;
	case AGM_NODE_OCX:
		s->Append("ocx"); count += 3;
		break;
	case AGM_NODE_VARIANT:
		s->Append("variant"); count += 7;
		break;
	case AGM_NODE_SQL_CONN:
		s->Append("connection"); count += 10;
		break;
	case AGM_NODE_SQL_STAT:
		s->Append("statement"); count += 9;
		break;
	default:
		s->Append("unknown"); count += 7;
		break;
	}
	return count;
}
static int _DumpList2(AGmString *s, int count, AGmNode *l, int depth) {
	char buf[256];
	if (l == NULL) { s->Append(")"); return count + 1; }
	AGmString *str;
	switch (l->type) {
	case AGM_NODE_INTEGER:
	case AGM_NODE_GFP:
		count += art_sprintf_1(buf, 256, ", %ld", l->a.i.v);
		s->Append(buf);
		if (count > dump_list_max_len) break;
		count = _DumpList2(s, count, l->a.i.t, depth);
		break;
	case AGM_NODE_REAL:
		count += art_sprintf_1(buf, 256, ", %lg", l->a.r.v);
		s->Append(buf); break;
	case AGM_NODE_STRING:
		str = new AGmString(64); str->Append(", "); str->WriteQString(l->a.s.v); s->Append(str);
		count += str->Length(); delete str;
		break;
	case AGM_NODE_BIGINT:
		str = BigInteger::ToString((long)l->a.bi.sign, l->a.bi.bin);
		count += str->Length(); s->Append(str); delete str;
		break;
	case AGM_NODE_LIST:
		s->Append(", "); count += 2;
		count = _DumpList1(s, count, l->a.l.h, depth);
		if (count > dump_list_max_len) break;
		count = _DumpList2(s, count, l->a.l.t, depth);
		if (count > dump_list_max_len) break;
		break;
	case AGM_NODE_ARRAY:
		s->Append(", array"); count += 7;
		break;
	case AGM_NODE_OBJECT:
		s->Append(", object"); count += 8;
		break;
	case AGM_NODE_DST_ITR:
		s->Append(", dst_itr"); count += 9;
		break;
	case AGM_NODE_SRC_ITR:
		s->Append(", src_itr"); count += 9;
		break;
	case AGM_NODE_MATRIX:
		s->Append(", matrix"); count += 8;
		break;
	case AGM_NODE_FILE:
		s->Append(", file");
		count += 6;
		break;
	case AGM_NODE_WINDOW:
		s->Append(", window"); count += 8;
		break;
	case AGM_NODE_BITMAP:
		s->Append(", bitmap"); count += 8;
		break;
	case AGM_NODE_PTR:
		s->Append(", pointer"); count += 9;
		break;
	case AGM_NODE_BINARY:
		s->Append(", binary"); count += 8;
		break;
	case AGM_NODE_OCX:
		s->Append(", ocx"); count += 5;
		break;
	case AGM_NODE_VARIANT:
		s->Append(", variant"); count += 9;
		break;
	case AGM_NODE_SQL_CONN:
		s->Append(", connection"); count += 12;
		break;
	case AGM_NODE_SQL_STAT:
		s->Append(", statement"); count += 11;
		break;
	default:
		s->Append(", unknown"); count += 9;
		break;
	}
	return count;
}

// --------------------
#define LIST_LEN	50
static void dump_graph(int, int, AGmNode*, AGmFile*, int, AGmNode*);
static void dump_graph_src(int, int, AGmNode*, AGmFile*, int);
static AGmNode *stack = NULL;

void DumpGraph(void* ptr, const char* s, AGmNode* n, int depth, AGmFile* f, int hex, AGmNode* r) {
	if (f) {
		f->WriteString("this = 0x");
#if	_WINDOWS && _64BIT
		f->WriteHexa((long long)ptr);
#else
		f->WriteHexa((long)ptr);
#endif
		f->WriteString(", ");
	} else {
		char msg[256];
		art_sprintf_1(msg, 256, "this = 0x%lx, ", AL_PTR(ptr));
		ADvWindow::Print(msg);
	}
	DumpGraph(s, n, depth, f, hex, r);
}
void DumpGraph(const char* s, AGmNode* n, int depth, AGmFile* f, int hex, AGmNode* r) {
	if (f) {
		f->WriteString(s);
	} else {
		ADvWindow::Print(s);
	}
	DumpGraph(n, depth, f, hex, r);
}
void DumpGraphSrc(const char* s, AGmNode* n, int depth, AGmFile* f, int hex) {
	if (f) {
		f->WriteString(s);
	} else {
		ADvWindow::Print(s);
	}
	DumpGraphSrc(n, depth, f, hex);
}
void DumpGraph(AGmNode* n, int depth, AGmFile* f, int hex, AGmNode* r) {
	num_line = 0; max_line = -1;
	print_node(NULL, n, f, hex);
	if (n) dump_graph(0, depth, n, f, hex, r);
}
void DumpGraphSrc(AGmNode* n, int depth, AGmFile* f, int hex) {
	num_line = 0; max_line = -1;
	print_node(NULL, n, f, hex);
	if (n) dump_graph_src(0, depth, n, f, hex);
}

void DumpGraph(void* ptr, const char* s, AGmNode* n, int depth, int max, AGmFile* f, int hex, AGmNode* r) {
	if (f) {
		f->WriteString("this = 0x");
#if	_WINDOWS && _64BIT
		f->WriteHexa((long long)ptr);
#else
		f->WriteHexa((long)ptr);
#endif
		f->WriteString(", ");
	} else {
		char msg[256];
		art_sprintf_1(msg, 256, "this = 0x%lx, ", AL_PTR(ptr));
		ADvWindow::Print(msg);
	}
	DumpGraph(s, n, depth, max, f, hex, r);
}
void DumpGraph(const char* s, AGmNode* n, int depth, int max, AGmFile* f, int hex, AGmNode* r) {
	if (f) {
		f->WriteString(s);
	} else {
		ADvWindow::Print(s);
	}
	DumpGraph(n, depth, max, f, hex, r);
}
void DumpGraphSrc(const char* s, AGmNode* n, int depth, int max, AGmFile* f, int hex) {
	if (f) {
		f->WriteString(s);
	} else {
		ADvWindow::Print(s);
	}
	DumpGraphSrc(n, depth, max, f, hex);
}
void DumpGraph(AGmNode* n, int depth, int max, AGmFile* f, int hex, AGmNode* r) {
	num_line = 0; max_line = max;
	print_node(NULL, n, f, hex);
	if (n) dump_graph(0, depth, n, f, hex, r);
}
void DumpGraphSrc(AGmNode* n, int depth, int max, AGmFile* f, int hex) {
	num_line = 0; max_line = max;
	print_node(NULL, n, f, hex);
	if (n) dump_graph_src(0, depth, n, f, hex);
}

void print_node(AGmString *s, AGmNode *n, AGmFile *f, int hex) {
	char v[LIST_LEN]; int count = 0;
	AGmString *str = new AGmString(10);
	AGmFile *ff = new AGmFile();
	ff->Create(str, AGM_FILE_STR_WRITE);
	ff->WriteList(n, count, LIST_LEN);
	if (str->Length() < LIST_LEN) {
		art_strcpy(v, LIST_LEN, str->Str());
	} else {
		art_strncpy(v, LIST_LEN, str->Str(), LIST_LEN - 4);
		v[LIST_LEN - 4] = 0;
		art_strcat(v, LIST_LEN, "...");
	}
	delete ff; delete str;
	if (s) {
		s->Append(v);
		if (hex) {
			s->WriteString(" ");
#if	_WINDOWS && _64BIT
			s->WriteHexa((long)n);
#else
			s->WriteHexa((long)n);
#endif
			s->WriteString(" cnt = "); s->WriteInteger(n->count);
		}
	} else {
		if (f) {
			f->WriteString(v);
			if (n && hex) {
				f->WriteString(" ");
#if	_WINDOWS && _64BIT
				f->WriteHexa((long long)n);
#else
				f->WriteHexa((long)n);
#endif
				f->WriteString(" cnt = "); f->WriteInteger(n->count);
			}
			f->WriteString("\n");
		} else {
			ADvWindow::Print(v);
			if (n && hex) {
				art_sprintf_2(v, LIST_LEN, " 0x%lx cnt = %ld", AL_PTR(n), n->count);
				ADvWindow::Print(v);
			}
			ADvWindow::Print("\n");
		}
	}
}
static void dump_graph(int indent, int depth, AGmNode *n, AGmFile *f, int hex, AGmNode* r) {
	if (depth <= 0) return;
	AGmLink *l;
	AGmNode *n2, *a2;
	for (l = AGm::next_outlink(n, NULL); l; l = AGm::next_outlink(n, l)) {
		n2 = l->arc->dst; a2 = l->arc->attr;
		AGmNode *ls = stack;
		for (ls = stack; ls; ls = ls->T) {
			if (ls->H == n2) return;
		}
		int i; AGmString* s = new AGmString(64);
		for (i = 0; i < indent; i++) s->Append("\t");
		s->Append("-/");
		print_node(s, a2, f, 0);
		s->Append("/->");
		print_node(s, n2, f, hex);
		if (r) {
			if (AGm::IsList(n2) && n2->T && AGm::IsList(n2->T) && n2->TH) {
				s->Append(" obj = ");
				s->WriteHexa((long)n2->TH);
			}
		}
		if (n2 == r) s->Append(" <==");
		s->Append("\n");
		if (f) {
			f->WriteString(s);
		} else {
			ADvWindow::Print(s->Str());
		}
		if (max_line > 0 && ++num_line > max_line) { delete s; return; }
		delete s;
		AGm::Push(stack, n2);
		dump_graph(indent + 1, depth - 1, n2, f, hex, r);
		AGm::Pop(stack);
	}
}
static void dump_graph_src(int indent, int depth, AGmNode *n, AGmFile *f, int hex) {
	if (depth <= 0) return;
	AGmLink *l;
	AGmNode *n2, *a2;
	for (l = AGm::next_inlink(n, NULL); l; l = AGm::next_inlink(n, l)) {
		n2 = l->arc->src; a2 = l->arc->attr;
		AGmNode *ls = stack;
		for (ls = stack; ls; ls = ls->T) {
			if (ls->H == n2) return;
		}
		int i; AGmString* s = new AGmString(64);
		for (i = 0; i < indent; i++) s->Append("\t");
		s->Append("<-/");
		print_node(s, a2, f, 0);
		s->Append("/-");
		print_node(s, n2, f, hex);
		s->Append("\n");
		if (f) {
			f->WriteString(s);
		} else {
			ADvWindow::Print(s->Str());
		}
		if (max_line > 0 && ++num_line > max_line) { delete s; return; }
		delete s;
		AGm::Push(stack, n2);
		dump_graph_src(indent + 1, depth - 1, n2, f, hex);
		AGm::Pop(stack);
	}
}

#if defined(_DEBUG) || defined(_LINUX_DEBUG)
void gc_test(char *str) {
	TRACE1(AL_STR("-------- %s --------\n"), str);
	char msg[64];
#if	_WINDOWS && _64BIT
	long long num_new_free, num_free, num_use;
#else
	long num_new_free, num_free, num_use;
#endif
	AGm::GC(&num_new_free, &num_free, &num_use);
	art_sprintf_1(msg, 64, "number of collected cells = %ld\n", num_new_free);
	TRACE0(msg);
	art_sprintf_1(msg, 64, "number of free cells = %ld\n", num_free);
	TRACE0(msg);
	art_sprintf_1(msg, 64, "number of cells in use = %ld\n", num_use);
	TRACE0(msg);
}

typedef struct _CellInfo {
	long type;
	void* ptr;
	struct _CellInfo* next;
} CellInfo;

static CellInfo *next = NULL;

void DebugAlloc(long type, void* ptr) {
	CellInfo *info = new CellInfo;
	info->type = type;
	info->ptr = ptr;
	info->next = next;
	next = info;
}

void DebugFree(void* ptr) {
	CellInfo **addr = &next;
	CellInfo *info = next;
	for (;;) {
		if (info == NULL) {
			TRACE0(AL_STR("can't find cell which should be freed.\n"));
			return;
		}
		if (info->ptr == ptr) break;
		addr = &info->next;
		info = info->next;
	}
	*addr = info->next;
	delete info;
}

void DebugLeakCells() {
	if (next == NULL) { TRACE0(AL_STR("No Leak\n")); return; }
	TRACE0(AL_STR("leek cells are the following\n"));
	CellInfo *info;
	for (info = next; info; info = info->next) {
		DumpKind((AGmNode*)info->ptr);
	}
}

void DumpKind(AGmNode *n, char* s, int size) {
	AGmString *ss; AGmFile *f;
	switch (n->type) {
	case AGM_NODE_INTEGER:
		art_sprintf_1(s, size, "Integer %ld\n", n->a.i.v);
		break;
	case AGM_NODE_REAL:
		art_sprintf_1(s, size, "Real %g\n", n->a.r.v);
		break;
	case AGM_NODE_LIST:
		ss = new AGmString("");
		f = new AGmFile();
		f->Create(ss, AGM_FILE_STR_WRITE);
		f->WriteHexa((long)n);
		f->WriteString(" ");
		DumpList("List: ", n, 10, f, 0);
		art_sprintf_0(s, size, ss->Str());
		delete f; delete ss;
		break;
	case AGM_NODE_ARRAY:
		art_sprintf_0(s, size, "Array\n");
		break;
	case AGM_NODE_OBJECT:
		art_sprintf_0(s, size, "Object\n");
		break;
	case AGM_NODE_DST_ITR:
		art_sprintf_0(s, size, "DstItr\n");
		break;
	case AGM_NODE_SRC_ITR:
		art_sprintf_0(s, size, "SrcItr\n");
		break;
	case AGM_NODE_STRING:
		art_sprintf_1(s, size, "String %s\n", n->a.s.v->Str());
		break;
	case AGM_NODE_MATRIX:
		art_sprintf_0(s, size, "Matrix\n");
		break;
	case AGM_NODE_FILE:
		art_sprintf_0(s, size, "File\n");
		break;
	case AGM_NODE_WINDOW:
		art_sprintf_0(s, size, "Window\n");
		break;
	case AGM_NODE_BITMAP:
		art_sprintf_0(s, size, "Bitmap\n");
		break;
	case AGM_NODE_PTR:
		art_sprintf_0(s, size, "Pointer\n");
		break;
	case AGM_NODE_OCX:
		art_sprintf_0(s, size, "OCX\n");
		break;
	case AGM_NODE_VARIANT:
		art_sprintf_0(s, size, "Variant\n");
		break;
	case AGM_NODE_SQL_CONN:
		art_sprintf_0(s, size, "RDBConnection\n");
		break;
	case AGM_NODE_SQL_STAT:
		art_sprintf_0(s, size, "RDBStatement\n");
		break;
	case AGM_BODY_OF_NODE:
		art_sprintf_0(s, size, "NodeBody\n");
		break;
	case AGM_DMY_OUT_LINK:
		art_sprintf_0(s, size, "DummyOutLink\n");
		break;
	case AGM_DMY_IN_LINK:
		art_sprintf_0(s, size, "DummyInLink\n");
		break;
	case AGM_OUT_LINK:
		art_sprintf_0(s, size, "OutLink\n");
		break;
	case AGM_IN_LINK:
		art_sprintf_0(s, size, "InLink\n");
		break;
	case AGM_ARC:
		art_sprintf_0(s, size, "Arc\n");
		break;
	default:
		art_sprintf_0(s, size, "Unknown\n");
		break;
	}
}
void DumpKind(AGmNode *n) {
	char s[4096];
	DumpKind(n, s, 4096);
	TRACE0(s);
}
void DumpKindOut(AGmNode *n) {
	char s[4096];
	DumpKind(n, s, 4096);
	ADvWindow::Print(s);
}
void CheckOutLink(const char* s, AGmNode* src) {
	if (s) ADvWindow::Print(s);
	char msg[256];
	art_sprintf_1(msg, 256, "src = %lx,", AL_PTR(src)); ADvWindow::Print(msg);
	DumpList("### src = ", src);
	if (src->body == NULL) {
		art_sprintf_0(msg, 256, "### node_body = NULL\n");
		ADvWindow::Print(msg); return;
	}
	if (src->body->out == NULL) {
		art_sprintf_0(msg, 256, "### dummy_outlink = NULL\n");
		ADvWindow::Print(msg); return;
	}
	AGmLink *l; int i;
	for (l = AGm::next_outlink(src, NULL), i = 1; l && i < 10; l = AGm::next_outlink(src, l), i++) {
		art_sprintf_4(msg, 256, "### outlink = %lx, arc = %lx, attr = %lx, dst = %lx\n",
			AL_PTR(l), AL_PTR(l->arc), AL_PTR(l->arc->attr), AL_PTR(l->arc->dst));
		if (l == l->next->prev) {
			ADvWindow::Print("### ->next->prev OK\n");
		} else {
			ADvWindow::Print("### ->next->prev NG\n");
		}
		if (l == l->prev->next) {
			ADvWindow::Print("### ->prev->next OK\n");
		} else {
			ADvWindow::Print("### ->prev->next NG\n");
		}
		ADvWindow::Print(msg);
		DumpList("### attr = ", l->arc->attr);
		DumpList("### dst = ", l->arc->dst);
	}
	ADvWindow::Print("### end of CheckOutLink\n");
}
void CheckInLink(const char* s, AGmNode* dst) {
	if (s) ADvWindow::Print(s);
	char msg[256];
	art_sprintf_1(msg, 256, "### dst = %lx,", AL_PTR(dst)); ADvWindow::Print(msg);
	DumpList("dst = ", dst);
	if (dst->body == NULL) {
		art_sprintf_0(msg, 256, "### node_body = NULL\n");
		ADvWindow::Print(msg); return;
	}
	if (dst->body->in == NULL) {
		art_sprintf_0(msg, 256, "### dummy_outlink = NULL\n");
		ADvWindow::Print(msg); return;
	}
	AGmLink *l; int i;
	for (l = AGm::next_inlink(dst, NULL), i = 1; l && i < 10; l = AGm::next_inlink(dst, l), i++) {
		art_sprintf_4(msg, 256, "### inlink = %lx, arc = %lx, attr = %lx, src = %lx\n",
			AL_PTR(l), AL_PTR(l->arc), AL_PTR(l->arc->attr), AL_PTR(l->arc->src));
		if (l == l->next->prev) {
			ADvWindow::Print("### ->next->prev OK\n");
		} else {
			ADvWindow::Print("### ->next->prev NG\n");
		}
		if (l == l->prev->next) {
			ADvWindow::Print("### ->prev->next OK\n");
		} else {
			ADvWindow::Print("### ->prev->next NG\n");
		}
		ADvWindow::Print(msg);
		DumpList("### attr = ", l->arc->attr);
		DumpList("### src = ", l->arc->src);
	}
	ADvWindow::Print("### end of CheckOutLink\n");
}
#endif
