/*====================================================================

				 並列構造解析

											   S.Kurohashi 91. 6.25
											   S.Kurohashi 93. 5.31

	$Id: para_analysis.c,v 1.16.2.1 2005/08/21 11:03:19 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

#define PENALTY 	1 /* 2 */
#define BONUS		2 /* 前は正規化の前に6 */
#define MINUS		7
#define PENA_MAX	1000
#define ENOUGH_MINUS	-100.0
#define START_HERE	-1

#define STRONG_PARA_TH	3.9

int 	score_matrix[BNST_MAX][BNST_MAX];
int 	prepos_matrix[BNST_MAX][BNST_MAX];
int 	maxpos_array[BNST_MAX];
int 	maxsco_array[BNST_MAX];
int 	penalty_table[BNST_MAX];

float	norm[] = {
	1.00f,	1.00f,	1.59f,	2.08f,	2.52f,	2.92f,	3.30f,	3.66f,	4.00f,	4.33f,
	4.64f,	4.95f,	5.24f,	5.53f,	5.81f,	6.08f,	6.35f,	6.61f,	6.87f,	7.12f,
	7.37f,	7.61f,	7.85f,	8.09f,	8.32f,	8.55f,	8.78f,	9.00f,	9.22f,	9.44f,
	9.65f,	9.87f, 10.08f, 10.29f, 10.50f, 10.70f, 10.90f, 11.10f, 11.30f, 11.50f,
   11.70f, 11.89f, 12.08f, 12.27f, 12.46f, 12.65f, 12.84f, 13.02f, 13.21f, 13.39f};

extern QUOTE_DATA quote_data;

#ifdef	_WINDOWS
static char* yougen = NULL;
static char* kakari = NULL;
static char* kakari_bunmatsu = NULL;
static char* kakari_to_kaku = NULL;
static char* keidai = NULL;
static char* mei_hei_to = NULL;
static char* mei_hei = NULL;
static char* jutsu_hei = NULL;
static char* hei_ki = NULL;
static char* shijishi = NULL;
static char* kugiri = NULL;
static char* dokuten = NULL;
static char yougen_2[] = "用言";
static char kakari_2[] = "係";
static char kakari_bunmatsu_2[] = "係:文末";
static char kakari_to_kaku_2[] = "係:ト格";
static char keidai_2[] = "提題";
static char mei_hei_to_2[] = "名並終点～と～と";
static char mei_hei_2[] = "名並終点";
static char jutsu_hei_2[] = "述並終点";
static char hei_ki_2[] = "並キ";
static char shijishi_2[] = "指示詞";
static char kugiri_2[] = "区切";
static char dokuten_2[] = "読点";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	yougen = toStringEUC(yougen_2);
	kakari = toStringEUC(kakari_2);
	kakari_bunmatsu = toStringEUC(kakari_bunmatsu_2);
	kakari_to_kaku = toStringEUC(kakari_to_kaku_2);
	keidai = toStringEUC(keidai_2);
	mei_hei_to = toStringEUC(mei_hei_to_2);
	mei_hei = toStringEUC(mei_hei_2);
	jutsu_hei = toStringEUC(jutsu_hei_2);
	hei_ki = toStringEUC(hei_ki_2);
	shijishi = toStringEUC(shijishi_2);
	kugiri = toStringEUC(kugiri_2);
	dokuten = toStringEUC(dokuten_2);
#endif
}
void final_str_para_analysis() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(yougen); yougen = NULL;
	free(kakari); kakari = NULL;
	free(kakari_bunmatsu); kakari_bunmatsu = NULL;
	free(kakari_to_kaku); kakari_to_kaku = NULL;
	free(keidai); keidai = NULL;
	free(mei_hei_to); mei_hei_to = NULL;
	free(mei_hei); mei_hei = NULL;
	free(jutsu_hei); jutsu_hei = NULL;
	free(hei_ki); hei_ki = NULL;
	free(shijishi); shijishi = NULL;
	free(kugiri); kugiri = NULL;
	free(dokuten); dokuten = NULL;
