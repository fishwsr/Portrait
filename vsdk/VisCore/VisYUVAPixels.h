///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISYUVAPIXELS
//
// @module VisYUVAPixels.h |
//
// This file defines a templated class for pixels containing Y, U, V,
// and A components of a specified numeric type.  It also defines
// standard pixel types based on this class.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// <nl>
// @head3 Classes |
// @index class | VISYUVAPIXELS
//
// @head3 Typedefs |
// @index type | VISYUVAPIXELS
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_YUVAPIXELS_H__
#define __VIS_CORE_YUVAPIXELS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// Forward declaration
template <class TNumYA, class TNumUV>
class CVisYUVA;



#ifndef VIS_DEFINED_SVIS_YUV422_16BPP

	#define VIS_DEFINED_SVIS_YUV422_16BPP

	struct SVisYUYV { BYTE bY1, bU, bY2, bV; };
	struct SVisYVYU { BYTE bY1, bV, bY2, bU; };
	struct SVisUYVY { BYTE bU, bY1, bV, bY2; };

#endif // VIS_DEFINED_SVIS_YUV422_16BPP



// @type CVisYUVACharPixel | <c CVisYUVA>\<BYTE, signed char>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<BYTE, signed char>			CVisYUVACharPixel;

// @type CVisYUVAShortPixel | <c CVisYUVA>\<unsigned short, short>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<unsigned short, short>		CVisYUVAShortPixel;

// @type CVisYUVAIntPixel | <c CVisYUVA>\<unsigned int, int>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<unsigned int, int>			CVisYUVAIntPixel;

// @type CVisYUVALongPixel | <c CVisYUVA>\<unsigned long, long>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<unsigned long, long>			CVisYUVALongPixel;


// @type CVisYUVABytePixel | <c CVisYUVA>\<BYTE, signed char>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<unsigned char, signed char>	CVisYUVABytePixel;
typedef CVisYUVA<unsigned char, signed char>	CVisYUVAUCharPixel;

// @type CVisYUVAUShortPixel | <c CVisYUVA>\<unsigned short, short>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<unsigned short, short>		CVisYUVAUShortPixel;

// @type CVisYUVAUIntPixel | <c CVisYUVA>\<unsigned int, int>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<unsigned int, int>			CVisYUVAUIntPixel;

// @type CVisYUVAULongPixel | <c CVisYUVA>\<unsigned long, long>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<unsigned long, long>			CVisYUVAULongPixel;


// @type CVisYUVAFloatPixel | <c CVisYUVA>\<float, float>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<float, float>				CVisYUVAFloatPixel;

// @type CVisYUVADoublePixel | <c CVisYUVA>\<double, double>
// @xref <l VisYUVAPixels\.h>
typedef CVisYUVA<double, double>				CVisYUVADoublePixel;



////////////////////////////////////////////////////////////////////////////
//  
// @class
//  
// This templated class defines pixels containing Y, U, V, and A components
// of a specified numeric type.
//  
// @tcarg class | TNum | Type of numbers used for the Y, U, V, and A values.
//
// @xref <l VisYUVAPixels\.h>
//
////////////////////////////////////////////////////////////////////////////
template<class TNumYA, class TNumUV>
class CVisYUVA
{
public:
	// @cmember
	// 
	CVisYUVA(void);

	// @cmember
	// 
	CVisYUVA(TNumYA numY, TNumUV numU, TNumUV numV);

	// @cmember
	// 
	CVisYUVA(TNumYA numY, TNumUV numU, TNumUV numV, TNumYA numA);

	// @cmember
	// 
	CVisYUVA(TNumYA numY);

	// @cmember
	// 
	CVisYUVA(const CVisYUVA<TNumYA, TNumUV>& refyuva);

	// @cmember
	// 
	CVisYUVA<TNumYA, TNumUV>& operator=(TNumYA num);

	// @cmember
	// 
	CVisYUVA<TNumYA, TNumUV>& operator=(const CVisYUVA<TNumYA, TNumUV>& refyuva);


	// @cmember
	// 
	bool operator==(const CVisYUVA<TNumYA, TNumUV>& refyuva) const;

	// @cmember
	// 
	bool operator!=(const CVisYUVA<TNumYA, TNumUV>& refyuva) const;

	// @cmember
	// This method is required for use in STL containers.
	bool operator<(const CVisYUVA<TNumYA, TNumUV>& refyuva) const;


	// @cmember
	// 
	const TNumYA& Y(void) const;

	// @cmember
	// 
	const TNumUV& U(void) const;

	// @cmember
	// 
	const TNumUV& V(void) const;

	// @cmember
	// 
	const TNumYA& A(void) const;


	// @cmember
	// 
	void SetY(const TNumYA& num);

	// @cmember
	// 
	void SetU(const TNumUV& num);

	// @cmember
	// 
	void SetV(const TNumUV& num);

	// @cmember
	// 
	void SetA(const TNumYA& num);

	// @cmember
	// 
	void SetYUV(TNumYA numY, TNumUV numU, TNumUV numV);

	// @cmember
	// 
	void SetYUVA(TNumYA numY, TNumUV numU, TNumUV numV, TNumYA numA);


#ifdef LATER  // Not sure how to handle UV offset in calculations
	CVisYUVA<TNum>& operator+=(TNum num);
	CVisYUVA<TNum>& operator+=(const CVisYUVA<TNum>& refyuva);

