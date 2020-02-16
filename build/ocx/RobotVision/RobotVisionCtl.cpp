// RobotVisionCtl.cpp : CRobotVisionCtrl ActiveX コントロール クラスのインプリメンテーション

#include "stdafx.h"
#include "RobotVision.h"
#include "RobotVisionCtl.h"
#include "RobotVisionPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRobotVisionCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// メッセージ マップ

BEGIN_MESSAGE_MAP(CRobotVisionCtrl, COleControl)
	//{{AFX_MSG_MAP(CRobotVisionCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ディスパッチ マップ

BEGIN_DISPATCH_MAP(CRobotVisionCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CRobotVisionCtrl)
	DISP_PROPERTY_NOTIFY(CRobotVisionCtrl, "DriverName", m_driverName, OnDriverNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CRobotVisionCtrl, "DriverVersion", m_driverVersion, OnDriverVersionChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CRobotVisionCtrl, "DeviceIndex", m_deviceIndex, OnDeviceIndexChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CRobotVisionCtrl, "PreviewRate", m_previewRate, OnPreviewRateChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CRobotVisionCtrl, "Preview", m_preview, OnPreviewChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CRobotVisionCtrl, "Overlay", m_overlay, OnOverlayChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CRobotVisionCtrl, "PreviewScale", m_previewScale, OnPreviewScaleChanged, VT_BOOL)
	DISP_FUNCTION(CRobotVisionCtrl, "Start", Start, VT_I2, VTS_NONE)
	DISP_FUNCTION(CRobotVisionCtrl, "Stop", Stop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CRobotVisionCtrl, "SaveBitmap", SaveBitmap, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CRobotVisionCtrl, "DlgVideoFormat", DlgVideoFormat, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CRobotVisionCtrl, "StartCapture", StartCapture, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CRobotVisionCtrl, "StopCapture", StopCapture, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CRobotVisionCtrl, "DlgVideoSource", DlgVideoSource, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CRobotVisionCtrl, "DlgVideoCompression", DlgVideoCompression, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CRobotVisionCtrl, "ImageWindow", ImageWindow, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CRobotVisionCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// イベント マップ

BEGIN_EVENT_MAP(CRobotVisionCtrl, COleControl)
	//{{AFX_EVENT_MAP(CRobotVisionCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// プロパティ ページ

BEGIN_PROPPAGEIDS(CRobotVisionCtrl, 1)
	PROPPAGEID(CRobotVisionPropPage::guid)
END_PROPPAGEIDS(CRobotVisionCtrl)


/////////////////////////////////////////////////////////////////////////////
// クラス ファクトリと guid を初期化します

IMPLEMENT_OLECREATE_EX(CRobotVisionCtrl, "ROBOTVISION.RobotVisionCtrl.1",
	0x36292a29, 0xaff3, 0x4f1d, 0xb1, 0xe7, 0x94, 0xd8, 0x75, 0x56, 0xf7, 0x25)


/////////////////////////////////////////////////////////////////////////////
// タイプ ライブラリ ID とバージョン

IMPLEMENT_OLETYPELIB(CRobotVisionCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// インターフェイス ID

const IID BASED_CODE IID_DRobotVision =
		{ 0x218a5b90, 0x1df3, 0x4456, { 0x81, 0x60, 0xd5, 0x40, 0x7c, 0xf6, 0x13, 0xd1 } };
const IID BASED_CODE IID_DRobotVisionEvents =
		{ 0x7f1eca08, 0x1d1, 0x49ef, { 0x8d, 0x75, 0x6a, 0x3f, 0x99, 0x99, 0xf4, 0x4d } };


/////////////////////////////////////////////////////////////////////////////
// コントロールの型情報の制御

static const DWORD BASED_CODE _dwRobotVisionOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CRobotVisionCtrl, IDS_ROBOTVISION, _dwRobotVisionOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl::CRobotVisionCtrlFactory::UpdateRegistry -
// CRobotVisionCtrl のシステム レジストリのエントリを追加または削除します 

BOOL CRobotVisionCtrl::CRobotVisionCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: コントロールがアパートメント モデルのスレッド処理の規則に従っていることを
	// 確認してください。詳細については MFC のテクニカル ノート 64 を参照してください。
	// アパートメント モデルのスレッド処理の規則に従わないコントロールの場合は、6 番目
	// のパラメータを以下のように変更してください。
	// afxRegInsertable | afxRegApartmentThreading を afxRegInsertable へ変更してください。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ROBOTVISION,
			IDB_ROBOTVISION,
			afxRegInsertable | afxRegApartmentThreading,
			_dwRobotVisionOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl::CRobotVisionCtrl - コンストラクタ

CRobotVisionCtrl::CRobotVisionCtrl()
{
	InitializeIIDs(&IID_DRobotVision, &IID_DRobotVisionEvents);

	theCtrl = this;
	m_Hardware = FALSE;
	m_hWndCap = NULL;

	m_deviceIndex = 0;
	m_previewRate = 15;
	m_previewScale = TRUE;
	m_preview = TRUE;
	m_overlay = FALSE;

	image_r = image_g = image_b = NULL;
	m_image_wnd = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl::~CRobotVisionCtrl - デストラクタ

CRobotVisionCtrl::~CRobotVisionCtrl()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl::OnDraw - 描画関数

void CRobotVisionCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl::DoPropExchange - 永続性のサポート

void CRobotVisionCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl::OnResetState - コントロールのプロパティ値をリセット

void CRobotVisionCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange を呼び出してデフォルト値にリセット
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl::AboutBox - "バージョン情報" のダイアログ ボックスを表示

void CRobotVisionCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ROBOTVISION);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl メッセージ ハンドラ

int CRobotVisionCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_hWndCap = capCreateCaptureWindow(
		NULL, WS_CHILD | WS_VISIBLE,
		0, 0, lpCreateStruct->cx, lpCreateStruct->cy,
		m_hWnd,	// parent window
		1		// child window id
		);

	return 0;
}
void CRobotVisionCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);

	if (m_hWndCap) {
		::MoveWindow(m_hWndCap, 0, 0, cx, cy, TRUE);
	}
	
}
void CRobotVisionCtrl::OnDestroy() 
{
	COleControl::OnDestroy();

	Stop();
	
}

