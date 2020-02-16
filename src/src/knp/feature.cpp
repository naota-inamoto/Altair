/*====================================================================

				 FEATURE����

											   S.Kurohashi 96. 7. 4

	$Id: feature.c,v 1.42.2.2 2005/09/05 12:17:07 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

/*
  FEATURE�̏����ɂ͎��̂R��ނ�����

	(1) �t�@�C��(S���܂��͕�����) ==�R�s�[==> ���[���\����

	(2) ���[���\���� ==�t�^==> �`�ԑf�܂��͕��ߍ\����
			<��:��>��<��:�c>�Ƃ���FEATURE�ւ̏㏑�� (�Ȃ���ΐV�K)
				<^��>��<��:�c>�̍폜 (�Ȃ���Ζ���)
		<&��>�͊֐��ďo
			&�\�w:�t�^ -- ���������ɂ��\�w�i�t�^
			&�\�w:�폜 -- ���ׂĂ̕\�w�i�폜
			&�\�w:���i -- ���i�t�^
			&�\�w:^���i -- ���i�폜
			&MEMO:�� -- MEMO�ւ̏�������

	(3) ���[���\���� <==�ƍ�==> �`�ԑf�܂��͕��ߍ\����
			<��>��<��:�c>�Ƃ���FEATURE�������OK
			<^��>��<��:�c>�Ƃ���FEATURE���Ȃ����OK
			<&��>�͊֐��ďo
			&�L�p���J -- �\�L���L��,�p����,����,�J�^�J�i (�`�ԑf)
			&���� -- �\�L������ (�`�ԑf)
				&�\�w:���i -- ���i������ (����)
				&�\�w:�ƍ� -- �W�̕\�w�i����ɂ��� (�W��)
			&D:n -- �\���̊Ԃ�����n�ȓ� (�W��)
			&���x��:�� -- �󂪌W�ȏ� (�W��)
			&���x��:l -- ���g��l�ȏ� (�W��)
			&�W��:�� -- �W�Ɂ� (�W��)

	�� �v���O�������Ō`�ԑf�܂��͕��ߍ\���̂�FEATURE��^����
	�ꍇ��(2)�̂Ȃ��� assign_cfeature ��p����D

	�� �v���O�������Ō`�ԑf�܂��͕��ߍ\���̂�����FEATURE������
	���ǂ����𒲂ׂ�ꍇ��(3)�̂Ȃ��� check_feature ��p����D
*/

#ifdef	_WINDOWS
static char* hyousou_fuyo = NULL;
static char* hyousou_del = NULL;
static char* hyousou_chiruda = NULL;
static char* hyousou = NULL;
static char* hinnshi_henkou = NULL;
static char* imiso_fuyo = NULL;
static char* denpan = NULL;
static char* denpan_format = NULL;
static char* hin_other = NULL;
static char* hin_format = NULL;
static char* alpah_digit = NULL;
static char* kanji = NULL;
static char* kana_kanji = NULL;
static char* hiragana = NULL;
static char* matsubi_hiragana = NULL;
static char* matsubi_mojiretsu = NULL;
static char* katakana = NULL;
static char* number = NULL;
static char* alpha_symbol = NULL;
static char* symbol = NULL;
static char* mixed = NULL;
static char* one_character = NULL;
static char* sememe = NULL;
static char* clause_sememe = NULL;
static char* clause_all_sememes = NULL;
static char* morpheme_length = NULL;
static char* morpheme_end = NULL;
static char* collation = NULL;
static char* modif = NULL;
static char* levelc = NULL;
static char* strong = NULL;
static char* clause_boundary = NULL;
static char* modif_side = NULL;
static char* independence_word = NULL;
static char* string_collation = NULL;
static char* voice = NULL;
static char* active_voice = NULL;
static char* passive_voice = NULL;
static char* employment = NULL;
static char hyousou_fuyo_2[] = "&�\�w:�t�^";
static char hyousou_del_2[] = "&�\�w:�폜";
static char hyousou_chiruda_2[] = "&�\�w:^";
static char hyousou_2[] = "&�\�w:";
static char hinnshi_henkou_2[] = "&�i���ύX:";
static char imiso_fuyo_2[] = "&�Ӗ��f�t�^:";
static char denpan_2[] = "&�`��:";
static char denpan_format_2[] = "&�`��:%d:%s";
static char hin_other_2[] = "�i�B-���̑�";
static char hin_format_2[] = "�i�B-%s";
static char alpah_digit_2[] = "&�L�p���J";
static char kanji_2[] = "&����";
static char kana_kanji_2[] = "&���Ȋ���";
static char hiragana_2[] = "&�Ђ炪��";
static char matsubi_hiragana_2[] = "&�����Ђ炪��";
static char matsubi_mojiretsu_2[] = "&����������:";
static char katakana_2[] = "&�J�^�J�i";
static char number_2[] = "&����";
static char alpha_symbol_2[] = "&�p�L��";
static char symbol_2[] = "&�L��";
static char mixed_2[] = "&����";
static char one_character_2[] = "&�ꕶ��";
static char sememe_2[] = "&�Ӗ��f:";
static char clause_sememe_2[] = "&���߈Ӗ��f:";
static char clause_all_sememes_2[] = "&���ߑS�Ӗ��f:";
static char morpheme_length_2[] = "&�`�ԑf��:";
static char morpheme_end_2[] = "&�`�ԑf����:";
static char collation_2[] = "�ƍ�";
static char modif_2[] = "�W:";
static char levelc_2[] = "&���x��:";
static char strong_2[] = "��";
static char clause_boundary_2[] = "&�ߋ��E:";
static char modif_side_2[] = "&�W��:";
static char independence_word_2[] = "&�������v";
static char string_collation_2[] = "&������ƍ�:";
static char voice_2[] = "&��:";
static char active_voice_2[] = "�\��";
static char passive_voice_2[] = "��";
static char employment_2[] = "�g��";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	hyousou_fuyo = toStringEUC(hyousou_fuyo_2);
	hyousou_del = toStringEUC(hyousou_del_2);
	hyousou_chiruda = toStringEUC(hyousou_chiruda_2);
	hyousou = toStringEUC(hyousou_2);
	hinnshi_henkou = toStringEUC(hinnshi_henkou_2);
	imiso_fuyo = toStringEUC(imiso_fuyo_2);
	denpan = toStringEUC(denpan_2);
	denpan_format = toStringEUC(denpan_format_2);
	hin_other = toStringEUC(hin_other_2);
	hin_format = toStringEUC(hin_format_2);
	alpah_digit = toStringEUC(alpah_digit_2);
	kanji = toStringEUC(kanji_2);
	kana_kanji = toStringEUC(kana_kanji_2);
	hiragana = toStringEUC(hiragana_2);
	matsubi_hiragana = toStringEUC(matsubi_hiragana_2);
	matsubi_mojiretsu = toStringEUC(matsubi_mojiretsu_2);
	katakana = toStringEUC(katakana_2);
	number = toStringEUC(number_2);
	alpha_symbol = toStringEUC(alpha_symbol_2);
	symbol = toStringEUC(symbol_2);
	mixed = toStringEUC(mixed_2);
	one_character = toStringEUC(one_character_2);
	sememe = toStringEUC(sememe_2);
	clause_sememe = toStringEUC(clause_sememe_2);
	clause_all_sememes = toStringEUC(clause_all_sememes_2);
	morpheme_length = toStringEUC(morpheme_length_2);
	morpheme_end = toStringEUC(morpheme_end_2);
	collation = toStringEUC(collation_2);
	modif = toStringEUC(modif_2);
	levelc = toStringEUC(levelc_2);
	strong = toStringEUC(strong_2);
	clause_boundary = toStringEUC(clause_boundary_2);
	modif_side = toStringEUC(modif_side_2);
	independence_word = toStringEUC(independence_word_2);
	string_collation = toStringEUC(string_collation_2);
	voice = toStringEUC(voice_2);
	active_voice = toStringEUC(active_voice_2);
	passive_voice = toStringEUC(passive_voice_2);
	employment = toStringEUC(employment_2);
