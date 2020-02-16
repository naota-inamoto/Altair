/*====================================================================

			   設定ファイル関連

											   S.Kurohashi 1999.11.13

	$Id: configfile.c,v 1.37.2.3 2005/09/05 12:17:07 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

extern char Jumangram_Dirname[];
extern int LineNoForError, LineNo;
char *Knprule_Dirname = NULL;
char *Knpdict_Dirname = NULL;

RuleVector *RULE = NULL;
int CurrentRuleNum = 0;
int RuleNumMax = 0;

char *DICT[DICT_MAX];
int knp_dict_file_already_defined = 0;
THESAURUS_FILE THESAURUS[THESAURUS_MAX];

#ifdef	_WINDOWS
static char* DEF_JUMAN_GRAM_FILE = NULL;
static char* DEF_KNP_FILE = 	   NULL;
static char* DEF_KNP_DIR =		   NULL;
static char* DEF_KNP_DICT_DIR =    NULL;
static char* DEF_KNP_DICT_FILE =   NULL;
static char* DEF_THESAURUS =	   NULL;
static char* DEF_CASE_THESAURUS =  NULL;
static char* DEF_PARA_THESAURUS =  NULL;
static char DEF_JUMAN_GRAM_FILE_2[] = "JUMAN文法ディレクトリ";
static char DEF_KNP_FILE_2[] =		  "KNPルールファイル";
static char DEF_KNP_DIR_2[] =		  "KNPルールディレクトリ";
static char DEF_KNP_DICT_DIR_2[] =	  "KNP辞書ディレクトリ";
static char DEF_KNP_DICT_FILE_2[] =   "KNP辞書ファイル";
static char DEF_THESAURUS_2[] = 	  "KNPシソーラス";
static char DEF_CASE_THESAURUS_2[] =  "KNP格解析シソーラス";
static char DEF_PARA_THESAURUS_2[] =  "KNP並列解析シソーラス";
static char* simword = NULL;
static char* morph = NULL;
static char* tag_unit = NULL;
static char* clause = NULL;
static char* modif = NULL;
static char* koou_str = NULL;
static char* phrase_morph = NULL;
static char* phrase_PRE = NULL;
static char* phrase = NULL;
static char* phrase_AUX = NULL;
static char* context = NULL;
static char* rule_loop = NULL;
static char* direction = NULL;
static char* opposite = NULL;
static char* case_frame_idx_db = NULL;
static char* case_frame_data = NULL;
static char* case_frame_sim_db = NULL;
static char* noun_case_frame_idx_db = NULL;
static char* noun_case_frame_data = NULL;
static char* vocabulary_db = NULL;
static char* surface_case_db = NULL;
static char* ntt_word_db = NULL;
static char* ntt_meaning_db = NULL;
static char* ntt_table_db = NULL;
static char simword_2[] = "同形異義語";
static char morph_2[] = "形態素";
static char tag_unit_2[] = "タグ単位";
static char clause_2[] = "文節";
static char modif_2[] = "係り受け";
static char koou_str_2[] = "呼応";
static char phrase_morph_2[] = "固有表現形態素";
static char phrase_PRE_2[] = "固有表現句-PRE";
static char phrase_2[] = "固有表現句";
static char phrase_AUX_2[] = "固有表現句-AUX";
static char context_2[] = "文脈";
static char rule_loop_2[] = "ルールループ先行";
static char direction_2[] = "順方向";
static char opposite_2[] = "逆方向";
static char case_frame_idx_db_2[] = "格フレームINDEXDB";
static char case_frame_data_2[] = "格フレームDATA";
static char case_frame_sim_db_2[] = "格フレームSIMDB";
static char noun_case_frame_idx_db_2[] = "名詞格フレームINDEXDB";
static char noun_case_frame_data_2[] = "名詞格フレームDATA";
static char vocabulary_db_2[] = "分類語彙表DB";
static char surface_case_db_2[] = "表層格DB";
static char ntt_word_db_2[] = "NTT単語DB";
static char ntt_meaning_db_2[] = "NTT意味素DB";
static char ntt_table_db_2[] = "NTT固有名詞変換テーブルDB";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	DEF_JUMAN_GRAM_FILE = toStringEUC(DEF_JUMAN_GRAM_FILE_2);
	DEF_KNP_FILE =		  toStringEUC(DEF_KNP_FILE_2);
	DEF_KNP_DIR =		  toStringEUC(DEF_KNP_DIR_2);
	DEF_KNP_DICT_DIR =	  toStringEUC(DEF_KNP_DICT_DIR_2);
	DEF_KNP_DICT_FILE =   toStringEUC(DEF_KNP_DICT_FILE_2);
	DEF_THESAURUS = 	  toStringEUC(DEF_THESAURUS_2);
	DEF_CASE_THESAURUS =  toStringEUC(DEF_CASE_THESAURUS_2);
	DEF_PARA_THESAURUS =  toStringEUC(DEF_PARA_THESAURUS_2);
	simword = toStringEUC(simword_2);
	morph = toStringEUC(morph_2);
	tag_unit = toStringEUC(tag_unit_2);
	clause = toStringEUC(clause_2);
	modif = toStringEUC(modif_2);
	koou_str = toStringEUC(koou_str_2);
	phrase_morph = toStringEUC(phrase_morph_2);
	phrase_PRE = toStringEUC(phrase_PRE_2);
	phrase = toStringEUC(phrase_2);
	phrase_AUX = toStringEUC(phrase_AUX_2);
	context = toStringEUC(context_2);
	rule_loop = toStringEUC(rule_loop_2);
	direction = toStringEUC(direction_2);
	opposite = toStringEUC(opposite_2);
	case_frame_idx_db = toStringEUC(case_frame_idx_db_2);
	case_frame_data = toStringEUC(case_frame_data_2);
	case_frame_sim_db = toStringEUC(case_frame_sim_db_2);
	noun_case_frame_idx_db = toStringEUC(noun_case_frame_idx_db_2);
	noun_case_frame_data = toStringEUC(noun_case_frame_data_2);
	vocabulary_db = toStringEUC(vocabulary_db_2);
	surface_case_db = toStringEUC(surface_case_db_2);
	ntt_word_db = toStringEUC(ntt_word_db_2);
	ntt_meaning_db = toStringEUC(ntt_meaning_db_2);
	ntt_table_db = toStringEUC(ntt_table_db_2);
#endif
}
void final_str_configfile() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(DEF_JUMAN_GRAM_FILE); DEF_JUMAN_GRAM_FILE = NULL;
	free(DEF_KNP_FILE); DEF_KNP_FILE = NULL;
	free(DEF_KNP_DIR); DEF_KNP_DIR = NULL;
	free(DEF_KNP_DICT_DIR); DEF_KNP_DICT_DIR = NULL;
	free(DEF_KNP_DICT_FILE); DEF_KNP_DICT_FILE = NULL;
	free(DEF_THESAURUS); DEF_THESAURUS = NULL;
	free(DEF_CASE_THESAURUS); DEF_CASE_THESAURUS = NULL;
	free(DEF_PARA_THESAURUS); DEF_PARA_THESAURUS = NULL;
	free(simword); simword = NULL;
	free(morph); morph = NULL;
	free(tag_unit); tag_unit = NULL;
	free(clause); clause = NULL;
	free(modif); modif = NULL;
	free(koou_str); koou_str = NULL;
	free(phrase_morph); phrase_morph = NULL;
	free(phrase_PRE); phrase_PRE = NULL;
	free(phrase); phrase = NULL;
	free(phrase_AUX); phrase_AUX = NULL;
	free(context); context = NULL;
	free(rule_loop); rule_loop = NULL;
	free(direction); direction = NULL;
	free(opposite); opposite = NULL;
	free(case_frame_idx_db); case_frame_idx_db = NULL;
	free(case_frame_data); case_frame_data = NULL;
	free(case_frame_sim_db); case_frame_sim_db = NULL;
	free(noun_case_frame_idx_db); noun_case_frame_idx_db = NULL;
	free(noun_case_frame_data); noun_case_frame_data = NULL;
	free(vocabulary_db); vocabulary_db = NULL;
	free(surface_case_db); surface_case_db = NULL;
	free(ntt_word_db); ntt_word_db = NULL;
	free(ntt_meaning_db); ntt_meaning_db = NULL;
	free(ntt_table_db); ntt_table_db = NULL;
#endif
}
#ifdef	_LINUX
static char DEF_JUMAN_GRAM_FILE[] = "JUMAN文法ディレクトリ";
static char DEF_KNP_FILE[] =		"KNPルールファイル";
static char DEF_KNP_DIR[] = 		"KNPルールディレクトリ";
static char DEF_KNP_DICT_DIR[] =	"KNP辞書ディレクトリ";
static char DEF_KNP_DICT_FILE[] =	"KNP辞書ファイル";
static char DEF_THESAURUS[] =		"KNPシソ\201\133ラス"; /* ー */
static char DEF_CASE_THESAURUS[] =	"KNP格解析シソ\201\133ラス"; /* ー */
static char DEF_PARA_THESAURUS[] =	"KNP並列解析シソ\201\133ラス"; /* ー */
static char simword[] = "同形異義語";
static char morph[] = "形態素";
static char tag_unit[] = "タグ単位";
static char clause[] = "文節";
static char modif[] = "係り受け";
static char koou_str[] = "呼応";
static char phrase_morph[] = "固有\225\134現形態素"; /* 表 */
static char phrase_PRE[] = "固有\225\134現句-PRE"; /* 表 */
static char phrase[] = "固有\225\134現句"; /* 表 */
static char phrase_AUX[] = "固有\225\134現句-AUX"; /* 表 */
static char context[] = "文脈";
static char rule_loop[] = "ルールループ先行";
static char direction[] = "順方向";
static char opposite[] = "逆方向";
static char case_frame_idx_db[] = "格フレームINDEXDB";
static char case_frame_data[] = "格フレームDATA";
static char case_frame_sim_db[] = "格フレームSIMDB";
static char noun_case_frame_idx_db[] = "名詞格フレームINDEXDB";
static char noun_case_frame_data[] = "名詞格フレームDATA";
static char vocabulary_db[] = "分類語彙\225\134DB"; /* 表 */
static char surface_case_db[] = "\225\134層格DB"; /* 表 */
static char ntt_word_db[] = "NTT単語DB";
static char ntt_meaning_db[] = "NTT意味素DB";
static char ntt_table_db[] = "NTT固有名詞変換テーブルDB";
#endif

