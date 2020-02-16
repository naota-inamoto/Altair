/*====================================================================
			   �o�͂�PS�ւ̕ϊ�

											   S.Kurohashi 91. 6.25
											   S.Kurohashi 93. 5.31

	$Id: lib_ps.c,v 1.7 2001/11/04 03:48:39 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

#define HEAD_FILE "/home/kuro/work/nl/E/knpe/src/head.dat"
#define TAIL_FILE "/home/kuro/work/nl/E/knpe/src/tail.dat"
#define X	197
#define Y	757
#define X2	497
#define Y2	757

/* #define FOR_GLOSS ��}���p */

#ifdef	_WINDOWS
static char* special = NULL;
static char* dookuten = NULL;
static char* character = NULL;
static char* hbar = NULL;
static char* hbar2 = NULL;
static char* branch = NULL;
static char* corner = NULL;
static char* vbar = NULL;
static char special_2[] = "����";
static char dookuten_2[] = "�Ǔ_";
static char character_2[] = "����";
static char hbar_2[] = "��";
static char hbar2_2[] = "����";
static char branch_2[] = "��";
static char corner_2[] = "��";
static char vbar_2[] = "��";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	special = toStringEUC(special_2);
	dookuten = toStringEUC(dookuten_2);
	character = toStringEUC(character_2);
	hbar = toStringEUC(hbar_2);
	hbar2 = toStringEUC(hbar2_2);
	branch = toStringEUC(branch_2);
	corner = toStringEUC(corner_2);
	vbar = toStringEUC(vbar_2);
#endif
}
void final_str_lib_ps() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(special); special = NULL;
	free(dookuten); dookuten = NULL;
	free(character); character = NULL;
	free(hbar); hbar = NULL;
	free(hbar2); hbar2 = NULL;
	free(branch); branch = NULL;
	free(corner); corner = NULL;
	free(vbar); vbar = NULL;
#endif
}
#ifdef	_LINUX
static char special[] = "����";
static char dookuten[] = "�Ǔ_";
static char character[] = "����";
static char hbar[] = "��";
static char hbar2[] = "����";
static char branch[] = "��";
static char corner[] = "��";
static char vbar[] = "��";
#endif

void write_head(FILE *fp)
{
	FILE *fpr;
	int c;

	fpr = art_fopen(HEAD_FILE, "r");
	while((c = fgetc(fpr)) != EOF)
	fputc(c, fp);
	fclose(fpr);
}

void write_tail(FILE *fp)
{
	FILE *fpr;
	int c;

	fpr = art_fopen(TAIL_FILE, "r");
	while((c = fgetc(fpr)) != EOF)
	fputc(c, fp);
	fclose(fpr);
} 

void write_text10(FILE *fp, const char *cp, int x, int y)
{
	fprintf(fp, "Begin %%I Text\n");
	fprintf(fp, "%%I cfg Black\n");
	fprintf(fp, "0 0 0 SetCFg\n");
	fprintf(fp, "%%I f *-times-medium-r-*-100-*\n");
	fprintf(fp, "/Times-Roman 10 SetF\n");
	fprintf(fp, "%%I t\n");
	fprintf(fp, "[ 1 0 0 1 %d %d ] concat\n", x, y-3);
	fprintf(fp, "%%I\n");
	fprintf(fp, "[\n");
	fprintf(fp, "(%s)\n", cp);
	fprintf(fp, "] WhiteBg Text\n");
	fprintf(fp, "End\n");
	fprintf(fp, "\n");
}
	
void write_text14(FILE *fp, const char *cp, int x, int y)
{
	fprintf(fp, "Begin %%I Text\n");
	fprintf(fp, "%%I cfg Black\n");
	fprintf(fp, "0 0 0 SetCFg\n");
	fprintf(fp, "%%I f *-times-medium-r-*-140-*\n");
	fprintf(fp, "/Times-Roman 14 SetF\n");
	fprintf(fp, "%%I t\n");
	fprintf(fp, "[ 1 0 0 1 %d %d ] concat\n", x, y-3);
	fprintf(fp, "%%I\n");
	fprintf(fp, "[\n");
	fprintf(fp, "(%s)\n", cp);
	fprintf(fp, "] WhiteBg Text\n");
	fprintf(fp, "End\n");
	fprintf(fp, "\n");
}

