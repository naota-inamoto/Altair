/*====================================================================

			   ����\���Ԃ̊֌W

											   S.Kurohashi 91.10.17
											   S.Kurohashi 93. 5.31

	$Id: para_relation.c,v 1.13 2003/04/15 09:09:40 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

int 	para_rel_matrix[PARA_MAX][PARA_MAX];

#ifdef	_WINDOWS
static char **RESULT = NULL;
static char* hei_kekku = NULL;
static char* hei_bunsetsu = NULL;
static char* RESULT_2[] = {
	"�d�Ȃ�Ȃ�", "�����d�Ȃ�", "�O�ŏd�Ȃ�", "��ŏd�Ȃ�",  "�d��",
	"�O���̏C��", "�܂܂��O", "�܂܂���", "���", NULL };
static char hei_kekku_2[] = "�����吔:%d";
static char hei_bunsetsu_2[] = "�������ߐ�:%d";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	RESULT = toStringEUCs(RESULT_2);
	hei_kekku = toStringEUC(hei_kekku_2);
	hei_bunsetsu = toStringEUC(hei_bunsetsu_2);
#endif
}
void final_str_para_relation() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	freeStrings(RESULT); RESULT = NULL;
	free(hei_kekku); hei_kekku = NULL;
	free(hei_bunsetsu); hei_bunsetsu = NULL;
#endif
}
#ifdef	_LINUX
static const char *RESULT[] = {
	"�d�Ȃ�Ȃ�", "�����d�Ȃ�", "�O�ŏd�Ȃ�", "��ŏd�Ȃ�",  "�d��",
	"�O���̏C��", "�܂܂��O", "�܂܂���", "���", NULL};
static const char hei_kekku[] = "�����吔:%d";
static const char hei_bunsetsu[] = "�������ߐ�:%d";
#endif

static int rel_matrix_normal[4][4] = {
	{REL_BIT, REL_POS, REL_BAD, REL_IN2},
	{REL_PRE, REL_PAR, REL_BAD, REL_IN2},
	{REL_REV, REL_REV, REL_BAD, REL_BAD},
	{REL_IN1, REL_IN1, REL_BAD, REL_BAD}
};
static int rel_matrix_strong[4][4] = {
	{REL_BAD, REL_POS, REL_BAD, REL_IN2}, /* (0,0) -> BAD */
	{REL_PRE, REL_PAR, REL_BAD, REL_IN2},
	{REL_REV, REL_REV, REL_BAD, REL_BAD},
	{REL_IN1, REL_IN1, REL_BAD, REL_BAD}
};

/*	
   strong��(0,1)��BAD�ɂ��Ă������CPOS�ł悢�ᕶ���������̂ŏC�������D

   �ᕶ) ���W���񎩐g�͉p���Ђ������A�C���h�n�̓񖜌ܐ�l�̂�����܁A
   �Z��l�̓C���h���Ђ��p���Ђ��Ȃ��A���`�Z���Ƃ��Ă̎��i�����Ȃ��B
*/

/*==================================================================*/
void print_two_para_relation(SENTENCE_DATA *sp, int p_num1, int p_num2)
/*==================================================================*/
{
	/* ����\���Ԃ̊֌W�̕\�� */

	int a1, a2, a3, b1, b2, b3;
	PARA_DATA *ptr1, *ptr2;

	init_str();

	ptr1 = &(sp->para_data[p_num1]);
	a1 = ptr1->max_path[0];
	a2 = ptr1->key_pos;
	a3 = ptr1->jend_pos;

	ptr2 = &(sp->para_data[p_num2]);
	b1 = ptr2->max_path[0];
	b2 = ptr2->key_pos;
	b3 = ptr2->jend_pos;

	fprintf(Outfp, "%-10s ==> ", RESULT[para_rel_matrix[p_num1][p_num2]]);

	if (a1 != a2)
	print_bnst(&(sp->bnst_data[a1]), NULL);
	fputc('(', Outfp);
	print_bnst(&(sp->bnst_data[a2]), NULL);
	fputc(')', Outfp);
	print_bnst(&(sp->bnst_data[a3]), NULL);

	fprintf(Outfp, " <=> ");

	if (b1 != b2)
	print_bnst(&(sp->bnst_data[b1]), NULL);
	fputc('(', Outfp);
	print_bnst(&(sp->bnst_data[b2]), NULL);
	fputc(')', Outfp);
	print_bnst(&(sp->bnst_data[b3]), NULL);
	fputc('\n', Outfp);
}

