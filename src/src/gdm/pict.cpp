/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/pict.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

AGmNode *AGmPict::CreatePolyPoly() {
	AGmNode *polypoly = Integer(0);
	CreateArc(polypoly, Integer(0));
	return polypoly;
}
int AGmPict::CreateNextPoly(AGmNode *polypoly) {
	CreateArc(polypoly, Integer(0));
	return 0;
}

int AGmPict::AddPointToPoly(AGmNode *polypoly, double x, double y) {
	AGmLink *l = prev_outlink(polypoly, NULL);
	if (l == NULL) return 1;
	AGmNode *poly = l->arc->dst;
	CreateArc(poly, List(Real(x), Real(y)));
	return 0;
}
int AGmPict::AddRectToPoly(AGmNode *polypoly,
		double left, double top, double right, double bottom) {
	AGmLink *l = prev_outlink(polypoly, NULL);
	if (l == NULL) return 1;
	AGmNode *poly = l->arc->dst;
	CreateArc(poly, List(Real(left),  Real(top)));
	CreateArc(poly, List(Real(right), Real(top)));
	CreateArc(poly, List(Real(right), Real(bottom)));
	CreateArc(poly, List(Real(left),  Real(bottom)));
	return 0;
}
int AGmPict::AddCurveToPoly(AGmNode *polypoly,
			double cx1, double cy1, double cx2, double cy2, double ex, double ey) {
	AGmLink *l = prev_outlink(polypoly, NULL);
	if (l == NULL) return 1;
	AGmNode *poly = l->arc->dst;
	l = prev_outlink(poly, NULL);
	if (l == NULL) return 1;
	AGmNode *xy = l->arc->dst;
	if (!IsList(xy) || !IsList(xy->T) || !IsReal(xy->H) || !IsReal(xy->TH))
		return 1;
	double sx = xy->H->a.r.v, sy = xy->TH->a.r.v;
	CurvePts(poly, sx, sy, cx1, cy1, cx2, cy2, ex, ey);
	return 0;
}

#define NUM_ERROR	1e-5
int AGmPict::IsEqualPolyPolygon(AGmNode* polypoly1, AGmNode* polypoly2) {
	if (polypoly1 == NULL && polypoly2 == NULL) return 1;
	if (polypoly1 == NULL || polypoly2 == NULL) return 0;
	AGmLink *ll1, *ll2;
	for (ll1 = next_outlink(polypoly1, NULL), ll2 = next_outlink(polypoly2, NULL);
			ll1 && ll2;
			ll1 = next_outlink(polypoly1, ll1), ll2 = next_outlink(polypoly2, ll2)) {
		AGmNode *poly1 = ll1->arc->dst;
		AGmNode *poly2 = ll2->arc->dst;
		AGmLink *l1, *l2;
		for (l1 = next_outlink(poly1, NULL), l2 = next_outlink(poly2, NULL);
				l1 && l2;
				l1 = next_outlink(poly1, l1), l2 = next_outlink(poly2, l2)) {
			double x1, y1, x2, y2; AGmNode *tail;
			if (Param(l1->arc->dst, x1, y1, tail)) return 0;
			if (Param(l2->arc->dst, x2, y2, tail)) return 0;
			if (fabs(x1-x2) > NUM_ERROR || fabs(y1-y2) > NUM_ERROR) return 0;
		}
		if (l1 || l2) return 0;
	}
	if (ll1 || ll2) return 0;
	return 1;	
}
int AGmPict::GetPolyPolygonBox(AGmNode *polypoly, double& left, double& top, double& right, double& bottom) {
	left = top = 1e30; right = bottom = -1e30;
	AGmLink *ll;
	for (ll = next_outlink(polypoly, NULL);
			ll; ll = next_outlink(polypoly, ll)) {
		AGmNode *poly = ll->arc->dst;
		AGmLink *l;
		for (l = next_outlink(poly, NULL);
				l; l = next_outlink(poly, l)) {
			double x, y; AGmNode *tail;
			if (Param(l->arc->dst, x, y, tail)) return 0;
			left   = min(left,	 x);
			top    = min(top,	 y);
			right  = max(right,  x);
			bottom = max(bottom, y);
		}
	}
	return 0;	
}

#define SMOOTHNESS	1e-4	
int AGmPict::CurvePts(AGmNode *poly,	 /* bedge curve */
					double x0, double y0, double x1, double y1,
					double x2, double y2, double x3, double y3) {
	double f1 = (x0 - x1) * (y3 - y1) - (y0 - y1) * (x3 - x1);
	double f2 = (x0 - x2) * (y3 - y2) - (y0 - y2) * (x3 - x2);
	if (f1*f1 + f2*f2 < SMOOTHNESS) {  /* whether four points is on the stright line */
		CreateArc(poly, List(Real(x3), Real(y3)));
		return 0;
	} else {
		double x11 = (x0  + x1)  * 0.5;
		double y11 = (y0  + y1)  * 0.5;
		double xx  = (x1  + x2)  * 0.5;
		double yy  = (y1  + y2)  * 0.5;
		double x22 = (x2  + x3)  * 0.5;
		double y22 = (y2  + y3)  * 0.5;
		double x12 = (x11 + xx)  * 0.5;
		double y12 = (y11 + yy)  * 0.5;
		double x21 = (xx  + x22) * 0.5;
		double y21 = (yy  + y22) * 0.5;
		double cx  = (x12 + x21) * 0.5;
		double cy  = (y12 + y21) * 0.5;
		int flag = 0;
		flag |= CurvePts(poly, x0, y0, x11, y11, x12, y12, cx, cy);
		flag |= CurvePts(poly, cx, cy, x21, y21, x22, y22, x3, y3);
		return flag;
	}
}