void write_kanji(FILE *fp, char *cp, int x, int y)
{
	fprintf(fp, "Begin %%I KText\n");
	fprintf(fp, "%%I cfg Black\n");
	fprintf(fp, "0 0 0 SetCFg\n");
	fprintf(fp, "%%I k k14\n");
	fprintf(fp, "/Ryumin-Light-H 14 SetF\n");
	fprintf(fp, "%%I t\n");
	fprintf(fp, "[ 1 0 0 1 %d %d ] concat\n", x, y);
	fprintf(fp, "%%I\n");
	fprintf(fp, "[\n");
	fprintf(fp, "<");
	while (*cp) {
	fprintf(fp, "%02x", *cp+128);
	cp++;
	}
	fprintf(fp, ">\n");
	fprintf(fp, "] WhiteBg Text\n");
	fprintf(fp, "End\n");
	fprintf(fp, "\n");
}

static char tmp[64];
/*==================================================================*/
	 void print_matrix2ps(SENTENCE_DATA *sp, int jlen, int type)
/*==================================================================*/
{
	int i, j, comma_p;
	int para_char = 0;	   /* para_key �̕\���p */
	char point_B[10];
	PARA_DATA *ptr;
	BNST_DATA *b_ptr;

	init_str();

	/* �p�X�̃}�[�N�t�� */

	for (i = 0; i < sp->Bnst_num; i++)
	for (j = 0; j < sp->Bnst_num; j++)
		path_matrix[i][j] = 0;

	if (type == PRINT_PARA) {
	for (i = 0; i < sp->Para_num; i++) {
		ptr = &sp->para_data[i];
		for (j = ptr->key_pos+1; j <= ptr->jend_pos; j++)
		path_matrix[ptr->max_path[j-ptr->key_pos-1]][j] =
			path_matrix[ptr->max_path[j-ptr->key_pos-1]][j] ?
			-1 : 'a' + i;
	}
	}

	/* �o�r�̏o�� */

	write_head(stdout);

	for (i = 0, b_ptr = sp->bnst_data; i < sp->Bnst_num; i++, b_ptr++) {

	if ( b_ptr->mrph_num == 1 ) {
		art_strcpy(tmp, 64, b_ptr->mrph_ptr->Goi2);
		comma_p = FALSE;
	} else {
		art_strcpy(tmp, 64, b_ptr->mrph_ptr->Goi2);
		for (j = 1; j < (b_ptr->mrph_num - 1); j++) 
		art_strcat(tmp, 64, (b_ptr->mrph_ptr + j)->Goi2);
		if (!strcmp((const char*)Class[(b_ptr->mrph_ptr + b_ptr->mrph_num - 1)->Hinshi][0].id,
			special) &&
		!strcmp((const char*)Class[(b_ptr->mrph_ptr + b_ptr->mrph_num - 1)->Hinshi]
			[(b_ptr->mrph_ptr + b_ptr->mrph_num - 1)->Bunrui].id,	
			(const char*)dookuten)) {
		comma_p = TRUE;
		} else {
		art_strcat(tmp, 64, (b_ptr->mrph_ptr + b_ptr->mrph_num - 1)->Goi2);
		comma_p = FALSE;			
		}
	}

	if (sp->bnst_data[i].para_key_type) {
		art_sprintf_1(point_B, 10, "%c>", 'a'+ para_char);
		para_char++;
		if (comma_p)
		write_text14(stdout,point_B,X+i*20+15-(int)strlen(tmp)*7-14-7,Y-i*20);
		else
		write_text14(stdout,point_B,X+i*20+15-(int)strlen(tmp)*7-14, Y-i*20);
	}

	if (comma_p) {
		write_kanji(stdout, tmp, X+i*20+15-(int)strlen(tmp)*7-7, Y-i*20);
		write_text14(stdout, ",", X+i*20+15-7, Y-i*20);
	} else {
		write_kanji(stdout, tmp, X+i*20+15-(int)strlen(tmp)*7, Y-i*20);
	}
	
	for (j = i+1; j < sp->Bnst_num; j++) {
		if (type == PRINT_PARA)
		art_sprintf_1(point_B, 10, "%2d", match_matrix[i][j]);
		else if (type == PRINT_DPND)
		art_sprintf_1(point_B, 10, "%2d", Dpnd_matrix[i][j]);
		switch(path_matrix[i][j]) {
		case  0:	point_B[2] = ' '; break;
		case -1:	point_B[2] = '*'; break;
		default:	point_B[2] = (char)path_matrix[i][j]; break;
		}
		point_B[3] = '\0';
		write_text14(stdout, point_B, X+j*20, Y-i*20);	
	}
	}
	
	if (type == PRINT_PARA) {
	art_sprintf_1(point_B, 10, "\\(%d", jlen);
	write_text14(stdout, point_B,	
			 X+(sp->Bnst_num-1)*20+15-((int)strlen(point_B)-1)*7-35,	
			 Y-sp->Bnst_num*20);
	write_kanji(stdout, character, X+(sp->Bnst_num-1)*20+15-35, Y-sp->Bnst_num*20);
	write_text14(stdout, "\\)", X+(sp->Bnst_num-1)*20+15-7, Y-sp->Bnst_num*20);
	}
	write_tail(stdout);
}

