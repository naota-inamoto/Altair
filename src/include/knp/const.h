/*====================================================================

				  CONSTANTS

											   S.Kurohashi 91. 6.25
											   S.Kurohashi 93. 5.31

	$Id: const.h,v 1.118.2.2 2005/09/06 06:42:37 kawahara Exp $
====================================================================*/

#include "dbm.h"

/*====================================================================
				MACRO
====================================================================*/

#define str_eq(c1, c2) ( ! strcmp(c1, c2) )
#define L_Jiritu_M(ptr)   (ptr->jiritu_ptr + ptr->jiritu_num - 1)

/*====================================================================
				LENGTH
====================================================================*/
#define MRPH_MAX	200
#define BNST_MAX	64
#define BNST_LENGTH_MAX 256
#define TAG_MAX 	200
#define PARA_MAX	32
#define PARA_PART_MAX	32
#define WORD_LEN_MAX	128
#define SENTENCE_MAX	512
#define PRINT_WIDTH 100
#define PARENT_MAX	20
#define BROTHER_MAX 20
#define TEIDAI_TYPES	5
#define HOMO_MAX	30
#define HOMO_MRPH_MAX	10

#define BGH_CODE_SIZE	11
#define SM_CODE_SIZE	12
#define SCASE_CODE_SIZE 11

#define HomoRule_MAX	128
#define BonusRule_MAX	16
#define KoouRule_MAX	124
#define DpndRule_MAX	124
#define DpndRule_G_MAX	16
#define ContRule_MAX	256
#define DicForRule_MAX	1024
#define NERule_MAX	512
#define CNRule_MAX	512
#define EtcRule_MAX 1024
#define GeneralRule_MAX 1024

#define IsMrphRule	1
#define IsBnstRule	2
#define IsMrph2Rule 3

#ifndef SMALL
#define ALL_CASE_FRAME_MAX	1024
#else
#define ALL_CASE_FRAME_MAX	0
#endif
#define CF_ELEMENT_MAX		20
#define PP_ELEMENT_MAX		10
#define SM_ELEMENT_MAX		256
#define EX_ELEMENT_MAX		256
#define MAX_MATCH_MAX		10

#define CMM_MAX 	5				/* �œK�i�t���[���� */
#define CPM_MAX 	64				/* �����q�ꐔ */
#define TM_MAX		5				/* �œK�ˑ��\���� */

#ifndef IMI_MAX
	#define IMI_MAX 129 /* defined in "juman.h" */	
#endif

#define DATA_LEN	5120
#define SMALL_DATA_LEN	128
#define SMALL_DATA_LEN2 256
#define ALLOCATION_STEP 1024
#define DEFAULT_PARSETIMEOUT	180

#define TBLSIZE 1024
#define NSEED	32	/* �����\�̎�ށB2 ��㰏�łȂ���΂Ȃ�Ȃ��B */
#define NSIZE	256

/*====================================================================
				DEFINE
====================================================================*/
#define OPT_CASE	1
#define OPT_CASE2	6
#define OPT_DPND	2
#define OPT_BNST	3
#define OPT_AssignF 4
#define OPT_ELLIPSIS	1
#define OPT_DEMO	2
#define OPT_REL_NOUN	4
#define OPT_RAW 	0
#define OPT_PARSED	1
#define OPT_INPUT_BNST	2
#define OPT_TREE	1
#define OPT_TREEF	2
#define OPT_SEXP	3
#define OPT_TAB 	4
#define OPT_NOTAG	5
#define OPT_PA		6
#define OPT_NOTAGTREE	7
#define OPT_NORMAL	1
#define OPT_DETAIL	2
#define OPT_DEBUG	3
#define OPT_ENTITY	4
#define OPT_SVM 	2
#define OPT_DT		3
#define OPT_SERV_FORE	1

#define OPT_CASE_ASSIGN_GA_SUBJ 2
#define OPT_CASE_NO 4
#define OPT_CASE_USE_EX_ALL 8

#define OPT_DISC_OR_CF	1
#define OPT_DISC_BEST	2
#define OPT_DISC_CLASS_ONLY 4
#define OPT_DISC_FLAT	8
#define OPT_DISC_TWIN_CAND	16
#define OPT_DISC_RANKING	48
#define OPT_DISC_NO_WO_TO	64

