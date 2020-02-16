#include "stdafx.h"

//#ifdef HAVE_CONFIG_H
#include <juman/config.h>
//#endif

#ifdef	_WINDOWS
#include <io.h>
#endif
#include <stdio.h>

#ifdef HAVE_FCNTL_H
#include  <fcntl.h>
#endif

#ifdef HAVE_STDLIB_H
#include  <stdlib.h>
#endif

#include "juman/juman_pat.h"
#ifndef _WIN32
#ifndef O_BINARY
#define O_BINARY (0)
#endif
#endif

int fd_pat; /* �p�g�؂̃Z�[�u/���[�h�p�t�@�C���f�B�X�N���v�^ */

/******************************************************
* com_l --- �؂̃��[�h
*  by �đ�b�i(keiji-y@is.aist-nara.ac.jp)
*
* �p�����[�^�A�Ԃ��l
*	�Ȃ�
******************************************************/
void com_l(char *fname_pat, pat_node *ptr){
/*	fprintf(stderr, "# Loading pat-tree \"%s\" ... ",fname_pat); */
  if ((fd_pat = art_open(fname_pat, O_RDONLY|O_BINARY)) == -1) {
	fprintf(stderr, "�t�@�C�� %s ���I�[�v���o���܂���B\n",fname_pat);
	exit(1);
  }
  OL(fd_pat)OI(fd_pat)
  ptr->right = load_anode(ptr);
  art_close(fd_pat);
/*	fprintf(stderr,"done.\n"); */
}


/****************************************************
* load_anode --- �p�g���V�A�؂����[�h
*  by �đ�b�i(keiji-y@is.aist-nara.ac.jp)
*
* �p�����[�^
*	in --- ���͐�t�@�C��
*	p_ptr --- ���̃m�[�h���O���ړ_�ł��������ɃC���f�b�N�X���i�[����ꏊ
*			  �����ړ_�ł������Ƃ��́A���̃|�C���^�͉E�̎q�ɓn�����B
*
* �A���S���Y��
*	�`�F�b�N�r�b�g��ǂݍ��񂾂�A����͓����ړ_������V�����m�[�h�����
*	  �������؁A�E�����؂̏��ɍċA����
*	  ���ċA�̎��͐V������������̐ړ_�̃|�C���^���A
*	  �E�ċA�̎��� p_ptr ���C���f�b�N�X�̊i�[�ꏊ�Ƃ��ēn���B
*	�C���f�b�N�X��ǂݍ��񂾂�A����͊O���ړ_������Ap_ptr->index �Ɋi�[
*
* ����
*	�C���f�b�N�X�̊i�[�ꏊ�����ƈႤ���A���ɖ��Ȃ��B
*************************************************************************/
struct nlist {
	void *ptr;
	struct nlist *next;
};
static struct nlist *pat_node_list = NULL;

static int	pat_index_idx = 1024;
pat_index_list *malloc_pat_index_list()
{
	//static int  idx = 1024;
	static char *ptr;

	if (pat_index_idx == 1024) {
		ptr = (char*)malloc(sizeof(pat_index_list) * pat_index_idx);
		pat_index_idx = 0;

		struct nlist *n = (struct nlist *)malloc(sizeof(struct nlist));
		n->ptr = ptr;
		n->next = pat_node_list; pat_node_list = n;
	}

	return (pat_index_list *)(ptr + sizeof(pat_index_list) * pat_index_idx++);
}

static int	pat_node_idx = 1024;
pat_node *malloc_pat_node()
{
	//static int  idx = 1024;
	static char *ptr;

	if (pat_node_idx == 1024) {
		ptr = (char*)malloc(sizeof(pat_node) * pat_node_idx);
		pat_node_idx = 0;

		struct nlist *n = (struct nlist *)malloc(sizeof(struct nlist));
		n->ptr = ptr;
		n->next = pat_node_list; pat_node_list = n;
	}

	return (pat_node *)(ptr + sizeof(pat_node) * pat_node_idx++);
}

static int fd_pat_check = -1;
static int ctr = BUFSIZ - 1;

void free_pat_node() {
	while (pat_node_list) {
		struct nlist *n = pat_node_list->next;
		free(pat_node_list->ptr);
		free(pat_node_list);
		pat_node_list = n;
	}
	pat_index_idx = 1024;
	pat_node_idx = 1024;
	fd_pat_check = -1;
	ctr = BUFSIZ - 1;
}

pat_node *load_anode(pat_node *p_ptr){
  unsigned char c;
  pat_node *new_ptr; /* �V����������m�[�h(==���̃m�[�h)���w���|�C���^ */
  long tmp_idx;
  pat_index_list *new_l_ptr,*t_ptr=NULL;

  if ((c = egetc(fd_pat)) & 0x80) { /* �t���ς̏����A�C���f�b�N�X�̓ǂݍ��� */
	while(c & 0x80) {
	  tmp_idx = (c & 0x3f) << 24;
	  tmp_idx |= egetc(fd_pat) << 16;
	  tmp_idx |= egetc(fd_pat) << 8;
	  tmp_idx |= egetc(fd_pat);

	  if((p_ptr->il).index < 0)
	new_l_ptr = &(p_ptr->il);
	  else {
	new_l_ptr = malloc_pat_index_list();
	t_ptr->next = new_l_ptr;
	  }
	  new_l_ptr->index = tmp_idx;
	  new_l_ptr->next = NULL;
	  t_ptr = new_l_ptr;

	  if(c & 0x40) break;
	  c = egetc(fd_pat);
	}

	return (p_ptr);
  }
  else { /* �����ړ_�̏����A�ċA���� */
	new_ptr = malloc_pat_node();
	new_ptr->checkbit = ((c << 8) | egetc(fd_pat)) - 1; /* �`�F�b�N�r�b�g */
/*	  printf("#cb %d\n",new_ptr->checkbit);*/
	(new_ptr->il).index = -1;
	new_ptr->left = load_anode(new_ptr);
	new_ptr->right = load_anode(p_ptr);
	return (new_ptr);
  }
}

