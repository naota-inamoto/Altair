
// AlRegPpg.cpp : CAlRegPropPage プロパティ ページ クラスのインプリメンテーション

#include "stdafx.h"
#include "AlReg/AlReg.h"
#include "AlReg/AlRegPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAlRegPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// メッセージ マップ

BEGIN_MESSAGE_MAP(CAlRegPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CAlRegPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリ と guid の初期化

IMPLEMENT_OLECREATE_EX(CAlRegPropPage, "ALREG.AlRegPropPage.1",
	0x22e60108, 0x815, 0x11d2, 0x86, 0xb0, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CAlRegPropPage::CAlRegPropPageFactory::UpdateRegistry -
// CAlRegPropPage のシステム レジストリのエントリを追加または削除します

BOOL CAlRegPropPage::CAlRegPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ALREG_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegPropPage::CAlRegPropPage - コンストラクタ

CAlRegPropPage::CAlRegPropPage() :
	COlePropertyPage(IDD, IDS_ALREG_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CAlRegPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegPropPage::DoDataExchange - ページとプロパティの間でデータの交換を行います

void CAlRegPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CAlRegPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegPropPage メッセージ ハンドラ

