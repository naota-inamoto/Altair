/*====================================================================

				 ŠbŠ‡ŒÊ‚Ìˆ—
					
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
static char kakko_begin_2[] = "Š‡ŒÊn";
static char kakko_begin2_2[] = "Š‡ŒÊn‚Q";
static char kakko_end_2[] = "Š‡ŒÊI";
static char kakko_end2_2[] = "Š‡ŒÊI‚Q";
static char kakari_renkaku_2[] = "ŒW:˜AŠi";
static char kakari_rentai_2[] = "ŒW:˜A‘Ì";
static char kakari_no_kaku_2[] = "ŒW:ƒmŠi";
static char kakari_doukaku_2[] = "ŒW:“¯Ši˜A‘Ì";
static char kakari_hei_2[] = "ŒW:Š‡ŒÊ•À—ñ";
static char kakari_bunmatsu_2[] = "ŒW:•¶––";
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
static char kakko_begin[] = "Š‡ŒÊn";
static char kakko_begin2[] = "Š‡ŒÊn‚Q";
static char kakko_end[] = "Š‡ŒÊI";
static char kakko_end2[] = "Š‡ŒÊI‚Q";
static char kakari_renkaku[] = "ŒW:˜AŠi";
static char kakari_rentai[] = "ŒW:˜A‘Ì";
static char kakari_no_kaku[] = "ŒW:ƒmŠi";
static char kakari_doukaku[] = "ŒW:“¯Ši˜A‘Ì";
static char kakari_hei[] = "ŒW:Š‡ŒÊ•À—ñ";
static char kakari_bunmatsu[] = "ŒW:•¶––";
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
	  "DDu››vDDDu~~vDD"	
	  "DDu››u~~~v››vDD"	
	  "DDu››››››››››››"	
	  "~~~~~~~~~~~~vDD"	‚È‚Ç‚Ìƒpƒ^[ƒ“‚É‘Îˆ
	*/

	int i, k, stack[QUOTE_MAX], s_num, quote_p = FALSE;

	init_str();

	k = 0;
	s_num = -1;

	for (i = 0; i < sp->Bnst_num; i++) {

	if (check_feature(sp->bnst_data[i].f, kakko_begin)) {
		/* Å‘å”‚ğ‰z‚¦‚È‚¢‚©ƒ`ƒFƒbƒN(ÅŒã‚Ì—v‘f‚ª”Ôl‚È‚Ì‚ÅA‚»‚ê‚ğ•Ï‚¦‚Ä‚Í
		   ‚¢‚¯‚È‚¢) */
		if (k >= QUOTE_MAX-1) {
		fprintf(stderr, "Too many quote (%s) ...\n", sp->Comment ? sp->Comment : "");
		return CONTINUE;
		}
		s_num ++;
		stack[s_num] = k;
		quote_data.in_num[k] = i;
		k++;

		/* uwd ‚ğˆµ‚¤‚½‚ßã‚Ì‚±‚Æ‚ğŒJ‚è•Ô‚· */
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
		quote_data.out_num[k] = i; /* Š‡ŒÊI‚ª‘½‚¢ê‡ */
		k++;
		} else {
		quote_data.out_num[stack[s_num]] = i;
		s_num--;
		}


		/* dxv ‚ğˆµ‚¤‚½‚ßã‚Ì‚±‚Æ‚ğŒJ‚è•Ô‚· */
		if (check_feature(sp->bnst_data[i].f, kakko_end2)) {
		if (s_num == -1) {
			quote_data.out_num[k] = i; /* Š‡ŒÊI‚ª‘½‚¢ê‡ */
			k++;
		} else {
			quote_data.out_num[stack[s_num]] = i;
			s_num--;
		}
		}
	}			
	}

	for (i = 0; i < k; i++) {

	/* Š‡ŒÊ‚ª•Â‚¶‚Ä‚¢‚È‚¢ê‡‚Í, •¶“ª‚Ü‚½‚Í•¶––‚ğ‹«ŠE‚É */	

	if (quote_data.in_num[i] == -1) 
		quote_data.in_num[i] = 0;
	if (quote_data.out_num[i] == -1)
		quote_data.out_num[i] = sp->Bnst_num - 1;

	/* ˆê•¶ß‚ÌŠ‡ŒÊ‚ğl—¶‚µ‚È‚¢ê‡
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

	if (start == end) continue; /* ‚P•¶ß‚¾‚¯‚ÌŠ‡ŒÊ‚Í–³‹ */

	/* Š‡ŒÊ‚Ìã‚Ìƒ}ƒXƒN */

	for (i = 0; i < start; i++) {
		for (j = start; j < end; j++)
		Quote_matrix[i][j] = 0;

		/*	
		   Š‡ŒÊ“à––”ö‚Ì•¶ß‚É‚Í˜AŠi,˜A‘Ì,ƒmŠi,“¯Ši˜A‘Ì,Š‡ŒÊ•À—ñ‚Ì‚İ
		   ŒW‚ê‚é‚Æ‚·‚éD
				—á) u„‚Ìu–{“–‚Ì‹C‚¿v‚Ícv

		   —pŒ¾‚É˜A—p‚ªŒW‚é‚±‚Æ‚à‹H‚É‚Í‚ ‚é‚ªC‚»‚ê‚ğ‹–‚·‚Æ’Êí‚Ìê‡‚Ì
		   ‰ğÍŒë‚è‚ª‘å—Ê‚É¶‚Ü‚ê‚é‚Ì‚Å–³‹‚·‚éD
				—á) u”Ş‚ªu“Œ‹‚É‚¢‚Á‚½v‚±‚Æ‚Ícv
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

	/* Š‡ŒÊ‚Ì‰E‚Ìƒ}ƒXƒN */

	for (i = start; i < end; i++)
		for (j = end + 1; j < sp->Bnst_num; j++)
		Quote_matrix[i][j] = 0;

	/* Š‡ŒÊ“à‚Ì‹å“_‚Ì‰Eã‚Ìƒ}ƒXƒN	
	   (‹å“_‚Ì‰E‚ÍŠJ‚¯‚Ä‚¨‚­ --> Ÿ‚Ì•¶––‚ÆP‚É‚È‚é) */

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

	if ((quote_p = check_quote(sp)) != 0) { /* ŠbŠ‡ŒÊ‚ÌŒŸo */
	if (quote_p == CONTINUE) return quote_p;

	if (OptDisplay == OPT_DEBUG) print_quote();

	mask_quote(sp); 		/* s—ñ‚Ì‘‚«Š·‚¦ */
	}

	return quote_p;
}

/*====================================================================
							   END
====================================================================*/
