/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/error.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

AlException::AlException(int error_code, char* msg) {
	this->error_code = error_code;
	this->msg = art_strdup(msg);
}
AlException::~AlException() {
	free(msg);
}
int AlException::ErrorCode() {
	return error_code;
}
char* AlException::Message() {
	return msg;
}

