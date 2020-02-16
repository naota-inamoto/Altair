
// AlCtrlPpg.cpp : CAlCtrlPropPage プロパティ ページ クラスのインプリメンテーション

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
// メッセージ マップ

BEGIN_MESSAGE_MAP(CAlCtrlPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CAlCtrlPropPage)
	// メモ - ClassWizard はこの位置にメッセージ マップのエントリを追加または削除します
	//	  この位置に生成されるコードを編集しないでください！
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリ と guid の初期化

IMPLEMENT_OLECREATE_EX(CAlCtrlPropPage, "ALCTRL.AlCtrlPropPage.1",
	0xcbe7bc04, 0x1f9e, 0x11d0, 0xb3, 0x4e, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage::CAlCtrlPropPageFactory::UpdateRegistry -
// CAlCtrlPropPage のシステム レジストリのエントリを追加または削除します

BOOL CAlCtrlPropPage::CAlCtrlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ALCTRL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage::CAlCtrlPropPage - コンストラクタ

CAlCtrlPropPage::CAlCtrlPropPage() :
	COlePropertyPage(IDD, IDS_ALCTRL_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CAlCtrlPropPage)
	m_transparent = FALSE;
	m_borderWidth = 0;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage::DoDataExchange - ページとプロパティの間でデータの交換を行います

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
// CAlCtrlPropPage メッセージ ハンドラ


