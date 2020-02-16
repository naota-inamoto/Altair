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
#include <gdm/matrix.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <dev/advwnd.h>

#include <solid/solid.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

static double PI = atan(1.0) * 4;

int AIpProcess::EvalSolid() {
	AGmNode *cmd, *arg1, *arg2, *arg3, *arg4;
	GetArg(cmd, arg1, arg2, arg3, arg4);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("Solid()");
	}

	AGmGWB::init_str();

	const char *s = cmd->a.s.v->Str();
	AGmNode *tail;
	Unref(val); val = NULL;
	if (strcmp(s, "draw") == 0) {
		if (!IsWindow(arg1) || !arg2 || !IsList(arg2) || !arg2->H || !IsReal(arg2->HH) || !IsReal(arg2->HTH) || !arg2->T || !IsMatrix(arg2->TH)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() draw");
		}
		AGmGWB::draw(arg1->a.w.v, arg2->H->HR, arg2->H->THR, arg2->TH->a.m.v, arg3, (int)arg4->a.i.v);

	} else if (strcmp(s, "transform") == 0) {
		if (!IsMatrix(arg1) || !arg2 || !IsList(arg2)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() transform");
		}
		AGmMatrix *mat = arg1->a.m.v;
		AGmNode *a = GraphCopy(arg2);
		MakeStrUnique(a);
		AGmNode *fs = dst_node2(a, AGmGWB::_gwb_faces);
		AGmNode *vs = dst_node2(a, AGmGWB::_gwb_vertices);
		AGmNode *f, *l, *v, *itr1, *itr2;

		itr1 = Ref(DstItr(fs));
		for (f = itr_next(itr1); f; f = itr_next(itr1)) {
			itr2 = Ref(DstItr(f));
			for (l = itr_next(itr2); l; l = itr_next(itr2)) {
				Set(l->TH, NULL);
				Set(l->TTH, NULL);
				Set(l->TTTH, NULL);
				Set(l->TTTTH, NULL);
			}
			Unref(itr2);
		}
		Unref(itr1);

		itr1 = Ref(DstItr(vs));
		for (v = itr_next(itr1); v; v = itr_next(itr1)) {
			double x, y, z;
			AGmGWB::vertex_T_xyz(v, mat, &x, &y, &z);
			v->THR = x;
			v->TTHR = y;
			v->TTTHR = z;
		}
		Unref(itr1);

		val = Ref(a);
	} else if (strcmp(s, "trans") == 0) {
		double dx, dy, dz;
		if (!IsNum(arg1, dx) || !IsNum(arg2, dy) || !IsNum(arg3, dz)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() trans");
		}
		AGmMatrix *mat = new AGmMatrix(4, 4);
		mat->Translate(dx, dy, dz);
		val = Ref(Matrix(mat));
	} else if (strcmp(s, "rot_x") == 0) {
		double angle;
		if (!IsNum(arg1, angle)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() trans");
		}
		AGmMatrix *mat = new AGmMatrix(4, 4);
		mat->Rotate_x(angle);
		val = Ref(Matrix(mat));
	} else if (strcmp(s, "rot_y") == 0) {
		double angle;
		if (!IsNum(arg1, angle)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() trans");
		}
		AGmMatrix *mat = new AGmMatrix(4, 4);
		mat->Rotate_y(angle);
		val = Ref(Matrix(mat));
	} else if (strcmp(s, "rot_z") == 0) {
		double angle;
		if (!IsNum(arg1, angle)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() trans");
		}
		AGmMatrix *mat = new AGmMatrix(4, 4);
		mat->Rotate_z(angle);
		val = Ref(Matrix(mat));

	} else if (strcmp(s, "union") == 0) {
		if (!arg1 || !IsList(arg1) || !arg2 || !IsList(arg2)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() union");
		}
		val = Ref(AGmGWB::setop(arg1, arg2, OP_UNION));
	} else if (strcmp(s, "intersection") == 0) {
		if (!arg1 || !IsList(arg1) || !arg2 || !IsList(arg2)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() intersection");
		}
		val = Ref(AGmGWB::setop(arg1, arg2, OP_INTERSECTION));
	} else if (strcmp(s, "minus") == 0) {
		if (!arg1 || !IsList(arg1) || !arg2 || !IsList(arg2)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() minus");
		}
		val = Ref(AGmGWB::setop(arg1, arg2, OP_MINUS));

	} else if (strcmp(s, "view") == 0) {
		val = Ref(Cons(NULL));
		CreateArc(val, List(Real(0), Real(0)), String("org"));
		CreateArc(val, List(Real(0), Real(0), Real(0)), String("lookat"));
		CreateArc(val, List(Real(0), Real(0), Real(1)), String("lookfrom"));
		CreateArc(val, List(Real(0), Real(1), Real(0)), String("lookup"));
	} else if (strcmp(s, "view_org") == 0) {
		AGmNode *l = dst_node(arg1, "org");
		double x, y;
		if (l == NULL || Param(arg2, x, y, tail)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() view_org");
		}
		l->HR = x; l->THR = y;
	} else if (strcmp(s, "view_lookat") == 0) {
		AGmNode *l = dst_node(arg1, "lookat");
		double ax, ay, az;
		if (l == NULL || Param(arg2, ax, ay, az, tail)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() view_lookat");
		}
		l->HR = ax; l->THR = ay; l->TTHR = az;
	} else if (strcmp(s, "view_lookfrom") == 0) {
		AGmNode *l = dst_node(arg1, "lookfrom");
		double fx, fy, fz;
		if (l == NULL || Param(arg2, fx, fy, fz, tail)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() view_lookfrom");
		}
		l->HR = fx; l->THR = fy; l->TTHR = fz;
	} else if (strcmp(s, "view_lookup") == 0) {
		AGmNode *l = dst_node(arg1, "lookup");
		double ux, uy, uz;
		if (l == NULL || Param(arg2, ux, uy, uz, tail)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() view_lookup");
		}
		l->HR = ux; l->THR = uy; l->TTHR = uz;
	} else if (strcmp(s, "view_T") == 0) {
		AGmNode *w = dst_node(arg1, "org");
		AGmNode *a = dst_node(arg1, "lookat");
		AGmNode *f = dst_node(arg1, "lookfrom");
		AGmNode *u = dst_node(arg1, "lookup");
		if (w == NULL || a == NULL || f == NULL || u == NULL) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() view_T");
		}
		double x = w->HR, y = w->THR;
		double ax = a->HR, ay = a->THR, az = a->TTHR;
		double fx = f->HR, fy = f->THR, fz = f->TTHR;
		double ux = u->HR, uy = u->THR, uz = u->TTHR;

		AGmMatrix *mat = new AGmMatrix(4, 4);
		mat->Translate(-ax, -ay, -az);

		double nxz = fx - ax;
		double nyz = fy - ay;
		double nzz = fz - az;
		double len = sqrt(nxz * nxz + nyz * nyz + nzz * nzz);
		nxz /= len; nyz /= len; nzz /= len;

		len = ux * nxz + uy * nyz + uz * nzz;
		double nxy = ux - nxz * len;
		double nyy = uy - nyz * len;
		double nzy = uz - nzz * len;
		len = sqrt(nxy * nxy + nyy * nyy + nzy * nzy);
		nxy /= len; nyy /= len; nzy /= len;

		double nxx = nyy * nzz - nzy * nyz;
		double nyx = nzy * nxz - nxy * nzz;
		double nzx = nxy * nyz - nyy * nxz;
		len = sqrt(nxx * nxx + nyx * nyx + nzx * nzx);
		nxx /= len; nyx /= len; nzx /= len;

		AGmMatrix *tmp = new AGmMatrix(4, 4);
		tmp->Id4();
		tmp->Component(0, 0) = nxx;
		tmp->Component(0, 1) = nxy;
		tmp->Component(0, 2) = nxz;
		tmp->Component(1, 0) = nyx;
		tmp->Component(1, 1) = nyy;
		tmp->Component(1, 2) = nyz;
		tmp->Component(2, 0) = nzx;
		tmp->Component(2, 1) = nzy;
		tmp->Component(2, 2) = nzz;
		AGmMatrix *tmp2 = mat->Mul(tmp);
		delete mat; mat = tmp2;
		delete tmp;

		val = Ref(List(List(x, y), Matrix(mat)));
		
	} else if (strcmp(s, "block") == 0) {
		double dx, dy, dz;
		if (!IsNum(arg1, dx) || !IsNum(arg2, dy) || !IsNum(arg3, dz)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() block");
		}
		val = Ref(AGmGWB::block(dx, dy, dz));
	} else if (strcmp(s, "cylinder") == 0) {
		double r, h;
		if (!IsNum(arg1, r) || !IsNum(arg2, h) || !IsInteger(arg3)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() cylinder");
		}
		val = Ref(AGmGWB::cylinder(r, h, (int)arg3->a.i.v));
	} else if (strcmp(s, "cone") == 0) {
		double r, h;
		if (!IsNum(arg1, r) || !IsNum(arg2, h) || !IsInteger(arg3)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() cone");
		}
		val = Ref(AGmGWB::cone(r, h, (int)arg3->a.i.v));
	} else if (strcmp(s, "sphere") == 0) {
		double r;
		if (!IsNum(arg1, r) || !IsInteger(arg2) || !IsInteger(arg3)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() cone");
		}
		val = Ref(AGmGWB::sphere(r, (int)arg2->a.i.v, (int)arg3->a.i.v));
	} else if (strcmp(s, "reverse") == 0) {
		if (arg1 == NULL || !IsList(arg1)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() reverse");
		}
		val = Ref(AGmGWB::reverse(arg1));

	} else if (strcmp(s, "dump") == 0) {
		if (!IsString(arg1) || !arg2 || !IsList(arg2)) {
			AGmGWB::finalize_str();
			Unref(cmd, arg1, arg2, arg3, arg4);
			return interp_error("Solid() dump");
		}
		AGmGWB::dump_breps((char*)arg1->a.s.v->Str(), arg2);

	} else {
		AGmGWB::finalize_str();
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("Solid()");
	}
	AGmGWB::finalize_str();
	Unref(cmd, arg1, arg2, arg3, arg4);
	return 0;
}

