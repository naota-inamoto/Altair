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
#include <gdm/big_int.h>

#include <symbol/parser_generator.h>
#include <symbol/pro_lisp.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif
//#define DEBUG_COMMENT_SPACE
//#define DEBUG_IMPROVE_ERR_MSG

static int indent_count = 0;
static void indent() {
	char s[256];
	art_sprintf_1(s, 256, "%3.d", indent_count);
	AGmString *ss = new AGmString(s);
	for (int i = 0; i < indent_count; i++) ss->Append(' ');
	ADvWindow::Print(ss->Str());
	delete ss;
}

#define IF_DEBUG_10 if (IfDebug10(f, unit_name))
#define IF_DEBUG_20 if (IfDebug20(f, unit_name))
#define IF_DEBUG_30 if (IfDebug30(f, unit_name))

#define ZENKAKU 0x10000

int AGmParserGenerator::parse_cut_depth = 0;

AGmParserGenerator::AGmParserGenerator() {
	SetConst();
}
AGmParserGenerator::AGmParserGenerator(AGmNode* l) {
	SetConst();
	if (l && IsList(l->H) && l->T && IsList(l->TH) && l->TT && IsList(l->TTH)
			&& l->TTT && IsList(l->TTTH) && l->TTTT && IsList(l->TTTTH)
			&& l->TTTTT && IsList(l->TTTTTH) && l->TTTTT->T && IsList(l->TTTTT->TH)) {
		syntax = Ref(l->H);
		ids = Ref(l->TH);
		comment = Ref(l->TTH);
		ids_cs = Ref(l->TTTH);
		reserve = Ref(l->TTTTH);
		gen_format = Ref(l->TTTTTH);
		extension = Ref(l->TTTTT->TH);
	} else {
		error = Ref(String("compiled syntax is illegal."));
	}
}
void AGmParserGenerator::SetConst() {
	syntax = NULL; ids = NULL;
	comment = NULL; ids_cs = NULL;
	reserve = NULL; gen_format = NULL;
	extension = NULL;

	error = NULL;
	def_stack = NULL;
	debug_compile = 0;
	debug_unit_10 = debug_unit_20 = debug_unit_30 = NULL;
	parse_full = 0;
	indent_stack = NULL;
	space_flag = 0;
	space_on = 1;
	nonewline = 0;

	str_const = Ref(UniqueStr("const"));
	str_no_sp_const = Ref(UniqueStr("no_sp_const"));
	str_not_const = Ref(UniqueStr("not_const"));
	str_no_sp_not_const = Ref(UniqueStr("no_sp_not_const"));
	str_not_nl = Ref(UniqueStr("not_nl"));
	str_reg_ref = Ref(UniqueStr("$reg_ref"));
	str_until = Ref(UniqueStr("$until"));
	str_redirect = Ref(UniqueStr("$redirect"));
	str_line = Ref(UniqueStr("$line"));
	str_regex = Ref(UniqueStr("regex"));
	str_define = Ref(UniqueStr("$define"));
	str_undef = Ref(UniqueStr("$undef"));
	str_ifdef = Ref(UniqueStr("$ifdef"));
	str_ifndef = Ref(UniqueStr("$ifndef"));
	str_nonewline = Ref(UniqueStr("$nonewline"));
	str_repeat_0 = Ref(UniqueStr("{}*"));
	str_repeat_1 = Ref(UniqueStr("{}+"));
	str_braket_0 = Ref(UniqueStr("{}?"));
	str_braket_1 = Ref(UniqueStr("{}"));
	str_eof = Ref(UniqueStr("EOF"));
	str_integer = Ref(UniqueStr("integer"));
	str_hex = Ref(UniqueStr("hex"));
	str_char = Ref(UniqueStr("char"));
	str_real = Ref(UniqueStr("real"));
	str_string = Ref(UniqueStr("string"));
	str_quote_string = Ref(UniqueStr("quote_string"));
	str_single_quote_string = Ref(UniqueStr("single_quote_string"));
	str_special = Ref(UniqueStr("special"));
	str_extension = Ref(UniqueStr("extension"));

	str_value = Ref(UniqueStr("$value"));
}
AGmParserGenerator::~AGmParserGenerator() {
	Unref(syntax, ids);
	Unref(comment, ids_cs);
	Unref(reserve, gen_format);
	Unref(extension);

	Unref(error);
	Unref(def_stack);
	Unref(debug_unit_10, debug_unit_20, debug_unit_30);
	Unref(indent_stack);

	Unref(str_const);
	Unref(str_no_sp_const);
	Unref(str_not_const);
	Unref(str_no_sp_not_const);
	Unref(str_not_nl);
	Unref(str_reg_ref);
	Unref(str_until);
	Unref(str_redirect);
	Unref(str_line);
	Unref(str_regex);
	Unref(str_define);
	Unref(str_undef);
	Unref(str_ifdef);
	Unref(str_ifndef);
	Unref(str_nonewline);
	Unref(str_repeat_0);
	Unref(str_repeat_1);
	Unref(str_braket_0);
	Unref(str_braket_1);
	Unref(str_eof);
	Unref(str_integer);
	Unref(str_hex);
	Unref(str_char);
	Unref(str_real);
	Unref(str_string);
	Unref(str_quote_string);
	Unref(str_single_quote_string);
	Unref(str_special);
	Unref(str_extension);
	Unref(str_value);
}

void AGmParserGenerator::DebugCompile(int debug) {
	debug_compile = debug;
}
void AGmParserGenerator::DebugUnit10(AGmNode* debug_unit) {
	Set(debug_unit_10, debug_unit);
}
void AGmParserGenerator::DebugUnit20(AGmNode* debug_unit) {
	Set(debug_unit_20, debug_unit);
}
void AGmParserGenerator::DebugUnit30(AGmNode* debug_unit) {
	Set(debug_unit_30, debug_unit);
}
int AGmParserGenerator::IfDebug10(AGmFile* f, const char* unit_name) {
	if (debug_unit_10 == NULL)
		return 0;
	if (IsInteger(debug_unit_10) && debug_unit_10->a.i.v > f->LineNumber())
		return 0;
	if (IsList(debug_unit_10) && debug_unit_10->T
			&& IsInteger(debug_unit_10->H) && IsInteger(debug_unit_10->TH)) {
		if (debug_unit_10->H->a.i.v > f->LineNumber())
			return 0;
		if (debug_unit_10->TH->a.i.v <= f->LineNumber())
			return 1;
	}
	if (dst_node(debug_unit_10) && dst_node(debug_unit_10, unit_name) == NULL)
		return 0;
	return 1;
}
int AGmParserGenerator::IfDebug20(AGmFile* f, const char* unit_name) {
	if (debug_unit_20 == NULL)
		return 0;
	if (IsInteger(debug_unit_20) && debug_unit_20->a.i.v > f->LineNumber())
		return 0;
	if (IsList(debug_unit_20) && debug_unit_20->T
			&& IsInteger(debug_unit_20->H) && IsInteger(debug_unit_20->TH)) {
		if (debug_unit_20->H->a.i.v > f->LineNumber())
			return 0;
		if (debug_unit_20->TH->a.i.v <= f->LineNumber())
			return 1;
	}
	if (dst_node(debug_unit_20) && dst_node(debug_unit_20, unit_name) == NULL)
		return 0;
	return 1;
}
int AGmParserGenerator::IfDebug30(AGmFile* f, const char* unit_name) {
	if (debug_unit_30 == NULL)
		return 0;
	if (IsInteger(debug_unit_30) && debug_unit_30->a.i.v > f->LineNumber())
		return 0;
	if (IsList(debug_unit_30) && debug_unit_30->T
			&& IsInteger(debug_unit_30->H) && IsInteger(debug_unit_30->TH)) {
		if (debug_unit_30->H->a.i.v > f->LineNumber())
			return 0;
		if (debug_unit_30->TH->a.i.v <= f->LineNumber())
			return 1;
	}
	if (dst_node(debug_unit_30) && dst_node(debug_unit_30, unit_name) == NULL)
		return 0;
	return 1;
}
void AGmParserGenerator::ParseFull(int parse_full) {
	this->parse_full = parse_full;
}

void AGmParserGenerator::ParseCutDepth(int depth) {
	parse_cut_depth = depth;
}
int AGmParserGenerator::ParseCutDepth() {
	return parse_cut_depth;
}

