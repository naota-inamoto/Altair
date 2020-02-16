/*====================================================================

			 KNP (Kurohashi-Nagao Parser)

	$Id: main.c,v 1.130.2.8 2005/09/06 06:42:37 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

SENTENCE_DATA	current_sentence_data;
SENTENCE_DATA	sentence_data[SENTENCE_MAX];

MRPH_DATA	mrph_data[MRPH_MAX];		/* �`�ԑf�f�[�^ */
BNST_DATA	bnst_data[BNST_MAX];		/* ���߃f�[�^ */
TAG_DATA	tag_data[MRPH_MAX]; 	/* �^�O�P�ʃf�[�^ */
PARA_DATA	para_data[PARA_MAX];		/* ����f�[�^ */
PARA_MANAGER	para_manager[PARA_MAX]; 	/* ����Ǘ��f�[�^ */
TOTAL_MGR	Best_mgr;			/* �ˑ��E�i��͊Ǘ��f�[�^ */
TOTAL_MGR	Op_Best_mgr;

extern	int 	Revised_para_num;			

extern	char	*ErrorComment;		/* �G���[�R�����g */
extern	char	PM_Memo[DATA_LEN];	/* �p�^�[���}�b�`���� */

extern	int 	match_matrix[BNST_MAX][BNST_MAX];
extern	int 	path_matrix[BNST_MAX][BNST_MAX];
extern	int 	restrict_matrix[BNST_MAX][BNST_MAX];
extern	int 	Dpnd_matrix[BNST_MAX][BNST_MAX]; /* �W��\�� 0, D, P, A */
extern	int 	Quote_matrix[BNST_MAX][BNST_MAX];/* ���ʃ}�X�N 0, 1 */
extern	int 	Mask_matrix[BNST_MAX][BNST_MAX]; /* ����}�X�N
								0:�W��󂯋֎~
								1:�W���OK
								2:�����head��,
								3:�����gap��head�� */
extern	char	**Options;

extern	int 	OptAnalysis;
extern	int 	OptInput;
extern	int 	OptExpress;
extern	int 	OptDisplay;
extern	int 	OptExpandP;
extern	int 	OptCheck;
extern	int 	OptServerFlag;
extern	char	OptIgnoreChar;
extern	int 	OptReadFeature;
extern	int 	OptCopula;

extern	VerboseType VerboseLevel;

/* Server Client Extension */
extern	int 	OptMode;
int 	OptPort = DEFAULT_PORT;
char	OptHostname[SMALL_DATA_LEN];

JUMAN_KNP_DATA FILE *Infp = NULL;
JUMAN_KNP_DATA FILE *Outfp = NULL;

extern FILE 	*Jumanrc_Fileptr;
extern CLASS	Class[CLASSIFY_NO + 1][CLASSIFY_NO + 1];
extern TYPE 	Type[TYPE_NO];
extern FORM 	Form[TYPE_NO][FORM_NO];
extern	int CLASS_num;

#ifndef _WIN32
jmp_buf timeout;
#endif
int ParseTimeout = DEFAULT_PARSETIMEOUT;
JUMAN_KNP_DATA char *Opt_knprc = NULL;

extern int	SOTO_THRESHOLD;
extern int	DISTANCE_STEP;
extern int	RENKAKU_STEP;
extern int	STRONG_V_COST;
extern int	ADJACENT_TOUTEN_COST;
extern int	LEVELA_COST;
extern int	TEIDAI_STEP;
extern int	EX_match_qua;
extern int	EX_match_unknown;
extern int	EX_match_sentence;
extern int	EX_match_tim;
extern int	EX_match_subject;

#ifdef	_WINDOWS
static char* buntou = NULL;
static char* bunmatsu = NULL;
static char* taigen = NULL;
static char* sahen = NULL;
static char buntou_2[] = "����";
static char bunmatsu_2[] = "����";
static char taigen_2[] = "�̌�";
static char sahen_2[] = "�T��";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	buntou = toStringEUC(buntou_2);
	bunmatsu = toStringEUC(bunmatsu_2);
	taigen = toStringEUC(taigen_2);
	sahen = toStringEUC(sahen_2);
#endif
}
void final_str_main() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(buntou); buntou = NULL;
	free(bunmatsu); bunmatsu = NULL;
	free(taigen); taigen = NULL;
	free(sahen); sahen = NULL;
#endif
}
#ifdef	_LINUX
static char buntou[] = "����";
static char bunmatsu[] = "����";
static char taigen[] = "�̌�";
static char sahen[] = "�T��";
#endif

extern void decide_head_ptr(BNST_DATA *ptr);
extern void make_Jiritu_Go(SENTENCE_DATA *sp, BNST_DATA *ptr);
extern void make_tag_units_pm(SENTENCE_DATA *sp);

#ifndef JUMAN_KNP
/*==================================================================*/
		   void option_proc(int argc, char **argv)
