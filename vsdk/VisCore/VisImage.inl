///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGEOF
//
// @module VisImageOf.inl |
//
// This file defines the inline methods for the typed image classes.
// These classes support two-dimensional, optionally multi-banded,
// arrays of pixels of any C++ or user-defined type.
//
// The size of an image is set when an image is constructed.  Subimages
// which share the same memory can be constructed from images.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @todo:(INTERNAL)
// Padding methods are not implemented yet.
//
// Need to make these methods inline
//
// <nl>
//
// @xref <l VisImageOf\.h> <l VisImage\.h> <l VisImage\.inl>
// <l VisImage\.cpp> <l VisFileIO\.cpp>
//
// <nl>
// @index | VISIMAGEOF
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_IMAGEOF_INL__
#define __VIS_CORE_IMAGEOF_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "VisEnumPixel.h"


#include "VisCopyStandardPixels.h"


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisImage | CVisImage |
//
// Constructor.
//
// @syntax CVisImage(void);
// @syntax CVisImage(int width, int height, int nbands = 1,
//		int imopts = evisimoptDefault, BYTE *pbData = 0);
// @syntax CVisImage(SIZE size, int nbands = 1,
//		int imopts = evisimoptDefault, BYTE *pbData = 0);
// @syntax CVisImage(const RECT& rect, int nbands = 1,
//		int imopts = evisimoptDefault, BYTE *pbData = 0);
// @syntax CVisImage(const CVisShape& shape,
//		int imopts = evisimoptDefault, BYTE *pbData = 0);
// @syntax CVisImage(CVisMemBlock& refmemblock, int width, int height,
//		int nbands = 1, int imopts = evisimoptDefault);
// @syntax CVisImage(CVisMemBlock& refmemblock, SIZE size, int nbands = 1,
//		int imopts = evisimoptDefault);
// @syntax CVisImage(CVisMemBlock& refmemblock, const RECT& rect,
//		int nbands = 1, int imopts = evisimoptDefault);
// @syntax CVisImage(CVisMemBlock& refmemblock, const CVisShape& shape,
//		int imopts = evisimoptDefault);
// @syntax CVisImage(const CVisImage& imageSrc);
//
// @parm int | width |
// The width of the image.  (When the width is given in the constructor,
// the leftmost image column will have x coordinate zero.)
//
// @parm int | height |
// The height of the image.  (When the height is given in the constructor,
// the topmost image row will have y coordinate zero.)
//
// @parm SIZE | size |
// Windows SIZE structure giving the image dimensions.  (The left column
// will have x coordinate zero and the top row will have y coordinate zero.)
//
// @parm const RECT& | rect |
// Windows RECT structure giving the image coordinates of the left column,
// top row, right column, and bottom row of the image.  The left column
// and top row are included in the image, but the right column and bottom
// row are not included in the image.
//
// @parm const CVisShape& | shape |
// A CVisShape object giving  the image coordinates of the left column,
// top row, right column, and bottom row of the image ang giving the number
// of bands in the image.  The left column and top row are included in the
// image, but the right column and bottom row are not included in the image.
//
// @parmopt int | nbands | 1 |
// The number of bands in the image.
//
// @parmopt int | imopts | evisimoptDefault |
// Options to use when constructing this image.
//
// @parmopt BYTE * | pbData | 0 |
// Pointer to pixel information to use in this image.
//
// @parm const CVisMemBlock& | refmemblock |
// Memory block containing pixel information to use in this image.
//
// @parm const CVisImage& | imageSrc |
// Image to copy.  (Used with the copy constructor).
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline CVisImage<TPixel>::CVisImage(void)
  : CVisImageBase(sizeof(TPixel), StaticPixFmt())
{
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(int width, int height,
		int nbands, int imopts, BYTE *pbData)
  : CVisImageBase(sizeof(TPixel), StaticPixFmt(), CVisRect(0, 0, width, height),
		nbands, imopts, pbData)
{
	if ((pbData == 0)
			&& ((imopts & evisimoptMemAllocMask) == evisimoptMemObjType))
	{
		SetMemBlockFromImopts(imopts);
	}
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(SIZE size,
		int nbands, int imopts, BYTE *pbData)
  : CVisImageBase(sizeof(TPixel), StaticPixFmt(), CVisRect(0, 0, size.cx, size.cy),
		nbands, imopts, pbData)
{
	if ((pbData == 0)
			&& ((imopts & evisimoptMemAllocMask) == evisimoptMemObjType))
	{
		SetMemBlockFromImopts(imopts);
	}
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(const RECT& rect,
		int nbands, int imopts, BYTE *pbData)
  : CVisImageBase(sizeof(TPixel), StaticPixFmt(), rect, nbands, imopts, pbData)
{
	if ((MemBlock().PbData() == 0)
			&& ((imopts & evisimoptMemAllocMask) == evisimoptMemObjType))
	{
		SetMemBlockFromImopts(imopts);
	}
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(const CVisShape& shape,
		int imopts, BYTE *pbData)
  : CVisImageBase(sizeof(TPixel), StaticPixFmt(), shape, shape.NBands(), imopts, pbData)
{
	if ((MemBlock().PbData() == 0)
			&& ((imopts & evisimoptMemAllocMask) == evisimoptMemObjType))
	{
		SetMemBlockFromImopts(imopts);
	}
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(CVisMemBlock& refmemblock,
		int width, int height, int nbands, int imopts)
  : CVisImageBase(refmemblock, sizeof(TPixel), StaticPixFmt(),
		CRect(0, 0, width, height), nbands, imopts)
{
	if ((MemBlock().PbData() == 0)
			&& ((imopts & evisimoptMemAllocMask) == evisimoptMemObjType))
	{
		SetMemBlockFromImopts(imopts);
	}
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(CVisMemBlock& refmemblock,
		SIZE size, int nbands, int imopts)
  : CVisImageBase(refmemblock, sizeof(TPixel), StaticPixFmt(),
		CVisRect(0, 0, size.cx, size.cy), nbands, imopts)
{
	if ((MemBlock().PbData() == 0)
			&& ((imopts & evisimoptMemAllocMask) == evisimoptMemObjType))
	{
		SetMemBlockFromImopts(imopts);
	}
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(CVisMemBlock& refmemblock,
		const RECT& rect, int nbands, int imopts)
  : CVisImageBase(refmemblock, sizeof(TPixel), StaticPixFmt(), rect,
		nbands, imopts)
{
	if ((MemBlock().PbData() == 0)
			&& ((imopts & evisimoptMemAllocMask) == evisimoptMemObjType))
	{
		SetMemBlockFromImopts(imopts);
	}
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(CVisMemBlock& refmemblock,
		const CVisShape& shape, int imopts)
  : CVisImageBase(refmemblock, sizeof(TPixel), StaticPixFmt(), shape,
		shape.NBands(), imopts)
{
	if ((MemBlock().PbData() == 0)
			&& ((imopts & evisimoptMemAllocMask) == evisimoptMemObjType))
	{
		SetMemBlockFromImopts(imopts);
	}
}

template <class TPixel>
inline CVisImage<TPixel>::CVisImage(const CVisImage<TPixel>& refimageSrc)
  : CVisImageBase(sizeof(TPixel), StaticPixFmt())
{
	// Use operator= to synchronize image creation, assignment, and
	// destruction.
	// Warning:  Derived classes should provide their own copy constructor
	// that first constructs a default object and then uses the assignment
	// operator to synchronize  image creation, assignment, and
	// destruction.  Derived classes should not call the copy constructor
	// in the base class.
	*this = refimageSrc;
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisImage& | CVisImage | operator= |
//
// Assignment operator.
//
// @parm const CVisImage& | imageSrc |
// The image to copy.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline CVisImage<TPixel>& CVisImage<TPixel>::operator=(
		const CVisImage<TPixel>& imageSrc)
{
	((CVisImageBase&) *this) = ((const CVisImageBase&) imageSrc);

	return *this;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TPixel& | CVisImage | Pixel |
//
// Given the coordinates of a point in the image, find the value
// of the pixel (in the first image band) with the specified
// coordinates.  In the debug builds, this will assert that the
// pixel coordinates are in the image rectangle, but this function
// may return an invalid pixel address if the pixel coordinates
// are not in the image rectangle.
//
// @syntax const TPixel& Pixel(POINT point) const;
// @syntax const TPixel& Pixel(int x, int y) const;
// @syntax const TPixel& Pixel(POINT point, int band) const;
// @syntax const TPixel& Pixel(int x, int y, int band) const;
// @syntax TPixel& Pixel(POINT point);
// @syntax TPixel& Pixel(int x, int y);
// @syntax TPixel& Pixel(POINT point, int band);
// @syntax TPixel& Pixel(int x, int y, int band);
//
// @parm POINT | point |
// Windows <t POINT> structure giving the location of the pixel.
//
// @parm int | x |
// Column of the pixel.
//
// @parm int | y |
// Row of the pixel.
//
// @parm int | band |
// Band of the pixel.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline const TPixel& CVisImage<TPixel>::Pixel(int x, int y) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return RowPointer(y)[x];
}

template <class TPixel>
inline TPixel &CVisImage<TPixel>::Pixel(int x, int y)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return RowPointer(y)[x];
}

template <class TPixel>
inline const TPixel& CVisImage<TPixel>::Pixel(int x, int y, int band)
		const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y, band));
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return RowPointer(y)[x * NBands() + band];
}

template <class TPixel>
inline TPixel& CVisImage<TPixel>::Pixel(int x, int y, int band)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y, band));
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return RowPointer(y)[x * NBands() + band];
}

template <class TPixel>
inline const TPixel& CVisImage<TPixel>::Pixel(POINT point) const
{
	return Pixel(point.x, point.y);
}

template <class TPixel>
inline TPixel& CVisImage<TPixel>::Pixel(POINT point)
{
	return Pixel(point.x, point.y);
}

template <class TPixel>
inline const TPixel& CVisImage<TPixel>::Pixel(POINT point, int band) const
{
	return Pixel(point.x, point.y, band);
}

template <class TPixel>
inline TPixel& CVisImage<TPixel>::Pixel(POINT point, int band)
{
	return Pixel(point.x, point.y, band);
}


template <class TPixel>
inline const TPixel& CVisImage<TPixel>::operator()(int x, int y) const
{
	return Pixel(x, y);
}

template <class TPixel>
inline const TPixel& CVisImage<TPixel>::operator()(POINT point) const
{
	return Pixel(point);
}

template <class TPixel>
inline TPixel& CVisImage<TPixel>::operator()(int x, int y)
{
	return Pixel(x, y);
}

template <class TPixel>
inline TPixel& CVisImage<TPixel>::operator()(POINT point)
{
	return Pixel(point);
}

template <class TPixel>
inline const TPixel& CVisImage<TPixel>::operator()(int x, int y, int band) const
{
	return Pixel(x, y, band);
}

template <class TPixel>
inline const TPixel& CVisImage<TPixel>::operator()(POINT point, int band) const
{
	return Pixel(point, band);
}

template <class TPixel>
inline TPixel& CVisImage<TPixel>::operator()(int x, int y, int band)
{
	return Pixel(x, y, band);
}

template <class TPixel>
inline TPixel& CVisImage<TPixel>::operator()(POINT point, int band)
{
	return Pixel(point, band);
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisImage | PixelAddress |
//
// Given the coordinates of a point in the image, find the address
// of the pixel with the specified
// coordinates.  In the debug builds, this will assert that the
// pixel coordinates are in the image rectangle, but this function
// may return an invalid pixel address if the pixel coordinates
// are not in the image rectangle.  If the image has more than one
// band, the caller must specify the band.
//
// @syntax const TPixel *PixelAddress(POINT point) const;
// @syntax const TPixel *PixelAddress(int x, int y) const;
// @syntax const TPixel *PixelAddress(POINT point, int band) const;
// @syntax const TPixel *PixelAddress(int x, int y, int band) const;
// @syntax TPixel *PixelAddress(POINT point);
// @syntax TPixel *PixelAddress(int x, int y);
// @syntax TPixel *PixelAddress(POINT point, int band);
// @syntax TPixel *PixelAddress(int x, int y, int band);
//
// @parm POINT | point |
// Windows <t POINT> structure giving the location of the pixel.
//
// @parm int | x |
// Column of the pixel.
//
// @parm int | y |
// Row of the pixel.
//
// @parm int | band |
// Band of the pixel.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline const TPixel *CVisImage<TPixel>::PixelAddress(POINT point) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(point));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return &(Pixel(point));
}

template <class TPixel>
inline const TPixel *CVisImage<TPixel>::PixelAddress(int x, int y) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return &(Pixel(x, y));
}

