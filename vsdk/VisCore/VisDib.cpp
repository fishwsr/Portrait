///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISMEMBLOCK
//
// @module VisDib.cpp |
//
// Copyright © 1999-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"

#include <assert.h>
#include <stdio.h>      // needed by PrintStatistics

//#include "VisDib.h"
//#include "VisDib.inl"
#include "Vfw.h"


#define VisImSrcExport  
#include "..\VisImSrc\VisConvertRGB.h"
#include "..\VisImSrc\VisConvertRGB.inl"
#include "..\VisImSrc\VisConvertCbitToCbit.inl"


CVisDIB::CVisDIB(const BITMAPINFOHEADER *pbmih, int cbBMI, CVisMemBlock& refmemblock)
{
	m_memblockHeader = CreateHeaderMemBlock(pbmih, cbBMI);
	m_fGray8bit = IsBMIGray8bit();
	m_memblockPixels = refmemblock;
}

CVisDIB::CVisDIB(const BITMAPINFOHEADER *pbmih, int cbBMI, void *pvData, 
			VisMemBlockCallback pfnCallback, void *pvUser)
{
	if(pbmih != NULL)
	{
		m_memblockHeader = CreateHeaderMemBlock(pbmih, cbBMI);
		m_fGray8bit = IsBMIGray8bit();
		if(pvData != NULL)
		{
			CVisMemBlock memblockPixels(pvData, CbPixelsFromBMIH(&BitmapInfoHeader()), pfnCallback, pvUser);
			m_memblockPixels = memblockPixels;
		}
		else
		{
			CVisMemBlock memblockPixels(CbPixelsFromBMIH(&BitmapInfoHeader()));
			m_memblockPixels = memblockPixels;
		}
	}
}

CVisMemBlock CVisDIB::CreateHeaderMemBlock(const void* pvDIBHeader, int cbBMI)
{
	CVisMemBlock memblockRet;

	assert(pvDIBHeader);
	const BITMAPINFOHEADER *pbmih = (const BITMAPINFOHEADER *) pvDIBHeader;
	if(cbBMI == 0)
		cbBMI = pbmih->biSize;
	if(pbmih->biSize < sizeof(BITMAPINFOHEADER))
	{
		memblockRet = BMCHToMemBlock(pvDIBHeader, cbBMI);
	}
	else
	{
		memblockRet = BMIHToMemBlock(pvDIBHeader, cbBMI);
	}
	return memblockRet;
}

CVisMemBlock CVisDIB::BMCHToMemBlock(const void* pvDIBHeader, int cbBMI)
{
	CVisMemBlock memblockRet;
	const BITMAPCOREHEADER* pbmch = (const BITMAPCOREHEADER *) pvDIBHeader;

	BITMAPINFOHEADER bmih;
	bmih.biWidth = pbmch->bcWidth;
	bmih.biHeight = pbmch->bcHeight;
	bmih.biPlanes = pbmch->bcPlanes;
	bmih.biBitCount = pbmch->bcBitCount;
 	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = bmih.biClrImportant = 0;

	int cColors = (cbBMI - pbmch->bcSize) / 3;
	if(bmih.biBitCount == 8 && cColors == 0)
	{
		// We assume gray colors for 8-bit RGB images without a colormap.
		m_fGray8bit = true;
		CVisMemBlock memblockHeader(sizeof(BITMAPINFOHEADER) + 1024);
		memcpy(memblockHeader.PbData(), &bmih, sizeof(BITMAPINFOHEADER));
		DWORD* pdwColor = (DWORD *) (memblockHeader.PbData() + sizeof(BITMAPINFOHEADER));
		for(int iColor = 0; iColor < 256; iColor++)
		{
			pdwColor[iColor] = GrayColorFromI(iColor);
		}
		memblockRet = memblockHeader;
	}
	else if(cColors != 0)
	{
		CVisMemBlock memblockHeader(sizeof(BITMAPINFOHEADER) + 4 * cColors);
		memcpy(memblockHeader.PbData(), &bmih, sizeof(BITMAPINFOHEADER));
		
		DWORD* pdwRGBA = (DWORD *)(memblockHeader.PbData() + sizeof(BITMAPINFOHEADER));
		BYTE* pbRGB = (BYTE *)pvDIBHeader + sizeof(BITMAPCOREHEADER);

		for(int iColor = 0; iColor < cColors; iColor++)
		{
			*pdwRGBA = *pbRGB++;
			*pdwRGBA = ((*pdwRGBA << 8) | *pbRGB++);
			*pdwRGBA = ((*pdwRGBA << 8) | *pbRGB++);
			*pdwRGBA |= 0xff000000;
			pdwRGBA++;
		}
		memblockRet = memblockHeader;
	}

	return memblockRet;
}