#define IS_BNST_DATA	1
#define IS_TAG_DATA 2

#define PP_NUMBER	44
#define LOC_NUMBER	21
#define UTYPE_NUMBER	12

typedef enum {VERBOSE0, VERBOSE1, VERBOSE2, 
		  VERBOSE3, VERBOSE4, VERBOSE5} VerboseType;

#define PARA_KEY_O			0
#define PARA_KEY_N			1	/* �̌��̕��� */
#define PARA_KEY_P			2	/* �p���̕��� */
#define PARA_KEY_A			4	/* �̌����p����������Ȃ����� */
#define PARA_KEY_I			3	/* GAP�̂������ �H�H */

#define PRINT_PARA	0
#define PRINT_DPND	1
#define PRINT_MASK	2
#define PRINT_QUOTE 3
#define PRINT_RSTR	4
#define PRINT_RSTD	5
#define PRINT_RSTQ	6

#define SEMANTIC_MARKER 1
#define EXAMPLE 	2

#define VOICE_SHIEKI	1
#define VOICE_UKEMI 	2
#define VOICE_SHIEKI_UKEMI	4
#define VOICE_MORAU 	8
#define VOICE_HOSHII	16
#define VOICE_UNKNOWN	32

#define FRAME_ACTIVE		1
#define FRAME_PASSIVE_I 	2
#define FRAME_PASSIVE_1 	3
#define FRAME_PASSIVE_2 	4
#define FRAME_CAUSATIVE_WO_NI	5
#define FRAME_CAUSATIVE_WO	6
#define FRAME_CAUSATIVE_NI	7
#define FRAME_CAUSATIVE_PASSIVE 8

#define FRAME_POSSIBLE		9
#define FRAME_POLITE		10
#define FRAME_SPONTANE		11

#define CF_CAUSATIVE_WO 	1
#define CF_CAUSATIVE_NI 	2
#define CF_PASSIVE_1		4
#define CF_PASSIVE_2		8
#define CF_PASSIVE_I		16
#define CF_POSSIBLE 	32
#define CF_POLITE		64
#define CF_SPONTANE 	128

#define UNASSIGNED	-1
#define NIL_ASSIGNED	-2

#define END_M		-10

#define CONTINUE	-1
#define GUARD		'\n'

#define TYPE_KATAKANA	1
#define TYPE_HIRAGANA	2
#define TYPE_KANJI	3
#define TYPE_SUUJI	4
#define TYPE_EIGO	5
#define TYPE_KIGOU	6

#define SM_NO_EXPAND_NE 1
#define SM_EXPAND_NE	2
#define SM_CHECK_FULL	3
#define SM_EXPAND_NE_DATA	4

#define RLOOP_MRM	0
#define RLOOP_RMM	1

#define RLOOP_BREAK_NONE	0
#define RLOOP_BREAK_NORMAL	1
#define RLOOP_BREAK_JUMP	2

#define LtoR		1
#define RtoL		-1

#define CF_DECIDE_THRESHOLD 7

/*====================================================================
				  ?
====================================================================*/

#define PARA_NIL	0
#define PARA_NORMAL 	1	/* <P> */
#define PARA_INCOMP 	2	/* <I> */

#define REL_NOT 	0 /* �d�Ȃ�Ȃ� */
#define REL_BIT 	1 /* �����d�Ȃ� */
#define REL_PRE 	2 /* �O�ŏd�Ȃ� */
#define REL_POS 	3 /* ��ŏd�Ȃ� */
#define REL_PAR 	4 /* �d��	*/
#define REL_REV 	5 /* �O���̏C�� */
#define REL_IN1 	6 /* �܂܂��O */
#define REL_IN2 	7 /* �܂܂��� */
#define REL_BAD 	8 /* ���	*/

/*====================================================================
			   Client/Server  ���샂�[�h
====================================================================*/

#define STAND_ALONE_MODE 0
#define SERVER_MODE 	 1
#define CLIENT_MODE 	 2

#define DEFAULT_PORT	 31000
#define EOf 0x0b

#define KNP_SERVER_USER "nobody"
#define KNP_PIDFILE 	"/var/run/knp.pid"

