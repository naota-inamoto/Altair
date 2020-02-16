/*
==============================================================================
	juman_lib.c
==============================================================================
*/
 
#include "stdafx.h"

/*
  �A�ڕ\�Ɋ�Â���{�I�������z���鏈���ɂ��āC�A���S���Y�����ȒP�ɐ�
  ������D

  �y�A�ꏈ���z

  m_buffer �y�� p_buffer �̍\���́C�ȑO�� juman �Ɖ���ς��Ȃ��D���
  ���āC���݂̃��e�B�X�����̂悤�ȏ�Ԃł���Ƃ���D

  ���\ -- ����
  (T)	  (T)

  �A��u���ƂɂȂ�v���ǉ������ꍇ�C��C�� 3�� m_buffer �� 3�� 
  p_buffer���V�K�쐬�����D

  ���\ -- ���� -- ���� -- �� -- �Ȃ�
   (T)	   (T)	   (F)	 (F)	 (T)
				^^^^^^^^^^^^^^^^^^^^ �ꊇ�ǉ�

  �A���Cp_buffer �̍\���̂� connect �Ƃ��������o���ǉ�����Ă���D(��
  �}�ɂ����� (T) �� connect = TRUE�C(F) �� connect = FALSE ��\���Ă�
  ��)����́C�A��̓r���ɕʂ̌`�ԑf�ւ̂Ȃ��肪�N������̂�h������
  �ł���D��̓I�ɂ́C�A����̌`�ԑf�̂����C�Ō�̌`�ԑf�ɑΉ����镔��
  �ȊO�� p_buffer�� connect �� FALSE �Ƃ���D����ɂ���āC���̂悤��
  ���e�B�X�̐������֎~�ł���D

  ���\ -- ���� -- ���� -- �� -- �Ȃ�
					   �_
						  �ɂ� -- ....	(�A����ւ̊��荞��)

  �y���ߏ����z

  ���ʂ�󔒂Ȃǂ������ꍇ�C���̑O�̌`�ԑf�̘A�ڑ����𓧉߂����鏈���D
  (��) �w�ЂƂ��Ɓx�Łc�c�c   �Ƃ����������͂��ꂽ�ꍇ
  �܂��C���̂悤�ȃ��e�B�X�����������

  �ЂƂ��Ɓ@--	�x(con_tbl �͊��ʕ̂���)
			�^
	  ����

  ���̒���� through_word ���Ă΂�C�V���� 2�� m_buffer �� 2��	
  p_buffer ���V�K�쐬�����D�V�K�쐬���ꂽ m_buffer�́Ccon_tbl ������
  �قȂ��Ă��āC���̑��̓I���W�i���ƑS�������ł���D

  �ЂƂ��Ɓ@--	�x(�u�ЂƂ��Ɓv�Ɠ��� con_tbl)
			�_
				�x(con_tbl �͊��ʕ̂���)
			�^	
	  ���Ɓ@--	�x(�u���Ɓv�Ɠ��� con_tbl)

  �ȏ�̂悤�ɁC�l�X�ȘA�ڑ���(con_tbl)�������� m_buffer �y�� p_buffer	
  �����A���邱�Ƃɂ���āC���ߏ������������Ă���D

  (�������C���̂܂܂ł͑�ʂ̋󔒂Ȃǂ������ꍇ��m_buffer, p_buffer ��
  ���Ă��܂��̂ŁC�d��������̂��o���邾���ė��p���C�}�����K�X�s��
  ���Ƃɂ���āC������h���ł���D)

  �y���������z

  �����̒���ɐ����������ꍇ�ɁC����2��A�������V���Ȍ`�ԑf��ǉ�����D

  ���� -- �� -- �Q

  ���̎��ɐ����u�O�v�������ꍇ�C�܂����̂悤�ȃ��e�B�X�����������D

  ���� -- �� -- �Q -- �O

  ���̒���� suusi_word ���Ă΂�C�V���ɐ����u�Q�O�v���ǉ������D

  ���� -- �� -- �Q -- �O
			 �_
				�Q�O

  �������C���́u�Q�O�v�̌`�ԑf�R�X�g�͌��́u�O�v�̕��̌`�ԑf�R�X�g��
  �R�s�[�����D
  */

/*
------------------------------------------------------------------------------
	inclusion of header files
------------------------------------------------------------------------------ */

//#ifdef HAVE_CONFIG_H
#include <juman/config.h>
//#endif

#include	<juman/juman.h>

/*
------------------------------------------------------------------------------
	definition of macros
------------------------------------------------------------------------------
*/

#ifdef	_WINDOWS
static char* DEF_CLASS_C =	  NULL;
static char* DEF_RENSETSU_W = NULL;
static char* DEF_KEITAISO_W = NULL;
static char* DEF_COST_HABA =  NULL;
static char* DEF_DIC_FILE =   NULL;
static char* DEF_GRAM_FILE =  NULL;
static char* DEF_SUUSI_HINSI =	  NULL;
static char* DEF_SUUSI_BUNRUI =   NULL;
static char* DEF_KAKKO_HINSI =	  NULL;
static char* DEF_KAKKO_BUNRUI1 =  NULL;
static char* DEF_KAKKO_BUNRUI2 =  NULL;
static char* DEF_KUUHAKU_HINSI =  NULL;
static char* DEF_KUUHAKU_BUNRUI = NULL;
static char* DEF_UNDEF =	  NULL;
static char* DEF_UNDEF_KATA = NULL;
static char* DEF_UNDEF_ALPH = NULL;
static char* DEF_UNDEF_ETC =  NULL;
static char* BASIC_FORM = NULL;
static char* buntou = NULL;
static char* bunmatsu = NULL;
static char DEF_CLASS_C_2[] =	 "�i���R�X�g";
static char DEF_RENSETSU_W_2[] = "�A�ڃR�X�g�d��";
static char DEF_KEITAISO_W_2[] = "�`�ԑf�R�X�g�d��";
static char DEF_COST_HABA_2[] =  "�R�X�g��";
static char DEF_DIC_FILE_2[] =	 "�����t�@�C��";
static char DEF_GRAM_FILE_2[] =  "���@�t�@�C��";
static char DEF_SUUSI_HINSI_2[] =	 "����";
static char DEF_SUUSI_BUNRUI_2[] =	 "����";
static char DEF_KAKKO_HINSI_2[] =	 "����";
static char DEF_KAKKO_BUNRUI1_2[] =  "���ʎn";
static char DEF_KAKKO_BUNRUI2_2[] =  "���ʏI";
static char DEF_KUUHAKU_HINSI_2[] =  "����";
static char DEF_KUUHAKU_BUNRUI_2[] = "��";
static char DEF_UNDEF_2[] = 	 "����`��";
static char DEF_UNDEF_KATA_2[] = "�J�^�J�i";
static char DEF_UNDEF_ALPH_2[] = "�A���t�@�x�b�g";
static char DEF_UNDEF_ETC_2[] =  "���̑�";
static char BASIC_FORM2[] = "��{�`";
static char buntou_2[] = "(����)";
static char bunmatsu_2[] = "(����)";
#endif
static int str_init = 0;
static void init_str() {
	if (str_init) return;
	str_init = 1;
#ifdef	_WINDOWS
	DEF_CLASS_C =	 toStringEUC(DEF_CLASS_C_2);
	DEF_RENSETSU_W = toStringEUC(DEF_RENSETSU_W_2);
	DEF_KEITAISO_W = toStringEUC(DEF_KEITAISO_W_2);
	DEF_COST_HABA =  toStringEUC(DEF_COST_HABA_2);
	DEF_DIC_FILE =	 toStringEUC(DEF_DIC_FILE_2);
	DEF_GRAM_FILE =  toStringEUC(DEF_GRAM_FILE_2);
	DEF_SUUSI_HINSI =	 toStringEUC(DEF_SUUSI_HINSI_2);
	DEF_SUUSI_BUNRUI =	 toStringEUC(DEF_SUUSI_BUNRUI_2);
	DEF_KAKKO_HINSI =	 toStringEUC(DEF_KAKKO_HINSI_2);
	DEF_KAKKO_BUNRUI1 =  toStringEUC(DEF_KAKKO_BUNRUI1_2);
	DEF_KAKKO_BUNRUI2 =  toStringEUC(DEF_KAKKO_BUNRUI2_2);
	DEF_KUUHAKU_HINSI =  toStringEUC(DEF_KUUHAKU_HINSI_2);
	DEF_KUUHAKU_BUNRUI = toStringEUC(DEF_KUUHAKU_BUNRUI_2);
	DEF_UNDEF = 	 toStringEUC(DEF_UNDEF_2);
	DEF_UNDEF_KATA = toStringEUC(DEF_UNDEF_KATA_2);
	DEF_UNDEF_ALPH = toStringEUC(DEF_UNDEF_ALPH_2);
	DEF_UNDEF_ETC =  toStringEUC(DEF_UNDEF_ETC_2);
	BASIC_FORM = toStringEUC(BASIC_FORM2);
	buntou = toStringEUC(buntou_2);
	bunmatsu = toStringEUC(bunmatsu_2);
#endif
}
void final_str_juman_lib() {
	if (str_init == 0) return;
	str_init = 0;
#ifdef	_WINDOWS
	free(DEF_CLASS_C); DEF_CLASS_C = NULL;
	free(DEF_RENSETSU_W); DEF_RENSETSU_W = NULL;
	free(DEF_KEITAISO_W); DEF_KEITAISO_W = NULL;
	free(DEF_COST_HABA); DEF_COST_HABA = NULL;
	free(DEF_DIC_FILE); DEF_DIC_FILE = NULL;
	free(DEF_GRAM_FILE); DEF_GRAM_FILE = NULL;
	free(DEF_SUUSI_HINSI); DEF_SUUSI_HINSI = NULL;
	free(DEF_SUUSI_BUNRUI); DEF_SUUSI_BUNRUI = NULL;
	free(DEF_KAKKO_HINSI); DEF_KAKKO_HINSI = NULL;
	free(DEF_KAKKO_BUNRUI1); DEF_KAKKO_BUNRUI1 = NULL;
	free(DEF_KAKKO_BUNRUI2); DEF_KAKKO_BUNRUI2 = NULL;
	free(DEF_KUUHAKU_HINSI); DEF_KUUHAKU_HINSI = NULL;
	free(DEF_KUUHAKU_BUNRUI); DEF_KUUHAKU_BUNRUI = NULL;
	free(DEF_UNDEF); DEF_UNDEF = NULL;
	free(DEF_UNDEF_KATA); DEF_UNDEF_KATA =	NULL;
	free(DEF_UNDEF_ALPH); DEF_UNDEF_ALPH = NULL;
	free(DEF_UNDEF_ETC); DEF_UNDEF_ETC = NULL;
	free(BASIC_FORM); BASIC_FORM = NULL;
	free(buntou); buntou = NULL;
	free(bunmatsu); bunmatsu = NULL;
#endif
}
#ifdef	_LINUX
static char DEF_CLASS_C[] =    "�i���R�X�g";
static char DEF_RENSETSU_W[] = "�A�ڃR�X�g�d��";
static char DEF_KEITAISO_W[] = "�`�ԑf�R�X�g�d��";
static char DEF_COST_HABA[] =  "�R�X�g��";
static char DEF_DIC_FILE[] =   "�����t�@�C��";
static char DEF_GRAM_FILE[] =  "���@�t�@�C��";
static char DEF_SUUSI_HINSI[] =    "����";
static char DEF_SUUSI_BUNRUI[] =   "����";
static char DEF_KAKKO_HINSI[] =    "����";
static char DEF_KAKKO_BUNRUI1[] =  "���ʎn";
static char DEF_KAKKO_BUNRUI2[] =  "���ʏI";
static char DEF_KUUHAKU_HINSI[] =  "����";
static char DEF_KUUHAKU_BUNRUI[] = "��";
static char DEF_UNDEF[] =	   "����`��";
static char DEF_UNDEF_KATA[] = "�J�^�J�i";
static char DEF_UNDEF_ALPH[] = "�A���t�@�x�b�g";
static char DEF_UNDEF_ETC[] =  "���̑�";
static char BASIC_FORM[] = "��{�`";
static char buntou[] = "(����)";
static char bunmatsu[] = "(����)";
#endif

