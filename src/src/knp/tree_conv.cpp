/*====================================================================

				  �؍\������

											   S.Kurohashi 92.10.17
											   S.Kurohashi 93. 5.31

	$Id: tree_conv.c,v 1.16.2.1 2005/07/25 09:17:27 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

#ifdef	_WINDOWS
static char* kakari_renyou = NULL;
static char* tag_unit_ignore = NULL;
static char* tag_unit = NULL;
static char kakari_renyou_2[] = "�W:�A�p";
static char tag_unit_ignore_2[] = "�^�O�P�ʎ󖳎�";
static char tag_unit_2[] = "�^�O�P�ʎ�";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	kakari_renyou = toStringEUC(kakari_renyou_2);
	tag_unit_ignore = toStringEUC(tag_unit_ignore_2);
	tag_unit = toStringEUC(tag_unit_2);
#endif
}
void final_str_tree_conv() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(kakari_renyou); kakari_renyou = NULL;
	free(tag_unit_ignore); tag_unit_ignore = NULL;
	free(tag_unit); tag_unit = NULL;
#endif
}
#ifdef	_LINUX
static char kakari_renyou[] = "�W:�A�p";
static char tag_unit_ignore[] = "�^�O�P�ʎ󖳎�";
static char tag_unit[] = "�^�O�P�ʎ�";
#endif

/*==================================================================*/
	   void init_bnst_tree_property(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	for (i = 0; i < BNST_MAX; i++) {
	sp->bnst_data[i].parent = NULL;
	sp->bnst_data[i].child[0] = NULL;
	sp->bnst_data[i].para_top_p = FALSE;
	sp->bnst_data[i].para_type = PARA_NIL;
	sp->bnst_data[i].to_para_p = FALSE;
	}
}

/*==================================================================*/
		void init_tag_tree_property(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	for (i = 0; i < TAG_MAX; i++) {
	sp->tag_data[i].parent = NULL;
	sp->tag_data[i].child[0] = NULL;
	sp->tag_data[i].para_top_p = FALSE;
	sp->tag_data[i].para_type = PARA_NIL;
	sp->tag_data[i].to_para_p = FALSE;
	}
}

/*==================================================================*/
BNST_DATA *t_add_node(BNST_DATA *parent, BNST_DATA *child, int pos)
/*==================================================================*/
{
	int i, child_num;

	for (child_num = 0; parent->child[child_num]; child_num++)
	;

	if (pos == -1) {
	parent->child[child_num] = child;
	parent->child[child_num + 1] = NULL;
	}
	else {
	for (i = child_num; i >= pos; i--)
		parent->child[i + 1] = parent->child[i];
	parent->child[pos] = child;
	}

	return parent;
}

/*==================================================================*/
BNST_DATA *t_attach_node(BNST_DATA *parent, BNST_DATA *child, int pos)
/*==================================================================*/
{
	child->parent = parent;

	return t_add_node(parent, child, pos);
}
/*==================================================================*/
	BNST_DATA *t_del_node(BNST_DATA *parent, BNST_DATA *child)
/*==================================================================*/
{
	int i, j;
	
	for (i = 0; parent->child[i]; i++) {
	if (parent->child[i] == child) {
		for (j = i; parent->child[j]; j++)
		  parent->child[j] = parent->child[j + 1];
		break;
	}
	}

	child->parent = NULL;
	
	return child;
}

