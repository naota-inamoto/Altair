/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_PRO_LISP_H
#define _AGM_PRO_LISP_H

#define RES_OK			1
#define RES_CONTINUE	10
#define RES_BREAK		20
#define RES_RETURN		30
#define RES_NEXT		40

#define DUMP_ADDR

//#define PROF_TIME
//#define DEBUG_PTREE

class AGmListItr;
class AGmProlog;

class AGmProLisp : public AGm {
public:
	AGmProLisp();
	AGmProLisp(AGmNode*);
	void SetConst();
	~AGmProLisp();

	AGmNode *PredManip();

	void DebugLevel(int);
	void DebugFunc(AGmNode*);

	static void MaxCallCount(int);
	static int MaxCallCount();

	static void GCmark();
	void GC_mark();

	AGmNode* Error();
	AGmNode* Error(const char*);
	AGmNode* Load(AGmNode*);
	AGmNode* Call(const char*, AGmNode*);

	AGmNode* get_loc(AGmNode*);
	void set_loc(AGmNode*, AGmNode*);
	AGmNode* loc_save();
	void pop_loc(AGmNode*);
	int unset_loc(AGmNode*);

	AGmNode* call(AGmNode* func_name, AGmNode*);
	AGmNode* error_return(AGmNode* func_name);
	AGmNode* eval_para(AGmNode* func_name, AGmNode*, int&);
	AGmNode* eval_sent(AGmNode* func_name, AGmNode*, int&);
	int eval_cond1(AGmNode* func_name, AGmNode*, int&);
	int eval_cond2(AGmNode* func_name, AGmNode*, int&);
	int eval_cmp(AGmNode* func_name, AGmNode*, int, int&);
	int unify(AGmNode* func_name, AGmNode*, AGmNode*, int&);
	int unify_sub(AGmNode* func_name, AGmNode*, AGmNode*);
	int eval_cmp_atom(AGmNode* func_name, AGmNode*, AGmNode*, int&);
	AGmNode* eval_exp(AGmNode* func_name, AGmNode*, int&);
	int assign(AGmNode*, AGmNode*, const char*);
	AGmNode *eval_stat(AGmNode* func_name, AGmNode*, int&);

	int GetArg(int, AGmNode*);
	int GetArg(int, AGmNode*&, AGmNode*, AGmNode*);
	int GetArg(int, AGmNode*&, AGmNode*&, AGmNode*, AGmNode*);
	int GetArg(int, AGmNode*&, AGmNode*&, AGmNode*&, AGmNode*, AGmNode*);
	int GetArg(int, AGmNode*&, AGmNode*&, AGmNode*&, AGmNode*&, AGmNode*, AGmNode*);
	int GetArg(int, AGmNode*&, AGmNode*&, AGmNode*&, AGmNode*&, AGmNode*&, AGmNode*, AGmNode*);

	AGmNode* SysFuncError(const char*, AGmNode*);
	AGmNode* SysFuncError(const char*, AGmNode*, AGmNode*);

	AGmNode* AttrGrmr(AGmNode*, AGmNode*, AGmNode*);
	void GenerateGrammarCallback(AGmNode*);
	void ComposeGrammarCallback(AGmNode*, int regex=0);

