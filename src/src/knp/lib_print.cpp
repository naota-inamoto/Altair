/*====================================================================

				 出力ルーチン

											   S.Kurohashi 91. 6.25
											   S.Kurohashi 93. 5.31

	$Id: lib_print.c,v 1.40.2.2 2005/07/29 08:19:05 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

#ifdef	_WINDOWS
static const char* special = NULL;
static const char* verb = NULL;
static const char* adj = NULL;
static const char* hantei = NULL;
static const char* jodoushi = NULL;
static const char* noun = NULL;
static const char* pronoun = NULL;
static const char* jinnmei = NULL;
static const char* chimei = NULL;
static const char* shijishi = NULL;
static const char* adv = NULL;
static const char* joshi = NULL;
static const char* conj = NULL;
static const char* rentai = NULL;
static const char* kandou = NULL;
static const char* settouji = NULL;
static const char* setsubiji = NULL;
static const char* yougen = NULL;
static const char* semi_yougen = NULL;
static const char* noudou = NULL;
static const char* kanju = NULL;
static const char* chokuju1 = NULL;
static const char* chokuju2 = NULL;
static const char* shieki_wo_ni = NULL;
static const char* shieki_wo = NULL;
static const char* shieki_ni = NULL;
static const char* shieki_ukemi = NULL;
static const char* kanou = NULL;
static const char* sonkei = NULL;
static const char* jihatsu = NULL;
static const char* hyousou = NULL;
static const char* ga_kaku = NULL;
static const char* ga = NULL;
static const char* wo_kaku = NULL;
static const char* wo = NULL;
static const char* ni_kaku = NULL;
static const char* ni = NULL;
static const char* de_kaku = NULL;
static const char* de = NULL;
static const char* kara_kaku = NULL;
static const char* kara = NULL;
static const char* to_kaku = NULL;
static const char* to = NULL;
static const char* yori_kaku = NULL;
static const char* yori = NULL;
static const char* he_keku = NULL;
static const char* he = NULL;
static const char* made_kaku = NULL;
static const char* made = NULL;
static const char* no_kaku = NULL;
static const char* no = NULL;
static const char* ga2_kaku = NULL;
static const char* ga2 = NULL;
static const char* ishi_irai = NULL;
static const char* ishi_ishi = NULL;
static const char* ishi_kanyuu = NULL;
static const char* ishi_ganbou = NULL;
static const char* ishi_kinnshi = NULL;
static const char* ishi_sanninshou = NULL;
static const char* ishi_moushide = NULL;
static const char* ishi_suiryou = NULL;
static const char* ishi_meirei = NULL;
static const char* toui = NULL;
static const char* toui_kyoka = NULL;
static const char* handan_kanou = NULL;
static const char* handan_kahi = NULL;
static const char* handan_suiryou = NULL;
static const char* handan_denbun = NULL;
static const char* handan_youtai = NULL;
static const char* dookuten = NULL;
static const char* ruijido = NULL;
static const char* hbar = NULL;
static const char* hbar2 = NULL;
static const char* branch = NULL;
static const char* corner = NULL;
static const char* vbar = NULL;
static const char* space = NULL;
static const char* space2 = NULL;
static const char special_2[] = "特殊";
static const char verb_2[] = "動詞";
static const char adj_2[] = "形容詞";
static const char hantei_2[] = "判定詞";
static const char jodoushi_2[] = "助動詞";
static const char noun_2[] = "名詞";
static const char pronoun_2[] = "固有名詞";
static const char jinnmei_2[] = "人名";
static const char chimei_2[] = "地名";
static const char shijishi_2[] = "指示詞";
static const char adv_2[] = "副詞";
static const char joshi_2[] = "助詞";
static const char conj_2[] = "接続詞";
static const char rentai_2[] = "連体詞";
static const char kandou_2[] = "感動詞";
static const char settouji_2[] = "接頭辞";
static const char setsubiji_2[] = "接尾辞";
static const char yougen_2[] = "用言";
static const char semi_yougen_2[] = "準用言";
static const char noudou_2[] = " 能動";
static const char kanju_2[] = " 間受";
static const char chokuju1_2[] = " 直受１";
static const char chokuju2_2[] = " 直受２";
static const char shieki_wo_ni_2[] = " 使役ヲニ";
static const char shieki_wo_2[] = " 使役ヲ";
static const char shieki_ni_2[] = " 使役ニ";
static const char shieki_ukemi_2[] = " 使役&受身";
static const char kanou_2[] = " 可能";
static const char sonkei_2[] = " 尊敬";
static const char jihatsu_2[] = " 自発";
static const char hyousou_2[] = " <表層格:";
static const char ga_kaku_2[] = "ガ格";
static const char ga_2[] = "ガ,";
static const char wo_kaku_2[] = "ヲ格";
static const char wo_2[] = "ヲ,";
static const char ni_kaku_2[] = "ニ格";
static const char ni_2[] = "ニ,";
static const char de_kaku_2[] = "デ格";
static const char de_2[] = "デ,";
static const char kara_kaku_2[] = "カラ格";
static const char kara_2[] = "カラ,";
static const char to_kaku_2[] = "ト格";
static const char to_2[] = "ト,";
static const char yori_kaku_2[] = "ヨリ格";
static const char yori_2[] = "ヨリ,";
static const char he_keku_2[] = "ヘ格";
static const char he_2[] = "ヘ,";
static const char made_kaku_2[] = "マデ格";
static const char made_2[] = "マデ,";
static const char no_kaku_2[] = "ノ格";
static const char no_2[] = "ノ,";
static const char ga2_kaku_2[] = "ガ２";
static const char ga2_2[] = "ガ２,";
static const char ishi_irai_2[] = "Modality-意思-依頼";
static const char ishi_ishi_2[] = "Modality-意思-意志";
static const char ishi_kanyuu_2[] = "Modality-意思-勧誘";
static const char ishi_ganbou_2[] = "Modality-意思-願望";
static const char ishi_kinnshi_2[] = "Modality-意思-禁止";
static const char ishi_sanninshou_2[] = "Modality-意思-三人称意志";
static const char ishi_moushide_2[] = "Modality-意思-申し出";
static const char ishi_suiryou_2[] = "Modality-意思-推量";
static const char ishi_meirei_2[] = "Modality-意思-命令";
static const char toui_2[] = "Modality-当為";
static const char toui_kyoka_2[] = "Modality-当為-許可";
static const char handan_kanou_2[] = "Modality-判断-可能性";
static const char handan_kahi_2[] = "Modality-判断-可能性-不可能";
static const char handan_suiryou_2[] = "Modality-判断-推量";
static const char handan_denbun_2[] = "Modality-判断-伝聞";
static const char handan_youtai_2[] = "Modality-判断-様態";
static const char dookuten_2[] = "読点";
static const char ruijido_2[] = "並列類似度:%.3f";
static const char hbar_2[] = "─";
static const char hbar2_2[] = "──";
static const char branch_2[] = "┤";
static const char corner_2[] = "┐";
static const char vbar_2[] = "│";
static const char space_2[] = "　";
static const char space2_2[] = "　　";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	special = toStringEUC(special_2);
	verb = toStringEUC(verb_2);
	adj = toStringEUC(adj_2);
	hantei = toStringEUC(hantei_2);
	jodoushi = toStringEUC(jodoushi_2);
	noun = toStringEUC(noun_2);
	pronoun = toStringEUC(pronoun_2);
	jinnmei = toStringEUC(jinnmei_2);
	chimei = toStringEUC(chimei_2);
	shijishi = toStringEUC(shijishi_2);
	adv = toStringEUC(adv_2);
	joshi = toStringEUC(joshi_2);
	conj = toStringEUC(conj_2);
	rentai = toStringEUC(rentai_2);
	kandou = toStringEUC(kandou_2);
	settouji = toStringEUC(settouji_2);
	setsubiji = toStringEUC(setsubiji_2);
	yougen = toStringEUC(yougen_2);
	semi_yougen = toStringEUC(semi_yougen_2);
	noudou = toStringEUC(noudou_2);
	kanju = toStringEUC(kanju_2);
	chokuju1 = toStringEUC(chokuju1_2);
	chokuju2 = toStringEUC(chokuju2_2);
	shieki_wo_ni = toStringEUC(shieki_wo_ni_2);
	shieki_wo = toStringEUC(shieki_wo_2);
	shieki_ni = toStringEUC(shieki_ni_2);
	shieki_ukemi = toStringEUC(shieki_ukemi_2);
	kanou = toStringEUC(kanou_2);
	sonkei = toStringEUC(sonkei_2);
	jihatsu = toStringEUC(jihatsu_2);
	hyousou = toStringEUC(hyousou_2);
	ga_kaku = toStringEUC(ga_kaku_2);
	ga = toStringEUC(ga_2);
	wo_kaku = toStringEUC(wo_kaku_2);
	wo = toStringEUC(wo_2);
	ni_kaku = toStringEUC(ni_kaku_2);
	ni = toStringEUC(ni_2);
	de_kaku = toStringEUC(de_kaku_2);
	de = toStringEUC(de_2);
	kara_kaku = toStringEUC(kara_kaku_2);
	kara = toStringEUC(kara_2);
	to_kaku = toStringEUC(to_kaku_2);
	to = toStringEUC(to_2);
	yori_kaku = toStringEUC(yori_kaku_2);
	yori = toStringEUC(yori_2);
	he_keku = toStringEUC(he_keku_2);
	he = toStringEUC(he_2);
	made_kaku = toStringEUC(made_kaku_2);
	made = toStringEUC(made_2);
	no_kaku = toStringEUC(no_kaku_2);
	no = toStringEUC(no_2);
	ga2_kaku = toStringEUC(ga2_kaku_2);
	ga2 = toStringEUC(ga2_2);
	ishi_irai = toStringEUC(ishi_irai_2);
	ishi_ishi = toStringEUC(ishi_ishi_2);
	ishi_kanyuu = toStringEUC(ishi_kanyuu_2);
	ishi_ganbou = toStringEUC(ishi_ganbou_2);
	ishi_kinnshi = toStringEUC(ishi_kinnshi_2);
	ishi_sanninshou = toStringEUC(ishi_sanninshou_2);
	ishi_moushide = toStringEUC(ishi_moushide_2);
	ishi_suiryou = toStringEUC(ishi_suiryou_2);
	ishi_meirei = toStringEUC(ishi_meirei_2);
	toui = toStringEUC(toui_2);
	toui_kyoka = toStringEUC(toui_kyoka_2);
	handan_kanou = toStringEUC(handan_kanou_2);
	handan_kahi = toStringEUC(handan_kahi_2);
	handan_suiryou = toStringEUC(handan_suiryou_2);
	handan_denbun = toStringEUC(handan_denbun_2);
	handan_youtai = toStringEUC(handan_youtai_2);
	dookuten = toStringEUC(dookuten_2);
	ruijido = toStringEUC(ruijido_2);
	hbar = toStringEUC(hbar_2);
	hbar2 = toStringEUC(hbar2_2);
	branch = toStringEUC(branch_2);
	corner = toStringEUC(corner_2);
	vbar = toStringEUC(vbar_2);
	space = toStringEUC(space_2);
	space2 = toStringEUC(space2_2);
#endif
}
void final_str_lib_print() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free((void*)special); special = NULL;
	free((void*)verb); verb = NULL;
	free((void*)adj); adj = NULL;
	free((void*)hantei); hantei = NULL;
	free((void*)jodoushi); jodoushi = NULL;
	free((void*)noun); noun = NULL;
	free((void*)pronoun); pronoun = NULL;
	free((void*)jinnmei); jinnmei = NULL;
	free((void*)chimei); chimei = NULL;
	free((void*)shijishi); shijishi = NULL;
	free((void*)adv); adv = NULL;
	free((void*)joshi); joshi = NULL;
	free((void*)conj); conj = NULL;
	free((void*)rentai); rentai = NULL;
	free((void*)kandou); kandou = NULL;
	free((void*)settouji); settouji = NULL;
	free((void*)setsubiji); setsubiji = NULL;
	free((void*)yougen); yougen = NULL;
	free((void*)semi_yougen); semi_yougen = NULL;
	free((void*)noudou); noudou = NULL;
	free((void*)kanju); kanju = NULL;
	free((void*)chokuju1); chokuju1 = NULL;
	free((void*)chokuju2); chokuju2 = NULL;
	free((void*)shieki_wo_ni); shieki_wo_ni = NULL;
	free((void*)shieki_wo); shieki_wo = NULL;
	free((void*)shieki_ni); shieki_ni = NULL;
	free((void*)shieki_ukemi); shieki_ukemi = NULL;
	free((void*)kanou); kanou = NULL;
	free((void*)sonkei); sonkei = NULL;
	free((void*)jihatsu); jihatsu = NULL;
	free((void*)hyousou); hyousou = NULL;
	free((void*)ga_kaku); ga_kaku = NULL;
	free((void*)ga); ga = NULL;
	free((void*)wo_kaku); wo_kaku = NULL;
	free((void*)wo); wo = NULL;
	free((void*)ni_kaku); ni_kaku = NULL;
	free((void*)ni); ni = NULL;
	free((void*)de_kaku); de_kaku = NULL;
	free((void*)de); de = NULL;
	free((void*)kara_kaku); kara_kaku = NULL;
	free((void*)kara); kara = NULL;
	free((void*)to_kaku); to_kaku = NULL;
	free((void*)to); to = NULL;
	free((void*)yori_kaku); yori_kaku = NULL;
	free((void*)yori); yori = NULL;
	free((void*)he_keku); he_keku = NULL;
	free((void*)he); he = NULL;
	free((void*)made_kaku); made_kaku = NULL;
	free((void*)made); made = NULL;
	free((void*)no_kaku); no_kaku = NULL;
	free((void*)no); no = NULL;
	free((void*)ga2_kaku); ga2_kaku = NULL;
	free((void*)ga2); ga2 = NULL;
	free((void*)ishi_irai); ishi_irai = NULL;
	free((void*)ishi_ishi); ishi_ishi = NULL;
	free((void*)ishi_kanyuu); ishi_kanyuu = NULL;
	free((void*)ishi_ganbou); ishi_ganbou = NULL;
	free((void*)ishi_kinnshi); ishi_kinnshi = NULL;
	free((void*)ishi_sanninshou); ishi_sanninshou = NULL;
	free((void*)ishi_moushide); ishi_moushide = NULL;
	free((void*)ishi_suiryou); ishi_suiryou = NULL;
	free((void*)ishi_meirei); ishi_meirei = NULL;
	free((void*)toui); toui = NULL;
	free((void*)toui_kyoka); toui_kyoka = NULL;
	free((void*)handan_kanou); handan_kanou = NULL;
	free((void*)handan_kahi); handan_kahi = NULL;
	free((void*)handan_suiryou); handan_suiryou = NULL;
	free((void*)handan_denbun); handan_denbun = NULL;
	free((void*)handan_youtai); handan_youtai = NULL;
	free((void*)dookuten); dookuten = NULL;
	free((void*)ruijido); ruijido = NULL;
	free((void*)hbar); hbar = NULL;
	free((void*)hbar2); hbar2 = NULL;
	free((void*)branch); branch = NULL;
	free((void*)corner); corner = NULL;
	free((void*)vbar); vbar = NULL;
	free((void*)space); space = NULL;
	free((void*)space2); space2 = NULL;
#endif
}
#ifdef	_LINUX
static const char special[] = "特殊";
static const char verb[] = "動詞";
static const char adj[] = "形容詞";
static const char hantei[] = "判定詞";
static const char jodoushi[] = "助動詞";
static const char noun[] = "名詞";
static const char pronoun[] = "固有名詞";
static const char jinnmei[] = "人名";
static const char chimei[] = "地名";
static const char shijishi[] = "指示詞";
static const char adv[] = "副詞";
static const char joshi[] = "助詞";
static const char conj[] = "接続詞";
static const char rentai[] = "連体詞";
static const char kandou[] = "感動詞";
static const char settouji[] = "接頭辞";
static const char setsubiji[] = "接尾辞";
static const char yougen[] = "用言";
static const char semi_yougen[] = "準用言";
static const char noudou[] = " \224\134動"; /* 能 */
static const char kanju[] = " 間受";
static const char chokuju1[] = " 直受１";
static const char chokuju2[] = " 直受２";
static const char shieki_wo_ni[] = " 使役ヲニ";
static const char shieki_wo[] = " 使役ヲ";
static const char shieki_ni[] = " 使役ニ";
static const char shieki_ukemi[] = " 使役&受身";
static const char kanou[] = " 可\224\134"; /* 能 */
static const char sonkei[] = " 尊敬";
static const char jihatsu[] = " 自発";
static const char hyousou[] = " <\225\134層格:"; /* 表 */
static const char ga_kaku[] = "ガ格";
static const char ga[] = "ガ,";
static const char wo_kaku[] = "ヲ格";
static const char wo[] = "ヲ,";
static const char ni_kaku[] = "ニ格";
static const char ni[] = "ニ,";
static const char de_kaku[] = "デ格";
static const char de[] = "デ,";
static const char kara_kaku[] = "カラ格";
static const char kara[] = "カラ,";
static const char to_kaku[] = "ト格";
static const char to[] = "ト,";
static const char yori_kaku[] = "ヨリ格";
static const char yori[] = "ヨリ,";
static const char he_keku[] = "ヘ格";
static const char he[] = "ヘ,";
static const char made_kaku[] = "マデ格";
static const char made[] = "マデ,";
static const char no_kaku[] = "ノ格";
static const char no[] = "ノ,";
static const char ga2_kaku[] = "ガ２";
static const char ga2[] = "ガ２,";
static const char ishi_irai[] = "Modality-意思-依頼";
static const char ishi_ishi[] = "Modality-意思-意志";
static const char ishi_kanyuu[] = "Modality-意思-勧誘";
static const char ishi_ganbou[] = "Modality-意思-願望";
static const char ishi_kinnshi[] = "Modality-意思-禁止";
static const char ishi_sanninshou[] = "Modality-意思-三人称意志";
static const char ishi_moushide[] = "Modality-意思-\220\134し出"; /* 申 */
static const char ishi_suiryou[] = "Modality-意思-推量";
static const char ishi_meirei[] = "Modality-意思-命令";
static const char toui[] = "Modality-当為";
static const char toui_kyoka[] = "Modality-当為-許可";
static const char handan_kanou[] = "Modality-判断-可\224\134性"; /* 能 */
static const char handan_kahi[] = "Modality-判断-可\224\134性-不可\224\134"; /* 能 */
static const char handan_suiryou[] = "Modality-判断-推量";
static const char handan_denbun[] = "Modality-判断-伝聞";
static const char handan_youtai[] = "Modality-判断-様態";
static const char dookuten[] = "読点";
static const char ruijido[] = "並列類似度:%.3f";
static const char hbar[] = "─";
static const char hbar2[] = "──";
static const char branch[] = "┤";
static const char corner[] = "┐";
static const char vbar[] = "│";
static const char space[] = "　";
static const char space2[] = "　　";
#endif

