/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_MATRIX_H
#define _AGM_MATRIX_H

class AGmMatrix : public AGm {
public:
	AGmMatrix(long, long);
	virtual ~AGmMatrix();

	long	NumRow() const { return nrow; }
	long	NumColumn() const { return ncol; }
	double& Component(long i, long j) const { return data[i][j]; }	

	int 		IsEqual(AGmMatrix*) const;
	AGmMatrix*	Copy();
	AGmMatrix*	Transpose();
	AGmMatrix*	ChangeSign();
	AGmMatrix*	Add(AGmMatrix*);
	AGmMatrix*	Sub(AGmMatrix*);
	AGmMatrix*	Mul(AGmMatrix*);
	AGmMatrix*	Inverse();

protected:
	int Gauss();
	double** data;
	long	nrow, ncol; 

public:
	int Transform(double lx, double ly, double& gx, double& gy);
	int Transform(AGmNode* polypoly, AGmNode*& polypoly2);
	int Inverse(double gx, double gy, double& lx, double& ly);
	int Inverse(AGmNode* polypoly, AGmNode*& polypoly2);

	int Id();
	int Translate(double dx, double dy);
	int Scale(double sx, double sy);
	int Rotate(double angle);

	int Id4();
	int Translate(double dx, double dy, double dz);
	int Scale(double sx, double sy, double sz);
	int Rotate_x(double angle);
	int Rotate_y(double angle);
	int Rotate_z(double angle);
};

#endif	/* _AGM_MATRIX_H */

