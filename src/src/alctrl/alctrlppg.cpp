
// AlCtrlPpg.cpp : CAlCtrlPropPage �v���p�e�B �y�[�W �N���X�̃C���v�������e�[�V����

#include "stdafx.h"
#include "AlCtrl/AlCtrl.h"
#include "AlCtrl/AlCtrlPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAlCtrlPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W �}�b�v

BEGIN_MESSAGE_MAP(CAlCtrlPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CAlCtrlPropPage)
	// ���� - ClassWizard �͂��̈ʒu�Ƀ��b�Z�[�W �}�b�v�̃G���g����ǉ��܂��͍폜���܂�
	//	  ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������I
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// �N���X �t�@�N�g�� �� guid �̏�����

IMPLEMENT_OLECREATE_EX(CAlCtrlPropPage, "ALCTRL.AlCtrlPropPage.1",
	0xcbe7bc04, 0x1f9e, 0x11d0, 0xb3, 0x4e, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage::CAlCtrlPropPageFactory::UpdateRegistry -
// CAlCtrlPropPage �̃V�X�e�� ���W�X�g���̃G���g����ǉ��܂��͍폜���܂�

BOOL CAlCtrlPropPage::CAlCtrlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ALCTRL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage::CAlCtrlPropPage - �R���X�g���N�^

CAlCtrlPropPage::CAlCtrlPropPage() :
	COlePropertyPage(IDD, IDS_ALCTRL_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CAlCtrlPropPage)
	m_transparent = FALSE;
	m_borderWidth = 0;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage::DoDataExchange - �y�[�W�ƃv���p�e�B�̊ԂŃf�[�^�̌������s���܂�

void CAlCtrlPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CAlCtrlPropPage)
	DDP_Check(pDX, IDC_TRANSPARENT, m_transparent, _T("Transparent") );
	DDX_Check(pDX, IDC_TRANSPARENT, m_transparent);
	DDP_Text(pDX, IDC_BORDERWIDTH, m_borderWidth, _T("BorderWidth") );
	DDX_Text(pDX, IDC_BORDERWIDTH, m_borderWidth);
	DDV_MinMaxInt(pDX, m_borderWidth, 0, 30);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage ���b�Z�[�W �n���h��


