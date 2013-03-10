///////////////////////////////////////////////////////////////////////////
//
// NAME
//  imgMagick.cpp -- read and write image files using ImageMagick software
//
// DESCRIPTION
//  Defines and instantiates a subclass of CVisFileHandler
//  which reads and write images using the ImageMagick package.
//
//  For a list of supported file types, see  MagickSupportedImageFormats
//  below.
//  Note that some of these formats may not be (yet) implemented:
//  some of them require external packages, which need to be imported
//  and converted into our environment.
//
// SEE ALSO
//  ImageFileIO.cpp     basic file i/o routines
//  convert.c           ImageMagick file conversion routine
//  magick/*.[hc]       ImageMagick implementation files
//  www/*.html          ImageMagick documentation
//
// BUGS
//  We don't currently deal with colormaps.  There is a slot in the
//  ImageClass for colormaps, but we have do decide several things:
//    . are colormapped images always stored using CVisByteImage
//    . if so, when you open a colormapped image, do you want the
//      indices and the color table, or just the gray value of each pixel?
//
// DESIGN
//
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


// UNDONE:  Redo the description at the top of this file.

// UNDONE:  Use std::string for global variables and synchornize access to
// the ImageMagick functions.

// UNDONE:  We really should add code to pass errors and warnings from
// this DLL back to the caller.  (We shouldn't throw any exceptions
// in this code.)


#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// UNDONE:  Remove the dependency on VisError.h
#include "..\VisCore\VisError.h"
const long
  MaxColormapSize = 65535L;

#define _VISUALC_ 1
extern "C" {
#include "..\..\ImageMagick\magick\magick.h"
}

extern "C" {
#include "..\..\ImageMagick\magick\colors.c"
#include "..\..\ImageMagick\magick\image.c"
}
#undef class    // undo redefinition in magick.h

#include <assert.h>

#include "VisXImageMagick.h"

#ifndef UncompressImage
#define UncompressImage  UncondenseImage
#endif // UncompressImage


static const char *g_szFnCur = 0;
static char *g_szWarningMessage = 0;
static char *g_szWarningQualifier = 0;
static WarningHandler g_pfnWarningHandlerPrev = 0;
static char *g_szErrorMessage = 0;
static char *g_szErrorQualifier = 0;
static ErrorHandler g_pfnErrorHandlerPrev = 0;
static bool g_fHadWarning = false;
static bool g_fHadError = false;


// UNDONE:  These two arrays should be in VisCore.  We should change this
// file to pass an error message back to the file handler in VisCore.
// The file handler should throw errors if needed.
const char *MagickErrorMessages[] = {
"Memory allocation error",
"Unable to allocate",
"Unable to open file",
"An error has occurred reading from file",
"Cannot read",
"Unable to read",
"Incorrect image header in file",
"Insufficient image data in file",
"insufficient image data in",
"Image file or does not contain any image data",
"too much image data in file",
"Cannot write",
"Unable to write",
"Not a",
"Corrupt",
"Invalid",
"image size is 0",
"image size is zero",
"Unable to",
"not supported",
"not available",
"failed",
0,
};


long MagickErrorCodes[] = {
	eviserrorMemory,
	eviserrorMemory,
    eviserrorOpen,
	eviserrorRead,
	eviserrorRead,
	eviserrorRead,
	eviserrorRead,
	eviserrorRead,
	eviserrorRead,
	eviserrorRead,
	eviserrorRead,
	eviserrorWrite,
	eviserrorWrite,
	eviserrorPixFmt,
	eviserrorCorrupt,
	eviserrorCorrupt,
	eviserrorCorrupt,
	eviserrorCorrupt,
	eviserrorOpFailed,
	eviserrorOpFailed,
	eviserrorOpFailed,
	eviserrorOpFailed,
	eviserrorMagickMisc
};



void __cdecl VisImageMagickWarningHandler(const WarningType wErrorType,
		const char *szMessage, const char *szQualifier)
{
	// Note:  wErrorType is unused.  It was added in ImageMagick verision 4.1.8
	// and may need to be removed if you're using an earlier version of ImageMagick.

	// Fail on any warning except the worning that no delagates configuration
	// file was found.
	if (strcmp(szMessage, "no delegates configuration file found") != 0)
	{
		g_fHadWarning = true;
		if ((g_szWarningMessage == 0) && (g_szWarningQualifier == 0))
		{
			if (szMessage != 0)
			{
				g_szWarningMessage = new char[strlen(szMessage)+1];
				strcpy(g_szWarningMessage, szMessage);
			}

			if (szQualifier != 0)
			{
				g_szWarningQualifier = new char[strlen(szQualifier) + 1];
				strcpy(g_szWarningQualifier, szQualifier);
			}
		}
	}
}


