/*====================================================================

			 NTT  �����v���O����

											   S.Kurohashi 91. 6.25
											   S.Kurohashi 93. 5.31

	$Id: lib_sm.c,v 1.51.2.2 2005/09/06 04:56:24 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

VerboseType VerboseLevel = VERBOSE0;

DBM_FILE	sm_db;
DBM_FILE	sm2code_db;
DBM_FILE	code2sm_db;
DBM_FILE	smp2smg_db;
int 	SMExist;
int 	SM2CODEExist;
int 	CODE2SMExist;
int 	SMP2SMGExist;

char		cont_str[DBM_CON_MAX];

SMLIST smlist[TBLSIZE];

extern void hash_close(HASH_FILE *hashdb);
extern int hash_fetch_size;

/*==================================================================*/
			   void init_ntt()
/*==================================================================*/
{
	char *filename;

	/***  �f�[�^�x�[�X�I�[�v��	***/
	
	/* �P�� <=> �Ӗ��f�R�[�h */

	// �t�@�C�������w�肷��
	if (DICT[SM_DB]) {		
	filename = check_dict_filename(DICT[SM_DB], TRUE);	// .knprc �Œ�`����Ă���Ƃ�	 �� SM_DB �� const.h �Œ�`����Ă���
														//									DICT[SM_DB] �� configfile.c �Ŏw�肳��Ă���
	}
	else {
	filename = check_dict_filename(SM_DB_NAME, FALSE);	// .knprc �Œ�`����Ă��Ȃ��Ƃ� �� path.h �� default�l(SM_DB_NAME) ���g��
	}

	if (OptDisplay == OPT_DEBUG) {
	fprintf(Outfp, "Opening %s ... ", filename);
	}

	if ((sm_db = DB_open(filename, O_RDONLY, 0)) == NULL) {
	if (OptDisplay == OPT_DEBUG) {
		fputs("failed.\n", Outfp);
	}
	SMExist = FALSE;
#ifdef KNP_DEBUG
	fprintf(stderr, ";; Cannot open NTT word dictionary <%s>.\n", filename);
#endif
	}
	else {
	if (OptDisplay == OPT_DEBUG) {
		fputs("done.\n", Outfp);
	}
	SMExist = TRUE;
	}
	free(filename);
	THESAURUS[USE_NTT].exist = SMExist;
	
	/* �Ӗ��f => �Ӗ��f�R�[�h */
	if (Thesaurus == USE_NTT) {
	if (DICT[SM2CODE_DB]) {
		filename = check_dict_filename(DICT[SM2CODE_DB], TRUE);
	}
	else {
		filename = check_dict_filename(SM2CODE_DB_NAME, FALSE);
	}

	if (OptDisplay == OPT_DEBUG) {
		fprintf(Outfp, "Opening %s ... ", filename);
	}

	if ((sm2code_db = DB_open(filename, O_RDONLY, 0)) == NULL) {
		if (OptDisplay == OPT_DEBUG) {
		fputs("failed.\n", Outfp);
		}
		SM2CODEExist = FALSE;
#ifdef KNP_DEBUG
		fprintf(stderr, ";; Cannot open NTT sm dictionary <%s>.\n", filename);
#endif
	}
	else {
		if (OptDisplay == OPT_DEBUG) {
		fputs("done.\n", Outfp);
		}
		SM2CODEExist = TRUE;
	}
	free(filename);
	}

	/* �Ӗ��f�R�[�h => �Ӗ��f */
	if (DICT[CODE2SM_DB]) {
	filename = check_dict_filename(DICT[CODE2SM_DB], TRUE);
	}
	else {
	filename = check_dict_filename(CODE2SM_DB_NAME, FALSE);
	}

	if (OptDisplay == OPT_DEBUG) {
	fprintf(Outfp, "Opening %s ... ", filename);
	}

	if ((code2sm_db = DB_open(filename, O_RDONLY, 0)) == NULL) {
	if (OptDisplay == OPT_DEBUG) {
		fputs("failed.\n", Outfp);
	}
	CODE2SMExist = FALSE;
#ifdef KNP_DEBUG
	fprintf(stderr, ";; Cannot open NTT code2sm dictionary <%s>.\n", filename);
#endif
	}
	else {
	if (OptDisplay == OPT_DEBUG) {
		fputs("done.\n", Outfp);
	}
	CODE2SMExist = TRUE;
	}
	free(filename);

	/* �ŗL�����̌n <=> ��ʖ����̌n */
	if (DICT[SMP2SMG_DB]) {
	filename = check_dict_filename(DICT[SMP2SMG_DB], TRUE);
	}
	else {
	filename = check_dict_filename(SMP2SMG_DB_NAME, FALSE);
	}

	if (OptDisplay == OPT_DEBUG) {
	fprintf(Outfp, "Opening %s ... ", filename);
	}

	if ((smp2smg_db = DB_open(filename, O_RDONLY, 0)) == NULL) {
	if (OptDisplay == OPT_DEBUG) {
		fputs("failed.\n", Outfp);
	}
	SMP2SMGExist = FALSE;
#ifdef KNP_DEBUG
	fprintf(stderr, ";; Cannot open NTT smp smg table <%s>.\n", filename);
#endif
	}
	else {
	if (OptDisplay == OPT_DEBUG) {
		fputs("done.\n", Outfp);
	}
	SMP2SMGExist = TRUE;
	}
	free(filename);
}


