///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISDibFILEIO
//
// @module VisDibFileIO.cpp |
//
// Read and write Dib image files.
// Defines and instantiates subclass of CVisFileHandler class.
//
// <nl>
//
// Copyright © 1999 - 2000 Microsoft Corporation.  All Rights Reserved
// <nl>
//
// @xref <l VisFileIO\.h> <l VisFileIO\.cpp>
//
// <nl>
// @index | VISDibFILEIO
//
///////////////////////////////////////////////////////////////////////////

#include "VisCorePch.h"
#include "VisCoreProj.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

// CVisDib conversion functions don't work with some images whose width
// is not a multiple of four bytes, so we still need to use the old DIB
// file I/O code.
#define VIS_OLD_DIB_CODE

// @class
// File handler to read and write Dib image files.
// @base public | CVisFileHandler
// @xref <c CVisFileHandler>
class VisCoreExport CVisDibFileHandler : public CVisFileHandler {
	BITMAPFILEHEADER m_bmfh;
	BITMAPINFOHEADER m_bmih;
	FILE *m_stream;
	CVisDIB m_dib;
public:
	// @cmember
	// Constructor.
	CVisDibFileHandler() : m_stream(0) {}

	// @cmember
	// Should the stream be opened outside of this file handler?
    virtual BOOL ExternallyOpenStream(void);

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

// @func Returns a handler for Dib image file i/o.
// @rdesc File handler for i/o.
CVisFileHandler *CVisDibFileHandler_Init()
{
    CVisDibFileHandler* Dib_handler = new CVisDibFileHandler;
    return Dib_handler;
}
        

// Should the stream be opened outside of this file handler?
BOOL CVisDibFileHandler::ExternallyOpenStream(void)
{
    return FALSE;
}

// @mfunc Does the handler support images of the specified pixel type with
// the specifed number of bands?
BOOL CVisDibFileHandler::SupportsPixelType(
                                          int evispixfmt,     // @parm pixel type
                                          int nbands)         // @parm number of bands
{
    return (evispixfmt == evispixfmtGrayByte || evispixfmt == evispixfmtRGBAByte) &&
        nbands <= 1;
}

// @mfunc Does the specified extension match the extension of a file type
// supported by this file handler?
BOOL CVisDibFileHandler::MatchExtension(
                                       const char *filetype)  // @parm Image file type
{
    return CVisFileHandler::MatchIgnoringCase(filetype, "bmp");
}

// @mfunc Attempt to read the file header.  
// Return 0 if successful.
BOOL CVisDibFileHandler::ReadHeader(
                                   SVisFileDescriptor &fd,  // @parm File descriptor
                                   CVisImageBase &img)      // @parm Image to be read in.
{
	SetClientName("CVisDibFileHandler::ReadHeader(SVisFileDescriptor&,CVisImageBase&)");

	assert((img.PixFmt() == evispixfmtGrayByte)
			|| (img.PixFmt() == evispixfmtRGBAByte));

	if (fd.filename == 0 || fd.filename[0] == 0 || strcmp(fd.filename, "-") == 0)
		m_stream = stdin;
	else
		m_stream = fopen(fd.filename, "rb");
	if ((m_stream == 0)
#ifdef VIS_OLD_DIB_CODE
		|| (fread(&m_bmfh, 1, sizeof(BITMAPFILEHEADER), m_stream)
					!= sizeof(BITMAPFILEHEADER))
			|| (m_bmfh.bfType != 0x4d42)
			|| (fread(&m_bmih, 1, sizeof(BITMAPINFOHEADER), m_stream)
					!= sizeof(BITMAPINFOHEADER)))
#else // VIS_OLD_DIB_CODE
		|| (!m_dib.ReadHeader(m_stream)))
#endif // VIS_OLD_DIB_CODE
	{
		if (m_stream != stdin && m_stream != 0)
			fclose(m_stream);
		m_stream = 0;
		return 1;
	}

