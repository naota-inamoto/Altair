/*====================================================================

			   文節間の比較・類似度計算

											   S.Kurohashi 91. 6.25
											   S.Kurohashi 93. 5.31

	$Id: bnst_compare.c,v 1.18 2004/11/27 06:14:52 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

#ifdef	_WINDOWS
static char* fuzoku = NULL;
static char* level = NULL;
static char* levelc = NULL; 
static char* yougen = NULL;
static char* taigen = NULL;
static char* hei_ki_mei = NULL; 
static char* gijikeisan_teki = NULL; 
static char* yougen_han = NULL;
static char* hei_ki_q = NULL;
static char* tame_sei = NULL;
static char* fukugouji = NULL;
static char* jinmei = NULL;
static char* chimei = NULL;
static char* soshikimei = NULL;
static char* suuryou = NULL;
static char* conuter = NULL;
static char* setsubiji = NULL;
static char* reru = NULL;
static char* rareru = NULL;
static char* seru = NULL;
static char* saseru = NULL;
static char* nai = NULL;
static char* nu = NULL;
static char* keidai = NULL;
static char* suru = NULL;
static char* dekiru = NULL;
static char* kdekiru = NULL;
static char fuzoku_2[] = "付属";
static char level_2[] = "レベル";
static char levelc_2[] = "レベル:"; 
static char yougen_2[] = "用言";
static char taigen_2[] = "体言";
static char hei_ki_mei_2[] = "並キ:名"; 
static char gijikeisan_teki_2[] = "類似計算:的"; 
static char yougen_han_2[] = "用言:判";
static char hei_ki_q_2[] = "並キ:？";
static char tame_sei_2[] = "ため-せい";
static char fukugouji_2[] = "複合辞";
static char jinmei_2[] = "人名";
static char chimei_2[] = "地名";
static char soshikimei_2[] = "組織名";
static char suuryou_2[] = "数量";
static char conuter_2[] = "カウンタ";
static char setsubiji_2[] = "接尾辞";
static char reru_2[] = "～れる";
static char rareru_2[] = "～られる";
static char seru_2[] = "～せる";
static char saseru_2[] = "～させる";
static char nai_2[] = "～ない";
static char nu_2[] = "～ぬ";
static char keidai_2[] = "提題";
static char suru_2[] = "する";
static char dekiru_2[] = "できる";
static char kdekiru_2[] = "出来る";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	fuzoku = toStringEUC(fuzoku_2);
	level = toStringEUC(level_2);
	levelc = toStringEUC(levelc_2); 
	yougen = toStringEUC(yougen_2);
	taigen = toStringEUC(taigen_2);
	hei_ki_mei = toStringEUC(hei_ki_mei_2); 
	gijikeisan_teki = toStringEUC(gijikeisan_teki_2);	
	yougen_han = toStringEUC(yougen_han_2);
	hei_ki_q = toStringEUC(hei_ki_q_2);
	tame_sei = toStringEUC(tame_sei_2);
	fukugouji = toStringEUC(fukugouji_2);
	jinmei = toStringEUC(jinmei_2);
	chimei = toStringEUC(chimei_2);
	soshikimei = toStringEUC(soshikimei_2);
	suuryou = toStringEUC(suuryou_2);
	conuter = toStringEUC(conuter_2);
	setsubiji = toStringEUC(setsubiji_2);
	reru = toStringEUC(reru_2);
	rareru = toStringEUC(rareru_2);
	seru = toStringEUC(seru_2);
	saseru = toStringEUC(saseru_2);
	nai = toStringEUC(nai_2);
	nu = toStringEUC(nu_2);
	keidai = toStringEUC(keidai_2);
	suru = toStringEUC(suru_2);
	dekiru = toStringEUC(dekiru_2);
	kdekiru = toStringEUC(kdekiru_2);
#endif
}
void final_str_bnst_compare() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(fuzoku); fuzoku = NULL;
	free(level); level = NULL;
	free(levelc); levelc = NULL;	
	free(yougen); yougen = NULL;
	free(taigen); taigen = NULL;
	free(hei_ki_mei); hei_ki_mei = NULL;	
	free(gijikeisan_teki); gijikeisan_teki = NULL;	
	free(yougen_han); yougen_han = NULL;
	free(hei_ki_q); hei_ki_q = NULL;
	free(tame_sei); tame_sei = NULL;
	free(fukugouji); fukugouji = NULL;
	free(jinmei); jinmei = NULL;
	free(chimei); chimei = NULL;
	free(soshikimei); soshikimei = NULL;
	free(suuryou); suuryou = NULL;
	free(conuter); conuter = NULL;
	free(setsubiji); setsubiji = NULL;
	free(reru); reru = NULL;
	free(rareru); rareru = NULL;
	free(seru); seru = NULL;
	free(saseru); saseru = NULL;
	free(nai); nai = NULL;
	free(nu); nu = NULL;
	free(keidai); keidai = NULL;
	free(suru); suru = NULL;
	free(dekiru); dekiru = NULL;
	free(kdekiru); kdekiru = NULL;
#endif
}
#ifdef	_LINUX
static char fuzoku[] = "付属";
static char level[] = "レベル";
static char levelc[] = "レベル:"; 
static char yougen[] = "用言";
static char taigen[] = "体言";
static char hei_ki_mei[] = "並キ:名"; 
static char gijikeisan_teki[] = "類似計算:的"; 
static char yougen_han[] = "用言:判";
static char hei_ki_q[] = "並キ:？";
static char tame_sei[] = "ため-せい";
static char fukugouji[] = "複合辞";
static char jinmei[] = "人名";
static char chimei[] = "地名";
static char soshikimei[] = "組織名";
static char suuryou[] = "数量";
static char conuter[] = "カウンタ";
static char setsubiji[] = "接尾辞";
static char reru[] = "～れる";
static char rareru[] = "～られる";
static char seru[] = "～せる";
static char saseru[] = "～させる";
static char nai[] = "～ない";
static char nu[] = "～ぬ";
static char keidai[] = "提題";
static char suru[] = "する";
static char dekiru[] = "できる";
static char kdekiru[] = "出来る";
#endif

/*==================================================================*/
			  int str_part_cmp(char *c1, char *c2)