template <class TPixel>
inline const TPixel *CVisImage<TPixel>::PixelAddress(POINT point, int band)
		const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(point, band));
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return &(Pixel(point, band));
}

template <class TPixel>
inline const TPixel *CVisImage<TPixel>::PixelAddress(int x, int y, int band)
		const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y, band));
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return &(Pixel(x, y, band));
}

template <class TPixel>
inline TPixel *CVisImage<TPixel>::PixelAddress(POINT point)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(point));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS

	return &(Pixel(point));
}

template <class TPixel>
inline TPixel *CVisImage<TPixel>::PixelAddress(int x, int y)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return &(Pixel(x, y));
}

template <class TPixel>
inline TPixel *CVisImage<TPixel>::PixelAddress(POINT point, int band)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(point, band));
#endif // VIS_CORE_EXTRA_ASSERTS

	return &(Pixel(point, band));
}

template <class TPixel>
inline TPixel *CVisImage<TPixel>::PixelAddress(int x, int y, int band)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y, band));
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return &(Pixel(x, y, band));
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TPixel * | CVisImage | RowPointer |
//
// Given a row containing pixels in the image, return a pointer
// to the pixel in the first band of column zero in the
// row specified.  In the debug builds, this will assert that the
// row does contain pixels in the image, but this function
// may return an invalid pixel address if the row does not
// contain pixels in the image.
//
// @syntax const TPixel *RowPointer(int y) const
// @syntax TPixel *RowPointer(int y)
//
// @parm int | y |
// The row.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline const TPixel *CVisImage<TPixel>::RowPointer(int y) const
{
	return (const TPixel *) PbColZeroOfRow(y);
}

