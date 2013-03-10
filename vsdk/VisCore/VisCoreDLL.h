///////////////////////////////////////////////////////////////////////////
// @doc INTERNAL EXTERNAL VISCORE
// @module VisCoreDLL.h |
//
// This header includes the files which define the core of MSVisSDK
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisEnumPixel\.h> <l VisError\.h> <l FisFileIO\.h>
//		<l VisImage\.h> <l VisImageOf\.h> <l VisImageTypes\.h>
//		<l VisMemBlock\.h> <l VisPixel\.h> <l VisRGBAPixels\.h> 
//		<l VisShape\.h> <l VisYUVAPixel\.h>
//
// <nl>
// @index | VISCORE
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_CORE_H__
#define __VIS_CORE_CORE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Warning that some STL type names are too long.  (Debug info is truncated.)
#pragma warning( disable : 4786 )

#define VISAPI __stdcall


// The old VisCore project defines VIS_CORE_DLL instead of VIS_CORE_BUILD
#ifdef VIS_CORE_DLL
	#define VIS_CORE_BUILD
#endif // VIS_CORE_DLL


#ifdef VIS_SDK_LIBS
	#define VIS_CORE_LIB
#else // VIS_SDK_LIBS
	// Warning about using types without a DLL interface in exported classes.
	#pragma warning(disable : 4251)

	#ifndef VIS_CORE_DLL
		#define VIS_CORE_DLL
	#endif // !VIS_CORE_DLL
#endif // VIS_SDK_LIBS

#ifdef VIS_CORE_LIB
#define VisCoreExport			
#define VisCoreExportDeclspec	;
#elif defined(VIS_CORE_BUILD)
#define VisCoreExport			_declspec(dllexport)
#define VisCoreExportDeclspec	_declspec(dllexport)
#else // VIS_CORE_LIB
#define VisCoreExport			_declspec(dllimport)
#define VisCoreExportDeclspec	_declspec(dllimport)
#endif // VIS_CORE_LIB

#define VisPropListExportDeclspec		VisCoreExportDeclspec
#define VisPropListExport				VisCoreExport

#include "VisCoreVersion.h"


// This macro is used to identify unreferenced parameters to functions (so
// that we don't get level 4 warnings).
#ifndef VisUnreferenced
#define VisUnreferenced(x)		(x)
#endif // !VisUnreferenced


// Allow lots of inlining
#pragma inline_depth(30)
#pragma inline_recursion(on)


#ifdef VIS_USE_WARN_4
// If we want to use Warning level 4 for this file, we need to avoid warnings
// in some standard header files.


	// We don't want warnings that assignment operators or copy constructors can not be generated.
	// LATER:  Probably only want to disable these around classes that give these warnings.
	#pragma warning(disable : 4511)  // copy constructors
	#pragma warning(disable : 4512)  // assignment operators


	// These are included in iosfwd.
	//#include <cstdio>
	//#include <cstring>
	//#include <cwchar>
	#include <xstddef>

	#pragma warning(disable : 4663)  // C++ language change: templates in iosfwd, xmemory, xlocale
	#pragma warning(disable : 4244)  // conversion from 'int' to 'char' in iosfwd and xlocale
	#include <iosfwd>
	#pragma warning(default : 4244)


	#pragma warning(disable : 4663)  // C++ language change: templates in iosfwd, xmemory, xlocale
	#pragma warning(disable : 4100)  // '_P' : unreferenced in xmemory, '_D' : unreferenced in xlocale
	#include <xmemory>


	// These are included in xlocale.
	//#include <climits>
	//#include <cstring>
	//#include <stdexcept>
	#include <typeinfo>
	//#include <xlocinfo>

	#pragma warning(disable : 4244)  // conversion from 'int' to 'char' in iosfwd and xlocale
	#pragma warning(disable : 4100)  // '_P' : unreferenced in xmemory, '_D' : unreferenced in xlocale
	#pragma warning(disable : 4018)  // signed/unsigned mismatch in xlocale
	#pragma warning(disable : 4663)  // C++ language change: templates in iosfwd, xmemory, xlocale
	//#pragma warning(disable : 4511)  // copy constructors in xlocale
	//#pragma warning(disable : 4512)  // assignment operators in xlocale
	#include <xlocale>
	//#pragma warning(default : 4512)
	//#pragma warning(default : 4511)
	#pragma warning(default : 4663)
	#pragma warning(default : 4018)  // We'll still get this warning below from the xlocale classes.
	#pragma warning(default : 4100)  // We'll still get this warning below from the xlocale classes.
	#pragma warning(default : 4244)  // We'll still get this warning below from the xlocale classes.


#endif // VIS_USE_WARN_4

#ifdef VIS_CORE_LIB
	#pragma comment(lib, "gdi32")
	#pragma comment(lib, "ddraw")
	#pragma comment(lib, "vfw32")
#endif // VIS_CORE_LIB

// Include files
#ifdef _DEBUG

#define _NORMAL_BLOCK    1
#include "crtdbg.h"


// Memory tracking allocation

void* __cdecl operator new(size_t nSize, LPCSTR lpszFileName, int nLine);

inline void* __cdecl operator new(size_t nSize, LPCSTR lpszFileName, 
        int nLine)
{
	return _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}

inline void* __cdecl operator new(size_t nSize)
{
    return _malloc_dbg(nSize, _NORMAL_BLOCK, __FILE__, __LINE__);
}

inline void __cdecl operator delete(void* p, LPCSTR lpszFileName, int nLine)
{
	_free_dbg(p, _NORMAL_BLOCK);
}

inline void __cdecl operator delete(void* p)
{
	_free_dbg(p, _NORMAL_BLOCK);
}

#undef DEBUG_NEW
#define DEBUG_NEW new(THIS_FILE, __LINE__)

#endif



#include <mmsystem.h>


//#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <memory.h>
#include <math.h>
#include <ddrawex.h>

#ifdef VIS_USE_DXMEDIA
	#include <dxtrans.h>
	#include <dxbounds.h>
	#include <dxhelper.h>
#endif // VIS_USE_DXMEDIA

//#include <atlbase.h>
//#include <atlwin.h>

#include "VisPropList.h"

#include "VisRegHelp.h"

#include "VisTimer2.h"

#include "VisGlobals.h"
#include "VisDiagnosticsManager.h"

#include "VisDiagnosticsMsg.h"
#include "VisCritSect.h"
#include "VisSDStream.h" // Template class
#include "VisNormalize.h"
#include "VisRGBAPixels.h" // Template class
#include "VisGrayAPixels.h"	// Template class
#include "VisYUVAPixels.h" // Template class
#include "VisPixelFnOps.h"
#include "VisImageBase.h"
#include "VisImage.h" // Template class
#include "VisImageTypes.h"
#include "VisFileIO.h"
#include "VisPictureFileHandler.h"
#include "VisFileStreamIO.h"
#include "VisAudioSamples.h"
#include "VisAudioOperation.h"
#include "VisError.h"
#include "VisMap.h" // Template class
#include "VisSequence.h"
#include "VisDib.h"

#include "VisConsoleWnd.h"
#include "VisRegHelp.h"
#include "VisPump.h"
#include "VisImageWnd.h"
#include "VisImageInfo.h"
#include "VisDiagnostics.h"

#include "VisDDrawConversion.h"

#include "VisXIpl.h"


#endif // __VIS_CORE_CORE_H__
