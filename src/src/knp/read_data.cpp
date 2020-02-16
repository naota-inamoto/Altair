/*====================================================================

		�`�ԑf��͗�̓ǂݍ��݁C���߂ւ̂܂Ƃ�

											   S.Kurohashi 91. 6.25
											   S.Kurohashi 93. 5.31

	$Id: read_data.c,v 1.65.2.4 2005/09/06 06:42:37 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

int Bnst_start[MRPH_MAX];
int Tag_start[MRPH_MAX];
int Tag_dpnd[TAG_MAX];
int Tag_type[TAG_MAX];
FEATURE *Input_bnst_feature[BNST_MAX];
FEATURE *Input_tag_feature[TAG_MAX];

int ArticleID = 0;
int preArticleID = 0;

extern char CorpusComment[BNST_MAX][DATA_LEN];

#ifdef	_WINDOWS
static char* BASIC_FORM = NULL;
static char* hin = NULL;
static char* aimai = NULL;
static char* hin_s = NULL;
static char* hin_other = NULL;
static char* daihyou = NULL;
static char* daihyou_s = NULL;
static char* hinnshi_henkou = NULL;
static char* buntou = NULL;
static char* bunsetsu_begin = NULL;
static char* inside_bunsetsu = NULL;
static char* jiritsu = NULL;
static char* fuzoku = NULL;
static char* settou = NULL;
static char* settou_hidokuritsu = NULL;
static char* dokuritsu_settou = NULL;
static char* dokuritsu_setsubi = NULL;
static char* dokuritsu_muimi = NULL;
static char* hidokuritsu_setsubi = NULL;
static char* ana_done = NULL;
static char* tag_unit = NULL;
static char* tag_unit_s = NULL;
static char BASIC_FORM_2[] = "��{�`";
static char hin_2[] = "�i�B";
static char aimai_2[] = "���`�B��";
static char hin_s_2[] = "�i�B-%s";
static char hin_other_2[] = "�i�B-���̑�";
static char daihyou_2[] = "��\�\�L:";
static char daihyou_s_2[] = "��\�\�L:%*s";
static char hinnshi_henkou_2[] = "�i���ύX:%s-%s-%s-%d-%d-%d-%d";
static char buntou_2[] = "����";
static char bunsetsu_begin_2[] = "���ߎn";
static char inside_bunsetsu_2[] = "���ߓ�";
static char jiritsu_2[] = "����";
static char fuzoku_2[] = "�t��";
static char settou_2[] = "�ړ�";
static char settou_hidokuritsu_2[] = "�ړ���Ɨ���";
static char dokuritsu_settou_2[] = "�Ɨ��ړ���";
static char dokuritsu_setsubi_2[] = "�Ɨ��ڔ���";
static char dokuritsu_muimi_2[] = "�Ɨ����Ӗ���";
static char hidokuritsu_setsubi_2[] = "��Ɨ��L�Ӗ��ڔ���";
static char ana_done_2[] = "��͍�";
static char tag_unit_2[] = "�^�O�P�ʎn";
static char tag_unit_s_2[] = ";; morpheme %d must be <�^�O�P�ʎn>! (%s)\n";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	BASIC_FORM = toStringEUC(BASIC_FORM_2);
	hin = toStringEUC(hin_2);
	aimai = toStringEUC(aimai_2);
	hin_s = toStringEUC(hin_s_2);
	hin_other = toStringEUC(hin_other_2);
	daihyou = toStringEUC(daihyou_2);
	daihyou_s = toStringEUC(daihyou_s_2);
	hinnshi_henkou = toStringEUC(hinnshi_henkou_2);
	buntou = toStringEUC(buntou_2);
	bunsetsu_begin = toStringEUC(bunsetsu_begin_2);
	inside_bunsetsu = toStringEUC(inside_bunsetsu_2);
	jiritsu = toStringEUC(jiritsu_2);
	fuzoku = toStringEUC(fuzoku_2);
	settou = toStringEUC(settou_2);
	settou_hidokuritsu = toStringEUC(settou_hidokuritsu_2);
	dokuritsu_settou = toStringEUC(dokuritsu_settou_2);
	dokuritsu_setsubi = toStringEUC(dokuritsu_setsubi_2);
	dokuritsu_muimi = toStringEUC(dokuritsu_muimi_2);
	hidokuritsu_setsubi = toStringEUC(hidokuritsu_setsubi_2);
	ana_done = toStringEUC(ana_done_2);
	tag_unit = toStringEUC(tag_unit_2);
	tag_unit_s = toStringEUC(tag_unit_s_2);
#endif
}
void final_str_read_data() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(BASIC_FORM); BASIC_FORM = NULL;
	free(hin); hin = NULL;
	free(aimai); aimai = NULL;
	free(hin_s); hin_s = NULL;
	free(hin_other); hin_other = NULL;
	free(daihyou); daihyou = NULL;
	free(daihyou_s); daihyou_s = NULL;
	free(hinnshi_henkou); hinnshi_henkou = NULL;
	free(buntou); buntou = NULL;
	free(bunsetsu_begin); bunsetsu_begin = NULL;
	free(inside_bunsetsu); inside_bunsetsu = NULL;
	free(jiritsu); jiritsu = NULL;
	free(fuzoku); fuzoku = NULL;
	free(settou); settou = NULL;
	free(settou_hidokuritsu); settou_hidokuritsu = NULL;
	free(dokuritsu_settou); dokuritsu_settou = NULL;
	free(dokuritsu_setsubi); dokuritsu_setsubi = NULL;
	free(dokuritsu_muimi); dokuritsu_muimi = NULL;
	free(hidokuritsu_setsubi); hidokuritsu_setsubi = NULL;
	free(ana_done); ana_done = NULL;
	free(tag_unit); tag_unit = NULL;
	free(tag_unit_s); tag_unit_s = NULL;
#endif
}
#ifdef	_LINUX
static char BASIC_FORM[] = "��{�`";
static char hin[] = "�i�B";
static char aimai[] = "���`�B��";
static char hin_s[] = "�i�B-%s";
static char hin_other[] = "�i�B-���̑�";
static char daihyou[] = "��\255\134\255\134�L:"; /* �\ */
static char daihyou_s[] = "��\255\134\255\134�L:%*s"; /* �\ */
static char hinnshi_henkou[] = "�i���ύX:%s-%s-%s-%d-%d-%d-%d";
static char buntou[] = "����";
static char bunsetsu_begin[] = "���ߎn";
static char inside_bunsetsu[] = "���ߓ�";
static char jiritsu[] = "����";
static char fuzoku[] = "�t��";
static char settou[] = "�ړ�";
static char settou_hidokuritsu[] = "�ړ���Ɨ���";
static char dokuritsu_settou[] = "�Ɨ��ړ���";
static char dokuritsu_setsubi[] = "�Ɨ��ڔ���";
static char dokuritsu_muimi[] = "�Ɨ����Ӗ���";
static char hidokuritsu_setsubi[] = "��Ɨ��L�Ӗ��ڔ���";
static char ana_done[] = "��͍�";
static char tag_unit[] = "�^�O�P�ʎn";
static char tag_unit_s[] = ";; morpheme %d must be <�^�O�P�ʎn>! (%s)\n";
#endif

extern int regexpmrphs_match(REGEXPMRPH *r_ptr,int r_num,
			MRPH_DATA *d_ptr, int d_num,	
			int fw_or_bw, int all_or_part, int short_or_long);
extern int get_form_id(U_CHAR *x, int type);
extern int get_hinsi_id(U_CHAR *x);
extern int get_bunrui_id(U_CHAR *x, int hinsi);
extern int get_type_id(U_CHAR *x);

