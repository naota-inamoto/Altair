
#if !defined(AFX_ALREGCTL_H__22E60114_0815_11D2_86B0_444553540000__INCLUDED_)
#define AFX_ALREGCTL_H__22E60114_0815_11D2_86B0_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// AlRegCtl.h : CAlRegCtrl ActiveX コントロール クラスの宣言

/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl : このクラスの動作の定義に関しては AlRegCtl.cpp を参照してください

class CAlRegCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAlRegCtrl)

// コンストラクタ
public:
	CAlRegCtrl();

// オーバーライド
	// ClassWizard は仮想関数のオーバライドを生成します
	//{{AFX_VIRTUAL(CAlRegCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	~CAlRegCtrl();

	DECLARE_OLECREATE_EX(CAlRegCtrl)	// クラス ファクトリ と guid
	DECLARE_OLETYPELIB(CAlRegCtrl)		// GetTypeInfo
	DECLARE_PROPPAGEIDS(CAlRegCtrl) 	// プロパティ ページ ID
	DECLARE_OLECTLTYPE(CAlRegCtrl)		// タイプ名とその他のステータス

	HKEY m_hKey;
	CString m_subKey;
	REGSAM m_accessMask;

	HKEY OpenKey();
	HKEY CreateKey();

	long m_intValue;
	CString m_stringValue;

// メッセージ マップ
	//{{AFX_MSG(CAlRegCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// ディスパッチ マップ
	//{{AFX_DISPATCH(CAlRegCtrl)
	afx_msg short SetRegistryKey(LPCTSTR hKey, LPCTSTR subKey, LPCTSTR accessMask);
	afx_msg short QueryInt(LPCTSTR name);
	afx_msg short QueryString(LPCTSTR name);
	afx_msg long GetIntValue();
	afx_msg BSTR GetStringValue();
	afx_msg short SetIntValue(LPCTSTR name, long value);
	afx_msg short SetStringValue(LPCTSTR name, LPCTSTR value);
	afx_msg short DeleteKey(LPCTSTR name);
	afx_msg short DeleteValue(LPCTSTR name);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// イベント マップ
	//{{AFX_EVENT(CAlRegCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// ディスパッチ と イベント ID
public:
	enum {
	//{{AFX_DISP_ID(CAlRegCtrl)
	dispidSetRegistryKey = 1L,
	dispidQueryInt = 2L,
	dispidQueryString = 3L,
	dispidGetIntValue = 4L,
	dispidGetStringValue = 5L,
	dispidSetIntValue = 6L,
	dispidSetStringValue = 7L,
	dispidDeleteKey = 8L,
	dispidDeleteValue = 9L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ALREGCTL_H__22E60114_0815_11D2_86B0_444553540000__INCLUDED)
