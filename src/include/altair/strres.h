/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ALTAIR_STRRES_H
#define _ALTAIR_STRRES_H

#ifdef	AL_LANG_JAPANESE

#define AL_MM_FILE		AL_STR("ファイル")
#define AL_MM_PROJECT	AL_STR("プロジェクト")
#define AL_MM_VIEW		AL_STR("表示")
#define AL_MM_WINDOW	AL_STR("ウィンドウ")
#define AL_MM_HELP		AL_STR("ヘルプ")
#define AL_MM_EXIT		AL_STR("終了")
#define AL_MM_STATUSBAR AL_STR("ステータスバー")
#define AL_MM_MANUAL	AL_STR("オンラインマニュアル")
#define AL_MM_VERSION	AL_STR("バージョン")

#define AL_M_FILE		AL_STR("ファイル")
#define AL_M_NEW		AL_STR("新規作成")
#define AL_M_SAVE		AL_STR("上書き保存")
#define AL_M_SAVE_AS	AL_STR("名前を変えて保存")
#define AL_M_OPEN		AL_STR("開く")
#define AL_M_PRINT		AL_STR("印刷用ファイル")
#define AL_M_EXIT		AL_STR("終了")

#define AL_M_EDIT		AL_STR("編集")
#define AL_M_POS		AL_STR("挿入位置")
#define AL_M_UPPER		AL_STR("上")
#define AL_M_LOWER		AL_STR("下")
#define AL_M_ELDER		AL_STR("兄")
#define AL_M_BROTHER	AL_STR("弟")
#define AL_M_CHILD		AL_STR("子")
#define AL_M_UP 		AL_STR("上へ移動")
#define AL_M_DOWN		AL_STR("下へ移動")
#define AL_M_LEFT		AL_STR("左へ移動")
#define AL_M_RIGHT		AL_STR("右へ移動")
#define AL_M_CUT		AL_STR("カット")
#define AL_M_COPY		AL_STR("コピー")
#define AL_M_PASTE		AL_STR("貼り付け")
#define AL_M_DELETE 	AL_STR("削除")

#define AL_M_WINDOW 	AL_STR("ウィンドウ")
#define AL_M_REDRAW 	AL_STR("再表示")
#define AL_M_PARENT 	AL_STR("親エディタ")
#define AL_M_CLOSE		AL_STR("閉じる")
#define AL_M_CLOSE_ALL	AL_STR("すべて閉じる")

#define AL_M_CLASS	AL_STR("クラス登録")
#define AL_M_REGIST_CLASS_LIST	AL_STR("登録クラスリスト")
#define AL_M_REGIST_CLASS	AL_STR("登録")
#define AL_M_UNREGIST_CLASS AL_STR("登録解除")

#define AL_M_LIB	AL_STR("ライブラリ")
#define AL_M_MKLIB	AL_STR("作成")
#define AL_M_MKLIB_TEXT 	AL_STR("テキスト形式")
#define AL_M_MKLIB_GRAPH	AL_STR("グラフ形式")
#define AL_M_MKLIB_TEXT2	AL_STR("テキスト２形式")
#define AL_M_TEXT2_CLEAR	AL_STR("クリア")
#define AL_M_TEXT2_LIST 	AL_STR("クラスリスト")
#define AL_M_TEXT2_ADD		AL_STR("クラスの追加")
#define AL_M_TEXT2_CREATE	AL_STR("作成")
#define AL_M_LOAD_LIB		AL_STR("ロード")
#define AL_M_LOAD_LIB_TMP	AL_STR("一時ロード")

#define AL_M_INTERP AL_STR("インタプリタ")
#define AL_M_TEST_EXEC	AL_STR("テスト実行")
#define AL_M_EXEC		AL_STR("実行")

#define AL_M_MISC	AL_STR("その他")
#define AL_M_OPEN_WITH_OUTPUT	AL_STR("ソースを出力しながら開く")
#define AL_M_MARK_OUTPUT	AL_STR("アウトプットにマーク")
#define AL_M_MEMORY_SIZE	AL_STR("メモリサイズ")
#define AL_M_GC AL_STR("ガーベージコレクション")
#define AL_M_CLEAR_VTBL AL_STR("VTBLアップデート")

#define AL_T_PROJECT	"プロジェクト"
#define AL_T_CLASS		"クラス  %s"
#define AL_T_MEMBER 	"メンバー  %s"
#define AL_T_BODY		"ボディ  %s"
#define AL_T_OUTPUT 	"アウトプット"