/*==================================================================*/
			   void close_ntt()
/*==================================================================*/
{
	if (SMExist == 1 /*TRUE*/)
	DB_close(sm_db);

	if (SM2CODEExist == 1 /*TRUE*/)
	DB_close(sm2code_db);

	if (SMP2SMGExist == 1 /*TRUE*/)
	DB_close(smp2smg_db);
}

/*==================================================================*/
		   int ne_check_all_sm(char *code)
/*==================================================================*/
{
	int i;

	/* ���ׂĂ̈Ӗ��������ŗL�����Ȃ� TRUE */

	for (i = 0; *(code+i); i+=SM_CODE_SIZE) {
	if (*(code+i) != '2') {
		return FALSE;
	}
	}
	return TRUE;
}

/*==================================================================*/
				 char *_get_ntt(char *cp, char *arg)
/*==================================================================*/
{

	/* �f�[�^�x�[�X������o���� code ���������� */
	int i, pos;
	char *code;

	code = db_get(sm_db, cp);
	if (code) {

	/* ��ꂽ��A�k�߂� */
	if (strlen(code) > SM_CODE_SIZE*SM_ELEMENT_MAX) {
#ifdef KNP_DEBUG
		fprintf(stderr, "Too long SM content <%s>.\n", code);
#endif
		code[SM_CODE_SIZE*SM_ELEMENT_MAX] = '\0';
	}
	
	pos = 0;
	
	/* ���ׂĂ̈Ӗ��������ŗL�����̂Ƃ� */
	if (ne_check_all_sm(code) == 1 /*TRUE*/) {
		for (i = 0; code[i]; i+=SM_CODE_SIZE) {
		if (code[i] == '2' &&	
			strncmp(code+i, "2001030", 7)) { /* �厚 �ł͂Ȃ� */
			art_strncpy(code+pos, SM_CODE_SIZE*SM_ELEMENT_MAX-pos, code+i, SM_CODE_SIZE);
			pos += SM_CODE_SIZE;
		}
		}
	}
	else {
		/* �Ӗ��f��t�^����i�� */
		for (i = 0; code[i]; i+=SM_CODE_SIZE) {
		if ((*arg && code[i] == *arg) ||	/* �w�肳�ꂽ�i�� */
			code[i] == '3' ||	/* �� */
			code[i] == '4' ||	/* ��(�`��) */
			code[i] == '5' ||	/* ��(�`��) */
			code[i] == '6' ||	/* ��(�]��) */
			code[i] == '7' ||	/* �T�� */
			code[i] == '9' ||	/* ���� */
			code[i] == 'a') {	/* �㖼 */
			art_strncpy(code+pos, SM_CODE_SIZE*SM_ELEMENT_MAX-pos, code+i, SM_CODE_SIZE);
			pos += SM_CODE_SIZE;
		}
		}
	}
	code[pos] = '\0';
	}
	return code;
}

