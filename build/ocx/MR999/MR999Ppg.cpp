// MR999Ppg.cpp : CMR999PropPage プロパティ ページ クラスのインプリメンテーション

#include "stdafx.h"
#include "MR999.h"
#include "MR999Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMR999PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// メッセージ マップ

BEGIN_MESSAGE_MAP(CMR999PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CMR999PropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリ と guid の初期化

IMPLEMENT_OLECREATE_EX(CMR999PropPage, "MR999.MR999PropPage.1",
	0xc68aaec8, 0xcb7f, 0x4fe0, 0x9e, 0x4a, 0xd0, 0xff, 0xd8, 0xc, 0xc, 0x14)


/////////////////////////////////////////////////////////////////////////////
// CMR999PropPage::CMR999PropPageFactory::UpdateRegistry -
// CMR999PropPage のシステム レジストリのエントリを追加または削除します

BOOL CMR999PropPage::CMR999PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_MR999_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CMR999PropPage::CMR999PropPage - コンストラクタ

CMR999PropPage::CMR999PropPage() :
	COlePropertyPage(IDD, IDS_MR999_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CMR999PropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CMR999PropPage::DoDataExchange - ページとプロパティの間でデータの交換を行います

void CMR999PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CMR999PropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CMR999PropPage メッセージ ハンドラ
