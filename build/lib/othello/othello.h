// othello.h : othello.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂ޑO�ɁA'stdafx.h' ���܂߂Ă��������B
#endif

#include "resource.h"		// ���C�� �V���{��


// CothelloApp
// ���̃N���X�̎����Ɋւ��Ă� othello.cpp ���Q�Ƃ��Ă��������B
//

class CothelloApp : public CWinApp
{
public:
	CothelloApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
