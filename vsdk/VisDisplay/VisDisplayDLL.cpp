////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISMESSAGEWND
//
// @module VisMessageWnd.h |
//
// This file defines the <c VisMessageWnd> class.
//
// <nl>
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisMessageWnd\.cpp>
//
// <nl>
// @index | VISMESSAGEWND
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// @doc
// @module VisDisplay.cpp |
// DllMain() definition for VisDisplay.dll
//
// Copyright © Microsoft Corp. 2000
//
////////////////////////////////////////////////////////////////////////////

// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisDisplayPch.h"
#include "VisDisplayProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

static AFX_EXTENSION_MODULE VisDisplayDLL = { FALSE, NULL, NULL, NULL,NULL };

// @func A main function that creates VisDisplay DLL.
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	VisUnreferenced(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("VisDisplay.dll Initializing!\n");
		
		afxCurrentInstanceHandle = hInstance;
		afxCurrentResourceHandle = hInstance;

        ::SetErrorMode(SEM_FAILCRITICALERRORS);

		// Extension DLL one-time initialization
		AfxInitExtensionModule(VisDisplayDLL, hInstance);

		// Insert this DLL into the resource chain
		new CDynLinkLibrary(VisDisplayDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("VisDisplay.dll Terminating!\n");
	}
	return 1;   // ok
}


#if defined(VIS_DISPLAY_BUILD) && defined(VIS_DISPLAY_DLL)

HMODULE VISAPI VisHModuleGet(void)
{
	return VisDisplayDLL.hModule;
}

HMODULE VISAPI VisHResourceGet(void)
{
	return VisDisplayDLL.hResource;
}

#endif // defined(VIS_DISPLAY_BUILD) && defined(VIS_DISPLAY_DLL)
