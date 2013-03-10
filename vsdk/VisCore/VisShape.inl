///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISSHAPE
//
// @module VisShape.inl |
//
// Objects derived from <c CRect> which also contain a number of
// bands.  These objects are used to describe parts of multi-band
// images.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisShape\.h> <c CRect>
//
// <nl>
// @index | VISSHAPE
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_SHAPE_INL__
#define __VIS_SHAPE_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG

#include <math.h>
inline CVisShape::CVisShape(void)
  : CVisRect(),
	m_nbands(1)
{
}

inline CVisShape::CVisShape(const CVisShape& shape)
  : CVisRect(shape),
	m_nbands(shape.m_nbands)
{
}

inline CVisShape::CVisShape(const RECT& rect, int nbands)
  : CVisRect(rect),
	m_nbands(nbands)
{
	assert(nbands > 0);
}

inline CVisShape::CVisShape(POINT point, SIZE size, int nbands)
  : CVisRect(point, size),
	m_nbands(nbands)
{
	assert(nbands > 0);
}
inline CVisShape::CVisShape(int left, int top, int right, int bottom,
		int nbands)
  : CVisRect(left, top, right, bottom),
	m_nbands(nbands)
{
	assert(nbands > 0);
}


inline int CVisShape::Left(void) const
{
	return left;
}


inline int CVisShape::Top(void) const
{
	return top;
}


inline int CVisShape::Right(void) const
{
	return right;
}


inline int CVisShape::Bottom(void) const
{
	return bottom;
}

inline int CVisShape::NBands(void) const
{
	return m_nbands;
}

inline void CVisShape::SetLeft(int l)
{
	left = l;
}

inline void CVisShape::SetTop(int t)
{
	top = t;
}

inline void CVisShape::SetRight(int r)
{
	right = r;
}

inline void CVisShape::SetBottom(int b)
{
	bottom = b;
}

inline void CVisShape::SetNBands(int nbands)
{
	m_nbands = nbands;
}

inline bool CVisShape::Includes(int x, int y, int nband) const
{
	assert(left <= right);
	assert(top <= bottom);
	return (x >= left) && (x < right)
			&& (y >= top) && (y < bottom)
			&& (nband >= 0) && (nband < m_nbands);
}

inline bool CVisShape::Includes(POINT point, int nband) const
{
	assert(left <= right);
	assert(top <= bottom);
	return (point.x >= left) && (point.x < right)
			&& (point.y >= top) && (point.y < bottom)
			&& (nband >= 0) && (nband < m_nbands);
}

inline bool CVisShape::Includes(const RECT& rect, int nband) const
{
	assert(left <= right);
	assert(top <= bottom);
	assert(rect.left <= rect.right);
	assert(rect.top <= rect.bottom);
	return (rect.left >= left) && (rect.right <= right)
			&& (rect.top >= top) && (rect.bottom <= bottom)
			&& (nband >= 0) && (nband < m_nbands);
}

inline bool CVisShape::Includes(const CVisShape& refshape) const
{
	assert(left <= right);
	assert(top <= bottom);
	assert(refshape.left <= refshape.right);
	assert(refshape.top <= refshape.bottom);
	return (refshape.left >= left) && (refshape.right <= right)
			&& (refshape.top >= top) && (refshape.bottom <= bottom)
			&& (refshape.NBands() == m_nbands);
}


inline CVisShape CVisShape::Decimated(int wRate, bool expand) const
{
	CVisShape shapeT(*this);

	return shapeT.Decimate(wRate, expand);
}

inline CVisShape& CVisShape::Decimate(int wRate, bool expand)
{
    if (expand) {
	    left = (int) floor(left / (float) wRate);
	    top = (int) floor(top / (float) wRate);
    	right = (int) ceil(right / (float) wRate);
	    bottom = (int) ceil(bottom / (float) wRate);
    } else {
	    left = (int) ceil(left / (float) wRate);
	    top = (int) ceil(top / (float) wRate);
	    right = 1 + (int) floor((right - 1) / (float) wRate);
	    bottom = 1 + (int) floor((bottom - 1) / (float) wRate);
    }
//	right = 1 + (int) ceil((right - 1) / (float) wRate);
//	bottom = 1 + (int) ceil((bottom - 1) / (float) wRate);
	assert(left < right);
	assert(top < bottom);

	return *this;
}


inline CVisShape CVisShape::ScaledUp(int wRate) const
{
	CVisShape shapeT(*this);

	return shapeT.ScaleUp(wRate);
}

inline CVisShape& CVisShape::ScaleUp(int wRate)
{
	left *= wRate;
	top *= wRate;
	right *= wRate;
	bottom *= wRate;

	return *this;
}


inline bool CVisShape::operator==(const CVisShape& shape) const
{
	assert(left <= right);
	assert(top <= bottom);
	assert(shape.left <= shape.right);
	assert(shape.top <= shape.bottom);
	return (shape.left == left) && (shape.right == right)
			&& (shape.top == top) && (shape.bottom == bottom)
			&& (shape.m_nbands == m_nbands);
}

inline bool CVisShape::operator!=(const CVisShape& shape) const
{
	return !operator==(shape);
}


inline CVisShape& CVisShape::operator=(const CVisShape& shape)
{
	assert(shape.left <= shape.right);
	assert(shape.top <= shape.bottom);

	left = shape.left;
	top = shape.top;
	right = shape.right;
	bottom = shape.bottom;
	m_nbands = shape.m_nbands;

	return *this;
}

