// jumanknp1.cpp : DLL �̏��������[�`���ł��B
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE jumanknp1DLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved ���g���ꍇ�͂������폜���Ă�������
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("jumanknp1.DLL ��������\n");
		
		// �g�� DLL ���P�񂾂����������܂��B
		if (!AfxInitExtensionModule(jumanknp1DLL, hInstance))
			return 0;

		// ���� DLL �����\�[�X �`�F�C���֑}�����܂��B
		// ����: �g�� DLL �� MFC �A�v���P�[�V�����ł͂Ȃ�
		//   MFC �W�� DLL (ActiveX �R���g���[���̂悤��)
		//   �ɈÖٓI�Ƀ����N�����ꍇ�A���̍s�� DllMain
		//   ����폜���āA���̊g�� DLL ����G�N�X�|�[�g
		//   ���ꂽ�ʂ̊֐����֒ǉ����Ă��������B  
		//   ���̊g�� DLL ���g�p����W�� DLL �͂��̊g�� DLL
		//   �����������邽�߂ɖ����I�ɂ��̊֐����Ăяo���܂��B 
		//   ����ȊO�̏ꍇ�́ACDynLinkLibrary �I�u�W�F�N�g��
		//   �W�� DLL �̃��\�[�X �`�F�C���փA�^�b�`���ꂸ�A
		//   ���̌��ʏd��Ȗ��ƂȂ�܂��B

		new CDynLinkLibrary(jumanknp1DLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("jumanknp1.DLL �I����\n");

		// �f�X�g���N�^���Ăяo�����O�Ƀ��C�u�������I�����܂�
		AfxTermExtensionModule(jumanknp1DLL);
	}
	return 1;   // OK
}
