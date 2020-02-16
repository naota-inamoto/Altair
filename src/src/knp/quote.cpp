/*====================================================================

				 �b���ʂ̏���
					
											   S.Kurohashi 1996. 4.18
											   S.Ozaki	   1994.12. 1

	$Id: quote.c,v 1.8 2001/11/04 03:48:39 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

QUOTE_DATA quote_data;

#ifdef	_WINDOWS
static char* kakko_begin = NULL;
static char* kakko_begin2 = NULL;
static char* kakko_end = NULL;
static char* kakko_end2 = NULL;
static char* kakari_renkaku = NULL;
static char* kakari_rentai = NULL;
static char* kakari_no_kaku = NULL;
static char* kakari_doukaku = NULL;
static char* kakari_hei = NULL;
static char* kakari_bunmatsu = NULL;
static char kakko_begin_2[] = "���ʎn";
static char kakko_begin2_2[] = "���ʎn�Q";
static char kakko_end_2[] = "���ʏI";
static char kakko_end2_2[] = "���ʏI�Q";
static char kakari_renkaku_2[] = "�W:�A�i";
static char kakari_rentai_2[] = "�W:�A��";
static char kakari_no_kaku_2[] = "�W:�m�i";
static char kakari_doukaku_2[] = "�W:���i�A��";
static char kakari_hei_2[] = "�W:���ʕ���";
static char kakari_bunmatsu_2[] = "�W:����";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	kakko_begin = toStringEUC(kakko_begin_2);
	kakko_begin2 = toStringEUC(kakko_begin2_2);
	kakko_end = toStringEUC(kakko_end_2);
	kakko_end2 = toStringEUC(kakko_end2_2);
	kakari_renkaku = toStringEUC(kakari_renkaku_2);
	kakari_rentai = toStringEUC(kakari_rentai_2);
	kakari_no_kaku = toStringEUC(kakari_no_kaku_2);
	kakari_doukaku = toStringEUC(kakari_doukaku_2);
	kakari_hei = toStringEUC(kakari_hei_2);
	kakari_bunmatsu = toStringEUC(kakari_bunmatsu_2);
#endif
}
void final_str_quote() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(kakko_begin); kakko_begin = NULL;
	free(kakko_begin2); kakko_begin2 = NULL;
	free(kakko_end); kakko_end = NULL;
	free(kakko_end2); kakko_end2 = NULL;
	free(kakari_renkaku); kakari_renkaku = NULL;
	free(kakari_rentai); kakari_rentai = NULL;
	free(kakari_no_kaku); kakari_no_kaku = NULL;
	free(kakari_doukaku); kakari_doukaku = NULL;
	free(kakari_hei); kakari_hei = NULL;
	free(kakari_bunmatsu); kakari_bunmatsu = NULL;
#endif
}
#ifdef	_LINUX
static char kakko_begin[] = "���ʎn";
static char kakko_begin2[] = "���ʎn�Q";
static char kakko_end[] = "���ʏI";
static char kakko_end2[] = "���ʏI�Q";
static char kakari_renkaku[] = "�W:�A�i";
static char kakari_rentai[] = "�W:�A��";
static char kakari_no_kaku[] = "�W:�m�i";
static char kakari_doukaku[] = "�W:���i�A��";
static char kakari_hei[] = "�W:���ʕ���";
static char kakari_bunmatsu[] = "�W:����";
#endif

/*==================================================================*/
		  void init_quote(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j;

	for (i = 0; i < QUOTE_MAX; i++) {
	quote_data.in_num[i] = -1;
	quote_data.out_num[i] = -1; 	
	}

	for (i = 0; i < sp->Bnst_num; i++)
	for (j = 0; j < sp->Bnst_num; j++)
		Quote_matrix[i][j] = 1;
}

/*==================================================================*/
						 void print_quote()
/*==================================================================*/
{
	int i;

	for (i = 0; quote_data.in_num[i] >= 0; i++) {
	fprintf(Outfp,"Quote_num %d in %d out %d \n", i,
		quote_data.in_num[i], quote_data.out_num[i]);
	}
}

/*==================================================================*/
		  int check_quote(SENTENCE_DATA *sp)
