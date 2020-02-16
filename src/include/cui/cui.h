/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AUI_CUI_H
#define _AUI_CUI_H

class AL_EXT_CLASS AUiProgramMgr : public AGm {
public:
	AUiProgramMgr();
	virtual ~AUiProgramMgr();
	static AUiProgramMgr* Instance();

	static AGmNode *CreateCommonClasses();
	
	AGmNode* LibSearchPath();
	AGmNode* LibList();
	AGmNode* ProjectList();

	int LoadLibrary(AGmNode *proj_root_class, AGmString *lib_name, AGmNode** lib);

	AGmNode *_lib_search_path;
	AGmNode *_lib_list;
	AGmNode *_proj_list;

public:
	static int New();
	static int Open();
	static int _Open(AGmString *dir, AGmString *name);
	static int __Open(AGmString *path, AGmNode* proj, int inet=0);
	static int Save(AGmNode *root_class);
	static int SaveAs(AGmNode *root_class);
	static int _Save(AGmString *path, AGmNode *root);
	static void Exit();

	static int MakeLibText(AGmNode *root_class, AGmNode *root);
	static int _MakeLibText(AGmFile*, AGmNode* root_class, AGmNode *root, int step);
	static int MakeLibGraph(AGmNode *root_class, AGmNode *root);
	static int MakeLibText2(AGmNode *root_class, AGmNode *classes);
	static int LoadLib(AGmNode *root_class, int tmp=0);
	static int _LoadLib(AGmFile*, AGmString *s, AGmNode *root_class, AGmNode** lib);
	static int _LoadLibText(AGmFile*, AGmString*, AGmNode *root_class, AGmNode *root, int, AGmNode **lib);
	static int _LoadLibGraph(AGmFile*, AGmNode *root_class, AGmNode *root, AGmNode **lib);

	static void CloseWindow(ADvWindow *aw);
	static void CloseAllWindows();

	static int check_load;

	static int OpenProjectEditor();

	static int IsReserveWord(AGmString*);

// for garbage collection
public:
	void GCmark();
};

#endif	/* _AUI_CUI_H */

