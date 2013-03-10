////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISVFWCAMDLL 
//
// @module VisVFWCamProj.cpp |
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
//#ifndef VIS_SDK_LIBS
//	#define INITGUID
//#endif //!VIS_SDK_LIBS

#include "VisVFWCamPch.h"
#include "VisVFWCamProj.h"


// Global variables.
/*
#ifndef VIS_VFW_LIB
	// The handle of this DLL.
	HANDLE g_hModule = 0;
#else
	#include "VisCore.h"
#endif //!VIS_VFW_LIB
*/
// The prefix for cameras handled by this DLL.
const char *g_szVFWProviderID = "VisVFW";

#ifdef _DEBUG
const char *g_szVFWProviderName = "Video for Windows (Debug)";
#else // _DEBUG
const char *g_szVFWProviderName = "Video for Windows";
#endif // _DEBUG



EXTERN_C
{
	VisVFWExport HRESULT STDAPICALLTYPE VisGetImSrcProvider
			(LPUNKNOWN pUnkOuter, const char *szProvider, REFIID riid,
			void **ppvObject)
	{
		HRESULT hrRet;

		if (ppvObject == 0)
		{
			hrRet = ResultFromScode(E_POINTER);
		}
		else if (pUnkOuter != 0)
		{
			hrRet = ResultFromScode(CLASS_E_NOAGGREGATION);
			*ppvObject = 0;
		}
		else if (szProvider == 0)
		{
			hrRet = ResultFromScode(E_POINTER);
			*ppvObject = 0;
		}
		else if ((lstrcmpi(szProvider, "VisVFW") != 0)
				&& (lstrcmpi(szProvider, "Video For Windows") != 0))
		{
			hrRet = ResultFromScode(E_FAIL);
			*ppvObject = 0;
		}
		else if ((riid == IID_IUnknown)
				|| (riid == IID_IVisImSrcProvider))
		{
			*ppvObject = (IVisImSrcProvider *) new CVisVFWProvider;
			if (*ppvObject == 0)
				hrRet = ResultFromScode(E_OUTOFMEMORY);
			else
				hrRet = ResultFromScode(S_OK);
		}
		else
		{
			*ppvObject = 0;
			hrRet = ResultFromScode(E_NOINTERFACE);
		}

		return hrRet;
	}
};
