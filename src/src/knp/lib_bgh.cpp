/*====================================================================

			  ���ތ�b�\  �����v���O����

											   S.Kurohashi 91. 6.25
											   S.Kurohashi 93. 5.31

	$Id: lib_bgh.c,v 1.24 2005/04/30 06:57:22 kuro Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

DBM_FILE	bgh_db;
int 	BGHExist;

extern void hash_close(HASH_FILE *hashdb);

/*==================================================================*/
			   void init_bgh()
/*==================================================================*/
{
	char *filename;

	if (DICT[BGH_DB]) {
	filename = check_dict_filename(DICT[BGH_DB], TRUE);
	}
	else {
	filename = check_dict_filename(BGH_DB_NAME, FALSE);
	}

	if (OptDisplay == OPT_DEBUG) {
	fprintf(Outfp, "Opening %s ... ", filename);
	}

	if ((bgh_db = DB_open(filename, O_RDONLY, 0)) == NULL) {
	if (OptDisplay == OPT_DEBUG) {
		fputs("failed.\n", Outfp);
	}
	BGHExist = FALSE;
#ifdef KNP_DEBUG
	fprintf(stderr, ";; Cannot open BGH dictionary <%s>.\n", filename);
#endif
	} else {
	if (OptDisplay == OPT_DEBUG) {
		fputs("done.\n", Outfp);
	}
	BGHExist = TRUE;
	}
	free(filename);
	THESAURUS[USE_BGH].exist = BGHExist;

	/* �Ӗ��f => �Ӗ��f�R�[�h */
	if (Thesaurus == USE_BGH) {
	if (DICT[SM2CODE_DB]) {
		filename = check_dict_filename(DICT[SM2CODE_DB], TRUE);
	}
	else {
		filename = check_dict_filename(SM2BGHCODE_DB_NAME, FALSE);
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
		fprintf(stderr, ";; Cannot open BGH sm dictionary <%s>.\n", filename);
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
}

/*==================================================================*/
					void close_bgh()
/*==================================================================*/
{
	if (BGHExist == 1 /*TRUE*/)
	DB_close(bgh_db);
}

/*==================================================================*/
		 char *_get_bgh(char *cp, char * /*arg*/)
/*==================================================================*/
{
	return db_get(bgh_db, cp);
}

/*==================================================================*/
		int bgh_code_match(char *c1, char *c2)
/*==================================================================*/
{
	int i, point = 0;

	/* 1���ڈ�v -> 1,2,3,4,5,6-7,8-10���ڔ�r

	   1���ڕs��v -> 1���ڂ�4(���̑�)�ȊO�Ȃ� 2�`4���ڔ�r 
				  2���ڈȍ~��v�̏ꍇ 1���ڂ͈�v�Ƃ݂Ȃ� */

	if (c1[0] == c2[0]) {
	point = 1;
	for (i = 1; c1[i] == c2[i] && i < BGH_CODE_SIZE; i++)
		if (i != 5 && i != 7 && i != 8 && i != 9)
		point ++;
	}
	else if (c1[0] != '4' && c2[0] != '4' && c1[1] == c2[1]) {
	point = 2;
	for (i = 2; c1[i] == c2[i] && i < 4; i++)	
		point ++;
	}			

	return point;
}

/*==================================================================*/
	  int bgh_code_match_for_case(char *cp1, char *cp2)
/*==================================================================*/
{
	/* ��̕��ތ�b�\�R�[�h�̃}�b�`���O�x�̌v�Z */

	int match = 0;

	/* �P�ʂ̍��ڂ͖��� */
	if (!strncmp(cp1, "11960", 5) || !strncmp(cp2, "11960", 5))
	return 0;
	
	/* ��r */
	match = bgh_code_match(cp1, cp2);

	/* �㖼���̍��ڂ͗ގ��x���������� */
	if ((!strncmp(cp1, "12000", 5) || !strncmp(cp2, "12000", 5)) &&
	match > 3)
	return 3;
	
	return match;
}

/*==================================================================*/
		  int comp_bgh(char *cpp, char *cpd)
/*==================================================================*/
{
	int i;

	if (cpp[0] == cpd[0]) {
	for (i = 1; i < BGH_CODE_SIZE; i++) {
		if (cpp[i] == '*') {
		return i;
		}
		else if (cpp[i] != cpd[i]) {
		return 0;
		}
	}
	}
	else if (cpp[0] != '4' && cpd[0] != '4' && cpp[1] == cpd[1]) {
	for (i = 2; i < 4; i++) {
		if (cpp[i] == '*') {
		return i;
		}
		else if (cpp[i] != cpd[i]) {
		return 0;
		}
	}
	}			
	return BGH_CODE_SIZE;
}

/*==================================================================*/
		 int bgh_match_check(char *pat, char *codes)
/*==================================================================*/
{
	int i;

	if (codes == NULL) {
	return FALSE;
	}

	for (i = 0; *(codes+i); i += BGH_CODE_SIZE) {
	if (comp_bgh(pat, codes+i) > 0) {
		return TRUE;
	}
	}
	return FALSE;
}

/*====================================================================
							   END
====================================================================*/
