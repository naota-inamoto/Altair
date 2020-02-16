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

#include <xml/xml.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#define _DEBUG_XSLT
#ifdef	_DEBUG_XSLT
#include <dev/advwnd.h>
void dump_xml(AGmNode *xml) {
	XmlNodeItr *xml_itr = new XmlNodeItr(xml);
	for (;;) {
		xml_itr->Next();
		char ss[256];
		art_sprintf_1(ss, 256, "tag_itr = %lx, ", (unsigned long)(xml_itr->tag_itr ? xml_itr->tag_itr : NULL));
		ADvWindow::Print(ss);
		switch (xml_itr->type) {
		case XML_ITR_BEGIN:
			ADvWindow::Print("XML_ITR_BEGIN ");
			ADvWindow::Print("\n");
			break;
		case XML_ITR_END:
			ADvWindow::Print("XML_ITR_END ");
			ADvWindow::Print("\n");
			break;
		case XML_ITR_TAG_BEGIN:
			ADvWindow::Print("XML_ITR_TAG_BEGIN ");
			ADvWindow::Print(xml_itr->tag_id->a.s.v->Str());
			ADvWindow::Print("\n");
			break;
		case XML_ITR_TAG_END:
			ADvWindow::Print("XML_ITR_TAG_END ");
			ADvWindow::Print("\n");
			break;
		case XML_ITR_PARAM:
			ADvWindow::Print("XML_ITR_PARAM ");
			ADvWindow::Print(xml_itr->param_id->a.s.v->Str());
			ADvWindow::Print("\n");
			break;
		case XML_ITR_TEXT:
			ADvWindow::Print("XML_ITR_TEXT ");
			ADvWindow::Print(xml_itr->value->a.s.v->Str());
			ADvWindow::Print("\n");
			break;
		case XML_ITR_COMMENT:
			ADvWindow::Print("XML_ITR_COMMENT ");
			ADvWindow::Print(xml_itr->value->a.s.v->Str());
			ADvWindow::Print("\n");
			break;
		}
		if (xml_itr->type == XML_ITR_END) break;
	}
	delete xml_itr;
}
#endif

extern AGmNode *split_str(AGmString *s, int d);

