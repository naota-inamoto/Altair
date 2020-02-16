/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ALTAIR_ERROR_H
#define _ALTAIR_ERROR_H

#define MEMORY_OVERFLOW 	1000
#define CELL_HEAP_OVERFLOW	1001
#define REF_COUNT_OVERFLOW	1002
#define DISK_IS_FULL		1003
#define VTBL_ERROR			1004

class AlException {
public:
	AlException(int, char*);
	~AlException();

	int ErrorCode();
	char*Message();

protected:
	int error_code;
	char* msg;
};

#endif	/* _ALTAIR_ERROR_H */

