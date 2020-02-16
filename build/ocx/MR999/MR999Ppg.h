#if !defined(AFX_MR999PPG_H__F9F7751C_01CA_4E5F_A301_696F7D0F6FE3__INCLUDED_)
#define AFX_MR999PPG_H__F9F7751C_01CA_4E5F_A301_696F7D0F6FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MR999Ppg.h : CMR999PropPage プロパティ ページ クラスの宣言を行います

////////////////////////////////////////////////////////////////////////////
// CMR999PropPage : このクラスの動作の定義に関しては MR999Ppg.cpp.cpp ファイルを参照してください

class CMR999PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CMR999PropPage)
	DECLARE_OLECREATE_EX(CMR999PropPage)

// コンストラクタ
public:
	CMR999PropPage();

// ダイアログ データ
	//{{AFX_DATA(CMR999PropPage)
	enum { IDD = IDD_PROPPAGE_MR999 };
	//}}AFX_DATA

// インプリメンテーション
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// メッセージ マップ
protected:
	//{{AFX_MSG(CMR999PropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MR999PPG_H__F9F7751C_01CA_4E5F_A301_696F7D0F6FE3__INCLUDED)