extern int bnst_to_tag_tree(SENTENCE_DATA *sp);

/*==================================================================*/
		 char pos2symbol(char *hinshi, char *bunrui)
/*==================================================================*/
{
	init_str();

	if (!strcmp(hinshi, special)) return ' ';
	else if (!strcmp(hinshi, verb)) return 'v';
	else if (!strcmp(hinshi, adj)) return 'j';
	else if (!strcmp(hinshi, hantei)) return 'c';
	else if (!strcmp(hinshi, jodoushi)) return 'x';
	else if (!strcmp(hinshi, noun) &&
		 !strcmp(bunrui, pronoun)) return 'N';
	else if (!strcmp(hinshi, noun) &&
		 !strcmp(bunrui, jinnmei)) return 'J';
	else if (!strcmp(hinshi, noun) &&
		 !strcmp(bunrui, chimei)) return 'C';
	else if (!strcmp(hinshi, noun)) return 'n';
	else if (!strcmp(hinshi, shijishi)) return 'd';
	else if (!strcmp(hinshi, adv)) return 'a';
	else if (!strcmp(hinshi, joshi)) return 'p';
	else if (!strcmp(hinshi, conj)) return 'c';
	else if (!strcmp(hinshi, rentai)) return 'm';
	else if (!strcmp(hinshi, kandou)) return '!';
	else if (!strcmp(hinshi, settouji)) return 'p';
	else if (!strcmp(hinshi, setsubiji)) return 's';

	return '?';
}

