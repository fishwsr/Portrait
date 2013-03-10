///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISRGBAPIXELS
//
// @module VisRGBAPixels.h |
//
// Templated <c CVisRGBA>\<storage-type> pixel definitions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @devnote:(INTERNAL)
//
// We should check the scalar types that we want to allow in the
// <c CVisRGBA> scalar assignment operators.
//
// Do we really need a templated CVisRGBA assignment operator?
//
// We should document the assignment operators supported in the
// final release.
//
// <nl>
// @xref <l VisImageTypes\.inl>
//
// <nl>
// @head3 Classes |
// @index class | VISRGBAPIXELS
//
// @head3 Typedefs |
// @index type | VISRGBAPIXELS
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_RGBAPIXELS_H__
#define __VIS_CORE_RGBAPIXELS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// Forward declaration
template <class TNum>
class CVisRGBA;



// @type CVisRGBACharPixel | <c CVisRGBA>\<c signed char>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<signed char>		CVisRGBACharPixel;

// @type CVisRGBAShortPixel | <c CVisRGBA>\<short>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<short>			CVisRGBAShortPixel;

// @type CVisRGBAIntPixel | <c CVisRGBA>\<int>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<int>				CVisRGBAIntPixel;

// @type CVisRGBALongPixel | <c CVisRGBA>\<long>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<long>			CVisRGBALongPixel;


// @type CVisRGBABytePixel | <c CVisRGBA>\<unsigned char>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<unsigned char>	CVisRGBABytePixel;
typedef CVisRGBA<unsigned char>	CVisRGBAUCharPixel;

// @type CVisRGBAUShortPixel | <c CVisRGBA>\<unsigned short>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<unsigned short>	CVisRGBAUShortPixel;

// @type CVisRGBAUIntPixel | <c CVisRGBA>\<unsigned int>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<unsigned int>	CVisRGBAUIntPixel;

// @type CVisRGBAULongPixel | <c CVisRGBA>\<unsigned long>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<unsigned long>	CVisRGBAULongPixel;


// @type CVisRGBAFloatPixel | <c CVisRGBA>\<float>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<float>			CVisRGBAFloatPixel;

// @type CVisRGBADoublePixel | <c CVisRGBA>\<double>
// @xref <l VisYUVAPixels\.h>
typedef CVisRGBA<double>			CVisRGBADoublePixel;



// #define VIS_OLD_COLOR_NAMES

#ifdef VIS_OLD_COLOR_NAMES

#define CVisColorCharPixel			CVisRGBACharPixel
#define	CVisColorShortPixel			CVisRGBAShortPixel
#define	CVisColorIntPixel			CVisRGBAIntPixel
#define	CVisColorLongPixel			CVisRGBALongPixel
#define	CVisColorBytePixel			CVisRGBABytePixel
#define	CVisColorUShortPixel		CVisRGBAUShortPixel
#define	CVisColorUIntPixel			CVisRGBAUIntPixel
#define	CVisColorULongPixel			CVisRGBAULongPixel
#define	CVisColorFloatPixel			CVisRGBAFloatPixel
#define	CVisColorDoublePixel		CVisRGBADoublePixel

#endif // VIS_OLD_COLOR_NAMES


////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisIColor |
// Indices used to access components in a <c CVisRGBA> object.
//
// @xref <l VisYUVAPixels\.h>
//
////////////////////////////////////////////////////////////////////////////
enum EVisIColor
{
	// @emem
	// Red component index.
	evisicolorRed				= 2,

	// @emem
	// Green component index.
	evisicolorGreen				= 1,

	// @emem
	// Blue component index.
	evisicolorBlue				= 0,

	// @emem
	// Alpha component index.
	evisicolorAlpha				= 3
};



////////////////////////////////////////////////////////////////////////////
//  
// @class CVisRGBA |
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
class CVisRGBA
{
public:
    // @cmember
	// Default constructor.
	CVisRGBA(void);

    // @cmember
	// Constructor from an other <c CVisRGBA> object.
	CVisRGBA(const CVisRGBA<TNum>& refrgba);
	
    // @cmember
	// Constructor (from a single number).
	CVisRGBA(TNum num);

    // @cmember
	// Constructor (from red, green, and blue values).  Alpha will not be
	// initialized.
	CVisRGBA(TNum numR, TNum numG, TNum numB);

    // @cmember
	// Constructor (from red, green, blue, and alpha values).
	CVisRGBA(TNum numR, TNum numG, TNum numB, TNum numA);

    // @cmember
	// Assignment operator.
	CVisRGBA<TNum>& operator=(const CVisRGBA<TNum>& refrgba);

    // @cmember
	// Assignment operator.
	CVisRGBA<TNum>& operator=(TNum num);


	// @cmember
	// 
	bool operator==(const CVisRGBA<TNum>& refrgba) const;

	// @cmember
	// 
	bool operator!=(const CVisRGBA<TNum>& refrgba) const;

	// @cmember
	// This method is required for use in STL containers.
	bool operator<(const CVisRGBA<TNum>& refrgba) const;


    // @cmember
	// Index operator.  The <t EVisIColor> constants specify the indices of
	// the components in this object.
	const TNum &operator[](int i) const;

    // @cmember
	// Index operator.  The <t EVisIColor> constants specify the indices of
	// the components in this object.
	TNum &operator[](int i);


    // @cmember
	// Return the red component value.
	const TNum &R(void) const;

    // @cmember
	// Return the green component value.
	const TNum &G(void) const;

    // @cmember
	// Return the blue component value.
	const TNum &B(void) const;

    // @cmember
	// Return the alpha component value.
	const TNum &A(void) const;