/*==================================================================*/
		   int make_simple_tree(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j, k, child_num;
	int buffer[BNST_MAX];

	/* dpnd.head[i]��buffer[i]�ɃR�s�[���C3�ȏォ��Ȃ����\���ł�
	   �W���ׂ�head���疖����head�ɕύX����D
	   �܂���������̌W���𖖔���head�ɕύX����D*/

	for (i = 0; i < sp->Bnst_num - 1; i++)
	buffer[i] = sp->bnst_data[i].dpnd_head;

	for (i = 0; i < sp->Para_M_num; i++) {
	for (j = 0; j < sp->para_manager[i].part_num - 1; j++) {
		buffer[sp->para_manager[i].end[j]] =	
		sp->para_manager[i].end[sp->para_manager[i].part_num - 1];

		/*
		printf(">>> (%d,%d) %d -> %d\n", i, j, sp->para_manager[i].end[j],
		   sp->para_manager[i].end[sp->para_manager[i].part_num - 1]);
		*/

		for (k = sp->para_manager[i].start[j];
		 k <= sp->para_manager[i].end[j]; k++)
		if (Mask_matrix[k][sp->para_manager[i].end[j]] == 3)	
			buffer[k] = 
			sp->para_manager[i].end[sp->para_manager[i].part_num - 1];
	}
	}

	/* �ˑ��\���؍\�������N�t�� */

	for (j = sp->Bnst_num - 1; j >= 0; j--) {
	child_num = 0;
	for (i = j - 1; i >= 0; i--) {
		if (buffer[i] == j) {
		sp->bnst_data[j].child[child_num++] = sp->bnst_data + i;
		if (child_num >= PARA_PART_MAX) {
			child_num = PARA_PART_MAX-1;
			break;
		}
		sp->bnst_data[i].parent = sp->bnst_data + j;
		if (Mask_matrix[i][j] == 3) {
			sp->bnst_data[i].para_type = PARA_INCOMP;
		}
		/* PARA_NORMAL�͓W�J���ɃZ�b�g */
		}
	}
	sp->bnst_data[j].child[child_num] = NULL;
	}

	return TRUE;
}

/*==================================================================*/
BNST_DATA *strong_corr_node(SENTENCE_DATA *sp, PARA_DATA *p_ptr, BNST_DATA *b_ptr)
/*==================================================================*/
{
	int i;

	for (i = p_ptr->jend_pos - p_ptr->key_pos - 1; i >= 0; i--) {
	if (sp->bnst_data + p_ptr->max_path[i] == b_ptr)
	  return sp->bnst_data + p_ptr->key_pos + i + 1;
	}
	return NULL;
}

/*==================================================================*/;
   void strong_para_expand(SENTENCE_DATA *sp, PARA_MANAGER *m_ptr)
/*==================================================================*/
{
	int i, j, k;
	PARA_DATA *p_ptr, *pp_ptr;
	BNST_DATA *start_b_ptr, *b_ptr, *bb_ptr;

	/* ��������ɌW�镶�߂�W�J : �R�s�[�� */

	for (i = 0; i < m_ptr->child_num; i++)
	  strong_para_expand(sp, m_ptr->child[i]);


	p_ptr = sp->para_data + m_ptr->para_data_num[0];
	if (p_ptr->status == 's') {
	start_b_ptr = sp->bnst_data + m_ptr->start[0];
	for (i = m_ptr->start[0], b_ptr = start_b_ptr; i < m_ptr->end[0];	
		 i++, b_ptr++)
		for (j = 0; b_ptr->child[j]; j++)
		if (b_ptr->child[j] < start_b_ptr) {
			b_ptr->child[j]->to_para_p = TRUE;
			bb_ptr = b_ptr;
			for (k = 0, pp_ptr = p_ptr; k < m_ptr->para_num; k++, pp_ptr++)
			if ((bb_ptr = strong_corr_node(sp, pp_ptr, bb_ptr)) != NULL)
				t_add_node(bb_ptr, b_ptr->child[j], -1);
		}
	}
}

