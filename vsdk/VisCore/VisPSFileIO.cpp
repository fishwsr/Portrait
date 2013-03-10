///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISPPMFILEIO
//
// @module VisPSFileIO.cpp |
//
// Read and write PS (Postscript) image files.
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
//
// @devnote:(INTERNAL)
//
// If the picstr command is used in place of the rowbuf command, this
// code will not be able to read the image from the file.  We might want
// to support the picstr command in the future.
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
class VisCoreExport CVisPSFileHandler : public CVisFileHandler {
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

private:
    static bool s_fMakeInvisibleWhite;
};

bool CVisPSFileHandler::s_fMakeInvisibleWhite = false;


// @func Returns a handler for PS image file i/o.
// @rdesc File handler for i/o.
VisCoreExport CVisFileHandler *CVisPSFileHandler_Init()
{
    CVisPSFileHandler* PS_handler = new CVisPSFileHandler;
    return PS_handler;
}
        

// @mfunc Does the handler support images of the specified pixel type with
// the specifed number of bands?
int CVisPSFileHandler::SupportsPixelType(
                                          int evispixfmt,     // @parm pixel type
                                          int nbands)               // @parm number of bands
{
    return ((evispixfmt == evispixfmtGrayByte)
				|| (evispixfmt == evispixfmtRGBAByte))
			&& (nbands <= 1);
}

// @mfunc Does the specified extension match the extension of a file type
// supported by this file handler?
int CVisPSFileHandler::MatchExtension(
                                       const char *filetype)  // @parm Image file type (tga, bmp, etc.)
{
    return
        CVisFileHandler::MatchIgnoringCase(filetype, "ps") ||
        CVisFileHandler::MatchIgnoringCase(filetype, "eps");
}

//
//  Header info for PostScript files
//

static char *EPS_header = "\
%%!PS-Adobe-3.0 EPSF-3.0\n\
%%%%BoundingBox: %d %d %d %d\n\
%%%%Creator: MSVisSDK\n\
%%%%CreationDate: %s\n\
%%%%LanguageLevel: 2\n\
%%%%EndComments\n\
\n\
%d %d translate\n\
%d %d scale\n\
\n";

// LATER (erikhan):  What if the picstr command is used in place of
// the rowbuf command?
// (Also, we could add support for lines of hex characters that are
// split by newline characters, since this happens in practice when
// PS files are sent via email.)

static char *PS_imagecmd = "\
/rowbuf %d string def\n\
%d %d 8 [%d 0 0 -%d 0 %d]\n\
{currentfile rowbuf readhexstring pop}\n\
%s\n\
\n";


static char *EPS_epilogue= "\n\
showpage\n\
";