/*==================================================================*/
		  void print_mrph(MRPH_DATA *m_ptr)
/*==================================================================*/
{
	fprintf(Outfp, "%s %s %s ", m_ptr->Goi2, m_ptr->Yomi, m_ptr->Goi);

	if (m_ptr->Hinshi >= CLASS_num) {
	fprintf(Outfp, "\n;; Hinshi number is invalid. (%d)\n", m_ptr->Hinshi);
	exit(1);
	}
	fprintf(Outfp, "%s ", Class[m_ptr->Hinshi][0].id);
	fprintf(Outfp, "%d ", m_ptr->Hinshi);
	
	if (m_ptr->Bunrui)	
	fprintf(Outfp, "%s ", Class[m_ptr->Hinshi][m_ptr->Bunrui].id);
	else
	fprintf(Outfp, "* ");
	fprintf(Outfp, "%d ", m_ptr->Bunrui);
	
	if (m_ptr->Katuyou_Kata)	
	fprintf(Outfp, "%s ", Type[m_ptr->Katuyou_Kata].name);
	else					
	fprintf(Outfp, "* ");
	fprintf(Outfp, "%d ", m_ptr->Katuyou_Kata);
	
	if (m_ptr->Katuyou_Kei) 
	fprintf(Outfp, "%s ",	
		Form[m_ptr->Katuyou_Kata][m_ptr->Katuyou_Kei].name);
	else	
	fprintf(Outfp, "* ");
	fprintf(Outfp, "%d ", m_ptr->Katuyou_Kei);
	
	fprintf(Outfp, "%s", m_ptr->Imi);
}

/*==================================================================*/
		  void print_mrph_f(MRPH_DATA *m_ptr)