/*==================================================================*/
{
	int len, len1, len2, pre, post, match;
	
	len1 = (int)strlen(c1);
	len2 = (int)strlen(c2); 	
	len = len1 < len2 ? len1 : len2;
	
	pre = 0;
	while (len > pre && *(c1 + pre) == *(c2 + pre)) {
	pre++;
	}

	post = 0;
	while (len > post && *(c1 + len1 - post - 1) == *(c2 + len2 - post - 1)) {
	post++;
	}
	
	match = pre > post ? pre : post;
	if (match % 2) match -= 1;
	return match;
}

/*==================================================================*/
  int check_fuzoku(BNST_DATA *ptr, int Hinshi, int Bunrui, char *cp)
/*==================================================================*/
{
	int i;

	init_str();

	/* 一致する付属語があれば真 */

	if (ptr == NULL) return 0;
	for (i = ptr->mrph_num - 1; i >= 0 ; i--) {
	if (check_feature((ptr->mrph_ptr + i)->f, fuzoku)) {
		if ((Hinshi == 0 || Hinshi == (ptr->mrph_ptr + i)->Hinshi) &&
		(Bunrui == 0 || Bunrui == (ptr->mrph_ptr + i)->Bunrui) &&
		(cp == NULL  || str_eq((ptr->mrph_ptr + i)->Goi, cp))) {
		return 1;
		}
	}
	/* 自立語など */
	else {
		return 0;
	}
	}
	return 0;
}