void __cdecl VisImageMagickErrorHandler(const ErrorType wErrorType,
		const char *szMessage, const char *szQualifier)
{
	// Note:  wErrorType is unused.  It was added in ImageMagick verision 4.1.8
	// and may need to be removed if you're using an earlier version of ImageMagick.

	g_fHadError = true;
	if ((g_szErrorMessage == 0) && (g_szErrorQualifier == 0))
	{
		if (szMessage != 0)
		{
			g_szErrorMessage = new char[strlen(szMessage)+1];
			strcpy(g_szErrorMessage, szMessage);
		}

		if (szQualifier != 0)
		{
			g_szErrorQualifier = new char[strlen(szQualifier) + 1];
			strcpy(g_szErrorQualifier, szQualifier);
		}
	}
}


void VisClearMagickError(void)
{
	g_fHadWarning = false;
	g_fHadError = false;
	if (g_szWarningMessage != 0)
	{
		delete[] g_szWarningMessage;
		g_szWarningMessage = 0;
	}
	if (g_szWarningQualifier != 0)
	{
		delete[] g_szWarningQualifier;
		g_szWarningQualifier = 0;
	}
	if (g_szErrorMessage != 0)
	{
		delete[] g_szErrorMessage;
		g_szErrorMessage = 0;
	}
	if (g_szErrorQualifier != 0)
	{
		delete[] g_szErrorQualifier;
		g_szErrorQualifier = 0;
	}
}


void VisEnterMagickFn(char *szFn)
{
	// For now, we assume that File I/O is single-threaded.
	// LATER:  If that's not the case, we'll need to add synchronization.
	assert(g_szFnCur == 0);

	g_szFnCur = szFn;

	VisClearMagickError();

	// Note:  In ImageMagick version 4.1.8, an integer argument was added to the
	// error and warning handlers.  If you're using an older version of ImageMagick,
	// you'll need to to remove the (unused) wErrorType argument from the
	// VisImageMagickErrorHandler and VisImageMagickWarningHandler function definitions
	// at the top of this file.
	g_pfnWarningHandlerPrev = SetWarningHandler(VisImageMagickWarningHandler);
	g_pfnErrorHandlerPrev = SetErrorHandler(VisImageMagickErrorHandler);
}


void VisLeaveMagickFn(void)
{
	g_szFnCur = 0;

	VisClearMagickError();

	SetWarningHandler(g_pfnWarningHandlerPrev);
	g_pfnWarningHandlerPrev = 0;
	SetErrorHandler(g_pfnErrorHandlerPrev);
	g_pfnErrorHandlerPrev = 0;
}


void VisMagickTestAndThrow(ImageInfo *pimageinfo = 0, Image *pimage = 0) 
{ 
	if ((g_fHadError) || (g_fHadWarning))
	{
		// Clean up
		if (pimage != 0)
		{
#ifdef VIS_OLD_CODE  // pimage->pixels may be invalid here.
			if (pimage->pixels != 0)
			{
				free(pimage->pixels);
				pimage->pixels = 0;
			}
#endif // VIS_OLD_CODE

			DestroyImages(pimage);
		}

		if (pimageinfo != 0)
		{
			delete pimageinfo;
		}
	}

	if (g_fHadError)
	{
		const char *szFn = g_szFnCur;
		g_szFnCur = 0;

		throw CVisFileIOError(g_szErrorQualifier, g_szErrorMessage,
		             VisErrorMessageToCode(g_szErrorMessage, MagickErrorMessages,
							MagickErrorCodes),
		             szFn, __FILE__);
	}

	if (g_fHadWarning)
	{
		const char *szFn = g_szFnCur;
		g_szFnCur = 0;

		throw CVisFileIOError(g_szWarningQualifier, g_szWarningMessage,
		             VisErrorMessageToCode(g_szWarningMessage, MagickErrorMessages,
							MagickErrorCodes),
		             szFn, __FILE__);
	}
} 


VisXImageMagickExport void __stdcall VisFinishedWithMagick(void)
{
	DestroyMagickInfo();
	VisClearMagickError();
}


