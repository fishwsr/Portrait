///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISRGBAPIXELS
//
// @module VisRGBAPixels.inl |
//
// Templated CVisRGBA\<storage-type> arithmetic operator definitions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisImageTypes\.h>
//
// <nl>
// @index | VISRGBAPIXELS
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_RGBAPIXELS_INL__
#define __VIS_CORE_RGBAPIXELS_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisRGBA | CVisRGBA |
//
// Constructor.
// 
// @syntax CVisRGBA(void);
// @syntax CVisRGBA(const CVisRGBA &refrgba);
// @syntax CVisRGBA(TNumOther v);
// @syntax CVisRGBA(TNumOther r, TNumOther g, TNumOther b);
// @syntax CVisRGBA(TNumOther r, TNumOther g, TNumOther b, TNumOther a);
//
// @tfarg class | TNumOther |
// The type of numbers passed to the constructor.  These numbers will be
// cast to the storage type of this class.
//  
// @parm TNumOther | v |
// If given, all components should be set to this value.  (We might
// choose to set alpha to a different value in the final release.)
//
// @parm TNumOther | r |
// Value of the red component.
//
// @parm TNumOther | g |
// Value of the green component.
//
// @parm TNumOther | b |
// Value of the blue component.
//
// @parm TNumOther | a |
// Value of the alpha component.
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline CVisRGBA<TNum>::CVisRGBA(void)
{
}

template <class TNum>
inline CVisRGBA<TNum>::CVisRGBA(TNum num)
  : m_numR(num),
	m_numG(num),
	m_numB(num)
{
}


inline CVisRGBA<BYTE>::CVisRGBA(const CVisRGBA<BYTE>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisRGBA<BYTE>) == sizeof(int));
#endif // VIS_CORE_EXTRA_ASSERTS
	*((int *) this) = *((int *) &refrgba);
}

inline CVisRGBA<signed char>::CVisRGBA(const CVisRGBA<signed char>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisRGBA<signed char>) == sizeof(int));
#endif // VIS_CORE_EXTRA_ASSERTS
	*((int *) this) = *((int *) &refrgba);
}

inline CVisRGBA<short>::CVisRGBA(const CVisRGBA<short>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisRGBA<short>) == sizeof(double));
#endif // VIS_CORE_EXTRA_ASSERTS
	*((double *) this) = *((double *) &refrgba);
}

inline CVisRGBA<unsigned short>::CVisRGBA(const CVisRGBA<unsigned short>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisRGBA<unsigned short>) == sizeof(double));
#endif // VIS_CORE_EXTRA_ASSERTS
	*((double *) this) = *((double *) &refrgba);
}

template <class TNum>
inline CVisRGBA<TNum>::CVisRGBA(const CVisRGBA<TNum>& refrgba)
  : m_numR(refrgba.R()),
	m_numG(refrgba.G()),
	m_numB(refrgba.B()),
	m_numA(refrgba.A())
{
}

template <class TNum>
inline CVisRGBA<TNum>::CVisRGBA(TNum numR, TNum numG, TNum numB)
  : m_numR(numR),
	m_numG(numG),
	m_numB(numB)
{
}

