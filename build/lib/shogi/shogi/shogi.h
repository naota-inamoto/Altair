// shogi.h : shogi.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CshogiApp
// ���̃N���X�̎����Ɋւ��Ă� shogi.cpp ���Q�Ƃ��Ă��������B
//

class CshogiApp : public CWinApp
{
public:
	CshogiApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
