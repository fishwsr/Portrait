// VisFileHandlerMagick.cpp
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#include "VisCorePch.h"
#include "VisCoreProj.h"

#include "..\VisXImageMagick\VisXImageMagick.h"
#include "..\VisXImageMagick\VisXImageMagickVersion.h"


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// File handler to read and write image files using the ImageMagick
// library.
//
// @base public | CVisFileHandler
//
// @xref <c CVisFileHandler>
//
////////////////////////////////////////////////////////////////////////////
class VisCoreExport CVisFileHandlerMagick : public CVisFileHandler
{
// @access Public members
public:
	//------------------------------------------------------------------
	// @group Constructor and destructor


	// @cmember
   	// 
	CVisFileHandlerMagick(void);

	// @cmember
   	// 
	~CVisFileHandlerMagick(void);


	//------------------------------------------------------------------
	// @group Virtual functions

	// @cmember
	// Does the handler support images of the specified pixel type with
	// the specifed number of bands?
    virtual BOOL SupportsPixelType(int evispixfmt, int nbands);

	// @cmember
	// Does the specified extension match the extension of a file type
	// supported by this file handler?
    virtual BOOL MatchExtension(const char *szFiletype);

	// @cmember
	// Should the stream be opened outside of this file handler.
	virtual BOOL ExternallyOpenStream(void);

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


	// Helper function
	bool IsAvailable(void);


// @access:(IMPL) Private members
private:
	HINSTANCE m_hinstImageMagickDll;

	T_PfnVisFReadImageMagickHeader m_pfnFReadImageMagickHeader;
	T_PfnVisFReadImageMagickBody m_pfnFReadImageMagickBody;
	T_PfnVisFWriteImageMagickHeader m_pfnFWriteImageMagickHeader;
	T_PfnVisFWriteImageMagickBody m_pfnFWriteImageMagickBody;
	T_PfnVisFinishedWithMagick m_pfnFinishedWithMagick;
	bool m_fLookedForImageMagickDLL;

	static const char *s_rgszMagickSupportedImageFormats[];
}; 

const char *CVisFileHandlerMagick::s_rgszMagickSupportedImageFormats[] = {
    "AVS",   // AVS X image file.
    "BMP",   // Microsoft Windows bitmap image file.
    "CMYK",  // Raw cyan, magenta, yellow, and black bytes.
    "DCX",   // ZSoft IBM PC multi-page Paintbrush file
    "DIB",   // Microsoft Windows bitmap image file.
    "EPS",   // Adobe Encapsulated PostScript file.
    "EPS2",  // Adobe Level II Encapsulated PostScript file.
    "EPSF",  // Adobe Encapsulated PostScript file.
    "EPSI",  // Adobe Encapsulated PostScript Interchange format.
    "FAX",   // Group 3.
    "FITS",  // Flexible Image Transport System.
    "GIF",   // CompuServe graphics interchange format; 8-bit color.
    "GIF87", // CompuServe graphics interchange format; 8-bit color (version 87a).
    "GRAY",  // Raw gray bytes.
    "HDF",   // Hierarchical Data Format.
    "HISTOGRAM",  // MIFF Histogram Image
    "HTML",  // Hypertext Markup Language with a client-side image map
    "JBIG",  // Joint Bi-level Image experts Group file interchange format.
    "JPEG",  // Joint Photographic Experts Group JFIF format; compressed 24-bit color.
    "JPG",   // Joint Photographic Experts Group JFIF format; compressed 24-bit color.
    "MAP",   // colormap intensities and indices.
    "MATTE", // Raw matte bytes.
    "MIFF",  // Magick image file format.
    "MPEG",  // Motion Picture Experts Group file interchange format.
    "MTV",   // MTV ray tracer format
    "NULL",  // NULL image.
    "PBM",   // Portable bitmap format (black and white).
    "PCD",   // Photo CD.
    "PCX",   // ZSoft IBM PC Paintbrush file.
    "PDF",   // Portable Document Format.
    "PGM",   // Portable graymap format (gray scale).
    "PICT",  // Apple Macintosh QuickDraw/PICT file.
    "PNG",   // Portable Network Graphics.
    "PNM",   // Portable anymap.
    "PPM",   // Portable pixmap format (color).
    "PS",    // Adobe PostScript file.
    "PS2",   // Adobe Level II PostScript file.
    "RAD",   // Radiance image file.
    "RGB",   // Raw red, green, and blue bytes.
    "RGBA",  // Raw red, green, blue, and matte bytes.
    "RLA",   // Alias/Wavefront image file; read only
    "RLE",   // Utah Run length encoded image file; read only.
    "SGI",   // Irix RGB image file.
    "SUN",   // SUN Rasterfile.
    "TEXT",  // raw text file; read only.
    "TGA",   // Truevision Targa image file.
    "TIF",   // Tagged Image File Format.
    "TIFF",  // Tagged Image File Format.
    "UYVY",  // 16bit/pixel interleaved YUV (e.g. used by AccomWSD)
    "TILE",  // tile image with a texture.
    "VICAR", // read only.
    "VID",   // Visual Image Directory.
    "VIFF",  // Khoros Visualization image file.
    "X",     // select image from X server screen.
    "XC",    // constant image of X server color.
    "XBM",   // X Windows system bitmap, black and white only.
    "XPM",   // X Windows system pixmap file (color).
    "XWD",   // X Windows system window dump file (color).
    "YUV",   // CCIR 601 1:1:1 file.
    "YUV3",  // CCIR 601 2:1:1 files.
    0
};


