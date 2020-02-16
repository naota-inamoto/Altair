/*====================================================================

				  �\�w�i���

											   S.Kurohashi 92.10.21
											   S.Kurohashi 93. 5.31

	$Id: lib_scase.c,v 1.23.2.2 2005/07/29 08:19:05 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

#ifdef	_WINDOWS
static char* yougen = NULL;
static char* yougen_hantei = NULL;
static char* yougen_keiyou = NULL;
static char* yougen_doushi = NULL;
static char* ga_kaku = NULL;
static char* ni_kaku = NULL;
static char* wo_kaku = NULL;
static char* he_kaku = NULL;
static char* to_kaku = NULL;
static char* voice = NULL;
static char* passive = NULL;
static char* shieki = NULL;
static char* morau = NULL;
static char* hoshii = NULL;
static char* shieki_passive = NULL;
static char* wo_ni = NULL;
static char* t_yougem_out = NULL;
static char* t_yougen_in = NULL;
static char yougen_2[] = "�p��";
static char yougen_hantei_2[] = "�p��:��";
static char yougen_keiyou_2[] = "�p��:�`";
static char yougen_doushi_2[] = "�p��:��";
static char ga_kaku_2[] = "�K�i";
static char ni_kaku_2[] = "�j�i";
static char wo_kaku_2[] = "���i";
static char he_kaku_2[] = "�w�i";
static char to_kaku_2[] = "�g�i";
static char voice_2[] = "��";
static char passive_2[] = "��";
static char shieki_2[] = "�g��";
static char morau_2[] = "���炤";
static char hoshii_2[] = "�ق���";
static char shieki_passive_2[] = "�g��&��";
static char wo_ni_2[] = "ID:�i�`���j�`��";
static char t_yougem_out_2[] = "�s�p�����o��";
static char t_yougen_in_2[] = "�s�p�����o��";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	yougen = toStringEUC(yougen_2);
	yougen_hantei = toStringEUC(yougen_hantei_2);
	yougen_keiyou = toStringEUC(yougen_keiyou_2);
	yougen_doushi = toStringEUC(yougen_doushi_2);
	ga_kaku = toStringEUC(ga_kaku_2);
	ni_kaku = toStringEUC(ni_kaku_2);
	wo_kaku = toStringEUC(wo_kaku_2);
	he_kaku = toStringEUC(he_kaku_2);
	to_kaku = toStringEUC(to_kaku_2);
	voice = toStringEUC(voice_2);
	passive = toStringEUC(passive_2);
	shieki = toStringEUC(shieki_2);
	morau = toStringEUC(morau_2);
	hoshii = toStringEUC(hoshii_2);
	shieki_passive = toStringEUC(shieki_passive_2);
	wo_ni = toStringEUC(wo_ni_2);
	t_yougem_out = toStringEUC(t_yougem_out_2);
	t_yougen_in = toStringEUC(t_yougen_in_2);
#endif
}
void final_str_lib_scase() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(yougen); yougen = NULL;
	free(yougen_hantei); yougen_hantei = NULL;
	free(yougen_keiyou); yougen_keiyou = NULL;
	free(yougen_doushi); yougen_doushi = NULL;
	free(ga_kaku); ga_kaku = NULL;
	free(ni_kaku); ni_kaku = NULL;
	free(wo_kaku); wo_kaku = NULL;
	free(he_kaku); he_kaku = NULL;
	free(to_kaku); to_kaku = NULL;
	free(voice); voice = NULL;
	free(passive); passive = NULL;
	free(shieki); shieki = NULL;
	free(morau); morau = NULL;
	free(hoshii); hoshii = NULL;
	free(shieki_passive); shieki_passive = NULL;
	free(wo_ni); wo_ni = NULL;
	free(t_yougem_out); t_yougem_out = NULL;
	free(t_yougen_in); t_yougen_in = NULL;
#endif
}
#ifdef	_LINUX
static char yougen[] = "�p��";
static char yougen_hantei[] = "�p��:��";
static char yougen_keiyou[] = "�p��:�`";
static char yougen_doushi[] = "�p��:��";
static char ga_kaku[] = "�K�i";
static char ni_kaku[] = "�j�i";
static char wo_kaku[] = "���i";
static char he_kaku[] = "�w�i";
static char to_kaku[] = "�g�i";
static char voice[] = "��";
static char passive[] = "��";
static char shieki[] = "�g��";
static char morau[] = "���炤";
static char hoshii[] = "�ق���";
static char shieki_passive[] = "�g��&��";
static char wo_ni[] = "ID:�i�`���j�`��";
static char t_yougem_out[] = "�s�p�����o��";
static char t_yougen_in[] = "�s�p�����o��";
#endif

DBM_FILE	scase_db;
int 	ScaseDicExist;
extern	int 	OptUseScase;

extern void hash_close(HASH_FILE *hashdb);

static int buffer_size;

/*==================================================================*/
			  void init_scase()