extern int get_hinsi_id(U_CHAR *x);
extern int get_bunrui_id(U_CHAR *x, int hinsi);

/*
------------------------------------------------------------------------------
	GLOBAL:
	definition of global variables			>>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/

char			*ProgName;
FILE			*db;

extern CLASS	Class[CLASSIFY_NO + 1][CLASSIFY_NO + 1];
extern TYPE 	Type[TYPE_NO];
extern FORM 	Form[TYPE_NO][FORM_NO];
extern RENSETU_PAIR 	*rensetu_tbl;
extern U_CHAR			*rensetu_mtr;

DIC_FILES		DicFile;

COST_OMOMI		cost_omomi;    
char			Jumangram_Dirname[FILENAME_MAX]; 

int 			LineNo;
int 			LineNoForError; 	  /* k.n */

char			pat_buffer[50000];

/*
------------------------------------------------------------------------------
	LOCAL:
	definition of global variables			>>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/
JUMAN_KNP_DATA int	   Show_Opt1;
JUMAN_KNP_DATA int	   Show_Opt2;
JUMAN_KNP_DATA char Show_Opt_tag[MIDASI_MAX];
JUMAN_KNP_DATA int		Show_Opt_jumanrc;
JUMAN_KNP_DATA int		Show_Opt_debug;

JUMAN_KNP_DATA U_CHAR StringBuff[LENMAX];
int 			m_buffer_num;
int 			Jiritsu_buffer[CLASSIFY_NO + 1];
int 			undef_hinsi;
int 			undef_kata_bunrui, undef_alph_bunrui, undef_etc_bunrui;
int 			undef_kata_con_tbl, undef_alph_con_tbl, undef_etc_con_tbl;
int 			suusi_hinsi, suusi_bunrui;
int 			kakko_hinsi, kakko_bunrui1, kakko_bunrui2;
int 			kuuhaku_hinsi, kuuhaku_bunrui, kuuhaku_con_tbl;
int 			jiritsu_num;
int 			p_buffer_num;
CONNECT_COST	connect_cache[CONNECT_MATRIX_MAX];

/* MRPH_BUFFER_MAX �̐�����P�p�C���I�Ƀ������m�� */
int 	 mrph_buffer_max = 0;
MRPH *			 m_buffer;
int *			 m_check_buffer;

/* PROCESS_BUFFER_MAX �̐�����P�p�C���I�Ƀ������m�� */
int 	 process_buffer_max = 0;
PROCESS_BUFFER * p_buffer;
int *			 path_buffer;
int *		 match_pbuf;

U_CHAR		kigou[MIDASI_MAX];	 /* �擪�̋L��(@) */
U_CHAR		midasi1[MIDASI_MAX]; /* ���p */
U_CHAR		midasi2[MIDASI_MAX]; /* ���` */
U_CHAR		yomi[MIDASI_MAX];	 /* ���p�̓ǂ� */


extern COST_OMOMI		cost_omomi; 	/*k.n*/
extern char 			Jumangram_Dirname[FILENAME_MAX];  /*k.n*/

/*
------------------------------------------------------------------------------
	prototype definition of functions		>>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/

void	realloc_mrph_buffer(void);
void	realloc_process_buffer(void);
void	read_class_cost(CELL *cell); /* k.n */
static	BOOL katuyou_process(int position, int *k, MRPH mrph, int *length);
int 	search_all(int position);
int 	take_data(int pos, char **pbuf);
char	*_take_data(char *s, MRPH *mrph);
int 	numeral_decode(char **str);
int 	numeral_decode2(char **str);
void	hiragana_decode(char **str, char *yomi);
void	check_rc(void);
void	changeDictionary(int number);

int 	trim_space(int pos);
int 	undef_word(int pos);
int 	check_code(U_CHAR *cp, int position);
int 	suusi_word(int pos , int m_num);
int 	through_word(int pos , int m_num);
int 	is_through(MRPH *mrph_p);

/*
  ���ʂ̏o�͊֐��ɂ��ẮA�T�[�o�[���[�h�Ή��̂��߁A�o�͐�������Ƃ��Ď��
  �悤�ɕύX
  NACSIS �g��
*/	 
void	print_path_mrph(FILE* output, int path_num , int para_flag);
char	**print_best_path(FILE* output);
char	**print_all_mrph(FILE* output);
void	_print_all_mrph(FILE* output, int path_num);
char	**print_all_path(FILE* output);
void	_print_all_path(FILE* output, int path_num, int pathes);
int _print_homograph_path(FILE* output, int pbuf_start, int new_p);

int pos_match_process(int pos, int p_start);
int pos_right_process(int position);
int check_connect(int pos_start, int m_num);

/*
------------------------------------------------------------------------------
	PROCEDURE: <changeDictionary>		   >>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/

void changeDictionary(int number)
{

	db = DicFile.dic[number];
	DicFile.now = number;
}

/*
------------------------------------------------------------------------------
	PROCEDURE: <juman_init_rc>
------------------------------------------------------------------------------
*/
JUMAN_KNP_API BOOL juman_init_rc(FILE *fp)
{
	int  num, win32_decided = 0;
	char dic_file_name[BUFSIZE], full_file_name[BUFSIZE];
	CELL *cell1, *cell2;
	
	init_str();

	LineNo = 0 ;
	
	/* DEFAULT VALUE*/
	
	cost_omomi.keitaiso = KEITAISO_WEIGHT_DEFAULT;
	cost_omomi.rensetsu = RENSETSU_WEIGHT_DEFAULT * MRPH_DEFAULT_WEIGHT;
	cost_omomi.cost_haba = COST_WIDTH_DEFAULT * MRPH_DEFAULT_WEIGHT;

	/*	
	 *	MS Windows �͎����̃p�X��,juman.ini ����擾���� (����Dir ��1�̂� (���蔲))
	 *	������ gramfile == dicfile
	 *	Changed by Taku Kudoh (taku@pine.kuee.kyoto-u.ac.jp)
	 */
#ifdef _WIN32
	/* ���@�t�@�C�� */
	num = 0;
#ifdef	JUMAN_KNP
	art_strcpy(Jumangram_Dirname, sizeof(Jumangram_Dirname), al_juman_dic);
#else
	GetPrivateProfileString("juman","dicfile","",Jumangram_Dirname,sizeof(Jumangram_Dirname),"juman.ini");
#endif
	if (Jumangram_Dirname[0]) {
	grammar(NULL);
	katuyou(NULL);
	connect_table(NULL);
	connect_matrix(NULL);

	/* �����t�@�C�� */
#ifdef	JUMAN_KNP
	art_strcpy(dic_file_name, sizeof(dic_file_name), al_juman_dic);
#else
	GetPrivateProfileString("juman","dicfile","",dic_file_name,sizeof(dic_file_name),"juman.ini");
#endif
	if ((endchar(dic_file_name)) != '\\')
		art_strcat(dic_file_name, BUFSIZE, "\\");
	
	art_sprintf_2(full_file_name, BUFSIZE, "%s%s", dic_file_name, PATFILE);
	art_strcat(dic_file_name, BUFSIZE, DICFILE);
	DicFile.dic[num] = my_fopen(dic_file_name , "rb");
	pat_init_tree_top(&DicFile.tree_top[0]);
	com_l(full_file_name, &DicFile.tree_top[0]);
	DicFile.number = 1;
	changeDictionary(0);
	win32_decided = 1;
	}
	/* juman.ini�����p�ł��Ȃ���΁Ajumanrc����ǂ� */
#endif
	
	while (!s_feof(fp)) {	
	LineNoForError = LineNo ;
	cell1 = s_read(fp);

	/* ���@�t�@�C�� */
	if (!win32_decided && !strcmp(DEF_GRAM_FILE, (const char*)_Atom(car(cell1)))) {
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		return FALSE;
		} else {
		art_strcpy(Jumangram_Dirname , FILENAME_MAX, (const char*)_Atom(cell2));
		grammar(NULL);
		katuyou(NULL);
		connect_table(NULL);
		connect_matrix(NULL);
		}
	}
		
	/* �����t�@�C�� */
	else if (!win32_decided && !strcmp(DEF_DIC_FILE, (const char*)_Atom(car(cell1)))) {
		cell2 = cdr(cell1) ;
		for(num = 0 ; ; num++) {
		if( Null(car(cell2)) )	break;
		else if ( !Atomp( car(cell2) ) ) {
			return FALSE;
		}
		else if(num >= MAX_DIC_NUMBER)
			error(ConfigError, "Too many dictionary files.", EOA);
		else {
			art_strcpy(dic_file_name, BUFSIZE, (const char*)_Atom(car(cell2)));
			if ((endchar(dic_file_name)) != '/')
			art_strcat(dic_file_name, BUFSIZE, "/");
			cell2 = cdr(cell2);
				
			/* �����̃I�[�v�� */
			art_sprintf_2(full_file_name, BUFSIZE, "%s%s", dic_file_name, PATFILE);
			art_strcat(dic_file_name, BUFSIZE, DICFILE);
			DicFile.dic[num] = my_fopen(dic_file_name , "r");
			pat_init_tree_top(&DicFile.tree_top[num]);
			com_l(full_file_name, &DicFile.tree_top[num]);
		}
		}
		DicFile.number = num;
		changeDictionary(0);
	}
	/* �A�ڃR�X�g�d�� */
	else if (!strcmp(DEF_RENSETSU_W, (const char*)_Atom(car(cell1)))) { 
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		return FALSE;
		} else	
		cost_omomi.rensetsu =	
			(int) atoi((const char*)_Atom(cell2)) * MRPH_DEFAULT_WEIGHT;
	}
		
	/* �`�ԑf�R�X�g�d�� */
	else if (!strcmp(DEF_KEITAISO_W, (const char*)_Atom(car(cell1)))) { 
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		return FALSE;
		} else	
		cost_omomi.keitaiso = (int) atoi((const char*)_Atom(cell2));
	}
		
	/* �R�X�g�� */
	else if (!strcmp(DEF_COST_HABA, (const char*)_Atom(car(cell1)))) {	
		if (!Atomp(cell2 = car(cdr(cell1)))) {
		return FALSE;
		} else	
		cost_omomi.cost_haba =	
			(int) atoi((const char*)_Atom(cell2)) * MRPH_DEFAULT_WEIGHT;
	}
		
	/* �i���R�X�g*/
	else if (!strcmp(DEF_CLASS_C, (const char*)_Atom(car(cell1)))) {	
		read_class_cost(cdr(cell1));
	}

	/* ����`��R�X�g (3.4�ȍ~�s�v)
	else if (!strcmp("����`��i��", _Atom(car(cell1))));
	*/
	}
	return TRUE;
}

