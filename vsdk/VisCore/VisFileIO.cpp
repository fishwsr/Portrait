///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISFILEIO
//
// @module VisFileIO.cpp |
//
// This file contains the implementation for the CVisImage Class
// file input/output methods.  After finding the appropriate handler
// based on extension or file type (in the fd structure), the file
// is optionally opened for reading or writing, and then the handler's
// read/write methods are invoked.  The reading routine allocates
// the necessary storage if none exists.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisFileIO\.h>
//
// <nl>
// @index | VISFILEIO
//
///////////////////////////////////////////////////////////////////////////


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"


#include <ctype.h>
#include <stdio.h>
#include <string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

//
//  Initialize initial file handler list to empty
//

VisCoreExport int SVisFileDescriptor::default_jpeg_quality = 0;


// @mfunc Sets the function name for the run-time exception handling.
void CVisFileHandler::SetClientName(
                                    const char *szClientName)	// @parm Function name.
{ 
    delete [] m_szClientName;
    m_szClientName = new char[strlen(szClientName)+1];
    strcpy(m_szClientName, szClientName);
}

// @mfunc Case insensitive string comparison.
int CVisFileHandler::MatchIgnoringCase(
                                       const char *sz1, // @parm first string to compare
									   const char *sz2) // @parm second string to compare
{
	// LATER:  We probably don't need to have our own function to do this.
    for (; *sz1 && *sz2 && (*sz1 == *sz2 ||
        isalpha(*sz1) && isalpha(*sz2) &&
        *sz1 - ((*sz1 > 'Z') ? 'a' - 'A' : 0) ==
        *sz2 - ((*sz2 > 'Z') ? 'a' - 'A' : 0)); sz1++, sz2++);
    return (*sz1 == 0) && (*sz2 == 0);
}

// @mfunc Find the handler associated with a given extension / type.
// @rdesc Returns a handler for image file i/o.
CVisFileHandler *CVisFileHandler::Handler(
                                          const char *szFiletype)	// @parm Image file type (tga, bmp, etc.).
{
	return (PGlobals()->FileHandler(szFiletype));
}

// Add a new handler.
void CVisFileHandler::AddHandler(CVisFileHandler *pfilehandler)
{
	PGlobals()->AddExternalFileHandler(pfilehandler);
}

//
//  List of instantiated handlers goes here (otherwise won't link library)
//

class CVisFileHandlerInit
{
public:
    CVisFileHandlerInit();
};

// @mfunc Initializes the file handler.
CVisFileHandlerInit::CVisFileHandlerInit()
{
	// The most recently added file handlers will be searched first.

    // Skeletal color PPM file handler, used for debugging only
    extern CVisFileHandler *CVisPPMFileHandler_Init();
    CVisFileHandler::AddHandler(CVisPPMFileHandler_Init());

    // ImageMagick, if available
    extern CVisFileHandler *Magick_FileHandler_Init();
    CVisFileHandler::AddHandler(Magick_FileHandler_Init());

	// Ross Cutler's DIB file I/O handler.
    extern CVisFileHandler *CVisDibFileHandler_Init();
    CVisFileHandler::AddHandler(CVisDibFileHandler_Init());

    // Our postscript file handler
    extern CVisFileHandler *CVisPSFileHandler_Init();
    CVisFileHandler::AddHandler(CVisPSFileHandler_Init());

    // Our MSV file handler
    extern CVisFileHandler *CVisMSVFileHandler_Init();
    CVisFileHandler::AddHandler(CVisMSVFileHandler_Init());

	// (Search Generic first, then our basic PS handler, then ImageMagick,
	// then our sample PPM file handler.)
#ifdef VIS_USE_GENERIC_FILE_HANDLER
    // Generic file handler, either ImageMagick or IFL
    extern CVisFileHandler *Generic_FileHandler_Init();
    CVisFileHandler::AddHandler(Generic_FileHandler_Init());
#endif // VIS_USE_GENERIC_FILE_HANDLER
}

//static CVisFileHandlerInit dummy;  // do the initialization