CVisMemBlock CVisDIB::BMIHToMemBlock(const void* pvDIBHeader, int cbBMI)
{
	CVisMemBlock memblockRet;
	const BITMAPINFOHEADER* pbmih = (const BITMAPINFOHEADER *) pvDIBHeader;
	int cColorSize = cbBMI - pbmih->biSize;
	if(pbmih->biBitCount == 8 && cColorSize == 0)
	{
		// We assume gray colors for 8-bit RGB images without a colormap.
		m_fGray8bit = true;
		CVisMemBlock memblockHeader(pbmih->biSize + 1024);
		memcpy(memblockHeader.PbData(), pvDIBHeader, memblockHeader.CbData());
		DWORD* pdwColor = (DWORD *) (memblockHeader.PbData() + pbmih->biSize);
		for(int iColor = 0; iColor < 256; iColor++)
		{
			pdwColor[iColor] = GrayColorFromI(iColor);
		}
		memblockRet = memblockHeader;
	}
	else
	{
		CVisMemBlock memblockHeader(cbBMI);
		memcpy(memblockHeader.PbData(), pvDIBHeader, memblockHeader.CbData());
		memblockRet = memblockHeader;
	}

	return memblockRet;
}

int CVisDIB::CbPixelsFromBMIH(const BITMAPINFOHEADER *pbmih)
{
	assert(pbmih);
	int cbPixels = pbmih->biSizeImage;
	if(pbmih->biCompression == BI_RGB)
	{
		cbPixels = (pbmih->biWidth * pbmih->biHeight * 
					  pbmih->biBitCount * pbmih->biPlanes)/8;
		if (cbPixels < 0)
			cbPixels = - cbPixels;
	}
	return cbPixels;
}

bool CVisDIB::ChangeToRGB(int cbitPixel, bool fTopDown)
{
	// if not RGB, change compression to RGB
	// if decompression fails, return false
	// otherwise, make sure we have a RGB bitmap with
	// the right cbit and fTopDown
	bool fChange = true;
	if(BitmapInfoHeader().biCompression != BI_RGB)
	{
		fChange = Decompress(cbitPixel);
	}

	if(fChange)
	{
		bool fNeedToFlip = (((fTopDown) && (Height() > 0))
				|| ((!fTopDown) && (Height() < 0)));
		if	(cbitPixel != CbitPixel())
			ConvertRGBBitmap(cbitPixel, fNeedToFlip); 
		else if (fNeedToFlip)
			ReflectDIB();
	}

	return fChange;
}

HIC CVisDIB::HDecompressor(int cbitRGB) const
{
	const BITMAPINFOHEADER& bmihCur = BitmapInfoHeader();
	
	CVisMemBlock memblockHeader(bmihCur.biSize);
	memcpy(memblockHeader.PbData(), &bmihCur, memblockHeader.CbData());

	BITMAPINFOHEADER* pbmih = (BITMAPINFOHEADER *)(memblockHeader.PbData());

	pbmih->biHeight =  -pbmih->biHeight;
	pbmih->biBitCount = cbitRGB;
	pbmih->biCompression = BI_RGB;
	pbmih->biSizeImage = 0;

	return HDecompressor(pbmih);

}

bool CVisDIB::Decompress(int cbitRGB)
{
	const BITMAPINFOHEADER& bmihCur = BitmapInfoHeader();
	
	CVisMemBlock memblockHeader(bmihCur.biSize);
	memcpy(memblockHeader.PbData(), &bmihCur, memblockHeader.CbData());

	BITMAPINFOHEADER* pbmih = (BITMAPINFOHEADER *)(memblockHeader.PbData());

	pbmih->biHeight =  -pbmih->biHeight;
	pbmih->biBitCount = cbitRGB;
	pbmih->biCompression = BI_RGB;
	pbmih->biSizeImage = 0;

	return Decompress(pbmih, memblockHeader.CbData());
}

bool CVisDIB::Decompress(const BITMAPINFOHEADER *pbmih, int cbBMI)
{
	HIC hicDecompressor = HDecompressor(pbmih);
	
	bool fDecompress = Decompress(hicDecompressor, pbmih, pbmih->biSize);

	CloseHDecompressor(hicDecompressor);

	return fDecompress;
}

// UNDONE:  Document that gray colormap is assumed with 8-bit RGB.
bool CVisDIB::Decompress(HIC hicDecompressor, const BITMAPINFOHEADER *pbmih, int cbBMI)
{

	bool fDecompress = false;
	// The IC Decompress functions need a non-const BITMAPINFOHEADER* as the argument
	// so BMIH() is used instead of BitmapInfoHeader()
	BITMAPINFOHEADER& refbmihCur = BMIH();
	
	CVisMemBlock memblockBMIOut = CreateHeaderMemBlock(pbmih, cbBMI);
	DWORD dwres = ICDecompressQuery(hicDecompressor, &refbmihCur, memblockBMIOut.PbData());

	assert(dwres == ICERR_OK);

	int nSize = CbPixelsFromBMIH((BITMAPINFOHEADER *) (memblockBMIOut.PbData()));

	CVisMemBlock memblockOutput(nSize);

	if(ICDecompressBegin(hicDecompressor, &refbmihCur, memblockBMIOut.PbData()) == ICERR_OK)
	{
		DWORD dwdecomp = ICDecompress(hicDecompressor, 0, 
										&refbmihCur, PvPixels(), 
										(BITMAPINFOHEADER *)(memblockBMIOut.PbData()), 
										memblockOutput.PbData());
		if(dwdecomp == ICERR_OK)
			fDecompress = true;

		ICDecompressEnd(hicDecompressor);
		
		m_memblockHeader = memblockBMIOut;	
		m_memblockPixels = memblockOutput;

	
	}
	
	return fDecompress;
}

FILE* CVisDIB::WriteHeader(const char *szFilename)
{
	FILE *pfile = fopen(szFilename, "wb");
	
	WriteHeader(pfile);
	return pfile;
}

