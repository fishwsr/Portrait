///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISGRAYAPIXELS
//
// @module VisGrayAPixels.h |
//
// Templated <c CVisGrayA>\<storage-type> pixel definitions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @devnote:(INTERNAL)
//
// We should check the scalar types that we want to allow in the
// <c CVisGrayA> scalar assignment operators.
//
// Do we really need a templated CVisGrayA assignment operator?
//
// We should document the assignment operators supported in the
// final release.
//
// <nl>
// @xref <l VisImageTypes\.inl>
//
// <nl>
// @head3 Classes |
// @index class | VISGRAYAPIXELS
//
// @head3 Typedefs |
// @index type | VISGRAYAPIXELS
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_GRAYAPIXELS_H__
#define __VIS_CORE_GRAYAPIXELS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// Forward declaration
template <class TNum>
class CVisGrayA;



// @type CVisGrayACharPixel | <c CVisGrayA>\<c signed char>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<signed char>		CVisGrayACharPixel;

// @type CVisGrayAShortPixel | <c CVisGrayA>\<short>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<short>			CVisGrayAShortPixel;

// @type CVisGrayAIntPixel | <c CVisGrayA>\<int>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<int>				CVisGrayAIntPixel;

// @type CVisGrayALongPixel | <c CVisGrayA>\<long>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<long>			CVisGrayALongPixel;


// @type CVisGrayABytePixel | <c CVisGrayA>\<unsigned char>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<unsigned char>	CVisGrayABytePixel;
typedef CVisGrayA<unsigned char>	CVisGrayAUCharPixel;

// @type CVisGrayAUShortPixel | <c CVisGrayA>\<unsigned short>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<unsigned short>	CVisGrayAUShortPixel;

// @type CVisGrayAUIntPixel | <c CVisGrayA>\<unsigned int>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<unsigned int>	CVisGrayAUIntPixel;

// @type CVisGrayAULongPixel | <c CVisGrayA>\<unsigned long>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<unsigned long>	CVisGrayAULongPixel;


// @type CVisGrayAFloatPixel | <c CVisGrayA>\<float>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<float>			CVisGrayAFloatPixel;

// @type CVisGrayADoublePixel | <c CVisGrayA>\<double>
// @xref <l VisYUVAPixels\.h>
typedef CVisGrayA<double>			CVisGrayADoublePixel;

////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisIColor |
// Indices used to access components in a <c CVisGrayA> object.
//
// @xref <l VisYUVAPixels\.h>
//
////////////////////////////////////////////////////////////////////////////
enum EVisIGray
{
	// @emem
	// Red component index.
	evisigrayIntensity			= 0,

	// @emem
	// Alpha component index.
	evisigrayAlpha				= 1
};



////////////////////////////////////////////////////////////////////////////
//  
// @class CVisGrayA |
//
// Object containing red, green, blue, and alpha values.  Used as
// pixels in color images.  (This class also includes standard
// arithmetic operators that are not listed in this documentation.)
//
// LATER:  We might try replacing the methods that take TNum with methods
// that take int, uint, long, ulong, float, and double.
//
// @tcarg class | TNum |
// The type of numbers used to store the red, green, blue, and alpha
// values.
//  
// @xref <l VisYUVAPixels\.h>
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
class CVisGrayA
{
public:
    // @cmember
	// Default constructor.
	CVisGrayA(void);

    // @cmember
	// Constructor from an other <c CVisGrayA> object.
	CVisGrayA(const CVisGrayA<TNum>& refrgba);
	
    // @cmember
    // @cmember
	// Constructor (from intensity value).  Alpha will not be
	// initialized.
	CVisGrayA(TNum numI);

    // @cmember
	// Constructor (from red, green, blue, and alpha values).
	CVisGrayA(TNum numI, TNum numA);

    // @cmember
	// Assignment operator.
	CVisGrayA<TNum>& operator=(const CVisGrayA<TNum>& refrgba);

    // @cmember
	// Assignment operator.
	CVisGrayA<TNum>& operator=(TNum num);


	// @cmember
	// 
	bool operator==(const CVisGrayA<TNum>& refrgba) const;

	// @cmember
	// 
	bool operator!=(const CVisGrayA<TNum>& refrgba) const;

	// @cmember
	// This method is required for use in STL containers.
	bool operator<(const CVisGrayA<TNum>& refrgba) const;


