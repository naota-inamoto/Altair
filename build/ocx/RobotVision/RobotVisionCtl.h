#if !defined(AFX_ROBOTVISIONCTL_H__E0DCC4F0_AAF3_4B63_B183_16CC52B0DD3F__INCLUDED_)
#define AFX_ROBOTVISIONCTL_H__E0DCC4F0_AAF3_4B63_B183_16CC52B0DD3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RobotVisionCtl.h : CRobotVisionCtrl ActiveX �R���g���[�� �N���X�̐錾

#include <vfw.h>

LRESULT FrameCallbackFunc(HWND hWnd, LPVIDEOHDR lpVHdr);

/////////////////////////////////////////////////////////////////////////////
// CRobotVisionCtrl : ���̃N���X�̓���̒�`�Ɋւ��Ă� RobotVisionCtl.cpp ���Q�Ƃ��Ă�������

class ImageFrame;

class CRobotVisionCtrl : public COleControl
{
	DECLARE_DYNCREATE(CRobotVisionCtrl)

// �R���X�g���N�^
public:
	CRobotVisionCtrl();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CRobotVisionCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	~CRobotVisionCtrl();

	DECLARE_OLECREATE_EX(CRobotVisionCtrl)   // �N���X �t�@�N�g�� �� guid
	DECLARE_OLETYPELIB(CRobotVisionCtrl)     // GetTypeInfo
	DECLARE_PROPPAGEIDS(CRobotVisionCtrl)    // �v���p�e�B �y�[�W ID
	DECLARE_OLECTLTYPE(CRobotVisionCtrl)     // �^�C�v���Ƃ��̑��̃X�e�[�^�X

// ---- Vision
	BOOL m_Hardware;
	HWND m_hWndCap;

public:
	CAPDRIVERCAPS  m_CapDriverCaps;
	CAPSTATUS      m_CapStatus;
	CAPTUREPARMS   m_CapParms;
	unsigned long  m_num_pixels;

	unsigned char** AllocImage();
	void FreeImage(unsigned char**);

	unsigned char** image_r;
	unsigned char** image_g;
	unsigned char** image_b;

	static CRobotVisionCtrl* theCtrl;
	void FrameCallback(LPVIDEOHDR lpVHdr);

	ImageFrame *m_image_wnd;
	CSingleDocTemplate *m_doc_template;

protected:

// ���b�Z�[�W �}�b�v
	//{{AFX_MSG(CRobotVisionCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// �f�B�X�p�b�` �}�b�v
	//{{AFX_DISPATCH(CRobotVisionCtrl)
	CString m_driverName;
	afx_msg void OnDriverNameChanged();
	CString m_driverVersion;
	afx_msg void OnDriverVersionChanged();
	long m_deviceIndex;
	afx_msg void OnDeviceIndexChanged();
	short m_previewRate;
	afx_msg void OnPreviewRateChanged();
	BOOL m_preview;
	afx_msg void OnPreviewChanged();
	BOOL m_overlay;
	afx_msg void OnOverlayChanged();
	BOOL m_previewScale;
	afx_msg void OnPreviewScaleChanged();
	afx_msg short Start();
	afx_msg void Stop();
	afx_msg BOOL SaveBitmap(LPCTSTR filename);
	afx_msg BOOL DlgVideoFormat();
	afx_msg BOOL StartCapture(LPCTSTR filename);
	afx_msg void StopCapture();
	afx_msg BOOL DlgVideoSource();
	afx_msg BOOL DlgVideoCompression();
	afx_msg void ImageWindow();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �C�x���g �}�b�v
	//{{AFX_EVENT(CRobotVisionCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// �f�B�X�p�b�` �� �C�x���g ID
public:
	enum {
	//{{AFX_DISP_ID(CRobotVisionCtrl)
	dispidDriverName = 1L,
	dispidDriverVersion = 2L,
	dispidDeviceIndex = 3L,
	dispidPreviewRate = 4L,
	dispidPreview = 5L,
	dispidOverlay = 6L,
	dispidPreviewScale = 7L,
	dispidStart = 8L,
	dispidStop = 9L,
	dispidSaveBitmap = 10L,
	dispidDlgVideoFormat = 11L,
	dispidStartCapture = 12L,
	dispidStopCapture = 13L,
	dispidDlgVideoSource = 14L,
	dispidDlgVideoCompression = 15L,
	dispidImageWindow = 16L,
	//}}AFX_DISP_ID
	};
};

class ImageFrame : public CFrameWnd {
	DECLARE_DYNCREATE(ImageFrame)
protected:
	virtual ~ImageFrame();
public:
	void FrameCallback();
};

class ImageView : public CView {
	DECLARE_DYNCREATE(ImageView)
protected:
	virtual void OnDraw(CDC *pDC);
};

class ImageDoc : public CDocument {
	DECLARE_DYNCREATE(ImageDoc)
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ROBOTVISIONCTL_H__E0DCC4F0_AAF3_4B63_B183_16CC52B0DD3F__INCLUDED)
