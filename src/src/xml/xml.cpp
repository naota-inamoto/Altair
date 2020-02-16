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
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <dev/advwnd.h>

#include <xml/xml.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

class AUiXmlShell : AGm {
public:
	AUiXmlShell(AIpProcess *process, AGmFile *file, AGmNode*& tree, AGmNode *opt);
	~AUiXmlShell();

	int Parse();
	int Generate();

protected:
	int body_scanf(AGmNode*& body);
	int tag_scanf(AGmNode*& tag, AGmNode*& tag_id);
	int param_scanf(AGmNode *tag);
	int body_printf(AGmNode* body);
	int tag_printf(AGmNode *tag_id, AGmNode *tag);
	int indent_printf();

protected:
	AGmFile *f;
	AGmNode **_tree;
	AGmNode *_opt;
	int check;
	int indent;
	int nest;
	int first;

	AIpProcess *process;
	AGmNode *obj;
	AGmNode *startElementFuncDcl;
	AGmNode *endElementFuncDcl;
	AGmNode *charactersFuncDcl;

	AGmNode *ns_stack;

	AGmNode *_xml_comment;
	AGmNode *_xml_body;
	AGmNode *_xml_text;

public:
	int no_indent;
};

AUiXmlShell::AUiXmlShell(AIpProcess *process, AGmFile *file, AGmNode*& tree, AGmNode *opt) {
	this->process = process;
	f = file;
	_tree = &tree;
	_opt = opt;
	check = 0;
	indent = 0;
	obj = NULL;
	no_indent = 0;
	ns_stack = Ref(Cons(NULL));
	_xml_comment = Ref(UniqueStr("!comment"));
	_xml_body = Ref(UniqueStr("!body"));
	_xml_text = Ref(UniqueStr("!text"));
}
AUiXmlShell::~AUiXmlShell() {
	Unref(ns_stack);
	Unref(_xml_comment, _xml_body, _xml_text);
}

