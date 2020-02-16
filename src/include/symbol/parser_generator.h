/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_PARSER_GENERATOR_H
#define _AGM_PARSER_GENERATOR_H

class AGmParserGenerator : public AGm {
public:
	AGmParserGenerator();
	AGmParserGenerator(AGmNode*);
	void SetConst();
	~AGmParserGenerator();

	AGmNode* Compile(AGmFile*);
	void DebugCompile(int);

#if	_WINDOWS && _64BIT
	AGmNode* gen_format_scanf(AGmFile*, long long&, AGmNode*&);

	AGmNode* def_scanf(AGmFile*, long long&, AGmNode*&, AGmNode* cs_name=zero);
	AGmNode* para_scanf(AGmFile*, long long&, AGmNode*&, AGmNode*);
	AGmNode* braket_scanf(AGmFile*, long long&, AGmNode*&, AGmNode*);
	AGmNode* unit_scanf(AGmFile*, long long&, AGmNode*&, AGmNode*);

	AGmNode* reg_scanf(AGmFile*, long long&, AGmNode*&);
	AGmNode* reg_unit_scanf(AGmFile*, long long&, AGmNode*&);
	AGmNode* reg_char_list_scanf(AGmFile*, long long&, AGmNode*&);
	long reg_char_scanf(AGmFile*, long long&, AGmNode*&, int unput=1);

	AGmNode* Parse(AGmFile*, const char*);
	void DebugUnit10(AGmNode*);
	void DebugUnit20(AGmNode*);
	void DebugUnit30(AGmNode*);
	int IfDebug10(AGmFile*, const char*);
	int IfDebug20(AGmFile*, const char*);
	int IfDebug30(AGmFile*, const char*);
	void ParseFull(int);
	static void ParseCutDepth(int);
	static int ParseCutDepth();

	AGmNode* parse_scanf(AGmFile*, AGmNode*, long long&, AGmNode*&);
	AGmNode* parse_para_scanf(AGmFile*, AGmNode*, long long&, long long&, AGmNode*&);
	AGmNode* parse_repeat0_scanf(AGmFile*, AGmNode*, long long&, long long&, AGmNode*&);
	AGmNode* parse_repeat1_scanf(AGmFile*, AGmNode*, long long&, long long&, AGmNode*&);
	AGmNode* parse_braket0_scanf(AGmFile*, AGmNode*, long long&, long long&, AGmNode*&);
	AGmNode* parse_unit_scanf(AGmFile*, AGmNode*, long long&, long long&, AGmNode*&);
	void parse_comment(AGmFile*, AGmNode*, AGmNode*&);
	AGmNode* parse_error(AGmFile*, long long&, AGmNode*, AGmNode*&);

	int parse_reg_scanf(AGmFile*, AGmNode*, long long&, AGmNode*&, AGmNode*&);
	int parse_reg_scanf(AGmFile*, const char*, AGmNode*, long long&, AGmNode*&, AGmString*);
	int parse_reg_unit_scanf(AGmFile*, const char*, AGmNode*, long long&, AGmNode*&, AGmString*);

	void set_lno(AGmFile*, long long&, AGmNode*, AGmNode*&);
#else
	AGmNode* gen_format_scanf(AGmFile*, long&, AGmNode*&);

	AGmNode* def_scanf(AGmFile*, long&, AGmNode*&, AGmNode* cs_name=zero);
	AGmNode* para_scanf(AGmFile*, long&, AGmNode*&, AGmNode*);
	AGmNode* braket_scanf(AGmFile*, long&, AGmNode*&, AGmNode*);
	AGmNode* unit_scanf(AGmFile*, long&, AGmNode*&, AGmNode*);

	AGmNode* reg_scanf(AGmFile*, long&, AGmNode*&);
	AGmNode* reg_unit_scanf(AGmFile*, long&, AGmNode*&);
	AGmNode* reg_char_list_scanf(AGmFile*, long&, AGmNode*&);
	long reg_char_scanf(AGmFile*, long&, AGmNode*&, int unput=1);

	AGmNode* Parse(AGmFile*, const char*);
	void DebugUnit10(AGmNode*);
	void DebugUnit20(AGmNode*);
	void DebugUnit30(AGmNode*);
	int IfDebug10(AGmFile*, const char*);
	int IfDebug20(AGmFile*, const char*);
	int IfDebug30(AGmFile*, const char*);
	void ParseFull(int);
	static void ParseCutDepth(int);
	static int ParseCutDepth();

