////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISVFWCAMDLL 
//
// @module VisVFWCamDLL.cpp |
//
// This file implements the exported function <f VisGetImSrcProvider>, which
// is used to create a <c CVisVFWProvider> object.  It also implements
// the DLL entry point, DllMain, and contains global variables used in the
// DLL.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisVFWCamDLL\.h>
//
////////////////////////////////////////////////////////////////////////////



// Warning that some STL type names are too long.  (Debug info is truncated.)
#pragma warning( disable : 4786 )

// Define INITGUID here to get GUIDs defined in the header files.
// (We'll also need to link with uuid.lib to get some standard
// GUIDs, like IID_IUnknown.)
#ifndef VIS_SDK_LIBS
	#define INITGUID
#endif //!VIS_SDK_LIBS

#include "VisVFWCamPch.h"
#include "VisVFWCamProj.h"


// Global variables.

#ifndef VIS_VFW_LIB
	// The handle of this DLL.
	HANDLE g_hModule = 0;
//#else
//	#include "VisCore.h"
#endif //!VIS_VFW_LIB

#ifndef VIS_VFW_LIB

BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ulReasonForCall, 
                      LPVOID lpReserved)
{
    switch(ulReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
			if (hModule != 0)
			{
				g_hModule = hModule;
				DisableThreadLibraryCalls((HINSTANCE) hModule);
			}
		break;

		case DLL_THREAD_ATTACH:
			if (g_hModule == 0)
				g_hModule = hModule;
		break;

		case DLL_THREAD_DETACH:
		break;

		case DLL_PROCESS_DETACH:
		break;
    }

    return TRUE;
}

#endif // VIS_VFW_LIB