/*==================================================================*/
		  void init_para_manager(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	sp->Para_M_num = 0;

	for (i = 0; i < sp->Para_num; i++) {
	sp->para_manager[i].para_num = 0;
	sp->para_manager[i].part_num = 0;
	sp->para_manager[i].parent = NULL;
	sp->para_manager[i].child_num = 0;

	sp->para_data[i].manager_ptr = NULL;
	}
}

/*==================================================================*/
	int para_location(SENTENCE_DATA *sp, int pre_num, int pos_num)
/*==================================================================*/
{
	/* ����\���Ԃ̊֌W�̌��� */

	int a1, a2, a3, b1, b2, b3;
	int rel_pre, rel_pos;

	a1 = sp->para_data[pre_num].max_path[0];
	a2 = sp->para_data[pre_num].key_pos;
	a3 = sp->para_data[pre_num].jend_pos;
	b1 = sp->para_data[pos_num].max_path[0];
	b2 = sp->para_data[pos_num].key_pos;
	b3 = sp->para_data[pos_num].jend_pos;

	if (a3 < b1) return REL_NOT;
	
	if ((a2+1) < b1)		rel_pre = 0;
	else if ((a2+1) == b1)	rel_pre = 1;
	else if (a1 < b1)		rel_pre = 2;
	else					rel_pre = 3;

	if (a3 < b2)			rel_pos = 0;
	else if (a3 == b2)		rel_pos = 1;
	else if (a3 < b3)		rel_pos = 2;
	else					rel_pos = 3;

	if (sp->para_data[pos_num].status == 's')	
	return rel_matrix_strong[rel_pre][rel_pos];
	else
	return rel_matrix_normal[rel_pre][rel_pos];
}

/*==================================================================*/
   int para_brother_p(SENTENCE_DATA *sp, int pre_num, int pos_num)
/*==================================================================*/
{
	/* REL_POS -> REL_PAR �ɕϊ��������
	   �O�̕���\����post-conjunct�ƌ�̕���\����pre-conjunct��
	   �傫��������قǂ����Ȃ��i�S�F�R�ȉ��j
	   */

	int pre_length, pos_length;

	pre_length = sp->para_data[pre_num].jend_pos - sp->para_data[pre_num].key_pos;
	pos_length = sp->para_data[pos_num].key_pos - sp->para_data[pos_num].max_path[0] + 1;
	
	if (pre_length * 3 <= pos_length * 4) return TRUE;
	else return FALSE;
}

/*==================================================================*/
 void delete_child(PARA_MANAGER *parent_ptr, PARA_MANAGER *child_ptr)
/*==================================================================*/
{
	int i, j;

	for (i = 0; i < parent_ptr->child_num; i++)
	if (parent_ptr->child[i] == child_ptr) {
		for (j = i; j < parent_ptr->child_num - 1; j++)
		parent_ptr->child[j] = parent_ptr->child[j+1];
		parent_ptr->child_num -= 1;
		break;
	}
}

/*==================================================================*/
  int set_parent(PARA_MANAGER *parent_ptr, PARA_MANAGER *child_ptr)