/*==================================================================*/
{
	int i, count = 0;

	/* �������� */

	OptAnalysis = OPT_DPND;
	OptInput = OPT_RAW;
	OptExpress = OPT_NOTAGTREE;
	OptDisplay = OPT_NORMAL;
	OptExpandP = FALSE;
	OptCheck = FALSE;
	OptUseScase = TRUE;
	OptServerFlag = 0;
	OptIgnoreChar = '\0';
	OptReadFeature = 0;
	OptCopula = 0;

	/* �I�v�V�����̕ۑ� */
	Options = (char **)malloc_data(sizeof(char *) * argc, "option_proc");
	for (i = 1; i < argc; i++) {
	if (**(argv + i) == '-') {
		*(Options + count++) = art_strdup(*(argv + i) + 1);
	}
	}
	*(Options + count) = NULL;

	while ((--argc > 0) && ((*++argv)[0] == '-')) {
	if (str_eq(argv[0], "-dpnd"))		  OptAnalysis = OPT_DPND;
	else if (str_eq(argv[0], "-bnst"))	  OptAnalysis = OPT_BNST;
	else if (str_eq(argv[0], "-assignf")) OptAnalysis = OPT_AssignF;
	else if (str_eq(argv[0], "-tree"))	  OptExpress  = OPT_NOTAGTREE;
	else if (str_eq(argv[0], "-treef"))   OptExpress  = OPT_TREEF;
	else if (str_eq(argv[0], "-sexp"))	  OptExpress  = OPT_SEXP;
	else if (str_eq(argv[0], "-tab"))	  OptExpress  = OPT_NOTAG;
	else if (str_eq(argv[0], "-tagtab"))  OptExpress  = OPT_TAB;
	else if (str_eq(argv[0], "-tag"))	  OptExpress  = OPT_TAB;
	else if (str_eq(argv[0], "-tagtree")) OptExpress  = OPT_TREE;
	else if (str_eq(argv[0], "-entity"))  OptDisplay  = OPT_ENTITY;
	else if (str_eq(argv[0], "-normal"))  OptDisplay  = OPT_NORMAL;
	else if (str_eq(argv[0], "-detail"))  OptDisplay  = OPT_DETAIL;
	else if (str_eq(argv[0], "-debug"))   OptDisplay  = OPT_DEBUG;
	else if (str_eq(argv[0], "-expand"))  OptExpandP  = TRUE;
	else if (str_eq(argv[0], "-S")) 	  OptMode	  = SERVER_MODE;
	else if (str_eq(argv[0], "-check"))   OptCheck	  = TRUE;
	else if (str_eq(argv[0], "-i")) {
		argv++; argc--;
		if (argc < 1) usage();
		OptIgnoreChar = *argv[0];
	}
	else if (str_eq(argv[0], "-N")) {
		argv++; argc--;
		if (argc < 1) usage();
		OptPort = atol(argv[0]);
	}
	else if (str_eq(argv[0], "-C")) {
		OptMode = CLIENT_MODE;
		argv++; argc--;
		if (argc < 1) usage();
		art_strcpy(OptHostname, SMALL_DATA_LEN, argv[0]);
	}
	/* daemon�ɂ��Ȃ��ꍇ (cygwin�p) */
	else if (str_eq(argv[0], "-F")) {
		OptMode = SERVER_MODE;
		OptServerFlag = OPT_SERV_FORE;
	}
	else if (str_eq(argv[0], "-timeout")) {
		argv++; argc--;
		if (argc < 1) usage();
		ParseTimeout = atoi(argv[0]);
	}
	else if (str_eq(argv[0], "-scode")) {
		argv++; argc--;
		if (argc < 1) usage();
		if (!art_strcasecmp(argv[0], "ntt")) {
		Thesaurus = USE_NTT;
		}
		else if (!art_strcasecmp(argv[0], "bgh")) {
		Thesaurus = USE_BGH;
		}
		else {
		usage();
		}
	}
	else if (str_eq(argv[0], "-para-scode")) {
		argv++; argc--;
		if (argc < 1) usage();
		if (!art_strcasecmp(argv[0], "ntt")) {
		ParaThesaurus = USE_NTT;
		}
		else if (!art_strcasecmp(argv[0], "bgh")) {
		ParaThesaurus = USE_BGH;
		}
		else if (!art_strcasecmp(argv[0], "none")) {
		ParaThesaurus = USE_NONE;
		}
		else {
		usage();
		}
	}
	else if (str_eq(argv[0], "-no-scase")) {
		OptUseScase = FALSE;
	}
	else if (str_eq(argv[0], "-r")) {
		argv++; argc--;
		if (argc < 1) usage();
		Opt_knprc = argv[0];
	}
	else if (str_eq(argv[0], "-v")) {
		argv++; argc--;
		if (argc < 1) usage();
		VerboseLevel = (VerboseType)atoi(argv[0]);
	}
	else if (str_eq(argv[0], "-read-feature")) {
		OptReadFeature = 1;
	}
	else if (str_eq(argv[0], "-copula")) {
		OptCopula = 1;
	}
	else {
		usage();
	}
	}
	if (argc != 0) {
	usage();
	}

	if ((OptExpress == OPT_NOTAG || OptExpress == OPT_NOTAGTREE) && 
	(OptAnalysis == OPT_CASE || OptAnalysis == OPT_CASE2)) {
	usage();
	}
}
#endif

/*==================================================================*/
			void init_juman(void)
/*==================================================================*/
{
	int i;

	/* rcfile ����������
	   1. -r �Ŏw�肳�ꂽ�t�@�C��
	   2. $HOME/.knprc
	   3. KNP_RC_DEFAULT (compile��)
	   �� rcfile���Ȃ���΃G���[
	*/

#ifndef JUMAN_KNP
	grammar(NULL);				/* ���@���� */
	katuyou(NULL);				/* ���p���� */
#endif

	for (i = 1; Class[i][0].id; i++);
	CLASS_num = i;
}