#endif
}
#ifdef	_LINUX
static char yougen[] = "用言";
static char kakari[] = "係";
static char kakari_bunmatsu[] = "係:文末";
static char kakari_to_kaku[] = "係:ト格";
static char keidai[] = "提題";
static char mei_hei_to[] = "名並終点～と～と";
static char mei_hei[] = "名並終点";
static char jutsu_hei[] = "述並終点";
static char hei_ki[] = "並キ";
static char shijishi[] = "指示詞";
static char kugiri[] = "区切";
static char dokuten[] = "読点";
#endif

/*==================================================================*/
	void mask_quote_scope(SENTENCE_DATA *sp, int key_pos)
/*==================================================================*/
{
	int i, j, k, l;

	init_str();

	/* 括弧がある場合に並列構造の範囲に制限を設ける */

	for (k = 0; quote_data.in_num[k] >= 0; k++) {

	int start = quote_data.in_num[k];
	int end = quote_data.out_num[k];

	/* 後に括弧がある場合 */

	if (key_pos < start) {
		for (i = 0; i < start; i++)
		for (j = start; j < end; j++)
			restrict_matrix[i][j] = 0;
	}	

	/* 前に括弧がある場合 (キーが括弧内の末尾の場合も) */

	else if (end <= key_pos) {
		for (i = start + 1; i <= end; i++)
		for (j = end + 1; j < sp->Bnst_num; j++)
			restrict_matrix[i][j] = 0;
	}

	/* キーが括弧の中にある場合 */

	else {
		for (i = 0; i <= end; i++)
		for (j = start; j < sp->Bnst_num; j++)
			if (i < start || end < j)
			restrict_matrix[i][j] = 0;

		/* 括弧の中に句点がある場合 */
		for (l = start; l < end; l++)
		if (check_feature(sp->bnst_data[l].f, kakari_bunmatsu))
			for (i = start; i <= l; i++)
			for (j = l + 1; j <= end; j++)
				restrict_matrix[i][j] = 0;
	}
	}

	if (k && OptDisplay == OPT_DEBUG)
	print_matrix(sp, PRINT_RSTQ, key_pos);
}

/*==================================================================*/
	int bnst_match(SENTENCE_DATA *sp, int pos1, int pos2)
/*==================================================================*/
{
	int flag1, flag2;
	char *cp1, *cp2;
	BNST_DATA *ptr1 = &(sp->bnst_data[pos1]);
	BNST_DATA *ptr2 = &(sp->bnst_data[pos2]);

	init_str();

	/*
	  パスのスコア計算において区切りペナルティをcancelする条件
		・係が同じ
	・用言であるかどうかが同じ
	・読点があるかないかが同じ
	
	   ※ 条件は少し緩くしてある．問題があれば強める	
	*/

	cp1 = (char *)check_feature(ptr1->f, kakari);
	cp2 = (char *)check_feature(ptr2->f, kakari);
	if (strcmp(cp1, cp2)) return 0;
	
	flag1 = check_feature(ptr1->f, yougen) ? 1 : 0;
	flag2 = check_feature(ptr2->f, yougen) ? 1 : 0;
	if (flag1 != flag2) return 0;

	if (check_feature(ptr1->f, yougen)) {
	cp1 = (char *)check_feature(ptr1->f, "ID");
	cp2 = (char *)check_feature(ptr2->f, "ID");
	if (!cp1 || !cp2 || strcmp(cp1, cp2)) return 0;
	}
	
	flag1 = check_feature(ptr1->f, dokuten) ? 1 : 0;
	flag2 = check_feature(ptr2->f, dokuten) ? 1 : 0;
	if (flag1 != flag2) return 0;

	return 1;
}