bool CVisDIB::WriteHeader(FILE *pfile)
{
	bool fStatus = false;
	if(pfile != 0)
	{
		BITMAPFILEHEADER bmfh;
		bmfh.bfType = 0x4d42;  // 'BM'
		int numColorTableEntries = (IsGray8bit() ? 256 : 0 );

		// UNDONE:  Colormap for 8-bit gray
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + m_memblockHeader.CbData() + m_memblockPixels.CbData();
		bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + m_memblockHeader.CbData();

		if (fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), pfile) == sizeof(BITMAPFILEHEADER))
		{
			fStatus = (fwrite(m_memblockHeader.PbData(), 1, m_memblockHeader.CbData(), pfile) ==
									m_memblockHeader.CbData());	
		}
	}
	return fStatus;
}

bool CVisDIB::WriteBody(FILE *pfile)
{
	bool fStatus = false;
	if(pfile != 0)
	{
		if(MemBlockPixels().CbData() != 0)
		{
			fseek(pfile, 0, SEEK_END);
			fStatus = (fwrite(m_memblockPixels.PbData(), 1, m_memblockPixels.CbData(), pfile) == 
								m_memblockPixels.CbData());
		}
	}

	return fStatus;
}


void CVisDIB::WriteToFile(const char* szFile) const
{
	FILE *pfile = fopen(szFile, "wb");
	if(pfile == 0)
		return;

	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	int numColorTableEntries = (IsGray8bit() ? 256 : 0 );

	// UNDONE:  Colormap for 8-bit gray
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + m_memblockHeader.CbData() + m_memblockPixels.CbData();
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + m_memblockHeader.CbData();
	
	if (fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), pfile) == sizeof(BITMAPFILEHEADER))
	{
		fwrite(m_memblockHeader.PbData(), 1, m_memblockHeader.CbData(), pfile);
		fwrite(m_memblockPixels.PbData(), 1, m_memblockPixels.CbData(), pfile);
	}

	fclose(pfile);
}

FILE* CVisDIB::ReadHeader(const char *szFilename)
{
	FILE *pfile = fopen(szFilename, "rb");
	
	ReadHeader(pfile);
	return pfile;
}

bool CVisDIB::ReadHeader(FILE *pfile)
{
	BITMAPFILEHEADER bmfh;
	bool fStatus = false;
	
	if( (pfile != 0) &&
		(fread(&bmfh, 1, sizeof(BITMAPFILEHEADER), pfile) == sizeof(BITMAPFILEHEADER)))
	{
		CVisMemBlock memblockHeader(bmfh.bfOffBits - sizeof(BITMAPFILEHEADER));
		int nHeader = fread(memblockHeader.PbData(), 1, memblockHeader.CbData(), pfile);

		assert(nHeader == memblockHeader.CbData());

		int cHeaderSize = ((const BITMAPINFOHEADER *)(memblockHeader.PbData()))->biSize;

		if(cHeaderSize < sizeof(BITMAPINFOHEADER))
		{
			memblockHeader = CreateHeaderMemBlock((const BITMAPINFOHEADER *)(memblockHeader.PbData()), 
													memblockHeader.CbData());
		}

		m_memblockHeader = memblockHeader;
		fStatus = true;
	}
	return fStatus;
}

bool CVisDIB::ReadBody(FILE *pfile)
{
	bool fStatus = false;
	if(pfile != 0)
	{
		if(MemBlockPixels().CbData() == 0)
		{
			CVisMemBlock memblockPixels(CbPixelsFromBMIH(&BitmapInfoHeader()));	
			m_memblockPixels = memblockPixels;
		}
		fStatus = (fread(m_memblockPixels.PbData(), 1, m_memblockPixels.CbData(), pfile) ==
										m_memblockPixels.CbData());
	}

	return fStatus;
}

void CVisDIB::ReadFromFile(const char* szFile)
{
	FILE *pfile = fopen(szFile, "rb");
	if(pfile == 0)
		return;

	BITMAPFILEHEADER bmfh;
	
	if(fread(&bmfh, 1, sizeof(BITMAPFILEHEADER), pfile) == sizeof(BITMAPFILEHEADER))
	{
		CVisMemBlock memblockHeader(bmfh.bfOffBits - sizeof(BITMAPFILEHEADER));
		int nBytesRead = fread(memblockHeader.PbData(), 1, memblockHeader.CbData(), pfile);
		assert(nBytesRead == memblockHeader.CbData());

		int cHeaderSize = ((const BITMAPINFOHEADER *)(memblockHeader.PbData()))->biSize;
		if(cHeaderSize < sizeof(BITMAPINFOHEADER))
		{
			memblockHeader = CreateHeaderMemBlock((const BITMAPINFOHEADER *)(memblockHeader.PbData()), 
													memblockHeader.CbData());
		}

		m_memblockHeader = memblockHeader;

		CVisMemBlock memblockPixels(CbPixelsFromBMIH(&BitmapInfoHeader()));	
		nBytesRead = fread(memblockPixels.PbData(), 1, memblockPixels.CbData(), pfile);
		assert(nBytesRead == memblockPixels.CbData());

		m_memblockPixels = memblockPixels;
	}

	fclose(pfile);
}