/*==================================================================*/
void lexical_disambiguation(SENTENCE_DATA *sp, MRPH_DATA *m_ptr, int homo_num)
/*==================================================================*/
{
	int i, j, k, flag, amb_flag, pref_mrph, pref_rule;
	int bw_length;
	int real_homo_num;
	int uniq_flag[HOMO_MAX];		/* ���ƕi�����قȂ�`�ԑf�Ȃ� 1 */
	int matched_flag[HOMO_MRPH_MAX];	/* �����ꂩ�̌`�ԑf�ƃ}�b�`����
					   ���[�����`�ԑf�p�^�[���� 1 */
	HomoRule	*r_ptr;
	MRPH_DATA	*loop_ptr, *loop_ptr2;
	char fname[SMALL_DATA_LEN2], *buf;
	int n;

	init_str();

	/* ��������ő吔���z���Ă���΁A�ő吔�����`�F�b�N���� */
	if (homo_num > HOMO_MAX) {
	homo_num = HOMO_MAX;
	}

	/* �i��(�ו���)���قȂ�`�ԑf�������c���Cuniq_flag��1�ɂ��� */

	uniq_flag[0] = 1;
	real_homo_num = 1;
	for (i = 1; i < homo_num; i++) {
	loop_ptr = m_ptr + i;
	uniq_flag[i] = 1;
	for (j = 0; j < i; j++) {
		loop_ptr2 = m_ptr + j;

		/* �ǂ݈ȊO���ׂē��� --> ���� --> mrph_homo���g�����đΉ� */
		if (0 &&
		loop_ptr2->Hinshi == loop_ptr->Hinshi &&
		loop_ptr2->Bunrui == loop_ptr->Bunrui &&
		str_eq(loop_ptr2->Goi, loop_ptr->Goi) &&
		loop_ptr2->Katuyou_Kata == loop_ptr->Katuyou_Kata &&
		loop_ptr2->Katuyou_Kei == loop_ptr->Katuyou_Kei) {
		uniq_flag[i] = 0;
		break;			
		}
		/* ���p�^,���p�`�̂����ꂩ���Ⴄ (����, ����, �䂭, ...) --> ���� */
		else if (loop_ptr2->Hinshi == loop_ptr->Hinshi &&
			 loop_ptr2->Bunrui == loop_ptr->Bunrui &&
			 str_eq(loop_ptr2->Goi, loop_ptr->Goi) &&
			  (loop_ptr2->Katuyou_Kata != loop_ptr->Katuyou_Kata ||
			  loop_ptr2->Katuyou_Kei != loop_ptr->Katuyou_Kei)) {
		uniq_flag[i] = 0;
		break;
		}
	}
	if (uniq_flag[i]) real_homo_num++;
	}

	/* �����I���`�ً`�ꂪ�Ȃ���Ή��������͂��Ȃ� */

	if (real_homo_num == 1) return;

	/* ���[�� (mrph_homo.rule)�ɏ]���ėD�悷��`�ԑf��I��
	   �� ���`�ً`�ꐔ�ƃ��[�����̌`�ԑf�����������Ƃ�����
		  �e���`�ً`�ꂪ���[�����̌`�ԑf�̂����ꂩ�Ƀ}�b�`����΂悢
	  ���[���̍ŏ��̌`�ԑf�Ƀ}�b�`�������̂�D��(pref_mrph ���L��)
	*/

	flag = FALSE;
	pref_mrph = 0;
	pref_rule = 0;
	for (i = 0, r_ptr = HomoRuleArray; i < CurHomoRuleSize; i++, r_ptr++) {
	if (r_ptr->pattern->mrphsize > HOMO_MRPH_MAX) {
		fprintf(stderr, ";; The number of Rule morphs is too large in HomoRule.\n");
		exit(1);
	}
	
	/* �����܂ł̌`�ԑf����`�F�b�N */
	bw_length = (int)(m_ptr - sp->mrph_data);
	if ((r_ptr->pre_pattern == NULL &&	/* �Ⴂ */
		 bw_length != 0) ||
		(r_ptr->pre_pattern != NULL &&
		 regexpmrphs_match(r_ptr->pre_pattern->mrph +	
				   r_ptr->pre_pattern->mrphsize - 1,
				   r_ptr->pre_pattern->mrphsize,
				   m_ptr - 1,	
				   bw_length,	/* �Ⴂ */
				   BW_MATCHING, 
				   ALL_MATCHING,/* �Ⴂ */
				   SHORT_MATCHING) == -1)) {
		continue;
	}
	
	pref_mrph = 0;
	for (k = 0; k < r_ptr->pattern->mrphsize; k++) matched_flag[k] = FALSE;
	for (j = 0, loop_ptr = m_ptr; j < homo_num; j++, loop_ptr++) {
		if (uniq_flag[j] == 0) continue;
		flag = FALSE;
		for (k = 0; k < r_ptr->pattern->mrphsize; k++) {
		if (matched_flag[k] && (r_ptr->pattern->mrph + k)->ast_flag != AST_FLG)
			continue;
		if (regexpmrph_match(r_ptr->pattern->mrph + k, loop_ptr)	
			== 1 /*TRUE*/) {
			flag = TRUE;
			if (k == 0) pref_mrph = j;
			matched_flag[k] = TRUE;
			break;
		}
		}
		if (flag == FALSE) break;
	}
	if (flag == 1 /*TRUE*/) {
		for (k = 0; k < r_ptr->pattern->mrphsize; k++) {
		if (matched_flag[k] == FALSE) {
			flag = FALSE;
			break;
		}
		}
		if (flag == 1 /*TRUE*/) {
		pref_rule = i;
		break;
		}
	}
	}

	if (flag == 1 /*TRUE*/) {

	if (0 && OptDisplay == OPT_DEBUG) {
		fprintf(Outfp, "Lexical Disambiguation "
			"(%ldth mrph -> %dth homo by %dth rule : %s :",	
			m_ptr - sp->mrph_data, pref_mrph, pref_rule,	
			(m_ptr+pref_mrph)->Goi2);
		for (i = 0, loop_ptr = m_ptr; i < homo_num; i++, loop_ptr++)
		if (uniq_flag[i])	
			fprintf(Outfp, " %s",	
				Class[loop_ptr->Hinshi][loop_ptr->Bunrui].id);
		fprintf(Outfp, ")\n");
	}

	/* ���`�����}�[�N����feature��^���� */
	assign_cfeature(&((m_ptr+pref_mrph)->f), hin);

	/* ���[���ɋL�q����Ă���feature��^���� (�u�i�B�v���폜���郋�[��������) */
	assign_feature(&((m_ptr+pref_mrph)->f), &((HomoRuleArray + pref_rule)->f), m_ptr);

	/* ���`���B���ȂƂ��͌`�ԑf����ۑ����� */
	amb_flag = 0;
	if (check_feature((m_ptr+pref_mrph)->f, hin)) {
		for (i = 0; i < homo_num; i++) {
		if (i != pref_mrph) {
			if (strcmp((m_ptr+i)->Goi, (m_ptr+pref_mrph)->Goi)) { /* ���`��pref_mrph�ƈقȂ�ꍇ */
			amb_flag = 1;
			}
			buf = (char*)malloc_data(n = strlen((m_ptr+i)->Goi2)+
					  strlen((m_ptr+i)->Yomi)+
					  strlen((m_ptr+i)->Goi)+
					  strlen((m_ptr+i)->Imi)+20, "lexical_disambiguation");
			/* ���Ƃ̌`�ԑf����feature�Ƃ��ĕۑ� */
			art_sprintf_8(buf, n, "ALT-%s-%s-%s-%d-%d-%d-%d-%s",	
				(m_ptr+i)->Goi2, (m_ptr+i)->Yomi, (m_ptr+i)->Goi,	
				(m_ptr+i)->Hinshi, (m_ptr+i)->Bunrui,	
				(m_ptr+i)->Katuyou_Kata, (m_ptr+i)->Katuyou_Kei,	
				(m_ptr+i)->Imi);
			assign_cfeature(&((m_ptr+pref_mrph)->f), buf);
			free(buf);
		}
		}
	}

	if (amb_flag) {
		assign_cfeature(&((m_ptr+pref_mrph)->f), aimai);
	}

	/* pref_mrph�Ԗڂ̃f�[�^���R�s�[ */
	if (pref_mrph != 0) {
		art_strcpy(m_ptr->Goi2, WORD_LEN_MAX+1, (m_ptr+pref_mrph)->Goi2);
		art_strcpy(m_ptr->Yomi, WORD_LEN_MAX+1, (m_ptr+pref_mrph)->Yomi);
		art_strcpy(m_ptr->Goi, WORD_LEN_MAX+1, (m_ptr+pref_mrph)->Goi);
		m_ptr->Hinshi = (m_ptr+pref_mrph)->Hinshi;
		m_ptr->Bunrui = (m_ptr+pref_mrph)->Bunrui;
		m_ptr->Katuyou_Kata = (m_ptr+pref_mrph)->Katuyou_Kata;
		m_ptr->Katuyou_Kei = (m_ptr+pref_mrph)->Katuyou_Kei;
		art_strcpy(m_ptr->Imi, IMI_MAX, (m_ptr+pref_mrph)->Imi);
		clear_feature(&(m_ptr->f));
		m_ptr->f = (m_ptr+pref_mrph)->f;
		(m_ptr+pref_mrph)->f = NULL;
	}

	for (i = 0; i < homo_num; i++) {
		if (uniq_flag[i] == 0) continue;
		art_sprintf_1(fname, SMALL_DATA_LEN2, hin_s,	
			Class[(m_ptr+i)->Hinshi][(m_ptr+i)->Bunrui].id);
		assign_cfeature(&(m_ptr->f), fname);
		
		/* �ŗL�����ȊO�ɂ�"���̑�"���ӂ��Ă��� */
		if (m_ptr->Bunrui != 3 && /* �ŗL���� */
		m_ptr->Bunrui != 4 && /* �n�� */
		m_ptr->Bunrui != 5 && /* �l�� */
		m_ptr->Bunrui != 6) /* �g�D�� */
		assign_cfeature(&(m_ptr->f), hin_other);
	}
	}	
	else {
	if (1 || OptDisplay == OPT_DEBUG) {
		fprintf(Outfp, ";; Cannot disambiguate lexical ambiguities"
			" (%ldth mrph : %s ?", m_ptr - sp->mrph_data,
			(m_ptr+pref_mrph)->Goi2);
		for (i = 0, loop_ptr = m_ptr; i < homo_num; i++, loop_ptr++)
		if (uniq_flag[i])	
			fprintf(Outfp, " %s",	
				Class[loop_ptr->Hinshi][loop_ptr->Bunrui].id);
		fprintf(Outfp, ")\n");
	}
	}
}