/*==================================================================*/
{
	/*
	  "�D�D�u�����v�D�D�D�u�~�~�v�D�D"	
	  "�D�D�u�����u�~�~�~�v�����v�D�D"	
	  "�D�D�u������������������������"	
	  "�~�~�~�~�~�~�~�~�~�~�~�~�v�D�D"	�Ȃǂ̃p�^�[���ɑΏ�
	*/

	int i, k, stack[QUOTE_MAX], s_num, quote_p = FALSE;

	init_str();

	k = 0;
	s_num = -1;

	for (i = 0; i < sp->Bnst_num; i++) {

	if (check_feature(sp->bnst_data[i].f, kakko_begin)) {
		/* �ő吔���z���Ȃ����`�F�b�N(�Ō�̗v�f���Ԑl�Ȃ̂ŁA�����ς��Ă�
		   �����Ȃ�) */
		if (k >= QUOTE_MAX-1) {
		fprintf(stderr, "Too many quote (%s) ...\n", sp->Comment ? sp->Comment : "");
		return CONTINUE;
		}
		s_num ++;
		stack[s_num] = k;
		quote_data.in_num[k] = i;
		k++;

		/* �u�w�d ���������ߏ�̂��Ƃ��J��Ԃ� */
		if (check_feature(sp->bnst_data[i].f, kakko_begin2)) {
		if (k >= QUOTE_MAX-1) {
			fprintf(stderr, "Too many quote (%s) ...\n", sp->Comment ? sp->Comment : "");
			return CONTINUE;
		}
		s_num ++;
		stack[s_num] = k;
		quote_data.in_num[k] = i;
		k++;
		}
	}
	if (check_feature(sp->bnst_data[i].f, kakko_end)) {
		if (s_num == -1) {
		if (k >= QUOTE_MAX-1) {
			fprintf(stderr, "Too many quote (%s) ...\n", sp->Comment ? sp->Comment : "");
			return CONTINUE;
		}
		quote_data.out_num[k] = i; /* ���ʏI�������ꍇ */
		k++;
		} else {
		quote_data.out_num[stack[s_num]] = i;
		s_num--;
		}


		/* �d�x�v ���������ߏ�̂��Ƃ��J��Ԃ� */
		if (check_feature(sp->bnst_data[i].f, kakko_end2)) {
		if (s_num == -1) {
			quote_data.out_num[k] = i; /* ���ʏI�������ꍇ */
			k++;
		} else {
			quote_data.out_num[stack[s_num]] = i;
			s_num--;
		}
		}
	}			
	}

	for (i = 0; i < k; i++) {

	/* ���ʂ����Ă��Ȃ��ꍇ��, �����܂��͕��������E�� */	

	if (quote_data.in_num[i] == -1) 
		quote_data.in_num[i] = 0;
	if (quote_data.out_num[i] == -1)
		quote_data.out_num[i] = sp->Bnst_num - 1;

	/* �ꕶ�߂̊��ʂ��l�����Ȃ��ꍇ
	if (quote_data.in_num[i] != quote_data.out_num[i])
	quote_p = TRUE;
	*/

	quote_p = TRUE;
	}

	return quote_p;
}

/*==================================================================*/
		  void mask_quote(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, j, k, l, start, end;

	init_str();

	for (k = 0; quote_data.in_num[k] >= 0; k++) {

	start = quote_data.in_num[k];
	end = quote_data.out_num[k];

	if (start == end) continue; /* �P���߂����̊��ʂ͖��� */

	/* ���ʂ̏�̃}�X�N */

	for (i = 0; i < start; i++) {
		for (j = start; j < end; j++)
		Quote_matrix[i][j] = 0;

		/*	
		   ���ʓ������̕��߂ɂ͘A�i,�A��,�m�i,���i�A��,���ʕ���̂�
		   �W���Ƃ���D
				��) �u���́u�{���̋C�����v�́c�v

		   �p���ɘA�p���W�邱�Ƃ��H�ɂ͂��邪�C����������ƒʏ�̏ꍇ��
		   ��͌�肪��ʂɐ��܂��̂Ŗ�������D
				��) �u�ނ��u�����ɂ������v���Ƃ́c�v
		*/

		if (Quote_matrix[i][end] &&
		(check_feature(sp->bnst_data[i].f, kakari_renkaku) ||
		 check_feature(sp->bnst_data[i].f, kakari_rentai) ||
		 check_feature(sp->bnst_data[i].f, kakari_no_kaku) ||
		 check_feature(sp->bnst_data[i].f, kakari_doukaku) ||
		 check_feature(sp->bnst_data[i].f, kakari_hei)))
		;
		else	
		Quote_matrix[i][end] = 0;
	}

	/* ���ʂ̉E�̃}�X�N */

	for (i = start; i < end; i++)
		for (j = end + 1; j < sp->Bnst_num; j++)
		Quote_matrix[i][j] = 0;

	/* ���ʓ��̋�_�̉E��̃}�X�N	
	   (��_�̉E�͊J���Ă��� --> ���̕�����P�ɂȂ�) */

	for (l = start; l < end; l++)
		if (check_feature(sp->bnst_data[l].f, kakari_bunmatsu))
		for (i = start; i < l; i++)
			for (j = l + 1; j <= end; j++)
			Quote_matrix[i][j] = 0;
	}
}

/*==================================================================*/
			 int quote(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int quote_p = FALSE;

	init_quote(sp);

	if ((quote_p = check_quote(sp)) != 0) { /* �b���ʂ̌��o */
	if (quote_p == CONTINUE) return quote_p;

	if (OptDisplay == OPT_DEBUG) print_quote();

	mask_quote(sp); 		/* �s��̏������� */
	}

	return quote_p;
}

/*====================================================================
							   END
====================================================================*/