#endif
}
void final_str_feature() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(hyousou_fuyo); hyousou_fuyo = NULL;
	free(hyousou_del); hyousou_del = NULL;
	free(hyousou_chiruda); hyousou_chiruda = NULL;
	free(hyousou); hyousou = NULL;
	free(hinnshi_henkou); hinnshi_henkou = NULL;
	free(imiso_fuyo); imiso_fuyo = NULL;
	free(denpan); denpan = NULL;
	free(denpan_format); denpan_format = NULL;
	free(hin_other); hin_other = NULL;
	free(hin_format); hin_format = NULL;
	free(alpah_digit); alpah_digit = NULL;
	free(kanji); kanji = NULL;
	free(kana_kanji); kana_kanji = NULL;
	free(hiragana); hiragana = NULL;
	free(matsubi_hiragana); matsubi_hiragana = NULL;
	free(matsubi_mojiretsu); matsubi_mojiretsu = NULL;
	free(katakana); katakana = NULL;
	free(number); number = NULL;
	free(alpha_symbol); alpha_symbol = NULL;
	free(symbol); symbol = NULL;
	free(mixed); mixed = NULL;
	free(one_character); one_character = NULL;
	free(sememe); sememe = NULL;
	free(clause_sememe); clause_sememe = NULL;
	free(clause_all_sememes); clause_all_sememes = NULL;
	free(morpheme_length); morpheme_length = NULL;
	free(morpheme_end); morpheme_end = NULL;
	free(collation); collation = NULL;
	free(modif); modif = NULL;
	free(levelc); levelc = NULL;
	free(strong); strong = NULL;
	free(clause_boundary); clause_boundary = NULL;
	free(modif_side); modif_side = NULL;
	free(independence_word); independence_word = NULL;
	free(string_collation); string_collation = NULL;
	free(voice); voice = NULL;
	free(active_voice); active_voice = NULL;
	free(passive_voice); passive_voice = NULL;
	free(employment); employment = NULL;
#endif
}
#ifdef	_LINUX
static char hyousou_fuyo[] = "&\255\134�w:�t�^"; /* �\ */
static char hyousou_del[] = "&\255\134�w:�폜"; /* �\ */
static char hyousou_chiruda[] = "&\255\134�w:^"; /* �\ */
static char hyousou[] = "&\255\134�w:"; /* �\ */
static char hinnshi_henkou[] = "&�i���ύX:";
static char imiso_fuyo[] = "&�Ӗ��f�t�^:";
static char denpan[] = "&�`��:";
static char denpan_format[] = "&�`��:%d:%s";
static char hin_other[] = "�i�B-���̑�";
static char hin_format[] = "�i�B-%s";
static char alpah_digit[] = "&�L�p���J";
static char kanji[] = "&����";
static char kana_kanji[] = "&���Ȋ���";
static char hiragana[] = "&�Ђ炪��";
static char matsubi_hiragana[] = "&�����Ђ炪��";
static char matsubi_mojiretsu[] = "&����������:";
static char katakana[] = "&�J�^�J�i";
static char number[] = "&����";
static char alpha_symbol[] = "&�p�L��";
static char symbol[] = "&�L��";
static char mixed[] = "&����";
static char one_character[] = "&�ꕶ��";
static char sememe[] = "&�Ӗ��f:";
static char clause_sememe[] = "&���߈Ӗ��f:";
static char clause_all_sememes[] = "&���ߑS�Ӗ��f:";
static char morpheme_length[] = "&�`�ԑf��:";
static char morpheme_end[] = "&�`�ԑf����:";
static char collation[] = "�ƍ�";
static char modif[] = "�W:";
static char levelc[] = "&���x��:";
static char strong[] = "��";
static char clause_boundary[] = "&�ߋ��E:";
static char modif_side[] = "&�W��:";
static char independence_word[] = "&�������v";
static char string_collation[] = "&������ƍ�:";
static char voice[] = "&��:";
static char active_voice[] = "\224\134��"; /* �\ */
static char passive_voice[] = "��";
static char employment[] = "�g��";
#endif

