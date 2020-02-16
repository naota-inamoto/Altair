
// AlMm.h : ALMM.DLL のメイン ヘッダー ファイルです

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CAlMmApp : このクラスの動作の定義に関しては AlMm.cpp ファイルを参照してください

class CAlMmApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;


