/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_PICT_H
#define _AGM_PICT_H

class AGmPict : public AGm {
public:
	static AGmNode* CreatePolyPoly();
	static int CreateNextPoly(AGmNode *polypoly);

	static int AddPointToPoly(AGmNode *polypoly, double x, double y);
	static int AddRectToPoly(AGmNode *polypoly,
			double left, double top, double right, double bottom);
	static int AddCurveToPoly(AGmNode *polypoly,
			double cx1, double cy1, double cx2, double cy2, double ex, double ey);

	static int IsEqualPolyPolygon(AGmNode *polypoly1, AGmNode *polypoly2);
	static int GetPolyPolygonBox(AGmNode *polypoly, double& l, double& t, double& r, double& b);

public:
	static int CurvePts(AGmNode *poly,	double sx, double sy,
			double cx1, double cy1, double cx2, double cy2, double ex, double ey);

};

#endif	/* _AGM_PICT_H */

