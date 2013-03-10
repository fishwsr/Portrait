// VisTimer2.inl
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved


inline int CVisTimer2::NTimers(void) const
{
	return m_nTimers;
}

inline int CVisTimer2::ICur(void) const
{
	return m_iCur;
}

inline void CVisTimer2::IncICur(void)
{
	if (++m_iCur == m_nTimers)
		m_iCur = 0;
}

inline void CVisTimer2::SetICur(int i)
{
	assert((i >= 0) && (i < NTimers()));
	m_iCur = i;
}