/*====================================================================
				   FEATURE
====================================================================*/

#define RF_MAX	16

/* FEATURE�\���� */
typedef struct _FEATURE *FEATUREptr;
typedef struct _FEATURE {
	char	*cp;
	FEATUREptr	next;
} FEATURE;

/* FEATURE�p�^�[�� */
typedef struct {
	FEATURE 	*fp[RF_MAX];
} FEATURE_PATTERN;

/*====================================================================
				 �^�����K�\��
====================================================================*/

#define NOT_FLG '^'
#define MAT_FLG '\0'
#define AST_FLG '*'
#define QST_FLG '?'
#define NOT_STR "^"
#define AST_STR "*"
#define QST_STR "?"
#define FW_MATCHING 0
#define BW_MATCHING 1
#define ALL_MATCHING 0
#define PART_MATCHING 1
#define SHORT_MATCHING 0
#define LONG_MATCHING 1

#define RM_HINSHI_MAX 64
#define RM_BUNRUI_MAX 64
#define RM_KATA_MAX 64
#define RM_KEI_MAX	64
#define RM_GOI_MAX	64

/* �`�ԑf�p�^�[�� */
typedef struct {
	char type_flag; /* '?' or '^' or NULL */
	char ast_flag;	/* '*' or NULL */
	char Hinshi_not;
	int Hinshi[RM_HINSHI_MAX];
	char Bunrui_not;
	int Bunrui[RM_BUNRUI_MAX];
	char Kata_not;
	int Katuyou_Kata[RM_KATA_MAX];
	char Kei_not;
	char *Katuyou_Kei[RM_KEI_MAX];
	char Goi_not;
	char *Goi[RM_GOI_MAX];
	FEATURE_PATTERN f_pattern;
} REGEXPMRPH;

/* �`�ԑf��p�^�[�� */
typedef struct {
	REGEXPMRPH	*mrph;
	char	mrphsize;
} REGEXPMRPHS;

/* ���߃p�^�[�� */
typedef struct {
	char	type_flag;	/* '?' or '^' or NULL */
	char	ast_flag;	/* '*' or NULL */
	REGEXPMRPHS *mrphs;
	FEATURE_PATTERN f_pattern;
} REGEXPBNST;

/* ���ߗ�p�^�[�� */
typedef struct {
	REGEXPBNST	*bnst;
	char	bnstsize;
} REGEXPBNSTS;

/*====================================================================
				 �K��
====================================================================*/

#define LOOP_BREAK	0
#define LOOP_ALL	1

/* ���`�ً`��K�� */
typedef struct {
	REGEXPMRPHS *pre_pattern;
	REGEXPMRPHS *pattern;
	FEATURE *f;
} HomoRule;

/* �`�ԑf��K�� */
typedef struct {
	REGEXPMRPHS *pre_pattern;
	REGEXPMRPHS *self_pattern;
	REGEXPMRPHS *post_pattern;
	FEATURE *f;
} MrphRule;

/* ���ߗ�K�� */
typedef struct {
	REGEXPBNSTS *pre_pattern;
	REGEXPBNSTS *self_pattern;
	REGEXPBNSTS *post_pattern;
	FEATURE *f;
} BnstRule;

/* �W��󂯋K�� */
typedef struct {
	FEATURE_PATTERN dependant;
	FEATURE_PATTERN governor[DpndRule_G_MAX];
	char		dpnd_type[DpndRule_G_MAX];
	FEATURE_PATTERN barrier;
	int 		preference;
	int 		decide; /* ��ӂɌ��肷�邩�ǂ��� */
} DpndRule;

/* �{�[�i�X�K�� */
typedef struct {
	REGEXPMRPHS *pattern;
	int 	type;		/* ����̃^�C�v */
} BonusRule;

/* �ĉ��K�� */
typedef struct {
	REGEXPMRPHS 	*start_pattern;
	REGEXPMRPHS 	*end_pattern;
	REGEXPMRPHS 	*uke_pattern;
} KoouRule;

#define QUOTE_MAX 40

typedef struct {
	int in_num[QUOTE_MAX];
	int out_num[QUOTE_MAX];
} QUOTE_DATA;

typedef struct {
	char *key;
	FEATUREptr f;
} DicForRule;

