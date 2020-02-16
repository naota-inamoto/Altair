/*====================================================================

				 依存構造解析

											   S.Kurohashi 93. 5.31

	$Id: dpnd_analysis.c,v 1.61.2.3 2005/08/06 13:17:36 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

int Possibility;	/* 依存構造の可能性の何番目か */
static int dpndID = 0;

#ifdef	_WINDOWS
static char* substantive = NULL;
static char* declinable = NULL;
static char* declinable_han = NULL;
static char* declinable_dou = NULL;
static char* semi_declinable = NULL;
static char* candidate = NULL;
static char* outer_rel = NULL;
static char* rel_out_of_rule = NULL;
static char* parentheses_end = NULL;
static char* when = NULL;
static char* amount = NULL;
static char* tag_unit_modif = NULL;
static char* ignore_tag_unit_modif = NULL;
static char* tomirareru = NULL;
static char* dai = NULL;
static char* dai_modif = NULL;
static char* dai_modif_30 = NULL;
static char* ga_kaku = NULL;
static char* ga_2_kaku = NULL;
static char* wo_kaku = NULL;
static char* ni_kaku = NULL;
static char* modif = NULL;
static char* modif_ga_kaku = NULL;
static char* modif_no_kaku = NULL;
static char* modif_matsu_kaku = NULL;
static char* modif_mu_kaku = NULL;
static char* modif_end = NULL;
static char* comma = NULL;
static char* sentence_end = NULL;
static char substantive_2[] = "体言";
static char declinable_2[] = "用言";
static char declinable_han_2[] = "用言:判";
static char declinable_dou_2[] = "用言:動";
static char semi_declinable_2[] = "準用言";
static char candidate_2[] = "候補";
static char outer_rel_2[] = "外の関係";
static char rel_out_of_rule_2[] = "ルール外の関係";
static char parentheses_end_2[] = "括弧終";
static char when_2[] = "時間";
static char amount_2[] = "数量";
static char tag_unit_modif_2[] = "タグ単位受";
static char ignore_tag_unit_modif_2[] = "タグ単位受無視";
static char tomirareru_2[] = "～とみられる";
static char dai_2[] = "提題";
static char dai_modif_2[] = "提題受";
static char dai_modif_30_2[] = "提題受:30";
static char ga_kaku_2[] = "ガ格";
static char ga_2_kaku_2[] = "ガ２";
static char wo_kaku_2[] = "ヲ格";
static char ni_kaku_2[] = "ニ格";
static char modif_2[] = "係";
static char modif_ga_kaku_2[] = "係:ガ格";
static char modif_no_kaku_2[] = "係:ノ格";
static char modif_matsu_kaku_2[] = "係:未格";
static char modif_mu_kaku_2[] = "係:無格従属";
static char modif_end_2[] = "係:文末";
static char comma_2[] = "読点";
static char sentence_end_2[] = "文末";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	substantive = toStringEUC(substantive_2);
	declinable = toStringEUC(declinable_2);
	declinable_han = toStringEUC(declinable_han_2);
	declinable_dou = toStringEUC(declinable_dou_2);
	semi_declinable = toStringEUC(semi_declinable_2);
	candidate = toStringEUC(candidate_2);
	outer_rel = toStringEUC(outer_rel_2);
	rel_out_of_rule = toStringEUC(rel_out_of_rule_2);
	parentheses_end = toStringEUC(parentheses_end_2);
	when = toStringEUC(when_2);
	amount = toStringEUC(amount_2);
	tag_unit_modif = toStringEUC(tag_unit_modif_2);
	ignore_tag_unit_modif = toStringEUC(ignore_tag_unit_modif_2);
	tomirareru = toStringEUC(tomirareru_2);
	dai = toStringEUC(dai_2);
	dai_modif = toStringEUC(dai_modif_2);
	dai_modif_30 = toStringEUC(dai_modif_30_2);
	ga_kaku = toStringEUC(ga_kaku_2);
	ga_2_kaku = toStringEUC(ga_2_kaku_2);
	wo_kaku = toStringEUC(wo_kaku_2);
	ni_kaku = toStringEUC(ni_kaku_2);
	modif = toStringEUC(modif_2);
	modif_ga_kaku = toStringEUC(modif_ga_kaku_2);
	modif_no_kaku = toStringEUC(modif_no_kaku_2);
	modif_matsu_kaku = toStringEUC(modif_matsu_kaku_2);
	modif_mu_kaku = toStringEUC(modif_mu_kaku_2);
	modif_end = toStringEUC(modif_end_2);
	comma = toStringEUC(comma_2);
	sentence_end = toStringEUC(sentence_end_2);
