// RobotVisionPpg.cpp : CRobotVisionPropPage �v���p�e�B �y�[�W �N���X�̃C���v�������e�[�V����

#include "stdafx.h"
#include "RobotVision.h"
#include "RobotVisionPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRobotVisionPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W �}�b�v

BEGIN_MESSAGE_MAP(CRobotVisionPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CRobotVisionPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// �N���X �t�@�N�g�� �� guid �̏�����

IMPLEMENT_OLECREATE_EX(CRobotVisionPropPage, "ROBOTVISION.RobotVisionPropPage.1",
	0x13a667ef, 0x297f, 0x4b7c, 0x81, 0xc5, 0x38, 0x31, 0xe8, 0x3a, 0x28, 0x5d)


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage::CRobotVisionPropPageFactory::UpdateRegistry -
// CRobotVisionPropPage �̃V�X�e�� ���W�X�g���̃G���g����ǉ��܂��͍폜���܂�

BOOL CRobotVisionPropPage::CRobotVisionPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ROBOTVISION_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage::CRobotVisionPropPage - �R���X�g���N�^

CRobotVisionPropPage::CRobotVisionPropPage() :
	COlePropertyPage(IDD, IDS_ROBOTVISION_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CRobotVisionPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage::DoDataExchange - �y�[�W�ƃv���p�e�B�̊ԂŃf�[�^�̌������s���܂�

void CRobotVisionPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CRobotVisionPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage ���b�Z�[�W �n���h��
