
// AlCtrl.h : ALCTRL.DLL のメイン ヘッダー ファイルです

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"		// メイン シンボル

#include <afxinet.h>

#include <altair/docvdef.h>

#include <gdm/gdm.h>
#include <gdm/string.h>
#include <gdm/file.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <dev/advwnd.h>
#include <dev/ocx.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <cui/cui.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/body.h>
#include <cui/project.h>

#include <altair/strres.h>

/////////////////////////////////////////////////////////////////////////////
// CAlCtrlApp : このクラスの動作の定義に関しては AlCtrl.cpp ファイルを参照してください

class CAlCtrlApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;