#endif
}
void final_str_dpnd_analysis() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(substantive); substantive = NULL;
	free(declinable); declinable = NULL;
	free(declinable_han); declinable_han = NULL;
	free(declinable_dou); declinable_dou = NULL;
	free(semi_declinable); semi_declinable = NULL;
	free(candidate); candidate = NULL;
	free(outer_rel); outer_rel = NULL;
	free(rel_out_of_rule); rel_out_of_rule = NULL;
	free(parentheses_end); parentheses_end = NULL;
	free(when); when = NULL;
	free(amount); amount = NULL;
	free(tag_unit_modif); tag_unit_modif = NULL;
	free(ignore_tag_unit_modif); ignore_tag_unit_modif = NULL;
	free(tomirareru); tomirareru = NULL;
	free(dai); dai = NULL;
	free(dai_modif); dai_modif = NULL;
	free(dai_modif_30); dai_modif_30 = NULL;
	free(ga_kaku); ga_kaku = NULL;
	free(ga_2_kaku); ga_2_kaku = NULL;
	free(wo_kaku); wo_kaku = NULL;
	free(ni_kaku); ni_kaku = NULL;
	free(modif); modif = NULL;
	free(modif_ga_kaku); modif_ga_kaku = NULL;
	free(modif_no_kaku); modif_no_kaku = NULL;
	free(modif_matsu_kaku); modif_matsu_kaku = NULL;
	free(modif_mu_kaku); modif_mu_kaku = NULL;
	free(modif_end); modif_end = NULL;
	free(comma); comma = NULL;
	free(sentence_end); sentence_end = NULL;
#endif
}
#ifdef	_LINUX
static char substantive[] = "体言";
static char declinable[] = "用言";
static char declinable_han[] = "用言:判";
static char declinable_dou[] = "用言:動";
static char semi_declinable[] = "準用言";
static char candidate[] = "候補";
static char outer_rel[] = "外の関係";
static char rel_out_of_rule[] = "ルール外の関係";
static char parentheses_end[] = "括弧終";
static char when[] = "時間";
static char amount[] = "数量";
static char tag_unit_modif[] = "タグ単位受";
static char ignore_tag_unit_modif[] = "タグ単位受無視";
static char tomirareru[] = "～とみられる";
static char dai[] = "提題";
static char dai_modif[] = "提題受";
static char dai_modif_30[] = "提題受:30";
static char ga_kaku[] = "ガ格";
static char ga_2_kaku[] = "ガ２";
static char wo_kaku[] = "ヲ格";
static char ni_kaku[] = "ニ格";
static char modif[] = "係";
static char modif_ga_kaku[] = "係:ガ格";
static char modif_no_kaku[] = "係:ノ格";
static char modif_matsu_kaku[] = "係:未格";
static char modif_mu_kaku[] = "係:無格従属";
static char modif_end[] = "係:文末";
static char comma[] = "読点";
static char sentence_end[] = "文末";
#endif

extern void dpnd_info_to_tag_pm(SENTENCE_DATA *sp);
extern int bnst_to_tag_tree(SENTENCE_DATA *sp);

/*==================================================================*/
		   void assign_dpnd_rule(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int 	i, j;
	BNST_DATA	*b_ptr;
	DpndRule	*r_ptr;

	for (i = 0, b_ptr = sp->bnst_data; i < sp->Bnst_num; i++, b_ptr++) {
	for (j = 0, r_ptr = DpndRuleArray; j < CurDpndRuleSize; j++, r_ptr++) {

		if (feature_pattern_match(&(r_ptr->dependant), b_ptr->f, NULL, NULL)	
		== 1 /*TRUE*/) {
		b_ptr->dpnd_rule = r_ptr;	
		break;
		}
	}

	if (b_ptr->dpnd_rule == NULL) {
		fprintf(stderr, ";; No DpndRule for %dth bnst (", i);
		print_feature(b_ptr->f, stderr);
		fprintf(stderr, ")\n");

		/* DpndRuleArray[0] はマッチしない時用 */
		b_ptr->dpnd_rule = DpndRuleArray;
	}
	}
}

/*==================================================================*/
		   void calc_dpnd_matrix(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j, k, value, first_uke_flag;
	BNST_DATA *k_ptr, *u_ptr;

	for (i = 0; i < sp->Bnst_num; i++) {
	k_ptr = sp->bnst_data + i;
	first_uke_flag = 1;
	for (j = i + 1; j < sp->Bnst_num; j++) {
		u_ptr = sp->bnst_data + j;
		Dpnd_matrix[i][j] = 0;
		for (k = 0; k_ptr->dpnd_rule->dpnd_type[k]; k++) {
		value = feature_pattern_match(&(k_ptr->dpnd_rule->governor[k]),
						  u_ptr->f, k_ptr, u_ptr);
		if (value == 1 /*TRUE*/) {
			Dpnd_matrix[i][j] = (int)k_ptr->dpnd_rule->dpnd_type[k];
			first_uke_flag = 0;
			break;
		}
		}
	}
	}
}

/*==================================================================*/
		   int relax_dpnd_matrix(SENTENCE_DATA *sp)