/*==================================================================*/
			void read_rules(void)
/*==================================================================*/
{
	int i;

	for (i = 0; i < CurrentRuleNum; i++) {
	/* ���`�ً`�ꃋ�[�� */
	if ((RULE+i)->type == HomoRuleType) {
		read_homo_rule((RULE+i)->file);
	}
	/* �`�ԑf���[�� or ���߃��[�� */
	else if ((RULE+i)->type == MorphRuleType || 
		 (RULE+i)->type == TagRuleType ||	
		 (RULE+i)->type == BnstRuleType) {
		read_general_rule(RULE+i);
	}
	/* �W��󂯃��[�� */
	else if ((RULE+i)->type == DpndRuleType) {
		read_dpnd_rule((RULE+i)->file);
	}
	/* �ĉ��\�����[�� */
	else if ((RULE+i)->type == KoouRuleType) {
		read_koou_rule((RULE+i)->file);
	}
	/* ���������̃��[�� */
	else if ((RULE+i)->type == ContextRuleType) {
		read_bnst_rule((RULE+i)->file, ContRuleArray, &ContRuleSize, ContRule_MAX);
	}
	}
}

void free_mrph(REGEXPMRPHS *ms) {
	int i, j;

	if (ms == NULL) return;

	if (ms->mrph) {
		for (i = 0; i < ms->mrphsize; i++) {
			for (j = 0; j < 64; j++) {
				char *s;
				s = (ms->mrph + i)->Katuyou_Kei[j];
				if (s) free(s);
				s = (ms->mrph + i)->Goi[j];
				if (s) free(s);
			}
			for (j = 0; (ms->mrph + i)->f_pattern.fp[j]; j++) {
				clear_feature(&((ms->mrph + i)->f_pattern.fp[j]));
			}
		}
		free(ms->mrph);
	}

	free(ms);
}
void free_bnst(REGEXPBNSTS *bs) {
	int i, j;

	if (bs == NULL) return;

	if (bs->bnst) {
		for (i = 0; i < bs->bnstsize; i++) {
			free_mrph((bs->bnst + i)->mrphs);
			for (j = 0; (bs->bnst + i)->f_pattern.fp[j]; j++) {
				clear_feature(&((bs->bnst + i)->f_pattern.fp[j]));
			}
		}
		free(bs->bnst);
	}

	free(bs);
}
void free_rules() {
	int i, j, k;

	{
		HomoRule *rp = HomoRuleArray;
		for (i = 0; i < CurHomoRuleSize; i++) {
			free_mrph(rp->pre_pattern);
			free_mrph(rp->pattern);
			if (rp->f) clear_feature(&(rp->f));
			rp++;
		}
		CurHomoRuleSize = 0;
	}
	{
		for (i = 0; i < GeneralRuleNum; i++) {
			if ((GeneralRuleArray+i)->type == MorphRuleType) {
				MrphRule *rp = (MrphRule *)(GeneralRuleArray+i)->RuleArray;
				for (j = 0; j < (GeneralRuleArray+i)->CurRuleSize; j++) {
					free_mrph(rp->pre_pattern);
					free_mrph(rp->self_pattern);
					free_mrph(rp->post_pattern);
					if (rp->f) clear_feature(&(rp->f));
					rp++;
				}
				free((GeneralRuleArray+i)->RuleArray);
			} else if ((GeneralRuleArray+i)->type == TagRuleType || 
					(GeneralRuleArray+i)->type == BnstRuleType) {
				BnstRule *rp = (BnstRule *)(GeneralRuleArray+i)->RuleArray;
				for (j = 0; j < (GeneralRuleArray+i)->CurRuleSize; j++) {
					free_bnst(rp->pre_pattern);
					free_bnst(rp->self_pattern);
					free_bnst(rp->post_pattern);
					if (rp->f) clear_feature(&(rp->f));
					rp++;
				}
				free((GeneralRuleArray+i)->RuleArray);
			}
		}
		GeneralRuleNum = 0;
		GeneralRuleMax = 0;

		free(GeneralRuleArray);
		GeneralRuleArray = NULL;
	}
	{
		DpndRule *rp = DpndRuleArray;
		for (i = 0; i < CurDpndRuleSize; i++) {
			for (j = 0; rp->dependant.fp[j]; j++) {
				clear_feature(&(rp->dependant.fp[j]));
			}
			for (j = 0; j < rp->dpnd_type[j]; j++) {
				for (k = 0; rp->governor[j].fp[k]; k++) {
					clear_feature(&(rp->governor[j].fp[k]));
				}
			}
			for (j = 0; rp->barrier.fp[j]; j++) {
				clear_feature(&(rp->barrier.fp[j]));
			}
			rp++;
		}
		CurDpndRuleSize = 0;
	}
	{
		KoouRule *rp = KoouRuleArray;
		for (i = 0; i < CurKoouRuleSize; i++) {
			free_mrph(rp->start_pattern);
			free_mrph(rp->end_pattern);
			free_mrph(rp->uke_pattern);
			rp++;
		}
		CurKoouRuleSize = 0;
	}
	{
		BnstRule *rp = ContRuleArray;
		for (i = 0; i < ContRuleSize; i++) {
			free_bnst(rp->pre_pattern);
			free_bnst(rp->self_pattern);
			free_bnst(rp->post_pattern);
			if (rp->f) clear_feature(&(rp->f));
			rp++;
		}
		ContRuleSize = 0;
	}

	for (i = 0; i < CurrentRuleNum; i++) {
		if ((RULE+i)->file) {
			free((RULE+i)->file);
		}
	}

	CurrentRuleNum = 0;

	free(RULE);
	RULE = NULL;
}