template <class TNum>
inline CVisRGBA<TNum>::CVisRGBA(TNum numR, TNum numG, TNum numB, TNum numA)
  : m_numR(numR),
	m_numG(numG),
	m_numB(numB),
	m_numA(numA)
{
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisRGBA<TNum>& | CVisRGBA | operator= |
//
// Assignment operator.
//
// @syntax CVisRGBA& operator=(const CVisRGBA<TNum>& refrgba);
// @syntax CVisRGBA& operator=(TNum num);
//
// @parm TNum | num |
// A value to be assigned to the R, G, and B components.
//
// @parm const CVisRGBA<TNum>& | refrgba |
// A <c CVisRGBA> object whose values are to be copied.
//
////////////////////////////////////////////////////////////////////////////
inline CVisRGBA<BYTE>& CVisRGBA<BYTE>::operator=(
		const CVisRGBA<BYTE>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(sizeof(CVisRGBA<BYTE>) == sizeof(int));
#endif // VIS_CORE_EXTRA_ASSERTS

	*((int *) this) = *((int *) &refrgba);

	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator=(
		const CVisRGBA<TNum>& refrgba)
{
	SetR((TNum) refrgba.R());
	SetG((TNum) refrgba.G());
	SetB((TNum) refrgba.B());
	SetA((TNum) refrgba.A());

	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator=(TNum num)
{
	SetR(num);
	SetG(num);
	SetB(num);

	return *this;
}


inline bool CVisRGBA<BYTE>::operator==(
		const CVisRGBA<BYTE>& refrgba) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(sizeof(CVisRGBA<BYTE>) == sizeof(int));
#endif // VIS_CORE_EXTRA_ASSERTS

	// UNDONE (in Rel1):  Should alpha be included in the comparison?
	return (*((int *) this) == *((int *) &refrgba));
}

template <class TNum>
inline bool CVisRGBA<TNum>::operator==(
		const CVisRGBA<TNum>& refrgba) const
{
	// UNDONE (in Rel1):  Should alpha be included in the comparison?
	return ((R() == refrgba.R())
			&& (G() == refrgba.G())
			&& (B() == refrgba.B())
			&& (A() == refrgba.A()));
}

template <class TNum>
inline bool CVisRGBA<TNum>::operator!=(
		const CVisRGBA<TNum>& refrgba) const
{
	return !operator==(refrgba);
}

template <class TNum>
inline bool CVisRGBA<TNum>::operator<(
		const CVisRGBA<TNum>& refrgba) const
{
	return memcmp(this, &refrgba, sizeof(CVisRGBA<TNum>));
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TNum& | CVisRGBA | operator[] |
//
// Index operator.  The <t EVisIColor> constants specify the indices of
// the components in this object.
//
// @syntax const TNum &operator[](int n) const;
// @syntax TNum &operator[](int n);
//
// @parm int | i |
// The index of the component in this object.
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline const TNum& CVisRGBA<TNum>::operator[](int i) const
{
	return ((const TNum *) this)[i];
}

template <class TNum>
inline TNum& CVisRGBA<TNum>::operator[](int i)
{
	return ((TNum *) this)[i];
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const TNum& | CVisRGBA | R |
//
// Return the red component value.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline const TNum& CVisRGBA<TNum>::R(void) const
{
	return m_numR;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc const TNum& | CVisRGBA | G |
//
// Return the green component value.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline const TNum& CVisRGBA<TNum>::G(void) const
{
	return m_numG;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc const TNum& | CVisRGBA | B |
//
// Return the blue component value.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline const TNum& CVisRGBA<TNum>::B(void) const
{
	return m_numB;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc const TNum& | CVisRGBA | A |
//
// Return the alpha component value.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline const TNum& CVisRGBA<TNum>::A(void) const
{
	return m_numA;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisRGBA | SetR |
//
// Set the red component value.
//
// @parm TNum | num |
// The new red component value.
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline void CVisRGBA<TNum>::SetR(TNum num)
{
	m_numR = num;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisRGBA | SetG |
//
// Set the green component value.
//
// @parm TNum | num |
// The new green component value.
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline void CVisRGBA<TNum>::SetG(TNum num)
{
	m_numG = num;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisRGBA | SetB |
//
// Set the blue component value.
//
// @parm TNum | num |
// The new blue component value.
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline void CVisRGBA<TNum>::SetB(TNum num)
{
	m_numB = num;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisRGBA | SetA |
//
// Set the alpha component value.
//
// @parm TNum | num |
// The new alpha component value.
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline void CVisRGBA<TNum>::SetA(TNum num)
{
	m_numA = num;
}


template <class TNum>
inline void CVisRGBA<TNum>::SetRGB(TNum numR, TNum numG, TNum numB)
{
	m_numR = numR;
	m_numG = numG;
	m_numB = numB;
}


template <class TNum>
inline void CVisRGBA<TNum>::SetRGBA(TNum numR, TNum numG, TNum numB,
		TNum numA)
{
	m_numR = numR;
	m_numG = numG;
	m_numB = numB;
	m_numA = numA;
}


template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator+=(TNum num)
{
	m_numR += num;
	m_numG += num;
	m_numB += num;
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator+=(const CVisRGBA<TNum>& refrgba)
{
	m_numR += refrgba.R();
	m_numG += refrgba.G();
	m_numB += refrgba.B();
	
	return *this;
}


template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator-=(TNum num)
{
	m_numR -= num;
	m_numG -= num;
	m_numB -= num;
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator-=(const CVisRGBA<TNum>& refrgba)
{
	m_numR -= refrgba.R();
	m_numG -= refrgba.G();
	m_numB -= refrgba.B();
	
	return *this;
}


template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator*=(int i)
{
	m_numR = (TNum) (m_numR * i);
	m_numG = (TNum) (m_numG * i);
	m_numB = (TNum) (m_numB * i);
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator*=(unsigned int ui)
{
	m_numR = (TNum) (m_numR * ui);
	m_numG = (TNum) (m_numG * ui);
	m_numB = (TNum) (m_numB * ui);
	
	return *this;
}


template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator*=(long l)
{
	m_numR = (TNum) (m_numR * l);
	m_numG = (TNum) (m_numG * l);
	m_numB = (TNum) (m_numB * l);
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator*=(unsigned long ul)
{
	m_numR = (TNum) (m_numR * ul);
	m_numG = (TNum) (m_numG * ul);
	m_numB = (TNum) (m_numB * ul);
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator*=(float flt)
{
	m_numR = (TNum) (m_numR * flt);
	m_numG = (TNum) (m_numG * flt);
	m_numB = (TNum) (m_numB * flt);
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator*=(double dbl)
{
	m_numR = (TNum) (m_numR * dbl);
	m_numG = (TNum) (m_numG * dbl);
	m_numB = (TNum) (m_numB * dbl);
	
	return *this;
}


template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator/=(int i)
{
	m_numR = (TNum) (m_numR / i);
	m_numG = (TNum) (m_numG / i);
	m_numB = (TNum) (m_numB / i);
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator/=(unsigned int ui)
{
	m_numR = (TNum) (m_numR / ui);
	m_numG = (TNum) (m_numG / ui);
	m_numB = (TNum) (m_numB / ui);
	
	return *this;
}


template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator/=(long l)
{
	m_numR = (TNum) (m_numR / l);
	m_numG = (TNum) (m_numG / l);
	m_numB = (TNum) (m_numB / l);
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator/=(unsigned long ul)
{
	m_numR = (TNum) (m_numR / ul);
	m_numG = (TNum) (m_numG / ul);
	m_numB = (TNum) (m_numB / ul);
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator/=(float flt)
{
	m_numR = (TNum) (m_numR / flt);
	m_numG = (TNum) (m_numG / flt);
	m_numB = (TNum) (m_numB / flt);
	
	return *this;
}

template <class TNum>
inline CVisRGBA<TNum>& CVisRGBA<TNum>::operator/=(double dbl)
{
	m_numR = (TNum) (m_numR / dbl);
	m_numG = (TNum) (m_numG / dbl);
	m_numB = (TNum) (m_numB / dbl);
	
	return *this;
}




//---------------------------------------------------------------------------------------------


// Helper functions

// Specialization
inline BYTE VisBrightnessFromRGBA(const CVisRGBA<BYTE>& refrgba)
{
	const unsigned int nYR8 = (((unsigned int) (0x2020202 * .299 + 1)) >> 1);
	const unsigned int nYG8 = (((unsigned int) (0x2020202 * .587 + 1)) >> 1);
	const unsigned int nYB8 = (((unsigned int) (0x2020202 * .114 + 1)) >> 1);
	return (BYTE)
			((nYB8 * refrgba.B() + nYG8 * refrgba.G() + nYR8 * refrgba.R())
					>> 24);
}

// General case
template <class TNum>
inline TNum VisBrightnessFromRGBA(const CVisRGBA<TNum>& refrgba)
{
	return (TNum) (.299 * refrgba.B() + .587 * refrgba.G() + .114 * refrgba.R());
}

// Helper function operator (used with
//    CVisGrayByteImage VisBrightnessFromRGBA(const CVisRGBAByteImage& refimageRGBA)
// defined in VisImageBase.cpp).

class CVisFnOpRGBAIntensity
{
public:
	template <class TNum>
	inline void operator()(TNum& refnumOut, const CVisRGBA<TNum>& refrgbaIn)
		{ refnumOut = VisBrightnessFromRGBA(refrgbaIn); }
};


template <class TNum>
CVisRGBA<TNum> operator+(const CVisRGBA<TNum>& refrgba1,
		const CVisRGBA<TNum>& refrgba2)
{
	return CVisRGBA<TNum>((TNum) (refrgba1.R() + refrgba2.R()),
			(TNum) (refrgba1.G() + refrgba2.G()),
			(TNum) (refrgba1.B() + refrgba2.B()));
}

template <class TNum>
CVisRGBA<TNum> operator-(const CVisRGBA<TNum>& refrgba1,
		const CVisRGBA<TNum>& refrgba2)
{
	return CVisRGBA<TNum>((TNum) (refrgba1.R() - refrgba2.R()),
			(TNum) (refrgba1.G() - refrgba2.G()),
			(TNum) (refrgba1.B() - refrgba2.B()));
}


template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, int i)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * i),
			(TNum) (refrgba.G() * i),
			(TNum) (refrgba.B() * i));
}

template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, unsigned int ui)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * ui),
			(TNum) (refrgba.G() * ui),
			(TNum) (refrgba.B() * ui));
}


template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, long l)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * l),
			(TNum) (refrgba.G() * l),
			(TNum) (refrgba.B() * l));
}

template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, unsigned long ul)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * ul),
			(TNum) (refrgba.G() * ul),
			(TNum) (refrgba.B() * ul));
}

template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, float flt)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * flt),
			(TNum) (refrgba.G() * flt),
			(TNum) (refrgba.B() * flt));
}

