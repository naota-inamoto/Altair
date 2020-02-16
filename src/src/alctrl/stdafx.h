
// stdafx.h : 標準のシステム インクルード ファイル、
//		 または参照回数が多く、かつあまり変更されない
//	   プロジェクト専用のインクルード ファイルを記述します。

#define VC_EXTRALEAN		// Windows ヘッダーから使用されていない部分を除外 

#define	WINVER			0x0501
#define	_WIN32_WINNT	0x0501
#define	_WIN32_WINDOWS	0x0501
#define	_WIN32_IE		0x0501

#include <afxctl.h> 		// MFC の OLE コントロール

//	MFC のデータベース クラスを使用しない場合は、以下の 2 行のインクルード
//	ファイルの記述を削除してください。
//#ifndef _UNICODE
//#include <afxdb.h>			// MFC のデータベース クラス
//#include <afxdao.h> 		// MFC の DAO データベース クラス
//#endif //_UNICODE

#include <altair/libinc.h>