void CRobotVisionCtrl::OnDriverNameChanged()  {
	SetModifiedFlag();
}
void CRobotVisionCtrl::OnDriverVersionChanged()  {
	SetModifiedFlag();
}
void CRobotVisionCtrl::OnDeviceIndexChanged()  {
	SetModifiedFlag();
}
void CRobotVisionCtrl::OnPreviewRateChanged() {
	if (m_hWndCap) {
		capPreviewRate(m_hWndCap, m_previewRate);
	}
	SetModifiedFlag();
}
void CRobotVisionCtrl::OnPreviewScaleChanged() {
	if (m_hWndCap) {
		capPreviewScale(m_hWndCap, m_previewScale);
	}
	SetModifiedFlag();
}
void CRobotVisionCtrl::OnPreviewChanged() {
	if (m_hWndCap) {
		capPreview(m_hWndCap, m_preview);
	}
	SetModifiedFlag();
}
void CRobotVisionCtrl::OnOverlayChanged() {
	if (m_hWndCap) {
	    capOverlay(m_hWndCap, m_overlay);
	}
	SetModifiedFlag();
}

short CRobotVisionCtrl::Start() 
{
	if (m_Hardware) return 1;
	if (m_hWndCap == NULL || capDriverConnect(m_hWndCap, m_deviceIndex) == 0) {
		return 0;
	}
	m_Hardware = TRUE;

    capDriverGetCaps(m_hWndCap, &m_CapDriverCaps, sizeof(CAPDRIVERCAPS)) ;
    capGetStatus(m_hWndCap, &m_CapStatus , sizeof(CAPSTATUS));
	m_num_pixels = m_CapStatus.uiImageWidth * m_CapStatus.uiImageHeight;
	capCaptureGetSetup(m_hWndCap, &m_CapParms, sizeof(CAPTUREPARMS));

	image_r = AllocImage();
	image_g = AllocImage();
	image_b = AllocImage();
	capSetCallbackOnFrame(m_hWndCap, FrameCallbackFunc);

	char name[1024], version[64];
	capGetDriverDescription(m_deviceIndex, name, 1024, version, 64);
	m_driverName = name;
	m_driverVersion = version;
 
	if (m_CapDriverCaps.fHasOverlay) {
		m_overlay = TRUE;
	}
	capOverlay(m_hWndCap, m_overlay);

	capPreviewRate(m_hWndCap, m_previewRate);
	capPreviewScale(m_hWndCap, m_previewScale);
	capPreview(m_hWndCap, m_preview);

	return 1;
}

void CRobotVisionCtrl::Stop() 
{
	if (m_Hardware == FALSE) return;
	m_Hardware = FALSE;
	capDriverDisconnect(m_hWndCap);

	FreeImage(image_r); image_r = NULL;
	FreeImage(image_g); image_g = NULL;
	FreeImage(image_b); image_b = NULL;
}

BOOL CRobotVisionCtrl::SaveBitmap(LPCTSTR filename) 
{
	if (m_Hardware == FALSE) return FALSE;
	return capFileSaveDIB(m_hWndCap, filename);
}

BOOL CRobotVisionCtrl::DlgVideoFormat() 
{
	if (m_Hardware == FALSE) return FALSE;
	if (m_hWndCap == NULL) return FALSE;
	BOOL r = capDlgVideoFormat(m_hWndCap);
    capGetStatus(m_hWndCap, &m_CapStatus , sizeof(CAPSTATUS));
	m_num_pixels = m_CapStatus.uiImageWidth * m_CapStatus.uiImageHeight;
	return r;
}

BOOL CRobotVisionCtrl::DlgVideoSource() 
{
	if (m_Hardware == FALSE) return FALSE;
	if (m_hWndCap == NULL) return FALSE;
	return capDlgVideoSource(m_hWndCap);
	return TRUE;
}

