//#define PAT_DEBUG

#ifdef PAT_DEBUG
#define OI(x) {printf("< %d >\n",x);fflush(stdout);} /* Output Integer */ /*�f�o�O�ɕ֗�*/
#define OS(x) {printf("[ %s ]\n",x);fflush(stdout);} /* Output String */
#define OS2(x) {printf("%s\n",x);fflush(stdout);} /* Output String */
#define OM(x) {printf("Message: " #x "\n");fflush(stdout);} /* Output Message */
#define OL(x) {printf(#x);fflush(stdout);} /* Output Label */
#else
#define OI(x)  /* */
#define OS(x)  /* */
#define OS2(x) /* */
#define OM(x)  /* */
#define OL(x)  /* */
#endif

/* �����n�b�V�� */
#ifdef USE_HASH
#include "t-hash.h"
#endif

/* �C���f�b�N�X�p�̃��X�g�^�̒�` */
typedef struct pat_index_list {
  struct pat_index_list *next; /* �� */
  long index; /* �t�@�C���̃C���f�b�N�X */
} pat_index_list;

/* �m�[�h�̃f�[�^�\���̒�` */
typedef struct pat_node {
  pat_index_list il; /* �C���f�b�N�X�̃��X�g */
  short checkbit; /* �`�F�b�N����r�b�g�̎w��B(���Ԗڂ̃r�b�g?) */
#if 0
  char *str; /* �g���C�Ƃ��Ă������߂ɍŒ���K�v�ȕ������ۑ� 960919 */
#endif
  struct pat_node *right; /* �E�ւ܂���܁`�� */
  struct pat_node *left; /* ���ւ܂���܁`�� */
} pat_node;


#define HASH_SIZE 131071 /* 107653	�n�b�V���e�[�u���̃T�C�Y 1162213*/


#ifndef MAX_DIC_NUMBER 
#define MAX_DIC_NUMBER 5 /* �����Ɏg���鎫���̐��̏�� (JUMAN) */
#endif
extern pat_node tree_top[MAX_DIC_NUMBER]; /* �؂̂˂����� �����̐������g�� */
extern FILE *dic_file[MAX_DIC_NUMBER]; /* �؂̂��ƃf�[�^(�����t�@�C��) */

#define SIKII_BIT 16  /* �����ƕ����̋�؂�͉��r�b�g��? (8 or 16) */

extern char line[50000]; /* ���͍s */
extern FILE *out_file, *in_file; /* �Z�[�u�t�@�C���E���[�h�t�@�C�� */
extern char  inkey[10000]; /* �����E�}���L�[ */

/*** JUMAN���������֘A ***/
extern int number_of_tree; /* �g�p���鎫��(�p�g��)�̐� */

/*** get_item()�p ***/
extern char partition_char; /* ��؂蕶�� */
extern int column; /* ���R�����ڂ� */

/**************************
 * �֐��̃v���g�^�C�v�錾 *
 **************************/ 
/* pat.c */
extern void pat_init_tree_top(pat_node*); /* �p�g���V�A�؂̍��̏����� */
extern pat_node *pat_search(FILE*,char*,pat_node*,char*); /* �p�g���V�A�؂Ō��� */
extern pat_node *pat_search4insert(char*,pat_node*); /* �}���p���� */
extern void pat_insert(FILE*,char*,long,pat_node*,char*); /* �p�g���V�A�؂ɑ}�� */
extern int pat_bits(char*,int,int); 
		 /* �����񒆂̎w�肳�ꂽ�ʒu�̃r�b�g��Ԃ� */
		 /* 960918	������strlrn()�����͖̂��ʂł��邱�Ƃ����� */
extern void show_pat(pat_node*,FILE*,char*); /* �p�g���V�A�؃f�[�^���o�� */
extern char *get_line(FILE*,long); /* �w�肳�ꂽ�ꏊ����'\n'�܂œǂ� */

/* file.c */
extern void com_s(char*,pat_node*); /* �Z�[�u�֘A */
extern void save_pat(pat_node*);
extern void eputc(unsigned char, int);
extern void com_l(char*,pat_node*); /* ���[�h�֘A */
extern pat_node *load_anode(pat_node*);
extern unsigned char egetc(int);
extern void dummy(void);
extern pat_node *malloc_pat_node(void); /* Matomete malloc */
extern pat_index_list *malloc_pat_index_list(void); /* Matomete malloc */

/* morph.c */
extern void jisyohiki(char*,pat_node*); /* �������� */
extern void insert_dic_data(FILE*,pat_node*,char*); /* �����f�[�^��}��*/

/************************************************************************
* 
* pat --- �p�g���V�A�؂̒T���Ƒ}��
* 
* ���: ����(tatuo-y@is.aist-nara.ac.jp)
* 
* �ړI: �p�g���V�A�؂̒T���Ƒ}�����s��
* 
* �Q�l����: 
*	�A���S���Y���̗����̂��߂ɕ���[1]���Q�Ƃ����BC����ł̎�����
*	����[2]�̃v���O�������Q�l�ɂ����B
* [1] R. Sedgewick �� �쉺�_���A����A�����n�A�c���� ����
*	  �A���S���Y�� (Algorithms) ������2�� ��2�� �T���E������E�v�Z��
*	  �ߑ�Ȋw��,1992. (B195-2,pp.68-72)
* [2] ��������A�L�V���A�쉺�_���A�l�c��ρA�������� �ҏW�ψ�
*	  �A���S���Y�����T
*	  �����o�Ŋ������,1994. (D74,pp.624-625)
* 
* ����:
*	1996/04/09	����! (������������f�[�^�̍ő咷��8bit�B[2]��͕�B)
*			10	�o�̓��[�`�����ċA�ɉ��ǁB������f�[�^�Ή�(�ő咷������)�B
*			30	�Z�[�u/���[�h�@�\�B�m�[�h�̃f�[�^�\����ID�ԍ���ǉ�(��)�B
*		  5/06	�����؂̑S�f�[�^�o�͏����B
*		  6/11	JUMAN�̎��������p�ɉ����D
*			21	�A�z�z��𓱓�(INDEX���L���b�V������)
*		  7/01	�����̎����t�@�C��(�p�g��)���猟���ł���悤�ɂ����D
* 
* ����: JUMAN�̎��������ɗ��p����
* 
************************************************************************/