/*==================================================================*/
void para_top_expand(SENTENCE_DATA *sp, PARA_MANAGER *m_ptr)
/*==================================================================*/
{
	int i;
	BNST_DATA *new_ptr, *end_ptr, *pre_end_ptr;

	/* ������܂Ƃ߂�m�[�h�̑}��

	   B  B<P> B  B<P>(end_ptr) B  B �b .... B(new_ptr)
				��					(�������܂ł��ʏ�̕���)
				�b
				B(new_ptr) �������ɑ}���� B<P>(end_ptr)�̓��e���R�s�[
		B<P>(end_ptr) �� PARA(������܂Ƃ߂�m�[�h)�ƂȂ�
	*/

	for (i = 0; i < m_ptr->child_num; i++)
	para_top_expand(sp, m_ptr->child[i]);

	end_ptr = sp->bnst_data + m_ptr->end[m_ptr->part_num - 1];
	pre_end_ptr = sp->bnst_data + m_ptr->end[m_ptr->part_num - 2];
	
	new_ptr = sp->bnst_data + sp->Bnst_num + sp->New_Bnst_num;
	sp->New_Bnst_num++;
	if ((sp->Bnst_num + sp->New_Bnst_num) > BNST_MAX) {
	fprintf(stderr, ";; Too many nodes in expanding para top .\n");
	exit(1);
	}
	if (sp->Max_New_Bnst_num < sp->New_Bnst_num) {
	sp->Max_New_Bnst_num = sp->New_Bnst_num;
	}
	*new_ptr = *end_ptr;	/* �R�s�[ */

	/*
	  new_ptr �� end_ptr ���R�s�[����Ƒo����f(feature�ւ̃|�C���^)����
	  f�̎��̂��|�C���g����Cfree����ۂɖ��ƂȂ�D������,

		end_ptr->f = NULL;
	
	  �Ƃ��đΏ����Ă���������ł͕��񖖔��̕��߂����̌�̗p���̊i��͂�
	  �i�v�f�Ƃ݂Ȃ���Ȃ��C�܂����x��make_tree���s��f���Ȃ��Ȃ��Ă��܂�
	  �Ȃǂ̖�肪�������̂ŁCclear_feature�𕶂��Ƃ̉�͂̃��[�v��
	  �擪�ōs���悤�ɏC������ (98/02/07)
	*/

	/* �q�m�[�h�̐��� */

	new_ptr->child[0] = NULL;
	t_attach_node(end_ptr, new_ptr, 0);
	while (pre_end_ptr < end_ptr->child[1] &&	
	   end_ptr->child[1]->para_type != PARA_INCOMP)
	t_attach_node(new_ptr, t_del_node(end_ptr, end_ptr->child[1]), -1);

	/* �t���O(PARA,<P>)�̐��� */

	end_ptr->para_type = PARA_NIL;
	end_ptr->para_top_p = TRUE;
	new_ptr->para_type = PARA_NORMAL;
	for (i = 0; i < m_ptr->part_num - 1; i++)
	sp->bnst_data[m_ptr->end[i]].para_type = PARA_NORMAL;
}

/*==================================================================*/
		  void para_modifier_expand(BNST_DATA *b_ptr)
/*==================================================================*/
{
	int i, j, k;

	init_str();

	/* PARA �ɌW���Ă���m�[�h�� <P> �ɌW���� : �R�s�[�� */

	if (b_ptr->para_top_p == 1 /*TRUE*/) {
	for ( i=0; b_ptr->child[i]; i++ ) {
		if (b_ptr->child[i]->para_type == PARA_NIL &&	
		!check_feature(b_ptr->child[i]->f, kakari_renyou)) {

		/* b_ptr->child[i] �C������ */

		b_ptr->child[i]->to_para_p = TRUE;

		for ( j=0; b_ptr->child[j]; j++ ) {
			if (b_ptr->child[j]->para_type == PARA_NORMAL) {

			/* b_ptr->child[j] <P>���� */
			
			for ( k=0; b_ptr->child[j]->child[k]; k++ );
			b_ptr->child[j]->child[k] = b_ptr->child[i];
			b_ptr->child[j]->child[k+1] = NULL;
			}
		}
		b_ptr->child[i] = NULL;
		}
	}
	}

	for ( i=0; b_ptr->child[i]; i++ )
	para_modifier_expand(b_ptr->child[i]);
}

/*==================================================================*/
   void incomplete_para_expand(SENTENCE_DATA *sp, BNST_DATA *b_ptr)