/*==================================================================*/
{
	/* 係り先がない場合の緩和

	   括弧によるマスクは優先し，その制限内で末尾に係れるように変更

	   ○ Ａ‥‥「‥‥‥‥‥」‥‥Ｂ (文末)
	   ○ ‥‥‥「Ａ‥‥‥Ｂ」‥‥‥ (括弧終)
	   ○ ‥‥‥「Ａ‥Ｂ．‥」‥‥‥ (係:文末)
	   × Ａ‥‥‥‥Ｂ「‥‥‥‥Ｃ」 (Ｂに係り得るとはしない．
									  Ｃとの関係は解析で対処)
	*/

	int i, j, ok_flag, relax_flag, last_possibility;
	
	init_str();

	relax_flag = FALSE;

	for (i = 0; i < sp->Bnst_num - 1  ; i++) {
	ok_flag = FALSE;
	last_possibility = i;
	for (j = i + 1; j < sp->Bnst_num ; j++) {
		if (Quote_matrix[i][j]) {
		if (Dpnd_matrix[i][j] > 0) {
			ok_flag = TRUE;
			break;
		} else if (check_feature(sp->bnst_data[j].f, modif_end)) {
			last_possibility = j;
			break;
		} else {
			last_possibility = j;
		}
		}
	}

	if (ok_flag == FALSE) {
		if (check_feature(sp->bnst_data[last_possibility].f, sentence_end) ||
		check_feature(sp->bnst_data[last_possibility].f, modif_end) ||
		check_feature(sp->bnst_data[last_possibility].f, parentheses_end)) {
		Dpnd_matrix[i][last_possibility] = 'R';
		relax_flag = TRUE;
		}
	}
	}

	return relax_flag;
}

/*==================================================================*/
int check_uncertain_d_condition(SENTENCE_DATA *sp, DPND *dp, int gvnr)
/*==================================================================*/
{
	/* 後方チ(ェック)の d の係り受けを許す条件

	   ・ 次の可能な係り先(D)が３つ以上後ろ ( d - - D など )
	   ・ 係り元とdの後ろが同じ格	例) 日本で最初に京都で行われた
	   ・ d(係り先)とdの後ろが同じ格	例) 東京で計画中に京都に変更された

	   ※ 「dに読点がある」ことでdを係り先とするのは不適切
	   例) 「うすい板を木目が直角になるように、何枚もはり合わせたもの。」
	*/

	int i, next_D;
	char *dpnd_cp, *gvnr_cp, *next_cp;

	init_str();

	next_D = 0;
	for (i = gvnr + 1; i < sp->Bnst_num ; i++) {
	if (Mask_matrix[dp->pos][i] &&
		Quote_matrix[dp->pos][i] &&
		dp->mask[i] &&
		Dpnd_matrix[dp->pos][i] == 'D') {
		next_D = i;
		break;
	}
	}
	dpnd_cp = check_feature(sp->bnst_data[dp->pos].f, modif);
	gvnr_cp = check_feature(sp->bnst_data[gvnr].f, modif);
	if (gvnr < sp->Bnst_num-1) {
	next_cp = check_feature(sp->bnst_data[gvnr+1].f, modif);	
	}
	else {
	next_cp = NULL;
	}

	if (next_D == 0 ||
	gvnr + 2 < next_D ||
	(gvnr + 2 == next_D && gvnr < sp->Bnst_num-1 &&
	 check_feature(sp->bnst_data[gvnr+1].f, substantive) &&
	 ((dpnd_cp && next_cp && !strcmp(dpnd_cp, next_cp)) ||
	  (gvnr_cp && next_cp && !strcmp(gvnr_cp, next_cp))))) {
	/* fprintf(stderr, "%d -> %d OK\n", i, j); */
	return 1;
	} else {
	return 0;
	}
}

/*==================================================================*/
int compare_dpnd(SENTENCE_DATA * /*sp*/, TOTAL_MGR * /*new_mgr*/, TOTAL_MGR * /*best_mgr*/)
/*==================================================================*/
{
	return FALSE;

#if 0
	if (Possibility == 1 || new_mgr->dflt < best_mgr->dflt) {
	return TRUE;
	} else {
	int i;
	for (i = sp->Bnst_num - 2; i >= 0; i--) {
		if (new_mgr->dpnd.dflt[i] < best_mgr->dpnd.dflt[i]) 
		return TRUE;
		else if (new_mgr->dpnd.dflt[i] > best_mgr->dpnd.dflt[i])	
		return FALSE;
	}
	}

	fprintf(stderr, ";; Error in compare_dpnd !!\n");
	exit(1);
#endif
}

/*==================================================================*/
	 void dpnd_info_to_bnst(SENTENCE_DATA *sp, DPND *dp)
/*==================================================================*/
{
	/* 係り受けに関する種々の情報を DPND から BNST_DATA にコピー */

	int 	i;
	BNST_DATA	*b_ptr;

	for (i = 0, b_ptr = sp->bnst_data; i < sp->Bnst_num; i++, b_ptr++) {

	if (i == sp->Bnst_num - 1) {		/* 最後の文節 */
		b_ptr->dpnd_head = -1;
		b_ptr->dpnd_type = 'D';
	} else if (dp->type[i] == 'd' || dp->type[i] == 'R') {
		b_ptr->dpnd_head = dp->head[i];
		b_ptr->dpnd_type = 'D'; /* relaxした場合もDに */
	} else {
		b_ptr->dpnd_head = dp->head[i];
		b_ptr->dpnd_type = dp->type[i];
	}
	b_ptr->dpnd_dflt = dp->dflt[i];
	}
}

