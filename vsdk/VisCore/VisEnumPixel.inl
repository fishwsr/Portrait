///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISENUMPIXEL
//
// @module VisEnumPixel.inl |
//
// Non-inline functions for objects used to enumerate pixels in images.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @todo:(INTERNAL) Don't require a copy of the image, just the pixels?
// <nl>
//
// @xref <l VisEnumPixel\.h> <l VisEnumPixel\.cpp>
//
// <nl>
// @index | VISENUMPIXEL
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_ENUMPIXEL_INL__
#define __VIS_CORE_ENUMPIXEL_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "VisImageBase.h"


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


////////////////////////////////////////////////////////////////////////////
//
// @mfunc BYTE * | CVisEnumPixel | PbPixelCur |
//
// Get a pointer to the first byte of the current pixel.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline BYTE *CVisEnumPixel::PbPixelCur(void) const
{
	return m_pbCur;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisEnumPixel | CbPixel |
//
// Get the number of bytes in each pixel.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisEnumPixel::CbPixel(void) const
{
	return m_cbPixelCur;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisEnumPixel | CbColCur |
//
// Get the number of bytes between adjacent columns in the same row.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisEnumPixel::CbColCur(void) const
{
	return m_cbColCur;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisEnumPixel | CbRowCur |
//
// Get the number of bytes between adjacent rows in the same column.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisEnumPixel::CbRowCur(void) const
{
	return m_cbRowCur;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisEnumPixel | CPixelColCur |
//
// Get the number of pixels between adjacent columns in the same row.
// (This may be equal to one or the number of bands in the image.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisEnumPixel::CPixelColCur(void) const
{
	// LATER:  Add additional member variables
	// for efficiency.
	return m_cbColCur / m_cbPixelCur;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisEnumPixel | CPixelRowCur |
//
// Get the number of pixels between adjacent rows in the same column.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisEnumPixel::CPixelRowCur(void) const
{
	// LATER:  Add additional member variables
	// for efficiency.
	return m_cbRowCur / m_cbPixelCur;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc BOOL | CVisEnumPixel | FMore |
//
// Are there more pixels (in addition to the current pixel)
// to be enumerated.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline BOOL CVisEnumPixel::FMore(void) const
{
	return (m_pbCur != 0);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc BOOL | CVisEnumPixel | Reset |
//
// Reset the enumerator.  If the image contains pixels, this will
// make the left-top pixel the current pixel in the enumerator.
//
////////////////////////////////////////////////////////////////////////////
inline BOOL CVisEnumPixel::Reset(void)
{
	// LATER:  Images with Borders.
	assert(m_dxBorder == 0);
	assert(m_dyBorder == 0);

	StartCenter();

	return (m_pbCur != 0);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisEnumPixel | Next |
//
// Advance the current pixel.  If there are no more pixels to be
// enumerated, the current pixel will not be valid after this method
// is called.
//
////////////////////////////////////////////////////////////////////////////
inline void CVisEnumPixel::Next(void)
{
	assert(m_pbCur != 0);
	if ((m_pbCur += m_dpbCur) != m_pbRowLim)
	{
		// Nothing needs to be done.
		// (This common branch is first for Pentium Pro branch
		// prediction alg.)
		return;
	}
	else if ((m_pbRowLim += m_cbRowCur) != m_pbSubimageLim)
	{
		// Start at the beginning of the next row.
		m_pbCur = m_pbRowLim - m_cbColEnumCur;
	}
	else
	{
		// Go to the next subimage.
		m_pfnNextSubsection(this);
	}
}


inline void CVisEnumPixel::GoToEnd(void)
{
	m_pbCur = 0;
}


inline bool CVisEnumPixel::operator==(const CVisEnumPixel& refenumpixel) const
{
	return (m_pbCur == refenumpixel.m_pbCur);
}


inline bool CVisEnumPixel::operator!=(const CVisEnumPixel& refenumpixel) const
{
	return (m_pbCur != refenumpixel.m_pbCur);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisImageBase& | CVisEnumPixel | Image |
//
// Return a reference to a copy of the image being enumerated.
// (The address of this image will be different to the address
// of the image given to the constructor of this object.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline CVisImageBase& CVisEnumPixel::Image(void) const
{
	return *m_pImage;
}




////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisEnumPixelOf | CVisEnumPixelOf |
//
// Constructor.
//
// @syntax CVisEnumPixelOf(const CVisImageBase& refImage, int band = -1,
//		int dxBorder = 0, int dyBorder = 0,
//		EVisPad pad = evispadNone);
// @syntax CVisEnumPixelOf(const CVisImageBase& refImage, const RECT& rect,
//		int band = -1, int dxBorder = 0, int dyBorder = 0,
//		EVisPad pad = evispadNone);
// @syntax CVisEnumPixelOf(const CVisImageBase& refImage, int xLeft, int yTop,
//		int dxWidth, int dyHeight, int band = -1,
//		int dxBorder = 0, int dyBorder = 0,
//		EVisPad pad = evispadNone);
//
// @parm const CVisImageBase& | refImage |
// The image whose pixels are to be enumerated.  This object will make
// a copy of the image (that shares the same pixel memory), so <p image>
// may be safely destroyed before this enumerator is destroyed, and
// changes to the pixels in <p image> will change the pixels enumerated
// by this object.
//
// @parm const RECT& | rect |
// The rectangle to be enumerated.
//
// @parm int | xLeft |
// Left column of the rectangle to be enumerated.
//
// @parm int | yTop |
// Top row of the rectangle to be enumerated.
//
// @parm int | xRight |
// Right column of the rectangle to be enumerated.  (This column defines
// the right of the enumeration but is not included in the enumeration.)
//
// @parm int | yBottom |
// Bottom row of the rectangle to be enumerated.  (This row defines the
// bottom of the enumeration but is not included in the enumeration.)
//
// @parmopt int | band | - 1 |
// Band to be enumerated.  If -1 is given, all bands will be enumerated.
//
// @parmopt int | dxBorder | 0 |
// Future border option.  Not supported in the beta release.
// (Users should always use the default value for this parameter.)
//
// @parmopt int | dyBorder | 0 |
// Future border option.  Not supported in the beta release.
// (Users should always use the default value for this parameter.)
//
// @parmopt EVisPad | pad | evispadNone |
// Future padding option.  Not supported in the beta release.
// (Users should always use the default value for this parameter.)
//
////////////////////////////////////////////////////////////////////////////
template<class TPixel>
inline CVisEnumPixelOf<TPixel>::CVisEnumPixelOf(
		const CVisImageBase& refImage, int band,
		int dxBorder, int dyBorder, EVisPad pad)
  : CVisEnumPixel(refImage, band, dxBorder, dyBorder)
{
}

template<class TPixel>
inline CVisEnumPixelOf<TPixel>::CVisEnumPixelOf(
		const CVisImageBase& refImage, const RECT& rect, int band,
		int dxBorder, int dyBorder, EVisPad pad)
  : CVisEnumPixel(refImage, rect, band, dxBorder, dyBorder, pad)
{
}

template<class TPixel>
inline CVisEnumPixelOf<TPixel>::CVisEnumPixelOf(
		const CVisImageBase& refImage, int xLeft, int yTop,
		int xRight, int yBottom, int band,
		int dxBorder, int dyBorder, EVisPad pad)
  : CVisEnumPixel(refImage, xLeft, yTop, xRight, yBottom,
		band, dxBorder, dyBorder, pad)
{
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TPixel * | CVisEnumPixelOf | PPixelCur |
//
// Get a pointer to the current pixel.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template<class TPixel>
inline TPixel *CVisEnumPixelOf<TPixel>::PPixelCur(void) const
{
	return (TPixel *) PbPixelCur();
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TPixel& | CVisEnumPixelOf | PixelCur |
//
// Get a reference to the current pixel.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template<class TPixel>
inline TPixel& CVisEnumPixelOf<TPixel>::PixelCur(void) const
{
	return *PPixelCur();
}



template<class TPixel>
inline TPixel& CVisEnumPixelOf<TPixel>::operator*(void) const
{
	return *PPixelCur();
}


template<class TPixel>
inline CVisEnumPixelOf<TPixel>& CVisEnumPixelOf<TPixel>::operator++(void)
{
	if (FMore())
		Next();

	return *this;
}


template<class TPixel>
inline CVisEnumPixelOf<TPixel> CVisEnumPixelOf<TPixel>::operator++(int)
{
	CVisEnumPixelOf<TPixel> iteratorRet(*this);

	++*this;

	return iteratorRet;
}




#ifdef _DEBUG
#undef new
#endif // _DEBUG


#endif // __VIS_CORE_ENUMPIXEL_INL__