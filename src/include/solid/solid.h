/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_SOLID_H
#define _AGM_SOLID_H

class AGmGWB : AGm {
public:
	static AGmNode* mvsf(double x, double y, double z,
		AGmNode** f, AGmNode** v);
	static int mev(AGmNode *s, AGmNode* f, AGmNode* v1,
		double x, double y, double z,
		AGmNode** v2);
	static int mef(AGmNode *s, AGmNode* f1, AGmNode* v1, AGmNode* v2,
		AGmNode** f2);

	static AGmNode* block(double dx, double dy, double dz);
	static AGmNode* cylinder(double r, double h, int n);
	static AGmNode* cone(double r, double h, int n);
	static AGmNode* sphere(double r, int n, int n2);

	static void loop_abcd(AGmNode* l, double* a, double *b, double *c, double *d);
	static void vertex_T_xyz(AGmNode* xyz, AGmMatrix* mat, double* x, double* y, double* z);
	static void draw(ADvWindow *wnd, double org_x, double org_y, AGmMatrix *mat, AGmNode *s, int opt);

	static AGmNode* setop(AGmNode *a, AGmNode* b, int op);
	static int setop_f_f(AGmNode *af, AGmNode* bf, AGmNode *nes, AGmNode* nfs);
	static int setop_f_e(AGmNode *f, AGmNode* he, AGmNode* n_he,
		AGmNode *nedges, AGmNode* nfaces);
	static void f_e_process(AGmNode* f, AGmNode* he1,
		double x, double y, double z, AGmNode* nedges, AGmNode* nholes);
	static void f_f_process(AGmNode* nedges, AGmNode* nholes, AGmNode* nbound);
	static void connect_he(AGmNode* he1, AGmNode* he2);
	static void union_v(AGmNode* he1, AGmNode *e);
	static void union_v1(AGmNode* dhe, AGmNode *she);
	static void mark_f_e_v(AGmNode *a);
	static void mark_from_v(AGmNode *v1, int side);
	static void remove_elem(AGmNode* a, int side);
	static void append_breps(AGmNode* a, AGmNode *b);
	static void clear_mark(AGmNode* a);
	static void dump_breps(char* s, AGmNode *a);

	static int include_check(AGmNode* af, AGmNode* bf);
	static void solid_box(AGmNode* f,
		double *xmin, double* ymin, double* zmin,
		double* xmax, double* ymax, double* zmax);
	static void intersection_count(AGmNode* bf, double x, double y, double z, int* count);
	static void remove_solid(AGmNode* f);

	static AGmNode* reverse(AGmNode* s);

	static int intersection_check_f_e(AGmNode* f, AGmNode* he, AGmNode* n_he,
		double *x, double *y, double *z, double *t, AGmNode** inter_he);
	static int point_in_polygon(AGmNode* l, double x, double y, double z, AGmNode** inter_he);

	static void init_str();
	static void finalize_str();
	static AGmNode *_gwb_faces;
	static AGmNode *_gwb_face;
	static AGmNode *_gwb_loop;
	static AGmNode *_gwb_l_he;
	static AGmNode *_gwb_he_vertex;
	static AGmNode *_gwb_edges;
	static AGmNode *_gwb_edge;
	static AGmNode *_gwb_he;
	static AGmNode *_gwb_vertices;
	static AGmNode *_gwb_vertex;
};

#define ERR 1e-5

#define OP_UNION		1
#define OP_INTERSECTION 2
#define OP_MINUS		3

#define NO_INTERSECTION 0
#define INTERSECTION	1
#define ON_VERTEX		2
#define ON_EDGE 		3
#define ON_FACE 		4
#define POINT_ON_VERTEX 5
#define POINT_ON_EDGE	6
#define POINT_ON_FACE	7
#define EDGE_ON_FACE	8

#define INSIDE		0
#define OUTSIDE 	1
#define BOUNDARY	2
#define B_EDGE		4
#define OPPOSITE_SIDE(side) ((side) == INSIDE ? OUTSIDE : INSIDE)
#define REMOVAL 	8

#define A_IN_B	1
#define B_IN_A	2

#endif