/*==================================================================*/
int check_fuzoku_substr(BNST_DATA *ptr, int Hinshi, int Bunrui, char *cp)
/*==================================================================*/
{
	int i;

	init_str();

	if (ptr == NULL) return 0;
	for (i = ptr->mrph_num - 1; i >= 0 ; i--) {
	if (check_feature((ptr->mrph_ptr + i)->f, fuzoku)) {
		if ((Hinshi == 0 || Hinshi == (ptr->mrph_ptr + i)->Hinshi) &&
		(Bunrui == 0 || Bunrui == (ptr->mrph_ptr + i)->Bunrui) &&
		(cp == NULL  || strstr((ptr->mrph_ptr + i)->Goi, cp))) {
		return 1;
		}
	}
	/* 自立語など */
	else {
		return 0;
	}
	}
	return 0;
}

/*==================================================================*/
int check_bnst_substr(BNST_DATA *ptr, int Hinshi, int Bunrui, char *cp)
/*==================================================================*/
{
	int i;

	if (ptr == NULL) return 0;
	for (i = 0; i < ptr->mrph_num; i++) 
	  if ((Hinshi == 0 || Hinshi == (ptr->mrph_ptr + i)->Hinshi) &&
	  (Bunrui == 0 || Bunrui == (ptr->mrph_ptr + i)->Bunrui) &&
	  (cp == NULL  || strstr((ptr->mrph_ptr + i)->Goi, cp)))
	return 1;
	return 0;
}

/*==================================================================*/
int jiritu_fuzoku_check(BNST_DATA *ptr1, BNST_DATA *ptr2, char *cp)
/*==================================================================*/
{
	if ((str_eq(ptr1->head_ptr->Goi, cp) && check_fuzoku(ptr2, 0, 0, cp)) ||	
	(str_eq(ptr2->head_ptr->Goi, cp) && check_fuzoku(ptr1, 0, 0, cp)))
	return 1;
	else	
	return 0;
}

/*==================================================================*/
		  int bgh_match(BNST_DATA *ptr1, BNST_DATA *ptr2)
/*==================================================================*/
{
	/* 返り値
		一方でも分類語彙表コードがない場合	: -1
	3桁未満の一致				: 0
	3桁以上一致している場合 		: (一致桁数 - 2)
	 */

	int i, j, point, max_point = 0;

	if (! *(ptr1->BGH_code) || ! *(ptr2->BGH_code))
	return -1;

	for (i = 0; ptr1->BGH_code[i]; i+=BGH_CODE_SIZE)
	for (j = 0; ptr2->BGH_code[j]; j+=BGH_CODE_SIZE) {
		point = bgh_code_match(ptr1->BGH_code+i, ptr2->BGH_code+j);
		if (max_point < point) max_point = point;
	}
	
	if (max_point < 3)	
	return 0;
	else
	return (max_point-2);
}

/*==================================================================*/
		int sm_match(BNST_DATA *ptr1, BNST_DATA *ptr2)
/*==================================================================*/
{
	/* 返り値
		一方でも NTT コードがない場合	: -1
	満点				: BGH_CODE_SIZE-2 == 8	
	 */

	int i, j, code_size;
	float point, max_point = 0;

	if (! *(ptr1->SM_code) || ! *(ptr2->SM_code))
	return -1;

	code_size = THESAURUS[ParaThesaurus].code_size;

	for (i = 0; ptr1->SM_code[i]; i+=code_size)
	for (j = 0; ptr2->SM_code[j]; j+=code_size) {
		if (ParaThesaurus == USE_NTT) {
		point = ntt_code_match(ptr1->SM_code+i, ptr2->SM_code+j, SM_EXPAND_NE);
		}
		else {
		point = general_code_match(&THESAURUS[ParaThesaurus], ptr1->SM_code+i, ptr2->SM_code+j);
		}
		if (max_point < point) max_point = point;
	}

	/* 類似度 0.4 以下は切る */
	max_point = (float)((max_point-0.4)*(BGH_CODE_SIZE-2)/(BGH_CODE_SIZE-4)*BGH_CODE_SIZE);
	if (max_point < 0)
	return 0;
	else
	return (int)(max_point);
}

