/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/matrix.h>
#include <gdm/error.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

AGmMatrix::AGmMatrix(long n, long m) {
	nrow = n; ncol = m;
	data = new double*[n];
	for (long i = 0; i < n; i++) {
		data[i] = new double[m];
	}
}
AGmMatrix::~AGmMatrix() {
	for (long i = 0; i < nrow; i++) delete [] data[i];
	delete [] data; 	
}

int AGmMatrix::IsEqual(AGmMatrix *m) const {
	if (nrow != m->nrow || ncol != m->ncol) return 0;
	for (long i = 0; i < nrow; i++)
	for (long j = 0; j < ncol; j++)
		if (data[i][j] != m->data[i][j]) return 0;
	return 1;
}
AGmMatrix *AGmMatrix::Copy() {
	AGmMatrix *m = new AGmMatrix(nrow, ncol);
	for (long i = 0; i < nrow; i++)
	for (long j = 0; j < ncol; j++)
		m->data[i][j] = data[i][j];
	return m;
}
AGmMatrix *AGmMatrix::Transpose() {
	AGmMatrix *m = new AGmMatrix(ncol, nrow);
	for (long i = 0; i < nrow; i++)
	for (long j = 0; j < ncol; j++)
		m->data[j][i] = data[i][j];
	return m;
}
AGmMatrix *AGmMatrix::ChangeSign() {
	AGmMatrix *m = new AGmMatrix(nrow, ncol);
	for (long i = 0; i < nrow; i++)
	for (long j = 0; j < ncol; j++)
		m->data[i][j] = - data[i][j];
	return m;
}
AGmMatrix *AGmMatrix::Add(AGmMatrix *b) {
	if (nrow != b->nrow || ncol != b->ncol) return NULL;
	AGmMatrix *m = new AGmMatrix(ncol, nrow);
	for (long i = 0; i < nrow; i++)
	for (long j = 0; j < ncol; j++)
		m->data[i][j] = data[i][j] + b->data[i][j];
	return m;
}
AGmMatrix *AGmMatrix::Sub(AGmMatrix *b) {
	if (nrow != b->nrow || ncol != b->ncol) return NULL;
	AGmMatrix *m = new AGmMatrix(ncol, nrow);
	for (long i = 0; i < nrow; i++)
	for (long j = 0; j < ncol; j++)
		m->data[i][j] = data[i][j] - b->data[i][j];
	return m;
}
AGmMatrix *AGmMatrix::Mul(AGmMatrix *b) {
	if (ncol != b->nrow) return NULL;
	AGmMatrix *m = new AGmMatrix(ncol, b->ncol);
	for (long i = 0; i < nrow; i++)
	for (long j = 0; j < b->ncol; j++) {
		m->data[i][j] = 0;
		for (long k = 0; k < ncol; k++)
			m->data[i][j] += data[i][k] * b->data[k][j];
	}
	return m;
}
AGmMatrix *AGmMatrix::Inverse() {
	long i, j;
	if (nrow != ncol) return NULL;
	AGmMatrix *m2 = new AGmMatrix(nrow, ncol*2);
	for (i = 0; i < nrow; i++)
	for (j = 0; j < ncol; j++)	{
		m2->data[i][j] = data[i][j];
		m2->data[i][ncol+j] = ((i==j)?1:0);
	}
	if (m2->Gauss()) { delete m2; return NULL; }

	AGmMatrix *m = new AGmMatrix(nrow, ncol);
	for (i = 0; i < nrow; i++)
	for (j = 0; j < ncol; j++)
		m->data[i][j] = m2->data[i][ncol+j];
	delete m2;
	return m;
}

#define NUM_ERROR	1e-8

int AGmMatrix::Gauss() {
	double p_max, tmp;
	long i, j, max_i = 0;
	for (long k = 0; k < nrow; k++) {
		// select pivot
		p_max = -1;
		for (i = k; i < nrow; i++) {
			if (fabs(data[i][k]) > p_max) {
				p_max = fabs(data[i][k]);
				max_i = i;
			}
		}
		if (p_max < NUM_ERROR) return 1;
		// exchange pivot row
		if (max_i != k) {
			for (j = 0; j < ncol; j++) {
				tmp = data[max_i][j];
				data[max_i][j] = data[k][j];
				data[k][j] = tmp;
			}
		}
		// normalize
		tmp = data[k][k];
		for (j = k; j < ncol; j++) data[k][j] /= tmp;
		// elimination
		for (i = 0; i < nrow; i++) {
			if (i != k) {
				tmp = data[i][k];
				for (j = 0; j < ncol; j++) data[i][j] -= tmp * data[k][j];	
			}
		}
	}
	return 0;
}

int AGmMatrix::Id() {
	if (nrow != 3 || ncol != 3) return 1;
	for (long i = 0; i < 3; i++)
	for (long j = 0; j < 3; j++)
		data[i][j] = ( (i == j) ? 1 : 0 );
	return 0;
}
int AGmMatrix::Translate(double dx, double dy) {
	if (Id()) return 1;
	data[0][2] = dx;
	data[1][2] = dy;
	return 0;
}
int AGmMatrix::Scale(double sx, double sy) {
	if (Id()) return 1;
	data[0][0] = sx;
	data[1][1] = sy;
	return 0;
}
int AGmMatrix::Rotate(double angle) {
	if (Id()) return 1;
	double c = cos(angle), s = sin(angle);
	data[0][0] = c;
	data[0][1] = -s;
	data[1][0] = s;
	data[1][1] = c;
	return 0;
}

