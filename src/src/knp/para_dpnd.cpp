/*====================================================================

	   ����\�������̈ˑ��\���̃`�F�b�N�C�ˑ��\���s��̃}�X�N

											   S.Kurohashi 93. 5.25
											   S.Kurohashi 93. 5.31

	$Id: para_dpnd.c,v 1.10.2.1 2005/08/21 11:03:19 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

#define PARA_INCOMP_TH	3.0

int D_check_array[BNST_MAX];
int D_found_array[BNST_MAX];

#ifdef	_WINDOWS
static char* dokuten = NULL;
static char* keidai = NULL;
static char* kakari_rentai = NULL;
static char* kakari_de_kaku = NULL;
static char* wa = NULL;
static char* yougen = NULL;
static char* taigen = NULL;
static char dokuten_2[] = "�Ǔ_";
static char keidai_2[] = "���";
static char kakari_rentai_2[] = "�W:���i�A��";
static char kakari_de_kaku_2[] = "�W:�f�i";
static char wa_2[] = "�n";
static char yougen_2[] = "�p��";
static char taigen_2[] = "�̌�";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	dokuten = toStringEUC(dokuten_2);
	keidai = toStringEUC(keidai_2);
	kakari_rentai = toStringEUC(kakari_rentai_2);
	kakari_de_kaku = toStringEUC(kakari_de_kaku_2);
	wa = toStringEUC(wa_2);
	yougen = toStringEUC(yougen_2);
	taigen = toStringEUC(taigen_2);
#endif
}
void final_str_para_dpnd() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(dokuten); dokuten = NULL;
	free(keidai); keidai = NULL;
	free(kakari_rentai); kakari_rentai = NULL;
	free(kakari_de_kaku); kakari_de_kaku = NULL;
	free(wa); wa = NULL;
	free(yougen); yougen = NULL;
	free(taigen); taigen = NULL;
#endif
}
#ifdef	_LINUX
static char dokuten[] = "�Ǔ_";
static char keidai[] = "���";
static char kakari_rentai[] = "�W:���i�A��";
static char kakari_de_kaku[] = "�W:�f�i";
static char wa[] = "�n";
static char yougen[] = "�p��";
static char taigen[] = "�̌�";
#endif

/*==================================================================*/
	  int check_stop_extend(SENTENCE_DATA *sp, int num)
/*==================================================================*/
{
	init_str();

	if ((check_feature(sp->bnst_data[num].f, dokuten) &&
	 !check_feature(sp->bnst_data[num].f, kakari_rentai)) ||
	check_feature(sp->bnst_data[num].f, keidai) ||
	(check_feature(sp->bnst_data[num].f, kakari_de_kaku) &&
	 check_feature(sp->bnst_data[num].f, wa)))
	return TRUE;
	else
	return FALSE;
}

/*==================================================================*/
	  int para_extend_p(SENTENCE_DATA *sp, PARA_MANAGER *m_ptr)
/*==================================================================*/
{
	/* ����\���O������������ : ������ or �p�����܂� */

	int i;

	init_str();

	if (sp->para_data[m_ptr->para_data_num[0]].status == 's')	
	return TRUE;

	for (i = m_ptr->start[0]; i <= m_ptr->end[0]; i++)
	if (check_feature(sp->bnst_data[i].f, yougen))
		return TRUE;
	return FALSE;
}

/*==================================================================*/
		   int parent_range(PARA_MANAGER *m_ptr)
/*==================================================================*/
{
	/* �e�̕���\��������ꍇ�͈͉̔����̐���
			�擪�����Ɋ܂܂��ꍇ : �����Ȃ�
		����ȊO�Ɋ܂܂��ꍇ : ���O�̃L�[�̈ʒu */

	int i;

	if (m_ptr->parent == NULL) return 0;
	
	for (i = m_ptr->parent->part_num - 1; i > 0; i--)	
	  if (m_ptr->parent->start[i] <= m_ptr->start[0])
	return m_ptr->parent->start[i];
	
	return 0;
}