AGmNode *AGmGWB::_gwb_faces;
AGmNode *AGmGWB::_gwb_face;
AGmNode *AGmGWB::_gwb_loop;
AGmNode *AGmGWB::_gwb_l_he;
AGmNode *AGmGWB::_gwb_he_vertex;
AGmNode *AGmGWB::_gwb_edges;
AGmNode *AGmGWB::_gwb_edge;
AGmNode *AGmGWB::_gwb_he;
AGmNode *AGmGWB::_gwb_vertices;
AGmNode *AGmGWB::_gwb_vertex;
void AGmGWB::init_str() {
	_gwb_faces = Ref(UniqueStr("faces"));
	_gwb_face = Ref(UniqueStr("face"));
	_gwb_loop = Ref(UniqueStr("loop"));
	_gwb_l_he = Ref(UniqueStr("l_he"));
	_gwb_he_vertex = Ref(UniqueStr("he_vertex"));
	_gwb_edges = Ref(UniqueStr("edges"));
	_gwb_edge = Ref(UniqueStr("edge"));
	_gwb_he = Ref(UniqueStr("he"));
	_gwb_vertices = Ref(UniqueStr("vertices"));
	_gwb_vertex = Ref(UniqueStr("vertex"));
}
void AGmGWB::finalize_str() {
	Unref(_gwb_face); _gwb_face = NULL;
	Unref(_gwb_loop); _gwb_loop = NULL;
	Unref(_gwb_l_he); _gwb_l_he = NULL;
	Unref(_gwb_he_vertex); _gwb_he_vertex = NULL;
	Unref(_gwb_edges); _gwb_edges = NULL;
	Unref(_gwb_edge); _gwb_edge = NULL;
	Unref(_gwb_he); _gwb_he = NULL;
	Unref(_gwb_vertices); _gwb_vertices = NULL;
	Unref(_gwb_vertex); _gwb_vertex = NULL;
}