// @mfunc Attempt to read the file header.  Return TRUE is successful.
int CVisPSFileHandler::ReadHeader(
                                   SVisFileDescriptor &fd,  // @parm File descriptor
                                   CVisImageBase &img)          // @parm Image to be read in.
{
	SetClientName("CVisPSFileHandler::ReadHeader()");

    FILE *stream = fd.stream;
    char line[1024], imagecmd[1024];
    int ncols3, ncols1, nrows1, nrows2;
    assert(&img != 0);  // unused argument

    // Check the magic number
    if (getc(stream) != '%' || getc(stream) != '!')
		throw CVisFileIOError(fd.filename,
            "File does not start with %!", eviserrorRead, GetClientName());

    // Find the image command
    while (fscanf(stream, PS_imagecmd, &ncols3, &fd.cols, &fd.rows,
					&ncols1, &nrows1, &nrows2, imagecmd) != 7)
	{
        if (fgets(line, 1024, stream) == NULL)    // skip rest of line
		    throw CVisFileIOError(fd.filename,
                "Could not find /rowbuf ... image command",
                eviserrorRead, GetClientName());
	}

    int colorimage = strcmp(imagecmd, "image") != 0;
    if (colorimage)
        fgets(line, 1024, stream);    // get the "3 colorimage" part
    fd.bands = 1;
    fd.bits_per_pixel = colorimage ? 8 : 24;
    fd.ascii = 1;
    fd.evispixfmt = colorimage ? evispixfmtRGBAByte : evispixfmtGrayByte;
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
int CVisPSFileHandler::ReadBody(
                                 SVisFileDescriptor &fd,    // @parm File descriptor
                                 CVisImageBase &img)            // @parm Image to be read in.
{
	SetClientName("CVisPSFileHandler::ReadBody()");
	
    FILE *stream = fd.stream;

    // Bitmap mode (P1, P4) not currently supported
    assert(fd.bits_per_pixel == 8 || fd.bits_per_pixel == 24);

    // Ascii (not raw) mode currently supported
    assert(fd.ascii != 0);

    int Height = img.Height(), Width = img.Width(), r, c, i;

    // Read in the data, one row at a time
    for (r = 0, i = 0; r < Height; r++) {
        BYTE *p = (BYTE *) img.PbPixel(img.Left(), img.Top()+r, 0);
        CVisRGBABytePixel *q = (CVisRGBABytePixel *) p;

        switch (img.PixFmt()) {
        case evispixfmtGrayByte:
            for (c = 0; c < Width; c++, i++) {
                p[c] = (BYTE) gethex(stream);
            }
            break;
        case evispixfmtRGBAByte:
            for (c = 0; c < Width; c++, i++) {
                q[c].SetR((BYTE) gethex(stream));
                q[c].SetG((BYTE) gethex(stream));
                q[c].SetB((BYTE) gethex(stream));
                q[c].SetA(255);
            }
            break;
        }
    }
    return 0;
}

// @func Helper function to write the date in a PS file.
static void PS_date(char *buf)
{
    // See on-line help for time() for alternative possibilities
    char buf1[9], buf2[9];
    /* Display operating system-style date and time. */
    _strdate(buf1);
    _strtime(buf2);
    sprintf(buf, "%s %s", buf1, buf2);
}
 
// @mfunc Attempt to write the file header.  Return TRUE is successful.
int CVisPSFileHandler::WriteHeader(
                                    SVisFileDescriptor &fd,     // @parm File descriptor
                                    CVisImageBase &img)             // @parm Image to be written out.
{
    FILE *stream = fd.stream;
    char datestr[128];
    PS_date(datestr);
    
    // Print encapsulated PostScript header
    int pwidth  = (fd.print_width  > 0) ? fd.print_width  : img.Width();
    int pheight = (fd.print_height > 0) ? fd.print_height : img.Height();
    int hoffset = ((85*72/10) - pwidth )/2;     // 8.5" wide page
    int voffset = ((11*72   ) - pheight)/2;     // 11"  high page
    fprintf(stream, EPS_header, hoffset, voffset,
            hoffset + pwidth, voffset + pheight, datestr,
            hoffset, voffset, pwidth, pheight);

    return 0;
}

// @func Helper function to write hex digits to a stream.
inline static void puthex(int c, FILE *stream)
{
    static const char hex[] = "0123456789abcdef";
    putc(hex[(c>>4)&0xf], stream);
    putc(hex[(c   )&0xf], stream);
}

// @mfunc Attempt to write the file body.  Return TRUE is successful.
int CVisPSFileHandler::WriteBody(
                                  SVisFileDescriptor &fd,   // @parm File descriptor
                                  CVisImageBase &img)           // @parm Image to be written out.
{
	SetClientName("CVisPSFileHandler::WriteBody()");
    const int rowsize = 78, graysize = rowsize/2, colorsize = rowsize/6;

    FILE *stream = fd.stream;
    int Height = img.Height(), Width = img.Width(), r, c, i;

    // Print the image or colorimage command
    int colorimage = (img.PixFmt() == evispixfmtRGBAByte);
    fprintf(stream, PS_imagecmd, (colorimage ? 3 : 1)*img.Width(),
            img.Width(), img.Height(),
            img.Width(), img.Height(), img.Height(),
            colorimage ? "false 3 colorimage" : "image");

    // Write out the data as hexadecimals
    for (r = 0, i = 0; r < Height; r++) {
        BYTE *p = (BYTE *) img.PbPixel(img.Left(), img.Top()+r, 0);
        CVisRGBABytePixel *q = (CVisRGBABytePixel *) p;

        switch (img.PixFmt()) {
        case evispixfmtGrayByte:
            for (c = 0; c < Width; c++, i++) {
                puthex(p[c], stream);
                if (i == graysize-1)
                    putc('\n', stream), i = -1;
            }
            break;
        case evispixfmtRGBAByte:
            for (c = 0; c < Width; c++, i++) {
                if (s_fMakeInvisibleWhite && q[c].A() != 255)
                    fprintf(stream, "ffffff");
                else {
                    puthex(q[c].R(), stream);
                    puthex(q[c].G(), stream);
                    puthex(q[c].B(), stream);
                }
                if (i == colorsize-1)
                    putc('\n', stream), i = -1;
            }
            break;
        }
    }

    // Epilogue
    if (i != 0)
        putc('\n', stream);
    if (fprintf(stream, EPS_epilogue) <= 0)
		throw CVisFileIOError(fd.filename,
					 "Error while writing file",eviserrorWrite,
			         GetClientName(), __FILE__, __LINE__);

    return 0;
}
