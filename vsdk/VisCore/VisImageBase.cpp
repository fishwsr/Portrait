///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGE
//
// @module VisImageBase.cpp |
//
// Implementations of non-inline functions for the base image classes
// defined in VisImage.h.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisImage\.h> <l VisImage\.inl> <l VisFileIO\.cpp>
// <l VisImageOf\.h> <l VisImageOf\.inl>
//
// <nl>
// @index | VISIMAGE
//
///////////////////////////////////////////////////////////////////////////



// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"
#include "VisFileStreamIO.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG



// CVisCritSect g_imagecritsect;
// (A thread must be able to reenter this synchronization object
// so that we don't deadlock when copying derived image classes
// that contain other images as data members.)
CVisStaticCritSect< 'VIS0' > g_imagecritsect;
// CVisMutex< (const char *) "CVisImageBase" > g_imagecritsect;


VisCoreExport bool CVisImageBase::IsAlphaWritten(void)
{
	return PGlobals()->IsAlphaWritten();
}

VisCoreExport void CVisImageBase::SetIsAlphaWritten(bool f)
{
	PGlobals()->SetIsAlphaWritten(f);
}


// When writing BMP files for images whose row width is not a multiple
// of 4 bytes, should the width in the BMP file header be increased to
// account for the padding added when rows are written to the file.

VisCoreExport bool CVisImageBase::DoesBMPFileWidthIncludePadding(void)
{
	return PGlobals()->DoesBMPFileWidthIncludePadding();
}

VisCoreExport void CVisImageBase::SetDoesBMPFileWidthIncludePadding(bool f)
{
	PGlobals()->SetDoesBMPFileWidthIncludePadding(f);
}


VisCoreExport int CVisImageBase::DefaultOptions(void)
{
	return PGlobals()->ImOptsDefault();
}

VisCoreExport void CVisImageBase::SetDefaultOptions(int wOptions)
{
	PGlobals()->SetImOptsDefault(wOptions);
}


// Helper function
VisCoreExport int CVisImageBase::GCD(int a, int b)
{
	int gcd;

	if (a < b)
	{
		gcd = CVisImageBase::GCD(b, a);
	}
	else
	{
		assert(a > 0);
		assert(b >= 0);

		while (b != 0)
		{
			int t = a % b;
			a = b;
			b = t;
		}
	
		gcd = a;
	}

	return gcd;
}

VisCoreExport int CVisImageBase::RealGetPixelValueString(const void *pixeladdress,
		char *szValue, int cbValue, int nFieldWidth, int nDecPlaces) const
{
	// This is the method that actually makes the pixel value string.
	int cbWritten = 0;
	if (szValue != 0)
	{
		// Call PixFmt to get pixel type information.
		int evispixfmt = PixFmt();

		// LATER:  We only handle the two common display cases, and the
		// formatting options are not supported.  We should support the
		// formatting options and add support formore pixel types.
		// mturk: I added support for Float and Double
		if ((evispixfmt & evispixfmtStructureMask) == evispixfmtGray)
		{
			if ((evispixfmt & evispixfmtNumericTypeMask) == evispixfmtFloat)
			{
				float f = *((float *) pixeladdress);
				cbWritten = _snprintf(szValue, cbValue, "%*.*f", nFieldWidth, nDecPlaces, f);
			}
			else if ((evispixfmt & evispixfmtNumericTypeMask) == evispixfmtDouble)
			{
				double d = *((double *) pixeladdress);
				cbWritten = _snprintf(szValue, cbValue, "%*.*f", nFieldWidth, nDecPlaces, d);
			}
			else if ((evispixfmt & evispixfmtUnsignedMask) == evispixfmtSigned)
			{
				long l;

				switch(evispixfmt & evispixfmtNumericTypeMask)
				{
				case evispixfmtChar:
					l = *((char *) pixeladdress);
					break;

				case evispixfmtShort:
					l = *((short *) pixeladdress);
					break;

				case evispixfmtInt:
					l = *((int *) pixeladdress);
					break;

				case evispixfmtLong:
					l = *((long *) pixeladdress);
					break;
				}

				cbWritten = _snprintf(szValue, cbValue, "%*.*d", nFieldWidth, nDecPlaces, l);
			}
			else
			{
				assert((evispixfmt & evispixfmtUnsignedMask) == evispixfmtUnsigned);

				unsigned long ul;

				switch(evispixfmt & evispixfmtNumericTypeMask)
				{
				case evispixfmtChar:
					ul = *((unsigned char *) pixeladdress);
					break;

				case evispixfmtShort:
					ul = *((unsigned short *) pixeladdress);
					break;

				case evispixfmtInt:
					ul = *((unsigned int *) pixeladdress);
					break;

				case evispixfmtLong:
					ul = *((unsigned long *) pixeladdress);
					break;
				}

				cbWritten = _snprintf(szValue, cbValue, "%*.*u", nFieldWidth, nDecPlaces, ul);
			}
		}
		else if (evispixfmt == evispixfmtRGBAByte)
		{
			CVisRGBABytePixel color = *((CVisRGBABytePixel *) pixeladdress);
			cbWritten = _snprintf(szValue, cbValue, "%*u, %*u, %*u, %*u", nFieldWidth, color.R(), nFieldWidth, color.G(),
					nFieldWidth, color.B(), nFieldWidth, color.A());
		}
		else if (evispixfmt == evispixfmtYUVAByte)
		{
			CVisYUVABytePixel color = *((CVisYUVABytePixel *) pixeladdress);
			cbWritten = _snprintf(szValue, cbValue, "%*u, %*d, %*d, %*u", nFieldWidth, color.Y(), nFieldWidth, color.U(),
					nFieldWidth, color.V(), nFieldWidth, color.A());
		}
		else
		{
			strcpy(szValue,
					"Pixel value for this type is not supported yet");
		}
	}
	return cbWritten;
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | Alias |
//
// Change this image to use the information from another image.  If
// the two images have the same pixel type, this method will do the
// same thing as the assignment operator.  If the two images have
// different pixel types, this will make changes to allow this image
// to use the same memory block and pixel information as the other
// image.
//
// @parm CVisImageBase& | refimage |
// The image to alias.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisImageBase::Alias(const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	DestroyHdc();

	// Idea:  (We want cbPtSrc | cbPtDest.)
	// if cbPix | cbPtSrc, nbands *= (cbPtSrc / cbPix).
	// else if cbPtSrc | cbPix, nbands = 1 and width /= cbPtSrc.
	// else we have the general case below.
	int cbPixelDest = CbPixel();
	int cbPointSrc = refimage.CbPoint();
	
	ComputeAliasShapes(cbPointSrc, refimage.Rect(), refimage.MemoryRect());
	
	m_memblock = refimage.MemBlock();
	m_proplist = refimage.m_proplist;

	ComputeMemPointers();
}

VisCoreExport void CVisImageBase::Alias(IVisImage* pVisImage, CVisMemBlock& refMemBlock)
{
	int cbPointSrc = pVisImage->CbPoint();
	ComputeAliasShapes(cbPointSrc, pVisImage->Rect(), pVisImage->MemoryRect());

	m_memblock = refMemBlock;
	// UNDONE: What about proplist?
	ComputeMemPointers();
}

VisCoreExport void CVisImageBase::ComputeAliasShapes(int cbPointSrc, const RECT& refrect, 
									   const RECT& refrectMem)
{
	int cbPixelDest = CbPixel();
	int gcd = CVisImageBase::GCD(cbPointSrc, cbPixelDest);
	int nbands = cbPointSrc / gcd;
	int wScale = cbPixelDest / gcd;

	m_shapeImage = refrect;
	m_shapeImage.SetNBands(nbands);
	m_shapeImage.left /= wScale;
	m_shapeImage.right = refrect.left
			+ (refrect.right - refrect.left) / wScale;

	m_shapeMemBlock = refrectMem;
	m_shapeMemBlock.SetNBands(nbands);
	m_shapeMemBlock.left /= wScale;
	m_shapeMemBlock.right = refrectMem.left
			+ (refrectMem.right - refrectMem.left) / wScale;

	m_cbCol = cbPixelDest * nbands;
	m_cbRow = m_cbCol * m_shapeMemBlock.Width();
}