/* �`�ԑf��K��, ���ߗ�K���̏W�܂���������߂̍\���� */
typedef struct {
	void	*RuleArray;
	int 	CurRuleSize;
	int 	type;
	int 	mode;
	int 	breakmode;
	int 	direction;
} GeneralRuleType;

typedef struct _RuleVector {
	char	*file;
	int 	type;
	int 	mode;
	int 	breakmode;
	int 	direction;
} RuleVector;

#define RuleIncrementStep 10

/* �ǂݍ��ݕ��@ */
#define MorphRuleType 1
#define BnstRuleType 2
#define HomoRuleType 3
#define DpndRuleType 4
#define KoouRuleType 5
#define NeMorphRuleType 6
#define NePhrasePreRuleType 7
#define NePhraseRuleType 8
#define NePhraseAuxRuleType 9
#define ContextRuleType 10
#define TagRuleType 11

/* �����̍ő吔 */
#define DICT_MAX	17

/* �����̒�` */
#define BGH_DB		1
#define SM_DB		2
#define SM2CODE_DB	3
#define SMP2SMG_DB	4
#define SCASE_DB	5
#define CF_INDEX_DB 6
#define CF_DATA 	7
#define PROPER_DB	8
#define PROPERC_DB	9
#define PROPERCASE_DB	10
#define CODE2SM_DB	12
#define EVENT_DB	13
#define CF_NOUN_INDEX_DB	14
#define CF_NOUN_DATA		15
#define CF_SIM_DB	16

/* �V�\�[���X�̍ő吔 */
#define THESAURUS_MAX	3


/*====================================================================
				  ��{�f�[�^
====================================================================*/

/* �`�ԑf�f�[�^ */
typedef struct {
	char	Goi[WORD_LEN_MAX+1];	/* ���^ */
	char	Yomi[WORD_LEN_MAX+1];
	char	Goi2[WORD_LEN_MAX+1];
	int 	Hinshi;
	int 	Bunrui;
	int 	Katuyou_Kata;
	int 	Katuyou_Kei;
	char	Imi[IMI_MAX];
	FEATUREptr	f;
	char	*SM;				/* �ǉ� */
} MRPH_DATA;

typedef struct cf_def *CF_ptr;
typedef struct cpm_def *CPM_ptr;
/* ���߃f�[�^ */
typedef struct tnode_b *Treeptr_B;
typedef struct tnode_b {
	int 	type;
	/* �ԍ� */
	int 	num;
	/* �`�ԑf�f�[�^ */
	int 	mrph_num;
	MRPH_DATA	*mrph_ptr, *head_ptr;
	/* �Ӗ���� */
	char	BGH_code[EX_ELEMENT_MAX*BGH_CODE_SIZE+1];
	int 	BGH_num;
	char	SM_code[SM_ELEMENT_MAX*SM_CODE_SIZE+1];
	int 		SM_num;
	/* �i��̓f�[�^ */
	int 	voice;
	int 	cf_num; 	/* ���̗p���ɑ΂���i�t���[���̐� */
	CF_ptr	cf_ptr; 	/* �i�t���[���Ǘ��z��(Case_frame_array)
				   �ł̂��̗p���̊i�t���[���̈ʒu */
	CPM_ptr 	cpm_ptr;	/* �i��͂̌��ʂ̕ێ� */
	/* feature */
	FEATUREptr	f;
	/* �؍\���|�C���^ */
	Treeptr_B	parent;
	Treeptr_B	child[PARA_PART_MAX];
	struct tnode_b *pred_b_ptr;
	/* tree�\���p */
	int 	length;
	int 	space;
	/* �W��󂯏�� (�������m���R�s�[) */
	int 	dpnd_head;	/* �W���̕��ߔԍ� */
	char	dpnd_type;	/* �W��̃^�C�v : D, P, I, A */
	int 	dpnd_dflt;	/* default�̌W��敶�ߔԍ� */
	/* �\�w�i�f�[�^ */
	char	SCASE_code[SCASE_CODE_SIZE];	/* �\�w�i */
	/* ����\�� */
	int 	para_num;	/* �Ή��������\���f�[�^�ԍ� */
	char	para_key_type;	/* ��|�q|�H feature����R�s�[ */
	char	para_top_p; /* TRUE -> PARA */
	char	para_type;	/* 0, 1:<P>, 2:<I> */
					/* ����2��PARA�m�[�h�𓱓����邽�߂̂���
				   dpnd_type�ȂǂƂ͔����ɈقȂ� */
	char	to_para_p;	/* �R�s�[ */
	int 	sp_level;	/* ����\���ɑ΂���o���A */

	char	Jiritu_Go[BNST_LENGTH_MAX];
	DpndRule	*dpnd_rule;

	struct tnode_t *tag_ptr;
	int 	tag_num;
} BNST_DATA;

