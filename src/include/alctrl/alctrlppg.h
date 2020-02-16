
// AlCtrlPpg.h : CAlCtrlPropPage プロパティ ページ クラスの宣言を行います

////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage : このクラスの動作の定義に関しては AlCtrlPpg.cpp.cpp ファイルを参照してください

class CAlCtrlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAlCtrlPropPage)
	DECLARE_OLECREATE_EX(CAlCtrlPropPage)

// コンストラクタ
public:
	CAlCtrlPropPage();

// ダイアログ データ
	//{{AFX_DATA(CAlCtrlPropPage)
	enum { IDD = IDD_PROPPAGE_ALCTRL };
	BOOL	m_transparent;
	int 	m_borderWidth;
	//}}AFX_DATA

// インプリメンテーション
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// メッセージ マップ
protected:
	//{{AFX_MSG(CAlCtrlPropPage)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します
		//	  この位置に生成されるコードを編集しないでください！
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