/*==================================================================*/
{
	char yomi_buffer[SMALL_DATA_LEN];

	art_sprintf_1(yomi_buffer, SMALL_DATA_LEN, "(%s)", m_ptr->Yomi);
	fprintf(Outfp, "%-16.16s%-18.18s %-14.14s",
		m_ptr->Goi2, yomi_buffer,	
		Class[m_ptr->Hinshi][m_ptr->Bunrui].id);
	if (m_ptr->Katuyou_Kata)
	fprintf(Outfp, " %-14.14s %-12.12s",
		Type[m_ptr->Katuyou_Kata].name,
		Form[m_ptr->Katuyou_Kata][m_ptr->Katuyou_Kei].name);
}

/*==================================================================*/
		 void print_tags(SENTENCE_DATA *sp, int flag)
/*==================================================================*/
{
	int 	i, j;
	MRPH_DATA	*m_ptr;
	TAG_DATA	*t_ptr;

	for (i = 0, t_ptr = sp->tag_data; i < sp->Tag_num; i++, t_ptr++) {
	if (flag == 1) {
		/* 文節行 */
		if (t_ptr->bnum >= 0) {
		fprintf(Outfp, "* %d%c",	
			(sp->bnst_data + t_ptr->bnum)->dpnd_head,	
			(sp->bnst_data + t_ptr->bnum)->dpnd_type);
		if ((sp->bnst_data + t_ptr->bnum)->f) {
			fputc(' ', Outfp);
			print_feature((sp->bnst_data + t_ptr->bnum)->f, Outfp);
		}
		fputc('\n', Outfp);
		}
		fprintf(Outfp, "+ %d%c", t_ptr->dpnd_head, t_ptr->dpnd_type);
		if (t_ptr->f) {
		fputc(' ', Outfp);
		print_feature(t_ptr->f, Outfp);
		}
		fputc('\n', Outfp);
	}
	else {
		fprintf(Outfp, "%c\n", t_ptr->bnum < 0 ? '+' : '*');
	}

	for (j = 0, m_ptr = t_ptr->mrph_ptr; j < t_ptr->mrph_num; j++, m_ptr++) {
		print_mrph(m_ptr);
		if (m_ptr->f) {
		fputc(' ', Outfp);
		print_feature(m_ptr->f, Outfp);
		}
		fputc('\n', Outfp);
	}
	}
	fputs("EOS\n", Outfp);
}

/*==================================================================*/
		void print_mrphs(SENTENCE_DATA *sp, int flag)
/*==================================================================*/
{
	int 	i, j;
	MRPH_DATA	*m_ptr;
	BNST_DATA	*b_ptr;

	for (i = 0, b_ptr = sp->bnst_data; i < sp->Bnst_num; i++, b_ptr++) {
	if (flag == 1) {
		fprintf(Outfp, "* %d%c", b_ptr->dpnd_head, b_ptr->dpnd_type);
		if (b_ptr->f) {
		fprintf(Outfp, " ");
		print_feature(b_ptr->f, Outfp);
		}
		fprintf(Outfp, "\n");
	}
	else {
		fprintf(Outfp, "*\n");
	}

	for (j = 0, m_ptr = b_ptr->mrph_ptr; j < b_ptr->mrph_num; j++, m_ptr++) {
		print_mrph(m_ptr);
		if (m_ptr->f) {
		fprintf(Outfp, " ");
		print_feature(m_ptr->f, Outfp);
		}
		/* print_mrph_f(m_ptr); */
		fprintf(Outfp, "\n");
	}
	}
	fprintf(Outfp, "EOS\n");
}

/*==================================================================*/
		   void _print_bnst(TAG_DATA *ptr)
/*==================================================================*/
{
	int i;
	for (i = 0; i < ptr->mrph_num; i++)
	fprintf(Outfp, "%s", (ptr->mrph_ptr + i)->Goi2);
}

/*==================================================================*/
		void print_bnst(BNST_DATA *ptr, char *cp)
/*==================================================================*/
{
	int i;

	if (cp && ptr) {
#if 0
	if ( ptr->para_type == PARA_NORMAL ) strcpy(cp, "<P>");
	else if ( ptr->para_type == PARA_INCOMP ) strcpy(cp, "<I>");
	else				 cp[0] = '\0';
	if ( ptr->para_top_p == 1 /*TRUE*/ )
	  strcat(cp, "PARA");
	else {
		strcpy(cp, ptr->mrph_ptr->Goi2);
		for (i = 1; i < ptr->mrph_num; i++) 
		  strcat(cp, (ptr->mrph_ptr + i)->Goi2);
	}
#endif
	} else if (cp == NULL && ptr) {
	if ( ptr->para_top_p == 1 /*TRUE*/ ) {
		fprintf(Outfp, "PARA");
	} else {
		for (i = 0; i < ptr->mrph_num; i++) {
		if (OptDisplay == OPT_NORMAL) {
			fprintf(Outfp, "%s", (ptr->mrph_ptr + i)->Goi2);
		} else {	
			fprintf(Outfp, "%s%c", (ptr->mrph_ptr + i)->Goi2,	
				pos2symbol((char*)Class[(ptr->mrph_ptr + i)->Hinshi]
					   [0].id,
					   (char*)Class[(ptr->mrph_ptr + i)->Hinshi]
					   [(ptr->mrph_ptr + i)->Bunrui].id));
		}
		}
	}

	if ( ptr->para_type == PARA_NORMAL ) fprintf(Outfp, "<P>");
	else if ( ptr->para_type == PARA_INCOMP ) fprintf(Outfp, "<I>");
	if ( ptr->to_para_p == 1 /*TRUE*/ ) fprintf(Outfp, "(D)");
	}
}

/*==================================================================*/
  void print_data2ipal_corr(BNST_DATA *b_ptr, CF_PRED_MGR *cpm_ptr)
/*==================================================================*/
{
	int i, j, elem_num = 0;
	int offset;
	int flag;

	switch (cpm_ptr->cmm[0].cf_ptr->voice) {
	  case FRAME_PASSIVE_I:
	  case FRAME_CAUSATIVE_WO_NI:
	  case FRAME_CAUSATIVE_WO:
	  case FRAME_CAUSATIVE_NI:
	offset = 0;
	break;
	  default:
	offset = 1;
	break;
	}

	flag = FALSE;
	if (cpm_ptr->elem_b_num[0] == -1) {
	elem_num = 0;
	flag = TRUE;
	}
	if (flag == 1 /*TRUE*/) {
	flag = FALSE;
	for (j = 0; j < cpm_ptr->cmm[0].cf_ptr->element_num; j++)
	  if (cpm_ptr->cmm[0].result_lists_p[0].flag[j] == elem_num) {
		  fprintf(Outfp, " N%d", offset + j);
		  flag = TRUE;
	  }
	}
	if (flag == FALSE)
	  fprintf(Outfp, " *");

	for (i = 0; b_ptr->child[i]; i++) {
	flag = FALSE;
	for (j = 0; j < cpm_ptr->cf.element_num; j++)
	  if (cpm_ptr->elem_b_num[j] == i) {
		  elem_num = j;
		  flag = TRUE;
		  break;
	  }
	if (flag == 1 /*TRUE*/) {
		flag = FALSE;
		for (j = 0; j < cpm_ptr->cmm[0].cf_ptr->element_num; j++)
		  if (cpm_ptr->cmm[0].result_lists_p[0].flag[j] == elem_num) {
		  fprintf(Outfp, " N%d", offset + j);
		  flag = TRUE;
		  }
	}
	if (flag == FALSE)
	  fprintf(Outfp, " *");
	}
}

/*==================================================================*/
		void print_bnst_detail(BNST_DATA *ptr)