template <class TPixel>
inline TPixel *CVisImage<TPixel>::RowPointer(int y)
{
	return (TPixel *) PbColZeroOfRow(y);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TPixel& | CVisImage | FirstPixelInRow |
//
// Given a row containing pixels in the image, return a reference
// to the pixel in the first band of the leftmost point in the
// row specified.  In the debug builds, this will assert that the
// row does contain pixels in the image, but this function
// may return an invalid pixel address if the row does not
// contain pixels in the image.
//
// @syntax const TPixel& FirstPixelInRow(int y) const
// @syntax TPixel& FirstPixelInRow(int y)
//
// @parm int | y |
// The row.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline const TPixel& CVisImage<TPixel>::FirstPixelInRow(int y) const
{
	return *((const TPixel *) PbFirstPixelInRow(y));
}

template <class TPixel>
inline TPixel& CVisImage<TPixel>::FirstPixelInRow(int y)
{
	return *((TPixel *) PbFirstPixelInRow(y));
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TPixel * | CVisImage | PtrToFirstPixelInRow |
//
// Given a row containing pixels in the image, return a pointer
// to the pixel in the first band of the leftmost point in the
// row specified.  In the debug builds, this will assert that the
// row does contain pixels in the image, but this function
// may return an invalid pixel address if the row does not
// contain pixels in the image.
//
// @syntax const TPixel *PtrToFirstPixelInRow(int y) const
// @syntax TPixel *PtrToFirstPixelInRow(int y)
//
// @parm int | y |
// The row.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline const TPixel *CVisImage<TPixel>::PtrToFirstPixelInRow(int y)
		const
{
	return (const TPixel *) PbFirstPixelInRow(y);
}

template <class TPixel>
inline TPixel *CVisImage<TPixel>::PtrToFirstPixelInRow(int y)
{
	return (TPixel *) PbFirstPixelInRow(y);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TPixel * * | CVisImage | FillIliffeVector |
//
// Fill an Iliffe vector.  A pointer to the start of the Iliffe
// vector memory is passed to this function.  Pointers to column
// zero in each image row are placed in the Iliffe vector.  A
// pointer that can be indexed by image row is returned.
//
// @syntax TPixel **FillIliffeVector(void **ppvIliffe) const;
// @syntax TPixel **FillIliffeVector(void **ppvIliffe, int yFirst, int yLim)
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
template <class TPixel>
inline TPixel **CVisImage<TPixel>::FillIliffeVector(void **ppvIliffe)
		const
{
	return (TPixel **) PpvFillIliffeVector(ppvIliffe);
}

template <class TPixel>
inline TPixel **CVisImage<TPixel>::FillIliffeVector(void **ppvIliffe,
		int yFirst, int yLim) const
{
	return (TPixel **) PpvFillIliffeVector(ppvIliffe, yFirst, yLim);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisImage<TPixel> | CVisImage | SubImage |
//
// Return a subimage of this image.  The image returned is of the
// same type as this image and it uses the same memory block to
// store pixel information.  The returned image's rectangle will
// have the coordinates specified.  These coordinates must be
// contained in the memory rectangle returned by the
// <mf CVisImageBase::MemoryRect> method.
//
// @syntax CVisImage SubImage(int left, int top, int right,
//		int bottom) const;
// @syntax CVisImage SubImage(POINT pointLeftTop,
//		POINT pointRightBottom) const;
// @syntax CVisImage SubImage(POINT pointLeftTop,
//		SIZE size) const;
// @syntax CVisImage SubImage(const RECT& rect) const;
//
// @this const
//
// @parm int | left |
// 
//
// @parm int | top |
// 
//
// @parm int | right |
// 
//
// @parm int | bottom |
// 
//
// @parm POINT | pointLeftTop |
// 
//
// @parm POINT | pointRightBottom |
// 
//
// @parm SIZE | size |
// 
//
// @parm RECT | rect |
// 
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline CVisImage<TPixel> CVisImage<TPixel>::SubImage(
		int left, int top, int right, int bottom) const
{
	CVisImage<TPixel> subimage(*this);
	subimage.SetRect(left, top, right, bottom);
	return subimage;
}

template <class TPixel>
inline CVisImage<TPixel> CVisImage<TPixel>::SubImage(
		POINT pointLeftTop, POINT pointRightBottom) const
{
	CVisImage<TPixel> subimage(*this);
	subimage.SetRect(pointLeftTop, pointRightBottom);
	return subimage;
}

template <class TPixel>
inline CVisImage<TPixel> CVisImage<TPixel>::SubImage(POINT pointLeftTop,
		SIZE size) const
{
	CVisImage<TPixel> subimage(*this);
	subimage.SetRect(pointLeftTop, size);
	return subimage;
}

template <class TPixel>
inline CVisImage<TPixel> CVisImage<TPixel>::SubImage(
		const RECT& rect) const
{
	CVisImage<TPixel> subimage(*this);
	subimage.SetRect(rect);
	return subimage;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImage | operator== |
//
// Comparison operator.  This will return true if both images have the
// same shape (dimension and number of bands) and all corresponding
// pixels are equal.
//
// @this const
//
// @parm const CVisImage& | refimage |
// The image to compare to this image.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline bool CVisImage<TPixel>::operator==(
		const CVisImage<TPixel>& refimage) const
{
	if (Shape() != refimage.Shape())
		return false;

	if ((Width() <= 0) || (Height() <= 0))
		return true;

	if (PbPixel(StartPoint()) == refimage.PbPixel(StartPoint()))
		return true;

	// This is the number of pixels to examine in each row.
	int dpPixelWidth = Width() * NBands();

	for (int y = Top(); y < Bottom(); ++y)
	{
		// This will loop over all pixels in the row.
		const TPixel *pPixelThis = PtrToFirstPixelInRow(y);
		const TPixel *pPixelThat = refimage.PtrToFirstPixelInRow(y);
		const TPixel *pPixelLim = pPixelThis + dpPixelWidth;
		for ( ; pPixelThis < pPixelLim; ++pPixelThis, ++pPixelThat)
		{
			if (*pPixelThis != *pPixelThat)
				return false;
		}
	}
	
	return true;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImage | operator!= |
//
// Comparison operator.  This will return false if both images have the
// same shape (dimension and number of bands) and all corresponding
// pixels are equal.
//
// @this const
//
// @parm const CVisImage& | refimage |
// The image to compare to this image.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline bool CVisImage<TPixel>::operator!=(
		const CVisImage<TPixel>& refimage) const
{
	return !(*this == refimage);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImage | Pad |
//
// Pad pixels between the image rectangle and the specified
// rectangle by setting them to the specified evispad value or by using
// the specified padding method.  The rectangle
// specifed must be contained in the memory rectangle returned by
// the <mf CVisImageBase::MemoryRect> method.
//
// @syntax void Pad(TPixel value, int left, int top, int right, int bottom);
// @syntax void Pad(TPixel value, POINT pointLeftTop, POINT pointRightBottom);
// @syntax void Pad(TPixel value, POINT pointLeftTop, SIZE size);
// @syntax void Pad(TPixel value, const RECT& rect);
// @syntax void Pad(EVisPad evispad, int left, int top, int right, int bottom);
// @syntax void Pad(EVisPad evispad, POINT pointLeftTop, POINT pointRightBottom);
// @syntax void Pad(EVisPad evispad, POINT pointLeftTop, SIZE size);
// @syntax void Pad(EVisPad evispad, const RECT& rect);
//
// @parm TPixel | value |
// If a pixel value is given, all pad pixels should be set to the value given.
//
// @parm EVisPad | evispad |
// The padding option.  This specifies how two determine the values of
// the pad pixels.
//
// @parm int | left |
// The left column of the padding rectangle.  (Pixels between the image
// rectangle and the padding rectangle are pad pixels.)
//
// @parm int | top |
// The top row of the padding rectangle.  (Pixels between the image
// rectangle and the padding rectangle are pad pixels.)
//
// @parm int | right |
// The right column of the padding rectangle.  (Pixels between the image
// rectangle and the padding rectangle are pad pixels.)
//
// @parm int | bottom |
// The bottom row of the padding rectangle.  (Pixels between the image
// rectangle and the padding rectangle are pad pixels.)
//
// @parm POINT | pointLeftTop |
// The left-top point of the padding rectangle.  (Pixels between the image
// rectangle and the padding rectangle are pad pixels.)
//
// @parm POINT | pointRightBottom |
// The right-bottom point of the padding rectangle.  (Pixels between the image
// rectangle and the padding rectangle are pad pixels.)
//
// @parm SIZE | size |
// The size of the padding rectangle.  (Pixels between the image
// rectangle and the padding rectangle are pad pixels.)
//
// @parm const RECT& | rect |
// The padding rectangle.  (Pixels between the image
// rectangle and the padding rectangle are pad pixels.)
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline void CVisImage<TPixel>::Pad(EVisPad evispad, int left, int top,
		int right, int bottom)
{
	assert(left < right);
	assert(top < bottom);
	assert(left >= MemoryRect().left);
	assert(right <= MemoryRect().right);
	assert(top >= MemoryRect().top);
	assert(bottom <= MemoryRect().bottom);
	RECT rectInner;
	RECT rectOuter;

	if (left < Left())
	{
		rectInner.left = Left();
		rectOuter.left = left;
	}
	else
	{
		rectInner.left = left;
		rectOuter.left = Left();
	}

	if (right > Right())
	{
		rectInner.right = Right();
		rectOuter.right = right;
	}
	else
	{
		rectInner.right = right;
		rectOuter.right = Right();
	}

	if (top < Top())
	{
		rectInner.top = Top();
		rectOuter.top = top;
	}
	else
	{
		rectInner.top = top;
		rectOuter.top = Top();
	}

	if (bottom > Bottom())
	{
		rectInner.bottom = Bottom();
		rectOuter.bottom = bottom;
	}
	else
	{
		rectInner.bottom = bottom;
		rectOuter.bottom = Bottom();
	}

	CVisImage<TPixel> imageT(*this);
	imageT.SetRect(rectOuter);

	switch (evispad)
	{
	case evispadZero:
		{
			int cbLeft = (rectInner.left - rectOuter.left) * CbPoint();
			int cbRight = (rectOuter.right - rectInner.right) * CbPoint();
			int dppixelRow = CbRow() / CbPixel();
			if ((cbLeft > 0) || (cbRight > 0))
			{
				TPixel *ppixelLeftFirst = &(imageT.Pixel(rectOuter.left, rectInner.top, 0));
				TPixel *ppixelRightFirst;
				if (cbRight > 0)
					ppixelRightFirst = &(imageT.Pixel(rectInner.right, rectInner.top, 0));
				for (int y = rectInner.top; y < rectInner.bottom;
						++y, ppixelLeftFirst += dppixelRow, ppixelRightFirst += dppixelRow)
				{
					if (cbLeft >0)
						ZeroMemory(ppixelLeftFirst, cbLeft);
					if (cbRight > 0)
						ZeroMemory(ppixelRightFirst, cbRight);
				}
			}

			// Do the top.
			int cbRowOuter = (rectOuter.right - rectOuter.left) * CbPoint();
			if (cbRowOuter > 0)
			{
				TPixel *ppixelCur = &(imageT.Pixel(rectOuter.left, rectOuter.top, 0));
				for (int y = rectOuter.top; y < rectInner.top;
						++y, ppixelCur += dppixelRow)
				{
					ZeroMemory(ppixelCur, cbRowOuter);
				}

				if (rectInner.bottom < rectOuter.bottom)
				{
					ppixelCur = &(imageT.Pixel(rectOuter.left, rectInner.bottom, 0));
					for (y = rectInner.bottom; y < rectOuter.bottom;
							++y, ppixelCur += dppixelRow)
					{
						ZeroMemory(ppixelCur, cbRowOuter);
					}
				}
			}
		}
		break;

	case evispadReplicate:
		{
			// Do the left side first.
			int i;
			int dppixelRow = CbRow() / CbPixel();
			int cpixel = (rectInner.left - rectOuter.left) * NBands();
			if (cpixel > 0)
			{
				const TPixel *ppixelSrc = &(imageT.Pixel(rectOuter.left + 1, rectInner.top, 0));
				TPixel *ppixelDest = &(imageT.Pixel(rectOuter.left, rectInner.top, 0));
				for (int y = rectInner.top; y < rectInner.bottom;
						++y, ppixelDest += dppixelRow, ppixelSrc += dppixelRow)
				{
					for (i = cpixel - 1; i >= 0; --i)
						ppixelDest[i] = ppixelSrc[i];
				}
			}

			// Do the right side.
			cpixel = (rectOuter.right - rectInner.right) * NBands();
			if (cpixel > 0)
			{
				const TPixel *ppixelSrc = &(imageT.Pixel(rectInner.right - 1, rectInner.top, 0));
				TPixel *ppixelDest = &(imageT.Pixel(rectInner.right, rectInner.top, 0));
				for (int y = rectInner.top; y < rectInner.bottom;
						++y, ppixelDest += dppixelRow, ppixelSrc += dppixelRow)
				{
					for (i = 0; i < cpixel; ++i)
						ppixelDest[i] = ppixelSrc[i];
				}
			}

			// Do the top.
			int cbRowOuter = (rectOuter.right - rectOuter.left) * CbPoint();
			if (cbRowOuter > 0)
			{
				TPixel *ppixelInnerTop = &(imageT.Pixel(rectOuter.left, rectInner.top, 0));
				TPixel *ppixelCur = ppixelInnerTop - dppixelRow;
				for (int y = rectOuter.top; y < rectInner.top; ++y, ppixelCur -= dppixelRow)
				{
					CopyMemory(ppixelCur, ppixelInnerTop, cbRowOuter);
				}

				// Do the bottom.
				TPixel *ppixelInnerLast = &(imageT.Pixel(rectOuter.left, rectInner.bottom - 1, 0));
				ppixelCur = ppixelInnerLast + dppixelRow;
				for (y = rectInner.bottom; y < rectOuter.bottom; ++y, ppixelCur += dppixelRow)
				{
					CopyMemory(ppixelCur, ppixelInnerLast, cbRowOuter);
				}
			}
		}
		break;

	case evispadReflect:
		{
			// LATER:  Remove this restriction.
			assert(rectInner.left - rectOuter.left <= rectInner.right - rectInner.left);
			assert(rectOuter.right - rectInner.right <= rectInner.right - rectOuter.left);
			assert(rectInner.top - rectOuter.top <= rectInner.bottom - rectInner.top);
			assert(rectOuter.bottom - rectInner.bottom <= rectInner.bottom - rectOuter.top);

			const TPixel *ppixelSrc;
			TPixel *ppixelDest;
			int i;
			int dppixelRow = CbRow() / CbPixel();

			if (NBands() == 1)
			{
				// Left side.
				int dx = (rectInner.left - rectOuter.left);
				if (dx > 0)
				{
					for (int y = rectInner.top; y < rectInner.bottom; ++y)
					{
						ppixelDest = &(imageT.Pixel(rectInner.left - 1, y, 0));
						ppixelSrc = &(imageT.Pixel(rectInner.left, y, 0));

						for (i = dx - 1; i >= 0; --i)
						{
							*ppixelDest = *ppixelSrc;
							++ ppixelSrc;
							-- ppixelDest;
						}
					}
				}

				// Right side.
				dx = (rectOuter.right - rectInner.right);
				if (dx > 0)
				{
					for (int y = rectInner.top; y < rectInner.bottom; ++y)
					{
						ppixelDest = &(imageT.Pixel(rectInner.right, y, 0));
						ppixelSrc = &(imageT.Pixel(rectInner.right - 1, y, 0));

						for (i = dx - 1; i >= 0; --i)
						{
							*ppixelDest = *ppixelSrc;
							-- ppixelSrc;
							++ ppixelDest;
						}
					}
				}
			}
			else
			{
				// Left side.
				int dx = (rectInner.left - rectOuter.left);
				if (dx > 0)
				{
					for (int y = rectInner.top; y < rectInner.bottom; ++y)
					{
						ppixelDest = &(imageT.Pixel(rectInner.left - 1, y, 0));
						ppixelSrc = &(imageT.Pixel(rectInner.left, y, 0));

						for (i = dx - 1; i >= 0; --i)
						{
							for (int iBand = NBands() - 1; iBand >= 0; --iBand)
								ppixelDest[iBand] = ppixelSrc[iBand];
							++ ppixelSrc;
							-- ppixelDest;
						}
					}
				}

				// Right side.
				dx = (rectOuter.right - rectInner.right);
				if (dx > 0)
				{
					for (int y = rectInner.top; y < rectInner.bottom; ++y)
					{
						ppixelDest = &(imageT.Pixel(rectInner.right, y, 0));
						ppixelSrc = &(imageT.Pixel(rectInner.right - 1, y, 0));

						for (i = dx - 1; i >= 0; --i)
						{
							for (int iBand = NBands() - 1; iBand >= 0; --iBand)
								ppixelDest[iBand] = ppixelSrc[iBand];
							-- ppixelSrc;
							++ ppixelDest;
						}
					}
				}
			}

			// Do the top.
			int cbRowOuter = (rectOuter.right - rectOuter.left) * CbPoint();
			if (cbRowOuter > 0)
			{
				if (rectOuter.top < rectInner.top)
				{
					ppixelDest = &(imageT.Pixel(rectOuter.left, rectInner.top - 1, 0));
					ppixelSrc = &(imageT.Pixel(rectOuter.left, rectInner.top, 0));
					for (int y = rectOuter.top; y < rectInner.top;
							++y, ppixelDest -= dppixelRow, ppixelSrc += dppixelRow)
					{
						CopyMemory(ppixelDest, ppixelSrc, cbRowOuter);
					}
				}

				// Do the bottom.
				if (rectInner.bottom < rectOuter.bottom)
				{
					ppixelDest = &(imageT.Pixel(rectOuter.left, rectInner.bottom, 0));
					ppixelSrc = &(imageT.Pixel(rectOuter.left, rectInner.bottom - 1, 0));
					for (int y = rectInner.bottom; y < rectOuter.bottom;
							++y, ppixelDest += dppixelRow, ppixelSrc -= dppixelRow)
					{
						CopyMemory(ppixelDest, ppixelSrc, cbRowOuter);
					}
				}
			}
		}
		break;

	case evispadCyclic:
		{
			// Do the left side first.
			int i;
			int dppixelRow = CbRow() / CbPixel();
			int dx = (rectInner.left - rectOuter.left);
			if (dx > 0)
			{
				int cpixel = dx * NBands();
				TPixel *ppixelDest = &(imageT.Pixel(rectOuter.left, rectInner.top, 0));
				const TPixel *ppixelSrc = &(imageT.Pixel(rectInner.right - dx, rectInner.top, 0));
				for (int y = rectInner.top; y < rectInner.bottom;
						++y, ppixelDest += dppixelRow, ppixelSrc += dppixelRow)
				{
					for (i = cpixel - 1; i >= 0; --i)
						ppixelDest[i] = ppixelSrc[i];
				}
			}

			// Do the right side.
			dx = (rectOuter.right - rectInner.right);
			if (dx > 0)
			{
				int cpixel = dx * NBands();
				TPixel *ppixelDest = &(imageT.Pixel(rectInner.right, rectInner.top, 0));
				const TPixel *ppixelSrc = &(imageT.Pixel(rectInner.left, rectInner.top, 0));
				for (int y = rectInner.top; y < rectInner.bottom;
						++y, ppixelDest += dppixelRow, ppixelSrc += dppixelRow)
				{
					for (i = 0; i < cpixel; ++i)
						ppixelDest[i] = ppixelSrc[i];
				}
			}

			// Do the top.
			int cbRowOuter = (rectOuter.right - rectOuter.left) * CbPoint();
			if (cbRowOuter > 0)
			{
				if (rectOuter.top < rectInner.top)
				{
					TPixel *ppixelDest = &(imageT.Pixel(rectOuter.left, rectInner.top - 1, 0));
					const TPixel *ppixelSrc = &(imageT.Pixel(rectOuter.left, rectInner.bottom - 1, 0));
					for (int y = rectOuter.top; y < rectInner.top;
							++y, ppixelDest -= dppixelRow, ppixelSrc -= dppixelRow)
					{
						CopyMemory(ppixelDest, ppixelSrc, cbRowOuter);
					}
				}

				// Do the bottom.
				if (rectInner.bottom < rectOuter.bottom)
				{
					TPixel *ppixelDest = &(imageT.Pixel(rectOuter.left, rectInner.bottom, 0));
					const TPixel *ppixelSrc = &(imageT.Pixel(rectOuter.left, rectInner.top, 0));
					for (int y = rectInner.bottom; y < rectOuter.bottom;
							++y, ppixelDest += dppixelRow, ppixelSrc += dppixelRow)
					{
						CopyMemory(ppixelDest, ppixelSrc, cbRowOuter);
					}
				}
			}
		}
		break;
	}
}

template <class TPixel>
inline void CVisImage<TPixel>::Pad(TPixel value, int left, int top,
		int right, int bottom)
{
	assert(left < right);
	assert(top < bottom);
	assert(left >= MemoryRect().left);
	assert(right <= MemoryRect().right);
	assert(top >= MemoryRect().top);
	assert(bottom <= MemoryRect().bottom);
	RECT rectInner;
	RECT rectOuter;

	if (left < Left())
	{
		rectInner.left = Left();
		rectOuter.left = left;
	}
	else
	{
		rectInner.left = left;
		rectOuter.left = Left();
	}

	if (right > Right())
	{
		rectInner.right = Right();
		rectOuter.right = right;
	}
	else
	{
		rectInner.right = right;
		rectOuter.right = Right();
	}

	if (top < Top())
	{
		rectInner.top = Top();
		rectOuter.top = top;
	}
	else
	{
		rectInner.top = top;
		rectOuter.top = Top();
	}

	if (bottom > Bottom())
	{
		rectInner.bottom = Bottom();
		rectOuter.bottom = bottom;
	}
	else
	{
		rectInner.bottom = bottom;
		rectOuter.bottom = Bottom();
	}

	CVisImage<TPixel> imageT(*this);
	imageT.SetRect(rectOuter);

	// Do the left and right sides first.
	int i;
	int cpixelLeft = (rectInner.left - rectOuter.left) * NBands();
	int cpixelRight = (rectOuter.right - rectInner.right) * NBands();
	int dppixelRow = CbRow() / CbPixel();
	TPixel *ppixelLeftFirst = &(imageT.Pixel(rectOuter.left, rectInner.top, 0));
	TPixel *ppixelRightFirst = &(imageT.Pixel(rectInner.right, rectInner.top, 0));
	for (int y = rectInner.top; y < rectInner.bottom;
			++y, ppixelLeftFirst += dppixelRow, ppixelRightFirst += dppixelRow)
	{
		for (i = cpixelLeft - 1; i >= 0; --i)
			ppixelLeftFirst[i] = value;
		for (i = cpixelRight - 1; i >= 0; --i)
			ppixelRightFirst[i] = value;
	}

	// Do the top.
	int cpixelRowOuter = (rectOuter.right - rectOuter.left) * NBands();
	int cbRowOuter = cpixelRowOuter * CbPixel();
	TPixel *ppixelTopFirst = &(imageT.Pixel(rectOuter.left, rectOuter.top, 0));
	if (rectOuter.top < rectInner.top)
	{
		// Fill the top row.
		for (i = cpixelRowOuter - 1; i >= 0; --i)
			ppixelTopFirst[i] = value;

		// Use memcopy to copy subsequent rows.
		TPixel *ppixelCur = ppixelTopFirst + dppixelRow;
		for (int y = rectOuter.top + 1; y < rectInner.top;
				++y, ppixelCur += dppixelRow)
		{
			CopyMemory(ppixelCur, ppixelTopFirst, cbRowOuter);
		}
	}

	// Do the bottom.
	TPixel *ppixelBottomFirst = &(imageT.Pixel(rectOuter.left, rectInner.bottom, 0));
	if (rectInner.bottom < rectOuter.bottom)
	{
		// Fill the top row.
		if (rectOuter.top < rectInner.top)
		{
			CopyMemory(ppixelBottomFirst, ppixelTopFirst, cbRowOuter);
		}
		else
		{
			for (i = cpixelRowOuter - 1; i >= 0; --i)
				ppixelBottomFirst[i] = value;
		}

		// Use memcopy to copy subsequent rows.
		TPixel *ppixelCur = ppixelBottomFirst + dppixelRow;
		for (int y = rectInner.bottom + 1; y < rectOuter.bottom;
				++y, ppixelCur += dppixelRow)
		{
			CopyMemory(ppixelCur, ppixelBottomFirst, cbRowOuter);
		}
	}
}

template <class TPixel>
inline void CVisImage<TPixel>::Pad(EVisPad evispad, POINT pointLeftTop,
		POINT pointRightBottom)
{
	Pad(evispad, pointLeftTop.x, pointLeftTop.y,
			pointRightBottom.x, pointRightBottom.y);
}

template <class TPixel>
inline void CVisImage<TPixel>::Pad(TPixel value, POINT pointLeftTop,
		POINT pointRightBottom)
{
	Pad(value, pointLeftTop.x, pointLeftTop.y,
			pointRightBottom.x, pointRightBottom.y);
}

template <class TPixel>
inline void CVisImage<TPixel>::Pad(EVisPad evispad, POINT pointLeftTop, SIZE size)
{
	Pad(evispad, pointLeftTop.x, pointLeftTop.y,
                    pointLeftTop.x + size.cx, pointLeftTop.y + size.cy);
}

template <class TPixel>
inline void CVisImage<TPixel>::Pad(TPixel value, POINT pointLeftTop, SIZE size)
{
	Pad(value, pointLeftTop.x, pointLeftTop.y,
			pointLeftTop.x + size.cx, pointLeftTop.y + size.cy);
}

template <class TPixel>
inline void CVisImage<TPixel>::Pad(EVisPad evispad, const RECT& rect)
{
	Pad(evispad, rect.left, rect.top, rect.right, rect.bottom);
}

template <class TPixel>
inline void CVisImage<TPixel>::Pad(TPixel value, const RECT& rect)
{
	Pad(value, rect.left, rect.top, rect.right, rect.bottom);
}




////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisImageBase * | CVisImage | Clone |
//
// Make a copy of this image or create an image of the same type as
// this image with the specified dimensions.  Most users of the
// library should not use this method.  Instead, they should use the
// <mf CVisImage::SubImage> methods in the typed image classes.
// The pointer returned must be deleted using the C++ <f delete>
// operator when the caller has finished using the image.
//
// This is a virtual function so that
// it can be called in places where the type of the
// <c CVisImageBase>-derived class is not known.  It fills a pointer
// with the address of a new object which is a copy of this object.
// That address must be passed to the C++ <f delete> function when
// the caller if finished using the object.  This method may throw
// a memory exception if the <f new> operation fails.  This method
// may also fail and fill the pointer with NULL if an unexpected
// error occurs.
//
// @todo:(INTERNAL) Might want to pass imopts as a method parameter
// or use m_imopts when <p pshape> != 0.
//
// @this const
// 
//
// @parmopt const CVisShape * | pshape | 0 |
// If <p pshape> is 0, the new image will share pixel memory with this
// image.  If <p pshape> is not 0, the new image will be of the same
// type as this image and its dimensions will be those given by
// <p pshape>, but it will not share pixel memory or pixel values with
// the pixels in this image.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline CVisImageBase *CVisImage<TPixel>::Clone(const CVisShape *pshape) const
{
	CVisImageBase *pimage = 0;
	
	if (pshape == 0)

		pimage = new CVisImage<TPixel>(*this);
	else
		pimage = new CVisImage<TPixel>(*pshape);
		
	return pimage;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisImage | FillPixels |
//
// Set each pixel in the image rectangle to the specified value.
//
// @parm TPixel | value |
// The value to be assigned to each pixel in the image.
//
// @parm int | band |
// 
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline CVisImage<TPixel>&
	CVisImage<TPixel>::FillPixels(TPixel value, int band)
{
	// UNDONE:  Make this more efficient by using memset when
	// sizeof(TPixel) == 1.
	// LATER:  We could probably also make this more efficient for
	// other pixel types when we're filling all bands in the image.
	CVisEnumPixelOf<TPixel> enumPixel(*this, band);

	for (enumPixel.Reset(); enumPixel.FMore(); enumPixel.Next())
		*((TPixel *) enumPixel.PbPixelCur()) = value;

	return *this;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const type_info& | CVisImage | ObjectTypeInfo |
//
// Information about the type of this object.
//
// @parm int | iLevel | - 1 |
// The iLevel
// argument specifies the class depth (with CVisImageBase being
// at depth 0).  Use - 1 to find information about the deepest
// class.
// 
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline const type_info& CVisImage<TPixel>::ObjectTypeInfo(int iLevel) const
{
	if (iLevel == 0)
		return typeid(CVisImageBase);
	
	return typeid(CVisImage<TPixel>);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const type_info& | CVisImage | PixelTypeInfo |
//
// Information about the pixels in this image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline const type_info& CVisImage<TPixel>::PixelTypeInfo(void) const
		{ return typeid(TPixel); }


////////////////////////////////////////////////////////////////////////////
//
// @mfunc:(IMPL) EVisPixFmt | CVisImage | StaticPixFmt |
//
// Return an <t EVisPixFmt> value that describes the type of the pixel
// values used in this image.  Returns <e evispixfmtUnknown> if the type
// can not be described using the <t EVisPixFmt> values.
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline EVisPixFmt CVisImage<TPixel>::StaticPixFmt(void)
{
#ifdef VIS_OLD_CODE

	static EVisPixFmt evispixfmtRet = evispixfmtNil;

	if (evispixfmtRet == evispixfmtNil)
	{
		evispixfmtRet = VisPixFmtGetTPixel(TPixel());
	}

	assert(evispixfmtRet == CVisPixFmtGetTPixel<TPixel>::PixFmt());

	return evispixfmtRet;

#else // VIS_OLD_CODE

	return CVisPixFmtGetTPixel<TPixel>::PixFmt();

#endif // VIS_OLD_CODE
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc:(IMPL) void | CVisImage | SetMemBlockFromImopts |
//
// Set the memory block used by this object.  This is called in the
// constructor if a memory block of <t TPixel> objects needs to be
// allocated for this image.
//
// @parm int | imopts |
// 
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline void CVisImage<TPixel>::SetMemBlockFromImopts(int imopts)
{
	CVisShape shapeMem(Shape());

	if ((imopts & evisimoptAlignMemMask) != evisimoptDontAlignMemory)
	{
		int cb;

		if ((imopts & evisimoptAlignMemMask) == evisimoptAlignMem8Byte)
			cb = 8;
		else
			cb = 4;

		if ((shapeMem.Width() * CbCol() % cb) != 0)
		{
			shapeMem.right = shapeMem.left +
					((((shapeMem.Width() * CbCol()
									+ cb - 1)
								/ cb)
							* cb)
						/ CbCol());
		}
	}

	int cObj = shapeMem.Width() * shapeMem.Height() * NBands();
	CVisMemBlockOf<TPixel> memblock(cObj);

	CVisImageBase::SetMemBlock(memblock, shapeMem, Shape());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc:(IMPL) bool | CVisImage | RealCopyPixelsTo |
//
// Copy this image to another image.  This may fail if the
// pixels in the source image can't be converted to values
// which can be used to set the pixels in the destination image.
//
// @this const
//
// @parm int | bandSrc |
// 
//
// @parm int | bandDest |
// 
//
// @parm CVisImageBase& | refimageDest |
// 
//
// @parm const RECT& | refrectSrc |
// 
//
// @parm const POINT | pointDest |
// 
//
// @parm int | evisnormalize |
// 
//
// @parm const double | dblMin |
// 
//
// @parm const double | dblMax |
// 
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
inline bool CVisImage<TPixel>::RealCopyPixelsTo(int bandSrc, int bandDest,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax) const
{
	// LATER:  Add code to use colormap when copying gray byte pixels to RGBA.
	// (Also add code to convert between RGBA and YUVA.)
	return VisCopyStandardPixels(bandSrc, bandDest, *this, refimageDest,
			refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
}




// STL-like iterator methods
template <class TPixel>
inline CVisImage<TPixel>::iterator CVisImage<TPixel>::begin(int iband)
{
	CVisImage<TPixel>::iterator iteratorRet(*this, iband);

	// Not sure if the iterator will work with with invalid images, but
	// we allow for that case by checking IsValid() here.
	if (IsValid())
		iteratorRet.Reset();
	else
		iteratorRet.GoToEnd();

	return iteratorRet;
}


template <class TPixel>
inline CVisImage<TPixel>::iterator CVisImage<TPixel>::end(int iband)
{
	CVisImage<TPixel>::iterator iteratorRet(*this, iband);

	return iteratorRet;
}


#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common image types
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<BYTE> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<int> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<float> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<double> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisImage<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisImage<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList<CVisImage<TPixel> >(*this, false);
#endif // VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisImage<TPixel>::PropTypeInfoLookup(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisImage<TPixel>));
#endif // VIS_ONLY_DECLARE_PROPLIST
}




///////////////////////////////////////////////////////////////////////////////
//
// @todo:(INTERNAL)
// Old stream I/O functions
//
///////////////////////////////////////////////////////////////////////////////


// WARNING:  This operator is provided to dump the contents of an image to
// stdout for debugging.  It should not be used to store images in text files.
static inline void pixel_out(std::ostream& os, CVisRGBA<BYTE>& p)
{
    os << "(" << int(p.R()) << ", " << int(p.G()) << ", "
			<< int(p.B()) << ", " << int(p.A()) << ")";
}

// WARNING:  This operator is provided to dump the contents of an image to
// stdout for debugging.  It should not be used to store images in text files.
static inline void pixel_out(std::ostream& os, unsigned char& p)
{
    os << int(p);
}

// func ostream& | operator\<\< | Output stream operator for CVisImage\<>.
// tcarg class  | PIXELTYPE | Pixel type.
// WARNING:  This operator is provided to dump the contents of an image to
// stdout for debugging.  It should not be used to store images in text files.
template <class PIXELTYPE>
inline std::ostream& operator<<(
                           std::ostream& os,                 // parm Output stream.
                           CVisImage<PIXELTYPE>& img) // parm Image to output.
{
    // Write out the pixels as a 2-D table using the pixel() method
    os << "# " << img.Height() << " rows x " << img.Width() << " cols ";
    if (img.NBands() > 1) 
        os << "x " << img.NBands() << " bands ";
    os << img.PixelTypeInfo().name() << " image ";
    os << "(left = " << img.Left();
    os << ", top =  = " << img.Top();
    os << ", right =  = " << img.Right();
    os << ", bottom =  = " << img.Bottom() << ")" << endl;

    for (int r = img.Top(); r < img.Bottom(); r++) {
        for (int c = img.Left(); c < img.Right(); c++) {
            for (int b = 0; b < img.NBands(); b++) {
                    pixel_out(os, img.Pixel(c, r, b));
                os << ((b < img.NBands()-1) ? ";" :
                       (c < img.Right() - 1) ? " "   : "\n");
            }
        }
    }
    os.flush();
    return os;
}


// This is the olld code that could read an image from a text file.  (We don't
// want to use input images from text files, so it is #ifdefed out.)
#ifdef SAVE

// func istream& | operator>> | Output stream operator for CVisImage\<>.
// tcarg class  | PIXELTYPE | Pixel type.
template <class PIXELTYPE>
#if (_MSC_VER >= 1100)	// VC 5.0 or greater
static inline void pixel_in(std::istream& os, PIXELTYPE& p)
#else // (_MSC_VER >= 1100)
static inline void pixel_in(istream& os, PIXELTYPE& p)
#endif // (_MSC_VER >= 1100)
{
    os >> p;
}

#if (_MSC_VER >= 1100)	// VC 5.0 or greater
static inline void pixel_in(std::istream& os, unsigned char& p)
#else // (_MSC_VER >= 1100)
static inline void pixel_in(istream& os, unsigned char& p)
#endif // (_MSC_VER >= 1100)
{
    int i;  os >> i; p = i;
}

template <class PIXELTYPE>
#if (_MSC_VER >= 1100)	// VC 5.0 or greater
inline std::istream& operator>>(
                           std::istream& is,            // parm Input stream.
#else // (_MSC_VER >= 1100)
inline istream& operator>>(
                           istream& is,            // parm Input stream.
#endif // (_MSC_VER >= 1100)
                           CVisImage<PIXELTYPE>& img) // parm Place to store image.

{
    // Skip any initial comment lines
    int c;
    is >> c;
    while (c == '#') {
        for (; c != '\n'; is >> c); // skip rest of line
    }
    is.putback(c);

    // Read in the pixels using the pixel() method
    for (int r = img.SRow(); r <= img.ERow(); r++) {
        for (int c = img.SCol(); c <= img.ECol(); c++) {
            for (int b = 0; b < img.NBands(); b++) {
                pixel_in(is, img.Pixel(r, c, b));
                is >> c;
            }
        }
    }
    return is;
}
#endif // SAVE


////////////////////////////////////////////////////////////////////////////
// Helper function


template<class TPixel>
inline CVisImage<TPixel> VisBrightnessFromRGBA(
		const CVisImage<CVisRGBA<TPixel> >& refimageRGBA)
{
	CVisImage<TPixel> imageGray;

	assert(&refimageRGBA != 0);
	if (refimageRGBA.IsValid())
	{
		imageGray.Allocate(refimageRGBA.Shape());

		VisMap2(CVisFnOpRGBAIntensity(), imageGray,
				const_cast<CVisImage<CVisRGBA<TPixel> >&>(refimageRGBA));
	}

	return imageGray;
}


////////////////////////////////////////////////////////////////////////////
// Overloaded operators

template<class TPixel>
inline CVisImage<TPixel> operator-(const CVisImage<TPixel>& refimage)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2(imageRet.Shape(), CVisFnOpNegatePixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage));
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel>& operator+=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	VisMap1(CVisFnOpAddPixels<TPixel>(refpixel), refimage);
	return refimage;
}

template<class TPixel>
inline CVisImage<TPixel>& operator+=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	VisMap2(CVisFnOpAddPixels<TPixel>(), refimage1,
			const_cast<CVisImage<TPixel>&>(refimage2));
	return refimage1;
}

template<class TPixel>
inline CVisImage<TPixel> operator+(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	CVisImage<TPixel> imageRet((refimage1.Rect() & refimage2.Rect()),
			refimage1.NBands());
	VisMap3(imageRet.Rect(), CVisFnOpAddPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage1),
			const_cast<CVisImage<TPixel>&>(refimage2));
	return imageRet;
}

template<class TPixel>
inline CVisImage<TPixel> operator+(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2P(imageRet.Rect(), CVisFnOpAddPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage), refpixel);
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel>& operator-=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	VisMap1(CVisFnOpSubPixels<TPixel>(refpixel), refimage);
	return refimage;
}

template<class TPixel>
inline CVisImage<TPixel>& operator-=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	VisMap2(CVisFnOpSubPixels<TPixel>(), refimage1,
			const_cast<CVisImage<TPixel>&>(refimage2));
	return refimage1;
}

template<class TPixel>
inline CVisImage<TPixel> operator-(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	CVisImage<TPixel> imageRet((refimage1.Rect() & refimage2.Rect()),
			refimage1.NBands());
	VisMap3(imageRet.Rect(), CVisFnOpSubPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage1),
			const_cast<CVisImage<TPixel>&>(refimage2));
	return imageRet;
}

template<class TPixel>
inline CVisImage<TPixel> operator-(const CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2P(imageRet.Rect(), CVisFnOpSubPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage), refpixel);
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel>& operator*=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	VisMap1(CVisFnOpMultPixels<TPixel>(refpixel), refimage);
	return refimage;
}

template<class TPixel>
inline CVisImage<TPixel>& operator*=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	VisMap2(CVisFnOpMultPixels<TPixel>(), refimage1,
			const_cast<CVisImage<TPixel>&>(refimage2));
	return refimage1;
}

template<class TPixel>
inline CVisImage<TPixel> operator*(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	CVisImage<TPixel> imageRet((refimage1.Rect() & refimage2.Rect()),
			refimage1.NBands());
	VisMap3(imageRet.Rect(), CVisFnOpMultPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage1),
			const_cast<CVisImage<TPixel>&>(refimage2));
	return imageRet;
}

template<class TPixel>
inline CVisImage<TPixel> operator*(const CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2P(imageRet.Rect(), CVisFnOpMultPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage), refpixel);
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel>& operator/=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	VisMap1(CVisFnOpDivPixels<TPixel>(refpixel), refimage);
	return refimage;
}

