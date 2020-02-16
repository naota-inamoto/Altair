
// AlCtrlCtl.h : CAlCtrlCtrl OLE コントロール クラスの宣言

/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl : このクラスの動作の定義に関しては AlCtrlCtl.cpp を参照してください

class CAlCtrlCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAlCtrlCtrl)

// コンストラクタ
public:
	CAlCtrlCtrl();

// オーバーライド

	// 描画関数
	virtual void OnDraw(
				CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

	// 永続性
	virtual void DoPropExchange(CPropExchange* pPX);

	// コントロールのプロパティ値のリセット
	virtual void OnResetState();

public:
	virtual int SetMethod(AGmNode* method, AGmNode* obj, AGmNode* func);
	virtual void CustomEvent(AGmNode* event, AGmNode* params);

// インプリメンテーション
protected:
	~CAlCtrlCtrl();

	DECLARE_OLECREATE_EX(CAlCtrlCtrl)	 // クラス ファクトリ と guid
	DECLARE_OLETYPELIB(CAlCtrlCtrl) 	 // GetTypeInfo
	DECLARE_PROPPAGEIDS(CAlCtrlCtrl)	 // プロパティ ページ ID
	DECLARE_OLECTLTYPE(CAlCtrlCtrl) 	// タイプ名とその他のステータス

// ---- Inet ----
	int CheckURL(LPCSTR url,
		DWORD &service, CString &server, CString &object, INTERNET_PORT &port,
		CString &name);
	AGmString *Download(
		DWORD service, CString server, CString object, INTERNET_PORT port);

	int InitRootClass(LPCSTR name, AGmNode *&proj);
	short LoadBody1(AGmString *str, int inet);

	AGmNode *m_eventParams;

// メッセージ マップ
	//{{AFX_MSG(CAlCtrlCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// ディスパッチ マップ
	//{{AFX_DISPATCH(CAlCtrlCtrl)
	BOOL m_transparent;
	afx_msg void OnTransparentChanged();
	short m_borderWidth;
	afx_msg void OnBorderWidthChanged();
	OLE_COLOR m_lightColor;
	afx_msg void OnLightColorChanged();
	OLE_COLOR m_shadowColor;
	afx_msg void OnShadowColorChanged();
	CString m_eventName;
	afx_msg void OnEventNameChanged();
	afx_msg VARIANT GetWnd();
	afx_msg VARIANT Invoke(LPCTSTR methodName, const VARIANT FAR& args);
	afx_msg VARIANT GetRootClass();
	afx_msg short SetRootClass(const VARIANT FAR& rootClass);
	afx_msg short SaveBody(LPCTSTR fileName);
	afx_msg short LoadBody(LPCTSTR fileName);
	afx_msg short LoadProgram(LPCTSTR dir, LPCTSTR name);
	afx_msg VARIANT Arg(LPCTSTR name, const VARIANT FAR& value, const VARIANT FAR& args);
	afx_msg VARIANT Param(const VARIANT FAR& params, short index);
	afx_msg void DebugMode(short mode);
	afx_msg void SetLibraryPath(LPCTSTR path);
	afx_msg short InetProgram(LPCTSTR url);
	afx_msg short InetLibrary(LPCTSTR url);
	afx_msg short InetBody(LPCTSTR url);
	afx_msg VARIANT EventParam(short index);
	afx_msg VARIANT EmptyArg();
	OLE_COLOR m_backColor;
	afx_msg void OnBackColorChanged();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// イベント マップ
	//{{AFX_EVENT(CAlCtrlCtrl)
	void FireCustomEvent(LPCTSTR eventName, const VARIANT FAR& params)
		{FireEvent(eventidFireEvent,EVENT_PARAM(VTS_BSTR  VTS_VARIANT), eventName, &params);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// ディスパッチ と イベント ID
public:
	enum {
	//{{AFX_DISP_ID(CAlCtrlCtrl)
	dispidTransparent = 1L,
	dispidBorderWidth = 2L,
	dispidLightColor = 3L,
	dispidShadowColor = 4L,
	dispidEventName = 5L,
	dispidGetWnd = 6L,
	dispidInvoke = 7L,
	dispidGetRootClass = 8L,
	dispidSetRootClass = 9L,
	dispidSaveBody = 10L,
	dispidLoadBody = 11L,
	dispidLoadProgram = 12L,
	dispidArg = 13L,
	dispidParam = 14L,
	dispidDebugMode = 15L,
	dispidSetLibraryPath = 16L,
	dispidInetProgram = 17L,
	dispidInetLibrary = 18L,
	dispidInetBody = 19L,
	dispidEventParam = 20L,
	eventidFireEvent = 1L,
	//}}AFX_DISP_ID
	};
};