/*==================================================================*/
int calc_static_level_penalty(SENTENCE_DATA *sp, int key_pos, int pos)
/*==================================================================*/
{
	int minus_score = 0;
	int level1 = sp->bnst_data[key_pos].sp_level;
	int level2 = sp->bnst_data[pos].sp_level;

	if (level1 <= level2)
	minus_score = MINUS * (level2 - level1 + 1);

	return minus_score;
}
/*==================================================================*/
int calc_dynamic_level_penalty(SENTENCE_DATA *sp, int key_pos, int pos1, int pos2)
/*==================================================================*/
{
	init_str();

	if (sp->bnst_data[pos1].sp_level == sp->bnst_data[pos2].sp_level &&
	bnst_match(sp, pos1, pos2) &&
	!bnst_match(sp, pos1, key_pos))
	return 0;
	else if (check_feature(sp->bnst_data[pos1].f, keidai) &&
		 check_feature(sp->bnst_data[pos2].f, keidai))
	return 0;
			/* 「～は」の場合は読点の有無,レベルを無視 */
	else
	return(penalty_table[pos1] + penalty_table[pos2]);
}

/*==================================================================*/
   int plus_bonus_score(SENTENCE_DATA *sp, int jend_pos, PARA_DATA *p_ptr)
/*==================================================================*/
{
	BNST_DATA *b_ptr;

	init_str();

	b_ptr = &sp->bnst_data[jend_pos];

	if (p_ptr->type == PARA_KEY_I) {	
	return 0;
	}	
	else if (p_ptr->type == PARA_KEY_N) {
		if (check_feature(sp->bnst_data[p_ptr->key_pos].f, kakari_to_kaku) &&
		check_feature(b_ptr->f, mei_hei_to))	
		return BONUS;
	if (check_feature(b_ptr->f, mei_hei))
		return BONUS;
	else return 0;
	}
	else if (p_ptr->type == PARA_KEY_P) {
	if (check_feature(b_ptr->f, jutsu_hei))
		return BONUS;
	else return 0;
	}
	else {
	return 0;
	}
}

/*==================================================================*/
void dp_search_scope(SENTENCE_DATA *sp, int key_pos, int iend_pos, int jend_pos)
/*==================================================================*/
{
	int i, j, current_max, score_upward, score_sideway;
	
	/* ＤＰマッチング */

	for (j = jend_pos; j > key_pos; j--)  {

	/* 最右列の処理 */
	
	if (j == jend_pos) {
		score_matrix[iend_pos][jend_pos] = match_matrix[iend_pos][jend_pos];
		prepos_matrix[iend_pos][jend_pos] = START_HERE;
		for (i=iend_pos-1; i>=0; i--)
		  score_matrix[i][jend_pos] = - PENA_MAX;
	}
	
	else {

		/* 最下行の処理 */

		score_sideway = score_matrix[iend_pos][j+1] 
					- PENALTY - penalty_table[j];
		score_matrix[iend_pos][j] = score_sideway;
		prepos_matrix[iend_pos][j] = iend_pos;

		/* 他の行の処理:下からと左からのスコアを比較 */

		for (i=iend_pos-1; i>=0; i--) {
		score_upward = match_matrix[i][j] + maxsco_array[i+1]
			- calc_dynamic_level_penalty(sp, key_pos, i, j);
		score_sideway = score_matrix[i][j+1]	
			- PENALTY - penalty_table[j];
		
		if (score_upward >= score_sideway) {
			score_matrix[i][j] = score_upward;
			prepos_matrix[i][j] = maxpos_array[i+1];
		} else {
			score_matrix[i][j] = score_sideway;
			prepos_matrix[i][j] = i;
		}
		}
	}

	/* 次の列のために最大値，最大位置を計算 */

	current_max = score_matrix[iend_pos][j];
	maxpos_array[iend_pos] = iend_pos;
	maxsco_array[iend_pos] = score_matrix[iend_pos][j];

	for (i=iend_pos-1; i>=0; i--) {

		current_max -= (PENALTY + penalty_table[i]);
		if (current_max <= score_matrix[i][j]) {
		current_max = score_matrix[i][j];
		maxpos_array[i] = i;
		maxsco_array[i] = current_max;
		} else {
		maxpos_array[i] = maxpos_array[i+1];
		maxsco_array[i] = current_max;
		}
	}
	}
}

