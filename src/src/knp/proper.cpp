/*====================================================================

				 ŒÅ—L–¼ŽŒˆ—

											   S.Kurohashi 96. 7. 4

	$Id: proper.c,v 1.31 2002/11/03 05:37:46 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

#ifdef	_WINDOWS
static char* jinmei = NULL;
static char* chimei = NULL;
static char* soshikimei = NULL;
static char* pronoun = NULL;
static char* address = NULL;
static char* taigen = NULL;
static char* hei_kekku = NULL;
static char* hei_bunsetsu = NULL;
static char* hei_kekku2 = NULL;
static char* hei_bunsetsu2 = NULL;
static char* p_gi = NULL;
static char* hei_ok = NULL;
static char jinmei_2[] = "l–¼";
static char chimei_2[] = "’n–¼";
static char soshikimei_2[] = "‘gD–¼";
static char pronoun_2[] = "ŒÅ—L–¼ŽŒ";
static char address_2[] = "ZŠ";
static char taigen_2[] = "‘ÌŒ¾";
static char hei_kekku_2[] = "•ÀŒ‹‹å”";
static char hei_bunsetsu_2[] = "•ÀŒ‹•¶ß”";
static char hei_kekku2_2[] = "•ÀŒ‹‹å”:";
static char hei_bunsetsu2_2[] = "•ÀŒ‹•¶ß”:";
static char p_gi_2[] = "%s‹^";
static char hei_ok_2[] = "ŒÅ•À—ñOK";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	jinmei = toStringEUC(jinmei_2);
	chimei = toStringEUC(chimei_2);
	soshikimei = toStringEUC(soshikimei_2);
	pronoun = toStringEUC(pronoun_2);
	address = toStringEUC(address_2);
	taigen = toStringEUC(taigen_2);
	hei_kekku = toStringEUC(hei_kekku_2);
	hei_bunsetsu = toStringEUC(hei_bunsetsu_2);
	hei_kekku2 = toStringEUC(hei_kekku2_2);
	hei_bunsetsu2 = toStringEUC(hei_bunsetsu2_2);
	p_gi = toStringEUC(p_gi_2);
	hei_ok = toStringEUC(hei_ok_2);
#endif
}
void final_str_proper() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(jinmei); jinmei = NULL;
	free(chimei); chimei = NULL;
	free(soshikimei); soshikimei = NULL;
	free(pronoun); pronoun = NULL;
	free(address); address = NULL;
	free(taigen); taigen = NULL;
	free(hei_kekku); hei_kekku = NULL;
	free(hei_bunsetsu); hei_bunsetsu = NULL;
	free(hei_kekku2); hei_kekku2 = NULL;
	free(hei_bunsetsu2); hei_bunsetsu2 = NULL;
	free(p_gi); p_gi = NULL;
	free(hei_ok); hei_ok = NULL;
#endif
}
#ifdef	_LINUX
static char jinmei[] = "l–¼";
static char chimei[] = "’n–¼";
static char soshikimei[] = "‘gD–¼";
static char pronoun[] = "ŒÅ—L–¼ŽŒ";
static char address[] = "ZŠ";
static char taigen[] = "‘ÌŒ¾";
static char hei_kekku[] = "•ÀŒ‹‹å”";
static char hei_bunsetsu[] = "•ÀŒ‹•¶ß”";
static char hei_kekku2[] = "•ÀŒ‹‹å”:";
static char hei_bunsetsu2[] = "•ÀŒ‹•¶ß”:";
static char p_gi[] = "%s‹^";
static char hei_ok[] = "ŒÅ•À—ñOK";
#endif

/*==================================================================*/
		   char *NEcheck(BNST_DATA *b_ptr)
/*==================================================================*/
{
	char *class2;

	init_str();

	if ((class2 = check_feature(b_ptr->f, jinmei)) == 0 &&	
	(class2 = check_feature(b_ptr->f, chimei)) == 0 &&	
	(class2 = check_feature(b_ptr->f, soshikimei)) == 0 &&	
	(class2 = check_feature(b_ptr->f, pronoun)) == 0) {
	return NULL;
	}
	return class2;
}

/*==================================================================*/
	 int NEparaCheck(BNST_DATA *b_ptr, BNST_DATA *h_ptr)
/*==================================================================*/
{
	char *class2, str[11];

	/* b_ptr ‚ªŒÅ—L•\Œ»‚Å‚ ‚é */
	if ((class2 = NEcheck(b_ptr)) == NULL) {
	return FALSE;
	}

	art_sprintf_1(str, 11, p_gi, class2);

	/* h_ptr ‚ÍŒÅ—L•\Œ»‚Å‚Í‚È‚¢
	   class2 ‚Æˆê’v‚·‚éu`‹^v‚ª‚Â‚¢‚Ä‚¢‚é */
	if (NEcheck(h_ptr) ||	
	!check_feature(h_ptr->f, str)) {
	return FALSE;
	}

	/* h_ptr ‚ð“¯‚¶ŒÅ—L•\Œ»ƒNƒ‰ƒX‚É‚·‚é */
	assign_cfeature(&(h_ptr->f), class2);
	assign_cfeature(&(h_ptr->f), hei_ok);

	return TRUE;
}

/*==================================================================*/
		   void ne_para_analysis(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i, value;
	char *cp;

	init_str();

	for (i = 0; i < sp->Bnst_num; i++) {
	/* ZŠ‚ª“ü‚é•À—ñ‚Í‚â‚ß‚Ä‚¨‚­ */
	if (check_feature(sp->bnst_data[i].f, address) ||	
		!check_feature(sp->bnst_data[i].f, taigen) ||	
		(sp->bnst_data[i].dpnd_head > 0 &&	
		 !check_feature(sp->bnst_data[sp->bnst_data[i].dpnd_head].f, taigen))) {
		continue;
	}
	cp = check_feature(sp->bnst_data[i].f, hei_kekku);
	if (cp) {
		value = atoi(cp+strlen(hei_kekku2));
		if (value > 2) {
		/* ŒW‚è‘¤‚ð’²‚×‚ÄŒÅ—L–¼ŽŒ‚¶‚á‚È‚©‚Á‚½‚çAŽó‚¯‘¤‚ð’²‚×‚é */
		if (NEparaCheck(&(sp->bnst_data[i]), &(sp->bnst_data[sp->bnst_data[i].dpnd_head])) == FALSE)
			NEparaCheck(&(sp->bnst_data[sp->bnst_data[i].dpnd_head]), &(sp->bnst_data[i]));
		continue;
		}
	}

	cp = check_feature(sp->bnst_data[i].f, hei_bunsetsu);
	if (cp) {
		value = atoi(cp+strlen(hei_bunsetsu2));
		if (value > 1) {
		if (NEparaCheck(&(sp->bnst_data[i]), &(sp->bnst_data[sp->bnst_data[i].dpnd_head])) == FALSE)
			NEparaCheck(&(sp->bnst_data[sp->bnst_data[i].dpnd_head]), &(sp->bnst_data[i]));
		}
	}
	}
}

/*====================================================================
							   END
====================================================================*/