#define AL_GC_NEW_CELL	"回収されたセルの数 = %ld\n"
#define AL_GC_FREE_CELL "元からあったフリーセルの数 = %ld\n"
#define AL_GC_USE_CELL	"使用中のセルの数 = %ld\n"

#define AL_S001 " ) がありません。\n"
#define AL_S002 " . の右側に予期せぬ文字があります。\n"
#define AL_S003 " . の左辺がクラスのオブジェクトではありません。\n"
#define AL_S004 " . の左辺はリストです。\n"
#define AL_S005 " :: の後にクラスのメンバー名がありません。\n"
#define AL_S006 " ; がありません。\n"
#define AL_S007 " ; の後に余計な文字があります。\n"
#define AL_S008 " ? に対応する : がありません。\n"
#define AL_S009 " break の後に ; がありません。\n"
#define AL_S010 " break 文の最後の ; の後に余計な文字があります。\n"
#define AL_S011 " continue の後に ; がありません。\n"
#define AL_S012 " continue 文の最後の ; の後に余計な文字があります。\n"
#define AL_S013 " else の後に { がありません。\n"
#define AL_S014 " else の後の { の後に余計な文字があります。\n"
#define AL_S015 " if 文の最初の { の後に余計な文字があります。\n"
#define AL_S016 " if 文の条件式に ( がありません。\n"
#define AL_S017 " if 文の条件式に ) がありません。\n"
#define AL_S018 " if 文の条件式の後に { がありません。\n"
#define AL_S019 " loop の後に { がありません。\n"
#define AL_S020 " loop 文の最初の { の後に余計な文字があります。\n"
#define AL_S021 " new の後にクラス名がありません。\n"
#define AL_S022 " para の後に関数式がありません。\n"
#define AL_S023 " para 文に ; がありません。\n"
#define AL_S024 " para 文の最後の ; の後に余計な文字があります。\n"
#define AL_S025 " return 文に ; がありません。\n"
#define AL_S026 " return 文に戻り値がありません。\n"
#define AL_S027 " return 文の最後の ; の後に余計な文字があります。\n"
#define AL_S028 " runtime の後にクラス名がありません。\n"
#define AL_S029 " } に対応する { がありません。\n"
#define AL_S030 " } に対応する 複文の {, if, loop がありません。\n"
#define AL_S031 " } の後に余計な文字があります。\n"
#define AL_S032 " 複文の最後の } の後に余計な文字があります。\n"
#define AL_S033 " 複文の最初の { の後に余計な文字があります。\n"
#define AL_S034 "%s はオブジェクトのメンバーではありません。\n"
#define AL_S035 "%s はクラス名ではありません。\n"
#define AL_S036 "%s は型名ではありません。\n"
#define AL_S037 "%s は識別子として認識できません。\n"
#define AL_S038 "%s プロトタイプに一致するメンバー関数がありません。\n"
#define AL_S039 "%s::%s は識別子として認識できません。\n"
#define AL_S040 "DeleteLineでエラーが起きました。\n"
#define AL_S041 "GUIpartsノードが不正です\n"

#define AL_S043 "GUIノードが不正です\n"
#define AL_S044 "InsertLineでエラーが起きました。\n"
#define AL_S045 "public:, protected:, private:がありません。\n"

