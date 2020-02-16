/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_XML_H
#define _AGM_XML_H

#define XML_ITR_BEGIN		1
#define XML_ITR_END 		2
#define XML_ITR_TAG_BEGIN	3
#define XML_ITR_TAG_END 	4
#define XML_ITR_PARAM		5
#define XML_ITR_TEXT		6
#define XML_ITR_COMMENT 	7

#define XML_ITR_NEXT			1
#define XML_ITR_FIND_CHILD		2
#define XML_ITR_FIND_DESCENDANT 3
#define XML_ITR_UP				4

#define TAG_ITR_END 		1
#define TAG_ITR_CHILD		2
#define TAG_ITR_DESCENDANT	3


class XmlNodeItr : AGm {
public:
	XmlNodeItr(AGmNode *body);
	~XmlNodeItr();

	int Next(int flag=XML_ITR_NEXT, AGmNode *tag_id2=NULL, const char *prefix=NULL);

	int type;
	AGmNode *tag, *tag_id, *param_id, *value;
	AGmNode *tag_itr, *param_itr, *stack;
	int tag_count;

protected:
	AGmNode *_xml_comment;
	AGmNode *_xml_body;
	AGmNode *_xml_text;
};

class XmlPathTagItr : AGm {
public:
	XmlPathTagItr(const char *path);
	~XmlPathTagItr();

	int Next();
	int Prev();

	AGmNode *tags, *path_itr, *tag_id, *stack;
	int tag_count;
};

class XmlPathItr : AGm {
public:
	XmlPathItr(AGmNode *xml, const char *path, const char *prefix=NULL);
	~XmlPathItr();

	XmlNodeItr *Next(const char *attr_name=NULL, const char *attr_value=NULL);

	XmlNodeItr *xml_itr;
	XmlPathTagItr *tag_itr;
	AGmNode *stack;
	const char *prefix;
};

class CondEvaluator : AGm {
public:
	CondEvaluator(AGmNode *xml, AGmString *test, AGmNode *variables);
	~CondEvaluator();

	int eval();

	AGmNode *xml; AGmString *test;
	AGmNode *variables;

protected:
	AGmNode *_xml_body;
	AGmNode *_xml_text;
};

class XsltProcessor : AGm {
public:
	XsltProcessor(AIpProcess *process, AGmNode* xsl, AGmNode *xml, AGmFile *out);
	~XsltProcessor();

	AGmString* GetDocType(AGmNode *xsl, AGmNode *xml, AGmNode *match2=NULL);
	AGmString* GetDocType2(AGmNode *xsl, AGmNode *xml);

	AGmString* Transform();
	AGmString* Transform(AGmNode *xsl, AGmNode *xml, AGmNode *match2=NULL);

	AGmString* ApplyTemplate(AGmNode* templ, AGmNode* xml);

protected:
	AIpProcess *_process;
	AGmNode *_xsl;
	AGmNode *_xml;
	AGmFile *_out;
	int root, in_attr;
	AGmNode *variables;
	AGmNode *stack;

	AGmNode *_xml_body;
	AGmNode *_xml_text;
};

/* type_info: (type, min, max, repr) */
/* type:
  Integer
  PositiveInteger	   ( 1 <= )
  PosigiveInteger <= n ( 1<=, <=n )
  NatuarlNumber 	   ( 1 <= )
  Real
*/
/* repr:
  DateTimeStamp = DateTime,13,20,9(8)X9(6)V9(3)X
  DateStamp = Date,9,9,9(8)X
  MonetaryAmount = Real,1,19,9(13)V9(5)

  9    ... digit   0<=,<=9
  9(n) ... n digit	 0<=,<=9
  X    ... arbitrary charactor
  X(n) ... arbitrary n charactors
  V    ... dot (.)
  V(n) ... n dots (.)
*/
int type_check(AGmString *value, AGmNode* type_info);

/* dic_info: ()-/-/=>value */
int dic_check(AGmString *value, AGmNode* dic_info);

#endif	/* _AGM_J_CODE_H */
