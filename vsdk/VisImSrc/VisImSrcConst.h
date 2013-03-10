////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMSRCCONST
//
// @module VisImSrcConst.h |
//
// This file defines constants used to communicate through standard
// interfaces with objects that provide images from digitizers.  (These
// objects are created in device-specific DLLs that are loaded explicitly as
// they are needed.)  Some <c CVisImageSource> methods use these constants
// to let the user adjust device settings.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisImSrcConst\.h>
// <nl>
//
// @head3 Constants |
// @index const enum | VISIMSRCCONST
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISIMSRCCONST_H__
#define __VIS_IMSRC_VISIMSRCCONST_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// @const const char * | VIS_SZ_REG_ROOT |
// This string is the root of the VisSDK registry key
// (under HKEY_LOCAL_MACHINE) for this version of the SDK.
#ifndef VIS_SZ_REG_ROOT
	#ifdef _DEBUG
		#define VIS_SZ_REG_ROOT		"Software\\Microsoft\\VisSDK\\Debug\\1.0"
	#else // _DEBUG
		#define VIS_SZ_REG_ROOT		"Software\\Microsoft\\VisSDK\\1.0"
	#endif // _DEBUG
#endif // !VIS_SZ_REG_ROOT


// @const const char * | VIS_SZ_REG_IMSRC |
// This string is the root of the registry key used by the VisImSrc code
// (under HKEY_LOCAL_MACHINE and for this version of the SDK).
#ifndef VIS_SZ_REG_IMSRC
#define VIS_SZ_REG_IMSRC	VIS_SZ_REG_ROOT  "\\ImSrc"
#endif // !VIS_SZ_REG_IMSRC


// This string is not being used in the beta but may be used later.
// (Change this comment if we decide to use this string.)
#ifndef VIS_SZ_REG_IMSRC_APPS
#define VIS_SZ_REG_IMSRC_APPS	VIS_SZ_REG_IMSRC  "\\Apps"
#endif // !VIS_SZ_REG_IMSRC_APPS


// @const const char * | VIS_SZ_REG_IMSRC_DEVS |
// This string is the root of the registry key used by the VisImSrc code
// for device-specific information
// (under HKEY_LOCAL_MACHINE and for this version of the SDK).
//
// Subkeys of this key are used to describe providers of image source
// devices.  The name of each subkey of this key is used to identify
// a provider.  The default value of each subkey is a name that can
// be shown to the user to describe this provider.
//
// The "Available" value
// under each subkey is used to indicate whether any devices supported
// be the provider are located on the local machine.
//
// The "DLL" value
// under each subkey gives the name of a DLL that the image source code
// can load when it wants to get an <i IVisImSrcProvider> interface
// for the provider.  The VisImSrc code will call <f LoadLibrary> to
// load the DLL and then look for an exported <f T_PfnVisGetImSrcProvider>
// function with the name "VisGetImSrcProvider" (actually
// "_VisGetImSrcProvider@16").  If this function is exported, it
// wil be called to get the <i IVisImSrcProvider> interface for the
// provider.
#ifndef VIS_SZ_REG_IMSRC_DEVS
#define VIS_SZ_REG_IMSRC_DEVS	VIS_SZ_REG_IMSRC  "\\Devs"
#endif // !VIS_SZ_REG_IMSRC_DEVS


// This string is not being used in the beta but may be used later.
// (Change this comment if we decide to use this string.)
#ifndef VIS_SZ_REG_IMSRC_DEFAULTS
#define VIS_SZ_REG_IMSRC_DEFAULTS	VIS_SZ_REG_IMSRC  "\\Defaults"
#endif // !VIS_SZ_REG_IMSRC_DEFAULTS


// This string is used internally by the VisImSrc code, but most users
// of the SDK will not need to use this string.
#define VIS_SZ_FN_GET_IM_SRC_PROVIDER		"_VisGetImSrcProvider@16"



