#if !defined(AFX_ROBOTVISIONPPG_H__DBC3030C_6E5F_4451_8AE9_46E674DFB199__INCLUDED_)
#define AFX_ROBOTVISIONPPG_H__DBC3030C_6E5F_4451_8AE9_46E674DFB199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RobotVisionPpg.h : CRobotVisionPropPage プロパティ ページ クラスの宣言を行います

////////////////////////////////////////////////////////////////////////////
// CRobotVisionPropPage : このクラスの動作の定義に関しては RobotVisionPpg.cpp.cpp ファイルを参照してください

class CRobotVisionPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRobotVisionPropPage)
	DECLARE_OLECREATE_EX(CRobotVisionPropPage)

// コンストラクタ
public:
	CRobotVisionPropPage();

// ダイアログ データ
	//{{AFX_DATA(CRobotVisionPropPage)
	enum { IDD = IDD_PROPPAGE_ROBOTVISION };
	//}}AFX_DATA

// インプリメンテーション
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// メッセージ マップ
protected:
	//{{AFX_MSG(CRobotVisionPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ROBOTVISIONPPG_H__DBC3030C_6E5F_4451_8AE9_46E674DFB199__INCLUDED)