#define AL_S047 "ReplaceLineでエラーが起きました。\n"
#define AL_S048 "static もしくは型名がありません。\n"
#define AL_S049 "Translate:: 関数の中身が定義されていません。\n"
#define AL_S050 "void はローカル変数の型として使えません。\n"
#define AL_S051 "このクラスもしくはこの派生クラスが外部参照しているのでグラフ形式のライブラリにはできません。\n"
#define AL_S052 "このクラスもしくはその派生クラスが参照されているため削除できません。\n"
#define AL_S053 "このプロジェクトは %ld バイトのメモリをプログラムに使用しています。\n"
#define AL_S054 "このメンバーは参照されているのでプロトタイプを変更できません。\n"
#define AL_S055 "これが最下位のクラスです。\n"
#define AL_S056 "これが最下位のプロジェクトです。\n"
#define AL_S057 "これが最下位のメンバーです。\n"
#define AL_S058 "これが最上位のクラスです。\n"
#define AL_S059 "これが最上位のプロジェクトです。\n"
#define AL_S060 "これが最上位のメンバーです。\n"
#define AL_S061 "インタプリタ実行でエラーが起きました。（ %s ）\n"
#define AL_S062 "オブジェクトポートのソースに型名がありません。\n"
#define AL_S063 "カーソルのある場所と行を挿入する場所が違います。\n"
#define AL_S064 "カーソルは %ld行目、挿入場所は %ld行目です。\n"
#define AL_S065 "カットバッファがオープンできません。\n"
#define AL_S066 "キャスト式で ) がありません。\n"
#define AL_S067 "クラス %s がプロジェクトに登録されました。\n"
#define AL_S068 "クラス %s が定義されていません。\n"
#define AL_S069 "クラス %s はプロジェクトから登録解除されました。\n"
#define AL_S070 "クラス %s はプロジェクトに登録されているため削除できません。\n"
#define AL_S071 "クラス %s は既に登録されています。\n"
#define AL_S072 "クラス %s は登録されていません。\n"
#define AL_S073 "クラスGraphが見つかりません。\n"
#define AL_S074 "クラスGUIbuilderが見つかりません。\n"
#define AL_S075 "クラスのリスト\n"
#define AL_S076 "クラスを指定してください。\n"
#define AL_S077 "クラスを選択してください。\n"
#define AL_S078 "クラス名がありません。\n"
#define AL_S079 "クラス名がクラスを表わしていません。\n"
#define AL_S080 "クラス名が文字列になっていません。\n"
#define AL_S081 "クラス名の後の :: がありません。\n"
#define AL_S082 "グラフ形式ライブラリ %s が作成されました。(%s)\n"
#define AL_S083 "グラフ形式ライブラリ %s を作成中 . . . (%s)\n"
#define AL_S084 "コメント文に予期せぬEOFがあります。\n"
#define AL_S085 "コントロール出力ポートが見つかりません。\n"
#define AL_S086 "コントロール文の置き換えはできません。\n"
#define AL_S087 "システム関数 %s の引き数の数があっていません。\n"
#define AL_S088 "テキスト形式ライブラリ %s が作成中されました。(%s)\n"
#define AL_S089 "テキスト形式ライブラリ %s を作成中 . . . (%s)\n"
#define AL_S090 "テンポラリファイルに文法エラーがあります。\n"
#define AL_S091 "テンポラリファイルに予期せぬEOFがあります。\n"
#define AL_S092 "データ出力ポートのデストがオブジェクトノードでありません。\n"
#define AL_S093 "データ出力ポートのデストに型名が見つかりません。\n"
#define AL_S094 "データ出力ポート先がオブジェクトノードでありません。\n"
#define AL_S095 "データ入力ポートが見つかりません。\n"
#define AL_S096 "データ入力ポートのソースに型名がありません。\n"

#define AL_S099 "トランスレートが正常に終了しました。\n"
#define AL_S100 "トランスレート中にエラーが発生しました。\n"
#define AL_S101 "ノードが関数ノードでありません。\n"
#define AL_S102 "ノードが関数ノードでもマクロノードでもありません。\n"
#define AL_S103 "ノード数が65535を越えるグラフはコピーできません。\n"
#define AL_S104 "ファイル %s がオープンできません。\n"
#define AL_S105 "ファイル %s をオープンできませんでした。\n"
#define AL_S106 "ファイル %s を読み込み中にエラーが発生しました。\n"
#define AL_S107 "ファイルがオープンできません。\n"
#define AL_S108 "ファイルが存在しません。\n"
#define AL_S109 "ファイルのオープンに失敗しました。\n"
#define AL_S110 "ファイル書き込みでエラーが発生しました。\n"
#define AL_S111 "ファイル読み込み中にエラーが発生しました。\n"
#define AL_S112 "プロジェクトがオープンされました。\n"
#define AL_S113 "プロジェクトを選択してください。\n"
#define AL_S114 "プロジェクトクラスをロード中 . . .\n"
#define AL_S115 "ベースとなるクラス %s が定義されていません。\n"
#define AL_S116 "ベースとなるクラスが見つかりません。\n"
#define AL_S117 "ベースとなるクラスが定義されていません。\n"

#define AL_S119 "マクロに入ってコントロールアークをたどれなくなりました。\n"