void final_str_bnst_compare();
void final_str_configfile();
void final_str_dpnd_analysis();
void final_str_feature();
void final_str_lib_print();
void final_str_lib_ps();
void final_str_lib_scase();
void final_str_main();
void final_str_para_analysis();
void final_str_para_dpnd();
void final_str_para_relation();
void final_str_proper();
void final_str_quote();
void final_str_read_data();
void final_str_read_rule();
void final_str_thesaurus();
void final_str_tree_conv();

void final_dt();
/*==================================================================*/
			   JUMAN_KNP_API void close_all()
/*==================================================================*/
{
	close_thesaurus();
	close_scase();

#ifdef DB3DEBUG
	db_teardown();
#endif
	int i;

	if (Knprule_Dirname) {
		free(Knprule_Dirname);
		Knprule_Dirname = NULL;
	}
	if (Knpdict_Dirname) {
		free(Knpdict_Dirname);
		Knpdict_Dirname = NULL;
	}

	for (i = 0; i < DICT_MAX; i++) {
		if (DICT[i]) {
			free(DICT[i]);
		}
	}

	for (int i = 0; i < 3; i++) {
		if (THESAURUS[i].path) {
			free(THESAURUS[i].path);
		}
		if (THESAURUS[i].name) {
			free(THESAURUS[i].name);
		}
	}

	free_rules();

	final_dt();

	final_str_bnst_compare();
	final_str_configfile();
	final_str_dpnd_analysis();
	final_str_feature();
	final_str_lib_print();
	final_str_lib_ps();
	final_str_lib_scase();
	final_str_main();
	final_str_para_analysis();
	final_str_para_dpnd();
	final_str_para_relation();
	final_str_proper();
	final_str_quote();
	final_str_read_data();
	final_str_read_rule();
	final_str_thesaurus();
	final_str_tree_conv();
}

#ifndef _WIN32
/*==================================================================*/
		static void timeout_function(int /*sig*/ )
/*==================================================================*/
{
	longjmp(timeout, 1);
}
#endif

/*==================================================================*/
			   JUMAN_KNP_API void init_all()
/*==================================================================*/
{
	int i;

	/* ������ */

#ifdef DB3DEBUG
	db_setup();
#endif
	init_hash();
	init_configfile(Opt_knprc); /* �e��t�@�C���ݒ菉���� */
	init_juman();	/* JUMAN�֌W */
	init_thesaurus();	/* �V�\�[���X�I�[�v�� */
	init_scase();	/* �\�w�i�����I�[�v�� */

	/* �`�ԑf, ���ߏ��̏����� */
	memset(mrph_data, 0, sizeof(MRPH_DATA)*MRPH_MAX);
	memset(bnst_data, 0, sizeof(BNST_DATA)*BNST_MAX);
	memset(tag_data, 0, sizeof(MRPH_DATA)*TAG_MAX);

	current_sentence_data.mrph_data = mrph_data;
	current_sentence_data.bnst_data = bnst_data;
	current_sentence_data.tag_data = tag_data;
	current_sentence_data.para_data = para_data;
	current_sentence_data.para_manager = para_manager;
	current_sentence_data.Sen_num = 0;	/* ���ꂾ���͑����Ă��� */
	current_sentence_data.Mrph_num = 0;
	current_sentence_data.Bnst_num = 0;
	current_sentence_data.New_Bnst_num = 0;
	current_sentence_data.Tag_num = 0;
	current_sentence_data.Best_mgr = &Best_mgr;
	current_sentence_data.KNPSID = NULL;
	current_sentence_data.Comment = NULL;

	for (i = 0; i < BNST_MAX; i++) {
	 current_sentence_data.bnst_data[i].f = NULL;
	}

#ifndef _WIN32
	signal(SIGALRM, timeout_function);
#endif
}

/*==================================================================*/
	  int one_sentence_analysis(SENTENCE_DATA *sp, FILE *input)