extern void set_cha_getc();
extern void unset_cha_getc();

/*==================================================================*/
		void check_duplicated(int value, const char *string)
/*==================================================================*/
{
	/* 値が 0 でないときはエラー */
	if (value) {
	fprintf(stderr, "%s is duplicately specified in .knprc\n", string);
	exit(0);	
	}
}

#if 0
/*==================================================================*/
		   void clear_rule_configuration()
/*==================================================================*/
{
	if (CurrentRuleNum) {
	free(RULE);
	RULE = NULL;
	CurrentRuleNum = 0;
	RuleNumMax = 0;
	}
	if (Knprule_Dirname) {
	free(Knprule_Dirname);
	Knprule_Dirname = NULL;
	}
	if (Knpdict_Dirname) {
	free(Knpdict_Dirname);
	Knpdict_Dirname = NULL;
	}
}
#endif

/*==================================================================*/
			char *check_tilde(char *file)
/*==================================================================*/
{
	char *home, *ret;
	int n;

	if (*file == '~' && (home = art_getenv("HOME")) != NULL) {
	ret = (char *)malloc_data(n = strlen(home)+strlen(file), "check_tilde");
	art_sprintf_2(ret, n, "%s%s", home, strchr(file, '/'));
	}
	else {
	ret = art_strdup(file);
	}
	return ret;
}