/*
------------------------------------------------------------------------------
		PROCEDURE: <juman_close>			 >>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/
void final_str_getid();
void final_str_juman_lib();
void lisp_free();
void free_pat_node();

JUMAN_KNP_API BOOL juman_close(void)
{
  int i, j;

  for(i = 0 ; i < DicFile.number ; i++)
	fclose(DicFile.dic[i]);

  free_pat_node();

  if (mrph_buffer_max > 0) {
		free(m_buffer);
		free(m_check_buffer);
		mrph_buffer_max = 0;
  }
  if (process_buffer_max > 0) {
		free(p_buffer);
		free(path_buffer);
		free(match_pbuf);
		process_buffer_max = 0;
  }

  for (i = 0; i < CLASSIFY_NO + 1; i++) {
	  for (j = 0; j < CLASSIFY_NO + 1; j++) {
		  if (Class[i][j].id) {
			  free(Class[i][j].id);
		  };
	  }
  }

  for (i = 0; i < TYPE_NO; i++) {
	  if (Type[i].name) {
		free(Type[i].name);
	  }
	  for (j = 0; j < FORM_NO; j++) {
		if (Form[i][j].name) {
			free(Form[i][j].name);
		}
		if (Form[i][j].gobi) {
			free(Form[i][j].gobi);
		}
		if (Form[i][j].gobi_yomi) {
			free(Form[i][j].gobi_yomi);
		}
	  }
  }

  for ( i=0; i<TBL_NUM; i++ ) {
	  if (rensetu_tbl[i].goi) {
		free(rensetu_tbl[i].goi);
	  }
  }

  free(rensetu_tbl);
  free(rensetu_mtr);

  lisp_free();

  final_str_getid();
  final_str_juman_lib();

  return TRUE;
}

/*
------------------------------------------------------------------------------
		PROCEDURE: <realloc_mrph_buffer>
------------------------------------------------------------------------------
*/
void	realloc_mrph_buffer(void)
{
	mrph_buffer_max += BUFFER_BLOCK_SIZE;
	m_buffer = (MRPH *)my_realloc(m_buffer, sizeof(MRPH)*mrph_buffer_max);
	m_check_buffer = (int *)my_realloc(m_check_buffer,
					   sizeof(int)*mrph_buffer_max);
}

/*
------------------------------------------------------------------------------
		PROCEDURE: <realloc_process_buffer>
------------------------------------------------------------------------------
*/
void	realloc_process_buffer(void)
{
	process_buffer_max += BUFFER_BLOCK_SIZE;
	p_buffer = (PROCESS_BUFFER *)my_realloc(p_buffer,
					sizeof(PROCESS_BUFFER)*process_buffer_max);
	path_buffer = (int *)my_realloc(path_buffer,
					   sizeof(int)*process_buffer_max);
	match_pbuf = (int *)my_realloc(match_pbuf,	
					   sizeof(int)*process_buffer_max);
}

/*
------------------------------------------------------------------------------
		PROCEDURE: <read_class_cost>
------------------------------------------------------------------------------
*/
void read_class_cost(CELL *cell)
{
	/* �i���R�X�g�̓ǂݍ��� (���̐ݒ��D��) */

	CELL *pos_cell;
	int hinsi, bunrui, cost;

	while (!Null(car(cell))) {

	pos_cell = car(car(cell)) ;
	cost = (int) atoi((const char*)_Atom(car(cdr(car(cell)))));

	if (!strcmp((const char*)_Atom(car(pos_cell)), "*")) {
		/* �i���� * �Ȃ�S�̂� */
		for (hinsi = 1 ; Class[hinsi][0].id; hinsi++)
		for (bunrui = 0 ; Class[hinsi][bunrui].id ; bunrui++)
			Class[hinsi][bunrui].cost = cost;
	}
	else {
		hinsi = get_hinsi_id(_Atom(car(pos_cell)));
		if (Null(car(cdr(pos_cell))) || 
		!strcmp((const char*)_Atom(car(cdr(pos_cell))), "*")) {
		/* �ו��ނ� * �܂��͖����Ȃ�i���S�̂� */
		for (bunrui = 0; Class[hinsi][bunrui].id ; bunrui++)
			Class[hinsi][bunrui].cost = cost;
		}
		else {
		/* �i���C�ו��ނƂ��Ɏw�肳�ꂽ�ꍇ */
		bunrui = get_bunrui_id(_Atom(car(cdr(pos_cell))), hinsi);
		Class[hinsi][bunrui].cost = cost;
		}
	}
	cell = cdr(cell);		
	}

	/* default */
	
	for (hinsi = 1; Class[hinsi][0].id; hinsi++)	
	for (bunrui = 0; Class[hinsi][bunrui].id; bunrui++)
		if (Class[hinsi][bunrui].cost == 0) 
		Class[hinsi][bunrui].cost = CLASS_COST_DEFAULT;
	
	/* For ���� ���� added by S.Kurohashi */

	Class[0][0].cost = 0;
}

/*
------------------------------------------------------------------------------
	PROCEDURE: <katuyou_process>	   >>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/
static BOOL katuyou_process(int position, int *k, MRPH mrph, int *length)
{
	 while (Form[mrph.katuyou1][*k].name) {
	  if (compare_top_str1(Form[mrph.katuyou1][*k].gobi,
				  StringBuff + position + mrph.length)) {
		   *length	   = mrph.length +	
			(int)strlen((const char*)Form[mrph.katuyou1][*k].gobi);
		   return TRUE;
	  } else {
		   (*k)++;
	  }
	 }
	 return FALSE;
}

/*
------------------------------------------------------------------------------
	PROCEDURE: <search_all> 	  >>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/
int search_all(int position)
{
	int 		dic_no;
	char	*pbuf;

	for(dic_no = 0 ; dic_no < DicFile.number ; dic_no++) {
	changeDictionary(dic_no);
	
	/* �p�g���V�A�؂���`�ԑf������ */
	pat_buffer[0] = '\0';
	pat_search(db, (char*)(StringBuff + position), &DicFile.tree_top[dic_no],
		   pat_buffer);
	pbuf = pat_buffer;

	while (*pbuf != '\0') {
		if (take_data(position, &pbuf) == FALSE) return FALSE;
	}
	}
	return TRUE;
}

