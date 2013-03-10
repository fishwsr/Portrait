////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMSRCNOMFC
//
// @module VisImSrcNoMFC.h |
//
// This file is the VisImSrc project include file for applications that
// do not use the VisImSrc DLL.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisConvertCbitToCbit\.h> <l VisConvertRGB\.h>
//		<l VisConvertYUV\.h> <l VisImEncoding\.h> <l VisImSrc\.h>
//		<l VisImSrcConst\.h> <l VisImSrcIFace\.h>
//		<l VisListSz\.h> <l VisOLEHelp\.h> <l VisRegHelp\.h> 
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISIMSRCNOMFC_H__
#define __VIS_IMSRC_VISIMSRCNOMFC_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// This macro is used to identify unreferenced parameters to functions (so
// that we don't get level 4 warnings).
#ifndef VisUnreferenced
#define VisUnreferenced(x)		(x)
#endif // !VisUnreferenced


#ifndef VisImSrcExport
#define VisImSrcExport	 _declspec(dllimport)
#endif // VisImSrcExport


#include "VisImSrcConst.h"
#include "VisImEncoding.h"
#include "VisImSrcIFace.h"

#include "VisConvertCbitToCbit.h"
#include "VisConvertYUV.h"
#include "VisConvertRGB.h"

#include "..\VisCore\VisRegHelp.h"
#include "VisOLEHelp.h"

#include "VisListSz.h"

#include "VisImEncoding.inl"

#include "VisListSz.inl"


enum EVisKeyList
{
	eviskeylistID					= 1,
	eviskeylistDefaultValue			= 2,
	eviskeylistIDAndDefaultValue		= 3,
	eviskeylistOnlyAvailableSubkeys	= 4,
	eviskeylistAvailID				= 5,
	eviskeylistAvailDefaultValue		= 6,
	eviskeylistAvailIDAndDefVal		= 7
};

inline IVisListSz *VisInlineGetSubkeyList(HKEY hkeyRoot, const char *szKey,
		int eviskeylist = eviskeylistID);

inline bool VisInlineFIsSzKeyAvailable(const char *szKey,
		HKEY hkeyRoot = HKEY_LOCAL_MACHINE);

inline void VisInlineAddProviderRegEntry(const char *szID, const char *szName,
		const char *szDLL, bool fAvailable = true);


#include "VisImSrcNoMFC.inl"


#endif // __VIS_IMSRC_VISIMSRCNOMFC_H__
