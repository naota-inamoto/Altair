
// AlMmPpg.cpp : CAlMmPropPage プロパティ ページ クラスのインプリメンテーション

#include "stdafx.h"
#include "AlMm/AlMm.h"
#include "AlMm/AlMmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAlMmPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// メッセージ マップ

BEGIN_MESSAGE_MAP(CAlMmPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CAlMmPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリ と guid の初期化

IMPLEMENT_OLECREATE_EX(CAlMmPropPage, "ALMM.AlMmPropPage.1",
	0x7755885, 0x7c16, 0x11d0, 0x84, 0x5c, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CAlMmPropPage::CAlMmPropPageFactory::UpdateRegistry -
// CAlMmPropPage のシステム レジストリのエントリを追加または削除します

BOOL CAlMmPropPage::CAlMmPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ALMM_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmPropPage::CAlMmPropPage - コンストラクタ

CAlMmPropPage::CAlMmPropPage() :
	COlePropertyPage(IDD, IDS_ALMM_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CAlMmPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmPropPage::DoDataExchange - ページとプロパティの間でデータの交換を行います

void CAlMmPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CAlMmPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmPropPage メッセージ ハンドラ


