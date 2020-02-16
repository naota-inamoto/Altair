
#if !defined(AFX_ALREGCTL_H__22E60114_0815_11D2_86B0_444553540000__INCLUDED_)
#define AFX_ALREGCTL_H__22E60114_0815_11D2_86B0_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// AlRegCtl.h : CAlRegCtrl ActiveX �R���g���[�� �N���X�̐錾

/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl : ���̃N���X�̓���̒�`�Ɋւ��Ă� AlRegCtl.cpp ���Q�Ƃ��Ă�������

class CAlRegCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAlRegCtrl)

// �R���X�g���N�^
public:
	CAlRegCtrl();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAlRegCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	~CAlRegCtrl();

	DECLARE_OLECREATE_EX(CAlRegCtrl)	// �N���X �t�@�N�g�� �� guid
	DECLARE_OLETYPELIB(CAlRegCtrl)		// GetTypeInfo
	DECLARE_PROPPAGEIDS(CAlRegCtrl) 	// �v���p�e�B �y�[�W ID
	DECLARE_OLECTLTYPE(CAlRegCtrl)		// �^�C�v���Ƃ��̑��̃X�e�[�^�X

	HKEY m_hKey;
	CString m_subKey;
	REGSAM m_accessMask;

	HKEY OpenKey();
	HKEY CreateKey();

	long m_intValue;
	CString m_stringValue;

// ���b�Z�[�W �}�b�v
	//{{AFX_MSG(CAlRegCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// �f�B�X�p�b�` �}�b�v
	//{{AFX_DISPATCH(CAlRegCtrl)
	afx_msg short SetRegistryKey(LPCTSTR hKey, LPCTSTR subKey, LPCTSTR accessMask);
	afx_msg short QueryInt(LPCTSTR name);
	afx_msg short QueryString(LPCTSTR name);
	afx_msg long GetIntValue();
	afx_msg BSTR GetStringValue();
	afx_msg short SetIntValue(LPCTSTR name, long value);
	afx_msg short SetStringValue(LPCTSTR name, LPCTSTR value);
	afx_msg short DeleteKey(LPCTSTR name);
	afx_msg short DeleteValue(LPCTSTR name);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �C�x���g �}�b�v
	//{{AFX_EVENT(CAlRegCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// �f�B�X�p�b�` �� �C�x���g ID
public:
	enum {
	//{{AFX_DISP_ID(CAlRegCtrl)
	dispidSetRegistryKey = 1L,
	dispidQueryInt = 2L,
	dispidQueryString = 3L,
	dispidGetIntValue = 4L,
	dispidGetStringValue = 5L,
	dispidSetIntValue = 6L,
	dispidSetStringValue = 7L,
	dispidDeleteKey = 8L,
	dispidDeleteValue = 9L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ALREGCTL_H__22E60114_0815_11D2_86B0_444553540000__INCLUDED)
