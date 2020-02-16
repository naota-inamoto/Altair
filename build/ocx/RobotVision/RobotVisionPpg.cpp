// RobotVisionPpg.cpp : CRobotVisionPropPage プロパティ ページ クラスのインプリメンテーション

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
// メッセージ マップ

BEGIN_MESSAGE_MAP(CRobotVisionPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CRobotVisionPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリ と guid の初期化

IMPLEMENT_OLECREATE_EX(CRobotVisionPropPage, "ROBOTVISION.RobotVisionPropPage.1",
	0x13a667ef, 0x297f, 0x4b7c, 0x81, 0xc5, 0x38, 0x31, 0xe8, 0x3a, 0x28, 0x5d)


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage::CRobotVisionPropPageFactory::UpdateRegistry -
// CRobotVisionPropPage のシステム レジストリのエントリを追加または削除します

BOOL CRobotVisionPropPage::CRobotVisionPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ROBOTVISION_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage::CRobotVisionPropPage - コンストラクタ

CRobotVisionPropPage::CRobotVisionPropPage() :
	COlePropertyPage(IDD, IDS_ROBOTVISION_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CRobotVisionPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage::DoDataExchange - ページとプロパティの間でデータの交換を行います

void CRobotVisionPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CRobotVisionPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage メッセージ ハンドラ
