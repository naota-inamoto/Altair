
#include "stdafx.h"

//#ifdef HAVE_CONFIG_H
#include <juman/config.h>
//#endif

#include <stdio.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

/*#define HAVE_MMAP*/
#ifdef HAVE_MMAP
#undef USE_HASH
#include <sys/mman.h>

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#endif

#if defined _WIN32 && ! defined __CYGWIN__
typedef char *	caddr_t;
#endif

#include "juman/juman_pat.h"

/* �n�b�V���e�[�u���̐錾 */
#ifdef USE_HASH
th_hash_node hash_array[HASH_SIZE];
#endif

pat_node tree_top[MAX_DIC_NUMBER]; /* �؂̂˂����� �����̐������g�� */
FILE *dic_file[MAX_DIC_NUMBER]; /* �؂̂��ƃf�[�^(�����t�@�C��) */

extern int hash_check_proc(FILE *f, long index, char *buf);

/******************************************************
* pat_strcmp_prefix --- �v���t�B�N�X�}�b�`
*
* �p�����[�^
*	s1 --- Prefix String
*	s2 --- ���ׂ��� String
*
* �Ԃ��l  ���� 1�A���s 0
******************************************************/
static int pat_strcmp_prefix(char *s1, char *s2)
{
  for (;;) {
	if (*s2 == '\t') return 1;
	if (*s1++ != *s2++) return 0;
  }
}

/******************************************************
* pat_strcpy --- ������R�s�[
*
* �p�����[�^
*	s1, s2
*
* �Ԃ��l
******************************************************/
static char *pat_strcpy(char *s1, char *s2)
{
	while ((*s1++ = *s2++) != 0);
	return s1 - 1;
}


/******************************************************
* pat_init_tree_top --- �p�g���V�A�؂̍��̏�����
*
* �p�����[�^
*	ptr --- ����������؂̍��ւ̃|�C���^
******************************************************/
void pat_init_tree_top(pat_node *ptr) {
  (ptr->il).index = -1; /* �C���f�b�N�X�̃��X�g */
  ptr->checkbit = -1;
  ptr->right = ptr;
  ptr->left = ptr;
}