/*==================================================================*/
		void print_one_feature(char *cp, FILE *filep)
/*==================================================================*/
{
	fprintf(filep, "<%s>", cp); 
}

/*==================================================================*/
		  void print_feature(FEATURE *fp, FILE *filep)
/*==================================================================*/
{
	/* <f1><f2> ... <f3> �Ƃ����`���̏o��	
	   (�������s�ł͂��܂�feature�͕\�����Ȃ�) */

	while (fp) {
	if (fp->cp &&	
		(strncmp(fp->cp, "�s", 2) ||
		 OptDisplay == OPT_DEBUG))
		print_one_feature(fp->cp, filep);
	fp = fp->next;
	}
}

/*==================================================================*/
	  void print_some_feature(FEATURE *fp, FILE *filep)
/*==================================================================*/
{
	/* <f1><f2> ... <f3> �Ƃ����`���̏o��	
	   �w�肵�����̂�����\�� */

	while (fp) {
	if (fp->cp && strncmp(fp->cp, "�b", 2) && !strncmp(fp->cp, "C", 1))
		print_one_feature(fp->cp, filep);
	fp = fp->next;
	}
}
/*==================================================================*/
		  void print_feature2(FEATURE *fp, FILE *filep)
/*==================================================================*/
{
	/* (f1 f2 ... f3) �Ƃ����`���̏o��
	   (�������s�ł͂��܂�feature�͕\�����Ȃ�) */
	if (fp) {
	fprintf(filep, "(");	
	while (fp) {
		if (fp->cp && strncmp(fp->cp, "�s", 2)) {
		fprintf(filep, "%s", fp->cp);	
		if (fp->next) fprintf(filep, " ");		
		}
		fp = fp->next;
	}
	fprintf(filep, ")");	
	} else {
	fprintf(filep, "NIL");	
	}
}

/*==================================================================*/
		   void clear_feature(FEATURE **fpp)
/*==================================================================*/
{
	FEATURE *fp, *next;

	fp = *fpp;
	*fpp = NULL;

	while (fp) {
	next = fp->next;
	free(fp->cp);
	free(fp);
	fp = next;
	}
}

/*==================================================================*/
	   void delete_cfeature(FEATURE **fpp, const char *type)
/*==================================================================*/
{
	FEATURE *prep = NULL;

	while (*fpp) {
	if (comp_feature((*fpp)->cp, type) == 1 /*TRUE*/) {
		FEATURE *next;
		free((*fpp)->cp);
		if (prep == NULL) {
		next = (*fpp)->next;
		free(*fpp);
		*fpp = next;
		}
		else {
		next = (*fpp)->next;
		free(*fpp);
		prep->next = next;
		}
		return;
	}
	prep = *fpp;
	fpp = &(prep->next);
	}
}	 

/*
 *
 *	�t�@�C��(S���܂��͕�����) ==�R�s�[==> ���[���\����
 *
 */

/*==================================================================*/
	   void copy_cfeature(FEATURE **fpp, char *fname)
/*==================================================================*/
{
	int n;

	while (*fpp) fpp = &((*fpp)->next);

	if (((*fpp) = (FEATURE *)(malloc_data(sizeof(FEATURE), "copy_cfeature"))) == NULL ||
	((*fpp)->cp = (char *)(malloc_data(n = strlen(fname) + 1, "copy_cfeature"))) == NULL) {
	fprintf(stderr, "Can't allocate memory for FEATURE\n");
	exit(-1);
	}
	art_strcpy((*fpp)->cp, n, fname);
	(*fpp)->next = NULL;
}

/*==================================================================*/
		  void list2feature(CELL *cp, FEATURE **fpp)
/*==================================================================*/
{
	while (!Null(car(cp))) {
	copy_cfeature(fpp, (char*)_Atom(car(cp)));
	fpp = &((*fpp)->next);
	cp = cdr(cp);		
	}
}

/*==================================================================*/
	  void list2feature_pattern(FEATURE_PATTERN *f, CELL *cell)
/*==================================================================*/
{
	/* ���X�g ((����)(�̌�)(���)) �Ȃǂ�FEATURE_PATTERN�ɕϊ� */

	int nth = 0;

	while (!Null(car(cell))) {
	clear_feature(f->fp+nth);		/* ?? &(f->fp[nth]) */	
	list2feature(car(cell), f->fp+nth); /* ?? &(f->fp[nth]) */	
	cell = cdr(cell);
	nth++;
	}
	f->fp[nth] = NULL;
}

#if 0
/*==================================================================*/
	  void string2feature_pattern_OLD(FEATURE_PATTERN *f, char *cp)