CVisFileHandlerMagick::CVisFileHandlerMagick(void)
  : m_hinstImageMagickDll(0),
	m_pfnFReadImageMagickHeader(0),
	m_pfnFReadImageMagickBody(0),
	m_pfnFWriteImageMagickHeader(0),
	m_pfnFWriteImageMagickBody(0),
	m_pfnFinishedWithMagick(0),
	m_fLookedForImageMagickDLL(false)
{
}




bool CVisFileHandlerMagick::IsAvailable(void)
{
	if (!m_fLookedForImageMagickDLL)
	{
		m_fLookedForImageMagickDLL = true;

#ifdef _DEBUG
		const char *szImageMagickDll = VIS_XIMAGEMAGICK_DEBUG_BASE_NAME ".dll";
#else // _DEBUG
		const char *szImageMagickDll = VIS_XIMAGEMAGICK_BASE_NAME ".dll";
#endif // _DEBUG

		m_hinstImageMagickDll = LoadLibrary(szImageMagickDll);

#ifdef _DEBUG
		DWORD err = GetLastError();
#endif // _DEBUG

		if (m_hinstImageMagickDll != 0)
		{
			m_pfnFReadImageMagickHeader = (T_PfnVisFReadImageMagickHeader)
					GetProcAddress(m_hinstImageMagickDll,
#ifdef _M_ALPHA
					// Alpha exports are __cdecl, but X86 exports are __stdcall
					"?VisFReadImageMagickHeader@@YAHAAUSVisImageMagickData@@@Z");
#else // _M_ALPHA
					"?VisFReadImageMagickHeader@@YGHAAUSVisImageMagickData@@@Z");
#endif // _M_ALPHA
			m_pfnFReadImageMagickBody = (T_PfnVisFReadImageMagickBody)
					GetProcAddress(m_hinstImageMagickDll,
#ifdef _M_ALPHA
					// Alpha exports are __cdecl, but X86 exports are __stdcall
					"?VisFReadImageMagickBody@@YAHAAUSVisImageMagickData@@@Z");
#else // _M_ALPHA
					"?VisFReadImageMagickBody@@YGHAAUSVisImageMagickData@@@Z");
#endif // _M_ALPHA
			m_pfnFWriteImageMagickHeader = (T_PfnVisFWriteImageMagickHeader)
					GetProcAddress(m_hinstImageMagickDll,
#ifdef _M_ALPHA
					// Alpha exports are __cdecl, but X86 exports are __stdcall
					"?VisFWriteImageMagickHeader@@YAHAAUSVisImageMagickData@@@Z");
#else // _M_ALPHA
					"?VisFWriteImageMagickHeader@@YGHAAUSVisImageMagickData@@@Z");