int AGmMatrix::Id4() {
	if (nrow != 4 || ncol != 4) return 1;
	for (long i = 0; i < 4; i++)
	for (long j = 0; j < 4; j++)
		data[i][j] = ( (i == j) ? 1 : 0 );
	return 0;
}
int AGmMatrix::Translate(double dx, double dy, double dz) {
	if (Id4()) return 1;
	data[0][3] = dx;
	data[1][3] = dy;
	data[2][3] = dz;
	return 0;
}
int AGmMatrix::Scale(double sx, double sy, double sz) {
	if (Id4()) return 1;
	data[0][0] = sx;
	data[1][1] = sy;
	data[2][2] = sz;
	return 0;
}
int AGmMatrix::Rotate_x(double angle) {
	if (Id4()) return 1;
	double c = cos(angle), s = sin(angle);
	data[1][1] = c;
	data[1][2] = -s;
	data[2][1] = s;
	data[2][2] = c;
	return 0;
}
int AGmMatrix::Rotate_y(double angle) {
	if (Id4()) return 1;
	double c = cos(angle), s = sin(angle);
	data[2][2] = c;
	data[2][0] = -s;
	data[0][2] = s;
	data[0][0] = c;
	return 0;
}
int AGmMatrix::Rotate_z(double angle) {
	if (Id4()) return 1;
	double c = cos(angle), s = sin(angle);
	data[0][0] = c;
	data[0][1] = -s;
	data[1][0] = s;
	data[1][1] = c;
	return 0;
}

int AGmMatrix::Transform(double lx, double ly, double& gx, double& gy) {
	if (nrow != 3 || ncol != 3) return 1;
	gx = data[0][0] * lx + data[0][1] * ly + data[0][2];
	gy = data[1][0] * lx + data[1][1] * ly + data[1][2];
	return 0;
}
int AGmMatrix::Transform(AGmNode* polypoly, AGmNode*& polypoly2) {
	if (nrow != 3 || ncol != 3) return 1;
	double a00 = data[0][0], a01 = data[0][1], a02 = data[0][2];
	double a10 = data[1][0], a11 = data[1][1], a12 = data[1][2];
	polypoly2 = Integer(0);
	AGmLink *ll;
	for (ll = next_outlink(polypoly, NULL);
			ll; ll = next_outlink(polypoly, ll)) {
		AGmNode *poly = ll->arc->dst;
		AGmNode *poly2 = Integer(0);
		CreateArc(polypoly2, poly2);
		AGmLink *l;
		for (l = next_outlink(poly, NULL);
				l; l = next_outlink(poly, l)) {
			AGmNode *n = l->arc->dst;
			double x, y; AGmNode *tail;
			if (Param(n, x, y, tail)) {
				return 1;
			}
			double x2 = a00 * x + a01 * y + a02;
			double y2 = a10 * x + a11 * y + a12;
			CreateArc(poly2, List(Real(x2), Real(y2)));
		}
	}
	return 0;
}
int AGmMatrix::Inverse(double gx, double gy, double& lx, double& ly) {
	if (nrow != 3 || ncol != 3) return 1;
	double a00 = data[0][0], a01 = data[0][1], a02 = data[0][2];
	double a10 = data[1][0], a11 = data[1][1], a12 = data[1][2];
	double d = a00 * a11 - a01 * a10;
	if (fabs(d) < NUM_ERROR) return 1;
	gx -= a02; gy -= a12;
	lx = (	a11 * gx - a01 * gy) / d;
	ly = (- a10 * gx + a00 * gy) / d;
	return 0;
}
int AGmMatrix::Inverse(AGmNode* polypoly, AGmNode*& polypoly2) {
	if (nrow != 3 || ncol != 3) return 1;
	double a00 = data[0][0], a01 = data[0][1], a02 = data[0][2];
	double a10 = data[1][0], a11 = data[1][1], a12 = data[1][2];
	double d = a00 * a11 - a01 * a10;
	if (fabs(d) < NUM_ERROR) return 1;
	polypoly2 = Integer(0);
	AGmLink *ll;
	for (ll = next_outlink(polypoly, NULL);
			ll; ll = next_outlink(polypoly, ll)) {
		AGmNode *poly = ll->arc->dst;
		AGmNode *poly2 = Integer(0);
		CreateArc(polypoly2, poly2);
		AGmLink *l;
		for (l = next_outlink(poly, NULL);
				l; l = next_outlink(poly, l)) {
			AGmNode *n = l->arc->dst;
			double x, y; AGmNode *tail;
			if (Param(n, x, y, tail)) {
				return 1;
			}
			x -= a02; y -= a12;
			double x2 = (  a11 * x - a01 * y) / d;
			double y2 = (- a10 * x + a00 * y) / d;
			CreateArc(poly2, List(Real(x2), Real(y2)));
		}
	}
	return 0;
}