/*==================================================================*/
	void dpnd_info_to_tag_raw(SENTENCE_DATA *sp, DPND *dp)
/*==================================================================*/
{
	/* 係り受けに関する種々の情報を DPND から TAG_DATA にコピー */

	int 	i, last_b = 0, offset;
	char	*cp;
	TAG_DATA	*t_ptr;

	init_str();

	for (i = 0, t_ptr = sp->tag_data; i < sp->Tag_num; i++, t_ptr++) {
	/* もっとも近い文節行を記憶 */
	if (t_ptr->bnum >= 0) {
		last_b = t_ptr->bnum;
	}

	/* 文末 */
	if (i == sp->Tag_num - 1) {
		t_ptr->dpnd_head = -1;
		t_ptr->dpnd_type = 'D';
	}
	/* 隣にかける */
	else if (t_ptr->inum != 0) {
		t_ptr->dpnd_head = t_ptr->num + 1;
		t_ptr->dpnd_type = 'D';
	}
	/* 文節内最後のタグ単位 (inum == 0) */
	else {
		if ((!check_feature((sp->bnst_data + last_b)->f, ignore_tag_unit_modif)) &&
		(cp = check_feature((sp->bnst_data + dp->head[last_b])->f, tag_unit_modif)) != NULL) {
		offset = atoi(cp + 11);
		if (offset > 0 || (sp->bnst_data + dp->head[last_b])->tag_num <= -1 * offset) {
			offset = 0;
		}
		}
		else {
		offset = 0;
		}

		t_ptr->dpnd_head = ((sp->bnst_data + dp->head[last_b])->tag_ptr +	
				(sp->bnst_data + dp->head[last_b])->tag_num - 1 + offset)->num;

		if (dp->type[last_b] == 'd' || dp->type[last_b] == 'R') {
		t_ptr->dpnd_type = 'D';
		}
		else {
		t_ptr->dpnd_type = dp->type[last_b];
		}
	}
	}
}

/*==================================================================*/
	  void dpnd_info_to_tag(SENTENCE_DATA *sp, DPND *dp)
/*==================================================================*/
{
	if (OptInput == OPT_RAW ||	
	(OptInput & OPT_INPUT_BNST)) {
	dpnd_info_to_tag_raw(sp, dp);
	}
	else {
	dpnd_info_to_tag_pm(sp);
	}
}

/*==================================================================*/
		   void para_postprocess(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	init_str();

	for (i = 0; i < sp->Bnst_num; i++) {
	if (check_feature((sp->bnst_data + i)->f, declinable) &&
		!check_feature((sp->bnst_data + i)->f, tomirareru) &&
		(sp->bnst_data + i)->para_num != -1 &&
		sp->para_data[(sp->bnst_data + i)->para_num].status != 'x') {
		
		assign_cfeature(&((sp->bnst_data + i)->f), dai_modif_30);
	}
	}
}

/*==================================================================*/
	  void dpnd_evaluation(SENTENCE_DATA *sp, DPND dpnd)