unsigned char egetc(int file_discripter){
  //static int fd_pat_check = -1;
  static char buf[BUFSIZ];
  //static int ctr = sizeof(buf) - 1;

  if(file_discripter != fd_pat_check) { /* �o�b�t�@�̏����� */
	fd_pat_check = file_discripter;
	ctr = sizeof(buf) - 1;
  }

  if(++ctr == sizeof(buf)){
	ctr = 0;
	art_read(file_discripter, buf, sizeof(buf));
/* OL(.);fflush(stdout);*/
  }

  return(buf[ctr]);
}

/*****************************************************
* com_s --- �؂̃Z�[�u 
*  by �đ�b�i(keiji-y@is.aist-nara.ac.jp)
*
* �p�����[�^�A�Ԃ��l
*	�Ȃ�
*****************************************************/
void com_s(char *fname_pat, pat_node *ptr){
  int i;

  printf("Saving pat-tree \"%s\" ...\n",fname_pat);
  if ((fd_pat = art_open2(fname_pat, O_WRONLY|O_CREAT|O_BINARY, 0644)) == -1) {
	fprintf(stderr, "�t�@�C�� %s ���I�[�v���o���܂���B\n", fname_pat);
	exit(1);
  }; 
  save_pat(ptr->right); /* �t�@�C���o�� */
  for(i = 0; i < BUFSIZ; i++)
	eputc(0, fd_pat); /* flush */
  art_close(fd_pat);
}


/****************************************************
* save_pat --- �p�g���V�A�؃f�[�^���Z�[�u 
*  by �đ�b�i(keiji-y@is.aist-nara.ac.jp)
*
* �p�����[�^
*	top_ptr --- �����J�n�m�[�h�̈ʒu(�|�C���^)
*	out_to --- �o�͐�(stdout��t�@�C��)
* 
* �Ԃ��l
*	�����B�p�g���V�A�؃f�[�^���o�́B
*
* �o�̓t�H�[�}�b�g --- 8�r�b�g�ɋ�؂��ăo�C�i���o��
*	���D��T���œ����ړ_�̓`�F�b�N�r�b�g�A�O���ړ_�̓C���f�b�N�X���o��
*	�`�F�b�N�r�b�g --- ��{�I�ɂ��̂܂� (�� 0 �r�b�g�� 0)
*	  ������ -1 �̂Ƃ�����̂� 1 �𑫂�
*	�C���f�b�N�X --- �� 0 �r�b�g�� 1 �ɂ���
****************************************************/
void save_pat(pat_node *top_ptr)
{
  pat_index_list *ptr;
  /* �����ړ_�̏����A�`�F�b�N�r�b�g���o�� */
  eputc ((unsigned char)(((top_ptr->checkbit + 1)>> 8) & 0x7f), fd_pat);
  eputc ((unsigned char)((top_ptr->checkbit + 1)& 0xff), fd_pat);

  /* ���E�� Subtree �̏����B�t���ςȂ�C���f�b�N�X���o�́A
	 �t���ςłȂ���΍ċA�B*/
  if(top_ptr->checkbit < top_ptr->left->checkbit)
	save_pat(top_ptr->left);
  else {
	ptr = &(top_ptr->left->il);
	if(ptr->index < 0) dummy();
	else {
	  while(ptr != NULL) {
	if(ptr->next == NULL) eputc ((unsigned char)(((ptr->index >> 24) & 0x3f) | 0xc0), fd_pat);
	else eputc ((unsigned char)(((ptr->index >> 24) & 0x3f) | 0x80), fd_pat);
	eputc ((unsigned char)((ptr->index >> 16) & 0xff), fd_pat);
	eputc ((unsigned char)((ptr->index >> 8) & 0xff), fd_pat);
	eputc ((unsigned char)((ptr->index) & 0xff), fd_pat);
	ptr = ptr->next;
	  }
	}
  }
  if(top_ptr->checkbit < top_ptr->right->checkbit)
	save_pat(top_ptr->right);
  else {
	ptr = &(top_ptr->right->il);
	if(ptr->index < 0) dummy();
	else {
	  while(ptr != NULL) {
	if(ptr->next == NULL) eputc ((unsigned char)(((ptr->index >> 24) & 0x3f) | 0xc0), fd_pat);
	else eputc ((unsigned char)(((ptr->index >> 24) & 0x3f) | 0x80), fd_pat);
	eputc ((unsigned char)((ptr->index >> 16) & 0xff), fd_pat);
	eputc ((unsigned char)((ptr->index >> 8) & 0xff), fd_pat);
	eputc ((unsigned char)((ptr->index) & 0xff), fd_pat);
	ptr = ptr->next;
	  }
	}
  }

  return;
}

void dummy() {
  eputc(0xff,fd_pat);eputc(0xff,fd_pat);eputc(0xff,fd_pat);eputc(0xff,fd_pat);
}

void eputc(unsigned char c, int file_discripter){
  static int ctr = 0;
  static unsigned char buf[BUFSIZ];

  buf[ctr] = (char) c;
  ctr++;

  if(ctr == BUFSIZ){
	ctr = 0;
	art_write(file_discripter, buf, BUFSIZ);
  }

  return;
}
