
#if !defined(AFX_ALREGPPG_H__22E60116_0815_11D2_86B0_444553540000__INCLUDED_)
#define AFX_ALREGPPG_H__22E60116_0815_11D2_86B0_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// AlRegPpg.h : CAlRegPropPage �v���p�e�B �y�[�W �N���X�̐錾���s���܂�

////////////////////////////////////////////////////////////////////////////
// CAlRegPropPage : ���̃N���X�̓���̒�`�Ɋւ��Ă� AlRegPpg.cpp.cpp �t�@�C�����Q�Ƃ��Ă�������

class CAlRegPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAlRegPropPage)
	DECLARE_OLECREATE_EX(CAlRegPropPage)

// �R���X�g���N�^
public:
	CAlRegPropPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAlRegPropPage)
	enum { IDD = IDD_PROPPAGE_ALREG };
	//}}AFX_DATA

// �C���v�������e�[�V����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

// ���b�Z�[�W �}�b�v
protected:
	//{{AFX_MSG(CAlRegPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ALREGPPG_H__22E60116_0815_11D2_86B0_444553540000__INCLUDED)