/*==================================================================*/
{
	int i;
	MRPH_DATA *m_ptr;
		
	init_str();

	fputc('(', Outfp);	/* 文節始り */

	if ( ptr->para_top_p == 1 /*TRUE*/ ) {
	if (ptr->child[1] &&	
		ptr->child[1]->para_key_type == PARA_KEY_N)
	  fprintf(Outfp, "noun_para");	
	else
	  fprintf(Outfp, "pred_para");
	}
	else {
	fprintf(Outfp, "%d ", ptr->num);

	/* 係り受け情報の表示 (追加:97/10/29) */

	fprintf(Outfp, "(type:%c) ", ptr->dpnd_type);

	fputc('(', Outfp);
	for (i=0, m_ptr=ptr->mrph_ptr; i < ptr->mrph_num; i++, m_ptr++) {
		fputc('(', Outfp);
		print_mrph(m_ptr);
		fprintf(Outfp, " ");
		print_feature2(m_ptr->f, Outfp);
		fputc(')', Outfp);
	}
	fputc(')', Outfp);

	fprintf(Outfp, " ");
	print_feature2(ptr->f, Outfp);

	if (OptAnalysis == OPT_DPND ||
		!check_feature(ptr->f, yougen) ||	/* 用言でない場合 */
		ptr->cpm_ptr == NULL) { 		/* 解析前 */
		fprintf(Outfp, " NIL");
	}
	else {
		fprintf(Outfp, " (");
		
		if (ptr->cpm_ptr->cmm[0].cf_ptr == NULL)
		fprintf(Outfp, "-2");	/* 格フレームにENTRYなし */
		else if ((ptr->cpm_ptr->cmm[0].cf_ptr)->cf_address == -1)
		fprintf(Outfp, "-1");	/* 格要素なし */
		else {
		fprintf(Outfp, "%s",	
			(ptr->cpm_ptr->cmm[0].cf_ptr)->cf_id);
		switch (ptr->cpm_ptr->cmm[0].cf_ptr->voice) {
		case FRAME_ACTIVE:
			fprintf(Outfp, noudou); break;
		case FRAME_PASSIVE_I:
			fprintf(Outfp, kanju); break;
		case FRAME_PASSIVE_1:
			fprintf(Outfp, chokuju1); break;
		case FRAME_PASSIVE_2:
			fprintf(Outfp, chokuju2); break;
		case FRAME_CAUSATIVE_WO_NI:
			fprintf(Outfp, shieki_wo_ni); break;
		case FRAME_CAUSATIVE_WO:
			fprintf(Outfp, shieki_wo); break;
		case FRAME_CAUSATIVE_NI:
			fprintf(Outfp, shieki_ni); break;
		case FRAME_CAUSATIVE_PASSIVE:
			fprintf(Outfp, shieki_ukemi); break;
		case FRAME_POSSIBLE:
			fprintf(Outfp, kanou); break;
		case FRAME_POLITE:
			fprintf(Outfp, sonkei); break;
		case FRAME_SPONTANE:
			fprintf(Outfp, jihatsu); break;
		default: break;
		}
		fprintf(Outfp, " (");
		print_data2ipal_corr(ptr, ptr->cpm_ptr);
		fprintf(Outfp, ")");
		}
		fprintf(Outfp, ")");

		/* ------------変更:述語素, 格形式を出力-----------------
		if (ptr->cpm_ptr != NULL &&
		ptr->cpm_ptr->cmm[0].cf_ptr != NULL &&
		(ptr->cpm_ptr->cmm[0].cf_ptr)->cf_address != -1) {
		get_ipal_frame(i_ptr,	
				   (ptr->cpm_ptr->cmm[0].cf_ptr)->cf_address);
		if (i_ptr->DATA[i_ptr->jyutugoso]) {
			fprintf(Outfp, " 述語素 %s",	
				i_ptr->DATA+i_ptr->jyutugoso);
		} else {
			fprintf(Outfp, " 述語素 nil");
		}
		fprintf(Outfp, " 格形式 (");
		for (j=0; *((i_ptr->DATA)+(i_ptr->kaku_keishiki[j]))	
				   != NULL; j++){
			fprintf(Outfp, " %s",	
				i_ptr->DATA+i_ptr->kaku_keishiki[j]);
		}
		fprintf(Outfp, ")");
		}
		------------------------------------------------------- */
	}
	}
	fputc(')', Outfp);	/* 文節終わり */
}

/*==================================================================*/
		 void print_sentence_slim(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	init_bnst_tree_property(sp);

	fputc('(', Outfp);
	for ( i=0; i<sp->Bnst_num; i++ )
	print_bnst(&(sp->bnst_data[i]), NULL);
	fputc(')', Outfp);
	fputc('\n', Outfp);
}

/*====================================================================
				   行列表示
====================================================================*/

/*==================================================================*/
void print_M_bnst(SENTENCE_DATA *sp, int b_num, int max_length, int *para_char)
/*==================================================================*/
{
	BNST_DATA *ptr = &(sp->bnst_data[b_num]);
	int i, len, space, comma_p;
	char tmp[BNST_LENGTH_MAX], *cp = tmp;

	init_str();

	if ( ptr->mrph_num == 1 ) {
	art_strcpy(tmp, BNST_LENGTH_MAX, ptr->mrph_ptr->Goi2);
	comma_p = FALSE;
	} else {
	art_strcpy(tmp, BNST_LENGTH_MAX, ptr->mrph_ptr->Goi2);
	for (i = 1; i < (ptr->mrph_num - 1); i++)	
	  art_strcat(tmp, BNST_LENGTH_MAX, (ptr->mrph_ptr + i)->Goi2);

	if (!strcmp((const char*)Class[(ptr->mrph_ptr + ptr->mrph_num - 1)->Hinshi][0].id,
			special) &&
		!strcmp((const char*)Class[(ptr->mrph_ptr + ptr->mrph_num - 1)->Hinshi]
			[(ptr->mrph_ptr + ptr->mrph_num - 1)->Bunrui].id,	
			dookuten)) {
		art_strcat(tmp, BNST_LENGTH_MAX, ",");
		comma_p = TRUE;
	} else {
		art_strcat(tmp, BNST_LENGTH_MAX, (ptr->mrph_ptr + ptr->mrph_num - 1)->Goi2);
		comma_p = FALSE;			
	}
	}

	space = ptr->para_key_type ?
	  max_length-(sp->Bnst_num-b_num-1)*3-2 : max_length-(sp->Bnst_num-b_num-1)*3;
	len = comma_p ? 
	  ptr->length - 1 : ptr->length;

	if ( len > space ) {
	if ( (space%2) != (len%2) ) {
		cp += len + 1 - space;
		fputc(' ', Outfp);
	} else
	  cp += len - space;
	} else
	  for ( i=0; i<space-len; i++ ) fputc(' ', Outfp);

	if ( ptr->para_key_type ) {
	fprintf(Outfp, "%c>", 'a'+ (*para_char));
	(*para_char)++;
	}
	fprintf(Outfp, "%s", cp);
}

/*==================================================================*/
		void print_line(int length, int flag)
/*==================================================================*/
{
	int i;
	for ( i=0; i<(length-1); i++ ) fputc('-', Outfp);
	flag ? fputc(')', Outfp) : fputc('-', Outfp);
	fputc('\n', Outfp);
}

/*==================================================================*/
	 void print_matrix(SENTENCE_DATA *sp, int type, int key_pos)