/****************************************************
* pat_search --- �p�g���V�A�؂�����
* 
* �p�����[�^
*	key --- �����L�[
*	x_ptr --- �����J�n�ʒu(�|�C���^)
*	rslt --- ���ʂ�����D
* 
* �Ԃ��l
*	�����I���ʒu(�|�C���^)
*
****************************************************/
pat_node *pat_search(FILE *f, char *key, pat_node *x_ptr, char *rslt)
{
  pat_node *ptr,*tmp_ptr,*top_ptr = x_ptr,*tmp_x_ptr = x_ptr;
  int in_hash = 0;
  pat_index_list *tmp_l_ptr;
  int key_length = (int)strlen(key); /* �L�[�̕������𐔂��Ă��� */
  char buffer[50000]; /* �ėp�o�b�t�@ */
  int totyu_match_len = 0; /* �r���Ń}�b�`����Prefix�̕����� */
  char *r;

  rslt += strlen(rslt);
  r = rslt;

/*	rslt[0] = '\0';*/

  /* OL(pat_search:\n); */
  do {
	ptr = x_ptr;
	/* �~���r�b�g�Ȃ�� */
	OL(checkbit:)OI(ptr->checkbit);
	if(ptr->checkbit%SIKII_BIT==0 && ptr->checkbit!=0){ /* �r���P���T�� */
	  tmp_x_ptr = ptr;
	  do { /* �������؂̈�ԍ��̃m�[�h�𒲂ׂ�D */
	tmp_ptr = tmp_x_ptr;
	tmp_x_ptr = tmp_x_ptr->left;
	  } while(tmp_ptr->checkbit < tmp_x_ptr->checkbit);

	  /* �n�b�V�����`�F�b�N */
	  in_hash = hash_check_proc(f,(tmp_x_ptr->il).index,buffer);
	  art_strtok(buffer,"\t"); /* �ŏ��� '\t' �� '\0' �ɂ���D*/
	  /* buffer�̐擪�́u���o����v���������Ń}�b�`���O���s�Ȃ� */
	  if(strncmp(key,buffer,ptr->checkbit/8) == 0) { /* ������ */
	totyu_match_len = ptr->checkbit/8; /* �r���Ń}�b�`����Prefix�̕����� */
	tmp_l_ptr = &(tmp_x_ptr->il); /* �S���X�g�v�f�̎��o�� */
	while(tmp_l_ptr != NULL){
	  in_hash = hash_check_proc(f,tmp_l_ptr->index,buffer);
	  r = pat_strcpy(r, buffer);
	  *r++ = '\n';
	  *r = '\0';

	  tmp_l_ptr = tmp_l_ptr->next;
	}
	  } else { /* �r���Ŏ��s�𔭌� */
	return x_ptr;
	  }
	}

	/* key �� checkbit�r�b�g�ڂō��E�ɐU�蕪�� */
	if(pat_bits(key,x_ptr->checkbit,key_length)==1){x_ptr = x_ptr->right;}
	else {x_ptr = x_ptr->left;}

  } while(ptr->checkbit < x_ptr->checkbit);
	

  if(tmp_x_ptr != x_ptr || top_ptr == x_ptr) { /* �I���m�[�h���`�F�b�N���� */
	char *s;
	int tmp_len;
	/* �n�b�V�����`�F�b�N */
	in_hash = hash_check_proc(f,(x_ptr->il).index,buffer);

	s = strchr(buffer,'\t'); /* �ŏ��� '\t' �� '\0' �ɂ���D*/
	*s = '\0';
	tmp_len = (int)(s - buffer);/*�J�n */

	/* buffer�̐擪�́u���o����v���������Ń}�b�`���O���s�Ȃ� */
	if(strncmp(key,buffer,tmp_len) == 0){ /* �����ǂ܂�P���Prefix�`�F�b�N */
	  if(totyu_match_len != key_length){ /* �V�o��̒P�ꂩ�ۂ��̃`�F�b�N */
	tmp_l_ptr = &(x_ptr->il); /* �S���X�g�v�f�̎��o�� */
	while(tmp_l_ptr != NULL){
	  in_hash = hash_check_proc(f,tmp_l_ptr->index,buffer);
	  r = pat_strcpy(r, buffer);
	  *r++ = '\n';
	  *r = '\0';

	  tmp_l_ptr = tmp_l_ptr->next;
	}
	  }
	}
  }

  return x_ptr;
}

#if 0
/****************************************************
* pat_search_exact --- �p�g���V�A�؂�����(exact match)
* 
* �p�����[�^
*	key --- �����L�[
*	x_ptr --- �����J�n�ʒu(�|�C���^)
*	rslt --- ���ʂ�����D
* 
* �Ԃ��l
*	�����I���ʒu(�|�C���^)
****************************************************/
pat_node *pat_search_exact(FILE *f, char *key, pat_node *x_ptr, char *rslt)
{
  pat_node *ptr;
  pat_index_list *tmp_l_ptr;
  int in_hash;
  int key_length = (int)strlen(key); /* �L�[�̕������𐔂��Ă��� */
  char buffer[50000]; /* �ėp�o�b�t�@ */
  char *r;

  rslt += strlen(rslt);
  r = rslt;

  /*  printf("##");*/
  do {
	ptr = x_ptr;
	/* key �� checkbit�r�b�g�ڂō��E�ɐU�蕪�� */
	if(pat_bits(key,x_ptr->checkbit,key_length)==1){x_ptr = x_ptr->right;}
	else {x_ptr = x_ptr->left;}

  } while(ptr->checkbit < x_ptr->checkbit);

  /* �t�@�C���������ė��� */
  in_hash = hash_check_proc(f,(x_ptr->il).index,buffer);
  /*buffer = get_line(f,x_ptr->il_ptr->index);*/

  art_strtok(buffer,"\t"); /* �ŏ��� '\t' �� '\0' �ɂ���D*/

  /* buffer�̐擪�́u���o����v���������Ń}�b�`���O���s�Ȃ� */
  if(strcmp(key,buffer) == 0){ /* �����ǂ܂�P��̃`�F�b�N */
	tmp_l_ptr = &(x_ptr->il); /* �S���X�g�v�f�̎��o�� */
	while(tmp_l_ptr != NULL){
	  in_hash = hash_check_proc(f,tmp_l_ptr->index,buffer);
	  r = pat_strcpy(r, buffer);
	  *r++ = '\n';
	  *r = '\0';

	  tmp_l_ptr = tmp_l_ptr->next;
	}
  }

  return x_ptr;
}