template<class TPixel>
inline CVisImage<TPixel>& operator/=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	VisMap2(CVisFnOpDivPixels<TPixel>(), refimage1,
			const_cast<CVisImage<TPixel>&>(refimage2));
	return refimage1;
}

template<class TPixel>
inline CVisImage<TPixel> operator/(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	CVisImage<TPixel> imageRet((refimage1.Rect() & refimage2.Rect()),
			refimage1.NBands());
	VisMap3(imageRet.Rect(), CVisFnOpDivPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage1),
			const_cast<CVisImage<TPixel>&>(refimage2));
	return imageRet;
}

template<class TPixel>
inline CVisImage<TPixel> operator/(const CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2P(imageRet.Rect(), CVisFnOpDivPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage), refpixel);
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel> operator~(const CVisImage<TPixel>& refimage)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2(imageRet.Shape(), CVisFnOpNotPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage));
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel>& operator&=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	VisMap1(CVisFnOpAndPixels<TPixel>(refpixel), refimage);
	return refimage;
}

template<class TPixel>
inline CVisImage<TPixel>& operator&=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	VisMap2(CVisFnOpAndPixels<TPixel>(), refimage1,
			const_cast<CVisImage<TPixel>&>(refimage2));
	return refimage1;
}

template<class TPixel>
inline CVisImage<TPixel> operator&(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	CVisImage<TPixel> imageRet((refimage1.Rect() & refimage2.Rect()),
			refimage1.NBands());
	VisMap3(imageRet.Rect(), CVisFnOpAndPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage1),
			const_cast<CVisImage<TPixel>&>(refimage2));
	return imageRet;
}