/*==================================================================*/
void _detect_para_scope(SENTENCE_DATA *sp, PARA_DATA *ptr, int jend_pos)
/*==================================================================*/
{
	int i, j, flag, nth;
	int key_pos = ptr->key_pos;
	int iend_pos = ptr->iend_pos;
	int ending_bonus_score;
	int max_pos = -1;
	float current_score, sim_threshold, new_threshold,
	max_score = ENOUGH_MINUS, pure_score = 0;
	char *cp;
	FEATURE *fp;

	init_str();

	/*								*/
	/* スタート位置(jend_pos)からの解析を本当に行うかどうか */
	/*								*/

	/* 類似度が0なら中止 */

	if (match_matrix[iend_pos][jend_pos] == 0) return;

	/* restrict_matrixで可能性がない場合は中止 */

	flag = FALSE;
	for (i = 0; i <= iend_pos; i++) {
	if (restrict_matrix[i][jend_pos]) {
		flag = TRUE; break;
	}
	}
	if (flag == FALSE) return;

	/* 「～，それを」という並列は中止 */

	if (key_pos + 1 == jend_pos &&	
	check_feature(sp->bnst_data[jend_pos].f, shijishi))
	return;

	/* ルールによる制限(類似スコアの閾値を取得) */

	/* 条件がなければ閾値は0.0に */
	if ((ptr->f_pattern).fp[0] == NULL) {
	sim_threshold = 0.0;
	}	
	/* 条件があれば，マッチするものの中で最低の閾値に */
	else {
	sim_threshold = 100.0;
	nth = 0;
	while ((fp = (ptr->f_pattern).fp[nth]) != NULL) {
		if (feature_AND_match(fp, sp->bnst_data[jend_pos].f,
				  sp->bnst_data + key_pos,
				  sp->bnst_data + jend_pos) == 1 /*TRUE*/) {
		if ((cp = (char *)check_feature(fp, "&ST")) != NULL) {
			art_sscanf_1(cp, "&ST:%f", &new_threshold);
		} else {
			new_threshold = 0.0;
		}
		if (new_threshold < sim_threshold )
			sim_threshold = new_threshold;
		}
		nth++;
	}
	if (sim_threshold == 100.0) return;
	}

	/* if (feature_pattern_match(&(ptr->f_pattern), 
	   sp->bnst_data[jend_pos].f,
	   sp->bnst_data + key_pos,
	   sp->bnst_data + jend_pos) == FALSE) */

	/*			*/
	/* DP MATCHING	*/
	/*			*/

	dp_search_scope(sp, key_pos, iend_pos, jend_pos);


	/* 最大パスの検出 */

	ending_bonus_score = plus_bonus_score(sp, jend_pos, ptr);
	for (i = iend_pos; i >= 0; i--) {
	current_score = 
	  (float)score_matrix[i][key_pos+1] / norm[jend_pos - i + 1]
	  + ending_bonus_score;

	if (restrict_matrix[i][jend_pos] && 
		max_score < current_score) {
		max_score = current_score;
		pure_score =	
		  (float)score_matrix[i][key_pos+1] / norm[jend_pos - i + 1];
		/* pure_score は末尾表現のボーナスを除いた値 */
		max_pos = i;
	}
	}


	/* 類似度が0なら中止 01/07/12 */
	if (max_score < 0.0) return;


	/* ▼ (a...)(b)という並列は扱えない．括弧の制限などでこうならざる
	   をえない場合は，並列とは認めないことにする (暫定的) */

	/* 「～はもちろん」の扱いで話が変ってっきた？？？
	if (key_pos + 1 == jend_pos && max_pos != key_pos) {
	max_pos = i;
	max_score = -100;
	return;
	}
	*/

	/*
	  閾値を越えて，まだstatusが x なら n に
	  閾値を越えて，statusが n なら スコア比較
	  閾値を越えなくても，参考のためスコアを記憶
	*/
	flag = FALSE;
	if (sim_threshold <= pure_score &&
	ptr->status == 'x') {
	ptr->status = 'n';
	flag = TRUE;
	}
	else if (sim_threshold <= pure_score &&
		 ptr->status == 'n' &&
		 ptr->max_score < max_score) {
	flag = TRUE;
	}
	else if (ptr->status == 'x' &&
		 ptr->max_score < max_score) {
	flag = TRUE;
	}	

	if (flag == 1 /*TRUE*/) {
	ptr->max_score = max_score;
	ptr->pure_score = pure_score;
	ptr->max_path[0] = max_pos;
	for (j = 0;; j++) {
		ptr->max_path[j+1] = prepos_matrix[ptr->max_path[j]][j+key_pos+1];
		if (ptr->max_path[j+1] == START_HERE) {
		ptr->jend_pos = j + key_pos + 1;
		break;
		}
	}
	}
}