VisXImageMagickExport int __stdcall VisFReadImageMagickHeader(
		SVisImageMagickData& refimagemagickdata)
{
	VisEnterMagickFn("VisFReadImageMagickHeader");

    // Initialize the image_info structure
    ImageInfo *image_info = new ImageInfo;
	if (image_info == 0)
	{
		g_szFnCur = 0;
 		throw CVisMemoryError("VisFReadImageMagickHeader", __FILE__, __LINE__);
	}

    GetImageInfo(image_info);
	VisMagickTestAndThrow(image_info);

    strcpy(image_info->filename, refimagemagickdata.m_filename);
    if (refimagemagickdata.m_filetype)
        strcpy(image_info->magick, refimagemagickdata.m_filetype);
    
    // Read the image using ImageMagick
    Image *image = ReadImage(image_info);
	VisMagickTestAndThrow(image_info, image);
	// Not sure if image can be 0 at this point, so we check to be safe.
	if (image == 0)
	{
		g_szFnCur = 0;
		delete image_info;
 		throw CVisMemoryError("VisFReadImageMagickHeader", __FILE__, __LINE__);
	}

    refimagemagickdata.m_cbitPixel = image->depth;       // is this right?
    refimagemagickdata.m_dxImage = image->columns;
    refimagemagickdata.m_dyImage = image->rows;
    if (image->c_class == PseudoClass)
		refimagemagickdata.m_cColor = image->colors;
	else
		refimagemagickdata.m_cColor = 0;
    refimagemagickdata.m_fHasAlpha = ((image->matte != 0) ? true : false);
    refimagemagickdata.m_pimageinfo = image_info;
    refimagemagickdata.m_pimage = image;

	VisLeaveMagickFn();
	
	return 0;
}


VisXImageMagickExport int __stdcall VisFReadImageMagickBody(
		SVisImageMagickData& refimagemagickdata)
{
	VisEnterMagickFn("VisFReadImageMagickBody");

    // Restore the internal parameters
    ImageInfo *image_info = refimagemagickdata.m_pimageinfo;
    Image *image = refimagemagickdata.m_pimage;

	VisMagickTestAndThrow(image_info, image);

    int cols = refimagemagickdata.m_dxImage;

    // Colorspace moved from Image to ImageInfo when we updated the ImaageMagick files.
	BOOL fUseColormap = (refimagemagickdata.m_cColor > 0)
//			&& ((image->colorspace == UndefinedColorspace)
//				|| (image->colorspace == RGBColorspace))
			&& ((image_info->colorspace == UndefinedColorspace)
				|| (image_info->colorspace == RGBColorspace))
			&& ((image->colors == 256) || (image->colors == 16))
			&& (image->colormap != 0);
	if (fUseColormap) {
		PixelPacket *pcolormap = image->colormap;
        BYTE no_alpha = (image->matte) ? 0 : 255;

		if (refimagemagickdata.m_cColor > image->colors)
			refimagemagickdata.m_cColor = image->colors;

		for (int i = 0; i < refimagemagickdata.m_cColor; ++i) {
			refimagemagickdata.m_prgbquadColorMap[i].rgbBlue = pcolormap[i].blue;
			refimagemagickdata.m_prgbquadColorMap[i].rgbGreen = pcolormap[i].green;
			refimagemagickdata.m_prgbquadColorMap[i].rgbRed = pcolormap[i].red;
			refimagemagickdata.m_prgbquadColorMap[i].rgbReserved
					= (no_alpha | (BYTE) pcolormap[i].opacity);
		}
	}
	else
	{
		refimagemagickdata.m_cColor = 0;
	}

    PixelPacket *p = image->pixels;
	int nRow = 0;
    BYTE *pbRowLim = refimagemagickdata.m_pbImageFirst
				+ refimagemagickdata.m_dyImage * refimagemagickdata.m_cbRow;
	for (BYTE *pbRowCur = refimagemagickdata.m_pbImageFirst;
			pbRowCur < pbRowLim;
			pbRowCur += refimagemagickdata.m_cbRow,nRow++) {
		if (refimagemagickdata.m_cbitPixel == 8) {
            BYTE *q = pbRowCur;
            if (image->c_class == PseudoClass && image->colors == 0) {
                // Really a GrayVisMatrixScale image: only index is valid
                for (int c = 0; c < cols; c++, p++)
				{
                    q[c] = image->indexes[c];
				}
            } else if (fUseColormap) {
                for (int c = 0; c < cols; c++, p++)
				{
                    q[c] = image->indexes[c];
				}
            } else {
				// Note:  this may not be the right decision:
				//  We may want to blend RGB to get Y.
				//  We can revisit this later, when we make more colormap changes.
                for (int c = 0; c < cols; c++, p++)
                    q[c] = p->green;
            }
        } else {
            BYTE no_alpha = (image->matte) ? 0 : 255;
            BYTE not_no_alpha = ~no_alpha;
            RGBQUAD *q = (RGBQUAD *) pbRowCur;
            if (image->c_class == PseudoClass && image->colors == 0) {
                // Really a GrayVisMatrixScale image: only index is valid
				for (int c = 0; c < cols; c++, p++) {
                    BYTE g = image->indexes[c];
                    q[c].rgbBlue = g;
					q[c].rgbGreen = g;
					q[c].rgbRed = g;
                    q[c].rgbReserved = 255;
                }
            } else {
                PixelPacket* pp = GetPixelCache(image,0,nRow,image->columns,1);
                for (int c = 0; c < cols; c++, p++) {
                    q[c].rgbBlue = pp->blue;
					q[c].rgbGreen = pp->green;
					q[c].rgbRed = pp->red;
// Changed for compatibility with files created with older versions of ImageMagick.
// UNDONE:  Review this change later.  Is the current ImageMagick TGA format right?
//                    q[c].rgbReserved = (no_alpha | (BYTE) p->index);
                    q[c].rgbReserved = ~(not_no_alpha & (BYTE) pp->opacity);
					pp++;
                }
            }
        }
    }

    // Clean up
	DestroyImages(image);
	delete image_info;
    refimagemagickdata.m_pimageinfo = 0;
    refimagemagickdata.m_pimage = 0;

	VisLeaveMagickFn();
	
	return 0;
}