/*==================================================================*/
{
	int i, j, length;
	int over_flag = 0;
	int max_length = 0;
	int para_char = 0;	   /* para_key の表示用 */
	PARA_DATA *ptr;

	for ( i=0; i<sp->Bnst_num; i++ )
	for ( j=0; j<sp->Bnst_num; j++ )
		path_matrix[i][j] = 0;
	
	/* パスのマーク付け(PARA) */

	if (type == PRINT_PARA) {
	for ( i=0; i<sp->Para_num; i++ ) {
		ptr = &sp->para_data[i];

		if (ptr->max_score < 0.0) continue;
		/* statusがxでもスコアがあれば参考のため表示 */

		for ( j=ptr->key_pos+1; j<=ptr->jend_pos; j++ )
		path_matrix[ptr->max_path[j-ptr->key_pos-1]][j] =
			path_matrix[ptr->max_path[j-ptr->key_pos-1]][j] ?
			-1 : 'a' + i;
	}
	}

	/* 長さの計算 */

	for ( i=0; i<sp->Bnst_num; i++ ) {
	length = sp->bnst_data[i].length + (sp->Bnst_num-i-1)*3;
		if ( sp->bnst_data[i].para_key_type ) length += 2;
	if ( max_length < length )	  max_length = length;
	}
	
	/* 印刷用の処理 */

	if ( 0 ) {
	if ( PRINT_WIDTH < sp->Bnst_num*3 ) {
		over_flag = 1;
		sp->Bnst_num = PRINT_WIDTH/3;
		max_length = PRINT_WIDTH;
	} else if ( PRINT_WIDTH < max_length ) {
		max_length = PRINT_WIDTH;
	}
	}

	if (type == PRINT_PARA)
	fprintf(Outfp, "<< PARA MATRIX >>\n");
	else if (type == PRINT_DPND)
	fprintf(Outfp, "<< DPND MATRIX >>\n");
	else if (type == PRINT_MASK)
	fprintf(Outfp, "<< MASK MATRIX >>\n");
	else if (type == PRINT_QUOTE)
	fprintf(Outfp, "<< QUOTE MATRIX >>\n");
	else if (type == PRINT_RSTR)
	fprintf(Outfp, "<< RESTRICT MATRIX for PARA RELATION>>\n");
	else if (type == PRINT_RSTD)
	fprintf(Outfp, "<< RESTRICT MATRIX for DEPENDENCY STRUCTURE>>\n");
	else if (type == PRINT_RSTQ)
	fprintf(Outfp, "<< RESTRICT MATRIX for QUOTE SCOPE>>\n");

	print_line(max_length, over_flag);
	for ( i=0; i<(max_length-sp->Bnst_num*3); i++ ) fputc(' ', Outfp);
	for ( i=0; i<sp->Bnst_num; i++ ) fprintf(Outfp, "%2d ", i);
	fputc('\n', Outfp);
	print_line(max_length, over_flag);

	for ( i=0; i<sp->Bnst_num; i++ ) {
	print_M_bnst(sp, i, max_length, &para_char);
	for ( j=i+1; j<sp->Bnst_num; j++ ) {

		if (type == PRINT_PARA) {
		fprintf(Outfp, "%2d", match_matrix[i][j]);
		} else if (type == PRINT_DPND) {
		if (Dpnd_matrix[i][j] == 0)
			fprintf(Outfp, " -");
		else
			fprintf(Outfp, " %c", (char)Dpnd_matrix[i][j]);
		
		} else if (type == PRINT_MASK) {
		fprintf(Outfp, "%2d", Mask_matrix[i][j]);

		} else if (type == PRINT_QUOTE) {
		fprintf(Outfp, "%2d", Quote_matrix[i][j]);

		} else if (type == PRINT_RSTR ||	
			   type == PRINT_RSTD ||
			   type == PRINT_RSTQ) {
		if (j <= key_pos)	
			fprintf(Outfp, "--");
		else if (key_pos < i)
			fprintf(Outfp, " |");
		else
			fprintf(Outfp, "%2d", restrict_matrix[i][j]);
		}

		switch(path_matrix[i][j]) {
		  case	0:	fputc(' ', Outfp); break;
		  case -1:	fputc('*', Outfp); break;
		  default:	fputc(path_matrix[i][j], Outfp); break;
		}
	}
	fputc('\n', Outfp);
	}

	print_line(max_length, over_flag);
	
	if (type == PRINT_PARA) {
	for (i = 0; i < sp->Para_num; i++) {
		fprintf(Outfp, "%c(%c):%4.1f(%4.1f) ",	
			sp->para_data[i].para_char, 
			sp->para_data[i].status,	
			sp->para_data[i].max_score,
			sp->para_data[i].pure_score);
	}
	fputc('\n', Outfp);
	}
}

/*==================================================================*/
	void assign_para_similarity_feature(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	char buffer[DATA_LEN];

	init_str();

	for (i = 0; i < sp->Para_num; i++) {
	art_sprintf_1(buffer, DATA_LEN, ruijido, sp->para_data[i].max_score);
	assign_cfeature(&(sp->bnst_data[sp->para_data[i].key_pos].f), buffer);
	}
}

/*====================================================================
					並列構造間の関係表示
====================================================================*/

/*==================================================================*/
void print_para_manager(SENTENCE_DATA *sp, PARA_MANAGER *m_ptr, int level)
/*==================================================================*/
{
	int i;
	
	for (i = 0; i < level * 5; i++)
	fputc(' ', Outfp);

	for (i = 0; i < m_ptr->para_num; i++)
	fprintf(Outfp, " %c", sp->para_data[m_ptr->para_data_num[i]].para_char);
	fputc(':', Outfp);

	for (i = 0; i < m_ptr->part_num; i++) {
	if (m_ptr->start[i] == m_ptr->end[i]) {
		fputc('(', Outfp);
		print_bnst(&sp->bnst_data[m_ptr->start[i]], NULL);
		fputc(')', Outfp);
	} else {
		fputc('(', Outfp);
		print_bnst(&sp->bnst_data[m_ptr->start[i]], NULL);
		fputc('-', Outfp);
		print_bnst(&sp->bnst_data[m_ptr->end[i]], NULL);
		fputc(')', Outfp);
	}
	}
	fputc('\n', Outfp);

	for (i = 0; i < m_ptr->child_num; i++)
	print_para_manager(sp, m_ptr->child[i], level+1);
}

/*==================================================================*/
		 void print_para_relation(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;
	
	for (i = 0; i < sp->Para_M_num; i++)
	if (sp->para_manager[i].parent == NULL)
		print_para_manager(sp, &sp->para_manager[i], 0);
}

/*====================================================================
					木構造表示(from JK)
====================================================================*/
static int max_width;			/* 木の最大幅 */

/*==================================================================*/
			   int mylog(int n)
/*==================================================================*/
{
	int i, num = 1;
	for (i=0; i<n; i++)
	num = num*2;
	return(num);
}

/*==================================================================*/
	   void calc_self_space(BNST_DATA *ptr, int depth2)
/*==================================================================*/
{
	if (ptr->para_top_p == 1 /*TRUE*/)	
	ptr->space = 4;
	else if (OptDisplay == OPT_NORMAL)
	ptr->space = ptr->length;
	else
	ptr->space = ptr->length + ptr->mrph_num; /* *4 */

	if (ptr->para_type == PARA_NORMAL ||	
	ptr->para_type == PARA_INCOMP ||
	ptr->to_para_p == 1 /*TRUE*/)
	ptr->space += 1;
	ptr->space += (depth2-1)*8;
}

/*==================================================================*/
	   void calc_tree_width(BNST_DATA *ptr, int depth2)
/*==================================================================*/
{
	int i;
	
	calc_self_space(ptr, depth2);

	if ( ptr->space > max_width )
	max_width = ptr->space;

	if ( ptr->child[0] )
	for ( i=0; ptr->child[i]; i++ )
		calc_tree_width(ptr->child[i], depth2+1);
}

