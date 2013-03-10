// VisImageProcVersion.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_IMAGEPROC_VERSION_H__
#define __VIS_IMAGEPROC_VERSION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Product definitions
#include "..\VisSDKVersion.h"


// Project definitions
#define VIS_IMAGEPROC_DESCRIPTION		"MS Vision SDK Image Proc code"


#ifndef VIS_SDK_LIBS
	#define VIS_IMAGEPROC_BASE_NAME		"VisImageProc"
#else
	#define VIS_IMAGEPROC_BASE_NAME		"VisImageProcLib"
#endif // VIS_SDK_LIBS


// Generated definitions
#define VIS_IMAGEPROC_DEBUG_BASE_NAME	VIS_IMAGEPROC_BASE_NAME  VIS_SDK_PRODUCT_DEBUG_EXT


#endif // __VIS_IMAGEPROC_VERSION_H__