VisXImageMagickExport int __stdcall VisFWriteImageMagickHeader(
		SVisImageMagickData& refimagemagickdata)
{
	VisEnterMagickFn("VisFWriteImageMagickHeader");

    // Initialize the image_info structure
    ImageInfo *image_info = new ImageInfo;
	if (image_info == 0)
 	{
		g_szFnCur = 0;
		throw CVisMemoryError("VisFWriteImageMagickHeader", __FILE__, __LINE__);
	}

    GetImageInfo(image_info);
	VisMagickTestAndThrow(image_info);

    strcpy(image_info->filename, refimagemagickdata.m_filename);
    if (refimagemagickdata.m_filetype)
        strcpy(image_info->magick, refimagemagickdata.m_filetype);

    if (refimagemagickdata.m_wQuality > 0
			&& refimagemagickdata.m_wQuality <= 100)
	{
        image_info->quality = refimagemagickdata.m_wQuality;
		if ((image_info->quality == 100)
				&& (stricmp(refimagemagickdata.m_filetype, "tga") == 0))
		{
			// Turn off TGA compression
			image_info->compression = NoCompression;
		}
	}

    // Allocate an ImageMagick image and fill in the slots
    Image *image = AllocateImage(image_info);
	VisMagickTestAndThrow(image_info, image);
	// Not sure if image can be 0 at this point, so we check to be safe.
	if (image == 0)
	{
		g_szFnCur = 0;
		delete image_info;
 		throw CVisMemoryError("VisFWriteImageMagickHeader", __FILE__, __LINE__);
	}

    image->rows = refimagemagickdata.m_dyImage;
    image->columns = refimagemagickdata.m_dxImage;
    if (refimagemagickdata.m_cbitPixel == 8) {
        image->matte = 0;
        image->c_class = PseudoClass;
    } else {
        image->matte = ((refimagemagickdata.m_fHasAlpha) ? 1 : 0);
        image->c_class = DirectClass;
    }

    // Allocate the storage
    image->pixels = (PixelPacket *) AllocateMemory(image->columns * image->rows * sizeof(PixelPacket));

    refimagemagickdata.m_pimageinfo = image_info;
    refimagemagickdata.m_pimage = image;

	VisLeaveMagickFn();
	
	return 0;
}


