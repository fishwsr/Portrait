///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISGRAYAPIXELS
//
// @module VisGRAYAPIXELS.inl |
//
// Templated CVisGrayA\<storage-type> arithmetic operator definitions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisImageTypes\.h>
//
// <nl>
// @index | VISGRAYAPIXELS
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_GRAYAPIXELS_INL__
#define __VIS_CORE_GRAYAPIXELS_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisGrayA | CVisGrayA |
//
// Constructor.
// 
// @syntax CVisGrayA(void);
// @syntax CVisGrayA(const CVisGrayA &refrgba);
// @syntax CVisGrayA(TNumOther v);
// @syntax CVisGrayA(TNumOther r, TNumOther g, TNumOther b);
// @syntax CVisGrayA(TNumOther r, TNumOther g, TNumOther b, TNumOther a);
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
inline CVisGrayA<TNum>::CVisGrayA(void)
{
}

template <class TNum>
inline CVisGrayA<TNum>::CVisGrayA(TNum num)
  : m_numI(num)
{
}

inline CVisGrayA<BYTE>::CVisGrayA(const CVisGrayA<BYTE>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisGrayA<BYTE>) == sizeof(short));
#endif // VIS_CORE_EXTRA_ASSERTS
	*((short *) this) = *((short *) &refrgba);
}

inline CVisGrayA<signed char>::CVisGrayA(const CVisGrayA<signed char>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisGrayA<signed char>) == sizeof(short));
#endif // VIS_CORE_EXTRA_ASSERTS
	*((short *) this) = *((short *) &refrgba);
}

inline CVisGrayA<short>::CVisGrayA(const CVisGrayA<short>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisGrayA<short>) == sizeof(long));
#endif // VIS_CORE_EXTRA_ASSERTS
	*((long *) this) = *((long *) &refrgba);
}

inline CVisGrayA<unsigned short>::CVisGrayA(const CVisGrayA<unsigned short>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisGrayA<unsigned short>) == sizeof(long));
#endif // VIS_CORE_EXTRA_ASSERTS
	*((long *) this) = *((long *) &refrgba);
}

template <class TNum>
inline CVisGrayA<TNum>::CVisGrayA(const CVisGrayA<TNum>& refrgba)
  : m_numI(refrgba.I()),
	m_numA(refrgba.A())
{
}

template <class TNum>
inline CVisGrayA<TNum>::CVisGrayA(TNum numI, TNum numA)
  : m_numI(numI),
	m_numA(numA)
{
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisGrayA<TNum>& | CVisGrayA | operator= |
//
// Assignment operator.
//
// @syntax CVisGrayA& operator=(const CVisGrayA<TNum>& refrgba);
// @syntax CVisGrayA& operator=(TNum num);
//
// @parm TNum | num |
// A value to be assigned to the R, G, and B components.
//
// @parm const CVisGrayA<TNum>& | refrgba |
// A <c CVisGrayA> object whose values are to be copied.
//
////////////////////////////////////////////////////////////////////////////
inline CVisGrayA<BYTE>& CVisGrayA<BYTE>::operator=(
		const CVisGrayA<BYTE>& refrgba)
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(sizeof(CVisGrayA<BYTE>) == sizeof(short));
#endif // VIS_CORE_EXTRA_ASSERTS

	*((short *) this) = *((short *) &refrgba);

	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator=(
		const CVisGrayA<TNum>& refrgba)
{
	SetI((TNum) refrgba.I());
	SetA((TNum) refrgba.A());

	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator=(TNum num)
{
	SetI(num);

	return *this;
}


inline bool CVisGrayA<BYTE>::operator==(
		const CVisGrayA<BYTE>& refrgba) const
{
#ifdef VIS_CORE_EXTRA_ASSERTS
	assert(sizeof(CVisGrayA<BYTE>) == sizeof(short));
#endif // VIS_CORE_EXTRA_ASSERTS

	// UNDONE (in Rel1):  Should alpha be included in the comparison?
	return (*((short *) this) == *((short *) &refrgba));
}

template <class TNum>
inline bool CVisGrayA<TNum>::operator==(
		const CVisGrayA<TNum>& refrgba) const
{
	// UNDONE (in Rel1):  Should alpha be included in the comparison?
	return ((I() == refrgba.I())
			&& (A() == refrgba.A()));
}

template <class TNum>
inline bool CVisGrayA<TNum>::operator!=(
		const CVisGrayA<TNum>& refrgba) const
{
	return !operator==(refrgba);
}

template <class TNum>
inline bool CVisGrayA<TNum>::operator<(
		const CVisGrayA<TNum>& refrgba) const
{
	return memcmp(this, &refrgba, sizeof(CVisGrayA<TNum>));
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TNum& | CVisGrayA | operator[] |
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
inline const TNum& CVisGrayA<TNum>::operator[](int i) const
{
	return ((const TNum *) this)[i];
}

template <class TNum>
inline TNum& CVisGrayA<TNum>::operator[](int i)
{
	return ((TNum *) this)[i];
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const TNum& | CVisGrayA | R |
//
// Return the red component value.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline const TNum& CVisGrayA<TNum>::I(void) const
{
	return m_numI;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc const TNum& | CVisGrayA | A |
//
// Return the alpha component value.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline const TNum& CVisGrayA<TNum>::A(void) const
{
	return m_numA;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisGrayA | SetI |
//
// Set the intensity component value.
//
// @parm TNum | num |
// The new intensity component value.
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline void CVisGrayA<TNum>::SetI(TNum num)
{
	m_numI = num;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisGrayA | SetA |
//
// Set the alpha component value.
//
// @parm TNum | num |
// The new alpha component value.
//
////////////////////////////////////////////////////////////////////////////
template <class TNum>
inline void CVisGrayA<TNum>::SetA(TNum num)
{
	m_numA = num;
}

template <class TNum>
inline void CVisGrayA<TNum>::SetIA(TNum numI, TNum numA)
{
	m_numI = numR;
	m_numA = numA;
}


template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator+=(TNum num)
{
	m_numI += num;
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator+=(const CVisGrayA<TNum>& refrgba)
{
	m_numI += refrgba.R();
	
	return *this;
}


template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator-=(TNum num)
{
	m_numI -= num;
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator-=(const CVisGrayA<TNum>& refrgba)
{
	m_numI -= refrgba.R();
	
	return *this;
}


template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator*=(int i)
{
	m_numI = (TNum) (m_numI * i);
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator*=(unsigned int ui)
{
	m_numI = (TNum) (m_numI * ui);
	
	return *this;
}


template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator*=(long l)
{
	m_numI = (TNum) (m_numI * l);
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator*=(unsigned long ul)
{
	m_numI = (TNum) (m_numI * ul);
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator*=(float flt)
{
	m_numI = (TNum) (m_numI * flt);
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator*=(double dbl)
{
	m_numI = (TNum) (m_numI * dbl);
	
	return *this;
}


template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator/=(int i)
{
	m_numI = (TNum) (m_numI / i);
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator/=(unsigned int ui)
{
	m_numI = (TNum) (m_numI / ui);
	
	return *this;
}


template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator/=(long l)
{
	m_numI = (TNum) (m_numI / l);
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator/=(unsigned long ul)
{
	m_numI = (TNum) (m_numI / ul);
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator/=(float flt)
{
	m_numI = (TNum) (m_numI / flt);
	
	return *this;
}

template <class TNum>
inline CVisGrayA<TNum>& CVisGrayA<TNum>::operator/=(double dbl)
{
	m_numI = (TNum) (m_numI / dbl);
	
	return *this;
}




//---------------------------------------------------------------------------------------------


// Helper functions

template <class TNum>
CVisGrayA<TNum> operator+(const CVisGrayA<TNum>& refrgba1,
		const CVisGrayA<TNum>& refrgba2)
{
	return CVisGrayA<TNum>((TNum) (refrgba1.I() + refrgba2.I()));
}

template <class TNum>
CVisGrayA<TNum> operator-(const CVisGrayA<TNum>& refrgba1,
		const CVisGrayA<TNum>& refrgba2)
{
	return CVisGrayA<TNum>((TNum) (refrgba1.I() - refrgba2.I()));
}


template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, int i)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * i));
}

template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, unsigned int ui)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * ui));
}


template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, long l)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * l));
}

template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, unsigned long ul)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * ul));
}