/*==================================================================*/
			   char *sm2code(char *cp)
/*==================================================================*/
{
	char *code;

	/* sm �� code �� 1:1 �Ή�	
	   -> cont_str �͈��Ȃ� */

	if (SM2CODEExist == FALSE) {
	cont_str[0] = '\0';
	return cont_str;
	}

	code = db_get(sm2code_db, cp);
	if (code) {
	art_strcpy(cont_str, DBM_CON_MAX, code);
	free(code);
	}
	else {
	cont_str[0] = '\0';
	}
	return cont_str;
}

/*==================================================================*/
			   char *code2sm(char *cp)
/*==================================================================*/
{
	char *sm;

	/* sm �� code �� 1:1 �Ή�	
	   -> cont_str �͈��Ȃ� */

	if (CODE2SMExist == FALSE) {
	cont_str[0] = '\0';
	return cont_str;
	}

	sm = db_get(code2sm_db, cp);
	if (sm) {
	art_strcpy(cont_str, DBM_CON_MAX, sm);
	free(sm);
	}
	else {
	cont_str[0] = '\0';
	}
	return cont_str;
}

/*==================================================================*/
		   void codes2sm_print(FILE *fp, char *cp)
/*==================================================================*/
{
	int i;
	char sm[SM_CODE_SIZE + 1];

	for (i = 0; cp[i]; i += SM_CODE_SIZE) {
	if (i != 0) fputc(',', fp);
	art_strncpy(sm, SM_CODE_SIZE + 1, cp + i, SM_CODE_SIZE);
	sm[0] = '1';
	sm[SM_CODE_SIZE] = '\0';
	fputs(code2sm(sm), fp);
	}
}

/*==================================================================*/
			   char *_smp2smg(char *cp)
/*==================================================================*/
{
	char *code, key[SM_CODE_SIZE+1];

	/* �l�͒����Ă� 52 bytes ���炢 */

	if (SMP2SMGExist == FALSE) {
	cont_str[0] = '\0';
	return cont_str;
	}

	art_strncpy(key, SM_CODE_SIZE+1, cp, SM_CODE_SIZE);
	key[SM_CODE_SIZE] = '\0';

	code = db_get(smp2smg_db, key);
	return code;
}

/*==================================================================*/
		  char *smp2smg(char *cpd, int flag)
/*==================================================================*/
{
	char *cp, *start;
	int storep = 0, inc, use = 1;

	if (SMP2SMGExist == FALSE) {
	fprintf(stderr, ";;; Cannot open smp2smg table!\n");
	return NULL;
	}

	start = _smp2smg(cpd);

	if (start == NULL) {
	return NULL;
	}

	for (cp = start; *cp; cp+=SM_CODE_SIZE) {
	use = 1;
	if (*(cp+SM_CODE_SIZE) == '/') {
		inc = 1;
	}
	else if (!strncmp(cp+SM_CODE_SIZE, " side-effect", 12)) {
		if (*(cp+SM_CODE_SIZE+12) == '/') {
		inc = 13;		
		}
		/* ����ŏI��� */
		else {
		inc = 0;
		}
		/* flag == FALSE �̏ꍇ side-effect ���g��Ȃ� */
		if (flag == FALSE) {
		use = 0;
		}
	}
	else if (*(cp+SM_CODE_SIZE) != '\0') {
		fprintf(stderr, ";;; Invalid delimiter! <%c> (%s)\n",	
			*(cp+SM_CODE_SIZE), "smp2smg");
		inc = 1;
	}
	/* ����ŏI��� '\0' */
	else {
		inc = 0;
	}

	if (use) {
		art_strncpy(start+storep, hash_fetch_size-storep, cp, SM_CODE_SIZE);
		storep+=SM_CODE_SIZE;
	}
	if (inc) {
		cp += inc;
	}
	else {
		break;
	}
	}

	if (storep) {
	*(start+storep) = '\0';
	return start;
	}
	free(start);
	return NULL;
}