/*==================================================================*/
			   int *str2ints(char *str)
/*==================================================================*/
{
	char *cp, *start, *token;
	int *ret, ret_size = 1, count = 0;

	if (str[0] == '\"') {
	start = str + 1;
	if ((cp = strchr(start, '\"')) != NULL) {
		*cp = '\0';
	}
	}
	else {
	start = str;
	}

	ret = (int *)malloc_data(sizeof(int) * ret_size, "str2ints");

	token = art_strtok(start, ",");
	while (token) {
	if (count >= ret_size - 1) {
		ret = (int *)realloc_data(ret, sizeof(int) * (ret_size <<= 1), "str2ints");
	}
	*(ret + count++) = atoi(token);
	token = art_strtok(NULL, ",");
	}
	*(ret + count) = 0;

	return ret;
}

/*==================================================================*/
		   FILE *find_rc_file(const char *opfile)
/*==================================================================*/
{
	FILE *fp;
	int n;

	if (opfile) {
	if ((fp = art_fopen(opfile, "r")) == NULL) {
		fprintf(stderr, "not found rc file <%s>.\n", opfile);
		exit(1);
	}
	}
	else {
	char *user_home, *filename;

	if((user_home = art_getenv("HOME")) == NULL) {
		filename = NULL;
	}
	else {
		filename = (char *)malloc_data(n = strlen(user_home)+strlen("/.knprc")+1, "find_rc_file");
		art_sprintf_1(filename, n, "%s/.knprc" , user_home);
	}

	if (filename == NULL || (fp = art_fopen(filename, "r")) == NULL) {
#ifdef KNP_RC_DEFAULT
		if ((fp = fopen(KNP_RC_DEFAULT, "r")) == NULL) {
		fprintf(stderr, "not found <.knprc> and KNP_RC_DEFAULT(<%s>).\n", KNP_RC_DEFAULT);
		exit(1);
		}
#else
		fprintf(stderr, "not found <.knprc> in your home directory.\n");
		exit(1);
#endif
	}
	if (filename) {
		free(filename);
	}
	}
	return fp;
}