/*==================================================================*/
	int _check_para_d_struct(SENTENCE_DATA *sp, int str, int end,
				 int extend_p, int limit, int *s_p)
/*==================================================================*/
{
	int i, j, k, found, success_p = TRUE;
	int hikousa_array[BNST_MAX];

	D_found_array[end] = TRUE; /* I�}�[�N�𖄂߂鎞�̕֋X */

	for (k = 0; k <= end; k++) hikousa_array[k] = 1;
	/* ���������ׂ�̂�,���̏�������str����łȂ�0���� */

	/* ����\�������̈ˑ��\���𒲂ׂ�
	   (�e���߂������Ƃ��߂����߂ɂ�����Ɖ���) */

	for (i = end - 1; i >= str; i--) {
	if (D_check_array[i] == 1 /*TRUE*/) {
		D_found_array[i] = TRUE;
	} else {
		found = FALSE;
		for (j = i + 1; j <= end; j++)
		if (Mask_matrix[i][j] &&
			Quote_matrix[i][j] &&
			Dpnd_matrix[i][j] &&
			hikousa_array[j]) {
			D_found_array[i] = TRUE;
			for (k = i + 1; k < j; k++) hikousa_array[k] = 0;
			found = TRUE;
			break;
		}
		if (found == FALSE) {
		D_found_array[i] = FALSE;
		/* revise_para_kakari����̌ďo(s_p == NULL)�͕\���Ȃ� */
		if (OptDisplay == OPT_DEBUG && s_p) {
			fprintf(Outfp, ";; Cannot find a head for bunsetsu <");
			print_bnst(sp->bnst_data + i, NULL);
			fprintf(Outfp, ">.\n");
		}
		success_p = FALSE;
		for (k = i + 1; k <= end; k++) hikousa_array[k] = 0;
		}
	}
	}	

	/* ����\���O���̉����\�͈͂𒲂ׂ� */
	
	if (extend_p == 1 /*TRUE*/ && success_p == 1 /*TRUE*/) {
	for (i = str - 1;; i--) {
		if (i < limit || check_stop_extend(sp, i) == 1 /*TRUE*/) {
		*s_p = i + 1;
		break;
		} else if (D_check_array[i] == 1 /*TRUE*/) {
		D_found_array[i] = TRUE;
		} else {
		found = FALSE;
		for (j = i + 1; j <= end; j++)

			/*	
			   '< end' �� '<= end' ����, ���񖖔����������镶�߂�
			   �W���ƂȂ蓾�邩�ǂ������ς��D

			   �����̌���,'<= end'�Ƃ�������S�̂̐��x�͂悢�D

			   ��̗�) 950101071-030, 950101169-002, 950101074-019
			*/

			if (Mask_matrix[i][j] &&	
			Quote_matrix[i][j] &&	
			Dpnd_matrix[i][j] &&	
			hikousa_array[j]) {
			D_found_array[i] = TRUE;
			for (k = i + 1; k < j; k++) hikousa_array[k] = 0;
			found = TRUE;
			break;	/* 96/01/22�܂łȂ��Ȃ���?? */
			}
		if (found == FALSE) {
			*s_p = i + 1;
			break;
		}
		}
	}
	}

	return success_p;
}