template <class TNum>
CVisRGBA<TNum> operator*(const CVisRGBA<TNum>& refrgba, double dbl)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * dbl),
			(TNum) (refrgba.G() * dbl),
			(TNum) (refrgba.B() * dbl));
}


template <class TNum>
CVisRGBA<TNum> operator*(int i, const CVisRGBA<TNum>& refrgba)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * i),
			(TNum) (refrgba.G() * i),
			(TNum) (refrgba.B() * i));
}

template <class TNum>
CVisRGBA<TNum> operator*(unsigned int ui, const CVisRGBA<TNum>& refrgba)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * ui),
			(TNum) (refrgba.G() * ui),
			(TNum) (refrgba.B() * ui));
}

template <class TNum>
CVisRGBA<TNum> operator*(long l, const CVisRGBA<TNum>& refrgba)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * l),
			(TNum) (refrgba.G() * l),
			(TNum) (refrgba.B() * l));
}

template <class TNum>
CVisRGBA<TNum> operator*(unsigned long ul, const CVisRGBA<TNum>& refrgba)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * ul),
			(TNum) (refrgba.G() * ul),
			(TNum) (refrgba.B() * ul));
}

template <class TNum>
CVisRGBA<TNum> operator*(float flt, const CVisRGBA<TNum>& refrgba)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * flt),
			(TNum) (refrgba.G() * flt),
			(TNum) (refrgba.B() * flt));
}

