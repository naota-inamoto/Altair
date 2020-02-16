/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/string.h>

#include <symbol/game_frwk.h>

AGmHyouka::AGmHyouka() {
	value = 0;
	state = 0;
	kakuritsu = 0;
}
AGmHyouka::~AGmHyouka() {
}

AGmKyokumen::AGmKyokumen(int w, int h) {
	int width = w;
	int height = h;
	int i;

	ban2d = Ref(Array(w));
	for (i = 0; i < width; i++) {
		ban2d->a.a.v[i] = Ref(Array(h));
	}
	mochi_goma = Ref(Array(GAME_MOCHI_MAX));
	for (i = 0; i < GAME_MOCHI_MAX; i++) {
		mochi_goma->a.a.v[i] = Ref(Integer(0));
	}

	cluster_ls = NULL;
	te_ls = NULL;
	hyouka = NULL;
}
AGmKyokumen::~AGmKyokumen() {
	Unref(ban2d, mochi_goma, cluster_ls);
	if (hyouka) delete hyouka;
}
void AGmKyokumen::GCmark2() {
	_GCmark(ban2d);
	_GCmark(mochi_goma);
	_GCmark(cluster_ls);
}

AGmKoma::AGmKoma(int k, int r, int c) {
	kind = k;
	row = r;
	col = c;
}
AGmKoma::~AGmKoma() {
}
void AGmKoma::GCmark2() {
}

AGmTe::AGmTe(AGmKoma* k, int r, int c) {
	koma = k;
	to_row = r;
	to_col = c;
}
AGmTe::~AGmTe() {
}
void AGmTe::GCmark2() {
}

AGmTesuji::AGmTesuji() {
}
AGmTesuji::~AGmTesuji() {
}
void AGmTesuji::GCmark2() {
}

AGmCluster::AGmCluster()  {
}
AGmCluster::~AGmCluster() {
}
void AGmCluster::GCmark2() {
}

AGmYomi::AGmYomi() {
	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmGame());
	if (n == NULL) {
		CreateArc(root, n = Cons(NULL), NmGame());
	}
	CreateArc(n, PtrCell(this));

	dag = NULL;
	turn = 0;
	keisei = 0;
}
AGmYomi::~AGmYomi() {
	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmGame());
	for (AGmLink *l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		if (l->arc->dst->a.p.ptr == this) {
			DeleteArc(l->arc); break;
		}
	}

	Unref(dag);
}
void AGmYomi::GCmark() {
	AGmNode *root = Root();
	AGmNode *n = dst_node2(root, NmGame());
	if (n == NULL) return;
	for (AGmLink *l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		AGmYomi* yomi = (AGmYomi*)l->arc->dst->a.p.ptr;
		yomi->GCmark2();
	}
}
void AGmYomi::GCmark2() {
	_GCmark(dag);
}

AGmNode *AGmYomi::TeList(AGmKyokumen* ban, int turn) {
	// ### TODO
	return NULL;
}
AGmKyokumen *AGmYomi::Play(AGmKyokumen* ban, int turn, AGmTe* te) {
	// ### TODO
	return NULL;
}
AGmHyouka *AGmYomi::Eval(AGmKyokumen*, int turn) {
	// ### TODO
	return NULL;
}
int AGmYomi::NumTurn() {
	return 2;
}
int AGmYomi::NextTurn(int turn) {
	int num = NumTurn();
	return (turn + num) % num;
}

AGmYomi *AGmYomi::depth_priority_search(AGmYomi* yomi, int cut_value, int depth, int time, int pass) {
	if (depth <= 0) return new AGmYomi();

	int max_value = -32767;
	AGmTe *max_te = NULL;

	AGmKyokumen* ban = (AGmKyokumen*)yomi->dag->a.p.ptr;
	int turn = yomi->turn;
	AGmNode *te_ls = TeList(ban, turn);
	ban->te_ls = te_ls;
	AGmTe *te;
	int i;
	for (i = 0; i < te_ls->a.a.size; i++) {
		te = (AGmTe*)te_ls->a.a.v[i];
		AGmKyokumen *next_ban = Play(ban, turn, te);
		int next_turn = NextTurn(turn);
		AGmHyouka *eval = Eval(next_ban, next_turn);
		if (eval->value > max_value) {
			max_value = eval->value;
			max_te = te;
		}
		if (eval->value > cut_value) {
			AGmYomi *next_yomi = depth_priority_search(next_yomi, max_value, depth-1, time, pass);
			next_yomi->turn = next_turn;
			next_yomi->keisei = max_value;
			CreateArc(yomi->dag, next_yomi->dag = Ref(PtrCell(next_yomi)));
			return next_yomi;
		}
	}
	return NULL;
}
AGmYomi *AGmYomi::breadth_first_search(AGmYomi* ban, int cut_value, int depth, int time, int pass) {
	return NULL;
}
AGmTe* AGmYomi::select_first(AGmYomi*) {
	return NULL;
}
AGmTe* AGmYomi::select_last(AGmYomi*) {
	return NULL;
}
AGmTe* AGmYomi::select_random(AGmYomi*) {
	return NULL;
}