	CVisYUVA<TNum>& operator-=(TNum num);
	CVisYUVA<TNum>& operator-=(const CVisYUVA<TNum>& refyuva);

	CVisYUVA<TNum>& operator*=(int i);
	CVisYUVA<TNum>& operator*=(unsigned int ui);
	CVisYUVA<TNum>& operator*=(long l);
	CVisYUVA<TNum>& operator*=(unsigned long ul);
	CVisYUVA<TNum>& operator*=(float flt);
	CVisYUVA<TNum>& operator*=(double dbl);

	CVisYUVA<TNum>& operator/=(int i);
	CVisYUVA<TNum>& operator/=(unsigned int ui);
	CVisYUVA<TNum>& operator/=(long l);
	CVisYUVA<TNum>& operator/=(unsigned long ul);
	CVisYUVA<TNum>& operator/=(float flt);
	CVisYUVA<TNum>& operator/=(double dbl);
#endif // LATER


	void BuildPropList(CVisPropList& refproplist);


protected:
#ifdef VIS_INCLUDE_SDSTREAM
	friend CVisSDStream& operator<<(CVisSDStream& s, CVisYUVA<TNumYA, TNumUV>& o);
#endif
	TNumYA m_numY;
	TNumUV m_numU;
	TNumUV m_numV;
	TNumYA m_numA;
};



// Helper functions

template<class TNumYA, class TNumUV>
inline TNumYA VisBrightnessFromYUVA(const CVisYUVA<TNumYA, TNumUV>& refyuva)
	{ return refyuva.Y(); }

// LATER:  Add
// CVisImage<TNumYA> VisBrightnessFromYUVA(
//		const CVisImage<CVisYUVA<TNumYA, TNumUV> >& refimageYUVA)
// to VisImage.h?

class CVisFnOpYUVAIntensity
{
public:
	template<class TNumYA, class TNumUV>
	inline void operator()(TNumYA& refnumOut,
			const CVisYUVA<TNumYA, TNumUV>& refyuva)
		{ refnumOut = VisBrightnessFromYUVA(refyuva); }
};



#ifdef LATER  // Not sure how to handle UV offset in calculations
template <class TNum>
CVisYUVA<TNum> operator+(const CVisYUVA<TNum>& refyuva1,
		const CVisYUVA<TNum>& refyuva2);
template <class TNum>
CVisYUVA<TNum> operator-(const CVisYUVA<TNum>& refyuva1,
		const CVisYUVA<TNum>& refyuva2);

template <class TNum>
CVisYUVA<TNum> operator*(const CVisYUVA<TNum>& refyuva, int i);
template <class TNum>
CVisYUVA<TNum> operator*(const CVisYUVA<TNum>& refyuva, unsigned int ui);
template <class TNum>
CVisYUVA<TNum> operator*(const CVisYUVA<TNum>& refyuva, long l);
template <class TNum>
CVisYUVA<TNum> operator*(const CVisYUVA<TNum>& refyuva, unsigned long ul);
template <class TNum>
CVisYUVA<TNum> operator*(const CVisYUVA<TNum>& refyuva, float flt);
template <class TNum>
CVisYUVA<TNum> operator*(const CVisYUVA<TNum>& refyuva, double dbl);

template <class TNum>
CVisYUVA<TNum> operator*(int i, const CVisYUVA<TNum>& refyuva);
template <class TNum>
CVisYUVA<TNum> operator*(unsigned int ui, const CVisYUVA<TNum>& refyuva);
template <class TNum>
CVisYUVA<TNum> operator*(long l, const CVisYUVA<TNum>& refyuva);
template <class TNum>
CVisYUVA<TNum> operator*(unsigned long ul, const CVisYUVA<TNum>& refyuva);
template <class TNum>
CVisYUVA<TNum> operator*(float flt, const CVisYUVA<TNum>& refyuva);
template <class TNum>
CVisYUVA<TNum> operator*(double dbl, const CVisYUVA<TNum>& refyuva);

template <class TNum>
CVisYUVA<TNum> operator/(const CVisYUVA<TNum>& refyuva, int i);
template <class TNum>
CVisYUVA<TNum> operator/(const CVisYUVA<TNum>& refyuva, unsigned int ui);
template <class TNum>
CVisYUVA<TNum> operator/(const CVisYUVA<TNum>& refyuva, long l);
template <class TNum>
CVisYUVA<TNum> operator/(const CVisYUVA<TNum>& refyuva, unsigned long ul);
template <class TNum>
CVisYUVA<TNum> operator/(const CVisYUVA<TNum>& refyuva, float flt);
template <class TNum>
CVisYUVA<TNum> operator/(const CVisYUVA<TNum>& refyuva, double dbl);
#endif // LATER



//---------------------------------------------------------------------------------------------
// Old self-describing stream I/O

#ifdef VIS_INCLUDE_SDSTREAM
template <class TNumYA, class TNumUV>
inline CVisSDStream& operator<<(CVisSDStream& s, CVisYUVA<TNumYA, TNumUV>& o);
#endif


#include "VisYUVAPixels.inl"


#endif // __VIS_CORE_YUVAPIXELS_H__

