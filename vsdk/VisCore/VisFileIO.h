///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISFILEIO
//
// @module VisFileIO.h |
//
// This file defines error objects used for the exception handling 
// in MSVisSDK. The basic object, CVisError, is a base class that provides
// some information about the exception (file name, function name, etc.)
// CVisFileIOError and CVisMemoryError are derived from CVisError class.
// They contain more specific information about the nature of the exception.
//
// <nl>
//
// @devnote:(INTERNAL)
//  
// One file handler is created per i/o package inside SDK. 
// It is not to be created by the user.
//
// Some implementations may not partition the file input/output
// in the below manner, but they must still support these methods.
// In particular, ReadHeader must fill in the fd structure so that
// data allocation can occur.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisFileIO\.cpp>
//
// <nl>
// @index | VISFILEIO
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_FILEIO_H__
#define __VIS_CORE_FILEIO_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <stdio.h>
//#include "VisCore.h" // don't need this -- VisCore.h includes this file
//#include "..\VisXImageMagick\VisXImageMagick.h"

////////////////////////////////////////////////////////////////////////////
//  
// @struct
//
// This structure contains information about a file and its contents.
//  
////////////////////////////////////////////////////////////////////////////
struct SVisFileDescriptor 
{
    const char *filename;   // @field file name
    const char *filetype;   // @field file type (same as allowed extensions)
    FILE *stream;           // @field optionally opened stream

    int rows, cols, bands;  // @field image shape
    int evispixfmt;   // @field pixel type
    int bits_per_pixel;     // @field bits per pixel (1, 4, 8, 24, 32, ...)

    int ascii;              // @field data is ASCII encoded (not packed)
    int n_colors;           // @field number of colomap entries
    int has_alpha_channel;  // @field contains an alpha channel
    int frame_number;       // @field for image sequence (.AVI)
    int print_width;        // @field for printer (PostScript)
    int print_height;       // @field for printer (PostScript)

    void *internal1;        // @field temporary data internal to file type
    void *internal2;        // @field temporary data internal to file type

    int jpeg_quality;       // @field JPEG quality (1..100)
    // etc. ...
	int left;
	int top;

    static VisCoreExport int default_jpeg_quality;
};


// @const | VIS_MAX_FILE_HANDLERS | Maximum number of file handlers.
enum { VIS_MAX_FILE_HANDLERS = 1024 };


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// The base file handler class.  All file handlers inherit from this class.
// The pure virtual functions in this class must be defined in the
// derived classes.
//  
// @xref <c CVisPPMFileHandler> <c CVisPSFileHandler>
// <c CVisFileHandlerMagick>
//
////////////////////////////////////////////////////////////////////////////
class VisCoreExport CVisFileHandler 
{
// @access Public members
public:
	//------------------------------------------------------------------
	// @group Constructor and destructor

	// @cmember
	// Constructor.
	CVisFileHandler() : m_szClientName(0) {}

	// @cmember
	// Destructor.
	virtual ~CVisFileHandler() { delete [] m_szClientName; };

	// @cmember
	// Add a new handler.
    static void AddHandler(CVisFileHandler *pfilehandler);


	//------------------------------------------------------------------
	// @group Client name

	// @cmember
	// Get the client name.
	const char *GetClientName() { return m_szClientName; }

	// @cmember
	// Set the client name.
	void SetClientName(const char *szClientName);


	//------------------------------------------------------------------
	// @group File handler interface (virtual functions)

	// @cmember
	// Attempt to open the file.  Return TRUE is successful.  The
	// default implementation just returns TRUE.
	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL ExternallyOpenStream() { return TRUE; }

	// @cmember
	// Does the handler support images of the specified pixel type with
	// the specifed number of bands?
    virtual BOOL SupportsPixelType(int evispixfmt, int nbands) = 0;

	// @cmember
	// Does the specified extension match the extension of a file type
	// supported by this file handler?
    virtual BOOL MatchExtension(const char *szFiletype) = 0;

	// @cmember
 	// Attempt to read the file header.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL ReadHeader(SVisFileDescriptor &fd, class CVisImageBase &img) = 0;

	// @cmember
  	// Attempt to read the file body.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL ReadBody(SVisFileDescriptor &fd, class CVisImageBase &img) = 0;

	// @cmember
  	// Attempt to write the file header.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL WriteHeader(SVisFileDescriptor &fd, class CVisImageBase &img) = 0;

	// @cmember
   	// Attempt to write the file body.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL WriteBody(SVisFileDescriptor &fd, class CVisImageBase &img) = 0;


	//------------------------------------------------------------------
	// @group Static file handler access function

	// @cmember
	// Static method to find the file handler to use with a specified file
	// name.
    static CVisFileHandler *Handler(const char *szFiletype);

	
// @access:(IMPL) Protected members
protected:
	//------------------------------------------------------------------
	// @group:(IMPL) Protected helper function

	// @cmember:(IMPL)
	// Static helper function.
    static BOOL MatchIgnoringCase(const char *sz1, const char *sz2);     


// @access:(IMPL) Private members
private:
	//------------------------------------------------------------------
	// @group:(IMPL) Private data members

	// @cmember:(IMPL)
	// Client name.
	char *m_szClientName;
};


#endif // __VIS_CORE_FILEIO_H__
