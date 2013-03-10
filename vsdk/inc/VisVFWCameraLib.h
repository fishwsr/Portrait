// VisVFWCameraLib.h
//
// Include this header file in one of your project's source (not header)
// files to link with the VisVFWCamera project's static library (so that
// your program does not need to use the VisVFWCamera project's DLL).
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#pragma once

#ifndef VIS_SDK_LIBS
#define VIS_SDK_LIBS
#endif // VIS_SDK_LIBS


#include "VisCore.h"


#include "..\VisVFWCamera\VisVFWCamProj.h"


// UNDONE:  Remove the "b" suffix when we release the SDK.
#ifdef _DEBUG
#pragma comment(lib, VIS_VFWCAMERA_DEBUG_BASE_NAME)
#else // _DEBUG
#pragma comment(lib, VIS_VFWCAMERA_BASE_NAME)
#endif // _DEBUG



class CVisVFWInit
{
public:
    CVisVFWInit()
	{
		PGlobals()->AddImSrcProvider(g_szVFWProviderID, 
				VisGetImSrcProvider, g_szVFWProviderName);
	}
};

// This object will be defined in applications that include the
// VisVFWCameraLib header file.
CVisVFWInit gvis_vfwinit;