	fd.bands = 1;
	fd.ascii = 0;
	fd.evispixfmt = img.PixFmt();
#ifdef VIS_OLD_DIB_CODE
	fd.bits_per_pixel = m_bmih.biBitCount;
	fd.cols = m_bmih.biWidth;
	fd.rows = m_bmih.biHeight;
#else // VIS_OLD_DIB_CODE
	fd.bits_per_pixel = m_dib.CbitPixel();
	fd.cols = m_dib.Width();
	fd.rows = m_dib.Height();
#endif // VIS_OLD_DIB_CODE
    return 0;
}

// @mfunc Attempt to read the file body.  Return TRUE is successful.
BOOL CVisDibFileHandler::ReadBody(
                                 SVisFileDescriptor &fd,    // @parm File descriptor
                                 CVisImageBase &img)        // @parm Image to be read in.
{
	SetClientName("CVisDibFileHandler::ReadBody(SVisFileDescriptor&,CVisImageBase&)");

	assert(m_stream != 0);

	// Bitmap info used with GetDiBits and colormap used with 8bpp bitmaps
	assert((sizeof(BITMAPINFOHEADER) % 4) == 0);
	DWORD rgdwBitmapInfoAndColormap[sizeof(BITMAPINFOHEADER) / 4 + 256];
	BITMAPINFO& bmi = *((BITMAPINFO *) &(rgdwBitmapInfoAndColormap[0]));
	DWORD *rgdwColormap
			= &(rgdwBitmapInfoAndColormap[sizeof(BITMAPINFOHEADER) / 4]);

#ifdef VIS_OLD_DIB_CODE
	if (((m_bmih.biBitCount != 8) && (m_bmih.biBitCount != 24)
				&& (m_bmih.biBitCount != 32))
			|| (m_bmih.biCompression != BI_RGB))
#else // VIS_OLD_DIB_CODE
	if (((m_dib.CbitPixel() != 8) && (m_dib.CbitPixel() != 24)
				&& (m_dib.CbitPixel() != 32))
			|| (m_dib.Compression() != BI_RGB))
#endif // VIS_OLD_DIB_CODE
	{
		if (m_stream == stdin)
		{
			// Fail
			goto LabelError;
		}
		else
		{
			assert((fd.filename != 0) && (fd.filename[0] != 0)
					&& (strcmp(fd.filename, "-") != 0));

			// Close the stream and use LoadImage
			fclose(m_stream);
			m_stream = 0;

			// Use LoadImage to read the bitmap from the file.
			// UNDONE:  Change to CVisDib::ReadUsingLoadImage.
			// (parameters for filename and bit count:
			//		8-bit gray, 24-bit RGB, or 32-bit RGBA.)
			// m_dib.ReadUsingLoadImage(fd.filename, );
#ifdef VIS_OLD_DIB_CODE

			HBITMAP hbm = (HBITMAP) LoadImage(0, fd.filename, IMAGE_BITMAP,
					0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			if (hbm == 0) 
				goto LabelError;

			HDC hdc = ::GetDC(0);
			::ZeroMemory (&bmi, sizeof(BITMAPINFO));
			bmi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = img.Width();
			bmi.bmiHeader.biHeight = - img.Height();
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biCompression = BI_RGB;

			if (img.PixFmt() == evispixfmtGrayByte)
			{
				bmi.bmiHeader.biBitCount = 8;

				// Initialize colormap with default values
				for (int i = 0; i < 256; ++i)
					rgdwColormap[i] = (0xff000000 | (i << 16) | (i << 8) | i);
			}
			else
			{
				bmi.bmiHeader.biBitCount = 32;
			}

            bool fSucceeded = (::GetDIBits (hdc, hbm, 0, img.Height(),
					img.PbFirstPixelInRow(img.Top()), &bmi, DIB_RGB_COLORS)
							!= 0);

			// Clean up
			::ReleaseDC(0, hdc);
			DeleteObject(hbm);
#else // VIS_OLD_DIB_CODE
			bool fSucceeded = 
				m_dib.ReadUsingLoadImage(fd.filename, img.PbFirstPixelInRow(img.Top()));
#endif // VIS_OLD_DIB_CODE
			if (!fSucceeded)
				goto LabelError;
		}
	}
	else
	{
		// Call CVisDib::ReadBody and then convert to 8-bit gray or 32-bit RGBA.
		// Special code may be needed to make sure that the CVisImage can get
		// the CVisMemBlock used by the CVisDib.
		// Note:  If the image already has a memory block, we should use its
		// memory block.
		// Remember to clear the CVisDib (release its memory references) after
		// we give the CVisMemBlock to the image.
#ifdef VIS_OLD_DIB_CODE
		if (fd.bits_per_pixel == 8)
		{
			// Initialize colormap with default values
			for (int i = 0; i < 256; ++i)
				rgdwColormap[i] = (0xff000000 | (i << 16) | (i << 8) | i);

			// Read colormap from file
			int ibColormapFirst = m_bmih.biSize + sizeof(BITMAPFILEHEADER);
			int ibColormapLim = m_bmfh.bfOffBits;
			if (ibColormapFirst < ibColormapLim)
			{
				bool fSeekAfterReading = false;
				if (ibColormapLim - ibColormapFirst > 256 * sizeof(DWORD))
				{
					ibColormapLim = ibColormapFirst + 256 * sizeof(DWORD);
					fSeekAfterReading = true;
				}
				if (fseek(m_stream, m_bmih.biSize + sizeof(BITMAPFILEHEADER), SEEK_SET))
					goto LabelError;
				if (fread(rgdwColormap, 1, ibColormapLim - ibColormapFirst, m_stream)
							!= ibColormapLim - ibColormapFirst)
					goto LabelError;
				if ((fSeekAfterReading) && (fseek(m_stream, m_bmfh.bfOffBits, SEEK_SET)))
					goto LabelError;

				// Make sure the alpha values are set correctly.
				for (i = 0; i < 256; ++i)
					rgdwColormap[i] |= 0xff000000;
			}

		}

		if (fseek(m_stream, m_bmfh.bfOffBits, SEEK_SET))
			goto LabelError;

		int height = img.Height();
		int width = img.Width();

// Row Padding
		int cbPixel = fd.bits_per_pixel / 8;
		int cbWidth = width * cbPixel;
		int cbRowPad = ((4 - (cbWidth % 4)) % 4);
		DWORD rgdwT[4];

		if (img.PixFmt() == evispixfmtGrayByte)
		{
			if (fd.bits_per_pixel == 8)
			{
				// read image one line at a time into proper location
				BYTE *pbRow = img.PbPixel(img.Left(), img.Bottom()-1);
				for (int r = 0; r < height; r++)
				{
					if (fread(pbRow, cbPixel, width, m_stream) != width)
						goto LabelError;
// Row Padding
					if ((cbRowPad != 0)
							&& (fread(rgdwT, 1, cbRowPad, m_stream) != cbRowPad))
						goto LabelError;
					pbRow -= img.CbRow();
				}

				// UNDONE:  Add colormap to image (if not grayscale)?
			}
			else if ((fd.bits_per_pixel == 24)
					|| (fd.bits_per_pixel == 32))
			{
				// Constants used to get intensity from RGB values.
				const DWORD dwRToY = (((DWORD) (0x2020202 * .299 + 1)) >> 1);
				const DWORD dwGToY = (((DWORD) (0x2020202 * .587 + 1)) >> 1);
				const DWORD dwBToY = (((DWORD) (0x2020202 * .114 + 1)) >> 1);

				bool fHaveAlpha = (fd.bits_per_pixel == 32);

				int cbBuf = width * cbPixel + cbRowPad;
				BYTE *pbBuf = new BYTE[cbBuf];
				BYTE *pbRow = img.PbPixel(img.Left(), img.Bottom()-1);
				for (int r = 0; r < height; r++)
				{
					// Read a row.
					if (fread(pbBuf, 1, cbBuf, m_stream) != cbBuf)
					{
						delete[] pbBuf;
						goto LabelError;
					}

					// Convert RGB values to grayscale intensity values.
					const BYTE *pbSrc = pbBuf;
					for (int c = 0; c < width; ++c)
					{
						DWORD dwT = *pbSrc * dwBToY;
						++ pbSrc;
						dwT += *pbSrc * dwGToY;
						++ pbSrc;
						dwT += *pbSrc * dwRToY;
						++ pbSrc;
						if (fHaveAlpha)
							++ pbSrc;
						pbRow[c] = (BYTE) (dwT >> 24);
					}
					pbRow -= img.CbRow();
				}
				delete[] pbBuf;
			}
		}
		else
		{
			assert(img.PixFmt() == evispixfmtRGBAByte);
			if (fd.bits_per_pixel == 8)
			{
				// read and unpack one line at a time
				BYTE *pbRow = img.PbPixel(img.Left(), img.Bottom()-1);
				for (int r = 0; r < height; r++)
				{
					// Read the row
					if (fread(pbRow, cbPixel, width, m_stream) != width)
						goto LabelError;
// Row Padding
					if ((cbRowPad != 0)
							&& (fread(rgdwT, 1, cbRowPad, m_stream) != cbRowPad))
						goto LabelError;

					// Unpack the row
					const BYTE *pbSrc = pbRow + width - 1;
					for (int c = width-1; c >= 0; c--)
					{
						((DWORD *) pbRow)[c] = rgdwColormap[*pbSrc];
						-- pbSrc;
					}
					pbRow -= img.CbRow();
				}
			}
			else if (fd.bits_per_pixel == 24)
			{
				// read and unpack one line at a time
				BYTE *pbRow = img.PbPixel(img.Left(), img.Bottom()-1);
				for (int r = 0; r < height; r++)
				{
					// Read the row
					if (fread(pbRow, cbPixel, width, m_stream) != width)
						goto LabelError;
// Row Padding
					if ((cbRowPad != 0)
							&& (fread(rgdwT, 1, cbRowPad, m_stream) != cbRowPad))
						goto LabelError;

					// Unpack the row
					const BYTE *pbSrc = pbRow + width * 3 - 1;
					BYTE *pbDest = pbRow + width * 4 - 1;
					for (int c = width-1; c >= 0; c--)
					{
						*pbDest = (BYTE) 0xff;
						-- pbDest;
						*pbDest = *pbSrc;
						-- pbDest;
						-- pbSrc;
						*pbDest = *pbSrc;
						-- pbDest;
						-- pbSrc;
						*pbDest = *pbSrc;
						-- pbDest;
						-- pbSrc;
					}
					pbRow -= img.CbRow();
				}
			}
			else if (fd.bits_per_pixel == 32)
			{
				// read image one line at a time into proper location
				BYTE *pbRow = img.PbPixel(img.Left(), img.Bottom()-1);
				for (int r = 0; r < height; r++)
				{
					if (fread(pbRow, cbPixel, width, m_stream) != width)
						goto LabelError;
					// No Row Padding needed with 32bpp
					pbRow -= img.CbRow();
				}
			}
		}
#else // VIS_OLD_DIB_CODE
		assert(img.Width() == m_dib.Width());
		assert(img.Height() == abs(m_dib.Height()));
		if((img.MemoryShape() != img.Shape()) || 
			(m_dib.Height() > 0) || 
			(img.CbPixel() * 8 != m_dib.CbitPixel()) ||
			(img.PixFmt() != evispixfmtRGBAByte) ||
			(img.PixFmt() != evispixfmtGrayByte))
		{
			m_dib.ReadBody(m_stream);
			VisCopyDibPixelsToImage(m_dib, img);
		}
		else
		{
			VisAliasImageInDib(img, m_dib);
			m_dib.ReadBody(m_stream);
		}
		m_dib.Invalidate();

#endif // VIS_OLD_DIB_CODE
	}

#ifdef SAVE
	if (fInvert)
	{
		int cbBuf;
		if (fd.bits_per_pixel == 8)
			cbBuf = width;
		else
			cbBuf = width * 4;
		BYTE *pbBuf = new BYTE[cbBuf];
		BYTE *pbTop = (BYTE *)img.PbPixel(img.Left(), img.Top());
		BYTE *pbBottom = (BYTE *)img.PbPixel(img.Left(), img.Bottom() - 1);

		int height2 = height / 2;
		for (int i = 0; i < height2; i++) {
			// swap rows pointed by p and p2
			memcpy(pbBuf, pbTop, cbBuf);
			memcpy(pbTop, pbBottom, cbBuf);
			memcpy(pbBottom, pbBuf, cbBuf);
			p += img.CbRow();
			p2 -= img.CbRow();
		}
		delete [] pbBuf;
	}
#endif // SAVE

	// Close the stream.
	if ((m_stream != 0) && (m_stream != stdin))
		fclose(m_stream);
	m_stream = 0;

	return 0;

LabelError:
	if ((m_stream != 0) && (m_stream != stdin))
		fclose(m_stream);
	m_stream = 0;
	return 1;
}

// @mfunc Attempt to write the file header.  Return 0 if successful.
BOOL CVisDibFileHandler::WriteHeader(
                                    SVisFileDescriptor &fd,     // @parm File descriptor
                                    CVisImageBase &img)         // @parm Image to be written out.
{
	SetClientName("CVisDibFileHandler::WriteHeader(SVisFileDescriptor&,CVisImageBase&)");


	assert((img.PixFmt() == evispixfmtGrayByte)
			|| (img.PixFmt() == evispixfmtRGBAByte));

	if (fd.filename == 0 || fd.filename[0] == 0 || strcmp(fd.filename, "-") == 0)
		m_stream = stdout;
	else
		m_stream = fopen(fd.filename, "wb");
	if (m_stream == 0)
		return 1;

//	BITMAPFILEHEADER m_bmfh;
	m_bmfh.bfType = 0x4d42;  // 'BM'
	int numColorTableEntries = ((img.PixFmt() == evispixfmtGrayByte) ? 256 : 0);
	m_bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
		numColorTableEntries * sizeof(RGBQUAD);
	m_bmfh.bfReserved1 = m_bmfh.bfReserved2 = 0;
	m_bmfh.bfOffBits = m_bmfh.bfSize;

//	BITMAPINFOHEADER m_bmih;
	m_bmih.biSize = sizeof(BITMAPINFOHEADER);
	m_bmih.biWidth = img.Width();
	m_bmih.biHeight = img.Height();
	m_bmih.biPlanes = 1;
	if (img.PixFmt() == evispixfmtGrayByte)
		m_bmih.biBitCount = 8;
	else if (img.PixFmt() == evispixfmtRGBAByte)
		if (fd.has_alpha_channel)
			m_bmih.biBitCount = 32;
		else
			m_bmih.biBitCount = 24;
	fd.bits_per_pixel = m_bmih.biBitCount;
	fd.evispixfmt = img.PixFmt();
// Row Padding
	if (CVisImageBase::DoesBMPFileWidthIncludePadding())
	{
		while (((m_bmih.biBitCount * m_bmih.biWidth) % 32) != 0)
			++ m_bmih.biWidth;
	}
	m_bmih.biCompression = BI_RGB;
	m_bmih.biSizeImage = 0;	// no compression used
	m_bmih.biXPelsPerMeter = 0;
	m_bmih.biYPelsPerMeter = 0;
	m_bmih.biClrUsed = 0;
	m_bmih.biClrImportant = 0;
	if ((fwrite(&m_bmfh, 1, sizeof(BITMAPFILEHEADER), m_stream)
				!= sizeof(BITMAPFILEHEADER))
			|| (fwrite(&m_bmih, 1, sizeof(BITMAPINFOHEADER), m_stream)
				!= sizeof(BITMAPINFOHEADER)))
	{
		if (m_stream != stdout)
			fclose(m_stream);
		m_stream = 0;
		return 1;
	}

	if (img.PixFmt() & evispixfmtGray) {
		// write a linear color table
		for (int i = 0; i < 256; i++) {
			RGBQUAD q;
			q.rgbBlue = q.rgbGreen = q.rgbRed = (BYTE)i;
			q.rgbReserved = 0;
			if (fwrite(&q, sizeof(RGBQUAD), 1, m_stream) != 1)
			{
				if (m_stream != stdout)
					fclose(m_stream);
				m_stream = 0;
				return 1;
			}
		}
	}
	return 0;
}


// @mfunc Attempt to write the file body.  Return 0 if successful.
BOOL CVisDibFileHandler::WriteBody(
                                  SVisFileDescriptor &fd,   // @parm File descriptor
                                  CVisImageBase &img)       // @parm Image to be written out.
{
	SetClientName("CVisDibFileHandler::WriteBody(SVisFileDescriptor&,CVisImageBase&)");

	assert(m_stream != 0);
    int height = img.Height();
	int cpixelRow = img.CbRow() / img.CbPixel();
	int width = img.Width();

	int cbPixel = fd.bits_per_pixel / 8;
	int cbWidth = width * cbPixel;
// Row Padding
	int cbRowPad = 0;
	while (((cbWidth + cbRowPad) % 4) != 0)
		cbRowPad += cbPixel;
	DWORD rgdwZero[4] = { 0, 0, 0, 0 };
	if (img.PixFmt() == evispixfmtGrayByte ||
		img.PixFmt() == evispixfmtGrayChar) {
		CVisBytePixel *p = img.PbPixel(img.Left(), img.Bottom()-1);
	    for (int r = 0; r < height; r++) {
			if (fwrite(p, 1, width, m_stream) != width)
				goto LabelError;
// Row Padding
			if ((cbRowPad != 0)
					&& (fwrite(rgdwZero, 1, cbRowPad, m_stream) != cbRowPad))
				goto LabelError;
			p -= cpixelRow;
		}
	} else if (img.PixFmt() == evispixfmtRGBAByte ||
			   img.PixFmt() == evispixfmtRGBAChar ||
			   img.PixFmt() == evispixfmtYUVAByte ||
			   img.PixFmt() == evispixfmtYUVAChar) {
		if (fd.has_alpha_channel) {
			CVisRGBABytePixel *p = (CVisRGBABytePixel *)img.PbPixel(img.Left(),
						img.Bottom()-1);
			for (int r = 0; r < height; r++) {
				if (fwrite(p, sizeof(CVisRGBABytePixel), width, m_stream) != width)
					goto LabelError;
				p -= cpixelRow;
			}
		} else {
			CVisRGBABytePixel *pRow = (CVisRGBABytePixel *)img.PbPixel(img.Left(),
						img.Bottom()-1);
			for (int r = 0; r < height; r++) {
				CVisRGBABytePixel *pCur = pRow;
				for (int c = 0; c < width; c++, pCur++) {
					if (fwrite(pCur, 1, 3, m_stream) != 3)
						goto LabelError;
				}
// Row Padding
				if ((cbRowPad != 0)
						&& (fwrite(rgdwZero, 1, cbRowPad, m_stream) != cbRowPad))
					goto LabelError;
				pRow -= cpixelRow;
			}
		}
	}
	// Close the stream.
	if (m_stream != stdout)
		fclose(m_stream);
	m_stream = 0;

	return 0;

LabelError:
	if (m_stream != stdout)
		fclose(m_stream);
	m_stream = 0;
	return 1;
}