	AGmNode* parse_scanf(AGmFile*, AGmNode*, long&, AGmNode*&);
	AGmNode* parse_para_scanf(AGmFile*, AGmNode*, long&, long&, AGmNode*&);
	AGmNode* parse_repeat0_scanf(AGmFile*, AGmNode*, long&, long&, AGmNode*&);
	AGmNode* parse_repeat1_scanf(AGmFile*, AGmNode*, long&, long&, AGmNode*&);
	AGmNode* parse_braket0_scanf(AGmFile*, AGmNode*, long&, long&, AGmNode*&);
	AGmNode* parse_unit_scanf(AGmFile*, AGmNode*, long&, long&, AGmNode*&);
	void parse_comment(AGmFile*, AGmNode*, AGmNode*&);
	AGmNode* parse_error(AGmFile*, long&, AGmNode*, AGmNode*&);

	int parse_reg_scanf(AGmFile*, AGmNode*, long&, AGmNode*&, AGmNode*&);
	int parse_reg_scanf(AGmFile*, const char*, AGmNode*, long&, AGmNode*&, AGmString*);
	int parse_reg_unit_scanf(AGmFile*, const char*, AGmNode*, long&, AGmNode*&, AGmString*);

	void set_lno(AGmFile*, long&, AGmNode*, AGmNode*&);
#endif

	AGmNode* Generate(AGmFile*, AGmNode*);

	AGmNode* gen_unit_printf(AGmFile*, AGmNode*);
	AGmNode* gen_braket_printf(AGmFile*, AGmNode*);
	AGmString* gen_reg_printf(AGmNode*, AGmNode*);
	void indent(AGmFile*);
	void set_space(int);
	void space(AGmFile*f);

	AGmNode* OutSyntaxUnit(AGmFile*, int);
	AGmNode* OutSyntaxUnit(AGmNode*&, AGmNode*, AGmNode*);
	AGmNode* EliminateGrammarID(AGmNode*, AGmNode*, AGmNode*);
	int IsOmitUnit(AGmNode*);
	int IsOmitUnits(AGmNode*);
	int IsFlatUnit(AGmNode*, int check=0);
	int FlatUnitStr(AGmNode*, AGmNode*, AGmString*&);
	int FlatUnitsStr(AGmNode*, AGmNode*, AGmString*&);
	int BinOpUnitStr(AGmNode*, AGmNode*, AGmString*&);
	int AsgnOpUnitStr(AGmNode*, AGmString*&);
	int CondOpUnitStr(AGmNode*, AGmNode*, AGmString*&);
	int UnaryOpUnitStr(AGmNode*, AGmString*&);
	AGmNode* MakeTree(AGmNode*, int, AGmNode*);
	int ParaFlatUnitStr(AGmNode*, AGmNode*, AGmNode*, AGmString*&);
	void RecursiveParaFlatUnitStr(int[], int, int, int, AGmNode*, int, int, AGmNode*, AGmString*&);
	void Indent(int, AGmString*&);
	void Index(int[], int, AGmString*&);
	void Var(int, AGmString*&);
	int ctrl_count(AGmNode*);

protected:
	AGmNode *syntax;
	AGmNode *ids;
	AGmNode *comment;
	AGmNode *ids_cs;
	AGmNode *reserve;
	AGmNode *gen_format;
	AGmNode *extension;

	AGmNode *error;

	int debug_compile;

	AGmNode *def_stack;
	int parse_depth;
	AGmNode *debug_unit_10;
	AGmNode *debug_unit_20;
	AGmNode *debug_unit_30;
	int parse_full;
	static int parse_cut_depth;
	int bol, eol;

	AGmNode *indent_stack;
	int space_on;
	int space_flag;
	int indent_flag;
	int split, last_ident;
	int nonewline;

	AGmNode *str_const;
	AGmNode *str_no_sp_const;
	AGmNode *str_not_const;
	AGmNode *str_no_sp_not_const;
	AGmNode *str_not_nl;
	AGmNode *str_line;
	AGmNode *str_reg_ref;
	AGmNode *str_until;
	AGmNode *str_redirect;
	AGmNode *str_regex;
	AGmNode *str_define;
	AGmNode *str_undef;
	AGmNode *str_ifdef;
	AGmNode *str_ifndef;
	AGmNode *str_nonewline;
	AGmNode *str_repeat_0;
	AGmNode *str_repeat_1;
	AGmNode *str_braket_0;
	AGmNode *str_braket_1;
	AGmNode *str_eof;
	AGmNode *str_integer;
	AGmNode *str_hex;
	AGmNode *str_char;
	AGmNode *str_real;
	AGmNode *str_string;
	AGmNode *str_quote_string;
	AGmNode *str_single_quote_string;
	AGmNode *str_special;
	AGmNode *str_extension;

	AGmNode *str_value;

	AGmNode *depend;
	AGmNode *units;
	AGmNode *syn_ids;
};

#endif	/* _AGM_PARSER_GENERATOR_H */