/****************************************************
* pat_search4insert --- �}���p�Ɍ���
* 
* �p�����[�^
*	key --- �����L�[
*	x_ptr --- �����J�n�ʒu(�|�C���^)
* 
* �Ԃ��l
*	�����I���ʒu(�|�C���^)
*
* ����
*	���ϐ� prefix_str �̎w����Ƀv���t�B�b�N�X�����������B
****************************************************/
pat_node *pat_search4insert(char *key, pat_node *x_ptr)
{
  pat_node *ptr;
  int key_length = (int)strlen(key); /* �L�[�̕������𐔂��Ă��� */

  do {
	ptr = x_ptr;
	/* key �� checkbit�r�b�g�ڂō��E�ɐU�蕪�� */
	if(pat_bits(key,x_ptr->checkbit,key_length)==1){
	  x_ptr = x_ptr->right; OL(R);}
	else {x_ptr = x_ptr->left; OL(L);}
  } while(ptr->checkbit < x_ptr->checkbit);
  OL(\n);
  return x_ptr;
}


/****************************************************
* pat_insert --- �p�g���V�A�؂Ƀf�[�^��}��
* 
* �p�����[�^
*	f --- �t�@�C��
*	line --- �f�[�^(�}���L�[�Ɠ��e����؂蕶���ŋ�؂��Ă���\��)
*	index --- �f�[�^�̃t�@�C����̃C���f�b�N�X
*	x_ptr --- �}���̂��߂̌����̊J�n�ʒu
*	kugiri --- �L�[�Ɠ��e�̋�؂蕶��
* 
* �Ԃ��l
*	����!
****************************************************/
void pat_insert(FILE *f,char *line, long index, pat_node *x_ptr, char *kugiri)
{
  pat_node *t_ptr, *p_ptr, *new_ptr;
  int diff_bit;
  pat_index_list *new_l_ptr, *tmp_l_ptr, *mae_wo_sasu_ptr = NULL;
  int in_hash;
  int buffer_length;
  int key_length;
  char key[500];
  char buffer[50000]; /* �ėp�o�b�t�@ */

  OL(line:)OS(line);
  art_strcpy(key,500,line);
  art_strtok(key,kugiri);  /* �ŏ��̋�؂蕶���� '\0' �ɂ���D*/
  key_length = (int)strlen(key); /* �L�[�̕������𐔂��Ă��� */

  OL(key:)OS(key);

  /* �L�[�̒T�� */
  t_ptr = (pat_node*)pat_search4insert(key,x_ptr);

  if((t_ptr->il).index >= 0) {
	/* �n�b�V�����`�F�b�N */
	in_hash = hash_check_proc(f,(t_ptr->il).index,buffer);

	if(strncmp(key,buffer,strlen(key)) == 0){ /* �L�[����v */
	  /* printf("%s: �L�[����v������̂�����\n",buffer);fflush(stdout); */

	  tmp_l_ptr = &(t_ptr->il);

	  while(tmp_l_ptr !=NULL){
	in_hash = hash_check_proc(f,tmp_l_ptr->index,buffer);
	if(strcmp(buffer,line)==0){
	  /* �S�������̂�����̂ő}�������Ƀ��^�[�� */
/*	  printf("%s: �S�������̂�����̂Ŗ���\n",buffer);*/
	  return;
	}
	mae_wo_sasu_ptr = tmp_l_ptr;
	tmp_l_ptr = tmp_l_ptr->next;
	  }  /* ���̎��_�� tmp_l_ptr �̓��X�g�̖������w�� */

	  /* ���ɂ���L�[�ɓ��e������ɑ}������ */
	  new_l_ptr = (pat_index_list*)malloc_pat_index_list(); /* index��list */
	  new_l_ptr->index = index;
	  new_l_ptr->next = NULL;
	  mae_wo_sasu_ptr->next = new_l_ptr;

	  return;
	} else { /* �L�[����v���Ȃ������ꍇ buffer �ɂ��̈�v���Ȃ������L�[ */
	}
  } else { /* �f�[�^�̖����m�[�h�ɗ������ꍇ: �ŏ��Ƀf�[�^�����ꂽ�Ƃ� */
	*(buffer) = 0;*(buffer+1) = '\0';
  }


  /* �}���L�[�ƏՓ˂���L�[�Ƃ̊Ԃōŏ��ɈقȂ� bit �̈ʒu(diff_bit)�����߂� */
  buffer_length = (int)strlen(buffer);
  for(diff_bit=0; pat_bits(key,diff_bit,key_length) == pat_bits(buffer,diff_bit,buffer_length); diff_bit++)
	;/* �� */

  OL(diff_bit:)OI(diff_bit);

  /* �L�[��u���ʒu(x_ptr)�����߂�B */
  do {
	p_ptr = x_ptr;
	/* key �� checkbit�r�b�g�ڂō��E�ɐU�蕪�� */
	if(pat_bits(key,x_ptr->checkbit,key_length)==1) {x_ptr = x_ptr->right;}
	else {x_ptr = x_ptr->left;}
  } while((x_ptr->checkbit < diff_bit)&&(p_ptr->checkbit < x_ptr->checkbit));

  /* �}������m�[�h�𐶐����L�[�E�����r�b�g����ݒ肷��B */
  new_ptr = (pat_node*)malloc_pat_node(); /* �m�[�h�{�� */
  new_ptr->checkbit = (short)diff_bit; /* �`�F�b�N�r�b�g */
  (new_ptr->il).index = (short)index;
  (new_ptr->il).next = NULL;

  /* �q�߂Ɛe�߂�ݒ肷��B */
  /* �r�b�g��1�Ȃ�E�����N���L�[�̂���ʒu���w���B0�Ȃ獶�����N�B */
  if(pat_bits(key,new_ptr->checkbit,key_length)==1){
	new_ptr->right = new_ptr; new_ptr->left = x_ptr;
  } else {new_ptr->left = new_ptr; new_ptr->right = x_ptr;}
  /* �r�b�g��1�Ȃ�A�e�̉E�ɂȂ��B0�Ȃ獶�B */
  if(pat_bits(key,p_ptr->checkbit,key_length)==1) p_ptr->right = new_ptr;
  else p_ptr->left = new_ptr;

  return;
}
#endif