XmlNodeItr::XmlNodeItr(AGmNode *body) {
	type = XML_ITR_BEGIN;
	if (body) {
		tag_itr = Ref(DstItr(body));
	} else {
		tag_itr = NULL;
	}
	param_itr = NULL;
	stack = NULL;
	tag_count = 0;
	_xml_comment = Ref(UniqueStr("!comment"));
	_xml_body = Ref(UniqueStr("!body"));
	_xml_text = Ref(UniqueStr("!text"));
}
XmlNodeItr::~XmlNodeItr() {
	Unref(tag_itr, param_itr, stack);
	Unref(_xml_comment, _xml_body, _xml_text);
}
int XmlNodeItr::Next(int flag, AGmNode *tag_id2, const char *prefix) {
	if (flag == XML_ITR_UP) {
		Unref(tag_itr);
		if (stack) {
			tag_itr = stack->H; Pop(stack); --tag_count;
			return type = XML_ITR_TAG_END;
		} else {
			tag_itr = NULL;
			return type = XML_ITR_END;
		}
	}
	int depth = 0;
	for (;;) {
		if (tag_itr == NULL) return type = XML_ITR_END;
		if (param_itr) {
			switch (flag) {
			case XML_ITR_NEXT:
				for (;;) {
					value = itr_next(param_itr);
					if (value == NULL) break;
					param_id = param_itr->a.itr.link->arc->attr;
					if (param_id == _xml_body) continue;
					return type = XML_ITR_PARAM;
				}
				Unref(param_itr); param_itr = NULL;
				break;
			case XML_ITR_FIND_CHILD:
			case XML_ITR_FIND_DESCENDANT:
				if (tag_id2->a.s.v->Str()[0] != '@') {
					Unref(param_itr); param_itr = NULL;
					break;
				}
				for (;;) {
					value = itr_next(param_itr);
					if (value == NULL) break;
					param_id = param_itr->a.itr.link->arc->attr;
					if (param_id == _xml_body) continue;
					if (param_id->a.s.v->Compare(&tag_id2->a.s.v->Str()[1]) != 0
							|| (prefix && dst_node(param_id, prefix) == NULL)) {
						continue;
					} else {
						return type = XML_ITR_PARAM;
					}
				}
				Unref(param_itr); param_itr = NULL;
				break;
			}
		}
		if (type == XML_ITR_TAG_BEGIN || type == XML_ITR_PARAM) {
			AGmNode *body;
			switch (flag) {
			case XML_ITR_NEXT:
				body = dst_node(tag, _xml_body);
				if (body) {
					Push(stack, tag_itr); tag_count++; depth++;
					tag_itr = Ref(DstItr(body));
				} else {
					return type = XML_ITR_TAG_END;
				}
				break;
			case XML_ITR_FIND_CHILD:
				if (depth == 0) {
					body = dst_node(tag, _xml_body);
					if (body) {
						Push(stack, tag_itr); tag_count++; depth++;
						tag_itr = Ref(DstItr(body));
					}
				}
				break;
			case XML_ITR_FIND_DESCENDANT:
				body = dst_node(tag, _xml_body);
				if (body) {
					Push(stack, tag_itr); tag_count++; depth++;
					tag_itr = Ref(DstItr(body));
				}
				break;
			}
		}
		for (;;) {
			value = itr_next(tag_itr);
			if (value) break;
			Unref(tag_itr);
			if (stack) {
				tag_itr = stack->H; Pop(stack); --tag_count; --depth;
				type = XML_ITR_TAG_END;
				switch (flag) {
				case XML_ITR_NEXT:
					return type;
				case XML_ITR_FIND_CHILD:
					if (depth < 0) return type;
					break;
				case XML_ITR_FIND_DESCENDANT:
					break;
				}
			} else {
				tag_itr = NULL;
				return type = XML_ITR_END;
			}
		}
		tag_id = tag_itr->a.itr.link->arc->attr;
		type = XML_ITR_TAG_BEGIN;
		tag = value;
		switch (flag) {
		case XML_ITR_NEXT:
			if (tag_id == _xml_comment) return type = XML_ITR_COMMENT;
			if (tag_id == _xml_text) return type = XML_ITR_TEXT;
			param_itr = Ref(DstItr(tag));
			return type;
		case XML_ITR_FIND_CHILD:
			if (tag_id == _xml_comment) { type = XML_ITR_COMMENT; continue; }
			if (tag_id == _xml_text) { type = XML_ITR_TEXT; continue; }
			param_itr = Ref(DstItr(tag));
			if (tag_id->a.s.v->Compare(tag_id2->a.s.v->Str()) == 0
					&& (prefix == NULL || dst_node(tag_id, prefix))) {
				return type;
			}
			break;
		case XML_ITR_FIND_DESCENDANT:
			if (tag_id == _xml_comment) { type = XML_ITR_COMMENT; continue; }
			if (tag_id == _xml_text) { type = XML_ITR_TEXT; continue; }
			param_itr = Ref(DstItr(tag));
			if (tag_id->a.s.v->Compare(tag_id2->a.s.v->Str()) == 0
					&& (prefix == NULL || dst_node(tag_id, prefix))) {
				return type;
			}
			break;
		}
	}
}

XmlPathTagItr::XmlPathTagItr(const char *path) {
	AGmString *path2;
	if (strncmp(path, "./", 2) == 0) {
		path2 = new AGmString(&path[2]);
	} else if (strncmp(path, "/", 1) == 0) {
		path2 = new AGmString(&path[1]);
	} else {
		path2 = new AGmString(path);
	}
	tags = Ref(split_str(path2, '/'));
	delete path2;
	path_itr = Ref(DstItr(tags));
	stack = NULL;
	tag_count = 0;
}
XmlPathTagItr::~XmlPathTagItr() {
	Unref(tags, path_itr, stack);
}
int XmlPathTagItr::Next() {
	Push(stack, Integer(tag_count));
	tag_id = itr_next(path_itr); tag_count++;
	if (tag_id == NULL) return TAG_ITR_END;
	int r = TAG_ITR_CHILD;
	while (tag_id->a.s.v->Compare("") == 0) {
		r = TAG_ITR_DESCENDANT;
		tag_id = itr_next(path_itr); tag_count++;
		if (tag_id == NULL) return TAG_ITR_END;
	}
	return r;
}
int XmlPathTagItr::Prev() {
	if (stack) {
		int count = (int)tag_count;
		tag_count = (int)stack->HI; Pop(stack);
		while (count > tag_count) {
			itr_prev(path_itr); --count;
		}
		return 1;
	}
	Unref(path_itr); path_itr = Ref(DstItr(tags));
	return 0;
}