template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, float flt)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * flt));
}

template <class TNum>
CVisGrayA<TNum> operator*(const CVisGrayA<TNum>& refrgba, double dbl)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * dbl));
}


template <class TNum>
CVisGrayA<TNum> operator*(int i, const CVisGrayA<TNum>& refrgba)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * i));
}

template <class TNum>
CVisGrayA<TNum> operator*(unsigned int ui, const CVisGrayA<TNum>& refrgba)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * ui));
}

template <class TNum>
CVisGrayA<TNum> operator*(long l, const CVisGrayA<TNum>& refrgba)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * l));
}

template <class TNum>
CVisGrayA<TNum> operator*(unsigned long ul, const CVisGrayA<TNum>& refrgba)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * ul));
}

template <class TNum>
CVisGrayA<TNum> operator*(float flt, const CVisGrayA<TNum>& refrgba)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * flt));
}

template <class TNum>
CVisGrayA<TNum> operator*(double dbl, const CVisGrayA<TNum>& refrgba)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() * dbl));
}


template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, int i)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() / i));
}

template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, unsigned int ui)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() / ui));
}

template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, long l)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() / l));
}

template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, unsigned long ul)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() / ul));
}

template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, float flt)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() / flt));
}

template <class TNum>
CVisGrayA<TNum> operator/(const CVisGrayA<TNum>& refrgba, double dbl)
{
	return CVisGrayA<TNum>((TNum) (refrgba.I() / dbl));
}



template <class TNum>
inline void CVisGrayA<TNum>::BuildPropList(CVisPropList& refproplist)
{
#ifndef VIS_ONLY_DECLARE_PROPLIST
	assert(refproplist.IsEmpty());

	refproplist.AddPropReference("intensity", m_numI, false);
	refproplist.AddPropReference("alpha", m_numA, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}


//---------------------------------------------------------------------------------------------
// Old self-describing stream I/O


#ifdef VIS_INCLUDE_SDSTREAM

template <class TNum>
inline CVisSDStream& operator<<(CVisSDStream& s, CVisGrayA<TNum>& refrgba)
{
	s.OpenParen();
	s.Num(refrgba.m_numI);
	s.Comma();
	s.Num(refrgba.m_numA);
	s.CloseParen();

	return s;
}
#endif

#endif // ifndef __VIS_CORE_GRAYAPIXELS_INL__