/*==================================================================*/
void show_link(int depth, char *ans_flag, char para_type, char to_para_p)
/*==================================================================*/
{
	int i;
	
	init_str();

	if (depth != 1) {

	/* 親への枝 (兄弟を考慮) */

	if (para_type == PARA_NORMAL || 
		para_type == PARA_INCOMP ||
		to_para_p == 1 /*TRUE*/)
		fprintf(Outfp, hbar);
	else	
		fprintf(Outfp, hbar2);

	if (ans_flag[depth-1] == '1')	
		fprintf(Outfp, branch);
	else	
		fprintf(Outfp, corner);

	fprintf(Outfp, space);

	/* 祖先の兄弟の枝 */

	for (i = depth - 1; i > 1; i--) {
		fprintf(Outfp, space2);
		if (ans_flag[i-1] == '1')	
		fprintf(Outfp, vbar);
		else	
		fprintf(Outfp, space);
		fprintf(Outfp, space);
	}
	}
}

/*==================================================================*/
 void show_self(BNST_DATA *ptr, int depth, char *ans_flag_p, int flag)
/*==================================================================*/
{
	/*	
	   depth は自分の深さ(根が1)

	   ans_flag は自分と祖先が最後の子かどうかの履歴
	   深さnの祖先(または自分)が最後の子であれば ans_flag[n-1] が '0'
	   そうでなければ '1'(この場合枝の描画が必要)
	*/

	int i, j;
	char ans_flag[BNST_MAX+1];

	init_str();

	if (ans_flag_p) {
	art_strncpy(ans_flag, BNST_MAX+1, ans_flag_p, BNST_MAX);
	} else {
	ans_flag[0] = '0';	/* 最初に呼ばれるとき */
	}

	if (ptr->child[0]) {
	for (i = 0; ptr->child[i]; i++);

	/* 最後の子は ans_flag を 0 に */	
	ans_flag[depth] = '0';
	show_self(ptr->child[i-1], depth+1, ans_flag, 0);

	if (i > 1) {
		/* 他の子は ans_flag を 1 に */ 
		ans_flag[depth] = '1';
		for (j = i - 2; j > 0; j--) {
		show_self(ptr->child[j], depth+1, ans_flag, 0);
		}

		/* flag: 1: ─PARA 2: -<P>PARA */
		if (ptr->para_top_p == 1 /*TRUE*/ &&	
		ptr->para_type == PARA_NIL &&
		ptr->to_para_p == FALSE) {
		show_self(ptr->child[0], depth+1, ans_flag, 1);
		} else if (ptr->para_top_p == 1 /*TRUE*/) {
		show_self(ptr->child[0], depth+1, ans_flag, 2);
		} else {
		show_self(ptr->child[0], depth+1, ans_flag, 0);
		}
	}
	}

	calc_self_space(ptr, depth);
	if ( ptr->para_top_p == FALSE ) {
	for (i = 0; i < max_width - ptr->space; i++)	
	  fputc(' ', Outfp);
	}
	print_bnst(ptr, NULL);
	
	if (flag == 0) {
	show_link(depth, ans_flag, ptr->para_type, ptr->to_para_p);
	if (OptExpress == OPT_TREEF) {
		print_some_feature(ptr->f, Outfp);
	}
	fputc('\n', Outfp);
	} else if ( flag == 1 ) {
	fprintf(Outfp, hbar);
	} else if ( flag == 2 ) {
	fprintf(Outfp, "-");
	}
}

/*==================================================================*/
	 void show_sexp(BNST_DATA *ptr, int depth, int pars)
/*==================================================================*/
{
	int i;

	for (i = 0; i < depth; i++) fputc(' ', Outfp);
	fprintf(Outfp, "(");

	if ( ptr->para_top_p == 1 /*TRUE*/ ) {
	if (ptr->child[1] &&	
		ptr->child[1]->para_key_type == PARA_KEY_N)
	  fprintf(Outfp, "(noun_para"); 
	else
	  fprintf(Outfp, "(pred_para");

	if (ptr->child[0]) {
		fputc('\n', Outfp);
		i = 0;
		while (ptr->child[i+1] && ptr->child[i+1]->para_type != PARA_NIL) {
		/* <P>の最後以外 */
		/* UCHI fputc(',', Outfp); */
		show_sexp(ptr->child[i], depth + 3, 0); i ++;
		}
		if (ptr->child[i+1]) { /* その他がある場合 */
		/* <P>の最後 */
		/* UCHI fputc(',', Outfp); */
		show_sexp(ptr->child[i], depth + 3, 1); i ++;
		/* その他の最後以外 */
		while (ptr->child[i+1]) {
			/* UCHI fputc(',', Outfp); */
			show_sexp(ptr->child[i], depth + 3, 0); i ++;
		}
		/* その他の最後 */
		/* UCHI fputc(',', Outfp); */
		show_sexp(ptr->child[i], depth + 3, pars + 1);
		}
		else {
		/* <P>の最後 */
		/* UCHI fputc(',', Outfp); */
		show_sexp(ptr->child[i], depth + 3, pars + 1 + 1);
		}
	}
	}
	else {
	
	print_bnst_detail(ptr);

	if (ptr->child[0]) {
		fputc('\n', Outfp);
		for ( i=0; ptr->child[i+1]; i++ ) {
		/* UCHI fputc(',', Outfp); */
		show_sexp(ptr->child[i], depth + 3, 0);
		}
		/* UCHI fputc(',', Outfp); */
		show_sexp(ptr->child[i], depth + 3, pars + 1);
	} else {
		for (i = 0; i < pars + 1; i++) fputc(')', Outfp);
		fputc('\n', Outfp);
	}
	}
}

/*==================================================================*/
		void print_kakari(SENTENCE_DATA *sp, int type)
/*==================================================================*/
{
	/* 依存構造木の表示 */

	/* tag単位のtreeを描くとき */
	if (type == OPT_TREE || type == OPT_TREEF) {
	max_width = 0;

	calc_tree_width((BNST_DATA *)(sp->tag_data + sp->Tag_num -1), 1);
	show_self((BNST_DATA *)(sp->tag_data + sp->Tag_num -1), 1, NULL, 0);
	}
	/* 文節のtreeを描くとき */
	else if (type == OPT_NOTAGTREE) {
	max_width = 0;

	calc_tree_width((sp->bnst_data + sp->Bnst_num -1), 1);
	show_self((sp->bnst_data + sp->Bnst_num -1), 1, NULL, 0);
	}
	else if (type == OPT_SEXP) {
	show_sexp((sp->bnst_data + sp->Bnst_num -1), 0, 0);
	}

	fprintf(Outfp, "EOS\n");
}


/*====================================================================
				  チェック用
====================================================================*/

