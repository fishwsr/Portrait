// VisHistogram.inl
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


inline int CVisHistogram::IFirst(void) const
{
	return m_iFirst;
}

inline int CVisHistogram::ILim(void) const
{
	return m_iLim;
}

inline int CVisHistogram::DiRange(void) const
{
	return m_iLim - m_iFirst;
}

inline bool CVisHistogram::IsLocked(void) const
{
	return m_fLocked;
}

inline void CVisHistogram::Unlock(void)
{
	m_fLocked = false;
}

inline unsigned long CVisHistogram::operator[](int i) const
{
	assert((i >= 0) && (i < 256));
	return m_rgul[i];
}

inline void CVisHistogram::Inc(int i)
{
	assert(!m_fLocked);
	assert((i >= m_iFirst) && (i < m_iLim));
	++ m_rgul[i];
}

inline void CVisHistogram::Inc(int i, unsigned long n)
{
	assert(!m_fLocked);
	assert((i >= m_iFirst) && (i < m_iLim));
	m_rgul[i] += n;
}

inline void CVisHistogram::Dec(int i)
{
	assert(!m_fLocked);
	assert((i >= m_iFirst) && (i < m_iLim));
	assert(m_rgul[i] > 0);
	-- m_rgul[i];
}

inline void CVisHistogram::Dec(int i, unsigned long n)
{
	assert(!m_fLocked);
	assert((i >= m_iFirst) && (i < m_iLim));
	assert(m_rgul[i] >= n);
	m_rgul[i] -= n;
}

inline void CVisHistogram::Set(int i, unsigned long n)
{
	assert(!m_fLocked);
	assert((i >= m_iFirst) && (i < m_iLim));
	m_rgul[i] = n;
}

inline unsigned long CVisHistogram::Low(void) const
{
	assert(m_fLocked);
	return m_ulLow;
}

inline unsigned long CVisHistogram::High(void) const
{
	assert(m_fLocked);
	return m_ulHigh;
}

inline unsigned int CVisHistogram::Sum0(void) const
{
	assert(m_fLocked);
	return m_ulSum0;
}

inline unsigned __int64 CVisHistogram::Sum1(void) const
{
	assert(m_fLocked && m_fValidSum1);
	return m_ullSum1;
}

inline unsigned __int64 CVisHistogram::Sum2(void) const
{
	assert(m_fLocked && m_fValidSum2);
	return m_ullSum2;
}

inline int CVisHistogram::IEffectiveFirst(void) const
{
	assert(m_fLocked);
	return m_iEffectiveFirst;
}

inline int CVisHistogram::IEffectiveLim(void) const
{
	assert(m_fLocked);
	return m_iEffectiveLim;
}

inline int CVisHistogram::DiEffectiveRange(void) const
{
	assert(m_fLocked);
	return m_iEffectiveLim - m_iEffectiveFirst;
}
