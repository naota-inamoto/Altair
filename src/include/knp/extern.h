/*====================================================================

				EXTERN

											   S.Kurohashi 93. 5.31

	$Id: extern.h,v 1.89.2.4 2005/09/06 04:56:24 kawahara Exp $
====================================================================*/

JUMAN_KNP_API void init_all();
JUMAN_KNP_API void close_all();
JUMAN_KNP_API void knp_main();

extern int		Thesaurus;
extern int		ParaThesaurus;
extern THESAURUS_FILE	THESAURUS[];

extern int		Revised_para_num;

extern char 	*ErrorComment;
extern char 	PM_Memo[];

extern int		BGHExist;
extern int		SMExist;
extern int		SM2CODEExist;
extern int		SMP2SMGExist;
extern DBM_FILE sm_db;
extern DBM_FILE sm2code_db;
extern DBM_FILE smp2smg_db;

extern SENTENCE_DATA	sentence_data[];

extern int		match_matrix[][BNST_MAX];
extern int		path_matrix[][BNST_MAX];
extern int		restrict_matrix[][BNST_MAX];
extern int		Dpnd_matrix[][BNST_MAX];
extern int		Quote_matrix[][BNST_MAX];
extern int		Mask_matrix[][BNST_MAX];

extern char 	**Options;
extern int		OptAnalysis;
extern int		OptInput;
extern int		OptExpress;
extern int		OptDisplay;
extern int		OptExpandP;
extern int		OptCheck;
extern int		OptUseScase;
extern char 	OptIgnoreChar;
extern int		OptReadFeature;
extern int		OptCopula;
extern VerboseType	VerboseLevel;
extern JUMAN_KNP_DATA	char* Opt_knprc;

extern CLASS	Class[CLASSIFY_NO + 1][CLASSIFY_NO + 1];
extern TYPE 	Type[TYPE_NO];
extern FORM 	Form[TYPE_NO][FORM_NO];
extern int		CLASS_num;

extern HomoRule HomoRuleArray[];
extern int		CurHomoRuleSize;

extern KoouRule KoouRuleArray[];
extern int		CurKoouRuleSize;
extern DpndRule DpndRuleArray[];
extern int		CurDpndRuleSize;

extern BnstRule ContRuleArray[];
extern int		ContRuleSize;

extern	void	*EtcRuleArray;
extern	int 	CurEtcRuleSize;

extern SMLIST	smlist[];


/* 関数プロトタイプ */

/* bnst_compare.c */
int subordinate_level_check(char *cp, BNST_DATA *ptr2);
int levelcmp(char *cp1, char *cp2);
int subordinate_level_comp(BNST_DATA *ptr1, BNST_DATA *ptr2);
int subordinate_level_forbid(char *cp, BNST_DATA *ptr2);
void calc_match_matrix(SENTENCE_DATA *sp);

/* configfile.c */
char *check_dict_filename(const char *file, int flag);
char *check_rule_filename(const char *file);
void init_configfile(const char *opfile);
void server_read_rc(FILE *fp);

/* db.c */
char *db_get(DBM_FILE db, char *buf);
DBM_FILE db_read_open(char *filename);

/* dpnd_analysis.c */
void dpnd_info_to_bnst(SENTENCE_DATA *sp, DPND *dp);
void dpnd_info_to_tag(SENTENCE_DATA *sp, DPND *dp);
int compare_dpnd(SENTENCE_DATA *sp, TOTAL_MGR *new_mgr, TOTAL_MGR *best_mgr);
int after_decide_dpnd(SENTENCE_DATA *sp);
void calc_dpnd_matrix(SENTENCE_DATA *sp);
int relax_dpnd_matrix(SENTENCE_DATA *sp);
void para_postprocess(SENTENCE_DATA *sp);
int detect_dpnd_case_struct(SENTENCE_DATA *sp);
void when_no_dpnd_struct(SENTENCE_DATA *sp);
void check_candidates(SENTENCE_DATA *sp);
void memo_by_program(SENTENCE_DATA *sp);