inline CVisShape& CVisShape::operator=(const RECT& rect)
{
	assert(rect.left <= rect.right);
	assert(rect.top <= rect.bottom);

	left = rect.left;
	top = rect.top;
	right = rect.right;
	bottom = rect.bottom;
	m_nbands = 1;

	return *this;
}


inline CVisShape& CVisShape::operator&=(const RECT& rect)
{
	assert(left <= right);
	assert(top <= bottom);
	assert(rect.left <= rect.right);
	assert(rect.top <= rect.bottom);

	if (rect.left > left)
		left = rect.left;
	if (rect.right < right)
		right = rect.right;
	if (rect.top > top)
		top = rect.top;
	if (rect.bottom < bottom)
		bottom = rect.bottom;
	m_nbands = 1;

	if ((left > right) || (top > bottom))
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
	}

	return *this;
}

inline CVisShape& CVisShape::operator|=(const RECT& rect)
{
	assert(left <= right);
	assert(top <= bottom);
	assert(rect.left <= rect.right);
	assert(rect.top <= rect.bottom);

	if (rect.left < left)
		left = rect.left;
	if (rect.right > right)
		right = rect.right;
	if (rect.top < top)
		top = rect.top;
	if (rect.bottom > bottom)
		bottom = rect.bottom;
	m_nbands = 1;

	return *this;
}

inline CVisShape& CVisShape::operator&=(const CVisShape& shape)
{
	assert(left <= right);
	assert(top <= bottom);
	assert(shape.left <= shape.right);
	assert(shape.top <= shape.bottom);

	if (shape.left > left)
		left = shape.left;
	if (shape.right < right)
		right = shape.right;
	if (shape.top > top)
		top = shape.top;
	if (shape.bottom < bottom)
		bottom = shape.bottom;
	if (shape.m_nbands < m_nbands)
		m_nbands = shape.m_nbands;

	if ((left > right) || (top > bottom))
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
	}

	return *this;
}

inline CVisShape& CVisShape::operator|=(const CVisShape& shape)
{
	assert(left <= right);
	assert(top <= bottom);
	assert(shape.left <= shape.right);
	assert(shape.top <= shape.bottom);

	if (shape.left < left)
		left = shape.left;
	if (shape.right > right)
		right = shape.right;
	if (shape.top < top)
		top = shape.top;
	if (shape.bottom > bottom)
		bottom = shape.bottom;
	if (shape.m_nbands > m_nbands)
		m_nbands = shape.m_nbands;

	return *this;
}


inline CVisShape CVisShape::operator&(const RECT& rect) const
{
	CVisShape shapeT(*this);
	return shapeT &= rect;
}

inline CVisShape CVisShape::operator|(const RECT& rect) const
{
	CVisShape shapeT(*this);
	return shapeT |= rect;
}

inline CVisShape CVisShape::operator&(const CVisShape& shape) const
{
	CVisShape shapeT(*this);
	return shapeT &= shape;
}

inline CVisShape CVisShape::operator|(const CVisShape& shape) const
{
	CVisShape shapeT(*this);
	return shapeT |= shape;
}


inline CVisShape CVisShape::IntersectedWith(const RECT& rect) const
{
	CVisShape shapeT(*this);
	return shapeT &= rect;
}

inline CVisShape CVisShape::IntersectedWith(const CVisShape& shape) const
{
	CVisShape shapeT(*this);
	return shapeT &= shape;
}

inline CVisShape CVisShape::UnionedWith(const RECT& rect) const
{
	CVisShape shapeT(*this);
	return shapeT |= rect;
}

inline CVisShape CVisShape::UnionedWith(const CVisShape& shape) const
{
	CVisShape shapeT(*this);
	return shapeT |= shape;
}


#ifdef VIS_INCLUDE_SDSTREAM

inline CVisSDStream& operator<<(CVisSDStream& s, CVisShape& refshape)
{
	s.OpenParen();

#ifdef LATER // UNDONE:  We'll need to change existing files to use the new order.

	s.Num(refshape.left);
	s.Comma();
	s.Num(refshape.top);
	s.Comma();
	s.Num(refshape.right);
	s.Comma();
	s.Num(refshape.bottom);
	s.Comma();
	s.Num(refshape.m_nbands);

#else // LATER

	s.Num(refshape.left);
	s.Comma();
	s.Num(refshape.top);
	s.Comma();

    if (s.Status() == CVisSDStream::Write)
	{
        int dy = refshape.Height();
        int dx = refshape.Width();

		s << dx;
		s.Comma();
		s << dy;
    }
	else if (s.Status() == CVisSDStream::Read)
	{
        int dy = 0;
        int dx = 0;

		s << dx;
		s.Comma();
		s << dy;

		refshape.right = refshape.left + dx;
		refshape.bottom = refshape.top + dy;
    }

	s.Comma();
	s.Num(refshape.m_nbands);

#endif // LATER

	s.CloseParen();

	return s;
}
#endif


// UNDONE:  Move these to a VisShape.cpp file (with some of the large
// CVisShape methods from VisShape.inl)?

#ifndef VIS_ONLY_DECLARE_PROPLIST
//VIS_DEFINE_SD_CLASS_WITH_IO_PL_TYPEDEF(inline, CVisShape)
VIS_SD_CLASS_WITH_IO_PL_TYPEDEF(CVisShape)

inline void CVisShape::BuildPropList(CVisPropList& refproplist)
{
	VisBuildObjPropList((RECT&) *this, refproplist);
	refproplist.AddPropReference("m_nbands", m_nbands, false);
}
#endif // VIS_ONLY_DECLARE_PROPLIST



#ifdef _DEBUG
#undef new
#endif // _DEBUG


#endif // __VIS_SHAPE_INL__