/*==================================================================*/
{
	int flag, i;
	int relation_error, d_struct_error;

	init_str();

	sp->Sen_num++;
	sp->available = 1;

	/* �`�ԑf�̓ǂݍ��� */

	if ((flag = read_mrph(sp, input)) == EOF) return EOF;
	if (flag == FALSE) return FALSE; /* EOS�����Ȃ���̕� */

	/* �`�ԑf�ւ̈Ӗ����t�^ (�ŗL�\����͂̂Ƃ�) */

	if (0 && SMExist == 1 /*TRUE*/) {
	char *code;
	for (i = 0; i < sp->Mrph_num; i++) {
		code = get_str_code((unsigned char*)sp->mrph_data[i].Goi, USE_NTT);
		if (code) {
		art_strcpy(sp->mrph_data[i].SM, SM_CODE_SIZE+1, code);
		free(code);
		}
		else {
		sp->mrph_data[i].SM[0] = '\0';
		}
	}
	}

	/* �`�ԑf�ւ�FEATURE�t�^ */

	assign_cfeature(&(sp->mrph_data[0].f), buntou);
	assign_cfeature(&(sp->mrph_data[sp->Mrph_num-1].f), bunmatsu);
	assign_general_feature(sp->mrph_data, sp->Mrph_num, MorphRuleType);

	/* �`�ԑf�𕶐߂ɂ܂Ƃ߂� */

	if (OptInput == OPT_RAW) {
	if (make_bunsetsu(sp) == FALSE) return FALSE;
	}
	else {
	if (make_bunsetsu_pm(sp) == FALSE) return FALSE;
	}

	/* ���߉������̏ꍇ */

	if (OptAnalysis == OPT_BNST) return TRUE;

	/* ���߂ւ̈Ӗ����t�^ */

	for (i = 0; i < sp->Bnst_num; i++) {
	decide_head_ptr(sp->bnst_data + i);
	make_Jiritu_Go(sp, sp->bnst_data + i);
	get_bnst_code_all(sp->bnst_data + i);
	}

	/* ���߂ւ�FEATURE�t�^ */

	assign_cfeature(&(sp->bnst_data[0].f), buntou);
	if (sp->Bnst_num > 0)
	assign_cfeature(&(sp->bnst_data[sp->Bnst_num - 1].f), bunmatsu);
	else
	assign_cfeature(&(sp->bnst_data[0].f), bunmatsu);
	assign_general_feature(sp->bnst_data, sp->Bnst_num, BnstRuleType);

	/* �T�ϓ����ȊO�̓����̈Ӗ��f�������͈̂Ӗ����Ȃ�
	   ���[���K�p�O�ɂ́Afeature���Ȃ����߂Ƀ`�F�b�N�ł��Ȃ�
	   �� ���[���K�p��ɈӖ��f�������Ȃ��̂�:
		   => �Ӗ��f�̓��[���Ŏg����������Ȃ��̂ŁA���[���K�p�O�ɗ^���Ă��� */
	for (i = 0; i < sp->Bnst_num; i++) {
	if (!check_feature((sp->bnst_data+i)->f, taigen) && 
		!check_feature((sp->bnst_data+i)->f, sahen)) {
		(sp->bnst_data+i)->SM_code[0] = '\0';
		delete_cfeature(&((sp->bnst_data+i)->f), "SM");
	}
	}

	/* �^�O�P�ʍ쐬 */
	if (OptExpress != OPT_NOTAG && OptExpress != OPT_NOTAGTREE) {
	if (OptInput == OPT_RAW ||	
		(OptInput & OPT_INPUT_BNST)) {
		make_tag_units(sp);
	}
	else {
		make_tag_units_pm(sp);
	}
	}

	if (OptDisplay == OPT_DETAIL || OptDisplay == OPT_DEBUG)
	print_mrphs(sp, 0);

	/* FEATURE�t�^�����̏ꍇ */

	if (OptAnalysis == OPT_AssignF) return TRUE;

	assign_dpnd_rule(sp);			/* �W��󂯋K�� */

	if (OptDisplay == OPT_DETAIL || OptDisplay == OPT_DEBUG)
	check_bnst(sp);

	/**************/
	/* �{�i�I��� */
	/**************/

	if (OptInput & OPT_PARSED) {
	dpnd_info_to_bnst(sp, &(sp->Best_mgr->dpnd));	
	para_recovery(sp);
	after_decide_dpnd(sp);
	goto PARSED;
	}

	calc_dpnd_matrix(sp);			/* �ˑ��\���v�Z */
	if (OptDisplay == OPT_DEBUG) print_matrix(sp, PRINT_DPND, 0);

	/* �ĉ��\���̏��� */

	if (koou(sp) == 1 /*TRUE*/ && OptDisplay == OPT_DEBUG)
	print_matrix(sp, PRINT_DPND, 0);

	/* �����ʂ̏��� */

	if ((flag = quote(sp)) == 1 /*TRUE*/ && OptDisplay == OPT_DEBUG)
	print_matrix(sp, PRINT_QUOTE, 0);

	if (flag == CONTINUE) return FALSE;

	/* �W��󂯊֌W���Ȃ��ꍇ�̒o�� */
	
	if (relax_dpnd_matrix(sp) == 1 /*TRUE*/ && OptDisplay == OPT_DEBUG) {
	fprintf(Outfp, "Relaxation ... \n");
	print_matrix(sp, PRINT_DPND, 0);
	}

	/****************/
	/* ����\����� */
	/****************/

	init_mask_matrix(sp);
	sp->Para_num = 0;
	sp->Para_M_num = 0;
	relation_error = 0;
	d_struct_error = 0;
	Revised_para_num = -1;

	if ((flag = check_para_key(sp)) > 0) {
	calc_match_matrix(sp);		/* ���ߊԗގ��x�v�Z */
	detect_all_para_scope(sp);		/* ����\������ */
	do {
		assign_para_similarity_feature(sp);
		if (OptDisplay == OPT_DETAIL || OptDisplay == OPT_DEBUG) {
		print_matrix(sp, PRINT_PARA, 0);
		/*
		  print_matrix2ps(sp, PRINT_PARA, 0);
		  exit(0);
		*/
		}
		/* ����\���Ԃ̏d�Ȃ��� */
		if (detect_para_relation(sp) == FALSE) {
		relation_error++;
		continue;
		}
		if (OptDisplay == OPT_DEBUG) print_para_relation(sp);
		/* ����\�����̈ˑ��\���`�F�b�N */
		if (check_dpnd_in_para(sp) == FALSE) {
		d_struct_error++;
		continue;
		}
		if (OptDisplay == OPT_DEBUG) print_matrix(sp, PRINT_MASK, 0);
		goto ParaOK;		/* ����\����͐��� */
	} while (relation_error <= 3 &&
		 d_struct_error <= 3 &&
		 detect_para_scope(sp, Revised_para_num, TRUE) == 1 /*TRUE*/);
	ErrorComment = art_strdup("Cannot detect consistent CS scopes");
	init_mask_matrix(sp);
	}
	else if (flag == CONTINUE)
	return FALSE;

 ParaOK:
	/********************/
	/* �ˑ��E�i�\����� */
	/********************/
	para_postprocess(sp);	/* �econjunct��head����̌W���� */

#ifndef _WIN32
	alarm(0);
	alarm(ParseTimeout);
#endif
	if (detect_dpnd_case_struct(sp) == FALSE) {
	sp->available = 0;
	ErrorComment = art_strdup("Cannot detect dependency structure");
	when_no_dpnd_struct(sp);	/* �W��󂯍\�������܂�Ȃ��ꍇ
					   ���ׂĕ��߂��ׂɌW��ƈ��� */
	}
	else if (OptCheck == 1 /*TRUE*/) {
	check_candidates(sp);
	}
#ifndef _WIN32
	alarm(0);
#endif

PARSED:
	/* �W��󂯏��� bnst �\���̂ɋL�� */
	dpnd_info_to_bnst(sp, &(sp->Best_mgr->dpnd));	
	{
		int i, j;
		for (i = 0; i < sp->Para_num; i++) {
			for (j = 0; sp->para_data[i].f_pattern.fp[j]; j++) {
				clear_feature(&(sp->para_data[i].f_pattern.fp[j]));
			}
		}
	}
	para_recovery(sp);

	if (OptExpress != OPT_NOTAG && OptExpress != OPT_NOTAGTREE) {
	dpnd_info_to_tag(sp, &(sp->Best_mgr->dpnd));
	}

	/* ����\�����݂ČŗL�\���F�����s�� */
	ne_para_analysis(sp);

	memo_by_program(sp);	/* �����ւ̏������� */

	return TRUE;
}