/*==================================================================*/
{
	int i, j, para_pos;
	int new_num, child_num;
	BNST_DATA *para_ptr, *new_ptr;
	BNST_DATA *pre_childs[10], *pos_childs[10];

	/* ��������̓W�J : �R�s�[�L(�q�ꂪ�V�f�[�^�C���Ƃ̏q���PARA��) */

	para_pos = -1;

	for ( i=0; b_ptr->child[i]; i++ )
	if (b_ptr->child[i]->para_top_p == 1 /*TRUE*/)
		for ( j=0; b_ptr->child[i]->child[j]; j++ )
		if (b_ptr->child[i]->child[j]->para_type == PARA_INCOMP) {
			para_pos = i;
			break;
		}

	if (para_pos != -1) {

	/* ���Ƃ̏C���v�f���X�g�b�N */

	for ( i=0; b_ptr->child[i] && i < para_pos; i++ )
		pre_childs[i] = b_ptr->child[i];
	pre_childs[i] = NULL;
	for ( i=para_pos+1, j = 0; b_ptr->child[i]; i++, j++ )
		pos_childs[j] = b_ptr->child[i];
	pos_childs[j] = NULL;

	para_ptr = b_ptr->child[para_pos];

	new_num = 0;
	for ( i=0; para_ptr->child[i]; i++ )
		if (para_ptr->child[i]->para_type == PARA_NORMAL) {
			
		new_ptr = sp->bnst_data + sp->Bnst_num + sp->New_Bnst_num;
		sp->New_Bnst_num++;
		if ((sp->Bnst_num + sp->New_Bnst_num) > BNST_MAX) {
			fprintf(stderr, 
				";; Too many nodes in expanding incomplete para .\n");
			exit(1);
		}
		*new_ptr = *b_ptr;		/* �R�s�[ */
		new_ptr->f = NULL;		/* ���ӁI�I �������Ȃ��ƌ��SF */

		new_ptr->parent = b_ptr;		/* �V�m�[�h�̐e(�������g) */

		b_ptr->child[new_num] = new_ptr;	/* ���m�[�h�� PARA */

		/* �V�����m�[�h�̎q��ݒ�
		   (���̏C���m�[�h�C<P>�C<I>, �O�̏C���m�[�h) */

		child_num = 0;
		for (j=0; pre_childs[j]; j++)
			new_ptr->child[child_num++] = pre_childs[j];
		new_ptr->child[child_num++] = para_ptr->child[i];
		while (para_ptr->child[i+1] &&
			   para_ptr->child[i+1]->para_type == PARA_INCOMP) {
			new_ptr->child[child_num++] = para_ptr->child[i+1];
			i++;
		}
		for (j=0; pos_childs[j]; j++)
			new_ptr->child[child_num++] = pos_childs[j];
		new_ptr->child[child_num++] = NULL;

		new_ptr++;
		new_num++;
		}
	b_ptr->child[new_num] = NULL;
	b_ptr->para_top_p = TRUE;
	}

	for ( i=0; b_ptr->child[i]; i++ )
	  incomplete_para_expand(sp, b_ptr->child[i]);
}

/*==================================================================*/
		int make_dpnd_tree(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	init_bnst_tree_property(sp);

	sp->New_Bnst_num = 0;					/* ������ */

	if (make_simple_tree(sp) == FALSE)			/* �����N�t�� */
	return FALSE;
	
	if (OptExpandP == 1 /*TRUE*/)	
	for (i = 0; i < sp->Para_M_num; i++)		/* ������̓W�J */
		if (sp->para_manager[i].parent == NULL)
		strong_para_expand(sp, sp->para_manager + i);		
	for (i = 0; i < sp->Para_M_num; i++)		/* PARA�̓W�J */
	if (sp->para_manager[i].parent == NULL)
		para_top_expand(sp, sp->para_manager + i);		
	if (OptExpandP == 1 /*TRUE*/)	
	para_modifier_expand(sp->bnst_data + sp->Bnst_num - 1); /* PARA�C���̓W�J */
	/*
	incomplete_para_expand(sp->bnst_data + sp->Bnst_num - 1);*/ /* ��������̓W�J */

	return TRUE;
}

/*==================================================================*/
	  void para_info_to_tag(BNST_DATA *bp, TAG_DATA *tp)
/*==================================================================*/
{
	tp->para_num = bp->para_num;
	tp->para_key_type = bp->para_key_type;
	tp->para_top_p = bp->para_top_p;
	tp->para_type = bp->para_type;
	tp->to_para_p = bp->to_para_p;
}

/*==================================================================*/
		  int find_head_tag_from_bnst(BNST_DATA *bp)
/*==================================================================*/
{
	int offset;
	char *cp;

	init_str();

	/* <P>�̂Ƃ��Ɓu�^�O�P�ʎ󖳎��v�̂Ƃ��͌W�����Ō�̃^�O�P�ʂƂ��� */
	if (bp->para_type != PARA_NORMAL && 
	(!check_feature(bp->f, tag_unit_ignore)) && 
	(cp = check_feature(bp->parent->f, tag_unit)) != NULL) {
	offset = atoi(cp + 11);
	if (offset > 0 || bp->parent->tag_num <= -1 * offset) {
		offset = 0;
	}
	}
	else {
	offset = 0;
	}

	return offset;
}

