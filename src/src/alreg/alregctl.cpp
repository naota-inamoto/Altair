
// AlRegCtl.cpp : CAlRegCtrl ActiveX コントロール クラスのインプリメンテーション

#include "stdafx.h"
#include "AlReg/AlReg.h"
#include "AlReg/AlRegCtl.h"
#include "AlReg/AlRegPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAlRegCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// メッセージ マップ

BEGIN_MESSAGE_MAP(CAlRegCtrl, COleControl)
	//{{AFX_MSG_MAP(CAlRegCtrl)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ディスパッチ マップ

BEGIN_DISPATCH_MAP(CAlRegCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CAlRegCtrl)
	DISP_FUNCTION(CAlRegCtrl, "SetRegistryKey", SetRegistryKey, VT_I2, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAlRegCtrl, "QueryInt", QueryInt, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlRegCtrl, "QueryString", QueryString, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlRegCtrl, "GetIntValue", GetIntValue, VT_I4, VTS_NONE)
	DISP_FUNCTION(CAlRegCtrl, "GetStringValue", GetStringValue, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CAlRegCtrl, "SetIntValue", SetIntValue, VT_I2, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CAlRegCtrl, "SetStringValue", SetStringValue, VT_I2, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAlRegCtrl, "DeleteKey", DeleteKey, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlRegCtrl, "DeleteValue", DeleteValue, VT_I2, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CAlRegCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// イベント マップ

BEGIN_EVENT_MAP(CAlRegCtrl, COleControl)
	//{{AFX_EVENT_MAP(CAlRegCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// プロパティ ページ

BEGIN_PROPPAGEIDS(CAlRegCtrl, 1)
	PROPPAGEID(CAlRegPropPage::guid)
END_PROPPAGEIDS(CAlRegCtrl)


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリと guid を初期化します

IMPLEMENT_OLECREATE_EX(CAlRegCtrl, "ALREG.AlRegCtrl.1",
	0x22e60107, 0x815, 0x11d2, 0x86, 0xb0, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// タイプ ライブラリ ID とバージョン

IMPLEMENT_OLETYPELIB(CAlRegCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// インターフェイス ID

const IID BASED_CODE IID_DAlReg =
		{ 0x22e60105, 0x815, 0x11d2, { 0x86, 0xb0, 0x44, 0x45, 0x53, 0x54, 0, 0 } };
const IID BASED_CODE IID_DAlRegEvents =
		{ 0x22e60106, 0x815, 0x11d2, { 0x86, 0xb0, 0x44, 0x45, 0x53, 0x54, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// コントロールの型情報の制御

static const DWORD BASED_CODE _dwAlRegOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAlRegCtrl, IDS_ALREG, _dwAlRegOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl::CAlRegCtrlFactory::UpdateRegistry -
// CAlRegCtrl のシステム レジストリのエントリを追加または削除します 

BOOL CAlRegCtrl::CAlRegCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// コントロールがアパートメント モデルのスレッド処理の規則に従っていることを
	// 確認してください。詳細については MFC のテクニカル ノート 64 を参照してください。
	// アパートメント モデルのスレッド処理の規則に従わないコントロールの場合は、6 番目
	// のパラメータを以下のように変更してください。
	// afxRegInsertable | afxRegApartmentThreading を afxRegInsertable へ変更してください。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ALREG,
			IDB_ALREG,
			afxRegInsertable | afxRegApartmentThreading,
			_dwAlRegOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl::CAlRegCtrl - コンストラクタ

CAlRegCtrl::CAlRegCtrl()
{
	InitializeIIDs(&IID_DAlReg, &IID_DAlRegEvents);

	m_hKey = HKEY_CURRENT_USER;
	m_subKey = "Altair";
	m_accessMask = KEY_ALL_ACCESS;

	m_intValue = 0;
	m_stringValue = "";
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl::~CAlRegCtrl - デストラクタ

CAlRegCtrl::~CAlRegCtrl()
{
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl::OnDraw - 描画関数

void CAlRegCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /*rcInvalid*/)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->TextOut(5, 5, "AlReg", 5);
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl::DoPropExchange - 永続性のサポート

void CAlRegCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}


/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl::OnResetState - コントロールのプロパティ値をリセット

void CAlRegCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange を呼び出してデフォルト値にリセット
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl::AboutBox - "バージョン情報" のダイアログ ボックスを表示

void CAlRegCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ALREG);
	dlgAbout.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl 内部関数

HKEY CAlRegCtrl::OpenKey() {
	HKEY hKey = NULL;
	LONG r = RegOpenKeyEx(m_hKey, m_subKey, 0, m_accessMask, &hKey);
	if (r != ERROR_SUCCESS)
		return NULL;
	return hKey;
}

HKEY CAlRegCtrl::CreateKey() {
	HKEY hKey = NULL;
	DWORD result;
	LONG r = RegCreateKeyEx(m_hKey, m_subKey, 0,
		NULL, REG_OPTION_NON_VOLATILE, m_accessMask, NULL, &hKey, &result);
	if (r != ERROR_SUCCESS)
		return NULL;
	return hKey;
}


/////////////////////////////////////////////////////////////////////////////
// CAlRegCtrl メッセージ ハンドラ


short CAlRegCtrl::SetRegistryKey(LPCTSTR hKey, LPCTSTR subKey, LPCTSTR accessMask) 
{
	HKEY lastKey = m_hKey;
	CString lastSubKey = m_subKey;

	if (strcmp(hKey, "HKEY_CLASSES_ROOT") == 0) {
		m_hKey = HKEY_CLASSES_ROOT;
	} else if (strcmp(hKey, "HKEY_CURRENT_USER") == 0) {
		m_hKey = HKEY_CURRENT_USER;
	} else if (strcmp(hKey, "HKEY_LOCAL_MACHINE") == 0) {
		m_hKey = HKEY_LOCAL_MACHINE;
	} else if (strcmp(hKey, "HKEY_USERS") == 0) {
		m_hKey = HKEY_USERS;
	} else {
		return -1;
	}
	m_subKey = subKey;
	if (strcmp(accessMask, "KEY_ALL_ACCESS") == 0) {
		m_accessMask = KEY_ALL_ACCESS;
	} else if (strcmp(accessMask, "KEY_READ") == 0) {
		m_accessMask = KEY_READ;
	} else if (strcmp(accessMask, "KEY_WRITE") == 0) {
		m_accessMask = KEY_WRITE;
	} else {
		m_hKey = lastKey;
		m_subKey = lastSubKey;
		return -2;
	}
	return 0;
}

short CAlRegCtrl::QueryInt(LPCTSTR name) 
{
	HKEY hKey = OpenKey();
	if (hKey == NULL)
		return -1;
	DWORD	type = REG_DWORD;		// 種類 
	DWORD	data;					// バッファ 
	DWORD	size = sizeof(DWORD);	// バッファサイズ	
	LONG r = RegQueryValueEx(hKey, name, 0, &type, (LPBYTE)&data, &size);
	RegCloseKey(hKey);
	if (r != ERROR_SUCCESS)
		return -2;
	if (type != REG_DWORD || size != sizeof(DWORD))
		return -3;
	m_intValue = data;
	return 0;
}

short CAlRegCtrl::QueryString(LPCTSTR name) 
{
	HKEY hKey = OpenKey();
	if (hKey == NULL)
		return -1;
	DWORD	type = REG_SZ;	// 種類 
	char*	data;			// バッファ 
	DWORD	size;			// バッファサイズ	
	LONG r = RegQueryValueEx(hKey, name, 0, &type, (LPBYTE)NULL, &size);
	if (r != ERROR_SUCCESS || size < 0)
		return -2;
	if (type != REG_SZ && type != REG_EXPAND_SZ)
		return -3;
	data = new char[size];
	r = RegQueryValueEx(hKey, name, 0, &type, (LPBYTE)data, &size);
	RegCloseKey(hKey);
	if (r != ERROR_SUCCESS)
		return -4;
	m_stringValue = data;
	delete [] data;
	return 0;
}

long CAlRegCtrl::GetIntValue() 
{
	return m_intValue;
}

BSTR CAlRegCtrl::GetStringValue() 
{
	CString strResult;
	strResult = m_stringValue;
	return strResult.AllocSysString();
}

short CAlRegCtrl::SetIntValue(LPCTSTR name, long value) 
{
	HKEY hKey = CreateKey();
	if (hKey == NULL)
		return -1;
	LONG r = RegSetValueEx(hKey, name, 0,
		REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
	RegCloseKey(hKey);
	if (r != ERROR_SUCCESS)
		return -2;
	return 0;
}

short CAlRegCtrl::SetStringValue(LPCTSTR name, LPCTSTR value) 
{
	HKEY hKey = CreateKey();
	if (hKey == NULL)
		return -1;
	LONG r = RegSetValueEx(hKey, name, 0,
		REG_SZ, (LPBYTE)value, strlen(value)+1);
	RegCloseKey(hKey);
	if (r != ERROR_SUCCESS)
		return -2;
	return 0;
}

short CAlRegCtrl::DeleteKey(LPCTSTR name) 
{
	HKEY hKey = OpenKey();
	if (hKey == NULL)
		return -1;
	LONG r = RegDeleteKey(hKey, name);
	RegCloseKey(hKey);
	if (r != ERROR_SUCCESS)
		return -2;
	return 0;
}

short CAlRegCtrl::DeleteValue(LPCTSTR name) 
{
	HKEY hKey = OpenKey();
	if (hKey == NULL)
		return -1;
	LONG r = RegDeleteValue(hKey, name);
	RegCloseKey(hKey);
	if (r != ERROR_SUCCESS)
		return -2;
	return 0;
}