/****************************************************
* pat_bits --- �����񒆂̎w�肳�ꂽ�ʒu�̃r�b�g��Ԃ�
* 
* �p�����[�^
*	string --- ������
*	cbit --- �w�肳�ꂽ�ʒu�B������S�̂���̃r�b�g��ƍl���A
*			�擪(��)bit���� 0,1,2,3... �Ŏw�肷��B
*	len --- ������̒����Dstrlen��������������Ă��񂶂��ς����� 900918
*
* �Ԃ��l
*	0,1(�r�b�g),2(������̒������w�肳�ꂽ�ʒu���傫���Ƃ�)
****************************************************/
int pat_bits(char *string, int cbit, int len)
{
  int moji_idx = cbit / 8; /* �w�肳�ꂽ�ʒu���������ڂ� (for DEBUG)*/
  char moji = *(string+moji_idx); /* ���̕��� */
  int idx_in_moji = cbit % 8; /* ���̕����̉��r�b�g�ڂ� */
  if(cbit == -1) return 1; /* �g�b�v�m�[�h�̂Ƃ���1��Ԃ�(top����͕K���E) */
  if(len-1 < moji_idx) return 0;  /* ������̒��� < �w�肳�ꂽ�ʒu�̃`�F�b�N */
  return(((moji << idx_in_moji) & 0x80) >> 7); /* 0 or 1 ��Ԃ��B */
}



