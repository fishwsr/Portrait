///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGE
//
// @module VisImage.inl |
//
// Inline functions for the base image classes defined in
// VisImage.h.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
//
// @todo:(INTERNAL)
// IOStream functions?
//
// <nl>
//
// @xref <l VisImage\.h> <l VisImage\.cpp> <l VisFileIO\.cpp>
// <l VisImageOf\.h> <l VisImageOf\.inl>
//
// <nl>
// @index | VISIMAGE
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_IMAGE_INL__
#define __VIS_CORE_IMAGE_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <stdio.h>      // needed by ReadWriteStream
#include <assert.h>
#include <string>


#include "VisEnumPixel.h"


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG



////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | Width |
//
// Get the width of the image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::Width(void) const
{
	return ((m_pbDataOrigin == 0) ? 0 : m_shapeImage.Width());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | Height |
//
// Get the height of the image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::Height(void) const
{
	return ((m_pbDataOrigin == 0) ? 0 : m_shapeImage.Height());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | NBands |
//
// Get the number of bands in each point in the image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::NBands(void) const
{
	return ((m_pbDataOrigin == 0) ? 1 : m_shapeImage.NBands());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | NPoints |
//
// Get the number of points in the image.  (This is the
// product or the width and height of the image.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::NPoints(void) const
{
	return Width() * Height();
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | NPixels |
//
// Get the number of pixels in the image.  (This is the products of
// the image height, image width, and number of image bands.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::NPixels(void) const
{
	return NPoints() * NBands();
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CSize | CVisImageBase | Size |
//
// Get a CSize structure giving the width and height of the image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline CVisSize CVisImageBase::Size(void) const
{
	return ((m_pbDataOrigin == 0)
			? CVisSize(0, 0) : m_shapeImage.Size());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | Left |
//
// Get the coordinate of the left column contained in the image
// rectangle.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::Left(void) const
{
	return ((m_pbDataOrigin == 0) ? 0 : m_shapeImage.left);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | Top |
//
// Get the coordinate of the top row contained in the image
// rectangle.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::Top(void) const
{
	return ((m_pbDataOrigin == 0) ? 0 : m_shapeImage.top);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | Right |
//
// Get the coordinate of the right column bounding the image
// rectangle.  This column is not contained in the image rectangle.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::Right(void) const
{
	return ((m_pbDataOrigin == 0) ? 0 : m_shapeImage.right);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | Bottom |
//
// Get the coordinate of the bottom row bounding the image
// rectangle.  This row is not contained in the image rectangle.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::Bottom(void) const
{
	return ((m_pbDataOrigin == 0) ? 0 : m_shapeImage.bottom);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CPoint | CVisImageBase | StartPoint |
//
// Get the location of the upper-left point contained in the
// image rectangle.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline CVisPoint CVisImageBase::StartPoint(void) const
{
	return ((m_pbDataOrigin == 0)
			? CVisPoint(0, 0) : m_shapeImage.TopLeft());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CPoint | CVisImageBase | CenterPoint |
//
// Get the location of the center point in the image.
// @todo:(INTERNAL) Document rounding used with <mf CVisImageBase::CenterPoint>.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline CVisPoint CVisImageBase::CenterPoint(void) const
{
	return ((m_pbDataOrigin == 0)
			? CVisPoint(0, 0) : m_shapeImage.CenterPoint());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CPoint | CVisImageBase | LastPoint |
//
// Get the location of the lower-right point contained in the
// image rectangle.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline CVisPoint CVisImageBase::LastPoint(void) const
{
	CVisPoint point = LimPoint();
	
	-- point.x;
	-- point.y;

	return point;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CPoint | CVisImageBase | LimPoint |
//
// Get the location of the lower-right limit of the image rectangle.
// This point is not contained in the image rectangle.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline CVisPoint CVisImageBase::LimPoint(void) const
{
	return ((m_pbDataOrigin == 0)
			? CVisPoint(0, 0) : m_shapeImage.BottomRight());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const CRect& | CVisImageBase | Rect |
//
// Get a <c CRect> object giving the top, left, bottom, and right
// limits of the image.  (The bottom row and right column are not
// part of the image.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const CVisRect& CVisImageBase::Rect(void) const
{
	return ((m_pbDataOrigin == 0) ? ShapeInvalid() : m_shapeImage);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const CVisShape& | CVisImageBase | Shape |
//
// Get a <c CVisShape> object giving the top, left, bottom, and right
// limits of the image and the number of bands in the image.  (The
// bottom row and right column are not part of the image.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const CVisShape& CVisImageBase::Shape(void) const
{
	return ((m_pbDataOrigin == 0) ? ShapeInvalid() : m_shapeImage);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc EVisPixFmt | CVisImageBase | PixFmt |
//
// Returns a union of <t VisPixel> values which give type
// information about the pixels used in this image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline EVisPixFmt CVisImageBase::PixFmt(void) const
{
	return m_evispixfmt;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | ContainsPoint |
//
// Is the specified point contained in the image rectangle?
//
// @parm int | x |
// The X coordinate of the point.
//
// @parm int | y |
// The Y coordinate of the point.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline bool CVisImageBase::ContainsPoint(int x, int y) const
{
	return (Left() <= x) && (Right() > x) && (Top() <= y)
			&& (Bottom() > y);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | ContainsPoint |
//
// Is the specified point contained in the image rectangle?
//
// @parm int | x |
// The X coordinate of the point.
//
// @parm int | y |
// The Y coordinate of the point.
//
// @parm int | band |
// The band of the point.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline bool CVisImageBase::ContainsPoint(int x, int y, int band) const
{
	return (ContainsPoint(x, y) && (band >= 0) && (band < NBands()));	
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | ContainsPoint |
//
// Is the specified point contained in the image rectangle?
//
// @parm POINT | point |
// The point.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline bool CVisImageBase::ContainsPoint(POINT point) const
{
	return ContainsPoint(point.x, point.y);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | ContainsPoint |
//
// Is the specified point contained in the image rectangle?
//
// @parm POINT | point |
// The point.
//
// @parm int | band |
// The band of the point.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline bool CVisImageBase::ContainsPoint(POINT point, int band) const
{
	return ContainsPoint(point.x, point.y, band);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | CbRow |
//
// The number of bytes between pixels in adjacent rows of the
// same column and band.
//
// @comm
// This may be larger than the product of <mf CVisImageBase::Width>
// and <mf CVisImageBase::CbCol>.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::CbRow(void) const
{
	return m_cbRow;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | CbCol |
//
// The number of bytes between pixels in adjacent columns
// of the same row and band.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::CbCol(void) const
{
	return m_cbCol;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | CbBand |
//
// The number of bytes between pixels in adjacent bands
// of the same row and column.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::CbBand(void) const
{
	return m_cbPixel;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | CbPoint |
//
// The size of image points in bytes.  (This will be the same
// as <mf CVisImageBase::CbCol>, since points in adjacent
// columns of the same row are adjacent in memory.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::CbPoint(void) const
{
	return m_cbCol;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | CbPixel |
//
// The size of image pixels in bytes.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::CbPixel(void) const
{
	return m_cbPixel;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisImageBase | PbPixel |
//
// Given the coordinates of a point in the image, find the address
// of the pixel with the specified
// coordinates.  In the debug builds, this will assert that the
// pixel coordinates are in the image rectangle, but this function
// may return an invalid pixel address if the pixel coordinates
// are not in the image rectangle.  If the image has more than one
// band, the caller must specify the band.
//
// @syntax const BYTE *PbPixel(POINT point) const;
// @syntax const BYTE *PbPixel(int x, int y) const;
// @syntax const BYTE *PbPixel(POINT point, int band) const;
// @syntax const BYTE *PbPixel(int x, int y, int band) const;
// @syntax BYTE *PbPixel(POINT point);
// @syntax BYTE *PbPixel(int x, int y);
// @syntax BYTE *PbPixel(POINT point, int band);
// @syntax BYTE *PbPixel(int x, int y, int band);
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
inline const BYTE *CVisImageBase::PbPixel(POINT point) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(point));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return PbColZeroOfRow(point.y) + point.x * m_cbCol;
}

inline const BYTE *CVisImageBase::PbPixel(int x, int y) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return PbColZeroOfRow(y) + x * m_cbCol;
}

inline const BYTE *CVisImageBase::PbPixel(POINT point, int band)
		const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(point, band));
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return PbColZeroOfRow(point.y) + point.x * m_cbCol
			+ band * m_cbPixel;
}

inline const BYTE *CVisImageBase::PbPixel(int x, int y, int band)
		const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y, band));
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return PbColZeroOfRow(y) + x * m_cbCol + band * m_cbPixel;
}

inline BYTE *CVisImageBase::PbPixel(POINT point)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(point));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS

	return PbColZeroOfRow(point.y) + point.x * m_cbCol;
}

inline BYTE *CVisImageBase::PbPixel(int x, int y)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y));
	assert(NBands() == 1);
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return PbColZeroOfRow(y) + x * m_cbCol;
}

inline BYTE *CVisImageBase::PbPixel(POINT point, int band)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(point, band));
#endif // VIS_CORE_EXTRA_ASSERTS

	return PbColZeroOfRow(point.y) + point.x * m_cbCol
			+ band * m_cbPixel;
}

inline BYTE *CVisImageBase::PbPixel(int x, int y, int band)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(x, y, band));
#endif // VIS_CORE_EXTRA_ASSERTS
	
	return PbColZeroOfRow(y) + x * m_cbCol + band * m_cbPixel;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const BYTE * | CVisImageBase | PbColZeroOfRow |
//
// Given a row containing pixels in the image, find the address of
// the pixel in the first band of the point in column zero of the
// row specified.  In the debug builds, this will assert that the
// row does contain pixels in the image, but this function
// may return an invalid pixel address if the row does not
// contain pixels in the image.
//
// @syntax const BYTE *PbColZeroOfRow(int y) const;
// @syntax BYTE *PbColZeroOfRow(int y);
//
// @parm int | y |
// The row of pixels.
//
////////////////////////////////////////////////////////////////////////////
inline const BYTE *CVisImageBase::PbColZeroOfRow(int y) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(Left(), y, 0));
#endif // VIS_CORE_EXTRA_ASSERTS

#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	return m_ppbIliffe[y];
#else // VIS_CORE_IMAGE_USE_ILIFFE
	return m_pbDataOrigin + y * m_cbRow;
#endif // VIS_CORE_IMAGE_USE_ILIFFE
}

inline BYTE *CVisImageBase::PbColZeroOfRow(int y)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(Left(), y, 0));
#endif // VIS_CORE_EXTRA_ASSERTS

#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	return m_ppbIliffe[y];
#else // VIS_CORE_IMAGE_USE_ILIFFE
	return m_pbDataOrigin + y * m_cbRow;
#endif // VIS_CORE_IMAGE_USE_ILIFFE
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const BYTE * | CVisImageBase | PbFirstPixelInRow |
//
// Given a row containing pixels in the image, find the address
// of the pixel in the first band of the leftmost point in the
// row specified.  In the debug builds, this will assert that the
// row does contain pixels in the image, but this function
// may return an invalid pixel address if the row does not
// contain pixels in the image.
//
// @syntax const BYTE *PbFirstPixelInRow(int y) const;
// @syntax BYTE *PbFirstPixelInRow(int y);
//
// @parm int | y |
// The row of pixels.
//
////////////////////////////////////////////////////////////////////////////
inline const BYTE *CVisImageBase::PbFirstPixelInRow(int y) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(Left(), y, 0));
#endif // VIS_CORE_EXTRA_ASSERTS

	return m_pbFirstPixelInRow0 + y * m_cbRow;
}

inline BYTE *CVisImageBase::PbFirstPixelInRow(int y)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPoint(Left(), y, 0));
#endif // VIS_CORE_EXTRA_ASSERTS

	return m_pbFirstPixelInRow0 + y * m_cbRow;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | ContainsPixelAddress |
//
// Does the given pixel address refer to a (byte in a) pixel
// in the image?
//
// @this const
//
// @parm const void * | pixeladdress |
// THe address.
//
////////////////////////////////////////////////////////////////////////////
inline bool CVisImageBase::ContainsPixelAddress(const void *pixeladdress)
		const
{
	return (pixeladdress >= PbPixel(StartPoint()))
			&& (pixeladdress < PbPixel(LastPoint(), 0) + CbPoint());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | RowOfPixelAddress |
//
// Given the address of a (byte in a) pixel in the image, find
// the row of the image that contains the specified pixel.  In
// the debug builds, this will assert that the address does
// refer to a (byte in a) pixel in the image, but this function
// may return an invalid row if the pixel address does not
// refer to a (byte in a) pixel in the image.
//
// @this const
//
// @parm const void * | pixeladdress |
// THe pixel address.
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::RowOfPixelAddress(const void *pixeladdress)
		const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPixelAddress(pixeladdress));
#endif // VIS_CORE_EXTRA_ASSERTS

	return (((const BYTE *) pixeladdress) - m_pbDataOrigin) / m_cbRow;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | ColOfPixelAddress |
//
// Given the address of a (byte in a) pixel in the image, find
// the column of the image that contains the specified pixel.  In
// the debug builds, this will assert that the address does
// refer to a (byte in a) pixel in the image, but this function
// may return an invalid column if the pixel address does not
// refer to a (byte in a) pixel in the image.
//
// @this const
//
// @parm const void * | pixeladdress |
// THe pixel address.
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::ColOfPixelAddress(const void *pixeladdress)
		const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPixelAddress(pixeladdress));
#endif // VIS_CORE_EXTRA_ASSERTS

	return ((((const BYTE *) pixeladdress) - m_pbDataOrigin) % m_cbRow)
			/ m_cbCol;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | BandOfPixelAddress |
//
// Given the address of a (byte in a) pixel in the image, find
// the band of the image that contains the specified pixel.  In
// the debug builds, this will assert that the address does
// refer to a (byte in a) pixel in the image, but this function
// may return an invalid band if the pixel address does not
// refer to a (byte in a) pixel in the image.
//
// @this const
//
// @parm const void * | pixeladdress |
// THe pixel address.
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::BandOfPixelAddress(const void *pixeladdress)
		const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(ContainsPixelAddress(pixeladdress));
#endif // VIS_CORE_EXTRA_ASSERTS

	return ((((const BYTE *) pixeladdress) - m_pbDataOrigin) % m_cbCol)
			/ m_cbPixel;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | GetPixelValueString |
//
// Given the location of a pixel value, get a string representing
// the pixel value, if the derived class knows how to get such a
// string.  This is used in the display code.  Most users of the
// library will not need to use this method.
//
// @syntax void GetPixelValueString(const void *pixeladdress,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1) const;
// @syntax void GetPixelValueString(POINT point,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1) const;
// @syntax void GetPixelValueString(POINT point, int band,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1) const;
// @syntax void GetPixelValueString(int x, int y,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1) const;
// @syntax void GetPixelValueString(int x, int y, int band,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1) const;
// @syntax void GetPixelValueString(const void *pixeladdress,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1);
// @syntax void GetPixelValueString(POINT point,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1);
// @syntax void GetPixelValueString(POINT point, int band,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1);
// @syntax void GetPixelValueString(int x, int y,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1);
// @syntax void GetPixelValueString(int x, int y, int band,
//		char *szValue, int nFieldWidth = - 1, int nDecPlaces = - 1);
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
// @parm char * | szValue |
// The string buffer to be filled with the pixel value string.
//
// @parm int | nFieldWidth |
// Field width to be used when formatting numbers.  Not currently
// supported.
//
// @parm int | nDecPlaces |
// Number of decimal places to be used when formatting floating-point
// numbers.  Not currently supported.
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::GetPixelValueString(const void *pixeladdress,
		char *szValue, int cbValue, int nFieldWidth, int nDecPlaces) const
{
	return RealGetPixelValueString(pixeladdress, szValue, cbValue,
			nFieldWidth, nDecPlaces);
}

inline int CVisImageBase::GetPixelValueString(POINT point,
		char *szValue, int cbValue, int nFieldWidth, int nDecPlaces) const
{
	return RealGetPixelValueString(PbPixel(point, 0), szValue, cbValue, nFieldWidth,
			nDecPlaces);
}

inline int CVisImageBase::GetPixelValueString(POINT point, int band,
		char *szValue, int cbValue, int nFieldWidth, int nDecPlaces) const
{
	return RealGetPixelValueString(PbPixel(point, band), szValue, cbValue,
			nFieldWidth, nDecPlaces);
}

inline int CVisImageBase::GetPixelValueString(int x, int y,
		char *szValue, int cbValue, int nFieldWidth, int nDecPlaces) const
{
	return RealGetPixelValueString(PbPixel(x, y, 0), szValue, cbValue,
			nFieldWidth, nDecPlaces);
}

inline int CVisImageBase::GetPixelValueString(int x, int y, int band,
		char *szValue, int cbValue, int nFieldWidth, int nDecPlaces) const
{
	return RealGetPixelValueString(PbPixel(x, y, band), szValue, cbValue,
			nFieldWidth, nDecPlaces);
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
inline CVisImageBase& CVisImageBase::ResetOrigin(POINT pointLeftTop)
{
	return ResetOrigin(pointLeftTop.x, pointLeftTop.y);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | MemoryWidth |
//
// Get the width of the memory associated with this image.  (The memory
// block containing this image may be wider than this image.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::MemoryWidth(void) const
{
	return ((m_pbDataOrigin == 0) ? 0 : m_shapeMemBlock.Width());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisImageBase | MemoryHeight |
//
// Get the height of the memory associated with this image.  (The memory
// block containing this image may be taller than this image.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisImageBase::MemoryHeight(void) const
{
	return ((m_pbDataOrigin == 0) ? 0 : m_shapeMemBlock.Height());
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const CRect& | CVisImageBase | MemoryRect |
//
// Get a <c CRect> object giving the top, left, bottom, and right
// limits of the memory associated with this image.  (The bottom
// row and right column are not part of the image.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const CVisRect& CVisImageBase::MemoryRect(void) const
{
	return ((m_pbDataOrigin == 0) ? ShapeInvalid() : m_shapeMemBlock);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const CVisShape& | CVisImageBase | MemoryShape |
//
// Get a <c CVisShape> object giving the top, left, bottom, and right
// limits of the memory associated with this image and the number of
// bands in the image.  (The bottom row and right column are not part
// of the image.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const CVisShape& CVisImageBase::MemoryShape(void) const
{
	return ((m_pbDataOrigin == 0) ? ShapeInvalid() : m_shapeMemBlock);
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
inline bool CVisImageBase::SetRect(POINT pointLeftTop, POINT pointRightBottom)
{
	return SetRect(pointLeftTop.x, pointLeftTop.y,
			pointRightBottom.x, pointRightBottom.y);
}

inline bool CVisImageBase::SetRect(POINT pointLeftTop, SIZE size)
{
	return SetRect(pointLeftTop.x, pointLeftTop.y,
			pointLeftTop.x + size.cx, pointLeftTop.y + size.cy);
}

inline bool CVisImageBase::SetRect(const RECT& rect)
{
	return SetRect(rect.left, rect.top, rect.right, rect.bottom);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const type_info& | CVisImageBase | ObjectTypeInfo |
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
inline const type_info& CVisImageBase::ObjectTypeInfo(int iLevel) const
		{ return typeid(CVisImageBase); }




////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | CopyPixelsTo |
//
// Copy pixels from this image to another image.  This may fail
// if the pixels in the source image can't be converted to values
// which can be used to set the pixels in the destination image.
//
//
////////////////////////////////////////////////////////////////////////////


inline bool CVisImageBase::CopyPixelsTo(CVisImageBase& refimageDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax) const
{
	return RealCopyPixelsTo(- 1, - 1, refimageDest, Rect(),
			refimageDest.StartPoint(), evisnormalize,
			dblMin, dblMax);
}

inline bool CVisImageBase::CopyPixelsTo(CVisImageBase& refimageDest,
		const RECT& refrectSrc,
		EVisNormalize evisnormalize, double dblMin, double dblMax) const
{
	return RealCopyPixelsTo(- 1, - 1, refimageDest, refrectSrc,
			refimageDest.StartPoint(), evisnormalize,
			dblMin, dblMax);
}

inline bool CVisImageBase::CopyPixelsTo(CVisImageBase& refimageDest,
		const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax) const
{
	return RealCopyPixelsTo(- 1, - 1, refimageDest, Rect(),
			pointDest, evisnormalize, dblMin, dblMax);
}

inline bool CVisImageBase::CopyPixelsTo(CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax) const
{
	return RealCopyPixelsTo(- 1, - 1, refimageDest, refrectSrc,
			pointDest, evisnormalize, dblMin, dblMax);
}


inline bool CVisImageBase::CopyPixelsTo(int bandSrc, int bandDest,
		CVisImageBase& refimageDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax) const
{
	return RealCopyPixelsTo(bandSrc, bandDest, refimageDest, Rect(),
			refimageDest.StartPoint(), evisnormalize,
			dblMin, dblMax);
}

inline bool CVisImageBase::CopyPixelsTo(int bandSrc, int bandDest,
		CVisImageBase& refimageDest, const RECT& refrectSrc,
		EVisNormalize evisnormalize, double dblMin, double dblMax) const
{
	return RealCopyPixelsTo(bandSrc, bandDest, refimageDest, refrectSrc,
			refimageDest.StartPoint(), evisnormalize,
			dblMin, dblMax);
}

inline bool CVisImageBase::CopyPixelsTo(int bandSrc, int bandDest,
		CVisImageBase& refimageDest, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax) const
{
	return RealCopyPixelsTo(bandSrc, bandDest, refimageDest, Rect(),
			pointDest, evisnormalize, dblMin, dblMax);
}

inline bool CVisImageBase::CopyPixelsTo(int bandSrc, int bandDest,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax) const
{
	return RealCopyPixelsTo(bandSrc, bandDest, refimageDest, refrectSrc,
			pointDest, evisnormalize, dblMin, dblMax);
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
//		int cbitPixel = 0) const;
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
inline bool CVisImageBase::DisplayInHdc(HDC hdc, int xDest, int yDest,
								   DWORD dwRop, int cbitPixel) const
{
	return DisplayInHdc(hdc, Rect(), xDest, yDest, dwRop,
			cbitPixel);
}

inline bool CVisImageBase::DisplayInHdc(HDC hdc, const POINT& pointDest,
			DWORD dwRop, int cbitPixel) const
{
	return DisplayInHdc(hdc, Rect(), pointDest.x, pointDest.y, dwRop,
			cbitPixel);
}

inline bool CVisImageBase::DisplayInHdc(HDC hdc, const RECT& rectSrc,
		const POINT& pointDest, DWORD dwRop, int cbitPixel) const
{
	return DisplayInHdc(hdc, rectSrc, pointDest.x, pointDest.y, dwRop,
			cbitPixel);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc  | CVisImageBase | ~CVisImageBase |
//
// Memory allocation is only done in the base class, so the
// destructor does not need to be virtual.
//
////////////////////////////////////////////////////////////////////////////
inline CVisImageBase::~CVisImageBase(void)
{
	Cleanup();
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | SetName |
//
// Associate a name with an image.
//
// @parm const char * | name |
// The null-terminated name string to be associated with this image.  The
// string will be copied by this image.
//
////////////////////////////////////////////////////////////////////////////
inline void CVisImageBase::SetName(const char *name)
{
	assert((m_strName.empty()) || (*m_strName.c_str() == 0)
			|| (name != m_strName.c_str()));
	// UNDONE:  If the name is changed, should we clear information about
	// the memory and image shapes that were used before the last call to
	// Deallocate?
	m_strName = name;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc FILETIME | CVisImageBase | Filetime |
//
// Get a time associated with an image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline FILETIME CVisImageBase::Filetime(void) const
{
	return m_filetime;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | SetFiletime |
//
// Associate the current time with an image.
//
////////////////////////////////////////////////////////////////////////////
inline void CVisImageBase::SetFiletime(void)
{
	GetSystemTimeAsFileTime(&m_filetime);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | SetFiletime |
//
// Associate a time with an image.
//
// @parm FILETIME | filetime |
// Windows <t FILETIME> structure to be associated with this image.
//
////////////////////////////////////////////////////////////////////////////
inline void CVisImageBase::SetFiletime(FILETIME filetime)
{
	m_filetime = filetime;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | IsValid |
//
// Is this a valid image?  (Does it have a non-empty memory block
// allocated for pixels?)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline bool CVisImageBase::IsValid(void) const
{
	if ((this != 0) && (m_pbDataOrigin != 0))
	{
// Asserts commented out to avoid synchronization problems.
//		assert(m_shapeMemBlock.Width() > 0);
//		assert(m_shapeMemBlock.Height() > 0);
//		assert(m_shapeMemBlock.NBands() > 0);
//		assert(m_memblock.PbData() != 0);

		return true;
	}

	return false;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const CVisMemBlock& | CVisImageBase | MemBlock |
//
// Get a reference to the <c CVisMemBlock> structure used to store
// pixel data for this image.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const CVisMemBlock& CVisImageBase::MemBlock(void) const
{
	return m_memblock;
}



inline bool CVisImageBase::ReadWriteFile(SVisFileDescriptor& reffiledescriptor,
			bool fWrite, bool fThrowError, bool fDisplayErrorMessage)
{
	return RealReadWriteFile(reffiledescriptor, fWrite, fThrowError,
			fDisplayErrorMessage);
}

inline CVisStreamHandler* CVisImageBase::StreamHandler(UINT *pFrameNum) const
{
    if( pFrameNum )
        *pFrameNum = m_uStreamFrameNum;
    return m_pVisStreamHandler;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc bool | CVisImageBase | UseColorMap |
//
// Should a color map be used with this image, if one is
// available?  If this is true, colormaps may be used when loading,
// saving, and displaying images containing BYTE pixels.  If this is false,
// grayscale pixel values will be used directly.
//
// Color maps are only supported for 8-bit pixels.
// This may change in the final version of the SDK.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline bool CVisImageBase::UseColorMap(void) const
{
	return m_fUseColorMap;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | SetUseColorMap |
//
// Should a color map be used with this image, if one is
// available?  If this is true, colormaps may be used when loading,
// saving, and displaying images containing BYTE pixels.  If this is false,
// grayscale pixel values will be used directly.
//
// Note that color map indices are not grayscale values, so changes to this
// flag could affect some code that works only with grayscale images.
//
// Color maps are only supported for 8-bit pixels.
// This may change in the final version of the SDK.
//
// @parm bool | fUseColorMap |
// Flag indicating whether color maps should be used with this image.
//
////////////////////////////////////////////////////////////////////////////
inline void CVisImageBase::SetUseColorMap(bool fUseColorMap)
{
	m_fUseColorMap = fUseColorMap;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc const CVisMemBlock& | CVisImageBase | ColorMap |
//
// Set a <c CMemBlock> object containing a colormap to use with
// this image.
//
// The pointer in the <c CMemBlock> object may be 0.
// Color maps are only supported for 8-bit pixels.
// This may change in the final version of the SDK.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const CVisMemBlock& CVisImageBase::ColorMap(void) const
{
	return m_memblockColorMap;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | SetColorMap |
//
// Set a <c CMemBlock> object containing a colormap to use with
// this image.
//
// The pointer in the <c CMemBlock> object may be 0.
// Color maps are only supported for 8-bit pixels.
// This may change in the final version of the SDK.
//
// @parm const CVisMemBlock& | memblockColorMap |
// 
//
////////////////////////////////////////////////////////////////////////////
inline void CVisImageBase::SetColorMap(const CVisMemBlock& memblockColorMap)
{
	m_memblockColorMap = memblockColorMap;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | SetMemBlock |
//
// Set the meory block (and memory block coordinates) used with
// this image.
//
// @syntax void SetMemBlock(const CVisMemBlock& refmemblock,
//		const CVisShape& refshapeMemBlock, const CVisShape& refshapeImage);
// @syntax void SetMemBlock(const CVisMemBlock& refmemblock,
//		const CVisShape& refshapeMemBlock);
//
// @parm const CVisMemBlock& | refmemblock |
// Reference to a <t CVisMemBlock> object representing the memory that
// is to be used to store the pixel values for this image.
//
// @parm const CVisShape& | refshapeMemBlock |
// A <c CVisShape> structure giving the dimensions of the image pixels
// in the <t CVisMemBlock> object.
//
// @parm const CVisShape& | refshapeImage |
// A <c CVisShape> structure giving the dimensions of the image pixels
// to be used in this image.  If this parameter is not given, it is assumed
// to be the same as <p refshapeMemBlock>.
//
////////////////////////////////////////////////////////////////////////////
inline void CVisImageBase::SetMemBlock(const CVisMemBlock& refmemblock,
		const CVisShape& refshapeMemBlock)
{
	SetMemBlock(refmemblock, refshapeMemBlock, refshapeMemBlock);
}


inline void CVisImageBase::SetMemBlock(const CVisMemBlock& refmemblock,
		int dxWidth, int dyHeight)
{
	CVisShape shapeMemBlock(0, 0, dxWidth, dyHeight);
	SetMemBlock(refmemblock, shapeMemBlock);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisImageBase | Allocate |
//
// Allocate or reallocate the memory block for this image.  Most
// users of the library will not need to use this method.
//
// @syntax void Allocate(int dx, int dy);
// @syntax void Allocate(SIZE size);
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
inline void CVisImageBase::Allocate(int dx, int dy)
{
	CVisShape shape(0, 0, dx, dy);
	Allocate(shape, shape);
}

inline void CVisImageBase::Allocate(SIZE size)
{
	CVisShape shape(0, 0, size.cx, size.cy);
	Allocate(shape, shape);
}

inline void CVisImageBase::Allocate(const CVisShape& shapeImage)
{
	Allocate(shapeImage, shapeImage);
}

inline const CVisShape& CVisImageBase::ShapeBeforeDeallocate(void) const
{
	return m_shapeImage;
}

inline const CVisShape& CVisImageBase::MemoryShapeBeforeDeallocate(void)
		const
{
	return m_shapeMemBlock;
}

inline void CVisImageBase::AllocateUsingPrevoiusShapeIfPossible(int dx,
		int dy, int nBands)
{
	AllocateUsingPrevoiusShapeIfPossible(CVisShape(0, 0, dx, dy, nBands));
}

inline int CVisImageBase::Options(void) const
{
	return m_imopts;
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
inline void **CVisImageBase::PpvFillIliffeVector(void **ppvIliffe)
		const
{
	return PpvFillIliffeVector(ppvIliffe, Top(), Bottom());
}

inline const CVisPropList& CVisImageBase::PropList(void) const
{
	return m_proplist;
}

inline CVisPropList& CVisImageBase::PropList(void)
{
	return m_proplist;
}


inline bool CVisImageBase::IsDirty(void) const
{
	return m_fDirty;
}

inline void CVisImageBase::SetIsDirty(bool f)
{
	m_fDirty = f;
}

inline bool CVisImageBase::DelayRead(void) const
{
	return m_fDelayRead;
}

inline void CVisImageBase::SetDelayRead(bool f)
{
	m_fDelayRead = f;
}


inline const CVisShape& CVisImageBase::ShapeInvalid(void) const
{
	assert(m_pbDataOrigin == 0);

#ifdef LATER // If needed in the future.
	// Try to align the shape on an 8-byte boundry.
	if ((((int) (const void * ) &m_cbCol) & 0x4) != 0)
	{
		return (const CVisShape&) *((const CVisShape *)
				(const void *) &m_cbRow);
	}
#endif // LATER

	return (const CVisShape&) *((const CVisShape *)
			(const void *) &m_cbCol);
}


// Old self-describing stream I/O.
/*
inline CVisSDObject< CVisImageBase >&
	CVisSDObject< CVisImageBase >::Exemplar(void)
{
	// Global variable used with self-describing streams.
	// LATER:  Find a way to avoid using this global variable.
	extern CVisSDObject< CVisImageBase > g_visimagebaseExemplar;

	return g_visimagebaseExemplar;
}
*/
#ifdef VIS_INCLUDE_SDSTREAM

inline CVisSDStream& operator<<(CVisSDStream& s, CVisImageBase& refimage)
	{ return VisSDReadWriteClass(s, refimage); }

#endif

inline void CVisImageBase::SDReadWriteProperties(CVisSDStream& s, bool fAddComma)
{
#ifdef VIS_INCLUDE_SDSTREAM
	s.OpenParen();
	s.Num(m_filetime.dwLowDateTime, true);
	s.Comma();
	s.Num(m_filetime.dwHighDateTime, true);
	s.CloseParen();
	if (fAddComma)
		s.Comma();
	s.Comment("Timestamp (low int, high int)");
#else
	assert(false);
#endif
}

inline void VisCheckConformal(const CVisImageBase *pimage1,
		const CVisImageBase *pimage2)
{
	if (pimage1->Shape() != pimage2->Shape())
	{
		assert(0);

		throw CVisError("Image shape mismatch",
				eviserrorImageShapeMismatch, "VisCheckConformal",
				"VisImage.inl", __LINE__);
	}
}

inline void VisCheckConformal(const CVisImageBase *pimage1,
		const CVisImageBase *pimage2,
		const CVisImageBase *pimage3, ...)
{
    VisCheckConformal(pimage1, pimage2);
    VisCheckConformal(pimage1, pimage3);

    va_list vl;
    va_start(vl, pimage3);
    const CVisImageBase *pimageNext = va_arg(vl, const CVisImageBase *);

    while (pimageNext != 0)
    {
        VisCheckConformal(pimage1, pimageNext);
        pimageNext = va_arg(vl, const CVisImageBase *);
    }
}



#ifdef _DEBUG
#undef new
#endif // _DEBUG


#endif // __VIS_CORE_IMAGE_INL__