#define AL_S121 "メンバーを選択してください。\n"
#define AL_S122 "メンバー変数の元がconstになっています。\n"
#define AL_S123 "メンバー変数名が見つかりません。\n"
#define AL_S124 "メンバー名がありません。\n"
#define AL_S125 "ライブラリ %s が見つかりません。\n"
#define AL_S126 "ライブラリ %s をロード中 . . .\n"
#define AL_S127 "ルートクラス AlObject はライブラリ化できません。\n"
#define AL_S128 "ルートクラス AlObject は削除できません。\n"
#define AL_S129 "ローカル宣言の最後に ; がありません。\n"
#define AL_S130 "ローカル変数は { } の間でしか使えません。\n"
#define AL_S131 "ローカル変数宣言で型名がありません。\n"
#define AL_S132 "ローカル変数名宣言に誤りがあります。\n"
#define AL_S133 "印刷でエラーが発生しました。\n"
#define AL_S134 "引き数なしのpublicクラス関数からしかテスト実行できません。\n"
#define AL_S135 "引き数なしのpublicクラス関数からしか実行できません。\n"
#define AL_S136 "引き数の型名 %s は宣言されていません。\n"
#define AL_S137 "引き数の型名がありません。\n"
#define AL_S138 "引き数名がありません。\n"
#define AL_S139 "引き数名が見つかりません。\n"
#define AL_S140 "括弧式の閉じ括弧 ) がありません。\n"
#define AL_S141 "関数 %s の引き数に間違いがあります。\n"
#define AL_S142 "関数 %s はプロトタイプに一致する関数がありません。\n"
#define AL_S143 "関数Graph::SrcNodeが見つかりません。\n"
#define AL_S144 "関数GUIbuilder::GetPartsが見つかりません。\n"
#define AL_S145 "関数GUIbuilder::LoadAndCreateWindowが見つかりません。\n"
#define AL_S146 "関数のベースクラスが見つかりません。\n"
#define AL_S147 "関数引き数の関数が見つかりません。\n"
#define AL_S148 "関数引き数の最後の ) がありません。\n"
#define AL_S149 "関数引き数リストの最後の ) がありません。\n"
#define AL_S150 "関数引き数リストの最初の ( がありません。\n"
#define AL_S151 "関数定義が見つかりません。\n"
#define AL_S152 "関数定義が見つかりません。おそらく引き数の型が間違っています。\n"
#define AL_S153 "既にあるライブラリが優先されます。\n"
#define AL_S154 "既にオープンされています。\n"
#define AL_S155 "型名 %s は宣言されていません。\n"
#define AL_S156 "型名がありません。\n"
#define AL_S157 "最初のサブクラスなので右へ移動できません。\n"

#define AL_S159 "子クラスを選択してください。\n"
#define AL_S160 "式の最後に ; がありません。\n"
#define AL_S161 "出力ポートのデストにメンバー変数名が見つかりません。\n"
#define AL_S162 "深さのより浅い制御文にまたがって削除することはできません。\n"
#define AL_S163 "対象オブジェクトが定数になっています\n"
#define AL_S164 "代入式の左辺が左辺値でありません。\n"
#define AL_S165 "定義されていないconstのデータです。\n"
#define AL_S166 "定義されていないlistのデータです。\n"
#define AL_S167 "定数式に誤りがあります。\n"
#define AL_S168 "同じクラス名が存在します。\n"
#define AL_S169 "同じプロとタイプを持った関数が既にあります。\n"
#define AL_S170 "同じプロトタイプのメンバーが他にあります。\n"
#define AL_S171 "同じ名前の関数があります。\n"
#define AL_S172 "同じ名前の変数があります。\n"
#define AL_S173 "同一名のプロジェクトが存在します。\n"
#define AL_S174 "同一名のライブラリが既にロードされています。\n"
#define AL_S175 "文がありません。\n"
#define AL_S176 "文字 * の近くで文法エラーがあります。\n"
#define AL_S177 "文法が間違っています。\n"
#define AL_S178 "文法エラーがあります。\n"
#define AL_S179 "未定義のファイルフォーマットです。\n"
#define AL_S180 "AlObjectの名前は変えられません。\n"
#define AL_S181 "クラス登録されているクラスの名前は変えられません。\n"
#define AL_S200 "テキスト２形式ライブラリ %s が作成中されました。\n"
#define AL_S201 "テキスト２形式ライブラリ %s を作成中 . . .\n"
#define AL_S202 "クラス関数ポインタがNULLです。\n"
#define AL_S203 "予約語は引数名として使えません。"
#define AL_S204 "予約語は変数名として使えません。"
#define AL_S205 "予約語はメンバー名として使えません。"
#define AL_S206 "static関数中でthisは使えません。"
#define AL_S207 "static関数中でstaticでないメンバー関数は使えません。"
#define AL_S208 "static関数中でstaticでないメンバー変数は使えません。"
#define AL_S209 "'%s'は、protected: または private: で隠蔽されています。"

#define AL_S210 "Windows3.1では64Kを超えるサイズのビットマップは読み込めません。\n"
#define AL_S211 "ソースを出力しながらロードするモードになりました。"
#define AL_S212 "ソースを出力しないでロードするモードになりました。"