XmlPathItr::XmlPathItr(AGmNode *xml, const char *path, const char *prefix) {
	xml_itr = new XmlNodeItr(xml);
	tag_itr = new XmlPathTagItr(path);
	stack = NULL;
	this->prefix = prefix;
}
XmlPathItr::~XmlPathItr() {
	delete xml_itr;
	delete tag_itr;
	Unref(stack);
}
XmlNodeItr* XmlPathItr::Next(const char *attr_name, const char *attr_value) {
	if (stack) {
		tag_itr->Prev();
		tag_itr->Prev();
		int count = (int)stack->HI; Pop(stack);
		while (count < xml_itr->tag_count) {
			xml_itr->Next(XML_ITR_UP);
		}
	}
	for (;;) {
		Push(stack, Integer(xml_itr->tag_count));
		int r = tag_itr->Next();
		if (r == TAG_ITR_END) {
			if (attr_name == NULL || attr_value == NULL) {
				return xml_itr;
			} else {
				AGmNode *attr = dst_node(xml_itr->tag, attr_name);
				if (attr && attr->H->a.s.v->Compare(attr_value) == 0) {
					return xml_itr;
				}
				// skip
				tag_itr->Prev();
				tag_itr->Prev();
				int count = (int)stack->HI; Pop(stack);
				while (count < xml_itr->tag_count) {
					xml_itr->Next(XML_ITR_UP);
				}
				continue;
			}
		}
		if (r == TAG_ITR_CHILD) {
			r = xml_itr->Next(XML_ITR_FIND_CHILD, tag_itr->tag_id, prefix);
		} else {
			r = xml_itr->Next(XML_ITR_FIND_DESCENDANT, tag_itr->tag_id, prefix);
		}
		if (r == XML_ITR_END) return NULL;
		if (r == XML_ITR_TAG_BEGIN || r == XML_ITR_PARAM) continue;
		tag_itr->Prev();
		tag_itr->Prev();
		int count = (int)stack->HI; Pop(stack);
		if (xml_itr->tag_count > count) {
			tag_itr->Prev();
			tag_itr->Prev();
			while (count < xml_itr->tag_count) {
				xml_itr->Next(XML_ITR_UP);
			}
		}
	}
}

CondEvaluator::CondEvaluator(AGmNode *xml, AGmString *test, AGmNode *variables) {
	this->xml = xml;
	this->test = test;
	this->variables = variables;
	_xml_body = Ref(UniqueStr("!body"));
	_xml_text = Ref(UniqueStr("!text"));
}
CondEvaluator::~CondEvaluator() {
	Unref(_xml_body, _xml_text);
}

