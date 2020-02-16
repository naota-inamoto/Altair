/*====================================================================

				 �ˑ��\�����

											   S.Kurohashi 93. 5.31

	$Id: dpnd_analysis.c,v 1.61.2.3 2005/08/06 13:17:36 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

int Possibility;	/* �ˑ��\���̉\���̉��Ԗڂ� */
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
static char substantive_2[] = "�̌�";
static char declinable_2[] = "�p��";
static char declinable_han_2[] = "�p��:��";
static char declinable_dou_2[] = "�p��:��";
static char semi_declinable_2[] = "���p��";
static char candidate_2[] = "���";
static char outer_rel_2[] = "�O�̊֌W";
static char rel_out_of_rule_2[] = "���[���O�̊֌W";
static char parentheses_end_2[] = "���ʏI";
static char when_2[] = "����";
static char amount_2[] = "����";
static char tag_unit_modif_2[] = "�^�O�P�ʎ�";
static char ignore_tag_unit_modif_2[] = "�^�O�P�ʎ󖳎�";
static char tomirareru_2[] = "�`�Ƃ݂���";
static char dai_2[] = "���";
static char dai_modif_2[] = "����";
static char dai_modif_30_2[] = "����:30";
static char ga_kaku_2[] = "�K�i";
static char ga_2_kaku_2[] = "�K�Q";
static char wo_kaku_2[] = "���i";
static char ni_kaku_2[] = "�j�i";
static char modif_2[] = "�W";
static char modif_ga_kaku_2[] = "�W:�K�i";
static char modif_no_kaku_2[] = "�W:�m�i";
static char modif_matsu_kaku_2[] = "�W:���i";
static char modif_mu_kaku_2[] = "�W:���i�]��";
static char modif_end_2[] = "�W:����";
static char comma_2[] = "�Ǔ_";
static char sentence_end_2[] = "����";
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
static char substantive[] = "�̌�";
static char declinable[] = "�p��";
static char declinable_han[] = "�p��:��";
static char declinable_dou[] = "�p��:��";
static char semi_declinable[] = "���p��";
static char candidate[] = "���";
static char outer_rel[] = "�O�̊֌W";
static char rel_out_of_rule[] = "���[���O�̊֌W";
static char parentheses_end[] = "���ʏI";
static char when[] = "����";
static char amount[] = "����";
static char tag_unit_modif[] = "�^�O�P�ʎ�";
static char ignore_tag_unit_modif[] = "�^�O�P�ʎ󖳎�";
static char tomirareru[] = "�`�Ƃ݂���";
static char dai[] = "���";
static char dai_modif[] = "����";
static char dai_modif_30[] = "����:30";
static char ga_kaku[] = "�K�i";
static char ga_2_kaku[] = "�K�Q";
static char wo_kaku[] = "���i";
static char ni_kaku[] = "�j�i";
static char modif[] = "�W";
static char modif_ga_kaku[] = "�W:�K�i";
static char modif_no_kaku[] = "�W:�m�i";
static char modif_matsu_kaku[] = "�W:���i";
static char modif_mu_kaku[] = "�W:���i�]��";
static char modif_end[] = "�W:����";
static char comma[] = "�Ǔ_";
static char sentence_end[] = "����";
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

		/* DpndRuleArray[0] �̓}�b�`���Ȃ����p */
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
	/* �W��悪�Ȃ��ꍇ�̊ɘa

	   ���ʂɂ��}�X�N�͗D�悵�C���̐������Ŗ����ɌW���悤�ɕύX

	   �� �`�d�d�u�d�d�d�d�d�v�d�d�a (����)
	   �� �d�d�d�u�`�d�d�d�a�v�d�d�d (���ʏI)
	   �� �d�d�d�u�`�d�a�D�d�v�d�d�d (�W:����)
	   �~ �`�d�d�d�d�a�u�d�d�d�d�b�v (�a�ɌW�蓾��Ƃ͂��Ȃ��D
									  �b�Ƃ̊֌W�͉�͂őΏ�)
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
	/* ����`(�F�b�N)�� d �̌W��󂯂���������

	   �E ���̉\�ȌW���(D)���R�ȏ��� ( d - - D �Ȃ� )
	   �E �W�茳��d�̌�낪�����i	��) ���{�ōŏ��ɋ��s�ōs��ꂽ
	   �E d(�W���)��d�̌�낪�����i	��) �����Ōv�撆�ɋ��s�ɕύX���ꂽ

	   �� �ud�ɓǓ_������v���Ƃ�d���W���Ƃ���͕̂s�K��
	   ��) �u��������ؖڂ����p�ɂȂ�悤�ɁA�������͂荇�킹�����́B�v
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
	/* �W��󂯂Ɋւ����X�̏��� DPND ���� BNST_DATA �ɃR�s�[ */

	int 	i;
	BNST_DATA	*b_ptr;

	for (i = 0, b_ptr = sp->bnst_data; i < sp->Bnst_num; i++, b_ptr++) {

	if (i == sp->Bnst_num - 1) {		/* �Ō�̕��� */
		b_ptr->dpnd_head = -1;
		b_ptr->dpnd_type = 'D';
	} else if (dp->type[i] == 'd' || dp->type[i] == 'R') {
		b_ptr->dpnd_head = dp->head[i];
		b_ptr->dpnd_type = 'D'; /* relax�����ꍇ��D�� */
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
	/* �W��󂯂Ɋւ����X�̏��� DPND ���� TAG_DATA �ɃR�s�[ */

	int 	i, last_b = 0, offset;
	char	*cp;
	TAG_DATA	*t_ptr;

	init_str();

	for (i = 0, t_ptr = sp->tag_data; i < sp->Tag_num; i++, t_ptr++) {
	/* �����Ƃ��߂����ߍs���L�� */
	if (t_ptr->bnum >= 0) {
		last_b = t_ptr->bnum;
	}

	/* ���� */
	if (i == sp->Tag_num - 1) {
		t_ptr->dpnd_head = -1;
		t_ptr->dpnd_type = 'D';
	}
	/* �ׂɂ����� */
	else if (t_ptr->inum != 0) {
		t_ptr->dpnd_head = t_ptr->num + 1;
		t_ptr->dpnd_type = 'D';
	}
	/* ���ߓ��Ō�̃^�O�P�� (inum == 0) */
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

	/* �ˑ��\��������]������ꍇ�̊֐�
	   (�e���߂ɂ��āC�����ɌW���Ă��镶�߂̕]���_���v�Z)

	   �]���
	   ========
	   0. �W����default�ʒu�Ƃ̍����y�i���e�B��(kakari_uke.rule)

	   1. �u�`�́v(���,�W:���i)�̌W���͗D�悳�����̂�����
			(bnst_etc.rule�Ŏw��C����̃L�[�͕����͌�v���O�����Ŏw��)

	   2. �u�`�́v�͈�q��Ɉ�W�邱�Ƃ�D��(����,���ʂ͕�)

	   3. ���ׂĂ̊i�v�f�͓���\�w�i����q��Ɉ�W�邱�Ƃ�D��(�K�K�͕�)

	   4. ���i�C�A�̏C����̓K,��,�j�i�̗]���Ă���X���b�g�������_���t�^
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

		/* �W����DEFAULT�̈ʒu�Ƃ̍����y�i���e�B��
			 �� ����C,B'�����߂ĉ����ɌW�邱�Ƃ����邪�C���ꂪ
				���̌W���ɉe�����Ȃ��悤,�y�i���e�B�ɍ������� */

		if (check_feature(d_ptr->f, dai)) {
			one_score -= dpnd.dflt[j];
		} else {
			one_score -= dpnd.dflt[j] * 2;
		}
		
		/* �Ǔ_�������̂��ׂɂ����邱�Ƃ�h�� */

		if (j + 1 == i && check_feature(d_ptr->f, comma)) {
			one_score -= 5;
		}

		if (pred_p &&
			(cp = check_feature(d_ptr->f, modif)) != NULL) {
			
			/* ���i ���(�u�`�́v)�̈��� */

			if (check_feature(d_ptr->f, dai) &&
			!strcmp(cp, modif_matsu_kaku)) {

			/* ����, �u�`���v�Ȃ�, ����, C, B'�ɌW�邱�Ƃ�D�� */

			if ((cp2 = check_feature(g_ptr->f, dai_modif))	
				!= NULL) {
				art_sscanf_1(cp2, "%*[^:]:%d", &topic_score);
				one_score += topic_score;
			}
			/* else {one_score -= 15;} */

			/* ��߂̒��ɂ����_��^���� (����,���ʂ͕�)
				 �� �����̒�肪����q��ɌW�邱�Ƃ�h�� */

			if (check_feature(d_ptr->f, when) ||
				check_feature(d_ptr->f, amount)) {
				one_score += 10;
			} else if (ha_check == 0){
				one_score += 10;
				ha_check = 1;
			}
			}

			k = case2num(cp+3);

			/* �i�v�f��ʂ̈��� */

			/* ���i : �����Ă����C��ŋ�X���b�g�𒲂ׂ� (����,���ʂ͕�) */

			if (!strcmp(cp, modif_matsu_kaku)) {
			if (check_feature(d_ptr->f, when) ||
				check_feature(d_ptr->f, amount)) {
				one_score += 10;
			} else {
				un_count++;
			}
			}

			/* �m�i : �̌��ȊO�Ȃ� break	
					  �� ������O�̊i�v�f�ɂ͓_��^���Ȃ��D
				  �� �m�i��������΂�����O�̊i�͂�����Ȃ�

				  �� �u�̌��v�Ƃ����͔̂��莌�̂��ƁC������
					 �����Ȃǂł͗p��:���ƂȂ��Ă��邱�Ƃ�
				 ����̂ŁC�u�̌��v�Ń`�F�b�N */

			else if (!strcmp(cp, modif_no_kaku)) {
			if (!check_feature(g_ptr->f, substantive)) {
				one_score += 10;
				break;
			}
			}	

			/* �K�i : �K�K�\��������̂ŏ������G */

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

			/* ���̊i : �e�i1�͓_������������
			   �� �j�i�̏ꍇ�C���ԂƂ���ȊO�͋�ʂ���������������H */
			else if (k != -1) {
			if (scase_check[k] == 0) {
				scase_check[k] = 1;
				one_score += 10;
			}	
			}

			/* �u�`����̂́`���v�Ƀ{�[�i�X 01/01/11
			   �قƂ�ǂ̏ꍇ���P�D

			   ���P��)
			   �u�R�c�����̂� �C���� ���ۂ���� ���R�� ��炵���v

			   �u�g���̂� ���낵�� ���Ƃ��B�v
			   �u�~�������� �Ȃ邩�ǂ����� ������ �Ƃ��낾�B�v
					�� �����̗�́u����/�Ƃ��낾�v�ɌW��ƈ���

			   �u���l�� ������̂� �D���ɂȂ� �����ł��v
					�� ���̗�͞B�������C�����㐳����

			   ����p��)
			   �u���ꂪ �l�u�o�� ������Ȃ� �����ł��傤�v
			   �u�` ����Ȃ� ���� �^���B�v
			   �u�r�U�� ���̂� ��ς� ����B�v
			   �u�g�� ���܂�̂� ������ꂻ���ɂȂ� �_�s�����B�v
			   �u���܂� �𗧂Ƃ� �v���Ȃ� �_�����B�v
			   �u�ǂ� �܂荇������ ��莋����Ă��� �@���B�v
			   �u�F�߂��邩�ǂ����� ����ꂽ �ٔ��Łv

			   ����聦
			   �u���̐푈�v���` �̂悤�ȏꍇ���p���Ƃ݂Ȃ����̂����
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

	/* �p���̏ꍇ�C�ŏI�I�ɖ��i,�K�i,���i,�j�i,�A�̏C���ɑ΂���
	   �K�i,���i,�j�i�̃X���b�g�������_����^���� */

	if (pred_p) {

		/* �A�̏C���̏ꍇ�C�W�悪
		   �E�`������,�����I����
		   �E�u�\��v,�u�����݁v�Ȃ�
		   �łȂ���Έ�̊i�v�f�ƍl���� */

		if (check_feature(g_ptr->f, "�W:�A�i")) {
		if (check_feature(sp->bnst_data[dpnd.head[i]].f, outer_rel) ||	
			check_feature(sp->bnst_data[dpnd.head[i]].f, rel_out_of_rule)) {
			rentai = 0;
			one_score += 10;	/* �O�̊֌W�Ȃ炱���ŉ��_ */
		} else {
			rentai = 1; /* ����ȊO�Ȃ��ŋ󂫃X���b�g���`�F�b�N */
		}
		} else {
		rentai = 0;
		}

		/* �󂢂Ă���K�i,���i,�j�i,�K�Q */

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
		/* �j�i�͓����ŘA�̏C���̏ꍇ�����l���C�܂�A��
		   �C���Ɋ��蓖�Ă邾���ŁC���i�̃X���b�g�Ƃ͂��Ȃ� */
		}
		if ((g_ptr->SCASE_code[case2num(ga_2_kaku)]
		 - scase_check[case2num(ga_2_kaku)]) == 1) {
		vacant_slot_num ++;
		}

		/* �󂫃X���b�g�������A�̏C���C���i�ɃX�R�A��^���� */

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
		bnst_to_tag_tree(sp); /* �^�O�P�ʂ̖؂� */
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

	/* �����܂ŉ�͂��I�������]���֐������ */

	if (dpnd.pos == -1) {
	/* ���i�]��: �O�̕��߂̌W��󂯂ɏ]���ꍇ */
	for (i = 0; i < sp->Bnst_num -1; i++)
		if (dpnd.head[i] < 0) {
		/* ���肦�Ȃ��W��� */
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

	/* (�O�̌W��ɂ��)����������̐ݒ� (dpnd.mask �� 0 �Ȃ�W��Ȃ�) */

	if (dpnd.pos < sp->Bnst_num - 2)
	for (i = dpnd.pos + 2; i < dpnd.head[dpnd.pos + 1]; i++)
		dpnd.mask[i] = 0;
	
	/* ����\���̃L�[����, ��������̕���<I>
	   (���łɍs��ꂽ����\����͂̌��ʂ��}�[�N���邾��) */

	for (i = dpnd.pos + 1; i < sp->Bnst_num; i++) {
	if (Mask_matrix[dpnd.pos][i] == 2) {
		dpnd.head[dpnd.pos] = i;
		dpnd.type[dpnd.pos] = 'P';
		/* �`�F�b�N�p */
		/* ����̏ꍇ�͈�ӂɌ��܂��Ă���̂ŁA����������͈̂Ӗ����Ȃ� */
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

	/* �O�̕��߂̌W��󂯂ɏ]���ꍇ  ��) �u�`�哝�͈̂���C�`�v */

	if ((cp = check_feature(b_ptr->f, modif_mu_kaku)) != NULL) {
		art_sscanf_1(cp, "%*[^:]:%*[^:]:%d", &(dpnd.head[dpnd.pos]));
		dpnd.type[dpnd.pos] = 'D';
		dpnd.dflt[dpnd.pos] = 0;
	dpnd.check[dpnd.pos].num = 1;
		decide_dpnd(sp, dpnd);
		return;
	}

	/* �ʏ�̌W��󂯉�� */

	/* �W���̌��𒲂ׂ� */
	
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

		/* �o���A�̃`�F�b�N */
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

	/* ���ۂɌ��������Ă���(���̊֐��̍ċA�I�Ăяo��) */

	if (count) {

	/* preference �͈�ԋ߂�:1, ��Ԗ�:2, �Ō�:-1
	   default_pos �͈�ԋ߂�:1, ��Ԗ�:2, �Ō�:count �ɕύX */

	default_pos = (b_ptr->dpnd_rule->preference == -1) ?
		count: b_ptr->dpnd_rule->preference;

	dpnd.check[dpnd.pos].num = count;	/* ��␔ */
	dpnd.check[dpnd.pos].def = default_pos; /* �f�t�H���g�̈ʒu */
	for (i = 0; i < count; i++) {
		dpnd.check[dpnd.pos].pos[i] = possibilities[i];
	}

	/* ��ӂɌ��肷��ꍇ */

	if (b_ptr->dpnd_rule->barrier.fp[0] == NULL ||	
		b_ptr->dpnd_rule->decide) {
		if (default_pos <= count) {
		dpnd.head[dpnd.pos] = possibilities[default_pos - 1];
		} else {
		dpnd.head[dpnd.pos] = possibilities[count - 1];
		/* default_pos �� 2 �Ȃ̂ɁCcount�� 1 �����Ȃ��ꍇ */
		}
		dpnd.type[dpnd.pos] = (char)Dpnd_matrix[dpnd.pos][dpnd.head[dpnd.pos]];
		dpnd.dflt[dpnd.pos] = 0;
		decide_dpnd(sp, dpnd);
	}	

	/* ���ׂẲ\��������o���ꍇ */
	/* �ߊԂ̌W��󂯂̏ꍇ�͈�ӂɌ��߂�ׂ� */

	else {
		for (i = 0; i < count; i++) {
		dpnd.head[dpnd.pos] = possibilities[i];
		dpnd.type[dpnd.pos] = (char)Dpnd_matrix[dpnd.pos][dpnd.head[dpnd.pos]];
		dpnd.dflt[dpnd.pos] = abs(default_pos - 1 - i);
		decide_dpnd(sp, dpnd);
		}
	}
	}	

	/* �W��悪�Ȃ��ꍇ
	   ����������Ƀ}�X�N����Ă��Ȃ���΁C�����ɌW��Ƃ��� */

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
	/* ��͍�: �\���͗^����ꂽ����1�̂� */
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

	sp->Best_mgr->score = -10000; /* �X�R�A�́u���傫���v���ɓ��ꊷ����̂ŁC
					�����l�͏\�����������Ă��� */
	sp->Best_mgr->dflt = 0;
	sp->Best_mgr->ID = -1;
	Possibility = 0;
	dpndID = 0;

	/* �W���Ԃ̏����� */

	for (i = 0; i < sp->Bnst_num; i++) {
	dpnd.head[i] = -1;
	dpnd.dflt[i] = 0;
	dpnd.mask[i] = 1;
	memset(&(dpnd.check[i]), 0, sizeof(CHECK_DATA));
	dpnd.check[i].num = -1;
	dpnd.f[i] = NULL;
	}
	dpnd.pos = sp->Bnst_num - 1;

	/* �ˑ��\����� */

	decide_dpnd(sp, dpnd);

	/* �\�������̏��� */

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

	/* �e���߂��ƂɃ`�F�b�N�p�� feature ��^���� */
	for (i = 0; i < sp->Bnst_num; i++)
	if (tm->dpnd.check[i].num != -1) {
		/* �W�葤 -> �W��� */
		art_strcpy(buffer, DATA_LEN, candidate);
		for (j = 0; j < tm->dpnd.check[i].num; j++) {
		/* ��₽�� */
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
	 *	�v���O�����ɂ�郁���ւ̏�������
	 */

	/* �ɘa�������ɋL�^����ꍇ
	int i;

	for (i = 0; i < sp->Bnst_num - 1; i++) {
	if (sp->Best_mgr->dpnd.type[i] == 'd') {
		strcat(PM_Memo, " �ɘad");
		sprintf(PM_Memo+strlen(PM_Memo), "(%d)", i);
	} else if (sp->Best_mgr->dpnd.type[i] == 'R') {
		strcat(PM_Memo, " �ɘaR");
		sprintf(PM_Memo+strlen(PM_Memo), "(%d)", i);
	}
	}
	*/

	/* �����W��󂯂������ɋL�^����ꍇ

	for (i = 0; i < sp->Bnst_num - 1; i++) {
	if (sp->Best_mgr->dpnd.head[i] > i + 3 &&
		!check_feature(sp->bnst_data[i].f, "�n") &&
		!check_feature(sp->bnst_data[i].f, "�Ǔ_") &&
		!check_feature(sp->bnst_data[i].f, "�p��") &&
		!check_feature(sp->bnst_data[i].f, "�W:�K�i") &&
		!check_feature(sp->bnst_data[i].f, "�p��:��") &&
		!check_feature(sp->bnst_data[i].f, "���L") &&
		!check_feature(sp->bnst_data[i+1].f, "���ʎn")) {
		strcat(PM_Memo, " ���W");
		sprintf(PM_Memo+strlen(PM_Memo), "(%d)", i);
	}
	}
	*/
}

/*====================================================================
							   END
====================================================================*/
