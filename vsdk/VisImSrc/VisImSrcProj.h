////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMSRC
//
// @module VisImSrcProj.h |
//
// This file is the VisImSrc project include file for applications that
// use the VisImSrc DLL.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisConvertCbitToCbit\.h> <l VisConvertRGB\.h>
//		<l VisConvertYUV\.h> <l VisImageFromImSrcFrame\.h>
//		<l VisSequence\.h> <l VisSequenceList\.h> <l VisImageSource\.h>
//		<l VisImEncoding\.h> <l VisImSrcNoMFC\.h> <l VisImSrcConst\.h>
//		<l VisImSrcIFace\.h> <l VisListSz\.h> <l VisOLEHelp\.h>
//		<l VisRegHelp\.h> 
//
// <nl>
// @head3 Functions |
// @index func | VISIMSRC
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISIMSRC_H__
#define __VIS_IMSRC_VISIMSRC_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef VIS_IMSRC_DLL
	#define VIS_IMSRC_BUILD
#endif // VIS_IMSRC_DLL

#ifdef VIS_SDK_LIBS
#define VIS_IMSRC_LIB
#else // VIS_SDK_LIBS
	#ifndef VIS_IMSRC_DLL
		#define VIS_IMSRC_DLL
	#endif // !VIS_IMSRC_DLL
#endif // VIS_SDK_LIBS


#include "VisImSrcVersion.h"

#include "VisCore.h"

// Need this to get the VFW DLL name.
#include "..\VisVFWCamera\VisVFWCameraVersion.h"

#include <ole2.h>

#ifdef VIS_IMSRC_LIB
#define VisImSrcExport			
#define VisImSrcExportDeclspec	;
#elif defined(VIS_IMSRC_BUILD)
#define VisImSrcExport			_declspec(dllexport)
#define VisImSrcExportDeclspec	_declspec(dllexport)
#else // VIS_IMSRC_LIB
#define VisImSrcExport			_declspec(dllimport)
#define VisImSrcExportDeclspec	_declspec(dllimport)
#endif // VIS_IMSRC_LIB



extern HINSTANCE imsrcCurrentInstanceHandle;

#include "resource.h"

#include "VisImSrcNoMFC.h"

#include "VisSequenceList.h"
#include "VisImageSource.h"

#include "VisImageFromImSrcFrame.h"


// @func
// This function puts up a dialog box to let the user choose a string.  If the
// user chooses a string, the index of the string is returned.  If the user
// cancels the dialog (or there are no strings to choose), - 1 is returned.
//
// Since the <c CVisListSz> class is derived from the <i IVisListSz> interface,
// a pointer to a <c CVisListSz> object can be used to pass the list of strings
// to this function.
VisImSrcExport int VISAPI VisDoImSrcDlgHwnd(LPCSTR szTitle, IVisListSz *pIVisListSz,
		HWND hWndParent = 0, int iSelectionInitial = - 1);

#ifdef __AFX_H__
inline int VisDoImSrcDlg(LPCSTR szTitle, IVisListSz *pIVisListSz,
		class CWnd* pWndParent = 0, int iSelectionInitial = - 1)
{
	return VisDoImSrcDlgHwnd(szTitle, pIVisListSz,
			pWndParent ? pWndParent->m_hWnd : 0, iSelectionInitial);
}
#endif // __AFX_H__


// @func
// Given a string specifying an image source device, attempt to load the DLL
// used with the device, create the interfaces used by the <c CVisImageSource>
// object to communicate with the image source device, and give the interfaces
// to the <c CVisImageSource> object.  After calling this function, be sure
// to check that the <c CVisImageSource> is valid before attempting to use it.
//
// An image source device is specified by an image source provider (which uses
// a DLL specified in the registry) and a device string that the provider
// uses to create the interfaces used by the <c CVisImageSource> object.  For
// example, "VisVFW" is the provider for cameras that support the Microsoft
// Video For Windows standard, and "ISVR III Capture Driver Beta" is the name
// of a VFW device driver that supports the ISVR III video capture card.  For
// another example, "VisMeteor" could specify the provider for the Matrox
// Meteor video capture card and "Meteor Card 0, NTSC RGB Input, Default Channel"
// could specify a card, channel, and video format to use.
//
// Strings have the format "\<Provider\>:\<Device\>" or "\<Provider\>".  An empty
// \<Provider\> (or \<Device\>) string specified the default provider (or device).
// A single-digit (or double-digit in some cases) \<Provider\> (or \<Device\>) string
// specifies an indexed provider (or device), using the list that would be
// displayed to the user if we were using dialogs to select the provider and
// device.
//
// If \<Provider\> is a string that is not empty and that does not consist of
// a single digit or a pair of digits, it specifies a subkey of
// "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\VisSDK\\97Beta1\\ImSrc\\Devs" (or
// "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\VisSDK\\Debug\\97Beta1\\ImSrc\\Devs"
// in _DEBUG builds) that contains information needed to create an
// <i IVisImSrcProvider> interface.
//
// If \<Device\> is a string that is not empty and that does not consist of
// a single digit or a pair of digits, it
// is passed to the provider's <i IVisImSrcProvider> interface to create
// the <i IVisImSrcDevice> interface used internally by the <c CVisImageSource>
// object.
VisImSrcExport CVisImageSource VISAPI VisFindImageSource(const char *szImageSourceID);