/* feature.c */
char *check_feature(FEATURE *fp, const char *fname);
void assign_cfeature(FEATURE **fpp, char *fname);
int feature_pattern_match(FEATURE_PATTERN *fr, FEATURE *fd, void *p1, void *p2);
void print_one_feature(char *cp, FILE *filep);
void print_feature(FEATURE *fp, FILE *filep);
int comp_feature(char *data, const char *pattern);
void print_feature2(FEATURE *fp, FILE *filep);
void print_some_feature(FEATURE *fp, FILE *filep);
int feature_AND_match(FEATURE *fp, FEATURE *fd, void *p1, void *p2);
void string2feature_pattern(FEATURE_PATTERN *f, char *cp);
void assign_feature(FEATURE **fpp1, FEATURE **fpp2, void *ptr);
void list2feature_pattern(FEATURE_PATTERN *f, CELL *cell);
void list2feature(CELL *cp, FEATURE **fpp);
void clear_feature(FEATURE **fpp);
void append_feature(FEATURE **fpp, FEATURE *afp);
void delete_cfeature(FEATURE **fpp, const char *type);
void copy_feature(FEATURE **dst_fpp, FEATURE *src_fp);

/* koou.c */
int koou(SENTENCE_DATA *sp);

/* lib_bgh.c */
char *_get_bgh(char *cp, char *arg);
int bgh_code_match(char *c1, char *c2);
int bgh_code_match_for_case(char *cp1, char *cp2);
void init_bgh();
void close_bgh();

/* lib_dt.c */
float dt_classify(char *data, int pp);

/* lib_event.c */
float get_event_value(SENTENCE_DATA *sp1, TAG_DATA *p1, 
				 SENTENCE_DATA *sp2, TAG_DATA *p2);
float get_cf_event_value(CASE_FRAME *cf1, CASE_FRAME *cf2);

/* lib_lib.c */
int str_part_eq(char *dat, char *pat);

/* lib_print.c */
void print_kakari(SENTENCE_DATA *sp, int type);
void _print_bnst(TAG_DATA *ptr);
void print_matrix(SENTENCE_DATA *sp, int type, int L_B);
void print_result(SENTENCE_DATA *sp);
void print_bnst(BNST_DATA *ptr, char *cp);
void check_bnst(SENTENCE_DATA *sp);
void print_para_relation(SENTENCE_DATA *sp);
void assign_para_similarity_feature(SENTENCE_DATA *sp);
void prepare_all_entity(SENTENCE_DATA *sp);

/* lib_scase.c */
void set_pred_voice(BNST_DATA *b_ptr);
void get_scase_code(BNST_DATA *ptr);
char *make_pred_string(TAG_DATA *t_ptr);
void init_scase();
void close_scase();

/* lib_sm.c */
char *_get_ntt(char *cp, char *arg);
char *_process_code(char *code, char *arg);
int _code_match(char *code1,char *code2);
char *_code_flow(char *code);
char *sm2code(char *cp);
char *code2sm(char *cp);
float ntt_code_match(char *c1, char *c2, int flag);
int comp_sm(char *cpp, char *cpd, int start);
int _sm_match_score(char *cpp, char *cpd, int flag);
int sm_time_match(char *c);
void init_ntt();
void close_ntt();
char *_smp2smg(char *cp);
char *smp2smg(char *cpd, int flag);
void merge_smp2smg(BNST_DATA *bp);
int sm_match_check(char *pat, char *codes, int expand);
int assign_sm(BNST_DATA *bp, char *cp);
int sm_all_match(char *c, char *target);
int delete_specified_sm(char *sm, char *del);
void sm2feature(SENTENCE_DATA *sp);
char *check_noun_sm(char *key);

/* main.c */
int one_sentence_analysis(SENTENCE_DATA *sp, FILE *input);
void usage();

/* para_dpnd.c */
int _check_para_d_struct(SENTENCE_DATA *sp, int str, int end,
				int extend_p, int limit, int *s_p);
void init_mask_matrix(SENTENCE_DATA *sp);
int check_dpnd_in_para(SENTENCE_DATA *sp);

/* para_analysis.c */
int para_recovery(SENTENCE_DATA *sp);
int check_para_key(SENTENCE_DATA *sp);
void detect_all_para_scope(SENTENCE_DATA *sp);
int detect_para_scope(SENTENCE_DATA *sp, int para_num, int restrict_p);

/* para_relation.c */
int detect_para_relation(SENTENCE_DATA *sp);

/* para_revision.c */
void revise_para_rel(SENTENCE_DATA *sp, int pre, int pos);
void revise_para_kakari(SENTENCE_DATA *sp, int num, int *array);