/*
------------------------------------------------------------------------------
		PROCEDURE: <take_data>					>>> Changed by yamaji <<<
------------------------------------------------------------------------------
*/
int take_data(int pos, char **pbuf)
{
	unsigned char	 *s;
	int 	i, k, num;
	int 	rengo_con_tbl, rengo_weight;
	MRPH	mrph;
	MRPH	*new_mrph;
	int 	length, con_tbl_bak, k2, pnum_bak;
	int 	new_mrph_num;

	s = (unsigned char*)*pbuf;

	k = 0 ;
	while ((mrph.midasi[k++] = *(s++)) != '\t') {}
	mrph.midasi[k-1] = '\0';

	if (*s == 0xff) { /* �A���񂾂����ꍇ */

#define 	RENGO_BUFSIZE		20

	MRPH mrph_buf[RENGO_BUFSIZE];
	int add_list[FORM_NO];
	int co, pos_bak, glen, cno;
	
	pos_bak = pos;

	s = (unsigned char*)_take_data((char*)s , &mrph);
	rengo_con_tbl = mrph.con_tbl;
	rengo_weight  = mrph.weight;
	num = mrph.bunrui;

	for (i = 0; i < num; i++) { /* �܂��C�A��S�̂��o�b�t�@�ɓǂݍ��� */
		new_mrph = &mrph_buf[i];

		k = 0;
		while ((new_mrph->midasi[k++] = *(s++)) != ' ') {}
		new_mrph->midasi[k-1] = '\0';

		s = (unsigned char*)_take_data((char*)s , new_mrph);

		length = (int)strlen((const char*)new_mrph->midasi);
		if (Class[new_mrph->hinsi][new_mrph->bunrui].kt) /* ���p���� */
		if (i < num-1) { /* �����ȊO�̊��p�� */
			length += (int)strlen(
			(const char*)Form[new_mrph->katuyou1][new_mrph->katuyou2].gobi);
			new_mrph->con_tbl += (new_mrph->katuyou2-1);
		}
		new_mrph->length = length;

		if (i < num-1) pos += length; /* �����`�ԑf�̈ʒu��ۑ� */
/*printf(".....%d/%d   %s %d\n", i, num, new_mrph->midasi, new_mrph->con_tbl);*/
	}

	new_mrph = &mrph_buf[num-1];
	if (Class[new_mrph->hinsi][new_mrph->bunrui].kt == 0) {
		add_list[0] = 0;
		add_list[1] = -1;
	} else {
		if (new_mrph->katuyou2) {
		add_list[0] = new_mrph->katuyou2;
		add_list[1] = -1;
		} else {
		/* �����̌`�ԑf�����p����ꍇ */
		k2 = strlen((const char*)new_mrph->midasi) ? 1 : 2;
		co = 0;
		while (katuyou_process(pos, &k2, *new_mrph, &length)) {
			add_list[co++] = k2;
			k2++;
		}
		add_list[co] = -1;
		}
	}

	/* ���p�`���ƂɘA���ǉ����Ă��� */
	for (co = 0; add_list[co] >= 0; co++) {
		pos = pos_bak;
		for (i = 0; i < num; i++) {
		m_buffer[m_buffer_num] = mrph_buf[i];
		new_mrph = &m_buffer[m_buffer_num];

		if (i == 0) { /* �A��̐擪�̌`�ԑf */
			con_tbl_bak = new_mrph->con_tbl;
/*printf("FROM %d %d\n", new_mrph->con_tbl, rengo_con_tbl);*/
			if (rengo_con_tbl != -1) {
			if (add_list[co]) cno = add_list[co]-1; else cno = 0;
			if (check_matrix_left(rengo_con_tbl+cno) == 1 /*TRUE*/)
				/* �A����L�̍��A�ڋK�� */
				new_mrph->con_tbl = rengo_con_tbl+cno;
			}
/*printf("  TO %d\n", new_mrph->con_tbl);*/
			pnum_bak = p_buffer_num;

			if (Show_Opt_debug) {
			printf("\\begin{�`�ԑf���A��} %s", mrph.midasi);
			if (add_list[co])
				printf("%s",
				   Form[mrph_buf[num-1].katuyou1][add_list[co]].gobi);
			printf("\n");
			}
			
			check_connect(pos, m_buffer_num);
			if (p_buffer_num == pnum_bak) break;

			p_buffer[pnum_bak].end = pos + new_mrph->length;
			p_buffer[pnum_bak].connect = FALSE;
			p_buffer[pnum_bak].score = p_buffer[pnum_bak].score +
			(Class[new_mrph->hinsi][new_mrph->bunrui].cost
			 * new_mrph->weight * cost_omomi.keitaiso	
			 * (rengo_weight-10)/10);
			new_mrph->con_tbl = con_tbl_bak;

			if (Show_Opt_debug) {
			printf("----- �A��� %s %d\n", new_mrph->midasi,	
				   p_buffer[pnum_bak].score);
			}
		} else {
			p_buffer[p_buffer_num].score =
			p_buffer[p_buffer_num-1].score +
			(Class[new_mrph->hinsi][new_mrph->bunrui].cost
			 * new_mrph->weight * cost_omomi.keitaiso +
			 (check_matrix(m_buffer[p_buffer[p_buffer_num-1].mrph_p].con_tbl, new_mrph->con_tbl) ?	
			  check_matrix(m_buffer[p_buffer[p_buffer_num-1].mrph_p].con_tbl, new_mrph->con_tbl) : DEFAULT_C_WEIGHT)
			 /* �A����̐ڑ��͐ڑ��\�Œ�`����Ă��Ȃ��ꍇ������
				���̏ꍇ�́C�ڑ��̃R�X�g�̓f�t�H���g�̒l�Ƃ���D */
			 * cost_omomi.rensetsu)
			 * rengo_weight/10;
			p_buffer[p_buffer_num].mrph_p = m_buffer_num;
			p_buffer[p_buffer_num].start = pos;
			p_buffer[p_buffer_num].end = pos + new_mrph->length;
			p_buffer[p_buffer_num].path[0] = p_buffer_num-1;
			p_buffer[p_buffer_num].path[1] = -1;

			if (i < num-1) { /* �A��̒����̌`�ԑf */
			p_buffer[p_buffer_num].connect = FALSE;
			} else { /* �A��̖����̌`�ԑf */
			p_buffer[p_buffer_num].connect = TRUE;

			if (rengo_con_tbl != -1) {
				if (add_list[co]) cno = add_list[co]-1;
				else cno = 0;
				if (check_matrix_right(rengo_con_tbl+cno) == 1 /*TRUE*/)
				/* �A����L�̉E�A�ڋK�� */
				new_mrph->con_tbl = rengo_con_tbl;
			}
			if (add_list[co]) {
				new_mrph->katuyou2 = (char)add_list[co];
				new_mrph->con_tbl += (add_list[co]-1);
				glen = (int)strlen((const char*)Form[new_mrph->katuyou1]
					  [new_mrph->katuyou2].gobi);
				new_mrph->length += glen;
				p_buffer[p_buffer_num].end += glen;
			}
			}
			if (Show_Opt_debug) {
			printf("----- �A��� %s %d\n", new_mrph->midasi,	
				   p_buffer[p_buffer_num].score);
			}

			if (++p_buffer_num == process_buffer_max)
			realloc_process_buffer();
		}
		pos += new_mrph->length;

		/* fixed by kuro on 01/01/19
		   �ȉ���realloc�����if-else�̒����������̂ŁCnew_mrph�ւ�
		   �A�N�Z�X�� segmentation fault �ƂȂ��Ă��� */
		if (++m_buffer_num == mrph_buffer_max)
		  realloc_mrph_buffer();
		}
		if (Show_Opt_debug) {
		printf("\\end{�`�ԑf���A��}\n");
		}
	}

	} else {		   /* ���ʂ̌`�ԑf�������ꍇ */
	s = (unsigned char*)_take_data((char*)s , &mrph);

	if ( Class[mrph.hinsi][mrph.bunrui].kt ) { /* ���p���� */
		if ( mrph.katuyou2 == 0 ) {   /* �ꊲ���� */
		k2 = 1;
		while (katuyou_process(pos, &k2, mrph, &length)) {
			m_buffer[m_buffer_num] = mrph;
			m_buffer[m_buffer_num].katuyou2 = (char)k2;
			m_buffer[m_buffer_num].length = length;
			m_buffer[m_buffer_num].con_tbl += (k2 - 1);
			check_connect(pos, m_buffer_num);
			if (++m_buffer_num == mrph_buffer_max)
			realloc_mrph_buffer();
			k2++;
		}
		} else {						 /* �ꊲ�Ȃ� */
		m_buffer[m_buffer_num] = mrph;
		m_buffer[m_buffer_num].midasi[0] = '\0';
		m_buffer[m_buffer_num].yomi[0]	= '\0';
		check_connect(pos, m_buffer_num);
		if (++m_buffer_num == mrph_buffer_max)
			realloc_mrph_buffer();
		}
	} else {									 /* ���p���Ȃ� */
		m_buffer[m_buffer_num] = mrph;
		check_connect(pos, m_buffer_num);
		new_mrph_num = m_buffer_num;
		if (++m_buffer_num == mrph_buffer_max)
		realloc_mrph_buffer();

#ifdef NUMERIC_P
		if (suusi_word(pos, new_mrph_num) == FALSE)
		return FALSE;
#endif
#ifdef THROUGH_P
		if (through_word(pos, new_mrph_num) == FALSE)
		return FALSE;
#endif
	}

	}
	*pbuf = (char*)s;
	return TRUE;
}

/*
------------------------------------------------------------------------------
		PROCEDURE: <_take_data> 				>>> added by T.Nakamura <<<
------------------------------------------------------------------------------
*/

char *_take_data(char *s , MRPH *mrph)
{
	int 	i, j, k = 0;
	
	mrph->hinsi    = (char)numeral_decode(&s);
	mrph->bunrui   = (char)numeral_decode(&s);
	mrph->katuyou1 = (char)numeral_decode(&s);
	mrph->katuyou2 = (char)numeral_decode(&s);
	mrph->weight   = (unsigned char)numeral_decode(&s);
	mrph->con_tbl  = numeral_decode2(&s);
	hiragana_decode(&s, (char*)mrph->yomi);
	mrph->length   = (int)strlen((const char*)mrph->midasi);

	if (*s != ' ' && *s != '\n') { /* �Ӗ���񂠂� */
	j = numeral_decode(&s);
	for (i = 0; i < j; i++) mrph->imis[k++] = *(s++);
	mrph->imis[k] = '\0';
	}
	s++;
	if (k == 0) art_strcpy((char*)mrph->imis, IMI_MAX, NILSYMBOL);

	return(s);
}

int numeral_decode(char **str)
{
	unsigned char *s;

	s = (unsigned char*)*str;
	if (*s < 0xf0) {
	*str = (char*)(s+1);
	return(*s-0x20);
	} else if (*s == 0xff) {
	*str = (char*)(s+1);
	return(atoi(RENGO_ID));
	} else {
	*str = (char*)(s+2);
	return((*s-0xf0)*(0xf0-0x20)+*(s+1)-0x20);
	}
}

int numeral_decode2(char **str)
{
	unsigned char *s;

	s = (unsigned char*)*str;
	*str = (char*)(s+2);
	return((*s-0x20)*(0x100-0x20)+*(s+1)-0x20-1);
}

