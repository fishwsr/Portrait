// VisMemoryChecks.h
//
// Include this header file in one of your project's source (not header)
// files to add exit code that will prevent erroneous memory leaks from
// being reported.  (There still may be a couple of erroneous memory
// leaks reported for memory allocated by the standard runtime library.)
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#pragma once


#include "VisCore.h"

#include <stdlib.h>


class CVisMemoryCheckInit
{
public:
    CVisMemoryCheckInit()
	{
		_onexit(CVisGlobals::OnExit);
	}
};

// This object will be defined in your application.  It will make sure that
// global Vision SDK variables are destroyed when exit routines for your
// application are called (before debug memory checks are made).
CVisMemoryCheckInit gvis_memorycheckinit;