JUMAN_KNP_DATA char* al_juman_dic = NULL;
JUMAN_KNP_DATA char* al_knp_dic = NULL;
JUMAN_KNP_DATA char* al_knp_rule = NULL;
/*==================================================================*/
			void read_rc(FILE *
#ifdef	_LINUX
in
#endif
)
/*==================================================================*/
{
#ifdef	_WIN32
	char buf[FILENAME_MAX];
	/* MS Windows の場合は、juman.ini, knp.ini を見に行く
	   dicfile == gramfile */
#ifdef	JUMAN_KNP
	art_strcpy(Jumangram_Dirname, FILENAME_MAX, al_juman_dic);
#else
	GetPrivateProfileString("juman", "dicfile", "", Jumangram_Dirname, FILENAME_MAX, "juman.ini");
#endif
#ifdef	JUMAN_KNP
	art_strcpy(buf, FILENAME_MAX, al_knp_rule);
#else
	GetPrivateProfileString("knp", "ruledir", "", buf, FILENAME_MAX, "knp.ini");
#endif
	if (buf[0]) {
	Knprule_Dirname = art_strdup(buf);
	}
#ifdef	JUMAN_KNP
	art_strcpy(buf, FILENAME_MAX, al_knp_dic);
#else
	GetPrivateProfileString("knp", "dictdir", "", buf, FILENAME_MAX, "knp.ini");
#endif
	if (buf[0]) {
	Knpdict_Dirname = art_strdup(buf);
	}
#else
	CELL *cell1,*cell2;
	char *dicttype;

	init_str();

	LineNo = 0 ;
	Jumangram_Dirname[0] = '\0';

	while (!s_feof(in))  {
	LineNoForError = LineNo;
	cell1 = s_read(in);

	if (!strcmp(DEF_JUMAN_GRAM_FILE, (const char*)_Atom(car(cell1)))) {
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		fprintf(stderr, "error in .knprc\n");
		exit(0);
		} else	
		strcpy(Jumangram_Dirname, (const char*)_Atom(cell2));
	}
	/* KNP ルールディレクトリ */
	else if (!strcmp(DEF_KNP_DIR, (const char*)_Atom(car(cell1)))) {
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		fprintf(stderr, "error in .knprc\n");
		exit(0);
		}
		else
		Knprule_Dirname = check_tilde((char*)_Atom(cell2));
	}
	/* KNP ルールファイル */
	else if (!strcmp(DEF_KNP_FILE, (const char*)_Atom(car(cell1)))) {
		cell1 = cdr(cell1);

		while (!Null(car(cell1))) {
		if (CurrentRuleNum >= RuleNumMax) {
			RuleNumMax += RuleIncrementStep;
			RULE = (RuleVector *)realloc_data(RULE, sizeof(RuleVector)*RuleNumMax, "read_rc");
		}

		/* デフォルト値設定 */
		(RULE+CurrentRuleNum)->file = (char *)strdup((const char*)_Atom(car(car(cell1))));
		(RULE+CurrentRuleNum)->mode = RLOOP_MRM;
		(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NONE;
		(RULE+CurrentRuleNum)->type = 0;
		(RULE+CurrentRuleNum)->direction = 0;

		cell2 = cdr(car(cell1));

		while (!Null(car(cell2))) {
			if (!strcmp((const char*)_Atom(car(cell2)), simword)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = HomoRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), morph)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = MorphRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), tag_unit)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = TagRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), clause)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = BnstRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), modif)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = DpndRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), koou_str)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = KoouRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), phrase_morph)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = NeMorphRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), phrase_PRE)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = NePhrasePreRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), phrase)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = NePhraseRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), phrase_AUX)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = NePhraseAuxRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), context)) {
			check_duplicated((RULE+CurrentRuleNum)->type, "Rule type");
			(RULE+CurrentRuleNum)->type = ContextRuleType;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), rule_loop)) {
			(RULE+CurrentRuleNum)->mode = RLOOP_RMM;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), "BREAK")) {
			/* RLOOP_BREAK_NONE は 0 なのでひっかからない */
			check_duplicated((RULE+CurrentRuleNum)->breakmode, "Break mode");
			(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NORMAL;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), "BREAKJUMP")) {
			/* RLOOP_BREAK_NONE は 0 なのでひっかからない */
			check_duplicated((RULE+CurrentRuleNum)->breakmode, "Break mode");
			(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_JUMP;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), direction)) {
			check_duplicated((RULE+CurrentRuleNum)->direction, "Direction");
			(RULE+CurrentRuleNum)->direction = LtoR;
			}
			else if (!strcmp((const char*)_Atom(car(cell2)), opposite)) {
			check_duplicated((RULE+CurrentRuleNum)->direction, "Direction");
			(RULE+CurrentRuleNum)->direction = RtoL;
			}
			else {
			fprintf(stderr, "%s is invalid in .knprc\n", _Atom(car(cell2)));
			exit(0);
			}
			cell2 = cdr(cell2);
		}

		/* ルールのタイプが指定されていないとき */
		if (!(RULE+CurrentRuleNum)->type) {
			fprintf(stderr, "Rule type for \'%s\' is not specified in .knprc\n",	
				(RULE+CurrentRuleNum)->file);
			exit(0);
		}

		/* デフォルトの方向 */
		if (!(RULE+CurrentRuleNum)->direction)
			(RULE+CurrentRuleNum)->direction = LtoR;