/*==================================================================*/
	int subordinate_level_comp(BNST_DATA *ptr1, BNST_DATA *ptr2)
/*==================================================================*/
{
	char *level1, *level2;

	init_str();

	level1 = (char *)check_feature(ptr1->f, level);
	level2 = (char *)check_feature(ptr2->f, level);

	if (level1 == NULL) return TRUE;		/* なし:何でも -> T */
	else if (level2 == NULL) return FALSE;	/* 何でも:なし -> F */
	else if (levelcmp(level1 + strlen(levelc),	
			  level2 + strlen(levelc)) <= 0)	/* ptr1 <= ptr2 -> T */
	return TRUE;
	else return FALSE;
}

/*==================================================================*/
	int subordinate_level_check(char *cp, BNST_DATA *ptr2)
/*==================================================================*/
{
	char *level1, *level2;

	init_str();

	level1 = cp;
	level2 = check_feature(ptr2->f, level);

	if (level1 == NULL) return TRUE;		/* なし:何でも -> T */
	else if (level2 == NULL) return FALSE;	/* 何でも:なし -> F */
	else if (levelcmp(level1, level2 + strlen(levelc)) <= 0)
	return TRUE;				/* cp <= ptr2 -> T */
	else return FALSE;
}

/*==================================================================*/
		  int levelcmp(char *cp1, char *cp2)
/*==================================================================*/
{
	int level1 = 0, level2 = 0;
	if (!strcmp(cp1, "A-")) 	 level1 = 1;
	else if (!strcmp(cp1, "A"))  level1 = 2;
	else if (!strcmp(cp1, "B-")) level1 = 3;
	else if (!strcmp(cp1, "B"))  level1 = 4;
	else if (!strcmp(cp1, "B+")) level1 = 5;
	else if (!strcmp(cp1, "C"))  level1 = 6;
	else fprintf(stderr, "Invalid level (%s)\n", cp1);
	if (!strcmp(cp2, "A-")) 	 level2 = 1;
	else if (!strcmp(cp2, "A"))  level2 = 2;
	else if (!strcmp(cp2, "B-")) level2 = 3;
	else if (!strcmp(cp2, "B"))  level2 = 4;
	else if (!strcmp(cp2, "B+")) level2 = 5;
	else if (!strcmp(cp2, "C"))  level2 = 6;
	else fprintf(stderr, "Invalid level (%s)\n", cp2);
	return level1 - level2;
}

/*==================================================================*/
	 int calc_match(SENTENCE_DATA *sp, int pre, int pos)