	AGmNode* InvAttrGrmr(AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	AGmNode* InvAttrGrmr2(AGmNode*, AGmNode*, int);
	AGmNode* ParseTreeNormalize(AGmNode*);
	int ParseTreeCorrect(AGmNode*, AGmNode*);
	AGmNode* GetSubParseTree(AGmNode*, AGmNode*, AGmNode*, int, int para=0);
	static int InvAttrGrmrSyntaxDepth();

	AGmNode* ToListFromListExp(AGmNode*);
	AGmNode* ToListFromListExp2(AGmNode*);

public:
	AGmNode *env;
protected:
	AGmNode *prog;

	AGmNode *pred;
	AGmNode *manip;

	AGmNode *loc;
	AGmNode *stack;
	AGmNode *arg_ls_stack;
	int call_count;
	static int max_call_count;

	AGmListItr *list_itr;

public:
	AGmNode *error;
	int inv_ag_error_grph;

	AGmNode *str_const;
	AGmNode *str_no_sp_const;
	AGmNode *str_not_const;
	AGmNode *str_no_sp_not_const;
	AGmNode *str_regex;
	AGmNode *str_value;
	AGmNode *str_not_nl;
	AGmNode *str_eof;
	AGmNode *str_reg_ref;
	AGmNode *str_redirect;
	AGmNode *str_define;
	AGmNode *str_undef;
	AGmNode *str_ifdef;
	AGmNode *str_ifndef;

	AGmNode *str_repeat_0;
	AGmNode *str_repeat_1;
	AGmNode *str_braket_0;
	AGmNode *str_braket_1;

	AGmNode *syntax_id_val;
	AGmNode *grammar_val;
	AGmNode *parse_tree_val;
	AGmNode *syntax_unit_ids;

	AGmNode *comp_grmr_val_stack;

protected:
	int error_count;
	AGmNode *error_stack_trace;

	int debug_level;
	AGmNode* debug_func;

	AGmNode* attr_grmr_depth;

	static int unq_str_seq;

public:
	AGmNode *cont;
	AGmNode *brk;
	AGmNode *next;
	AGmNode *para_1;
	AGmNode *cond_1;
	AGmNode *s_exp;

	AGmNode *asgn_1;
	AGmNode *or_1;
	AGmNode *and_1;
	AGmNode *cmp_1;
	AGmNode *void_func;
	AGmNode *last_eq;
	AGmNode *eq;
	AGmNode *ne;
	AGmNode *ge;
	AGmNode *gt;
	AGmNode *le;
	AGmNode *lt;
	AGmNode *ls_1;
	AGmNode *ls_n;
	AGmNode *wc_t;
	AGmNode *wc_e;
	AGmNode *t;
	AGmNode *add_1;
	AGmNode *add;
	AGmNode *sub;
	AGmNode *mul_1;
	AGmNode *mul;
	AGmNode *div;
	AGmNode *mod;
	AGmNode *null;
	AGmNode *const_i;
	AGmNode *const_s;
	AGmNode *neg;
	AGmNode *l_not;
	AGmNode *var;
	AGmNode *braket;
	AGmNode *func_exp;
	AGmNode *arg_1;
	AGmNode *pre_inc;
	AGmNode *pre_dec;
	AGmNode *post_inc;
	AGmNode *post_dec;

	AGmNode* gcd;
	AGmNode* gcd_ex;
	AGmNode* inv_mod_m;
	AGmNode* crt;
	AGmNode* factor;
	AGmNode* next_prime;
	AGmNode* set_var;
	AGmNode* get_var;
	AGmNode* ifdef_var;
	AGmNode* undef_var;
	AGmNode* set_ary;
	AGmNode* get_ary;
	AGmNode* attr_grmr;
	AGmNode* inv_attr_grmr;
	AGmNode* stop;
	AGmNode* math;
	AGmNode* unq_str;

	AGmNode* undefined;

	AGmNode* ag_prog;

	AGmNode *prog_2;
	AGmNode *block_2;
	AGmNode *if_2;
	AGmNode *while_2;
	AGmNode *for_range_2;
	AGmNode *for_list_2;
	AGmNode *for_2;
	AGmNode *inv_for_2;
	AGmNode *switch_2;
	AGmNode *exp_2;
	AGmNode *ret;

	AGmNode *syn_var;
	AGmNode *syn_ret;
	AGmNode *syn_ret_str;
	AGmNode *syn_loc;
	AGmNode *syn_gen;
	AGmNode *syn_gen_str;
	AGmNode *syn_tree;
	AGmNode *syn_tree_str;
	AGmNode *syn_1;
	AGmNode *syn_2;
	AGmNode *syn_3;
	AGmNode *syn_4;
	AGmNode *syn_5;
	AGmNode *syn_6;
	AGmNode *syn_7;
	AGmNode *syn_8;
	AGmNode *syn_9;

	AGmNode *count;
	AGmNode *count_str;

	AGmNode *if_str;
	AGmNode *for_str;
	AGmNode *inv_for_str;
	AGmNode *switch_str;
	AGmNode *delay_val;

	AGmNode *rule;
	AGmNode *query;
	AGmNode *pred_goal;
	AGmNode *pred_not;
	AGmNode *next_if_false;
	AGmNode *goals;
	AGmNode *cons;
	AGmNode *list;
	AGmNode *append;
	AGmNode *member;
	AGmNode *length;
	AGmNode *starts_with;
	AGmNode *ends_with;
	AGmNode *grph;
	AGmNode *null_a;
	AGmNode *grph_eq;
	AGmNode *dest_node;
	AGmNode *sorc_node;
	AGmNode *must_be;
	AGmNode *eval_script;
	AGmNode *dump;
	AGmNode *print;
	AGmNode *debug;
	AGmNode *undef;

	AGmNode *pm_rep;
	AGmNode *rel_1_1;
	AGmNode *rel_1_n;
	AGmNode *inv_rel_1_1;
	AGmNode *inv_rel_1_n;
	AGmNode *sub_graph;
	AGmNode *node;
	AGmNode *arc;
	AGmNode *any_list;

	AGmNode* grph_src;
	AGmNode* grph_dst;
	AGmNode* comm_arc;
	AGmNode* bace;
	AGmNode* cwh;
	AGmNode* twh;
	AGmNode* invariant;
	AGmNode* b_nodes;
	AGmNode* b_arcs;
	AGmNode* a_nodes;
	AGmNode* a_arcs;
	AGmNode *nodes;
	AGmNode *arcs;
	AGmNode* p_final;

	AGmNode *stat;
	AGmNode *opt_grph;
	AGmNode *blk_grph;
	AGmNode *rpt_grph;
	AGmNode *chc_grph;
	AGmNode *chc_before;
	AGmNode *chc_after;
	AGmNode *sub_grph;

	AGmNode* obj_db_str;

	AGmNode* seq;
	AGmNode* glb_seq;
	AGmNode* is_type;
	AGmNode* if_e;
	AGmNode* if_ne;
	AGmNode* exist;
	AGmNode* not_exist;
	AGmNode *brk_if_found;

	AGmNode *syntax_id_stack;

protected:
	AGmNode *attr_grmr_units;
	AGmNode *ag_unit;

	AGmNode *ids;

	AGmProlog *prolog;

	AGmNode *pred_manips;
	AGmNode *infinit_recursive_check;

	static int inv_attr_grmr_syntax_depth;
};

class AGmListCB : public AGm {
public:
	AGmListCB();
	virtual ~AGmListCB();
	virtual void GenerateGrammarCallback(AGmNode*);
	virtual void ComposeGrammarCallback(AGmNode*, int regex=0);
	virtual AGmNode* Error();
};

class AGmAttrGrmrCB : public AGmListCB {
public:
	AGmAttrGrmrCB(AGmProLisp* cb);
	virtual ~AGmAttrGrmrCB();
	virtual void GenerateGrammarCallback(AGmNode* ls);
	virtual void ComposeGrammarCallback(AGmNode* ls, int regex=0);
	virtual AGmNode* Error();