int CondEvaluator::eval() {
	AGmString *path = new AGmString(16);
	AGmString *value_str = new AGmString(16);
	int i, n = test->Length();
	char c, op = 0;
	const char *buf = test->Str();
	for (i = 0; i < n; i++)  {
		c = buf[i];
		if (c == '=' || c == '<' || c == '>') { op = c; break; }
		path->Append(c);
	}
	for (i++; i < n; i++)  {
		c = buf[i];
		value_str->Append(c);
	}

	AGmNode *value1 = NULL, *value2 = NULL;

	XmlPathItr *itr = new XmlPathItr(xml, path->Str());
	XmlNodeItr *xml_itr = itr->Next();
	if (xml_itr) {
		AGmNode *body = dst_node(xml_itr->tag, _xml_body);
		if (body) {
			value1 = dst_node(body, _xml_text);
		}
	}
	delete itr;

	AGmFile *f = new AGmFile();
	f->Create(value_str, AGM_FILE_STR_READ);
	int type = 0;
	if (f->MatchString("$")) {
		if (f->ReadString(value2) == 0) {
			AGmNode *l;
			for (l = variables; l; l = l->T) {
				if (value2->a.s.v->Compare(l->HH->a.s.v) == 0) {
					type = 's';
					break;
				}
			}
			if (l) {
				RefUnref(value2); value2 = Ref(l->HTH);
			} else {
				type = -1;
			}
		}
	} else if (f->ReadSingleQString(value2) == 0) {
		type = 's';
	} else if (f->ReadInteger(value2) == 0) {
		type = 'i';
	}
	delete f;

	int r = 0;
	if (type == 0) {
		r = (value1 != NULL);
	} else if (type == 's') {
		switch (op) {
		case '=':
			r = (value1 && value2 && value1->a.s.v->Compare(value2->a.s.v) == 0);
			break;
		case '<':
			r = (value1 && value2 && value1->a.s.v->Compare(value2->a.s.v) < 0);
			break;
		case '>':
			r = (value1 && value2 && value2->a.s.v->Compare(value1->a.s.v) < 0);
			break;
		}
	} else if (type == 'i') {
		AGmNode *n = NULL;
		f = new AGmFile();
		f->Create(value1->a.s.v, AGM_FILE_STR_READ);
		if (f->ReadInteger(n)) {
			delete f; RefUnref(value2);
			delete path; delete value_str; return 0;
		}
		RefUnref(value1); value1 = n;
		delete f;
		switch (op) {
		case '=':
			r = (value1->a.i.v == value2->a.i.v);
			break;
		case '<':
			r = (value1->a.i.v < value2->a.i.v);
			break;
		case '>':
			r = (value1->a.i.v > value2->a.i.v);
			break;
		}
	}
	RefUnref(value1, value2);
	delete path; delete value_str;
	return r;
}

XsltProcessor::XsltProcessor(AIpProcess *process, AGmNode* xsl, AGmNode *xml, AGmFile *out) {
	_process = process;
	_xsl = xsl;
	_xml = xml;
	_out = out;
	variables = NULL;
	stack = NULL;
	_xml_body = Ref(UniqueStr("!body"));
	_xml_text = Ref(UniqueStr("!text"));
}

XsltProcessor::~XsltProcessor() {
	Unref(variables, stack);
	Unref(_xml_body, _xml_text);
}

AGmString* XsltProcessor::Transform() {
	XmlPathItr *itr = new XmlPathItr(_xsl, "stylesheet", "xsl");
	XmlNodeItr *xml_itr = itr->Next();
	if (xml_itr == NULL) {
		delete itr;
		itr = new XmlPathItr(_xsl, "transform", "xsl");
		xml_itr = itr->Next();
		if (xml_itr == NULL) {
			delete itr;
			return new AGmString("<xsl:stylesheet> or <xsl:transform> not found.");
		} else {
			_xsl = dst_node(xml_itr->tag, _xml_body);
		}
	} else {
		_xsl = dst_node(xml_itr->tag, _xml_body);
	}
	delete itr;
	if (_xsl == NULL) {
		return new AGmString("<xsl:stylesheet> is empty.");
	}
	itr = new XmlPathItr(_xsl, "output", "xsl");
	xml_itr = itr->Next();
	if (xml_itr) {
		AGmNode *encoding = dst_node(xml_itr->tag, "encoding");
		AGmNode *doctype = dst_node(xml_itr->tag, "doctype-system");
		if (encoding) {
			_out->WriteString("<?xml version=\"1.0\" encoding=\"");
			_out->WriteString(encoding->H->a.s.v->Str());
			_out->WriteString("\"?>\n");
		}
		if (doctype) {
			_out->WriteString("<!DOCTYPE ");
			root = 0;
			AGmString *doc_type = GetDocType(_xsl, _xml);
			if (doc_type == NULL) {
				delete itr;
				return new AGmString("root element for output not found.");
			}
			_out->WriteString(doc_type->Str());
			delete doc_type;
			_out->WriteString(" SYSTEM \"");
			_out->WriteString(doctype->H->a.s.v->Str());
			_out->WriteString("\">\n");

		}
	}
	delete itr;
	root = 0; in_attr = 0;
	return Transform(_xsl, _xml);
}