// parse
int AUiXmlShell::Parse() {
	if (*_tree) {
		AGmNode *tail;
		if (Param(*_tree, obj, tail) == 0 && obj
				&& Param(tail, startElementFuncDcl, tail) == 0
				&& Param(tail, endElementFuncDcl, tail) == 0
				&& Param(tail, charactersFuncDcl, tail) == 0) {
			*_tree = NULL;
		} else {
			*_tree = NULL;
			return 200;
		}
	}
	if (obj == NULL && _opt && IsInteger(_opt) && (_opt->a.i.v & 0x01))
		check = 1;
	AGmNode *tree;
	int err = body_scanf(tree);
	if (err) return err;
	if (obj == NULL) *_tree = tree;
	return 0;
}
int AUiXmlShell::body_scanf(AGmNode*& body) {
	body = NULL;
	char c, c2;
	for (;;) {
		f->ReadSpace();
#if	_WINDOWS && _64BIT
		long long pos = f->CurrentPoint();
#else
		long pos = f->CurrentPoint();
#endif
		c = f->Input();
		c2 = f->Input();
		// if ("</" or EOF) break;
		if (c == EOF) { f->Back(pos); break; }
		if (c == '<') {
			if (c2 == '/') {
				f->Back(pos); break;
			}
			// if ("<?") { read until ">" and continue; }
			if (c2 == '?') {
				AGmString *s = NULL;
				if (obj == NULL) s = new AGmString("?");
				c = f->Input();
				for (;;) {
#ifdef	_WINDOWS
					if (isFirstChar(c)) {
						if (obj == NULL) s->Append(c);
						c = f->Input();
						if (obj == NULL) s->Append(c);
					} else {
						if (obj == NULL) s->Append(c);
					}
#endif
#ifdef	_LINUX
					int i2;
					f->Unput();
					if (isUTF8(f, i2)) {
						if (obj == NULL) s->Append((const char*)utf8);
					} else {
						c = f->Input();
						if (obj == NULL) s->Append(c);
					}
#endif
					c = f->Input();
					if (c == EOF) {
						if (obj == NULL) { delete s; f->Back(pos); }
						return 100;
					}
					if (c == '>') break;
				}
				if (obj == NULL) {
					if (body == NULL) body = Cons(Integer(1));
					CreateArc(body, String(s), _xml_comment);
				}

				if (check) {
					ADvWindow::Print("comment: ");
					ADvWindow::Print(s->Str());
					ADvWindow::Print("\n");
				}
				continue;
			}
			// if ("<!") { read until ">" and continue; }
			if (c2 == '!') {
				int count = 0;
				AGmString *s = NULL;
				if (obj == NULL) s = new AGmString("!");
				c = f->Input();
				for (;;) {
#ifdef	_WINDOWS
					if (isFirstChar(c)) {
						if (obj == NULL) s->Append(c);
						c = f->Input();
						if (obj == NULL) s->Append(c);
					} else {
						if (obj == NULL) s->Append(c);
						if (c == '<') {
							count++;
						} else if (c == '>') {
							count--;
						}
					}
#endif
#ifdef	_LINUX
					int i2;
					f->Unput();
					if (isUTF8(f, i2)) {
						if (obj == NULL) s->Append((const char*)utf8);
					} else {
						c = f->Input();
						if (obj == NULL) s->Append(c);
						if (c == '<') {
							count++;
						} else if (c == '>') {
							count--;
						}
					}
#endif
					c = f->Input();
					if (c == EOF) {
						if (obj == NULL) { delete s; f->Back(pos); }
						return 100;
					}
					if (count == 0 && c == '>') break;
				}
				if (obj == NULL) {
					if (body == NULL) body = Cons(Integer(1));
					CreateArc(body, String(s), _xml_comment);
				}
				if (check) {
					ADvWindow::Print("comment: ");
					ADvWindow::Print(s->Str());
					ADvWindow::Print("\n");
				}
				continue;
			}
		}
		f->Unput(2);
		// if (tag) { if err return err; else continue; }
		AGmNode *tag, *tag_id;
		int err = tag_scanf(tag, tag_id);
		if (err >= 100) return err;
		if (err == 0) {
			if (obj == NULL) {
				if (body == NULL) body = Cons(Integer(1));
				CreateArc(body, tag, tag_id);
				Unref(tag, tag_id);
			}
			continue;
		}
		// read until just before "<" or EOF
		AGmString *s = new AGmString(16);
		for (;;) {
			c = f->Input();
			if (c == '<' || c == EOF) { f->Unput(); break; }
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				s->Append(c);
				s->Append(f->Input());
			} else {
				s->Append(c);
			}
#endif
#ifdef	_LINUX
			int i2;
			f->Unput();
			if (isUTF8(f, i2)) {
				s->Append((const char*)utf8);
			} else {
				c = f->Input();
				s->Append(c);
			}
#endif
		}
		int len = s->Length();
		int i;
		for (i = len-1; i > 0; i--) {
			c = s->Component(i);
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;
			break;
		}
		if (i != len-1) {
			AGmString *ss = s->SubString(0, i+1);
			delete s;
			s = ss;
		}
		AGmString *s2 = s->XmlDecode(3);
		delete s; s = s2;
		if (obj == NULL) {
			if (body == NULL) body = Cons(Integer(1));
			CreateArc(body, String(s2), _xml_text);
		} else {
			AGmNode *args = Ref(Cons(String(s)));	
			AGmNode *err = process->CallListener(
				obj, charactersFuncDcl, args);
			Unref(args);
			if (err) { f->Back(pos); return 303; }
		}
		if (check) {
			ADvWindow::Print("text: ");
			ADvWindow::Print(s->Str());
			ADvWindow::Print("\n");
		}
	}
	return 0;
}
int AUiXmlShell::tag_scanf(AGmNode*& tag, AGmNode*& tag_id) {
	char c, c2;
#if	_WINDOWS && _64BIT
	long long pos = f->CurrentPoint();
#else
	long pos = f->CurrentPoint();
#endif
	c = f->Input();
	c2 = f->Input();
	// if ("</") return 101;
	if (c == '<') {
		if (c2 == '/') {
			f->Back(pos); return 101;
		}
	} else {
		// if (not "<") return 1;
		f->Back(pos); return 1;
	}
	c = c2;
	// if (not tag_id) return 102;
	AGmString *s = new AGmString(16);
	for (;;) {
		if (c == EOF || c == '<' || c == '>') {
			delete s; f->Back(pos); return 102;
		} else {
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				s->Append(c);
				s->Append(f->Input());
			} else {
				s->Append(c);
			}
#endif
#ifdef	_LINUX
			int i2;
			f->Unput();
			if (isUTF8(f, i2)) {
				s->Append((const char*)utf8);
			} else {
				c = f->Input();
				s->Append(c);
			}
#endif
			break;
		}
	}
	for (;;) {
		c = f->Input();
		if (c == EOF || c == '/' || c == '>'
				|| c == ' ' || c == '\t' || c == '\r' || c == '\n') {
			f->Unput(); break;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			s->Append(f->Input());
		} else {
			s->Append(c);
		}
#endif
#ifdef	_LINUX
		int i2;
		f->Unput();
		if (isUTF8(f, i2)) {
			s->Append((const char*)utf8);
		} else {
			c = f->Input();
			s->Append(c);
		}
#endif
	}

	if (check) {
		ADvWindow::Print("tag_id: ");
		ADvWindow::Print(s->Str());
		ADvWindow::Print("\n");
	}
	// <namespace>
	AGmString *ss2;
	int ns_idx = s->Search(":", 0), new_ns = 0;
	AGmNode *ns_name = NULL, *ns_url = NULL;
	if (ns_idx >= 0) {
		// name space name found
		ns_name = Ref(String(s->SubString(0, ns_idx)));
		AGmString* s2 = s->SubString(ns_idx + 1, s->Length());
		ss2 = s; s = s2;
		ns_url = dst_node(ns_stack, ns_name->a.s.v->Str());
		if (ns_url == NULL) {
			// push namespace name to ns_stack
			if (ns_stack->body == NULL) {
				AGmNodeBody *nb = ns_stack->body = (AGmNodeBody*)Alloc();
				nb->type = AGM_BODY_OF_NODE;
				nb->flag = 0;
				nb->nid = 0;
				nb->in = nb->out = NULL;
			}
			if (ns_stack->body->out == NULL) {
				AGmLink *l = ns_stack->body->out = (AGmLink*)Alloc();
				l->type = AGM_DMY_OUT_LINK;
				l->flag = 0;
				l->next = l->prev = l;
			}
			new_ns = 1;
			CreateArc(ns_stack, ns_url = Cons(NULL), ns_name, ns_stack->body->out->next);
		}
	} else {
		ss2 = new AGmString(16);
		ss2->Copy(s);
	}
	tag_id = Ref(String(s));
	if (ns_url) {
		// add namespace info
		CreateArc(tag_id, ns_url, ns_name);
	}
	Unref(ns_name);
	// </namespace>

	tag = Ref(Cons(Integer(2)));
	// if (not param) return 103;
	if (param_scanf(tag)) {
		Unref(ns_name);
		delete ss2; Unref(tag_id, tag); f->Back(pos); return 103;
	}

	c = f->Input();
	c2 = f->Input();
	// if ("/>") return 0;
	if (c == '/' && c2 == '>') {
		if (obj) {
			AGmNode *args = Ref(List(tag_id, tag));
			AGmNode *err = process->CallListener(
				obj, startElementFuncDcl, args);
			Unref(args);
			if (err) { delete ss2; Unref(tag_id, tag); f->Back(pos); return 301; }
			args = Ref(Cons(tag_id));
			err = process->CallListener(
				obj, endElementFuncDcl, args);
			Unref(args);
			if (err) { delete ss2; Unref(tag_id, tag); f->Back(pos); return 302; }
			Unref(tag_id, tag);
		}
		if (new_ns) {
			// pop namespace name from ns_stack
			AGmLink *l = next_outlink(ns_stack, NULL);
			DeleteArc(l->arc);
		}
		delete ss2; f->ReadSpace(); return 0;
	}
	// if (not ">") return 104;
	if (c != '>') {
		delete ss2; Unref(tag_id, tag); f->Back(pos); return 104;
	}
	if (obj) {
		AGmNode *args = Ref(List(tag_id, tag));
		AGmNode *err = process->CallListener(
			obj, startElementFuncDcl, args);
		Unref(args);
		if (err) { delete ss2; Unref(tag_id, tag); f->Back(pos); return 301; }
	}
	f->Unput();

	// body
	AGmNode *body;
	int err = body_scanf(body);
	if (err) { delete ss2; Unref(tag_id, tag); f->Back(pos); return err; }
	if (obj == NULL) {
		if (body) CreateArc(tag, body, _xml_body);
	}
	// if (not "</") return 105;
	c = f->Input();
	c2 = f->Input();
	if (c != '<' || c2 != '/') {
		delete ss2; Unref(tag_id, tag); f->Back(pos); return 105;
	}
	// if (not tag_id) return 106;
	if (f->_MatchString(ss2->Str()) == 0) {
		if (check) {
			char ss[256];
			art_sprintf_1(ss, 256, "</%s> expected.\n", ss2->Str());
			ADvWindow::Print(ss);
		}
		delete ss2; Unref(tag_id, tag); f->Back(pos); return 106;
	}
	// if (not ">") return 107;
	c = f->Input();
	if (c != '>') {
		delete ss2; Unref(tag_id, tag); f->Back(pos); return 107;
	}
	if (check) {
		ADvWindow::Print("end_tag_id: ");
		ADvWindow::Print(ss2->Str());
		ADvWindow::Print("\n");
	}
	delete ss2; 
	if (obj) {
		AGmNode *args = Ref(Cons(tag_id));
		AGmNode *err = process->CallListener(
			obj, endElementFuncDcl, args);
		Unref(args);
		if (err) { Unref(tag_id, tag); f->Back(pos); return 302; }
	}
	if (obj) Unref(tag_id, tag);
	if (new_ns) {
		// pop namespace name from ns_stack
		AGmLink *l = next_outlink(ns_stack, NULL);
		DeleteArc(l->arc);
	}
	f->ReadSpace(); return 0;
}
int AUiXmlShell::param_scanf(AGmNode *tag) {
	int new_ns = 0;
	char c;
	for (;;) {
		f->ReadSpace();
#if	_WINDOWS && _64BIT
		long long pos = f->CurrentPoint();
#else
		long pos = f->CurrentPoint();
#endif
		// if (not param_id) return 0;
		AGmString *s = NULL;
		int brk = 0;
		for (;;) {
			c = f->Input();
			if (c == EOF || c == '<' || c == '/' || c == '>' || c == '=') {
				f->Back(pos); brk = 1; break;
			} else {
				s = new AGmString(16);
#ifdef	_WINDOWS
				if (isFirstChar(c)) {
					s->Append(c);
					s->Append(f->Input());
				} else {
					s->Append(c);
				}
#endif
#ifdef	_LINUX
				int i2;
				f->Unput();
				if (isUTF8(f, i2)) {
					s->Append((const char*)utf8);
				} else {
					c = f->Input();
					s->Append(c);
				}
#endif
				break;
			}
		}
		if (brk) break;
		for (;;) {
			c = f->Input();
			if (c == EOF || c == '<' || c == '/' || c == '>' || c == '='
					|| c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				f->Unput(); break;
			}
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				s->Append(c);
				s->Append(f->Input());
			} else {
				s->Append(c);
			}
#endif
#ifdef	_LINUX
			int i2;
			f->Unput();
			if (isUTF8(f, i2)) {
				s->Append((const char*)utf8);
			} else {
				c = f->Input();
				s->Append(c);
			}
#endif
		}
		if (check) {
			ADvWindow::Print("param_id: ");
			ADvWindow::Print(s->Str());
			ADvWindow::Print("\n");
		}
		AGmNode *param_id;
		// <namespace>
		int ns_idx = s->Search(":", 0), xmlns = 0;
		AGmNode *ns_name = NULL, *ns_url = NULL;
		if (ns_idx >= 0) {
			// name space name found
			ns_name = Ref(String(s->SubString(0, ns_idx)));
			AGmString* s2 = s->SubString(ns_idx + 1, s->Length());
			delete s; s = s2;
			xmlns = (ns_name->a.s.v->Compare("xmlns") == 0);
			ns_url = dst_node(ns_stack, ns_name->a.s.v->Str());
			if (ns_url == NULL) {
				// push namespace name to ns_stack
				if (ns_stack->body == NULL) {
					AGmNodeBody *nb = ns_stack->body = (AGmNodeBody*)Alloc();
					nb->type = AGM_BODY_OF_NODE;
					nb->flag = 0;
					nb->nid = 0;
					nb->in = nb->out = NULL;
				}
				if (ns_stack->body->out == NULL) {
					AGmLink *l = ns_stack->body->out = (AGmLink*)Alloc();
					l->type = AGM_DMY_OUT_LINK;
					l->flag = 0;
					l->next = l->prev = l;
				}
				new_ns++;
				CreateArc(ns_stack, ns_url = Cons(NULL), ns_name, ns_stack->body->out->next);
			}
		}
		param_id = Ref(String(s));
		if (ns_url) {
			// add namespace info
			CreateArc(param_id, ns_url, ns_name);
		}
		Unref(ns_name);
		// </namespace>

		int cont = 0;
		AGmNode *param_value = NULL;
		for (;;) {
			// if (not "=") no value
			if (f->MatchString("=") == 0) {
				param_value = Ref(Cons(NULL));
				cont = 1; break;
			}
			// if (not value) return 1;
			AGmString *sv; double rv;
#if	_WINDOWS && _64BIT
			long long iv;
#else
			long iv;
#endif
			if (f->ReadQString(sv, 0) == 0) {
				AGmString *sv2 = sv->XmlDecode(3);
				param_value = Ref(Cons(String(sv2)));
				delete sv;
				if (check) {
					ADvWindow::Print("param_value: ");
					ADvWindow::Print(sv2->Str());
					ADvWindow::Print("\n");
				}
				cont = 1; break;
			} else if (f->ReadReal(rv) == 0) {
				param_value = Ref(Cons(Real(rv)));
				if (check) {
					char s[64];
					art_sprintf_1(s, 64, "%.16g", rv);
					ADvWindow::Print("param_value: ");
					ADvWindow::Print(s);
					ADvWindow::Print("\n");
				}
				cont = 1; break;
			} else if (f->ReadInteger(iv) == 0) {
				param_value = Ref(Cons(Integer(iv)));
				if (check) {
					char s[64];
					art_sprintf_1(s, 64, "%ld", iv);
					ADvWindow::Print("param_value: ");
					ADvWindow::Print(s);
					ADvWindow::Print("\n");
				}
				cont = 1; break;
			} else {
				break;
			}
		}
		if (cont == 0) {
			Unref(param_id); f->Back(pos); return 1;
		}
		if (xmlns) {
			ns_url = dst_node(ns_stack, param_id->a.s.v->Str());
			if (ns_url == NULL) {
				new_ns++;
				CreateArc(ns_stack, ns_url = Cons(NULL), param_id, ns_stack->body->out->next);
			}
			if (ns_url->H) {
				if (Compare(ns_url->H, param_value) != 0) {
					Unref(param_id, param_value); f->Back(pos); return 1;
				}
			} else {
				Set(ns_url->H, param_value);
			}
		}
		CreateArc(tag, param_value, param_id);
		Unref(param_id, param_value);	
	}
	for (; new_ns; --new_ns) {
		// pop namespace name from ns_stack
		AGmLink *l = next_outlink(ns_stack, NULL);
		DeleteArc(l->arc);
	}
	return 0;
}

