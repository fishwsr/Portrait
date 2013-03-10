// VisYUVAPixels.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved



template<class TNumYA, class TNumUV>
inline CVisYUVA<TNumYA, TNumUV>::CVisYUVA(void)
{
}


template<class TNumYA, class TNumUV>
inline CVisYUVA<TNumYA, TNumUV>::CVisYUVA(TNumYA num)
  : m_numY(num),
	m_numU((TNumUV) 0),
	m_numV((TNumUV) 0)
{
}


template<class TNumYA, class TNumUV>
inline CVisYUVA<TNumYA, TNumUV>::CVisYUVA(TNumYA numY, TNumUV numU,
		 TNumUV numV)
  : m_numY(numY),
	m_numU(numU),
	m_numV(numV)
{
}


template<class TNumYA, class TNumUV>
inline CVisYUVA<TNumYA, TNumUV>::CVisYUVA(TNumYA numY, TNumUV numU,
		TNumUV numV, TNumYA numA)
  : m_numY(numY),
	m_numU(numU),
	m_numV(numV),
	m_numA(numA)
{
}


inline CVisYUVA<BYTE, signed char>::CVisYUVA(
		const CVisYUVA<BYTE, signed char>& refrgba)
{
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisYUVA<BYTE, signed char>) == sizeof(int));
	*((int *) this) = *((int *) &refrgba);
}

inline CVisYUVA<unsigned short, short>::CVisYUVA(
		const CVisYUVA<unsigned short, short>& refrgba)
{
	// LATER:  Move this assert to a static validity-checking function?
	assert(sizeof(CVisYUVA<unsigned short, short>) == sizeof(double));
	*((double *) this) = *((double *) &refrgba);
}

template<class TNumYA, class TNumUV>
inline CVisYUVA<TNumYA, TNumUV>::CVisYUVA(
		 const CVisYUVA<TNumYA, TNumUV>& refyuva)
  : m_numY(refyuva.m_numY),
	m_numU(refyuva.m_numU),
	m_numV(refyuva.m_numV),
	m_numA(refyuva.m_numA)
{
}


template<class TNumYA, class TNumUV>
inline CVisYUVA<TNumYA, TNumUV>& CVisYUVA<TNumYA, TNumUV>::operator=(
		TNumYA num)
{
	m_numY = num;
	m_numU = (TNumUV) 0;
	m_numV = (TNumUV) 0;

	return *this;
}


template<class TNumYA, class TNumUV>
inline CVisYUVA<TNumYA, TNumUV>& CVisYUVA<TNumYA, TNumUV>::operator=(
		const CVisYUVA<TNumYA, TNumUV>& refyuva)
{
	m_numY = refyuva.m_numY;
	m_numU = refyuva.m_numU;
	m_numV = refyuva.m_numV;
	m_numA = refyuva.m_numA;

	return *this;
}


template<class TNumYA, class TNumUV>
inline bool CVisYUVA<TNumYA, TNumUV>::operator==(
		const CVisYUVA<TNumYA, TNumUV>& refyuva) const
{
	// UNDONE (in Rel1):  Should alpha be included in the comparison?
	return ((m_numY == refyuva.m_numY)
			&& (m_numU == refyuva.m_numU)
			&& (m_numV == refyuva.m_numV)
			&& (m_numA == refyuva.m_numA));
}


template<class TNumYA, class TNumUV>
inline bool CVisYUVA<TNumYA, TNumUV>::operator!=(
		const CVisYUVA<TNumYA, TNumUV>& refyuva) const
{
	return !operator==(refyuva);
}


template<class TNumYA, class TNumUV>
inline bool CVisYUVA<TNumYA, TNumUV>::operator<(
		const CVisYUVA<TNumYA, TNumUV>& refyuva) const
{
	return memcmp(this, &refyuva, sizeof(CVisYUVA<TNumYA, TNumUV>));
}


