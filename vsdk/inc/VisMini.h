// VisMini.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_INC_MINI_H__
#define __VIS_INC_MINI_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "VisCore.h"


#include "..\VisMini\VisMiniProj.h"


// UNDONE:  Remove the "b" suffix when we release the SDK.
#ifdef _DEBUG
#pragma comment(lib, VIS_MINI_DEBUG_BASE_NAME)
#else // _DEBUG
#pragma comment(lib, VIS_MINI_BASE_NAME)
#endif // _DEBUG


#endif // __VIS_INC_MINI_H__
