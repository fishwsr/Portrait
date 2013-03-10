// VisSFM.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_INC_SFM_H__
#define __VIS_INC_SFM_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "VisCore.h"

#include "VisMatrix.h"

#include "VisImageProc.h"


#include "..\VisSFM\VisSFM.h"


#ifdef _DEBUG
#pragma comment(lib, "VisSFMDB")
#else // _DEBUG
#pragma comment(lib, "VisSFM")
#endif // _DEBUG


#endif // __VIS_INC_SFM_H__