AGmNode* AGmGWB::mvsf(double x, double y, double z, AGmNode** f, AGmNode** v) {
	AGmNode *s = Cons(NULL);
	CreateArc(s, Cons(NULL), _gwb_faces);
	CreateArc(s, Cons(NULL), _gwb_edges);
	CreateArc(s, Cons(NULL), _gwb_vertices);

	AGmNode *fs = dst_node2(s, _gwb_faces);
	*f = Cons(NULL);
	AGmNode *l = List((AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL);
	CreateArc(fs, *f, _gwb_face);
	CreateArc(*f, l, _gwb_loop);

	AGmNode *vs = dst_node2(s, _gwb_vertices);
	*v = Cons(NULL, List(x, y, z));
	CreateArc(vs, *v, _gwb_vertex);

	return s;
}

int AGmGWB::mev(AGmNode *s, AGmNode* f, AGmNode* v1,
		double x, double y, double z, AGmNode** v2) {
	AGmNode *es = dst_node2(s, _gwb_edges);
	AGmNode *vs = dst_node2(s, _gwb_vertices);
	AGmNode *l = dst_node2(f, _gwb_loop);
	AGmLink *he = NULL;

	if (l == NULL) { ASSERT(0); return 1; }

	if (dst_node2(l, _gwb_l_he)) {
		AGmLink *lnk;
		for (lnk = next_outlink2(f, NULL, _gwb_loop); lnk; lnk = next_outlink2(f, lnk, _gwb_loop)) {
			l = lnk->arc->dst;
			for (he = next_outlink2(l, NULL, _gwb_l_he); he; he = next_outlink2(l, he, _gwb_l_he)) {
				if (dst_node2(he->arc->dst, _gwb_he_vertex) == v1) break;
			}
			if (he) break;
		}
		if (he == NULL) { ASSERT(0); return 1; }
	}

	AGmNode *e = List((AGmNode*)NULL, (AGmNode*)NULL);
	AGmNode *he1 = Cons(NULL);
	AGmNode *he2 = Cons(NULL);
	*v2 = Cons(NULL, List(x, y, z));

	CreateArc(l, he1, _gwb_l_he, he);
	CreateArc(l, he2, _gwb_l_he, he);
	CreateArc(es, e, _gwb_edge);
	CreateArc(e, he1, _gwb_he);
	CreateArc(e, he2, _gwb_he);
	CreateArc(he1, v1, _gwb_he_vertex);
	CreateArc(he2, *v2, _gwb_he_vertex);
	CreateArc(vs, *v2, _gwb_vertex);

	return 0;
}
int AGmGWB::mef(AGmNode *s, AGmNode* f1, AGmNode* v1, AGmNode* v2, AGmNode** f2) {
	AGmNode *fs = dst_node2(s, _gwb_faces);
	AGmNode *es = dst_node2(s, _gwb_edges);
	AGmNode *l = NULL;
	AGmLink *he1 = NULL, *he2 = NULL;
	AGmLink *lnk;

	for (lnk = next_outlink2(f1, NULL, _gwb_loop); lnk; lnk = next_outlink2(f1, lnk, _gwb_loop)) {
		l = lnk->arc->dst;
		for (he1 = next_outlink2(l, NULL, _gwb_l_he); he1; he1 = next_outlink2(l, he1, _gwb_l_he)) {
			if (dst_node2(he1->arc->dst, _gwb_he_vertex) == v1) break;
		}
		if (he1) break;
	}
	if (he1 == NULL) { ASSERT(0); return 1; }

	for (he2 = next_outlink2(l, NULL, _gwb_l_he); he2; he2 = next_outlink2(l, he2, _gwb_l_he)) {
		if (dst_node2(he2->arc->dst, _gwb_he_vertex) == v2) break;
	}
	if (he2 == NULL) { ASSERT(0); return 1; }

	*f2 = Cons(NULL);
	AGmNode *l2 = List((AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL);
	AGmNode *e = List((AGmNode*)NULL, (AGmNode*)NULL);
	AGmNode *he_1 = Cons(NULL);
	AGmNode *he_2 = Cons(NULL);

	CreateArc(fs, *f2, _gwb_face);
	CreateArc(*f2, l2, _gwb_loop);
	CreateArc(l, he_1, _gwb_l_he, he1);
	CreateArc(l, he_2, _gwb_l_he, he2);
	CreateArc(es, e, _gwb_edge);

	AGmLink *he, *n_he;
	for (he = he1; he != he2; he = n_he) {
		n_he = next_outlink2(l, he, _gwb_l_he);
		if (n_he == NULL) n_he = next_outlink2(l, NULL, _gwb_l_he);
		ReplaceArcSrc(he->arc, l2);
	}

	CreateArc(e, he_1, _gwb_he);
	CreateArc(e, he_2, _gwb_he);
	CreateArc(he_1, v1, _gwb_he_vertex);
	CreateArc(he_2, v2, _gwb_he_vertex);

	return 0;
}

AGmNode* AGmGWB::block(double dx, double dy, double dz) {
	AGmNode *f[6], *v[8];
	dx /= 2; dy /= 2; dz /= 2;

	// face1
	AGmNode *s = mvsf(-dx, -dy, -dz, &f[0], &v[0]);
	mev(s, f[0], v[0],	dx, -dy, -dz, &v[1]);
	mev(s, f[0], v[1],	dx,  dy, -dz, &v[2]);
	mev(s, f[0], v[2], -dx,  dy, -dz, &v[3]);
	mef(s, f[0], v[3], v[0], &f[1]);
	// sides
	mev(s, f[0], v[0], -dx, -dy, dz, &v[4]);
	mev(s, f[0], v[1],	dx, -dy, dz, &v[5]);
	mev(s, f[0], v[2],	dx,  dy, dz, &v[6]);
	mev(s, f[0], v[3], -dx,  dy, dz, &v[7]);
	// face2
	mef(s, f[0], v[4], v[5], &f[2]);
	mef(s, f[0], v[5], v[6], &f[3]);
	mef(s, f[0], v[6], v[7], &f[4]);
	mef(s, f[0], v[7], v[4], &f[5]);

	return s;
}

AGmNode* AGmGWB::cylinder(double r, double h, int n) {
	AGmNode **f = new AGmNode*[n + 2];
	AGmNode **v = new AGmNode*[n * 2];
	h /= 2;

	// circle1
	AGmNode *s = mvsf(r, 0, -h, &f[0], &v[0]);
	int i;
	for (i = 0; i < n - 1; i++) {
		double x = r * cos(2 * PI * (i+1) / n);
		double y = r * sin(2 * PI * (i+1) / n);
		mev(s, f[0], v[i],	x, y, -h, &v[i + 1]);
	}
	mef(s, f[0], v[n - 1], v[0], &f[1]);
	// sides
	for (i = 0; i < n; i++) {
		double x = r * cos(2 * PI * i / n);
		double y = r * sin(2 * PI * i / n);
		mev(s, f[0], v[i],	x, y, h, &v[n + i]);
	}
	// circle2
	for (i = 0; i < n - 1; i++) {
		mef(s, f[0], v[n + i], v[n + i + 1], &f[i + 2]);
	}
	mef(s, f[0], v[n*2 - 1], v[n], &f[n + 1]);

	delete [] f; delete [] v;

	return s;
}

AGmNode* AGmGWB::cone(double r, double h, int n) {
	AGmNode **f = new AGmNode*[n + 1];
	AGmNode **v = new AGmNode*[n + 1];

	// circle
	AGmNode *s = mvsf(r, 0, -h/4, &f[0], &v[0]);
	int i;
	for (i = 0; i < n - 1; i++) {
		double x = r * cos(2 * PI * (i+1) / n);
		double y = r * sin(2 * PI * (i+1) / n);
		mev(s, f[0], v[i],	x, y, -h/4, &v[i + 1]);
	}
	mef(s, f[0], v[n - 1], v[0], &f[1]);
	// pole
	mev(s, f[0], v[0],	0, 0, h*3/4, &v[n]);
	// sides
	for (i = 1; i < n; i++) {
		mef(s, f[0], v[n], v[i], &f[i + 1]);
	}

	delete [] f; delete [] v;

	return s;
}

AGmNode* AGmGWB::sphere(double r, int n, int n2) {
	AGmNode *cf, **f = new AGmNode*[n * (n2 * 2) + 1];
	AGmNode *nv, *sv, **v = new AGmNode*[n * n2];

	int i, j;
	double z, r1;

	// north pole
	AGmNode *s = mvsf(0, 0, r, &cf, &nv);
	z = r * cos(PI / (n2+1));
	r1 = r * sin(PI / (n2+1));
	for (j = 0; j < n; j++) {
		double x = r1 * cos(2 * PI * j / n);
		double y = r1 * sin(2 * PI * j / n);
		// sides
		mev(s, cf, nv, x, y, z, &v[j]);
	}
	for (j = 0; j < n; j++) {
		// sides
		mef(s, cf, v[(j+1) % n], v[j], &f[j]);
	}
	for (i = 2; i <= n2; i++) {
		z = r * cos(PI * i / (n2+1));
		r1 = r * sin(PI * i / (n2+1));
		for (j = 0; j < n; j++) {
			double x = r1 * cos(2 * PI * j / n);
			double y = r1 * sin(2 * PI * j / n);
			// sides
			mev(s, cf, v[j + n * (i-2)],  x, y, z, &v[j + n * (i-1)]);
		}
		for (j = 0; j < n; j++) {
			// sides
			mef(s, cf, v[(j+1) % n + n * (i-1)],
				v[j + n * (i-1)], &f[j + n * (i-1)]);
		}
	}
	// south pole
	mev(s, cf, v[n * (n2-1)], 0, 0, -r, &sv);
	for (j = 0; j < n; j++) {
		// sides
		mef(s, cf, v[j + n * (n2-1)], sv, &f[j + n * (n2-1)]);
	}

	delete [] f; delete [] v;

	return s;
}

void AGmGWB::loop_abcd(AGmNode* l, double* a, double *b, double *c, double *d) {
	if (IsReal(l->TH)) {
		*a = l->THR;
		*b = l->TTHR;
		*c = l->TTTHR;
		*d = l->TTTTHR;
		return;
	}
	*a = *b = *c = *d = 0;
	AGmNode *he = dst_node2(l, _gwb_l_he), *n_he;
	AGmNode *xyz = dst_node2(he, _gwb_he_vertex);
	double x = xyz->THR;
	double y = xyz->TTHR;
	double z = xyz->TTTHR;
	AGmLink *hel, *n_hel;
	for (hel = next_outlink2(l, NULL, _gwb_l_he); hel; hel = next_outlink2(l, hel, _gwb_l_he)) {
		he = hel->arc->dst;
		n_hel = next_outlink2(l, hel, _gwb_l_he);
		if (n_hel == NULL) n_hel = next_outlink2(l, NULL, _gwb_l_he);
		n_he = n_hel->arc->dst;
		AGmNode *xyz1 = dst_node2(he, _gwb_he_vertex);
		AGmNode *xyz2 = dst_node2(n_he, _gwb_he_vertex);
		if (xyz1 == xyz || xyz2 == xyz) continue;
		double x1 = xyz1->THR  - x;
		double y1 = xyz1->TTHR - y;
		double z1 = xyz1->TTTHR - z;
		double x2 = xyz2->THR  - x;
		double y2 = xyz2->TTHR - y;
		double z2 = xyz2->TTTHR - z;
		double cmx = y1 * z2 - y2 * z1;
		double cmy = z1 * x2 - z2 * x1;
		double cmz = x1 * y2 - x2 * y1;
		*a += cmx; *b += cmy; *c += cmz;
	}
	double len = sqrt(*a * *a + *b * *b + *c * *c);
	*a /= len; *b /= len; *c /= len;
	*d = - (*a * x + *b * y + *c * z);
	Set(l->TH, Real(*a));
	Set(l->TTH, Real(*b));
	Set(l->TTTH, Real(*c));
	Set(l->TTTTH, Real(*d));
}

void AGmGWB::vertex_T_xyz(AGmNode* xyz, AGmMatrix* mat, double* x, double* y, double* z) {
	if (mat) {
		AGmMatrix *xyz1 = new AGmMatrix(4, 1);
		xyz1->Component(0, 0) = xyz->THR;
		xyz1->Component(1, 0) = xyz->TTHR;
		xyz1->Component(2, 0) = xyz->TTTHR;
		xyz1->Component(3, 0) = 1;
		AGmMatrix *xyz2 = mat->Mul(xyz1);
		delete xyz1; xyz1 = xyz2;
		*x = xyz1->Component(0, 0);
		*y = xyz1->Component(1, 0);
		*z = xyz1->Component(2, 0);
		delete xyz1;
	} else {
		*x = xyz->THR;
		*y = xyz->TTHR;
		*z = xyz->TTTHR;
	}
}

void AGmGWB::draw(ADvWindow *wnd, double org_x, double org_y, AGmMatrix *mat, AGmNode *s, int opt) {
	AGmNode *fs = dst_node(s, _gwb_faces);
	AGmLink *fl; AGmNode *f;
	AGmLink *ll; AGmNode *l;
	AGmLink *hel; AGmNode *he;
	AGmNode *es = dst_node(s, _gwb_edges);
	AGmLink *el; AGmNode *e;

	int backface_removal = 0;
	if (opt == 1) backface_removal = 1;
	switch (opt) {
	case 0:
	case 1:
		if (backface_removal) {
			AGmMatrix *inv_mat = mat->Copy();
			mat->Component(0, 3) = mat->Component(3, 0) = 0;
			mat->Component(1, 3) = mat->Component(3, 1) = 0;
			mat->Component(2, 3) = mat->Component(3, 2) = 0;
			mat->Component(3, 3) = 1;
			AGmMatrix *abcd = new AGmMatrix(4, 1);
			for (fl = next_outlink2(fs, NULL, _gwb_face); fl; fl = next_outlink2(fs, fl, _gwb_face)) {
				f = fl->arc->dst;
				l = dst_node2(f, _gwb_loop);
				loop_abcd(l, &abcd->Component(0, 0), &abcd->Component(1, 0), &abcd->Component(2, 0), &abcd->Component(3, 0));
				AGmMatrix *abcd2 = mat->Mul(abcd);
				delete abcd; abcd = abcd2;
				if (abcd->Component(2, 0) > 0) {
					// positive face
					for (ll = next_outlink2(f, NULL, _gwb_loop); ll; ll = next_outlink2(f, ll, _gwb_loop)) {
						l = ll->arc->dst;
						for (hel = next_outlink2(l, NULL, _gwb_l_he); hel; hel = next_outlink2(l, hel, _gwb_l_he)) {
							he = hel->arc->dst;
							Set(he->H, Integer(1)); // mark
						}
					}
				}
			}
			delete inv_mat; delete abcd;
		}
		for (el = next_outlink2(es, NULL, _gwb_edge); el; el = next_outlink2(es, el, _gwb_edge)) {
			e = el->arc->dst;
			AGmNode *itr = Ref(DstItr(e));
			AGmNode *he1 = itr_next(itr);
			AGmNode *he2 = itr_next(itr);
			Unref(itr);
			double x1, y1, z1, x2, y2, z2;
			if (backface_removal == 0 || he1->H || he2->H) {
				vertex_T_xyz(dst_node2(he1, _gwb_he_vertex), mat, &x1, &y1, &z1);
				vertex_T_xyz(dst_node2(he2, _gwb_he_vertex), mat, &x2, &y2, &z2);
				wnd->Line(org_x + x1, org_y - y1, org_x + x2, org_y - y2);
			}
		}
		if (backface_removal) {
			for (el = next_outlink2(es, NULL, _gwb_edge); el; el = next_outlink2(es, el, _gwb_edge)) {
				e = el->arc->dst;
				AGmNode *itr = Ref(DstItr(e));
				AGmNode *he1 = itr_next(itr);
				AGmNode *he2 = itr_next(itr);
				Set(he1->H, NULL); // unmark
				Set(he2->H, NULL); // unmark
				Unref(itr);
			}
		}
		break;
	}
}

AGmNode* AGmGWB::setop(AGmNode *a, AGmNode* b, int op) {
	a = GraphCopy(a);
	MakeStrUnique(a);
	switch (op) {
	case OP_UNION:
	case OP_INTERSECTION:
		b = GraphCopy(b);
		MakeStrUnique(b);
		break;
	case OP_MINUS:
		b = reverse(b);
		break;
	}
	AGmNode *afs = dst_node2(a, _gwb_faces);
	AGmNode *bfs = dst_node2(b, _gwb_faces);
	AGmLink *afl; AGmNode *af;
	AGmLink *bfl; AGmNode *bf = NULL;
	AGmNode *nes = Ref(Cons(NULL));
	AGmNode *nfs = Ref(Cons(NULL));
	AGmNode *nbound = Ref(Cons(NULL));
	int flag = 0;

	for (afl = next_outlink2(afs, NULL, _gwb_face); afl; afl = next_outlink2(afs, afl, _gwb_face)) {
		af = afl->arc->dst;
		for (bfl = next_outlink2(bfs, NULL, _gwb_face); bfl; bfl = next_outlink2(bfs, bfl, _gwb_face)) {
			bf = bfl->arc->dst;
			flag |= setop_f_f(af, bf, nes, nfs);
		}
	}

	if (flag) {
		AGmNode *p, *itr0, *itr;
		AGmNode *e1 = NULL, *e2, *fe1 = NULL, *fe2, *e, *es;
		AGmNode *he1, *he2 = NULL, *fhe1 = NULL, *fhe2 = NULL, *itr1, *itr2, *itr3, *itr4;

		itr1 = Ref(DstItr(nes));
		itr2 = Ref(DstItr(nfs));
		for (;;) {
			AGmNode *nedges = itr_next(itr1);
			AGmNode *nholes = itr_next(itr2);
			if (nedges == NULL && nholes == NULL) break;
			f_f_process(nedges, nholes, nbound);
			itr_remove(itr1);
			itr_remove(itr2);
		}
		Unref(itr1, itr2);

		itr0 = Ref(DstItr(nbound));
		for (p = itr_next(itr0); p; p = itr_next(itr0)) {
			itr = Ref(DstItr(p));
			e1 = itr_next(itr); itr_remove(itr);
			fe1 = itr_next(itr); itr_remove(itr);
			e2 = itr_next(itr); itr_remove(itr);
			fe2 = itr_next(itr); itr_remove(itr);
			Unref(itr);

			int flag = 0;
			itr1 = Ref(DstItr(e1));
			itr2 = Ref(DstItr(e2));
			itr3 = Ref(DstItr(fe1));
			itr4 = Ref(DstItr(fe2));
			for (he1 = itr_next(itr1); he1; he1 = itr_next(itr1)) {
				for (he2 = itr_next(itr2); he2; he2 = itr_next(itr2)) {
					if (src_node2(src_node2(he1, _gwb_l_he), _gwb_loop)
							!= src_node2(src_node2(he2, _gwb_l_he), _gwb_loop))
						continue;
					if (Compare(dst_node2(he1, _gwb_he_vertex)->H, dst_node2(he2, _gwb_he_vertex)->H) == 0)
						continue;
					for (fhe1 = itr_next(itr3); fhe1; fhe1 = itr_next(itr3)) {
						for (fhe2 = itr_next(itr4); fhe2; fhe2 = itr_next(itr4)) {
							if (src_node2(src_node2(fhe1, _gwb_l_he), _gwb_loop)
									!= src_node2(src_node2(fhe2, _gwb_l_he), _gwb_loop))
								continue;
							if (Compare(dst_node2(fhe1, _gwb_he_vertex)->H, dst_node2(fhe2, _gwb_he_vertex)->H) == 0)
								continue;
							if (Compare(dst_node2(fhe1, _gwb_he_vertex)->H, dst_node2(he1, _gwb_he_vertex)->H) == 0)
								continue;
							flag = 1; break;
						}
						if (flag) break;
					}
					if (flag) break;
				}
				if (flag) break;
			}
			Unref(itr1, itr2, itr3, itr4);
			if (flag) {
				CreateArc(p, he1); CreateArc(p, fhe1);
				CreateArc(p, he2); CreateArc(p, fhe2);
				continue;
			}

			flag = 0;
			itr1 = Ref(DstItr(e1));
			itr2 = Ref(DstItr(fe2));
			itr3 = Ref(DstItr(fe1));
			itr4 = Ref(DstItr(e2));
			for (he1 = itr_next(itr1); he1; he1 = itr_next(itr1)) {
				for (he2 = itr_next(itr2); he2; he2 = itr_next(itr2)) {
					if (src_node2(src_node2(he1, _gwb_l_he), _gwb_loop)
							!= src_node2(src_node2(he2, _gwb_l_he), _gwb_loop))
						continue;
					if (Compare(dst_node2(he1, _gwb_he_vertex)->H, dst_node2(he2, _gwb_he_vertex)->H) == 0)
						continue;
					for (fhe1 = itr_next(itr3); fhe1; fhe1 = itr_next(itr3)) {
						for (fhe2 = itr_next(itr4); fhe2; fhe2 = itr_next(itr4)) {
							if (src_node2(src_node2(fhe1, _gwb_l_he), _gwb_loop)
									!= src_node2(src_node2(fhe2, _gwb_l_he), _gwb_loop))
								continue;
							if (Compare(dst_node2(fhe1, _gwb_he_vertex)->H, dst_node2(fhe2, _gwb_he_vertex)->H) == 0)
								continue;
							if (Compare(dst_node2(fhe1, _gwb_he_vertex)->H, dst_node2(he1, _gwb_he_vertex)->H) == 0)
								continue;
							flag = 1; break;
						}
						if (flag) break;
					}
					if (flag) break;
				}
				if (flag) break;
			}
			Unref(itr1, itr2, itr3, itr4);
			if (flag) {
				CreateArc(p, he1); CreateArc(p, fhe1);
				CreateArc(p, he2); CreateArc(p, fhe2);
				continue;
			}
			ASSERT(0);
		}
		Unref(itr0);

		itr0 = Ref(DstItr(nbound));
		for (p = itr_next(itr0); p; p = itr_next(itr0)) {
			itr = Ref(DstItr(p));
			he1 = itr_next(itr); itr_remove(itr);
			fhe1 = itr_next(itr); itr_remove(itr);
			he2 = itr_next(itr); itr_remove(itr);
			fhe2 = itr_next(itr); itr_remove(itr);
			Unref(itr);

			connect_he(he1, he2);
			connect_he(fhe1, fhe2);

			es = src_node2(e1, _gwb_edge);
			e = List(Integer(B_EDGE), (AGmNode*)NULL);
			CreateArc(es, e, _gwb_edge);
			CreateArc(e, he1, _gwb_he);
			CreateArc(e, fhe2, _gwb_he);

			es = src_node2(fe1, _gwb_edge);
			e = List(Integer(B_EDGE), (AGmNode*)NULL);
			CreateArc(es, e, _gwb_edge);
			CreateArc(e, fhe1, _gwb_he);
			CreateArc(e, he2, _gwb_he);
		}
		Unref(itr0);

		es = dst_node2(a, _gwb_edges);
		itr1 = Ref(DstItr(es));
		for (e = itr_next(itr1); e; e = itr_next(itr1)) {
			if (IsInteger(e->H) && e->HI == BOUNDARY) {
				itr_remove(itr1); continue;
			}
			if (e->H == NULL || e->HI != B_EDGE) {
				continue;
			}
			itr2 = Ref(DstItr(e));
			for (he1 = itr_next(itr2); he1; he1 = itr_next(itr2)) {
				union_v(he1, e);
			}
			Unref(itr2);
			Set(e->H, NULL);
		}
		Unref(itr1);

		es = dst_node2(b, _gwb_edges);
		itr1 = Ref(DstItr(es));
		for (e = itr_next(itr1); e; e = itr_next(itr1)) {
			if (IsInteger(e->H) && e->HI == BOUNDARY) {
				itr_remove(itr1); continue;
			}
			if (e->H == NULL || e->HI != B_EDGE) {
				continue;
			}
			itr2 = Ref(DstItr(e));
			for (he1 = itr_next(itr2); he1; he1 = itr_next(itr2)) {
				union_v(he1, e);
			}
			Unref(itr2);
			Set(e->H, NULL);
		}
		Unref(itr1);
		Unref(nes, nfs, nbound);

		mark_f_e_v(a);
		mark_f_e_v(b);
		switch (op) {
		case OP_UNION:
			remove_elem(a, INSIDE);
			remove_elem(b, INSIDE);
			break;
		case OP_INTERSECTION:
		case OP_MINUS:
			remove_elem(a, OUTSIDE);
			remove_elem(b, OUTSIDE);
			break;
		}
	} else {
		// todo
	}

	for (;;) {
		int flag = 1;
		for (afl = next_outlink2(afs, NULL, _gwb_face); afl; afl = next_outlink2(afs, afl, _gwb_face)) {
			af = afl->arc->dst;
			if (af->H) continue;
			for (bfl = next_outlink2(bfs, NULL, _gwb_face); bfl; bfl = next_outlink2(bfs, bfl, _gwb_face)) {
				bf = bfl->arc->dst;
				if (bf->H) continue;
				switch (op) {
				case OP_UNION:
					switch (include_check(af, bf)) {
					case A_IN_B:
						remove_solid(af); flag = 0; break;
					case B_IN_A:
						remove_solid(bf); flag = 0; break;
					default:
						break;
					}
					break;
				case OP_INTERSECTION:
					switch (include_check(af, bf)) {
					case A_IN_B:
						remove_solid(bf); flag = 0; break;
					case B_IN_A:
						remove_solid(af); flag = 0; break;
					default:
						remove_solid(af); remove_solid(bf); flag = 0; break;
						break;
					}
					break;
				case OP_MINUS:
					switch (include_check(af, bf)) {
					case A_IN_B:
						remove_solid(af); remove_solid(bf); flag = 0; break;
					case B_IN_A: // cave
						// not allow caves
						remove_solid(bf); flag = 0; break;
					default:
						break;
					}
					break;
				}
			}
		}
		if (flag) break;
	}

	if (op == OP_MINUS) {
		for (;;) {
			for (bfl = next_outlink2(bfs, NULL, _gwb_face); bfl; bfl = next_outlink2(bfs, bfl, _gwb_face)) {
				bf = bfl->arc->dst;
				if (bf->H == NULL) break;
			}
			if (bfl == NULL) break;
			remove_solid(bf);
		}
	}

	append_breps(a, b);
	Unref(b);
	clear_mark(a);

	return a;
}

int AGmGWB::setop_f_f(AGmNode *af, AGmNode* bf,
		AGmNode *nes, AGmNode* nfs) {
	AGmNode *nedges = Cons(NULL);
	AGmNode *nholes = Cons(NULL);
	AGmLink *ll; AGmNode *l;
	AGmLink *hel, *n_hel; AGmNode *he, *n_he;
	int flag = 0;

	for (ll = next_outlink2(bf, NULL, _gwb_loop); ll; ll = next_outlink2(bf, ll, _gwb_loop)) {
		l = ll->arc->dst;
		for (hel = next_outlink2(l, NULL, _gwb_l_he); hel; hel = next_outlink2(l, hel, _gwb_l_he)) {
			he = hel->arc->dst;
			n_hel = next_outlink2(l, hel, _gwb_l_he);
			if (n_hel == NULL) n_hel = next_outlink2(l, NULL, _gwb_l_he);
			n_he = n_hel->arc->dst;
			flag |= setop_f_e(af, he, n_he, nedges, nholes);
		}
	}

	for (ll = next_outlink2(af, NULL, _gwb_loop); ll; ll = next_outlink2(af, ll, _gwb_loop)) {
		l = ll->arc->dst;
		for (hel = next_outlink2(l, NULL, _gwb_l_he); hel; hel = next_outlink2(l, hel, _gwb_l_he)) {
			he = hel->arc->dst;
			n_hel = next_outlink2(l, hel, _gwb_l_he);
			if (n_hel == NULL) n_hel = next_outlink2(l, NULL, _gwb_l_he);
			n_he = n_hel->arc->dst;
			flag |= setop_f_e(bf, he, n_he, nedges, nholes);
		}
	}

	if (flag == 0) {
		RefUnref(nedges, nholes);
	} else {
		CreateArc(nes, nedges);
		CreateArc(nfs, nholes);
	}

	return flag;
}

int AGmGWB::setop_f_e(AGmNode *f, AGmNode* he, AGmNode* n_he,
		AGmNode *nedges, AGmNode* nholes) {

	if (IsInteger(he->H) && he->HI == BOUNDARY) return 0;

	double x, y, z, t;
	AGmNode *inter_he;
	AGmNode *e, *inter_e;
	AGmNode *itr1, *itr2, *he0, *l;
	AGmNode *he_v, *inter_he_v;
	AGmLink *he_l;
	int flag = intersection_check_f_e(f, he, n_he, &x, &y, &z, &t, &inter_he);
	switch (flag) {
	case NO_INTERSECTION:
		return 0;
	case INTERSECTION:
		if (fabs(t) < ERR || fabs(1 - t) < ERR) {
			//ASSERT(0);
			break;
		}
		f_e_process(f, he, x, y, z, nedges, nholes);
		return 1;
	case ON_VERTEX:
	case ON_EDGE:
	case ON_FACE:
		ASSERT(0);
		break;
	case POINT_ON_VERTEX:
		e = src_node2(he, _gwb_he);
		if (e->H == NULL || e->HI != BOUNDARY) break;
		inter_e = src_node2(inter_he, _gwb_he);
		if (inter_e->H == NULL || inter_e->HI != BOUNDARY) break;
		
		itr1 = Ref(DstItr(inter_e));
		for (he0 = itr_next(itr1); he0; he0 = itr_next(itr1)) {
			if (he0 != inter_he) break;
		}
		Unref(itr1);
		ASSERT(he0);
		if (IsInteger(inter_he->H) && inter_he->HI == BOUNDARY
				&& IsInteger(he0->H) && he0->HI == BOUNDARY) break;
		he_v = dst_node2(he, _gwb_he_vertex);
		inter_he_v = dst_node2(inter_he, _gwb_he_vertex);
		if (Compare(he_v->H, inter_he_v->H) != 0) break;
		
		CreateArc(nedges, inter_e);
		CreateArc(nholes, e);
		Set(he->H, Integer(BOUNDARY));
		Set(inter_he->H, Integer(BOUNDARY));
		Set(he0->H, Integer(BOUNDARY));
		return 1;
	case POINT_ON_EDGE:
		ASSERT(0);
		return 0;
	case POINT_ON_FACE:
		e = src_node2(he, _gwb_he);
		if (e->H == NULL || e->HI != BOUNDARY) break;
		itr1 = Ref(DstItr(f));
		for (l = itr_next(itr1); l; l = itr_next(itr1)) {
			if (l == dst_node2(f, _gwb_loop)) continue;
			int flag1 = point_in_polygon(l, x, y, z, &inter_he);
			if (flag1 != POINT_ON_VERTEX && flag1 != ON_VERTEX) continue;
			inter_e = src_node2(inter_he, _gwb_he);
			if (inter_e->H == NULL || inter_e->HI != BOUNDARY) break;

			itr2 = Ref(DstItr(inter_e));
			for (he0 = itr_next(itr2); he0; he0 = itr_next(itr2)) {
				if (he0 != inter_he) break;
			}
			Unref(itr2);
			ASSERT(he0);
			if (IsInteger(inter_he->H) && inter_he->HI == BOUNDARY
					&& IsInteger(he0->H) && he0->HI == BOUNDARY) break;

			he_l = find_outlink(e, he);
			if (he_l->next->next->next == he_l) {
				he_v = dst_node2(he, _gwb_he_vertex);
				inter_he_v = dst_node2(inter_he, _gwb_he_vertex);
				if (Compare(he_v->H, inter_he_v->H) != 0) break;

				CreateArc(nedges, inter_e);
				CreateArc(nholes, e);
				Set(he->H, Integer(BOUNDARY));
				Set(inter_he->H, Integer(BOUNDARY));
				Set(he0->H, Integer(BOUNDARY));
				return 1;
			} else {
				CreateArc(nedges, e);
				CreateArc(nholes, inter_e);
				Set(he->H, Integer(BOUNDARY));
				Set(inter_he->H, Integer(BOUNDARY));
				Set(he0->H, Integer(BOUNDARY));
				return 1;
			}
		}
		Unref(itr1);
		break;
	case EDGE_ON_FACE:
	default:
		ASSERT(0);
		break;
	}
	return 0;
}

void AGmGWB::f_e_process(AGmNode* f, AGmNode* he1,
		double x, double y, double z, AGmNode* nedges, AGmNode* nholes) {
	AGmNode *l0 = dst_node(f, _gwb_loop);
	AGmNode *v0 = dst_node(he1, _gwb_he_vertex);
	double a, b, c, d;
	double x1, y1, z1;
	loop_abcd(l0, &a, &b, &c, &d);
	vertex_T_xyz(v0, NULL, &x1, &y1, &z1);
	int side;
	if (a * x1 + b * y1 + c * z1 + d < 0) {
		side = INSIDE;
	} else {
		side = OUTSIDE;
	}

	// split edge
	{
		AGmNode *e = src_node2(he1, _gwb_he);
		AGmNode *es = src_node2(e, _gwb_edge);
		AGmNode *v = dst_node2(he1, _gwb_he_vertex);
		AGmNode *vs = src_node2(v, _gwb_vertex);
		AGmLink *hel; AGmNode *he2 = NULL;
		for (hel = next_outlink2(e, NULL, _gwb_he); hel; hel = next_outlink2(e, hel, _gwb_he)) {
			he2 = hel->arc->dst;
			if (he2 != he1) break;
		}
		ASSERT(he2);
		AGmNode *l1 = src_node2(he1, _gwb_l_he);
		AGmNode *l2 = src_node2(he2, _gwb_l_he);
		AGmLink *he1l = find_outlink(l1, he1);
		AGmLink *he2l = find_outlink(l2, he2);

		AGmNode *e1 = List((AGmNode*)NULL, (AGmNode*)NULL);
		AGmNode *e2 = List((AGmNode*)NULL, (AGmNode*)NULL);
		AGmNode *v1 = Cons(NULL, List(x, y, z));
		AGmNode *v2 = Cons(NULL, List(x, y, z));
		AGmNode *he11 = Cons(NULL);
		AGmNode *he12 = Cons(NULL);
		AGmNode *he21 = Cons(NULL);
		AGmNode *he22 = Cons(NULL);
		CreateArc(es, e1, _gwb_edge);
		CreateArc(es, e2, _gwb_edge);
		CreateArc(vs, v1, _gwb_vertex);
		CreateArc(vs, v2, _gwb_vertex);
		AGmLink *lnk;
		CreateArc(l1, he11, _gwb_l_he, he1l, NULL, &lnk);
		CreateArc(l1, he21, _gwb_l_he, lnk);
		CreateArc(l2, he12, _gwb_l_he, he2l->next, NULL, &lnk);
		CreateArc(l2, he22, _gwb_l_he, lnk->next);

		CreateArc(e1, he11, _gwb_he);
		CreateArc(e1, he12, _gwb_he);
		CreateArc(e2, he21, _gwb_he);
		CreateArc(e2, he22, _gwb_he);
		Ref(v);
		ReplaceArcDst(he1l->arc->dst->body->out->next->arc, v1);
		CreateArc(he12, v1, _gwb_he_vertex);
		CreateArc(he11, v2, _gwb_he_vertex);
		CreateArc(he22, v2, _gwb_he_vertex);
		CreateArc(he21, v, _gwb_he_vertex);
		Unref(v);

		Set(e1->H, Integer(BOUNDARY));
		Set(he11->H, Integer(BOUNDARY));
		Set(v1->H, Integer(OPPOSITE_SIDE(side)));	
		Set(v2->H, Integer(side));
		
		CreateArc(nedges, e1);
	}

	// make pin hole in face
	{
		AGmNode *fs = src_node2(f, _gwb_face);
		AGmNode *s = src_node2(fs, _gwb_faces);
		AGmNode *es = dst_node2(s, _gwb_edges);
		AGmNode *vs = dst_node2(s, _gwb_vertices);

		AGmNode *l = List((AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL);
		AGmNode *he1 = Cons(NULL);
		AGmNode *he2 = Cons(NULL);
		AGmNode *e = List((AGmNode*)NULL, (AGmNode*)NULL);
		AGmNode *v1 = Cons(NULL, List(x, y, z));
		AGmNode *v2 = Cons(NULL, List(x, y, z));
		CreateArc(f, l, _gwb_loop);
		CreateArc(l, he1, _gwb_l_he);
		CreateArc(l, he2, _gwb_l_he);
		CreateArc(es, e, _gwb_edge);
		CreateArc(vs, v1, _gwb_vertex);
		CreateArc(vs, v2, _gwb_vertex);

		CreateArc(e, he1, _gwb_he);
		CreateArc(e, he2, _gwb_he);
		CreateArc(he2, v1, _gwb_he_vertex);
		CreateArc(he1, v2, _gwb_he_vertex);

		Set(e->H, Integer(BOUNDARY));
		Set(he1->H, Integer(BOUNDARY));
		Set(v1->H, Integer(OPPOSITE_SIDE(side)));	
		Set(v2->H, Integer(side));

		CreateArc(nholes, e);
	}
}

void AGmGWB::f_f_process(AGmNode* nedges, AGmNode* nholes, AGmNode* nbound) {
	AGmNode *itr = Ref(DstItr(nedges));
	AGmNode *e1 = itr_next(itr), *fe1;
	AGmNode *e2 = itr_next(itr), *fe2;
	Unref(itr);
	AGmNode *he1 = dst_node2(e1, _gwb_he);
	AGmNode *he2 = dst_node2(e2, _gwb_he);
	AGmNode *v1 = dst_node2(he1, _gwb_he_vertex);
	AGmNode *v2 = dst_node2(he2, _gwb_he_vertex);
	double x1, y1, z1, x2, y2, z2;
	vertex_T_xyz(v1, NULL, &x1, &y1, &z1);
	vertex_T_xyz(v2, NULL, &x2, &y2, &z2);
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dz = z2 - z1;

	AGmNode *e;
	itr = Ref(DstItr(nedges));
	for (e = itr_next(itr); e; e = itr_next(itr)) {
		he2 = dst_node2(e, _gwb_he);
		v2 = dst_node2(he2, _gwb_he_vertex);
		vertex_T_xyz(v2, NULL, &x2, &y2, &z2);
		double dx2 = x2 - x1;
		double dy2 = y2 - y1;
		double dz2 = z2 - z1;
		Set(e->TH, Real(dx * dx2 + dy * dy2 + dz * dz2));
	}
	Unref(itr);

	for (;;) {
		int flag = 0;
		itr = Ref(DstItr(nedges));
		for (e = itr_next(itr); e; e = itr_next(itr)) {
			if (e->TH) flag = 1;
		}
		Unref(itr);
		if (flag == 0) break;

		double mini = 1e30;
		AGmLink *e_l = nedges->body->out->next, *e_l_min = NULL;
		AGmLink *fe_l = nholes->body->out->next, *fe_l_min = NULL;
		while (e_l != nedges->body->out) {
			if (e_l->arc->dst->TH && e_l->arc->dst->THR < mini) {
				mini = e_l->arc->dst->THR;
				e_l_min = e_l; fe_l_min = fe_l;
			}
			e_l = e_l->next; fe_l = fe_l->next;
		}
		ASSERT(mini < 1e30);
		e1 = e_l_min->arc->dst; fe1 = fe_l_min->arc->dst;
		Set(e1->TH, NULL);

		mini = 1e30;
		e_l = nedges->body->out->next;
		fe_l = nholes->body->out->next;
		while (e_l != nedges->body->out) {
			if (e_l->arc->dst->TH && e_l->arc->dst->THR < mini) {
				mini = e_l->arc->dst->THR;
				e_l_min = e_l; fe_l_min = fe_l;
			}
			e_l = e_l->next; fe_l = fe_l->next;
		}
		ASSERT(mini < 1e30);
		e2 = e_l_min->arc->dst; fe2 = fe_l_min->arc->dst;
		Set(e2->TH, NULL);

		AGmNode *p = Cons(NULL);
		CreateArc(nbound, p);
		CreateArc(p, e1);
		CreateArc(p, fe1);
		CreateArc(p, e2);
		CreateArc(p, fe2);
	}
}

void AGmGWB::connect_he(AGmNode* he1, AGmNode* he2) {
	AGmNode *l1 = src_node2(he1, _gwb_l_he);
	AGmNode *l2 = src_node2(he2, _gwb_l_he);
	AGmNode *f1 = src_node2(l1, _gwb_loop);
	AGmNode *f2 = src_node2(l2, _gwb_loop);
	AGmNode *fs = src_node2(f1, _gwb_face);

	AGmLink *he1_l = find_outlink(l1, he1);
	AGmLink *he2_l = find_outlink(l2, he2);
	AGmLink *n_he1_l = he1_l->next;
	AGmLink *n_he2_l = he2_l->next;
	he1_l->next = n_he2_l; n_he2_l->prev = he1_l;
	he2_l->next = n_he1_l; n_he1_l->prev = he2_l;

	if (l1 != l2) { // union
		if (l1 == dst_node2(f1, _gwb_loop)) { // l1 is outer loop
			AGM_REMOVE(l2->body->out);
			l2->body->out->next = l2->body->out->prev = l2->body->out;
			AGmLink *hel = find_outlink(f2, l2);
			DeleteArc(hel->arc);
			for (hel = next_outlink2(l1, NULL, _gwb_l_he); hel; hel = next_outlink2(l1, hel, _gwb_l_he)) {
				hel->arc->src = l1;
			}
		} else { // l1 is inner loop
			AGM_REMOVE(l1->body->out);
			l1->body->out->next = l1->body->out->prev = l1->body->out;
			AGmLink *hel = find_outlink(f1, l1);
			DeleteArc(hel->arc);
			for (hel = next_outlink2(l2, NULL, _gwb_l_he); hel; hel = next_outlink2(l2, hel, _gwb_l_he)) {
				hel->arc->src = l2;
			}
		}
	} else { // disunion
		f2 = Cons(NULL);
		l2 = List((AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL, (AGmNode*)NULL);
		{
			AGmNodeBody *nb = l2->body = (AGmNodeBody*)Alloc();
			nb->type = AGM_BODY_OF_NODE;
			nb->flag = 0;
			nb->nid = 0;
			nb->in = nb->out = NULL;

			AGmLink *l = l2->body->out = (AGmLink*)Alloc();
			l->type = AGM_DMY_OUT_LINK;
			l->flag = 0;
			l->next = l->prev = l;
		}
		CreateArc(fs, f2, _gwb_face);
		CreateArc(f2, l2, _gwb_loop);
		if (l1 == dst_node2(f1, _gwb_loop)) { // l1 is outer loop
			AGM_REMOVE(l1->body->out);
			AGM_INSERT(l1->body->out, he1_l);
			AGM_INSERT(l2->body->out, he2_l);
			if (f1->body->out->next->next != f1->body->out) { // include inner loop
				double a, b, c, d;
				loop_abcd(l1, &a, &b, &c, &d);
				double x1, y1, z1, x2, y2, z2;
				vertex_T_xyz(dst_node2(he1, _gwb_he_vertex), NULL, &x1, &y1, &z1);
				vertex_T_xyz(dst_node2(he2, _gwb_he_vertex), NULL, &x2, &y2, &z2);
				double dx = x2 - x1;
				double dy = y2 - y1;
				double dz = z2 - z1;
				AGmLink *ll; AGmNode *l;
				for (ll = next_outlink2(f1, NULL, _gwb_loop); ll; ll = next_outlink2(f1, ll, _gwb_loop)) {
					l = ll->arc->dst;
					if (l == l1) continue;
					AGmNode *he = dst_node2(l, _gwb_l_he);
					vertex_T_xyz(dst_node2(he, _gwb_he_vertex), NULL, &x2, &y2, &z2);
					double dx2 = x2 - x1;
					double dy2 = y2 - y1;
					double dz2 = z2 - z1;
					double x = dy * dz2 - dy2 * dz;
					double y = dz * dx2 - dz2 * dx;
					double z = dx * dy2 - dx2 * dy;
					if (a * x + b * y + c * z < 0) {
						AGmLink *p_ll = ll->prev;
						AGM_REMOVE(ll);
						AGM_INSERT(ll, f2->body->out);
						ll->arc->src = f2;
						ll = p_ll;
					}
				}
			}
		} else { // l1 is inner loop
			AGM_REMOVE(l1->body->out);
			AGmNode *he1_v = dst_node(he1, _gwb_he_vertex);
			if (IsInteger(he1_v->H) && he1_v->HI == OUTSIDE) {
				AGM_INSERT(l1->body->out, he1_l);
				AGM_INSERT(l2->body->out, he2_l);
			} else {
				AGM_INSERT(l2->body->out, he1_l);
				AGM_INSERT(l1->body->out, he2_l);
			}
		}
		AGmLink *hel;
		for (hel = next_outlink2(l2, NULL, _gwb_l_he); hel; hel = next_outlink2(l2, hel, _gwb_l_he)) {
			hel->arc->src = l2;
		}
	}
}

void AGmGWB::union_v(AGmNode* he1, AGmNode *e) {
	AGmNode *itr = Ref(DstItr(e));
	AGmNode *he2;
	for (he2 = itr_next(itr); he2; he2 = itr_next(itr)) {
		if (he2 != he1) break;
	}
	Unref(itr);
	ASSERT(he2);

	union_v1(he1, he2);
	union_v1(he2, he1);
}
void AGmGWB::union_v1(AGmNode* dhe, AGmNode *she) {
	AGmNode *dv = dst_node2(dhe, _gwb_he_vertex);
	AGmNode *sl = src_node2(she, _gwb_l_he);
	AGmLink *she_l = find_outlink(sl, she);
	she_l = next_outlink(sl, she_l);
	if (she_l == NULL) she_l = next_outlink(sl, NULL);
	she = she_l->arc->dst;
	AGmNode *sv = dst_node2(she, _gwb_he_vertex);
	if (dv == sv) return;

	AGmLink *hel, *p_hel;
	for (hel = next_inlink2(sv, NULL, _gwb_he_vertex); hel; hel = next_inlink2(sv, hel, _gwb_he_vertex)) {
		p_hel = hel->prev;
		ReplaceArcDst(hel->arc, dv);
		hel = p_hel;
	}

	AGmNode *vs = src_node2(sv, _gwb_vertex);
	AGmLink *lnk = find_outlink(vs, sv);
	DeleteArc(lnk->arc);
}

void AGmGWB::mark_f_e_v(AGmNode *a) {
	AGmNode *vs = dst_node2(a, _gwb_vertices);
	AGmNode *itr = Ref(DstItr(vs));
	AGmNode *v;
	for (v = itr_next(itr); v; v = itr_next(itr)) {
		if (IsInteger(v->H)) {
			mark_from_v(v, (int)v->HI);
		}
	}
	Unref(itr);
}
void AGmGWB::mark_from_v(AGmNode *v1, int side) {
	Set(v1->H, Integer(side));
	
	AGmNode *itr1 = Ref(SrcItr(v1));
	AGmNode *he1;
	for (he1 = itr_next(itr1, _gwb_he_vertex); he1; he1 = itr_next(itr1, _gwb_he_vertex)) {
		Set(he1->H, Integer(side));
		AGmNode *l1 = src_node2(he1, _gwb_l_he);
		Set(l1->H, Integer(side));
		AGmNode *f1 = src_node2(l1, _gwb_loop);
		Set(f1->H, Integer(side));

		AGmNode *itr2 = Ref(DstItr(f1));
		AGmNode *l2;
		for (l2 = itr_next(itr2, _gwb_loop); l2; l2 = itr_next(itr2, _gwb_loop)) {
			if (l2 == l1) continue;
			AGmNode *he2 = dst_node2(l2, _gwb_l_he);
			AGmNode *v2 = dst_node2(he2, _gwb_he_vertex);
			if (IsInteger(v2->H) && v2->HI == side) continue;
			ASSERT(v2->H == NULL);
			mark_from_v(v2, side);
		}
		Unref(itr2);

		AGmNode *e = src_node2(he1, _gwb_he);
		ASSERT(e);
		Set(e->H, Integer(side));

		itr2 = Ref(DstItr(e));
		AGmNode *he2;
		for (he2 = itr_next(itr2, _gwb_he); he2; he2 = itr_next(itr2, _gwb_he)) {
			if (he2 != he1) break;
		}
		Unref(itr2);
		ASSERT(he2);

		Set(he2->H, Integer(side));
		l2 = src_node2(he2, _gwb_l_he);
		Set(l2->H, Integer(side));
		AGmNode *f2 = src_node2(l2, _gwb_loop);
		Set(f2->H, Integer(side));

		AGmNode *v2 = dst_node2(he2, _gwb_he_vertex);
		if (IsInteger(v2->H) && v2->HI == side) continue;
		ASSERT(v2->H == NULL);
		mark_from_v(v2, side);
	}
	Unref(itr1);
}

void AGmGWB::remove_elem(AGmNode* a, int side) {
	AGmNode *fs = dst_node2(a, _gwb_faces);
	AGmNode *es = dst_node2(a, _gwb_edges);
	AGmNode *vs = dst_node2(a, _gwb_vertices);
	AGmNode *itr, *f, *e, *v;

	itr = Ref(DstItr(fs));
	for (f = itr_next(itr); f; f = itr_next(itr)) {
		if (IsInteger(f->H) && f->HI == side) {
			itr_remove(itr);
		}
	}
	Unref(itr);

	itr = Ref(DstItr(es));
	for (e = itr_next(itr); e; e = itr_next(itr)) {
		if (IsInteger(e->H) && e->HI == side) {
			itr_remove(itr);
		}
	}
	Unref(itr);

	itr = Ref(DstItr(vs));
	for (v = itr_next(itr); v; v = itr_next(itr)) {
		if (IsInteger(v->H) && v->HI == side) {
			itr_remove(itr);
		}
	}
	Unref(itr);
}

void AGmGWB::append_breps(AGmNode* a, AGmNode *b) {
	AGmNode *dfs = dst_node2(a, _gwb_faces);
	AGmNode *des = dst_node2(a, _gwb_edges);
	AGmNode *dvs = dst_node2(a, _gwb_vertices);
	AGmNode *fs = dst_node2(b, _gwb_faces);
	AGmNode *es = dst_node2(b, _gwb_edges);
	AGmNode *vs = dst_node2(b, _gwb_vertices);
	AGmNode *itr, *f, *e, *v;

	itr = Ref(DstItr(fs));
	for (f = itr_next(itr); f; f = itr_next(itr)) {
		CreateArc(dfs, f, _gwb_face);		
		itr_remove(itr);
	}
	Unref(itr);

	itr = Ref(DstItr(es));
	for (e = itr_next(itr); e; e = itr_next(itr)) {
		CreateArc(des, e, _gwb_edge);		
		itr_remove(itr);
	}
	Unref(itr);

	itr = Ref(DstItr(vs));
	for (v = itr_next(itr); v; v = itr_next(itr)) {
		CreateArc(dvs, v, _gwb_face);		
		itr_remove(itr);
	}
	Unref(itr);
}

void AGmGWB::clear_mark(AGmNode* a) {
	AGmNode *fs = dst_node2(a, _gwb_faces);
	AGmNode *es = dst_node2(a, _gwb_edges);
	AGmNode *vs = dst_node2(a, _gwb_vertices);
	AGmNode *itr, *f, *e, *v;
	AGmNode *itr2, *he;

	itr = Ref(DstItr(fs));
	for (f = itr_next(itr); f; f = itr_next(itr)) {
		Set(f->H, NULL);
	}
	Unref(itr);

	itr = Ref(DstItr(es));
	for (e = itr_next(itr); e; e = itr_next(itr)) {
		Set(e->H, NULL);
		itr2 = Ref(DstItr(e));
		for (he = itr_next(itr2); he; he = itr_next(itr2)) {
			Set(he->H, NULL);
		}
		Unref(itr2);
	}
	Unref(itr);

	itr = Ref(DstItr(vs));
	for (v = itr_next(itr); v; v = itr_next(itr)) {
		Set(v->H, NULL);
	}
	Unref(itr);
}

void AGmGWB::dump_breps(char* s, AGmNode *a) {
	ADvWindow::Print("======== ");
	ADvWindow::Print(s);
	ADvWindow::Print(" ========\n");
	char msg[256];

	AGmNode *fs = dst_node2(a, _gwb_faces);
	AGmNode *es = dst_node2(a, _gwb_edges);
	AGmNode *vs = dst_node2(a, _gwb_vertices);
	AGmNode *itr1, *itr2, *itr3, *f, *l, *he, *e, *v;

	itr1 = Ref(DstItr(fs));
	for (f = itr_next(itr1); f; f = itr_next(itr1)) {
		art_sprintf_2(msg, 256, "face = %lx, mark=%ld\n", AL_PTR(f), (IsInteger(f->H) ? f->HI : -1));
		ADvWindow::Print(msg);
		itr2 = Ref(DstItr(f));
		for (l = itr_next(itr2); l; l = itr_next(itr2)) {
			art_sprintf_2(msg, 256, "\tloop = %lx, mark=%ld\n", AL_PTR(l), (IsInteger(l->H) ? l->HI : -1));
			ADvWindow::Print(msg);
			itr3 = Ref(DstItr(l));
			for (he = itr_next(itr3); he; he = itr_next(itr3)) {
				art_sprintf_2(msg, 256, "\t\the = %lx, mark=%ld\n", AL_PTR(he), (IsInteger(l->H) ? l->HI : -1));
				ADvWindow::Print(msg);
				v = dst_node2(he, _gwb_he_vertex);
				art_sprintf_5(msg, 256, "\t\t\tv = %lx, mark=%ld, (%f, %f, %f)\n", AL_PTR(v), (IsInteger(v->H) ? v->HI : -1),
					v->THR, v->TTHR, v->TTTHR);
				ADvWindow::Print(msg);
			}
			Unref(itr3);
		}
		Unref(itr2);
	}
	Unref(itr1);

	itr1 = Ref(DstItr(es));
	for (e = itr_next(itr1); e; e = itr_next(itr1)) {
		art_sprintf_2(msg, 256, "edge = %lx, mark=%ld\n", AL_PTR(e), (IsInteger(e->H) ? e->HI : -1));
		ADvWindow::Print(msg);
		itr2 = Ref(DstItr(e));
		for (he = itr_next(itr2); he; he = itr_next(itr2)) {
			art_sprintf_2(msg, 256, "\the = %lx, mark=%ld\n", AL_PTR(he), (IsInteger(he->H) ? he->HI : -1));
			ADvWindow::Print(msg);
			v = dst_node2(he, _gwb_he_vertex);
			art_sprintf_5(msg, 256, "\t\tv = %lx, mark=%ld, (%f, %f, %f)\n", AL_PTR(v), (IsInteger(v->H) ? v->HI : -1),
				v->THR, v->TTHR, v->TTTHR);
			ADvWindow::Print(msg);
		}
		Unref(itr2);
	}
	Unref(itr1);

	itr1 = Ref(DstItr(vs));
	for (v = itr_next(itr1); v; v = itr_next(itr1)) {
		art_sprintf_5(msg, 256, "vertex = %lx, mark=%ld, (%f, %f, %f)\n", AL_PTR(v), (IsInteger(v->H) ? v->HI : -1),
			v->THR, v->TTHR, v->TTTHR);
		ADvWindow::Print(msg);
	}
	Unref(itr1);
}

int AGmGWB::include_check(AGmNode* af, AGmNode* bf) {
	double axmin, aymin, azmin, axmax, aymax, azmax;
	double bxmin, bymin, bzmin, bxmax, bymax, bzmax;
	AGmNode *l, *he, *v;
	double x, y, z;
	int count;

	axmin = aymin = azmin = bxmin = bymin = bzmin = 1e30;
	axmax = aymax = azmax = bxmax = bymax = bzmax = -1e30;

	solid_box(af, &axmin, &aymin, &azmin, &axmax, &aymax, &azmax);
	solid_box(bf, &bxmin, &bymin, &bzmin, &bxmax, &bymax, &bzmax);

	if (bxmin < axmin && axmax < bxmax
			&& bymin < aymin && aymax < bymax
			&& bzmin < azmin && azmax < bzmax) {
		l = dst_node(af, _gwb_loop);
		he = dst_node(l, _gwb_l_he);
		v = dst_node(he, _gwb_he_vertex);
		vertex_T_xyz(v, NULL, &x, &y, &z);
		count = 0;
		intersection_count(bf, x, y, z, &count);
		if (count % 2 == 1) return A_IN_B;
		return 0;
	}
	if (axmin < bxmin && bxmax < axmax
			&& aymin < bymin && bymax < aymax
			&& azmin < bzmin && bzmax < azmax) {
		l = dst_node(bf, _gwb_loop);
		he = dst_node(l, _gwb_l_he);
		v = dst_node(he, _gwb_he_vertex);
		vertex_T_xyz(v, NULL, &x, &y, &z);
		count = 0;
		intersection_count(af, x, y, z, &count);
		if (count % 2 == 1) return B_IN_A;
		return 0;
	}
	return 0;
}
void AGmGWB::solid_box(AGmNode* f,
		double *xmin, double* ymin, double* zmin,
		double* xmax, double* ymax, double* zmax) {
	AGmNode *itr1, *itr2, *itr3;
	AGmNode *l, *he, *e, *he0, *l0, *f0, *v;
	double x, y, z;

	Set(f->H, Integer(-1));

	itr1 = Ref(DstItr(f));
	for (l = itr_next(itr1); l; l = itr_next(itr1)) {
		itr2 = Ref(DstItr(l));
		for (he = itr_next(itr2); he; he = itr_next(itr2)) {
			v = dst_node2(he, _gwb_he_vertex);
			vertex_T_xyz(v, NULL, &x, &y, &z);
			*xmin = min(*xmin, x); *ymin = min(*ymin, y); *zmin = min(*zmin, z);
			*xmax = max(*xmax, x); *ymax = max(*ymax, y); *zmax = max(*zmax, z);
			e = src_node2(he, _gwb_he);
			itr3 = Ref(DstItr(e));
			for (he0 = itr_next(itr3); he0; he0 = itr_next(itr3)) {
				if (he0 != he) break;
			}
			Unref(itr3);
			ASSERT(he0);
			l0 = src_node2(he0, _gwb_l_he);
			f0 = src_node2(l0, _gwb_loop);
			if (f0->H == NULL || f0->HI != -1) {
				solid_box(f0, xmin, ymin, zmin, xmax, ymax, zmax);
			}
		}
		Unref(itr2);
	}
	Unref(itr1);

	Set(f->H, NULL);
}
void AGmGWB::intersection_count(AGmNode* f, double x0, double y0, double z0, int* count) {
	AGmNode *itr1, *itr2, *itr3;
	AGmNode *l1 = dst_node(f, _gwb_loop);
	AGmNode *l, *he, *e, *he0, *l0, *f0, *inter_he;
	double a, b, c, d;
	double x, y, z, t;

	loop_abcd(l1, &a, &b, &c, &d);
	double dis1 = a * x0 + b * y0 + c * z0 + d;
	double dis2 = a * 1e30 + b * 1e30 + c * 1e30 + d;
	
	if (dis1 * dis2 <= ERR) {
		t = dis1 / (dis1 - dis2);
		x = x0 + t * (1e30 - x0);
		y = y0 + t * (1e30 - y0);
		z = z0 + t * (1e30 - z0);
		int flag = point_in_polygon(l1, x, y, z, &inter_he);
		if (flag == ON_FACE) (*count)++;
	}

	Set(f->H, Integer(-1));

	itr1 = Ref(DstItr(f));
	for (l = itr_next(itr1); l; l = itr_next(itr1)) {
		itr2 = Ref(DstItr(l));
		for (he = itr_next(itr2); he; he = itr_next(itr2)) {
			e = src_node2(he, _gwb_he);
			itr3 = Ref(DstItr(e));
			for (he0 = itr_next(itr3); he0; he0 = itr_next(itr3)) {
				if (he0 != he) break;
			}
			Unref(itr3);
			ASSERT(he0);
			l0 = src_node2(he0, _gwb_l_he);
			f0 = src_node2(l0, _gwb_loop);
			if (f0->H == NULL || f0->HI != -1) {
				intersection_count(f0, x0, y0, z0, count);
			}
		}
		Unref(itr2);
	}
	Unref(itr1);

	Set(f->H, NULL);
}

void AGmGWB::remove_solid(AGmNode* f) {
	AGmNode *itr1, *itr2, *itr3, *l, *he, *v, *e, *he0, *l0, *f0;

	Set(f->H, Integer(REMOVAL));
	itr1 = Ref(DstItr(f));
	for (l = itr_next(itr1); l; l = itr_next(itr1)) {
		itr2 = Ref(DstItr(l));
		for (he = itr_next(itr2); he; he = itr_next(itr2)) {
			v = dst_node2(he, _gwb_he_vertex);
			DeleteNode(v);
			e = src_node2(he, _gwb_he);
			itr3 = Ref(DstItr(e));
			for (he0 = itr_next(itr3); he0; he0 = itr_next(itr3)) {
				if (he0 != he) break;
			}
			Unref(itr3);
			DeleteNode(e);
			if (he0 == NULL) continue;
			l0 = src_node2(he0, _gwb_l_he);
			f0 = src_node2(l0, _gwb_loop);
			if (f0->H == NULL || f0->HI != REMOVAL) {
				remove_solid(f0);
			}
		}
		Unref(itr2);
	}
	Unref(itr1);
	DeleteNode(f);
}

AGmNode* AGmGWB::reverse(AGmNode* s) {
	s = GraphCopy(s);
	MakeStrUnique(s);
	AGmNode *fs = dst_node(s, _gwb_faces);
	AGmLink *fl; AGmNode *f;
	AGmLink *ll; AGmNode *l;
	for (fl = next_outlink2(fs, NULL, _gwb_face); fl; fl = next_outlink2(fs, fl, _gwb_face)) {
		f = fl->arc->dst;
		for (ll = next_outlink2(f, NULL, _gwb_loop); ll; ll = next_outlink2(f, ll, _gwb_loop)) {
			l = ll->arc->dst;
			AGmLink *out = (AGmLink*)Alloc();
			out->type = AGM_DMY_OUT_LINK;
			out->flag = 0;
			out->next = out->prev = out;
			AGmLink *lnk0 = l->body->out->next;
			AGmNode *he0 = lnk0->arc->dst;
			AGmNode *v0 = Ref(dst_node(he0, _gwb_he_vertex));
			while (l->body->out->next != l->body->out) {
				AGmLink *lnk = l->body->out->next;
				AGM_REMOVE(lnk);
				AGmLink *p = out->next;
				AGM_INSERT(lnk, p);
				AGmNode *v2;
				if (l->body->out->next == l->body->out) {
					v2 = v0;
				} else {
					AGmLink *lnk2 = l->body->out->next;
					AGmNode *he2 = lnk2->arc->dst;
					v2 = Ref(dst_node2(he2, _gwb_he_vertex));
				}
				ReplaceArcDst(lnk->arc->dst->body->out->next->arc, v2);
				Unref(v2);
			}
			Free((AGmCell*)l->body->out);
			l->body->out = out;
		}
	}
	return s;
}

int AGmGWB::intersection_check_f_e(AGmNode* f, AGmNode* he, AGmNode* n_he,
		double *x, double *y, double *z, double *t, AGmNode** inter_he) {
	AGmNode *l0 = dst_node(f, _gwb_loop);
	AGmNode *v1 = dst_node(he, _gwb_he_vertex);
	AGmNode *v2 = dst_node(n_he, _gwb_he_vertex);
	double a, b, c, d;
	double x1, y1, z1, x2, y2, z2;
	AGmLink *ll; AGmNode *l;

	loop_abcd(l0, &a, &b, &c, &d);
	vertex_T_xyz(v1, NULL, &x1, &y1, &z1);
	vertex_T_xyz(v2, NULL, &x2, &y2, &z2);

	double dis1 = a * x1 + b * y1 + c * z1 + d;
	double dis2 = a * x2 + b * y2 + c * z2 + d;

	if (dis1 * dis2 > ERR) return NO_INTERSECTION;

	if (fabs(dis1 -dis2) < ERR) {
		if (fabs(dis1) > ERR || fabs(dis2) > ERR) return NO_INTERSECTION;

		// null edge
		if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2)) < ERR) {
			int flag = point_in_polygon(l0, x1, y1, z1, inter_he);
			switch (flag) {
			case NO_INTERSECTION:
				return NO_INTERSECTION;
			case ON_VERTEX:
				return POINT_ON_VERTEX;
			case ON_EDGE:
				return POINT_ON_EDGE;
			case ON_FACE:
				for (ll = next_outlink2(f, NULL, _gwb_loop); ll; ll = next_outlink2(f, ll, _gwb_loop)) {
					l = ll->arc->dst;
					if (l == l0) continue;
					flag = point_in_polygon(l, x1, y1, z1, inter_he);
					switch (flag) {
					case NO_INTERSECTION:
						continue;
					case ON_VERTEX:
						return POINT_ON_VERTEX;
					case ON_EDGE:
						return POINT_ON_EDGE;
					case ON_FACE:
						return NO_INTERSECTION;
					}

				}
				return POINT_ON_FACE;
			}
		}

		return EDGE_ON_FACE;
	}

	*t = dis1 / (dis1 - dis2);
	*x = x1 + *t * (x2 - x1);
	*y = y1 + *t * (y2 - y1);
	*z = z1 + *t * (z2 - z1);

	AGmLink *hel; AGmNode *p_l, *p_he, *p_e, *n_e;
	int flag = point_in_polygon(l0, *x, *y, *z, inter_he);
	switch (flag) {
	case NO_INTERSECTION:
		return NO_INTERSECTION;
	case ON_VERTEX:
		return ON_VERTEX;
	case ON_EDGE:
		return ON_EDGE;
	case ON_FACE:
		for (ll = next_outlink2(f, NULL, _gwb_loop); ll; ll = next_outlink2(f, ll, _gwb_loop)) {
			l = ll->arc->dst;
			if (l == l0) continue;
			flag = point_in_polygon(l, x1, y1, z1, inter_he);
			switch (flag) {
			case NO_INTERSECTION:
				continue;
			case ON_VERTEX:
				p_l = src_node2(he, _gwb_l_he);
				hel = find_outlink(p_l, he);
				hel = prev_outlink(p_l, hel);
				if (hel == NULL) hel = prev_outlink(p_l, NULL);
				p_he = hel->arc->dst;
				p_e = src_node2(p_he, _gwb_he);
				n_e = src_node2(n_he, _gwb_he);
				if (IsInteger(p_e->H) && p_e->HI == BOUNDARY)
					return NO_INTERSECTION;
				if (IsInteger(n_e->H) && n_e->HI == BOUNDARY)
					return NO_INTERSECTION;
				return ON_VERTEX;
			case ON_EDGE:
				return ON_EDGE;
			case ON_FACE:
				return NO_INTERSECTION;
			}
		}
		return INTERSECTION;
	}

	return 0;
}

int AGmGWB::point_in_polygon(AGmNode* l, double x, double y, double z, AGmNode** inter_he) {
	double a, b, c, d;

	// pin hole
	AGmNode *he = dst_node2(l, _gwb_l_he);
	AGmLink *he_l = find_outlink(l, he);
	if (he_l->next->next->next == he_l) {
		AGmNode *he = l->body->out->next->arc->dst;
		AGmNode *v = dst_node2(he, _gwb_he_vertex);
		double x1, y1, z1;
		vertex_T_xyz(v, NULL, &x1, &y1, &z1);
		if (sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y) + (z1 - z) * (z1 - z)) < ERR) {
			*inter_he = he;
			return ON_VERTEX;
		}
		return NO_INTERSECTION;
	}

	loop_abcd(l, &a, &b, &c, &d);
	double theta, sum_theta = 0;

	AGmLink *hel, *n_hel;
	for (hel = next_outlink2(l, NULL, _gwb_l_he); hel; hel = next_outlink2(l, hel, _gwb_l_he)) {
		n_hel = next_outlink2(l, hel, _gwb_l_he);
		if (n_hel == NULL) n_hel = next_outlink2(l, NULL, _gwb_l_he);
		AGmNode *he = hel->arc->dst;
		AGmNode *n_he = n_hel->arc->dst;
		AGmNode *v1 = dst_node(he, _gwb_he_vertex);
		AGmNode *v2 = dst_node(n_he, _gwb_he_vertex);

		double x1, y1, z1, x2, y2, z2;
		vertex_T_xyz(v1, NULL, &x1, &y1, &z1);
		x1 -= x; y1 -= y; z1 -= z;
		double len1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
		if (len1 < ERR) {
			*inter_he = he; return ON_VERTEX;
		}
		vertex_T_xyz(v2, NULL, &x2, &y2, &z2);
		x2 -= x; y2 -= y; z2 -= z;
		double len2 = sqrt(x2 * x2 + y2 * y2 + z2 * z2);
		if (len2 < ERR) {
			*inter_he = n_he; return ON_VERTEX;
		}

		double tmp = (x1 * x2 + y1 * y2 + z1 * z2) / (len1 * len2);
		if (fabs(tmp) > 1) tmp = (tmp > 0 ? 1 : -1);
		theta = acos(tmp);
		if (fabs(PI - theta) < ERR) {
			*inter_he = he; return ON_EDGE;
		}

		double cmx = y1 * z2 - y2 * z1;
		double cmy = z1 * x2 - z2 * x1;
		double cmz = x1 * y2 - x2 * y1;
		if (a * cmx + b * cmy + c * cmz < 0) theta = -theta;
		sum_theta += theta;
	}
	sum_theta = fabs(sum_theta);

	if (sum_theta < PI) {
		return NO_INTERSECTION;
	} else {
		return ON_FACE;
	}
}