#ifdef _LINUX_DEBUG
printf("knp: Rule Type = %d\n", (RULE+CurrentRuleNum)->type);
#endif

		CurrentRuleNum++;
		cell1 = cdr(cell1);
		}
	}
	/* KNP 辞書ディレクトリ */
	else if (!strcmp(DEF_KNP_DICT_DIR, (const char*)_Atom(car(cell1)))) {
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		fprintf(stderr, "error in .knprc\n");
		exit(0);
		}
		else
		Knpdict_Dirname = check_tilde((char*)_Atom(cell2));
	}
	/* KNP 辞書ファイル */
	else if (!strcmp(DEF_KNP_DICT_FILE, (const char*)_Atom(car(cell1))) &&	
		 !knp_dict_file_already_defined) {
		cell1 = cdr(cell1);
		knp_dict_file_already_defined = 1;

		while (!Null(car(cell1))) {
		dicttype = (char*)_Atom(car(cdr(car(cell1))));
		if (!strcmp(dicttype, case_frame_idx_db)) {
			DICT[CF_INDEX_DB] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, case_frame_data)) {
			DICT[CF_DATA] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, case_frame_sim_db)) {
			DICT[CF_SIM_DB] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, noun_case_frame_idx_db)) {
			DICT[CF_NOUN_INDEX_DB] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, noun_case_frame_data)) {
			DICT[CF_NOUN_DATA] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, vocabulary_db)) {
			DICT[BGH_DB] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, surface_case_db)) {
			DICT[SCASE_DB] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, ntt_word_db)) {
			DICT[SM_DB] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, ntt_meaning_db)) {
			DICT[SM2CODE_DB] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else if (!strcmp(dicttype, ntt_table_db)) {
			DICT[SMP2SMG_DB] = strdup((const char*)_Atom(car(car(cell1))));
		}
		else {
			fprintf(stderr, "%s is invalid in .knprc\n", _Atom(car(cdr(car(cell1)))));
			exit(0);
		}
		cell1 = cdr(cell1);
		}
	}
	/* 新たなシソーラス */
	else if (!strcmp(DEF_THESAURUS, (const char*)_Atom(car(cell1)))) {
		int i;
		cell1 = cdr(cell1);
		while (!Null(car(cell1))) {
		THESAURUS[0].path = strdup((const char*)_Atom(car(car(cell1))));
		THESAURUS[0].name = strdup((const char*)_Atom(car(cdr(car(cell1)))));
		THESAURUS[0].format = str2ints((char*)_Atom(car(cdr(cdr(car(cell1))))));
		for (i = 0; THESAURUS[0].format[i]; i++) {
			THESAURUS[0].code_size += THESAURUS[0].format[i];
		}
		cell1 = cdr(cell1);
		}
	}
	/* 格解析用シソーラス */
	else if (!strcmp(DEF_CASE_THESAURUS, (const char*)_Atom(car(cell1)))) {
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		fprintf(stderr, "error in .knprc\n");
		exit(0);
		}
		else {
		int i;

		Thesaurus = USE_NONE;
		if (strcasecmp((const char*)_Atom(cell2), "NONE")) { /* NONEではないとき */
			for (i = 0; THESAURUS[i].name && i < THESAURUS_MAX; i++) {
			if (!strcasecmp((const char*)_Atom(cell2), THESAURUS[i].name)) {
				Thesaurus = i;
				if (OptDisplay == OPT_DEBUG) {
				fprintf(Outfp, "Thesaurus for case analysis ... %s\n", THESAURUS[i].name);
				}
				break;
			}
			}
			if (Thesaurus == USE_NONE) {
			fprintf(stderr, "%s is invalid in .knprc\n", _Atom(cell2));
			exit(0);
			}
		}
		}
	}
	/* 並列解析用シソーラス */
	else if (!strcmp(DEF_PARA_THESAURUS, (const char*)_Atom(car(cell1)))) {
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		fprintf(stderr, "error in .knprc\n");
		exit(0);
		}
		else {
		int i;

		ParaThesaurus = USE_NONE;
		if (strcasecmp((const char*)_Atom(cell2), "NONE")) { /* NONEではないとき */
			for (i = 0; THESAURUS[i].name && i < THESAURUS_MAX; i++) {
			if (!strcasecmp((const char*)_Atom(cell2), THESAURUS[i].name)) {
				ParaThesaurus = i;
				if (OptDisplay == OPT_DEBUG) {
				fprintf(Outfp, "Thesaurus for para analysis ... %s\n", THESAURUS[i].name);
				}
				break;
			}
			}
			if (ParaThesaurus == USE_NONE) {
			fprintf(stderr, "%s is invalid in .knprc\n", _Atom(cell2));
			exit(0);
			}
		}
		}
	}
	}