static int X_pos, Y_pos, Wid, Hig;
/*==================================================================*/
		void print_bnst2ps(BNST_DATA *ptr)
/*==================================================================*/
{
	int i;

	if (ptr) {
	if (ptr->para_top_p == 1 /*TRUE*/) {
		write_text10(stdout, "PARA", X_pos, Y_pos); X_pos += Wid*4;
	} else {
		art_strcpy(tmp, 64, ptr->mrph_ptr->Goi2);
		for (i = 1; i < ptr->mrph_num; i++) 
		  art_strcat(tmp, 64, (ptr->mrph_ptr + i)->Goi2);
		write_kanji(stdout, tmp, X_pos, Y_pos); X_pos += Wid*(int)strlen(tmp);
	}

	if (ptr->para_type == PARA_NORMAL) {
		write_text10(stdout, "<P>", X_pos, Y_pos); X_pos += Wid*3;
	} else if (ptr->para_type == PARA_INCOMP) {
		write_text10(stdout, "<I>", X_pos, Y_pos); X_pos += Wid*3;
	}
	if (ptr->to_para_p == 1 /*TRUE*/) {
		write_text10(stdout, "\\(D\\)", X_pos, Y_pos); X_pos += Wid*3;
	}
	}
}

/*==================================================================*/
	   void calc_tree_width2ps(BNST_DATA *ptr, int depth2)
/*==================================================================*/
{
	int i;
	
	if (ptr->para_top_p == 1 /*TRUE*/) ptr->space = 4;
	else					ptr->space = ptr->length;
	if (ptr->para_type == PARA_NORMAL || ptr->para_type == PARA_INCOMP)
	ptr->space += 1;
	if (ptr->to_para_p == 1 /*TRUE*/)
	ptr->space += 3;
	ptr->space += (depth2-1)*8;

	if (ptr->child[0])
	for (i = 0; ptr->child[i]; i++)
		calc_tree_width2ps(ptr->child[i], depth2+1);
}

