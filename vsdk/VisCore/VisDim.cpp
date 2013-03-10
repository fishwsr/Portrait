// VisDim.cpp
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#ifdef VIS_CORE_BUILD
	#include "VisCorePch.h"
	#include "VisCoreProj.h"
#else
	#include "VisPropListProj.h"
#endif

CVisDimIndex::CVisDimIndex(int i0, int i1, int i2, int i3)
{
	m_rgi[0] = i0;
	m_rgi[1] = i1;
	m_rgi[2] = i2;
	m_rgi[3] = i3;
}

CVisDimIndex::CVisDimIndex(const CVisDimIndex& refdimindex)
{
	m_rgi[0] = refdimindex.m_rgi[0];
	m_rgi[1] = refdimindex.m_rgi[1];
	m_rgi[2] = refdimindex.m_rgi[2];
	m_rgi[3] = refdimindex.m_rgi[3];
}

CVisDimIndex& CVisDimIndex::operator=(const CVisDimIndex& refdimindex)
{
	m_rgi[0] = refdimindex.m_rgi[0];
	m_rgi[1] = refdimindex.m_rgi[1];
	m_rgi[2] = refdimindex.m_rgi[2];
	m_rgi[3] = refdimindex.m_rgi[3];

	return *this;
}

bool CVisDimIndex::operator==(const CVisDimIndex& refdimindex) const
{
	return ((m_rgi[0] == refdimindex.m_rgi[0])
			&& (m_rgi[1] == refdimindex.m_rgi[1])
			&& (m_rgi[2] == refdimindex.m_rgi[2])
			&& (m_rgi[3] == refdimindex.m_rgi[3]));
}

#ifdef _DEBUG
bool CVisDimIndex::IsInRange(const CVisDim& refdim) const
{
	assert(&refdim != 0);

	bool fRet = true;
	for (int i = 0; i < refdim.CDim(); ++i)
	{
		if ((m_rgi[i] < 0) || (m_rgi[i] >= refdim[i]))
		{
			fRet = false;
			break;
		}
	}

	return fRet;
}
#endif _DEBUG



CVisDimIterator::CVisDimIterator(const CVisDim& refdim)
  : CVisDimIndex(),
	m_pdim(&refdim),
	m_fAtEnd(false)
{
	m_rgi[0] = m_rgi[1] = m_rgi[2] = m_rgi[3] = 0;
}

CVisDimIterator::CVisDimIterator(const CVisDim& refdim, int i0, int i1,
		int i2, int i3)
  : CVisDimIndex(i0, i1, i2, i3),
	m_pdim(&refdim),
	m_fAtEnd(false)
{
	assert(IsInRange(*m_pdim));
}

CVisDimIterator::CVisDimIterator(const CVisDim& refdim,
		const CVisDimIndex& refdimindex)
  : CVisDimIndex(refdimindex),
	m_pdim(&refdim),
	m_fAtEnd(false)
{
	assert(IsInRange(*m_pdim));
}

CVisDimIterator::CVisDimIterator(const CVisDimIterator& refdimiterator)
  : CVisDimIndex(refdimiterator),
	m_pdim(refdimiterator.m_pdim),
	m_fAtEnd(refdimiterator.m_fAtEnd)
{
}

CVisDimIterator& CVisDimIterator::operator=(const CVisDimIndex& refdimindex)
{
	*((CVisDimIndex *) this) = refdimindex;

	assert(IsInRange(*m_pdim));

	return *this;
}

CVisDimIterator& CVisDimIterator::operator=(const CVisDimIterator& refdimiterator)
{
	*((CVisDimIndex *) this) = (const CVisDimIndex&) refdimiterator;
	m_pdim = refdimiterator.m_pdim;
	m_fAtEnd = refdimiterator.m_fAtEnd;

	return *this;
}

bool CVisDimIterator::operator==(const CVisDimIterator& refdimiterator) const
{
	return ((*m_pdim == *refdimiterator.m_pdim)
			&& (m_fAtEnd == refdimiterator.m_fAtEnd)
			&& (*this == (const CVisDimIndex&) refdimiterator));
}

CVisDimIterator& CVisDimIterator::operator++(void)
{
	if (!m_fAtEnd)
	{
		for (int iDim = m_pdim->CDim() - 1; iDim >= 0; --iDim)
		{
			if (++m_rgi[iDim] < (*m_pdim)[iDim])
				break;
			m_rgi[iDim] = 0;
		}

		if (iDim < 0)
			m_fAtEnd = true;
	}

	return *this;
}


#ifdef LATER
CVisDimIterator& CVisDimIterator::operator--(void)
{
}

CVisDimIterator CVisDimIterator::operator--(int)
{
}
#endif // LATER



CVisDim::CVisDim(int w0, int w1, int w2, int w3)
{
	// Allow dimensions == 0 in case we want to use this to specify indices.
	// LATER:  Should we have an iterator class to specify indices?
	if ((m_rgwDim[0] = w0) <= 0)
		m_cDim = 0;
	else if  ((m_rgwDim[1] = w1) <= 0)
		m_cDim = 1;
	else if  ((m_rgwDim[2] = w2) <= 0)
		m_cDim = 2;
	else if  ((m_rgwDim[3] = w3) <= 0)
		m_cDim = 3;
	else
		m_cDim = 4;

	// CObj is computed only once (in the constructor).
	m_cObj = 1;
	for (int i = 0; i < m_cDim; ++i)
		m_cObj *= m_rgwDim[i];
}

CVisDim::CVisDim(const CVisDim& refdim)
  : m_cDim(refdim.m_cDim),
	m_cObj(refdim.m_cObj)
{
	m_rgwDim[0] = refdim.m_rgwDim[0];
	m_rgwDim[1] = refdim.m_rgwDim[1];
	m_rgwDim[2] = refdim.m_rgwDim[2];
	m_rgwDim[3] = refdim.m_rgwDim[3];
}

CVisDim& CVisDim::operator=(const CVisDim& refdim)
{
	m_cDim = refdim.m_cDim;
	m_cObj = refdim.m_cObj;
	m_rgwDim[0] = refdim.m_rgwDim[0];
	m_rgwDim[1] = refdim.m_rgwDim[1];
	m_rgwDim[2] = refdim.m_rgwDim[2];
	m_rgwDim[3] = refdim.m_rgwDim[3];

	return *this;
}

bool CVisDim::operator==(const CVisDim& refdim) const
{
	bool fRet = (m_cDim == refdim.m_cDim);

	if (fRet)
	{
		// Need to check indices
		for (int iDim = 0; iDim < m_cDim; ++iDim)
		{
			if (m_rgwDim[iDim] != refdim.m_rgwDim[iDim])
			{
				fRet = false;
				break;
			}
		}
	}

	return fRet;
}