/*==================================================================*/
{
	int i, j, k, one_score, score, rentai, vacant_slot_num;
	int topic_score;
	int scase_check[SCASE_CODE_SIZE], ha_check, un_count, pred_p;
	char *cp, *cp2;
	BNST_DATA *g_ptr, *d_ptr;

	/* 依存構造だけを評価する場合の関数
	   (各文節について，そこに係っている文節の評価点を計算)

	   評価基準
	   ========
	   0. 係り先のdefault位置との差をペナルティに(kakari_uke.rule)

	   1. 「～は」(提題,係:未格)の係り先は優先されるものがある
			(bnst_etc.ruleで指定，並列のキーは並列解析後プログラムで指定)

	   2. 「～は」は一述語に一つ係ることを優先(時間,数量は別)

	   3. すべての格要素は同一表層格が一述語に一つ係ることを優先(ガガは別)

	   4. 未格，連体修飾先はガ,ヲ,ニ格の余っているスロット数だけ点数付与
	*/

	init_str();

	score = 0;
	for (i = 1; i < sp->Bnst_num; i++) {
	g_ptr = sp->bnst_data + i;

	one_score = 0;
	for (k = 0; k < SCASE_CODE_SIZE; k++) scase_check[k] = 0;
	ha_check = 0;
	un_count = 0;

	if (check_feature(g_ptr->f, declinable) ||
		check_feature(g_ptr->f, semi_declinable)) {
		pred_p = 1;
	} else {
		pred_p = 0;
	}

	for (j = i-1; j >= 0; j--) {
		d_ptr = sp->bnst_data + j;

		if (dpnd.head[j] == i) {

		/* 係り先のDEFAULTの位置との差をペナルティに
			 ※ 提題はC,B'を求めて遠くに係ることがあるが，それが
				他の係り先に影響しないよう,ペナルティに差をつける */

		if (check_feature(d_ptr->f, dai)) {
			one_score -= dpnd.dflt[j];
		} else {
			one_score -= dpnd.dflt[j] * 2;
		}
		
		/* 読点をもつものが隣にかかることを防ぐ */

		if (j + 1 == i && check_feature(d_ptr->f, comma)) {
			one_score -= 5;
		}

		if (pred_p &&
			(cp = check_feature(d_ptr->f, modif)) != NULL) {
			
			/* 未格 提題(「～は」)の扱い */

			if (check_feature(d_ptr->f, dai) &&
			!strcmp(cp, modif_matsu_kaku)) {

			/* 文末, 「～が」など, 並列末, C, B'に係ることを優先 */

			if ((cp2 = check_feature(g_ptr->f, dai_modif))	
				!= NULL) {
				art_sscanf_1(cp2, "%*[^:]:%d", &topic_score);
				one_score += topic_score;
			}
			/* else {one_score -= 15;} */

			/* 一つめの提題にだけ点を与える (時間,数量は別)
				 → 複数の提題が同一述語に係ることを防ぐ */

			if (check_feature(d_ptr->f, when) ||
				check_feature(d_ptr->f, amount)) {
				one_score += 10;
			} else if (ha_check == 0){
				one_score += 10;
				ha_check = 1;
			}
			}

			k = case2num(cp+3);

			/* 格要素一般の扱い */

			/* 未格 : 数えておき，後で空スロットを調べる (時間,数量は別) */

			if (!strcmp(cp, modif_matsu_kaku)) {
			if (check_feature(d_ptr->f, when) ||
				check_feature(d_ptr->f, amount)) {
				one_score += 10;
			} else {
				un_count++;
			}
			}

			/* ノ格 : 体言以外なら break	
					  → それより前の格要素には点を与えない．
				  → ノ格がかかればそれより前の格はかからない

				  ※ 「体言」というのは判定詞のこと，ただし
					 文末などでは用言:動となっていることも
				 あるので，「体言」でチェック */

			else if (!strcmp(cp, modif_no_kaku)) {
			if (!check_feature(g_ptr->f, substantive)) {
				one_score += 10;
				break;
			}
			}	

			/* ガ格 : ガガ構文があるので少し複雑 */

			else if (!strcmp(cp, modif_ga_kaku)) {
			if (g_ptr->SCASE_code[case2num(ga_kaku)] &&
				scase_check[case2num(ga_kaku)] == 0) {
				one_score += 10;
				scase_check[case2num(ga_kaku)] = 1;
			}	
			else if (g_ptr->SCASE_code[case2num(ga_2_kaku)] &&
				scase_check[case2num(ga_2_kaku)] == 0) {
				one_score += 10;
				scase_check[case2num(ga_kaku)] = 1;
			}
			}

			/* 他の格 : 各格1つは点数をあたえる
			   ※ ニ格の場合，時間とそれ以外は区別する方がいいかも？ */
			else if (k != -1) {
			if (scase_check[k] == 0) {
				scase_check[k] = 1;
				one_score += 10;
			}	
			}

			/* 「～するのは～だ」にボーナス 01/01/11
			   ほとんどの場合改善．

			   改善例)
			   「抗議したのも 任官を 拒否される 理由の 一つらしい」

			   「使うのは 恐ろしい ことだ。」
			   「円満決着に なるかどうかは 微妙な ところだ。」
					※ これらの例は「こと/ところだ」に係ると扱う

			   「他人に 教えるのが 好きになる やり方です」
					※ この例は曖昧だが，文脈上正しい

			   副作用例)
			   「だれが ＭＶＰか 分からない 試合でしょう」
			   「～ 殴るなど した 疑い。」
			   「ビザを 取るのも 大変な 時代。」
			   「波が 高まるのは 避けられそうにない 雲行きだ。」
			   「あまり 役立つとは 思われない 論理だ。」
			   「どう 折り合うかが 問題視されてきた 法だ。」
			   「認められるかどうかが 争われた 裁判で」

			   ※問題※
			   「あの戦争」が～ のような場合も用言とみなされるのが問題
			 */

			if (check_feature(d_ptr->f, declinable) &&
			(check_feature(d_ptr->f, modif_matsu_kaku) ||
			 check_feature(d_ptr->f, modif_ga_kaku)) &&
			check_feature(g_ptr->f, declinable_han)) {
			  one_score += 3;
			}
		}
		}
	}

	/* 用言の場合，最終的に未格,ガ格,ヲ格,ニ格,連体修飾に対して
	   ガ格,ヲ格,ニ格のスロット分だけ点数を与える */

	if (pred_p) {

		/* 連体修飾の場合，係先が
		   ・形式名詞,副詞的名詞
		   ・「予定」,「見込み」など
		   でなければ一つの格要素と考える */

		if (check_feature(g_ptr->f, "係:連格")) {
		if (check_feature(sp->bnst_data[dpnd.head[i]].f, outer_rel) ||	
			check_feature(sp->bnst_data[dpnd.head[i]].f, rel_out_of_rule)) {
			rentai = 0;
			one_score += 10;	/* 外の関係ならここで加点 */
		} else {
			rentai = 1; /* それ以外なら後で空きスロットをチェック */
		}
		} else {
		rentai = 0;
		}

		/* 空いているガ格,ヲ格,ニ格,ガ２ */

		vacant_slot_num = 0;
		if ((g_ptr->SCASE_code[case2num(ga_kaku)]
		 - scase_check[case2num(ga_kaku)]) == 1) {
		vacant_slot_num ++;
		}
		if ((g_ptr->SCASE_code[case2num(wo_kaku)]
		 - scase_check[case2num(wo_kaku)]) == 1) {
		vacant_slot_num ++;
		}
		if ((g_ptr->SCASE_code[case2num(ni_kaku)]
		 - scase_check[case2num(ni_kaku)]) == 1 &&
		rentai == 1 &&
		check_feature(g_ptr->f, declinable_dou)) {
		vacant_slot_num ++;
		/* ニ格は動詞で連体修飾の場合だけ考慮，つまり連体
		   修飾に割り当てるだけで，未格のスロットとはしない */
		}
		if ((g_ptr->SCASE_code[case2num(ga_2_kaku)]
		 - scase_check[case2num(ga_2_kaku)]) == 1) {
		vacant_slot_num ++;
		}

		/* 空きスロット分だけ連体修飾，未格にスコアを与える */

		if ((rentai + un_count) <= vacant_slot_num) 
		one_score += (rentai + un_count) * 10;
		else	
		one_score += vacant_slot_num * 10;
	}

	score += one_score;

	if (OptDisplay == OPT_DEBUG) {
		if (i == 1) 
		fprintf(Outfp, "Score:    ");
		if (pred_p) {
		fprintf(Outfp, "%2d*", one_score);
		} else {
		fprintf(Outfp, "%2d ", one_score);
		}
	}
	}

	if (OptDisplay == OPT_DEBUG) {
	fprintf(Outfp, "=%d\n", score);
	}

	if (OptDisplay == OPT_DEBUG) {
	dpnd_info_to_bnst(sp, &dpnd);
	if (OptExpress != OPT_NOTAG && OptExpress != OPT_NOTAGTREE) {
		dpnd_info_to_tag(sp, &dpnd);	
	}
	make_dpnd_tree(sp);
	if (OptExpress != OPT_NOTAG && OptExpress != OPT_NOTAGTREE) {
		bnst_to_tag_tree(sp); /* タグ単位の木へ */
	}
	print_kakari(sp, (OptExpress == OPT_NOTAG || OptExpress == OPT_NOTAGTREE) ? OPT_NOTAGTREE : OPT_TREE);
	}

	if (score > sp->Best_mgr->score) {
	sp->Best_mgr->dpnd = dpnd;
	sp->Best_mgr->score = score;
	sp->Best_mgr->ID = dpndID;
	Possibility++;
	}

	dpndID++;
}