/*==================================================================*/
{
	int 	i, j, part_mt_point, mt_point, point = 0;
	int 	flag1, flag2, content_word_match;
	BNST_DATA	*ptr1, *ptr2;

	init_str();

	ptr1 = &(sp->bnst_data[pre]);
	ptr2 = &(sp->bnst_data[pos]);

	/* 用言，体言 */

	if ((check_feature(ptr1->f, yougen) &&
	 check_feature(ptr2->f, yougen)) ||

	(check_feature(ptr1->f, taigen) &&
	 check_feature(ptr2->f, taigen)) || 
	
	/* 「的，」と「的だ」 */
	(check_feature(ptr1->f, hei_ki_mei) &&	
	 check_feature(ptr1->f, gijikeisan_teki) && 
	 check_feature(ptr2->f, gijikeisan_teki))
	 /* check_bnst_substr(ptr1, 0, 0, "的") &&	
		check_bnst_substr(ptr2, 0, 0, "的だ")) */
	) {

	/* ただし，判定詞 -- 体言 の類似度は 0 */
	if (check_feature(ptr1->f, yougen_han) &&
		!check_feature(ptr1->f, hei_ki_q) && /* 「～ではなく」「ですとか」を除く */
		check_feature(ptr2->f, taigen) &&
		!check_feature(ptr2->f, yougen_han)) return 0;
	
	/* 「ため」「せい」と他の体言に類似度を与えないように */

	if ((check_feature(ptr1->f, tame_sei) &&
		 !check_feature(ptr2->f, tame_sei)) ||
		(!check_feature(ptr1->f, tame_sei) &&
		 check_feature(ptr2->f, tame_sei))) return 0;

	/* 複合辞とそれ以外も類似度 0 */

	if ((check_feature(ptr1->f, fukugouji) &&
		 !check_feature(ptr2->f, fukugouji)) ||
		(!check_feature(ptr1->f, fukugouji) &&
		 check_feature(ptr2->f, fukugouji))) return 0;

	point += 2;

	if (check_feature(ptr1->f, taigen) &&
		check_feature(ptr2->f, taigen)) {

		/*	
		   体言同士の場合
		   ・人名同士 -- 5
		   ・地名同士 -- 5
		   ・組織名同士 -- 5
		   ・人名地名組織名 -- 2 (形態素解析のズレを考慮)
		   ・数量同士 -- 2 (続く名詞(助数辞)で評価)
				※ 助数辞が一致しなくても類似することもある
				例)「人口は八万七千人だったが、人口増加率は一位で、…」
		   ・時間同士 -- 2			
		   ・その他同士 -- 自立語の比較
		*/

		if (check_feature(ptr1->f, jinmei)) {
		flag1 = 0;
		} else if (check_feature(ptr1->f, chimei)) {
		flag1 = 1;
		} else if (check_feature(ptr1->f, soshikimei)) {
		flag1 = 2;
		} else if (check_feature(ptr1->f, suuryou)) {
		flag1 = 3;
		/* } else if (check_feature(ptr1->f, "時間")) {
		flag1 = 4; */
		} else {
		flag1 = 5;
		}

		if (check_feature(ptr2->f, jinmei)) {
		flag2 = 0;
		} else if (check_feature(ptr2->f, chimei)) {
		flag2 = 1;
		} else if (check_feature(ptr2->f, soshikimei)) {
		flag2 = 2;
		} else if (check_feature(ptr2->f, suuryou)) {
		flag2 = 3;
		/* } else if (check_feature(ptr2->f, "時間")) {
		flag2 = 4; */
		} else {
		flag2 = 5;
		}

		if (flag1 == 0 && flag2 == 0) {
		point += 5;
		content_word_match = 0;
		}
		else if (flag1 == 1 && flag2 == 1) {
		point += 5;
		content_word_match = 0;
		}
		else if (flag1 == 2 && flag2 == 2) {
		point += 5;
		content_word_match = 0;
		}
		else if ((flag1 == 0 || flag1 == 1 || flag1 == 2) &&
			 (flag2 == 0 || flag2 == 1 || flag2 == 2)) {
		point += 2; /* 組織と人名などの対応を考慮 */
		content_word_match = 0;
		}
		else if (flag1 == 3 && flag2 == 3) {
		point += 2;
		for (i = 0; i < ptr1->mrph_num; i++)	
			for (j = 0; j < ptr2->mrph_num; j++)	
			if (str_eq((ptr1->mrph_ptr + i)->Goi,	
				   (ptr2->mrph_ptr + j)->Goi) &&
				check_feature((ptr1->mrph_ptr + i)->f, conuter) &&
				check_feature((ptr2->mrph_ptr + j)->f, conuter)) {
				point += 5;
				goto Counter_check;
			}
		  Counter_check:
		content_word_match = 0;
		}
		else if (flag1 == 4 && flag2 == 4) {
		point += 2;
		content_word_match = 0;
		}
		else if (flag1 == 5 && flag2 == 5) {
			content_word_match = 1;
		}
		else {
		content_word_match = 0;
		}
	}
	else {
		content_word_match = 1;
	}

	if (content_word_match == 1) {

		/* 自立語の一致 */
	
		/* if (str_eq(ptr1->head_ptr->Goi, ptr2->head_ptr->Goi)) { */
		if (str_eq(ptr1->Jiritu_Go, ptr2->Jiritu_Go)) {
		point += 10;
		
		} else {

		/* シソーラスによる類似度 */

		if (ParaThesaurus == USE_NONE) {
			mt_point = -1;
		}
		else if (ParaThesaurus == USE_BGH) {
			mt_point = bgh_match(ptr1, ptr2) * 2;
		}
		else {
			mt_point = sm_match(ptr1, ptr2) * 2;
		}


		/* 自立語の部分一致 (少なくとも一方の意味属性コードがない場合) */
		
		part_mt_point = 0;
		if (mt_point < 0) {
			mt_point = 0;
			if (check_feature(ptr1->f, taigen) &&
			check_feature(ptr2->f, taigen))
			part_mt_point = str_part_cmp(ptr1->head_ptr->Goi, ptr2->head_ptr->Goi);
		}

		/* シソーラスと部分一致の得点は最大10 */

		if ((part_mt_point + mt_point) > 10)
			point += 10;
		else
			point += (part_mt_point + mt_point);
		}
	}

	/* 主辞形態素より後, 接尾辞以外の付属語の一致 */

	for (i = ptr1->mrph_num - 1; i >= 0 ; i--) {
		if (check_feature((ptr1->mrph_ptr + i)->f, fuzoku) &&	
		ptr1->mrph_ptr + i > ptr1->head_ptr) {
		if (!strcmp((const char*)Class[(ptr1->mrph_ptr + i)->Hinshi][0].id, setsubiji)) {
			continue;
		}
		for (j = ptr2->mrph_num - 1; j >= 0 ; j--) {
			if (check_feature((ptr2->mrph_ptr + j)->f, fuzoku) &&	
			ptr2->mrph_ptr + j > ptr2->head_ptr) {
			if (!strcmp((const char*)Class[(ptr2->mrph_ptr + j)->Hinshi][0].id, setsubiji)) {
				continue;
			}
			if (str_eq((ptr1->mrph_ptr + i)->Goi,	
				   (ptr2->mrph_ptr + j)->Goi)) {
				point += 2; /* 3 */
			}
			}
			else {
			break;
			}
		}
		}
		else {
		break;
		}
	}

	if ((check_feature(ptr1->f, reru) &&
		 check_feature(ptr2->f, rareru)) ||
		(check_feature(ptr1->f, rareru) &&
		 check_feature(ptr2->f, reru))) {	
		point += 2;
	}
	if ((check_feature(ptr1->f, seru) &&
		 check_feature(ptr2->f, saseru)) ||
		(check_feature(ptr1->f, saseru) &&
		 check_feature(ptr2->f, seru))) {	
		point += 2;
	}
	if ((check_feature(ptr1->f, nai) &&
		 check_feature(ptr2->f, nu)) ||
		(check_feature(ptr1->f, nu) &&
		 check_feature(ptr2->f, nai))) {	
		point += 2;
	}

	/* 追加 */

	if (check_feature(ptr1->f, keidai) &&
		check_feature(ptr2->f, keidai))
		point += 3;

	/* 「する」,「できる」などの自立語付属語のずれ */

	if (jiritu_fuzoku_check(ptr1, ptr2, suru))
		point += 1;

	if (jiritu_fuzoku_check(ptr1, ptr2, dekiru) ||
		jiritu_fuzoku_check(ptr1, ptr2, kdekiru))
		point += 3;
	}
	
	return point;
}

/*==================================================================*/
		  void calc_match_matrix(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j;
	
	for (i = 0; i < sp->Bnst_num; i++)	
	for (j = i+1; j < sp->Bnst_num; j++)
		match_matrix[i][j] = calc_match(sp, i, j);
}

/*====================================================================
							   END
====================================================================*/
