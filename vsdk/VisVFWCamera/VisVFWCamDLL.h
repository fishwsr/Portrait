////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISVFWCAMDLL
//
// @module VisVFWCamDLL.h |
//
// This file defines the exported function <f VisGetImSrcProvider>, which
// is used to create a <c CVisVFWProvider> object.  It also defines
// the DLL entry point, DllMain, and two global string variables.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisVFWCamDLL\.cpp>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISVFWCAMDLL_H__
#define __VIS_IMSRC_VISVFWCAMDLL_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef VIS_VFW_DLL
	#define VIS_VFW_BUILD
#endif // VIS_VFW_DLL

#ifdef VIS_SDK_LIBS
#define VIS_VFW_LIB
#else // VIS_SDK_LIBS
	#ifndef VIS_VFW_DLL
		#define VIS_VFW_DLL
	#endif // !VIS_VFW_DLL
#endif // VIS_SDK_LIBS

#include "VisVFWCameraVersion.h"


#ifdef VIS_VFW_LIB
#define VisVFWExport			
#elif defined(VIS_VFW_BUILD)
#define VisVFWExport			_declspec(dllexport)
#else // VIS_VFW_LIB
#define VisVFWExport			_declspec(dllimport)
#endif // VIS_VFW_LIB

#include "VisVFWCamera.h"

// The prefix for cameras handled by this DLL.
extern const char *g_szVFWProviderID;

extern const char *g_szVFWProviderName;


EXTERN_C
{
	VisVFWExport HRESULT STDAPICALLTYPE VisGetImSrcProvider
			(LPUNKNOWN pUnkOuter, const char *szProvider, REFIID riid,
			void **ppvObject);
};

#ifndef VIS_VFW_LIB
extern HANDLE g_hModule;

// DLL entry point (called from standard library).  Used to find the
// handle for this DLL.
BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ulReasonForCall, 
                      LPVOID lpReserved);

#endif // !VIS_VFW_LIB


#endif // __VIS_IMSRC_VISVFWCAMDLL_H__
