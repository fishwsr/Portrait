///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISDib
//
// @module VisDib.cpp |
//
// Operations on Dib images.
//
//
// @index | VISDib
//
// Copyright © 1999-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#pragma once


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward Declarations
class CVisImageBase;

class VisCoreExport CVisDIB {
public:
    
	CVisDIB(const BITMAPINFOHEADER *pbmih = 0, int cbBMI = 0, void *pvData = 0, 
			VisMemBlockCallback pfnCallback = 0, void *pvUser = 0);
	
	CVisDIB(const BITMAPINFOHEADER *pbmih, int cbBMI, CVisMemBlock& refmemblock);

	~CVisDIB(void);


	const BITMAPINFOHEADER& BitmapInfoHeader(void) const;

	long Width(void) const;

	long Height(void) const;

	DWORD Compression(void) const;

	int CbitPixel(void) const;

	void Invalidate(void);

	bool IsValid(void);


	bool IsGray8bit(void) const;

	const RGBQUAD* ColorMap(void) const;

	RGBQUAD Color(int iColor) const;


	const CVisMemBlock& MemBlockPixels(void) const;

	const void *PvPixels(void) const;

	void *PvPixels(void);


	// Change to 32-bpp, 24-bpp, or 8-bpp (gray) RGB.
	// (This shouldn't change the bitmap if the current format is
	// as specified.)
	// Returns true if the resulting bitmap is in the RGB format
	// sprecified.
	//bool ChangeToRGB();

	bool ChangeToRGB(int cbitPixel, bool fTopDown);

	void WriteToFile(const char* szFile) const;

	void ReadFromFile(const char* szFile);


	void ReflectDIB(void);


	bool Decompress(int cbitRGB = 32);

	bool Decompress(const BITMAPINFOHEADER *pbmih, int cbBMI);

	bool Decompress(HIC hicDecompressor, const BITMAPINFOHEADER *pbmih, int cbBMI);

	HIC HDecompressor(int cbitRGB = 32) const;

	HIC HDecompressor(const BITMAPINFOHEADER *pbmih) const;

	static void CloseHDecompressor(HIC hicDecompressor);

	void ChangeToCopy();

	// @access Protected Members
protected:

	friend bool VisCopyDibPixelsToImage(const CVisDIB& refdib, CVisImageBase& refimagebase);

	friend class CVisDibFileHandler;

	FILE* ReadHeader(const char *szFilename);

	bool ReadHeader(FILE *pfile);

	bool ReadBody(FILE *pfile);

	bool ReadUsingLoadImage(const char *szFilename, void* pvPixelAddress);

	FILE* WriteHeader(const char *szFilename);

	bool WriteHeader(FILE *pfile);

	bool WriteBody(FILE *pfile);

	void CopyToBuffer(void* pvFirst, int cbRowDest = 0);

	void CopyFromBuffer(const void* pvFirst, int cbRowSrc = 0);
	
	bool IsBMIGray8bit(bool fZeroClr = false) const;
	
	CVisMemBlock CreateHeaderMemBlock(const void* pvDIBHeader, int cbBMI);

	CVisMemBlock BMCHToMemBlock(const void* pvDIBHeader, int cbBMI);

	CVisMemBlock BMIHToMemBlock(const void* pvDIBHeader, int cbBMI);

	BITMAPINFOHEADER& BMIH(void);

	void ConvertRGBBitmap(int cbitPixel, bool fReflect);

	void ConvertRGB4To32(bool fReflect = false);

	void ConvertRGB8To32(bool fReflect = false);

	void ConvertRGB16To32(bool fReflect = false);

	void ConvertRGB24To32(bool fReflect = false);

	void ConvertRGB4To24(bool fReflect = false);

	void ConvertRGB8To24(bool fReflect = false);

	void ConvertRGB16To24(bool fReflect = false);

	void ConvertRGB32To24(bool fReflect = false);

	void ConvertRGB4To16(bool fReflect = false);

	void ConvertRGB8To16(bool fReflect = false);

	void ConvertRGB24To16(bool fReflect = false);

	void ConvertRGB32To16(bool fReflect = false);

