#if !defined(AFX_MR999CTL_H__463543CD_587D_4D93_8886_442292D4BCD3__INCLUDED_)
#define AFX_MR999CTL_H__463543CD_587D_4D93_8886_442292D4BCD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MR999Ctl.h : CMR999Ctrl ActiveX コントロール クラスの宣言

/////////////////////////////////////////////////////////////////////////////
// CMR999Ctrl : このクラスの動作の定義に関しては MR999Ctl.cpp を参照してください

#define	MR999_CTRL_TIMER_EVENT_ID	211

class CMR999Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CMR999Ctrl)

// コンストラクタ
public:
	CMR999Ctrl();

// オーバーライド
	// ClassWizard は仮想関数のオーバライドを生成します
	//{{AFX_VIRTUAL(CMR999Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	~CMR999Ctrl();

	DECLARE_OLECREATE_EX(CMR999Ctrl)   // クラス ファクトリ と guid
	DECLARE_OLETYPELIB(CMR999Ctrl)     // GetTypeInfo
	DECLARE_PROPPAGEIDS(CMR999Ctrl)    // プロパティ ページ ID
	DECLARE_OLECTLTYPE(CMR999Ctrl)     // タイプ名とその他のステータス

// ---- MR-999 Control

	HANDLE m_hCom;
	COMMTIMEOUTS m_ComTime;
	DCB m_Dcb;
	int m_state;
	int m_GrpDir;
	int m_GrpDirPrev;
	int m_GrpStop;
	int m_WstDir;
	int m_WstDirPrev;
	int m_WstStop;
	int m_ElbDir;
	int m_ElbDirPrev;
	int m_ElbStop;
	int m_SldDir;
	int m_SldDirPrev;
	int m_SldStop;
	int m_BaseDir;
	int m_BaseDirPrev;
	int m_BaseStop;

public:
	static CMR999Ctrl* theCtrl;
	void TimerCB();

protected:
// メッセージ マップ
	//{{AFX_MSG(CMR999Ctrl)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// ディスパッチ マップ
	//{{AFX_DISPATCH(CMR999Ctrl)
	CString m_portName;
	afx_msg void OnPortNameChanged();
	short m_timerPeriod;
	afx_msg void OnTimerPeriodChanged();
	afx_msg BOOL Start();
	afx_msg void Stop();
	afx_msg void GrpClose();
	afx_msg void GrpOpen();
	afx_msg void GrpStop();
	afx_msg void WstLeft();
	afx_msg void WstRight();
	afx_msg void WstStop();
	afx_msg void ElbDown();
	afx_msg void ElbUp();
	afx_msg void ElbStop();
	afx_msg void SldDown();
	afx_msg void SldUp();
	afx_msg void SldStop();
	afx_msg void BaseCw();
	afx_msg void BaseCcw();
	afx_msg void BaseStop();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// イベント マップ
	//{{AFX_EVENT(CMR999Ctrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// ディスパッチ と イベント ID
public:
	enum {
	//{{AFX_DISP_ID(CMR999Ctrl)
	dispidPortName = 1L,
	dispidTimerPeriod = 2L,
	dispidStart = 3L,
	dispidStop = 4L,
	dispidGrpClose = 5L,
	dispidGrpOpen = 6L,
	dispidGrpStop = 7L,
	dispidWstLeft = 8L,
	dispidWstRight = 9L,
	dispidWstStop = 10L,
	dispidElbDown = 11L,
	dispidElbUp = 12L,
	dispidElbStop = 13L,
	dispidSldDown = 14L,
	dispidSldUp = 15L,
	dispidSldStop = 16L,
	dispidBaseCw = 17L,
	dispidBaseCcw = 18L,
	dispidBaseStop = 19L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MR999CTL_H__463543CD_587D_4D93_8886_442292D4BCD3__INCLUDED)