void CVisDIB::ConvertRGBBitmap(int cbitPixel, bool fReflect)
{
	
	// Conversion to 8 bit Gray and 24 bit should
	// be provided

	// Converting from 1, 4 and 8 bits needs color table
	switch(cbitPixel) // Destination bpp
	{
		case 32:
		{
			switch(CbitPixel()) // Source bpp
			{
				case 4:
				{
					ConvertRGB4To32(fReflect);
					break;
				}
				case 8:
				{
					ConvertRGB8To32(fReflect);
					break;
				}
				case 16:
				{
					ConvertRGB16To32(fReflect);
					break;
				}
				case 24:
				{
					ConvertRGB24To32(fReflect);
					break;
				}
				default:
				{
					break;
				}

			} // 	ENDOF switch
			break;
		}
		case 24:
		{
			switch(CbitPixel()) // Source bpp
			{
				case 4:
				{
					ConvertRGB4To24(fReflect);
					break;
				}
				case 8:
				{
					ConvertRGB8To24(fReflect);
					break;
				}
				case 16:
				{
					ConvertRGB16To24(fReflect);
					break;
				}
				case 32:
				{
					ConvertRGB32To24(fReflect);
					break;
				}
				default:
				{
					break;
				}

			} // 	ENDOF switch
			break;
		}
		case 16:
		{
			switch(CbitPixel()) // Source bpp
			{
				case 4:
				{
					ConvertRGB4To16(fReflect);
					break;
				}
				case 8:
				{
					ConvertRGB8To16(fReflect);
					break;
				}
				case 24:
				{
					ConvertRGB24To16(fReflect);
					break;
				}
				case 32:
				{
					ConvertRGB32To16(fReflect);
					break;
				}
				default:
				{
					break;
				}

			} // 	ENDOF switch
			break;
		}
		case 8:
		{
			switch(CbitPixel()) // Source bpp
			{
				case 4:
				{
					ConvertRGB4To8(fReflect);
					break;
				}
				case 16:
				{
					ConvertRGB16To8(fReflect);
					break;
				}
				case 24:
				{
					ConvertRGB24To8(fReflect);
					break;
				}
				case 32:
				{
					ConvertRGB32To8(fReflect);
					break;
				}
				default:
				{
					break;
				}

			} // 	ENDOF switch
			break;
		}
		default:
		{
			break;
		}
	}
	

}

void CVisDIB::ConvertRGB4To32(bool fReflect)
{
	CVisMemBlock& memblockHeader = ConvertHeaderMemblock(fReflect);

	CVisMemBlock memblockPixels(MemBlockPixels().CbData() * 8);

	DWORD aColor[16];

	const BITMAPINFOHEADER& bmihSrc = BitmapInfoHeader();

	FillColors(aColor, 16);

	int nHeight = abs(bmihSrc.biHeight);
	
	DWORD * pdwRGB32 = (DWORD * )memblockPixels.PbData();
	if(fReflect)
	{
		pdwRGB32 = pdwRGB32 + (bmihSrc.biWidth) * (nHeight - 1);
	}

	const BYTE* pbRGB4 = (const BYTE *) PvPixels();
	
	assert(bmihSrc.biWidth % 2 == 0);
	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		for(int iCol = 0; iCol < bmihSrc.biWidth; iCol+=2)
		{
			pdwRGB32[iCol] = aColor[*pbRGB4 & 0x0F];
			pdwRGB32[iCol + 1] = aColor[(*pbRGB4 >> 4) & 0x0F];
			pbRGB4++;
		}

		pdwRGB32 += (fReflect) ? - bmihSrc.biWidth : bmihSrc.biWidth;
	}
	
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblockPixels;


}

void CVisDIB::ConvertRGB8To32(bool fReflect)
{
	
	CVisMemBlock memblockHeader = ConvertHeaderMemblock(fReflect);

	const BITMAPINFOHEADER& bmihSrc = BitmapInfoHeader();	
	
	DWORD aColor[256];
	CVisMemBlock memblockPixels(MemBlockPixels().CbData() * 4);
	
	FillColors(aColor, 256);

	int nHeight = abs(bmihSrc.biHeight);
	
	DWORD * pdwRGB32 = (DWORD * )memblockPixels.PbData();
	if(fReflect)
	{
		pdwRGB32 = pdwRGB32 + (bmihSrc.biWidth) * (nHeight - 1);
	}

	const BYTE* pbRGB8 = (const BYTE *) PvPixels();
	
	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		for(int iCol = 0; iCol < bmihSrc.biWidth; iCol++)
		{
			pdwRGB32[iCol] = aColor[pbRGB8[iCol]];
		}

		pbRGB8 += bmihSrc.biWidth;
		pdwRGB32 += (fReflect) ? - bmihSrc.biWidth : bmihSrc.biWidth;
	}
	
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblockPixels;
}

void CVisDIB::ConvertRGB16To32(bool fReflect)
{
	CVisMemBlock memblockHeader(BitmapInfoHeader().biSize);
	memcpy(memblockHeader.PbData(), &BitmapInfoHeader(), memblockHeader.CbData());
	
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();

	pbmihDest->biBitCount = 32;
	pbmihDest->biCompression = BI_RGB;
	pbmihDest->biSizeImage = 0;
	pbmihDest->biClrUsed = 0;
	pbmihDest->biClrImportant = 0;

	CVisMemBlock memblock(CbPixelsFromBMIH(pbmihDest));
	VisInlineConvert555To32bit(PvPixels(), memblock.PbData(), 
								Width(), Height(), 
								fReflect);
	
	if(fReflect)
		pbmihDest->biHeight = - Height();
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblock;
}