inline bool VisFindImageSource(CVisSequenceBase& refsequence,
		const char *szImageSourceID)
{
	// LATER:  Should this set the sequence length to zero in some cases?
	return refsequence.ConnectToSource(VisFindImageSource(szImageSourceID));
}

// @func
// Display dialogs to let the user choose the image source device to
// use in an application.  After calling this function, be sure
// to check that the <c CVisImageSource> is valid before attempting to use it.
VisImSrcExport CVisImageSource VISAPI VisFindImageSourceHwnd(HWND hWndParent = 0,
		bool fAlwaysAsk = false);

#ifdef __AFX_H__
inline CVisImageSource VisFindImageSource(class CWnd* pWndParent = 0,
		bool fAlwaysAsk = false)
{
	return VisFindImageSourceHwnd(pWndParent ? pWndParent->m_hWnd : 0, fAlwaysAsk);
}
#endif // __AFX_H__

inline bool VisFindImageSourceHwnd(CVisSequenceBase& refsequence,
		HWND hWndParent = 0, bool fAlwaysAsk = false)
{
	// LATER:  Should this set the sequence length to zero in some cases?
	return refsequence.ConnectToSource(VisFindImageSourceHwnd(hWndParent,
			fAlwaysAsk));
}

#ifdef __AFX_H__
inline bool VisFindImageSource(CVisSequenceBase& refsequence,
		class CWnd* pWndParent = 0, bool fAlwaysAsk = false)
{
	return refsequence.ConnectToSource(VisFindImageSource(pWndParent,
			fAlwaysAsk));
}
#endif // __AFX_H__


// @func
// Add a registry entry for the specified image source device DLL.  Applications
// that expect to work with a specific image source ddevice might want to use this
// function, but most users of the SDK will not need to use this function.
VisImSrcExport void VISAPI VisAddProviderRegEntry(const char *szID, const char *szName,
		const char *szDLL, bool fAvailable = true);


// @func
// This is a helper function to get a list of subkeys (or subkey values)
// of a specified registry key.
VisImSrcExport IVisListSz * VISAPI VisGetSubkeyList(HKEY hkeyRoot, const char *szKey,
		int eviskeylist = eviskeylistID);


// @func
// Add the registry entries used by the image source device that works with
// cameras that support the Microsoft Video For Windows (VFW) standard.
// (This won't add camera-specific registry entries.  It's better to use the
// VFWCamera.reg file to set up the VFW device DLL.)
// UNDONE:  Add a flag to only add the entry if it does not already exist.
// (By default, we should not add the entry if it already exists.)
inline void VisAddProviderRegEntryForVFW(void)
{
#ifdef _DEBUG
	VisAddProviderRegEntry("VisVFW", "Video for Windows (Debug)",
			VIS_VFWCAMERA_DEBUG_DLL_NAME ".dll");
#else // _DEBUG
	VisAddProviderRegEntry("VisVFW", "Video for Windows", VIS_VFWCAMERA_DLL_NAME ".dll");
#endif // _DEBUG
}



VisImSrcExport HRESULT VISAPI VisGetProviderList(IVisListSz **ppIVisListSz, int eviskeylist = eviskeylistAvailIDAndDefVal);

VisImSrcExport void VISAPI GetGlobalProviders(IVisListSz **ppIVisListSz);

VisImSrcExport IVisImSrcProvider * VISAPI VisPIVisImSrcProviderGetI(int iProvider);

VisImSrcExport IVisImSrcProvider * VISAPI VisPIVisImSrcProviderGetSz(const char *szProvider);

#include "VisSequenceList.inl"
#include "VisImageSource.inl"


#include "VisImagePump.h"
#include "VisDlgListSz.h"

#include "VisImageHandler.h"


#endif // __VIS_IMSRC_VISIMSRC_H__
