// VisXImageMagick.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// UNDONE:  Add fields for exception (file, line, function, and message)
// strings to SVisImageMagickData, change exported functions to place
// exception strings in structure and return an HRESULT, and change
// VisCore functions to check the HRESULT and throw if needed.  (We might
// want an option to throw or return an HRESULT in the file I/O functions.)


// Struct used to pass information to the ImageMagick helper functions.
struct SVisImageMagickData
{
	// SVisFileDescriptor entries
    const char *m_filename;   // file name
    const char *m_filetype;   // file type (same as allowed extensions)
	int m_cbitPixel;	// We only support 8 and 32
	int m_dxImage;		// Width of image
	int m_dyImage;		// Height of Image
	struct _ImageInfo *m_pimageinfo;	// temporary ImageMagick data (Internal1)
	struct _Image *m_pimage;			// temporary ImageMagick data (Internal2)

	// Other entries
	int m_cbRow;		// Offset between pixels in same col and adjacent rows
	BYTE *m_pbImageFirst;	// Pointer to first byte in top-left pixel
	RGBQUAD *m_prgbquadColorMap;// Colormap, may be 0 if not used
	int m_cColor;		// 0 if colormap is not used
	bool m_fHasAlpha;	// Has a valid alpha channel

	int m_wQuality;		// Quality of compresstion (0,,100)
};


#ifdef VIS_XIMAGEMAGICK_DLL
#define VisXImageMagickExport	_declspec(dllexport)
#else // VIS_XIMAGEMAGICK_DLL
#define VisXImageMagickExport	_declspec(dllimport)
#endif // VIS_XIMAGEMAGICK_DLL


typedef int (__stdcall *T_PfnVisFReadImageMagickHeader)
		(SVisImageMagickData& imagemagickdata);

typedef int (__stdcall *T_PfnVisFReadImageMagickBody)
		(SVisImageMagickData& imagemagickdata);

typedef int (__stdcall *T_PfnVisFWriteImageMagickHeader)
		(SVisImageMagickData& imagemagickdata);

typedef int (__stdcall *T_PfnVisFWriteImageMagickBody)
		(SVisImageMagickData& imagemagickdata);

typedef void (__stdcall *T_PfnVisFinishedWithMagick)(void);



VisXImageMagickExport int __stdcall VisFReadImageMagickHeader(
		SVisImageMagickData& imagemagickdata);

VisXImageMagickExport int __stdcall VisFReadImageMagickBody(
		SVisImageMagickData& imagemagickdata);

VisXImageMagickExport int __stdcall VisFWriteImageMagickHeader(
		SVisImageMagickData& imagemagickdata);

VisXImageMagickExport int __stdcall VisFWriteImageMagickBody(
		SVisImageMagickData& imagemagickdata);

VisXImageMagickExport void __stdcall VisFinishedWithMagick(void);