#define AL_S400 " try 文に { がありません。\n"
#define AL_S401 " try 文の { の後に余計な文字があります。\n"
#define AL_S402 " catch の後に ( がありません。\n"
#define AL_S403 " catch の後の ( の後に例外クラス名がありません。\n"
#define AL_S404 " catch の後の例外クラス名の後に変数名がありません。\n"
#define AL_S405 " catch の後の変数名の後に ) がありません。\n"
#define AL_S406 " catch の後の ) の後に { がありません。\n"
#define AL_S407 " catch の後の { の後に余計な文字があります。\n"
#define AL_S408 " throw の後に式がありません。\n"
#define AL_S409 " throw の後の式の後に ; がありません。\n"
#define AL_S410 " throw の後の ; の後に余計な文字があります。\n"

#define AL_O1	"OLEの初期化に失敗しました。\n"
#define AL_O2	"%s: オートメーション引数は32個までです。"
#define AL_O3	"%s: オートメーションの対象がIDispatchでありません。"
#define AL_O4	"%s: オートメーションメソッドが見つかりません。"
#define AL_O5	"%s: OLE例外が発生しました。"
#define AL_O6	"%s: OLEディスパッチ例外が発生しました。"
#define AL_O7	"%s: メモリ例外が発生しました。"
#define AL_O8	"%s: オートメーションプロパティが見つかりません。"
#define AL_O9	"OLEオートメーションはサポートされていません。\n"
#define AL_O10	"%s: IDispatchがNULLです。"

#endif	/* AL_LANG_JAPANESE */

#ifdef	AL_LANG_ENGLISH

#define AL_MM_FILE		AL_STR("File")
#define AL_MM_PROJECT	AL_STR("Project")
#define AL_MM_VIEW		AL_STR("View")
#define AL_MM_WINDOW	AL_STR("Window")
#define AL_MM_HELP		AL_STR("Help")
#define AL_MM_EXIT		AL_STR("Exit")
#define AL_MM_STATUSBAR AL_STR("StatsuBar")
#define AL_MM_MANUAL	AL_STR("Online Manual")
#define AL_MM_VERSION	AL_STR("Version")

#define AL_M_FILE		AL_STR("File")
#define AL_M_NEW		AL_STR("New")
#define AL_M_SAVE		AL_STR("Save")
#define AL_M_SAVE_AS	AL_STR("Save As")
#define AL_M_OPEN		AL_STR("Open")
#define AL_M_PRINT		AL_STR("File for Print")
#define AL_M_EXIT		AL_STR("Exit")

#define AL_M_EDIT		AL_STR("Edit")
#define AL_M_POS		AL_STR("Insert Position")
#define AL_M_UPPER		AL_STR("Upper")
#define AL_M_LOWER		AL_STR("Lower")
#define AL_M_ELDER		AL_STR("Elder Brother")
#define AL_M_BROTHER	AL_STR("Brother")
#define AL_M_CHILD		AL_STR("Child")
#define AL_M_UP 		AL_STR("Move Up")
#define AL_M_DOWN		AL_STR("Move Down")
#define AL_M_LEFT		AL_STR("Move Left")
#define AL_M_RIGHT		AL_STR("Move Right")
#define AL_M_CUT		AL_STR("Cut")
#define AL_M_COPY		AL_STR("Copy")
#define AL_M_PASTE		AL_STR("Paste")
#define AL_M_DELETE 	AL_STR("Delete")

#define AL_M_WINDOW 	AL_STR("Window")
#define AL_M_REDRAW 	AL_STR("Redraw")
#define AL_M_PARENT 	AL_STR("Parent Editor")
#define AL_M_CLOSE		AL_STR("Close")
#define AL_M_CLOSE_ALL	AL_STR("Close All")

#define AL_M_CLASS	AL_STR("Class Registration")
#define AL_M_REGIST_CLASS_LIST	AL_STR("Registrated Class List")
#define AL_M_REGIST_CLASS	AL_STR("Regist Class")
#define AL_M_UNREGIST_CLASS AL_STR("Unregist Class")

#define AL_M_LIB	AL_STR("Library")
#define AL_M_MKLIB	AL_STR("Create")
#define AL_M_MKLIB_TEXT 	AL_STR("Text Format")
#define AL_M_MKLIB_GRAPH	AL_STR("Graph Format")
#define AL_M_MKLIB_TEXT2	AL_STR("Text2 Format")
#define AL_M_TEXT2_CLEAR	AL_STR("Clear")
#define AL_M_TEXT2_LIST 	AL_STR("Class List")
#define AL_M_TEXT2_ADD		AL_STR("Add Class")
#define AL_M_TEXT2_CREATE	AL_STR("Create")
#define AL_M_LOAD_LIB		AL_STR("Load")
#define AL_M_LOAD_LIB_TMP	AL_STR("Temp Load")