    // @cmember
	// Index operator.  The <t EVisIColor> constants specify the indices of
	// the components in this object.
	const TNum &operator[](int i) const;

    // @cmember
	// Index operator.  The <t EVisIColor> constants specify the indices of
	// the components in this object.
	TNum &operator[](int i);


    // @cmember
	// Return the intensity component value.
	const TNum &I(void) const;

    // @cmember
	// Return the alpha component value.
	const TNum &A(void) const;

    // @cmember
	// Set the red component value.
	void SetI(TNum num);

    // @cmember
	// Set the alpha component value.
	void SetA(TNum num);

    // @cmember
	// Set the red, green, blue, and alpha values.
	void SetIA(TNum numI, TNum numA);

	CVisGrayA<TNum>& operator+=(TNum num);
	CVisGrayA<TNum>& operator+=(const CVisGrayA<TNum>& refrgba);

	CVisGrayA<TNum>& operator-=(TNum num);
	CVisGrayA<TNum>& operator-=(const CVisGrayA<TNum>& refrgba);

	CVisGrayA<TNum>& operator*=(int i);
	CVisGrayA<TNum>& operator*=(unsigned int ui);
	CVisGrayA<TNum>& operator*=(long l);
	CVisGrayA<TNum>& operator*=(unsigned long ul);
	CVisGrayA<TNum>& operator*=(float flt);
	CVisGrayA<TNum>& operator*=(double dbl);

	CVisGrayA<TNum>& operator/=(int i);
	CVisGrayA<TNum>& operator/=(unsigned int ui);
	CVisGrayA<TNum>& operator/=(long l);
	CVisGrayA<TNum>& operator/=(unsigned long ul);
	CVisGrayA<TNum>& operator/=(float flt);
	CVisGrayA<TNum>& operator/=(double dbl);

	void BuildPropList(CVisPropList& refproplist);


protected:
#ifdef VIS_INCLUDE_SDSTREAM
	friend CVisSDStream& operator<<(CVisSDStream& s, CVisGrayA<TNum>& refrgba);
#endif
    // @cmember:(IMPL)
	// The intensity component.
	TNum m_numI;

    // @cmember:(IMPL)
	// The alpha component.
	TNum m_numA;
};



// Helper function

template <class TNum>
CVisGrayA<TNum> operator+(const CVisGrayA<TNum>& refrgba1,
		const CVisGrayA<TNum>& refrgba2);
template <class TNum>
CVisGrayA<TNum> operator-(const CVisGrayA<TNum>& refrgba1,
		const CVisGrayA<TNum>& refrgba2);

template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, int i);
template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, unsigned int ui);
template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, long l);
template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, unsigned long ul);
template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, float flt);
template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, double dbl);

template <class TNum>
CVisGrayA<TNum> operator*(int i, const CVisGrayA<TNum>& refrgba);
template <class TNum>
CVisGrayA<TNum> operator*(unsigned int ui, const CVisGrayA<TNum>& refrgba);
template <class TNum>
CVisGrayA<TNum> operator*(long l, const CVisGrayA<TNum>& refrgba);
template <class TNum>
CVisGrayA<TNum> operator*(unsigned long ul, const CVisGrayA<TNum>& refrgba);
template <class TNum>
CVisGrayA<TNum> operator*(float flt, const CVisGrayA<TNum>& refrgba);
template <class TNum>
CVisGrayA<TNum> operator*(double dbl, const CVisGrayA<TNum>& refrgba);

template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, int i);
template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, unsigned int ui);
template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, long l);
template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, unsigned long ul);
template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, float flt);
template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, double dbl);



#ifndef VIS_ONLY_DECLARE_PROPLIST

// Specialized PropList helper functions for common sequence types
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisGrayA<BYTE> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisGrayA<int> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisGrayA<float> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisGrayA<double> const &refobj, bool fAlwaysTypedef);

VIS_TEMPLATE_SD_CLASS_WITH_IO_PL_TYPEDEF(TNum, CVisGrayA<TNum>)

#endif // !VIS_ONLY_DECLARE_PROPLIST


//---------------------------------------------------------------------------------------------
// Old self-describing stream I/O

#ifdef VIS_INCLUDE_SDSTREAM
template <class TNum>
inline CVisSDStream& operator<<(CVisSDStream& s, CVisGrayA<TNum>& o);
#endif


#include "VisGrayAPixels.inl"


#endif // __VIS_CORE_GRAYAPIXELS_H__
