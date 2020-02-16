
// AlMmPpg.h : CAlMmPropPage プロパティ ページ クラスの宣言を行います

////////////////////////////////////////////////////////////////////////////
// CAlMmPropPage : このクラスの動作の定義に関しては AlMmPpg.cpp.cpp ファイルを参照してください

class CAlMmPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAlMmPropPage)
	DECLARE_OLECREATE_EX(CAlMmPropPage)

// コンストラクタ
public:
	CAlMmPropPage();

// ダイアログ データ
	//{{AFX_DATA(CAlMmPropPage)
	enum { IDD = IDD_PROPPAGE_ALMM };
	//}}AFX_DATA

// インプリメンテーション
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// メッセージ マップ
protected:
	//{{AFX_MSG(CAlMmPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