/*==================================================================*/
int detect_para_scope(SENTENCE_DATA *sp, int para_num, int restrict_p)
/*==================================================================*/
{
	int i, j, k;
	PARA_DATA *para_ptr = &(sp->para_data[para_num]);
	int key_pos = para_ptr->key_pos;

	/*	
	   restrict_p
		 TRUE : 前の並列解析の失敗によって特定のキーだけを処理する場合
	 FALSE : はじめにすべてのキーを処理する場合
		
	   restrict_matrix
		 括弧による制限と前の並列構造解析による制限(restrict_pの場合)
	 (restrict_p==FALSEの場合ここで初期化)
	*/

	para_ptr->status = 'x';
	para_ptr->max_score = ENOUGH_MINUS;
	para_ptr->pure_score = ENOUGH_MINUS;
	para_ptr->manager_ptr = NULL;

	if (restrict_p == FALSE)
	for (i = 0; i < sp->Bnst_num; i++)
		for (j = i + 1; j < sp->Bnst_num; j++)
		restrict_matrix[i][j] = 1;

	mask_quote_scope(sp, key_pos);

	for (k = 0; k < sp->Bnst_num; k++) {
	penalty_table[k] = (k == key_pos) ? 
	  0 : calc_static_level_penalty(sp, key_pos, k);
	}

	for (j = key_pos+1; j < sp->Bnst_num; j++)
	_detect_para_scope(sp, para_ptr, j);

	if (para_ptr->status == 'x') {
	;
	/*
	fprintf(Outfp, ";; Cannot find proper CS for the key <");
	print_bnst(sp->bnst_data + ptr->key_pos, NULL);
	fprintf(Outfp, ">.\n");
	*/
	} else if (para_ptr->status == 'n' &&
		   para_ptr->pure_score >= STRONG_PARA_TH) {
	para_ptr->status = 's';
	}
	
	return TRUE;	/* 解析結果statusがxでも,一応TRUEを返す */
}

/*==================================================================*/
		void detect_all_para_scope(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	for (i = 0; i < sp->Para_num; i++)	
	detect_para_scope(sp, i, FALSE);
}