/*==================================================================*/
			   int readtoeos(FILE *fp)
/*==================================================================*/
{
	U_CHAR input_buffer[DATA_LEN];

	while (1) {
	if (fgets((char*)input_buffer, DATA_LEN, fp) == NULL) return EOF;
	if (str_eq((const char*)input_buffer, "EOS\n")) return FALSE;
	}
}

/*==================================================================*/
			int readtonl(FILE *fp)
/*==================================================================*/
{
	int input_buffer;

	while (1) {
	if ((input_buffer = fgetc(fp)) == EOF) return EOF;
	if (input_buffer == '\n') {
		return FALSE;
	}
	}
}

/*==================================================================*/
		 int read_mrph_file(FILE *fp, U_CHAR *buffer)
/*==================================================================*/
{
	int len;
#ifdef _WIN32
	char *EUCbuffer;
#endif

	if (fgets((char*)buffer, DATA_LEN, fp) == NULL) return EOF;

#ifdef _WIN32
	EUCbuffer = toStringEUC((char*)buffer);
	char c = buffer[DATA_LEN-1];
	art_strcpy((char*)buffer, DATA_LEN, EUCbuffer);
	buffer[DATA_LEN-1] = c;
	free(EUCbuffer);
#endif

	/* Server ���[�h�̏ꍇ�� ���� \r\n �ɂȂ�*/
	if (OptMode == SERVER_MODE) {
	len = (int)strlen((const char*)buffer);
	if (len > 2 && buffer[len-1] == '\n' && buffer[len-2] == '\r') {
		buffer[len-2] = '\n';
		buffer[len-1] = '\0';
	}

	if (buffer[0] == EOf)	
		return EOF;
	}

	return TRUE;
}

/*==================================================================*/
		 int imi2feature(char *str, MRPH_DATA *m_ptr)
/*==================================================================*/
{
	char *token;

	token = art_strtok(str, " ");
	while (token) {
	assign_cfeature(&(m_ptr->f), token);
	token = art_strtok(NULL, " ");
	}
	return 0;
}

/*==================================================================*/
		 int feature_string2f(char *str, FEATURE **f)
/*==================================================================*/
{
	char *token;

	token = art_strtok(str, "><");
	while (token) {
	assign_cfeature(f, token);
	token = art_strtok(NULL, "><");
	}
	return 0;
}

/*==================================================================*/
		  int read_mrph(SENTENCE_DATA *sp, FILE *fp)