/* ����\���f�[�^ */
typedef struct node_para_manager *Para_M_ptr;
typedef struct tnode_p *Treeptr_P;
typedef struct tnode_p {
	char	para_char;
	int 	type;
	int 	max_num;
	int 		key_pos, iend_pos, jend_pos, max_path[BNST_MAX];
	FEATURE_PATTERN f_pattern;	/* �������߂̏��� */
	float	max_score;	/* �ގ����̍ő�l */
	float	pure_score; /* �����\���̃{�[�i�X���������l,������̊ */
	char		status;
	Para_M_ptr	manager_ptr;
} PARA_DATA;

typedef struct node_para_manager {
	int 	para_num;
	int 	para_data_num[PARA_PART_MAX];
	int 	part_num;
	int 	start[PARA_PART_MAX];
	int 	end[PARA_PART_MAX];
	Para_M_ptr	parent;
	Para_M_ptr	child[PARA_PART_MAX];
	int 	child_num;
	BNST_DATA	*bnst_ptr;
	char	status;
} PARA_MANAGER;

typedef struct _check {
	int num;
	int def;
	int pos[BNST_MAX];
} CHECK_DATA;

/* �����̊e���߂̌W���Ȃǂ̋L�^ */
typedef struct {
	int 	head[BNST_MAX]; /* �W��� */
	char	type[BNST_MAX]; /* �W��^�C�v */
	int 	dflt[BNST_MAX]; /* �W��̋��� */
	int 	mask[BNST_MAX]; /* ��������� */
	int 	pos;		/* ���݂̏����ʒu */
	CHECK_DATA	check[BNST_MAX];
	FEATURE *f[BNST_MAX];	/* feature */
} DPND;

typedef struct thesaurus {
	char	*path;
	char	*name;
	int 	*format;
	int 	code_size;
	int 	exist;
	DBM_FILE	db;
} THESAURUS_FILE;

/*====================================================================
				�i���
====================================================================*/

typedef struct tnode_t {
	int 	type;
	/* �ԍ� */
	int 	num;
	/* �`�ԑf�f�[�^ */
	int 	mrph_num;
	MRPH_DATA	*mrph_ptr, *head_ptr;
	/* �Ӗ���� */
	char	BGH_code[EX_ELEMENT_MAX*BGH_CODE_SIZE+1];
	int 	BGH_num;
	char	SM_code[SM_ELEMENT_MAX*SM_CODE_SIZE+1];
	int 		SM_num;
	/* �i��̓f�[�^ */
	int 	voice;
	int 	cf_num;
	CF_ptr	cf_ptr;
	CPM_ptr 	cpm_ptr;
	/* feature */
	FEATUREptr	f;
	/* �؍\���|�C���^ */
	struct tnode_t	*parent;
	struct tnode_t	*child[PARA_PART_MAX];
	struct tnode_t	*pred_b_ptr;
	/* tree�\���p */
	int 	length;
	int 	space;
	/* �W��󂯏�� */
	int 	dpnd_head;
	char	dpnd_type;
	int 	dpnd_dflt;	/* ����Ȃ�? */
	/* �\�w�i�f�[�^ */
	char	SCASE_code[SCASE_CODE_SIZE];
	/* ����\�� */
	int 	para_num;
	char	para_key_type;
	char	para_top_p;
	char	para_type;
	char	to_para_p;
	/* ���߂Ƃ̊֌W */
	int 	bnum;	/* ���ߋ�؂�ƈ�v����Ƃ��̔ԍ� */
	int 	inum;
	BNST_DATA	*b_ptr; /* �������镶�� */
	/* �`�ԑf�f�[�^ */
	int 	settou_num, jiritu_num, fuzoku_num;
	MRPH_DATA	*settou_ptr, *jiritu_ptr, *fuzoku_ptr;
	int 	e_cf_num;
} TAG_DATA;