/*==================================================================*/
{
	int i, j, i_num, j_num;

	if (child_ptr->parent) {
	if (child_ptr->parent == parent_ptr) return TRUE;

	for (i = 0; i < child_ptr->parent->para_num; i++) {
		i_num = child_ptr->parent->para_data_num[i];
		for (j = 0; j < parent_ptr->para_num; j++) {
		j_num = parent_ptr->para_data_num[j];

		/* ���̐e�����ڂ̐e */

		if ((i_num < j_num &&
			 (para_rel_matrix[i_num][j_num] == REL_BIT ||
			  para_rel_matrix[i_num][j_num] == REL_PRE ||
			  para_rel_matrix[i_num][j_num] == REL_REV ||
			  para_rel_matrix[i_num][j_num] == REL_IN1)) ||
			(j_num < i_num &&
			 (para_rel_matrix[j_num][i_num] == REL_POS ||
			  para_rel_matrix[j_num][i_num] == REL_IN2)))
		  return TRUE;

		/* �V�����e�����ڂ̐e */

		else if ((i_num < j_num &&
			  (para_rel_matrix[i_num][j_num] == REL_POS ||
			   para_rel_matrix[i_num][j_num] == REL_IN2)) ||
			 (j_num < i_num &&
			  (para_rel_matrix[j_num][i_num] == REL_BIT ||
			   para_rel_matrix[j_num][i_num] == REL_PRE ||
			   para_rel_matrix[j_num][i_num] == REL_REV ||
			   para_rel_matrix[j_num][i_num] == REL_IN1))) {
			delete_child(child_ptr->parent, child_ptr);
			child_ptr->parent = parent_ptr;
			parent_ptr->child[parent_ptr->child_num++] = child_ptr;
			if (parent_ptr->child_num >= PARA_PART_MAX) {
			fprintf(stderr, ";; Too many para!\n");
			return FALSE;
			}
			return TRUE;
		}
		}
	}

#ifdef KNP_DEBUG
	/* ���̐e�ƐV�����e�Ɋ֌W���Ȃ� */
	fprintf(stderr, ";; Invalid relation !!\n");
#endif
	
	} else {
	child_ptr->parent = parent_ptr;
	parent_ptr->child[parent_ptr->child_num++] = child_ptr;
	if (parent_ptr->child_num >= PARA_PART_MAX) {
		fprintf(stderr, ";; Too many para!\n");
		return FALSE;
	}
	}
	return TRUE;
}

/*==================================================================*/
		  void para_revise_scope(PARA_MANAGER *ptr)
/*==================================================================*/
{
	int i;
	PARA_MANAGER *child_ptr;

	if (ptr->child_num) {

	/* �q���̏��� */

	for (i = 0; i < ptr->child_num; i++)
		para_revise_scope(ptr->child[i]);


	/* �����̏C�� */

	if (ptr->child[0]->start[0] < ptr->start[0])
		ptr->start[0] = ptr->child[0]->start[0];
	

	/* �E���̏C�� */
	
	child_ptr = ptr->child[ptr->child_num-1];
	if (ptr->end[ptr->part_num-1] < child_ptr->end[child_ptr->part_num-1])
		ptr->end[ptr->part_num-1] = child_ptr->end[child_ptr->part_num-1];
	}
}

/*==================================================================*/
		 int detect_para_relation(SENTENCE_DATA *sp)
