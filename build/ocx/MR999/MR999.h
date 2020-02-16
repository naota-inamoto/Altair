#if !defined(AFX_MR999_H__829F1628_C6F9_4F0D_846D_AC7B6A06048A__INCLUDED_)
#define AFX_MR999_H__829F1628_C6F9_4F0D_846D_AC7B6A06048A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MR999.h : MR999.DLL のメイン ヘッダー ファイルです

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMR999App : このクラスの動作の定義に関しては MR999.cpp ファイルを参照してください

class CMR999App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MR999_H__829F1628_C6F9_4F0D_846D_AC7B6A06048A__INCLUDED)