/*==================================================================*/
{
	char *filename;

	if (OptUseScase == FALSE) {
	ScaseDicExist = FALSE;
	return;
	}

	if (DICT[SCASE_DB]) {
	filename = check_dict_filename(DICT[SCASE_DB], FALSE);
	}
	else {
	filename = check_dict_filename(SCASE_DB_NAME, FALSE);
	}

	if (OptDisplay == OPT_DEBUG) {
	fprintf(Outfp, "Opening %s ... ", filename);
	}

	if ((scase_db = DB_open(filename, O_RDONLY, 0)) == NULL) {
	if (OptDisplay == OPT_DEBUG) {
		fputs("failed.\n", Outfp);
	}
	ScaseDicExist = FALSE;
#ifdef KNP_DEBUG
	fprintf(stderr, ";; Cannot open SCASE dictionary <%s>.\n", filename);
#endif
	} else {
	if (OptDisplay == OPT_DEBUG) {
		fputs("done.\n", Outfp);
	}
	ScaseDicExist = TRUE;
	}
	free(filename);
}

/*==================================================================*/
					void close_scase()
/*==================================================================*/
{
	if (ScaseDicExist == 1 /*TRUE*/)
	  DB_close(scase_db);
}

/*==================================================================*/
					char *get_scase(char *cp)
/*==================================================================*/
{
	int i;
	char *value;

	if (ScaseDicExist == FALSE)
	return NULL;

	value = db_get(scase_db, cp);
	
	if (value) {
	for (i = 0; *(value+i) != '\0'; i++)
	  *(value+i) -= '0';
	return value;
	}
	else {
	return NULL;
	}
}

/*==================================================================*/
		 void get_scase_code(BNST_DATA *ptr)
/*==================================================================*/
{
	int i;
	char *cp, *ans, *anscp, *str_buffer, *vtype, voice[3];

	init_str();

	/* ������: init_bnst �ł����Ă��� */
	for (i = 0, cp = ptr->SCASE_code; i < SCASE_CODE_SIZE; i++, cp++) *cp = 0;

	if (ScaseDicExist == 1 /*TRUE*/ &&	
	(vtype = check_feature(ptr->f, yougen)) != NULL &&	
	strcmp(vtype, yougen_hantei) != 0) { /* ���莌�ł͂Ȃ��ꍇ */
	vtype += 5;

	voice[0] = '\0';
	if (ptr->voice & VOICE_UKEMI) {
		art_strcpy(voice, 3, ":P");
	}
	else if (ptr->voice & VOICE_SHIEKI) {
		art_strcpy(voice, 3, ":C");
	}
	else if (ptr->voice & VOICE_SHIEKI_UKEMI) {
		art_strcpy(voice, 3, ":PC");
	}

	str_buffer = make_pred_string((TAG_DATA *)ptr);
	art_strcat(str_buffer, buffer_size, ":");
	art_strcat(str_buffer, buffer_size, vtype);
	if (voice[0]) art_strcat(str_buffer, buffer_size, voice);

	ans = get_scase(str_buffer);

	if (ans != NULL) {
		/* DEBUG �\�� */
		if (OptDisplay == OPT_DEBUG) {
		char *print_buffer;
		int n;

		print_buffer = (char *)malloc_data(n = strlen(str_buffer) + 10, "get_scase_code");
		art_sprintf_1(print_buffer, n, "SCASEUSE:%s", str_buffer);
		assign_cfeature(&(ptr->f), print_buffer);
		free(print_buffer);
		}

		cp = ptr->SCASE_code;
		anscp = ans;
		for (i = 0; i < SCASE_CODE_SIZE; i++) *cp++ = *anscp++;
		free(ans);
		free(str_buffer);
		goto Match;
	}
	else {
		free(str_buffer);
	}
	}

	/* ���莌�Ȃǂ̏ꍇ,
	   �\�w�i�������Ȃ��ꍇ,	
	   �܂��͎����ɂȂ��p���̏ꍇ */
	
	if (check_feature(ptr->f, yougen_hantei)) {
	ptr->SCASE_code[case2num(ga_kaku)] = 1;
	}	
	else if (check_feature(ptr->f, yougen_keiyou)) {
	ptr->SCASE_code[case2num(ga_kaku)] = 1;
	ptr->SCASE_code[case2num(ni_kaku)] = 1;
	/* �`�e���̕\�w�i�̕t�^�͕���p�������̂Ő���
	ptr->SCASE_code[case2num("�����i")] = 1;
	ptr->SCASE_code[case2num("�g�i")] = 1;
	*/
	}	
	else if (check_feature(ptr->f, yougen_doushi)) {
	ptr->SCASE_code[case2num(ga_kaku)] = 1;
	ptr->SCASE_code[case2num(wo_kaku)] = 1;
	ptr->SCASE_code[case2num(ni_kaku)] = 1;
	ptr->SCASE_code[case2num(he_kaku)] = 1;
	ptr->SCASE_code[case2num(to_kaku)] = 1;
	}

  Match:

	/* ���H�C�X�ɂ��C�� */

	if (ptr->voice & VOICE_SHIEKI) {
	ptr->SCASE_code[case2num(wo_kaku)] = 1;
	ptr->SCASE_code[case2num(ni_kaku)] = 1;
	}
	else if (ptr->voice & VOICE_UKEMI ||	
		 ptr->voice & VOICE_SHIEKI_UKEMI) {
	ptr->SCASE_code[case2num(ni_kaku)] = 1;
	}
	else if (ptr->voice & VOICE_MORAU ||	
		 ptr->voice & VOICE_HOSHII) {
	ptr->SCASE_code[case2num(wo_kaku)] = 1;
	ptr->SCASE_code[case2num(ni_kaku)] = 1;
	}
}