/*==================================================================*/
{
	U_CHAR input_buffer[DATA_LEN], rest_buffer[DATA_LEN];
	MRPH_DATA  *m_ptr = sp->mrph_data;
	int homo_num, offset, mrph_item, bnst_item, tag_item, i, homo_flag;

	sp->Mrph_num = 0;
	homo_num = 0;
	ErrorComment = NULL;
	PM_Memo[0] = '\0';
	input_buffer[DATA_LEN-1] = '\n';

	while (1) {
	if (read_mrph_file(fp, input_buffer) == EOF) return EOF;

	if (input_buffer[DATA_LEN-1] != '\n') {
		input_buffer[DATA_LEN-1] = '\0';
		fprintf(stderr, ";; Too long mrph <%s> !\n", input_buffer);
		return readtonl(fp);
	}
	else if (input_buffer[strlen((const char*)input_buffer)-1] != '\n') {
		fprintf(stderr, ";; Too long mrph <%s> !\n", input_buffer);
		return FALSE;
	}

	/* -i �ɂ��R�����g�s */
	if (OptIgnoreChar && *input_buffer == OptIgnoreChar) {
		fprintf(Outfp, "%s", input_buffer);
		fflush(Outfp);
		continue;
	}

	/* # �ɂ�鐳�K�̃R�����g�s */

	if (input_buffer[0] == '#') {
		int n;
		input_buffer[strlen((const char*)input_buffer)-1] = '\0';
		sp->Comment = art_strdup((const char*)input_buffer);
		sp->KNPSID = (char *)malloc_data(n = strlen((const char*)input_buffer), "read_mrph");
		art_sscanf_2((const char*)input_buffer, "# %s", sp->KNPSID, n);
	}

	/* ��͍ς݂̏ꍇ */
	/* ���ߍs */
	else if (input_buffer[0] == '*') {
		if (sp->Mrph_num == 0) {
		OptInput |= OPT_PARSED;
		sp->Bnst_num = 0;
		sp->Tag_num = 0;
		memset(Bnst_start, 0, sizeof(int)*MRPH_MAX);
		memset(Tag_start, 0, sizeof(int)*MRPH_MAX);
		if (OptReadFeature) {
			memset(Input_bnst_feature, 0, sizeof(FEATURE *) *BNST_MAX);
			memset(Input_tag_feature, 0, sizeof(FEATURE *) *TAG_MAX);
		}
		}

		if (OptInput == OPT_RAW) {
		fprintf(stderr, ";; Invalid input <%s> !\n", input_buffer);
		return readtoeos(fp);
		}

#if _WINDOWS && _MFC_VER >= 0x0800
		bnst_item = art_sscanf_5((const char*)input_buffer, "* %d%c %[^\n]",	
				   &(sp->Best_mgr->dpnd.head[sp->Bnst_num]),
				   &(sp->Best_mgr->dpnd.type[sp->Bnst_num]), 1,
				   rest_buffer, DATA_LEN);
#else
		bnst_item = art_sscanf_3((const char*)input_buffer, "* %d%c %[^\n]",	
				   &(sp->Best_mgr->dpnd.head[sp->Bnst_num]),
				   &(sp->Best_mgr->dpnd.type[sp->Bnst_num]),
				   rest_buffer);
#endif

		/* ���߂̓��͂��ꂽfeature���g�� */
		if (bnst_item == 3) {
		if (OptReadFeature) {	
			/* feature��<>��split����f�ɕϊ� */
			feature_string2f((char*)rest_buffer, &Input_bnst_feature[sp->Bnst_num]);
		}
		}
		else if (bnst_item != 2) {
		fprintf(stderr, ";; Invalid input <%s> !\n", input_buffer);
		OptInput = OPT_RAW;
		return readtoeos(fp);
		}

		Bnst_start[sp->Mrph_num] = 1;
		sp->Bnst_num++;
	}
	/* �^�O�P�ʍs */
	else if (input_buffer[0] == '+') {
		if (OptInput == OPT_RAW) {
		fprintf(stderr, ";; Invalid input <%s> !\n", input_buffer);
		return readtoeos(fp);
		}

#if _WINDOWS && _MFC_VER >= 0x0800
		tag_item = art_sscanf_5((const char*)input_buffer, "+ %d%c %[^\n]", 
				  &Tag_dpnd[sp->Tag_num],
				  &Tag_type[sp->Tag_num], 1,
				  rest_buffer, DATA_LEN);
#else
		tag_item = art_sscanf_3((const char*)input_buffer, "+ %d%c %[^\n]", 
				  &Tag_dpnd[sp->Tag_num],
				  &Tag_type[sp->Tag_num],
				  rest_buffer);
#endif

		/* �^�O�P�ʂ̓��͂��ꂽfeature���g�� */
		if (tag_item == 3) {
		if (OptReadFeature) {	
			/* feature��<>��split����f�ɕϊ� */
			feature_string2f((char*)rest_buffer, &Input_tag_feature[sp->Tag_num]);
		}
		}
		else if (tag_item != 2) {
		fprintf(stderr, ";; Invalid input <%s> !\n", input_buffer);
		OptInput = OPT_RAW;
		return readtoeos(fp);
		}

		Tag_start[sp->Mrph_num] = 1;
		sp->Tag_num++;
	}

	/* ���� */

	else if (str_eq((const char*)input_buffer, "EOS\n")) {
		/* �`�ԑf������Ȃ��Ƃ� */
		if (sp->Mrph_num == 0) {
		return FALSE;
		}

		/* �^�O�P�ʂ̂Ȃ���͍ς̏ꍇ */
		if ((OptInput & OPT_PARSED) && sp->Tag_num == 0) {
		OptInput |= OPT_INPUT_BNST;
		}

		if (homo_num) { /* �O�ɓ��`�ً`��Z�b�g������Ώ������� */
		lexical_disambiguation(sp, m_ptr - homo_num - 1, homo_num + 1);
		sp->Mrph_num -= homo_num;
		m_ptr -= homo_num;
		for (i = 0; i < homo_num; i++) {
			clear_feature(&((m_ptr+i)->f));
		}
		homo_num = 0;
		}
		return TRUE;
	}

	/* �ʏ�̌`�ԑf */

	else {

		/* ���`�ً`�ꂩ�ǂ��� */
		if (input_buffer[0] == '@' && input_buffer[1] == ' ' && input_buffer[2] != '@') {
		homo_flag = 1;
		}
		else {
		homo_flag = 0;
		}
		
		if (homo_flag == 0 && homo_num) {

		/* ���`�ً`��}�[�N���Ȃ��C�O�ɓ��`�ً`��Z�b�g�������
			   lexical_disambiguation���Ă�ŏ��� */			

		lexical_disambiguation(sp, m_ptr - homo_num - 1, homo_num + 1);
		sp->Mrph_num -= homo_num;
		m_ptr -= homo_num;
		for (i = 0; i < homo_num; i++) {
			clear_feature(&((m_ptr+i)->f));
		}
		homo_num = 0;
		}

		/* �ő吔���z���Ȃ��悤�Ƀ`�F�b�N */
		if (sp->Mrph_num >= MRPH_MAX) {
		fprintf(stderr, ";; Too many mrph (%s %s%s...)!\n", 
			sp->Comment ? sp->Comment : "", sp->mrph_data, sp->mrph_data+1);
		sp->Mrph_num = 0;
		return readtoeos(fp);
		}

		/* �`�ԑf��� :
		   ��b(���p�`) �ǂ� ��b(���^) 
		   �i��(+�ԍ�) �ו���(+�ԍ�) ���p�^(+�ԍ�) ���p�`(+�ԍ�)	
		   �Ӗ����
		*/

		offset = homo_flag ? 2 : 0;
#if _WINDOWS && _MFC_VER >= 0x0800
		mrph_item = art_sscanf_12((const char*)input_buffer + offset,
				   "%s %s %s %*s %d %*s %d %*s %d %*s %d %[^\n]",	
				   m_ptr->Goi2, WORD_LEN_MAX+1, m_ptr->Yomi, WORD_LEN_MAX+1, m_ptr->Goi, WORD_LEN_MAX+1,
				   &(m_ptr->Hinshi), &(m_ptr->Bunrui),
				   &(m_ptr->Katuyou_Kata), &(m_ptr->Katuyou_Kei),
				   rest_buffer, DATA_LEN);
#else
		mrph_item = art_sscanf_8((const char*)input_buffer + offset,
				   "%s %s %s %*s %d %*s %d %*s %d %*s %d %[^\n]",	
				   m_ptr->Goi2, m_ptr->Yomi, m_ptr->Goi,	
				   &(m_ptr->Hinshi), &(m_ptr->Bunrui),
				   &(m_ptr->Katuyou_Kata), &(m_ptr->Katuyou_Kei),
				   rest_buffer);
#endif

		if (mrph_item == 8) {
		/* �Ӗ�����feature�� */
		if (strncmp((const char*)rest_buffer, "NIL", 3)) {
			char *imip, *cp;

			/* �ʏ� "" �Ŋ����Ă��� */
			if (rest_buffer[0] == '\"') {
			imip = (char*)&rest_buffer[1];
			if ((cp = strchr(imip, '\"')) != NULL) {
				*cp = '\0';
			}
			art_sprintf_1(m_ptr->Imi, IMI_MAX, "\"%s\"", imip);
			}
			else {
			imip = (char*)rest_buffer;
			if ((cp = strchr(imip, ' ')) != NULL) {
				*cp = '\0';
			}
			art_strcpy(m_ptr->Imi, IMI_MAX, imip);
			}

			imi2feature(imip, m_ptr);
		}
		else {
			art_strcpy(m_ptr->Imi, IMI_MAX, "NIL");
		}
		}
		else if (mrph_item == 7) {
		art_strcpy(m_ptr->Imi, IMI_MAX, "NIL");
		}
		else {
		fprintf(stderr, ";; Invalid input (%d items)<%s> !\n",	
			mrph_item, input_buffer);
		if (sp->Comment) fprintf(stderr, "(%s)\n", sp->Comment);
		return readtoeos(fp);
		}	

		/* clear_feature(&(m_ptr->f));	
		   main�̕����Ƃ̃��[�v�̐擪�ŏ����Ɉړ� */

		/* ���`�ً`��͈�U sp->mrph_data �ɂ���� */
		if (homo_flag) homo_num++;

		sp->Mrph_num++;
		m_ptr++;
	}
	}
}

/*==================================================================*/
		   void *change_mrph_rep(MRPH_DATA *m_ptr)
