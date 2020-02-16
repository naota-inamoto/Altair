
#include "stdafx.h"

#include "knp/knp.h"

#include <stdio.h>

int 	Revised_para_num;

char	*ErrorComment = NULL;	/* �G���[�R�����g */
char	PM_Memo[DATA_LEN];		/* �p�^�[���}�b�`���� */

int 	match_matrix[BNST_MAX][BNST_MAX];
int 	path_matrix[BNST_MAX][BNST_MAX];
int 	restrict_matrix[BNST_MAX][BNST_MAX];
int 	Dpnd_matrix[BNST_MAX][BNST_MAX]; /* �W��\�� 0, D, P, A */
int 	Quote_matrix[BNST_MAX][BNST_MAX];/* ���ʃ}�X�N 0, 1 */
int 	Mask_matrix[BNST_MAX][BNST_MAX]; /* ����}�X�N
							0:�W��󂯋֎~
							1:�W���OK
							2:�����head��,
							3:�����gap��head�� */

char	**Options;

int 	OptAnalysis = OPT_DPND;
int 	OptInput = OPT_RAW;
int 	OptExpress = OPT_NOTAGTREE;
int 	OptDisplay = OPT_NORMAL;
int 	OptExpandP = FALSE;
char	OptIgnoreChar = '\0';
int 	OptReadFeature = 0;
int 	OptMode = STAND_ALONE_MODE;

int 	OptCheck = FALSE;
int 	OptUseScase = TRUE;
int 	OptServerFlag = 0;
int 	OptCopula = 0;

int CLASS_num;

/*==================================================================*/
				 void usage()
/*==================================================================*/
{
	fprintf(stderr, "Usage: knp [-dpnd|bnst]\n" 
		"           [-tree|tab|sexp|tagtree|tagtab]\n"	
		"           [-normal|detail|debug]\n"	
		"           [-expand]\n"
		"           [-C host:port] [-S|F] [-N port]\n"
		"           [-timeout second] [-r rcfile]\n");
	exit(1);	
}