/*==================================================================*/
		   void merge_smp2smg(BNST_DATA *bp)
/*==================================================================*/
{
	int i;
	char *p;

	/* smp2smg �̌��ʂ��������� */

	if (bp->SM_code[0] == '\0') {
	return;
	}

	for (i = 0; i < bp->SM_num; i++) {
	if (bp->SM_code[i*SM_CODE_SIZE] == '2') {
		p = smp2smg(&(bp->SM_code[i*SM_CODE_SIZE]), FALSE);
		if (p) {
		/* ��ꂽ�ꍇ */
		if ((strlen(bp->SM_code)+strlen(p))/SM_CODE_SIZE > SM_ELEMENT_MAX) {
			return;
		}
		art_strcat(bp->SM_code, SM_ELEMENT_MAX*SM_CODE_SIZE+1, p);
		free(p);
		}
	}
	}
	bp->SM_num = (int)strlen(bp->SM_code)/SM_CODE_SIZE;
}

/*==================================================================*/
		  float _ntt_code_match(char *c1, char *c2)
/*==================================================================*/
{
	int i, d1, d2, min;

	if ((*c1 == '2' && *c2 != '2') ||	
	(*c1 != '2' && *c2 == '2')) {
	return 0;
	}

	d1 = code_depth(c1, SM_CODE_SIZE);
	d2 = code_depth(c2, SM_CODE_SIZE);

	if (d1 + d2 == 0) {
	return 0;
	}

	min = d1 < d2 ? d1 : d2;

	if (min == 0) {
	return 0;
	}

	for (i = 1; i <= min; i++) {
	if (*(c1+i) != *(c2+i)) {
		return (float)2*(i-1)/(d1+d2);
	}
	}
	return (float)2*min/(d1+d2);
}

/*==================================================================*/
	  float ntt_code_match(char *c1, char *c2, int flag)
/*==================================================================*/
{
	if (flag == SM_EXPAND_NE) {
	float score, maxscore = 0;
	char *cp1, *cp2;
	int i, j;
	int f1 = 0, f2 = 0, c1num = 1, c2num = 1;

	if (*c1 == '2') {
		c1 = smp2smg(c1, FALSE);
		if (!c1) {
		return 0;
		}
		f1 = 1;
		c1num = (int)strlen(c1)/SM_CODE_SIZE;
	}
	if (*c2 == '2') {
		c2 = smp2smg(c2, FALSE);
		if (!c2) {
		if (f1 == 1) {
			free(c1);
		}
		return 0;
		}
		f2 = 1;
		c2num = (int)strlen(c2)/SM_CODE_SIZE;
	}

	for (cp1 = c1, i = 0; i < c1num; cp1+=SM_CODE_SIZE, i++) {
		for (cp2 = c2, j = 0; j < c2num; cp2+=SM_CODE_SIZE, j++) {
		score = _ntt_code_match(cp1, cp2);
		if (score > maxscore) {
			maxscore = score;
		}
		}
	}
	if (f1 == 1) {
		free(c1);
	}
	if (f2 == 1) {
		free(c2);
	}
	return maxscore;
	}
	else if (flag == SM_EXPAND_NE_DATA) {
	float score, maxscore = 0;
	char *cp2;
	int i;
	int f2 = 0, c2num = 1;

	/* PATTERN: �ŗL���� */
	if (*c1 == '2') {
		return _ntt_code_match(c1, c2);
	}

	/* PATTERN: ���ʖ��� */

	if (*c2 == '2') {
		c2 = smp2smg(c2, FALSE);
		if (!c2) {
		return 0;
		}
		f2 = 1;
		c2num = (int)strlen(c2)/SM_CODE_SIZE;
	}

	for (cp2 = c2, i = 0; i < c2num; cp2+=SM_CODE_SIZE, i++) {
		score = _ntt_code_match(c1, cp2);
		if (score > maxscore) {
		maxscore = score;
		}
	}
	if (f2 == 1) {
		free(c2);
	}
	return maxscore;
	}
	else {
	return _ntt_code_match(c1, c2);
	}
}

