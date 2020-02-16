// othello.h : othello.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含む前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"		// メイン シンボル


// CothelloApp
// このクラスの実装に関しては othello.cpp を参照してください。
//

class CothelloApp : public CWinApp
{
public:
	CothelloApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
