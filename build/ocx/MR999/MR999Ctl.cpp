// MR999Ctl.cpp : CMR999Ctrl ActiveX コントロール クラスのインプリメンテーション

#include "stdafx.h"
#include "MR999.h"
#include "MR999Ctl.h"
#include "MR999Ppg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMR999Ctrl, COleControl)

void TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

/////////////////////////////////////////////////////////////////////////////
// メッセージ マップ

BEGIN_MESSAGE_MAP(CMR999Ctrl, COleControl)
	//{{AFX_MSG_MAP(CMR999Ctrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ディスパッチ マップ

BEGIN_DISPATCH_MAP(CMR999Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CMR999Ctrl)
	DISP_PROPERTY_NOTIFY(CMR999Ctrl, "PortName", m_portName, OnPortNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMR999Ctrl, "TimerPeriod", m_timerPeriod, OnTimerPeriodChanged, VT_I2)
	DISP_FUNCTION(CMR999Ctrl, "Start", Start, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "Stop", Stop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "GrpClose", GrpClose, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "GrpOpen", GrpOpen, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "GrpStop", GrpStop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "WstLeft", WstLeft, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "WstRight", WstRight, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "WstStop", WstStop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "ElbDown", ElbDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "ElbUp", ElbUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "ElbStop", ElbStop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "SldDown", SldDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "SldUp", SldUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "SldStop", SldStop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "BaseCw", BaseCw, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "BaseCcw", BaseCcw, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMR999Ctrl, "BaseStop", BaseStop, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMR999Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// イベント マップ

BEGIN_EVENT_MAP(CMR999Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CMR999Ctrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// プロパティ ページ

BEGIN_PROPPAGEIDS(CMR999Ctrl, 1)
	PROPPAGEID(CMR999PropPage::guid)
END_PROPPAGEIDS(CMR999Ctrl)


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリと guid を初期化します

IMPLEMENT_OLECREATE_EX(CMR999Ctrl, "MR999.MR999Ctrl.1",
	0x2eb3ea21, 0x1198, 0x47dc, 0x88, 0xbd, 0x1e, 0x3b, 0xf5, 0x32, 0xf9, 0xc2)


/////////////////////////////////////////////////////////////////////////////
// タイプ ライブラリ ID とバージョン

IMPLEMENT_OLETYPELIB(CMR999Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// インターフェイス ID

const IID BASED_CODE IID_DMR999 =
		{ 0xbb88f641, 0x8986, 0x4aa6, { 0xab, 0xd4, 0xa7, 0x85, 0x1a, 0xf7, 0x97, 0x79 } };
const IID BASED_CODE IID_DMR999Events =
		{ 0xcd38e4e0, 0x8722, 0x4fb9, { 0x95, 0x96, 0xb5, 0x4d, 0x32, 0xc2, 0x56, 0xa5 } };


/////////////////////////////////////////////////////////////////////////////
// コントロールの型情報の制御

static const DWORD BASED_CODE _dwMR999OleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CMR999Ctrl, IDS_MR999, _dwMR999OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl::CMR999CtrlFactory::UpdateRegistry -
// CMR999Ctrl のシステム レジストリのエントリを追加または削除します 

BOOL CMR999Ctrl::CMR999CtrlFactory::UpdateRegistry(BOOL bRegister)
{

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_MR999,
			IDB_MR999,
			afxRegInsertable | afxRegApartmentThreading,
			_dwMR999OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl::CMR999Ctrl - コンストラクタ

CMR999Ctrl::CMR999Ctrl()
{
	InitializeIIDs(&IID_DMR999, &IID_DMR999Events);

	theCtrl = this;

	m_portName = "COM1";
	m_timerPeriod = 200;

	m_hCom = NULL;
	m_state = 0;

	m_GrpDir      = 0;
	m_GrpDirPrev  = 1;
	m_GrpStop     = 1;
	m_WstDir      = 0;
	m_WstDirPrev  = 1;
	m_WstStop     = 1;
	m_ElbDir      = 0;
	m_ElbDirPrev  = 1;
	m_ElbStop     = 1;
	m_SldDir      = 0;
	m_SldDirPrev  = 1;
	m_SldStop     = 1;
	m_BaseDir     = 0;
	m_BaseDirPrev = 1;
	m_BaseStop    = 1;
}


/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl::~CMR999Ctrl - デストラクタ

CMR999Ctrl::~CMR999Ctrl()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl::OnDraw - 描画関数

void CMR999Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->TextOut(5, 5, "MR-999", 6);
}


/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl::DoPropExchange - 永続性のサポート

void CMR999Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}


/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl::OnResetState - コントロールのプロパティ値をリセット

void CMR999Ctrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange を呼び出してデフォルト値にリセット
}


/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl::AboutBox - "バージョン情報" のダイアログ ボックスを表示

void CMR999Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_MR999);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl メッセージ ハンドラ

void CMR999Ctrl::OnPortNameChanged() 
{
	SetModifiedFlag();
}

void CMR999Ctrl::OnTimerPeriodChanged() 
{
	SetModifiedFlag();
}

