/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AL_CRT_H
#define _AL_CRT_H

#if _WINDOWS && _MFC_VER >= 0x0800

#define	AL_STR(str)	str
#define	AL_PTR(ptr)	(unsigned long long)ptr

typedef int mode_t;

int art_open(const char* pathname, int flags);
int art_open2(const char* pathname, int flags, mode_t mode);
#define art_close(fd)	_close(fd)
#define art_read(fd, buf, count)	_read(fd, buf, count)
#define art_write(fd, buf, count)	_write(fd, buf, count)

FILE* art_fopen(const char* path, const char* mode);
#define art_fdopen(fildes, mode)	_fdopen(fildes, mode)
#define art_fileno(stream)	_fileno(stream)

#define art_fscanf_1(stream, format, a1)	fscanf_s(stream, format, a1)
#define art_fscanf_2(stream, format, a1,a2) fscanf_s(stream, format, a1,a2)
#define art_sscanf_1(str, format, a1)	sscanf_s(str, format, a1)
#define art_sscanf_2(str, format, a1,a2)	sscanf_s(str, format, a1,a2)
#define art_sscanf_3(str, format, a1,a2,a3) sscanf_s(str, format, a1,a2,a3)
#define art_sscanf_4(str, format, a1,a2,a3,a4)	sscanf_s(str, format, a1,a2,a3,a4)
#define art_sscanf_5(str, format, a1,a2,a3,a4,a5)	sscanf_s(str, format, a1,a2,a3,a4,a5)
#define art_sscanf_6(str, format, a1,a2,a3,a4,a5,a6)	sscanf_s(str, format, a1,a2,a3,a4,a5,a6)
#define art_sscanf_7(str, format, a1,a2,a3,a4,a5,a6,a7) sscanf_s(str, format, a1,a2,a3,a4,a5,a6,a7)
#define art_sscanf_8(str, format, a1,a2,a3,a4,a5,a6,a7,a8)	sscanf_s(str, format, a1,a2,a3,a4,a5,a6,a7,a8)
#define art_sscanf_9(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9)	sscanf_s(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define art_sscanf_10(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)	sscanf_s(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)
#define art_sscanf_11(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)	sscanf_s(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)
#define art_sscanf_12(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)	sscanf_s(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
#define art_sprintf_0(str, size, format)	sprintf_s(str, size, format)
#define art_sprintf_1(str, size, format, a1)	sprintf_s(str, size, format, a1)
#define art_sprintf_2(str, size, format, a1,a2) sprintf_s(str, size, format, a1,a2)
#define art_sprintf_3(str, size, format, a1,a2,a3)	sprintf_s(str, size, format, a1,a2,a3)
#define art_sprintf_4(str, size, format, a1,a2,a3,a4)	sprintf_s(str, size, format, a1,a2,a3,a4)
#define art_sprintf_5(str, size, format, a1,a2,a3,a4,a5)	sprintf_s(str, size, format, a1,a2,a3,a4,a5)
#define art_sprintf_6(str, size, format, a1,a2,a3,a4,a5,a6) sprintf_s(str, size, format, a1,a2,a3,a4,a5,a6)
#define art_sprintf_7(str, size, format, a1,a2,a3,a4,a5,a6,a7)	sprintf_s(str, size, format, a1,a2,a3,a4,a5,a6,a7)
#define art_sprintf_8(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8)	sprintf_s(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8)
#define art_sprintf_9(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9)	sprintf_s(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define art_sprintf_10(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)	sprintf_s(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)
#define art_sprintf_11(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)	sprintf_s(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)
#define art_sprintf_12(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)	sprintf_s(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
#define art_sprintf_13(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)	sprintf_s(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)
#define art_vsprintf(str, size, format, va_list)	vsprintf_s(str, size, format, va_list)

#define art_strcasecmp(s1, s2)	_stricmp(s1, s2)
char* art_strtok(char* s, const char* delim);
#define art_strdup(s)	_strdup(s)
#define art_strcpy(dest, size, src) strcpy_s(dest, size, src)
#define art_strcat(dest, size, src) strcat_s(dest, size, src)
#define art_strncpy(dest, size, src, n) strncpy_s(dest, size, src, n)
#define art_strncat(dest, size, src, n) strncat_s(dest, size, src, n)

size_t art_wcstombs(char* dest, size_t size, const wchar_t *src, size_t n);
size_t art_mbstowcs(wchar_t *dst, size_t size, const char *src, size_t n);

struct tm* art_localtime(const time_t* timep);
struct tm* art_gmtime(const time_t* timep);
char* art_asctime(const struct tm* tm);

#define art_getcwd(buf, size)	_getcwd(buf, size)
#define art_chdir(path) _chdir(path)
#define art_mkdir(pathname) _mkdir(pathname)
#define art_rmdir(pathname) _rmdir(pathname)
#define art_chmod(path, mode)	_chmod(path, mode)

char* art_getenv(const char* name);
int art_putenv(char* string);
char* art_tempnam(const char* dir, const char* pfx);