#define AL_M_INTERP AL_STR("Interpreter")
#define AL_M_TEST_EXEC	AL_STR("Test Execution")
#define AL_M_EXEC		AL_STR("Execution")

#define AL_M_MISC	AL_STR("Others")
#define AL_M_OPEN_WITH_OUTPUT	AL_STR("Open width Soruce Output")
#define AL_M_MARK_OUTPUT	AL_STR("Mark Output")
#define AL_M_MEMORY_SIZE	AL_STR("Momory Size")
#define AL_M_GC AL_STR("Garbage Collection")
#define AL_M_CLEAR_VTBL AL_STR("update VTBL")

#define AL_T_PROJECT	"Project"
#define AL_T_CLASS		"Class  %s"
#define AL_T_MEMBER 	"Member  %s"
#define AL_T_BODY		"Body  %s"
#define AL_T_OUTPUT 	"Output"

#define AL_GC_NEW_CELL	"number of collected cells = %ld\n"
#define AL_GC_FREE_CELL "number of free cells = %ld\n"
#define AL_GC_USE_CELL	"number of cells in use = %ld\n"

#define AL_FONT "System"

#define AL_S001 " expected ')' .\n"
#define AL_S002 " unexpected character on right hand side of '.' .\n"
#define AL_S003 " left hand side of '.' is not instance of class.\n"
#define AL_S004 " left hand side of '.' ls list.\n"
#define AL_S005 " after '::' member name of class is missing.\n"
#define AL_S006 " expected ';' .\n"
#define AL_S007 " unexpected character after ';' .\n"
#define AL_S008 " expected ':' corresponding to '?' .\n"
#define AL_S009 " expected ';' after 'break'.\n"
#define AL_S010 " unexpected character after 'break'.\n"
#define AL_S011 " expected ';' after 'continue'.\n"
#define AL_S012 " unexpected character after 'continue'.\n"
#define AL_S013 " expected '{' corresponding to 'else'."
#define AL_S014 " unexpected character after '} else {'.\n"
#define AL_S015 " unexpected character after 'if (expression) {'.\n"
#define AL_S016 " expected '(' for condition of if statement.\n"
#define AL_S017 " expected ')' for condition of if statement.\n"
#define AL_S018 " expected '{' for if statement.\n"
#define AL_S019 " expected '{' after 'loop'.\n"
#define AL_S020 " unexpected character after 'loop {'.\n"
#define AL_S021 " there is no class name after 'new'.\n"
#define AL_S022 " there is no function expression after 'para'.\n"
#define AL_S023 " expected ';' after 'para' statement.\n"
#define AL_S024 " unexpected character after 'para' statement.\n"
#define AL_S025 " ';' is missing in 'return' statement.\n"
#define AL_S026 " return value is missing in 'return' statement.\n"
#define AL_S027 " unexpected character after 'return' statement.\n"
#define AL_S028 " class name is missing after 'runtime'.\n"
#define AL_S029 " expected '}' corresponding to '{'.\n"
#define AL_S030 " '{', 'if' and 'loop' are missing which correspond to '}'.\n"
#define AL_S031 " unexpected character after '}'.\n"
#define AL_S032 " unexpected character after '}'.\n"
#define AL_S033 " unexpected character after '{'.\n"
#define AL_S034 "%s is not a member of object.\n"
#define AL_S035 "%s is not a class name.\n"
#define AL_S036 "%s is not a type name\n"
#define AL_S037 "%s can not be recognized as an identifier.\n"
#define AL_S038 "function %s does not match any prototype.\n"
#define AL_S039 "%s::%s can not be recognized as an identifier.\n"
#define AL_S040 "error occurres in DeleteLine.\n"
#define AL_S041 "GUIparts node is illegal.\n"

#define AL_S043 "GUI node is illegal.\n"
#define AL_S044 "error occurres in InsertLine.\n"
#define AL_S045 "no 'public:', 'protected:', 'private:'.\n"