/*==================================================================*/
		void decide_dpnd(SENTENCE_DATA *sp, DPND dpnd)
/*==================================================================*/
{
	int i, count, possibilities[BNST_MAX], default_pos, d_possibility;
	int MaskFlag = 0;
	char *cp;
	BNST_DATA *b_ptr;
	
	init_str();

	if (OptDisplay == OPT_DEBUG) {
	if (dpnd.pos == sp->Bnst_num - 1) {
		fprintf(Outfp, "------");
		for (i = 0; i < sp->Bnst_num; i++)
		  fprintf(Outfp, "-%02d", i);
		fputc('\n', Outfp);
	}
	fprintf(Outfp, "In %2d:", dpnd.pos);
	for (i = 0; i < sp->Bnst_num; i++)
		fprintf(Outfp, " %2d", dpnd.head[i]);
	fputc('\n', Outfp);
	}

	dpnd.pos --;

	/* 文頭まで解析が終わったら評価関数をよぶ */

	if (dpnd.pos == -1) {
	/* 無格従属: 前の文節の係り受けに従う場合 */
	for (i = 0; i < sp->Bnst_num -1; i++)
		if (dpnd.head[i] < 0) {
		/* ありえない係り受け */
		if (i >= dpnd.head[i + dpnd.head[i]]) {
			return;
		}
		dpnd.head[i] = dpnd.head[i + dpnd.head[i]];
		dpnd.check[i].pos[0] = dpnd.head[i];
		}

	if (OptAnalysis == OPT_DPND) {
		dpnd_evaluation(sp, dpnd);
	}	
	return;
	}

	b_ptr = sp->bnst_data + dpnd.pos;
	dpnd.f[dpnd.pos] = b_ptr->f;

	/* (前の係りによる)非交差条件の設定 (dpnd.mask が 0 なら係れない) */

	if (dpnd.pos < sp->Bnst_num - 2)
	for (i = dpnd.pos + 2; i < dpnd.head[dpnd.pos + 1]; i++)
		dpnd.mask[i] = 0;
	
	/* 並列構造のキー文節, 部分並列の文節<I>
	   (すでに行われた並列構造解析の結果をマークするだけ) */

	for (i = dpnd.pos + 1; i < sp->Bnst_num; i++) {
	if (Mask_matrix[dpnd.pos][i] == 2) {
		dpnd.head[dpnd.pos] = i;
		dpnd.type[dpnd.pos] = 'P';
		/* チェック用 */
		/* 並列の場合は一意に決まっているので、候補を挙げるのは意味がない */
		dpnd.check[dpnd.pos].num = 1;
		dpnd.check[dpnd.pos].pos[0] = i;
		decide_dpnd(sp, dpnd);
		return;
	} else if (Mask_matrix[dpnd.pos][i] == 3) {
		dpnd.head[dpnd.pos] = i;
		dpnd.type[dpnd.pos] = 'I';

		dpnd.check[dpnd.pos].num = 1;
		dpnd.check[dpnd.pos].pos[0] = i;
		decide_dpnd(sp, dpnd);
		return;
	}
	}

	/* 前の文節の係り受けに従う場合  例) 「～大統領は一日，～」 */

	if ((cp = check_feature(b_ptr->f, modif_mu_kaku)) != NULL) {
		art_sscanf_1(cp, "%*[^:]:%*[^:]:%d", &(dpnd.head[dpnd.pos]));
		dpnd.type[dpnd.pos] = 'D';
		dpnd.dflt[dpnd.pos] = 0;
	dpnd.check[dpnd.pos].num = 1;
		decide_dpnd(sp, dpnd);
		return;
	}

	/* 通常の係り受け解析 */

	/* 係り先の候補を調べる */
	
	count = 0;
	d_possibility = 1;
	for (i = dpnd.pos + 1; i < sp->Bnst_num; i++) {
	if (Mask_matrix[dpnd.pos][i] &&
		Quote_matrix[dpnd.pos][i] &&
		dpnd.mask[i]) {

		if (d_possibility && Dpnd_matrix[dpnd.pos][i] == 'd') {
		if (check_uncertain_d_condition(sp, &dpnd, i)) {
			possibilities[count] = i;
			count++;
		}
		d_possibility = 0;
		}
		else if (Dpnd_matrix[dpnd.pos][i] &&	
			 Dpnd_matrix[dpnd.pos][i] != 'd') {
		possibilities[count] = i;
		count++;
		d_possibility = 0;
		}

		/* バリアのチェック */
		if (count &&
		b_ptr->dpnd_rule->barrier.fp[0] &&
		feature_pattern_match(&(b_ptr->dpnd_rule->barrier), 
					  sp->bnst_data[i].f,
					  b_ptr, sp->bnst_data + i) == 1 /*TRUE*/)
		break;
	}
	else {
		MaskFlag = 1;
	}
	}

	/* 実際に候補をつくっていく(この関数の再帰的呼び出し) */

	if (count) {

	/* preference は一番近く:1, 二番目:2, 最後:-1
	   default_pos は一番近く:1, 二番目:2, 最後:count に変更 */

	default_pos = (b_ptr->dpnd_rule->preference == -1) ?
		count: b_ptr->dpnd_rule->preference;

	dpnd.check[dpnd.pos].num = count;	/* 候補数 */
	dpnd.check[dpnd.pos].def = default_pos; /* デフォルトの位置 */
	for (i = 0; i < count; i++) {
		dpnd.check[dpnd.pos].pos[i] = possibilities[i];
	}

	/* 一意に決定する場合 */

	if (b_ptr->dpnd_rule->barrier.fp[0] == NULL ||	
		b_ptr->dpnd_rule->decide) {
		if (default_pos <= count) {
		dpnd.head[dpnd.pos] = possibilities[default_pos - 1];
		} else {
		dpnd.head[dpnd.pos] = possibilities[count - 1];
		/* default_pos が 2 なのに，countが 1 しかない場合 */
		}
		dpnd.type[dpnd.pos] = (char)Dpnd_matrix[dpnd.pos][dpnd.head[dpnd.pos]];
		dpnd.dflt[dpnd.pos] = 0;
		decide_dpnd(sp, dpnd);
	}	

	/* すべての可能性をつくり出す場合 */
	/* 節間の係り受けの場合は一意に決めるべき */

	else {
		for (i = 0; i < count; i++) {
		dpnd.head[dpnd.pos] = possibilities[i];
		dpnd.type[dpnd.pos] = (char)Dpnd_matrix[dpnd.pos][dpnd.head[dpnd.pos]];
		dpnd.dflt[dpnd.pos] = abs(default_pos - 1 - i);
		decide_dpnd(sp, dpnd);
		}
	}
	}	

	/* 係り先がない場合
	   文末が並列にマスクされていなければ，文末に係るとする */

	else {
	if (Mask_matrix[dpnd.pos][sp->Bnst_num - 1]) {
		dpnd.head[dpnd.pos] = sp->Bnst_num - 1;
		dpnd.type[dpnd.pos] = 'D';
		dpnd.dflt[dpnd.pos] = 10;
		dpnd.check[dpnd.pos].num = 1;
		dpnd.check[dpnd.pos].pos[0] = sp->Bnst_num - 1;
		decide_dpnd(sp, dpnd);
	}
	}
}