/*==================================================================*/
			   JUMAN_KNP_API void knp_main()
/*==================================================================*/
{
	int i, success = 1, flag;

	SENTENCE_DATA *sp = &current_sentence_data;

	/* ���[���ǂݍ���
	   Server Mode �ɂ����āA�ǂݍ��ރ��[���̕ύX�����肦��̂ŁA�����ōs�� */
	read_rules();

	while ( 1 ) {

	/* Server Mode �̏ꍇ �O��̏o�͂��������ĂȂ��ꍇ��	
	   ERROR �Ƃ͂� Server/Client ���[�h�̏ꍇ��,�o�͂̓���������ōs�� */
	if (!success && OptMode == SERVER_MODE) {
		fprintf(Outfp, "EOS ERROR\n");
		fflush(Outfp);
	}

	/********************/
	/* �O�̉�͂̌�n�� */
	/********************/

	/* �^�C���A�E�g�� */

#if 0
//#ifndef _WIN32
	if (setjmp(timeout)) {
		/* timeout��������stderr�ɏo�� */
		fprintf(stderr, ";; Parse timeout.\n;; %s (", sp->KNPSID);
		for (i = 0; i < sp->Mrph_num; i++)
		fprintf(stderr, "%s", sp->mrph_data[i].Goi2);
		fprintf(stderr, ")\n");

		ErrorComment = strdup("Parse timeout");
		when_no_dpnd_struct(sp);
		dpnd_info_to_bnst(sp, &(sp->Best_mgr->dpnd));
		if (OptExpress != OPT_NOTAG  && OptExpress != OPT_NOTAGTREE) {
		dpnd_info_to_tag(sp, &(sp->Best_mgr->dpnd));	
		}
		print_result(sp);
		fflush(Outfp);

		/* �Ƃ肠�����I��邱�Ƃ� */
		close_all();
		exit(100);
	}
#endif

	/* ������ */
	if (sp->KNPSID) {
		free(sp->KNPSID);
		sp->KNPSID = NULL;
	}
	if (sp->Comment) {
		free(sp->Comment);
		sp->Comment = NULL;
	}

	/* FEATURE �̏����� */
	for (i = 0; i < sp->Mrph_num; i++) {
		clear_feature(&(sp->mrph_data[i].f));
	}
	for (i = 0; i < sp->Bnst_num; i++) {
		clear_feature(&(sp->bnst_data[i].f));
	}
	for (i = 0; i < sp->Tag_num; i++) {
		clear_feature(&(sp->tag_data[i].f));
	}

	/* New_Bnst�͂��Ƃ���pointer */
	for (i = sp->Bnst_num; i < sp->Bnst_num + sp->Max_New_Bnst_num; i++) {
		(sp->bnst_data+i)->f = NULL;
	}
	for (i = sp->Tag_num; i < sp->Tag_num + sp->New_Tag_num; i++) {
		(sp->tag_data+i)->f = NULL;
	}


	/**************/
	/* ���C����� */
	/**************/

	success = 0;

	if ((flag = one_sentence_analysis(sp, Infp)) == EOF) break;
	if (flag == FALSE) { /* ��͎��s���ɂ͕��̐��𑝂₳�Ȃ� */
		sp->Sen_num--;		
		continue;
	}

	/* entity ���� feature �̍쐬 */
	if (OptDisplay	== OPT_ENTITY) {
		prepare_all_entity(sp);
	}

	/************/
	/* ���ʕ\�� */
	/************/

	if (OptAnalysis == OPT_BNST) {
		print_mrphs(sp, 0);
	} else {
		print_result(sp);
	}
	fflush(Outfp);

	success = 1;	/* OK ���� */
	}
}

