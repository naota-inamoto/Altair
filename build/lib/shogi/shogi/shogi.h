// shogi.h : shogi.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CshogiApp
// このクラスの実装に関しては shogi.cpp を参照してください。
//

class CshogiApp : public CWinApp
{
public:
	CshogiApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