#define AL_S047 "error occurred in ReplaceLine.\n"
#define AL_S048 "'static' or type name is missing.\n"
#define AL_S049 "Translate:: function is not defined.\n"
#define AL_S050 "void can not be used as a type for local variable.\n"
#define AL_S051 "can't make this class library because this class or its derived class have outside reference.\n"
#define AL_S052 "can't delete this class because this class is refered to.\n"
#define AL_S053 "this project uses %d bytes memory for program.\n"
#define AL_S054 "can't change prototype of this member because it is refered to.\n"
#define AL_S055 "this class is the lowest.\n"
#define AL_S056 "this project is the lowest.\n"
#define AL_S057 "this member is the lowest\n"
#define AL_S058 "this class is the highest\n"
#define AL_S059 "this project is the highest.\n"
#define AL_S060 "this member is the highest.\n"
#define AL_S061 "interpreter error occurred. (%s)\n"
#define AL_S062 "source of object port does not have a type name.\n"
#define AL_S063 "caret position and insert position is defferent.\n"
#define AL_S064 "caret is at the line %ld, insert position is al the line %ld.\n"
#define AL_S065 "can't open cutbuffer.\n"
#define AL_S066 "expected ')' at the case expression.\n"
#define AL_S067 "class %s is registrated into the project.\n"
#define AL_S068 "class %s is not defined.\n"
#define AL_S069 "class %s is unregistrated from the project.\n"
#define AL_S070 "can't delete class %s because it is registrated.\n"
#define AL_S071 "class %s is already registrated.\n"
#define AL_S072 "class %s is not registrated.\n"
#define AL_S073 "can't find class Graph.\n"
#define AL_S074 "can't find class GUIbuilder.\n"
#define AL_S075 "List of Classes\n"
#define AL_S076 "please specify class.\n"
#define AL_S077 "please select class.\n"
#define AL_S078 "no class name.\n"
#define AL_S079 "this name is not a class name.\n"
#define AL_S080 "argument representing class is not string.\n"
#define AL_S081 "expected '::' after class name.\n"
#define AL_S082 "graph-format library %s is created. (%s)\n"
#define AL_S083 "creating graph-format library %s . . . (%s)\n"
#define AL_S084 "unexpected EOF in comment statement.\n"
#define AL_S085 "can't find output control port.\n"
#define AL_S086 "can't replace control statement.\n"
#define AL_S087 "number of arguments system function %s is not matched.\n"
#define AL_S088 "text-format library %s is created. (%s)\n"
#define AL_S089 "creating text-format library %s  . . . (%s)\n"
#define AL_S090 "there is syntax error in temporary file.\n"
#define AL_S091 "unexpected EOF in temporary file.\n"
#define AL_S092 "destination of output data port is not object node.\n"
#define AL_S093 "can't find type name in destination of output data port.\n"
#define AL_S094 "destination of output data port is not object node.\n"
#define AL_S095 "can't find input data port.\n"
#define AL_S096 "can't find type name in soruce of input data port.\n"

#define AL_S099 "translation is succeded.\n"
#define AL_S100 "translation if failed.\n"
#define AL_S101 "node is not function node.\n"
#define AL_S102 "node is not function node nor macro node\n"
#define AL_S103 "can't copy graph if number of nodes of the graph is grater than 65535. \n"
#define AL_S104 "can't open file %s.\n"
#define AL_S105 "can't open file %s.\n"
#define AL_S106 "file read error occurred at the  file %s.\n"
#define AL_S107 "can't open file.\n"
#define AL_S108 "file does not exist.\n"
#define AL_S109 "can't open file.\n"
#define AL_S110 "file write error occurred.\n"
#define AL_S111 "file read error occurred.\n"
#define AL_S112 "the project is opend.\n"
#define AL_S113 "please select project.\n"
#define AL_S114 "loading project classes . . .\n"
#define AL_S115 "base-class %s is not defined.\n"
#define AL_S116 "can't find base-class.\n"
#define AL_S117 "base-class is not defined.\n"

#define AL_S119 "can't traversal control arc in macro.\n"

