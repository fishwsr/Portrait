// VisPropListVersion.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#ifndef __VIS_PROPLIST_VERSION_H__
#define __VIS_PROPLIST_VERSION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Product definitions
#include "..\VisSDKVersion.h"


// Project definitions
#define VIS_PROPLIST_DESCRIPTION		"MS Vision SDK proplist code"


#ifdef VIS_NO_MFC
	#define VIS_CORE_BASE_NAME		"VisPropListDLL"
#elif defined(VIS_SDK_LIBS)
	#define VIS_CORE_BASE_NAME		"VisPropListLib"
#else // VIS_NO_MFC
	#define VIS_CORE_BASE_NAME		"VisPropList"
#endif // VIS_NO_MFC


// Generated definitions
#define VIS_PROPLIST_DEBUG_BASE_NAME	VIS_PROPLIST_BASE_NAME  VIS_SDK_PRODUCT_DEBUG_EXT


#endif // __VIS_PROPLIST_VERSION_H__