/*==================================================================*/
int check_error_state(SENTENCE_DATA * /*sp*/, PARA_MANAGER *m_ptr, int error[])
/*==================================================================*/
{
	/* �G���[��Ԃ̃`�F�b�N :	

		   �C���\�ȏꍇ : 2�̂����̕�������Ȃ����\��
				3�ȏ�̕�������Ȃ����\���Ō�肪�擪
				3�ȏ�̕�������Ȃ����\���Ō�肪����

	   ����ȊO�̏ꍇ�͏C���f�O (return -1) */

	int i;

	if (m_ptr->part_num == 2) {
	return m_ptr->para_data_num[0];
	}	
	else if (error[0] == 1 /*TRUE*/) {
	for (i = 1; i < m_ptr->part_num; i++)
		if (error[i] == 1 /*TRUE*/) {
		fprintf(Outfp,	
			";; Cannot revise invalid kakari struct in para!!\n");
		return -1;
		}
	return m_ptr->para_data_num[0];
	}	
	else if (error[m_ptr->part_num - 1] == 1 /*TRUE*/) {
	for (i = 0; i < m_ptr->part_num - 1; i++)
		if (error[i] == 1 /*TRUE*/) {
		fprintf(Outfp,	
			";; Cannot revise invalid kakari struct in para!!\n");
		return -1;
		}
	return m_ptr->para_data_num[m_ptr->para_num-1];
	}
	else
	return -1;
}

/*==================================================================*/
   int check_para_d_struct(SENTENCE_DATA *sp, PARA_MANAGER *m_ptr)