// generate
int AUiXmlShell::Generate() {
	AGmNode *tree = *_tree;
	if (tree == NULL || !IsList(tree) || !IsInteger(tree->H)) return 1;
	if (_opt && IsList(_opt) && IsInteger(_opt->H)	&& (_opt->HI & 0x02)) no_indent = 1;
	first = 1;
	int err = 1;
	if (tree->HI == 1) {
		err = body_printf(tree);
	}
	if (err == 0 && no_indent == 0) f->WriteString("\n");
	return err;
}
int AUiXmlShell::body_printf(AGmNode* body) {
	AGmLink *l; AGmNode *attr, *dst; int err;
	int text_first = 1;
	for (l = next_outlink(body, NULL);
			l; l = next_outlink(body, l)) {
		attr = l->arc->attr; dst = l->arc->dst;
		if (IsString(attr)) {
			if (attr->a.s.v->Compare("!comment") == 0) {
				if (!IsString(dst)) return 10;
				indent_printf();
				f->WriteString("<");
				f->WriteString(dst->a.s.v->Str());
				f->WriteString(">");
				nest = 1;
				text_first = 0;
			} else if (attr->a.s.v->Compare("!text") == 0) {
				if (!IsString(dst)) return 10;
				if (text_first == 0)
					indent_printf();
				AGmString *str = dst->a.s.v->XmlEncode(1);
				f->WriteString(str->Str());
				delete str;
			} else {
				if (!IsList(dst) || !IsInteger(dst->H) || dst->HI != 2) return 11;
				err = tag_printf(attr, dst);
				if (err) return err;
				nest = 1;
				text_first = 0;
			}
		}
	}
	return 0;
}
int AUiXmlShell::tag_printf(AGmNode *tag_id, AGmNode *tag) {
	AGmLink *l; AGmNode *attr, *dst; AGmString *attr_str; int err;
	indent_printf();

	// <namespace>
	AGmString *tag_id_str = new AGmString(16);
	AGmLink *l2 = next_outlink(tag_id, NULL);
	if (l2 && IsString(l2->arc->attr)) {
		tag_id_str->Append(l2->arc->attr->a.s.v->Str());
		tag_id_str->Append(':');
	}
	tag_id_str->Append(tag_id->a.s.v->Str());
	// </namespace>

	f->WriteString("<");
	f->WriteString(tag_id_str);
	for (l = next_outlink(tag, NULL);
			l; l = next_outlink(tag, l)) {
		attr = l->arc->attr; dst = l->arc->dst;
		if (IsString(attr)) {
			attr_str = attr->a.s.v;
			if (attr_str->Compare("!body") == 0) continue;
			if (!IsList(dst)) { delete tag_id_str; return 21; }
			AGmNode *param_value = dst->H;
			f->WriteString(" ");

			// <namespace>
			AGmLink *l2 = next_outlink(attr, NULL);
			if (l2 && IsString(l2->arc->attr)) {
				f->WriteString(l2->arc->attr->a.s.v->Str());
				f->WriteString(":");
			}
			// </namespace>

			f->WriteString(attr_str);
			if (IsString(param_value)) {
				f->WriteString("=");
				AGmString *str = param_value->a.s.v->XmlEncode(3);
				f->WriteQString(str->Str(), 0);
				delete str;
			} else if (IsReal(param_value)) {
				f->WriteString("=");
				f->WriteReal(param_value->a.r.v);
			} else if (IsInteger(param_value)) {
				f->WriteString("=");
				f->WriteInteger((long)param_value->a.i.v);
			}
		}
	}
	nest = 0;
	dst = dst_node(tag, "!body");
	if (dst) {
		f->WriteString(">");
		if (!IsList(dst) || !IsInteger(dst->H) || dst->HI != 1) {
			delete tag_id_str; return 23;
		}
		AGmFile *prev_f = f;	
		f = new AGmFile;
		AGmString *ss = new AGmString(16);
		f->Create(ss, AGM_FILE_STR_WRITE);
		nest = 0;
		indent++;
		err = body_printf(dst);
		indent--;
		delete f;
		f = prev_f;
		f->WriteString(ss->Str());
		if (nest) {
			indent_printf();
		}
		delete ss;
		if (err) { delete tag_id_str; return err; }

		AGmNode *n = NULL;
		if (_opt && IsList(_opt)
			&& IsInteger(_opt->H)  && (_opt->HI & 0x01)) {
		AGmNode *n1 = dst_node(_opt, tag_id->a.s.v->Str());
		if (IsString(n1)) n = n1;
		}
		if (n) {
			f->WriteString(n->a.s.v->Str());
		} else {
			f->WriteString("</");
			f->WriteString(tag_id_str);
			f->WriteString(">");
		}
	} else {
		f->WriteString("/>");
	}
	delete tag_id_str; return 0;
}
int AUiXmlShell::indent_printf() {
	if (no_indent) return 0;
	if (first) {
		first = 0;
	} else {
		f->WriteString("\n");
	}
	for (int i = 0; i < indent; i++) {
		f->WriteString("  ");
	}
	return 0;
}

