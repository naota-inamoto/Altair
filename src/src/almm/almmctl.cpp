
// AlMmCtl.cpp : CAlMmCtrl OLE �R���g���[�� �N���X�̃C���v�������e�[�V����

#include "stdafx.h"
#include "AlMm/AlMm.h"
#include "AlMm/AlMmCtl.h"
#include "AlMm/AlMmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AMmDevice::AMmDevice(CString aName, int t, CString fName, UINT id) {
	aliasName = aName;
	type = t;
	fileName = fName;
	deviceID = id;
	wait = 0;
	duration = -1;
}
AMmDevice::~AMmDevice() {
}

IMPLEMENT_DYNCREATE(CAlMmCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W �}�b�v

BEGIN_MESSAGE_MAP(CAlMmCtrl, COleControl)
	//{{AFX_MSG_MAP(CAlMmCtrl)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(MM_MCINOTIFY, OnMciNotify)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// �f�B�X�p�b�` �}�b�v

BEGIN_DISPATCH_MAP(CAlMmCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CAlMmCtrl)
	DISP_FUNCTION(CAlMmCtrl, "Open", Open, VT_I2, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAlMmCtrl, "Select", Select, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlMmCtrl, "GetAliasName", GetAliasName, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Close", Close, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Length", Length, VT_I4, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "StartPosition", StartPosition, VT_I4, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Position", Position, VT_I4, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "SeekTo", SeekTo, VT_I2, VTS_I4)
	DISP_FUNCTION(CAlMmCtrl, "SeekToStart", SeekToStart, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "SeekToEnd", SeekToEnd, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Wait", Wait, VT_I2, VTS_I2)
	DISP_FUNCTION(CAlMmCtrl, "SetDuration", SetDuration, VT_I2, VTS_I4)
	DISP_FUNCTION(CAlMmCtrl, "Play", Play, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Stop", Stop, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Pause", Pause, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Resume", Resume, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Mode", Mode, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Cue", Cue, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Record", Record, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "RecordOver", RecordOver, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Erase", Erase, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "Save", Save, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CAlMmCtrl, "Eject", Eject, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "TrackIndex", TrackIndex, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "TrackNum", TrackNum, VT_I2, VTS_NONE)
	DISP_FUNCTION(CAlMmCtrl, "SetTrackIndex", SetTrackIndex, VT_I2, VTS_I2)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CAlMmCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// �C�x���g �}�b�v

BEGIN_EVENT_MAP(CAlMmCtrl, COleControl)
	//{{AFX_EVENT_MAP(CAlMmCtrl)
	EVENT_CUSTOM("NotifySuccessful", FireNotifySuccessful, VTS_NONE)
	EVENT_CUSTOM("NotifySuperseded", FireNotifySuperseded, VTS_NONE)
	EVENT_CUSTOM("NotifyAborted", FireNotifyAborted, VTS_NONE)
	EVENT_CUSTOM("NotifyFailure", FireNotifyFailure, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// �v���p�e�B �y�[�W

BEGIN_PROPPAGEIDS(CAlMmCtrl, 1)
	PROPPAGEID(CAlMmPropPage::guid)
END_PROPPAGEIDS(CAlMmCtrl)


/////////////////////////////////////////////////////////////////////////////
// �N���X �t�@�N�g���� guid �����������܂�

IMPLEMENT_OLECREATE_EX(CAlMmCtrl, "ALMM.AlMmCtrl.1",
	0x7755884, 0x7c16, 0x11d0, 0x84, 0x5c, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// �^�C�v ���C�u���� ID �ƃo�[�W����

IMPLEMENT_OLETYPELIB(CAlMmCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// �C���^�[�t�F�C�X ID

const IID BASED_CODE IID_DAlMm =
		{ 0x7755882, 0x7c16, 0x11d0, { 0x84, 0x5c, 0x44, 0x45, 0x53, 0x54, 0, 0 } };
const IID BASED_CODE IID_DAlMmEvents =
		{ 0x7755883, 0x7c16, 0x11d0, { 0x84, 0x5c, 0x44, 0x45, 0x53, 0x54, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// �R���g���[���̌^���̐���

static const DWORD BASED_CODE _dwAlMmOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAlMmCtrl, IDS_ALMM, _dwAlMmOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl::CAlMmCtrlFactory::UpdateRegistry -
// CAlMmCtrl �̃V�X�e�� ���W�X�g���̃G���g����ǉ��܂��͍폜���܂� 

BOOL CAlMmCtrl::CAlMmCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// �R���g���[�����A�p�[�g�����g ���f���̃X���b�h�����̋K���ɏ]���Ă��邱�Ƃ�
	// �m�F���Ă��������B�ڍׂɂ��Ă� MFC �̃e�N�j�J�� �m�[�g 64 ���Q�Ƃ��Ă��������B
	// �A�p�[�g�����g ���f���̃X���b�h�����̋K���ɏ]��Ȃ��R���g���[���̏ꍇ�́A6 �Ԗ�
	// �̃p�����[�^���ȉ��̂悤�ɕύX���Ă��������B
	// afxRegInsertable | afxRegApartmentThreading �� afxRegInsertable �֕ύX���Ă��������B

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ALMM,
			IDB_ALMM,
			afxRegInsertable | afxRegApartmentThreading,
			_dwAlMmOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl::CAlMmCtrl - �R���X�g���N�^

CAlMmCtrl::CAlMmCtrl()
{
	InitializeIIDs(&IID_DAlMm, &IID_DAlMmEvents);

	curr_dev = NULL;
	last_avi = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl::~CAlMmCtrl - �f�X�g���N�^

CAlMmCtrl::~CAlMmCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl::OnDraw - �`��֐�

void CAlMmCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (last_avi == NULL) {
		pdc->FillRect(rcBounds,
			CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
		pdc->Rectangle(rcBounds);
		return;
	}

	UINT deviceID = last_avi->deviceID;
	MCI_DGV_UPDATE_PARMS parms;
	parms.rc = rcBounds;
	parms.hDC = pdc->m_hDC;
	err = mciSendCommand(deviceID, MCI_UPDATE,
		MCI_DGV_UPDATE_PAINT|MCI_DGV_UPDATE_HDC|MCI_DGV_RECT|MCI_WAIT,
		(DWORD)&parms);
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl::DoPropExchange - �i�����̃T�|�[�g

void CAlMmCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl::OnResetState - �R���g���[���̃v���p�e�B�l�����Z�b�g

void CAlMmCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange ���Ăяo���ăf�t�H���g�l�Ƀ��Z�b�g
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl::AboutBox - "�o�[�W�������" �̃_�C�A���O �{�b�N�X��\��

void CAlMmCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ALMM);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl ���b�Z�[�W �n���h��

void CAlMmCtrl::OnDestroy() 
{
	COleControl::OnDestroy();
	
	CString key;
	void *dev;
	POSITION p = map.GetStartPosition();
	while (p) {
		map.GetNextAssoc(p, key, dev);
		curr_dev = (AMmDevice*)dev;
		Close();
	}
	map.RemoveAll();
}

void CAlMmCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	CString key;
	AMmDevice *dev;
	POSITION p = map.GetStartPosition();
	while (p) {
		map.GetNextAssoc(p, key, (void*&)dev);
		if (dev->type != AL_MM_AVI)
			continue;
		MCI_DGV_PUT_PARMS putParms;
		RECT r;
		r.left = r.top = 0;
		r.right = cx; r.bottom = cy;
		putParms.rc= r;
		err = mciSendCommand(dev->deviceID, MCI_PUT,	
			MCI_DGV_PUT_DESTINATION|MCI_DGV_RECT|MCI_WAIT,
			(DWORD)&putParms);
	}
}

LRESULT CAlMmCtrl::OnMciNotify(WPARAM w, LPARAM) {
	switch (w) {
	case MCI_NOTIFY_SUCCESSFUL:
		FireNotifySuccessful();
		break;
	case MCI_NOTIFY_SUPERSEDED:
		FireNotifySuperseded();
		break;
	case MCI_NOTIFY_ABORTED:	
		FireNotifyAborted();
		break;
	case MCI_NOTIFY_FAILURE:
		FireNotifyFailure();
		break;
	default:
		break;
	}
	return TRUE;
}

short CAlMmCtrl::Open(LPCTSTR fileName, LPCTSTR aliasName) 
{
	void *dev;
	if (map.Lookup(aliasName, (void*&)dev))
		return -1; // ���ɓ���alias��������
	CString f(fileName);
	CString ext = f.Right(4);
	int type;
	UINT deviceID;

	if (ext.CompareNoCase(".avi") == 0) {
		type = AL_MM_AVI;
		MCI_DGV_OPEN_PARMS openParms;
		openParms.lpstrDeviceType = "AVIVideo";
		openParms.lpstrElementName = (char*)fileName;
		openParms.dwCallback = 0;
		openParms.wDeviceID = 0;
		openParms.lpstrAlias = (char*)aliasName;
		openParms.dwStyle = 0;
		openParms.hWndParent = 0;
		err = mciSendCommand(NULL, MCI_OPEN,
			MCI_OPEN_TYPE|MCI_OPEN_ELEMENT|MCI_OPEN_ALIAS|MCI_WAIT,
			(DWORD)&openParms);
		if (err)
			return -3; // �I�[�v���Ɏ��s
		deviceID = openParms.wDeviceID;
		
		{
			MCI_DGV_WINDOW_PARMS parms;
			parms.hWnd = m_hWnd;
			parms.nCmdShow = 0;
			err = mciSendCommand(deviceID, MCI_WINDOW,	
				MCI_DGV_WINDOW_HWND|MCI_WAIT, (DWORD)&parms);
		}

		{
			MCI_DGV_SET_PARMS setParms;
			setParms.dwSpeed = 1000;
			err = mciSendCommand(deviceID, MCI_SET,
				MCI_DGV_SET_SPEED|MCI_WAIT, (DWORD)&setParms);
		}
		_TimeFormat(deviceID, MCI_FORMAT_MILLISECONDS);
		{
			MCI_GENERIC_PARMS parms;
			err = mciSendCommand(deviceID, MCI_REALIZE,
				MCI_DGV_REALIZE_BKGD|MCI_WAIT, (DWORD)&parms);
		}
		{
			MCI_DGV_PUT_PARMS putParms;
			RECT r;
			GetClientRect(&r);
			putParms.rc= r;
			err = mciSendCommand(deviceID, MCI_PUT, 
				MCI_DGV_PUT_DESTINATION|MCI_DGV_RECT|MCI_WAIT,
				(DWORD)&putParms);
		}
	} else if (ext.CompareNoCase(".wav") == 0) {
		type = AL_MM_WAVE;
		MCI_WAVE_OPEN_PARMS openParms;
		openParms.lpstrDeviceType = "waveaudio";
		openParms.lpstrElementName = (char*)fileName;
		openParms.lpstrAlias = (char*)aliasName;
		err = mciSendCommand(NULL, MCI_OPEN,
			MCI_OPEN_TYPE|MCI_OPEN_ELEMENT|MCI_OPEN_ALIAS|MCI_WAIT,
			(DWORD)&openParms);
		if (err)
			return -3; // �I�[�v���Ɏ��s
		deviceID = openParms.wDeviceID;
		_TimeFormat(deviceID, MCI_FORMAT_MILLISECONDS);
	} else if (ext.CompareNoCase(".mid") == 0) {
		type = AL_MM_MIDI;
		MCI_WAVE_OPEN_PARMS openParms;
		openParms.lpstrDeviceType = "sequencer";
		openParms.lpstrElementName = (char*)fileName;
		openParms.lpstrAlias = (char*)aliasName;
		err = mciSendCommand(NULL, MCI_OPEN,
			MCI_OPEN_TYPE|MCI_OPEN_ELEMENT|MCI_OPEN_ALIAS|MCI_WAIT,
			(DWORD)&openParms);
		if (err)
			return -3; // �I�[�v���Ɏ��s
		deviceID = openParms.wDeviceID;
		_TimeFormat(deviceID, MCI_FORMAT_MILLISECONDS);
		{
			MCI_SEQ_SET_PARMS setParms;
			setParms.dwTempo = 120;
			err = mciSendCommand(deviceID, MCI_SET,
				MCI_SEQ_SET_TEMPO|MCI_WAIT, (DWORD)&setParms);
		}
	} else if (ext.CompareNoCase(".cda") == 0) {
		type = AL_MM_CDA;
		MCI_OPEN_PARMS openParms;
		openParms.lpstrDeviceType = "cdaudio";
		err = mciSendCommand(NULL, MCI_OPEN,	
			MCI_OPEN_TYPE|MCI_WAIT, (DWORD)&openParms);
		if (err)
			return -3; // �I�[�v���Ɏ��s
		deviceID = openParms.wDeviceID;
		_TimeFormat(deviceID, MCI_FORMAT_MILLISECONDS);
	} else {
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	curr_dev = new AMmDevice(aliasName, type, fileName, deviceID);
	if (type == AL_MM_AVI) {
		last_avi = curr_dev;
		InvalidateRect(NULL);
	}
	map.SetAt(aliasName, (void*)curr_dev);
	return 0;
}

short CAlMmCtrl::Select(LPCTSTR aliasName) 
{
	void *dev;
	if (map.Lookup(aliasName, dev) == 0)
		return -1; // ����alias�����Ȃ�
	curr_dev = (AMmDevice*)dev;
	if (curr_dev->type == AL_MM_AVI) {
		last_avi = curr_dev;
		InvalidateRect(NULL);
	}
	return 0;
}

BSTR CAlMmCtrl::GetAliasName() 
{
	CString strResult;
	if (curr_dev == NULL) {
		strResult = "no current device";
	} else {
		strResult = curr_dev->aliasName;
	}
	return strResult.AllocSysString();
}

short CAlMmCtrl::Close() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	Stop();
	UINT deviceID = curr_dev->deviceID;
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
		err = mciSendCommand(deviceID, MCI_CLOSE, MCI_WAIT, NULL);
		break;
	case AL_MM_CDA:
		err = mciSendCommand(deviceID, MCI_CLOSE, 0, NULL);
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}

	if (curr_dev->type == AL_MM_AVI) {
		last_avi = NULL;
		InvalidateRect(NULL);
	}
	LPCSTR aliasName = curr_dev->aliasName;
	map.RemoveKey(aliasName);
	delete curr_dev;
	curr_dev = NULL;
	if (err)
		return -3; // Close�Ɏ��s
	return 0;
}

long CAlMmCtrl::Length() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_STATUS_PARMS statusParms;
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
		statusParms.dwItem = MCI_STATUS_LENGTH;
		err = mciSendCommand(deviceID, MCI_STATUS,
			MCI_STATUS_ITEM|MCI_STATUS_LENGTH|MCI_WAIT,
			(DWORD)&statusParms);
		if (err)
			return -3; // �l�̎擾�Ɏ��s
		return statusParms.dwReturn;
	case AL_MM_CDA:
		statusParms.dwItem = MCI_STATUS_LENGTH;
		err = mciSendCommand(deviceID, MCI_STATUS,
			MCI_STATUS_ITEM,
			(DWORD)&statusParms);	
		if (err)
			return -3; // �l�̎擾�Ɏ��s
		return statusParms.dwReturn;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
}

long CAlMmCtrl::StartPosition() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_STATUS_PARMS statusParms;
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
	case AL_MM_CDA:
		statusParms.dwItem = MCI_STATUS_POSITION;
		err = mciSendCommand(deviceID, MCI_STATUS,
			MCI_STATUS_ITEM|MCI_STATUS_START|MCI_WAIT,
			(DWORD)&statusParms);
		if (err)
			return -3; // �l�̎擾�Ɏ��s
		return statusParms.dwReturn;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
}

long CAlMmCtrl::Position() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_STATUS_PARMS statusParms;
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
	case AL_MM_CDA:
		statusParms.dwItem = MCI_STATUS_POSITION;
		err = mciSendCommand(deviceID, MCI_STATUS,
			MCI_STATUS_ITEM|MCI_STATUS_POSITION|MCI_WAIT,
			(DWORD)&statusParms);
		if (err)
			return -3; // �l�̎擾�Ɏ��s
		return statusParms.dwReturn;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
}

short CAlMmCtrl::SeekTo(long position) 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_SEEK_PARMS seekParms;	
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
		if (position <= 0)	
			return SeekToStart();
		if (position >= Length())	
			return SeekToEnd();
		seekParms.dwTo = position;
		err = mciSendCommand(deviceID, MCI_SEEK,
			MCI_TO|MCI_WAIT, (DWORD)&seekParms);
		if (err)
			return -1; // Seek�Ɏ��s
		break;
	case AL_MM_CDA:
		seekParms.dwTo = position;
		err = mciSendCommand(deviceID, MCI_SEEK,
			MCI_TO|MCI_WAIT, (DWORD)&seekParms);	
		if (err)
			return -1; // Seek�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::SeekToStart() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_SEEK_PARMS seekParms;	
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
		err = mciSendCommand(deviceID, MCI_SEEK,
			MCI_SEEK_TO_START|MCI_WAIT, (DWORD)&seekParms);
		if (err)
			return -1; // Seek�Ɏ��s
		break;
	case AL_MM_CDA:
		err = mciSendCommand(deviceID, MCI_SEEK,
			MCI_SEEK_TO_START, NULL);
		if (err)
			return -1; // Seek�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::SeekToEnd() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_SEEK_PARMS seekParms;	
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
		err = mciSendCommand(deviceID, MCI_SEEK,
			MCI_SEEK_TO_END|MCI_WAIT, (DWORD)&seekParms);
		if (err)
			return -1; // Seek�Ɏ��s
		break;
	case AL_MM_CDA:
		err = mciSendCommand(deviceID, MCI_SEEK,
			MCI_SEEK_TO_END, NULL);
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::Wait(short flag) 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	curr_dev->wait = flag;
	return 0;
}

short CAlMmCtrl::SetDuration(long duration) 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	if (duration <= 0)
		return -2; // duration�̒l��0�ȉ�
	if (duration >= Length()) {
		curr_dev->duration = -1; // ������Length�ȏ�̏ꍇ�̓f�t�H���g
		return 0;
	}
	curr_dev->duration = duration;
	return 0;
}

short CAlMmCtrl::Play() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	DWORD notifyFlag;
	if (curr_dev->wait) {
		notifyFlag = MCI_WAIT;
	} else {
		notifyFlag = MCI_NOTIFY;
	}
	UINT deviceID = curr_dev->deviceID;
	MCI_PLAY_PARMS	playParms;
	long from, to;
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:	// TimeFormat��millisecond�Ɖ���
	case AL_MM_CDA: 	// TimeFormat��millisecond�Ɖ���
		from = Position();
		if (curr_dev->duration == -1) {
			to = Length();
		} else {
			to = from + curr_dev->duration;
			if (to > Length()) to = Length();
		}
		playParms.dwCallback = (DWORD)m_hWnd;
		playParms.dwFrom = from;
		playParms.dwTo = to;
		err = mciSendCommand(deviceID, MCI_PLAY,	
			MCI_FROM|MCI_TO|notifyFlag, (DWORD)&playParms);
		if (err)
			return -3; // Play�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::Stop() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
		err = mciSendCommand(deviceID, MCI_STOP, MCI_WAIT, NULL);
		if (err)
			return -3; // Stop�Ɏ��s
		break;
	case AL_MM_CDA:
		long pos;
		pos = Position();
		err = mciSendCommand(deviceID, MCI_STOP, MCI_WAIT, NULL);
		if (err)
			return -3; // Stop�Ɏ��s
		SeekTo(pos);
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::Pause() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
	case AL_MM_CDA:
		if (_Mode() == MCI_MODE_PAUSE)
			return 0; // ���Ɉꎞ��~���Ă���
		err = mciSendCommand(deviceID, MCI_PAUSE, MCI_WAIT, NULL);
		if (err)
			return -3; // �ꎞ��~�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::Resume() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	DWORD notifyFlag;
	if (curr_dev->wait) {
		notifyFlag = MCI_WAIT;
	} else {
		notifyFlag = MCI_NOTIFY;
	}
	UINT deviceID = curr_dev->deviceID;
	MCI_GENERIC_PARMS	param;
	switch (curr_dev->type) {
	case AL_MM_AVI:
		param.dwCallback = (DWORD)m_hWnd;
		err = mciSendCommand(deviceID, MCI_RESUME,
			notifyFlag, (DWORD)&param);
		if (err)
			return -3; // �Đ��Ɏ��s
		break;
	case AL_MM_WAVE:
		err = mciSendCommand(deviceID, MCI_RESUME,
			MCI_WAIT, NULL);
		if (err)
			return -3; // �Đ��Ɏ��s
		break;
	case AL_MM_MIDI: // TimeFormat��milliseconnd�Ɖ���
		if (_Mode() != MCI_MODE_PAUSE)
			return -4; // �ꎞ��~��ԂłȂ�
		return Play();
	case AL_MM_CDA: // TimeFormat��milliseconnd�Ɖ���
		return Play();
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

long CAlMmCtrl::_Mode() {
	MCI_STATUS_PARMS statusParms;
	statusParms.dwItem = MCI_STATUS_MODE;
	err = mciSendCommand(curr_dev->deviceID, MCI_STATUS,
		MCI_STATUS_ITEM|MCI_WAIT, (DWORD)&statusParms);
	return statusParms.dwReturn;
}

BSTR CAlMmCtrl::Mode() 
{
	CString strResult;
	if (curr_dev == NULL)
		strResult = "device not selected";
	err = 0;
	switch (_Mode()) {
	case MCI_MODE_NOT_READY:
		strResult = "not ready";
		break;
	case MCI_MODE_PAUSE:
		strResult = "pause";
		break;
	case MCI_MODE_PLAY:
		strResult = "play";
		break;
	case MCI_MODE_STOP:
		strResult = "stop";
		break;
	case MCI_MODE_RECORD:
		strResult = "recored";
		break;
	case MCI_MODE_SEEK:
		strResult = "seek";
		break;
	default:
		strResult = "other status";
		break;
	}
	if (err)
		strResult = "failed";
	return strResult.AllocSysString();
}

short CAlMmCtrl::Cue() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	switch (curr_dev->type) {
	case AL_MM_WAVE:
		err = mciSendCommand(deviceID, MCI_CUE,
			MCI_WAIT, NULL);
		if (err)
			return -3; // Cue�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::Record() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	DWORD notifyFlag;
	if (curr_dev->wait) {
		notifyFlag = MCI_WAIT;
	} else {
		notifyFlag = MCI_NOTIFY;
	}
	UINT deviceID = curr_dev->deviceID;
	MCI_RECORD_PARMS recParms;
	switch (curr_dev->type) {
	long from, to;
	case AL_MM_WAVE:
		from = Position();
		if (curr_dev->duration == -1) {
			recParms.dwCallback = (DWORD)m_hWnd;
			recParms.dwFrom = from;
			err = mciSendCommand(deviceID, MCI_RECORD,
				MCI_FROM|notifyFlag, (DWORD)&recParms);
		} else {
			to = from + curr_dev->duration;
			if (to > Length()) to = Length();
			recParms.dwCallback = (DWORD)m_hWnd;
			recParms.dwFrom = from;
			recParms.dwTo = to;
			err = mciSendCommand(deviceID, MCI_RECORD,
				MCI_FROM|MCI_TO|notifyFlag, (DWORD)&recParms);
		}
		if (err)
			return -3; // Record�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::RecordOver() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	DWORD notifyFlag;
	if (curr_dev->wait) {
		notifyFlag = MCI_WAIT;
	} else {
		notifyFlag = MCI_NOTIFY;
	}
	UINT deviceID = curr_dev->deviceID;
	MCI_RECORD_PARMS recParms;
	long from, to;
	switch (curr_dev->type) {
	case AL_MM_WAVE:
		from = Position();
		if (curr_dev->duration == -1) {
			recParms.dwCallback = (DWORD)m_hWnd;
			recParms.dwFrom = from;
			err = mciSendCommand(deviceID, MCI_RECORD_OVERWRITE,
				MCI_FROM|notifyFlag, (DWORD)&recParms);
		} else {
			to = from + curr_dev->duration;
			if (to > Length()) to = Length();
			recParms.dwCallback = (DWORD)m_hWnd;
			recParms.dwFrom = from;
			recParms.dwTo = to;
			err = mciSendCommand(deviceID, MCI_RECORD_OVERWRITE,
				MCI_FROM|MCI_TO|notifyFlag, (DWORD)&recParms);
		}
		if (err)
			return -3; // RecordOver�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::Erase() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_WAVE_DELETE_PARMS deleteParms;
	long from, to;
	switch (curr_dev->type) {
	case AL_MM_WAVE:
		from = Position();
		if (curr_dev->duration == -1) {
			to = Length();
		} else {
			to = from + curr_dev->duration;
			if (to > Length()) to = Length();
		}
		deleteParms.dwCallback = (DWORD)m_hWnd;
		deleteParms.dwFrom = from;
		deleteParms.dwTo = to;
		err = mciSendCommand(deviceID, MCI_DELETE,
			MCI_FROM|MCI_TO|MCI_WAIT, (DWORD)&deleteParms);
		if (err)
			return -3; // Erase�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::Save(LPCTSTR fileName) 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_SAVE_PARMS saveParms;
	switch (curr_dev->type) {
	case AL_MM_WAVE:
	case AL_MM_MIDI:
		saveParms.lpfilename = (char*)fileName;
		err = mciSendCommand(deviceID, MCI_SAVE,
			MCI_SAVE_FILE|MCI_WAIT, (DWORD)&saveParms );
		if (err)
			return -3; // Erase�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::Eject() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_SET_PARMS setParms;
	switch (curr_dev->type) {
	case AL_MM_CDA:
		err = mciSendCommand(deviceID, MCI_SET, 
			MCI_SET_DOOR_OPEN | MCI_WAIT, (DWORD)&setParms);
		if (err)
			return -3; // Eject�Ɏ��s
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

short CAlMmCtrl::TrackIndex() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_STATUS_PARMS statusParms;	
	switch (curr_dev->type) {
	case AL_MM_WAVE:
	case AL_MM_MIDI:
	case AL_MM_CDA:
		statusParms.dwItem = MCI_STATUS_CURRENT_TRACK;		
		err = mciSendCommand(deviceID, MCI_STATUS,
			MCI_STATUS_ITEM|MCI_WAIT, (DWORD)&statusParms);
		if (err)
			return -3; // �擾�Ɏ��s
		return (short)statusParms.dwReturn;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
}

short CAlMmCtrl::TrackNum() 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	UINT deviceID = curr_dev->deviceID;
	MCI_STATUS_PARMS statusParms;
	switch (curr_dev->type) {
	case AL_MM_AVI:
	case AL_MM_WAVE:
	case AL_MM_MIDI:
		statusParms.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
		err = mciSendCommand(deviceID, MCI_STATUS,
			MCI_STATUS_ITEM|MCI_WAIT, (DWORD)&statusParms);
		if (err)
			return -3; // �擾�Ɏ��s
		return (short)statusParms.dwReturn;
	case AL_MM_CDA:
		statusParms.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
		err = mciSendCommand(deviceID, MCI_STATUS,
			MCI_STATUS_ITEM, (DWORD)&statusParms);
		if (err)
			return -3; // �擾�Ɏ��s
		return (short)statusParms.dwReturn;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
}

short CAlMmCtrl::SetTrackIndex(short index) 
{
	if (curr_dev == NULL)
		return -1; // �f�o�C�X���I������Ă��Ȃ�
	long timeFormat, position;
	switch (curr_dev->type) {
	case AL_MM_CDA:
		if (index < 1 || TrackNum() < index)
			return -3; // index���s��
		position = MCI_MAKE_TMSF(index, 0, 0, 30);
		timeFormat = _TimeFormat(curr_dev->deviceID);
		_TimeFormat(curr_dev->deviceID, MCI_FORMAT_TMSF);	
		err = SeekTo(position);
		_TimeFormat(curr_dev->deviceID, timeFormat);
		break;
	default:
		return -2; // �T�|�[�g���Ă��Ȃ����f�B�A
	}
	return 0;
}

long CAlMmCtrl::_TimeFormat(UINT deviceID) {
	MCI_STATUS_PARMS statusParms;
	statusParms.dwItem = MCI_STATUS_TIME_FORMAT;
	err = mciSendCommand(deviceID, MCI_STATUS,
		MCI_STATUS_ITEM|MCI_WAIT, (DWORD)&statusParms);
	return statusParms.dwReturn;
}

short CAlMmCtrl::_TimeFormat(UINT deviceID, long timeFormat) {
	MCI_SET_PARMS setParms;
	setParms.dwTimeFormat = timeFormat;
	err = mciSendCommand(deviceID, MCI_SET, 
		MCI_SET_TIME_FORMAT, (DWORD)&setParms);
	return 0; 
}
