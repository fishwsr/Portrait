// VisXCLAPACKVersion.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_XCLAPACK_VERSION_H__
#define __VIS_XCLAPACK_VERSION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Product definitions
#include "..\VisSDKVersion.h"


// Project definitions
#define VIS_XCLAPACK_DESCRIPTION		"MS Vision SDK interface to the CLAPACK library"
#define VIS_XCLAPACK_DLL				"VisXCLAPACK"

#ifdef VIS_SDK_LIBS
	#define VIS_XCLAPACK_BASE_NAME			"VisXCLAPACKLib"
#else // VIS_SDK_LIBS
	#define VIS_XCLAPACK_BASE_NAME			"VisXCLAPACK"
#endif // VIS_SDK_LIBS


// Generated definitions
#define VIS_XCLAPACK_DEBUG_BASE_NAME	VIS_XCLAPACK_BASE_NAME  VIS_SDK_PRODUCT_DEBUG_EXT


#ifdef _DEBUG
#define VIS_XCLAPACK_NAME				VIS_XCLAPACK_DLL  VIS_SDK_PRODUCT_DEBUG_EXT
#else // _DEBUG
#define VIS_XCLAPACK_NAME				VIS_XCLAPACK_DLL
#endif // _DEBUG


#endif // __VIS_XCLAPACK_VERSION_H__