#else	/* _LINUX || WINDOWS (_MFC_VER < 0x0800) */

#define	AL_STR(str)	(gchar*)str
#define	AL_PTR(ptr)	(unsigned long)ptr

#define art_open(pathname, flags)	open(pathname, flags)
#define art_open2(pathname, flags, mode)	open(pathname, flags, mode)
#define art_close(fd)	close(fd)
#define art_read(fd, buf, count)	read(fd, buf, count)
#define art_write(fd, buf, count)	write(fd, buf, count)

#define art_fopen(path, mode)	fopen(path, mode)
#define art_fdopen(fildes, mode)	fdopen(fildes, mode)
#define art_fileno(stream)	fileno(stream)

#define art_fscanf_1(stream, format, a1)	fscanf(stream, format, a1)
#define art_fscanf_2(stream, format, a1,a2) fscanf(stream, format, a1,a2)
#define art_sscanf_1(str, format, a1)	sscanf(str, format, a1)
#define art_sscanf_2(str, format, a1,a2)	sscanf(str, format, a1,a2)
#define art_sscanf_3(str, format, a1,a2,a3) sscanf(str, format, a1,a2,a3)
#define art_sscanf_4(str, format, a1,a2,a3,a4)	sscanf(str, format, a1,a2,a3,a4)
#define art_sscanf_5(str, format, a1,a2,a3,a4,a5)	sscanf(str, format, a1,a2,a3,a4,a5)
#define art_sscanf_6(str, format, a1,a2,a3,a4,a5,a6)	sscanf(str, format, a1,a2,a3,a4,a5,a6)
#define art_sscanf_7(str, format, a1,a2,a3,a4,a5,a6,a7) sscanf(str, format, a1,a2,a3,a4,a5,a6,a7)
#define art_sscanf_8(str, format, a1,a2,a3,a4,a5,a6,a7,a8)	sscanf(str, format, a1,a2,a3,a4,a5,a6,a7,a8)
#define art_sscanf_9(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9)	sscanf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define art_sscanf_10(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)	sscanf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)
#define art_sscanf_11(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)	sscanf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)
#define art_sscanf_12(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)	sscanf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
#define art_sprintf_0(str, size, format)	sprintf(str, format)
#define art_sprintf_1(str, size, format, a1)	sprintf(str, format, a1)
#define art_sprintf_2(str, size, format, a1,a2) sprintf(str, format, a1,a2)
#define art_sprintf_3(str, size, format, a1,a2,a3)	sprintf(str, format, a1,a2,a3)
#define art_sprintf_4(str, size, format, a1,a2,a3,a4)	sprintf(str, format, a1,a2,a3,a4)
#define art_sprintf_5(str, size, format, a1,a2,a3,a4,a5)	sprintf(str, format, a1,a2,a3,a4,a5)
#define art_sprintf_6(str, size, format, a1,a2,a3,a4,a5,a6) sprintf(str, format, a1,a2,a3,a4,a5,a6)
#define art_sprintf_7(str, size, format, a1,a2,a3,a4,a5,a6,a7)	sprintf(str, format, a1,a2,a3,a4,a5,a6,a7)
#define art_sprintf_8(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8)	sprintf(str, format, a1,a2,a3,a4,a5,a6,a7,a8)
#define art_sprintf_9(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9)	sprintf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define art_sprintf_10(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)	sprintf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)
#define art_sprintf_11(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)	sprintf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)
#define art_sprintf_12(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)	sprintf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
#define art_sprintf_13(str, size, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)	sprintf(str, format, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)
#define art_vsprintf(str, size, format, va_list)	vsprintf(str, format, va_list)

#define art_strcasecmp(s1, s2)	strcasecmp(s1, s2)
#define art_strtok(s, delim)	strtok(s, delim)
#define art_strdup(s)	strdup(s)
#define art_strcpy(dest, size, src) strcpy(dest, src)
#define art_strcat(dest, size, src) strcat(dest, src)
#define art_strncpy(dest, size, src, n) strncpy(dest, src, n)
#define art_strncat(dest, size, src, n) strncat(dest, src, n)

#define art_wcstombs(dest, size, src, n)	wcstombs(dest, src, n)
#define art_mbstowcs(dest, size, src, n)	mbstowcs(dest, src, n)

#define art_localtime(timep)	localtime(timep)
#define art_gmtime(timep)	gmtime(timep)
#define art_asctime(tm) asctime(tm)

#define art_getcwd(buf, size)	getcwd(buf, size)
#define art_chdir(path) chdir(path)
#define art_mkdir(pathname) mkdir(pathname)
#define art_rmdir(pathname) rmdir(pathname)
#define art_chmod(path, mode)	chmod(path, mode)

#define art_getenv(name)	getenv(name)
#define art_putenv(string)	putenv(string)
#define art_tempnam(dir, pfx)	tempnam(dir, pfx)

#endif

#endif	/* _AL_CRT_H */