AGmNode* AGmParserGenerator::Compile(AGmFile* f) {

	Unref(syntax); syntax = Ref(Cons(NULL));
	Unref(ids); ids = Ref(Cons(NULL));
	Unref(comment); comment = Ref(Cons(NULL));
	Unref(ids_cs); ids_cs = Ref(Cons(NULL));
	Unref(reserve); reserve = Ref(Cons(NULL));
	Unref(gen_format); gen_format = Ref(Cons(NULL));
	Unref(extension); extension = Ref(Cons(NULL));

#if	_WINDOWS && _64BIT
	long long lno = 0;
#else
	long lno = 0;
#endif
	AGmNode* errRes = NULL;
	char msg[512];
	AGmNode* err = gen_format_scanf(f, lno, errRes);
	if (err) {
		art_sprintf_2(msg, 512, "compile_syntax: parse error, line(%ld): %s", lno, err->a.s.v->Str());
		RefUnref(err); return String(msg);
	}
	err = def_scanf(f, lno, errRes);
	if (err) {
		art_sprintf_2(msg, 512, "compile_syntax: parse error, line(%ld): %s", lno, err->a.s.v->Str());
		RefUnref(err); return String(msg);
	}

	AGmLink *l;
	for (l = next_outlink(syntax, NULL); l; l = next_outlink(syntax, l)) {
		AGmNode *n = l->arc->dst;
		if (n->H == NULL) {
			art_sprintf_1(msg, 512, "compile_syntax: definition '%s' not found.", l->arc->attr->a.s.v->Str());
			return String(msg);
		}
	}

	for (l = next_outlink(gen_format, NULL); l; l = next_outlink(gen_format, l)) {
		AGmNode *a = l->arc->attr;
		if (a && IsList(a) && IsInteger(a->H) && a->HI == 1 && a->T && IsString(a->TH) && dst_node(ids, a->TH) == NULL) {
			art_sprintf_1(msg, 512, "compile_syntax: definition '%s' not found.", a->TH->a.s.v->Str());
			return String(msg);
		}
	}

	AGmNode *n = List(syntax, ids, comment, ids_cs, reserve, gen_format, extension);
	MakeStrUnique(n);
	return n;
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::gen_format_scanf(AGmFile* f, long long& lno, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::gen_format_scanf(AGmFile* f, long& lno, AGmNode*& errRes) {
#endif
	for (;;) {
		for (;;) {
			AGmString *ss = f->ReadComment();
			if (ss == NULL) break;
			delete ss;
		}
		int cmd = 0; AGmNode *c_or_id = NULL, *str = NULL;
#if	_WINDOWS && _64BIT
		long long n;
#else
		long n;
#endif
		if (f->MatchIdent("pre")) {
			if (f->MatchIdent("newline")) {
				cmd = 10;
				if (f->MatchIdent("indent")) {
					cmd = 12;
				}
			} else if (f->MatchIdent("indent")) {
				cmd = 11;
			} else if (f->MatchIdent("space")) {
				cmd = 15;
			} else {
				AGmNode* errMsg = String("'newline' or 'indent' required after 'pre'.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
		} else if (f->MatchIdent("post")) {
			if (f->MatchIdent("newline")) {
				cmd = 20;
				if (f->MatchIdent("indent")) {
					cmd = 22;
				}
			} else if (f->MatchIdent("indent")) {
				cmd = 21;
			} else if (f->MatchIdent("space")) {
				cmd = 25;
			} else {
				AGmNode* errMsg = String("'newline' or 'indent' required after 'post'.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
		} else if (f->MatchString("+indent")) {
			cmd = 31;
			if (f->ReadQString(str)) {
				AGmNode* errMsg = String("<quote-string> required after 'indent+'.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
		} else if (f->MatchString("-indent")) {
			cmd = 32;
		} else if (f->MatchString("indent+")) {
			cmd = 41;
			if (f->ReadQString(str)) {
				AGmNode* errMsg = String("<quote-string> required after 'indent+'.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
		} else if (f->MatchString("indent-")) {
			cmd = 42;
		} else if (f->MatchIdent("newline_is_token")) {
			cmd = 100;
		} else if (f->MatchIdent("split_ident")) {
			cmd = 200;
		} else {
			break;
		}
		if (cmd < 100) {
			if (f->ReadQString(c_or_id) == 0) {
				c_or_id = List(Integer(0), c_or_id);
			} else {
				if (!f->MatchString("<") || f->ReadIdent(c_or_id)) {
					AGmNode* errMsg = String("quote-string or <syntax_name:n> required.");
					set_lno(f, lno, errMsg, errRes);
					return errMsg;
				}
				if (!f->MatchString(":") || f->ReadInteger(n) || !f->MatchString(">")) {
					RefUnref(c_or_id);
					AGmNode* errMsg = String("'>' expected.");
					set_lno(f, lno, errMsg, errRes);
					return errMsg;
				}
				c_or_id = List(Integer(1), c_or_id, Integer(n));
			}
		}
		if (!f->MatchString(";")) {
			RefUnref(c_or_id);
			AGmNode* errMsg = String("';' expected.");
			set_lno(f, lno, errMsg, errRes);
			return errMsg;
		}
		if (cmd < 100) {
			AGmNode *dst = List(Integer(cmd), str);
			AGmLink *l;
			for (l = next_outlink(gen_format, NULL); l; l = next_outlink(gen_format, l)) {
				if (Compare(l->arc->attr, c_or_id) || Compare(l->arc->dst, dst)) continue;
				RefUnref(dst, c_or_id); break;
			}
			if (l == NULL) CreateArc(gen_format, dst, c_or_id);
		} else {
			CreateArc(gen_format, Cons(NULL), Integer(cmd));
		}
	}
	return NULL;
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::def_scanf(AGmFile* f, long long& lno, AGmNode*& errRes, AGmNode *cs_name) {
#else
AGmNode* AGmParserGenerator::def_scanf(AGmFile* f, long& lno, AGmNode*& errRes, AGmNode *cs_name) {
#endif
	char msg[256];
	for (;;) {
		for (;;) {
			AGmString *ss = f->ReadComment();
			if (ss == NULL) break;
			delete ss;
		}
		while (f->MatchIdent("include")) {
			AGmString *dir, *name;
			if (f->ReadQString(name)) {
				AGmNode* errMsg = String("no filename at include");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			dir = f->GetDir();
			if (dir == NULL) {
				AGmNode* errMsg = String("no directory info in file-object");
				set_lno(f, lno, errMsg, errRes);
				delete name;
				return errMsg;
			}
			AGmString *path = dir;
			path->Append("/");
			path->Append(name);
			AGmFile *f2 = new AGmFile();
			if (f2->Create(path, AGM_FILE_READ)) {
				path->Append(" can't open.");
				delete f2; delete name;
				AGmNode* errMsg = String(path);
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			AGmNode *comm_sp_name = Ref(UniqueStr(name));
			delete path; delete name;
#if	_WINDOWS && _64BIT
			long long lno2 = 0;
#else
			long lno2 = 0;
#endif
			AGmNode* errRes = NULL;
			AGmNode* err = gen_format_scanf(f2, lno2, errRes);
			if (err) {
				Unref(comm_sp_name); delete f2;
				return err;
			}
			err = def_scanf(f2, lno2, comm_sp_name);
			if (err) {
				Unref(comm_sp_name); delete f2;
				return err;
			}
			Unref(comm_sp_name); delete f2;
		}
		if (!f->MatchString("<")) {
			if (f->IsEnd()) return NULL;
			AGmNode* errMsg = String("EOF expected.");
			set_lno(f, lno, errMsg, errRes);
			return errMsg;
		}
		AGmString *def_name;
		if (f->ReadIdent(def_name)) {
			AGmNode* errMsg = String("definition name not found after '<'.");
			set_lno(f, lno, errMsg, errRes);
			return errMsg;
		}
		if (!f->MatchString(">") || !f->MatchString("=")) {
			art_sprintf_1(msg, 256, "'>' or '=' not found after definition '%s'", def_name->Str());
			delete def_name;
			AGmNode* errMsg = String(msg);
			set_lno(f, lno, errMsg, errRes);
			return errMsg;
		}
if (debug_compile) {
art_sprintf_1(msg, 256, "======================== <%s> ========================\n", def_name->Str()); 
ADvWindow::Print(msg);
}
		AGmNode *nn = dst_node(syntax, def_name->Str());
		if (nn && nn->H) {
			art_sprintf_1(msg, 256, "multiple definition '%s'", def_name->Str());
			delete def_name;
			AGmNode* errMsg = String(msg);
			set_lno(f, lno, errMsg, errRes);
			return errMsg;
		}
			
		if (f->MatchIdent("regex")) {
			f->ReadSpace();
			AGmNode *reg = reg_scanf(f, lno, errRes);
			if (IsString(reg)) { delete def_name; return reg; }
			AGmString *type;
			if (f->ReadIdent(type)) {
				RefUnref(reg); delete def_name;
				AGmNode* errMsg = String("regex: type string missing.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			const char *t = type->Str();
			if (strcmp(t, "integer") == 0) {
			} else if (strcmp(t, "hex") == 0) {
			} else if (strcmp(t, "char") == 0) {
			} else if (strcmp(t, "real") == 0) {
			} else if (strcmp(t, "string") == 0) {
			} else if (strcmp(t, "quote_string") == 0) {
			} else if (strcmp(t, "single_quote_string") == 0) {
			} else if (strcmp(t, "special") == 0) {
			} else {
				art_sprintf_1(msg, 256, "unknown type '%s'", t);
				RefUnref(reg); delete type; delete def_name;
				AGmNode* errMsg = String(msg);
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			if (!f->MatchString(";")) {
				RefUnref(reg); delete type; delete def_name;
				AGmNode* errMsg = String("';' expected.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			AGmNode *regex = List(str_regex, reg, String(type), String(def_name->Str()));
			AGmLink *l;
			for (l = next_outlink(syntax, NULL); l; l = next_outlink(syntax, l)) {
				if (l->arc->attr->a.s.v->Compare(def_name) == 0) {
					AGmNode *n = l->arc->dst;
					Set(n->H, regex);
					l = l->prev; DeleteArc(l->next->arc);
				}
			}
			CreateArc(syntax, regex, String(def_name));
		} else if (f->MatchIdent("comment")) {
			f->ReadSpace();
			AGmNode *reg = reg_scanf(f, lno, errRes);
			if (IsString(reg)) { delete def_name; return reg; }
			if (!f->MatchString(";")) {
				RefUnref(reg); delete def_name;
				AGmNode* errMsg = String("';' expected.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			Push(comment->H, List(cs_name, Integer(0), reg, String(def_name)));
		} else if (f->MatchIdent("comment_begin_end")) {
			AGmString *b = NULL, *e = NULL;
			if (f->ReadQString(b)) {
				delete def_name;
				AGmNode* errMsg = String("comment begin string expected.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			if (f->ReadQString(e)) {
				delete b; delete def_name;
				AGmNode* errMsg = String("comment end string expected.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			if (!f->MatchString(";")) {
				delete b; delete e; delete def_name;
				AGmNode* errMsg = String("';' expected.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			Push(comment->H, List(cs_name, Integer(1), String(b), String(e), String(def_name)));
		} else if (f->MatchIdent("comment_until_newline")) {
			AGmString *b = NULL;
			if (f->ReadQString(b)) {
				delete def_name;
				AGmNode* errMsg = String("comment begin string expected.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			if (!f->MatchString(";")) {
				delete b; delete def_name;
				AGmNode* errMsg = String("';' expected.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			Push(comment->H, List(cs_name, Integer(2), String(b), String(def_name)));
		} else if (f->MatchIdent("reserve")) {
			AGmNode *n = String(def_name);
			for (;;) {
				if (f->MatchString(";")) break;
				AGmString *word = NULL;
				if (f->ReadString(word)) {
					RefUnref(n);
					AGmNode* errMsg = String("reserve part illegal.");
					set_lno(f, lno, errMsg, errRes);
					return errMsg;
				}
				CreateArc(reserve, n, String(word));
			}
		} else {
			AGmNode *para = para_scanf(f, lno, errRes, cs_name);
			if (IsString(para)) { delete def_name; return para; }
			if (!f->MatchString(";")) {
				art_sprintf_1(msg, 256, "';' missing definition '%s'", def_name->Str());
				delete def_name; RefUnref(para); return String(msg);
			}
			AGmNode *unit = para;
			if (Length(para) == 1) {
				unit = para->H; unit->count--;
				para->H = NULL; RefUnref(para);
			} else {
				unit = Cons(str_braket_1, para);
			}
			AGmLink *l;
			for (l = next_outlink(syntax, NULL); l; l = next_outlink(syntax, l)) {
				if (l->arc->attr->a.s.v->Compare(def_name) == 0) {
					AGmNode *n = l->arc->dst;
					Set(n->H, unit);
					l = l->prev; DeleteArc(l->next->arc);
				}
			}
			CreateArc(syntax, unit, String(def_name));
		}
	}
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::para_scanf(AGmFile* f, long long& lno, AGmNode*& errRes, AGmNode* cs_name) {
#else
AGmNode* AGmParserGenerator::para_scanf(AGmFile* f, long& lno, AGmNode*& errRes, AGmNode* cs_name) {
#endif
	AGmNode *unit1 = unit_scanf(f, lno, errRes, cs_name);
	if (IsString(unit1)) return unit1;
	if (unit1 == NULL) return NULL;
	unit1 = Cons(unit1);
	if (!f->MatchString("|")) {
		return unit1;
	}
	AGmNode* addr = unit1;
	for (;;) {
		AGmNode *unit2 = unit_scanf(f, lno, errRes, cs_name);
		if (IsString(unit2)) { RefUnref(unit1); return unit2; }
		if (unit2 == NULL) { RefUnref(unit1); return String("syntax unit required after '|'."); }
		Set(addr->T, Cons(unit2)); addr = addr->T;
		if (!f->MatchString("|")) break;
	}
	return unit1;
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::braket_scanf(AGmFile* f, long long& lno, AGmNode*& errRes, AGmNode* cs_name) {
#else
AGmNode* AGmParserGenerator::braket_scanf(AGmFile* f, long& lno, AGmNode*& errRes, AGmNode* cs_name) {
#endif
	if (!f->MatchString("{")) return NULL;
	AGmNode *para = para_scanf(f, lno, errRes, cs_name);
	if (para == NULL) return NULL;
	if (IsString(para)) return para;
	if (f->MatchString("}*")) {
		return Cons(str_repeat_0, para);
	} else if (f->MatchString("}+")) {
		return Cons(str_repeat_1, para);
	} else if (f->MatchString("}?")) {
		return Cons(str_braket_0, para);
	} else if (f->MatchString("}")) {
		return Cons(str_braket_1, para);
	} else {
		RefUnref(para);
		AGmNode* errMsg = String("'}' expected.");
		set_lno(f, lno, errMsg, errRes);
		return errMsg;
	}
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::unit_scanf(AGmFile* f, long long& lno, AGmNode*& errRes, AGmNode* cs_name) {
#else
AGmNode* AGmParserGenerator::unit_scanf(AGmFile* f, long& lno, AGmNode*& errRes, AGmNode* cs_name) {
#endif
	ADvDoEvents(); if (ADvExiting) return String("abort request from user.");
	char msg[256];
	AGmNode *id, *id0; int count = 0;
	if (f->ReadIdent(id0)) {
		AGmNode* errMsg = String("id-string required.");
		set_lno(f, lno, errMsg, errRes);
		return errMsg;
	}
	id = UniqueStr(id0);

	if (id != str_extension && dst_node2(ids, id)) {
		art_sprintf_1(msg, 256, "list-string for id '%s' is not unique.", id->a.s.v->Str());
		RefUnref(id); if (id != id0) RefUnref(id0);
		AGmNode* errMsg = String(msg);
		set_lno(f, lno, errMsg, errRes);
		return errMsg;
	}

	AGmNode *unit = Cons(id); RefUnref(id0);
	if (id != str_extension) {
		CreateArc(ids, unit, id);
		CreateArc(ids_cs, (cs_name ? cs_name : UniqueStr("$base")), id);
	}

	AGmNode* addr = unit;
	for (;;) {
		AGmNode* unit2;
		AGmString *str;
#if	_WINDOWS && _64BIT
		long long pos = f->CurrentPoint();
#else
		long pos = f->CurrentPoint();
#endif
		long ignore_case = f->MatchIdent("i");
		if (f->ReadQString(str) == 0) {
			unit2 = List(str_const, String(str), Integer(ignore_case));
			Set(addr->T, Cons(unit2)); addr = addr->T; count++; continue;
		}
		if (f->ReadSingleQString(str) == 0) {
			unit2 = List(str_no_sp_const, String(str), Integer(ignore_case));
			Set(addr->T, Cons(unit2)); addr = addr->T; count++; continue;
		}
		if (f->MatchString("!") && f->ReadQString(str) == 0) {
			unit2 = List(str_not_const, String(str), Integer(ignore_case));
			Set(addr->T, Cons(unit2)); addr = addr->T; continue;
		}
		f->Back(pos);
		ignore_case = f->MatchIdent("i");
		if (f->MatchString("!") && f->ReadSingleQString(str) == 0) {
			unit2 = List(str_no_sp_not_const, String(str), Integer(ignore_case));
			Set(addr->T, Cons(unit2)); addr = addr->T; continue;
		}
		f->Back(pos);
		if (f->MatchString("!$")) {
			unit2 = List(str_not_nl);
			Set(addr->T, Cons(unit2)); addr = addr->T; continue;
		}
#if	_WINDOWS && _64BIT
		long long n;
#else
		long n;
#endif
		ignore_case = f->MatchIdent("i");
		if (f->MatchString("$") && f->ReadInteger(n) == 0) {
			AGmNode *ls = unit->T;
			for (int i = 1; i < n && ls; i++, ls = ls->T) ;
			if (ls) {
				unit2 = List(str_reg_ref, Integer(n), Integer(ignore_case));
				Set(addr->T, Cons(unit2)); addr = addr->T; count++; continue;
			}
		}
		f->Back(pos);
		if (f->MatchString("$until") && f->MatchString("$") && f->ReadQString(str) == 0) {
			unit2 = List(str_until, String(str));
			Set(addr->T, Cons(unit2)); addr = addr->T; count++; continue;
		}
		f->Back(pos);
		if (f->MatchString("$until") && f->MatchString("$") && f->ReadInteger(n) == 0) {
			AGmNode *ls = unit->T;
			for (int i = 1; i < n && ls; i++, ls = ls->T) ;
			if (ls) {
				unit2 = List(str_until, Integer(n));
				Set(addr->T, Cons(unit2)); addr = addr->T; count++; continue;
			}
		}
		f->Back(pos);
		if (f->MatchString("$redirect") && f->MatchString("$") && f->ReadInteger(n) == 0) {
			AGmNode *ls = unit->T;
			for (int i = 1; i < n && ls; i++, ls = ls->T) ;
			if (ls) {
				unit2 = List(str_redirect, Integer(n));
				Set(addr->T, Cons(unit2)); addr = addr->T; continue;
			}
		}
		f->Back(pos);
		if (f->MatchString("$line")) {
			unit2 = List(str_line);
			Set(addr->T, Cons(unit2)); addr = addr->T; count++; continue;
		}

		if (f->MatchString("$define") && f->MatchString("(")) {
			if (f->ReadIdent(str) == 0) {
				if (f->MatchString(")")) {
					unit2 = List(str_define, String(str));
					Set(addr->T, Cons(unit2)); addr = addr->T; continue;
				}
				delete str;
			}
		}
		f->Back(pos);
		if (f->MatchString("$undef") && f->MatchString("(")) {
			if (f->ReadIdent(str) == 0) {
				if (f->MatchString(")")) {
					unit2 = List(str_undef, String(str));
					Set(addr->T, Cons(unit2)); addr = addr->T; continue;
				}
				delete str;
			}
		}
		f->Back(pos);
		if (f->MatchString("$ifdef") && f->MatchString("(")) {
			if (f->ReadIdent(str) == 0) {
				if (f->MatchString(")")) {
					unit2 = List(str_ifdef, String(str));
					Set(addr->T, Cons(unit2)); addr = addr->T; continue;
				}
				delete str;
			}
		}
		f->Back(pos);
		if (f->MatchString("$ifndef") && f->MatchString("(")) {
			if (f->ReadIdent(str) == 0) {
				if (f->MatchString(")")) {
					unit2 = List(str_ifndef, String(str));
					Set(addr->T, Cons(unit2)); addr = addr->T; continue;
				}
				delete str;
			}
		}
		f->Back(pos);
		if (f->MatchString("$nonewline")) {
			unit2 = Cons(str_nonewline);
			Set(addr->T, Cons(unit2)); addr = addr->T; continue;
		}
		f->Back(pos);

		if (f->MatchString("<")) {
			if (f->ReadIdent(str)) {
				RefUnref(unit);
				AGmNode* errMsg = String("definition name not found.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			if (!f->MatchString(">")) {
				art_sprintf_1(msg, 256, "'>' expected after '%s'.", str->Str());
				RefUnref(unit); delete str;
				AGmNode* errMsg = String(msg);
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			if (id == str_extension) {
				CreateArc(extension, unit, String(str));
				count++; continue;
			}
			AGmNode *n = dst_node(syntax, str->Str());
			if (n == NULL || n->H == NULL) {
				CreateArc(syntax, unit2 = Cons(NULL), String(str));
				Set(addr->T, unit2);
			} else {
				unit2 = n; delete str;
				Set(addr->T, Cons(unit2));	
			}
			addr = addr->T; count++; continue;
		}
		if (f->MatchIdent("EOF")) {
			Set(addr->T, Cons(Cons(String("EOF")))); addr = addr->T; continue;
		}
		unit2 = braket_scanf(f, lno, errRes, cs_name);
		if (IsString(unit2)) { RefUnref(unit); return unit2; }
		if (unit2 != NULL) {
			Set(addr->T, Cons(unit2)); addr = addr->T; count++; continue;
		}
		if (count == 0) {
			RefUnref(unit);
			AGmNode* errMsg = String("syntax empty.");
			set_lno(f, lno, errMsg, errRes);
			return errMsg;
		}

		AGmNode *ex = dst_node(extension, id->a.s.v->Str());
		if (ex) {
			if (ex->T) {
				art_sprintf_1(msg, 256, "multiple extension definition '%s',", id->a.s.v->Str());
				RefUnref(unit);
				AGmNode* errMsg = String(msg);
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			Set(ex->T, unit);
		}

if (debug_compile) {
DumpList(unit, 3);
}
		return unit;
	}
}

#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::reg_scanf(AGmFile* f, long long& lno, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::reg_scanf(AGmFile* f, long& lno, AGmNode*& errRes) {
#endif
	int count = 0;
	AGmNode *reg = Cons(str_regex);
	AGmNode* addr = reg;
	for (;;) {
		AGmNode *unit2 = NULL;
		AGmNode *unit = reg_unit_scanf(f, lno, errRes);
		if (IsString(unit))  { RefUnref(reg); return unit; }
		if (unit == NULL) {
			if (count == 0) {
				RefUnref(reg);
				AGmNode* errMsg = String("empty regex.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			return reg;
		}
		if (f->_MatchString("*")) {
			unit2 = List(Integer('*'), unit);
		} else if (f->_MatchString("+")) {
			unit2 = List(Integer('+'), unit);
		} else if (f->_MatchString("?")) {
			unit2 = List(Integer('?'), unit);
		} else if (f->_MatchString("{")) {
#if	_WINDOWS && _64BIT
			long long n, m = 0;
#else
			long n, m = 0;
#endif
			if (f->_ReadInteger(n)) {
				RefUnref(reg, unit);
				AGmNode* errMsg = String("number expected after '{'.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			if (f->_MatchString(",")) {
				if (f->_ReadInteger(m)) m = -1;
			}
			if (!f->_MatchString("}")) {
				RefUnref(reg, unit);
				AGmNode* errMsg = String("'}' expected.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			unit2 = List(Integer('{'), unit, Integer(n), Integer(m));
		} else {
			unit2 = unit;
		}
		Set(addr->T, Cons(unit2)); addr = addr->T; count++;
	}
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::reg_unit_scanf(AGmFile* f, long long& lno, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::reg_unit_scanf(AGmFile* f, long& lno, AGmNode*& errRes) {
#endif
	AGmNode *unit;
	if (f->_MatchString("^")) {
		return Cons(Integer('^'));
	} else if (f->_MatchString("$")) {
		return Cons(Integer('$'));
	} else if (f->_MatchString(".")) {
		return Cons(Integer('.'));
	} else if (f->_MatchString("(")) {
		AGmNode *reg = reg_scanf(f, lno, errRes);
		if (IsString(reg)) return reg;
		unit = List(Integer('('), reg);
		AGmNode *addr = unit->T;
		while (f->_MatchString("|")) {
			AGmNode *reg2 = reg_scanf(f, lno, errRes);
			if (IsString(reg2)) { RefUnref(unit); return reg2; }
			Set(addr->T, Cons(reg2)); addr = addr->T;
		}
		if (!f->_MatchString(")")) { RefUnref(unit); return String("')' expected."); }
		return unit;
	} else if (f->_MatchString("[^")) {
		AGmNode *list = reg_char_list_scanf(f, lno, errRes);
		if (IsString(list)) return list;
		unit = List(Integer('!'), list);
		if (!f->_MatchString("]")) { RefUnref(unit); return String("']' expected."); }
		return unit;
	} else if (f->_MatchString("[")) {
		AGmNode *list = reg_char_list_scanf(f, lno, errRes);
		if (IsString(list)) return list;
		unit = List(Integer('['), list);
		if (!f->_MatchString("]")) { RefUnref(unit); return String("']' expected."); }
		return unit;
	} else {
		long c = reg_char_scanf(f, lno, errRes);
		if (c < 0) return NULL;
		return List(Integer(0), Integer(c));
	}
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::reg_char_list_scanf(AGmFile* f, long long& lno, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::reg_char_list_scanf(AGmFile* f, long& lno, AGmNode*& errRes) {
#endif
	int count = 0;
	AGmNode *list = Cons(Integer('l'));
	AGmNode *addr = list;
	for (;;) {
		long b = reg_char_scanf(f, lno, errRes, 0);
		if (b == -1) {
			if (count == 0) {
				RefUnref(list);
				AGmNode* errMsg = String("regex: char-list empty.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			f->Unput(); return list;
		}
		if (b < 0) {
			RefUnref(list);
			AGmNode* errMsg = String("regex: incomplete char-list.");
			set_lno(f, lno, errMsg, errRes);
			return errMsg;
		}
#if	_WINDOWS && _64BIT
		long long pos = f->CurrentPoint();
#else
		long pos = f->CurrentPoint();
#endif
		long e = reg_char_scanf(f, lno, errRes, 0);
		if (e == -1) {
			AGmNode *ch = Cons(Integer(b));
			Set(addr->T, Cons(ch)); addr = addr->T; count++;
			f->Unput(); return list;
		}
		if (e == -2) {
			e = reg_char_scanf(f, lno, errRes);
			if (e < 0) {
				RefUnref(list);
				AGmNode* errMsg = String("regex: incomplete char-list.");
				set_lno(f, lno, errMsg, errRes);
				return errMsg;
			}
			AGmNode *ch = List(Integer(b), Integer(e));
			Set(addr->T, Cons(ch)); addr = addr->T; count++; continue;
		}
		if (e < 0) {
			RefUnref(list);
			AGmNode* errMsg = String("regex: incomplete char-list.");
			set_lno(f, lno, errMsg, errRes);
			return errMsg;
		}
		f->Back(pos);
		AGmNode *ch = Cons(Integer(b));
		Set(addr->T, Cons(ch)); addr = addr->T; count++;
	}
}
#if	_WINDOWS && _64BIT
long AGmParserGenerator::reg_char_scanf(AGmFile* f, long long& lno, AGmNode*& errRes, int unput) {
#else
long AGmParserGenerator::reg_char_scanf(AGmFile* f, long& lno, AGmNode*& errRes, int unput) {
#endif
	long c = f->GetChar();
	if (c == EOF || c == ' ' || c == '\t' || c == '\r' || c == '\n') {
		AGmNode* errMsg = String("reg_char_scanf: unexpected EOF, space or newline");
		f->Unput(); set_lno(f, lno, errRes, error);
		RefUnref(errMsg);
		return -100;
	}

	if (c == ']') {
		if (unput) f->Unput();
		AGmNode* errMsg = String("reg_char_scanf: unexpected ']'");
		set_lno(f, lno, errMsg, errRes);
		RefUnref(errMsg);
		return -1;
	}
	if (c == '-') {
		if (unput) f->Unput();
		AGmNode* errMsg = String("reg_char_scanf: unexpected '-'");
		set_lno(f, lno, errMsg, errRes);
		RefUnref(errMsg);
		return -2;
	}

	if (c == '^' || c == '$' || c == '.'
			|| c == '*' || c == '+' || c == '?' || c == '|'
			|| c == '{' || c == '}' || c == '(' || c == ')' || c == '[') {
		f->Unput();
		AGmNode* errMsg = String("reg_char_scanf: unexpected 'reserve char'");
		set_lno(f, lno, errMsg, errRes);
		RefUnref(errMsg);
		return -100;
	}
	if (c == '\\') {
		c = f->GetChar();
		if (c == EOF) {
			f->Unput();
			AGmNode* errMsg = String("reg_char_scanf: unexpected EOF");
			set_lno(f, lno, errMsg, errRes);
			RefUnref(errMsg);
			return -100;
		}
		if (c == 't') c = '\t';
		else if (c == 'r') c = '\r';
		else if (c == 'n') c = '\n';
		else if (c == 'M') c = ZENKAKU;
	}
	return c;
}

AGmNode* AGmParserGenerator::Parse(AGmFile* f, const char *def) {
	if (error) return String(error->a.s.v->Str());
	AGmNode *curr_syntax = dst_node(syntax, def);
	char msg[512];
	if (curr_syntax == NULL) {
		art_sprintf_1(msg, 512, "parse: definition '%s' not found", def);
		return String(msg);
	}
#if	_WINDOWS && _64BIT
	long long lno = 0;
#else
	long lno = 0;
#endif
	AGmNode* errRes = NULL;
	indent_count = 0;
	if (dst_node(gen_format, 100)) f->NewlineToken(1);
	f->AutoExpandUnputBuffer(1);
	parse_depth = 0;
	AGmNode *tree = parse_scanf(f, curr_syntax, lno, errRes);
	f->AutoExpandUnputBuffer(0);
	if (dst_node(gen_format, 100)) f->NewlineToken(0);
	if (IsString(tree)) {
		art_sprintf_2(msg, 512, "parse error, line(%ld): %s", lno, tree->a.s.v->Str());
		return String(msg);
	}
	MakeStrUnique(tree);
	return tree;
}

#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::parse_scanf(AGmFile* f, AGmNode* curr_syntax, long long& lno, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::parse_scanf(AGmFile* f, AGmNode* curr_syntax, long& lno, AGmNode*& errRes) {
#endif
	AGmNode *s = curr_syntax->H;
#if	_WINDOWS && _64BIT
	long long pos2;
#else
	long pos2;
#endif
	if (s == str_braket_1) {
		return parse_para_scanf(f, curr_syntax, lno, pos2, errRes);
	} else {
		return parse_unit_scanf(f, curr_syntax, lno, pos2, errRes);
	}
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::parse_para_scanf(AGmFile* f, AGmNode* curr_syntax, long long& lno, long long& pos, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::parse_para_scanf(AGmFile* f, AGmNode* curr_syntax, long& lno, long& pos, AGmNode*& errRes) {
#endif
	parse_depth++;
	AGmNode *ls, *tree = NULL;
	for (ls = curr_syntax->T; ls; ls = ls->T) {
		tree = parse_unit_scanf(f, ls->H, lno, pos, errRes);
		if (IsString(tree)) {
			RefUnref(tree); continue;
		}
		if (parse_depth < parse_cut_depth) {
			f->Cut();
		}
		parse_depth--;
		return tree;
	}
	parse_depth--;
	return parse_error(f, lno, String("no match unit"), errRes);
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::parse_repeat0_scanf(AGmFile* f, AGmNode* curr_syntax, long long& lno, long long& pos, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::parse_repeat0_scanf(AGmFile* f, AGmNode* curr_syntax, long& lno, long& pos, AGmNode*& errRes) {
#endif
	AGmNode *tree = Cons(curr_syntax->H);
	AGmNode *addr = tree;
	int first;
	for (first = 1;; first = 0) {
#if	_WINDOWS && _64BIT
		long long pos2;
#else
		long pos2;
#endif
		AGmNode *tree2 = parse_para_scanf(f, curr_syntax, lno, pos2, errRes);
		if (first) pos = pos2;
		if (IsString(tree2)) {
			RefUnref(tree2); break;
		}
		if (tree2) {
			Set(addr->T, Cons(tree2)); addr = addr->T;
		}
	}
	return tree;
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::parse_repeat1_scanf(AGmFile* f, AGmNode* curr_syntax, long long& lno, long long& pos, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::parse_repeat1_scanf(AGmFile* f, AGmNode* curr_syntax, long& lno, long& pos, AGmNode*& errRes) {
#endif
	int count = 0;
	AGmNode *tree = Cons(curr_syntax->H);
	AGmNode *addr = tree;
	int first;
	for (first = 1;; first = 0) {
#if	_WINDOWS && _64BIT
		long long pos2;
#else
		long pos2;
#endif
		AGmNode *tree2 = parse_para_scanf(f, curr_syntax, lno, pos2, errRes);
		if (first) pos = pos2;
		if (IsString(tree2)) {
			RefUnref(tree2); break;
		}
		if (tree2) {
			Set(addr->T, Cons(tree2)); addr = addr->T;
		}
		count++;
	}
	if (count == 0) {
		RefUnref(tree);
		return parse_error(f, lno, String("missing elements at {}+"), errRes);
	}
	return tree;
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::parse_braket0_scanf(AGmFile* f, AGmNode* curr_syntax, long long& lno, long long& pos, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::parse_braket0_scanf(AGmFile* f, AGmNode* curr_syntax, long& lno, long& pos, AGmNode*& errRes) {
#endif
	AGmNode *tree = Cons(curr_syntax->H);
	AGmNode *addr = tree;
	AGmNode *tree2 = parse_para_scanf(f, curr_syntax, lno, pos, errRes);
	if (IsString(tree2)) {
		RefUnref(tree2);
	} else if (tree2) {
		Set(addr->T, Cons(tree2)); addr = addr->T;
	}
	return tree;
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::parse_unit_scanf(AGmFile* f, AGmNode* curr_syntax, long long& lno, long long& pos, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::parse_unit_scanf(AGmFile* f, AGmNode* curr_syntax, long& lno, long& pos, AGmNode*& errRes) {
#endif
	ADvDoEvents();
	if (ADvExiting) {
		return parse_error(f, lno, String("abort request from user."), errRes);
	}
	if (curr_syntax->H == str_extension) {
		curr_syntax = curr_syntax->T;
		if (curr_syntax == NULL) {
			return parse_error(f, lno, String("extension not defined."), errRes);
		}
	}
	const char *unit_name = curr_syntax->H->a.s.v->Str();
	Push(def_stack, NULL);
	long start_lno = f->LineNumber();
	char msg[256];
IF_DEBUG_30 {
::indent();
art_sprintf_2(msg, 256, "begin(%s), lno = %ld\n", unit_name, start_lno);
ADvWindow::Print(msg);
}
indent_count++;
	int const_regex_count = 0;
	int not_optional_count = 0;
	int optional_is_empty = 1;
	AGmNode *not_optional = NULL;
	AGmNode *tree = Cons(curr_syntax->H);
	AGmNode *ls, *addr = tree, *tree2 = NULL;
	int cont;
	if (f->NewlineToken()) {
		pos = f->CurrentPoint(); cont = 1;
	} else {
		parse_comment(f, curr_syntax->H, errRes);
		pos = f->CurrentPoint(); cont = 0;
	}
	int first;
	for (ls = curr_syntax->T, first = 1; ls; ls = ls->T, cont = 1, first = 0) {
		AGmNode* next_syntax = ls->H;
		AGmNode *s = next_syntax->H;
#if	_WINDOWS && _64BIT
		long long pos2;
#else
		long pos2;
#endif
		if (s == str_const) {
			AGmNode* errMsg = NULL;
			set_lno(f, lno, errMsg, errRes);
			const char* token = next_syntax->TH->a.s.v->Str();
			long ignore_case = (long)next_syntax->TTHI;
			unsigned char c = next_syntax->TH->a.s.v->LastChar();
			int ident = (isIdentTail(c) || isFirstChar(c));
			if (cont) parse_comment(f, curr_syntax->H, errRes);
			if ((ident && f->MatchIdent(token, ignore_case)) || (ident == 0 && f->MatchString(token, ignore_case))) {
IF_DEBUG_30 {
::indent();
art_sprintf_3(msg, 256, "token(OK) = %s, lno = %ld, fp = %ld\n", token, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
				eol = 0;
				const_regex_count++; continue;
			}
indent_count--;
IF_DEBUG_30 {
::indent();
art_sprintf_4(msg, 256, "failure(%s), token(NG) = %s, lno = %ld, fp = %ld\n", unit_name, token, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
			RefUnref(tree);
			art_sprintf_2(msg, 256, "missing '%s' at '%s'", next_syntax->TH->a.s.v->Str(), unit_name);
			f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String(msg), errRes);
		} else if (s == str_no_sp_const) {
			AGmNode* errMsg = NULL;
			set_lno(f, lno, errMsg, errRes);
			const char* token = next_syntax->TH->a.s.v->Str();
			long ignore_case = (long)next_syntax->TTHI;
			if (f->_MatchString(token, ignore_case)) {
IF_DEBUG_30 {
::indent();
art_sprintf_3(msg, 256, "no_sp_token(OK) = %s, lno = %ld, fp = %ld\n", token, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
				eol = 0;
				const_regex_count++; continue;
			}
indent_count--;
IF_DEBUG_30 {
::indent();
art_sprintf_4(msg, 256, "failure(%s), no_sp_token(NG) = %s, lno = %ld, fp = %ld\n", unit_name, token, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
			RefUnref(tree);
			art_sprintf_2(msg, 256, "missing '%s' at '%s'", next_syntax->TH->a.s.v->Str(), unit_name);
			f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String(msg), errRes);
		} else if (s == str_not_const) {
			AGmNode* errMsg = NULL;
			set_lno(f, lno, errMsg, errRes);
			const char* token = next_syntax->TH->a.s.v->Str();
			long ignore_case = (long)next_syntax->TTHI;
			if (!f->MatchString(token, ignore_case)) {
IF_DEBUG_30 {
::indent();
art_sprintf_3(msg, 256, "not_token(OK) = %s, lno = %ld, fp = %ld\n", token, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
				continue;
			}
indent_count--;
IF_DEBUG_30 {
::indent();
art_sprintf_4(msg, 256, "failure(%s), not_token(NG) = %s, lno = %ld, fp = %ld\n", unit_name, token, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
			RefUnref(tree);
			art_sprintf_2(msg, 256, "unexpected '%s' at '%s'", next_syntax->TH->a.s.v->Str(), unit_name);
			f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String(msg), errRes);
		} else if (s == str_no_sp_not_const) {
			AGmNode* errMsg = NULL;
			set_lno(f, lno, errMsg, errRes);
			const char* token = next_syntax->TH->a.s.v->Str();
			long ignore_case = (long)next_syntax->TTHI;
			if (!f->_MatchString(token, ignore_case)) {
IF_DEBUG_30 {
::indent();
art_sprintf_3(msg, 256, "not_token(OK) = %s, lno = %ld, fp = %ld\n", token, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
				continue;
			}
indent_count--;
IF_DEBUG_30 {
::indent();
art_sprintf_4(msg, 256, "failure(%s), not_token(NG) = %s, lno = %ld, fp = %ld\n", unit_name, token, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
			RefUnref(tree);
			art_sprintf_2(msg, 256, "unexpected '%s' at '%s'", next_syntax->TH->a.s.v->Str(), unit_name);
			f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String(msg), errRes);
		} else if (s == str_not_nl) {
			AGmNode* errMsg = NULL;
			set_lno(f, lno, errMsg, errRes);
			if (cont) parse_comment(f, curr_syntax->H, errRes);
			if (eol == 0) continue;
			 RefUnref(tree); f->Back(pos); Pop(def_stack);
			 return parse_error(f, lno, String("not expected newline"), errRes);
		} else if (s == str_reg_ref) {
			AGmNode* errMsg = NULL;
			set_lno(f, lno, errMsg, errRes);
			long n = (long)next_syntax->THI;
			int ignore_case = (long)next_syntax->TTHI;
			AGmNode *ls2 = tree->T;
			for (int i = 1; i < n && ls2; i++, ls2 = ls2->T) ;
			if (ls2 == NULL) {
				RefUnref(tree); f->Back(pos); Pop(def_stack);
				return parse_error(f, lno, String("$<n> part illegal."), errRes);
			}
			if (!IsString(ls2->H)) {
				RefUnref(tree); f->Back(pos); Pop(def_stack);
				return parse_error(f, lno, String("$<n> part is not <regex>."), errRes);
			}
			if (!f->MatchString(ls2->H->a.s.v, ignore_case)) {
				RefUnref(tree); f->Back(pos); Pop(def_stack);
				return parse_error(f, lno, String("$<n> part not match <regex>."), errRes);
			}
			continue;
		} else if (s == str_until) {
			const char *ss = NULL;
			if (IsString(next_syntax->TH)) {
				ss = next_syntax->TH->a.s.v->Str();
			} else if (IsInteger(next_syntax->TH)) {
				long n = (long)next_syntax->THI;
				AGmNode *ls2 = tree->T;
				for (int i = 1; i < n && ls2; i++, ls2 = ls2->T) ;
				if (ls2 == NULL) {
					RefUnref(tree); f->Back(pos); Pop(def_stack);
					return parse_error(f, lno, String("$<n> part illegal."), errRes);
				}
				if (!IsString(ls2->H)) {
					RefUnref(tree); f->Back(pos); Pop(def_stack);
					return parse_error(f, lno, String("$<n> part is not <regex>."), errRes);
				}
				ss = ls2->H->a.s.v->Str();
			}
			if (ss == NULL) {
				RefUnref(tree); f->Back(pos); Pop(def_stack);
				return parse_error(f, lno, String("$<n> part is illegal."), errRes);
			}
			AGmString *str = new AGmString(64);
			if (f->SearchIdent(ss, str)) {
				Set(addr->T, Cons(String(str))); addr = addr->T; continue;
			}
			delete str; RefUnref(tree); f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String("expected until term token"), errRes);
		} else if (s == str_redirect) {
			const char *ss = NULL;
			if (IsInteger(next_syntax->TH)) {
				long n = (long)next_syntax->THI;
				AGmNode *ls2 = tree->T;
				for (int i = 1; i < n && ls2; i++, ls2 = ls2->T) ;
				if (ls2 == NULL) {
					RefUnref(tree); f->Back(pos); Pop(def_stack);
					return parse_error(f, lno, String("$<n> part illegal."), errRes);
				}
				if (!IsString(ls2->H)) {
					RefUnref(tree); f->Back(pos); Pop(def_stack);
					return parse_error(f, lno, String("$<n> part is not <regex>."), errRes);
				}
				ss = ls2->H->a.s.v->Str();
			}
			if (ss == NULL) {
				RefUnref(tree); f->Back(pos); Pop(def_stack);
				return parse_error(f, lno, String("$<n> part is illegal."), errRes);
			}
			if (f->Redirect(ss) == 0) continue;
			RefUnref(tree); f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String("expected until term token"), errRes);
		} else if (s == str_line) {
			AGmNode* errMsg = NULL;
			set_lno(f, lno, errMsg, errRes);
			AGmString *str = NULL;
			if (f->ReadLine(str) == 0) {
				Set(addr->T, Cons(String(str))); addr = addr->T; continue;
			}
			RefUnref(tree); f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String("not newline end string"), errRes);
		} else if (s == str_define) {
			AGmNode *name = next_syntax->TH;
			Push(def_stack->H, name);
			continue;
		} else if (s == str_undef) {
			AGmNode *name = next_syntax->TH, *ls, *prev_ls = NULL;
			for (ls = def_stack->H; ls; prev_ls = ls, ls = ls->T) if (ls->H == name) break;
			if (ls) {
				if (prev_ls == NULL) {
					Set(def_stack->H, ls->T);
				} else {
					Set(prev_ls->T, ls->T);
				}
			}
			continue;
		} else if (s == str_ifdef) {
			AGmNode *name = next_syntax->TH, *ls1=NULL, *ls2=NULL;
			for (ls1 = def_stack; ls1; ls1 = ls1->T) {
				for (ls2 = ls1->H; ls2; ls2 = ls2->T) {
					if (ls2->H == name) break;
				}
				if (ls2) break;
			}
			if (ls2) continue;
			RefUnref(tree); f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String("ifdef ident not defined."), errRes);
		} else if (s == str_ifndef) {
			AGmNode *name = next_syntax->TH, *ls1=NULL, *ls2=NULL;
			for (ls1 = def_stack; ls1; ls1 = ls1->T) {
				for (ls2 = ls1->H; ls2; ls2 = ls2->T) {
					if (ls2->H == name) break;
				}
				if (ls2) break;
			}
			if (ls2 == NULL) continue;
			RefUnref(tree); f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String("ifndef ident defined."), errRes);
		} else if (s == str_nonewline) {
			continue;
		} else if (s == str_regex) {
			AGmNode* errMsg = NULL;
			set_lno(f, lno, errMsg, errRes);
			AGmNode *value;
			if (cont) parse_comment(f, curr_syntax->H, errRes);
			int err = parse_reg_scanf(f, next_syntax, lno, errRes, value);
			if (err == 0) {
				Set(addr->T, Cons(value)); addr = addr->T;
IF_DEBUG_30 {
::indent();
AGmNode* s = BigInteger::ToString(value);
art_sprintf_3(msg, 256, "regex(OK) = %s, lno = %ld, fp = %ld\n", s->a.s.v->Str(), f->LineNumber(), f->CurrentPoint());
RefUnref(s);
ADvWindow::Print(msg);
}
				const_regex_count++; continue;
			}
			RefUnref(tree);
			art_sprintf_1(msg, 256, "regex not match at '%s'", unit_name);
indent_count--;
IF_DEBUG_30 {
::indent();
art_sprintf_3(msg, 256, "failure(%s), regex(NG), lno = %ld, fp = %ld\n", unit_name, f->LineNumber(), f->CurrentPoint());
ADvWindow::Print(msg);
}
			f->Back(pos); Pop(def_stack);
			return parse_error(f, lno, String(msg), errRes);
		} else if (s == str_repeat_0) {
			tree2 = parse_repeat0_scanf(f, next_syntax, lno, pos2, errRes);
			if (first) pos = pos2;
			if (tree2 && IsList(tree2) && tree2->T) optional_is_empty = 0;
		} else if (s == str_repeat_1) {
			tree2 = parse_repeat1_scanf(f, next_syntax, lno, pos2, errRes);
			if (first) pos = pos2;
			not_optional_count++; not_optional = tree2;
		} else if (s == str_braket_0) {
			tree2 = parse_braket0_scanf(f, next_syntax, lno, pos2, errRes);
			if (first) pos = pos2;
			if (tree2 && IsList(tree2) && tree2->T) optional_is_empty = 0;
		} else if (s == str_braket_1) {
			tree2 = parse_para_scanf(f, next_syntax, lno, pos2, errRes);
			if (first) pos = pos2;
			not_optional_count++; not_optional = tree2;
		} else if (s == str_eof) {
			if (cont) parse_comment(f, curr_syntax->H, errRes);
			if (f->IsEnd() == 0) {
				RefUnref(tree); f->Back(pos); Pop(def_stack);
				return parse_error(f, lno, String("EOF expected."), errRes);
			}
			continue;
		} else {
			tree2 = parse_unit_scanf(f, next_syntax, lno, pos2, errRes);
			if (first) pos = pos2;
			not_optional_count++; not_optional = tree2;
		}
		if (IsString(tree2)) {
indent_count--;
IF_DEBUG_30 {
::indent();
art_sprintf_2(msg, 256, "failure(%s), lno = %ld\n", unit_name, f->LineNumber());
ADvWindow::Print(msg);
}
			RefUnref(tree); f->Back(pos); Pop(def_stack); return tree2;
		}
		Set(addr->T, Cons(tree2)); addr = addr->T;
IF_DEBUG_30 {
::indent();
art_sprintf_1(msg, 256, "unit(OK), lno = %ld\n", f->LineNumber());
ADvWindow::Print(msg);
}
	}
	if (parse_full == 0 && const_regex_count == 0 && not_optional_count == 1 && optional_is_empty == 1) {
		for (ls = tree;; ls = ls->T) {
			if (ls->H == not_optional) break;
		}
		tree2 = ls->H; tree2->count--;
		ls->H = NULL; RefUnref(tree);
indent_count--;
IF_DEBUG_10 {
::indent();
art_sprintf_2(msg, 256, "success(%s), lno = %ld\n", unit_name, f->LineNumber());
ADvWindow::Print(msg);
DumpList(tree2, 50);
}
IF_DEBUG_20 {
::indent();
art_sprintf_3(msg, 256, "success(%s), start_lno = %ld, end_lno = %ld\n", unit_name, start_lno, f->LineNumber());
ADvWindow::Print(msg);
DumpList(tree2, 50);
}
IF_DEBUG_30 {
::indent();
art_sprintf_2(msg, 256, "success(%s), lno = %ld\n", unit_name, f->LineNumber());
ADvWindow::Print(msg);
DumpList(tree2, 50);
}
		Pop(def_stack); return tree2;
	}
indent_count--;
IF_DEBUG_10 {
::indent();
art_sprintf_2(msg, 256, "success(%s), lno = %ld\n", unit_name, f->LineNumber());
ADvWindow::Print(msg);
DumpList(tree, 50);
}
IF_DEBUG_20 {
::indent();
art_sprintf_3(msg, 256, "success(%s), start_lno = %ld, end_lno = %ld\n", unit_name, start_lno, f->LineNumber());
ADvWindow::Print(msg);
DumpList(tree, 50);
}
IF_DEBUG_30 {
::indent();
art_sprintf_2(msg, 256, "success(%s), lno = %ld\n", unit_name, f->LineNumber());
ADvWindow::Print(msg);
DumpList(tree, 50);
}
	Pop(def_stack); return tree;
}
void AGmParserGenerator::parse_comment(AGmFile* f, AGmNode* def, AGmNode*& errMsg) {
	AGmNode *cs_name = dst_node2(ids_cs, def);
	long cc = f->LineNumber();
#ifdef DEBUG_COMMENT_SPACE
const char *unit_name = def->a.s.v->Str();
char msg[256];
IF_DEBUG_30 {
::indent();
art_sprintf_2(msg, 256, "parse_COMMENT(%s), lno = %d\n", IsString(cs_name) ? cs_name->a.s.v->Str() : "-base-", cc);
ADvWindow::Print(msg);
}
#endif
	for (;;) {
		f->ReadSpace();
		int found = 0;
		AGmNode *ls;
		for (ls = comment->H; ls; ls = ls->T) {
			AGmNode *n = ls->H;
			if (n->H != cs_name) continue;
			AGmString *str, *b, *e;
#if	_WINDOWS && _64BIT
			long long lno = 0, pos = f->CurrentPoint();
#else
			long lno = 0, pos = f->CurrentPoint();
#endif
			switch (n->THI) {
			case 0: // regex
				str = new AGmString(64);
				if (parse_reg_scanf(f, n->TTTH->a.s.v->Str(), n->TTH, lno, errMsg, str) == 0 && str->Length() != 0) {
					if (f->NewlineToken() && str->LastChar() == '\n') f->Unput();
#ifdef DEBUG_COMMENT_SPACE
IF_DEBUG_30 {
::indent();
art_sprintf_2(msg, 256, "COMMENT(%s), lno = %d\n", str->Str(), f->LineNumber());
ADvWindow::Print(msg);
}
#endif
					found = 1; delete str; break;
				}
				delete str; str = NULL;
				f->Back(pos); break;
			case 1: // begin_end
				b = n->TTH->a.s.v; e = n->TTTH->a.s.v;
				if (f->MatchString(b->Str()) && f->Search(e->Str())) {
#ifdef DEBUG_COMMENT_SPACE
IF_DEBUG_30 {
::indent();
art_sprintf_3(msg, 256, "COMMENT(%s...%s), lno = %d\n", b->Str(), e->Str(), f->LineNumber());
ADvWindow::Print(msg);
}
#endif
					found = 1; break;
				}
				f->Back(pos); break;
			case 2: // until_newline
				b = n->TTH->a.s.v;
				if (f->MatchString(b->Str())) {
					if (f->ReadLine(e) == 0) delete e;
					if (f->NewlineToken()) f->Unput();
#ifdef DEBUG_COMMENT_SPACE
IF_DEBUG_30 {
::indent();
art_sprintf_2(msg, 256, "COMMENT(...%s), lno = %d\n", b->Str(), f->LineNumber());
ADvWindow::Print(msg);
}
#endif
					found = 1; break;
				}
				f->Back(pos); break;
			}
			if (found) break;
		}
		if (found == 0) break;
	}
	cc = f->LineNumber() - cc;
	if (cc > 0) {
		eol = 1;
	} else {
		eol = 0;
	}
}
#if	_WINDOWS && _64BIT
AGmNode* AGmParserGenerator::parse_error(AGmFile* f, long long& lno, AGmNode* errMsg, AGmNode*& errRes) {
#else
AGmNode* AGmParserGenerator::parse_error(AGmFile* f, long& lno, AGmNode* errMsg, AGmNode*& errRes) {
#endif
#ifdef DEBUG_IMPROVE_ERR_MSG
	if (error == NULL || lno < f->LineNumber()) {
		Set(error, errMsg);
	} else {
		RefUnref(errMsg);
	}
	return error;
#else
	if (error == NULL || lno < f->LineNumber()) {
		set_lno(f, lno, errMsg, errRes);
	} else {
		RefUnref(errMsg);
	}
	return error;
#endif
}

#if	_WINDOWS && _64BIT
int AGmParserGenerator::parse_reg_scanf(AGmFile* f, AGmNode* curr_syntax, long long& lno, AGmNode*& errRes, AGmNode*& value) {
#else
int AGmParserGenerator::parse_reg_scanf(AGmFile* f, AGmNode* curr_syntax, long& lno, AGmNode*& errRes, AGmNode*& value) {
#endif
	AGmString* str = new AGmString(64);
	if (parse_reg_scanf(f, curr_syntax->TTTH->a.s.v->Str(), curr_syntax->TH, lno, errRes, str) ||
			str->Length() == 0 || dst_node(reserve, str->Str())) {
		delete str; return -1;
	}
	AGmNode *t = curr_syntax->TTH;
	if (t == str_integer) {
		value = BigInteger::FromString(str->Str()); delete str;
		AGmNode *v = BigInteger::ToInteger(value);
		if (v) {
			RefUnref(value); value = v;
		}
	} else if (t == str_hex) {
		long lv;
		art_sscanf_1(str->Str(), "0x%lx", &lv); delete str;
		value = Integer(lv);
	} else if (t == str_char) {
		char c = str->Component(1);
		if (c == '\\') {
			c = str->Component(2);
			if (c == 't') c = '\t';
			else if (c == 'r') c = '\r';
			else if (c == 'n') c = '\n';
		}
		delete str;
		value = Integer(c);
	} else if (t == str_real) {
		double rv;
		art_sscanf_1(str->Str(), "%lg", &rv); delete str;
		value = Real(rv);
	} else if (t == str_string || t == str_special) {
		value = String(str);
	} else if (t == str_quote_string) {
		AGmString *str2 = str->SubString(1, str->Length()-1); delete str;
		int i;
		for (i = 0; i < str2->Length(); i++) {
			char c = str2->Component(i);
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				i++; continue;
			}
#endif
#ifdef	_LINUX
			int i2;
			if (isUTF8((unsigned char*)str2->Str()+i, i2)) {
				i += i2-1; continue;
			}
#endif
			if (c == '\\' && i+1 < str2->Length()) {
				c = str2->Component(i+1);
				if (c == 't') {
					str2->ReplaceStr(i, i+2, "\t");
				} else if (c == 'r') {
					str2->ReplaceStr(i, i+2, "\r");
				} else if (c == 'n') {
					str2->ReplaceStr(i, i+2, "\n");
				} else if (c == '\\') {
					str2->ReplaceStr(i, i+2, "\\");
				} else {
					str2->ReplaceStr(i, i+1, "");
				}
			}
		}
		value = String(str2);
	} else if (t == str_single_quote_string) {
		AGmString *str2 = str->SubString(1, str->Length()-1); delete str;
		int i;
		for (i = 0; i < str2->Length(); i++) {
			char c = str2->Component(i);
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				i++; continue;
			}
#endif
#ifdef	_LINUX
			int i2;
			if (isUTF8((unsigned char*)str2->Str()+i, i2)) {
				i += i2-1; continue;
			}
#endif
			if (c == '\\' && i+1 < str2->Length()) {
				c = str2->Component(i+1);
				if (c == 't') {
					str2->ReplaceStr(i, i+2, "\t");
				} else if (c == 'r') {
					str2->ReplaceStr(i, i+2, "\r");
				} else if (c == 'n') {
					str2->ReplaceStr(i, i+2, "\n");
				} else if (c == '\\') {
					str2->ReplaceStr(i, i+2, "\\");
				} else {
					str2->ReplaceStr(i, i+1, "");
				}
			}
			if (c == '\'' && i+1 < str2->Length()) {
				c = str2->Component(i+1);
				if (c == '\'') {
					str2->ReplaceStr(i, i+2, "'");
				} else {
				}
			}
		}
		value = String(str2);
	}
	return 0;
}
#if	_WINDOWS && _64BIT
int AGmParserGenerator::parse_reg_scanf(AGmFile* f, const char* def_name, AGmNode* curr_syntax, long long& lno, AGmNode*& errRes, AGmString* str) {
	long long pos = f->CurrentPoint();
#else
int AGmParserGenerator::parse_reg_scanf(AGmFile* f, const char* def_name, AGmNode* curr_syntax, long& lno, AGmNode*& errRes, AGmString* str) {
	long pos = f->CurrentPoint();
#endif
	AGmNode *ls, *unit; int cmd, count;
	AGmString *str2 = NULL;
	for (ls = curr_syntax->T; ls; ls = ls->T) {
		unit = ls->H; cmd = (long)unit->HI;
		if (cmd == '*' || cmd == '+' || cmd == '?' || cmd == '{') {
			unit = unit->TH;
			for (count = 0;; count++) {
				str2 = new AGmString(64);
				if (parse_reg_unit_scanf(f, def_name, unit, lno, errRes, str2) || str2->Length() == 0) {
					delete str2;
					if (cmd == '*' || cmd == '?') break;
					if (cmd == '+') {
						if (count > 0) break;
						f->Back(pos); return -1;
					}
					if (cmd == '{') {
						long n = (long)ls->THI, m = (long)ls->TTHI;
						if ((m == 0 && count == n) || (m == -1 && n < count) || (n <= count && count <= m)) break;
						f->Back(pos); return -1;
					}
				} else {
					str->Append(str2); delete str2;
					if (cmd == '?') break;
				}
			}
		} else {
			str2 = new AGmString(64);
			if (parse_reg_unit_scanf(f, def_name, unit, lno, errRes, str2) || str2->Length() == 0) {
				delete str2; f->Back(pos); return -1;
			}
			str->Append(str2); delete str2;
		}
	}
	return 0;
}
#if	_WINDOWS && _64BIT
int AGmParserGenerator::parse_reg_unit_scanf(AGmFile* f, const char* def_name, AGmNode* curr_syntax, long long& lno, AGmNode*& errRes, AGmString* str) {
	long long pos = f->CurrentPoint();
#else
int AGmParserGenerator::parse_reg_unit_scanf(AGmFile* f, const char* def_name, AGmNode* curr_syntax, long& lno, AGmNode*& errRes, AGmString* str) {
	long pos = f->CurrentPoint();
#endif
	int cmd = (long)curr_syntax->HI;
	if (cmd == 0) {
		long c1 = (long)curr_syntax->THI;
		long ch = f->GetChar();
		if (ch == EOF) { f->Back(pos); return -1; }
		if (c1 == ZENKAKU) {
			if ((ch & 0xff00) == 0) {
				f->Back(pos); return -1;
			}
		} else if (ch != c1) {
			f->Back(pos); return -1;
		}
		eol = 0;
		str->AddChar(ch);
	} else if (cmd == '[') {
		AGmNode *ls;
		long ch = f->GetChar();
		if (ch == EOF) { f->Back(pos); return -1; }
		for (ls = curr_syntax->THT; ls; ls = ls->T) {
			long c1 = (long)ls->HHI;
			if (ls->HT == NULL) {
				if (c1 == ZENKAKU) {
					if((ch & 0xff00) != 0) {
						eol = 0;
						str->AddChar(ch); return 0;
					}
				} else if (ch == c1) {
					eol = 0;
					str->AddChar(ch); return 0;
				}
			} else {
				long c2 = (long)ls->HTHI;
				if (c1 <= ch && ch <= c2) {
					eol = 0;
					str->AddChar(ch); return 0;
				}
			}
		}
		f->Back(pos); return -1;
	} else if (cmd == '!') {
		AGmNode *ls;
		long ch = f->GetChar();
		if (ch == EOF) { f->Back(pos); return -1; }
		for (ls = curr_syntax->THT; ls; ls = ls->T) {
			long c1 = (long)ls->HHI;
			if (ls->HT == NULL) {
				if (c1 == ZENKAKU) {
					if((ch & 0xff00) != 0) {
						f->Back(pos); return -1;
					}
				} else if (ch == c1) {
					f->Back(pos); return -1;
				}
			} else {
				long c2 = (long)ls->HTHI;
				if (c1 <= ch && ch <= c2) {
					f->Back(pos); return -1;
				}
			}
		}
		eol = 0;
		str->AddChar(ch);
	} else if (cmd == '(') {
		AGmNode *ls;
		for (ls = curr_syntax->T; ls; ls = ls->T) {
			AGmString *str2 = new AGmString(str->Str());
			if (parse_reg_scanf(f, def_name, ls->H, lno, errRes, str2) || str2->Length() == 0) { delete str2; continue; }
			eol = 0;
			str->Append(str2); delete str2; return 0;
		}
		return -1;
	} else if (cmd == '^') {
		long cc = f->LineNumber(); f->ReadSpace(); cc = f->LineNumber() - cc;
		if (cc <= 0) { f->Back(pos); return -1; }
		eol = 0;
		str->Append('\n');
	} else if (cmd == '$') {
		if (f->NewlineToken()) {
			if (eol == 0) {
				char c = f->_Input();
				if (c != '\n') { f->Back(pos); return -1; }
			}
		} else {
			if (eol == 0) {
				AGmString *line;
				if (f->ReadLine(line)) { f->Back(pos); return -1; }
				if (!line->IsEmpty()) { delete line; f->Back(pos); return -1; }
				delete line;
			}
			eol = 0;
		}
		str->Append('\n');
	} else if (cmd == '.') {
		long c = f->GetChar(); if (c == EOF) { f->Back(pos); return -1; }
		eol = 0;
		str->AddChar(c);
	}
	return 0;
}
#if	_WINDOWS && _64BIT
void AGmParserGenerator::set_lno(AGmFile* f, long long& lno, AGmNode* errMsg, AGmNode*& errRes) {
#else
void AGmParserGenerator::set_lno(AGmFile* f, long& lno, AGmNode* errMsg, AGmNode*& errRes) {
#endif
#ifdef DEBUG_IMPROVE_ERR_MSG
	if (lno < f->LineNumber()) {
		lno = f->LineNumber();
	}
#else
	if (error == NULL || lno < f->LineNumber()) {
		Set(error, errMsg);
		lno = f->LineNumber();
		errRes = errMsg;
	}
#endif
}

AGmNode* AGmParserGenerator::Generate(AGmFile* f, AGmNode* tree) {
	if (error) return String(error->a.s.v->Str());
	Unref(indent_stack); indent_stack = NULL;
	indent_flag = 0;
	if (dst_node(gen_format, 200)) {
		split = 1;
		last_ident = 0;
	} else {
		split = 0;
	}
	return gen_unit_printf(f, tree);
}
AGmNode* AGmParserGenerator::gen_unit_printf(AGmFile* f, AGmNode* tree) {
	if (tree == NULL) return NULL;
	if (!IsList(tree)) {
		int count = 0;
		AGmString *str = new AGmString("<unit> is not list: value = ");
		f = new AGmFile();
		f->Create(str, AGM_FILE_STR_APPEND);
		f->WriteList(tree, count, 20);
		delete f;
		return String(str);
	}
	char msg[256];
	AGmNode *curr_syntax = dst_node(ids, tree->H->a.s.v->Str());
	if (curr_syntax == NULL) {
		AGmNode *s = tree->H;
		if (s == str_repeat_0 || s == str_repeat_1 || s == str_braket_0) {
			return gen_braket_printf(f, tree);
		} else {
			art_sprintf_1(msg, 256, "generate: unit '%s' not found", tree->H->a.s.v->Str());
			return String(msg);
		}
	}
	int set_nonewline = 0;
	AGmNode *ls, *ls2, *err; int n = 1;
	for (ls = curr_syntax->T, ls2 = tree->T; ls; ls = ls->T, n++) {

		for (; ls; ls = ls->T) {
			AGmNode *h = ls->HH;
			if (h == str_not_const || h == str_no_sp_not_const
					|| h == str_not_nl || h == str_line || h == str_redirect || h == str_eof
					|| h == str_define || h == str_undef || h == str_ifdef || h == str_ifndef) continue;
			break;
		}
		if (ls == NULL) break;


		AGmNode *next_syntax = ls->H;
		AGmNode *s = next_syntax->H;
		if (s == str_nonewline) {
			nonewline = 1; set_nonewline = 1; continue;
		}
		if (nonewline == 0) {
			AGmLink *l;
			for (l = next_outlink(gen_format, NULL); l; l = next_outlink(gen_format, l)) {
				AGmNode *format = l->arc->dst, *c_or_id = l->arc->attr;
				if (!IsList(c_or_id)) continue;
				int cmd = (long)c_or_id->HI;
				if (cmd == 0) {
					if (c_or_id->TH != next_syntax->TH) continue;
					cmd = (long)format->HI;
				} else if (cmd == 1) {
					if (c_or_id->TH != tree->H) continue;
					if ((long)c_or_id->TTHI != n) continue;
					cmd = (int)format->HI;
				}
				if (cmd == 10) { // newline
					f->WriteString("\n"); space_flag = 0;
					if (indent_flag == 1) indent_flag = 2;
				} else if (cmd == 11) { // indent
					indent_flag = 1;
				} else if (cmd == 12) { // newline indent
					f->WriteString("\n"); space_flag = 0;
					indent_flag = 2;
				} else if (cmd == 15) { // space
					space_flag = 1;
				}
				if (cmd == 31) { // +indent
					const char *str = format->TH->a.s.v->Str();
					if (indent_stack == NULL) {
						Push(indent_stack, String(str));
					} else {
						AGmString *ss = new AGmString(indent_stack->H->a.s.v->Str());
						ss->Append(str);
						Push(indent_stack, String(ss));
					}
				} else if (cmd == 32) { // -indent
					if (indent_stack == NULL) return String("Pop: indent_stack empty.");	
					Pop(indent_stack);
				}
			}
		}
		if (s == str_const || s == str_no_sp_const) {
			const char *ss = next_syntax->TH->a.s.v->Str();
			unsigned char c = next_syntax->TH->a.s.v->LastChar();
			if (space_flag || (split && last_ident && (isIdentTail(*ss) || isFirstChar(*ss)))) {
				f->WriteString(" "); space_flag = 0;
			}
			if (indent_flag == 2) {
				indent(f); indent_flag = 0;
			}
			f->WriteString(ss); space(f);
			last_ident = (isIdentTail(c) || isFirstChar(c));
		} else if (s == str_reg_ref) {
			long n = (long)next_syntax->THI;
			AGmNode *ls2 = tree->T;
			for (int i = 1; i < n && ls2; i++, ls2 = ls2->T) ;
			if (ls2) {
				const char *ss = ls2->H->a.s.v->Str();
				unsigned char c = ls2->H->a.s.v->LastChar();
				if (space_flag || (split && last_ident && (isIdentTail(*ss) || isFirstChar(*ss)))) {
					f->WriteString(" "); space_flag = 0;
				}
				if (indent_flag == 2) {
					indent(f); indent_flag = 0;
				}
				f->WriteString(ss); space(f);
				last_ident = (isIdentTail(c) || isFirstChar(c));
			}
		} else if (s == str_regex) {
			if (ls2 == NULL) {
				art_sprintf_2(msg, 256, "number of elements mismatch. (%s, %s)", tree->H->a.s.v->Str(), s->a.s.v->Str());
				return String(msg);
			}
			AGmString *str = gen_reg_printf(next_syntax, ls2->H);
			if (str == NULL) {
				str = new AGmString("regex illegal: value = ");
				int count = 0;
				str->WriteList(ls2->H, count);
				return String(str);
			}
			const char *ss = str->Str();
			unsigned char c = str->LastChar();
			if (space_flag || (split && last_ident && (isIdentTail(*ss) || isFirstChar(*ss)))) {
				f->WriteString(" "); space_flag = 0;
			}
			if (indent_flag == 2) {
				indent(f); indent_flag = 0;
			}
			f->WriteString(ss); space(f);
			last_ident = (isIdentTail(c) || isFirstChar(c));

			if (strcmp(ss, "\n") == 0) {
				space_flag = 0; indent_flag = 2;
			}

			delete str;
			ls2 = ls2->T;
		} else if (s == str_until) {
			long n = (long)next_syntax->THI;
			AGmNode *ls2 = tree->T;
			for (int i = 1; i < n && ls2; i++, ls2 = ls2->T) ;
			if (ls2) {
				if (ls2->T && ls2->TT && ls2->TTH) {
					if (f->HereDoc(ls2->TTH, ls2->H)) {
						return String("multiple defintion of here doc.");
					}
				}
			}
		} else if (s == str_repeat_0 || s == str_repeat_1 || s == str_braket_0) {
			if (ls2 == NULL) {
				art_sprintf_2(msg, 256, "number of elements mismatch. (%s, %s)", tree->H->a.s.v->Str(), s->a.s.v->Str());
				return String(msg);
			}
			if (ls2->HT) {
				err = gen_braket_printf(f, ls2->H);
				if (err) return err;
			}
			ls2 = ls2->T;
		} else { // '{|}'
			if (ls2 == NULL) {
				art_sprintf_2(msg, 256, "number of elements mismatch. (%s, %s)", tree->H->a.s.v->Str(), s->a.s.v->Str());
				return String(msg);
			}
			err = gen_unit_printf(f, ls2->H);
			if (err) return err;
			ls2 = ls2->T;
		}
		if (nonewline == 0) {
			AGmLink *l;
			for (l = next_outlink(gen_format, NULL); l; l = next_outlink(gen_format, l)) {
				AGmNode *format = l->arc->dst, *c_or_id = l->arc->attr;
				if (!IsList(c_or_id)) continue;
				int cmd = (long)c_or_id->HI;
				if (cmd == 0) {
					if (c_or_id->TH != next_syntax->TH) continue;
					cmd = (long)format->HI;
				} else if (cmd == 1) {
					if (c_or_id->TH != tree->H) continue;
					if ((long)c_or_id->TTHI != n) continue;
					cmd = (long)format->HI;
				}
				if (cmd == 20) { // newline
					f->WriteString("\n"); space_flag = 0;
					if (indent_flag == 1) indent_flag = 2;
				} else if (cmd == 21) { //indent
					indent_flag = 1;
				} else if (cmd == 22) { // newline indent
					f->WriteString("\n"); space_flag = 0;
					indent_flag = 2;
				} else if (cmd == 25) { // space
					space_flag = 1;
				}
				if (cmd == 41) { // indent+
					const char *str = format->TH->a.s.v->Str();
					if (indent_stack == NULL) {
						Push(indent_stack, String(str));
					} else {
						AGmString *ss = new AGmString(indent_stack->H->a.s.v->Str());
						ss->Append(str);
						Push(indent_stack, String(ss));
					}
				} else if (cmd == 42) { // indent-
					if (indent_stack == NULL) return String("Pop: indent_stack empty.");	
					Pop(indent_stack);
				}
			}
		}
	}
	if (set_nonewline) nonewline = 0;
	return NULL;
}
AGmNode* AGmParserGenerator::gen_braket_printf(AGmFile* f, AGmNode* tree) {
	AGmNode *ls;
	for (ls = tree->T; ls; ls = ls->T) {
		AGmNode *next_tree = ls->H;
		AGmNode *err = gen_unit_printf(f, next_tree);
		if (err) return err;
	}
	return NULL;
}
AGmString* AGmParserGenerator::gen_reg_printf(AGmNode* next_syntax, AGmNode* value) {
	AGmString *s = new AGmString(64);
	AGmNode *t = next_syntax->TTH;
	if (t == str_integer) {
		s->WriteInteger(value);
	} else if (t == str_hex) {
		if (!IsInteger(value)) { delete s; return NULL; }
		s->WriteString("0x");
		s->WriteHexa((long)value->a.i.v);
	} else if (t == str_char) {
		if (!IsInteger(value)) { delete s; return NULL; }
		s->WriteQChar((char)value->a.i.v);
	} else if (t == str_real) {
		if (!IsReal(value)) { delete s; return NULL; }
		s->WriteReal(value->a.r.v);
	} else if (t == str_string) {
		if (!IsString(value)) { delete s; return NULL; }
		s->WriteString(value->a.s.v);
	} else if (t == str_quote_string) {
		if (!IsString(value)) { delete s; return NULL; }
		s->WriteQString(value->a.s.v);
	} else if (t == str_single_quote_string) {
		if (!IsString(value)) { delete s; return NULL; }
		s->WriteString("'");
		s->WriteSingleQString(value->a.s.v);
		s->WriteString("'");

	} else if (t == str_special) {
		if (!IsString(value)) { delete s; return NULL; }
		if (value->a.s.v->Compare("$value") == 0) {
			s->WriteString("\n");
		} else {
			s->WriteString(value->a.s.v);
		}
	}

	return s;
}
void AGmParserGenerator::indent(AGmFile* f) {
	if (indent_stack == NULL) return;
	f->WriteString(indent_stack->H->a.s.v->Str());
}
void AGmParserGenerator::set_space(int space_on) {
	this->space_on = space_on;
}
void AGmParserGenerator::space(AGmFile* f) {
	if (space_on) {
		f->WriteString(" ");
	}
}

AGmNode* AGmParserGenerator::OutSyntaxUnit(AGmFile* f, int out_skelton) {
	if (error) return String(error->a.s.v->Str());

	depend = Ref(String("$depend"));
	units = Ref(Cons(NULL));
	syn_ids = Ref(Cons(NULL));

	AGmLink *l;
	// Generation of grammar ID, grammar data and dependency graph of grammar ID
	for (l = next_outlink(ids, NULL); l; l = next_outlink(ids, l)) {
		AGmNode *n = l->arc->dst;
		if (!n || !IsList(n)) { Unref(depend, units, syn_ids); return String("unit is not list."); }
		AGmNode *id = n->H;
		if (!IsString(id)) { Unref(depend, units, syn_ids); return String("unit-list-head is not string."); }
		AGmNode *unit = NULL;
		AGmNode *ret = OutSyntaxUnit(unit, id, n->T);
		if (ret) { Unref(depend, units, syn_ids); return ret; }
		if (unit == NULL) continue;
		CreateArc(units, unit, id);
		Unref(unit);
	}

	// Generation of grammar ID referred to directly from def_name
	for (l = next_outlink(syntax, NULL); l; l = next_outlink(syntax, l)) {
		AGmNode *unit = l->arc->dst;
		if (!IsList(unit) || !IsString(unit->H)) continue;
		if (unit->H == str_regex) continue;
		if (unit->H == str_braket_1) {
			AGmNode *ls;
			for (ls = unit->T; ls && IsList(ls); ls = ls->T) {
				if (!IsList(ls) || !ls->H || !IsList(ls->H) || !IsString(ls->HH)) continue;
				CreateArc(syn_ids, Cons(NULL), ls->HH);
			}
		} else {
			CreateArc(syn_ids, Cons(NULL), unit->H);
		}
	}

	// Elimination of grammar ID which are not referred to directly from def_name
	for (l = next_outlink(units, NULL); l; l = next_outlink(units, l)) {
		// id is ID to be deleted
		AGmNode *id = l->arc->attr;
		if (dst_node(syn_ids, id)) continue;
		AGmNode *unit = l->arc->dst;

		AGmLink *l2;
		for (l2 = next_inlink(id, NULL, depend); l2; l2 = next_inlink(id, l2, depend)) {
			// src_id uses id.	
			// replaced the id part in src_unit with unit of id.	
			AGmNode *src_id = l2->arc->src;
			AGmNode *src_unit = dst_node(units, src_id);
			if (!src_unit) { Unref(depend, units, syn_ids); return String("src_unit not found at 'grammar id elimination."); }
			AGmNode *ret = EliminateGrammarID(src_unit, id, unit);
			if (ret) { Unref(depend, units, syn_ids); return ret; }

			// Elimination grammar ID dependency from grammar ID dependency graph	
			l2 = l2->prev;
			DeleteArc(l2->next->arc);
		}
	}
	// Elimination grammar ID to be deleted.
	for (l = next_outlink(units, NULL); l; l = next_outlink(units, l)) {
		AGmNode *id = l->arc->attr;
		if (dst_node(syn_ids, id)) continue;
		l = l->prev;
		DeleteArc(l->next->arc);
	}

	// Generation of skelton of attribute grammer
	for (l = next_outlink(units, NULL); l; l = next_outlink(units, l)) {
		AGmNode *id = l->arc->attr;
		AGmNode *unit = l->arc->dst;

		AGmString *s = new AGmString(64), *ss = NULL;
		s->Append(id->a.s.v);
		s->Append(":\n");

		AGmFile *ff = new AGmFile();
		ff->Create(s, AGM_FILE_STR_APPEND);
		DumpList(unit, 100, ff);
		delete ff;

		if (out_skelton) {
			s->Append("attr_grmr {\n");

			if (IsOmitUnits(unit)) {
			} else if (unit->H == str_value && unit->T == NULL) {
				s->Append("  $$ = $[1];\n");
			} else if (FlatUnitsStr(unit, units, ss)) {
				s->Append(ss); delete ss;
			} else if (BinOpUnitStr(unit, units, ss)) {
				s->Append(ss); delete ss;
			} else if (AsgnOpUnitStr(unit, ss)) {
				s->Append(ss); delete ss;
			} else if (CondOpUnitStr(unit, units, ss)) {
				s->Append(ss); delete ss;
			} else if (UnaryOpUnitStr(unit, ss)) {
				s->Append(ss); delete ss;
			} else if (ParaFlatUnitStr(id, unit, units, ss)) {
				s->Append(ss); delete ss;
			}

			s->Append("}\n");
		}

		s->Append("\n");
		f->WriteString(s);
		delete s;
	}

	Unref(depend, units, syn_ids);
	return NULL;
}
AGmNode* AGmParserGenerator::OutSyntaxUnit(AGmNode* &unit, AGmNode* id, AGmNode* list) {
	for (AGmNode *ls = list; ls; ls = ls->T) {

		if (!IsList(ls)) return String("syntax is not list.");
		if (!ls->H || !IsList(ls->H)) return String("syntax-list is not list.");
		if (!IsString(ls->HH)) return String("syntax-list-head is not string.");

		AGmNode *h = ls->HH;

		if (h == str_const || h == str_no_sp_const || h == str_reg_ref || h == str_eof) continue;
		if (h == str_not_const || h == str_no_sp_not_const) continue;	
		if (h == str_nonewline || h == str_not_nl || h == str_redirect) continue;	
		if (h == str_define || h == str_undef || h == str_ifdef || h == str_ifndef) continue;	

		if (h == str_regex || h == str_until || h == str_line )  {
			unit = Nconc(unit, Cons(str_value));
		} else if (h == str_repeat_0 || h == str_repeat_1 || h == str_braket_1 || h == str_braket_0) {
			AGmNode *unit2 = Ref(Cons(h));
			AGmNode *ret = OutSyntaxUnit(unit2, id, ls->HT);
			if (ret) { Unref(unit2); return ret; }
			unit = Nconc(unit, Cons(unit2));
			Unref(unit2);
		} else {
			CreateArc(id, h, depend);
			unit = Nconc(unit, Cons(Cons(h)));
		}
	}
	return NULL;
}

AGmNode *AGmParserGenerator::EliminateGrammarID(AGmNode* src_unit, AGmNode* id, AGmNode* unit) {
	int done = 0;
	AGmListItr* itr = new AGmListItr(src_unit);
	for (;;) {
		AGmNode *ls = itr->Next(id);
		if (ls == NULL) break;
		Set(ls->HT, Cons(unit));
		done = 1; break;
	}
	delete itr;
	if (!done) return String("id to be eliminated not found.");
	return NULL;
}

int AGmParserGenerator::IsOmitUnit(AGmNode* unit) {
	AGmNode *h = unit->H;
	if (h == str_braket_0 || h == str_braket_1 || h == str_repeat_0 || h == str_repeat_1) {
		AGmNode *ls;
		for (ls = unit->T; ls; ls = ls->T) {
			if (IsOmitUnit(ls->H)) continue;
			return 0;
		}
		return 1;
	} else {
		return 0;
	}
}
int AGmParserGenerator::IsOmitUnits(AGmNode* unit) {
	AGmNode *ls;
	for (ls = unit; ls; ls = ls->T) {
		if (IsOmitUnit(ls->H)) continue;
		return 0;
	}
	return 1;
}
int AGmParserGenerator::IsFlatUnit(AGmNode* unit, int check) {
	if (unit == str_value) return 1;
	if (!IsString(unit->H)) return 0;
	if (unit->T == NULL) return 1;
	if (check == 4) {
		if (unit->H != str_braket_1 && unit->H != str_braket_0 && unit->H != str_repeat_1 && unit->H != str_repeat_0) return 0;
	} else if (check == 2) {
		if (unit->H != str_braket_1 && unit->H != str_braket_0) return 0;
	} else {
		if (unit->H != str_braket_1) return 0;
	}
	AGmNode *ls;
	for (ls = unit->T; ls; ls = ls->T) {
		if (IsOmitUnit(ls->H)) continue;
		if (!IsList(ls->H) || !IsString(ls->HH) || ls->HT != NULL) return 0;
	}
	return 1;
}
int AGmParserGenerator::FlatUnitStr(AGmNode* unit, AGmNode* units, AGmString*& ss) {
	if (!IsString(unit->H)) return 0;
	if (unit->T == NULL) {
		ss = new AGmString("  $$ = ($[1]);\n");
		return 1;
	}
	if (unit->H != str_braket_1) return 0;
	ss = new AGmString("  switch ($[1]) {\n");
	AGmNode *ls;
	for (ls = unit->T; ls; ls = ls->T) {
		if (IsOmitUnit(ls->H)) continue;
		if (!IsList(ls->H) || !IsString(ls->HH) || ls->HT != NULL) { delete ss; return 0; }
		AGmNode *h = ls->HH;
		AGmNode *n = dst_node(units, h);
		if (n == NULL || n->H == NULL) {
			ss->Append("  case \""); ss->Append(h->a.s.v); ss->Append("\":\n");
			ss->Append("    $$ = (\""); ss->Append(h->a.s.v); ss->Append("\");\n");
		} else {
			ss->Append("  case \""); ss->Append(h->a.s.v); ss->Append("\":\n");
			ss->Append("    $$ = (\""); ss->Append(h->a.s.v); ss->Append("\", $[1]);\n");
		}
	}
	ss->Append("  }\n");
	return 1;
}
int AGmParserGenerator::FlatUnitsStr(AGmNode* unit, AGmNode* units, AGmString*& ss) {
	AGmNode *ls;
	for (ls = unit; ls; ls = ls->T) {
		if (IsOmitUnit(ls->H)) continue;
		break;
	}
	if (ls == NULL) return 0;
	if (FlatUnitStr(ls->H, units, ss) == 0) return 0;
	for (ls = ls->T; ls; ls = ls->T) {
		if (IsOmitUnit(ls->H)) continue;
		break;
	}
	if (ls) { delete ss; return 0; }
	return 1;
}
int AGmParserGenerator::BinOpUnitStr(AGmNode *unit, AGmNode* units, AGmString*& ss) {
	if (!IsList(unit->H) || !IsString(unit->HH)) return 0;
	if (FlatUnitStr(unit->H, units, ss) == 0) return 0;
	AGmNode *ls = unit->T;
	if (ls == NULL || !IsList(ls->H) || !IsString(ls->HH)) { delete ss; return 0; }
	AGmNode *h = ls->HH;
	if (h != str_braket_0 && h != str_repeat_0) { delete ss; return 0; }
	if (ls->T) { delete ss; return 0; }
	if (h == str_braket_0) {
		ss->Append("  if ($[2]) {\n");
	} else if (h == str_repeat_0) {
		ss->Append("  for ($[2]) {\n");
	}
	ls = ls->HT;
	if (!IsList(ls->H) || !IsString(ls->HH)) { delete ss; return 0; }
	h = ls->HH;
	if (ls->T) { delete ss; return 0; }
	if (h == str_braket_1 || h == str_braket_0 || h == str_repeat_1 || h == str_repeat_0) { delete ss; return 0; }
	AGmNode *op_str = h;

	if (ls->HT == NULL) { delete ss; return 0; }
	int idx = 1;
	AGmNode *op_unit = NULL; int op_idx = 0;
	AGmNode *exp_unit = NULL; int exp_idx = 0;
	for (ls = ls->HTH; ls; ls = ls->T) {
		if (!IsList(ls->H) || !IsString(ls->HH)) { delete ss; return 0; }
		h = ls->HH;
		if (ls->T == NULL) {
			if (IsFlatUnit(ls->H) == 0) { delete ss; return 0; }
			exp_unit = ls->H; exp_idx = idx++;
		} else if (IsOmitUnit(ls->H)) {
			idx++; continue;
		} else if (h != str_braket_1) {
			delete ss; return 0;
		} else {
			if (op_unit) { delete ss; return 0; }
			if (IsFlatUnit(ls->H) == 0) { delete ss; return 0; }
			op_unit = ls->H; op_idx = idx++;
		}
	}
	if (exp_unit == NULL) { delete ss; return 0; }
	if (op_unit) {
		ss->Append("    switch ($[2]["); ss->WriteInteger(op_idx); ss->Append("]) {\n");
		AGmNode *ls;
		for (ls = op_unit->T; ls; ls = ls->T) {
			if (IsOmitUnit(ls->H)) continue;
			if (!IsList(ls->H) || !IsString(ls->HH) || ls->HT != NULL) { delete ss; return 0; }
			AGmNode *h = ls->HH;
			ss->Append("    case \""); ss->Append(h->a.s.v); ss->Append("\":\n");
			if (exp_unit->H == str_braket_1) {
				ss->Append("      switch ($[2]["); ss->WriteInteger(exp_idx); ss->Append("]) {\n");
				AGmNode *ls2;
				for (ls2 = exp_unit->T; ls2; ls2 = ls2->T) {
					if (IsOmitUnit(ls2->H)) continue;
					if (!IsList(ls2->H) || !IsString(ls2->HH) || ls2->HT != NULL) { delete ss; return 0; }
					AGmNode *h2 = ls2->HH;
					ss->Append("      case \""); ss->Append(h2->a.s.v); ss->Append("\":\n");
					ss->Append("        $$ = (\""); ss->Append(h2->a.s.v); ss->Append("\", $$, $[2][");
					ss->WriteInteger(exp_idx); ss->Append("]);\n");
				}
				ss->Append("      }\n");
			} else {
				ss->Append("      $$ = (\""); ss->Append(h->a.s.v); ss->Append("\", $$, $[2][");
				ss->WriteInteger(exp_idx); ss->Append("]);\n");
			}
		}
		ss->Append("    }\n");
	} else {
		if (exp_unit->H == str_braket_1) {
			ss->Append("    switch ($[2]["); ss->WriteInteger(exp_idx); ss->Append("]) {\n");
			AGmNode *ls2;
			for (ls2 = exp_unit->T; ls2; ls2 = ls2->T) {
				if (IsOmitUnit(ls2->H)) continue;
				if (!IsList(ls2->H) || !IsString(ls2->HH) || ls2->HT != NULL) { delete ss; return 0; }
				AGmNode *h2 = ls2->HH;
				ss->Append("    case \""); ss->Append(h2->a.s.v); ss->Append("\":\n");
				ss->Append("      $$ = (\""); ss->Append(h2->a.s.v); ss->Append("\", $$, $[2][");
				ss->WriteInteger(exp_idx); ss->Append("]);\n");
			}
			ss->Append("    }\n");
		} else {
			ss->Append("    $$ = (\""); ss->Append(op_str->a.s.v); ss->Append("\", $$, $[2][");
			ss->WriteInteger(exp_idx); ss->Append("]);\n");
		}
	}
	ss->Append("  }\n");
	return 1;
}
int AGmParserGenerator::AsgnOpUnitStr(AGmNode *unit, AGmString*& ss) {
	if (!IsList(unit->H) || !IsString(unit->HH)) return 0;
	AGmNode *h = unit->HH;
	if (h != str_repeat_1) return 0;
	AGmNode *ls = unit->HT;
	if (ls == NULL || !IsList(ls->H) || !IsString(ls->HH)) return 0;
	h = ls->HH;
	if (ls->T) return 0;
	if (h == str_braket_1 || h == str_braket_0 || h == str_repeat_1 || h == str_repeat_0) return 0;
	AGmNode *op_str = h;

	AGmNode *exp1_unit = NULL; int exp1_idx = 0;
	AGmNode *op_unit = NULL; int op_idx = 0;
	if (ls->HT == NULL) {
		exp1_unit = ls->H;
	} else {
		int idx = 1;
		for (ls = ls->HTH; ls; ls = ls->T) {
			if (!IsList(ls->H) || !IsString(ls->HH)) return 0;
			h = ls->HH;
			if (IsOmitUnit(ls->H)) {
				idx++; continue;
			} else if (h != str_braket_1) {
				return 0;
			} else if (IsFlatUnit(ls->H)) {
				if (exp1_unit == NULL) {
					exp1_unit = ls->H; exp1_idx = idx++;
				} else if (op_unit == NULL) {
					op_unit = ls->H; op_idx = idx++;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		}
	}
	if (exp1_unit == 0) return 0;
	AGmNode *exp2_unit = unit->T;
	if (exp2_unit == NULL) return 0;

	if (exp2_unit->T == NULL) {
		ss = new AGmString("  $$ = ($[2]);\n");
	} else {
		if (exp2_unit->H != str_braket_1) return 0;
		ss = new AGmString("  switch ($[2]) {\n");
		AGmNode *ls;
		for (ls = exp2_unit->T; ls; ls = ls->T) {
			if (IsOmitUnit(ls->H)) continue;
			if (!IsList(ls->H) || !IsString(ls->HH) || ls->HT != NULL) { delete ss; return 0; }
			AGmNode *h = ls->HH;
			ss->Append("  case \""); ss->Append(h->a.s.v); ss->Append("\":\n");
			ss->Append("    $$ = (\""); ss->Append(h->a.s.v); ss->Append("\", $[1]);\n");
		}
		ss->Append("  }\n");
	}

	ss->Append("  inv_for ($[1]) {\n");
	if (op_idx == 0) {
		if (exp1_unit->T == NULL) {
			ss->Append("    $$ = (\""); ss->Append(op_str->a.s.v); ss->Append("\", $[1], $$);\n");
		} else {
			ss->Append("    $$ = (\""); ss->Append(op_str->a.s.v); ss->Append("\", $[1][1], $$);\n");
		}
	} else {
		ss->Append("    switch ($[1]["); ss->WriteInteger(op_idx); ss->Append("]) {\n");
		AGmNode *ls2;
		for (ls2 = op_unit->T; ls2; ls2 = ls2->T) {
			if (IsOmitUnit(ls2->H)) continue;
			if (!IsList(ls2->H) || !IsString(ls2->HH) || ls2->HT != NULL) { delete ss; return 0; }
			AGmNode *h2 = ls2->HH;
			ss->Append("    case \""); ss->Append(h2->a.s.v); ss->Append("\":\n");
			ss->Append("      $$ = (\""); ss->Append(h2->a.s.v); ss->Append("\", $[1][");
			ss->WriteInteger(exp1_idx); ss->Append("], $$);\n");
		}
		ss->Append("    }\n");
	}
	ss->Append("  }\n");
	return 1;
}
int AGmParserGenerator::CondOpUnitStr(AGmNode *unit, AGmNode* units, AGmString*& ss) {
	if (!IsList(unit->H) || !IsString(unit->HH)) return 0;
	if (FlatUnitStr(unit->H, units, ss) == 0) return 0;
	AGmNode *ls = unit->T;
	if (ls == NULL || !IsList(ls->H) || !IsString(ls->HH)) { delete ss; return 0; }
	AGmNode *h = ls->HH;
	if (h != str_braket_0) { delete ss; return 0; }
	if (ls->T) { delete ss; return 0; }
	if (h == str_braket_0) {
		ss->Append("  if ($[2]) {\n");
	}
	ls = ls->HT;
	if (!IsList(ls->H) || !IsString(ls->HH)) { delete ss; return 0; }
	h = ls->HH;
	if (ls->T) { delete ss; return 0; }
	if (h == str_braket_1 || h == str_braket_0 || h == str_repeat_1 || h == str_repeat_0) { delete ss; return 0; }
	AGmNode *op_str = h;

	if (ls->HT == NULL) { delete ss; return 0; }
	int idx = 1;
	AGmNode *exp1_unit = NULL; int exp1_idx = 0;
	AGmNode *exp2_unit = NULL; int exp2_idx = 0;
	for (ls = ls->HTH; ls; ls = ls->T) {
		if (IsOmitUnit(ls->H)) continue;
		if (!IsList(ls->H) || !IsString(ls->HH)) { delete ss; return 0; }
		h = ls->HH;
		if (IsOmitUnit(ls->H)) {
			idx++; continue;
		} else if (IsFlatUnit(ls->H)) {
			if (exp1_unit == NULL) {
				exp1_unit = ls->H; exp1_idx = idx++;	
			} else if (exp2_unit == NULL) {
				exp2_unit = ls->H; exp2_idx = idx++;
			} else {
				delete ss; return 0;
			}
		} else {
			delete ss; return 0;
		}
	}
	if (exp2_unit == NULL) { delete ss; return 0; }
	h = exp1_unit->H;
	if (h == str_braket_1 || h == str_braket_0 || h == str_repeat_1 || h == str_repeat_0) { delete ss; return 0; }
	ss->Append("    $$ = (\""); ss->Append(op_str->a.s.v); ss->Append("\", $$, $[2][");
	ss->WriteInteger(exp1_idx); ss->Append("], $[2]["); ss->WriteInteger(exp2_idx); ss->Append("]);\n");
	ss->Append("  }\n");
	return 1;
}
int AGmParserGenerator::UnaryOpUnitStr(AGmNode *unit, AGmString*& ss) {
	if (!IsList(unit->H) || !IsString(unit->HH) || !unit->T || unit->TT) return 0;
	AGmNode *op_unit = unit->H;
	AGmNode *h = op_unit->H;
	if (h != str_braket_0) return 0;
	if (IsFlatUnit(op_unit, 2) == 0) return 0;
	AGmNode *exp_unit = unit->TH;
	if (IsFlatUnit(exp_unit) == 0) return 0;

	ss = new AGmString("  if ($[1]) {\n");
	ss->Append("    switch ($[1]) {\n");
	AGmNode *ls;
	for (ls = op_unit->T; ls; ls = ls->T) {
		if (IsOmitUnit(ls->H)) continue;
		if (!IsList(ls->H) || !IsString(ls->HH) || ls->HT != NULL) { delete ss; return 0; }
		AGmNode *h = ls->HH;
		ss->Append("    case \""); ss->Append(h->a.s.v); ss->Append("\":\n");
		ss->Append("      $$ = (\""); ss->Append(h->a.s.v); ss->Append("\", $[2]);\n");
	}
	ss->Append("    }\n");
	ss->Append("  }\n");
	return 1;
}
AGmNode* AGmParserGenerator::MakeTree(AGmNode* unit, int idx0, AGmNode* units) {
	AGmNode *h = unit->H;
	AGmNode *ret = List(h, Integer(idx0));
	if (h == str_braket_1 || h == str_braket_0 || h == str_repeat_1 || h == str_repeat_0) {
		AGmNode *ls;
		for (ls = unit->T; ls; ls = ls->T) {
			if (IsOmitUnit(ls->H)) continue;
			AGmNode *ret2 = MakeTree(ls->H, -1, unit);
			CreateArc(ret, ret2);
		}
	} else {
		AGmNode *n = dst_node(units, h);
		if (n == NULL || n->H == NULL) {
			AGmNode *ls; int idx = 1;
			if (unit->T) {
				for (ls = unit->TH; ls; ls = ls->T) {
					if (IsOmitUnit(ls->H)) { idx++; continue; }
					AGmNode *ret2 = MakeTree(ls->H, idx++, unit);
					CreateArc(ret, ret2);
				}
			}
		}
	}
	return ret;
}
int AGmParserGenerator::ParaFlatUnitStr(AGmNode* id, AGmNode* unit, AGmNode* units, AGmString*& ss) {
	AGmNode *set = Ref(Cons(NULL));
	AGmNode *ls, *tree; int idx = 1;
	for (ls = unit; ls; ls = ls->T) {
		if (IsOmitUnit(ls->H)) { idx++; continue; }
		if (IsFlatUnit(ls->H, 2)) {
			tree = NULL;
		} else {
			tree = MakeTree(ls->H, idx, units);
		}
		CreateArc(set, List(tree, ls->H, Integer(idx++)));
	}
	AGmLink *l; int first = 1;
	ss = new AGmString(256);
	for (l = next_outlink(set, NULL); l; l = next_outlink(set, l)) {
		tree = l->arc->dst->H;
		if (tree == NULL) {
			ls = l->arc->dst->TH;
			idx = (long)l->arc->dst->TTHI;
			if (ls->H == str_braket_1) {
				ss->Append("  switch ($["); ss->WriteInteger(idx); ss->Append("]) {\n");
				AGmNode *ls2;
				for (ls2 = ls->T; ls2; ls2 = ls2->T) {
					if (!IsList(ls2->H) || !IsString(ls2->HH)) { Unref(set); delete ss; return 0; }
					AGmNode *h2 = ls2->HH;
					ss->Append("  case \""); ss->Append(h2->a.s.v); ss->Append("\":\n");
					AGmNode *n = dst_node(units, h2);
					if (n && n->H) {
						ss->Append("    $1 = (\""); ss->Append(h2->a.s.v); ss->Append("\", $[");
						ss->WriteInteger(idx); ss->Append("]);\n");
					} else {
						ss->Append("    $1 = (\""); ss->Append(h2->a.s.v); ss->Append("\");\n");
					}
				}
				ss->Append("  }\n");
			} else if (ls->H == str_braket_0) {
				ss->Append("  if ($["); ss->WriteInteger(idx); ss->Append("]) {\n");
				ss->Append("    switch ($["); ss->WriteInteger(idx); ss->Append("]) {\n");
				AGmNode *ls2;
				for (ls2 = ls->T; ls2; ls2 = ls2->T) {
					if (!IsList(ls2->H) || !IsString(ls2->HH)) { Unref(set); delete ss; return 0; }
					AGmNode *h2 = ls2->HH;
					ss->Append("    case \""); ss->Append(h2->a.s.v); ss->Append("\":\n");
					AGmNode *n = dst_node(units, h2);
					if (n && n->H) {
						ss->Append("      $1 = (\""); ss->Append(h2->a.s.v); ss->Append("\", $[");
						ss->WriteInteger(idx); ss->Append("]);\n");
					} else {
						ss->Append("      $1 = (\""); ss->Append(h2->a.s.v); ss->Append("\");\n");
					}
				}
				ss->Append("    }\n");
				ss->Append("  } else {\n");
				ss->Append("    $1 = null;\n");
				ss->Append("  }\n");
			} else if (ls->H == str_repeat_1 || ls->H == str_repeat_0) {
				ss->Append("  $1 = null;\n");
				ss->Append("  for ($["); ss->WriteInteger(idx); ss->Append("]) {\n");
				ss->Append("    switch ($["); ss->WriteInteger(idx); ss->Append("]) {\n");
				AGmNode *ls2;
				for (ls2 = ls->T; ls2; ls2 = ls2->T) {
					if (!IsList(ls2->H) || !IsString(ls2->HH)) { Unref(set); delete ss; return 0; }
					AGmNode *h2 = ls2->HH;
					ss->Append("    case \""); ss->Append(h2->a.s.v); ss->Append("\":\n");
					AGmNode *n = dst_node(units, h2);
					if (n && n->H) {
						ss->Append("      $1 = append($1, ((\""); ss->Append(h2->a.s.v); ss->Append("\", $[");
						ss->WriteInteger(idx); ss->Append("])));\n");
					} else {
						ss->Append("      $1 = append($1, ((\""); ss->Append(h2->a.s.v); ss->Append("\")));\n");
					}
				}
				ss->Append("    }\n");
				ss->Append("  }\n");
			} else {
				ss->Append("  $1 = ($["); ss->WriteInteger(idx); ss->Append("]);\n");
			}
			if (first) {
				first = 0;
				ss->Append("  $$ = (\""); ss->Append(id->a.s.v); ss->Append("\", $1);\n");
			} else {
				ss->Append("  $$ = append($$, (($1)));\n");
			}
		} else {
			int idxs[256];
			RecursiveParaFlatUnitStr(idxs, 0, 1, 1, tree, 0, 0, units, ss);
			ss->Append("  $$ = append($$, (($1)));\n");
		}
	}
	Unref(set);
	return 1;
}
void AGmParserGenerator::RecursiveParaFlatUnitStr(
		int idxs[], int depth, int indent, int var, AGmNode* tree, int loop, int sw, AGmNode* units, AGmString*& ss) {

	int depth2 = depth, indent2 = indent, var2 = var, loop2 = loop, sw2 = sw;

	if (sw) {
		Indent(indent, ss); ss->Append("case \""); ss->Append(tree->H->a.s.v); ss->Append("\":\n");
		indent2++;
		sw2 = 0;
	}

	int n = out_count(tree);

	int m = ctrl_count(tree);
	if (m > 1) {
		var2++; loop2 = 0;
	}

	idxs[depth] = (long)tree->THI;

	if (tree->H == str_braket_1) {
		Indent(indent2, ss); ss->Append("switch ($"); Index(idxs, depth + 1, ss); ss->Append(") {\n"); sw2 = 1;
		depth2++;

	} else if (tree->H == str_braket_0) {
		Indent(indent2, ss); ss->Append("if ($"); Index(idxs, depth + 1, ss); ss->Append(") {\n");
		depth2++;
		indent2++;
		if (n > 1) {
			Indent(indent2, ss); ss->Append("switch ($"); Index(idxs, depth + 1, ss); ss->Append(") {\n"); sw2 = 1;
		}
	} else if (tree->H == str_repeat_1 || tree->H == str_repeat_0) {
		Indent(indent2, ss); Var(var, ss); ss->Append(" = null;\n");
		Indent(indent2, ss); ss->Append("for ($"); Index(idxs, depth + 1, ss); ss->Append(") {\n"); loop2 = 1;
		depth2++;
		indent2++;
		if (n > 1) {
			Indent(indent2, ss); ss->Append("switch ($"); Index(idxs, depth + 1, ss); ss->Append(") {\n"); sw2 = 1;
		}
	}

	int done = 0;
	AGmLink *l;
	for (l = next_outlink(tree, NULL); l; l = next_outlink(tree, l)) {
		AGmNode *tree2 = l->arc->dst;
		RecursiveParaFlatUnitStr(idxs, depth2, indent2, var2, tree2, loop2, sw2, units, ss);
		if (m > 1) {
			Indent(indent2, ss);
			Var(var, ss); ss->Append(" = append(");
			Var(var, ss); ss->Append(", ((");
			Var(var2, ss); ss->Append(")));\n");
		}
		done = 1;
	}

	if (tree->H == str_braket_1) {
		Indent(indent, ss); ss->Append("}\n");

	} else if (tree->H == str_braket_0) {
		if (n > 1) {
			Indent(indent+1, ss); ss->Append("}\n");
		}
		Indent(indent, ss); ss->Append("} else {\n");
		if (loop) {
			Indent(indent+1, ss); Var(var, ss); ss->Append(" = append("); Var(var, ss); ss->Append(", ((null)));\n");
		} else {
			Indent(indent+1, ss); Var(var, ss); ss->Append(" = null;\n");
		}
		Indent(indent, ss); ss->Append("}\n");

	} else if (tree->H == str_repeat_1 || tree->H == str_repeat_0) {
		if (n > 1) {
			Indent(indent+1, ss); ss->Append("}\n");
		}
		Indent(indent, ss); ss->Append("}\n");
	} else {
		AGmNode *n = dst_node(units, tree->H);
		if (n && n->H) {
			if (loop) {
				if (done == 0) {
					if (sw) {
						Indent(indent+1, ss);
						Var(var, ss); ss->Append(" = append(");
						Var(var, ss); ss->Append(", ((\""); ss->Append(tree->H->a.s.v);
						ss->Append("\", $"); Index(idxs, depth + 1, ss); ss->Append(")));\n");
					} else {
						Indent(indent, ss);
						Var(var, ss); ss->Append(" = append(");
						Var(var, ss); ss->Append(", (($");
						Index(idxs, depth + 1, ss); ss->Append(")));\n");
					}
				}		
			} else {
				if (done == 0) {
					if (sw) {
						Indent(indent+1, ss);
						n = dst_node(units, tree->H);
						if (n == NULL || n->H == NULL) {
							Var(var, ss); ss->Append(" = (\""); ss->Append(tree->H->a.s.v); ss->Append("\");\n");
						} else {
							Var(var, ss); ss->Append(" = (\""); ss->Append(tree->H->a.s.v);
							ss->Append("\", $"); Index(idxs, depth + 1, ss); ss->Append(");\n");
						}
					} else {
						Indent(indent, ss);
						Var(var, ss); ss->Append(" = ($"); Index(idxs, depth + 1, ss); ss->Append(");\n");
					}
				}		
			}
		} else {
			if (done == 0) {
				if (sw) {
					Indent(indent+1, ss);
					n = dst_node(units, tree->H);
					if (n == NULL || n->H == NULL) {
						Var(var, ss); ss->Append(" = (\""); ss->Append(tree->H->a.s.v); ss->Append("\");\n");
					} else {
						Var(var, ss); ss->Append(" = (\""); ss->Append(tree->H->a.s.v);
						ss->Append("\", $"); Index(idxs, depth + 1, ss); ss->Append(");\n");
					}
				} else {
					Indent(indent, ss);
					Var(var, ss); ss->Append(" = $"); Index(idxs, depth + 1, ss); ss->Append(";\n");
				}
			}
		}
	}
}
void AGmParserGenerator::Indent(int indent, AGmString*& ss) {
	int i;
	for (i = 0; i < indent; i++) {
		ss->Append("  ");
	}
}
void AGmParserGenerator::Index(int idxs[], int depth, AGmString*& ss) {
	int i;
	for (i = 0; i < depth; i++) {
		if (idxs[i] < 0) continue;
		ss->Append("["); ss->WriteInteger(idxs[i]); ss->Append("]");
	}
}
void AGmParserGenerator::Var(int var, AGmString*& ss) {
	ss->Append("$"); ss->WriteInteger(var);
}
int AGmParserGenerator::ctrl_count(AGmNode* tree) {
	AGmLink *l; int count = 0;
	for (l = next_outlink(tree, NULL); l; l = next_outlink(tree, l)) {
		AGmNode *tree2 = l->arc->dst;
		AGmNode *h = tree2->H;
		if (h == str_braket_1 || h == str_braket_0 || h == str_repeat_1 || h == str_repeat_0) count++;
	}
	return count;
}
