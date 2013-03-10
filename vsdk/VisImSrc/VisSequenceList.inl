// VisSequenceList.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


inline unsigned long CVisSequenceList::AddRef(void)
{
	assert(m_cRef > 0);

	return ++m_cRef;
}

inline unsigned long CVisSequenceList::Release(void)
{
	unsigned long cRef = --m_cRef;

	if (cRef == 0)
		delete this;

	return cRef;
}


inline int CVisSequenceList::NSequences(void) const
{
	return m_vectorSequenceList.size();
}


inline class CVisSequenceBase *CVisSequenceList::PSequenceGetI(int i)
{
	class CVisSequenceBase *psequenceRet;
	
	EnterCriticalSection(&m_criticalsectionSequenceList);

	if ((i >= 0) && (i < (int) m_vectorSequenceList.size()))
		psequenceRet = m_vectorSequenceList[i];
	else
		psequenceRet = 0;

	LeaveCriticalSection(&m_criticalsectionSequenceList);

	return psequenceRet;
}


inline void CVisSequenceList::GrabNextInBackground(void)
{
	m_fWantFrame = true;
	ReleaseSemaphore(m_hSemaphoreGrabber, 1, 0);
}


inline bool CVisSequenceList::AllowVolatileImages(void) const
{
	return m_fAllowVolatileImages;
}


inline void CVisSequenceList::SetAllowVolatileImages(bool f)
{
	m_fAllowVolatileImages = f;
}



inline bool CVisSequenceList::WantContinuousGrab(void) const
{
	return m_fWantContinuousGrab;
}


inline void CVisSequenceList::SetWantContinuousGrab(bool fWantContinuousGrab,
		bool fHaveContinuousGrab)
{
	m_fWantContinuousGrab = fWantContinuousGrab;
	m_fHaveContinuousGrab = fHaveContinuousGrab;
	
	if ((fWantContinuousGrab) && (!fHaveContinuousGrab))
		GrabIfNeeded();
}
