// VisCoreVersion.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_CORE_VERSION_H__
#define __VIS_CORE_VERSION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Product definitions
#include "..\VisSDKVersion.h"


// Project definitions
#define VIS_CORE_DESCRIPTION		"MS Vision SDK core code"


#ifndef VIS_SDK_LIBS
	#define VIS_CORE_BASE_NAME		"VisCore"
#else
	#define VIS_CORE_BASE_NAME		"VisCoreLib"
#endif // VIS_SDK_LIBS


// Generated definitions
#define VIS_CORE_DEBUG_BASE_NAME	VIS_CORE_BASE_NAME  VIS_SDK_PRODUCT_DEBUG_EXT


#endif // __VIS_CORE_VERSION_H__