	AGmProLisp *cb;
};

class AGmListItr : public AGm {
public:
	AGmListItr(AGmNode*, AGmListCB *cb=NULL);
	~AGmListItr();

	void GCmark();

	AGmNode* Next();
	AGmNode* Next(const char*);
	AGmNode* Next(AGmNode*);

	void AttrGrmrCB() { attr_grmr = 1; }

protected:
	AGmNode *root;
	AGmNode *stack;

	AGmListCB *cb;
	int attr_grmr;
};

#define PRO_PRED		1
#define PRO_FUNC		2
#define PRO_SYN			3
#define PRO_PM_REP		4

#define PRO_NOT_PRED	0x8000
#define PRO_NEXT_IF_F	0x4000

#define SYN_VAR_NO_DATA 	-1
#define FOR_STACK_EMPTY 	-2
#define NOT_YET_FOR_MODIF	-3
#define END_OF_FOR			-4
#define NOT_YET_SWITCH_ID	-5
#define TARGET_IS_REGEX 	-6
#define NOT_MATCH_BRAKET_0	-7
#define NOT_MATCH_REPEAT	-8

class AGmExecStack;

#define	REL_1_1		1
#define	REL_1_N		2
#define	INV_REL_1_1	3
#define	INV_REL_1_N	4

#define	BACK_SET_H_TH		zero
#define	BACK_SET2			one
#define	BACK_REP_ARC_DST	two
#define	BACK_REP_ARC_SRC	three
#define	BACK_DELETE_ARC		four

#define	BACK_CREATE_ARC		five
#define	BACK_CREATE_ARC2	six

#define	BACK_DELAYED		seven
#define	BACK_GEN_STATUS		eight
#define	BACK_FOR_INDEX		nine
#define	BACK_DEC_OF_DELAYD	minus_one

class AGmProlog : public AGm {
public:
	AGmProlog(AGmProLisp*);
	~AGmProlog();