VisCoreExport void CVisImageBase::Alias(_IplImage *piplimage)
{
#ifdef VIS_IPL_IPL_H
	assert(piplimage != 0);
	CVisImageBase *pimagebase = &(VisImageFromIplImage(piplimage));

	// UNDONE:  We don't support wrapping IPL images that we not
	// created from CVisImage objects.
	assert(pimagebase != 0);
	Alias(*pimagebase);
#else // VIS_IPL_IPL_H
	// UNDONE:  Throw an exception?
#endif // VIS_IPL_IPL_H
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | Copy |
//
// @cmember
// Change this image to use a copy of information from another image.
// This is similar to <mf CVisImageBase::Alias>, but <mf CVisImageBase::Copy>
// makes a copy of the memory block containing the pixel information.
//
// @parm CVisImageBase& | refimage |
// The image to copy.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisImageBase::Copy(const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	DestroyHdc();

	// LATER:  Is this fine, or should border pixels also be copied?
	if (PixelTypeInfo() == refimage.PixelTypeInfo())
	{
		Allocate(refimage.Shape());
		refimage.CopyPixelsTo(*this);
	}
	else
	{
		// With different pixel types, we first Alias the source image
		// with pimageAlias, then we copy the pixels.
		CVisImageBase *pimageAlias = Clone();
		pimageAlias->Alias(refimage);

		Allocate(pimageAlias->Shape());
		pimageAlias->CopyPixelsTo(*this);
		delete pimageAlias;
	}

	m_proplist = refimage.m_proplist;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | Assign |
//
// Assignment method (called by the assignment operator).
//
// @parm const | CVisImageBase& refImage |
// Reference to the image whosh information should be assigned to this
// image.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisImageBase::Assign(const CVisImageBase& refimage)
{
	// UNDONE:  Combine Assign, Alias, and Copy (by adding a method
	// that takes an fCopy flag and checks for fSameType).

	assert(&refimage != 0);

	// We only allow assignment between images of the same
	// (derived) type.
	assert(PixelTypeInfo() == refimage.PixelTypeInfo());

	// UNDONE:  Should this be an assert?
	m_evispixfmt = refimage.m_evispixfmt;

	DestroyHdc();

	m_shapeMemBlock = refimage.m_shapeMemBlock;
	m_shapeImage = refimage.m_shapeImage;
	m_memblock = refimage.m_memblock;
	m_cbPixel = refimage.m_cbPixel;
	m_cbCol = refimage.m_cbCol;
	m_cbRow = refimage.m_cbRow;
	m_pbDataOrigin = refimage.m_pbDataOrigin;
	m_pbFirstPixelInRow0 = refimage.m_pbFirstPixelInRow0;

#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	m_ppbIliffe = refimage.m_ppbIliffe;
	m_pbOriginIliffe = refimage.m_pbOriginIliffe;
	m_memblockIliffe = refimage.m_memblockIliffe;
#endif // VIS_CORE_IMAGE_USE_ILIFFE


	// UNDONE:  Move m_fDelayRead into m_imopts?
	m_imopts = refimage.m_imopts;
	m_fDelayRead = refimage.m_fDelayRead;


	// UNDONE:  Move some or all of these into m_proplist?
	m_strName = refimage.m_strName;
    m_pVisStreamHandler = refimage.m_pVisStreamHandler;
    if( m_pVisStreamHandler )
    {
        m_pVisStreamHandler->AddRef();
    }
    m_uStreamFrameNum = refimage.m_uStreamFrameNum;
	m_filetime = refimage.m_filetime;
	m_fUseColorMap = refimage.m_fUseColorMap;
	m_memblockColorMap = refimage.m_memblockColorMap;
	m_fDirty = refimage.m_fDirty;
	m_proplist = refimage.m_proplist;

// UNDONE:  Remind Rick to add asserts about uninitialized LayerFrames in
// the Assign method.
}

VisCoreExport void CVisImageBase::Alias(IDirectDrawSurface* pIDDS)
{
	DDSURFACEDESC ddsd;
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	ddsd.dwFlags = DDSD_PITCH | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;

	HRESULT hr = pIDDS->GetSurfaceDesc(&ddsd);

	int cbPixelDest = CbPixel();
	
	int cbPointSrc = ddsd.ddpfPixelFormat.dwRGBBitCount / 8;
	
	assert((ddsd.lPitch % cbPointSrc) == 0);
	int memRight = ddsd.lPitch / cbPointSrc;

	ComputeAliasShapes(cbPointSrc, CVisRect(0, 0, ddsd.dwWidth, ddsd.dwHeight),
						CVisRect(0, 0, memRight, ddsd.dwHeight));

	m_memblock = CVisMemBlock(pIDDS);

	ComputeMemPointers();
}

// @mfunc Reads image from a file (calls to ReadWriteFile).
VisCoreExport void CVisImageBase::ReadFile(
                         SVisFileDescriptor &reffiledescriptor)	// @parm File descriptor
{
	bool fSetName = ((reffiledescriptor.filename != 0)
			&& (*reffiledescriptor.filename != 0));
	
	if (!fSetName)
		reffiledescriptor.filename = Name(true);

    ReadWriteFile(reffiledescriptor, false, true, false);

 	if (fSetName)
		SetName(reffiledescriptor.filename);
}

// @mfunc Writes image to a file (calls to ReadWriteFile).
VisCoreExport void CVisImageBase::WriteFile(
		SVisFileDescriptor &reffiledescriptor)	// @parm File descriptor.
{
	if ((reffiledescriptor.filename == 0)
			|| (*reffiledescriptor.filename == 0))
	{
		reffiledescriptor.filename = Name(true);
	}

    ReadWriteFile(reffiledescriptor, true, true, false);
}

// @mfunc Reads image from a file (calls to ReadWriteFile).
VisCoreExport bool CVisImageBase::FReadFile(
                        SVisFileDescriptor &reffiledescriptor,	// @parm File descriptor
						bool fDisplayErrorMessage)
{
	bool fSetName = ((reffiledescriptor.filename != 0)
			&& (*reffiledescriptor.filename != 0));
	
	if (!fSetName)
		reffiledescriptor.filename = Name(true);

    bool fRet = ReadWriteFile(reffiledescriptor, false, false, fDisplayErrorMessage);
    
	if ((fRet) && (fSetName))
		SetName(reffiledescriptor.filename);

	return fRet;
}

// @mfunc Writes image to a file (calls to ReadWriteFile).
VisCoreExport bool CVisImageBase::FWriteFile(
		SVisFileDescriptor &reffiledescriptor,	// @parm File descriptor.
		bool fDisplayErrorMessage)
{
	if ((reffiledescriptor.filename == 0)
			|| (*reffiledescriptor.filename == 0))
	{
		reffiledescriptor.filename = Name(true);
	}

    return ReadWriteFile(reffiledescriptor, true, false, fDisplayErrorMessage);
}

VisCoreExport void CVisImageBase::SetMemBlock(const CVisMemBlock& refmemblock,
		const CVisShape& refshapeMemBlock, const CVisShape& refshapeImage)
{
	assert(refshapeMemBlock.right > refshapeMemBlock.left);
	assert(refshapeMemBlock.bottom > refshapeMemBlock.top);
	assert(refshapeMemBlock.NBands() > 0);

	assert(refshapeImage.right > refshapeImage.left);
	assert(refshapeImage.bottom > refshapeImage.top);
	assert(refshapeImage.NBands() > 0);

	assert(refshapeMemBlock.NBands() == refshapeImage.NBands());
	assert(refshapeMemBlock.Width() >= refshapeImage.Width());
	assert(refshapeMemBlock.Height() >= refshapeImage.Height());

	m_shapeMemBlock = refshapeMemBlock;
	m_shapeImage = refshapeImage;

	m_cbCol = m_shapeMemBlock.NBands() * m_cbPixel;
	m_cbRow = m_shapeMemBlock.Width() * m_cbCol;

	assert(m_cbPixel > 0);
	assert(m_cbCol > 0);
	assert(m_cbRow > 0);

	m_memblock = refmemblock;

	ComputeMemPointers();
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc:(IMPL) void | CVisImageBase | ComputeMemPointers |
//
// Compute memory pointers after memory block, cbCol, or cbRow
// are changed.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisImageBase::ComputeMemPointers(void)
{
	m_pbDataOrigin = m_memblock.PbData() - m_shapeMemBlock.top * m_cbRow
			- m_shapeMemBlock.left * m_cbCol;
	m_pbFirstPixelInRow0 = m_pbDataOrigin + m_shapeImage.left * m_cbCol;

#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	if ((m_pbOriginIliffe == 0)
			|| (m_pbOriginIliffe != m_pbDataOrigin)
			|| (m_shapeMemBlock.Height() != m_memblockIliffe.CObj()))
	{
		// We need to update the Iliffe vector.
		CVisMemBlockOf<BYTE *> memblockIliffeT(m_shapeMemBlock.Height());
		m_ppbIliffe = (BYTE * *) PpvFillIliffeVector(
				(void **) memblockIliffeT.PObj(),
				m_shapeMemBlock.top, m_shapeMemBlock.bottom);
		m_memblockIliffe = memblockIliffeT;
		m_pbOriginIliffe = m_pbDataOrigin;
	}
#endif // VIS_CORE_IMAGE_USE_ILIFFE
}


// These virtual functions could be pure virtual functions.
// UNDONE (in Rel1):  Make them pure after changing the self-describing
// stream code to not require static exemplar objects.
VisCoreExport const type_info& CVisImageBase::PixelTypeInfo(void) const
{
	assert(0);
	throw CVisError(
			"CVisImageBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisImageBase::PixelTypeInfo",
			__FILE__, __LINE__);
	return *((const type_info *) 0);
}

VisCoreExport CVisImageBase *CVisImageBase::Clone(const CVisShape *pshape) const
{
	assert(0);
	throw CVisError(
			"CVisImageBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisImageBase::Clone",
			__FILE__, __LINE__);
	return 0;
}

VisCoreExport bool CVisImageBase::RealCopyPixelsTo(int bandSrc, int bandDest,
			CVisImageBase& refimageDest, const RECT& refrectSrc,
			const POINT pointDest, EVisNormalize evisnormalize,
			double dblMin, double dblMax) const
{
	assert(0);
	throw CVisError(
			"CVisImageBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisImageBase::RealCopyPixelsTo",
			__FILE__, __LINE__);
	return false;
}




////////////////////////////////////////////////////////////////////////////
//
// @mfunc:(IMPL) const unsigned long * | CVisImageBase | MpbulColorMap |
//
// Get a pointer to an array of <t unsigned long> values giving
// a color map to use with this image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport const unsigned long *CVisImageBase::MpbulColorMap(int cbitPixel) const
{
	const unsigned long *mpbulColorMap;

	if ((m_fUseColorMap) && (ColorMap().CbData() >= 1024))
		mpbulColorMap = (const unsigned long *) ColorMap().PbData();
	else if (cbitPixel > 4)
		mpbulColorMap = PGlobals()->ColorMapGray8bit();
	else if (m_fUseColorMap)
		mpbulColorMap = PGlobals()->ColorMapDefault4bit();
	else
		mpbulColorMap = PGlobals()->ColorMapGray4bit();

	assert(mpbulColorMap != 0);

	return mpbulColorMap;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisRect& | CVisImageBase | ResetOrigin |
//
// Translate the coordinate origin for this image to make the
// left-top corner of the image have the coordinates specified.
// The mapping from coordinates to memory addresses will be
// changed by this function so that the memory address of the
// top-left pixel in the image is not changed by the coordinate
// translation.
//
// @syntax CVisImageBase& ResetOrigin(int left, int top);
// @syntax CVisImageBase& ResetOrigin(POINT pointLeftTop);
//
// @parm POINT | pointLeftTop |
// The coordinates which will refer to the top-left pixel in this image
// after this method is called.
//
// @parm int | left |
// The coordinate which will refer to the left of this image after this
// method is called.
//
// @parm int | top |
// The coordinate which will refer to the top of this image after this
// method is called.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport CVisImageBase& CVisImageBase::ResetOrigin(int left, int top)
{
	assert(m_cbRow == m_shapeMemBlock.Width() * m_cbCol);
	assert(m_memblock.PbData() - m_shapeMemBlock.top * m_cbRow
			- m_shapeMemBlock.left * m_cbCol == m_pbDataOrigin);
	assert(m_pbFirstPixelInRow0 == m_pbDataOrigin + Left() * m_cbCol);

	int dxOffset = left - m_shapeImage.left;
	int dyOffset = top - m_shapeImage.top;
	::OffsetRect(&m_shapeMemBlock, dxOffset, dyOffset);
	::OffsetRect(&m_shapeImage, dxOffset, dyOffset);

	ComputeMemPointers();

	return *this;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | SetRect |
//
// Set new coordinates for the image rectangle.  The mapping from
// coordinates to memory addresses is not changed by this method.
// This method will fail if the rectangle specified is not
// contained in the memory rectangle returned by the
// <mf CVisImageBase::MemoryRect> method.
//
// @syntax bool SetRect(int left, int top, int right, int bottom);
// @syntax bool SetRect(POINT pointLeftTop, POINT pointRightBottom);
// @syntax bool SetRect(POINT pointLeftTop, SIZE size);
// @syntax bool SetRect(const RECT& rect);
//
// @parm int | left |
// The coordinate which will refer to the left of this image after this
// method is called.
//
// @parm int | top |
// The coordinate which will refer to the top of this image after this
// method is called.
//
// @parm int | right |
// The coordinate which will refer to the right of this image after this
// method is called.
//
// @parm int | bottom |
// The coordinate which will refer to the bottom of this image after this
// method is called.
//
// @parm const RECT& | rect |
// Windows <t RECT> structure giving the coordinates of the new
// memory rectangle.
//
// @parm POINT | pointLeftTop |
// Windows <t POINT> structure giving the left and top coordinates of the new
// memory rectangle.
//
// @parm POINT | pointRightBottom |
// Windows <t POINT> structure giving the bottom and right coordinates of the
// new memory rectangle.
//
// @parm SIZE | size |
// Windows <t SIZE> structure giving the width and height of the
// new memory rectangle.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport bool CVisImageBase::SetRect(int left, int top, int right, int bottom)
{
	bool fRet = (left <= right)
			&& (top <= bottom)
			&& (left >= m_shapeMemBlock.left)
			&& (top >= m_shapeMemBlock.top)
			&& (right <= m_shapeMemBlock.right)
			&& (bottom <= m_shapeMemBlock.bottom);

	if (fRet)
	{
		m_shapeImage.left = left;
		m_shapeImage.top = top;
		m_shapeImage.right = right;
		m_shapeImage.bottom = bottom;

		ComputeMemPointers();
	}

	return fRet;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | FillBitmapInfoHeader |
//
// Fill a Windows <t BITMAPINFOHEADER> structure with information
// about this image.  This may fail for some nonstandard images.
//
// If this pixel information for this image can be used in a Windows
// Device Independent Bitmap, this method will fill a Windows
// <t BITMAPINFOHEADER> structure with information about the pixels in
// this image.  If the  <t BITMAPINFOHEADER> structure was filled with
// information about this image, this method will return <t true>.
// Otherwise, this method will return <t false> without changing
// the information in the <t BITMAPINFOHEADER> structure.  Because
// the memory used to store the image may be larger than the image,
// the <e BITMAPINFOHEADER.biWidth> member of the <t BITMAPINFOHEADER>
// structure may be filled with a value that is larger than the width
// of the image.
//
// @this const
//
// @parm BITMAPINFOHEADER * | pbmih |
// Pointer to the Windows <t BITMAPINFOHEADER> structure to be filled
// with information about this image.
//
// @parmopt int | cbitPixel | 0 |
// If non-zero, this specifies  the width of displayed pixels, in bits.
// Most callers can just pass in 0 to have the image calculate the
// number of bits to use from the pixel type.
//
// @xref <t BITMAPINFOHEADER>
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport bool CVisImageBase::FillBitmapInfoHeader(BITMAPINFOHEADER *pbmih,
		int cbitPixel) const
{
	// Find the correct pixel size.
	if (cbitPixel == 0)
	{
		if (CbPoint() <= 4)
			cbitPixel = CbPoint() * 8;
		else if (CbPixel() <= 4)
			cbitPixel = CbPixel() * 8;
	}
	else if (cbitPixel == 15)
	{
		// Special case of 5-5-5 RGB.  15 bits stored in 16 bits.
		cbitPixel = 16;
	}

	// Can we fill a BITMAPINFOHEADER for this image?
	bool fRet = (pbmih != 0)
			&& (m_cbRow % sizeof(DWORD) == 0)
			&& ((cbitPixel == 1)
				|| (cbitPixel == 4)
				|| (cbitPixel == 8)
				|| (cbitPixel == 16)
				|| (cbitPixel == 24)
				|| (cbitPixel == 32));

	if (fRet)
	{
		int biWidth = m_cbRow  * 8 / cbitPixel;

		pbmih->biSize = sizeof(BITMAPINFOHEADER);
		pbmih->biWidth = biWidth;
		pbmih->biHeight = - Height();
		pbmih->biPlanes = 1;
		pbmih->biBitCount = (WORD) cbitPixel;
		pbmih->biCompression = BI_RGB;
		pbmih->biSizeImage = 0;
		pbmih->biXPelsPerMeter = 3150;
		pbmih->biYPelsPerMeter = 3150;
		pbmih->biClrUsed = 0;
		pbmih->biClrImportant = 0;
	}

	return fRet;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc HDC | CVisImageBase | Hdc |
//
// Create a Windows device context that can be used to draw into
// this image.  If this method fails, 0 if returned.  The class
// will manage the device context after it is created, so the
// caller does not need to call Windows functions to destroy the
// device context.  The coordinates used with the device context
// will be relative to the StartPoint() of this image.
//
// LATER:  Should this also set a clipping region for the HDC?
// LATER:  Should this return a CDC instead?  (Or should there be a
// DC method that returns a CDC?)
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport HDC CVisImageBase::Hdc(void)
{
	if (m_hdc == 0)
	{
		m_hdc = CreateCompatibleDC(0);

		if (m_hdc != 0)
		{
			HBITMAP hbitmap = HBitmapCreateForHdc(m_hdc);
			if (hbitmap != 0)
			{
				m_hbitmapOld = (HBITMAP) SelectObject(m_hdc, hbitmap);
			}
			else
			{
				DeleteDC(m_hdc);
				m_hdc = 0;
			}
		}
	}

	return m_hdc;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | DestroyHdc |
//
// Destroy the Windows device context that was be used to draw into
// this image.  This function will free resources used to draw into
// this image.  Callers do not need to call this function, but they
// may want to call it to free resources when they have finished
// drawing into the image.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisImageBase::DestroyHdc(void)
{
	if (m_hdc != 0)
	{
		if (m_hbitmapOld != 0)
		{
			DeleteObject(SelectObject(m_hdc, m_hbitmapOld));
			m_hbitmapOld = 0;
		}

		DeleteDC(m_hdc);
		m_hdc = 0;
	}
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc HBITMAP | CVisImageBase | HBitmapCreateForHdc |
//
// Given a Windows memory device context, create a device-dependent
// Windows <t BITMAP> using the Windows <f CreateDIBSection>
// function.
//
// If this pixel information for this image can be used in a Windows
// Device Independent Bitmap, this method will create a Windows <t BITMAP>
// for this image.  (The Windows <t BITMAP> can be
// selected into the device context to allow us to modify the
// image using the Windows GDI functions.)  Returns 0 if a
// Windows BITMAP could not be created for this image.
// Otherwise, the handle to a Windows <t BITMAP> structure is
// returned.
//
// @parm HDC | hdcMem |
// A Windows memory device context created using the Windows
// <f CreateCompatibleDC> function.
//
// @comm
// Internally, this function called the Windows <f CreateDIBSection>
// function to create the Windows <t BITMAP>.  As stated in the
// description of the <f CreateDIBSection> function, it is important
// that the application call the Windows <f GdiFlush> function on
// Windows NT after calling Windows functions that use the
// <t BITMAP> if the application wants to modify the image pixels
// directly.
// 
// If a handle to a Windows <t BITMAP> structure is
// returned, its origin will coorespond to the top-left pixel
// in the image.
// 
// @xref BITMAP CreateCompatibleDC CreateDIBSection CreateFileMapping
// GdiFlush
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport HBITMAP CVisImageBase::HBitmapCreateForHdc(HDC hdcMem) const
{
	HBITMAP hbitmapRet = 0;
	BITMAPINFOHEADER bitmapinfoheader;

	// Currently, we only support RGB images with 1-byte pixels.
	// LATER:  We might also be able to get this to work with BYTE images,
	// if we use a colormap for the grayscale values.
	int evispixfmt = PixFmt();
	const BYTE *pbFirstPixel = PbPixel(StartPoint(), 0);
	if ((m_memblock.HFileMapping() != 0)
			&& ((((int) pbFirstPixel) % sizeof(DWORD)) == 0)
			&& ((CbPoint() == 3) || (CbPoint() == 4))
			&& (((evispixfmt & evispixfmtNumericTypeMask)
					== evispixfmtChar)
				|| ((evispixfmt & evispixfmtNumericTypeMask)
						== evispixfmtInt)
				|| ((evispixfmt & evispixfmtNumericTypeMask)
						== evispixfmtLong))
			&& ((evispixfmt & evispixfmtUnsignedMask)
					== evispixfmtUnsigned)
			&& (FillBitmapInfoHeader(&bitmapinfoheader)))
	{
		void *pv = 0;
		hbitmapRet = ::CreateDIBSection(hdcMem,
				(BITMAPINFO *) &bitmapinfoheader, DIB_RGB_COLORS, &pv,
				m_memblock.HFileMapping(),
				(pbFirstPixel - m_memblock.PbData()) + m_memblock.IbOffset());
	}

	return hbitmapRet;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | DisplayInHdc |
//
// Attempt to display this image in a Windows Device Context.  This
// should succeed for images that have
//
// If this pixel information for this image can be used in a Windows
// Device Independent Bitmap, this method will copy the image pixel
// values to a device context using the Windows
// <f SetDIBitsToDevice> function.
//
// @syntax bool DisplayInHdc(HDC hdc, int xDest = 0, int yDest = 0,
//		DWORD dwRop = SRCCOPY, int cbitPixel = 0) const;
// @syntax bool DisplayInHdc(HDC hdc, const POINT& pointDest,
//		DWORD dwRop = SRCCOPY, int cbitPixel = 0) const;
// @syntax bool DisplayInHdc(HDC hdc, const RECT& rectSrc, int xDest = 0,
//		int yDest = 0, DWORD dwRop = SRCCOPY,
//		int cbitPixel = 0) const;
// @syntax bool DisplayInHdc(HDC hdc, const RECT& rectSrc,
//		const POINT& pointDest, DWORD dwRop = SRCCOPY,
//		int cbitPixel = 0) const;
// @syntax bool DisplayInHdc(HDC hdc, const RECT& rectSrc,
//		const RECT& rectDest, DWORD dwRop = SRCCOPY,
//		int cbitPixel = 0, bool fMirror = false) const;
//
// @parm HDC | hdc |
// Windows handle to a Device Context.  The image will be displayed in
// this Device Context.
//
// @parm const RECT& | rectSrc |
// Windows <t RECT> structure specifying the part of the image that
// should be displayed.  This parameter is not needed if all of the
// image should be displayed.
//
// @parm const POINT& | pointDest |
// Windows <t POINT> structure specifying the location (upper-left corner)
// where the image should be drawn in the destination device context.
//
// @parmopt int | xDest | 0 |
// The (leftmost) column where the image should be drawn in the destination
// device context.
//
// @parmopt int | yDest | 0 |
// The (topmost) row where the image should be drawn in the destination
// device context.
//
// @parmopt DWORD | dwRop | SRCCOPY |
// Specifies how the source pixels, the destination device context’s
// current brush, and the destination pixels are to be combined to form
// the new image.
//
// @parmopt int | cbitPixel | 0 |
// If non-zero, this specifies  the width of displayed pixels, in bits.
// Most callers can just pass in 0 to have the image calculate the
// number of bits to use from the pixel type.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport bool CVisImageBase::DisplayInHdc(HDC hdc, const RECT& rectSrc,
		int xDest, int yDest, DWORD dwRop, int cbitPixel) const
{
	bool fRet = false;

	// LATER:  Add an option to convert the image if needed.
	// LATER:  Should this be a virtual helper method that derived classes
	// could override?

	// May need to use a BITMAPINFO structure if we have a color map.
	struct
	{
		BITMAPINFOHEADER header;
		unsigned long mpbul[256];
	}			bmi;

	const BYTE *pbFirstPixel = PbPixel(StartPoint(), 0);
	// LATER:  This may fail for Byte images when ((Left() % 4) != 0).
	// Should we add code to handle that case?
	if (((((int) pbFirstPixel) % sizeof(DWORD)) == 0)
			&& (FillBitmapInfoHeader(&bmi.header, cbitPixel)))
	{
		if (dwRop != SRCCOPY)
		{
			// Need to use StretchDIBits.
			CVisRect rectDest;
			rectDest.left = xDest;
			rectDest.top = yDest;
			rectDest.right = xDest + (rectSrc.right - rectSrc.left);
			rectDest.bottom = yDest + (rectSrc.bottom - rectSrc.top);
			fRet = DisplayInHdc(hdc, rectSrc, rectDest, dwRop,
					cbitPixel);
		}
		else
		{
			CVisRect rect;
			rect.left = rectSrc.left - Left();
			rect.top = rectSrc.top - Top();
			rect.right = rectSrc.right - Left();
			rect.bottom = rectSrc.bottom - Top();

			// Adjust the coordinates to use the pixel size in the
			// BITMAPINFO structure.
			if (bmi.header.biBitCount > CbPoint() * 8)
			{
				assert(bmi.header.biBitCount % (CbPoint() * 8) == 0);
				int wDiv = (bmi.header.biBitCount / (CbPoint() * 8));
				rect.left /= wDiv;
				rect.right /= wDiv;
			}
			else if (bmi.header.biBitCount < CbPoint() * 8)
			{
				assert((CbPoint() * 8) % bmi.header.biBitCount == 0);
				int wMult = ((CbPoint() * 8) / bmi.header.biBitCount);
				rect.left *= wMult;
				rect.right *= wMult;
			}

			// May need to fill in color map values.
			if (bmi.header.biBitCount <= 8)
			{
				CopyMemory(&(bmi.mpbul),
						MpbulColorMap(bmi.header.biBitCount),
						sizeof(bmi.mpbul));
			}

			// Check for special pixel types.
			switch (PixFmt())
			{
			case evispixfmtGrayUShort:
				// Use high byte to display grayscale images.
				// (This may not work right on Win95.)
				bmi.header.biCompression = BI_BITFIELDS;
				bmi.mpbul[0] = 0xff00;
				bmi.mpbul[1] = 0xff00;
				bmi.mpbul[2] = 0xff00;
				break;

			case evispixfmtGrayUInt:
			case evispixfmtGrayULong:
				// Use high byte to display grayscale images.
				// (This may not work right on Win95.)
				bmi.header.biCompression = BI_BITFIELDS;
				bmi.mpbul[0] = 0xff000000;
				bmi.mpbul[1] = 0xff000000;
				bmi.mpbul[2] = 0xff000000;
				break;

			case evispixfmtGrayShort:
				// This gives partial support for signed images.
				// (The sign bit is ignored.)
				// (This may not work right on Win95.)
				bmi.header.biCompression = BI_BITFIELDS;
				bmi.mpbul[0] = 0x7f00;
				bmi.mpbul[1] = 0x7f00;
				bmi.mpbul[2] = 0x7f00;
				break;

			case evispixfmtGrayInt:
			case evispixfmtGrayLong:
				// This gives partial support for signed images.
				// (The sign bit is ignored.)
				// (This may not work right on Win95.)
				bmi.header.biCompression = BI_BITFIELDS;
				bmi.mpbul[0] = 0x7f000000;
				bmi.mpbul[1] = 0x7f000000;
				bmi.mpbul[2] = 0x7f000000;
				break;

			case evispixfmtYUVAByte:
				// Display YUV images as grayscale.
				// (This may not work right on Win95.)
				bmi.header.biCompression = BI_BITFIELDS;
				bmi.mpbul[0] = 0xff;
				bmi.mpbul[1] = 0xff;
				bmi.mpbul[2] = 0xff;
				break;
			}

			fRet = (SetDIBitsToDevice(hdc, xDest, yDest,
					rect.Width(), rect.Height(),
					rect.left, Height() - rect.bottom,
					0, Height(), pbFirstPixel,
					(BITMAPINFO *) &bmi,
					DIB_RGB_COLORS) > 0);
		}
	}

	return fRet;
}


VisCoreExport bool CVisImageBase::DisplayInHdc(HDC hdc, const RECT& rectSrc,
		const RECT& rectDest, DWORD dwRop, int cbitPixel, bool fMirror) const
{
	bool fRet = false;

	// May need to use a BITMAPINFO structure if we have a color map.
	struct
	{
		BITMAPINFOHEADER header;
		unsigned long mpbul[256];
	}			bmi;

	const BYTE *pbFirstPixel = PbPixel(StartPoint(), 0);
	// LATER:  This may fail for Byte images when ((Left() % 4) != 0).
	// Should we add code to handle that case?
	if (((((int) pbFirstPixel) % sizeof(DWORD)) == 0)
			&& (FillBitmapInfoHeader(&bmi.header, cbitPixel)))
	{
		CVisRect rect;
		rect.left = rectSrc.left - Left();
		rect.top = rectSrc.top - Top();
		rect.right = rectSrc.right - Left();
		rect.bottom = rectSrc.bottom - Top();

		// Adjust the coordinates to use the pixel size in the
		// BITMAPINFO structure.
		if (bmi.header.biBitCount > CbPoint() * 8)
		{
			assert(bmi.header.biBitCount % (CbPoint() * 8) == 0);
			int wDiv = (bmi.header.biBitCount / (CbPoint() * 8));
			rect.left /= wDiv;
			rect.right /= wDiv;
		}
		else if (bmi.header.biBitCount < CbPoint() * 8)
		{
			assert((CbPoint() * 8) % bmi.header.biBitCount == 0);
			int wMult = ((CbPoint() * 8) / bmi.header.biBitCount);
			rect.left *= wMult;
			rect.right *= wMult;
		}

		// May need to fill in color map values.
		if (bmi.header.biBitCount <= 8)
		{
			CopyMemory(&(bmi.mpbul),
					MpbulColorMap(bmi.header.biBitCount),
					sizeof(bmi.mpbul));
		}

		// Check for special pixel types.
		switch (PixFmt())
		{
		case evispixfmtGrayUShort:
			// Use high byte to display grayscale images.
			// (This may not work right on Win95.)
			bmi.header.biCompression = BI_BITFIELDS;
			bmi.mpbul[0] = 0xff00;
			bmi.mpbul[1] = 0xff00;
			bmi.mpbul[2] = 0xff00;
			break;

		case evispixfmtGrayUInt:
		case evispixfmtGrayULong:
			// Use high byte to display grayscale images.
			// (This may not work right on Win95.)
			bmi.header.biCompression = BI_BITFIELDS;
			bmi.mpbul[0] = 0xff000000;
			bmi.mpbul[1] = 0xff000000;
			bmi.mpbul[2] = 0xff000000;
			break;

		case evispixfmtGrayShort:
			// This gives partial support for signed images.
			// (The sign bit is ignored.)
			// (This may not work right on Win95.)
			bmi.header.biCompression = BI_BITFIELDS;
			bmi.mpbul[0] = 0x7f00;
			bmi.mpbul[1] = 0x7f00;
			bmi.mpbul[2] = 0x7f00;
			break;

		case evispixfmtGrayInt:
		case evispixfmtGrayLong:
			// This gives partial support for signed images.
			// (The sign bit is ignored.)
			// (This may not work right on Win95.)
			bmi.header.biCompression = BI_BITFIELDS;
			bmi.mpbul[0] = 0x7f000000;
			bmi.mpbul[1] = 0x7f000000;
			bmi.mpbul[2] = 0x7f000000;
			break;

		case evispixfmtYUVAByte:
			// Display YUV images as grayscale.
			// (This may not work right on Win95.)
			bmi.header.biCompression = BI_BITFIELDS;
			bmi.mpbul[0] = 0xff;
			bmi.mpbul[1] = 0xff;
			bmi.mpbul[2] = 0xff;
			break;
		}

		int wStretchBltModePrev = GetStretchBltMode(hdc);
		if (wStretchBltModePrev != COLORONCOLOR)
			SetStretchBltMode(hdc, COLORONCOLOR);

		if (fMirror)
		{
			fRet = (StretchDIBits(hdc, rectDest.left, rectDest.top,
				rectDest.right - rectDest.left,
				rectDest.bottom - rectDest.top,
				rect.right, Height() - rect.bottom,
				-rect.Width(), rect.Height(),
				pbFirstPixel, (BITMAPINFO *) &bmi,
				DIB_RGB_COLORS, dwRop) != GDI_ERROR);
		}
		else
		{
			fRet = (StretchDIBits(hdc, rectDest.left, rectDest.top,
				rectDest.right - rectDest.left,
				rectDest.bottom - rectDest.top,
				rect.left, Height() - rect.bottom,
				rect.Width(), rect.Height(),
				pbFirstPixel, (BITMAPINFO *) &bmi,
				DIB_RGB_COLORS, dwRop) != GDI_ERROR);
		}

		if (wStretchBltModePrev != COLORONCOLOR)
			SetStretchBltMode(hdc, wStretchBltModePrev);
	}

	return fRet;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | ClearPixels |
//
// Clear the image pixels by filling all bytes in the pixel memory
// with zero values.  This method could cause problems if used with
// some used-defined types.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisImageBase::ClearPixels(void)
{
	if ((Width() > 0) && (Height() > 0))
	{
		int cbRow = CbRow();
		int cbImageRow = Width() * CbPoint();
		BYTE *pbFirst = PbPixel(StartPoint(), 0);
		const BYTE *pbLim = pbFirst + Height() * cbRow;

		if (cbRow == cbImageRow)
		{
			// We can clear all pixels in one function call.
			ZeroMemory(pbFirst, pbLim - pbFirst);
		}
		else
		{
			// Need to clear pixels one row at a time.
			for (BYTE *pb = pbFirst; pb != pbLim; pb += cbRow)
				ZeroMemory(pb, cbImageRow);
		}
	}
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | Allocate |
//
// Allocate or reallocate the memory block for this image.  Most
// users of the library will not need to use this method.
//
// @syntax Allocate(const CVisShape& shapeImage);
// @syntax Allocate(const CVisShape& shapeImage,
//		const CVisShape& shapeMemBlock);
//
// @parm const CVisShape& | shapeImage |
// The new image dimensions and valid coordinate ranges.
//
// @parm const CVisShape& | shapeMemBlock |
// The new image memory dimensions and valid coordinate ranges.  If
// specified, <p shapeMemBlock> must contain <p shapeImage>.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisImageBase::Allocate(const CVisShape& shapeImage,
		const CVisShape& shapeMemBlock)
{
	// LATER:  Do we want to add a default imopts parameter to change
	// m_imopts in some cases?
	assert(shapeImage.right > shapeImage.left);
	assert(shapeImage.bottom > shapeImage.top);
	assert(shapeImage.NBands() > 0);
	assert(shapeMemBlock.left <= shapeImage.left);
	assert(shapeMemBlock.right >= shapeImage.right);
	assert(shapeMemBlock.top <= shapeImage.top);
	assert(shapeMemBlock.bottom >= shapeImage.bottom);
	assert(shapeMemBlock.NBands() == shapeMemBlock.NBands());
	assert(m_cbPixel > 0);

	assert(((m_imopts & evisimoptMemAllocMask) == evisimoptMemShared)
			|| ((m_imopts & evisimoptMemAllocMask) == evisimoptMemNotShared));

	CVisShape shapeMemUsed = shapeMemBlock;
	int cbCol = m_cbPixel * shapeMemBlock.NBands();
	if ((m_imopts & evisimoptAlignMemMask) != evisimoptDontAlignMemory)
	{
		int cb;

		if ((m_imopts & evisimoptAlignMemMask) == evisimoptAlignMem8Byte)
			cb = 8;
		else
			cb = 4;

		if ((shapeMemUsed.Width() * cbCol % cb) != 0)
		{
			shapeMemUsed.right = shapeMemUsed.left +
					((((shapeMemUsed.Width() * cbCol
									+ cb - 1)
								/ cb)
							* cb)
						/ cbCol);
		}
	}
	int cbMem = cbCol * shapeMemUsed.Width() * shapeMemUsed.Height();

	int opt;
	if ((m_imopts & evisimoptMemAllocMask) == evisimoptMemShared)
		opt = evismemblockAllocFileMapping;
	else
		opt = evismemblockAllocGlobalAlloc;

	CVisMemBlock memblock(cbMem, opt);

	if ((m_imopts & evisimoptInitMemMask) == evisimoptZeroInitMemory)
	{
		ZeroMemory(memblock.PbData(), cbMem);
	}

	SetMemBlock(memblock, shapeMemUsed, shapeImage);
}

VisCoreExport void CVisImageBase::Allocate(void)
{
	if ((!IsValid())
			&& (MemoryShapeBeforeDeallocate().Width() != 0))
	{
		Allocate(ShapeBeforeDeallocate(), MemoryShapeBeforeDeallocate());
	}
}

VisCoreExport void CVisImageBase::AllocateUsingPrevoiusShapeIfPossible(
		const CVisShape& refshape)
{
	DWORD dwMem = CbPixel() * refshape.Width() * refshape.Height();	

	if((dwMem > PGlobals()->CbImageSizeLimit()) ||
	   (dwMem <= refshape.Width() * CbPixel())  ||
	   (dwMem <= refshape.Height() * CbPixel()))
	{
		return;
	}
	if ((ShapeBeforeDeallocate().Width() == refshape.Width())
			&& (ShapeBeforeDeallocate().Height() == refshape.Height())
			&& (ShapeBeforeDeallocate().NBands() == refshape.NBands()))
	{
		Allocate(ShapeBeforeDeallocate(),
				MemoryShapeBeforeDeallocate());
	}
	else if ((((MemoryShapeBeforeDeallocate().Width() + 7) & ~7)
					== ((refshape.Width() + 7) & ~7))
			&& (MemoryShapeBeforeDeallocate().Width() >= refshape.Width())
			&& (MemoryShapeBeforeDeallocate().Height() == refshape.Height())
			&& (MemoryShapeBeforeDeallocate().NBands() == refshape.NBands()))
	{
		// Note:  The width comparison above allows for the case that
		// the memory width is slightly larger than the image width
		// (because of row alignment).
		Allocate(MemoryShapeBeforeDeallocate());
	}
	else
	{
		Allocate(refshape);
	}
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | Deallocate |
//
// Free the memory block used by this image.  Most users of
// the library will not need to use this method.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisImageBase::Deallocate(bool fRememberShapes)
{
	DestroyHdc();

	if (!fRememberShapes)
	{
		m_shapeMemBlock.left = 0;
		m_shapeMemBlock.top = 0;
		m_shapeMemBlock.right = 0;
		m_shapeMemBlock.bottom = 0;
		m_shapeMemBlock.SetNBands(1);

		m_shapeImage = m_shapeMemBlock;
	}

	// We assume that some members are contiguous in the ShapeInvalid
	// method where we may need to cast a group of members to a
	// CVisShape object whose members are all equal to zero.
	assert(((int) (const void *) &m_cbRow)
			== ((int) (const void *) (&m_cbCol + 1)));
	assert(((int) (const void *) &m_pbDataOrigin)
			== ((int) (const void *) (&m_cbRow + 1)));
	assert(((int) (const void *) &m_pbFirstPixelInRow0)
			== ((int) (const void *) (&m_pbDataOrigin + 1)));
	assert(((int) (const void *) &m_hdc)
			== ((int) (const void *) (&m_pbFirstPixelInRow0 + 1)));
	assert(((int) (const void *) &m_hbitmapOld)
			== ((int) (const void *) (&m_hdc + 1)));

	m_cbCol = 0;
	m_cbRow = 0;
	m_pbDataOrigin = 0;
	m_pbFirstPixelInRow0 = 0;

	CVisMemBlock memblockEmpty;
	m_memblock = memblockEmpty;

#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	m_memblockIliffe = CVisMemBlockOf<BYTE *>();
	m_ppbIliffe = 0;
	m_pbOriginIliffe = 0;
#endif // VIS_CORE_IMAGE_USE_ILIFFE

	m_fUseColorMap = false;
	m_fDirty = false;
	m_memblockColorMap = memblockEmpty;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc:(IMPL) | CVisImageBase | CVisImageBase |
//
// Constructors.  The default constructor does not allocate memory to
// store image pixel values.
//
// @syntax CVisImageBase(int cbPixel, const RECT& rect, int nbands, int imopts,
//		BYTE *pbData);
// @syntax CVisImageBase(int cbPixel);
// @syntax CVisImageBase(const CVisImageBase& refImage);
//
// @parm int | cbPixel |
// The size of image pixels, in bytes.
//
// @parm EVisPixFmt | evispixfmt |
// The type of image pixels.
//
// @parm int | xLeft |
// The left column of the rectangle containing valid pixel addresses for
// this image.
//
// @parm int | yTop |
// The top row of the rectangle containing valid pixel addresses for
// this image.
//
// @parm int | dxWidth |
// The width of the rectangle containing valid pixel addresses for
// this image.
//
// @parm int | dyHeight |
// The height of the rectangle containing valid pixel addresses for
// this image.
//
// @parm int | nbands |
// The number of bands of pixels in this image.
//
// @parm BYTE | *pbData |
// Pointer to memory containing pixel values to use in this image.
// If this is NULL, the image will allocate memory for pixel values.
//
// @parm int | imopts |
// Options to use with this image.  (For example, memory allocation
// options.)
//
// @parm const | CVisImageBase& refImage |
// Reference to another image used in the copy constructor.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport CVisImageBase::CVisImageBase(int cbPixel, EVisPixFmt evispixfmt, const RECT& rect,
		int nbands, int imopts, BYTE *pbData)
  : m_shapeMemBlock(rect, nbands),
	m_shapeImage(rect, nbands),
	m_memblock(),
	m_evispixfmt(evispixfmt),
	m_cbPixel(cbPixel),
	m_cbCol(cbPixel * nbands),
	m_cbRow(m_cbCol * (rect.right - rect.left)),
	m_pbDataOrigin(0),
	m_pbFirstPixelInRow0(0),
    m_pVisStreamHandler(NULL),
#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	m_ppbIliffe(0),
	m_pbOriginIliffe(0),
#endif // VIS_CORE_IMAGE_USE_ILIFFE
	m_fUseColorMap(false),
	m_fDirty(false),
	m_fDelayRead(false),
	m_memblockColorMap(),
	m_imopts(imopts == - 1 ? DefaultOptions() : imopts),
	m_hdc(0),
	m_hbitmapOld(0)
{
	// Finish member variable initialization.
	m_filetime.dwLowDateTime = 0;
	m_filetime.dwHighDateTime = 0;

	// Allow a NULL RECT to be passed to this ctor for compatibility with the old SDK.
	// UNDONE:  We should review this.  It's probably best to assert that the RECT is
	// not empty here.
	if ((rect.left == 0)
			&& (rect.top == 0)
			&& (rect.right == 0)
			&& (rect.bottom == 0))
	{
		m_shapeImage.SetNBands(1);
	}
	else
	{
		// Asserts.
		assert(((m_imopts & evisimoptMemAllocMask) == evisimoptMemShared)
				|| ((m_imopts & evisimoptMemAllocMask) == evisimoptMemNotShared)
				|| ((m_imopts & evisimoptMemAllocMask) == evisimoptMemObjType));
		assert(m_shapeImage.right > m_shapeImage.left);
		assert(m_shapeImage.bottom > m_shapeImage.top);
		assert(m_cbPixel > 0);
		assert(nbands > 0);

		if (pbData != 0)
		{
			CVisMemBlock memblock(pbData, m_cbRow * m_shapeMemBlock.Height());
			SetMemBlock(memblock, m_shapeMemBlock, m_shapeImage);
		}
		else if (m_cbRow * m_shapeMemBlock.Height() <= 0)
		{
			// Handle the case of an empty rect here.
			m_shapeMemBlock.left = 0;
			m_shapeMemBlock.top = 0;
			m_shapeMemBlock.right = 0;
			m_shapeMemBlock.bottom = 0;
			m_shapeMemBlock.SetNBands(1);
			m_shapeImage = m_shapeMemBlock;
			m_cbCol = 0;
			m_cbRow = 0;
		}
		else if (((m_imopts & evisimoptMemAllocMask) == evisimoptMemShared)
				|| ((m_imopts & evisimoptMemAllocMask) == evisimoptMemNotShared))
		{
			Allocate(m_shapeImage);
		}
	}

}

VisCoreExport CVisImageBase::CVisImageBase(CVisMemBlock& refmemblock, int cbPixel,
		EVisPixFmt evispixfmt, const RECT& rect, int nbands, int imopts)
  : m_shapeMemBlock(rect, nbands),
	m_shapeImage(rect, nbands),
	m_memblock(),
	m_evispixfmt(evispixfmt),
	m_cbPixel(cbPixel),
	m_cbCol(cbPixel * nbands),
	m_cbRow(m_cbCol * (rect.right - rect.left)),
	m_pbDataOrigin(0),
	m_pbFirstPixelInRow0(0),
    m_pVisStreamHandler(NULL),
#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	m_ppbIliffe(0),
	m_pbOriginIliffe(0),
#endif // VIS_CORE_IMAGE_USE_ILIFFE
	m_fUseColorMap(false),
	m_fDirty(false),
	m_fDelayRead(false),
	m_memblockColorMap(),
	m_imopts(imopts == - 1 ? DefaultOptions() : imopts),
	m_hdc(0),
	m_hbitmapOld(0)
{
	// Finish member variable initialization.
	m_filetime.dwLowDateTime = 0;
	m_filetime.dwHighDateTime = 0;

	// Asserts.
	assert(((m_imopts & evisimoptMemAllocMask) == evisimoptMemShared)
			|| ((m_imopts & evisimoptMemAllocMask) == evisimoptMemNotShared)
			|| ((m_imopts & evisimoptMemAllocMask) == evisimoptMemObjType));
	assert(m_shapeImage.right > m_shapeImage.left);
	assert(m_shapeImage.bottom > m_shapeImage.top);
	assert(m_cbPixel > 0);
	assert(nbands > 0);

	if (refmemblock.PbData() != 0)
	{
		SetMemBlock(refmemblock, m_shapeMemBlock, m_shapeImage);
	}
	else if (m_cbRow * m_shapeMemBlock.Height() <= 0)
	{
		// Handle the case of an empty rect here.
		m_shapeMemBlock.left = 0;
		m_shapeMemBlock.top = 0;
		m_shapeMemBlock.right = 0;
		m_shapeMemBlock.bottom = 0;
		m_shapeMemBlock.SetNBands(1);
		m_shapeImage = m_shapeMemBlock;
		m_cbCol = 0;
		m_cbRow = 0;
	}
	else if (((m_imopts & evisimoptMemAllocMask) == evisimoptMemShared)
			|| ((m_imopts & evisimoptMemAllocMask) == evisimoptMemNotShared))
	{
		Allocate(m_shapeImage);
	}
}

VisCoreExport CVisImageBase::CVisImageBase(int cbPixel, EVisPixFmt evispixfmt)
  : m_shapeMemBlock(CVisRect(0, 0, 0, 0), 1),
	m_shapeImage(CVisRect(0, 0, 0, 0), 1),
	m_memblock(),
	m_evispixfmt(evispixfmt),
	m_cbPixel(cbPixel),
	m_cbCol(0),
	m_cbRow(0),
	m_pbDataOrigin(0),
	m_pbFirstPixelInRow0(0),
#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	m_ppbIliffe(0),
	m_pbOriginIliffe(0),
#endif // VIS_CORE_IMAGE_USE_ILIFFE
	m_strName(),
    m_pVisStreamHandler(NULL),
	m_fUseColorMap(false),
	m_fDirty(false),
	m_fDelayRead(false),
	m_memblockColorMap(),
	m_imopts(DefaultOptions()),
	m_hdc(0),
	m_hbitmapOld(0)
{
	m_filetime.dwLowDateTime = 0;
	m_filetime.dwHighDateTime = 0;
}

// UNDONE:  Remove this later.  (It is kept here so that the exports from
// VisCore.dll will not change from version 1.0, but this should never be
// called.)
VisCoreExport CVisImageBase::CVisImageBase(const CVisImageBase& refimage)
  : m_shapeMemBlock(CVisRect(0, 0, 0, 0), 1),
	m_shapeImage(CVisRect(0, 0, 0, 0), 1),
	m_memblock(),
	m_evispixfmt(refimage.m_evispixfmt),
	m_cbPixel(refimage.m_cbPixel),
	m_cbCol(0),
	m_cbRow(0),
	m_pbDataOrigin(0),
	m_pbFirstPixelInRow0(0),
#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	m_ppbIliffe(0),
	m_pbOriginIliffe(0),
#endif // VIS_CORE_IMAGE_USE_ILIFFE
	m_strName(),
    m_pVisStreamHandler(NULL),
	m_fUseColorMap(false),
	m_fDirty(false),
	m_fDelayRead(false),
	m_memblockColorMap(),
	m_imopts(DefaultOptions()),
	m_hdc(0),
	m_hbitmapOld(0)
{
	assert(0);

	// UNDONE:  We should also throw an exception here, since this function
	// should not be called.
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc:(IMPL) CVisImageBase& | CVisImageBase | operator= |
//
// Assignment operator.
//
// @parm const | CVisImageBase& refImage |
// Reference to the image whosh information should be assigned to this
// image.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport CVisImageBase& CVisImageBase::operator=(const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	g_imagecritsect.Enter();
	Assign(refimage);
	g_imagecritsect.Leave();
	return *this;
}


VisCoreExport void CVisImageBase::Cleanup(void)
{
	// We assume that some members are contiguous in the ShapeInvalid
	// method where we may need to cast a group of members to a
	// CVisShape object whose members are all equal to zero.
	assert(((int) (const void *) &m_cbRow)
			== ((int) (const void *) (&m_cbCol + 1)));
	assert(((int) (const void *) &m_pbDataOrigin)
			== ((int) (const void *) (&m_cbRow + 1)));
	assert(((int) (const void *) &m_pbFirstPixelInRow0)
			== ((int) (const void *) (&m_pbDataOrigin + 1)));
	assert(((int) (const void *) &m_hdc)
			== ((int) (const void *) (&m_pbFirstPixelInRow0 + 1)));
	assert(((int) (const void *) &m_hbitmapOld)
			== ((int) (const void *) (&m_hdc + 1)));

	g_imagecritsect.Enter();

	Deallocate();

    if( m_pVisStreamHandler )
    {
        m_pVisStreamHandler->Release();
    }

	g_imagecritsect.Leave();

}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc void * * | CVisImageBase | PpvFillIliffeVector |
//
// Fill an Iliffe vector.  A pointer to the start of the Iliffe
// vector memory is passed to this function.  Pointers to column
// zero in each image row are placed in the Iliffe vector.  A
// pointer that can be indexed by image row is returned.
//
// @syntax TPixel **PpvFillIliffeVector(void **ppvIliffe) const;
// @syntax TPixel **PpvFillIliffeVector(void **ppvIliffe, int yFirst, int yLim)
//		const;
//
// @this const
//
// @parm int | yFirst |
// The first row to store in the Iliffe vector.  (The "top" row if not
// specified.)
//
// @parm int | yLim |
// The limit row for the Iliffe vector.  Rows between <p yFirst> and
// <p yLim>, including <p yFirst> but not including <p yLim>, will be
// stored in the Iliffe vector.  (The "bottom" row if not specified.)
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void **CVisImageBase::PpvFillIliffeVector(void **ppvIliffe,
		int yFirst, int yLim) const
{
//	assert(IsValid());
	assert(ppvIliffe != 0);
	assert(yFirst <= yLim);
	assert(yFirst >= m_shapeMemBlock.Top());
	assert(yLim <= m_shapeMemBlock.Bottom());

	void **ppvBase = ppvIliffe - yFirst;
	BYTE *pbCur = m_pbDataOrigin + m_cbRow * yFirst;
	for (int y = yFirst; y < yLim; ++y, pbCur += m_cbRow)
		ppvBase[y] = pbCur;

	return ppvBase;
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisImageBase | Name |
//
// Get a name associated with an image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport const char *CVisImageBase::Name(bool fIncludePath) const
{
	const char *szName = m_strName.c_str();

	if ((szName != 0) && (szName[0] != 0) && (!fIncludePath))
	{
		int cch = strlen(szName);
		assert(cch > 0);

		for (int ich = cch - 1; ich > 0; --ich)
		{
			if ((szName[ich] == '/') || (szName[ich] == '\\'))
			{
				szName += (ich + 1);
				break;
			}
		}
	}
	return szName;
}

//
//  Publically visible file read/write routines
//

// @mfunc Reads image from a file (calls to ReadWriteFile).
VisCoreExport void CVisImageBase::ReadFile(
                         const char *szFilename)	// @parm File name to read image from.
{
    SVisFileDescriptor filedescriptorT;
	ZeroMemory(&filedescriptorT, sizeof(SVisFileDescriptor));
    filedescriptorT.has_alpha_channel = IsAlphaWritten();

	filedescriptorT.filename = szFilename;

	ReadFile(filedescriptorT);
}

// @mfunc Writes image to a file with the given format.
VisCoreExport void CVisImageBase::WriteFile(
                          const char *szFilename,		// @parm File name.
						  const char *szFormat)		// @parm File format to write
{
    SVisFileDescriptor filedescriptorT;
	ZeroMemory(&filedescriptorT, sizeof(SVisFileDescriptor));
    filedescriptorT.has_alpha_channel = IsAlphaWritten();

	filedescriptorT.filename = szFilename;
    filedescriptorT.filetype = szFormat;

    WriteFile(filedescriptorT);
}


// @mfunc Reads image from a file (calls to ReadWriteFile).
VisCoreExport bool CVisImageBase::FReadFile(
                        const char *szFilename,	// @parm File name to read image from.
						bool fDisplayErrorMessage)
{
    SVisFileDescriptor filedescriptorT;
	ZeroMemory(&filedescriptorT, sizeof(SVisFileDescriptor));
    filedescriptorT.has_alpha_channel = IsAlphaWritten();

	filedescriptorT.filename = szFilename;

	return FReadFile(filedescriptorT, fDisplayErrorMessage);
}

// @mfunc Writes image to a file with the given format.
VisCoreExport bool CVisImageBase::FWriteFile(
								const char *szFilename,		// @parm File name.
								const char *szFormat,		// @parm File format to write
								bool fDisplayErrorMessage)
{
    SVisFileDescriptor filedescriptorT;
	ZeroMemory(&filedescriptorT, sizeof(SVisFileDescriptor));
    filedescriptorT.has_alpha_channel = IsAlphaWritten();

	filedescriptorT.filename = szFilename;
    filedescriptorT.filetype = szFormat;

	return FWriteFile(filedescriptorT, fDisplayErrorMessage);
}

VisCoreExport void CVisImageBase::SetStreamHandler( CVisStreamHandler* pStrmHandler,
                                                    UINT frame_num )
{
    char img_name[MAX_PATH];

    if( pStrmHandler )
    {
        pStrmHandler->AddRef();
    }

    if( m_pVisStreamHandler )
    {
        m_pVisStreamHandler->Release();
    }
    
    m_pVisStreamHandler = pStrmHandler;
    m_uStreamFrameNum   = frame_num;

    if( pStrmHandler )
    {
        sprintf(img_name,"%s[%d]", pStrmHandler->FileName(), frame_num);
        SetName(img_name);
    }
    else
    {
        SetName("");
    }
}




// @mfunc Reads/writes image file.
VisCoreExport bool CVisImageBase::RealReadWriteFile(
                                                   SVisFileDescriptor& fd,   // @parm File descriptor.
                                                   bool fWrite,              // @parm Write flag (if 1 do write, if 0 do read).
                                                   bool fThrowError,     // @parm if 1 throw error, if 0 return false after error.
                                                   bool fDisplayErrorMessage)// @parm if 1 display error message, if 0 don't.
{
    // LATER:  Consider changes to the file handler interface that would
    // pass errors up to this function instead throwing errors in the
    // file handler methods.
    bool fRet = true;

    char errmsg[1024];

    try
	{
        if ( m_pVisStreamHandler && strcmp(fd.filename, Name(true))==0 )
        {
            BOOL bSuccess;
            UINT uMode = m_pVisStreamHandler->Mode(); 
            if( fWrite )
            {
                if( !(uMode & OF_WRITE) && !(uMode & OF_READWRITE)  )
                {
                    sprintf(errmsg, "stream not open for writing");
                    bSuccess = FALSE;
                }
                else
                {
                    bSuccess = m_pVisStreamHandler->WriteFrame(*this, m_uStreamFrameNum);
                    if( !bSuccess )
                    {
                        sprintf(errmsg, "WriteFrame failed");
                    }
                }
            }
            else
            {

                if( (uMode & OF_WRITE) && !(uMode & OF_READWRITE) )
                {
                    sprintf(errmsg, "stream not open for reading");
                    bSuccess = FALSE;
                }
                else
                {
                    bSuccess = m_pVisStreamHandler->ReadFrame(*this, m_uStreamFrameNum);
                    if( !bSuccess )
                    {
                        sprintf(errmsg, "ReadFrame failed");
                    }
                }
            }
                
            if( !bSuccess )
            {                
                if (fThrowError)
                {
                    throw CVisFileIOError(m_pVisStreamHandler->FileName(), errmsg, eviserrorOpen,
                                          "CVisImageBase::ReadWriteFile", __FILE__);
                }
                else if (fDisplayErrorMessage)
                {
                    CVisDiagnostics::Warning(errmsg);
                }

                return false;
            }
        }
        else
        {
            // Check for valid filename
            if (fd.filename == 0 || fd.filename[0] == 0 ||
				fd.filename[strlen(fd.filename)-1] == ']' )
            {
                if (fThrowError)
                {
                    throw CVisFileIOError("", "Bad filename given",
                                          eviserrorNoFilename, "",
                                          __FILE__);
                }
                else
                {
                    if (fDisplayErrorMessage)
                        CVisDiagnostics::Warning("No filename given in CVisImageBase::ReadWriteFile");

                    return false;
                }
            }

            // UNDONE:  We need an option to indicate whether to do a progress report here.
            // (Sequences and other code that read files will also need these options.)
            if (fWrite)
                CVisDiagnostics::ProgressReport("Writing %s", fd.filename);
            else
                CVisDiagnostics::ProgressReport("Reading %s", fd.filename);

            // Determine the extension, if none is given
            if (fd.filetype == 0)
            {
                char *dot = strrchr(fd.filename, '.');
                fd.filetype = (dot) ? dot+1 : fd.filename;
            }

            // Find a handler, and make sure it handles the image type
            CVisFileHandler *handler = CVisFileHandler::Handler(fd.filetype);
            if ((handler == 0) && fWrite)
            {
				sprintf(errmsg, "No file i/o handler for filetype %s", fd.filetype);

				if (fThrowError)
				{
					throw CVisFileIOError(fd.filename, errmsg, eviserrorOpen,
										  "CVisImageBase::ReadWriteFile", __FILE__);
				}
				else
				{
					if (fDisplayErrorMessage)
						CVisDiagnostics::Warning(errmsg);

					return false;
				}
            }
            if ( (handler != 0) && (! handler->SupportsPixelType(PixFmt(), NBands())))
            {
                sprintf(errmsg, "File i/o handler for filetype %s does not support %s pixels (%d bands)\n",
                        fd.filetype, PixelTypeInfo().name(), NBands());

                if (fThrowError)
                {
                    throw CVisFileIOError(fd.filename, errmsg, eviserrorOpen,
                                          "CVisImageBase::ReadWriteFile", __FILE__);
                }
                else
                {
                    if (fDisplayErrorMessage)
                        CVisDiagnostics::Warning(errmsg);

                    return false;
                }
            }

            // Optionally open the file for input/output
            if ((handler != 0) && (handler->ExternallyOpenStream()))
            {
                const char *filename = fd.filename;
                if (filename == 0 || filename[0] == 0 || strcmp(filename, "-") == 0)
                    fd.stream = (fWrite) ? stdout : stdin;
                else
                    fd.stream = fopen(filename, (fWrite) ? "wb" : "rb");
                if (fd.stream == 0)
                {
                    sprintf(errmsg, "Could not open the file for %s", 
                            (fWrite) ? "writing" : "reading");

                    if (fThrowError)
                    {
                        throw CVisFileIOError(fd.filename, errmsg, eviserrorOpen,
                                              "CVisImageBase::ReadWriteFile", __FILE__);
                    }
                    else
                    {
                        if (fDisplayErrorMessage)
                            CVisDiagnostics::Warning(errmsg);

                        return false;
                    }
                }
            }

            // Special code for JPEG
            if (fd.jpeg_quality == 0 && fd.default_jpeg_quality > 0)
                fd.jpeg_quality = fd.default_jpeg_quality;

            // UNDONE (in Rel1):  We should add a try-catch block here to make sure that
            // we close the stream if there is an exception.

            // Read/write the header
            int return_val1;
			if(handler != 0)
			{
				return_val1 = (fWrite) ?
							  handler->WriteHeader(fd, *this) :
							  handler->ReadHeader(fd, *this);
			}
			else
			{
				CVisFileHandler* pPictureHandler = PGlobals()->PictureFileHandler();
				return_val1 = pPictureHandler->ReadHeader(fd, *this);
			}
			if (return_val1)
			{
				sprintf(errmsg, "Could not open for %s or failed to %s header\n",
						(fWrite) ? "writing" : "reading", (fWrite) ? "write" : "read");

				if (fThrowError)
				{
					throw CVisFileIOError(fd.filename, errmsg, eviserrorOpen,
										  "CVisImageBase::ReadWriteFile", __FILE__);
				}
				else
				{
					if (fDisplayErrorMessage)
						CVisDiagnostics::Warning(errmsg);

					return false;
				}
			}
			// Variables used to remember whether the image shape needs to be
			// cropped after reading.  (This is a special case used with delayed
			// reading in sequences).
			bool fCropAfterReading = false;
			RECT rectCrop;

            // Allocate the space, if necessary;  check for consistency
            if ((!fWrite) && (NPixels() == 0))
            {
				CVisShape shapeT(fd.left, fd.top, fd.left + fd.cols,
						fd.top + fd.rows, fd.bands);
				AllocateUsingPrevoiusShapeIfPossible(shapeT);
				if (((Width() != fd.cols) || (Height() != fd.rows))
						&& ((MemoryWidth() >= fd.cols) || (MemoryHeight() >= fd.rows)))
				{
					// Remember the desired RECT and temporarily change the image
					// rect to match the dimensions of the image that we're reading.
					fCropAfterReading = true;
					rectCrop = Rect();
					SetRect(m_shapeMemBlock.left, m_shapeMemBlock.top,
							m_shapeMemBlock.left + fd.cols,
							m_shapeMemBlock.top + fd.rows);
				}
            }
            if (! (fWrite ||
                   (fd.rows == Height() && fd.cols == Width() &&
                    fd.bands == NBands() && fd.evispixfmt == PixFmt())))
            {
                sprintf(errmsg, "Image shape/type not consistent with file: %d x %d x %d (%d) vs. %d x %d x %d (%d)\n",
                        Width(), Height(), NBands(), PixFmt(),
                        fd.cols, fd.rows, fd.bands, fd.evispixfmt);

				// Don't crop the image if there's an error.
				fCropAfterReading = false;

                if (fThrowError)
                {
                    throw CVisFileIOError(fd.filename, errmsg, eviserrorShape,
                                          "CVisImageBase::ReadWriteFile", __FILE__);
                }
                else
                {
                    if (fDisplayErrorMessage)
                        CVisDiagnostics::Warning(errmsg);

                    return false;
                }
            }

            // Read/write the body
            // LATER:  It appears that return_val2 is not used below.  Is that intentional?
            int return_val2;
			if(handler != 0)
			{
				return_val2 = (fWrite) ? 
                              handler->WriteBody(fd, *this) :
                              handler->ReadBody(fd, *this);
			}
			else
			{
				CVisFileHandler* pPictureHandler = PGlobals()->PictureFileHandler();
				return_val1 = pPictureHandler->ReadBody(fd, *this);
				PGlobals()->ReleasePictureFileHandler();
			}

			// Special cropping that may be needed with delayed reading.
			if (fCropAfterReading)
				SetRect(rectCrop);

            // Optionally close the stream
            if ((handler != 0) &&
				handler->ExternallyOpenStream() &&
                fd.stream != stdin && fd.stream != stdout)
                fclose(fd. stream);
        }

        // If the (read or write) operation succeeded, clear the "dirty" flag.
        SetIsDirty(false);
    }
    catch (CVisError& error)
    {
        if (fThrowError)
            error.ThrowLast();

        if (fDisplayErrorMessage)
            CVisDiagnostics::Warning(error.FullMessage());

        fRet = false;
    }
    catch (...)
    {
        if (fThrowError)
            throw;

        if (fDisplayErrorMessage)
            CVisDiagnostics::Warning("Unexpected Error");

        fRet = false;
    }

    return fRet;
}

/*
VisCoreExport const char *CVisImageBase::ReadWriteField(CVisSDStream& s, int field_id)
{
#ifdef VIS_INCLUDE_SDSTREAM

    switch (FieldType(field_id))
	{
    case eftName:
        return "class CVisImageBase {";
		
	case eftPixFmt:
		if (s.Status() == CVisSDStream::Read)
		{
			int w;
			s.Num(w, true);
			assert(m_evispixfmt == w);
			if (m_evispixfmt != w)
			{
				throw CVisError(
					"Pixel type mismatch when reading image information",
					eviserrorTypeMismatch, "CVisImageBase::ReadWriteField",
					__FILE__, __LINE__);
			}
		}
		else
		{
			s.NewLine();
			assert(sizeof(EVisPixFmt) == sizeof(int));
			s.Num((int&) m_evispixfmt, true);
		}
		return "EVisPixFmt m_evispixfmt;  // Pixel type";
		
    case eftShape:
		if (s.Status() == CVisSDStream::Read)
		{
			// We don't read the image from the self-describing stream,
			// but we will read its file name and timestamp below.  Here
			// we clear any previous image to indicate that we still need
			// to read this image from a file.
			Deallocate();
			
			// Still need to read a CVisShape from the stream.
			CVisShape shapeT;
			s << shapeT;
		}
		else
		{
			s.NewLine();
			s << m_shapeImage;
		}
        return "CVisShape m_shape;    // Dimensions and number of bands";
		
    case eftFilename:
		s.NewLine();
		s << m_strName;
		return "std::string m_strFilename;    // File name";
		
    case eftProperties:
		s.NewLine();
		s.OpenParen();
		s.NewLine();
		SDReadWriteProperties(s);
		// UNDONE (in Rel1):  Skip extra fields until we get to the next brace.
		// UNDONE (in Rel1):  Should the (derived) class name be written to the file.
		s.CloseParen();
		return "std::list<void *> m_listProps;  // Properties";
    }
	return 0;
#else
	assert(false);  //SDStreams are not included
	return 0;
#endif
}

*/
void CVisImageBase::PreReadValue(CVisSDIStream& refsdistream)
{
	Deallocate();
}

void CVisImageBase::PostReadValue(CVisSDIStream& refsdistream)
{
	// UNDONE:  The code below is not right.  We've changed the code to remember
	// the memory and image shapes after Deallocate is called.  We should change
	// code below to allow the memory and image shapes to be stored in a file.

	// Clear the memory shape.
	m_shapeMemBlock.left = 0;
	m_shapeMemBlock.right = 0;
	m_shapeMemBlock.top = 0;
	m_shapeMemBlock.bottom = 0;
	m_shapeMemBlock.SetNBands(0);
	
	// Remember the shape that we read (for use below).  Then set m_shapeImage
	// to be an empty RECT (using m_shapeMemBlock, which we checked in the
	// assert above.)
	CVisShape shapeT(m_shapeImage);
	m_shapeImage = m_shapeMemBlock;
	
	// We may have read the (active) image dimensions from the file.  Save the
	// dimensions for use after we read the image.
	if ((!DelayRead()) && (Name(true) != 0) && (*Name(true) != 0) &&
        Name()[strlen(Name())-1] != ']' )
	{
		// LATER:  We might want to add code to allocate a temporary image if no
		// file name was given.
		ReadFile();
		
		// Change the image dimensions if needed.
		if ((IsValid()) && (shapeT.Width() != 0) && (shapeT.Height() != 0))
		{
			assert(shapeT.NBands() == Shape().NBands());
			if (Rect() != shapeT)
			{
				assert(Width() >= shapeT.Width());
				assert(Height() >= shapeT.Height());
				if ((Left() > shapeT.Left())
						|| (Right() < shapeT.Right())
						|| (Top() > shapeT.Top())
						|| (Bottom() < shapeT.Bottom()))
				{
					// If the shape specified is not contained in the shape of
					// the image that we read from the file, we need to offset
					// the origin.
					// LATER:  We could still have problems if we write a subimage
					// of an image that has an offset origin, because the shape
					// of the offset subimage might be contained in the shape of
					// the unoffset image.  To handle that case, we may want to
					// write the shape of the image memory with the image
					// description and use the image memory shape to offset the
					// image, if needed.
					ResetOrigin(shapeT.Left(), shapeT.Top());
				}
				
				// Set the shape of the image to agree with the shape specified
				// in the image description.
				SetRect(shapeT);
			}
		}
	}
}

void CVisImageBase::PreWriteValue(CVisSDOStream& refsdostream) const
{
	// Write data if dirty
	if ((IsDirty()) && (Name(true) != 0) && (*Name(true) != 0))
	{
		assert(IsValid());
		(const_cast<CVisImageBase *>(this))->WriteFile();
		
		// WriteFile should clear the dirty bit.
		assert(!IsDirty());
	}
}

void CVisImageBase::PostWriteValue(CVisSDOStream& refsdostream) const
{
}

void CVisImageBase::ReadValue(CVisSDIStream& refsdistream,
			bool fForceDelayRead)
{
	PreReadValue(refsdistream);

	EVisPixFmt pixfmtPrev = PixFmt();

#ifdef VIS_NO_SD_STREAM
	assert(0);
#else // VIS_NO_SD_STREAM
	refsdistream.ReadClassUsingPropList(*this);
#endif // VIS_NO_SD_STREAM

	if (PixFmt() != pixfmtPrev)
	{
		// LATER:  Should this be a CVisFileIOError?
		throw CVisError(
				"Image pixel type mismatch when reading from a stream.",
				eviserrorPixFmt, "CVisImageBase::ReadValue",
				__FILE__, __LINE__);
	}

	if (fForceDelayRead)
		m_fDelayRead = true;

	PostReadValue(refsdistream);
}

void CVisImageBase::WriteValue(CVisSDOStream& refsdostream) const
{
	PreWriteValue(refsdostream);
#ifdef VIS_NO_SD_STREAM
	assert(0);
#else // VIS_NO_SD_STREAM
	refsdostream.WriteClassUsingPropList(*this);
#endif // !VIS_NO_SD_STREAM
	PostWriteValue(refsdostream);
}

void CVisImageBase::BuildPropList(CVisPropList& refproplist)
{
	assert(refproplist.IsEmpty());
	refproplist.AddPropReference("m_shapeImage", m_shapeImage, false);

	// LATER:  Add macros to define VisGetPropTypeInfo and VisPPropTypeInfo
	// functions for enum types that return int type information (so we don't
	// need to cast enums to int references when adding them to prop lists).
	refproplist.AddPropReference("m_evispixfmt",
			(unsigned int &) m_evispixfmt, false);
	refproplist.AddPropReference("m_strName", m_strName, false);

	// Add m_fDelayRead a property that is not printed by default.  (If we print
	// it, we can control whether images are read when the file is read.)
	refproplist.AddPropReference("m_fDelayRead", m_fDelayRead, false);
	refproplist.SetPrinting("m_fDelayRead", false);

	// LATER:  Add m_filetime as a property that is not printed by default?

	// LATER:  Add m_imopts as a property that is not printed by default?

	// The name VIS_SZ_SD_USER_PROP_LIST indicates a special class member
	// that stores properties added by the user.
	refproplist.AddPropReference(VIS_SZ_SD_USER_PROP_LIST, m_proplist, false);
}

// UNDONE:  Make these pure virtual functions when the old SDStream code is no
// longer used.
CVisPropTypeInfoBase *CVisImageBase::PropTypeInfoLookup(void) const
{
	// LATER:  Make this a pure virtual method after the old SDStream code is removed.
	assert(0);
	return 0;
}

CVisPropTypeInfoBase *CVisImageBase::PropTypeInfoNew(void) const
{
	// LATER:  Make this a pure virtual method after the old SDStream code is removed.
	assert(0);
	return 0;
}

#ifdef VIS_INCLUDE_SDSTREAM
inline CVisSDObject< CVisImageBase >&
	CVisSDObject< CVisImageBase >::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject< CVisImageBase > g_visimagebaseExemplar;

	return g_visimagebaseExemplar;
}
#endif // VIS_INCLUDE_SDSTREAM

VIS_DEFINE_PROP_LIST_CLASS_VIRTUAL(VisCoreExportDeclspec, CVisImageBase)


VIS_DEFINE_SDI_FIND_TYPES_PL(VisCoreExportDeclspec, CVisImageBase)
VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_METHOD(VisCoreExportDeclspec, CVisImageBase)
VIS_DEFINE_SDI_READ_OBJ_DEFAULT(VisCoreExportDeclspec, CVisImageBase)
VIS_DEFINE_SDI_READ_OP_DEFAULT(VisCoreExportDeclspec, CVisImageBase)

VIS_DEFINE_SDO_FIND_TYPES_PL(VisCoreExportDeclspec, CVisImageBase)
VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_METHOD(VisCoreExportDeclspec, CVisImageBase)
VIS_DEFINE_SDO_WRITE_OBJ_DEFAULT(VisCoreExportDeclspec, CVisImageBase)
VIS_DEFINE_SDO_WRITE_OP_DEFAULT(VisCoreExportDeclspec, CVisImageBase)




// Helper function declared in VisImage.h.

VisCoreExport CVisImage<BYTE> VisBrightnessFromRGBA(
		const CVisImage<CVisRGBA<BYTE> >& refimageRGBA)
{
	CVisImage<BYTE> imageGray;

	assert(&refimageRGBA != 0);
	if (refimageRGBA.IsValid())
	{
		imageGray.Allocate(refimageRGBA.Shape());

		VisMap2(CVisFnOpRGBAIntensity(), imageGray,
				const_cast<CVisImage<CVisRGBA<BYTE> >&>(refimageRGBA));
	}

	return imageGray;
}

// Move to VisImageBase.cpp

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisImage<BYTE> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<int> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisImage<int> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<float> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisImage<float> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<double> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisImage<double> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisImage<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisImage<CVisRGBA<int> > >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisImage<CVisRGBA<float> > >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisImage<CVisRGBA<double> > >(0, fAlwaysTypedef); }

