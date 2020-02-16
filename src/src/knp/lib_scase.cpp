/*====================================================================

				  表層格情報

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
static char yougen_2[] = "用言";
static char yougen_hantei_2[] = "用言:判";
static char yougen_keiyou_2[] = "用言:形";
static char yougen_doushi_2[] = "用言:動";
static char ga_kaku_2[] = "ガ格";
static char ni_kaku_2[] = "ニ格";
static char wo_kaku_2[] = "ヲ格";
static char he_kaku_2[] = "ヘ格";
static char to_kaku_2[] = "ト格";
static char voice_2[] = "態";
static char passive_2[] = "受動";
static char shieki_2[] = "使役";
static char morau_2[] = "もらう";
static char hoshii_2[] = "ほしい";
static char shieki_passive_2[] = "使役&受動";
static char wo_ni_2[] = "ID:（～を）～に";
static char t_yougem_out_2[] = "Ｔ用言見出→";
static char t_yougen_in_2[] = "Ｔ用言見出←";
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
static char yougen[] = "用言";
static char yougen_hantei[] = "用言:判";
static char yougen_keiyou[] = "用言:形";
static char yougen_doushi[] = "用言:動";
static char ga_kaku[] = "ガ格";
static char ni_kaku[] = "ニ格";
static char wo_kaku[] = "ヲ格";
static char he_kaku[] = "ヘ格";
static char to_kaku[] = "ト格";
static char voice[] = "態";
static char passive[] = "受動";
static char shieki[] = "使役";
static char morau[] = "もらう";
static char hoshii[] = "ほしい";
static char shieki_passive[] = "使役&受動";
static char wo_ni[] = "ID:（～を）～に";
static char t_yougem_out[] = "Ｔ用言見出→";
static char t_yougen_in[] = "Ｔ用言見出←";
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

	/* 初期化: init_bnst でもしている */
	for (i = 0, cp = ptr->SCASE_code; i < SCASE_CODE_SIZE; i++, cp++) *cp = 0;

	if (ScaseDicExist == 1 /*TRUE*/ &&	
	(vtype = check_feature(ptr->f, yougen)) != NULL &&	
	strcmp(vtype, yougen_hantei) != 0) { /* 判定詞ではない場合 */
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
		/* DEBUG 表示 */
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

	/* 判定詞などの場合,
	   表層格辞書がない場合,	
	   または辞書にない用言の場合 */
	
	if (check_feature(ptr->f, yougen_hantei)) {
	ptr->SCASE_code[case2num(ga_kaku)] = 1;
	}	
	else if (check_feature(ptr->f, yougen_keiyou)) {
	ptr->SCASE_code[case2num(ga_kaku)] = 1;
	ptr->SCASE_code[case2num(ni_kaku)] = 1;
	/* 形容詞の表層格の付与は副作用が多いので制限
	ptr->SCASE_code[case2num("ヨリ格")] = 1;
	ptr->SCASE_code[case2num("ト格")] = 1;
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

	/* ヴォイスによる修正 */

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
	/* ヴォイスの設定 */

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
		/* 「可能」は未扱い */

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

	/* 用言タイプ, voiceの分(7)も確保しておく */

	/* 「（～を）～に」 のときは 「する」 で探す */
	if (check_feature(t_ptr->f, wo_ni)) {
	buffer = (char *)malloc_data(buffer_size = 12, "make_pred_string"); /* 4 + 8 */
	art_strcpy(buffer, buffer_size, "する");
	}
	/* 「形容詞+なる」など */
	else if (check_feature(t_ptr->f, t_yougem_out)) {
	buffer = (char *)malloc_data(buffer_size = strlen(t_ptr->head_ptr->Goi2) + strlen((t_ptr->head_ptr + 1)->Goi) + 8,	
					 "make_pred_string");
	art_strcpy(buffer, buffer_size, t_ptr->head_ptr->Goi2);
	art_strcat(buffer, buffer_size, (t_ptr->head_ptr + 1)->Goi);
	}
	/* 「形容詞語幹+的だ」など */
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