// @enum EVisImSrc |
// Image Source properties.
enum EVisImSrc
{
    //------------------------------------------------------------------
	// @group Image encoding

	// @emem
	// 
	evisimsrcImEncoding			= 0,

	evisimsrcFirst				= 0,

    //------------------------------------------------------------------
	// @group Image rectangle

	// @emem
	// 
	evisimsrcWidth,

	// @emem
	// 
	evisimsrcHeight,

	// @emem
	// 
	evisimsrcXOffset,

	// @emem
	// 
	evisimsrcYOffset,

    //------------------------------------------------------------------
	// @group From DirectX's IAMVideoProcAmp

	// @emem
	// 
	evisimsrcBrightness,

	// @emem
	// 
	evisimsrcContrast,

	// @emem
	// 
	evisimsrcHue,

	// @emem
	// 
	evisimsrcSaturation,

	// @emem
	// 
	evisimsrcSharpness,

	// @emem
	// 
	evisimsrcGamma,

	// @emem
	// 
	evisimsrcColorEnable,

	// @emem
	// 
	evisimsrcWhiteBalance,

	// @emem
	// 
	evisimsrcBacklightCompensation,

    //------------------------------------------------------------------
	// @group From DirectX's IAMCameraControl

	// @emem
	// 
	evisimsrcPan,

	// @emem
	// 
	evisimsrcTilt,

	// @emem
	// 
	evisimsrcRoll,

	// @emem
	// 
	evisimsrcZoom,

	// @emem
	// 
	evisimsrcExposure,

	// @emem
	// 
	evisimsrcIris,

	// @emem
	// 
	evisimsrcFocus,

    //------------------------------------------------------------------
	// @group Reflection and rotation

	// @emem
	// 
	evisimsrcFReflectHoriz,

	// @emem
	// 
	evisimsrcFReflectVert,

	// @emem
	// 
	evisimsrcCCWRotation,

    //------------------------------------------------------------------
	// @group Properties used by Image Sequences

	// @emem
	// 
	evisimsrcNBuffers,

	// @emem
	// 
	evisimsrcFContinuousGrab,

	// @emem
	// 
	evisimsrcFCanAcceptNonvolatileBuffers,

	evisimsrcLim,

    //------------------------------------------------------------------
	// @group Device specific information

	// @emem
	// 
	evisimsrcDevSpecificFirst			= 0x1000,

	// @emem
	// 
	evisimsrcDevSpecificLim				= 0x8000
};


// @enum EVisImSrcFlag
// Flags used with Image Source properties.
// (These correspond to the flags used by DirectX.)
enum EVisImSrcFlag
{
	// @group Manual or automatic settings
	// 

	// @emem
	// 
	evisimsrcflagManual				= 0,

	// @emem
	// 
	evisimsrcflagAuto				= 0x1,


	// @group Absolute or relative settings
	// 

	// @emem
	// 
	evisimsrcflagAbsolute			= 0,

	// @emem
	// 
	evisimsrcflagRelative			= 0x10
};

// @enum EVisVidDlg
// These correspond to the standard VFW dialogs.
enum EVisVidDlg
{
	// @emem
	// 
	evisviddlgVFWCompression		= 0,

	evisviddlgFirst,

	// @emem
	// 
	evisviddlgVFWDisplay,

	// @emem
	// 
	evisviddlgVFWFormat,

	// @emem
	// 
	evisviddlgVFWSource,	// Port, channel, and signal

	evisviddlgLim
};

// @enum EVisPrpPage
// These correspond to the WDM property pages.
enum EVisDSPropPage
{
	
	// @emem
	// Video Capture Filter
	evisdsproppageCapFil                       = 0, 

	evisdsproppageFirst                        = 0,

	// @emem
	// Video Capture Pin
	evisdsproppageCapPin,

	evisdsproppageLim
};


#endif // __VIS_IMSRC_VISIMSRCCONST_H__