/*==================================================================*/
{
	char pre[WORD_LEN_MAX + 1], str[WORD_LEN_MAX + 1], post[WORD_LEN_MAX + 1], *cp;

	init_str();

	/* ���p����i���ł͂Ȃ��ꍇ */
	if (m_ptr->Katuyou_Kata == 0 || m_ptr->Katuyou_Kei == 0) {
		return NULL;
	}

	/* �u��\�\�L:�����v->�u��\�\�L:�����v */

	if ((cp = strstr(m_ptr->Imi, daihyou)) != NULL) {
	cp += 9;
	art_sscanf_2(cp, "%[^ \"]", str, WORD_LEN_MAX + 1);
	pre[0] = '\0';
	post[0] = '\0';
	art_strncat(pre, WORD_LEN_MAX + 1, m_ptr->Imi, cp - m_ptr->Imi);
	art_strcat(post, WORD_LEN_MAX + 1, cp + strlen(str));
	}
	else {
		return NULL;
	}

	/* �ꊲ�ɂ��� */
	str[strlen((const char*)str) - strlen((const char*)Form[m_ptr->Katuyou_Kata][get_form_id((unsigned char*)BASIC_FORM, m_ptr->Katuyou_Kata)].gobi)] = '\0';

	/* ���p�`������ */
	art_strcat(str, WORD_LEN_MAX + 1, (const char*)Form[m_ptr->Katuyou_Kata][m_ptr->Katuyou_Kei].gobi);

	art_sprintf_3(m_ptr->Imi, "%s%*s%s", pre, strlen(str), str, post);

	/* feature�̏C�� */
	art_sprintf_2(pre, WORD_LEN_MAX + 1, daihyou_s, strlen(str), str);
	assign_cfeature(&(m_ptr->f), pre);

	return NULL;
}

/*==================================================================*/
		void change_mrph(MRPH_DATA *m_ptr, FEATURE *f)
/*==================================================================*/
{
	char h_buffer[62], b_buffer[62], kata_buffer[62], kei_buffer[62];
	char org_buffer[512];
	int num;

	init_str();

	/* ���Ƃ̌`�ԑf����feature�Ƃ��ĕۑ� */
	art_sprintf_7(org_buffer, 512, hinnshi_henkou,	
		m_ptr->Goi2, m_ptr->Yomi, m_ptr->Goi,	
		m_ptr->Hinshi, m_ptr->Bunrui,	
		m_ptr->Katuyou_Kata, m_ptr->Katuyou_Kei);
	assign_cfeature(&(m_ptr->f), org_buffer);

	/* �܂��A��\�\�L���C�� */
	change_mrph_rep(m_ptr);

	m_ptr->Hinshi = 0;
	m_ptr->Bunrui = 0;
	m_ptr->Katuyou_Kata = 0;
	m_ptr->Katuyou_Kei = 0;

#if _WINDOWS && _MFC_VER >= 0x0800
	num = art_sscanf_8(f->cp, "%*[^:]:%[^:]:%[^:]:%[^:]:%[^:]", 
		 h_buffer, 62, b_buffer, 62, kata_buffer, 62, kei_buffer, 62);
#else
	num = art_sscanf_4(f->cp, "%*[^:]:%[^:]:%[^:]:%[^:]:%[^:]", 
		 h_buffer, b_buffer, kata_buffer, kei_buffer);
#endif

	m_ptr->Hinshi = get_hinsi_id((unsigned char*)h_buffer);
	if (num >= 2) {
	if (!strcmp(b_buffer, "*"))
		m_ptr->Bunrui = 0;
	else	
		m_ptr->Bunrui = get_bunrui_id((unsigned char*)b_buffer, m_ptr->Hinshi);
	}
	if (num >= 3) {
	m_ptr->Katuyou_Kata = get_type_id((unsigned char*)kata_buffer);
	m_ptr->Katuyou_Kei = get_form_id((unsigned char*)kei_buffer,	
					 m_ptr->Katuyou_Kata);
	}
	
	/* �i���ύX�����p�Ȃ��̏ꍇ�͌��^���ύX���� */
	/* �� �t(���p�Ȃ������p����)�͈����Ă��Ȃ� */
	if (m_ptr->Katuyou_Kata == 0) {
	art_strcpy(m_ptr->Goi, WORD_LEN_MAX+1, m_ptr->Goi2);
	}
}

/*==================================================================*/
			  int get_Bunrui(char *cp)
/*==================================================================*/
{
	int j;

	for (j = 1; Class[6][j].id; j++) {
	if (str_eq((const char*)Class[6][j].id, cp))
		return j;
	}
	return 0;
}

/*==================================================================*/
			int break_feature(FEATURE *fp)
/*==================================================================*/
{
	while (fp) {
	if (!strcmp(fp->cp, "&break:normal"))	
		return RLOOP_BREAK_NORMAL;
	else if (!strcmp(fp->cp, "&break:jump"))	
		return RLOOP_BREAK_JUMP;
	else if (!strncmp(fp->cp, "&break", strlen("&break")))
		return RLOOP_BREAK_NORMAL;
	fp = fp->next;
	}
	return RLOOP_BREAK_NONE;
}

/*==================================================================*/
	   void assign_mrph_feature(MrphRule *s_r_ptr, int r_size,
				MRPH_DATA *s_m_ptr, int m_length,
				int mode, int break_mode, int direction)
/*==================================================================*/
{
	/* ����͈�(���S��,���ߓ��Ȃ�)�ɑ΂��Č`�ԑf�̃}�b�`���O���s�� */

	int i, j, k, match_length, feature_break_mode;
	MrphRule *r_ptr;
	MRPH_DATA *m_ptr;

	/* �t�����ɓK�p����ꍇ�̓f�[�^�̂�����������Ă����K�v������ */
	if (direction == RtoL)
	s_m_ptr += m_length-1;
	
	/* MRM
		1.self_pattern�̐擪�̌`�ԑf�ʒu
	  2.���[��
		3.self_pattern�̖����̌`�ԑf�ʒu
	�̏��Ƀ��[�v����� (3�̃��[�v��regexpmrphrule_match�̒�)
	
	break_mode == RLOOP_BREAK_NORMAL
		2�̃��x����break����
	break_mode == RLOOP_BREAK_JUMP
		2�̃��x����break���Cself_pattern������1�̃��[�v��i�߂�
	 */

	if (mode == RLOOP_MRM) {
	for (i = 0; i < m_length; i++) {
		r_ptr = s_r_ptr;
		m_ptr = s_m_ptr+(i*direction);
		for (j = 0; j < r_size; j++, r_ptr++) {
		if ((match_length = 
			 regexpmrphrule_match(r_ptr, m_ptr, 
					  direction == LtoR ? i : m_length-i-1, 
					  direction == LtoR ? m_length-i : i+1)) != -1) {
			for (k = 0; k < match_length; k++)
			assign_feature(&((s_m_ptr+i*direction+k)->f),	
					   &(r_ptr->f), s_m_ptr+i*direction+k);
			feature_break_mode = break_feature(r_ptr->f);
			if (break_mode == RLOOP_BREAK_NORMAL ||
			feature_break_mode == RLOOP_BREAK_NORMAL) {
			break;
			} else if (break_mode == RLOOP_BREAK_JUMP ||
				   feature_break_mode == RLOOP_BREAK_JUMP) {
			i += match_length - 1;
			break;
			}
		}
		}
	}
	}

	/* RMM
		1.���[��
	  2.self_pattern�̐擪�̌`�ԑf�ʒu
		3.self_pattern�̖����̌`�ԑf�ʒu
	�̏��Ƀ��[�v����� (3�̃��[�v��regexpmrphrule_match�̒�)
	
	break_mode == RLOOP_BREAK_NORMAL||RLOOP_BREAK_JUMP
		2�̃��x����break���� (�����̎g�����͍l���ɂ�����)
	*/

	else if (mode == RLOOP_RMM) {
	r_ptr = s_r_ptr;
	for (j = 0; j < r_size; j++, r_ptr++) {
		feature_break_mode = break_feature(r_ptr->f);
		for (i = 0; i < m_length; i++) {
		m_ptr = s_m_ptr+(i*direction);
		if ((match_length = 
			 regexpmrphrule_match(r_ptr, m_ptr, 
					  direction == LtoR ? i : m_length-i-1, 
					  direction == LtoR ? m_length-i : i+1)) != -1) {
			for (k = 0; k < match_length; k++)
			assign_feature(&((s_m_ptr+i*direction+k)->f),	
					   &(r_ptr->f), s_m_ptr+i*direction+k);
			if (break_mode == RLOOP_BREAK_NORMAL ||
			break_mode == RLOOP_BREAK_JUMP ||
			feature_break_mode == RLOOP_BREAK_NORMAL ||
			feature_break_mode == RLOOP_BREAK_JUMP) {
			break;
			}
		}
		}
	}
	}
}

