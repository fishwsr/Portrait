// VisDim.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


inline int CVisDimIndex::operator[](int i) const
{
//	assert(m_pdim->CDim() > i);
	assert(i >= 0);
	return m_rgi[i];
}

inline bool CVisDimIndex::operator!=(const CVisDimIndex& refdimindex) const
{
	return !(*this == refdimindex);
}


inline bool CVisDimIterator::operator==(const CVisDimIndex& refdimindex) const
{
	return (*((const CVisDimIndex *) this) == (const CVisDimIndex&) refdimindex);
}

inline bool CVisDimIterator::operator!=(const CVisDimIndex& refdimindex) const
{
	return !(*this == refdimindex);
}

inline bool CVisDimIterator::operator!=(const CVisDimIterator& refdimiterator) const
{
	return !(*this == refdimiterator);
}

inline CVisDimIterator CVisDimIterator::operator++(int)
{
	CVisDimIterator dimiteratorRet(*this);
	++*this;
	return dimiteratorRet;
}


inline const CVisDim& CVisDimIterator::Dim(void) const
{
	return *m_pdim;
}


inline void CVisDimIterator::SetToEnd(void)
{
	m_fAtEnd = true;
}


inline bool CVisDim::operator!=(const CVisDim& refdim) const
{
	return !(*this == refdim);
}

inline int CVisDim::operator[](int i) const
{
	assert((i >= 0) && (i < m_cDim));
	return m_rgwDim[i];
}

inline int CVisDim::CDim(void) const
{
	return m_cDim;
}

inline int CVisDim::CObj(void) const
{
	return m_cObj;
}

inline CVisDimIterator CVisDim::begin(void) const
{
	return CVisDimIterator(*this);
}

inline CVisDimIterator CVisDim::end(void) const
{
	CVisDimIterator dimiteratorRet(*this);
	dimiteratorRet.SetToEnd();
	return dimiteratorRet;
}
