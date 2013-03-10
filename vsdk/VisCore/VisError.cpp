///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISERROR
//
// @module VisError.cpp |
//
// Implementation of the error objects member functions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisError\.h>
//
// <nl>
// @index | VISERROR
//
///////////////////////////////////////////////////////////////////////////


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"


#include <stdarg.h>
#include "VisError.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

// @func Look through the error code array and return an error message from
//        the message array that has the same index. If the code in not
//        found, return a message "Unknown error".
// @rdesc An error message corresponding to an error code.
VisCoreExport const char * VISAPI VisErrorCodeToMessage(
      long lErrorCode,					// @parm An error code.
	  const char *rgszErrorMessage[],	// @parm An array of error messages
	  long rglErrorCode[])				// @parm A dictionary of error codes.
{
	for ( int i = 0; rgszErrorMessage[i]; i++ )
	{
		if ( lErrorCode == rglErrorCode[i] )
			return rgszErrorMessage[i];
	}

    return "Unknown error";
}