#define CASE_MAX_NUM	20
#define CASE_TYPE_NUM	50

#define USE_NONE -1
#define USE_BGH 1
#define USE_NTT 2
#define STOREtoCF	4
#define USE_BGH_WITH_STORE	5
#define USE_NTT_WITH_STORE	6
#define USE_SUFFIX_SM	8
#define USE_PREFIX_SM	16

#define CF_PRED 1
#define CF_NOUN 2

#define CF_NORMAL	0
#define CF_SUM		1	/* OR �̊i�t���[�� */
#define CF_GA_SEMI_SUBJECT	2
#define CF_CHANGE	4

#define CF_UNDECIDED	0
#define CF_CAND_DECIDED 1
#define CF_DECIDED	2

#define MATCH_SUBJECT	-1
#define MATCH_NONE	-2

typedef struct {
	char *kaku_keishiki;	/* �i�`�� */
	char *meishiku; 	/* ������ */
	char *imisosei; 	/* �Ӗ��f�� */
} CF_CASE_SLOT;

typedef struct {
	char *yomi;
	char *hyoki;
	char *feature;
	char pred_type[3];
	int voice;
	int etcflag;
	int casenum;
	CF_CASE_SLOT cs[CASE_MAX_NUM];
	int samecase[CF_ELEMENT_MAX][2];
	unsigned char *DATA;
} CF_FRAME;

/* �i�t���[���\����
	�� ���͕��ɑ΂��č����
	�� �܂��C�i�t���[�������̊e�G���g���ɂ������
		(�u�`���v�Ȃǂ̏ꍇ�͎�g,���h�Ȃǂɂ��ꂼ����)
 */
typedef struct cf_def {
	int 	type;
	int 		type_flag;								/* ���莌�ł��邩�ǂ��� */
	int 	element_num;				/* �i�v�f�� */
	int 	oblig[CF_ELEMENT_MAX];			/* �K�{�i���ǂ��� */
	int 	adjacent[CF_ELEMENT_MAX];		/* ���O�i���ǂ��� */
	int 	pp[CF_ELEMENT_MAX][PP_ELEMENT_MAX]; 	/* �i���� */
	int 	sp[CF_ELEMENT_MAX]; 		/* �\�w�i (���͑�) */
	char	*pp_str[CF_ELEMENT_MAX];
	char	*sm[CF_ELEMENT_MAX];			/* �Ӗ��}�[�J */
	char	*sm_delete[CF_ELEMENT_MAX]; 	/* �g�p�֎~�Ӗ��}�[�J */
	int 	sm_delete_size[CF_ELEMENT_MAX];
	int 	sm_delete_num[CF_ELEMENT_MAX];
	char	*sm_specify[CF_ELEMENT_MAX];		/* �����Ӗ��}�[�J */
	int 	sm_specify_size[CF_ELEMENT_MAX];
	int 	sm_specify_num[CF_ELEMENT_MAX];
	char	*ex[CF_ELEMENT_MAX];			/* �p�� */
	char	**ex_list[CF_ELEMENT_MAX];
	int 	*ex_freq[CF_ELEMENT_MAX];
	int 	ex_size[CF_ELEMENT_MAX];
	int 	ex_num[CF_ELEMENT_MAX];
	char	*semantics[CF_ELEMENT_MAX];
	int 	voice;					/* ���H�C�X */
	int 	cf_address; 			/* �i�t���[���̃A�h���X */
	int 	cf_size;				/* �i�t���[���̃T�C�Y */
	char	cf_id[SMALL_DATA_LEN];			/* �i�t���[����ID */
	char	pred_type[3];				/* �p���^�C�v (��, �`, ��) */
	char	*entry; 				/* �p���̕\�L */
	char	imi[SMALL_DATA_LEN];
	int 	etcflag;				/* �i�t���[���� OR ���ǂ��� */
	char	*feature;
	int 	weight[CF_ELEMENT_MAX];
	int 	samecase[CF_ELEMENT_MAX][2];
	TAG_DATA	*pred_b_ptr;
	float	cf_similarity;
} CASE_FRAME;