/*==================================================================*/
void assign_tag_feature(BnstRule *s_r_ptr, int r_size,
			TAG_DATA *s_b_ptr, int b_length,
			int mode, int break_mode, int direction)
/*==================================================================*/
{
	/* ����͈�(���S��,���ߓ��Ȃ�)�ɑ΂��ă^�O�P�ʂ̃}�b�`���O���s�� */

	int i, j, k, match_length, feature_break_mode;
	BnstRule *r_ptr;
	TAG_DATA *b_ptr;

	/* �t�����ɓK�p����ꍇ�̓f�[�^�̂�����������Ă����K�v������ */
	if (direction == RtoL)
	s_b_ptr += b_length-1;
	
	/* MRM
		1.self_pattern�̐擪�̕��߈ʒu
	  2.���[��
		3.self_pattern�̖����̕��߈ʒu
	�̏��Ƀ��[�v����� (3�̃��[�v��regexpbnstrule_match�̒�)
	
	break_mode == RLOOP_BREAK_NORMAL
		2�̃��x����break����
	break_mode == RLOOP_BREAK_JUMP
		2�̃��x����break���Cself_pattern������1�̃��[�v��i�߂�
	 */

	if (mode == RLOOP_MRM) {
	for (i = 0; i < b_length; i++) {
		r_ptr = s_r_ptr;
		b_ptr = s_b_ptr+(i*direction);
		for (j = 0; j < r_size; j++, r_ptr++) {
		if ((match_length = 
			 regexptagrule_match(r_ptr, b_ptr,	
					 direction == LtoR ? i : b_length-i-1,	
					 direction == LtoR ? b_length-i : i+1)) != -1) {
			for (k = 0; k < match_length; k++)
			assign_feature(&((s_b_ptr+i*direction+k)->f),	
					   &(r_ptr->f), s_b_ptr+i*direction+k);
			feature_break_mode = break_feature(r_ptr->f);
			if (break_mode == RLOOP_BREAK_NORMAL ||
			feature_break_mode == RLOOP_BREAK_NORMAL) {
			break;
			} else if (break_mode == RLOOP_BREAK_JUMP ||
				   feature_break_mode == RLOOP_BREAK_JUMP) {
			i += match_length - 1;
			break;
			}
		}
		}
	}
	}

	/* RMM
		1.���[��
	  2.self_pattern�̐擪�̕��߈ʒu
		3.self_pattern�̖����̕��߈ʒu
	�̏��Ƀ��[�v����� (3�̃��[�v��regexpbnstrule_match�̒�)
	
	break_mode == RLOOP_BREAK_NORMAL||RLOOP_BREAK_JUMP
		2�̃��x����break���� (�����̎g�����͍l���ɂ�����)
	*/

	else if (mode == RLOOP_RMM) {
	r_ptr = s_r_ptr;
	for (j = 0; j < r_size; j++, r_ptr++) {
		feature_break_mode = break_feature(r_ptr->f);
		for (i = 0; i < b_length; i++) {
		b_ptr = s_b_ptr+(i*direction);
		if ((match_length = 
			 regexptagrule_match(r_ptr, b_ptr,	
					 direction == LtoR ? i : b_length-i-1, 
					 direction == LtoR ? b_length-i : i+1)) != -1) {
			for (k = 0; k < match_length; k++)
			assign_feature(&((s_b_ptr+i*direction+k)->f),	
					   &(r_ptr->f), s_b_ptr+i*direction+k);
			if (break_mode == RLOOP_BREAK_NORMAL ||
			break_mode == RLOOP_BREAK_JUMP ||
			feature_break_mode == RLOOP_BREAK_NORMAL ||
			feature_break_mode == RLOOP_BREAK_JUMP) {
			break;
			}
		}
		}
	}
	}
}

/*==================================================================*/
void assign_bnst_feature(BnstRule *s_r_ptr, int r_size,
			 BNST_DATA *s_b_ptr, int b_length,
			 int mode, int break_mode, int direction)
/*==================================================================*/
{
	/* ����͈�(���S��,���ߓ��Ȃ�)�ɑ΂��ĕ��߂̃}�b�`���O���s�� */

	int i, j, k, match_length, feature_break_mode;
	BnstRule *r_ptr;
	BNST_DATA *b_ptr;

	/* �t�����ɓK�p����ꍇ�̓f�[�^�̂�����������Ă����K�v������ */
	if (direction == RtoL)
	s_b_ptr += b_length-1;
	
	/* MRM
		1.self_pattern�̐擪�̕��߈ʒu
	  2.���[��
		3.self_pattern�̖����̕��߈ʒu
	�̏��Ƀ��[�v����� (3�̃��[�v��regexpbnstrule_match�̒�)
	
	break_mode == RLOOP_BREAK_NORMAL
		2�̃��x����break����
	break_mode == RLOOP_BREAK_JUMP
		2�̃��x����break���Cself_pattern������1�̃��[�v��i�߂�
	 */

	if (mode == RLOOP_MRM) {
	for (i = 0; i < b_length; i++) {
		r_ptr = s_r_ptr;
		b_ptr = s_b_ptr+(i*direction);
		for (j = 0; j < r_size; j++, r_ptr++) {
		if ((match_length = 
			 regexpbnstrule_match(r_ptr, b_ptr, 
					  direction == LtoR ? i : b_length-i-1, 
					  direction == LtoR ? b_length-i : i+1)) != -1) {
			for (k = 0; k < match_length; k++)
			assign_feature(&((s_b_ptr+i*direction+k)->f),	
					   &(r_ptr->f), s_b_ptr+i*direction+k);
			feature_break_mode = break_feature(r_ptr->f);
			if (break_mode == RLOOP_BREAK_NORMAL ||
			feature_break_mode == RLOOP_BREAK_NORMAL) {
			break;
			} else if (break_mode == RLOOP_BREAK_JUMP ||
				   feature_break_mode == RLOOP_BREAK_JUMP) {
			i += match_length - 1;
			break;
			}
		}
		}
	}
	}

	/* RMM
		1.���[��
	  2.self_pattern�̐擪�̕��߈ʒu
		3.self_pattern�̖����̕��߈ʒu
	�̏��Ƀ��[�v����� (3�̃��[�v��regexpbnstrule_match�̒�)
	
	break_mode == RLOOP_BREAK_NORMAL||RLOOP_BREAK_JUMP
		2�̃��x����break���� (�����̎g�����͍l���ɂ�����)
	*/

	else if (mode == RLOOP_RMM) {
	r_ptr = s_r_ptr;
	for (j = 0; j < r_size; j++, r_ptr++) {
		feature_break_mode = break_feature(r_ptr->f);
		for (i = 0; i < b_length; i++) {
		b_ptr = s_b_ptr+(i*direction);
		if ((match_length = 
			 regexpbnstrule_match(r_ptr, b_ptr, 
					  direction == LtoR ? i : b_length-i-1, 
					  direction == LtoR ? b_length-i : i+1)) != -1) {
			for (k = 0; k < match_length; k++)
			assign_feature(&((s_b_ptr+i*direction+k)->f),	
					   &(r_ptr->f), s_b_ptr+i*direction+k);
			if (break_mode == RLOOP_BREAK_NORMAL ||
			break_mode == RLOOP_BREAK_JUMP ||
			feature_break_mode == RLOOP_BREAK_NORMAL ||
			feature_break_mode == RLOOP_BREAK_JUMP) {
			break;
			}
		}
		}
	}
	}
}

