// VisTimer2.cpp
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG


CVisTimer2::CVisTimer2(int nTimers)
  : m_nTimers(nTimers),
	m_iCur(0),
	m_rgliPerformanceCounter(new LARGE_INTEGER[nTimers])
{
	assert(nTimers > 0);

	m_liPerformanceFrequency.QuadPart = 0;
	QueryPerformanceFrequency(&m_liPerformanceFrequency);

	// Assume that there is a performance counter.
	assert(m_liPerformanceFrequency.QuadPart != 0);

	Reset();
}

CVisTimer2::~CVisTimer2(void)
{
	assert(m_rgliPerformanceCounter != 0);
	delete[] m_rgliPerformanceCounter;
}

unsigned int CVisTimer2::CmsElapsed(int i) const
{
	LARGE_INTEGER liT;
	QueryPerformanceCounter(&liT);
	return CmsElapsed(i, liT);
}

unsigned int CVisTimer2::CmsElapsed(void)
{
	LARGE_INTEGER liT;
	QueryPerformanceCounter(&liT);
	unsigned int cmsRet = CmsElapsed(ICur(), liT);

	IncICur();

	return cmsRet;
}

unsigned int CVisTimer2::CmsUpdate(int i)
{
	LARGE_INTEGER liT;
	QueryPerformanceCounter(&liT);
	unsigned int cmsRet = CmsElapsed(i, liT);

	if (i == eiCurrentTimer)
	{
		m_rgliPerformanceCounter[ICur()].QuadPart = liT.QuadPart;
		IncICur();
	}
	else
	{
		m_rgliPerformanceCounter[i].QuadPart = liT.QuadPart;
	}

	return cmsRet;
}

double CVisTimer2::RateUpdate(void)
{
	// Note: the following avoid rounding errors that we could get by
	// returning (((float) NTimers()) / CmsUpdate()).

	LARGE_INTEGER liPerformanceCounter;
	QueryPerformanceCounter(&liPerformanceCounter);

	int nTimers;
	double dblPerformanceCounterDifference;

	if (m_cSpecialRateUpdateAfterReset == 0)
	{
		nTimers = NTimers();
		dblPerformanceCounterDifference
				= ((double) (liPerformanceCounter.QuadPart
						- m_rgliPerformanceCounter[ICur()].QuadPart));
	}
	else if (m_cSpecialRateUpdateAfterReset == NTimers())
	{
		// Very special case:  This is the first call to RateUpdate()
		// since Reset() was called.

		// These values will cause this method to return a rate == - 1.0.
		nTimers = 1;
		dblPerformanceCounterDifference
				= - ((double) m_liPerformanceFrequency.QuadPart);

		-- m_cSpecialRateUpdateAfterReset;
	}
	else
	{
		// Special case:  RateUpdate() has been called fewer than
		// NTimers() times since the last call to Reset().
		nTimers = NTimers() - m_cSpecialRateUpdateAfterReset;
		int iTimerFirst = ICur() - nTimers;
		if (iTimerFirst < 0)
			iTimerFirst += NTimers();

		dblPerformanceCounterDifference
				= ((double) (liPerformanceCounter.QuadPart
						- m_rgliPerformanceCounter[iTimerFirst].QuadPart));

		-- m_cSpecialRateUpdateAfterReset;
	}

	double dblRate = ((double) nTimers)
			* ((double) m_liPerformanceFrequency.QuadPart)
			/ dblPerformanceCounterDifference;

	m_rgliPerformanceCounter[ICur()].QuadPart
			= liPerformanceCounter.QuadPart;
	IncICur();

	return dblRate;
}

void CVisTimer2::Reset(void)
{
	assert(NTimers() > 0);

	LARGE_INTEGER liT;
	QueryPerformanceCounter(&liT);

	for (int i = 0; i < NTimers(); ++i)
	{
		m_rgliPerformanceCounter[i].QuadPart = liT.QuadPart;
	}

	m_cSpecialRateUpdateAfterReset = NTimers();
}


unsigned int CVisTimer2::CmsElapsed(int i,
		const LARGE_INTEGER& refliPerformanceCounter) const
{
	if (i == eiCurrentTimer)
		i = m_iCur;
	assert((i >= 0) && (i < NTimers()));

	LARGE_INTEGER liT;
	liT.QuadPart = refliPerformanceCounter.QuadPart
			- m_rgliPerformanceCounter[i].QuadPart;
	liT.QuadPart *= 1000;
	liT.QuadPart += (m_liPerformanceFrequency.QuadPart / 2);
	liT.QuadPart /= m_liPerformanceFrequency.QuadPart;

	assert(liT.HighPart == 0);
	return liT.LowPart;
}
