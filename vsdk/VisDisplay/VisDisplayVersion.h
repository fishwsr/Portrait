// VisDisplayVersion.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_DISPLAY_VERSION_H__
#define __VIS_DISPLAY_VERSION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Product definitions
#include "..\VisSDKVersion.h"


// Project definitions
#define VIS_DISPLAY_DESCRIPTION		"MS Vision SDK display code"
//#define VIS_DISPLAY_BASE_NAME		"VisDisplay"

#ifndef VIS_SDK_LIBS
	#define VIS_DISPLAY_BASE_NAME		"VisDisplay"
#else // VIS_SDK_LIBS
	#define VIS_DISPLAY_BASE_NAME		"VisDisplayLib"
#endif // VIS_SDK_LIBS


// Generated definitions
#define VIS_DISPLAY_DEBUG_BASE_NAME	VIS_DISPLAY_BASE_NAME  VIS_SDK_PRODUCT_DEBUG_EXT


#endif // __VIS_DISPLAY_VERSION_H__