/*==================================================================*/
		 int comp_sm(char *cpp, char *cpd, int start)
/*==================================================================*/
{
	/* start ����`�F�b�N����
	   ���ʂ� 1 
	   �i�����ƃ`�F�b�N����Ƃ��� 0 */

	int i;

	for (i = start; i < SM_CODE_SIZE; i++) {
	if (cpp[i] == '*')
		return i;
	else if (cpd[i] == '*')
		return 0;
	else if (cpp[i] != cpd[i])
		return 0;
	}
	return SM_CODE_SIZE;
}

/*==================================================================*/
	 int _sm_match_score(char *cpp, char *cpd, int flag)
/*==================================================================*/
{
	/*
	  NTT�̈Ӗ��f�̈ꌅ�ڂ͕i�����
	  �i�t���[�� <-----> �f�[�^
	   x(�╶)	 <-----> x����OK
	   1(����)	 <-----> x�ȊOOK
						 (�����ȊO�̂��̂�get_sm�̎��_�Ŕr�� 99/01/13)
	*/

	/*	
	   flag == SM_EXPAND_NE    : �ŗL�����Ӗ���������ʖ����Ӗ������ɕϊ�����
	   flag == SM_NO_EXPAND_NE : �ŗL�����Ӗ���������ʖ����Ӗ������ɕϊ����Ȃ�
	   flag == SM_CHECK_FULL   : �R�[�h�̈ꕶ���ڂ���`�F�b�N����
	 */

	int current_score, score = 0;
	char *cp;

	if (flag == SM_CHECK_FULL)
	return comp_sm(cpp, cpd, 0);

	if (cpp[0] == 'x') {
	if (cpd[0] == 'x')
		return SM_CODE_SIZE;
	else
		return 0;
	} else {
	if (cpd[0] == 'x')
		return 0;
	}

	/* �Ӗ��}�[�J�̃}�b�`���O�x�̌v�Z

	   �E�p�^�[�������* --- �}�b�`
	   �E�f�[�^�����* --- �}�b�`���Ȃ�
	   �E�Ō�܂ň�v --- �}�b�`

		 �}�b�` : �}�b�`����K�w�̐[����Ԃ�
	 �}�b�`���Ȃ��Ƃ� : 0��Ԃ�
	*/

	/* �f�[�^���ŗL�����̂Ƃ� */
	if (cpd[0] == '2') {
	if (flag == SM_EXPAND_NE && cpp[0] != '2') {
		if (SMP2SMGExist == FALSE) {
		fprintf(stderr, ";;; Cannot open smp2smg table!\n");
		return 0;
		}
		else {
		char *start;
		start = _smp2smg(cpd);
		if (start == NULL) {
			return score;
		}
		for (cp = start; *cp; cp+=SM_CODE_SIZE) {
			if (*cp == '/') {
			cp++;
			}
			else if (cp != start) {
			fprintf(stderr, ";;; Invalid delimiter! <%c> (%s)\n",	
				*cp, start);
			}

			/* ����p�t���O������Ӗ��f�ϊ��͍s��Ȃ� */
			if (!strncmp(cp+SM_CODE_SIZE, " side-effect", 12)) {
			cp += 12; /* " side-effect" �̕��i�߂� */
			continue;
			}

			current_score = comp_sm(cpp, cp, 1);
			if (current_score > score) {
			score = current_score;
			}
		}
		free(start);
		return score;
		}
	}
	else if (flag == SM_NO_EXPAND_NE && cpp[0] == '2')
		return comp_sm(cpp, cpd, 1);
	else
		return 0;
	}
	/* �����Ƃ���ʖ����̂Ƃ� */
	else if (cpp[0] != '2')
	return comp_sm(cpp, cpd, 1);
	else
	return 0;
}

/*==================================================================*/
	int sm_match_check(char *pat, char *codes, int expand)
