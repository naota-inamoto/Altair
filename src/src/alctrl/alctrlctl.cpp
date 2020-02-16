
// AlCtrlCtl.cpp : CAlCtrlCtrl OLE コントロール クラスのインプリメンテーション

#include "stdafx.h"

#include "AlCtrl/AlCtrl.h"
#include "AlCtrl/AlCtrlCtl.h"
#include "AlCtrl/AlCtrlPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAlCtrlCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// メッセージ マップ

BEGIN_MESSAGE_MAP(CAlCtrlCtrl, COleControl)
	//{{AFX_MSG_MAP(CAlCtrlCtrl)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ディスパッチ マップ

BEGIN_DISPATCH_MAP(CAlCtrlCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CAlCtrlCtrl)
	DISP_PROPERTY_NOTIFY(CAlCtrlCtrl, "Transparent", m_transparent, OnTransparentChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CAlCtrlCtrl, "BorderWidth", m_borderWidth, OnBorderWidthChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CAlCtrlCtrl, "LightColor", m_lightColor, OnLightColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CAlCtrlCtrl, "ShadowColor", m_shadowColor, OnShadowColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CAlCtrlCtrl, "EventName", m_eventName, OnEventNameChanged, VT_BSTR)
	DISP_FUNCTION(CAlCtrlCtrl, "GetWnd", GetWnd, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CAlCtrlCtrl, "Invoke", Invoke, VT_VARIANT, VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CAlCtrlCtrl, "GetRootClass", GetRootClass, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CAlCtrlCtrl, "SetRootClass", SetRootClass, VT_I2, VTS_VARIANT)
	DISP_FUNCTION(CAlCtrlCtrl, "SaveBody", SaveBody, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlCtrlCtrl, "LoadBody", LoadBody, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlCtrlCtrl, "LoadProgram", LoadProgram, VT_I2, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAlCtrlCtrl, "Arg", Arg, VT_VARIANT, VTS_BSTR VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CAlCtrlCtrl, "Param", Param, VT_VARIANT, VTS_VARIANT VTS_I2)
	DISP_FUNCTION(CAlCtrlCtrl, "DebugMode", DebugMode, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CAlCtrlCtrl, "SetLibraryPath", SetLibraryPath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CAlCtrlCtrl, "InetProgram", InetProgram, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlCtrlCtrl, "InetLibrary", InetLibrary, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlCtrlCtrl, "InetBody", InetBody, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlCtrlCtrl, "EventParam", EventParam, VT_VARIANT, VTS_I2)
	DISP_FUNCTION(CAlCtrlCtrl, "EmptyArg", EmptyArg, VT_VARIANT, VTS_NONE)
	DISP_STOCKFUNC_REFRESH()
	DISP_PROPERTY_NOTIFY_ID(CAlCtrlCtrl, "BackColor", DISPID_BACKCOLOR, m_backColor, OnBackColorChanged, VT_COLOR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CAlCtrlCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// イベント マップ

BEGIN_EVENT_MAP(CAlCtrlCtrl, COleControl)
	//{{AFX_EVENT_MAP(CAlCtrlCtrl)
	EVENT_CUSTOM("FireEvent", FireCustomEvent, VTS_BSTR  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// プロパティ ページ

BEGIN_PROPPAGEIDS(CAlCtrlCtrl, 2)
	PROPPAGEID(CAlCtrlPropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
END_PROPPAGEIDS(CAlCtrlCtrl)


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリと guid を初期化します

IMPLEMENT_OLECREATE_EX(CAlCtrlCtrl, "ALCTRL.AlCtrlCtrl.1",
	0xcbe7bc03, 0x1f9e, 0x11d0, 0xb3, 0x4e, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// タイプ ライブラリ ID とバージョン

IMPLEMENT_OLETYPELIB(CAlCtrlCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// インターフェイス ID

const IID BASED_CODE IID_DAlCtrl =
		{ 0xcbe7bc01, 0x1f9e, 0x11d0, { 0xb3, 0x4e, 0x44, 0x45, 0x53, 0x54, 0, 0 } };
const IID BASED_CODE IID_DAlCtrlEvents =
		{ 0xcbe7bc02, 0x1f9e, 0x11d0, { 0xb3, 0x4e, 0x44, 0x45, 0x53, 0x54, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// タイプ情報の制御

static const DWORD BASED_CODE _dwAlCtrlOleMisc =
	OLEMISC_SIMPLEFRAME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAlCtrlCtrl, IDS_ALCTRL, _dwAlCtrlOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl::CAlCtrlCtrlFactory::UpdateRegistry -
// CAlCtrlCtrl のシステム レジストリのエントリを追加または削除します 

BOOL CAlCtrlCtrl::CAlCtrlCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ALCTRL,
			IDB_ALCTRL,
			TRUE,						//	挿入可能
			_dwAlCtrlOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl::CAlCtrlCtrl - コンストラクタ

CAlCtrlCtrl::CAlCtrlCtrl()
{
	InitializeIIDs(&IID_DAlCtrl, &IID_DAlCtrlEvents);

	EnableSimpleFrame();

	m_eventParams = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl::~CAlCtrlCtrl - デストラクタ

CAlCtrlCtrl::~CAlCtrlCtrl()
{
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl::OnDraw - 描画関数

void CAlCtrlCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (m_transparent == FALSE) {
		CBrush back(TranslateColor(m_backColor));
		pdc->FillRect(rcBounds, &back);
	}

	ADvCOleControl::OnDraw(pdc, rcBounds, rcInvalid);

	if (m_transparent == FALSE && m_borderWidth > 0) {
		POINT pts[4];

		CBrush light_brush(TranslateColor(m_lightColor));
		CPen light_pen(PS_SOLID, 0, TranslateColor(m_lightColor));
		CBrush shadow_brush(TranslateColor(m_shadowColor));
		CPen shadow_pen(PS_SOLID, 0, TranslateColor(m_shadowColor));

		CBrush *last_brush = pdc->SelectObject(&light_brush);
		CPen *last_pen = pdc->SelectObject(&light_pen);

		pts[0].x = rcBounds.left;
		pts[0].y = rcBounds.top;
		pts[1].x = rcBounds.right  -1;
		pts[1].y = rcBounds.top;
		pts[2].x = rcBounds.right  -1 -m_borderWidth +1;
		pts[2].y = rcBounds.top 	  +m_borderWidth -1;
		pts[3].x = rcBounds.left	  +m_borderWidth -1;
		pts[3].y = rcBounds.top 	  +m_borderWidth -1;
		pdc->Polygon(pts, 4);

		pts[0].x = rcBounds.left;
		pts[0].y = rcBounds.top;
		pts[1].x = rcBounds.left	  +m_borderWidth -1;
		pts[2].y = rcBounds.top 	  +m_borderWidth -1;
		pts[2].x = rcBounds.left	  +m_borderWidth -1;
		pts[2].x = rcBounds.left	  +m_borderWidth -1;
		pts[2].y = rcBounds.bottom -1 -m_borderWidth +1;
		pts[3].x = rcBounds.left;
		pts[3].y = rcBounds.bottom -1;
		pdc->Polygon(pts, 4);

		pdc->SelectObject(&shadow_brush);
		pdc->SelectObject(&shadow_pen);

		pts[0].x = rcBounds.right  -1;
		pts[0].y = rcBounds.bottom -1;
		pts[1].x = rcBounds.left;
		pts[1].y = rcBounds.bottom -1;
		pts[2].x = rcBounds.left	  +m_borderWidth -1;
		pts[2].y = rcBounds.bottom -1 -m_borderWidth +1;
		pts[3].x = rcBounds.right  -1 -m_borderWidth +1;
		pts[3].y = rcBounds.bottom -1 -m_borderWidth +1;
		pdc->Polygon(pts, 4);

		pts[0].x = rcBounds.right  -1;
		pts[0].y = rcBounds.bottom -1;
		pts[1].x = rcBounds.right  -1 -m_borderWidth +1;
		pts[1].y = rcBounds.bottom -1 -m_borderWidth +1;
		pts[2].x = rcBounds.right  -1 -m_borderWidth +1;
		pts[2].y = rcBounds.top 	  +m_borderWidth -1;
		pts[3].x = rcBounds.right  -1;
		pts[3].y = rcBounds.top;
		pdc->Polygon(pts, 4);

		pdc->SelectObject(last_brush);
		pdc->SelectObject(last_pen);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl::DoPropExchange - 永続性のサポート

void CAlCtrlCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	PX_Bool(pPX, "Transparent", m_transparent);
	PX_Color(pPX, "BackColor", m_backColor);
	PX_Short(pPX, "BorderWidth", m_borderWidth);
	PX_Color(pPX, "LightColor", m_lightColor);
	PX_Color(pPX, "ShadowColor", m_shadowColor);
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl::OnResetState - コントロールのプロパティ値をリセット

void CAlCtrlCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange を呼び出してデフォルト値にリセット

	m_transparent = FALSE;
	m_backColor = ADV_BG_COLOR;
	m_borderWidth = 1;
	m_lightColor = ADV_FG_COLOR;
	m_shadowColor = ADV_FG_COLOR;
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl::AboutBox - "バージョン情報" のダイアログ ボックスを表示

void CAlCtrlCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ALCTRL);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CAlCtrlCtrl メッセージ ハンドラ

void CAlCtrlCtrl::CustomEvent(AGmNode* evnt, AGmNode* params) {
	VARIANT v;
	VariantInit(&v);
	IAGmNode *pNode = AGmNodeToVariant(params, &v);
	
	m_eventName = evnt->a.s.v->Str();
	m_eventParams = params;

	FireCustomEvent(evnt->a.s.v->Str(), v);

	m_eventName = "";
	m_eventParams = NULL;

	pNode->Release();
}

VARIANT CAlCtrlCtrl::Param(const VARIANT FAR& params, short index) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	AGmNode *p = VariantToAGmNode(&params, "Param", "params");
	for (; p && index > 1; p = p->T, index--) {
		if (!AGm::IsList(p))
			AfxThrowOleDispatchException(1, "Param: params is not list");
	}
	if (p == NULL)
		AfxThrowOleDispatchException(1, "Param: index out of range");
	COleVariant *var = ADvOcx::ToVariant(p->H);
	VariantCopy(&vaResult, var);
	delete var;

	return vaResult;
}

void CAlCtrlCtrl::OnTransparentChanged() 
{
	Refresh();
	SetModifiedFlag();
}

void CAlCtrlCtrl::OnBackColorChanged() 
{
	Refresh();
	SetModifiedFlag();
}

void CAlCtrlCtrl::OnBorderWidthChanged() 
{
	Refresh();
	SetModifiedFlag();
}

void CAlCtrlCtrl::OnLightColorChanged() 
{
	Refresh();
	SetModifiedFlag();
}

void CAlCtrlCtrl::OnShadowColorChanged() 
{
	Refresh();
	SetModifiedFlag();
}

VARIANT CAlCtrlCtrl::GetWnd() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	void *value;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, value);
	ASSERT(value);
	AGmNodeToVariant((AGmNode*)value, &vaResult);
	return vaResult;
}

short CAlCtrlCtrl::SetRootClass(const VARIANT FAR& rootClass) 
{
	AGmNode *n = VariantToAGmNode(&rootClass, "Param", "params");
	AGm::Set(_root_class, n);
	return 0;
}

VARIANT CAlCtrlCtrl::GetRootClass() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	AGmNodeToVariant(_root_class, &vaResult);
	return vaResult;
}

int CAlCtrlCtrl::SetMethod(AGmNode* method, AGmNode* obj, AGmNode* func) {
	AGm::CreateArc(_method_tbl, AGm::List(obj, func), AGm::UniqueStr(method));
	return 0;
}

VARIANT CAlCtrlCtrl::Invoke(LPCTSTR methodName, const VARIANT FAR& args) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	AGmNode* name = AGm::UniqueStr(methodName);
	AGmNode* obj_func = AGm::dst_node2(_method_tbl, name);
	if (!obj_func) {
		char err[256];
		art_sprintf_1(err, 256, "Invoke: \"%s\" not found in method_tbl", methodName);
		AfxThrowOleDispatchException(1, err);
	}
	AGmNode *obj = obj_func->H;
	ASSERT(obj);
	AGmNode *func_dcl = obj_func->TH;
	ASSERT(func_dcl);
	AGmNode *vtbl = AGm::dst_node2(func_dcl, AGm::NmVtbl());
	ASSERT(vtbl);
	AGmNode *class_id = obj->a.o.class_id;
	ASSERT(class_id);
	AGmNode *func = AGm::dst_node2(vtbl, class_id);
	if (func == NULL) {
		char err[256];
		art_sprintf_2(err, 256, "Invoke: \"%s::%s\" not found in vtbl\n",
			class_id->H->a.s.v->Str(), methodName);
		AfxThrowOleDispatchException(1, err);
	}
	AGmNode *_args = VariantToAGmNode(&args, "Invoke", "args");
	AGmNode *n = EvalCallback(_root_class, _args, obj, func);
	AGmNode *n2 = AGm::Ref(n->H);
	AGm::Unref(n);
	AGmNodeToVariant(n2, &vaResult);
	if (n2) n2->count --;
	return vaResult;
}

VARIANT CAlCtrlCtrl::Arg(LPCTSTR name, const VARIANT FAR& value, const VARIANT FAR& args) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	AGmNode *_args = VariantToAGmNode(&args, "Arg", "args");
	AGmNode *val = ADvOcx::FromVariant(value);
	AGmNode *u_name = AGm::UniqueStr(name);
	AGmNode *n = AGm::Cons(AGm::List(u_name, val, NULL), _args);
	if (_args) _args->count --;

	AGmNodeToVariant(n, &vaResult);
	return vaResult;
}

short CAlCtrlCtrl::LoadProgram(LPCTSTR dir, LPCTSTR name) 
{
	ADvWindow *aw = _ole_wnd;
	void *v;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, v);
	AGmNode *wnd = (AGmNode*)v;
	ASSERT(wnd);
	ASSERT(wnd->a.w.v == aw);

	ADvBeginWaitCursor();
	AGmString *d = new AGmString(dir);
	AGmString *n = new AGmString(name);
	AGmLink *l = AUiProjectShell::SearchProject(d, n);
	if (l) {
		ADvEndWaitCursor();
		AGm::Set(_root_class, l->arc->dst->H);
		delete d; delete n; return 0; // Already Loaded
	}
	AGmString *path = new AGmString(d); path->Append("/"); path->Append(n);
	if (AGmFile::DoesExist(path->Str()) == 0) {
		ADvEndWaitCursor();
		delete path;
		delete d; delete n; return 0x01; // File Not Found
	}
	AGmNode *root = AUiProgramMgr::CreateCommonClasses();
	AGmNode *lib_list = AGm::Cons(NULL);
	AGmNode *class_list = AGm::Cons(NULL);
	AGmNode *proj = AUiProjectShell::AddProject(d, n, root, lib_list, class_list);
	delete d; delete n;
	if (AUiProgramMgr::__Open(path, proj)) {
		AUiProjectShell::RemoveProject(proj);
		ADvEndWaitCursor();
		delete path; return 0x02; // Fail to Read File
	}
	delete path;
	AGm::MakeStrUnique(root);
	AGm::Set(_root_class, root);
	AUiMemberShell::CleanVtbl(_root_class);
	AUiMemberShell::UpdateVtbl(_root_class, _root_class);
	ADvEndWaitCursor();

	if (AIpEvalClassFunc(_root_class,
			AGm::UniqueStr("GUI"), AGm::UniqueStr("Initialize"))) {
		return 0x20; // Fail to GUI::Initialize();
	}

	return 0;
}

short CAlCtrlCtrl::SaveBody(LPCTSTR fileName) 
{
	ADvWindow *aw = _ole_wnd;
	void *v;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, v);
	AGmNode *wnd = (AGmNode*)v;
	ASSERT(wnd);
	ASSERT(wnd->a.w.v == aw);

	// RootGlyph root;
	AGmNode **value;
	if (AIpProcess::SearchVar(aw->Arg(), AGm::UniqueStr("root"), value))
		return 0x10; // ルートGlyphがない
	AGmNode *data = AGm::Ref(AGm::List(*value, _method_tbl));
	AGmFile *f = new AGmFile();
	if (f->Create(fileName, AGM_FILE_WRITE)) {
		delete f; return 0x01; // ファイルがオープンできない	
	}
	if (f->WriteGraph(data)) {
		AGm::Unref(data); delete f; return 0x02; // ファイル書き込みに失敗
	}
	AGm::Unref(data);

	// _method_tbl書き込み
	AGmLink *l;
	for (l = AGm::next_outlink(_method_tbl, NULL);
			l; l = AGm::next_outlink(_method_tbl, l)) {
		AGmNode *func_dcl = l->arc->dst->TH;
		AGmNode *_class = AGm::src_node3(func_dcl, AGm::NmVtbl());
		ASSERT(_class);
		AGmLink *lnk = AGm::find_outlink(_class, func_dcl);
		ASSERT(lnk);
		if (f->WriteString(l->arc->attr->a.s.v->Str()) ||
				f->WriteString(" ") ||
				f->WriteString(_class->H->a.s.v->Str()) ||
				f->WriteString(" ") ||
				f->WriteString(lnk->arc->attr->a.s.v->Str()) ||
				f->WriteString("\n")) {
			delete f; return 0x03; // DISKがいっぱい
		}
	}
	if (f->WriteString("$END_METHOD_TBL\n")) {
		delete f; return 0x03; // DISKがいっぱい
	}

	delete f; return 0;
}

short CAlCtrlCtrl::LoadBody(LPCTSTR fileName) {
	AGmString *str = new AGmString(fileName);
	short r = LoadBody1(str, 0);
	delete str;
	return r;
}

short CAlCtrlCtrl::LoadBody1(AGmString *str, int inet) 
{
	if (_root_class == NULL) {
		AfxThrowOleDispatchException(1, "LoadBody: RootClass is null");
	}

	ADvWindow *aw = _ole_wnd;
	void *v;
	AGm::GetValue(this, AGM_VM_KEY_WINDOW_NODE, v);
	AGmNode *wnd = (AGmNode*)v;
	ASSERT(wnd);
	ASSERT(wnd->a.w.v == aw);

	AGmFile *f = new AGmFile();
	if (inet == 0) {
		if (f->Create(str, AGM_FILE_READ)) {
			delete f; return 0x01; // ファイルのオープンに失敗
		}
	} else {
		if (f->Create(str, AGM_FILE_STR_READ)) {
			delete f; return 0x01; // ファイルのオープンに失敗
		}
	}

	AGmNode *data;
	if (f->ReadGraph(data)) {
		delete f; return 0x02; // ファイルの読み込みに失敗
	}
	AGm::Ref(data);
	AGm::MakeStrUnique(data);
	if (AUiClassShell::LinkClassObj(_root_class, data)) {
		delete f; AGm::Unref(data); return 0x03; // オブジェクトのクラスがない
	}
	AGmNode **value;
	// ルートGlyphのセット
	if (AIpProcess::SearchVar(aw->Arg(), AGm::UniqueStr("root"), value)) {
		AGmNode *w_arg = aw->Arg();
		w_arg = AGm::Cons(AGm::List(AGm::UniqueStr("root"), data->H, NULL), w_arg);
		aw->Arg(w_arg);
	} else {
		AGm::Set(*value, data->H);
	}
	// ルートGlyphの関数引数のウィンドウ wのセット;
	if (AIpProcess::SearchVar(aw->Arg(), AGm::UniqueStr("w"), value)) {
		AGmNode *w_arg = aw->Arg();
		w_arg = AGm::Cons(AGm::List(AGm::UniqueStr("w"), (AGmNode*)wnd, NULL), w_arg);
		aw->Arg(w_arg);
	} else {
		AGm::Set(*value, (AGmNode*)wnd);
	}
	// ルートGlyphのメンバー変数 wndのセット
	AGm::set_dst_node(data->H, AGm::UniqueStr("wnd"), wnd);

	AGm::Set(_method_tbl, data->TH);
	AGm::Unref(data);

	// _method_tbl読み込み
	AGmString *s;
	for (;;) {
		if (f->ReadString(s)) {
			delete f; return 0x10; // メソッドTBL読み込みでシンボルがない
		}
		if (s->Compare("$END_METHOD_TBL") == 0) {
			delete s; break;
		}
		AGmNode *name = AGm::UniqueStr(s);
		delete s;
		if (f->ReadIdent(s)) {
			delete f; return 0x10; // シンボルでない
		}
		AGmNode *_class = AUiClassShell::NameToClass(_root_class, s);
		delete s;
		if (_class == NULL) {
			delete f; return 0x11; // クラスが見つからない
		}
		if (f->ReadIdent(s)) {
			delete f; return 0x10; // シンボルでない
		}
		AGmNode *func = AGm::UniqueStr(s);
		delete s;
		AGmNode *func_dcl = AGm::dst_node2(_class, func);
		if (func_dcl == NULL) {
			delete f; return 0x12; // メンバーメソッドが見つからない
		}
		AGmNode *elem = AGm::dst_node2(_method_tbl, name);
		if (elem) {
			AGm::Set(elem->TH, func_dcl);
		} else {
			delete f; return 0x13; // 指定されたメソッドがメソッドTBLにない
		}
	}
	delete f;

	AGmNode *w_class = AUiClassShell::NameToClass(_root_class, "Window");
	if (w_class == NULL) return 0x20; // クラスWindowがない
	AGmNode *func_dcl;

	int protect;

	func_dcl = AUiBodyShell::find_class_func(
		w_class, w_class, AGm::UniqueStr("DrawProc"), NULL, protect, 1, 1);
	if (func_dcl == NULL) return 0x21; // メンバーWindow::DrawProcがない
	aw->DrawProg(func_dcl);

	func_dcl = AUiBodyShell::find_class_func(
		w_class, w_class, AGm::UniqueStr("EventProc"), NULL, protect, 1, 1);
	if (func_dcl == NULL) return 0x22; // メンバーWindow::EventProcがない
	aw->EventProg(func_dcl);
		
	CRect r;
	GetClientRect(&r);
	PostMessage(WM_SIZE, SIZE_RESTORED,
		MAKELPARAM((WORD)r.Width(), (WORD)r.Height()));
	InvalidateRect(NULL);

	return 0;
}

void CAlCtrlCtrl::DebugMode(short mode) 
{
	if (mode) {
		AGm::SetValue(NULL, AGM_VM_KEY_OCX_DEBUG_MODE, (void*)1);
	} else {
		void *value;
		AGm::GetValue(NULL, AGM_VM_KEY_OCX_DEBUG_MODE, value);
		if (value)
			AGm::RemoveValue(NULL, AGM_VM_KEY_OCX_DEBUG_MODE);
	}
}

void CAlCtrlCtrl::SetLibraryPath(LPCTSTR path) 
{
	AUiProgramMgr *pm = AUiProgramMgr::Instance();
	if (pm == NULL)
		return;
	AGmNode *list = pm->LibSearchPath();
	if (list == NULL)
		return;
	AGm::CreateArc(list, AGm::String(path));
}

VARIANT CAlCtrlCtrl::EmptyArg() {
	VARIANT vaResult;
	VariantInit(&vaResult);

	AGmNodeToVariant(NULL, &vaResult);
	return vaResult;
}

// -------- Inet --------

int CAlCtrlCtrl::CheckURL(LPCSTR url,
		DWORD &service, CString &server, CString &object, INTERNET_PORT &port,
		CString &name) {
	if (!AfxParseURL(url, service, server, object, port)) return 1;
	AGmString *s1 = new AGmString(url);
	long i = 0, j = 0;
	for (;;) {
		j = s1->Search("/", i+1);
		if (j < 0) break;
		i = j;
	}
	AGmString *s2 = s1->SubString(i+1, s1->Length());
	name = s2->Str();
	delete s2; delete s1;
	return 0;
}

const char *type[] = {
	"text/*, application/*",
	NULL
};
AGmString *CAlCtrlCtrl::Download(
		DWORD service, CString server, CString object, INTERNET_PORT port) {

	if (service != AFX_INET_SERVICE_HTTP && service != AFX_INET_SERVICE_HTTPS) {
		ADvWindow::Print("not support protocol\n");
		return NULL;
	}

	CInternetSession *session = new CInternetSession();
	CHttpConnection *conn;
	CHttpFile *file;
	try {
		conn = session->GetHttpConnection(server, port);
		if (conn == NULL) {
			ADvWindow::Print("fail to connect\n");
			delete session; return NULL;
		}
	} catch (CInternetException *e) {
		ADvWindow::Print("fail to connect\n");
		e->Delete();
		delete session; return NULL;
	}
	int nVerb = CHttpConnection::HTTP_VERB_GET;
	DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT;
	if (service == AFX_INET_SERVICE_HTTPS)
		dwFlags |= INTERNET_FLAG_SECURE;
	file = conn->OpenRequest(
		nVerb, object, NULL, 1, type, NULL, dwFlags);
	if (file == NULL) {
		ADvWindow::Print("fail to OpenRequest\n");
		conn->Close(); delete conn;
		delete session; return NULL;
	}
	file->AddRequestHeaders("User-Agent: Altair OLE Control for Internet");
	try {
		if (!file->SendRequest()) {
			ADvWindow::Print("fail to SendRequest\n");
			file->Close(); delete file;
			conn->Close(); delete conn;
			delete session; return NULL;
		}
	} catch (CInternetException *e) {
		ADvWindow::Print("fail to SendRequest\n");
		e->Delete();
		file->Close(); delete file;
		conn->Close(); delete conn;
		delete session; return NULL;
	}
	char buf[1025];
	AGmString *str = new AGmString(16);
	for (;;) {
		int cc = file->Read(buf, 1023);
		if (cc == 0) break;
		buf[cc] = 0;
		str->Append(buf);
	}
	file->Close(); delete file;
	conn->Close(); delete conn;
	delete session; return str;
}

int CAlCtrlCtrl::InitRootClass(LPCSTR name, AGmNode *&proj) {
	AGmString *d = new AGmString("AlCtrl");
	AGmString *n;
	if (name == NULL) {
		n = new AGmString("Inet");
	} else {
		n = new AGmString(name);
	}
	AGmLink *l = AUiProjectShell::SearchProject(d, n);
	if (l) {
		proj = l->arc->dst;
		AGm::Set(_root_class, l->arc->dst->H);
		delete d; delete n;
		return 1;
	}
	AGmNode *root = AUiProgramMgr::CreateCommonClasses();
	AGmNode *lib_list = AGm::Cons(NULL);
	AGmNode *class_list = AGm::Cons(NULL);
	proj = AUiProjectShell::AddProject(d, n, root, lib_list, class_list);
	AGm::Set(_root_class, proj->H);
	delete d; delete n;
	return 0;
}

short CAlCtrlCtrl::InetProgram(LPCTSTR url) 
{
	DWORD service; CString server, object; INTERNET_PORT port; CString name;
	if (CheckURL(url, service, server, object, port, name)) return 0x101;
	AGmNode *proj;
	if (InitRootClass(url, proj))
		return 0;
	char msg[256];
	art_sprintf_1(msg, 256, "download '%s' ...\n", url);
	ADvWindow::Print(msg);
	AGmString *str = Download(service, server, object, port);
	if (str == NULL) return 0x102;

	ADvWindow::Print("loading program ...\n");
	if (AUiProgramMgr::__Open(str, proj, 1)) {
		AUiProjectShell::RemoveProject(proj);
		delete str; return 0x103;
	}
	delete str;
	AUiMemberShell::CleanVtbl(_root_class);
	AUiMemberShell::UpdateVtbl(_root_class, _root_class);

	if (AIpEvalClassFunc(_root_class,
			AGm::UniqueStr("GUI"), AGm::UniqueStr("Initialize"))) {
		return 0x104;
	}

	return 0;
}

short CAlCtrlCtrl::InetLibrary(LPCTSTR url) 
{
	DWORD service; CString server, object; INTERNET_PORT port; CString name;
	if (CheckURL(url, service, server, object, port, name)) return 0x101;
	AGmNode *proj;
	if (InitRootClass(url, proj))
		return 0;
	char msg[256];
	art_sprintf_1(msg, 256, "download '%s' ...\n", url);
	ADvWindow::Print(msg);
	AGmString *str = Download(service, server, object, port);
	if (str == NULL) return 0x102;

	AUiProgramMgr *pgm = AUiProgramMgr::Instance();
	AGmLink *l; char err[256];
	for (l = AGm::next_outlink(pgm->LibList(), NULL);
			l; l = AGm::next_outlink(pgm->LibList(), l)) {
		if (l->arc->attr->H->a.s.v->CompareI(url) == 0) {
			AGmNode *base_class = AUiClassShell::NameToClass(_root_class, l->arc->attr->TH->a.s.v);
			if (base_class == NULL) {
				art_sprintf_1(err, 256, AL_S115, l->arc->attr->TH->a.s.v->Str());
				ADvWindow::Print(err);
				return 0x103;
			}
			if (AGm::find_outlink(base_class, l->arc->dst) == NULL)
				AGm::CreateArc(base_class, l->arc->dst, AGm::NmSubClass());
			return 0;
		}
	}
	ADvWindow::Print("loading library ...\n");
	AGmFile *f = new AGmFile;
	if (f->Create(str, AGM_FILE_STR_READ)) {
		art_sprintf_1(err, 256, AL_S104, url);
		ADvWindow::Print(err);
		delete f; delete str; return 0x104;
	}
	AGmNode *lib = NULL;
	if (AUiProgramMgr::_LoadLib(f, NULL, _root_class, &lib)) {
		delete f; delete str; return 0x105;
	}
	delete str;
	ASSERT(lib);
	AGmNode *base_class = AGm::src_node(lib, AGm::NmSubClass());
	ASSERT(base_class);
	AGm::CreateArc(pgm->LibList(), lib, AGm::List(AGm::String(name), base_class->H));
	delete f;

	return 0;
}

short CAlCtrlCtrl::InetBody(LPCTSTR url) 
{
	if (_root_class == NULL)
		return 0x100;

	DWORD service; CString server, object; INTERNET_PORT port; CString name;
	if (CheckURL(url, service, server, object, port, name)) return 0x101;
	char msg[256];
	art_sprintf_1(msg, 256, "download '%s' ...\n", url);
	ADvWindow::Print(msg);
	AGmString *str = Download(service, server, object, port);
	if (str == NULL) return 0x102;

	ADvWindow::Print("loading body data ...\n");
	short r = LoadBody1(str, 1);
	delete str;
	return r;
}

void CAlCtrlCtrl::OnEventNameChanged() 
{
	SetModifiedFlag();
}

VARIANT CAlCtrlCtrl::EventParam(short index) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	AGmNode *p = m_eventParams;
	for (; p && index > 1; p = p->T, index--) {
		if (!AGm::IsList(p))
			AfxThrowOleDispatchException(1, "Param: params is not list");
	}
	if (p == NULL)
		AfxThrowOleDispatchException(1, "Param: index out of range");
	COleVariant *var = ADvOcx::ToVariant(p->H);
	VariantCopy(&vaResult, var);
	delete var;

	return vaResult;
}
