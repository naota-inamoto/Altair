// igo.h : igo.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CigoApp
// ���̃N���X�̎����Ɋւ��Ă� igo.cpp ���Q�Ƃ��Ă��������B
//

class CigoApp : public CWinApp
{
public:
	CigoApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
