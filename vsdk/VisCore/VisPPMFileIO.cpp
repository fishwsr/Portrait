///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISPPMFILEIO
//
// @module VisPPMFileIO.cpp |
//
// Read and write PPM (Portable Pixmap) image files.
// Defines and instantiates subclass of CVisFileHandler class.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisFileIO\.h> <l VisFileIO\.cpp>
//
// <nl>
// @index | VISPPMFILEIO
//
///////////////////////////////////////////////////////////////////////////


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VisFileIO.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG


// @class
// File handler to read and write PPM (Portable Pixmap) image files.
// @base public | CVisFileHandler
// @xref <c CVisFileHandler>
class VisCoreExport CVisPPMFileHandler : public CVisFileHandler {
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

// @func Returns a handler for ppm image file i/o.
// @rdesc File handler for i/o.
CVisFileHandler *CVisPPMFileHandler_Init()
{
    CVisPPMFileHandler* ppm_handler = new CVisPPMFileHandler;
    return ppm_handler;
}
        

// @mfunc Does the handler support images of the specified pixel type with
// the specifed number of bands?
BOOL CVisPPMFileHandler::SupportsPixelType(
                                          int evispixfmt,     // @parm pixel type
                                          int nbands)               // @parm number of bands
{
    return (evispixfmt == evispixfmtGrayByte || evispixfmt == evispixfmtRGBAByte) &&
        nbands <= 1;
}

// @mfunc Does the specified extension match the extension of a file type
// supported by this file handler?
BOOL CVisPPMFileHandler::MatchExtension(
                                       const char *filetype)  // @parm Image file type (tga, bmp, etc.)
{
    return
        CVisFileHandler::MatchIgnoringCase(filetype, "pbm") ||
        CVisFileHandler::MatchIgnoringCase(filetype, "pgm") ||
        CVisFileHandler::MatchIgnoringCase(filetype, "ppm");
}

// @mfunc Attempt to read the file header.  Return TRUE is successful.
BOOL CVisPPMFileHandler::ReadHeader(
                                   SVisFileDescriptor &fd,  // @parm File descriptor
                                   CVisImageBase &img)          // @parm Image to be read in.
{
	SetClientName("CVisPPMFileHandler::ReadHeader(SVisFileDescriptor&,CVisImageBase&)");

    FILE *stream = fd.stream;
    char magic[1024], c;
    int max_gray;
    assert(&img != 0);  // unused argument

    // Check the magic number
    if (fscanf(stream, "%s", magic) != 1)
		throw CVisFileIOError(fd.filename,"Error in reading PPM header",
			                eviserrorRead, GetClientName());
    if (magic[0] != 'P' && magic[0] != 'p' ||
        magic[1] < '1' || magic[1] > '6') {
		char message_tmp[100];
		sprintf(message_tmp,"Invalid magic number %s in file", magic);
		throw CVisFileIOError(fd.filename,message_tmp,eviserrorRead,GetClientName());
    }
    fd.bands = 1;  // (magic[1] == '3' || magic[1] == '6') ? 3 : 1;
    fd.bits_per_pixel = (magic[1] == '1' || magic[1] == '4') ? 1 :
        (magic[1] == '2' || magic[1] == '5') ? 8 : 24;
    fd.ascii = (magic[1] < '4');
    fd.evispixfmt = (magic[1] == '3' || magic[1] == '6') ? 
        evispixfmtRGBAByte : evispixfmtGrayByte;

    // Skip rest of line and also comment lines
    while ((c = (char) fgetc(stream)) != '\n');
    while ((c = (char) fgetc(stream)) == '#')  // comment line
        while ((c = (char) fgetc(stream)) != '\n');    // skip rest of line
    ungetc(c, stream);

    // Read in the dimensions
    if (fscanf(stream, (fd.bits_per_pixel == 1) ? "%d %d" : "%d %d %d",
        &fd.cols, &fd.rows, &max_gray) != 2 + (fd.bits_per_pixel > 1)) {
		throw CVisFileIOError(fd.filename,"Error in reading width/height/values",
			                 eviserrorRead, GetClientName());
    }

    // Skip rest of line
    if ((c = (char) fgetc(stream)) != '\n')
        ungetc(c, stream);
    return 0;
}

// @mfunc Attempt to read the file body.  Return TRUE is successful.
BOOL CVisPPMFileHandler::ReadBody(
                                 SVisFileDescriptor &fd,    // @parm File descriptor
                                 CVisImageBase &img)            // @parm Image to be read in.
{
	SetClientName("CVisPPMFileHandler::ReadBody(SVisFileDescriptor&,CVisImageBase&)");
	
    FILE *stream = fd.stream;

    // Bitmap mode (P1, P4) not currently supported
    assert(fd.bits_per_pixel == 8 || fd.bits_per_pixel == 24);

    // Ascii (not raw) mode not currently supported
    assert(fd.ascii == 0);

    int Height = img.Height(), Width = img.Width(), bands = img.NBands();
    int psize = img.CbPixel();
    char *line = (char *) new double[Width*bands*psize/sizeof(double)+1];

    // Read in the data, one row at a time
    for (int r = 0; r < Height; r++) {
        char *imgdata = (char *)img.PbPixel(img.Left(), img.Top()+r, 0);

        // Gray image
        if (bands == 1 && psize == 1 && fd.bits_per_pixel == 8) {
            if (fread(imgdata, sizeof(BYTE), Width, stream) !=
                      (size_t) Width) {
				throw CVisFileIOError(fd.filename,
					         "Error while reading file (file too short)",eviserrorRead,
			                 GetClientName());
            }
        } else {

            // Color image
            if (fread(line, sizeof(BYTE), 3*Width, stream) !=
                      (size_t) 3*Width) {
				throw CVisFileIOError(fd.filename,
					         "Error while reading file (file too short)",eviserrorRead,
			                 GetClientName());
            }

            CVisRGBABytePixel *p = (CVisRGBABytePixel *) imgdata;
            for (int c = 0, c3 = 0; c < Width; c++, c3 += 3) {
                p[c].SetR(line[c3+0]);
                p[c].SetG(line[c3+1]);
                p[c].SetB(line[c3+2]);
                p[c].SetA(255);
            }
        }
    }
    delete [] line;
    return 0;
}

// @mfunc Attempt to write the file header.  Return TRUE is successful.
BOOL CVisPPMFileHandler::WriteHeader(
                                    SVisFileDescriptor &fd,     // @parm File descriptor
                                    CVisImageBase &img)             // @parm Image to be written out.
{
    FILE *stream = fd.stream;
    // Write out the header
    fprintf(stream, "P%d\n%d %d\n255\n",
            img.PixFmt() == evispixfmtRGBAByte ? 6 : 5, img.Width(), img.Height());
    return 0;
}


// @mfunc Attempt to write the file body.  Return TRUE is successful.
BOOL CVisPPMFileHandler::WriteBody(
                                  SVisFileDescriptor &fd,   // @parm File descriptor
                                  CVisImageBase &img)           // @parm Image to be written out.
{
	SetClientName("CVisPPMFileHandler::WriteBody(SVisFileDescriptor&,CVisImageBase&)");

    FILE *stream = fd.stream;
    int Height = img.Height(), Width = img.Width(), bands = img.NBands();
    int psize = img.CbPixel();
    char *line = (char *) new double[Width*bands*psize/sizeof(double)+1];

    // Write out the data, one row at a time
    for (int r = 0; r < Height; r++) {
        char *imgdata = (char *)img.PbPixel(img.Left(), img.Top()+r, 0);

        // Gray image
        if (bands == 1 && psize == 1) {
            if (fwrite(imgdata, sizeof(BYTE), Width, stream) !=
                      (size_t) Width) {
				throw CVisFileIOError(fd.filename,
					         "Error while writing file (file too short)",eviserrorWrite,
			                 GetClientName(), __FILE__, __LINE__);
            }
        } else {

            // Color image
            CVisRGBABytePixel *p = (CVisRGBABytePixel *) imgdata;
            for (int c = 0, c3 = 0; c < Width; c++, c3 += 3) {
                line[c3+0] = p[c].R();
                line[c3+1] = p[c].G();
                line[c3+2] = p[c].B();
            }

            if (fwrite(line, sizeof(BYTE), 3*Width, stream) !=
                      (size_t) 3*Width) {
				throw CVisFileIOError(fd.filename,
					         "Error while writing file (file too short)",eviserrorWrite,
			                  GetClientName(), __FILE__, __LINE__);
            }
        }
    }
    delete [] line;
    return 0;
}
