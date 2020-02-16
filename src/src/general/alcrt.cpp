/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <altair/libinc.h>

#if _WINDOWS && _MFC_VER >= 0x0800

#include <share.h>

static struct tm _tm;
static char *context;
static char strRet[4096];

int art_open(const char* pathname, int flags) {
	return art_open2(pathname, flags, (flags & _O_CREAT) ? (_S_IREAD|_S_IWRITE) : 0);
}
int art_open2(const char* pathname, int flags, mode_t mode) {
	int fd = -1;
	errno_t err = _sopen_s(&fd, pathname, flags, _SH_DENYNO, mode);
	if (err) {
		return -1;
	} else {
		return fd;
	}
}

FILE* art_fopen(const char* path, const char* mode) {
	return _fsopen(path, mode, _SH_DENYNO);
}

char* art_strtok(char* s, const char* delim) {
	return strtok_s(s, delim, &context);
}

size_t art_wcstombs(char* dest, size_t size, const wchar_t *src, size_t n) {
	size_t count;
	errno_t err = wcstombs_s(&count, dest, size, src, n);
	if (err) {
		return (size_t)-1;
	} else {
		return count;
	}
}
size_t art_mbstowcs(wchar_t* dest, size_t size, const char *src, size_t n) {
	size_t count;
	errno_t err = mbstowcs_s(&count, dest, size, src, n);
	if (err) {
		return (size_t)-1;
	} else {
		return count;
	}
}

struct tm* art_localtime(const time_t* timep) {
	errno_t err = localtime_s(&_tm, timep); 
	if (err) {
		return NULL;
	} else {
		return &_tm;
	}
}
struct tm* art_gmtime(const time_t* timep) {
	errno_t err = gmtime_s(&_tm, timep);	
	if (err) {
		return NULL;
	} else {
		return &_tm;
	}
}
char* art_asctime(const struct tm* tm) {
	errno_t err = asctime_s(strRet, 4096, tm);
	if (err) {
		return NULL;
	} else {
		return strRet;
	}
}

char* art_getenv(const char* name) {
	size_t size;
	errno_t err = getenv_s(&size, strRet, 4096, name);
	if (err) {
		return NULL;
	} else {
		return strRet;
	}
}
int art_putenv(char* string) {
	int n = (int)strlen(string);
	if (n+1 > 4096) return -1;
	strcpy_s(strRet, 4096, string);

	int i;
	for (i = 0; i < n; i++) {
		if (strRet[i] == '=') break;
	}
	if (i == n) return -1;
	strRet[i] = 0;

	char *name = &strRet[0], *value = &strRet[i+1];
	errno_t err = _putenv_s(name, value);
	if (err) {
		return -1;
	} else {
		return 0;
	}
}
char* art_tempnam(const char* /*dir*/, const char* /*pfx*/) {
	errno_t err = tmpnam_s(strRet, 4096);
	if (err) {
		return NULL;
	} else {
		return strRet;
	}
}

#endif