    // @cmember
	// Set the red component value.
	void SetR(TNum num);

    // @cmember
	// Set the green component value.
	void SetG(TNum num);

    // @cmember
	// Set the blue component value.
	void SetB(TNum num);

    // @cmember
	// Set the alpha component value.
	void SetA(TNum num);

    // @cmember
	// Set the red, green, and blue values.
	void SetRGB(TNum numR, TNum numG, TNum numB);

    // @cmember
	// Set the red, green, blue, and alpha values.
	void SetRGBA(TNum numR, TNum numG, TNum numB, TNum numA);


	CVisRGBA<TNum>& operator+=(TNum num);
	CVisRGBA<TNum>& operator+=(const CVisRGBA<TNum>& refrgba);

	CVisRGBA<TNum>& operator-=(TNum num);
	CVisRGBA<TNum>& operator-=(const CVisRGBA<TNum>& refrgba);

	CVisRGBA<TNum>& operator*=(int i);
	CVisRGBA<TNum>& operator*=(unsigned int ui);
	CVisRGBA<TNum>& operator*=(long l);
	CVisRGBA<TNum>& operator*=(unsigned long ul);
	CVisRGBA<TNum>& operator*=(float flt);
	CVisRGBA<TNum>& operator*=(double dbl);

	CVisRGBA<TNum>& operator/=(int i);
	CVisRGBA<TNum>& operator/=(unsigned int ui);
	CVisRGBA<TNum>& operator/=(long l);
	CVisRGBA<TNum>& operator/=(unsigned long ul);
	CVisRGBA<TNum>& operator/=(float flt);
	CVisRGBA<TNum>& operator/=(double dbl);


#ifdef VIS_OLD_METHODS
	// These methods were in the old implementation of this class.  They
	// are included in this file to get old code working before it is
	// changed to use the new SetR(), SetG(), SetB(), and SetA() methods.
	// These methods will be removed in the final release of the SDK.
	// LATER:  Remove these methods.
	inline TNum &R(void)  { return m_numR; }
	inline TNum &G(void)  { return m_numG; }
	inline TNum &B(void)  { return m_numB; }
	inline TNum &A(void)  { return m_numA; }
#endif // VIS_OLD_METHODS


	void BuildPropList(CVisPropList& refproplist);


protected:
#ifdef VIS_INCLUDE_SDSTREAM
	friend CVisSDStream& operator<<(CVisSDStream& s, CVisRGBA<TNum>& refrgba);
#endif
    // @cmember:(IMPL)
	// The blue component.
	TNum m_numB;

    // @cmember:(IMPL)
	// The green component.
	TNum m_numG;

    // @cmember:(IMPL)
	// The red component.
	TNum m_numR;

    // @cmember:(IMPL)
	// The alpha component.
	TNum m_numA;
};



// Helper function

// General case
template <class TNum>
inline TNum VisBrightnessFromRGBA(const CVisRGBA<TNum>& refrgba);

// Specialization
inline BYTE VisBrightnessFromRGBA(const CVisRGBA<BYTE>& refrgba);

// CVisImage<TNum> VisBrightnessFromRGBA(
//		const CVisImage<CVisRGBA<TNum> >& refimageRGBA)
// is declared in VisImage.h.

// Old name.  (Use "brightness" in new code, as it better
// decribes the formula used.)
#define VisIntensityFromRGBA VisBrightnessFromRGBA


template <class TNum>
CVisRGBA<TNum> operator+(const CVisRGBA<TNum>& refrgba1,
		const CVisRGBA<TNum>& refrgba2);
template <class TNum>
CVisRGBA<TNum> operator-(const CVisRGBA<TNum>& refrgba1,
		const CVisRGBA<TNum>& refrgba2);

template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, int i);
template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, unsigned int ui);
template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, long l);
template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, unsigned long ul);
template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, float flt);
template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, double dbl);

template <class TNum>
CVisRGBA<TNum> operator*(int i, const CVisRGBA<TNum>& refrgba);
template <class TNum>
CVisRGBA<TNum> operator*(unsigned int ui, const CVisRGBA<TNum>& refrgba);
template <class TNum>
CVisRGBA<TNum> operator*(long l, const CVisRGBA<TNum>& refrgba);
template <class TNum>
CVisRGBA<TNum> operator*(unsigned long ul, const CVisRGBA<TNum>& refrgba);
template <class TNum>
CVisRGBA<TNum> operator*(float flt, const CVisRGBA<TNum>& refrgba);
template <class TNum>
CVisRGBA<TNum> operator*(double dbl, const CVisRGBA<TNum>& refrgba);

template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, int i);
template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, unsigned int ui);
template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, long l);
template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, unsigned long ul);
template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, float flt);
template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, double dbl);


#ifndef VIS_ONLY_DECLARE_PROPLIST

// Specialized PropList helper functions for common sequence types
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisRGBA<BYTE> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisRGBA<int> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisRGBA<float> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisRGBA<double> const &refobj, bool fAlwaysTypedef);

VIS_TEMPLATE_SD_CLASS_WITH_IO_PL_TYPEDEF(TNum, CVisRGBA<TNum>)

#endif // !VIS_ONLY_DECLARE_PROPLIST


//---------------------------------------------------------------------------------------------
// Old self-describing stream I/O

#ifdef VIS_INCLUDE_SDSTREAM
template <class TNum>
inline CVisSDStream& operator<<(CVisSDStream& s, CVisRGBA<TNum>& o);
#endif


#include "VisRGBAPixels.inl"


#endif // __VIS_CORE_RGBAPIXELS_H__
