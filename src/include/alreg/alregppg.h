
#if !defined(AFX_ALREGPPG_H__22E60116_0815_11D2_86B0_444553540000__INCLUDED_)
#define AFX_ALREGPPG_H__22E60116_0815_11D2_86B0_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// AlRegPpg.h : CAlRegPropPage プロパティ ページ クラスの宣言を行います

////////////////////////////////////////////////////////////////////////////
// CAlRegPropPage : このクラスの動作の定義に関しては AlRegPpg.cpp.cpp ファイルを参照してください

class CAlRegPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAlRegPropPage)
	DECLARE_OLECREATE_EX(CAlRegPropPage)

// コンストラクタ
public:
	CAlRegPropPage();

// ダイアログ データ
	//{{AFX_DATA(CAlRegPropPage)
	enum { IDD = IDD_PROPPAGE_ALREG };
	//}}AFX_DATA

// インプリメンテーション
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// メッセージ マップ
protected:
	//{{AFX_MSG(CAlRegPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ALREGPPG_H__22E60116_0815_11D2_86B0_444553540000__INCLUDED)
