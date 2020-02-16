
// AlCtrl.cpp : CAlCtrlApp �N���X�̃C���v�������e�[�V��������� DLL �̓o�^���s���܂��B

#include "stdafx.h"
#include "AlCtrl/AlCtrl.h"

#include <sql/sql.h>

#include <net/socket.h>
#include <net/crypt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAlCtrlApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xcbe7bc00, 0x1f9e, 0x11d0, { 0xb3, 0x4e, 0x44, 0x45, 0x53, 0x54, 0, 0 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CAlCtrlApp::InitInstance - DLL �̏�����

static BOOL bInit;
BOOL CAlCtrlApp::InitInstance()
{
	bInit = COleControlModule::InitInstance();
	if (bInit) {
		AfxOleLockApp();
		ADvSocket::Initialize();
	}
	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CAlCtrlApp::ExitInstance - DLL �̏I��

int CAlCtrlApp::ExitInstance()
{
	if (bInit) {
		AGmPNumMgr::Finalize();
		ADvSocket::Finalize();
		ADvSQLFinalize();

		AfxOleUnlockApp();
	}
	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - �V�X�e�� ���W�X�g���̃G���g����ǉ�

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - �V�X�e�� ���W�X�g������̃G���g�����폜

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