//#ifndef _WIN32
#if 0
/*==================================================================*/
			  void server_mode()
/*==================================================================*/
{
	/* �T�[�o���[�h */

	int i, sfd, fd;
	struct sockaddr_in sin;
	FILE *pidfile;
	struct passwd *ent_pw;

	/* �V�O�i������ */
	void sig_child()
	{
		int status;
		while(waitpid(-1, &status, WNOHANG) > 0) {};	
		signal(SIGCHLD, sig_child); 
	}

	void sig_term()
	{
		shutdown(sfd,2);
		shutdown(fd, 2);
		exit(0);
	}

	if (OptServerFlag != OPT_SERV_FORE) {
	/* parent */
	if ((i = fork()) > 0) {
		return;
	}
	else if (i == -1) {
		fprintf(stderr, ";; unable to fork new process\n");
		return;
	}
	/* child */
	}

	signal(SIGHUP,	SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, sig_term);
	signal(SIGINT,	sig_term);
	signal(SIGQUIT, sig_term);
	signal(SIGCHLD, sig_child);
	
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	fprintf(stderr,";; socket error\n");
	exit(1);
	}
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_port		= htons(OptPort);
	sin.sin_family		= AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	
	/* bind */	
	if (bind(sfd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
	fprintf(stderr, ";; bind error\n");
	close(sfd);
	exit(1);
	}
	
	/* listen */	
	if (listen(sfd, SOMAXCONN) < 0) {
	fprintf(stderr, ";; listen error\n");
	close(sfd);
	exit(1);
	}

	/* make pid file */
	umask(022);
	pidfile = fopen(KNP_PIDFILE, "w");
	if (!pidfile) {
	fputs(";; can't write pidfile: " KNP_PIDFILE "\n", stderr);
	}
	else {
	fprintf(pidfile, "%d\n", getpid());
	fclose(pidfile);
	}
	umask(0);

	/* change uid and gid for security */
	ent_pw = getpwnam(KNP_SERVER_USER);
	if(ent_pw){
	gid_t dummy;
	struct group *gp;
	/* remove all supplementary groups */
	setgroups(0, &dummy);
	if ((gp = getgrgid(ent_pw->pw_gid)))
		setgid(gp->gr_gid); 
	/* finally drop root */
	setuid(ent_pw->pw_uid);
	}

	/* accept loop */
	while (1) {
	int pid;

	if ((fd = accept(sfd, NULL, NULL)) < 0) {
		if (errno == EINTR) 
		continue;
		fprintf(stderr, ";; accept error\n");
		close(sfd);
		exit(1);
	}
	
	if ((pid = fork()) < 0) {
		fprintf(stderr, ";; fork error\n");
		sleep(1);
		continue;
	}

	/* �q�� */
	if (pid == 0) {
		char buf[1024];

		/* ? */
		chdir("/tmp");

		close(sfd);
		Infp  = fdopen(fd, "r");
		Outfp = fdopen(fd, "w");

		/* ���A */
		fprintf(Outfp, "200 Running KNP Server\n");
		fflush(Outfp);

		/* �I�v�V������� */
		while (fgets(buf, sizeof(buf), Infp)) {

		/* QUIT */
		if (strncasecmp(buf, "QUIT", 4) == 0) {
			fprintf(Outfp, "200 OK Quit\n");
			fflush(Outfp);
			exit(0);
		}

		if (strncasecmp(buf, "RC", 2) == 0) {
			server_read_rc(Infp);
			fprintf(Outfp, "200 OK\n");
			fflush(Outfp);
			continue;
		}

		/* RUN */
		/* Option ��͂� strstr �Ȃ񂩂ł��Ȃ肢��������	
		   �܂�Ԉ�����I�v�V�����̓G���[�ɂȂ�Ȃ� */
		if (strncasecmp(buf, "RUN", 3) == 0) {
			char *p;

			if (strstr(buf, "-case"))	OptAnalysis = OPT_CASE;
			if (strstr(buf, "-case2"))	OptAnalysis = OPT_CASE2;
			if (strstr(buf, "-dpnd"))	OptAnalysis = OPT_DPND;
			if (strstr(buf, "-bnst"))	OptAnalysis = OPT_BNST;
			if (strstr(buf, "-tree"))	OptExpress = OPT_TREE;
			if (strstr(buf, "-sexp"))	OptExpress = OPT_SEXP;
			if (strstr(buf, "-tab"))	OptExpress = OPT_TAB;
			if (strstr(buf, "-normal")) OptDisplay = OPT_NORMAL;
			if (strstr(buf, "-detail")) OptDisplay = OPT_DETAIL;
			if (strstr(buf, "-debug"))	OptDisplay = OPT_DEBUG;
			if (strstr(buf, "-expand")) OptExpandP = TRUE;
			if ((p = strstr(buf, "-i")) != NULL) {
			p += 3;
			while(*p != '\0' && (*p == ' ' || *p == '\t')) p++;
			if (*p != '\0') OptIgnoreChar = *p;
			}	
			fprintf(Outfp, "200 OK option=[Analysis=%d Express=%d"
				" Display=%d IgnoreChar=%c]\n",
				OptAnalysis, OptExpress, OptDisplay, OptIgnoreChar);
			fflush(Outfp);
			break;
		} else {
			fprintf(Outfp, "500 What?\n");
			fflush(Outfp);
		}
		}

		/* ��� */
		knp_main();

		/* �㏈�� */
		shutdown(fd, 2);
		fclose(Infp);
		fclose(Outfp);
		close(fd);
		exit(0); /* ���ꂵ�Ȃ��Ƒ�ςȂ��ƂɂȂ邩�� */
	}

	/* �e */
	close(fd);
	}
}