/*==================================================================*/
{
	/* ����\���Ԃ̊֌W�̐��� */

	int i, j, k, flag;
	PARA_MANAGER *m_ptr, *m_ptr1, *m_ptr2;
	char buffer1[64], buffer2[64];

	init_str();

	/* �ʒu�֌W�̌���C���̏C�� */

	for (i = 0; i < sp->Para_num; i++) {
	if (sp->para_data[i].status == 'x') continue;
		for (j = i+1; j < sp->Para_num; j++) {
		if (sp->para_data[j].status == 'x') continue;
		if ((para_rel_matrix[i][j] = para_location(sp, i, j)) == REL_BAD) {
		if (OptDisplay == OPT_DEBUG)
			print_two_para_relation(sp, i, j);
		revise_para_rel(sp, i, j);
		return FALSE;
		}
	}
	}

	init_para_manager(sp);

	/* REL_POS�ŏd�Ȃ�̊������傫���ꍇREL_PAR�ɕύX */

	for (i = 0; i < sp->Para_num; i++) {
	if (sp->para_data[i].status == 'x') continue;
	for (j = 0; j < sp->Para_num; j++) {
		if (sp->para_data[j].status == 'x') continue;
		if (para_rel_matrix[i][j] == REL_POS &&
		para_brother_p(sp, i, j) == 1 /*TRUE*/) {
		para_rel_matrix[i][j] = REL_PAR;
		}
	}
	}

	/* ����REL_POS�C�E��REL_PRE�̏ꍇ�C���̊Ԃ�REL_REV�ɕύX */

	for (i = 1; i < sp->Para_num-1; i++) {
	if (sp->para_data[i].status == 'x') continue;
	for (j = 0; j < i; j++) {
		if (sp->para_data[j].status == 'x') continue;
		if (para_rel_matrix[j][i] == REL_POS) {
		for (k = i+1; k < sp->Para_num; k++) {
			if (sp->para_data[k].status == 'x') continue;
			if (para_rel_matrix[i][k] == REL_PRE) {
			para_rel_matrix[j][k] = REL_REV;
			}
		}
		}
	}
	}

	/* �Z��֌W�̂܂Ƃ߁CMANAGER�ɂ��Ǘ� */

	for (i = 0; i < sp->Para_num; i++) {
	if (sp->para_data[i].status == 'x') continue;
	if (sp->para_data[i].manager_ptr) {
		m_ptr = sp->para_data[i].manager_ptr;
	} else {
		m_ptr = &sp->para_manager[sp->Para_M_num++];
		sp->para_data[i].manager_ptr = m_ptr;
		m_ptr->para_data_num[m_ptr->para_num++] = i;
		if (m_ptr->para_num >= PARA_PART_MAX) {
		fprintf(stderr, ";; Too many para (%s)!\n", sp->Comment ? sp->Comment : "");
		return FALSE;
		}
		m_ptr->start[m_ptr->part_num] = sp->para_data[i].max_path[0];
		m_ptr->end[m_ptr->part_num++] = sp->para_data[i].key_pos;
		m_ptr->start[m_ptr->part_num] = sp->para_data[i].key_pos+1;
		m_ptr->end[m_ptr->part_num++] = sp->para_data[i].jend_pos;
	}		
		for (j = i+1; j < sp->Para_num; j++) {
		if (sp->para_data[j].status == 'x') continue;
		switch (para_rel_matrix[i][j]) {
		  case REL_PAR:
		sp->para_data[j].manager_ptr = m_ptr;
		m_ptr->para_data_num[m_ptr->para_num++] = j;
		if (m_ptr->para_num >= PARA_PART_MAX) {
			fprintf(stderr, ";; Too many para (%s)!\n", sp->Comment ? sp->Comment : "");
			return FALSE;
		}
		m_ptr->start[m_ptr->part_num] = sp->para_data[j].key_pos+1;
		m_ptr->end[m_ptr->part_num++] = sp->para_data[j].jend_pos;
		break;
		  default:
		break;
		}
	}
	}
	
	/* �e�q�֌W�̂܂Ƃ� m_ptr1���q�Cm_ptr2���e�̎��ɏ��� */

	for (i = 0; i < sp->Para_num; i++) {
	if (sp->para_data[i].status == 'x') continue;
	m_ptr1 = sp->para_data[i].manager_ptr;
		for (j = 0; j < sp->Para_num; j++) {
		if (sp->para_data[j].status == 'x') continue;
		m_ptr2 = sp->para_data[j].manager_ptr;
		if ((i < j &&
		 (para_rel_matrix[i][j] == REL_BIT ||
		  para_rel_matrix[i][j] == REL_PRE ||
		  para_rel_matrix[i][j] == REL_REV ||
		  para_rel_matrix[i][j] == REL_IN1)) ||
		(j < i &&
		 (para_rel_matrix[j][i] == REL_POS ||
		  para_rel_matrix[j][i] == REL_IN2))) {
		if (set_parent(m_ptr2, m_ptr1) == FALSE) {
			return FALSE;
		}
		}
	}
	}

	/* �͈͂̏C�� */

	for (i = 0; i < sp->Para_M_num; i++)
	if (sp->para_manager[i].parent == NULL)
		para_revise_scope(&sp->para_manager[i]);	

	/* ������̃}�[�N */

	for (i = 0; i < sp->Para_M_num; i++) {
	flag = TRUE;
	for (j = 0; j < sp->para_manager[i].para_num; j++) {
		if (sp->para_data[sp->para_manager[i].para_data_num[j]].status != 's') {
		flag = FALSE;
		break;
		}
	}
	sp->para_manager[i].status = (flag == 1 /*TRUE*/) ? 's' : 'w';
	}

	/* �����͌��ʂ�feature�� */

	for (i = 0; i < sp->Para_M_num; i++) {
	for (j = 0; j < sp->para_manager[i].part_num-1; j++) {
		art_sprintf_1(buffer1, 64, hei_kekku, sp->para_manager[i].part_num);
		art_sprintf_1(buffer2, 64, hei_bunsetsu,	
			sp->para_manager[i].end[1] - sp->para_manager[i].start[1] + 1);
		assign_cfeature(&(sp->bnst_data[sp->para_manager[i].end[j]].f), buffer1);
		assign_cfeature(&(sp->bnst_data[sp->para_manager[i].end[j]].f), buffer2);
	}
	}

	return TRUE;
}

/*====================================================================
							   END
====================================================================*/