/* proper.c */
void ne_para_analysis(SENTENCE_DATA *sp);

/* quote.c */
int quote(SENTENCE_DATA *sp);

/* read_data.c */
int imi2feature(char *str, MRPH_DATA *m_ptr);
int read_mrph(SENTENCE_DATA *sp, FILE *fp);
void change_mrph(MRPH_DATA *m_ptr, FEATURE *f);
void assign_mrph_feature(MrphRule *s_r_ptr, int r_size,
				MRPH_DATA *s_m_ptr, int m_length,
				int mode, int break_mode, int direction);
void assign_general_feature(void *data, int size, int flag);
int make_bunsetsu(SENTENCE_DATA *sp);
int make_bunsetsu_pm(SENTENCE_DATA *sp);
void print_mrphs(SENTENCE_DATA *sp, int flag);
void assign_dpnd_rule(SENTENCE_DATA *sp);
int calc_bnst_length(SENTENCE_DATA *sp, BNST_DATA *b_ptr);
void make_tag_units(SENTENCE_DATA *sp);

/* read_rule.c */
int case2num(const char *cp);
char *get_rulev(char *cp);
void read_homo_rule(char *file_name);
void read_general_rule(RuleVector *rule);
void read_dpnd_rule(char *file_name);
void read_koou_rule(char *file_name);
void read_mrph_rule(char *file_name, MrphRule *rp, int *count, int max);
void read_bnst_rule(char *file_name, BnstRule *rp, int *count, int max);

/* regexp.c */
void store_regexpmrphs(REGEXPMRPHS **mspp, CELL *cell);
void store_regexpbnsts(REGEXPBNSTS **bspp, CELL *cell);
int regexpmrph_match(REGEXPMRPH *ptr1, MRPH_DATA *ptr2);
int regexpmrphrule_match(MrphRule *r_ptr, MRPH_DATA *d_ptr,
				int bw_length, int fw_length);
int regexpbnstrule_match(BnstRule *r_ptr, BNST_DATA *d_ptr,
				int bw_length, int fw_length);
int regexptagrule_match(BnstRule *r_ptr, TAG_DATA *d_ptr,	
				   int bw_length, int fw_length);
int _regexpbnst_match(REGEXPMRPHS *r_ptr, BNST_DATA *b_ptr);

/* thesaurus.c */
void init_thesaurus();
void close_thesaurus();
char *get_str_code(unsigned char *cp, int flag);
void get_bnst_code_all(BNST_DATA *ptr);
void get_bnst_code(BNST_DATA *ptr, int flag);
int code_depth(char *cp, int code_size);
float general_code_match(THESAURUS_FILE *th, char *c1, char *c2);
float calc_similarity(char *exd, char *exp, int expand);
float CalcWordsSimilarity(char *exd, char **exp, int num, int *pos);
float CalcSmWordsSimilarity(char *smd, char **exp, int num, int *pos, char *del, int expand);
void overflowed_function(char *str, int max, const char *function);
char *get_most_similar_code(char *exd, char *exp);

/* tools.c */
void *malloc_data(size_t size, const char *comment);
void *realloc_data(void *ptr, size_t size, const char *comment);
void init_hash();
int hash(unsigned char *key, int keylen);
unsigned char *katakana2hiragana(unsigned char *cp);
char *strdup_with_check(const char *s);

/* tree_conv.c */
int make_dpnd_tree(SENTENCE_DATA *sp);
void init_bnst_tree_property(SENTENCE_DATA *sp);
BNST_DATA *t_add_node(BNST_DATA *parent, BNST_DATA *child, int pos);

char **GetDefinitionFromBunsetsu(BNST_DATA *bp);
int ParseSentence(SENTENCE_DATA *s, char *input);

/* KNP 初期化 */
extern char *Knprule_Dirname;
extern char *Knpdict_Dirname;
extern RuleVector *RULE;
extern int CurrentRuleNum;
extern int RuleNumMax;
extern char *DICT[];

extern GeneralRuleType *GeneralRuleArray;
extern int GeneralRuleNum;
extern int GeneralRuleMax;

/* Server Client extension */
extern JUMAN_KNP_DATA FILE *Infp;
extern JUMAN_KNP_DATA FILE *Outfp;
extern int	 OptMode;

/*====================================================================
				 END
====================================================================*/