template<class TPixel>
inline CVisImage<TPixel> operator&(const CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2P(imageRet.Rect(), CVisFnOpAndPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage), refpixel);
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel>& operator|=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	VisMap1(CVisFnOpOrPixels<TPixel>(refpixel), refimage);
	return refimage;
}

template<class TPixel>
inline CVisImage<TPixel>& operator|=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	VisMap2(CVisFnOpOrPixels<TPixel>(), refimage1,
			const_cast<CVisImage<TPixel>&>(refimage2));
	return refimage1;
}

template<class TPixel>
inline CVisImage<TPixel> operator|(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	CVisImage<TPixel> imageRet((refimage1.Rect() & refimage2.Rect()),
			refimage1.NBands());
	VisMap3(imageRet.Rect(), CVisFnOpOrPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage1),
			const_cast<CVisImage<TPixel>&>(refimage2));
	return imageRet;
}

template<class TPixel>
inline CVisImage<TPixel> operator|(const CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2P(imageRet.Rect(), CVisFnOpOrPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage), refpixel);
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel>& operator<<=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	VisMap1(CVisFnOpLShiftPixels<TPixel>(refpixel), refimage);
	return refimage;
}

template<class TPixel>
inline CVisImage<TPixel>& operator<<=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	VisMap2(CVisFnOpLShiftPixels<TPixel>(), refimage1,
			const_cast<CVisImage<TPixel>&>(refimage2));
	return refimage1;
}