/*==================================================================*/
		 void set_pred_voice(BNST_DATA *ptr)
/*==================================================================*/
{
	/* ���H�C�X�̐ݒ� */

	char *cp;

	init_str();

	ptr->voice = 0;

	if ((cp = check_feature(ptr->f, voice)) != NULL) {
	char *token, *str;

	str = art_strdup(cp + 3);
	token = art_strtok(str, "|");
	while (token) {
		if (!strcmp(token, passive)) {
		ptr->voice |= VOICE_UKEMI;
		}
		else if (!strcmp(token, shieki)) {
		ptr->voice |= VOICE_SHIEKI;
		}
		else if (!strcmp(token, morau)) {
		ptr->voice |= VOICE_MORAU;
		}
		else if (!strcmp(token, hoshii)) {
		ptr->voice |= VOICE_HOSHII;
		}
		else if (!strcmp(token, shieki_passive)) {
		ptr->voice |= VOICE_SHIEKI_UKEMI;
		}
		/* �u�\�v�͖����� */

		token = art_strtok(NULL, "|");
	}
	free(str);
	}
}

/*==================================================================*/
		   char *make_pred_string(TAG_DATA *t_ptr)
/*==================================================================*/
{
	char *buffer;

	init_str();

	buffer_size = 0;

	/* �p���^�C�v, voice�̕�(7)���m�ۂ��Ă��� */

	/* �u�i�`���j�`�Ɂv �̂Ƃ��� �u����v �ŒT�� */
	if (check_feature(t_ptr->f, wo_ni)) {
	buffer = (char *)malloc_data(buffer_size = 12, "make_pred_string"); /* 4 + 8 */
	art_strcpy(buffer, buffer_size, "����");
	}
	/* �u�`�e��+�Ȃ�v�Ȃ� */
	else if (check_feature(t_ptr->f, t_yougem_out)) {
	buffer = (char *)malloc_data(buffer_size = strlen(t_ptr->head_ptr->Goi2) + strlen((t_ptr->head_ptr + 1)->Goi) + 8,	
					 "make_pred_string");
	art_strcpy(buffer, buffer_size, t_ptr->head_ptr->Goi2);
	art_strcat(buffer, buffer_size, (t_ptr->head_ptr + 1)->Goi);
	}
	/* �u�`�e���ꊲ+�I���v�Ȃ� */
	else if (check_feature(t_ptr->f, t_yougen_in)) {
	buffer = (char *)malloc_data(buffer_size = strlen((t_ptr->head_ptr - 1)->Goi2) + strlen(t_ptr->head_ptr->Goi) + 8,	
					 "make_pred_string");
	art_strcpy(buffer, buffer_size, (t_ptr->head_ptr - 1)->Goi2);
	art_strcat(buffer, buffer_size, t_ptr->head_ptr->Goi);
	}
	else {
	buffer = (char *)malloc_data(buffer_size = strlen(t_ptr->head_ptr->Goi) + 8, "make_pred_string");
	art_strcpy(buffer, buffer_size, t_ptr->head_ptr->Goi);
	}

	return buffer;
}

/*====================================================================
							   END
====================================================================*/
