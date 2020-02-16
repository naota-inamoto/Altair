/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_GAME_FRWK_H
#define _AGM_GAME_FRWK_H

#define	GAME_MOCHI_MAX	32
#define	GAME_TESUJI_MAX	256

class AGmHyouka : public AGm {
public:
	AGmHyouka();
	~AGmHyouka();

	int value;
	int state;
	int kakuritsu;
};

class AGmKyokumen : public AGm {
public:
	AGmKyokumen(int w, int h);
	~AGmKyokumen();

	int width;
	int height;
	AGmNode *ban2d;
	AGmNode *mochi_goma;
	AGmNode *cluster_ls;

	AGmNode *te_ls;

	AGmHyouka *hyouka;

	void GCmark2();
};

class AGmKoma : public AGmNode {
public:
	AGmKoma(int k, int r, int c);
	~AGmKoma();

	int kind;
	int row;
	int col;

	void GCmark2();
};

class AGmTe : public AGmNode {
public:
	AGmTe(AGmKoma* k, int r, int c);
	~AGmTe();

	AGmKoma *koma;
	int to_row;
	int to_col;

	void GCmark2();
};

class AGmTesuji : public AGm {
public:
	AGmTesuji();
	~AGmTesuji();

	void GCmark2();
};

class AGmCluster : public AGm {
public:
	AGmCluster();
	~AGmCluster();

	void GCmark2();
};

class AGmYomi : public AGm {
public:
	AGmYomi();
	~AGmYomi();

	AGmNode *dag; // node is PtrCell(AGmKyokumen*)
	int turn;
	int keisei;

	static AGmNode *TeList(AGmKyokumen*, int turn); // array not list
	static AGmKyokumen *Play(AGmKyokumen*, int turn, AGmTe*);
	static AGmHyouka *Eval(AGmKyokumen*, int turn);
	static int NumTurn();
	static int NextTurn(int turn);

	AGmYomi *depth_priority_search(AGmYomi*, int cut_value, int depth=3, int time=-1, int pass=0);
	AGmYomi *breadth_first_search(AGmYomi*, int cut_value, int depth=3, int time=-1, int pass=0);

	static AGmTe* select_first(AGmYomi*);
	static AGmTe* select_last(AGmYomi*);
	static AGmTe* select_random(AGmYomi*);

	static void GCmark();
	void GCmark2();
};

#endif