#define AL_S121 "please select mamber.\n"
#define AL_S122 "root of member variable is const.\n"
#define AL_S123 "can't find member variable.\n"
#define AL_S124 "can't find member name.\n"
#define AL_S125 "can't find library %s.\n"
#define AL_S126 "loading library %s . . .\n"
#define AL_S127 "can't make root Class AlObject library.\n"
#define AL_S128 "can't delete root class AlObject.\n"
#define AL_S129 "expected ';' after local variable declaration.\n"
#define AL_S130 "local variables can be used only between '{' and '}'.\n"
#define AL_S131 "expected type name in local variable declaration.\n"
#define AL_S132 "there is syntax error in local variable declaration.\n"
#define AL_S133 "print error occurred.\n"
#define AL_S134 "only public class-function without arguments invokes test-execution.\n"
#define AL_S135 "only public class-function without arguments invokes execution.\n"
#define AL_S136 "argument type name %s is not defined.\n"
#define AL_S137 "no argument type name.\n"
#define AL_S138 "no argument name.\n"
#define AL_S139 "can't find argument name.\n"
#define AL_S140 "expected ')' corresponding to '('.\n"
#define AL_S141 "there is an error in armugent of function %s.\n"
#define AL_S142 "there is no function that prototypes is same with function %s.\n"
#define AL_S143 "can't find function Graph::SrcNode.\n"
#define AL_S144 "can't find function GUIbuilder::GetParts.\n"
#define AL_S145 "can't find function GUIbuilder::LoadAndCreateWindow.\n"
#define AL_S146 "can't find base-class of function.\n"
#define AL_S147 "can't find function.\n"
#define AL_S148 "expected ')' for last of function argument list.\n"
#define AL_S149 "expected ')' for last of function argument list.\n"
#define AL_S150 "expected '(' for the first of function argument list.\n"
#define AL_S151 "can't find function definition.\n"
#define AL_S152 "can't find function deficition. argument type will be incorrect.\n"
#define AL_S153 "first loaded library is used.\n"
#define AL_S154 "already opened.\n"
#define AL_S155 "type name %s is not defined.\n"
#define AL_S156 "no type name.\n"
#define AL_S157 "can't move right because it is first subclass.\n"

#define AL_S159 "please select child class.\n"
#define AL_S160 "expected ';' last of expression.\n"
#define AL_S161 "can't find member variable in destination of output port.\n"
#define AL_S162 "can't delete statements over the deferent depth.\n"
#define AL_S163 "object is constant.\n"
#define AL_S164 "left hand side of assignment statement is not LHS value.\n"
#define AL_S165 "undefined const data.\n"
#define AL_S166 "undefined list data.\n"
#define AL_S167 "there is an error in constant expression.\n"
#define AL_S168 "there is a class that name is same with.\n"
#define AL_S169 "there is a function that prototype is same with\n"
#define AL_S170 "there is a function that prototype is same with.\n"
#define AL_S171 "there is a function that name is same with.\n"
#define AL_S172 "there is a variable that name is same with.\n"
#define AL_S173 "there is a project that name is same with.\n"
#define AL_S174 "same name library is already loaded.\n"
#define AL_S175 "no statement.\n"
#define AL_S176 "there is syntax error near '*'.\n"
#define AL_S177 "syntax error.\n"
#define AL_S178 "syntax error.\n"
#define AL_S179 "undefined file format.\n"
#define AL_S180 "class name of AlObject can't be changed.\n"
#define AL_S181 "name of registered class can't be changed.\n"
#define AL_S200 "text2-format library %s is created.\n"
#define AL_S201 "creating text2-format library %s  . . .\n"
#define AL_S202 "class function pointer is null.\n"
#define AL_S203 "preserved identifier cannot be used as an argument name."
#define AL_S204 "preserved identifier cannot be used as a variable name."
#define AL_S205 "preserved identifier cannot be used as a member name."
#define AL_S206 "'this' cannot be used in static functions."
#define AL_S207 "not-static-member function cannot be used in static functions."
#define AL_S208 "not-static-member variable cannot be used in static functions."
#define AL_S209 "'%s' is concealed by 'protected:' or 'private:'"
#define AL_S210 "In Windows3.1, can't read bitmap size of which is greater than 64K.\n"
#define AL_S211 "load mode with source output."
#define AL_S212 "load mode without source output."

#define AL_S400 " '{' missing after 'try'.\n"
#define AL_S401 " unexpected token after 'try {'.\n"
#define AL_S402 " '(' missing after 'catch'.\n"
#define AL_S403 " Exception classname missing after 'catch ('.\n"
#define AL_S404 " variable name missing after Exception classname.\n"
#define AL_S405 " ')' missing after variable name.\n"
#define AL_S406 " '{' missing after ')'.\n"
#define AL_S407 " unexpected token after '{'.\n"
#define AL_S408 " expession missing after 'throw'.\n"
#define AL_S409 " ';' missing after expression.\n"
#define AL_S410 " unexpected token after ';'\n"

#define AL_O1	"fail to initialize OLE.\n"
#define AL_O2	"%s: maximun size of arguments is 32."
#define AL_O3	"%s: autometion object is not IDispatch."
#define AL_O4	"%s: can't find autometion method."
#define AL_O5	"%s: COleException occurs."
#define AL_O6	"%s: COleDispatchException occurs."
#define AL_O7	"%s: CMemoryException occurs."
#define AL_O8	"%s: can7t find automation property."
#define AL_O9	"OLE automation is not supported.\n"
#define AL_O10	"%s: IDispatch is NULL."

#endif	/* AL_LANG_ENGLISH */

#endif