/*==================================================================*/
		   int bnst_to_tag_tree(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j, offset;
	BNST_DATA *bp;
	TAG_DATA *tp;

	/* ���߂̖؍\������^�O�P�ʂ̖؍\���֕ϊ� */

	init_tag_tree_property(sp);
	sp->New_Tag_num = 0;

	/* new bnst -> tag */
	for (i = sp->New_Bnst_num - 1; i >= 0; i--) { /* <PARA>(1)-<PARA>(2) �̂Ƃ��̂��߂Ɍォ�炷�� */
	bp = sp->bnst_data + sp->Bnst_num + i;

	/* new�̈��copy */
	*(sp->tag_data + sp->Tag_num + sp->New_Tag_num) = *(bp->tag_ptr + bp->tag_num - 1);
	sp->New_Tag_num++;

	tp = sp->tag_data + sp->Tag_num + sp->New_Tag_num - 1;

	para_info_to_tag(bp, tp);
	tp->child[0] = NULL;

	/* <PARA>�̂Ƃ���head�̂� */
	if (bp->para_top_p == 1 /*TRUE*/) {
		bp->tag_ptr = tp;
		bp->tag_num = 1;
	}
	else {
		if (bp->tag_num > 1) {
		/* ���ߓ��^�O�P�ʂ̐e�� <P>(-<PARA>) �̂Ƃ� */
		(bp->tag_ptr + bp->tag_num - 2)->parent = tp;
		t_add_node((BNST_DATA *)tp, 
			   (BNST_DATA *)(bp->tag_ptr + bp->tag_num - 2), -1);

		/* ���ߓ� */
		for (j = 0; j < bp->tag_num - 2; j++) {
			(bp->tag_ptr + j)->parent = bp->tag_ptr + j + 1;
			t_add_node((BNST_DATA *)(bp->tag_ptr + j + 1),	
				   (BNST_DATA *)(bp->tag_ptr + j), -1);
		}
		/* bp->tag_ptr �̍Ō�̂ЂƂ͊Ԉ���Ă��� (<PARA>�̕�) */
		}
		else {
		bp->tag_ptr = tp;
		}
	}

	/* �e�Ǝq�̃����N�� (new) */
	tp->parent = bp->parent->tag_ptr + bp->parent->tag_num - 1; /* <PARA>�� */
	t_add_node((BNST_DATA *)(bp->parent->tag_ptr + bp->parent->tag_num - 1),	
		   (BNST_DATA *)tp, -1);
	}

	/* orig */
	for (i = sp->Bnst_num - 1; i >= 0; i--) {
	bp = sp->bnst_data + i;

	para_info_to_tag(bp, bp->tag_ptr + bp->tag_num - 1);

	/* <PARA>�̂Ƃ���head�݂̂����Atag_ptr, tag_num�̕ύX�͂��Ȃ� */
	if (bp->para_top_p == FALSE) {
		/* ���ߓ� */
		for (j = 0; j < bp->tag_num - 1; j++) {
		(bp->tag_ptr + j)->parent = bp->tag_ptr + j + 1;
		t_add_node((BNST_DATA *)(bp->tag_ptr + j + 1),	
			   (BNST_DATA *)(bp->tag_ptr + j), -1);
		}
	}

	if (i == sp->Bnst_num - 1) continue;

	/* �e�Ǝq */
	if (bp->parent) {
		offset = find_head_tag_from_bnst(bp); /* �^�O�P�ʓ��̌W�������[�����瓾�� */
		(bp->tag_ptr + bp->tag_num - 1)->parent = bp->parent->tag_ptr + bp->parent->tag_num - 1 + offset;
		t_add_node((BNST_DATA *)(bp->parent->tag_ptr + bp->parent->tag_num - 1 + offset),	
			   (BNST_DATA *)(bp->tag_ptr + bp->tag_num - 1), -1);
	}
	else {
		fprintf(stderr, ";; %s(%d)'s parent doesn't exist!\n", bp->Jiritu_Go, i);
	}
	}
	return 0;
}

/*====================================================================
							   END
====================================================================*/
