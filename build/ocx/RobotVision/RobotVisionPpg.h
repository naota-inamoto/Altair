#if !defined(AFX_ROBOTVISIONPPG_H__DBC3030C_6E5F_4451_8AE9_46E674DFB199__INCLUDED_)
#define AFX_ROBOTVISIONPPG_H__DBC3030C_6E5F_4451_8AE9_46E674DFB199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RobotVisionPpg.h : CRobotVisionPropPage �v���p�e�B �y�[�W �N���X�̐錾���s���܂�

////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage : ���̃N���X�̓���̒�`�Ɋւ��Ă� RobotVisionPpg.cpp.cpp �t�@�C�����Q�Ƃ��Ă�������

class CRobotVisionPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRobotVisionPropPage)
	DECLARE_OLECREATE_EX(CRobotVisionPropPage)

// �R���X�g���N�^
public:
	CRobotVisionPropPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CRobotVisionPropPage)
	enum { IDD = IDD_PROPPAGE_ROBOTVISION };
	//}}AFX_DATA

// �C���v�������e�[�V����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ���b�Z�[�W �}�b�v
protected:
	//{{AFX_MSG(CRobotVisionPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ROBOTVISIONPPG_H__DBC3030C_6E5F_4451_8AE9_46E674DFB199__INCLUDED)