int AIpProcess::EvalXML() {
	AGmNode *cmd, *arg1, *arg2, *arg3;
	GetArg(cmd, arg1, arg2, arg3);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("XML()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "parse") == 0) {
		if (IsFile(arg1)) {
			AGmNode *tree = NULL;
			if (arg2) tree = arg2;
			AUiXmlShell *xml = new AUiXmlShell(this, arg1->a.f.v, tree, arg3);
			long err = xml->Parse();
			Val(NULL);
			if (err == 0) {
				val = Ref(List(tree, (AGmNode*)NULL));
			} else {
				char str[256];
				art_sprintf_1(str, 256, "error cdoe = %ld\n", err);
				val = Ref(List((AGmNode*)NULL, String(str)));
			}
			delete xml;
		} else {
			val = Ref(List((AGmNode*)NULL, String("input is not stream.")));
		}

	} else if (strcmp(s, "generate") == 0) {
		if (IsFile(arg1)) {
			AUiXmlShell *xml = new AUiXmlShell(this, arg1->a.f.v, arg2, arg3);
			int err = xml->Generate();
			Val(NULL);
			if (err) {
				val = Ref(Integer(err));
			}
			delete xml;
		}

	} else if (strcmp(s, "xslt") == 0) {
		if (arg1 && arg2 && IsFile(arg3)) {
			XsltProcessor *processor = new XsltProcessor(this, arg1, arg2, arg3->a.f.v);
			AGmString* err = processor->Transform();
			Val(NULL);
			if (err) {
				val = Ref(String(err));
			}
			delete processor;
		}

	} else if (strcmp(s, "type_check") == 0) {
		if (IsString(arg1)) {
			int err = type_check(arg1->a.s.v, arg2);
			if (err) {
				val = Ref(Integer((long)err));
			}
		}

	} else if (strcmp(s, "dic_check") == 0) {
		if (IsString(arg1)) {
			int err = dic_check(arg1->a.s.v, arg2);
			if (err) {
				val = Ref(Integer((long)err));
			}
		}

	} else {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("XML()");
	}
	Unref(cmd, arg1, arg2, arg3);
	return 0;
}
