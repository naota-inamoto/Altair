/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ALTAIR_LIBINC_H
#define _ALTAIR_LIBINC_H

#ifdef	_WINDOWS

//#define	WINVER			0x0501
//#define	_WIN32_WINNT	0x0501
//#define	_WIN32_WINDOWS	0x0501
//#define	_WIN32_IE		0x0501

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// eliminate un-used part from Windows header
#endif

#include <afxwin.h> 	// standard component of MFC
#include <afxext.h> 	// extension part of MFC (include VB)

#include <afxctl.h>
#include <afxcmn.h> 	// common control
#include <afxdisp.h>	// OLE automation class of MFC
#include <afxpriv.h>

#if _MFC_VER <= 0x0600
#include <occimpl.h>
#endif
#if _MFC_VER >= 0x0700
#include <afxocc.h>
#endif

#include <sql.h>
#include <sqlext.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>
#include <sys/utime.h>
#include <sys/timeb.h>

#include <io.h>

#endif	// _WINDOWS

#include <altair/alext.h>

#if defined(_WINDOWS) && defined(AL_USE_PCH)
#include <gdm/gdm.h>
#include <gdm/string.h>
#include <gdm/matrix.h>
#include <gdm/file.h>
#include <gdm/vmkey.h>
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <gdm/j_code.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <interp/basefunc.h>
#include <interp/sysfunc.h>
#include <interp/prog_if.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <dev/bitmap.h>
#include <dev/ocx.h>
#include <dev/olectrl.h>
#include <dev/mainfrm.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/body.h>
#include <cui/cui.h>
#include <cui/project.h>
#include <altair/strres.h>
#endif	/* AL_USE_PCH */

#ifdef	_LINUX

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>
#include <sys/timeb.h>
#include <signal.h>

#ifdef	_LINUX_DEBUG
#include <assert.h>
#define ASSERT(arg) assert((arg)!=0)
int trace(char *format, ...);
#define TRACE(f)				trace(f)
#define TRACE0(f)				trace(f)
#define TRACE1(f,a1)			trace(f,a1)
#define TRACE2(f,a1,a2) 		trace(f,a1,a2)
#define TRACE3(f,a1,a2,a3)		trace(f,a1,a2,a3)
#define TRACE4(f,a1,a2,a3,a4)	trace(f,a1,a2,a3,a4)
#else
#define ASSERT(arg)
#define TRACE(f)
#define TRACE0(f)
#define TRACE1(f,a1)
#define TRACE2(f,a1,a2)
#define TRACE3(f,a1,a2,a3)
#define TRACE4(f,a1,a2,a3,a4)
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkpixbuf.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib.h>

typedef unsigned char BYTE;

#define MAX_PATH	256

#define max(a,b)	((a) > (b) ? (a) : (b))
#define min(a,b)	((a) < (b) ? (a) : (b))

#define MB_OK		1
#define MB_OKCANCEL 2
#define MB_YESNO	3
#define IDOK		1
#define IDYES		1
int MessageBox(void*, const char* text, const char* caption, int type);

#include <gdm/gdm.h>
#include <gdm/pict.h>
#include <gdm/j_code.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/advwnd.h>

#endif	// _LINUX

#include <altair/alcrt.h>

#endif	/* _ALTAIR_LIBINC_H */