/*==================================================================*/
{
	int i, j, k;
	int noun_flag;
	int start_pos;
	int invalid_flag = FALSE;
	int no_more_error, no_more_error_here;
	int error_check[PARA_PART_MAX], error_pos[PARA_PART_MAX];
	int revised_p_num;
	
	init_str();

	for (i = 0; i < m_ptr->child_num; i++)	/* �q���̍ċA���� */
	if (check_para_d_struct(sp, m_ptr->child[i]) == FALSE)
		return FALSE;
	
	/* �̌����߂̓������B���Ȃ��̂��C����\����͂Ŗ��m�ɂȂ�ꍇ�̏���
	   ----------------------------------------------------------------
	   ��) �u���Ԃ���͎G���̕ҏW�ҁA�b���q����͎ԃ��[�J�[�̃R���p�j�I���B�v
			�u�ҏW�ҁv�����莌�ȗ��ł��邱�Ƃ��킩��

	   ��) �u���d�\�͂Ŗ�l�\�{�A�d�͗ʂł͖�Z�\�{�����₵�A�c�v
		   �u���N�͎l���ɓ���n���I���A�����ɎQ�@�I��������܂��B�v
			�u��l�\�{�v,�u����n���I���v���T�Ϗȗ��łȂ����Ƃ��킩��
		
	   �� �ua1 a2 a3, b1 b2 b3�v�ɂ����āC���ׂđ̌��̏ꍇ�C(a1 a3),(a2 a3)
	   �̌W��󂯂�(a1 b3),(a2 b3)�̌W��󂯂ŏ㏑������D
	   �������C�ׂɂ����W��悤�ȌW��󂯂̏ꍇ�ɕ���p������̂ŁC(a2 a3)
	   �̏㏑���́C(a2 a3)��(a2 b3)���قȂ�ꍇ�݂̂Ƃ���D

	   ��) �u�f��͓񌎃N�����N�C���A�\�����J�̗\��B�v


	   �� ���̕��@�ł͑O��conjunct�̌W��󂯂��C�����邱�Ƃ����ł����C
	   ���̗�͐����������Ȃ��D

	   ��) �u�O��͂V�킾�������A����͋�ԏ����Ő�ɂT�������c�v

	   �{���I�ɂ�,�V���ɐ�����feature��^���ČW��󂯂̉�͂���蒼��
	   �K�v������D���̏ꍇ�ɂ́C
	�E�Е������莌 �� ������������莌
		�E�Е����T�� �� �T�ς�������
	   �Ƃ����������s���΂悢���낤�D��������Ώ�̗����������͂����
	   �悤�ɂȂ�D
	*/

	if (m_ptr->status == 's') {
	noun_flag = 1;	
	for (k = 0; k < m_ptr->part_num; k++)
		if (!check_feature(sp->bnst_data[m_ptr->end[k]].f, taigen))
		noun_flag = 0;
	if (noun_flag) {
		for (k = 0; k < m_ptr->part_num - 1; k++)
			for (i = m_ptr->start[k]; i < m_ptr->end[k]; i ++)
			if (!(i == m_ptr->end[k] - 1 &&
			  Dpnd_matrix[i][m_ptr->end[k]] ==		
			  Dpnd_matrix[m_ptr->end[m_ptr->part_num - 1] - 1][m_ptr->end[m_ptr->part_num - 1]]))
			  Dpnd_matrix[i][m_ptr->end[k]] =		
			Dpnd_matrix[i][m_ptr->end[m_ptr->part_num - 1]];
	}
	}

	/* �ˑ��\����͉\���̃`�F�b�N */

	start_pos = m_ptr->start[0];
	for (k = 0; k < m_ptr->part_num; k++)
	  if (_check_para_d_struct(sp, m_ptr->start[k], m_ptr->end[k],
				   (k == 0) ? para_extend_p(sp, m_ptr): FALSE,	
				   (k == 0) ? parent_range(m_ptr): 0,
				   &start_pos) == FALSE) {
	  invalid_flag = TRUE;
	  error_check[k] = TRUE;
	  }
	
	/* �ˑ��\����͂Ɏ��s�����ꍇ

	   �u�ނ͓����Ŕ��S����,�ޏ��͑��Ŏ�w��,���͋��s�Ŋw�������Ă���v
	   �̂悤�ɏq��̏ȗ����ꂽ�܂ޕ���\�������o����D

	   �e�����̌W���̂Ȃ����߂̐���������,������ł����
	   ��L�̃^�C�v�̕���\���ƍl����D
	   (�W���̂Ȃ����߂̃^�C�v(�K�i�Ȃ�)��,�����ɑΉ�����Ƃ͌���Ȃ�
	   �̂Ő������Ȃ�)

	   �A���S���Y�� :	
	   �擪�����̊e�W���̂Ȃ����߂ɂ���
			�e�����ɌW��̂Ȃ�����Ɠ����^�C�v�̕��߂����邩�ǂ������ׂ�
	   */

	if (invalid_flag == 1 /*TRUE*/) {

	/* �v���� */
		/* if (sp->para_data[m_ptr->para_data_num[0]].pure_score < PARA_INCOMP_TH) { */

	if (m_ptr->status != 's') {
		if ((revised_p_num = check_error_state(sp, m_ptr, error_check)) 
		!= -1) {
		revise_para_kakari(sp, revised_p_num, D_found_array);
		return FALSE;
		} else {
		goto cannnot_revise;
		}
	}

	for (k = 0; k < m_ptr->part_num; k++)
	  error_pos[k] = m_ptr->end[k];
	while (1) {
		no_more_error = FALSE;
		no_more_error_here = FALSE;

		for (i = error_pos[0] - 1;	
		 D_found_array[i] == 1 /*TRUE*/ && start_pos <= i; i--);
		error_pos[0] = i;
		if (i == start_pos - 1) no_more_error = TRUE;

		for (k = 1; k < m_ptr->part_num; k++) {
		for (i = error_pos[k] - 1;	
			 D_found_array[i] == 1 /*TRUE*/ && m_ptr->start[k] <= i; i--);
		error_pos[k] = i;
		if (i == m_ptr->start[k] - 1) no_more_error_here = TRUE;

		/* �G���[�̑Ή������Ȃ�(��������łȂ�) */
		if (no_more_error != no_more_error_here) {
			if ((revised_p_num = check_error_state(sp, m_ptr, error_check))
			!= -1) {
			revise_para_kakari(sp, revised_p_num, D_found_array);
			return FALSE;
			} else {
			goto cannnot_revise;
			}
		}
		}
		if (no_more_error == 1 /*TRUE*/) break;
		else continue;
	}	
	}

 cannnot_revise:
	/* �`�F�b�N�ς݂̈� */
	for (k = start_pos; k < m_ptr->end[m_ptr->part_num-1]; k++)
	D_check_array[k] = TRUE;

	/* �擪��conjunct�̃}�X�N */
	k = 0;
	for (i = 0; i < start_pos; i++) 		   /* < start_pos */
	for (j = m_ptr->start[k]; j <= m_ptr->end[k]; j++)
		Mask_matrix[i][j] = 0;
	/* ���� ���� end�̏�̃J�o�[���Ȃ�
	for (i = start_pos; i < m_ptr->start[k]; i++)		end �̏�
	  Mask_matrix[i][m_ptr->end[k]] = 0;
	*/
	for (i = m_ptr->start[k]; i <= m_ptr->end[k]; i++)
	for (j = m_ptr->end[k] + 1; j < sp->Bnst_num; j++)
		Mask_matrix[i][j] = 0;

	if (sp->para_data[m_ptr->para_data_num[0]].status == 's') /* ������ ??? */
	for (i = 0; i < m_ptr->start[0]; i++)
		Mask_matrix[i][m_ptr->end[0]] = 0;
	
	/* ������conjunct�̃}�X�N */
	for (k = 1; k < m_ptr->part_num - 1; k++) {
	for (i = 0; i < m_ptr->start[k]; i++)
		for (j = m_ptr->start[k]; j <= m_ptr->end[k]; j++)
		Mask_matrix[i][j] = 0;
	for (i = m_ptr->start[k]; i <= m_ptr->end[k]; i++)
		for (j = m_ptr->end[k] + 1; j < sp->Bnst_num; j++)
		Mask_matrix[i][j] = 0;
	}
	
	/* ������conjunct�̃}�X�N */
	k = m_ptr->part_num - 1;
	for (i = 0; i < m_ptr->start[k]; i++)
	for (j = m_ptr->start[k]; j < m_ptr->end[k]; j++) /* < end */
		Mask_matrix[i][j] = 0;
	for (i = m_ptr->start[k]; i < m_ptr->end[k]; i++)	/* < end */
	for (j = m_ptr->end[k] + 1; j < sp->Bnst_num; j++)
		Mask_matrix[i][j] = 0;

	/* ����̌W��� */
	for (k = 0; k < m_ptr->part_num - 1; k++) {
	Mask_matrix[m_ptr->end[k]][m_ptr->end[k+1]] = 2;
	/*
	  Mask_matrix[m_ptr->end[k]][m_ptr->end[m_ptr->part_num - 1]] = 2;
	*/
	}
	if (invalid_flag == 1 /*TRUE*/)
	for (k = 0; k < m_ptr->part_num; k++)
		for (i = m_ptr->start[k]; i <= m_ptr->end[k]; i++)
		if (D_found_array[i] == FALSE) {
			Mask_matrix[i][m_ptr->end[k]] = 3;
			Mask_matrix[i][m_ptr->end[m_ptr->part_num - 1]] = 3;
		}

	/* ��������̏ꍇ,Mask_matrix�͍ŏ���head�ƍŌ��head��3�ɂ��Ă����D
	   �ŏ���head��dpnd.head������Ƃ��C�Ō��head��tree����鎞�Ɏg�� */

	return TRUE;
}

/*==================================================================*/
		   void init_mask_matrix(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j;

	for (i = 0; i < sp->Bnst_num; i++)
	for (j = 0; j < sp->Bnst_num; j++)
		Mask_matrix[i][j] = 1;
}

/*==================================================================*/
		  int check_dpnd_in_para(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	/* ������ */

	init_mask_matrix(sp);
	for (i = 0; i < sp->Bnst_num; i++)
	D_check_array[i] = FALSE;

	/* ����\�����̌W�󂯃`�F�b�N�C�}�X�N */
	
	for (i = 0; i < sp->Para_M_num; i++)
	if (sp->para_manager[i].parent == NULL)
		if (check_para_d_struct(sp, &sp->para_manager[i]) == FALSE)
		return FALSE;

	return TRUE;
}

/*====================================================================
							   END
====================================================================*/
