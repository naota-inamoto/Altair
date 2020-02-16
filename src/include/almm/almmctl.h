
// AlMmCtl.h : CAlMmCtrl OLE �R���g���[�� �N���X�̐錾

class AMmDevice {
public:
	AMmDevice(CString, int, CString, UINT);
	~AMmDevice();

	CString aliasName;
	int type;
	CString fileName;
	UINT deviceID;
	int wait;
	long duration;
};

/////////////////////////////////////////////////////////////////////////////
// CAlMmCtrl : ���̃N���X�̓���̒�`�Ɋւ��Ă� AlMmCtl.cpp ���Q�Ƃ��Ă�������

class CAlMmCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAlMmCtrl)

// �R���X�g���N�^
public:
	CAlMmCtrl();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAlMmCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	~CAlMmCtrl();

	DECLARE_OLECREATE_EX(CAlMmCtrl)    // �N���X �t�@�N�g�� �� guid
	DECLARE_OLETYPELIB(CAlMmCtrl)	   // GetTypeInfo
	DECLARE_PROPPAGEIDS(CAlMmCtrl)	   // �v���p�e�B �y�[�W ID
	DECLARE_OLECTLTYPE(CAlMmCtrl)		// �^�C�v���Ƃ��̑��̃X�e�[�^�X

// ���b�Z�[�W �}�b�v
	//{{AFX_MSG(CAlMmCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnMciNotify(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// �f�B�X�p�b�` �}�b�v
	//{{AFX_DISPATCH(CAlMmCtrl)
	afx_msg short Open(LPCTSTR fileName, LPCTSTR aliasName);
	afx_msg short Select(LPCTSTR aliasName);
	afx_msg BSTR GetAliasName();
	afx_msg short Close();
	afx_msg long Length();
	afx_msg long StartPosition();
	afx_msg long Position();
	afx_msg short SeekTo(long position);
	afx_msg short SeekToStart();
	afx_msg short SeekToEnd();
	afx_msg short Wait(short flag);
	afx_msg short SetDuration(long duration);
	afx_msg short Play();
	afx_msg short Stop();
	afx_msg short Pause();
	afx_msg short Resume();
	afx_msg BSTR Mode();
	afx_msg short Cue();
	afx_msg short Record();
	afx_msg short RecordOver();
	afx_msg short Erase();
	afx_msg short Save(LPCTSTR fileName);
	afx_msg short Eject();
	afx_msg short TrackIndex();
	afx_msg short TrackNum();
	afx_msg short SetTrackIndex(short index);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �C�x���g �}�b�v
	//{{AFX_EVENT(CAlMmCtrl)
	void FireNotifySuccessful()
		{FireEvent(eventidNotifySuccessful,EVENT_PARAM(VTS_NONE));}
	void FireNotifySuperseded()
		{FireEvent(eventidNotifySuperseded,EVENT_PARAM(VTS_NONE));}
	void FireNotifyAborted()
		{FireEvent(eventidNotifyAborted,EVENT_PARAM(VTS_NONE));}
	void FireNotifyFailure()
		{FireEvent(eventidNotifyFailure,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// �f�B�X�p�b�` �� �C�x���g ID
public:
	enum {
	//{{AFX_DISP_ID(CAlMmCtrl)
	dispidOpen = 1L,
	dispidSelect = 2L,
	dispidGetAliasName = 3L,
	dispidClose = 4L,
	dispidLength = 5L,
	dispidStartPosition = 6L,
	dispidPosition = 7L,
	dispidSeekTo = 8L,
	dispidSeekToStart = 9L,
	dispidSeekToEnd = 10L,
	dispidWait = 11L,
	dispidSetDuration = 12L,
	dispidPlay = 13L,
	dispidStop = 14L,
	dispidPause = 15L,
	dispidResume = 16L,
	dispidMode = 17L,
	dispidCue = 18L,
	dispidRecord = 19L,
	dispidRecordOver = 20L,
	dispidErase = 21L,
	dispidSave = 22L,
	dispidEject = 23L,
	dispidTrackIndex = 24L,
	dispidTrackNum = 25L,
	dispidSetTrackIndex = 26L,
	eventidNotifySuccessful = 1L,
	eventidNotifySuperseded = 2L,
	eventidNotifyAborted = 3L,
	eventidNotifyFailure = 4L,
	//}}AFX_DISP_ID
	};

protected:
	long _Mode();
	long _TimeFormat(UINT);
	short _TimeFormat(UINT, long);

protected:
	CMapStringToPtr map;
	AMmDevice *curr_dev;
	AMmDevice *last_avi;
	MCIERROR err;
};

#define AL_MM_AVI	10
#define AL_MM_WAVE	20
#define AL_MM_MIDI	21
#define AL_MM_CDA	22

