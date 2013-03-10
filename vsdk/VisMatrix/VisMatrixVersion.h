// VisMatrixVersion.h
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_MATRIX_VERSION_H__
#define __VIS_MATRIX_VERSION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Product definitions
#include "..\VisSDKVersion.h"


// Project definitions
#define VIS_MATRIX_DESCRIPTION		"MS Vision SDK matrix code"


#ifndef VIS_SDK_LIBS
	#define VIS_MATRIX_BASE_NAME		"VisMatrix"
#else // VIS_SDK_LIBS
	#define VIS_MATRIX_BASE_NAME		"VisMatrixLib"
#endif // VIS_SDK_LIBS

// Generated definitions
#define VIS_MATRIX_DEBUG_BASE_NAME	VIS_MATRIX_BASE_NAME  VIS_SDK_PRODUCT_DEBUG_EXT


#endif // __VIS_MATRIX_VERSION_H__