/* �����̊i�v�f�Ɗi�t���[���̃X���b�g�Ƃ̑Ή��t���L�^ */
typedef struct {
	int 	flag[CF_ELEMENT_MAX];
	int 	score[CF_ELEMENT_MAX];
	int 	pos[CF_ELEMENT_MAX];
} LIST;

/* ���Ɗi�t���[���̑Ή��t�����ʂ̋L�^ */
typedef struct {
	CASE_FRAME	*cf_ptr;			/* �i�t���[���ւ̃|�C���^ */
	float	score;				/* �X�R�A */
	int 	pure_score[MAX_MATCH_MAX];	/* ���K������O�̃X�R�A */
	float	sufficiency;			/* �i�t���[���̖��܂肮���� */
	int 	result_num; 		/* �L������Ή��֌W�� */
	LIST	result_lists_p[MAX_MATCH_MAX];	/* �X�R�A�ő�̑Ή��֌W
						   (���_�̏ꍇ�͕���) */
	LIST	result_lists_d[MAX_MATCH_MAX];

	struct cpm_def	*cpm;
} CF_MATCH_MGR;

/* ����(�p���ɑ΂��镡���̉\��)�i�t���[���̑Ή��t�����ʂ̋L�^ */
typedef struct cpm_def {
	CASE_FRAME	cf; 			/* ���͕��̊i�\�� */
	TAG_DATA	*pred_b_ptr;			/* ���͕��̗p������ */
	TAG_DATA	*elem_b_ptr[CF_ELEMENT_MAX];	/* ���͕��̊i�v�f���� */
	struct sentence *elem_s_ptr[CF_ELEMENT_MAX];	/* �ǂ̕��̗v�f�ł��邩 (�ȗ��p) */
	int 	elem_b_num[CF_ELEMENT_MAX]; /* ���͕��̊i�v�f����(�A�i�̌W����-1,���͎q�̏���) */
	int 	score;				/* �X�R�A�ő�l(=cmm[0].score) */
	int 	result_num; 		/* �L������i�t���[���� */
	int 	tie_num;
	CF_MATCH_MGR cmm[CMM_MAX];			/* �X�R�A�ő�̊i�t���[���Ƃ�
						   �Ή��t�����L�^
						   (���_�̏ꍇ�͕���) */
	int 	decided;
} CF_PRED_MGR;

/* �ꕶ�̉�͌��ʂ̑S�L�^ */
typedef struct {
	DPND	dpnd;		/* �ˑ��\�� */
	int 	pssb;		/* �ˑ��\���̉\���̉��Ԗڂ� */
	int 	dflt;		/* �H */
	int 	score;		/* �X�R�A */
	int 	pred_num;	/* �����̗p���� */
	CF_PRED_MGR cpm[CPM_MAX];	/* �����̊e�p���̊i��͌��� */
	int 	ID; 	/* DPND �� ID */
} TOTAL_MGR;

/*====================================================================
				   ��������
====================================================================*/

typedef struct sentence {
	int 		Sen_num;	/* ���ԍ� 1�` */
	int 		available;
	int 		Mrph_num;
	int 		Bnst_num;
	int 		New_Bnst_num;
	int 		Max_New_Bnst_num;
	int 		Tag_num;
	int 		New_Tag_num;
	int 		Para_M_num; /* ����Ǘ��}�l�[�W���� */
	int 		Para_num;	/* ����\���� */
	MRPH_DATA		*mrph_data;
	BNST_DATA		*bnst_data;
	TAG_DATA		*tag_data;
	PARA_DATA		*para_data;
	PARA_MANAGER	*para_manager;
	CF_PRED_MGR 	*cpm;
	CASE_FRAME		*cf;
	TOTAL_MGR		*Best_mgr;
	char		*KNPSID;
	char		*Comment;
} SENTENCE_DATA;

/* �����ƈӖ��f�̍\���� */
typedef struct sm_list {
	char	*key;		/* ���� */
	char	*sm;		/* �Ӗ��f */
	struct sm_list *next;
} SMLIST;

/*====================================================================
							   END
====================================================================*/
