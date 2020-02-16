
#if !defined(AFX_ALREG_H__22E6010C_0815_11D2_86B0_444553540000__INCLUDED_)
#define AFX_ALREG_H__22E6010C_0815_11D2_86B0_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// AlReg.h : ALREG.DLL のメイン ヘッダー ファイルです

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CAlRegApp : このクラスの動作の定義に関しては AlReg.cpp ファイルを参照してください

class CAlRegApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ALREG_H__22E6010C_0815_11D2_86B0_444553540000__INCLUDED)
