#if !defined(AFX_MR999_H__829F1628_C6F9_4F0D_846D_AC7B6A06048A__INCLUDED_)
#define AFX_MR999_H__829F1628_C6F9_4F0D_846D_AC7B6A06048A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MR999.h : MR999.DLL �̃��C�� �w�b�_�[ �t�@�C���ł�

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CMR999App : ���̃N���X�̓���̒�`�Ɋւ��Ă� MR999.cpp �t�@�C�����Q�Ƃ��Ă�������

class CMR999App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MR999_H__829F1628_C6F9_4F0D_846D_AC7B6A06048A__INCLUDED)