void CVisDIB::ConvertRGB24To32(bool fReflect)
{
	CVisMemBlock memblockHeader(BitmapInfoHeader().biSize);
	memcpy(memblockHeader.PbData(), &BitmapInfoHeader(), memblockHeader.CbData());
	
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();

	pbmihDest->biBitCount = 32;
	pbmihDest->biCompression = BI_RGB;
	pbmihDest->biSizeImage = 0;
	pbmihDest->biClrUsed = 0;
	pbmihDest->biClrImportant = 0;
	
	CVisMemBlock memblock(CbPixelsFromBMIH(pbmihDest));
	VisInlineConvertCbitToCbit(PvPixels(), 24, memblock.PbData(), 
								32, Width(), Height(), 
								fReflect);
	if(fReflect)
		pbmihDest->biHeight = - Height();
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblock;

}

void CVisDIB::ConvertRGB4To24(bool fReflect)
{
	CVisMemBlock& memblockHeader = ConvertHeaderMemblock(fReflect, 24);

	CVisMemBlock memblockPixels(MemBlockPixels().CbData() * 6);

	DWORD aColor[16];

	const BITMAPINFOHEADER& bmihSrc = BitmapInfoHeader();

	FillColors(aColor, 16);

	int nHeight = abs(bmihSrc.biHeight);
	
	BYTE * pbRGB24 = (BYTE * )memblockPixels.PbData();
	int nRowLength24 = bmihSrc.biWidth * 3;

	if(fReflect)
	{
		pbRGB24 = pbRGB24 + nRowLength24 * (nHeight - 1);
	}

	const BYTE* pbRGB4 = (const BYTE *) PvPixels();
	
	assert(bmihSrc.biWidth % 2 == 0);
	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		for(int iCol = 0; iCol < bmihSrc.biWidth; iCol+=2)
		{
			DWORD dwColor = aColor[*pbRGB4 & 0x0F];
			*pbRGB24++ = (dwColor & 0xff);
			*pbRGB24++ = ((dwColor >> 8) & 0xff);
			*pbRGB24++ = ((dwColor >> 16) & 0xff);

			dwColor = aColor[(*pbRGB4 >> 4) & 0x0F];
			*pbRGB24++ = (dwColor >> 0 & 0xff);
			*pbRGB24++ = ((dwColor >> 8) & 0xff);
			*pbRGB24++ = ((dwColor >> 16) & 0xff);

			pbRGB4++;
		}

		pbRGB24 += (fReflect) ? - nRowLength24 * 2 : 0;
	}
	
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblockPixels;

}

void CVisDIB::ConvertRGB8To24(bool fReflect)
{
	
	CVisMemBlock memblockHeader = ConvertHeaderMemblock(fReflect, 24);

	const BITMAPINFOHEADER& bmihSrc = BitmapInfoHeader();	
	
	DWORD aColor[256];
	CVisMemBlock memblockPixels(MemBlockPixels().CbData() * 3);
	
	FillColors(aColor, 256);

	int nHeight = abs(bmihSrc.biHeight);
	
	BYTE * pbRGB24 = (BYTE * )memblockPixels.PbData();

	int nRowLength24 = bmihSrc.biWidth * 3;

	if(fReflect)
	{
		pbRGB24 = pbRGB24 + nRowLength24 * (nHeight - 1);
	}

	const BYTE* pbRGB8 = (const BYTE *) PvPixels();
	
	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		for(int iCol = 0; iCol < bmihSrc.biWidth; iCol++)
		{
			DWORD dwColor = aColor[*pbRGB8++];
			*pbRGB24++ = (dwColor & 0xff);
			*pbRGB24++ = ((dwColor >> 8) & 0xff);
			*pbRGB24++ = ((dwColor >> 16) & 0xff);
		}

		pbRGB24 += (fReflect) ? - nRowLength24 * 2 : 0;
	}
	
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblockPixels;
}

void CVisDIB::ConvertRGB16To24(bool fReflect)
{
	CVisMemBlock memblockHeader = ConvertHeaderMemblock(fReflect, 24);

	const BITMAPINFOHEADER& bmihSrc = BitmapInfoHeader();	
	
	int nPixels = MemBlockPixels().CbData() * 3 / 2;

	CVisMemBlock memblockPixels(nPixels);
	
	int nHeight = abs(bmihSrc.biHeight);
	
	int nRowLength24 = bmihSrc.biWidth * 3;

	BYTE * pbRGB24 = (BYTE * )memblockPixels.PbData();
	if(fReflect)
	{
		pbRGB24 = pbRGB24 + nRowLength24 * (nHeight - 1);
	}

	const WORD* pwRGB16 = (const WORD *) PvPixels();
	
	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		for(int iCol = 0; iCol < bmihSrc.biWidth; iCol++)
		{
			// Shifting 5-bit values left 3 places is like multiplying by 8.
			// That gives values in the range 0 to 248.
			// LATER:  It would be better to get numbers in the range of
			// 0 to 255 by multiplying by 8.25.  We can do that by ORing
			// these values with the result of shifting the 5-bit values
			// 2 places to the right.
			*pbRGB24++ = (((*pwRGB16 << 3) & 0xf8) | ((*pwRGB16 >> 2) & 0x7));
			*pbRGB24++ = (((*pwRGB16 >> 2) & 0xf8) | ((*pwRGB16 >> 7) & 0x7));
			*pbRGB24++ = (((*pwRGB16 >> 7) & 0xf8) | ((*pwRGB16++ >> 12) & 0x7));
		}

		pbRGB24 += (fReflect) ? - nRowLength24 * 2 : 0;
	}
	
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblockPixels;
}

