#if !defined(AFX_ROBOTVISION_H__A2D4CDA5_B9CA_4EC2_B8E3_B6C81118F4B3__INCLUDED_)
#define AFX_ROBOTVISION_H__A2D4CDA5_B9CA_4EC2_B8E3_B6C81118F4B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RobotVision.h : ROBOTVISION.DLL のメイン ヘッダー ファイルです

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CRobotVisionApp : このクラスの動作の定義に関しては RobotVision.cpp ファイルを参照してください

class CRobotVisionApp : public COleControlModule
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

#endif // !defined(AFX_ROBOTVISION_H__A2D4CDA5_B9CA_4EC2_B8E3_B6C81118F4B3__INCLUDED)