/*==================================================================*/
		int check_para_key(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	char *cp, type[16], condition[256];

	init_str();

	for (i = 0; i < sp->Bnst_num; i++) {

	if ((cp = (char *)check_feature(sp->bnst_data[i].f, hei_ki)) != NULL) {

		sp->bnst_data[i].para_num = sp->Para_num;
		sp->para_data[sp->Para_num].para_char = (char)('a'+ sp->Para_num);
		sp->para_data[sp->Para_num].key_pos = i;

		type[0] = '\0';
		condition[0] = '\0';
#if _WINDOWS && _MFC_VER >= 0x0800
		art_sscanf_4(cp, "%*[^:]:%[^:]:%s", type, 16, condition, 256);
#else
		art_sscanf_2(cp, "%*[^:]:%[^:]:%s", type, condition);
#endif
		if (!strncmp(type, "名", 2)) {
		sp->bnst_data[i].para_key_type = PARA_KEY_N ;
		} else if (!strncmp(type, "述", 2)) {
		sp->bnst_data[i].para_key_type = PARA_KEY_P;
		} else if (!strncmp(type, "？", 2)) {
		sp->bnst_data[i].para_key_type = PARA_KEY_A;
		}
		sp->para_data[sp->Para_num].type = sp->bnst_data[i].para_key_type;
		
		/* 「～はもちろん」などの場合の"並キ:名-1:...."の処理 */
		if (*(type+2)) {
		sp->para_data[sp->Para_num].iend_pos = i + atoi(type+2);
		} else {
		sp->para_data[sp->Para_num].iend_pos = i;
		}

		string2feature_pattern(&(sp->para_data[sp->Para_num].f_pattern),condition);
		
		sp->Para_num ++;
		if (sp->Para_num >= PARA_MAX) {
		fprintf(stderr, ";; Too many para (%s)!\n", sp->Comment ? sp->Comment : "");
		return CONTINUE;
		}
	}
	else {
		sp->bnst_data[i].para_num = -1;
	}
	}

	if (sp->Para_num == 0) return 0;

	for (i = 0; i < sp->Bnst_num; i++) {

	if ((cp = (char *)check_feature(sp->bnst_data[i].f, kugiri)) != NULL) {
		if (check_feature(sp->bnst_data[i].f, dokuten)) {
		art_sscanf_1(cp, "%*[^:]:%*d-%d", &(sp->bnst_data[i].sp_level));
		} else {
		art_sscanf_1(cp, "%*[^:]:%d-%*d", &(sp->bnst_data[i].sp_level));
		}
	} else {
		sp->bnst_data[i].sp_level = 0;
	}
	}

	return sp->Para_num;
}


/*==================================================================*/
	   int farthest_child(SENTENCE_DATA *sp, BNST_DATA *b_ptr)
/*==================================================================*/
{
	/* 一番遠い子供の文節番号を返す
	   (今のところこの関数は使っていない) */

	int i;
	BNST_DATA	*loop_ptr = b_ptr;
	
	while (loop_ptr->child[0]) {
	for (i = 0; loop_ptr->child[i]; i++);
	loop_ptr = loop_ptr->child[i-1];
	}
	
	return (int)(loop_ptr - sp->bnst_data);
}

/*==================================================================*/
		 int para_recovery(SENTENCE_DATA *sp)
/*==================================================================*/
{
	/* 並列構造の情報の再現 */

	int 	i, j;
	BNST_DATA	*b_ptr;

	sp->Para_num = 0;
	sp->Para_M_num = 0;

	for (i = 0, b_ptr = sp->bnst_data; i < sp->Bnst_num; i++, b_ptr++) {
	if (b_ptr->dpnd_type == 'P') {
		if (sp->Para_num >= PARA_MAX) {
		break;
		}
		sp->para_data[sp->Para_num].key_pos = i;
		sp->para_data[sp->Para_num].jend_pos = b_ptr->dpnd_head;
		for (j = i - 1; 
		 j >= 0 &&	
			 (sp->bnst_data[j].dpnd_head < i ||
			  (sp->bnst_data[j].dpnd_head == i &&
			   sp->bnst_data[j].dpnd_type != 'P'));
		 j--);
		sp->para_data[sp->Para_num].max_path[0] = j + 1;
		sp->para_data[sp->Para_num].status = 'n';
		sp->Para_num++;
	}
	}
	return detect_para_relation(sp);
}

/*====================================================================
							   END
====================================================================*/
