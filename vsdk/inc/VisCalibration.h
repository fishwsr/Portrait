// VisCalibration.h
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_INC_CALIBRATION_H__
#define __VIS_INC_CALIBRATION_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include "..\VisCalibration\VisCalibrationProj.h"


// UNDONE:  Remove the "b" suffix when we release the SDK.
#ifdef _DEBUG
#pragma comment(lib, VIS_CALIBRATION_DEBUG_BASE_NAME)
#else // _DEBUG
#pragma comment(lib, VIS_CALIBRATION_BASE_NAME)
#endif // _DEBUG


#endif // __VIS_INC_CALIBRATION_H__