VisXImageMagickExport int __stdcall VisFWriteImageMagickBody(
		SVisImageMagickData& refimagemagickdata)
{
	VisEnterMagickFn("VisFWriteImageMagickBody");

    // Restore the internal parameters
    ImageInfo *image_info = refimagemagickdata.m_pimageinfo;
    Image *image = refimagemagickdata.m_pimage;

    // Convert to ImageMagick's RunlengthPacket structure
    PixelPacket *p = image->pixels;
    int cols = refimagemagickdata.m_dxImage;
    BYTE *pbRowLim = refimagemagickdata.m_pbImageFirst
				+ refimagemagickdata.m_dyImage * refimagemagickdata.m_cbRow;
	int nRow = 0;

	for (BYTE *pbRowCur = refimagemagickdata.m_pbImageFirst;
			pbRowCur < pbRowLim;
			pbRowCur += refimagemagickdata.m_cbRow,nRow++) {
        if (refimagemagickdata.m_cbitPixel == 8) {
            BYTE *q = pbRowCur;
            for (int c = 0; c < cols; c++, p++) {
               p->red = p->green = p->blue = q[c];
               p->opacity = q[c];
            }
        } else {
            RGBQUAD *q = (RGBQUAD *) pbRowCur;
            PixelPacket* pp = GetPixelCache(image,0,nRow,image->columns,1);
			for (int c = 0; c < cols; c++, pp++) {
               pp->red   = q[c].rgbRed;
               pp->green = q[c].rgbGreen;
               pp->blue  = q[c].rgbBlue;
// Changed for compatibility with files created with older versions of ImageMagick.
// UNDONE:  Review this change later.  Is the current ImageMagick TGA format right?
//               p->index = q[c].rgbReserved;
               pp->opacity = ~q[c].rgbReserved;
            }
			SyncPixelCache(image);
        }
    }

	// Set the colormap to use when writing grayscale images.
	// (Should we do this for all types, or only for types like
	// GIF and BMP that use colormaps?)
	// LATER:  We could probably improve the efficiency of this code.
	bool fAddedColormap = false;
	if ((refimagemagickdata.m_cbitPixel == 8)
			&& (image->colors == 0)
			&& (image->colormap == 0))
//			&& ((stricmp(image_info->magick, "bmp") == 0)
//				|| ((stricmp(image_info->magick, "gif") == 0))))
	{
		if (refimagemagickdata.m_cColor > 0)
		{
			assert(refimagemagickdata.m_prgbquadColorMap != 0);

			image->colors = refimagemagickdata.m_cColor;
			// LATER:  We might want to allow more colors.
			if (image->colors > 256)
				image->colors = 256;

			image->colormap = (PixelPacket *) AllocateMemory(image->colors * sizeof(ColorPacket));

			for (int iColor = 0; iColor < image->colors; ++iColor)
			{
				image->colormap[iColor].red
						= refimagemagickdata.m_prgbquadColorMap[iColor].rgbRed;
				image->colormap[iColor].green
						= refimagemagickdata.m_prgbquadColorMap[iColor].rgbGreen;
				image->colormap[iColor].blue
						= refimagemagickdata.m_prgbquadColorMap[iColor].rgbBlue;
				image->colormap[iColor].opacity
						= refimagemagickdata.m_prgbquadColorMap[iColor].rgbReserved;
			}
		}
		else
		{
			image->colors = 256;

			image->colormap = (PixelPacket *) AllocateMemory(image->colors * sizeof(ColorPacket));

			for (int iColor = 0; iColor < image->colors; ++iColor)
			{
				image->colormap[iColor].red = iColor;
				image->colormap[iColor].green = iColor;
				image->colormap[iColor].blue = iColor;
				image->colormap[iColor].opacity = iColor;
			}
		}

		fAddedColormap = true;
	}

#if 0
    // Prepare the image for writing (is all of this necessary?)
    CompressImage(image);
    IsGrayImage(image);
    SyncImage(image);
#endif

    // Write out the image
    int return_status = WriteImage(image_info, image);

	// Need to destroy the colormap if we created it above.
	if (fAddedColormap)
	{
		image->colors = 0;
		FreeMemory(image->colormap);
		image->colormap = 0;
	}
	
    // Clean up
	if (image->pixels != 0)
	{
		FreeMemory(image->pixels);
		image->pixels = 0;
	}

    DestroyImages(image);
    delete image_info;
    refimagemagickdata.m_pimageinfo = 0;
    refimagemagickdata.m_pimage = 0;

	VisLeaveMagickFn();
	
    return (return_status) ? 0 : -1;
}
