#if !defined(AFX_MR999PPG_H__F9F7751C_01CA_4E5F_A301_696F7D0F6FE3__INCLUDED_)
#define AFX_MR999PPG_H__F9F7751C_01CA_4E5F_A301_696F7D0F6FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MR999Ppg.h : CMR999PropPage �v���p�e�B �y�[�W �N���X�̐錾���s���܂�

////////////////////////////////////////////////////////////////////////////
// CMR999PropPage : ���̃N���X�̓���̒�`�Ɋւ��Ă� MR999Ppg.cpp.cpp �t�@�C�����Q�Ƃ��Ă�������

class CMR999PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CMR999PropPage)
	DECLARE_OLECREATE_EX(CMR999PropPage)

// �R���X�g���N�^
public:
	CMR999PropPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMR999PropPage)
	enum { IDD = IDD_PROPPAGE_MR999 };
	//}}AFX_DATA

// �C���v�������e�[�V����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ���b�Z�[�W �}�b�v
protected:
	//{{AFX_MSG(CMR999PropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MR999PPG_H__F9F7751C_01CA_4E5F_A301_696F7D0F6FE3__INCLUDED)
