/*====================================================================

				 鈎括弧の処理
					
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
static char kakko_begin_2[] = "括弧始";
static char kakko_begin2_2[] = "括弧始２";
static char kakko_end_2[] = "括弧終";
static char kakko_end2_2[] = "括弧終２";
static char kakari_renkaku_2[] = "係:連格";
static char kakari_rentai_2[] = "係:連体";
static char kakari_no_kaku_2[] = "係:ノ格";
static char kakari_doukaku_2[] = "係:同格連体";
static char kakari_hei_2[] = "係:括弧並列";
static char kakari_bunmatsu_2[] = "係:文末";
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
static char kakko_begin[] = "括弧始";
static char kakko_begin2[] = "括弧始２";
static char kakko_end[] = "括弧終";
static char kakko_end2[] = "括弧終２";
static char kakari_renkaku[] = "係:連格";
static char kakari_rentai[] = "係:連体";
static char kakari_no_kaku[] = "係:ノ格";
static char kakari_doukaku[] = "係:同格連体";
static char kakari_hei[] = "係:括弧並列";
static char kakari_bunmatsu[] = "係:文末";
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
	  "．．「○○」．．．「××」．．"	
	  "．．「○○「×××」○○」．．"	
	  "．．「○○○○○○○○○○○○"	
	  "××××××××××××」．．"	などのパターンに対処
	*/

	int i, k, stack[QUOTE_MAX], s_num, quote_p = FALSE;

	init_str();

	k = 0;
	s_num = -1;

	for (i = 0; i < sp->Bnst_num; i++) {

	if (check_feature(sp->bnst_data[i].f, kakko_begin)) {
		/* 最大数を越えないかチェック(最後の要素が番人なので、それを変えては
		   いけない) */
		if (k >= QUOTE_MAX-1) {
		fprintf(stderr, "Too many quote (%s) ...\n", sp->Comment ? sp->Comment : "");
		return CONTINUE;
		}
		s_num ++;
		stack[s_num] = k;
		quote_data.in_num[k] = i;
		k++;

		/* 「『‥ を扱うため上のことを繰り返す */
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
		quote_data.out_num[k] = i; /* 括弧終が多い場合 */
		k++;
		} else {
		quote_data.out_num[stack[s_num]] = i;
		s_num--;
		}


		/* ‥』」 を扱うため上のことを繰り返す */
		if (check_feature(sp->bnst_data[i].f, kakko_end2)) {
		if (s_num == -1) {
			quote_data.out_num[k] = i; /* 括弧終が多い場合 */
			k++;
		} else {
			quote_data.out_num[stack[s_num]] = i;
			s_num--;
		}
		}
	}			
	}

	for (i = 0; i < k; i++) {

	/* 括弧が閉じていない場合は, 文頭または文末を境界に */	

	if (quote_data.in_num[i] == -1) 
		quote_data.in_num[i] = 0;
	if (quote_data.out_num[i] == -1)
		quote_data.out_num[i] = sp->Bnst_num - 1;

	/* 一文節の括弧を考慮しない場合
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

	if (start == end) continue; /* １文節だけの括弧は無視 */

	/* 括弧の上のマスク */

	for (i = 0; i < start; i++) {
		for (j = start; j < end; j++)
		Quote_matrix[i][j] = 0;

		/*	
		   括弧内末尾の文節には連格,連体,ノ格,同格連体,括弧並列のみ
		   係れるとする．
				例) 「私の「本当の気持ち」は…」

		   用言に連用が係ることも稀にはあるが，それを許すと通常の場合の
		   解析誤りが大量に生まれるので無視する．
				例) 「彼が「東京にいった」ことは…」
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

	/* 括弧の右のマスク */

	for (i = start; i < end; i++)
		for (j = end + 1; j < sp->Bnst_num; j++)
		Quote_matrix[i][j] = 0;

	/* 括弧内の句点の右上のマスク	
	   (句点の右は開けておく --> 次の文末とPになる) */

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

	if ((quote_p = check_quote(sp)) != 0) { /* 鈎括弧の検出 */
	if (quote_p == CONTINUE) return quote_p;

	if (OptDisplay == OPT_DEBUG) print_quote();

	mask_quote(sp); 		/* 行列の書き換え */
	}

	return quote_p;
}

/*====================================================================
							   END
====================================================================*/