/*==================================================================*/
{
	/* ������ "����|�̌�|���" �Ȃǂ�FEATURE_PATTERN�ɕϊ�
	   �{��list2feature_pattern�ɑΉ�������̂���,
	   OR������AND�̓T�|�[�g���Ă��Ȃ� */

	int nth = 0;
	char buffer[256], *scp, *ecp;

	if (cp == NULL || cp[0] == '\0') {
	f->fp[nth] = NULL;
	return;
	}

	art_strcpy(buffer, 256, cp);
	scp = ecp = buffer;
	while (*ecp) {
	if (*ecp == '|') {
		*ecp = '\0';
		clear_feature(f->fp+nth);		/* ?? &(f->fp[nth]) */
		copy_cfeature(f->fp+nth, scp);	/* ?? &(f->fp[nth]) */
		nth++;
		scp = ecp + 1;	
	}
	ecp ++;
	}
	
	clear_feature(f->fp+nth);			/* ?? &(f->fp[nth]) */	
	copy_cfeature(&(f->fp[nth]), scp);
	nth++;

	f->fp[nth] = NULL;
}
#endif

/*==================================================================*/
	  void string2feature_pattern(FEATURE_PATTERN *f, char *cp)
/*==================================================================*/
{
	/* ������ "����|�̌�|���" �Ȃǂ�FEATURE_PATTERN�ɕϊ�
	   �{��list2feature_pattern�ɑΉ�������̂���,
	   OR������AND�̓T�|�[�g���Ă��Ȃ� */

	int nth;
	char buffer[256], *start_cp, *loop_cp;
	FEATURE **fpp;
	
	if (!*cp) {
	f->fp[0] = NULL;
	return;
	}

	art_strcpy(buffer, 256, cp);
	nth = 0;
	clear_feature(f->fp+nth);
	fpp = f->fp+nth;
	loop_cp = buffer;
	start_cp = loop_cp;
	while (*loop_cp) {
	if (*loop_cp == '&' && *(loop_cp+1) == '&') {
		*loop_cp = '\0';
		copy_cfeature(fpp, start_cp);
		fpp = &((*fpp)->next);
		loop_cp += 2;
		start_cp = loop_cp;
	}
	else if (*loop_cp == '|' && *(loop_cp+1) == '|') {
		*loop_cp = '\0';
		copy_cfeature(fpp, start_cp);
		nth++;
		clear_feature(f->fp+nth);
		fpp = f->fp+nth;
		loop_cp += 2;
		start_cp = loop_cp;
	}
	else {
		loop_cp ++;
	}
	}
	copy_cfeature(fpp, start_cp);

	nth++;
	f->fp[nth] = NULL;
}

/*
 *
 * ���[���\���� ==�t�^==> �`�ԑf�܂��͕��ߍ\����
 *
 */

/*==================================================================*/
	   void append_feature(FEATURE **fpp, FEATURE *afp)
/*==================================================================*/
{
	while (*fpp) {
	fpp = &((*fpp)->next);
	}
	*fpp = afp;
}	 

/*==================================================================*/
	   void assign_cfeature(FEATURE **fpp, char *fname)
/*==================================================================*/
{
	char type[256];
	int n;

	/* �㏑���̉\�����`�F�b�N */

	art_sscanf_1(fname, "%[^:]", type/*, 256*/);	/* �� fname��":"���Ȃ��ꍇ��
					   type��fname�S�̂ɂȂ� */

	/* quote('"')����":"�Ő؂��Ă���΁A���Ƃɖ߂� */
	if (strcmp(type, fname)) {
	int i, count = 0;

	for (i = 0; i < (int)strlen(type); i++) {
		if (type[i] == '"') {
		count++;
		}
	}
	if (count % 2 == 1) { /* '"'��� */
		art_strcpy(type, 256, fname);
	}
	}

	while (*fpp) {
	if (comp_feature((*fpp)->cp, type) == 1 /*TRUE*/) {
		free((*fpp)->cp);
		if (((*fpp)->cp = (char *)(malloc_data(n = strlen(fname) + 1, "assign_cfeature"))) == NULL) {
		fprintf(stderr, "Can't allocate memory for FEATURE\n");
		exit(-1);
		}
		art_strcpy((*fpp)->cp, n, fname);
		return; /* �㏑���ŏI�� */
	}
	fpp = &((*fpp)->next);
	}

	/* �㏑���ł��Ȃ���Ζ����ɒǉ� */

	if (((*fpp) = (FEATURE *)(malloc_data(sizeof(FEATURE), "assign_cfeature"))) == NULL ||
	((*fpp)->cp = (char *)(malloc_data(n = strlen(fname) + 1, "assign_cfeature"))) == NULL) {
	fprintf(stderr, "Can't allocate memory for FEATURE\n");
	exit(-1);
	}
	art_strcpy((*fpp)->cp, n, fname);
	(*fpp)->next = NULL;
}	 

/*==================================================================*/
	void assign_feature(FEATURE **fpp1, FEATURE **fpp2, void *ptr)
