/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AIP_BODY_ITR_H
#define _AIP_BODY_ITR_H

class AGpProgIF : public AGm {
public:
	AGpProgIF();	
	virtual ~AGpProgIF();

	AGmNode** Prog();
	void Prog(AGmNode **);

	int More();
	void Next();

	AGmNode* MemberVar();
	AGmNode* MemberSFunc();
	AGmNode* MemberVFunc();

	int Code_i(long i);
	int Code_i_i(long i, long i2);
	int Code_i_n(long i, AGmNode *n);
	int Code_i_n_n(long i, AGmNode *n, AGmNode *n2);
	void Code_Del();

protected:
	AGmNode **prog, *ctrl_stack;
	int _end;
};

#endif	/* _AIP_BODY_ITR_H */