/*==================================================================*/
{
	int i;

	if (codes == NULL) {
	return FALSE;
	}

	for (i = 0; *(codes+i); i += SM_CODE_SIZE) {
	if (_sm_match_score(pat, codes+i, expand) > 0) {
		return TRUE;
	}
	}
	return FALSE;
}

/*==================================================================*/
		int assign_sm(BNST_DATA *bp, char *cp)
/*==================================================================*/
{
	char *code;
	code = sm2code(cp);

	/* ���łɂ��̈Ӗ������������Ă���Ƃ� */
	if (sm_match_check(code, bp->SM_code, SM_NO_EXPAND_NE) == 1 /*TRUE*/) {
	return FALSE;
	}

	art_strcat(bp->SM_code, SM_ELEMENT_MAX*SM_CODE_SIZE+1, code);
	bp->SM_num++;
	return TRUE;
}

/*==================================================================*/
		   int sm_all_match(char *c, char *target)
/*==================================================================*/
{
	char *p, flag = 0;

	/* �ŗL�����̂Ƃ��ȊO�ŁA���ׂĂ̈Ӗ����������Ԃł���� TRUE */
	for (p = c;*p; p+=SM_CODE_SIZE) {
	/* �ŗL�����̂Ƃ����̂��� */
	if (*p == '2') {
		continue;
	}

	/* �Ӗ��f�̃`�F�b�N */
	if (!comp_sm(target, p, 1)) {
		return FALSE;
	}
	else if (!flag) {
		flag = 1;
	}
	}

	if (flag) {
	return TRUE;
	}
	else {
	return FALSE;
	}
}

/*==================================================================*/
		 int delete_specified_sm(char *sm, char *del)
/*==================================================================*/
{
	int i, j, flag, pos = 0;

	for (i = 0; sm[i]; i += SM_CODE_SIZE) {
	flag = 1;
	/* �ŗL�ł͂Ȃ��Ƃ���ΏۂƂ��� */
	if (sm[i] != '2') {
		for (j = 0; del[j]; j += SM_CODE_SIZE) {
		if (!strncmp(sm+i+1, del+j+1, SM_CODE_SIZE-1)) {
			flag = 0;
			break;
		}
		}
	}
	if (flag) {
		art_strncpy(sm+pos, SM_ELEMENT_MAX, sm+i, SM_CODE_SIZE);
		pos += SM_CODE_SIZE;
	}
	}
	*(sm+pos) = '\0';
	return 1;
}

/*==================================================================*/
			char *check_noun_sm(char *key)
/*==================================================================*/
{
	SMLIST *slp;

	slp = &(smlist[hash((unsigned char*)key, (int)strlen(key))]);
	if (!slp->key) {
	return NULL;
	}
	while (slp) {
	if (!strcmp(slp->key, key)) {
		char *newsm;

		newsm = art_strdup(slp->sm);

		if (VerboseLevel >= VERBOSE2) {
		fprintf(stderr, ";; Cache hit!: %s [", key);
		codes2sm_print(stderr, newsm);
		fprintf(stderr, "]\n");
		}

		return newsm;
	}
	slp = slp->next;
	}
	return NULL;
}

/*==================================================================*/
		  void sm2feature(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	char *cp, feature_buffer[BNST_LENGTH_MAX + SM_CODE_SIZE * SM_ELEMENT_MAX + 4];

	for (i = 0; i < sp->Tag_num; i++) {
	/* thesaurus.c: get_bnst_code() �ŗ^����ꂽfeature���㏑�� */
	if ((cp = check_feature((sp->tag_data + i)->f, "SM")) != NULL) {
		art_sprintf_3(feature_buffer, BNST_LENGTH_MAX + SM_CODE_SIZE * SM_ELEMENT_MAX + 4, "SM:%*s:%s", strlen(cp) - 3, cp + 3, (sp->tag_data + i)->SM_code);
		assign_cfeature(&((sp->tag_data + i)->f), feature_buffer);
	}
	}
}

/*====================================================================
							   END
====================================================================*/