/*==================================================================*/
{
	/*
	 *	���[����K�p�̌��ʁC���[������\���̂�FEATURE��t�^����
	 *	�\���̎��g�ɑ΂��鏈�����\�Ƃ��Ă���
	 */

	int i;
	char *cp;
	FEATURE **fpp, *next;

	init_str();

	while (*fpp2) {

	if (*((*fpp2)->cp) == '^') {	/* �폜�̏ꍇ */
		
		fpp = fpp1;
		
		while (*fpp) {
		if (comp_feature((*fpp)->cp, &((*fpp2)->cp[1])) == 1 /*TRUE*/) {
			free((*fpp)->cp);
			next = (*fpp)->next;
			free(*fpp);
			*fpp = next;
		} else {
			fpp = &((*fpp)->next);
		}
		}
	
	} else if (*((*fpp2)->cp) == '&') { /* �֐��̏ꍇ */

		if (!strcmp((*fpp2)->cp, hyousou_fuyo)) {
		set_pred_voice((BNST_DATA *)ptr);	/* ���H�C�X */
		get_scase_code((BNST_DATA *)ptr);	/* �\�w�i */
		}
		else if (!strcmp((*fpp2)->cp, hyousou_del)) {
		for (i = 0, cp = ((BNST_DATA *)ptr)->SCASE_code;	
			 i < SCASE_CODE_SIZE; i++, cp++)	
			*cp = 0;		
		}
		else if (!strncmp((*fpp2)->cp, hyousou_chiruda, strlen(hyousou_chiruda))) {
		((BNST_DATA *)ptr)->
			SCASE_code[case2num((*fpp2)->cp + strlen(hyousou_chiruda))] = 0;
		}
		else if (!strncmp((*fpp2)->cp, hyousou, strlen(hyousou))) {
		((BNST_DATA *)ptr)->
			SCASE_code[case2num((*fpp2)->cp + strlen(hyousou))] = 1;
		}
		else if (!strncmp((*fpp2)->cp, "&MEMO:", strlen("&MEMO:"))) {
		art_strcat(PM_Memo, DATA_LEN, " ");
		art_strcat(PM_Memo, DATA_LEN, (*fpp2)->cp + strlen("&MEMO:"));
		}
		else if (!strncmp((*fpp2)->cp, hinnshi_henkou, strlen(hinnshi_henkou))) {
		change_mrph((MRPH_DATA *)ptr, *fpp2);
		}
		else if (!strncmp((*fpp2)->cp, imiso_fuyo, strlen(imiso_fuyo))) {
		assign_sm((BNST_DATA *)ptr, (*fpp2)->cp + strlen(imiso_fuyo));
		}
		else if (!strncmp((*fpp2)->cp, denpan, strlen(denpan))) {
		int dir;
		char trans_feature[DATA_LEN];
		art_sscanf_3((*fpp2)->cp, denpan_format, &dir, trans_feature, DATA_LEN);


		cp = check_feature(*fpp1, trans_feature);


		if (cp) {
			assign_cfeature(&((((BNST_DATA *)ptr)+dir)->f), cp);
		}
		}
	} else {			/* �ǉ��̏ꍇ */
		assign_cfeature(fpp1, (*fpp2)->cp); 
	}

	fpp2 = &((*fpp2)->next);
	}
}

/*==================================================================*/
	void copy_feature(FEATURE **dst_fpp, FEATURE *src_fp)
/*==================================================================*/
{
	while (src_fp) {
	assign_cfeature(dst_fpp, src_fp->cp);
	src_fp = src_fp->next;
	}
}

/*
 *
 * ���[���\���� <==�ƍ�==> �`�ԑf�܂��͕��ߍ\����
 *
 */

/*==================================================================*/
		 int comp_feature(char *data, const char *pattern)
/*==================================================================*/
{
	/*	
	 *	���S��v �܂��� ������v(pattern���Z��,���̕�����':')�Ȃ�}�b�`
	 */
	
	if (data && !strcmp(data, pattern)) {
	return TRUE;
	} else if (data && !strncmp(data, pattern, strlen(pattern)) &&
		   data[strlen(pattern)] == ':') {
	return TRUE;
	} else {
	return FALSE;
	}
}

/*==================================================================*/
		 int comp_feature_NE(char *data, const char *pattern)
/*==================================================================*/
{
	char decision[9];

	decision[0] = '\0';
	art_sscanf_1(data, "%*[^:]:%*[^:]:%s", decision/*, 9*/);

	if (decision[0] && !strcmp(decision, pattern))
	return TRUE;
	else
	return FALSE;
}

/*==================================================================*/
		char *check_feature(FEATURE *fp, const char *fname)
/*==================================================================*/
{
	while (fp) {
	if (comp_feature(fp->cp, fname) == 1 /*TRUE*/) {
		return fp->cp;
	}
	fp = fp->next;
	}
	return NULL;
}

/*==================================================================*/
		char *check_feature_NE(FEATURE *fp, const char *fname)
/*==================================================================*/
{
	while (fp) {
	if (comp_feature_NE(fp->cp, fname) == 1 /*TRUE*/) {
		return fp->cp;
	}
	fp = fp->next;
	}
	return NULL;
}

/*==================================================================*/
	int compare_threshold(int value, int threshold, char *eq)
/*==================================================================*/
{
	if (str_eq(eq, "lt")) {
	if (value < threshold)
		return TRUE;
	else
		return FALSE;
	}
	else if (str_eq(eq, "le")) {
	if (value <= threshold)
		return TRUE;
	else
		return FALSE;
	}
	else if (str_eq(eq, "gt")) {
	if (value > threshold)
		return TRUE;
	else
		return FALSE;
	}
	else if (str_eq(eq, "ge")) {
	if (value >= threshold)
		return TRUE;
	else
		return FALSE;
	}
	return FALSE;
}

/*==================================================================*/
	  int check_Bunrui_others(MRPH_DATA *mp, int flag)
/*==================================================================*/
{
	if (mp->Bunrui != 3 && /* �ŗL���� */
	mp->Bunrui != 4 && /* �n�� */
	mp->Bunrui != 5 && /* �l�� */
	mp->Bunrui != 6) /* �g�D�� */
	return flag;

	init_str();

	if (check_feature(mp->f, hin_other))
	return flag;

	return 1-flag;
}

/*==================================================================*/
	  int check_Bunrui(MRPH_DATA *mp, char *class2, int flag)
/*==================================================================*/
{
	char string[14];

	if (str_eq((const char*)Class[6][mp->Bunrui].id, class2))
	return flag;

	init_str();

	art_sprintf_1(string, 14, hin_format, class2);
	if (check_feature(mp->f, string))
	return flag;

	return 1-flag;
}