/*==================================================================*/
		  void check_bnst(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int 	i, j;
	BNST_DATA	*ptr;
	char b_buffer[BNST_LENGTH_MAX];

	init_str();

	for (i = 0; i < sp->Bnst_num; i++) {
	ptr = &sp->bnst_data[i];
	
	b_buffer[0] = '\0';

	for (j = 0; j < ptr->mrph_num; j++) {
		/* buffer overflow */
		if (strlen(b_buffer) + strlen((ptr->mrph_ptr + j)->Goi2) + 4 > BNST_LENGTH_MAX) {
		break;
		}

		if (ptr->mrph_ptr + j == ptr->head_ptr) {
		art_strcat(b_buffer, BNST_LENGTH_MAX, "[");
		art_strcat(b_buffer, BNST_LENGTH_MAX, (ptr->mrph_ptr + j)->Goi2);
		art_strcat(b_buffer, BNST_LENGTH_MAX, "]");
		}
		else {
		art_strcat(b_buffer, BNST_LENGTH_MAX, (ptr->mrph_ptr + j)->Goi2);
		}
		art_strcat(b_buffer, BNST_LENGTH_MAX, " ");
	}
	fprintf(Outfp, "%-20s", b_buffer);

	print_feature(ptr->f, Outfp);

	if (check_feature(ptr->f, yougen) ||
		check_feature(ptr->f, semi_yougen)) {

		fprintf(Outfp, hyousou);
		if (ptr->SCASE_code[case2num(ga_kaku)])
		  fprintf(Outfp, ga);
		if (ptr->SCASE_code[case2num(wo_kaku)])
		  fprintf(Outfp, wo);
		if (ptr->SCASE_code[case2num(ni_kaku)])
		  fprintf(Outfp, ni);
		if (ptr->SCASE_code[case2num(de_kaku)])
		  fprintf(Outfp, de);
		if (ptr->SCASE_code[case2num(kara_kaku)])
		  fprintf(Outfp, kara);
		if (ptr->SCASE_code[case2num(to_kaku)])
		  fprintf(Outfp, to);
		if (ptr->SCASE_code[case2num(yori_kaku)])
		  fprintf(Outfp, yori);
		if (ptr->SCASE_code[case2num(he_keku)])
		  fprintf(Outfp, he);
		if (ptr->SCASE_code[case2num(made_kaku)])
		  fprintf(Outfp, made);
		if (ptr->SCASE_code[case2num(no_kaku)])
		  fprintf(Outfp, no);
		if (ptr->SCASE_code[case2num(ga2_kaku)])
		  fprintf(Outfp, ga2);
		fprintf(Outfp, ">");
	}

	fputc('\n', Outfp);
	}
}

/*==================================================================*/
		 void print_result(SENTENCE_DATA *sp)
/*==================================================================*/
{
	char *date_p, time_string[64];
	time_t t;
	struct tm *tms;

	/* 時間の取得 */
	t = time(NULL);
	tms = art_localtime(&t);
	if (!strftime(time_string, 64, "%Y/%m/%d", tms))
	time_string[0] = '\0';
	
	/* PS出力の場合
	   dpnd_info_to_bnst(&(tm->dpnd));
	   make_dpnd_tree();
	   print_kakari2ps();
	   return;
	*/	

	/* 既解析へのパターンマッチで, マッチがなければ出力しない
	   if (OptAnalysis == OPT_AssignF && !PM_Memo[0]) return;
	*/

	/* ヘッダの出力 */

	if (sp->Comment) {
	fprintf(Outfp, "%s", sp->Comment);
	} else {
	fprintf(Outfp, "# S-ID:%d", sp->Sen_num);
	}

	if (OptInput == OPT_RAW) {
	if ((date_p = (char *)art_getenv("DATE")) != NULL)
		fprintf(Outfp, " KNP:%s", date_p);
	else if (time_string[0])
		fprintf(Outfp, " KNP:%s", time_string);
	}

	/* エラーがあれば、エラーの内容 */
	if (ErrorComment) {
	fprintf(Outfp, " ERROR:%s", ErrorComment);
	free(ErrorComment);
	ErrorComment = NULL;
	}

	if (PM_Memo[0]) {
	if (sp->Comment && strstr(sp->Comment, "MEMO")) {
		fprintf(Outfp, "%s", PM_Memo);
	} else {
		fprintf(Outfp, " MEMO:%s", PM_Memo);
	}	
	}
	fprintf(Outfp, "\n");

	/* 解析結果のメインの出力 */

	if (OptExpress == OPT_TAB) {
	sm2feature(sp);
	print_tags(sp, 1);
	}
	else if (OptExpress == OPT_NOTAG) {
	print_mrphs(sp, 1);
	}
	else if (OptExpress == OPT_NOTAGTREE) {
	/* tree出力 */
	make_dpnd_tree(sp);
	print_kakari(sp, OptExpress);
	}
	else {
	/* tree出力 */
	make_dpnd_tree(sp);
	bnst_to_tag_tree(sp); /* タグ単位 */
	print_kakari(sp, OptExpress);
	}
}

/*==================================================================*/
	void push_entity(char ***list, char *key, int count, int *max)
/*==================================================================*/
{
	if (*max == 0) {
	*max = ALLOCATION_STEP;
	*list = (char **)malloc_data(sizeof(char *)*(*max), "push_entity");
	}
	else if (*max <= count) {
	*list = (char **)realloc_data(*list, sizeof(char *)*(*max <<= 1), "push_entity");
	}

	*(*list+count) = key;
}

/*==================================================================*/
		  void prepare_entity(BNST_DATA *bp)
/*==================================================================*/
{
	int count = 0, max = 0, i, flag = 0;
	char *cp, **list = NULL, *str;

	init_str();

	/* モダリティ */
	flag = 0;
	for (i = 0; i < bp->mrph_num; i++) {
	if ((flag & 0x0001) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_irai)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0001;
	}
	if ((flag & 0x0002) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_ishi)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0002;
	}
	if ((flag & 0x0004) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_kanyuu)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0004;
	}
	if ((flag & 0x0008) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_ganbou)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0008;
	}
	if ((flag & 0x0010) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_kinnshi)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0010;
	}
	if ((flag & 0x0020) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_sanninshou)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0020;
	}
	if ((flag & 0x0040) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_moushide)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0040;
	}
	if ((flag & 0x0080) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_suiryou)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0080;
	}
	if ((flag & 0x0100) && (cp = check_feature((bp->mrph_ptr+i)->f, ishi_meirei)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0100;
	}
	if ((flag & 0x0200) && (cp = check_feature((bp->mrph_ptr+i)->f, toui)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0200;
	}
	if ((flag & 0x0400) && (cp = check_feature((bp->mrph_ptr+i)->f, toui_kyoka)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0400;
	}
	if ((flag & 0x0800) && (cp = check_feature((bp->mrph_ptr+i)->f, handan_kanou)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x0800;
	}
	if ((flag & 0x1000) && (cp = check_feature((bp->mrph_ptr+i)->f, handan_kahi)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x1000;
	}
	if ((flag & 0x2000) && (cp = check_feature((bp->mrph_ptr+i)->f, handan_suiryou)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x2000;
	}
	if ((flag & 0x4000) && (cp = check_feature((bp->mrph_ptr+i)->f, handan_denbun)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x4000;
	}
	if ((flag & 0x8000) && (cp = check_feature((bp->mrph_ptr+i)->f, handan_youtai)) == 0) {
		push_entity(&list, cp, count++, &max);
		flag |= 0x8000;
	}
	}

	/* 出力するfeatureがあれば出力 */
	if (count) {
	int i, len = 0;
	for (i = 0; i < count; i++) {
		len += (int)strlen(list[i])+1;
	}
	int n;
	str = (char *)malloc_data(n = sizeof(char)*(len+2), "print_entity");
	art_strcpy(str, n, "C:");
	for (i = 0; i < count; i++) {
		if (i != 0) art_strcat(str, n, " ");
		art_strcat(str, n, list[i]);
	}
	assign_cfeature(&(bp->f), str);
	free(str);
	free(list);
	}
}

/*==================================================================*/
		  void prepare_all_entity(SENTENCE_DATA *sp)
/*==================================================================*/
{
	int i;

	for (i = 0; i < sp->Bnst_num; i++) {
	prepare_entity(sp->bnst_data+i);
	}
}

/*====================================================================
							   END
====================================================================*/
