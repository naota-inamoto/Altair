#if !defined(AFX_ROBOTVISION_H__A2D4CDA5_B9CA_4EC2_B8E3_B6C81118F4B3__INCLUDED_)
#define AFX_ROBOTVISION_H__A2D4CDA5_B9CA_4EC2_B8E3_B6C81118F4B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RobotVision.h : ROBOTVISION.DLL �̃��C�� �w�b�_�[ �t�@�C���ł�

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CRobotVisionApp : ���̃N���X�̓���̒�`�Ɋւ��Ă� RobotVision.cpp �t�@�C�����Q�Ƃ��Ă�������

class CRobotVisionApp : public COleControlModule
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

#endif // !defined(AFX_ROBOTVISION_H__A2D4CDA5_B9CA_4EC2_B8E3_B6C81118F4B3__INCLUDED)