/*==================================================================*/
			int check_char_type(int code)
/*==================================================================*/
{
	/* �J�^�J�i�� "�[" */
	if ((0xa5a0 < code && code < 0xa6a0) || code == 0xa1bc) {
	return TYPE_KATAKANA;
	}
	/* �Ђ炪�� */
	else if (0xa4a0 < code && code < 0xa5a0) {
	return TYPE_HIRAGANA;
	}
	/* ���� */
	else if (0xb0a0 < code || code == 0xa1b9) {
	return TYPE_KANJI;
	}
	/* ������ "�E", "�D" */
	else if ((0xa3af < code && code < 0xa3ba) || code == 0xa1a5 || code == 0xa1a6) {
	return TYPE_SUUJI;
	}
	/* �A���t�@�x�b�g */
	else if (0xa3c0 < code && code < 0xa3fb) {
	return TYPE_EIGO;
	}
	/* �L�� */
	else {
	return TYPE_KIGOU;
	}
}

/*==================================================================*/
 int check_function(char *rule, FEATURE * /*fd*/, void *ptr1, void *ptr2)
/*==================================================================*/
{
	/* rule : ���[��
	   fd : �f�[�^����FEATURE
	   p1 : ���[�����̍\����(MRPH_DATA,BNST_DATA�Ȃ�)
	   p2 : �f�[�^���̍\����(MRPH_DATA,BNST_DATA�Ȃ�)
	*/

	int i, code, type, pretype, flag;
	char *cp;
	unsigned char *ucp; 

	init_str();

	/* &�L�p���J : �L�p���J �`�F�b�N (��Ǔ_�ȊO) (�`�ԑf���x��) */

	if (!strcmp(rule, alpah_digit)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		if (!(0xa1a5 < code && code < 0xa4a0) && /* �L���͈̔� */
		!(0xa5a0 < code && code < 0xb0a0))
		return FALSE;
		ucp += 2;
	}		
	return TRUE;
	}

	/* &���� : ���� �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, kanji)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		if (code >= 0xb0a0 ||	/* �����͈̔� */
		code == 0xa1b9 ||	/* �X */
		(code == 0xa4ab && ucp == (unsigned char *)((MRPH_DATA *)ptr2)->Goi2) ||	/* �� */
		(code == 0xa5ab && ucp == (unsigned char *)((MRPH_DATA *)ptr2)->Goi2) ||	/* �J */
		(code == 0xa5f6 && ucp == (unsigned char *)((MRPH_DATA *)ptr2)->Goi2))		/* �� */
		  ;
		else	
		  return FALSE;
		ucp += 2;
	}		
	return TRUE;
	}

	/* &���Ȋ��� : ���Ȋ����`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, kana_kanji)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		code = check_char_type(code);
		if (!(code == TYPE_KANJI || code == TYPE_HIRAGANA))
		return FALSE;
		ucp += 2;
	}		
	return TRUE;
	}

	/* &�Ђ炪�� : �Ђ炪�� �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, hiragana)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		if (check_char_type(code) != TYPE_HIRAGANA)
		return FALSE;
		ucp += 2;
	}		
	return TRUE;
	}

	/* &�����Ђ炪�� : �����̈ꕶ�����Ђ炪�Ȃ� �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, matsubi_hiragana)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;	/* �\�L���`�F�b�N */
	ucp += strlen((const char*)ucp)-2;
	code = (*ucp)*0x100+*(ucp+1);
	if (check_char_type(code) != TYPE_HIRAGANA)
		return FALSE;
	return TRUE;
	}

	/* &���������� : �����̕������ �`�F�b�N (�`�ԑf���x��) */

	else if (!strncmp(rule, matsubi_mojiretsu, strlen(matsubi_mojiretsu))) {
	cp = rule + strlen(matsubi_mojiretsu);

	/* �p�^�[���̕����傫�����FALSE */
	if (strlen(cp) > strlen(((MRPH_DATA *)ptr2)->Goi2))
		return FALSE;

	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;	/* �\�L���`�F�b�N */
	ucp += strlen((const char*)ucp)-strlen(cp);
	if (strcmp((const char*)ucp, cp))
		return FALSE;
	return TRUE;
	}

	/* &�J�^�J�i : �J�^�J�i �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, katakana)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		if (check_char_type(code) != TYPE_KATAKANA)
		return FALSE;
		ucp += 2;
	}		
	return TRUE;
	}

	/* &���� : ���� �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, number)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		if (check_char_type(code) != TYPE_SUUJI)
		return FALSE;
		ucp += 2;
	}		
	return TRUE;
	}

	/* &�p�L�� : �p�L�� �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, alpha_symbol)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		type = check_char_type(code);
		if (type != TYPE_EIGO && type != TYPE_KIGOU)
		return FALSE;
		ucp += 2;
	}		
	return TRUE;
	}

	/* &�L�� : �L�� �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, symbol)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		type = check_char_type(code);
		if (type != TYPE_KIGOU)
		return FALSE;
		ucp += 2;
	}		
	return TRUE;
	}

	/* &���� : ���� (����+...) �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, mixed)) {
	ucp = (unsigned char*)((MRPH_DATA *)ptr2)->Goi2;
	pretype = 0;
	while (*ucp) {
		code = (*ucp)*0x100+*(ucp+1);
		type = check_char_type(code);
		if (pretype && pretype != type)
		return TRUE;
		pretype = type;
		ucp += 2;
	}
	return FALSE;
	}

	/* &�ꕶ�� : ������ �`�F�b�N (�`�ԑf���x��) */

	else if (!strcmp(rule, one_character)) {
	if (strlen(((MRPH_DATA *)ptr2)->Goi2) == 2)
		return TRUE;
	else	
		return FALSE;
	}

	/* &�Ӗ��f: �Ӗ��f�`�F�b�N (�`�ԑf) */

	else if (!strncmp(rule, sememe, strlen(sememe))) {
	if (Thesaurus != USE_NTT || ((MRPH_DATA *)ptr2)->SM == NULL) {
		return FALSE;
	}

	cp = rule + strlen(sememe);
	/* ������������Ӗ�������, ����ȊO�Ȃ�R�[�h���̂܂� */
	if (*cp & 0x80) {
		if (SM2CODEExist == 1 /*TRUE*/)
		cp = sm2code(cp);
		else
		cp = NULL;
		flag = SM_NO_EXPAND_NE;
	}
	else {
		flag = SM_CHECK_FULL;
	}

	if (cp) {
		for (i = 0; ((MRPH_DATA *)ptr2)->SM[i]; i+=SM_CODE_SIZE) {
		if (_sm_match_score(cp, 
					&(((MRPH_DATA *)ptr2)->SM[i]), flag))
			return TRUE;
		}
	}
	return FALSE;
	}

	/* &���߈Ӗ��f: �Ӗ��f�`�F�b�N (����) */

	else if (!strncmp(rule, clause_sememe, strlen(clause_sememe))) {
	if (Thesaurus != USE_NTT) {
		return FALSE;
	}

	cp = rule + strlen(clause_sememe);
	/* ������������Ӗ�������, ����ȊO�Ȃ�R�[�h���̂܂� */
	if (*cp & 0x80) {
		if (SM2CODEExist == 1 /*TRUE*/)
		cp = sm2code(cp);
		else
		cp = NULL;
		flag = SM_NO_EXPAND_NE;
	}
	else {
		flag = SM_CHECK_FULL;
	}

	if (cp) {
		for (i = 0; ((BNST_DATA *)ptr2)->SM_code[i]; i+=SM_CODE_SIZE) {
		if (_sm_match_score(cp, &(((BNST_DATA *)ptr2)->SM_code[i]), flag))
			return TRUE;
		}
	}
	return FALSE;
	}

	/* &���ߑS�Ӗ��f: ���߂̂��ׂĂ̈Ӗ��f���w��Ӗ��f�ȉ��ɂ��邩�ǂ��� */

	else if (!strncmp(rule, clause_all_sememes, strlen(clause_all_sememes))) {
	if (Thesaurus != USE_NTT) {
		return FALSE;
	}

	cp = rule + strlen(clause_all_sememes);
	/* ������������Ӗ�������, ����ȊO�Ȃ�R�[�h���̂܂� */
	if (*cp & 0x80) {
		if (SM2CODEExist == 1 /*TRUE*/)
		cp = sm2code(cp);
		else
		cp = NULL;
	}

	if (cp && ((BNST_DATA *)ptr2)->SM_code[0] &&	
		sm_all_match(((BNST_DATA *)ptr2)->SM_code, cp)) {
		return TRUE;
	}
	return FALSE;
	}

	/* �`�ԑf�̒��� */
	
	else if (!strncmp(rule, morpheme_length, strlen(morpheme_length))) {
	cp = rule + strlen(morpheme_length);
	if (cp)
		code = atoi(cp);
	else
		code = 0;
	if ((int)strlen(((MRPH_DATA *)ptr2)->Goi2) >= code*2) {
		return TRUE;
	}
	return FALSE;
	}

	else if (!strncmp(rule, morpheme_end, strlen(morpheme_end))) {
	cp = rule + strlen(morpheme_end);
	i = (int)strlen(((MRPH_DATA *)ptr2)->Goi2) - (int)strlen(cp);
	if (*cp && i >= 0 && !strcmp((((MRPH_DATA *)ptr2)->Goi2)+i, cp)) {
		return TRUE;
	}
	return FALSE;
	}

	/* &�\�w: �\�w�i�`�F�b�N (���߃��x��,�W�󃌃x��) */

	else if (!strncmp(rule, hyousou, strlen(hyousou))) {
	if (!strcmp(rule + strlen(hyousou), collation)) {
		if ((cp = check_feature(((BNST_DATA *)ptr1)->f, "�W")) == NULL) {
		return FALSE;
		}
		if (((BNST_DATA *)ptr2)->
		SCASE_code[case2num(cp + strlen(modif))]) {
		return TRUE;
		}
		else {
		return FALSE;
		}
	}
	else if (((BNST_DATA *)ptr2)->
		 SCASE_code[case2num(rule + strlen(hyousou))]) {
		return TRUE;
	}
	else {
		return FALSE;
	}
	}

	/* &D : ������r (�W�󃌃x��) */

	else if (!strncmp(rule, "&D:", strlen("&D:"))) {
	if (((BNST_DATA *)ptr2 - (BNST_DATA *)ptr1)
		<= atoi(rule + strlen("&D:"))) {
		return TRUE;
	}
	else {
		return FALSE;
	}
	}

	/* &���x�� : �p���̃��x����r (�W�󃌃x��) */

	else if (!strncmp(rule, levelc, strlen(levelc))) {
	if (!strcmp(rule + strlen(levelc), strong)) {
		/* �q��Ԃ̋���̔�r */
		return subordinate_level_comp((BNST_DATA *)ptr1,	
					  (BNST_DATA *)ptr2);
	}
	else {
		/* �q��͌W��󂯉\�� */
		return subordinate_level_check(rule + strlen(levelc),	
					   (BNST_DATA *)ptr2);
	}
	}


	/* &�ߋ��E : �ߊԂ̕ǃ`�F�b�N */

	else if (!strncmp(rule, clause_boundary, strlen(clause_boundary))) {
	/*	
	   1. ���[���ɏ����Ă��郌�x����苭�����Ƃ��`�F�b�N
	   2. �W�葤���󂯑��̃��x�����������Ƃ��`�F�b�N
	*/
	return (subordinate_level_check(rule + strlen(clause_boundary), 
					(BNST_DATA *)ptr2) &&	
		subordinate_level_comp((BNST_DATA *)ptr1,	
					   (BNST_DATA *)ptr2));
	}

	/* &�W�� : �W����FEATURE�`�F�b�N (�W�󃌃x��) */
	
	else if (!strncmp(rule, modif_side, strlen(modif_side))) {
	cp = rule + strlen(modif_side);
	if ((*cp != '^' && check_feature(((BNST_DATA *)ptr1)->f, cp)) ||
		(*cp == '^' && !check_feature(((BNST_DATA *)ptr1)->f, cp))) {
		return TRUE;
	} else {
		return FALSE;
	}
	}

	/* &�������v : �����ꂪ�������ǂ��� */
	
	else if (!strncmp(rule, independence_word, strlen(independence_word))) {
	/* if (!strcmp(((BNST_DATA *)ptr1)->head_ptr->Goi,	
	   ((BNST_DATA *)ptr2)->head_ptr->Goi)) { */
	if (!strcmp(((BNST_DATA *)ptr1)->Jiritu_Go, 
			((BNST_DATA *)ptr2)->Jiritu_Go)) {
		return TRUE;
	} else {
		return FALSE;
	}
	}


	/* &������ƍ� : ���`�Ƃ̕����񕔕��}�b�` by kuro 00/12/28 */
	
	else if (!strncmp(rule, string_collation, strlen(string_collation))) {
		cp = rule + strlen(string_collation);
	if (strstr(((MRPH_DATA *)ptr2)->Goi, cp)) {
		return TRUE;
	} else {
		return FALSE;
	}
	}

	/* &ST : ����\����͂ł̗ގ��x��臒l (�����ł͖���) */
	
	else if (!strncmp(rule, "&ST", strlen("&ST"))) {
	return TRUE;
	}

	/* &OPTCHECK : �I�v�V�����̃`�F�b�N */
	
	else if (!strncmp(rule, "&OptCheck:", strlen("&OptCheck:"))) {
	char **opt;

	cp = rule + strlen("&OptCheck:");
	if (*cp == '-') { /* '-'���܂�ł������΂� */
		cp++;
	}

	for (opt = Options; *opt != NULL; opt++) {
		if (!art_strcasecmp(cp, *opt)) {
		return TRUE;		
		}
	}
	return FALSE;
	}

	/*
	else if (!strncmp(rule, "&����", strlen("&����"))) {
	if (sm_all_match(((BNST_DATA *)ptr2)->SM_code, "1128********")) {
		return TRUE;
	}
	else {
		return FALSE;
	}
	} */

	/* &�� : �Ԃ��`�F�b�N */

	else if (!strncmp(rule, voice, strlen(voice))) {
	cp = rule + strlen(voice);
	if ((!strcmp(cp, active_voice) && ((BNST_DATA *)ptr2)->voice == 0) ||	
		(!strcmp(cp, passive_voice) && (((BNST_DATA *)ptr2)->voice & VOICE_UKEMI || 
					 ((BNST_DATA *)ptr2)->voice & VOICE_SHIEKI_UKEMI)) ||	
		(!strcmp(cp, employment) && (((BNST_DATA *)ptr2)->voice & VOICE_SHIEKI ||	
					 ((BNST_DATA *)ptr2)->voice & VOICE_SHIEKI_UKEMI))) {
		return TRUE;
	}
	else {
		return FALSE;
	}
	}

	else {
#ifdef KNP_DEBUG
	fprintf(stderr, "Invalid Feature-Function (%s)\n", rule);
#endif
	return TRUE;
	}
}

