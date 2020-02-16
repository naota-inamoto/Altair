/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

// alcore.cpp : definition of initialization for DLL
//

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE alcoreDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("ALCORE1D.DLL Initializing!\n");
		
		if (!AfxInitExtensionModule(alcoreDLL, hInstance))
			return 0;

		// insert this dll to resource chain
		new CDynLinkLibrary(alcoreDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ALCORE1D.DLL Terminating!\n");
		AfxTermExtensionModule(alcoreDLL);
	}
	return 1;	// ok
}