void hiragana_decode(char **str, char *yomi)
{
	unsigned char *s;

	s = (unsigned char*)*str;
	while (*s != 0x20) {
	if (*s < 0x80) {
		int c = 0xa4;
		*(yomi++) = (char)c;
		*(yomi++) = *s-0x21+0xa0;
		s++;
	} else {
		*(yomi++) = *(s++);
		*(yomi++) = *(s++);
	}
	}
	*str = (char*)(s+1);
	*yomi = '\0';
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <trim_space> �X�y�[�X�̍폜 (���ߏ���������g�p����)
------------------------------------------------------------------------------
*/
int trim_space(int pos)
{
	while (1) {
	/* �S�p�X�y�[�X */
	if (StringBuff[pos] == 0xa1 && StringBuff[pos+1] == 0xa1)
	  pos += 2;
	else
	  break;
	}
	return pos;
}
	
/*
------------------------------------------------------------------------------
  PROCEDURE: <undef_word> ����`�ꏈ��
------------------------------------------------------------------------------
*/
	
int undef_word(int pos)
{
	int end, code, next_code;
	
	/* ����ɂ��؂肾��
	   �������C�����C���p�� �� �ꕶ��
	   ���p(�󔒈ȊO)�C�Љ���(�u�[�v��)�C�A���t�@�x�b�g(�u�D�v��) �� �A�� */

	code = check_code(StringBuff, pos);
	if (code == HIRAGANA || code == KANJI) {
	end = (pos + 2);
	} else if (code == KUUHAKU) {
	end = (pos + 1);
	} else {
	end = pos;
	while(1) {
		end += (code == HANKAKU) ? 1 : 2;
		next_code = check_code(StringBuff, end);
		if (next_code == code ||
		(code == KATAKANA && next_code == CHOON) ||
		(code == ALPH	  && next_code == PRIOD)) continue;
		else break;
	}
	}

	switch (code) {
	case KUUHAKU:
	m_buffer[m_buffer_num].hinsi = (char)kuuhaku_hinsi;
	m_buffer[m_buffer_num].bunrui = (char)kuuhaku_bunrui;
	m_buffer[m_buffer_num].con_tbl = kuuhaku_con_tbl;
	break;
	case KATAKANA:
	m_buffer[m_buffer_num].hinsi = (char)undef_hinsi;
	m_buffer[m_buffer_num].bunrui = (char)undef_kata_bunrui;
	m_buffer[m_buffer_num].con_tbl = undef_kata_con_tbl;
	break;
	case ALPH:
	m_buffer[m_buffer_num].hinsi = (char)undef_hinsi;
	m_buffer[m_buffer_num].bunrui = (char)undef_alph_bunrui;
	m_buffer[m_buffer_num].con_tbl = undef_alph_con_tbl;
	break;
	default:
	m_buffer[m_buffer_num].hinsi = (char)undef_hinsi;
	m_buffer[m_buffer_num].bunrui = (char)undef_etc_bunrui;
	m_buffer[m_buffer_num].con_tbl = undef_etc_con_tbl;
	break;
	}

	m_buffer[m_buffer_num].katuyou1 = 0;
	m_buffer[m_buffer_num].katuyou2 = 0;
	m_buffer[m_buffer_num].length = end - pos;
	if (end - pos >= MIDASI_MAX) {
	fprintf(stderr, "Too long undef_word<%s>\n", StringBuff);
	return FALSE;
	}
	if (code == KUUHAKU) {
	art_strcpy((char*)m_buffer[m_buffer_num].midasi, MIDASI_MAX, "\\ ");
	art_strcpy((char*)m_buffer[m_buffer_num].yomi, YOMI_MAX, "\\ ");
	} else {
	art_strncpy((char*)m_buffer[m_buffer_num].midasi, MIDASI_MAX, (const char*)(StringBuff+pos), end - pos);
	m_buffer[m_buffer_num].midasi[end - pos] = '\0';
	art_strncpy((char*)m_buffer[m_buffer_num].yomi, YOMI_MAX, (const char*)(StringBuff+pos), end - pos);
	m_buffer[m_buffer_num].yomi[end - pos] = '\0';
	}
	m_buffer[m_buffer_num].weight = MRPH_DEFAULT_WEIGHT;
	art_strcpy((char*)m_buffer[m_buffer_num].imis, IMI_MAX, NILSYMBOL);

	check_connect(pos, m_buffer_num);
	if (++m_buffer_num == mrph_buffer_max)
	realloc_mrph_buffer();

#ifdef THROUGH_P				/* ���p�X�y�[�X���� */
	if (code == KUUHAKU) {
	return (through_word(pos, m_buffer_num-1));
	}
#endif

	return TRUE;
}

int check_code(U_CHAR *cp, int pos)
{
	int code;
	
	if ( cp[pos] == '\0')			return 0;
	else if ( cp[pos] == KUUHAKU )		return KUUHAKU;
	else if ( cp[pos] < HANKAKU )		return HANKAKU;
	
	code = cp[pos]*256 + cp[pos+1];
	
	if ( code == PRIOD )			return PRIOD;
	else if ( code == CHOON )			return CHOON;
	else if ( code < KIGOU )			return KIGOU;
	else if ( code < SUJI ) 			return SUJI;
	else if ( code < ALPH ) 		return ALPH;
	else if ( code < HIRAGANA ) 		return HIRAGANA;
	else if ( code < KATAKANA ) 		return KATAKANA;
	else if ( code < GR )			return GR;
	else return KANJI;
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <juman_init_etc> ����`�ꏈ���C���������C���ߏ������̏���
------------------------------------------------------------------------------
*/
JUMAN_KNP_API void juman_init_etc(void)
{
	init_str();

	/* ����`�ꏈ���̏��� */
	undef_hinsi = get_hinsi_id((unsigned char*)DEF_UNDEF);
	undef_kata_bunrui = get_bunrui_id((unsigned char*)DEF_UNDEF_KATA, undef_hinsi);
	undef_alph_bunrui = get_bunrui_id((unsigned char*)DEF_UNDEF_ALPH, undef_hinsi);
	undef_etc_bunrui = get_bunrui_id((unsigned char*)DEF_UNDEF_ETC, undef_hinsi);
	undef_kata_con_tbl = check_table_for_undef(undef_hinsi, undef_kata_bunrui);
	undef_alph_con_tbl = check_table_for_undef(undef_hinsi, undef_alph_bunrui);
	undef_etc_con_tbl = check_table_for_undef(undef_hinsi, undef_etc_bunrui);

	/* ���������̏��� */
	suusi_hinsi = get_hinsi_id((unsigned char*)DEF_SUUSI_HINSI);
	suusi_bunrui = get_bunrui_id((unsigned char*)DEF_SUUSI_BUNRUI, suusi_hinsi);

	/* ���ߏ����̏��� */
	kakko_hinsi = get_hinsi_id((unsigned char*)DEF_KAKKO_HINSI);
	kakko_bunrui1 = get_bunrui_id((unsigned char*)DEF_KAKKO_BUNRUI1, kakko_hinsi);
	kakko_bunrui2 = get_bunrui_id((unsigned char*)DEF_KAKKO_BUNRUI2, kakko_hinsi);

	kuuhaku_hinsi = get_hinsi_id((unsigned char*)DEF_KUUHAKU_HINSI);
	kuuhaku_bunrui = get_bunrui_id((unsigned char*)DEF_KUUHAKU_BUNRUI, kuuhaku_hinsi);
	kuuhaku_con_tbl = check_table_for_undef(kuuhaku_hinsi, kuuhaku_bunrui);
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <suusi_word> ��������
------------------------------------------------------------------------------
*/
	
int 	suusi_word(int pos , int m_num)
{
	int i, j;
	MRPH *new_mrph , *pre_mrph;

	new_mrph = &m_buffer[m_num];
	if (new_mrph->hinsi != suusi_hinsi || new_mrph->bunrui != suusi_bunrui)
		return TRUE;

	for (j = 0; (i = match_pbuf[j]) >= 0; j++) {
	pre_mrph = &m_buffer[p_buffer[i].mrph_p];
	if (pre_mrph->hinsi == suusi_hinsi &&
		pre_mrph->bunrui == suusi_bunrui &&
		check_matrix(pre_mrph->con_tbl, new_mrph->con_tbl) != 0) {

		if (strlen((const char*)pre_mrph->midasi)+strlen((const char*)new_mrph->midasi) >= MIDASI_MAX
		|| strlen((const char*)pre_mrph->yomi)+strlen((const char*)new_mrph->yomi) >= YOMI_MAX) {
		fprintf(stderr, "Too long suusi<%s>\n", StringBuff);
		return FALSE;
		}
		m_buffer[m_buffer_num] = *pre_mrph;
		art_strcat((char*)m_buffer[m_buffer_num].midasi , MIDASI_MAX, (const char*)new_mrph->midasi);
		art_strcat((char*)m_buffer[m_buffer_num].yomi	, YOMI_MAX, (const char*)new_mrph->yomi);
		m_buffer[m_buffer_num].length += (int)strlen((const char*)new_mrph->midasi);
		/* �R�X�g�͌�둤�̕����p�� */
		m_buffer[m_buffer_num].weight = new_mrph->weight;
		m_buffer[m_buffer_num].con_tbl = new_mrph->con_tbl;

		p_buffer[p_buffer_num] = p_buffer[i];
		p_buffer[p_buffer_num].end = pos+ (int)strlen((const char*)new_mrph->midasi);
		p_buffer[p_buffer_num].mrph_p = m_buffer_num;
		p_buffer[p_buffer_num].score += (new_mrph->weight-pre_mrph->weight)
		*Class[new_mrph->hinsi][new_mrph->bunrui].cost
		*cost_omomi.keitaiso;

		if (++m_buffer_num == mrph_buffer_max) {
		realloc_mrph_buffer();
			new_mrph = &m_buffer[m_num];	/* fixed by kuro 99/09/01 */
		}
		if (++p_buffer_num == process_buffer_max)
		realloc_process_buffer();
	}
	}
	return TRUE;
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <through_word> ���ʁC�󔒂̓��ߏ���
------------------------------------------------------------------------------
*/
	
int 	through_word(int pos , int m_num)
{
	int i, j, k, l, n, nn, sc, scmin;
	MRPH *now_mrph, *mrph_p;

	now_mrph = &m_buffer[m_num];
	if (!is_through(now_mrph)) return TRUE;

	for (l = 0; (i = match_pbuf[l]) >= 0; l++) {
	for (j = 0 ; j < m_buffer_num ; j++) {
		mrph_p = &m_buffer[j];
		if (mrph_p->hinsi	== now_mrph->hinsi	 &&
		mrph_p->bunrui	== now_mrph->bunrui  &&
		mrph_p->con_tbl == m_buffer[p_buffer[i].mrph_p].con_tbl &&
		mrph_p->weight	== now_mrph->weight  &&
		strcmp((const char*)mrph_p->midasi, (const char*)now_mrph->midasi) == 0 &&
		strcmp((const char*)mrph_p->yomi,	(const char*)now_mrph->yomi  ) == 0) break;
	}
	if ((n = j) == m_buffer_num) {
		/* ���̕��̒��ŏ��߂ďo�������^�C�v�̊��ʂł���΁C���O��
		   �`�ԑf��con_tbl�𓧉߂�����悤��con_tbl��ω������āC
		   m_buffer�ɒǉ����� */
		m_buffer[m_buffer_num] = *now_mrph;
		m_buffer[m_buffer_num].con_tbl
		  = m_buffer[p_buffer[i].mrph_p].con_tbl;
		if (++m_buffer_num == mrph_buffer_max) {
		realloc_mrph_buffer();
		now_mrph = &m_buffer[m_num];	/* fixed by kuro 99/09/01 */
		}
	}

	/* ���ߋK���Ɋ�Â��A�� */
	sc = (now_mrph->weight*cost_omomi.keitaiso*
		  Class[now_mrph->hinsi][now_mrph->bunrui].cost);
	for (j = 0 ; j < p_buffer_num ; j++)
	  if (p_buffer[j].mrph_p == n && p_buffer[j].start == pos) break;
	if ((nn = j) == p_buffer_num) {
		/* ���߂Č�����悤�ȓ��߂Ȃ�Cp_buffer�ɐV���ɒǉ����� */
		p_buffer[p_buffer_num].score = p_buffer[i].score+sc;
		p_buffer[p_buffer_num].mrph_p = n;
		p_buffer[p_buffer_num].start = pos;
		p_buffer[p_buffer_num].end = pos + now_mrph->length;
		p_buffer[p_buffer_num].path[0] = i;
		p_buffer[p_buffer_num].path[1] = -1;
		p_buffer[p_buffer_num].connect = TRUE;
		if (++p_buffer_num == process_buffer_max)
		realloc_process_buffer();
	} else {
		/* p_buffer�̔�����h�����߁C�ȑO��p_buffer�̎g���񂵂����� */
		for (j = 0 ; p_buffer[nn].path[j] != -1 ; j++) {}
		p_buffer[nn].path[j]   = i;
		p_buffer[nn].path[j+1] = -1;

		/* �R�X�g���������͎̂}���肷�� */
		scmin = INT_MAX;
		for (j = 0 ; p_buffer[nn].path[j] != -1 ; j++)
		  if (scmin > p_buffer[p_buffer[nn].path[j]].score)
		scmin = p_buffer[p_buffer[nn].path[j]].score;
		for (j = 0 ; p_buffer[nn].path[j] != -1 ; j++)
		  if (p_buffer[p_buffer[nn].path[j]].score
		  > scmin + cost_omomi.cost_haba) {
		  for (k = j ; p_buffer[nn].path[k] != -1 ; k++)
			p_buffer[nn].path[k] = p_buffer[nn].path[k+1];
		  j--;
		  }
		p_buffer[nn].score = scmin+sc;
	}
	}
	return TRUE;
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <is_through>
------------------------------------------------------------------------------
*/
	
int 	is_through(MRPH *mrph_p)
{
	if ((mrph_p->hinsi == kakko_hinsi &&
	 (mrph_p->bunrui == kakko_bunrui1 ||	
	  mrph_p->bunrui == kakko_bunrui2))||
	(mrph_p->hinsi == kuuhaku_hinsi &&	
	 mrph_p->bunrui == kuuhaku_bunrui))
	return TRUE;
	else	
	return FALSE;
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <my_fprintf> (MS Windows �̂��� �ɏo�͂� SJIS �ɂ���)
							 >>> Added by Taku Kudoh <<<
------------------------------------------------------------------------------
*/
#ifdef _WIN32
void my_fprintf(FILE* output, const char *fmt, ...)
{
	va_list  ap;
	char	 buf[1024];
	char	 *sjisstr;

	va_start(ap,fmt);
	art_vsprintf(buf,1024,fmt,ap);
	va_end(ap);

	/* SJIS �ɕύX���ďo�� */
	sjisstr = toStringSJIS(buf);
	fwrite(sjisstr,sizeof(char),strlen(sjisstr),output);
	free(sjisstr);
}
#endif

char **OutputAV;
int OutputAVnum;
int OutputAVmax;

MRPH *prepare_path_mrph(int path_num , int para_flag)
{
	MRPH		*mrph_p;
	int 		j;

	mrph_p = &(m_buffer[p_buffer[path_num].mrph_p]);

	if (para_flag != 0 && is_through(mrph_p) == 1 /*TRUE*/) return NULL;
	
	if (para_flag)
	art_strcpy((char*)kigou, MIDASI_MAX, "@ ");
	else
	kigou[0] = '\0';
	art_strcpy((char*)midasi1, MIDASI_MAX, (const char*)mrph_p->midasi);
	art_strcpy((char*)midasi2, MIDASI_MAX, (const char*)mrph_p->midasi);
	art_strcpy((char*)yomi, MIDASI_MAX, (const char*)mrph_p->yomi);
	if ( (mrph_p->katuyou1 > 0) && (mrph_p->katuyou2 > 0) ) {
	art_strcat((char*)midasi1, MIDASI_MAX, (const char*)Form[mrph_p->katuyou1][mrph_p->katuyou2].gobi);
	for(j = 1; strcmp((const char*)Form[mrph_p->katuyou1][j].name, (const char*)BASIC_FORM); j++);
	art_strcat((char*)midasi2, MIDASI_MAX, (const char*)Form[mrph_p->katuyou1][j].gobi);
	art_strcat((char*)yomi, MIDASI_MAX, (const char*)Form[mrph_p->katuyou1][mrph_p->katuyou2].gobi_yomi);
	}
	return mrph_p;
}

char *get_path_mrph(int path_num , int para_flag)
{
	int len = 0;
	MRPH *mrph_p;
	char *ret = NULL;

	if ((mrph_p = prepare_path_mrph(path_num, para_flag)) == NULL) return NULL;
	len = (int)(strlen((const char*)kigou)+strlen((const char*)midasi1)+strlen((const char*)yomi)+strlen((const char*)midasi2)+strlen((const char*)Class[mrph_p->hinsi][0].id)+mrph_p->hinsi/10+1);

	if ( mrph_p->bunrui ) {
	len += (int)strlen((const char*)Class[mrph_p->hinsi][mrph_p->bunrui].id);
	}
	else {
	len += 1;
	}

	len += mrph_p->bunrui/10+1;
	
	if ( mrph_p->katuyou1 ) {	
	len += (int)strlen((const char*)Type[mrph_p->katuyou1].name);
	}
	else {
	len += 1;
	}

	len += mrph_p->katuyou1/10+1;
	
	if ( mrph_p->katuyou2 ) {
	len += (int)strlen((const char*)Form[mrph_p->katuyou1][mrph_p->katuyou2].name);
	}
	else {
	len += 1;
	}

	len += mrph_p->katuyou2/10+1;

	len += 12;	/* ���� 10, ���s 1, �I�[ 1 */

	switch (Show_Opt2) {
	case Op_E2:
	len += (int)strlen((const char*)mrph_p->imis) + 1;
	ret = (char *)malloc(len);
	art_sprintf_13(ret, len, "%s%s %s %s %s %d %s %d %s %d %s %d %s\n", kigou, midasi1, yomi, midasi2,	
		Class[mrph_p->hinsi][0].id, mrph_p->hinsi,	
		mrph_p->bunrui ? Class[mrph_p->hinsi][mrph_p->bunrui].id : (U_CHAR *)"*", mrph_p->bunrui,	
		mrph_p->katuyou1 ? Type[mrph_p->katuyou1].name : (U_CHAR *)"*", mrph_p->katuyou1,	
		mrph_p->katuyou2 ? Form[mrph_p->katuyou1][mrph_p->katuyou2].name : (U_CHAR *)"*", mrph_p->katuyou2, 
		mrph_p->imis);
	break;
	case Op_E:
	ret = (char *)malloc(len);
	art_sprintf_12(ret, len, "%s%s %s %s %s %d %s %d %s %d %s %d\n", kigou, midasi1, yomi, midasi2, 
		Class[mrph_p->hinsi][0].id, mrph_p->hinsi,	
		mrph_p->bunrui ? Class[mrph_p->hinsi][mrph_p->bunrui].id : (U_CHAR *)"*", mrph_p->bunrui,	
		mrph_p->katuyou1 ? Type[mrph_p->katuyou1].name : (U_CHAR *)"*", mrph_p->katuyou1,	
		mrph_p->katuyou2 ? Form[mrph_p->katuyou1][mrph_p->katuyou2].name : (U_CHAR *)"*", mrph_p->katuyou2);
	break;
	}
	return ret;
}

int get_best_path_num()
{
	int j, last;

	j = 0;
	last = p_buffer_num-1;
	do {
	last = p_buffer[last].path[0];
	path_buffer[j] = last;
	j++;
	} while ( p_buffer[last].path[0] );

	return j;
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <print_path_mrph> �`�ԑf�̕\�� 	 >>> changed by yamaji <<<
------------------------------------------------------------------------------
*/
/*
  �T�[�o�[���[�h�Ή��̂��߁A����output�𑝂₵�ďo�͐�̕ύX���\�ɂ���B
  NACSIS �g��
*/
/* para_flag != 0 �Ȃ� @�o�� */
void print_path_mrph(FILE* output, int path_num , int para_flag)
{
	PROCESS_BUFFER	*proc_p;
	MRPH		*mrph_p;
	int 	pos;
	int 	len;

	if ((mrph_p = prepare_path_mrph(path_num, para_flag)) == NULL) return;
	proc_p = &(p_buffer[path_num]);
	pos = proc_p->start;

	fputs((const char*)kigou, output);

	switch (Show_Opt2) {
	case Op_F:	
	len = (int)strlen((const char*)yomi); yomi[len] = ')'; yomi[len+1] = '\0';
	my_fprintf(output, "%-12.12s(%-12.12s%-10.10s %-14.14s",
		midasi1, yomi, midasi2,
		Class[mrph_p->hinsi][mrph_p->bunrui].id);
	if (mrph_p->katuyou1)
		my_fprintf(output, " %-14.14s %-12.12s",
			Type[mrph_p->katuyou1].name,
			Form[mrph_p->katuyou1][mrph_p->katuyou2].name);
	fputc('\n', output);
	break;
	
	case Op_C:
	my_fprintf(output, "%s %s %s %d %d %d %d\n", midasi1, yomi, midasi2,
		mrph_p->hinsi, mrph_p->bunrui,
		mrph_p->katuyou1, mrph_p->katuyou2);
	break;

	case Op_EE:
	fprintf(output, "%d ", pos);
	if (!strcmp((const char*)midasi1, "\\ ")) pos++; else pos += (int)strlen((const char*)midasi1);
	fprintf(output, "%d ", pos);
	/* -E �I�v�V�����͂��̕��������ǉ�,
	   ���̌�� -e -e2 �I�v�V�����Ɠ��l�̏o�͂�����̂� break ���Ȃ� */

	case Op_E:
	case Op_E2:
	my_fprintf(output, "%s %s %s ", midasi1, yomi, midasi2);
	
	my_fprintf(output, "%s ", Class[mrph_p->hinsi][0].id);
	fprintf(output, "%d ", mrph_p->hinsi);
	
	if ( mrph_p->bunrui )	
	  my_fprintf(output, "%s ", Class[mrph_p->hinsi][mrph_p->bunrui].id);
	else
	  my_fprintf(output, "* ");
	fprintf(output, "%d ", mrph_p->bunrui);
	
	if ( mrph_p->katuyou1 ) 
	  my_fprintf(output, "%s ", Type[mrph_p->katuyou1].name);
	else					
	  fprintf(output, "* ");
	fprintf(output, "%d ", mrph_p->katuyou1);
	
	if ( mrph_p->katuyou2 ) 
	  my_fprintf(output, "%s ", Form[mrph_p->katuyou1][mrph_p->katuyou2].name);
	else	
	  fprintf(output, "* ");
	fprintf(output, "%d", mrph_p->katuyou2);

	if (Show_Opt2 == Op_E) {
		fprintf(output, "\n");	/* -e �ł� imis �͏o�͂��Ȃ� */
		break;
	}

	/* for SRI
	   fprintf(stdout, "\n");
	   if (para_flag) fprintf(stdout , "@ ");
	   */

	my_fprintf(output, " %s\n", mrph_p->imis);
	break;
	}
}

void process_path_mrph(FILE* output, int path_num , int para_flag) {
	if (output) {
	print_path_mrph(output, path_num, para_flag);
	}
	else {
	char *p;
	if (OutputAVnum == 0) {
		OutputAVmax = 10;
		OutputAV = (char **)malloc(sizeof(char *)*OutputAVmax);
	}
	else if (OutputAVnum >= OutputAVmax-1) {
		OutputAV = (char **)realloc(OutputAV, sizeof(char *)*(OutputAVmax <<= 1));
	}
	p =  get_path_mrph(path_num, para_flag);
	if (p != NULL) {
		*(OutputAV+OutputAVnum++) = p;
		*(OutputAV+OutputAVnum) =  NULL;
	}
	}
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <print_best_path> ����Œ���v��PATH�𒲂ׂ�
------------------------------------------------------------------------------
*/
/*
  �T�[�o�[���[�h�Ή��̂��߁A����output�𑝂₵�ďo�͐�̕ύX���\�ɂ���B
  NACSIS �g��
*/
char **print_best_path(FILE* output)
{
	int i, j, last;

	j = 0;
	last = p_buffer_num-1;
	do {
	last = p_buffer[last].path[0];
	path_buffer[j] = last;
	j++;
	} while ( p_buffer[last].path[0] );

	/* ���ʂ� buffer �ɓ����ꍇ */
	if (!output) {
	OutputAVnum = 0;
	OutputAVmax = 0;
	}

	for ( i=j-1; i>=0; i-- ) {
	process_path_mrph(output, path_buffer[i] , 0);
	}
	return OutputAV;	/* ��ł����� free ���邱�� */
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <print_all_mrph> ��������͌��ʂɊ܂܂��S�Ă̌`�ԑf
------------------------------------------------------------------------------
*/
/*
  �T�[�o�[���[�h�Ή��̂��߁A����output�𑝂₵�ďo�͐�̕ύX���\�ɂ���B
  NACSIS �g��
*/
char **print_all_mrph(FILE* output)
{
	int  i;

	for (i = 0 ; i < m_buffer_num ; i++)
	m_check_buffer[i] = 0;
	
	_print_all_mrph(output, p_buffer_num-1);
	m_check_buffer[0] = 0;

	/* ���ʂ� buffer �ɓ����ꍇ */
	if (!output) {
	OutputAVnum = 0;
	OutputAVmax = 0;
	}

	for (i = 0 ; i < m_buffer_num ; i++)
	if (m_check_buffer[i])
		process_path_mrph(output, i , 0);

	return OutputAV;	/* ��ł����� free ���邱�� */
}

void _print_all_mrph(FILE* output, int path_num)
{
	int i;
	
	for (i = 0 ; p_buffer[path_num].path[i] != -1 ; i++) {
	if (!m_check_buffer[p_buffer[path_num].path[i]]) {
		m_check_buffer[p_buffer[path_num].path[i]] = 1;
		_print_all_mrph(output, p_buffer[path_num].path[i]);
	}
	}
}
/*
------------------------------------------------------------------------------
  PROCEDURE: <print_all_path> ��������͌��ʂɊ܂܂��S�Ă�PATH
------------------------------------------------------------------------------
*/
/*
  �T�[�o�[���[�h�Ή��̂��߁A����output�𑝂₵�ďo�͐�̕ύX���\�ɂ���B
  NACSIS �g��
*/
char **print_all_path(FILE* output)
{
/*	int i,j;
	for (i = 0 ; i < p_buffer_num ; i++) {
	printf("%d %s %d %d --- " , i , m_buffer[p_buffer[i].mrph_p].midasi ,
		   p_buffer[i].start , p_buffer[i].end);
	for (j = 0 ; p_buffer[i].path[j] != -1 ; j++)
	  printf("%d ",p_buffer[i].path[j]);
	printf("\n");
	}*/

	/* ���ʂ� buffer �ɓ����ꍇ */
	if (!output) {
	OutputAVnum = 0;
	OutputAVmax = 0;
	}

	_print_all_path(output, p_buffer_num-1, 0);
	return OutputAV;	/* ��ł����� free ���邱�� */   
}

void _print_all_path(FILE* output, int path_num, int pathes)
{
	int i, j;
	
	for ( i=0; p_buffer[path_num].path[i] != -1; i++ ) {
	if ( p_buffer[path_num].path[0] == 0 ) {
		for ( j=pathes-1; j>=0; j-- )
		  process_path_mrph(output, path_buffer[j] , 0);
		if (output) fprintf(output, "EOP\n");
	} else {
		path_buffer[pathes] = p_buffer[path_num].path[i];
		_print_all_path(output, p_buffer[path_num].path[i], pathes+1);
	}
	}
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <print_homograph_path> �����̌����Ƃ��`�ԑf�͗񋓂���
------------------------------------------------------------------------------
*/
/*
  �T�[�o�[���[�h�Ή��̂��߁A����output�𑝂₵�ďo�͐�̕ύX���\�ɂ���B
  NACSIS �g��
*/
JUMAN_KNP_API char **print_homograph_path(FILE* output)
{
	path_buffer[0] = p_buffer_num-1;
	path_buffer[1] = -1;

	/* ���ʂ� buffer �ɓ����ꍇ */
	if (!output) {
	OutputAVnum = 0;
	OutputAVmax = 0;
	}

	_print_homograph_path(output, 0, 2);
	return OutputAV;	/* ��ł����� free ���邱�� */
}

int _print_homograph_path(FILE* output, int pbuf_start, int new_p)
{
	int i, j, k, l, now_pos, len, ll, pt, pt2, f;

	if (p_buffer[path_buffer[pbuf_start]].path[0] == 0) {
	/* �擪�܂Ń����N�����ǂ�I����� */
	for (j = new_p-2; j >= 1; j--) {
		/* �����ً`��Q����C�ɏo�� */
		for ( ; path_buffer[j] >= 0; j--) {}
		for (k = j+1, l = 0; path_buffer[k] >= 0; k++) {
		process_path_mrph(output, path_buffer[k] , l++);
		}
	}
	if (Show_Opt1 == Op_BB) return(1);
	if (output) fprintf(output, "EOP\n");
	return(0);
	}

	/* ����D��K��
		   3�����̕������ 2-1
	   4�����̕������ 2-2
	   5�����̕������ 3-2
	   �ƕ�������邱�Ƃ������̂ŁC���̕�����D��I�ɏo�͂���悤�ɂ���D
	   ���̂��߁C
	   �E2������̍���2,3,4�����ꂪ�A�ڂ��Ă���ꍇ��2�������D��
	   �E����ȊO�Ȃ�Ε������̏��Ȃ����D��
	   �Ƃ���D */
	f = 0;
	now_pos = p_buffer[path_buffer[pbuf_start]].start;
	for (j = pbuf_start; path_buffer[j] >= 0; j++)
	for (i = 0; (pt = p_buffer[path_buffer[j]].path[i]) != -1; i++)
		/* 2�������T�� */
		if (p_buffer[pt].start == now_pos-2*2) {
		for (k = 0; (pt2 = p_buffer[pt].path[k]) != -1; k++)
			/* 2������̍��ɘA�ڂ��Ă���ꂪ2�`4�����ꂩ�H */
			if (p_buffer[pt2].start <= now_pos-(2+2)*2 &&
			p_buffer[pt2].start >= now_pos-(2+4)*2) f = 1;
		}

	for (ll = 1; ll <= now_pos; ll++) { /* ���p�����̂��߁C1byte���Ƃɏ��� */
	len = ll;
	if (f) {
		/* 1�������2������̗D�揇�ʂ����ւ��� */
		if (ll == 2) { len = 4; } else if (ll == 4) { len = 2; }
	}

	/* ���������̌`�ԑf��S�ă��X�g�A�b�v���� */
	l = new_p;
	for (j = pbuf_start; path_buffer[j] >= 0; j++) {
		for (i = 0; (pt = p_buffer[path_buffer[j]].path[i]) != -1; i++) {
		if (p_buffer[pt].start == now_pos-len) {
			/* �����ً`��Q�����߂�(�A���d�����Ȃ��悤�ɂ���) */
			for (k = new_p; k < l; k++)
			if (path_buffer[k] == pt) break;
			if (k == l) path_buffer[l++] = pt;
		}
		}
	}
	path_buffer[l] = -1; /* �����ً`��Q�̃G���h�}�[�N */
	if (l != new_p)
		if (_print_homograph_path(output, new_p, l+1)) return(1);
	}
	return(0);
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <pos_match_process> <pos_right_process>
------------------------------------------------------------------------------
*/
int pos_match_process(int pos, int p_start)
{
	int i, j;

	/* �ʒu���}�b�`���� p_buffer �𒊏o���āCmatch_pbuf �ɏ������ׂ� */
	j = 0;
	for (i = p_start; i < p_buffer_num; i++) {
	if (p_buffer[i].end <= pos || p_buffer[i].connect == FALSE) {
		if (i == p_start)
		  /* p_start �ȑO�� p_buffer �͏\�� pos ���������̂ŁC�T�����ȗ� */
		  p_start++;
		if (p_buffer[i].end == pos && p_buffer[i].connect == 1 /*TRUE*/)
		  match_pbuf[j++] = i;
	}
	}
	match_pbuf[j] = -1;

	return p_start;
}

int pos_right_process(int pos)
{
	int i;
	
	for ( i=0; i<p_buffer_num; i++ )
	  if ( p_buffer[i].end > pos )
	return 1;
	
	return 0;
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <check_connect>			Changed by yamaji
------------------------------------------------------------------------------
*/

int check_connect(int pos, int m_num)
{
	static CHK_CONNECT_WK chk_connect[MAX_PATHES_WK];
	int chk_con_num;
	int i, j, pathes;
	int score, best_score, haba_score, best_score_num;
	int c_score, class_score;
	MRPH *new_mrph;
	MRPH *pre_mrph;
	int left_con, right_con;
	CONNECT_COST *c_cache;

	new_mrph = &m_buffer[m_num];
	best_score = INT_MAX;  /* maximam value of int */
	chk_con_num = 0;
	score = class_score = best_score_num = 0;
	class_score = Class[new_mrph->hinsi][new_mrph->bunrui].cost 
	* new_mrph->weight * cost_omomi.keitaiso;

	/* �A�ڃL���b�V���Ƀq�b�g����΁C�T�����s��Ȃ� */
	c_cache = &connect_cache[rensetu_tbl[new_mrph->con_tbl].j_pos];
	if (Show_Opt_debug == 0) {
	if (c_cache->pos == pos && c_cache->p_no > 0) {
		p_buffer[p_buffer_num].score = c_cache->cost + class_score;
		p_buffer[p_buffer_num].mrph_p = m_num;
		p_buffer[p_buffer_num].start = pos;
		p_buffer[p_buffer_num].end = pos + new_mrph->length;
		for (i = 0; (p_buffer[p_buffer_num].path[i] = p_buffer[c_cache->p_no].path[i]) >= 0; i++) {}
		p_buffer[p_buffer_num].path[i] = -1;
		p_buffer[p_buffer_num].connect = TRUE;
		if (++p_buffer_num == process_buffer_max)
		realloc_process_buffer();
		return TRUE;
	}
	}

	for (i = 0; (j = match_pbuf[i]) >= 0; i++) {

	/* �ȑO�̌`�ԑf��con_tbl�����o�� */
	left_con = m_buffer[p_buffer[j].mrph_p].con_tbl;
	/* �V�����`�ԑf��con_tbl�����o�� */
	right_con = new_mrph->con_tbl;

	c_score = check_matrix(left_con , right_con);

	if (c_score) {
		chk_connect[chk_con_num].pre_p = j;

		/* calculate the score */
		score = p_buffer[j].score + c_score * cost_omomi.rensetsu;

		chk_connect[chk_con_num].score = score;
		if (score < best_score) {
		best_score = score;
		best_score_num = chk_con_num;
		}
		chk_con_num++;
	}

	/* �f�o�O�p�R�X�g�\�� */

	if (Show_Opt_debug == 2 || (Show_Opt_debug == 1 && c_score)) {

		fprintf(stderr, "%3d " , pos);

		pre_mrph = &m_buffer[p_buffer[j].mrph_p];
		fprintf(stderr, "%s" , pre_mrph->midasi);
		if (Class[pre_mrph->hinsi][pre_mrph->bunrui].kt)
		fprintf(stderr, "%s", Form[pre_mrph->katuyou1][pre_mrph->katuyou2].gobi);
		if (Class[pre_mrph->hinsi][0].id) {
		fprintf(stderr, "(%s" , Class[pre_mrph->hinsi][0].id);
		if (pre_mrph->bunrui)
			fprintf(stderr, "-%s", Class[pre_mrph->hinsi][pre_mrph->bunrui].id);
		if (pre_mrph->katuyou1)
			fprintf(stderr, "-%s", Type[pre_mrph->katuyou1].name);
		if (pre_mrph->katuyou2)
			fprintf(stderr, "-%s", Form[pre_mrph->katuyou1][pre_mrph->katuyou2].name);
		fprintf(stderr, ")");
		}
		fprintf(stderr, "[= %d]", p_buffer[j].score);

		if (c_score) {
		fprintf(stderr, "--[+%d*%d]--", c_score, cost_omomi.rensetsu);
		} else {
		fprintf(stderr, "--XXX--");
		}

		fprintf(stderr, "%s" , new_mrph->midasi);
		if (Class[new_mrph->hinsi][new_mrph->bunrui].kt)
		fprintf(stderr, "%s", Form[new_mrph->katuyou1][new_mrph->katuyou2].gobi);
		if (Class[new_mrph->hinsi][0].id) {
		fprintf(stderr, "(%s" , Class[new_mrph->hinsi][0].id);
		if (new_mrph->bunrui)
			fprintf(stderr, "-%s",Class[new_mrph->hinsi][new_mrph->bunrui].id);
		if (new_mrph->katuyou1)
			fprintf(stderr, "-%s", Type[new_mrph->katuyou1].name);
		if (new_mrph->katuyou2)
			fprintf(stderr, "-%s", Form[new_mrph->katuyou1][new_mrph->katuyou2].name);
		fprintf(stderr, ")");
		}

		if (c_score == 0) {
		fprintf(stderr, "\n");
		} else {
		fprintf(stderr, "[+%d*%d.%d*%d = %d]\n",	
			Class[new_mrph->hinsi][new_mrph->bunrui].cost,
			new_mrph->weight/10, new_mrph->weight%10,	
			cost_omomi.keitaiso*10, 
			/* = class_score */
			p_buffer[j].score + c_score * cost_omomi.rensetsu + class_score);
		}
	}
	}

	/* return immidiately, because if best_score is
	   INT_MAX then no path exists. */
	if (best_score == INT_MAX) return TRUE;

	/* ����̘A�ڂ̗l�q��A�ڃL���b�V���ɓ���� */
	c_cache->p_no = (short)p_buffer_num;
	c_cache->cost = best_score;
	c_cache->pos = (short)pos;

	/* ��芸�����x�X�g�p�X��1��0�Ԃɓo�^ */
	p_buffer[p_buffer_num].path[0] = chk_connect[best_score_num].pre_p;
	pathes = 1;
	haba_score = best_score + cost_omomi.cost_haba;
	for (j = 0; j < chk_con_num; j++) /* ����ȊO�̃p�X���ǉ� */
	  if (chk_connect[j].score <= haba_score && j != best_score_num)
	p_buffer[p_buffer_num].path[pathes++] = chk_connect[j].pre_p;
	p_buffer[p_buffer_num].path[pathes] = -1;

	p_buffer[p_buffer_num].score = best_score+class_score;
	p_buffer[p_buffer_num].mrph_p = m_num;
	p_buffer[p_buffer_num].start = pos;
	p_buffer[p_buffer_num].end = pos + new_mrph->length;
	p_buffer[p_buffer_num].connect = TRUE;
	if (++p_buffer_num == process_buffer_max)
	realloc_process_buffer();
	return TRUE;
}

/*
------------------------------------------------------------------------------
  PROCEDURE: <juman_sent> �ꕶ���`�ԑf��͂���	  by T.Utsuro
------------------------------------------------------------------------------
*/
JUMAN_KNP_API int juman_sent(void)
{
	int 	   i;
	int 	   length;
	int 	   pre_m_buffer_num;
	int 	   pre_p_buffer_num;
	int 	   pos, next_pos = 0;
	int 	   p_start = 0;

	init_str();

	if (mrph_buffer_max == 0) {
	m_buffer = (MRPH *)my_alloc(sizeof(MRPH)*BUFFER_BLOCK_SIZE);
	m_check_buffer = (int *)my_alloc(sizeof(int)*BUFFER_BLOCK_SIZE);
	mrph_buffer_max += BUFFER_BLOCK_SIZE;
	}
	if (process_buffer_max == 0) {
	p_buffer = (PROCESS_BUFFER *)
		my_alloc(sizeof(PROCESS_BUFFER)*BUFFER_BLOCK_SIZE);
	path_buffer = (int *)my_alloc(sizeof(int)*BUFFER_BLOCK_SIZE);
	match_pbuf = (int *)my_alloc(sizeof(int)*BUFFER_BLOCK_SIZE);
	process_buffer_max += BUFFER_BLOCK_SIZE;
	}

	length = (int)strlen((const char*)StringBuff);

	if (length == 0) return FALSE;	/* ��s�̓X�L�b�v */

	for (i = 0; i < CONNECT_MATRIX_MAX; i++) connect_cache[i].p_no = 0;

	/* �������� */
	p_buffer[0].end = 0;		
	p_buffer[0].path[0] = -1;
	p_buffer[0].score = 0;	
	p_buffer[0].mrph_p = 0;
	p_buffer[0].connect = TRUE;
	m_buffer[0].hinsi = 0;
	m_buffer[0].bunrui = 0;
	m_buffer[0].con_tbl = 0;
	m_buffer[0].weight = MRPH_DEFAULT_WEIGHT;
	art_strcpy((char*)m_buffer[0].midasi , MIDASI_MAX, buntou);
	m_buffer_num = 1;
	p_buffer_num = 1;
	
	for (pos = 0; pos < length; pos+=next_pos) {

	p_start = pos_match_process(pos, p_start);
	if (match_pbuf[0] >= 0) {
	
		pre_m_buffer_num = m_buffer_num;
		pre_p_buffer_num = p_buffer_num;
	
		if (StringBuff[pos]&0x80) { /* �S�p�̏ꍇ�C�������Ђ� */
		if (search_all(pos) == FALSE) return FALSE;
		next_pos = 2;
		} else {
		next_pos = 1;
		}

		if (undef_word(pos) == FALSE) return FALSE;
	}
	}
	
	/* �������� */
	art_strcpy((char*)m_buffer[m_buffer_num].midasi , MIDASI_MAX, bunmatsu);
	m_buffer[m_buffer_num].hinsi = 0;
	m_buffer[m_buffer_num].bunrui = 0;
	m_buffer[m_buffer_num].con_tbl = 0;
	m_buffer[m_buffer_num].weight = MRPH_DEFAULT_WEIGHT;
	if (++m_buffer_num == mrph_buffer_max)
	realloc_mrph_buffer();

	pos_match_process(pos, p_start);
	if (check_connect(length, m_buffer_num-1) == FALSE)
	return FALSE;

	return TRUE;
}