template <class TNum>
CVisRGBA<TNum> operator*(double dbl, const CVisRGBA<TNum>& refrgba)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() * dbl),
			(TNum) (refrgba.G() * dbl),
			(TNum) (refrgba.B() * dbl));
}


template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, int i)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() / i),
			(TNum) (refrgba.G() / i),
			(TNum) (refrgba.B() / i));
}

template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, unsigned int ui)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() / ui),
			(TNum) (refrgba.G() / ui),
			(TNum) (refrgba.B() / ui));
}

template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, long l)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() / l),
			(TNum) (refrgba.G() / l),
			(TNum) (refrgba.B() / l));
}

template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, unsigned long ul)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() / ul),
			(TNum) (refrgba.G() / ul),
			(TNum) (refrgba.B() / ul));
}

template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, float flt)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() / flt),
			(TNum) (refrgba.G() / flt),
			(TNum) (refrgba.B() / flt));
}

template <class TNum>
CVisRGBA<TNum> operator/(const CVisRGBA<TNum>& refrgba, double dbl)
{
	return CVisRGBA<TNum>((TNum) (refrgba.R() / dbl),
			(TNum) (refrgba.G() / dbl),
			(TNum) (refrgba.B() / dbl));
}



template <class TNum>
inline void CVisRGBA<TNum>::BuildPropList(CVisPropList& refproplist)
{
#ifndef VIS_ONLY_DECLARE_PROPLIST
	assert(refproplist.IsEmpty());

	refproplist.AddPropReference("red", m_numR, false);
	refproplist.AddPropReference("green", m_numG, false);
	refproplist.AddPropReference("blue", m_numB, false);
	refproplist.AddPropReference("alpha", m_numA, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}


//---------------------------------------------------------------------------------------------
// Old self-describing stream I/O


#ifdef VIS_INCLUDE_SDSTREAM

template <class TNum>
inline CVisSDStream& operator<<(CVisSDStream& s, CVisRGBA<TNum>& refrgba)
{
	s.OpenParen();
	s.Num(refrgba.m_numR);
	s.Comma();
	s.Num(refrgba.m_numG);
	s.Comma();
	s.Num(refrgba.m_numB);
	s.Comma();
	s.Num(refrgba.m_numA);
	s.CloseParen();

	return s;
}
#endif

#endif // ifndef __VIS_CORE_RGBAPIXELS_INL__