/*==================================================================*/
	 void assign_general_feature(void *data, int size, int flag)
/*==================================================================*/
{
	int i;
	void (*assign_function)(...) = NULL;

	/* �`�ԑf, �^�O�P��, ���߂̏ꍇ���� */
	if (flag == MorphRuleType) {
	assign_function = (void(*)(...))assign_mrph_feature;
	}
	else if (flag == TagRuleType) {
	assign_function = (void(*)(...))assign_tag_feature;
	}
	else if (flag == BnstRuleType) {
	assign_function = (void(*)(...))assign_bnst_feature;
	}

	for (i = 0; i < GeneralRuleNum; i++) {
	if ((GeneralRuleArray + i)->type == flag) {
		assign_function((GeneralRuleArray+i)->RuleArray,	
				(GeneralRuleArray+i)->CurRuleSize,	
				data, size, 
				(GeneralRuleArray+i)->mode, 
				(GeneralRuleArray+i)->breakmode,	
				(GeneralRuleArray+i)->direction);
	}
	}
}

/*==================================================================*/
	  BNST_DATA *init_bnst(SENTENCE_DATA *sp, MRPH_DATA *m_ptr)
/*==================================================================*/
{
	int i;
	char *cp;
	BNST_DATA *b_ptr;

	b_ptr = sp->bnst_data + sp->Bnst_num;
	b_ptr->type = IS_BNST_DATA;
	b_ptr->num = sp->Bnst_num;
	sp->Bnst_num++;
	if (sp->Bnst_num > BNST_MAX) {
	fprintf(stderr, ";; Too many bnst (%s %s%s...)!\n", 
		sp->Comment ? sp->Comment : "", sp->mrph_data, sp->mrph_data+1);
	sp->Bnst_num = 0;
	return NULL;
	}

	b_ptr->mrph_ptr = m_ptr;
	b_ptr->mrph_num = 0;

	b_ptr->BGH_num = 0;
	b_ptr->SM_num = 0;

	b_ptr->para_key_type = PARA_KEY_O;
	b_ptr->para_top_p = FALSE;
	b_ptr->para_type = PARA_NIL;
	b_ptr->to_para_p = FALSE;

	b_ptr->cpm_ptr = NULL;
	b_ptr->voice = 0;

	b_ptr->length = 0;
	b_ptr->space = 0;

	b_ptr->pred_b_ptr = NULL;
	
	for (i = 0, cp = b_ptr->SCASE_code; i < SCASE_CODE_SIZE; i++, cp++) *cp = 0;

	/* clear_feature(&(b_ptr->f));
	   main�̕����Ƃ̃��[�v�̐擪�ŏ����Ɉړ� */

	return b_ptr;
}

/*==================================================================*/
	void make_Jiritu_Go(SENTENCE_DATA *sp, BNST_DATA *ptr)
/*==================================================================*/
{
	MRPH_DATA *mp;

	init_str();

	ptr->Jiritu_Go[0] = '\0';

	/* �厫���O�̕����Őړ����ȊO��������Ƃ��Ă����Ă��� */
	for (mp = ptr->mrph_ptr; mp <= ptr->head_ptr; mp++) {
	if (!check_feature(mp->f, settou)) {
		if (strlen(ptr->Jiritu_Go) + strlen(mp->Goi) + 2 > BNST_LENGTH_MAX) {
		fprintf(stderr, ";; Too big bunsetsu (%s %s...)!\n",	
			sp->Comment ? sp->Comment : "", ptr->mrph_ptr);
		return;
		}
		art_strcat(ptr->Jiritu_Go, BNST_LENGTH_MAX, mp->Goi);
	}
	}
}

/*==================================================================*/
		 void decide_head_ptr(BNST_DATA *ptr)
/*==================================================================*/
{
	int i;

	init_str();

	for (i = ptr->mrph_num - 1; i >= 0 ; i--) {
	if (check_feature((ptr->mrph_ptr + i)->f, fuzoku)) {
		/* �J�E���^�ȂǁA�t����ł��邪�Ӗ��f���������瓾��ꍇ */
		if (check_feature((ptr->mrph_ptr + i)->f, dokuritsu_setsubi) || 
		check_feature((ptr->mrph_ptr + i)->f, hidokuritsu_setsubi) ||	
		(ptr->type == IS_TAG_DATA && check_feature((ptr->mrph_ptr + i)->f, dokuritsu_muimi))) { /* �u�́v */
		ptr->head_ptr = ptr->mrph_ptr + i;
		return;
		}
	}
	/* �Ō�̎����� */
	else {
		ptr->head_ptr = ptr->mrph_ptr + i;
		return;
	}
	}

	/* �t���ꂵ���Ȃ��ꍇ */
	ptr->head_ptr = ptr->mrph_ptr + ptr->mrph_num - 1;
}

/*==================================================================*/
	  int calc_bnst_length(SENTENCE_DATA *sp, BNST_DATA *b_ptr)
/*==================================================================*/
{
	int j;
	MRPH_DATA *m_ptr;

	for (j = 0, m_ptr = b_ptr->mrph_ptr; j < b_ptr->mrph_num; j++, m_ptr++) {
	if ((b_ptr->length += (int)strlen(m_ptr->Goi2)) > BNST_LENGTH_MAX) {
		fprintf(stderr, ";; Too big bunsetsu (%s %s...)!\n",	
			sp->Comment ? sp->Comment : "", b_ptr->mrph_ptr);
		return FALSE;
	}
	}
	return TRUE;
}

/*==================================================================*/
		 int make_bunsetsu(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	MRPH_DATA	*m_ptr;
	BNST_DATA	*b_ptr = NULL;

	init_str();

	sp->Bnst_num = 0;
	sp->Max_New_Bnst_num = 0;

	for (i = 0, m_ptr = sp->mrph_data; i < sp->Mrph_num; i++, m_ptr++) {
	if (check_feature(m_ptr->f, bunsetsu_begin)) {
		if ((b_ptr = init_bnst(sp, m_ptr)) == NULL) return FALSE;
	}
	b_ptr->mrph_num++;
	}

	for (i = 0, b_ptr = sp->bnst_data; i < sp->Bnst_num; i++, b_ptr++) {
	if (calc_bnst_length(sp, b_ptr) == FALSE) {
		return FALSE;
	}
	}
	return TRUE;
}

/*==================================================================*/
		   int make_bunsetsu_pm(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j;
	char *cp;
	MRPH_DATA	*m_ptr;
	BNST_DATA	*b_ptr = sp->bnst_data;

	init_str();

	for (i = 0, m_ptr = sp->mrph_data; i < sp->Mrph_num; i++, m_ptr++) {
	if (Bnst_start[i]) {
		if (i != 0) b_ptr++;
		b_ptr->type = IS_BNST_DATA;
		b_ptr->num = (int)(b_ptr-sp->bnst_data);
		b_ptr->mrph_ptr = m_ptr;
		b_ptr->mrph_num = 1;
		b_ptr->length = 0;
		b_ptr->cpm_ptr = NULL;
		b_ptr->voice = 0;
		b_ptr->pred_b_ptr = NULL;
		for (j = 0, cp = b_ptr->SCASE_code; j < SCASE_CODE_SIZE; j++, cp++)
		*cp = 0;
		/* clear_feature(&(b_ptr->f));
		   main�̕����Ƃ̃��[�v�̐擪�ŏ����Ɉړ� */
	}
	else {
		b_ptr->mrph_num++;
	}
	}

	for (i = 0, b_ptr = sp->bnst_data; i < sp->Bnst_num; i++, b_ptr++) {
	if (OptReadFeature) {
		b_ptr->f = Input_bnst_feature[i];
	}
	assign_cfeature(&(b_ptr->f), ana_done);
	if (calc_bnst_length(sp, b_ptr) == FALSE) {
		return FALSE;
	}
	}
	return TRUE;
}

