
#if !defined(AFX_ALREG_H__22E6010C_0815_11D2_86B0_444553540000__INCLUDED_)
#define AFX_ALREG_H__22E6010C_0815_11D2_86B0_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// AlReg.h : ALREG.DLL �̃��C�� �w�b�_�[ �t�@�C���ł�

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CAlRegApp : ���̃N���X�̓���̒�`�Ɋւ��Ă� AlReg.cpp �t�@�C�����Q�Ƃ��Ă�������

class CAlRegApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ALREG_H__22E6010C_0815_11D2_86B0_444553540000__INCLUDED)
