// VisImSrcVersion.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_IMSRC_VERSION_H__
#define __VIS_IMSRC_VERSION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Product definitions
#include "..\VisSDKVersion.h"


// Project definitions
#define VIS_IMSRC_DESCRIPTION		"MS Vision SDK image source (camera interface) code"

#ifndef VIS_SDK_LIBS
	#define VIS_IMSRC_BASE_NAME		"VisImSrc"
#else // VIS_SDK_LIBS
	#define VIS_IMSRC_BASE_NAME		"VisImSrcLib"
#endif // VIS_SDK_LIBS

// Generated definitions
#define VIS_IMSRC_DEBUG_BASE_NAME	VIS_IMSRC_BASE_NAME  VIS_SDK_PRODUCT_DEBUG_EXT


#endif // __VIS_IMSRC_VERSION_H__