void CVisDIB::ConvertRGB32To24(bool fReflect)
{
	CVisMemBlock memblockHeader(BitmapInfoHeader().biSize);
	memcpy(memblockHeader.PbData(), &BitmapInfoHeader(), memblockHeader.CbData());
	
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();

	pbmihDest->biBitCount = 24;
	pbmihDest->biCompression = BI_RGB;
	pbmihDest->biSizeImage = 0;
	pbmihDest->biClrUsed = 0;
	pbmihDest->biClrImportant = 0;
	
	CVisMemBlock memblock(CbPixelsFromBMIH(pbmihDest));
	VisInlineConvertCbitToCbit(PvPixels(), 32, memblock.PbData(), 
								24, Width(), Height(), 
								fReflect);
	
	if(fReflect)
		pbmihDest->biHeight = - Height();
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblock;
}

void CVisDIB::ConvertRGB4To16(bool fReflect)
{
	CVisMemBlock& memblockHeader = ConvertHeaderMemblock(fReflect, 16);

	CVisMemBlock memblockPixels(MemBlockPixels().CbData() * 4);

	DWORD aColor[16];

	const BITMAPINFOHEADER& bmihSrc = BitmapInfoHeader();

	FillColors(aColor, 16);

	int nHeight = abs(bmihSrc.biHeight);
	
	WORD * pwRGB16 = (WORD * )memblockPixels.PbData();

	if(fReflect)
	{
		pwRGB16 = pwRGB16 + bmihSrc.biWidth * (nHeight - 1);
	}

	const BYTE* pbRGB4 = (const BYTE *) PvPixels();
	
	assert(bmihSrc.biWidth % 2 == 0);
	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		for(int iCol = 0; iCol < bmihSrc.biWidth; iCol+=2)
		{
			DWORD dwColor = aColor[*pbRGB4 & 0x0F];
			*pwRGB16 = ((dwColor >> 19) & 0x1f);
			*pwRGB16 = ((*pwRGB16 << 5) & 0x83e0);
			*pwRGB16 |= ((dwColor >> 11) & 0x1f);
			*pwRGB16 = ((*pwRGB16 << 5) & 0xffe0);
			*pwRGB16 |= ((dwColor >> 3) & 0x1f);
			
			pwRGB16++;

			dwColor = aColor[(*pbRGB4 >> 4) & 0x0F];
			*pwRGB16 = ((dwColor >> 19) & 0x1f);
			*pwRGB16 = ((*pwRGB16 << 5) & 0x83e0);
			*pwRGB16 |= ((dwColor >> 11) & 0x1f);
			*pwRGB16 = ((*pwRGB16 << 5) & 0xffe0);
			*pwRGB16 |= ((dwColor >> 3) & 0x1f);

			pwRGB16++;

			pbRGB4++;
		}

		pwRGB16 += (fReflect) ? - bmihSrc.biWidth * 2 : 0;
	}

	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblockPixels;
}

void CVisDIB::ConvertRGB8To16(bool fReflect)
{
	CVisMemBlock& memblockHeader = ConvertHeaderMemblock(fReflect, 16);

	CVisMemBlock memblockPixels(MemBlockPixels().CbData() * 2);

	DWORD aColor[256];

	const BITMAPINFOHEADER& bmihSrc = BitmapInfoHeader();

	FillColors(aColor, 256);

	int nHeight = abs(bmihSrc.biHeight);
	
	WORD * pwRGB16 = (WORD * )memblockPixels.PbData();

	if(fReflect)
	{
		pwRGB16 = pwRGB16 + bmihSrc.biWidth * (nHeight - 1);
	}

	const BYTE* pbRGB8 = (const BYTE *) PvPixels();
	
	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		for(int iCol = 0; iCol < bmihSrc.biWidth; iCol++)
		{
			DWORD dwColor = aColor[pbRGB8[iCol]];
			
			*pwRGB16 = ((dwColor >> 9) & 0x7c00);
			*pwRGB16 |= ((dwColor >> 6) & 0x03e0);
			*pwRGB16 |= ((dwColor >> 3) & 0x001f);
			
			pwRGB16++;
		}

		pbRGB8 += bmihSrc.biWidth;
		pwRGB16 += (fReflect) ? - bmihSrc.biWidth * 2 : 0;
	}

	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblockPixels;
}

void CVisDIB::ConvertRGB24To16(bool fReflect)
{
}

void CVisDIB::ConvertRGB32To16(bool fReflect)
{

}