#endif

	/* knprc にルールが指定されていない場合のデフォルトルール */
	if (CurrentRuleNum == 0) {
	if (OptDisplay == OPT_DEBUG) {
		fprintf(Outfp, "Setting default rules ... ");
	}

	RuleNumMax = 8;
	RULE = (RuleVector *)realloc_data(RULE, sizeof(RuleVector)*RuleNumMax, "read_rc");

	/* mrph_homo 同形異義語 */
	(RULE+CurrentRuleNum)->file = art_strdup("mrph_homo");
	(RULE+CurrentRuleNum)->mode = RLOOP_MRM;
	(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NONE;
	(RULE+CurrentRuleNum)->type = HomoRuleType;
	(RULE+CurrentRuleNum)->direction = LtoR;
	CurrentRuleNum++;

	/* mrph_basic 形態素 ルールループ先行 */
	(RULE+CurrentRuleNum)->file = art_strdup("mrph_basic");
	(RULE+CurrentRuleNum)->mode = RLOOP_RMM;
	(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NONE;
	(RULE+CurrentRuleNum)->type = MorphRuleType;
	(RULE+CurrentRuleNum)->direction = LtoR;
	CurrentRuleNum++;

	/* bnst_basic 文節 逆方向 ルールループ先行 */
	(RULE+CurrentRuleNum)->file = art_strdup("bnst_basic");
	(RULE+CurrentRuleNum)->mode = RLOOP_RMM;
	(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NONE;
	(RULE+CurrentRuleNum)->type = BnstRuleType;
	(RULE+CurrentRuleNum)->direction = RtoL;
	CurrentRuleNum++;

	/* bnst_type 文節 逆方向 BREAK */
	(RULE+CurrentRuleNum)->file = art_strdup("bnst_type");
	(RULE+CurrentRuleNum)->mode = RLOOP_MRM;
	(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NORMAL;
	(RULE+CurrentRuleNum)->type = BnstRuleType;
	(RULE+CurrentRuleNum)->direction = RtoL;
	CurrentRuleNum++;

	/* bnst_etc 文節 逆方向 ルールループ先行 */
	(RULE+CurrentRuleNum)->file = art_strdup("bnst_etc");
	(RULE+CurrentRuleNum)->mode = RLOOP_RMM;
	(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NONE;
	(RULE+CurrentRuleNum)->type = BnstRuleType;
	(RULE+CurrentRuleNum)->direction = RtoL;
	CurrentRuleNum++;

	/* kakari_uke 係り受け */
	(RULE+CurrentRuleNum)->file = art_strdup("kakari_uke");
	(RULE+CurrentRuleNum)->mode = RLOOP_MRM;
	(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NONE;
	(RULE+CurrentRuleNum)->type = DpndRuleType;
	(RULE+CurrentRuleNum)->direction = LtoR;
	CurrentRuleNum++;

	/* koou 呼応 */
	(RULE+CurrentRuleNum)->file = art_strdup("koou");
	(RULE+CurrentRuleNum)->mode = RLOOP_MRM;
	(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NONE;
	(RULE+CurrentRuleNum)->type = KoouRuleType;
	(RULE+CurrentRuleNum)->direction = LtoR;
	CurrentRuleNum++;

	/* bnst_basic タグ単位 逆方向 ルールループ先行 */
	(RULE+CurrentRuleNum)->file = art_strdup("bnst_basic");
	(RULE+CurrentRuleNum)->mode = RLOOP_RMM;
	(RULE+CurrentRuleNum)->breakmode = RLOOP_BREAK_NONE;
	(RULE+CurrentRuleNum)->type = TagRuleType;
	(RULE+CurrentRuleNum)->direction = RtoL;
	CurrentRuleNum++;

	if (OptDisplay == OPT_DEBUG) {
		fprintf(Outfp, "done.\n");
	}
	}
}

#if 0
/*==================================================================*/
			void server_read_rc(FILE *fp)
/*==================================================================*/
{
	clear_rule_configuration();
	set_cha_getc();
	read_rc(fp);
	unset_cha_getc();
}
#endif

/*==================================================================*/
	 void check_data_newer_than_rule(time_t data, char *datapath)
/*==================================================================*/
{
	/* ルールファイルとの時間チェック */

	char *rulename;
	int status;
	struct stat sb;
	int n;

	rulename = art_strdup(datapath);
	//*(rulename+strlen(rulename)-5) = '\0';
	n = strlen(rulename);
	*(rulename+n-5) = '\0';
	art_strcat(rulename, n+1, ".rule");
	status = stat(rulename, &sb);
	if (!status) {
	if (data < sb.st_mtime) {
		fprintf(stderr, ";; %s: older than rule file!\n", datapath);
	}
	}
	free(rulename);
}

/*==================================================================*/
		char *check_rule_filename(const char *file)
/*==================================================================*/
{
	/* ルールファイル (*.data) の fullpath を返す関数 */

	char *fullname, *home;
	int status;
	struct stat sb;
	int n;

	if (!Knprule_Dirname) {
#ifdef KNP_RULE
	Knprule_Dirname = strdup(KNP_RULE);
#else
	fprintf(stderr, ";; Please specify rule directory in .knprc\n");
	exit(0);
#endif
	}

	fullname = (char *)malloc_data(n = strlen(Knprule_Dirname)+strlen(file)+7, "check_rule_filename");
	art_sprintf_2(fullname, n, "%s/%s.data", Knprule_Dirname, file);
	/* dir + filename + ".data" */
	status = stat(fullname, &sb);

	if (status < 0) {
	*(fullname+strlen(fullname)-5) = '\0';
	/* dir + filename */
	status = stat(fullname, &sb);
	if (status < 0) {
		/* filename + ".data" */
		if (*file == '~' && (home = art_getenv("HOME")) != NULL) {
		free(fullname);
		fullname = (char *)malloc_data(n = strlen(home)+strlen(file)+6, "check_dict_filename");
		art_sprintf_2(fullname, n, "%s%s.data", home, strchr(file, '/'));
		}
		else {
		art_sprintf_1(fullname, n, "%s.data", file);
		}
		status = stat(fullname, &sb);
		if (status < 0) {
		*(fullname+strlen(fullname)-5) = '\0';
		/* filename */
		status = stat(fullname, &sb);
		if (status < 0) {
			fprintf(stderr, "%s: No such file.\n", fullname);
			exit(1);
		}
		}
	}
	}

	/* ルールファイルとの時間チェック */
	check_data_newer_than_rule(sb.st_mtime, fullname);

	return fullname;
}

/*==================================================================*/
	   char *check_dict_filename(const char *file, int flag)
/*==================================================================*/
{
	char *fullname, *home;
	int status;
	struct stat sb;
	int n;

	if (!Knpdict_Dirname) {
#ifdef KNP_DICT
	Knpdict_Dirname = strdup(KNP_DICT);
#else
	fprintf(stderr, "Please specify dict directory in .knprc\n");
	exit(0);
#endif
	}

	fullname = (char *)malloc_data(n = strlen(Knpdict_Dirname)+strlen(file)+2, "check_dict_filename");
	art_sprintf_2(fullname, n, "%s/%s", Knpdict_Dirname, file);

	/* dir + filename */
	status = stat(fullname, &sb);

	if (status < 0) {
	free(fullname);
	if (*file == '~' && (home = art_getenv("HOME")) != NULL) {
		fullname = (char *)malloc_data(n = strlen(home)+strlen(file), "check_dict_filename");
		art_sprintf_2(fullname, n, "%s%s", home, strchr(file, '/'));
	}
	else {
		fullname = art_strdup(file);
	}
	status = stat(fullname, &sb);
	if (status < 0) {
		/* flag が FALSE のときはファイルが存在するかどうかチェックしない */
		if (flag == FALSE) {
		return fullname;
		}
		fprintf(stderr, "%s: No such file.\n", fullname);
		exit(1);
	}
	}
	return fullname;
}

/*==================================================================*/
		  void init_configfile(const char *opfile)
/*==================================================================*/
{
	int i;

	for (i = 0; i < DICT_MAX; i++) {
	DICT[i] = NULL;
	}

	THESAURUS[0].path = NULL;
	THESAURUS[0].name = NULL;
	THESAURUS[0].format = NULL;
	THESAURUS[0].exist = 0;
	THESAURUS[1].path = NULL;
	THESAURUS[1].name = art_strdup("BGH");
	THESAURUS[1].format = NULL;
	THESAURUS[1].code_size = BGH_CODE_SIZE;
	THESAURUS[2].path = NULL;
	THESAURUS[2].name = art_strdup("NTT");
	THESAURUS[2].format = NULL;
	THESAURUS[2].code_size = SM_CODE_SIZE;

#ifdef _WIN32
	if (!opfile) {
	read_rc(NULL);
	}
	else
#endif
	read_rc(find_rc_file(opfile));
}

/*====================================================================
				 END
====================================================================*/