/*==================================================================*/
void show_link2ps(int depth, char *ans_flag, int para_flag, int /*x_pos*/)
/*==================================================================*/
{
	int i;
	
	init_str();

	if (depth != 1) {

	if (para_flag == PARA_NORMAL || para_flag == PARA_INCOMP) {
		write_kanji(stdout, hbar, X_pos, Y_pos); X_pos += Wid*2;
	} else {
		write_kanji(stdout, hbar2, X_pos, Y_pos); X_pos += Wid*4;
	}

	if (ans_flag[depth-1] == '1') {
		write_kanji(stdout, branch, X_pos, Y_pos); X_pos += Wid*2;
	} else {
		write_kanji(stdout, corner, X_pos, Y_pos); X_pos += Wid*2;
	}

	X_pos += Wid*2;

	for (i = depth - 1; i > 1; i--) {
		X_pos += Wid*4;
		if (ans_flag[i-1] == '1') {
		write_kanji(stdout, vbar, X_pos, Y_pos); X_pos += Wid*2;
		} else {
		X_pos += Wid*2;
		}
		X_pos += Wid*2;
	}

#ifdef FOR_GLOSS /* ��}���p */
	Y_pos -= Hig;
	X_pos = x_pos;

	if (para_flag == PARA_NORMAL || para_flag == PARA_INCOMP) {
			X_pos += Wid*2;
		} else {
			X_pos += Wid*4;
		}

	write_kanji(stdout, vbar, X_pos, Y_pos); X_pos += Wid*2;

	X_pos += Wid*2;

	for (i = depth - 1; i > 1; i--) {
		X_pos += Wid*4;
		if (ans_flag[i-1] == '1') {
		write_kanji(stdout, vbar, X_pos, Y_pos); X_pos += Wid*2;
		} else {
		X_pos += Wid*2;
		}
		X_pos += Wid*2;
	}
#endif
	Y_pos -= Hig;
	X_pos = X2;
	}
}

/*==================================================================*/
void show_self2ps(BNST_DATA *ptr, int depth, char *ans_flag_p, int flag)
/*==================================================================*/
{
	int i, j;
	char ans_flag[BNST_MAX+1];

	init_str();

	if (ans_flag_p) {
	art_strncpy(ans_flag, BNST_MAX+1, ans_flag_p, BNST_MAX);
	} else {
	ans_flag[0] = '0';	/* �ŏ��ɌĂ΂��Ƃ� */
	}

	if (ptr->child[0]) {
	for (i = 0; ptr->child[i]; i++);

	/* �Ō�̎q�� ans_flag �� 0 �� */	
	ans_flag[depth] = '0';
	show_self2ps(ptr->child[i-1], depth+1, ans_flag, 0);

	if (i > 1) {
		/* ���̎q�� ans_flag �� 1 �� */ 
		ans_flag[depth] = '1';
		for (j = i - 2; j > 0; j--) {
		show_self2ps(ptr->child[j], depth+1, ans_flag, 0);
		}

		/* flag: 1: ��PARA 2: -<P>PARA */
		if (ptr->para_top_p == 1 /*TRUE*/ && ptr->para_type == PARA_NIL)
		show_self2ps(ptr->child[0], depth+1, ans_flag, 1);
		else if (ptr->para_top_p == 1 /*TRUE*/ && ptr->para_type != PARA_NIL)
		show_self2ps(ptr->child[0], depth+1, ans_flag, 2);
		else
		show_self2ps(ptr->child[0], depth+1, ans_flag, 0);
	}
	}

	if (ptr->para_top_p == FALSE)
	X_pos -= ptr->space*7;

	print_bnst2ps(ptr);
	
	if (flag == 0) {
	show_link2ps(depth, ans_flag, ptr->para_type, X_pos);
	} else if (flag == 1) {
	write_kanji(stdout, hbar, X_pos, Y_pos); X_pos += Wid*2;
	} else if (flag == 2) {
	write_text10(stdout, "-", X_pos, Y_pos); X_pos += Wid;
	}
}

/*==================================================================*/
		   void print_kakari2ps(SENTENCE_DATA *sp)
/*==================================================================*/
{
	/* �ˑ��\���؂̕\�� */

	calc_tree_width2ps((sp->bnst_data + sp->Bnst_num - 1), 1);

	X_pos = X2; Y_pos = Y2;
	Wid = 7;   Hig = 14;

	/* �o�r�̏o�� */

	write_head(stdout);

	show_self2ps((sp->bnst_data + sp->Bnst_num - 1), 1, NULL, 0);

	write_tail(stdout);
}
/*====================================================================
							   END
====================================================================*/