AGmString* XsltProcessor::GetDocType(AGmNode *xsl, AGmNode *xml, AGmNode *match2) {
	AGmString *doc_type = NULL;

	if (root == 0) {
		XmlPathItr *itr1 = new XmlPathItr(xsl, "template", "xsl");
		XmlNodeItr *xml_itr1 = itr1->Next("match", "/");
		if (xml_itr1) {
			root = 1;
			AGmNode *body = dst_node(xml_itr1->tag, _xml_body);
			if (body) {
				doc_type = GetDocType2(body, xml);
				delete itr1;
				return doc_type;
			}
		}
		delete itr1;
	}

	XmlPathItr *itr1 = new XmlPathItr(xsl, "template", "xsl");
	for (;;) {
		XmlNodeItr *xml_itr1 = itr1->Next();
		if (xml_itr1 == NULL) break;
		if (xml_itr1->type != XML_ITR_TAG_BEGIN) continue;
			AGmNode *match = dst_node(xml_itr1->tag, "match");
			if (match) {
				if (root && match->H->a.s.v->Compare("/") == 0) continue;
				if (match2 == NULL || match->H->a.s.v->Compare(match2->H->a.s.v) == 0) {
					XmlPathItr *itr2 = new XmlPathItr(xml, match->H->a.s.v->Str(), NULL);
					for (;;) {
						XmlNodeItr *xml_itr2 = itr2->Next();
						if (xml_itr2 == NULL) break;
						if (xml_itr2->type != XML_ITR_TAG_BEGIN) continue;
						AGmNode *body1 = dst_node(xml_itr1->tag, _xml_body);
						AGmNode *body2 = dst_node(xml_itr2->tag, _xml_body);
						if (body1 && body2) {
							doc_type = GetDocType2(body1, body2);
							if (doc_type) break;
						}
					}
					delete itr2;
				}
			}
	}
	delete itr1;
	if (doc_type == NULL) doc_type = new AGmString("Unknown");
	return doc_type;
}
AGmString* XsltProcessor::GetDocType2(AGmNode* templ, AGmNode* xml) {
	AGmString *doc_type = NULL;
	XmlNodeItr *xsl_itr = new XmlNodeItr(templ);
	for (;;) {
		int r = xsl_itr->Next();
		if (r == XML_ITR_END) break;
		switch (xsl_itr->type) {
		case XML_ITR_TAG_BEGIN:
			if (dst_node(xsl_itr->tag_id, "xsl")) {
				AGmString *s = xsl_itr->tag_id->a.s.v;
				if (s->Compare("apply-templates") == 0) {
					AGmNode *select = dst_node(xsl_itr->tag, "select");
					if (select) {
						doc_type = GetDocType(_xsl, xml, select);
						if (doc_type) break;
					}
				} else if (s->Compare("element") == 0) {
					AGmNode *name = dst_node(xsl_itr->tag, "name");
					if (name) {
						doc_type = new AGmString(name->H->a.s.v->Str());
						break;
					}
				}
			} else {
				doc_type = new AGmString(xsl_itr->tag_id->a.s.v->Str());
				break;
			}
			break;
		}
		if (doc_type) break;
	}
	delete xsl_itr;
	return doc_type;
}

AGmString* XsltProcessor::Transform(AGmNode *xsl, AGmNode *xml, AGmNode *match2) {
	AGmString *err = NULL;

	if (root == 0) {
		XmlPathItr *itr1 = new XmlPathItr(xsl, "template", "xsl");
		XmlNodeItr *xml_itr1 = itr1->Next("match", "/");
		if (xml_itr1) {
			root = 1;
			AGmNode *body = dst_node(xml_itr1->tag, _xml_body);
			if (body) {
				err = ApplyTemplate(body, xml);
				delete itr1;
				return err;
			}
		}
		delete itr1;
	}

	XmlPathItr *itr1 = new XmlPathItr(xsl, "template", "xsl");
	for (;;) {
		XmlNodeItr *xml_itr1 = itr1->Next();
		if (xml_itr1 == NULL) break;
			AGmNode *name = dst_node(xml_itr1->tag, "name");
			if (name) {
				continue;
			}

			AGmNode *match = dst_node(xml_itr1->tag, "match");
			if (match) {
				if (root && match->H->a.s.v->Compare("/") == 0) continue;
				if (match2 == NULL || match->H->a.s.v->Compare(match2->H->a.s.v) == 0) {
					XmlPathItr *itr2 = new XmlPathItr(xml, match->H->a.s.v->Str(), NULL);
					for (;;) {
						XmlNodeItr *xml_itr2 = itr2->Next();
						if (xml_itr2 == NULL) break;
						AGmNode *body1 = dst_node(xml_itr1->tag, _xml_body);
						AGmNode *body2 = dst_node(xml_itr2->tag, _xml_body);
						if (body1 && body2) {
							err = ApplyTemplate(body1, body2);
							if (err) break;
						}
					}
					delete itr2;
				}
			}
	}
	delete itr1;
	return err;
}

