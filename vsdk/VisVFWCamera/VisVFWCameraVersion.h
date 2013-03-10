// VisVFWCameraVersion.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_VFWCAMERA_VERSION_H__
#define __VIS_VFWCAMERA_VERSION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Product definitions
#include "..\VisSDKVersion.h"


// Project definitions
#define VIS_VFWCAMERA_DESCRIPTION		"MS Vision SDK interface to Video For Windows digitizers"
#define VIS_VFWCAMERA_DLL_NAME			"VisVFWCamera"

#ifndef VIS_SDK_LIBS
	#define VIS_VFWCAMERA_BASE_NAME		"VisVFWCamera"
#else // VIS_SDK_LIBS
	#define VIS_VFWCAMERA_BASE_NAME		"VisVFWCameraLib"
#endif // VIS_SDK_LIBS


// Generated definitions
#define VIS_VFWCAMERA_DEBUG_BASE_NAME	VIS_VFWCAMERA_BASE_NAME  VIS_SDK_PRODUCT_DEBUG_EXT
#define VIS_VFWCAMERA_DEBUG_DLL_NAME	VIS_VFWCAMERA_DLL_NAME   VIS_SDK_PRODUCT_DEBUG_EXT

#endif // __VIS_VFWCAMERA_VERSION_H__
