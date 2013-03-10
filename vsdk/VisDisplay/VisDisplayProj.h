////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISDISPLAY
//
// @module VisDisplayProj.h |
//
// Includes all .h files required for use of the ImageDisplay subproject.
//
// <nl>
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisCreatePanes\.h> <l VisDisplayGlobals\.h>
//		<l VisDisplayMessages\.h> <l VisEventHandlers\.h>
//		<l VisMessageWnd\.h> <l VisPane\.h> <l VisWnd\.h>
//		<l VisWndThread\.h>
//
// <nl>
// @index | VISDISPLAY
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_DISPLAY_H__
#define __VIS_DISPLAY_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef VIS_DISPLAY_DLL
	#define VIS_DISPLAY_BUILD
#endif // VIS_DISPLAY_DLL

#include "VisDisplayVersion.h"

#include "..\inc\VisCore.h"


#ifdef VIS_SDK_LIBS
	#define VIS_DISPLAY_LIB
#else // VIS_SDK_LIBS
	#define VIS_DISPLAY_DLL
#endif // VIS_SDK_LIBS



#ifdef VIS_DISPLAY_LIB
#define VisDisplayExport			
#define VisDisplayExportDeclspec	;
#elif defined(VIS_DISPLAY_BUILD)
#define VisDisplayExport			_declspec(dllexport)
#define VisDisplayExportDeclspec	_declspec(dllexport)
#else // VIS_DISPLAY_LIB
#define VisDisplayExport			_declspec(dllimport)
#define VisDisplayExportDeclspec	_declspec(dllimport)
#endif // VIS_DISPLAY_LIB


#include "resource.h"
#include "VisPane.h"
#include "VisPaneArray.h"
#include "VisDisplayImage.h"


// These two functions are used (internally) to access information about
// the VisDisplay DLL.
// LATER:  Is there a better way to get this information from MFC?
#if defined(VIS_DISPLAY_BUILD) && defined(VIS_DISPLAY_DLL)
HMODULE VISAPI VisHModuleGet(void);
HMODULE VISAPI VisHResourceGet(void);
#endif // defined(VIS_DISPLAY_BUILD) && defined(VIS_DISPLAY_DLL)


#endif //__VIS_DISPLAY_H__