template<class TNumYA, class TNumUV>
inline const TNumYA& CVisYUVA<TNumYA, TNumUV>::Y(void) const
	{ return m_numY; }

template<class TNumYA, class TNumUV>
inline const TNumUV& CVisYUVA<TNumYA, TNumUV>::U(void) const
	{ return m_numU; }

template<class TNumYA, class TNumUV>
inline const TNumUV& CVisYUVA<TNumYA, TNumUV>::V(void) const
	{ return m_numV; }

template<class TNumYA, class TNumUV>
inline const TNumYA& CVisYUVA<TNumYA, TNumUV>::A(void) const
	{ return m_numA; }

template<class TNumYA, class TNumUV>
inline void CVisYUVA<TNumYA, TNumUV>::SetY(const TNumYA& num)
	{ m_numY = num; }

template<class TNumYA, class TNumUV>
inline void CVisYUVA<TNumYA, TNumUV>::SetU(const TNumUV& num)
	{ m_numU = num; }

template<class TNumYA, class TNumUV>
inline void CVisYUVA<TNumYA, TNumUV>::SetV(const TNumUV& num)
	{ m_numV = num; }

template<class TNumYA, class TNumUV>
inline void CVisYUVA<TNumYA, TNumUV>::SetA(const TNumYA& num)
	{ m_numA = num; }

template<class TNumYA, class TNumUV>
inline void CVisYUVA<TNumYA, TNumUV>::SetYUV(TNumYA numY,
	TNumUV numU, TNumUV numV)
{
	m_numY = numY;
	m_numU = numU;
	m_numV = numV;
}

template<class TNumYA, class TNumUV>
inline void CVisYUVA<TNumYA, TNumUV>::SetYUVA(TNumYA numY,
	TNumUV numU, TNumUV numV, TNumYA numA)
{
	m_numY = numY;
	m_numU = numU;
	m_numV = numV;
	m_numA = numA;
}


// UNDONE (in Rel1):  Arithmetic operators


#ifndef VIS_ONLY_DECLARE_PROPLIST

template <class TNumYA, class TNumUV>
inline CVisPropTypeInfoBase *VisPPropTypeInfoForClass(
		const CVisYUVA<TNumYA, TNumUV>& refyuva)
{
	return VisLookupPropTypeInfo(typeid(CVisYUVA<TNumYA, TNumUV>));
}
template <class TNumYA, class TNumUV>
inline CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(
		const CVisYUVA<TNumYA, TNumUV>& refyuva)
{
	return CVisPropTypeInfoWithIOPropList<CVisYUVA<TNumYA, TNumUV> >(0, true);
}

#ifdef LATER
// UNDONE:  Need to add the other CVisYUVA stream helper functions.  (Can't use
// macros, because CVisYUVA takes two template parameters.)
#endif // LATER

#endif // !VIS_ONLY_DECLARE_PROPLIST


template <class TNumYA, class TNumUV>
inline void CVisYUVA<TNumYA, TNumUV>::BuildPropList(CVisPropList& refproplist)
{
#ifndef VIS_ONLY_DECLARE_PROPLIST
	assert(refproplist.IsEmpty());

	refproplist.AddPropReference("Y", m_numY, false);
	refproplist.AddPropReference("U", m_numU, false);
	refproplist.AddPropReference("V", m_numV, false);
	refproplist.AddPropReference("alpha", m_numA, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}


//----------------------------------------------------------------------------
// Old self-describing stream I/O


#ifdef VIS_INCLUDE_SDSTREAM

template <class TNumYA, class TNumUV>
inline CVisSDStream& operator<<(CVisSDStream& s, CVisYUVA<TNumYA, TNumUV>& refyuva)
{
	s.OpenParen();
	s.Num(refyuva.m_numY);
	s.Comma();
	s.Num(refyuva.m_numU);
	s.Comma();
	s.Num(refyuva.m_numV);
	s.Comma();
	s.Num(refyuva.m_numA);
	s.CloseParen();

	return s;
}
#endif