#define CHECK_START_TAG_END \
	AGmNode *stack2 = stack;	\
	while (stack2 && (stack2->HTHI == -2 || stack2->HTHI == -10 || stack2->HTHI == -20)) stack2 = stack2->T;	\
	if (stack2 && stack2->HTHI == 0) {	\
		stack2->HTHI = 1;	\
		_out->WriteString(">"); \
	}

#define IS_NOT_IN_START_TAG \
	AGmNode *stack2 = stack;	\
	while (stack2 && (stack2->HTHI == -10 || stack2->HTHI == -20)) stack2 = stack2->T;	\
	if (stack2 && stack2->HTHI != 0)

AGmString* XsltProcessor::ApplyTemplate(AGmNode* templ, AGmNode* xml) {
	AGmString *err = NULL;
	XmlNodeItr *xsl_itr = new XmlNodeItr(templ);
	for (;;) {
		int r = xsl_itr->Next();
		if (r == XML_ITR_END) break;
		switch (xsl_itr->type) {
		case XML_ITR_TAG_BEGIN:
			if (dst_node(xsl_itr->tag_id, "xsl")) {
				AGmString *s = xsl_itr->tag_id->a.s.v;
				// ---- template
				if (s->Compare("apply-templates") == 0) {
					CHECK_START_TAG_END
					Push(stack, List((AGmNode*)NULL, Integer(-1)));
					AGmNode *select = dst_node(xsl_itr->tag, "select");
					if (select) {
						err = Transform(_xsl, xml, select);
						if (err) break;
					} else {
						err = new AGmString("attribute 'select' missing in <xsl:appply-templates>");
						break;
					}
				} else if (s->Compare("call-template") == 0) {
					CHECK_START_TAG_END
					Push(stack, List((AGmNode*)NULL, Integer(-1)));
					AGmNode *name = dst_node(xsl_itr->tag, "name");
					if (name) {
						XmlPathItr *itr1 = new XmlPathItr(_xsl, "template", "xsl");
						XmlNodeItr *xml_itr1 = itr1->Next("name", name->H->a.s.v->Str());
						if (xml_itr1) {
							AGmNode *body = dst_node(xml_itr1->tag, _xml_body);
							if (body) {
								err = ApplyTemplate(body, xml);
								if (err) break;
							}
						}
						delete itr1;
					} else {
						err = new AGmString("attribute 'name' missing in <xsl:call-template>");
						break;
					}
				// ---- output
				} else if (s->Compare("value-of") == 0) {
					if (in_attr == 0) {
						CHECK_START_TAG_END
					}
					Push(stack, List((AGmNode*)NULL, Integer(-1)));
					AGmNode *select = dst_node(xsl_itr->tag, "select");
					if (select) {
						if (select->H->a.s.v->Str()[0] == '{' && select->H->a.s.v->Str()[1] == '$') {
							AGmString *str = select->a.s.v->SubString(2, select->a.s.v->Length() - 1);
							AGmNode *l;
							for (l = variables; l; l = l->T) {
								if (str->Compare(l->HH->a.s.v) == 0) break;
							}
							if (l) {
								_out->WriteString(l->HTH->a.s.v->Str());
							} else {
							}
							delete str;
						} else {
							XmlPathItr *itr = new XmlPathItr(xml, select->H->a.s.v->Str(), NULL);
							for (;;) {
								XmlNodeItr *xml_itr = itr->Next();
								if (xml_itr == NULL) break;
								if (xml_itr->type != XML_ITR_TAG_BEGIN) continue;
								AGmNode *body = dst_node(xml_itr->tag, _xml_body);
								if (body) {
									AGmNode *text = dst_node(body, _xml_text);
									if (text) {
										_out->WriteString(text->a.s.v->Str());
									}
								}
								break;
							}
							delete itr;
						}
					}
				} else if (s->Compare("element") == 0) {
					CHECK_START_TAG_END
					AGmNode *name = dst_node(xsl_itr->tag, "name");
					if (name) {
						_out->WriteString("<");
						_out->WriteString(name->H->a.s.v->Str());
						Push(stack, List(name->H, Integer(0)));
					} else {
						err = new AGmString("attribute 'name' missing in <xsl:element>");
						break;
					}
				} else if (s->Compare("attribute") == 0) {
					if (in_attr) {
						err = new AGmString("nexted <xsl:attribute>");
						break;
					}
					IS_NOT_IN_START_TAG {
						err = new AGmString("begin tag already closed before <xsl:attribute>");
						break;
					}
					AGmNode *name = dst_node(xsl_itr->tag, "name");
					if (name) {
						_out->WriteString(" ");
						_out->WriteString(name->H->a.s.v->Str());
						_out->WriteString("=\"");
						Push(stack, List((AGmNode*)NULL, Integer(-2)));
						in_attr = 1;
					} else {
						err = new AGmString("attribute 'name' missing in <xsl:attribute>");
						break;
					}
				} else if (s->Compare("text") == 0) {
					CHECK_START_TAG_END
					Push(stack, List((AGmNode*)NULL, Integer(-1)));
				// ---- variable
				} else if (s->Compare("variable") == 0) {
					AGmNode *name = dst_node(xsl_itr->tag, "name");
					if (name == NULL) {
						err = new AGmString("attribute 'name' missing in <xsl:variable>");
						break;
					}
					AGmNode *select = dst_node(xsl_itr->tag, "select");
					if (select == NULL) {
						err = new AGmString("attribute 'select' missing in <xsl:variable>");
						break;
					}
					Push(stack, List((AGmNode*)name->H, Integer(-20)));
					XmlPathItr *itr = new XmlPathItr(xml, select->H->a.s.v->Str(), NULL);
					AGmNode *n = NULL;
					for (;;) {
						XmlNodeItr *xml_itr = itr->Next();
						if (xml_itr == NULL) {
							n = String("");
							break;
						}
						if (xml_itr->type != XML_ITR_TAG_BEGIN) continue;
						AGmNode *body = dst_node(xml_itr->tag, _xml_body);
						if (body) {
							AGmNode *text = dst_node(body, _xml_text);
							if (text) {
								n =  String(text->a.s.v->Str());
							} else {
								n = String("");
							}
						}
						break;
					}
					delete itr;
					AGmNode *l;
					for (l = variables; l; l = l->T) {
						if (name->H->a.s.v->Compare(l->HH->a.s.v) == 0) break;
					}
					if (l) {
						Set(l->HTH, n);
					} else {
						Push(variables, List(name->H, n));
					}
				// ---- control
				} else if (s->Compare("if") == 0) {
					AGmNode *test = dst_node(xsl_itr->tag, "test");
					if (test) {
						CondEvaluator *evaluator = new CondEvaluator(xml, test->H->a.s.v, variables);
						int cond = evaluator->eval();
						delete evaluator;
						if (cond) {
							AGmNode *body = dst_node(xsl_itr->tag, _xml_body);
							if (body) {
								err = ApplyTemplate(body, xml);
								if (err) break;
							}
						}
						int count = 1;
						for (;;) {
							int r = xsl_itr->Next();
							if (r == XML_ITR_TAG_BEGIN) count++;
							if (r == XML_ITR_TAG_END) --count;
							if (count == 0) break;
						}
					} else {
						err = new AGmString("attribute 'test' missing in <xsl:if>");
						break;
					}
				} else if (s->Compare("choose") == 0) {
					Push(stack, List((AGmNode*)NULL, Integer(-10)));
					AGmNode *body = dst_node(xsl_itr->tag, _xml_body);
					int done = 0;
					if (body) {
						XmlPathItr *itr1 = new XmlPathItr(body, "when", "xsl");
						for (;;) {
							XmlNodeItr *xml_itr1 = itr1->Next();
							if (xml_itr1 == NULL) break;
							AGmNode *test = dst_node(xml_itr1->tag, "test");
							if (test) {
								CondEvaluator *evaluator = new CondEvaluator(xml, test->H->a.s.v, variables);
								int cond = evaluator->eval();
								delete evaluator;
								if (cond) {
									done = 1;
									AGmNode *body = dst_node(xml_itr1->tag, _xml_body);
									if (body) {
										err = ApplyTemplate(body, xml);
										if (err) break;
									}
								}
							} else {
								err = new AGmString("attribute 'test' missing in <xsl:when>");
								break;
							}
							if (done) break;
						}
						delete itr1;
						if (err) break;
						if (done == 0) {
							XmlPathItr *itr1 = new XmlPathItr(body, "otherwise", "xsl");
							XmlNodeItr *xml_itr1 = itr1->Next();
							if (xml_itr1) {
								AGmNode *body = dst_node(xml_itr1->tag, _xml_body);
								if (body) {
									err = ApplyTemplate(body, xml);
									if (err) break;
								}
							}
							delete itr1;
							if (err) break;
						}
					}
				} else if (s->Compare("when") == 0) {
					int count = 1;
					for (;;) {
						int r = xsl_itr->Next();
						if (r == XML_ITR_TAG_BEGIN) count++;
						if (r == XML_ITR_TAG_END) --count;
						if (count == 0) break;
					}
				} else if (s->Compare("otherwise") == 0) {
					int count = 1;
					for (;;) {
						int r = xsl_itr->Next();
						if (r == XML_ITR_TAG_BEGIN) count++;
						if (r == XML_ITR_TAG_END) --count;
						if (count == 0) break;
					}
				} else if (s->Compare("for-each") == 0) {
					CHECK_START_TAG_END
					AGmNode *select = dst_node(xsl_itr->tag, "select");
					if (select) {
						XmlPathItr *itr2 = new XmlPathItr(xml, select->H->a.s.v->Str(), NULL);
						for (;;) {
							XmlNodeItr *xml_itr2 = itr2->Next();
							if (xml_itr2 == NULL) break;
							AGmNode *body1 = dst_node(xsl_itr->tag, _xml_body);
							AGmNode *body2 = dst_node(xml_itr2->tag, _xml_body);
							if (body1 && body2) {
								err = ApplyTemplate(body1, body2);
								if (err) break;
							}
						}
						delete itr2;
						int count = 1;
						for (;;) {
							int r = xsl_itr->Next();
							if (r == XML_ITR_TAG_BEGIN) count++;
							if (r == XML_ITR_TAG_END) --count;
							if (count == 0) break;
						}
					} else {
						err = new AGmString("attribute 'select' missing in <xsl:for-each>");
						break;
					}
				} else {
					char ss[256];
					art_sprintf_1(ss, 256, "<xsl:%s> not supported.", s->Str());
					err = new AGmString(ss);
					break;
				}
			} else {
				CHECK_START_TAG_END
				AGmNode *name = xsl_itr->tag_id;
				_out->WriteString("<");
				_out->WriteString(name->a.s.v->Str());
				Push(stack, List(name, Integer(0)));
				AGmNode *param_itr = Ref(DstItr(xsl_itr->tag));
				AGmNode *n, *v;
				for (v = itr_next(param_itr); v; v = itr_next(param_itr)) {
					AGmLink *l = param_itr->a.itr.link;
					n = l->arc->attr;
					if (n == NULL || n == _xml_body) continue;
					_out->WriteString(" ");
					_out->WriteString(n->a.s.v->Str());
					_out->WriteString("=\"");
					_out->WriteString(v->H->a.s.v->Str());
					_out->WriteString("\"");
				}
				Unref(param_itr);
			}
			break;
		case XML_ITR_TAG_END:
			if (stack->HTHI == 0) {
				_out->WriteString("/>");
			} else if (stack->HTHI == 1) {
				AGmNode *name = stack->HH;
				_out->WriteString("</");
				_out->WriteString(name->a.s.v->Str());
				_out->WriteString(">");
			} else if (stack->HTHI == -2) {
				_out->WriteString("\"");
				in_attr = 0;
			}
			Pop(stack);
			break;
		case XML_ITR_PARAM:
			break;
		case XML_ITR_TEXT:
			if (stack && stack->HTHI == 0) {
				stack->HTHI = 1;
				_out->WriteString(">");
			}
			_out->WriteString(xsl_itr->value->a.s.v->Str());
			break;
		case XML_ITR_COMMENT:
			break;
		}
		if (err) break;
	}
	delete xsl_itr;
	return err;
}

