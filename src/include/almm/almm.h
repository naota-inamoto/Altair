
// AlMm.h : ALMM.DLL �̃��C�� �w�b�_�[ �t�@�C���ł�

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CAlMmApp : ���̃N���X�̓���̒�`�Ɋւ��Ă� AlMm.cpp �t�@�C�����Q�Ƃ��Ă�������

class CAlMmApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;