/*==================================================================*/
		 void when_no_dpnd_struct(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	sp->Best_mgr->dpnd.head[sp->Bnst_num - 1] = -1;

	for (i = sp->Bnst_num - 2; i >= 0; i--) {
	sp->Best_mgr->dpnd.head[i] = i + 1;
	sp->Best_mgr->dpnd.type[i] = 'D';
	sp->Best_mgr->dpnd.check[i].num = 1;
	sp->Best_mgr->dpnd.check[i].pos[0] = i + 1;
	}

	sp->Best_mgr->score = 0;
}

/*==================================================================*/
		   int after_decide_dpnd(SENTENCE_DATA * /*sp*/)
/*==================================================================*/
{
	/* 解析済: 構造は与えられたもの1つのみ */
	if (OptInput & OPT_PARSED) {
	Possibility = 1;
	}

	if (Possibility != 0) {
	return TRUE;
	}
	else {	
	return FALSE;
	}
}

/*==================================================================*/
		int detect_dpnd_case_struct(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	DPND dpnd;

	sp->Best_mgr->score = -10000; /* スコアは「より大きい」時に入れ換えるので，
					初期値は十分小さくしておく */
	sp->Best_mgr->dflt = 0;
	sp->Best_mgr->ID = -1;
	Possibility = 0;
	dpndID = 0;

	/* 係り状態の初期化 */

	for (i = 0; i < sp->Bnst_num; i++) {
	dpnd.head[i] = -1;
	dpnd.dflt[i] = 0;
	dpnd.mask[i] = 1;
	memset(&(dpnd.check[i]), 0, sizeof(CHECK_DATA));
	dpnd.check[i].num = -1;
	dpnd.f[i] = NULL;
	}
	dpnd.pos = sp->Bnst_num - 1;

	/* 依存構造解析 */

	decide_dpnd(sp, dpnd);

	/* 構造決定後の処理 */

	return after_decide_dpnd(sp);
}

