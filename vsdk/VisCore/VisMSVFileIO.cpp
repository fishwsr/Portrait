///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISMSVFILEIO
//
// @module VisMSVFileIO.cpp |
//
// Read and write MSV image files.  (MSV is the Vision SDK file format.)
// Defines and instantiates subclass of CVisFileHandler class.
//
// <nl>
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisFileIO\.h> <l VisFileIO\.cpp>
//
// <nl>
//
// <nl>
// @index | VISMSVFILEIO
//
///////////////////////////////////////////////////////////////////////////


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"


#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "VisFileIO.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG


// @class
// File handler to read and write PS (Postscript) image files.
// @base public | CVisFileHandler
// @xref <c CVisFileHandler>
class VisCoreExport CVisMSVFileHandler : public CVisFileHandler {
public:
	// @cmember
	// Does the handler support images of the specified pixel type with
	// the specifed number of bands?
    virtual BOOL SupportsPixelType(int evispixfmt, int nbands);

	// @cmember
	// Does the specified extension match the extension of a file type
	// supported by this file handler?
    virtual BOOL MatchExtension(const char *szFiletype);

	// @cmember
 	// Attempt to read the file header.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL ReadHeader(SVisFileDescriptor &fd, class CVisImageBase &img);

	// @cmember
  	// Attempt to read the file body.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL ReadBody(SVisFileDescriptor &fd, class CVisImageBase &img);

	// @cmember
  	// Attempt to write the file header.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL WriteHeader(SVisFileDescriptor &fd, class CVisImageBase &img);

	// @cmember
   	// Attempt to write the file body.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL WriteBody(SVisFileDescriptor &fd, class CVisImageBase &img);
};


// @func Returns a handler for PS image file i/o.
// @rdesc File handler for i/o.
VisCoreExport CVisFileHandler *CVisMSVFileHandler_Init()
{
    CVisMSVFileHandler* MSV_handler = new CVisMSVFileHandler;
    return MSV_handler;
}
        

// @mfunc Does the handler support images of the specified pixel type with
// the specifed number of bands?
int CVisMSVFileHandler::SupportsPixelType(
                                          int evispixfmt,     // @parm pixel type
                                          int nbands)               // @parm number of bands
{
    return true;
}

// @mfunc Does the specified extension match the extension of a file type
// supported by this file handler?
int CVisMSVFileHandler::MatchExtension(
                                       const char *filetype)  // @parm Image file type (tga, bmp, etc.)
{
    return CVisFileHandler::MatchIgnoringCase(filetype, "msv");
}

//
//  Header info for MSV files
//
// (version) (cbit) (left) (top) (right) (bottom) (nbands) (bpp) (evispixfmt)
//

static char *g_szMSVHeader = "%d %d %d %d %d %d %d %d %#x\n";
static char *g_szMSVHeaderR = "%d %d %d %d %d %d %d %d %x\n";
const int kwVersionCur = 0;

// @mfunc Attempt to read the file header.  Return TRUE is successful.
int CVisMSVFileHandler::ReadHeader(
                                   SVisFileDescriptor &fd,  // @parm File descriptor
                                   CVisImageBase &img)          // @parm Image to be read in.
{
	SetClientName("CVisMSVFileHandler::ReadHeader()");

    FILE *stream = fd.stream;
	int wVersion, cbit, wRight, wBottom;

    // Find the image command
    if (fscanf(stream, g_szMSVHeaderR, &wVersion, &cbit, &fd.left, &fd.top,
			&wRight, &wBottom, &fd.bands, &fd.bits_per_pixel, &fd.evispixfmt) != 9)
	{
        return 1;  // fail
//        throw CVisFileIOError(fd.filename,
//				"Invalid MSV file header",
//				eviserrorRead, GetClientName());
	}

	fd.cols = wRight - fd.left;
	fd.rows = wBottom - fd.top;

	if ((wVersion > kwVersionCur) || (cbit != 4))  // LATER:  Add 6 and 8 to a later version.
	{
         return 1;  // fail
//         throw CVisFileIOError(fd.filename,
//				"Unsupported MSV file version",
//				eviserrorRead, GetClientName());
	}

    fd.ascii = 1;
    return 0;
}

// @func Helper function to read a hex digit from a stream.
inline static int gethex(FILE *stream)
{
    int c = getc(stream);
    while (isspace(c))
        c = getc(stream);
    int nibble1 = (c < 'a') ? c - '0' : c - 'a' + 10;
    c = getc(stream);
    int nibble2 = (c < 'a') ? c - '0' : c - 'a' + 10;
    return (nibble1 << 4) + nibble2;
}

// @mfunc Attempt to read the file body.  Return TRUE is successful.
int CVisMSVFileHandler::ReadBody(
                                 SVisFileDescriptor &fd,    // @parm File descriptor
                                 CVisImageBase &img)            // @parm Image to be read in.
{
	SetClientName("CVisMSVFileHandler::ReadBody()");
	
	assert(((fd.bits_per_pixel) & 0x7) == 0);
	assert(fd.bits_per_pixel / 8 == img.CbPixel());

    int cbRow = img.CbRow();

    // Read in the data, one row at a time
    for (int y = img.Top(); y < img.Bottom(); ++y)
	{
        BYTE *pb = (BYTE *) img.PbPixel(img.Left(), y, 0);
		for (int ib = 0; ib < cbRow; ++ib)
			pb[ib] = (BYTE) gethex(fd.stream);
    }
    return 0;
}
 
// @mfunc Attempt to write the file header.  Return TRUE is successful.
int CVisMSVFileHandler::WriteHeader(
                                    SVisFileDescriptor &fd,     // @parm File descriptor
                                    CVisImageBase &img)             // @parm Image to be written out.
{
	fd.ascii = 1;
	fd.cols = img.Width();
	fd.rows = img.Height();
	fd.left = img.Left();
	fd.top = img.Top();
	fd.bands = img.NBands();
	fd.bits_per_pixel = img.CbPixel() * 8;
	fd.evispixfmt = img.PixFmt();

    fprintf(fd.stream, g_szMSVHeader, kwVersionCur, 4, fd.left, fd.top,
			img.Right(), img.Bottom(), fd.bands, fd.bits_per_pixel, fd.evispixfmt);

    return 0;
}

// @func Helper function to write hex digits to a stream.
inline static void puthex(BYTE b, FILE *stream)
{
    static const char hex[] = "0123456789abcdef";
    putc(hex[(b>>4)&0xf], stream);
    putc(hex[(b   )&0xf], stream);
}

// @mfunc Attempt to write the file body.  Return TRUE is successful.
int CVisMSVFileHandler::WriteBody(
                                  SVisFileDescriptor &fd,   // @parm File descriptor
                                  CVisImageBase &img)           // @parm Image to be written out.
{
	SetClientName("CVisMSVFileHandler::WriteBody()");
	
	assert(((fd.bits_per_pixel) & 0x7) == 0);
	assert(fd.bits_per_pixel / 8 == img.CbPixel());

    int cbRow = img.CbRow();

    // Read in the data, one row at a time
    for (int y = img.Top(); y < img.Bottom(); ++y)
	{
        BYTE *pb = (BYTE *) img.PbPixel(img.Left(), y, 0);
		for (int ib = 0; ib < cbRow; ++ib)
			puthex(pb[ib], fd.stream);
		fprintf(fd.stream, "\n");
    }

    return 0;
}