/*==================================================================*/
	   void push_tag_units(TAG_DATA *tp, MRPH_DATA *mp)
/*==================================================================*/
{
	init_str();

	if (check_feature(mp->f, settou_hidokuritsu)) {
	if (tp->settou_num == 0) {
		tp->settou_ptr = mp;
	}
	tp->settou_num++;
	}
	else if (check_feature(mp->f, jiritsu) ||	
		 check_feature(mp->f, dokuritsu_settou) ||	
		 check_feature(mp->f, dokuritsu_setsubi) || 
		 check_feature(mp->f, dokuritsu_muimi)) {
	if (tp->jiritu_num == 0) {
		tp->jiritu_ptr = mp;
	}
	tp->jiritu_num++;
	}
	else {
	if (tp->fuzoku_num == 0) {
		tp->fuzoku_ptr = mp;
	}
	tp->fuzoku_num++;
	}
	tp->mrph_num++;
}

/*==================================================================*/
		 void after_make_tag_units(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	TAG_DATA *tp;

	init_str();

	for (i = 0; i < sp->Tag_num; i++) {
	tp = sp->tag_data + i;

	tp->type = IS_TAG_DATA;

	decide_head_ptr((BNST_DATA *)tp);

	/* BNST_DATA��cast���Ă��� tricky? */
	get_bnst_code_all((BNST_DATA *)tp);

	if (tp->inum != 0) {
		/* case_analysis.rule �Ŏg���Ă��� */
		assign_cfeature(&(tp->f), inside_bunsetsu);
	}
	else {
		/* head�̂Ƃ��͕��߂�feature���R�s�[ */
		/* <����>, <����>�������A�����̕��߂�2�^�O�P�ʈȏ���ꍇ�́A
		   <����>�̂��ʒu���Ԉ���Ă���̂ŉ��ŏC������ */
		copy_feature(&(tp->f), tp->b_ptr->f);
	}

	/* �e�^�O�P�ʂ̒������v�Z���Ă��� */
	calc_bnst_length(sp, (BNST_DATA *)tp);

	if (OptReadFeature) {
		tp->f = Input_tag_feature[i];
	}
	}

	/* <����>�̏C�� */
	if (sp->bnst_data->tag_num > 1) {
	delete_cfeature(&((sp->bnst_data->tag_ptr + sp->bnst_data->tag_num - 1)->f), buntou);
	assign_cfeature(&(sp->tag_data->f), buntou);
	}

	/* �^�O�P�ʃ��[����K�p���� */
	assign_general_feature(sp->tag_data, sp->Tag_num, TagRuleType);
}

/*==================================================================*/
	   void make_tag_unit_set_inum(SENTENCE_DATA *sp, int num)
/*==================================================================*/
{
	int j, count = 0;

	for (j = num - 2; j >= 0; j--) {
	(sp->tag_data + j)->inum = ++count;
	if ((sp->tag_data + j)->bnum >= 0) {
		break;
	}
	}
}

/*==================================================================*/
		void make_tag_units(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	char *flag;
	MRPH_DATA *mp;
	TAG_DATA *tp = NULL;
	BNST_DATA *bp = sp->bnst_data, *pre_bp = NULL;

	init_str();

	sp->Tag_num = 0;

	for (i = 0; i < sp->Mrph_num; i++) {
	mp = sp->mrph_data + i;
	flag = check_feature(mp->f, tag_unit);

	/* ���ߎn�܂�̌`�ԑf������<�^�O�P�ʎn>�����Ă��Ȃ��ꍇ������ */
	if (flag || 
		(bp != NULL && bp->mrph_ptr == mp)) {
		tp = sp->tag_data + sp->Tag_num;

		if (flag == NULL) {
		fprintf(stderr, tag_unit_s, i,	
			sp->KNPSID ? sp->KNPSID : "?");
		}

		memset(tp, 0, sizeof(TAG_DATA));
		tp->num = sp->Tag_num;
		tp->mrph_ptr = mp;

		/* ���ߋ�؂�ƈ�v����Ƃ� */
		if (bp != NULL && bp->mrph_ptr == tp->mrph_ptr) {
		/* �k����inum��t�^ */
		if (sp->Tag_num > 0 && (tp - 1)->bnum < 0) {
			make_tag_unit_set_inum(sp, sp->Tag_num);
		}
		tp->bnum = bp->num;
		tp->b_ptr = bp; 	/* �^�O�P�ʂ��當�߂փ}�[�N */
		bp->tag_ptr = tp;	/* ���߂���^�O�P�ʂփ}�[�N */
		bp->tag_num = 1;
		pre_bp = bp;
		if (bp->num < sp->Bnst_num - 1) {
			bp++;
		}
		else {
			/* �Ō�̕��߂��I����� */
			bp = NULL;
		}
		}
		else {
		tp->bnum = -1;
		tp->b_ptr = pre_bp;
		pre_bp->tag_num++;
		}
		sp->Tag_num++;
	}
	push_tag_units(tp, mp);
	}

	if ((sp->tag_data + sp->Tag_num - 1)->bnum < 0) {
	make_tag_unit_set_inum(sp, sp->Tag_num);
	}

	after_make_tag_units(sp);
}

/*==================================================================*/
		  void make_tag_units_pm(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	MRPH_DATA *mp;
	TAG_DATA *tp = sp->tag_data;
	BNST_DATA *bp = sp->bnst_data, *pre_bp = NULL;

	init_str();

	for (i = 0; i < sp->Mrph_num; i++) {
	mp = sp->mrph_data + i;

	if (Tag_start[i]) {
		if (i != 0) tp++;

		if (check_feature(mp->f, tag_unit) == NULL) {
		fprintf(stderr, tag_unit_s, i,	
			sp->KNPSID ? sp->KNPSID : "?");
		}

		memset(tp, 0, sizeof(TAG_DATA));
		tp->num = (int)(tp - sp->tag_data);
		tp->mrph_ptr = mp;

		/* ���ߋ�؂�ƈ�v����Ƃ� */
		if (bp != NULL && bp->mrph_ptr == tp->mrph_ptr) {
		/* �k����inum��t�^ */
		if (tp->num > 0 && (tp - 1)->bnum < 0) {
			make_tag_unit_set_inum(sp, tp->num);
		}
		tp->bnum = bp->num;
		tp->b_ptr = bp; 	/* �^�O�P�ʂ��當�߂փ}�[�N */
		bp->tag_ptr = tp;	/* ���߂���^�O�P�ʂփ}�[�N */
		bp->tag_num = 1;
		pre_bp = bp;
		if (bp->num < sp->Bnst_num - 1) {
			bp++;
		}
		else {
			/* �Ō�̕��߂��I����� */
			bp = NULL;
		}
		}
		else {
		tp->bnum = -1;
		tp->b_ptr = pre_bp;
		pre_bp->tag_num++;
		}
	}
	push_tag_units(tp, mp);
	}

	if ((sp->tag_data + sp->Tag_num - 1)->bnum < 0) {
	make_tag_unit_set_inum(sp, sp->Tag_num);
	}

	after_make_tag_units(sp);
}


/*==================================================================*/
		 void dpnd_info_to_tag_pm(SENTENCE_DATA *sp)
/*==================================================================*/
{
	/* �W��󂯂Ɋւ����X�̏��� DPND ���� TAG_DATA �ɃR�s�[ (��͍ϔ�) */

	int 	i;

	for (i = 0; i < sp->Tag_num; i++) {
	(sp->tag_data + i)->dpnd_head = Tag_dpnd[i];
	(sp->tag_data + i)->dpnd_type = (char)Tag_type[i];
	}
}

/*====================================================================
				 END
====================================================================*/