/*==================================================================*/
		   void check_candidates(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j;
	TOTAL_MGR *tm = sp->Best_mgr;
	char buffer[DATA_LEN], buffer2[SMALL_DATA_LEN];

	init_str();

	/* 各文節ごとにチェック用の feature を与える */
	for (i = 0; i < sp->Bnst_num; i++)
	if (tm->dpnd.check[i].num != -1) {
		/* 係り側 -> 係り先 */
		art_strcpy(buffer, DATA_LEN, candidate);
		for (j = 0; j < tm->dpnd.check[i].num; j++) {
		/* 候補たち */
		art_sprintf_1(buffer2, SMALL_DATA_LEN, ":%d", tm->dpnd.check[i].pos[j]);
		if (strlen(buffer)+strlen(buffer2) >= DATA_LEN) {
			fprintf(stderr, ";; Too long string <%s> (%d) in check_candidates. (%s)\n", 
				buffer, tm->dpnd.check[i].num, sp->KNPSID ? sp->KNPSID+5 : "?");
			return;
		}
		art_strcat(buffer, DATA_LEN, buffer2);
		}
		assign_cfeature(&(sp->bnst_data[i].f), buffer);
	}
}

/*==================================================================*/
		   void memo_by_program(SENTENCE_DATA * /*sp*/)
/*==================================================================*/
{
	/*
	 *	プログラムによるメモへの書き込み
	 */

	/* 緩和をメモに記録する場合
	int i;

	for (i = 0; i < sp->Bnst_num - 1; i++) {
	if (sp->Best_mgr->dpnd.type[i] == 'd') {
		strcat(PM_Memo, " 緩和d");
		sprintf(PM_Memo+strlen(PM_Memo), "(%d)", i);
	} else if (sp->Best_mgr->dpnd.type[i] == 'R') {
		strcat(PM_Memo, " 緩和R");
		sprintf(PM_Memo+strlen(PM_Memo), "(%d)", i);
	}
	}
	*/

	/* 遠い係り受けをメモに記録する場合

	for (i = 0; i < sp->Bnst_num - 1; i++) {
	if (sp->Best_mgr->dpnd.head[i] > i + 3 &&
		!check_feature(sp->bnst_data[i].f, "ハ") &&
		!check_feature(sp->bnst_data[i].f, "読点") &&
		!check_feature(sp->bnst_data[i].f, "用言") &&
		!check_feature(sp->bnst_data[i].f, "係:ガ格") &&
		!check_feature(sp->bnst_data[i].f, "用言:無") &&
		!check_feature(sp->bnst_data[i].f, "並キ") &&
		!check_feature(sp->bnst_data[i+1].f, "括弧始")) {
		strcat(PM_Memo, " 遠係");
		sprintf(PM_Memo+strlen(PM_Memo), "(%d)", i);
	}
	}
	*/
}

/*====================================================================
							   END
====================================================================*/