	void ConvertRGB4To8(bool fReflect = false);

	void ConvertRGB16To8(bool fReflect = false);

	void ConvertRGB24To8(bool fReflect = false);

	void ConvertRGB32To8(bool fReflect = false);

	CVisMemBlock ConvertHeaderMemblock(bool fReflect, int cbitPixel = 32) const;

	void FillColors(DWORD dwColors[], int cColors) const;

	static DWORD GrayColorFromI(int iColor);

// @access Private Members
private:
	
	static int CbPixelsFromBMIH(const BITMAPINFOHEADER *pbmih);

	CVisMemBlock m_memblockPixels;
	
	CVisMemBlock m_memblockHeader;

	bool m_fGray8bit;
};


// Copy will always copy pixels, alias will only copy pointers to pixels.
// The initial implementations only work with GrayByte and RGBAByte images.
inline bool VisAliasImageInDib(const CVisImageBase& refimagebase, CVisDIB& refdib)
{
	assert(refimagebase.MemoryShape() == refimagebase.Shape());
	bool fRet = false;

	// We can only alias 8-bit gray and 32-bit color images.
	if ((refimagebase.PixFmt() == evispixfmtRGBAByte)
			|| (refimagebase.PixFmt() == evispixfmtGrayByte))
	{
		BITMAPINFOHEADER bmih;
		
		refimagebase.FillBitmapInfoHeader(&bmih);
		CVisMemBlock memblockAlias = refimagebase.MemBlock();
		CVisDIB dibAlias(&bmih, bmih.biSize, memblockAlias);
		refdib = dibAlias;
		
		fRet = true;
	}

	return fRet;
}

inline bool VisCopyImagePixelsToDib(const CVisImageBase& refimagebase, CVisDIB& refdib)
{
	bool fRet = false;

	if (VisAliasImageInDib(refimagebase, refdib))
	{
		refdib.ChangeToCopy();
		fRet = true;
	}

	// LATER:  Change other image types to 32-bit RGBA and then copy?
	assert(fRet);

	return fRet;
}
inline bool VisAliasDibInImage(const CVisDIB& refdib, CVisImageBase& refimagebase)
{
	bool fRet = false;

	// We can only alias 8-bit gray and 32-bit color images.
	if (((refdib.Compression() == BI_RGB) && (refdib.CbitPixel() == 32)
				&& (refimagebase.PixFmt() == evispixfmtRGBAByte))
			|| ((refdib.IsGray8bit())
				&& (refimagebase.PixFmt() == evispixfmtGrayByte)))
	{
		// This is a tricky API.
		// (Most VisSDK users should not need use SetMemBlock.)
		refimagebase.SetMemBlock(refdib.MemBlockPixels(),
				refdib.Width(), refdib.Height());

		fRet = true;
	}

	return fRet;
}


inline bool VisCopyDibPixelsToImage(const CVisDIB& refdib, CVisImageBase& refimagebase)
{
	bool fRet = false;

	CVisDIB dibCopy = refdib;
	if (refimagebase.PixFmt() == evispixfmtRGBAByte)
	{
		// Get a 32-bit RGBA DIB from refdib
		// Allocate memory in the image if needed
		// Copy line-by-line
		dibCopy.ChangeToRGB(32, (dibCopy.Height() > 0));
		dibCopy.CopyToBuffer(refimagebase.PbFirstPixelInRow(refimagebase.Top()), refimagebase.CbRow());
		fRet = true;
	}
	else if (refimagebase.PixFmt() == evispixfmtGrayByte)
	{
		// Get an 8-bit gray DIB from refdib
		// Allocate memory in the image if needed
		// Copy line-by-line
		dibCopy.ChangeToRGB(8, (dibCopy.Height() > 0));
		dibCopy.CopyToBuffer(refimagebase.PbFirstPixelInRow(refimagebase.Top()), refimagebase.CbRow());
		fRet = true;
	}

	return fRet;
}


// Only allow Cbit == 24 or Cbit == 32
//void VisConvertRGB4bitToRGBCbit(const void *pvSrc, void *pvDest,
//		int cbitDest, int dx, int dy, bool fReflect);
//


#include "VisDib.inl"