BOOL CRobotVisionCtrl::DlgVideoCompression() 
{
	if (m_Hardware == FALSE) return FALSE;
	if (m_hWndCap == NULL) return FALSE;
	return capDlgVideoCompression(m_hWndCap);
	return TRUE;
}

BOOL CRobotVisionCtrl::StartCapture(LPCTSTR filename) 
{
	if (m_Hardware == FALSE) return FALSE;
	if (m_hWndCap == NULL) return FALSE;
	if (capFileSetCaptureFile(m_hWndCap, filename) == FALSE) return FALSE;
	return capCaptureSequence(m_hWndCap);
}

void CRobotVisionCtrl::StopCapture() 
{
	if (m_Hardware == FALSE) return;
	if (m_hWndCap == NULL) return;
	capCaptureStop(m_hWndCap);
}

void CRobotVisionCtrl::ImageWindow() 
{
	if (m_image_wnd) {
		return;
	}

	m_doc_template = new CSingleDocTemplate(
		IDR_MENU1,
		RUNTIME_CLASS(ImageDoc),
		RUNTIME_CLASS(ImageFrame),
		RUNTIME_CLASS(ImageView));

	CWinApp *app = AfxGetApp();
	CWnd *pMainWnd = app->m_pMainWnd;
	app->m_pMainWnd = NULL;
	m_doc_template->OpenDocumentFile(NULL);
	m_image_wnd = (ImageFrame*)app->m_pMainWnd;
	app->m_pMainWnd = pMainWnd;
	m_image_wnd->SetWindowText("Image");
	m_image_wnd->ShowWindow(SW_SHOW);
	m_image_wnd->UpdateWindow();
}

CRobotVisionCtrl* CRobotVisionCtrl::theCtrl;

LRESULT FrameCallbackFunc(HWND hWnd, LPVIDEOHDR lpVHdr) {
	CRobotVisionCtrl::theCtrl->FrameCallback(lpVHdr);
	return TRUE;
}

void CRobotVisionCtrl::FrameCallback(LPVIDEOHDR lpVHdr) {
	unsigned long len = lpVHdr->dwBufferLength;
	unsigned short *pShortBuff = (unsigned short*)lpVHdr->lpData;
	unsigned char *pByteBuff = (unsigned char*)lpVHdr->lpData;
	int i, j;
	for(i = m_CapStatus.uiImageHeight - 1; i >= 0 ; i--) {
		for(j = 0; j < (int)m_CapStatus.uiImageWidth; j++) {
			if (len == m_num_pixels * 2) {
				unsigned short value = *pShortBuff++;
				image_b[i][j] = ((value << 3) & 0xf8);
				image_g[i][j] = ((value >> 2) & 0xf8);
				image_r[i][j] = ((value >> 7) & 0xf8);
			} else if (len == m_num_pixels * 3) {
				image_b[i][j] = *pByteBuff++;
				image_g[i][j] = *pByteBuff++;
				image_r[i][j] = *pByteBuff++;
			}
		}
	}
	if (m_image_wnd) {
		m_image_wnd->FrameCallback();
	}
}

unsigned char** CRobotVisionCtrl::AllocImage() {
	unsigned char **image = new unsigned char*[m_CapStatus.uiImageHeight];
	int i;
	for(i = 0; i < (int)m_CapStatus.uiImageHeight; i++) {
		image[i] = new unsigned char[m_CapStatus.uiImageWidth];
	}
	return image;
}
void CRobotVisionCtrl::FreeImage(unsigned char** image) {
	unsigned int i;
	for(i = 0; i < m_CapStatus.uiImageHeight; i++) {
		delete [] image[i];
	}
	delete [] image;
}

// --------------------------------------------------------------

IMPLEMENT_DYNCREATE(ImageFrame, CFrameWnd)
IMPLEMENT_DYNCREATE(ImageView, CView)
IMPLEMENT_DYNCREATE(ImageDoc, CDocument)

ImageFrame::~ImageFrame() {
	CRobotVisionCtrl * ctrl = CRobotVisionCtrl::theCtrl;
	ctrl->m_image_wnd = NULL;
}
void ImageFrame::FrameCallback() {
	CView *view = GetActiveView();
	CRobotVisionCtrl * ctrl = CRobotVisionCtrl::theCtrl;
	CRect rect(10, 10, 10 + ctrl->m_CapStatus.uiImageWidth,  10 + ctrl->m_CapStatus.uiImageHeight);
	view->InvalidateRect(rect, FALSE);
	view->UpdateWindow();
}
void ImageView::OnDraw(CDC *pDC) {
	CRobotVisionCtrl * ctrl = CRobotVisionCtrl::theCtrl;
	if (ctrl->image_r && ctrl->image_b && ctrl->image_b) {
		int i, j;
		for(i = ctrl->m_CapStatus.uiImageHeight - 1; i >= 0 ; i--) {
			for(j = 0; j < (int)ctrl->m_CapStatus.uiImageWidth; j++) {
				COLORREF color = ctrl->image_r[i][j] + (ctrl->image_g[i][j] << 8) + (ctrl->image_b[i][j] << 16);
				pDC->SetPixel(10 + j, 10 + i, color);
			}
		}
	}
}

