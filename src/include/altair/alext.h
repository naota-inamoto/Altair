/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ALTAIR_ALEXT_H
#define _ALTAIR_ALEXT_H

#ifdef	_WINDOWS
#ifdef	AL_NO_USE_DLL
#define AL_EXT_CLASS
#define AL_EXT_API
#define AL_EXT_DATA
#else	/* AL_NO_USE_DLL */
#ifdef	AL_CORE_DLL
#define AL_EXT_CLASS	__declspec(dllexport)
#define AL_EXT_API		__declspec(dllexport)
#define AL_EXT_DATA 	__declspec(dllexport)
#else	/* AL_CORE_DLL */
#define AL_EXT_CLASS	__declspec(dllimport)
#define AL_EXT_API		__declspec(dllimport)
#define AL_EXT_DATA 	__declspec(dllimport)
#endif	/* AL_CORE_DLL */
#endif	/* AL_NO_USE_DLL */
#endif	// _WINDOWS

#ifdef	_LINUX
#ifdef	_MAC
#define AL_EXT_CLASS	__attribute__((visibility("default")))
#define AL_EXT_API		__attribute__((visibility("default")))
#define AL_EXT_DATA		__attribute__((visibility("default")))
#else
#define AL_EXT_CLASS
#define AL_EXT_API
#define AL_EXT_DATA
#endif
#endif	// _LINUX

#endif	/* _ALTAIR_ALEXT_H */