#endif // _M_ALPHA
			m_pfnFWriteImageMagickBody = (T_PfnVisFWriteImageMagickBody)
					GetProcAddress(m_hinstImageMagickDll,
#ifdef _M_ALPHA
					// Alpha exports are __cdecl, but X86 exports are __stdcall
					"?VisFWriteImageMagickBody@@YAHAAUSVisImageMagickData@@@Z");
#else // _M_ALPHA
					"?VisFWriteImageMagickBody@@YGHAAUSVisImageMagickData@@@Z");
#endif // _M_ALPHA

			m_pfnFinishedWithMagick = (T_PfnVisFinishedWithMagick)
					GetProcAddress(m_hinstImageMagickDll,
#ifdef _M_ALPHA
					// Alpha exports are __cdecl, but X86 exports are __stdcall
					"?VisFinishedWithMagick@@YAXXZ");
#else // _M_ALPHA
					"?VisFinishedWithMagick@@YGXXZ");
#endif // _M_ALPHA

			if ((m_pfnFReadImageMagickHeader == 0)
					|| (m_pfnFReadImageMagickBody == 0)
					|| (m_pfnFWriteImageMagickHeader == 0)
					|| (m_pfnFWriteImageMagickBody == 0)
					|| (m_pfnFinishedWithMagick == 0))
			{
				m_pfnFReadImageMagickHeader = 0;
				m_pfnFReadImageMagickBody = 0;
				m_pfnFWriteImageMagickHeader = 0;
				m_pfnFWriteImageMagickBody = 0;
				m_pfnFinishedWithMagick = 0;

				FreeLibrary(m_hinstImageMagickDll);
				m_hinstImageMagickDll = 0;
			}
		}
	}

	return (m_hinstImageMagickDll != 0);
}




// @cmember
// 
CVisFileHandlerMagick::~CVisFileHandlerMagick(void)
{
	// Tell the image magick DLL to free strings and unload.
	if (m_pfnFinishedWithMagick != 0)
		m_pfnFinishedWithMagick();

	FreeLibrary(m_hinstImageMagickDll);

	m_hinstImageMagickDll = 0;
}



////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      Magick_FileHandler_Init
//  
//  DECLARATION:
//      CVisFileHandler *Magick_FileHandler_Init()
//  
//  RETURN VALUE:
//      File handler for i/o.
//  
//  DESCRIPTION:
//      Return a magick file handler.
//  
////////////////////////////////////////////////////////////////////////////
CVisFileHandler *Magick_FileHandler_Init()
{
    CVisFileHandlerMagick* magic_handler = new CVisFileHandlerMagick;

    return magic_handler;
}


////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisFileHandlerMagick::MatchExtension
//  
//  DECLARATION:
//      BOOL CVisFileHandlerMagick::MatchExtension(char *filetype)
//  
//  RETURN VALUE:
//      1 id the extension found, 0 otherwise.
//  
//  PARAMETERS:
//      filetype - file exctension
//  
//  DESCRIPTION:
//      Matches file extension to the array of supported formats.
//  
////////////////////////////////////////////////////////////////////////////

BOOL CVisFileHandlerMagick::MatchExtension(const char *szFiletype)
{
	if (IsAvailable())
	{
		for (int i = 0; s_rgszMagickSupportedImageFormats[i]; i++)
			if (MatchIgnoringCase(szFiletype, s_rgszMagickSupportedImageFormats[i]))
				return 1;
	}

    return 0;
}


