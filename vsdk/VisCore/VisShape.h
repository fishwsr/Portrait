///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISSHAPE
//
// @module VisShape.h |
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
// @xref <l VisShape\.inl> <c CRect>
//
// <nl>
// @index | VISSHAPE
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_SHAPE_H__
#define __VIS_CORE_SHAPE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "VisCoreProj.h"


////////////////////////////////////////////////////////////////////////////
//  
// @class A CRect with a number of bands.
//  
// @xref <c CRect>
//
////////////////////////////////////////////////////////////////////////////
class CVisShape : public CVisRect
{
public:
	CVisShape(void);
	CVisShape(const CVisShape& shape);
	CVisShape(const RECT& rect, int nbands = 1);
	CVisShape(POINT point, SIZE size, int nbands = 1);
	CVisShape(int left, int top, int right, int bottom, int nbands = 1);

	int Left(void) const;
	int Top(void) const;
	int Right(void) const;
	int Bottom(void) const;
	int NBands(void) const;

	void SetLeft(int l);
	void SetTop(int t);
	void SetRight(int r);
	void SetBottom(int b);
	void SetNBands(int nbands);

	bool Includes(int x, int y, int nband = 0) const;
	bool Includes(POINT point, int nband = 0) const;
	bool Includes(const RECT& rect, int nband = 0) const;
	bool Includes(const CVisShape& refshape) const;  // requires same NBands()

	CVisShape Decimated(int wRate, bool expand = false) const;
	CVisShape& Decimate(int wRate, bool expand = false);

	CVisShape ScaledUp(int wRate) const;
	CVisShape& ScaleUp(int wRate);

	bool operator==(const CVisShape& shape) const;
	bool operator!=(const CVisShape& shape) const;

	CVisShape& operator=(const CVisShape& shape);
	CVisShape& operator=(const RECT& rect);

	CVisShape& operator&=(const RECT& rect);
	CVisShape& operator|=(const RECT& rect);
	CVisShape& operator&=(const CVisShape& shape);
	CVisShape& operator|=(const CVisShape& shape);

	CVisShape operator&(const RECT& rect) const;
	CVisShape operator|(const RECT& rect) const;
	CVisShape operator&(const CVisShape& shape) const;
	CVisShape operator|(const CVisShape& shape) const;

	CVisShape IntersectedWith(const RECT& rect) const;
	CVisShape IntersectedWith(const CVisShape& shape) const;
	CVisShape UnionedWith(const RECT& rect) const;
	CVisShape UnionedWith(const CVisShape& shape) const;


	// Helper method used with prop lists and stream I/O.
	VisCoreExport void BuildPropList(CVisPropList& refproplist);

protected:
#ifdef VIS_INCLUDE_SDSTREAM
	friend CVisSDStream& operator<<(CVisSDStream& s, CVisShape& refshape);
#endif
	int m_nbands;
};


#ifndef VIS_NO_SD_STREAM
VIS_DECLARE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisCoreExportDeclspec, CVisShape)
#endif // !VIS_NO_SD_STREAM


// Old self-describing stream I/O

#ifdef VIS_INCLUDE_SDSTREAM
inline CVisSDStream& operator<<(CVisSDStream& s, CVisShape& o);
#endif


#include "VisShape.inl"


#endif // __VIS_CORE_SHAPE_H__