/*==================================================================*/
			  void client_mode()
/*==================================================================*/
{
	/* �N���C�A���g���[�h (TCP/IP�Őڑ����邾��) */

	struct sockaddr_in sin;
	struct hostent *hp;
	int fd;
	FILE *fi, *fo;
	char *p;
	char buf[1024*8];
	char option[1024];
	int  port = DEFAULT_PORT;
	int  strnum = 0;

	/* ������𑗂��āA�X�e�[�^�X�R�[�h��Ԃ� */	
	int send_string(char *str)
	{
		int len, result = 0;
		char buf[1024];
	
		if (str != NULL){
		fwrite(str, sizeof(char), strlen(str), fo);
		fflush(fo);
		}

		while (fgets(buf, sizeof(buf)-1, fi) != NULL){
		len = strlen(buf);
		if (len >= 3 && buf[3] == ' ') {
			buf[3] = '\0';
			result = atoi(&buf[0]);
			break;
		}
		}

		return result;
	}	

	/* host:port �Ƃ����`�̏ꍇ */
	if ((p = strchr(OptHostname, ':')) != NULL) {
	*p++ = '\0';
	port = atoi(p);
	}

	/* �Ȃ��鏀�� */
	if ((hp = gethostbyname(OptHostname)) == NULL) {
	fprintf(stderr, ";; host unkown\n");
	exit(1);
	}
	
	while ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
	fprintf(stderr, ";; socket error\n");
	exit(1);
	}
	
	sin.sin_family = AF_INET;
	sin.sin_port   = htons(port);
	sin.sin_addr = *((struct in_addr * )hp->h_addr);

	if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
	fprintf(stderr, ";; connect error\n");
	exit(1);
	}

	/* Server �p�Ƃ̒ʐM�n���h�����쐬 */
	if ((fi = fdopen(fd, "r")) == NULL || (fo = fdopen(fd, "w")) == NULL) {
	close(fd);
	fprintf(stderr, ";; fd error\n");
	exit(1);
	}

	/* ���A */
	if (send_string(NULL) != 200) {
	fprintf(stderr, ";; greet error\n");
	exit(1);
	}

	/* �I�v�V������� (����������) */
	option[0] = '\0';
	switch (OptAnalysis) {
	case OPT_CASE: strcat(option, " -case"); break;
	case OPT_DPND: strcat(option, " -dpnd"); break;
	case OPT_BNST: strcat(option, " -bnst"); break;
	}

	switch (OptExpress) {
	case OPT_TREE: strcat(option, " -tree"); break;
	case OPT_SEXP: strcat(option, " -sexp"); break;
	case OPT_TAB:  strcat(option, " -tab");  break;
	}

	switch (OptDisplay) {
	case OPT_NORMAL: strcat(option, " -normal"); break;
	case OPT_DETAIL: strcat(option, " -detail"); break;
	case OPT_DEBUG:  strcat(option, " -debug");  break;
	}
	
	if (OptExpandP) strcat(option, " -expand");
	if (!OptIgnoreChar) {
	sprintf(buf, " -i %c", OptIgnoreChar);
	strcat(option, buf);
	}

	/* ���ꂩ�瓮�� */
	sprintf(buf, "RUN%s\n", option);
	if (send_string(buf) != 200) {
	fprintf(stderr, ";; argument error OK? [%s]\n", option);
	close(fd);
	exit(1);
	}

	/* LOOP */
	strnum = 0;
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
	if (strncmp(buf, "EOS", 3) == 0) {
		if (strnum != 0) {
		fwrite(buf, sizeof(char), strlen(buf), fo);
		fflush(fo);
		strnum = 0;
		while (fgets(buf, sizeof(buf), fi) != NULL) {
			fwrite(buf, sizeof(char), strlen(buf), stdout);
			fflush(stdout);
			if (strncmp(buf, "EOS", 3) == 0)  break;
		}
		}
	} else {
		fwrite(buf, sizeof(char), strlen(buf), fo);
		fflush(fo);
		strnum++;
	}
	}

	/* �I������ */
	fprintf(fo,"\n%c\nQUIT\n", EOf);
	fclose(fo);
	fclose(fi);
	close(fd);
	exit(0);
}
#endif

#ifndef JUMAN_KNP
/*==================================================================*/
		   int main(int argc, char **argv)
/*==================================================================*/
{
	option_proc(argc, argv);

	Infp  = stdin;
	Outfp = stdout;

	/* ���[�h�ɂ���ď����𕪊� */
	if (OptMode == STAND_ALONE_MODE) {
	init_all();
	knp_main();
	close_all();
	}
//#ifndef _WIN32
#if 0
	else if (OptMode == SERVER_MODE) {
	init_all();
	server_mode();
	close_all();
	}
	else if (OptMode == CLIENT_MODE) {
	client_mode();
	}
#endif

	exit(0);
}
#endif

/*====================================================================
							   END
====================================================================*/