void CVisDIB::ConvertRGB4To8(bool fReflect)
{
	CVisMemBlock memblockHeader(BitmapInfoHeader().biSize);
	memcpy(memblockHeader.PbData(), &BitmapInfoHeader(), memblockHeader.CbData());
	
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();

	pbmihDest->biBitCount = 8;
	pbmihDest->biCompression = BI_RGB;
	pbmihDest->biSizeImage = 0;
	pbmihDest->biClrUsed = 0;
	pbmihDest->biClrImportant = 0;
	
	CVisMemBlock memblock(CbPixelsFromBMIH(pbmihDest));
	VisInlineConvertCbitToCbit(PvPixels(), 4, memblock.PbData(), 
								8, Width(), Height(), 
								fReflect);
	
	if(fReflect)
		pbmihDest->biHeight = - Height();
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblock;
}

void CVisDIB::ConvertRGB16To8(bool fReflect)
{
	CVisMemBlock memblockHeader(BitmapInfoHeader().biSize);
	memcpy(memblockHeader.PbData(), &BitmapInfoHeader(), memblockHeader.CbData());
	
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();

	pbmihDest->biBitCount = 8;
	pbmihDest->biCompression = BI_RGB;
	pbmihDest->biSizeImage = 0;
	pbmihDest->biClrUsed = 0;
	pbmihDest->biClrImportant = 0;
	
	CVisMemBlock memblock(CbPixelsFromBMIH(pbmihDest));
	VisInlineRGBToGray(PvPixels(), 16, memblock.PbData(), 
								8, Width(), Height(), 
								fReflect);
	
	if(fReflect)
		pbmihDest->biHeight = - Height();
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblock;
}

void CVisDIB::ConvertRGB24To8(bool fReflect)
{
	CVisMemBlock memblockHeader(BitmapInfoHeader().biSize);
	memcpy(memblockHeader.PbData(), &BitmapInfoHeader(), memblockHeader.CbData());
	
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();

	pbmihDest->biBitCount = 8;
	pbmihDest->biCompression = BI_RGB;
	pbmihDest->biSizeImage = 0;
	pbmihDest->biClrUsed = 0;
	pbmihDest->biClrImportant = 0;
	
	CVisMemBlock memblock(CbPixelsFromBMIH(pbmihDest));
	VisInlineRGBToGray(PvPixels(), 24, memblock.PbData(), 
								8, Width(), Height(), 
								fReflect);
	
	if(fReflect)
		pbmihDest->biHeight = - Height();
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblock;

}

void CVisDIB::ConvertRGB32To8(bool fReflect)
{
	CVisMemBlock memblockHeader(BitmapInfoHeader().biSize);
	memcpy(memblockHeader.PbData(), &BitmapInfoHeader(), memblockHeader.CbData());
	
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();

	pbmihDest->biBitCount = 8;
	pbmihDest->biCompression = BI_RGB;
	pbmihDest->biSizeImage = 0;
	pbmihDest->biClrUsed = 0;
	pbmihDest->biClrImportant = 0;
	
	CVisMemBlock memblock(CbPixelsFromBMIH(pbmihDest));
	VisInlineRGBToGray(PvPixels(), 32, memblock.PbData(), 
								8, Width(), Height(), 
								fReflect);
	
	if(fReflect)
		pbmihDest->biHeight = - Height();
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblock;

}

void CVisDIB::ReflectDIB(void)
{
	CVisMemBlock memblockHeader(BitmapInfoHeader().biSize);
	memcpy(memblockHeader.PbData(), &BitmapInfoHeader(), memblockHeader.CbData());
	
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();
	pbmihDest->biHeight = - pbmihDest->biHeight;

	CVisMemBlock memblockPixels(CbPixelsFromBMIH(&BitmapInfoHeader()));

	int nHeight = abs(pbmihDest->biHeight);
	
	const BYTE* pbData = (const BYTE * ) PvPixels();
	int cbRow = pbmihDest->biWidth * CbitPixel() / 8;
	BYTE* pbReflectData = memblockPixels.PbData() + cbRow * (nHeight - 1);

	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		memcpy(pbReflectData, pbData, cbRow);
		pbData += cbRow;
		pbReflectData -= cbRow;
	}
	m_memblockHeader = memblockHeader;
	m_memblockPixels = memblockPixels;

}

CVisMemBlock CVisDIB::ConvertHeaderMemblock(bool fReflect, int cbitPixel) const
{
	const BITMAPINFOHEADER& bmihSrc = BitmapInfoHeader();

	CVisMemBlock memblockHeader(bmihSrc.biSize);
	BITMAPINFOHEADER* pbmihDest = (BITMAPINFOHEADER*)memblockHeader.PbData();

	pbmihDest->biSize = bmihSrc.biSize;
	pbmihDest->biWidth = bmihSrc.biWidth;
	pbmihDest->biHeight =  (fReflect) ? - bmihSrc.biHeight : bmihSrc.biHeight;
	pbmihDest->biPlanes = bmihSrc.biPlanes;
	pbmihDest->biBitCount = cbitPixel;
	pbmihDest->biCompression = BI_RGB;
	pbmihDest->biSizeImage = 0;
	pbmihDest->biXPelsPerMeter = bmihSrc.biXPelsPerMeter;
	pbmihDest->biYPelsPerMeter = bmihSrc.biYPelsPerMeter;
	pbmihDest->biClrUsed = 0;
	pbmihDest->biClrImportant = 0;

	return memblockHeader;
}