BOOL CMR999Ctrl::Start() 
{
	if (m_hCom) return TRUE;
	m_hCom = CreateFile(m_portName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hCom == INVALID_HANDLE_VALUE) return FALSE;

	GetCommState( m_hCom, &m_Dcb );

	//　通信デバイス情報の修正
	m_Dcb.BaudRate = 9600;
	m_Dcb.ByteSize = 8;
	m_Dcb.Parity = NOPARITY;
	m_Dcb.StopBits = ONESTOPBIT;

	if(SetCommState(m_hCom, &m_Dcb) == 0) {
		CloseHandle(m_hCom); m_hCom = NULL; return FALSE;
	}

	m_ComTime.ReadIntervalTimeout = MAXDWORD;
	m_ComTime.ReadTotalTimeoutMultiplier = 0;
	m_ComTime.ReadTotalTimeoutConstant = 0;
	m_ComTime.WriteTotalTimeoutMultiplier = 0;
	m_ComTime.WriteTotalTimeoutConstant = 0;

	EscapeCommFunction(m_hCom, SETDTR);

	if (::SetTimer(NULL, MR999_CTRL_TIMER_EVENT_ID, m_timerPeriod, (TIMERPROC)TimerProc) == 0) {
		CloseHandle(m_hCom); m_hCom = NULL; return FALSE;
	}
	return TRUE;
}

void CMR999Ctrl::Stop() 
{
	if (m_hCom == NULL) return;

	EscapeCommFunction(m_hCom, CLRDTR);

	::KillTimer(NULL, MR999_CTRL_TIMER_EVENT_ID);
	CloseHandle(m_hCom); m_hCom = NULL;
}

void CMR999Ctrl::GrpClose() 
{
	m_GrpDir = 0;
	m_GrpStop = 0;
}

void CMR999Ctrl::GrpOpen() 
{
	m_GrpDir = 1;
	m_GrpStop = 0;
}

void CMR999Ctrl::GrpStop() 
{
	m_GrpStop = 1;
}

void CMR999Ctrl::WstLeft() 
{
	m_WstDir = 0;
	m_WstStop = 0;
}

void CMR999Ctrl::WstRight() 
{
	m_WstDir = 1;
	m_WstStop = 0;
}

void CMR999Ctrl::WstStop() 
{
	m_WstStop = 1;
}

void CMR999Ctrl::ElbDown() 
{
	m_ElbDir = 0;
	m_ElbStop = 0;
}

void CMR999Ctrl::ElbUp() 
{
	m_ElbDir = 1;
	m_ElbStop = 0;
}

void CMR999Ctrl::ElbStop() 
{
	m_ElbStop = 1;
}

void CMR999Ctrl::SldDown() 
{
	m_SldDir = 0;
	m_SldStop = 0;
}

void CMR999Ctrl::SldUp() 
{
	m_SldDir = 1;
	m_SldStop = 0;
}

void CMR999Ctrl::SldStop() 
{
	m_SldStop = 1;
}

void CMR999Ctrl::BaseCw() 
{
	m_BaseDir = 0;
	m_BaseStop = 0;
}

void CMR999Ctrl::BaseCcw() 
{
	m_BaseDir = 1;
	m_BaseStop = 0;
}

void CMR999Ctrl::BaseStop() 
{
	m_BaseStop = 1;
}

void CMR999Ctrl::OnDestroy() 
{
	COleControl::OnDestroy();
	
	Stop();
}

CMR999Ctrl* CMR999Ctrl::theCtrl;

void TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	CMR999Ctrl::theCtrl->TimerCB();
}

void CMR999Ctrl::TimerCB() 
{
	char data;
	if (m_state == 0) {
		if (m_GrpDir == m_GrpDirPrev &&
				m_WstDir == m_WstDirPrev &&
				m_ElbDir == m_ElbDirPrev &&
				m_SldDir == m_SldDirPrev &&
				m_BaseDir == m_BaseDirPrev) {
			data = 0xe0;
			if (m_GrpStop)  data |= 0x01;
			if (m_WstStop)  data |= 0x02;
			if (m_ElbStop)  data |= 0x04;
			if (m_SldStop)  data |= 0x08;
			if (m_BaseStop) data |= 0x10;
		} else {
			data = 0xdf;
			m_state = 1;
		}
	} else {
		data = 0x80;
		if (m_GrpDir)  data |= 0x01;
		if (m_WstDir)  data |= 0x02;
		if (m_ElbDir)  data |= 0x04;
		if (m_SldDir)  data |= 0x08;
		if (m_BaseDir) data |= 0x10;
		m_GrpDirPrev  = m_GrpDir;
		m_WstDirPrev  = m_WstDir;
		m_ElbDirPrev  = m_ElbDir;
		m_SldDirPrev  = m_SldDir;
		m_BaseDirPrev = m_BaseDir;
		m_state = 0;
	}
	unsigned long num;
	WriteFile(m_hCom, &data, 1, &num, NULL);
}