	AGmNode *PredManip();
	void PredManip(AGmNode*);
	void Clear();
	void Back();
	void TreeConv(AGmNode* id0=NULL, AGmNode* grmr0=NULL);

	static void MaxCallCount(int);
	static int MaxCallCount();
	void CallCount(int);
	int CallCount();

	static void GCmark();
	void GC_mark();

	AGmNode* ToInternal(AGmNode*);

	AGmNode* ToInternalRule(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	AGmNode* ToInternalQuery(AGmNode*, AGmNode*, AGmNode*, AGmNode*);

	AGmNode* ToInternalPatturnMatchAndReplace(AGmNode*, AGmNode*, AGmNode*, AGmNode*);

	AGmNode* ToInternalGoals(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	AGmNode* ToInternalPred(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	AGmNode* ToInternalNotPred(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	AGmNode* ToInternalNextIfFalse(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	AGmNode* ToInternalFunc(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	AGmNode* ToInternalArg(AGmNode*, AGmNode*, AGmNode*, AGmNode*, int syn=0);
	AGmNode* CreateIdxs(AGmNode*);

	AGmNode* BACTs(AGmNode* before, AGmNode* after, AGmNode* common, AGmNode* tail,
		AGmNode* obj_db, AGmNode* syn_db,
		AGmNode*& before_after);

	AGmNode* PmAndRep(AGmNode*, AGmNode* ,AGmNode*, AGmNode*, AGmNode*, 
		AGmNode*, AGmNode*, AGmNode*, int&);
	AGmNode* add_s_d_a(
		AGmNode* nodes_0, AGmNode* arcs_0, AGmNode* nodes, AGmNode* arcs,
		AGmNode*& s, AGmNode*& d, AGmNode* a, int ai, AGmNode* obj_db, AGmNode** arc_a=NULL);
	AGmNode *sub_graph_stat(AGmNode*, AGmNode* ,AGmNode*, AGmNode*, AGmNode*, 
		AGmNode*, AGmNode*, AGmNode* ns=NULL, AGmNode* as=NULL);
	AGmNode* eval_exp(AGmNode*, AGmNode*);
	int unify(AGmNode*, AGmNode*, AGmNode*);
	int unify_sub(AGmNode**, AGmNode*, AGmNode*);
	int unify2(AGmNode*, AGmNode*, AGmNode*);
	int unify_sub2(AGmNode*, AGmNode*, AGmNode*);
	AGmNode* eval(AGmNode*, AGmNode*);
	AGmNode* Execute();
	AGmNode* Execute(AGmNode*, AGmLink*);
	AGmNode* PushPred(AGmNode*, AGmLink*);
	AGmNode* ExecutePred(AGmNode*&, AGmLink*&);
	AGmNode* ExecuteNotPred(AGmNode*, AGmLink*&);
	AGmNode* ExecuteNextIfFalse(AGmNode*, AGmLink*&);
	void DebugPred(int, const char*, AGmNode*, int);

	AGmNode* ExecuteGen(AGmNode*, AGmLink*&, AGmNode*&, AGmNode*&);
	AGmNode* ExecuteDef(AGmNode*, AGmLink*&, AGmNode*&, AGmNode*&);

	void Cut();

	void PushExecStack(AGmExecStack* stack);
	void PopExecStack();
	void FreeExecStack();

	void DeleteArcs(AGmNode*&);
	void Set_H_TH(AGmNode*, AGmNode*, AGmNode*&);
	void Set2(AGmNode**, AGmNode*, AGmNode*&);
	void ReplaceArcDst2(AGmArc*, AGmNode*, AGmNode*&);
	void ReplaceArcSrc2(AGmLink*, AGmNode*, AGmLink*, AGmNode*&);
	void DeleteArc2(AGmLink*, AGmNode*, AGmNode*&);
	void CreateArc2(AGmNode*, AGmNode*, AGmNode*, AGmNode*&);

	AGmNode* ApplyPred(AGmNode*, AGmLink*, AGmNode*, AGmNode*&, AGmLink*&, AGmNode*&, AGmNode*&);
	void CopyGraph(AGmNode*, AGmLink*, AGmNode*&, AGmLink*&);
	void NodeBody(AGmNode*);
	AGmNode* ApplyFunc(AGmNode*, AGmLink*, AGmLink*&, AGmNode*&, AGmNode*&);
	AGmNode* ApplySynFunc(AGmNode*, AGmLink*, AGmLink*&, AGmNode*&, AGmNode*&);
	AGmNode* error(const char*, AGmNode*);
	AGmNode *sub_graph_null_error(AGmNode*, AGmNode*, AGmNode*);

	AGmNode* ApplyPmAndRep(AGmNode*, AGmLink*&);

	void DebugDump(const char*, AGmNode*, AGmNode*);
	void dump_graph(int, AGmNode*, AGmNode*);

	AGmProLisp *p;
	int	tree_conv;

	int	grph;
	int	status;

	AGmNode *pred;
	AGmNode *manip;

	AGmExecStack *exec_stack;
	int resolve_count;

	int call_count;
	static int max_call_count;

	AGmNode *pred_stack;

	// Read Parse Tree
	int SynVar(AGmNode*);
	int GetFirst(AGmNode*);
	int GetNext();
	void GetSwitch();
	AGmNode *GetStackValue();
	AGmNode *GetParseTreeValue();
	AGmNode *GetSwitchId();

	// Write Parse Tree
	int gen_SynVar(AGmNode*, AGmNode*&, int, AGmNode** delay_for=NULL);
	int gen_ModifTree(AGmNode*&, int);
	int gen_GetFirst(AGmNode*);
	int gen_GetNext(AGmNode*&, AGmNode**);
	int gen_GetSwitch(AGmNode*&);
	AGmNode *gen_SynVarRet(int, AGmNode*);
	AGmNode *gen_DelayVal(AGmNode*, AGmNode*, AGmNode*&, AGmNode*);

	// Read & Write Parse Tree
	void init_idx(AGmNode*);
	int next_idx();
	int is_last_idx();
	void WriteSynVar(AGmString*&);

	AGmNode *id0;
	AGmNode *grmr0;
	AGmNode *ptree0;

	AGmNode *for_stack;

	static int debug_pred;
	static int debug_dump;
	static int debug_syn;
	static int debug_syn2;
	static int debug_syn3;

	static AGmNode* debug_syntax_id;
	static AGmNode* debug_syntax_id2;
	static AGmNode* debug_syntax_id3;

	AGmNode *idxs0, *idxs, *idxs2;
	AGmNode *id, *last_id;
	AGmNode *grmr;
	AGmNode *ptree;
	AGmNode *itr;
	AGmNode *stack;
	AGmNode *gen_status;
	AGmNode **addr;
};

#define OP_DUMMY	0
#define OP_PRED		1
#define OP_CUTTED	2

#define	OP_NOP			3
#define	OP_OPT_GRPH		4
#define	OP_BLK_GRPH		5
#define	OP_RPT_GRPH		6
#define	OP_CHC_GRPH		7
#define	OP_CHC_BEFORE	8

#define FOR_ALL		1
#define INV_FOR_ALL	0

class AGmExecStack : AGm {
public:
	AGmExecStack(int op, AGmNode* g1=NULL, AGmLink* rl1=NULL,
		AGmNode* n=NULL, AGmLink* l=NULL);
	~AGmExecStack();

	AGmExecStack *FindExecStack(int);

	void			make_exist(AGmProlog*, AGmNode*, int);
	AGmNode*		arc_exist_info(AGmProlog*, AGmNode*);

	AGmExecStack	*next;
	int 	op;
	int		for_all;
	AGmNode *g1;	// clause
	AGmLink *rl1;	// link to predicate node
	AGmNode *n; 	// iteration src
	AGmLink *l; 	// iteration link
	int 	pred_count;
	AGmNode *arcs;	// current back track information
	AGmNode *obj_db;
	int level;

	AGmNode* save_stat;
	AGmNode* save_stat2;
	AGmLink* save_l1;
	AGmLink *save_l2;

};

class AGmGdb : AGm {
public:
	AGmGdb();
	~AGmGdb();

	void Init(AGmProLisp*, AGmProlog*, AGmNode*, AGmLink*);
	void Final();

	void RepBeforeAfter();
	void PatternMatch(int first);
	void PatternMatchRel11();
	void PatternMatchRel1N();
	void PatternMatchInvRel11();
	void PatternMatchInvRel1N();
	void NextRepetition();

	AGmNode *get_b_stat(AGmNode*, AGmNode*, int);
	AGmNode *get_next_arc_name(AGmNode*);
	void StartModify(AGmNode*, AGmNode*);
	void ModifyCreateNode(AGmNode*);
	void ModifyCreateArc(AGmNode*);
	void ModifyDeleteArc(AGmNode*);
	void EvaluateChoiceCondition();
	void EvaluateCommonCondition();
	void Tail();

	AGmProLisp *p;
	AGmProlog *prolog;
	AGmNode *g1;
	AGmLink *rl1, *rl2;
	AGmNode *rel;
	AGmNode *obj_db;
	AGmNode* bace;
	AGmNode *err;
	AGmLink *l1;
	int match;
	int no_rep;
	int to_be_modified;
	int no_more_rep;

	AGmNode *before_and_after;
	AGmNode *b_stat;
	AGmNode *a_stat;
	AGmNode *invariant;
	AGmNode *b_nodes;
	AGmNode *b_arcs;
	AGmNode *a_nodes;
	AGmNode *a_arcs;
	AGmNode *cwh;
	AGmNode *twh;

	AGmNode *ret;

	AGmNode *arc;
	AGmNode *an;
	AGmNode *av;
	long ai;
	AGmNode *src;
	AGmNode *dst;
	AGmNode *n1;
	AGmNode *n;
	AGmLink *lnk;
	int brk;
};

#endif	/* _AGM_PRO_LISP_H */