/****************************************************
* hash_check_proc --- �C���f�b�N�X�Ńn�b�V��������
* 
* �p�����[�^
*	index --- �C���f�b�N�X
* 
* �Ԃ��l  �n�b�V���ɂȂ���΃t�@�C��������D
*		  �������當����擪�|�C���^�C�Ȃ���� NULL ( �s�v��? )
****************************************************/
int hash_check_proc(FILE *f, long index, char *buf) {

  /* �L���b�V�������̏ꍇ */
#ifndef USE_HASH
  art_strcpy(buf, 50000, get_line(f,index));
  return(0);
#else
  if((data = th_hash_out( hash_array, HASH_SIZE, index, f)) == NULL) {
	strcpy(buf, get_line(f,index)); /* �Ȃ���΃t�@�C�������� */

	th_hash_in(hash_array,HASH_SIZE,index,buf,f);

	return(0);
  } else {
	strcpy(buf,data); /* ����Ηp���� */
	return(1);
  }
#endif
}


/****************************************************
* get_line --- �t�@�C���� pos �����ڂ��� \n �܂œǂ�
* 
* �p�����[�^
*	f --- �ǂރt�@�C��
*	pos --- �ǂݍ��ݎn�߂�ʒu
*	buf --- �ǂݍ��ރo�b�t�@
* 
* �Ԃ��l
*	������(strlen����) 
*	-1 : ���s
****************************************************/
char *get_line(FILE *f, long pos){
#ifdef HAVE_MMAP
  static int oldf = -1;
  static caddr_t addr;
  static off_t size;
  struct stat st;
#endif

#ifdef HAVE_MMAP
  if (oldf != ffd){
	for (i = 0; i < MAX_DIC_NUMBER; i++){
	  if (ffd == dicinfo[i].fd && dicinfo[i].used){
	oldf = dicinfo[i].fd;
	addr = dicinfo[i].addr;
	size = dicinfo[i].size;
	break;
	  }
	  if (dicinfo[i].used == 0){
	dicinfo[i].fd	= ffd;
	dicinfo[i].used = 1;
	fstat(dicinfo[i].fd, &st);
	dicinfo[i].size = size = st.st_size;
	dicinfo[i].addr = addr = mmap(NULL, dicinfo[i].size, PROT_READ,
					  MAP_PRIVATE, dicinfo[i].fd, 0);
	break;
	  }
	}
	if (i == MAX_DIC_NUMBER){
	  exit(1);
	}
	oldf = ffd;
  }

  if (pos >= size)
	return NULL;

#if 1
  return addr + pos;
#else
#if 1
  {
	  char *b = buf;
	  char *a = addr + pos;
	  i = 0;
	  while (*a && *a != '\n') {
	  *b++ = *a++;
	  i++;
	  }
	  *b = '\0';
  }
#else
  for (i = 0; addr[pos+i] && addr[pos+i] != '\n'; i++)
	buf[i] = addr[pos+i];
  buf[i] = 0;
#endif

  return i+1;
#endif

#else
	if(fseek(f, pos, 0) == 0){
	  static char buf[1000];
	  if(NULL == fgets(buf,sizeof(buf),f))
	return NULL;
	  return buf;
	}
	else return NULL; /* seek ���s */
#endif
}


