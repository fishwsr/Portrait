// VisImageProc.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_INC_IMAGEPROC_H__
#define __VIS_INC_IMAGEPROC_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "VisCore.h"

// UNODNE:  Is VisMAtrix also needed?
#include "VisMatrix.h"


#include "..\VisImageProc\VisImageProc.h"


#ifdef _DEBUG
#pragma comment(lib, "VisImageProcDB")
#else // _DEBUG
#pragma comment(lib, "VisImageProc")
#endif // _DEBUG


#endif // __VIS_INC_IMAGEPROC_H__