void CVisDIB::FillColors(DWORD dwColors[], int cColors) const
{
	int cColorMax = (m_memblockHeader.CbData() - BitmapInfoHeader().biSize) / 4;
	int cClrUsed = BitmapInfoHeader().biClrUsed;
	int cColorUsed = ((cClrUsed == 0) || (cColorMax < cClrUsed)) ? cColorMax : cClrUsed;
	assert(cColorUsed <= cColors);

	int iColor = 0;
	for( ; iColor < cColorUsed; iColor++)
	{
		dwColors[iColor] = (((const DWORD&) Color(iColor)) | 0xFF000000);
	}

	for( ; iColor < cColors; iColor++)
	{
		dwColors[iColor] = GrayColorFromI(iColor);
	}
}

DWORD CVisDIB::GrayColorFromI(int iColor)
{
	BYTE bClr = iColor & 0xff;
	DWORD dwGrayColor = bClr;

	dwGrayColor = ((dwGrayColor << 8) | bClr);
	dwGrayColor = ((dwGrayColor << 8) | bClr);
	
	return (dwGrayColor & 0xffffffff);
}

bool CVisDIB::IsBMIGray8bit(bool fZeroClr) const
{
	if(CbitPixel() != 8)
		return false;

	int cColors = (m_memblockHeader.CbData() - BitmapInfoHeader().biSize) / 4;
	bool fGray8bit = (!cColors) ? fZeroClr : true;

	for(int iColor = 0; iColor < cColors; iColor++)
	{
		RGBQUAD rgbquad = Color(iColor);
		if (!(rgbquad.rgbBlue == rgbquad.rgbGreen == rgbquad.rgbRed == (BYTE) iColor))
		{
			fGray8bit = false;
			break;
		}
	}
	return fGray8bit;
}

void CVisDIB::ChangeToCopy()
{
	CVisMemBlock memblockHeaderCopy(m_memblockHeader.CbData());
	memcpy(memblockHeaderCopy.PbData(), 
			m_memblockHeader.PbData(), memblockHeaderCopy.CbData());
	CVisMemBlock memblockPixelsCopy(m_memblockPixels.CbData());
	memcpy(memblockPixelsCopy.PbData(), 
			m_memblockPixels.PbData(), memblockPixelsCopy.CbData());

	m_memblockHeader = memblockHeaderCopy;
	m_memblockPixels = memblockPixelsCopy;

}

bool CVisDIB::ReadUsingLoadImage(const char *szFilename, void* pvPixelAddress)
{
	bool fSucceeded = false;
	HBITMAP hbm = (HBITMAP) LoadImage(0, szFilename, IMAGE_BITMAP,
			0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (hbm != 0) 
	{
		HDC hdc = ::GetDC(0);
		assert((sizeof(BITMAPINFOHEADER) % 4) == 0);
		DWORD rgdwBitmapInfoAndColormap[sizeof(BITMAPINFOHEADER) / 4 + 256];
		BITMAPINFO& bmi = *((BITMAPINFO *) &(rgdwBitmapInfoAndColormap[0]));
		DWORD *rgdwColormap
			= &(rgdwBitmapInfoAndColormap[sizeof(BITMAPINFOHEADER) / 4]);
		::ZeroMemory (&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = Width();
		bmi.bmiHeader.biHeight = Height();
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biCompression = BI_RGB;

		if (IsGray8bit())
		{
			bmi.bmiHeader.biBitCount = 8;

			FillColors(rgdwColormap, 256);
		}
		else
		{
			bmi.bmiHeader.biBitCount = 32;
		}

		fSucceeded = (::GetDIBits (hdc, hbm, 0, Height(),
							pvPixelAddress, &bmi, DIB_RGB_COLORS)
							!= 0);

		// Clean up
		::ReleaseDC(0, hdc);
		DeleteObject(hbm);
	}
	return fSucceeded;
}

void CVisDIB::CopyToBuffer(void* pvFirst, int cbRowDest)
{
	assert(pvFirst);
	int cbRowSrc = Width() * CbitPixel() / 8;
	if (cbRowDest == 0)
		cbRowDest = cbRowSrc;
	const BYTE* pbSource = (const BYTE* )PvPixels();
	BYTE* pbDest = (BYTE* )pvFirst;
	assert(Height() < 0);
	int nHeight = abs(Height());

	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		CopyMemory(pbDest, pbSource, cbRowSrc);
		pbDest += cbRowDest;
		pbSource += cbRowSrc;
	}
}

void CVisDIB::CopyFromBuffer(const void* pvFirst, int cbRowSrc)
{
	assert(pvFirst);
	int cbRowDest = Width() * CbitPixel() / 8;
	if (cbRowSrc == 0)
		cbRowSrc = cbRowDest;
	BYTE* pbDest = (BYTE* )PvPixels();
	const BYTE* pbSource = (const BYTE* )pvFirst;
	assert(Height() < 0);
	int nHeight = abs(Height());

	for(int iRow = 0; iRow < nHeight; iRow++)
	{
		CopyMemory(pbDest, pbSource, cbRowDest);
		pbDest += cbRowDest;
		pbSource += cbRowSrc;
	}
	
}

void CVisDIB::Invalidate(void)
{
	m_memblockHeader = CVisMemBlock();
	m_memblockPixels = CVisMemBlock();
}

bool CVisDIB::IsValid(void)
{
	return (m_memblockHeader.CbData() && m_memblockPixels.CbData());
}