/****************************************************
* show_pat --- �p�g���V�A�؃f�[�^���o��
*
* �p�����[�^
*	top_ptr --- �����J�n�m�[�h�̈ʒu(�|�C���^)
*	out_to --- �o�͐�(stdout��t�@�C��)
* 
* �Ԃ��l
*	�����B�p�g���V�A�؃f�[�^���o�́B
****************************************************/
void show_pat(pat_node * /*top_ptr*/, FILE * /*out_to*/, char * /*prefix*/)
{
#if 0
  long idx = -1;
  pat_index_list *t_ptr;
  char word[100];
  char pftmp[100];
  char prefix_keep[100];

  word[0] = '\0';

  strcpy(prefix_keep,prefix);

  OL(-------\n);
  OL(prefix:)OS(prefix);
  OL(<checkbit>)OI(top_ptr->checkbit);

  OL(## ��\n)
  /* �~���r�b�g�̂Ƃ� */
  if(top_ptr->checkbit % SIKII_BIT == 0 && top_ptr->checkbit != 0){
	strcpy(word, get_line(dic_file[0],top_ptr->left->il_ptr->index));
	strtok(word,"\t");
	OL(SIKIIbitProcess\n)
	OL(SIKIIword:)OS(word);
	strcpy(pftmp,(word+strlen(prefix)));
	OL(keep:)OS(pftmp);

/*
	printf("#@# %i\n",strlen(word));
	printf("### %i\n",strlen(pftmp));

	top_ptr->left->str = (char*)malloc(strlen(word)+1);
	strcpy(top_ptr->left->str,word);
*/
	top_ptr->left->str = (char*)malloc(strlen(pftmp)+1);
	strcpy(top_ptr->left->str,pftmp);

	strcat(prefix,pftmp);

	OS(pftmp);

  } else {
	/* ���E�� Subtree �̏����B�t���ςłȂ���΍ċA�B*/
	if(top_ptr->checkbit < top_ptr->left->checkbit){
	  show_pat(top_ptr->left,out_to,prefix);}
	else {
	  if(top_ptr->left->il_ptr != NULL) {
	strcpy(word, get_line(dic_file[0],top_ptr->left->il_ptr->index));
	strtok(word,"\t");
	OL(word:)OS(word);
	strcpy(pftmp,(word+strlen(prefix)));
	OL(keep:)OS(pftmp);

/*
	printf("#@# %i\n",strlen(word));
	printf("### %i\n",strlen(pftmp));

	top_ptr->left->str = (char*)malloc(strlen(word)+1);
	strcpy(top_ptr->left->str,word);
*/
	top_ptr->left->str = (char*)malloc(strlen(pftmp)+1);
	strcpy(top_ptr->left->str,pftmp);

	OS(word);
	  }
	}

  }

  OL(## �E\n)
  if(top_ptr->checkbit < top_ptr->right->checkbit){
	show_pat(top_ptr->right,out_to,prefix);}
  else {
	if(top_ptr->right->il_ptr != NULL) {
	  strcpy(word, get_line(dic_file[0],top_ptr->right->il_ptr->index));
	  strtok(word,"\t");
	  OL(word:)OS(word);
	  strcpy(pftmp,(word+strlen(prefix)));
	  OL(keep:)OS(pftmp);

/*
	printf("#@# %i\n",strlen(word));
	printf("### %i\n",strlen(pftmp));

	top_ptr->left->str = (char*)malloc(strlen(word)+1);
	strcpy(top_ptr->left->str,word);
*/
	  top_ptr->right->str = (char*)malloc(strlen(pftmp)+1);
	  strcpy(top_ptr->right->str,pftmp);

	  OS(word);
	}
  }

  OL(---------back-------\n);

  strcpy(prefix,prefix_keep);
  return;
#endif
}

