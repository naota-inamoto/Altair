/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AL_MATH_H
#define _AL_MATH_H

/* -------- internal format of polynomial --------
 <term_list> = (<term>, ...)
 <term> = ((<const>, <const>), <var_list>, <func_list>)
 <var_list>  = (<var>, ...)
 <func_list> = (<func>, ...)
 <var> = ("var", <name>, <index_list>, <degree>)
 <index_list> = (<index-1>, ... , <index-k>)
 <func> = ("func", <name>, <A_list_list>, <degree>)
 <A_list_list> = (<A-list-1>, ..., <A-list-k>)
 <index>
 <degree>
 */

class AGmMath : public AGm {
public:
	static int error(const char*, const char*, AGmNode*&);
	static int InterpretAndExecution(int, AGmNode*, AGmNode*&);

	static int is_zero(AGmNode*);
	static int is_one(AGmNode*);
	static int is_plus(AGmNode*);
	static int is_minus(AGmNode*);

	static AGmNode *add_poly(AGmNode*, AGmNode*);
	static AGmNode *add_term(AGmNode*, AGmNode*);
	static AGmNode *sub_poly(AGmNode*, AGmNode*);
	static AGmNode *sub_term(AGmNode*, AGmNode*);
	static AGmNode *neg_poly(AGmNode*);
	static AGmNode *neg_term(AGmNode*);
	static int term_comp(AGmNode*, AGmNode*);
	static int term_comp_by_var(AGmNode*, AGmNode*);
	static int term_comp_by_func(AGmNode*, AGmNode*);

	static AGmNode *mul_poly(AGmNode*, AGmNode*);
	static AGmNode *mul_poly_by_term(AGmNode*, AGmNode*);
	static AGmNode *mul_term(AGmNode*, AGmNode*);
	static AGmNode *mul_term_by_var(AGmNode*, AGmNode*);
	static AGmNode *mul_term_by_func(AGmNode*, AGmNode*);

	static AGmNode *divmod_poly(AGmNode*, AGmNode*);
	static AGmNode *divmod_poly2(AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *divmod_poly_by_term(AGmNode*, AGmNode*,
				AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *div_term(AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *get_first_var(AGmNode*);
	static AGmNode *degree_poly(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *degree_poly_var(AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *degree_poly_var2(AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *degree_poly_func(AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *degree_poly_func2(AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *coef_poly(AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *coef_poly_var(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *coef_poly_var2(AGmNode*, int&, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *coef_poly_func(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *coef_poly_func2(AGmNode*, int&, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *sort_poly_insert(AGmNode*, AGmNode*);
	static int sort_poly_comp(AGmNode*, AGmNode*, int);
	static AGmNode *common_num(AGmNode*);
	static AGmNode *common_num2(AGmNode*, AGmNode*);
	static AGmNode *common_var(AGmNode*);
	static AGmNode *common_var2(AGmNode*, AGmNode*);
	static AGmNode *common_var3(AGmNode*, AGmNode*);
	static AGmNode *common_func(AGmNode*);
	static AGmNode *common_func2(AGmNode*, AGmNode*);
	static AGmNode *common_func3(AGmNode*, AGmNode*);
	static AGmNode *normalize(AGmNode*);
	static AGmNode *normalize2(AGmNode*, AGmNode*);
	static AGmNode *normalize3(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode *normalize3_var(AGmNode*, AGmNode*);
	static AGmNode *normalize3_func(AGmNode*, AGmNode*);
	static AGmNode *to_gfp_poly(AGmNode*, unsigned long);
	static AGmNode *from_gfp_poly(AGmNode*);
};

#endif