////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisFileHandlerMagick::ExternallyOpenStream
//  
//  DECLARATION:
//      BOOL CVisFileHandlerMagick::ExternallyOpenStream()
//  
//  RETURN VALUE:
//      FALSE  (ImageMagick uses its own stream.)
//  
//  PARAMETERS:
//      none
//  
//  DESCRIPTION:
//      Should the stream be opened outside of this file handler?
//  
////////////////////////////////////////////////////////////////////////////

BOOL CVisFileHandlerMagick::ExternallyOpenStream(void)
{
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisFileHandlerMagick::SupportsPixelType
//  
//  DECLARATION:
//      BOOL CVisFileHandlerMagick::SupportsPixelType(int evispixfmt, int nbands)
//  
//  DESCRIPTION:
//      Returns 1 id the handler can support the given pixel type.
//  
////////////////////////////////////////////////////////////////////////////

BOOL CVisFileHandlerMagick::SupportsPixelType(int evispixfmt, int nbands)
{
    return (IsAvailable())
			&& (evispixfmt == evispixfmtGrayByte || evispixfmt == evispixfmtRGBAByte)
			&& (nbands <= 1);
}


////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisFileHandlerMagick::ReadHeader
//  
//  DECLARATION:
//      BOOL CVisFileHandlerMagick::ReadHeader(SVisFileDescriptor &fd, CVisImageBase &img) 
//  
//  
//  PARAMETERS:
//      fd - file descriptor
//		img - image to be read.
//  
//  DESCRIPTION:
//      Reads image header.
//  
////////////////////////////////////////////////////////////////////////////
BOOL CVisFileHandlerMagick::ReadHeader(SVisFileDescriptor &fd, CVisImageBase &img) 
{
	assert(m_hinstImageMagickDll != 0);

	// Img must be a CVisByteImage or a CVisRGBAByteImage
    assert(img.NBands() <= 1 &&
           (img.PixFmt() == evispixfmtGrayByte || img.PixFmt() == evispixfmtRGBAByte));

	SVisImageMagickData imagemagickdata;
	ZeroMemory(&imagemagickdata, sizeof(SVisImageMagickData));
	imagemagickdata.m_fHasAlpha = (fd.has_alpha_channel > 0) ? true : false;
	if (img.PixFmt() == evispixfmtGrayByte)
		imagemagickdata.m_cbitPixel = 8;
	else
		imagemagickdata.m_cbitPixel = ((imagemagickdata.m_fHasAlpha) ? 32 : 24);
	imagemagickdata.m_filename = fd.filename;
	imagemagickdata.m_filetype = fd.filetype;

	//////////////////////////////////////////////////////////////////

	assert(m_pfnFReadImageMagickHeader != 0);
	BOOL fRet = m_pfnFReadImageMagickHeader(imagemagickdata);

	//////////////////////////////////////////////////////////////////

	// Fill in the fd structure
    fd.rows = imagemagickdata.m_dyImage;
    fd.cols = imagemagickdata.m_dxImage;
    fd.bands = 1;       // gray or color only
    fd.evispixfmt = img.PixFmt();
 	fd.bits_per_pixel = imagemagickdata.m_cbitPixel;
    fd.has_alpha_channel = ((imagemagickdata.m_fHasAlpha) ? 1 : 0);
    fd.n_colors = imagemagickdata.m_cColor;

    // Save the image_info and image structures internally
    fd.internal1 = (void *) imagemagickdata.m_pimageinfo;
    fd.internal2 = (void *) imagemagickdata.m_pimage;

    return fRet;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisFileHandlerMagick::ReadBody
//  
//  DECLARATION:
//      BOOL CVisFileHandlerMagick::ReadBody(SVisFileDescriptor &fd, CVisImageBase &img) 
//  
//  
//  PARAMETERS:
//      fd - file descriptor
//		img - image to be read.
//  
//  DESCRIPTION:
//      Reads image pixels.
//  
////////////////////////////////////////////////////////////////////////////

BOOL CVisFileHandlerMagick::ReadBody(SVisFileDescriptor &fd, CVisImageBase &img)
{
	assert(m_hinstImageMagickDll != 0);

	SVisImageMagickData imagemagickdata;
	ZeroMemory(&imagemagickdata, sizeof(SVisImageMagickData));

	imagemagickdata.m_filename = fd.filename;
	imagemagickdata.m_filetype = fd.filetype;

	if (img.PixFmt() == evispixfmtGrayByte)
	{
		imagemagickdata.m_cbitPixel = 8;
	}
	else if (fd.has_alpha_channel)
	{
		imagemagickdata.m_cbitPixel = 32;
		imagemagickdata.m_fHasAlpha = true;
	}
	else
	{
		imagemagickdata.m_cbitPixel = 24;
		imagemagickdata.m_fHasAlpha = false;
	}
    imagemagickdata.m_cbRow = img.CbRow();
    imagemagickdata.m_pbImageFirst = img.PbPixel(img.StartPoint());

    imagemagickdata.m_dxImage = img.Width();
    imagemagickdata.m_dyImage = img.Height();

    imagemagickdata.m_pimageinfo = (struct _ImageInfo *) fd.internal1;
    imagemagickdata.m_pimage = (struct _Image *) fd.internal2;

	CVisMemBlock memblockColormap(256 * sizeof(RGBQUAD));
	if ((img.UseColorMap()) && (img.PixFmt() == evispixfmtGrayByte))
	{
		imagemagickdata.m_prgbquadColorMap
				= (RGBQUAD *) memblockColormap.PbData();
		imagemagickdata.m_cColor = 256;
	}

	//////////////////////////////////////////////////////////////////

	assert(m_pfnFReadImageMagickBody != 0);
	BOOL fRet = m_pfnFReadImageMagickBody(imagemagickdata);

	//////////////////////////////////////////////////////////////////

	if (imagemagickdata.m_cColor != 0)
		img.SetColorMap(memblockColormap);

    fd.internal1 = (void *) imagemagickdata.m_pimageinfo;
    fd.internal2 = (void *) imagemagickdata.m_pimage;

    return fRet;
}



////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisFileHandlerMagick::WriteHeader
//  
//  DECLARATION:
//      BOOL CVisFileHandlerMagick::WriteHeader(SVisFileDescriptor &fd, CVisImageBase &img) 
//  
//  
//  PARAMETERS:
//      fd - file descriptor
//		img - image to be read.
//  
//  DESCRIPTION:
//      Write image header.
//  
////////////////////////////////////////////////////////////////////////////

BOOL CVisFileHandlerMagick::WriteHeader(SVisFileDescriptor &fd, CVisImageBase &img)
{
	assert(m_hinstImageMagickDll != 0);

	// Img must be a CVisByteImage or a CVisRGBAByteImage
    assert(img.NBands() <= 1 &&
           (img.PixFmt() == evispixfmtGrayByte || img.PixFmt() == evispixfmtRGBAByte));

	SVisImageMagickData imagemagickdata;
	ZeroMemory(&imagemagickdata, sizeof(SVisImageMagickData));

	imagemagickdata.m_filename = fd.filename;
	imagemagickdata.m_filetype = fd.filetype;

	imagemagickdata.m_wQuality = fd.jpeg_quality;

	if (img.PixFmt() == evispixfmtGrayByte)
	{
		imagemagickdata.m_cbitPixel = 8;
		if (img.UseColorMap())
		{
			imagemagickdata.m_cColor = 256;
			imagemagickdata.m_prgbquadColorMap
					= (RGBQUAD *) img.ColorMap().PbData();
		}
	}
	else if (fd.has_alpha_channel)
	{
		imagemagickdata.m_cbitPixel = 32;
		imagemagickdata.m_fHasAlpha = true;
	}
	else
	{
		imagemagickdata.m_cbitPixel = 24;
		imagemagickdata.m_fHasAlpha = false;
	}
    imagemagickdata.m_cbRow = img.CbRow();
    imagemagickdata.m_pbImageFirst = img.PbPixel(img.StartPoint());

    imagemagickdata.m_dxImage = img.Width();
    imagemagickdata.m_dyImage = img.Height();

	//////////////////////////////////////////////////////////////////

	assert(m_pfnFWriteImageMagickHeader != 0);
	BOOL fRet = m_pfnFWriteImageMagickHeader(imagemagickdata);

	//////////////////////////////////////////////////////////////////

	// Fill in the fd structure
    fd.rows = imagemagickdata.m_dyImage;
    fd.cols = imagemagickdata.m_dxImage;
    fd.bands = 1;       // gray or color only
    fd.evispixfmt = img.PixFmt();
 	fd.bits_per_pixel = imagemagickdata.m_cbitPixel;
    fd.has_alpha_channel = ((imagemagickdata.m_fHasAlpha) ? 1 : 0);
    fd.n_colors = imagemagickdata.m_cColor;

    // Save the image_info and image structures internally
    fd.internal1 = (void *) imagemagickdata.m_pimageinfo;
    fd.internal2 = (void *) imagemagickdata.m_pimage;

    return fRet;
}



////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisFileHandlerMagick::WriteBody
//  
//  DECLARATION:
//      BOOL CVisFileHandlerMagick::WriteBody(SVisFileDescriptor &fd, CVisImageBase &img) 
//  
//  
//  PARAMETERS:
//      fd - file descriptor
//		img - image to be read.
//  
//  DESCRIPTION:
//      Write image body.
//  
////////////////////////////////////////////////////////////////////////////
BOOL CVisFileHandlerMagick::WriteBody(SVisFileDescriptor &fd, CVisImageBase &img)
{
	assert(m_hinstImageMagickDll != 0);

	SVisImageMagickData imagemagickdata;
	ZeroMemory(&imagemagickdata, sizeof(SVisImageMagickData));

	imagemagickdata.m_filename = fd.filename;
	imagemagickdata.m_filetype = fd.filetype;

	if (img.PixFmt() == evispixfmtGrayByte)
	{
		imagemagickdata.m_cbitPixel = 8;
		if (img.UseColorMap())
		{
			imagemagickdata.m_cColor = 256;
			imagemagickdata.m_prgbquadColorMap
					= (RGBQUAD *) img.ColorMap().PbData();
		}
	}
	else if (fd.has_alpha_channel)
	{
		imagemagickdata.m_cbitPixel = 32;
		imagemagickdata.m_fHasAlpha = true;
	}
	else
	{
		imagemagickdata.m_cbitPixel = 24;
		imagemagickdata.m_fHasAlpha = false;
	}
    imagemagickdata.m_cbRow = img.CbRow();
    imagemagickdata.m_pbImageFirst = img.PbPixel(img.StartPoint());

    imagemagickdata.m_dxImage = img.Width();
    imagemagickdata.m_dyImage = img.Height();

    imagemagickdata.m_pimageinfo = (struct _ImageInfo *) fd.internal1;
    imagemagickdata.m_pimage = (struct _Image *) fd.internal2;

	//////////////////////////////////////////////////////////////////

	assert(m_pfnFWriteImageMagickBody != 0);
	BOOL fRet = m_pfnFWriteImageMagickBody(imagemagickdata);

	//////////////////////////////////////////////////////////////////

    fd.internal1 = (void *) imagemagickdata.m_pimageinfo;
    fd.internal2 = (void *) imagemagickdata.m_pimage;

	//Run-time exception generation
	if ( fRet )
	{
		throw CVisFileIOError(fd.filename, 
		        "Error while attempting to write image into file", 
				eviserrorWrite, GetClientName(), __FILE__, __LINE__);
	}

    return fRet;
}