template<class TPixel>
inline CVisImage<TPixel> operator<<(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	CVisImage<TPixel> imageRet((refimage1.Rect() & refimage2.Rect()),
			refimage1.NBands());
	VisMap3(imageRet.Rect(), CVisFnOpLShiftPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage1),
			const_cast<CVisImage<TPixel>&>(refimage2));
	return imageRet;
}

template<class TPixel>
inline CVisImage<TPixel> operator<<(const CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2P(imageRet.Rect(), CVisFnOpLShiftPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage), refpixel);
	return imageRet;
}


template<class TPixel>
inline CVisImage<TPixel>& operator>>=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel)
{
	VisMap1(CVisFnOpRShiftPixels<TPixel>(refpixel), refimage);
	return refimage;
}

template<class TPixel>
inline CVisImage<TPixel>& operator>>=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	VisMap2(CVisFnOpRShiftPixels<TPixel>(), refimage1,
			const_cast<CVisImage<TPixel>&>(refimage2));
	return refimage1;
}

template<class TPixel>
inline CVisImage<TPixel> operator>>(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2)
{
	CVisImage<TPixel> imageRet((refimage1.Rect() & refimage2.Rect()),
			refimage1.NBands());
	VisMap3(imageRet.Rect(), CVisFnOpRShiftPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage1),
			const_cast<CVisImage<TPixel>&>(refimage2));
	return imageRet;
}

template<class TPixel>
inline CVisImage<TPixel> operator>>(const CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	CVisImage<TPixel> imageRet(refimage.Shape());
	VisMap2P(imageRet.Rect(), CVisFnOpRShiftPixels<TPixel>(), imageRet,
			const_cast<CVisImage<TPixel>&>(refimage), refpixel);
	return imageRet;
}



#ifdef _DEBUG
#undef new
#endif // _DEBUG

#endif // __VIS_CORE_IMAGEOF_INL__
