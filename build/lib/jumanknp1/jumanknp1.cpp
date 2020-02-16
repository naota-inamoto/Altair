// jumanknp1.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE jumanknp1DLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("jumanknp1.DLL 初期化中\n");
		
		// 拡張 DLL を１回だけ初期化します。
		if (!AfxInitExtensionModule(jumanknp1DLL, hInstance))
			return 0;

		// この DLL をリソース チェインへ挿入します。
		// メモ: 拡張 DLL が MFC アプリケーションではなく
		//   MFC 標準 DLL (ActiveX コントロールのような)
		//   に暗黙的にリンクされる場合、この行を DllMain
		//   から削除して、この拡張 DLL からエクスポート
		//   された別の関数内へ追加してください。  
		//   この拡張 DLL を使用する標準 DLL はこの拡張 DLL
		//   を初期化するために明示的にその関数を呼び出します。 
		//   それ以外の場合は、CDynLinkLibrary オブジェクトは
		//   標準 DLL のリソース チェインへアタッチされず、
		//   その結果重大な問題となります。

		new CDynLinkLibrary(jumanknp1DLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("jumanknp1.DLL 終了中\n");

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(jumanknp1DLL);
	}
	return 1;   // OK
}