/*==================================================================*/
 int feature_AND_match(FEATURE *fp, FEATURE *fd, void *p1, void *p2)
/*==================================================================*/
{
	int value;

	while (fp) {
	if (fp->cp[0] == '^' && fp->cp[1] == '&') {
		value = check_function(fp->cp+1, fd, p1, p2);
		if (value == 1 /*TRUE*/) {
		return FALSE;
		}
	} else if (fp->cp[0] == '&') {
		value = check_function(fp->cp, fd, p1, p2);
		if (value == FALSE) {
		return FALSE;
		}
	} else if (fp->cp[0] == '^') {
		if (check_feature(fd, fp->cp+1)) {
		return FALSE;
		}
	} else {
		if (!check_feature(fd, fp->cp)) {
		return FALSE;
		}
	}
	fp = fp->next;
	}
	return TRUE;
}

/*==================================================================*/
int feature_pattern_match(FEATURE_PATTERN *fr, FEATURE *fd,
			  void *p1, void *p2)
/*==================================================================*/
{
	/* fr : ���[������FEATURE_PATTERN,
	   fd : �f�[�^����FEATURE
	   p1 : ���[�����̍\����(MRPH_DATA,BNST_DATA�Ȃ�)
	   p2 : �f�[�^���̍\����(MRPH_DATA,BNST_DATA�Ȃ�)
	*/

	int i, value;

	/* PATTERN���Ȃ���΃}�b�` */
	if (fr->fp[0] == NULL) return TRUE;

	/* OR�̊e�����𒲂ׂ� */
	for (i = 0; fr->fp[i]; i++) {
	value = feature_AND_match(fr->fp[i], fd, p1, p2);
	if (value == 1 /*TRUE*/)	
		return TRUE;
	}
	return FALSE;
}

/*====================================================================
							   END
====================================================================*/
