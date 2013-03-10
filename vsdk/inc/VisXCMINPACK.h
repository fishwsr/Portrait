// VisXCMINPACK.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved


#pragma once

#include "VisCore.h"


#include "..\VisXCMINPACK\VisXCMINPACK.h"


// UNDONE:  Remove the "b" suffix when we release the SDK.
#ifdef _DEBUG
#pragma comment(lib, VIS_XCMINPACK_DEBUG_BASE_NAME)
#else // _DEBUG
#pragma comment(lib, VIS_XCMINPACK_BASE_NAME)
#endif // _DEBUG

