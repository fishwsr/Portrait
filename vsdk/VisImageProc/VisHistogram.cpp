// VisHistogram.cpp
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "VisImageProcPch.h"
#include "VisImageProc.h"

#include "VisHistogram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CVisHistogram::CVisHistogram(int iFirst, int iLim)
  : m_iFirst(iFirst),
	m_iLim(iLim),
	m_rgul((new unsigned long[iLim - iFirst]) - iFirst)
//	, m_iEffectiveFirst(iLim),
//	m_iEffectiveLim(iLim),
//	m_ulLow(0),
//	m_ulHigh(0),
//	m_ulSum0(0),
//	m_ullSum0Squared(0),
//	m_ullSum1(0),
//	m_ullSum2(0),
//	m_fLocked(false),
//	m_fValidSum1(false),
//	m_fValidSum2(false)
{
	assert(iFirst < iLim);

	Clear();
}

CVisHistogram::CVisHistogram(const CVisHistogram& refhistogram)
  : m_iFirst(refhistogram.m_iFirst),
	m_iLim(refhistogram.m_iLim),
	m_rgul((new unsigned long[m_iLim - m_iFirst]) - m_iFirst),
	m_iEffectiveFirst(refhistogram.m_iEffectiveFirst),
	m_iEffectiveLim(refhistogram.m_iEffectiveLim),
	m_ulLow(refhistogram.m_ulLow),
	m_ulHigh(refhistogram.m_ulHigh),
	m_ulSum0(refhistogram.m_ulSum0),
	m_ullSum0Squared(refhistogram.m_ullSum0Squared),
	m_ullSum1(refhistogram.m_ullSum1),
	m_ullSum2(refhistogram.m_ullSum2),
	m_fLocked(refhistogram.m_fLocked),
	m_fValidSum1(refhistogram.m_fValidSum1),
	m_fValidSum2(refhistogram.m_fValidSum2)
{
	CopyMemory((m_rgul + m_iFirst), (refhistogram.m_rgul + m_iFirst),
			sizeof(unsigned long) * (m_iLim - m_iFirst));
}

CVisHistogram& CVisHistogram::operator=(const CVisHistogram& refhistogram)
{
	if (DiRange() != refhistogram.DiRange())
	{
		delete [] (m_rgul + m_iFirst);
		m_rgul = new unsigned long[refhistogram.DiRange()]
				- refhistogram.m_iFirst;
	}

	m_iFirst = refhistogram.m_iFirst;
	m_iLim = refhistogram.m_iLim;
	m_iEffectiveFirst = refhistogram.m_iEffectiveFirst;
	m_iEffectiveLim = refhistogram.m_iEffectiveLim;
	m_ulLow = refhistogram.m_ulLow;
	m_ulHigh = refhistogram.m_ulHigh;
	m_ulSum0 = refhistogram.m_ulSum0;
	m_ullSum0Squared = refhistogram.m_ullSum0Squared;
	m_ullSum1 = refhistogram.m_ullSum1;
	m_ullSum2 = refhistogram.m_ullSum2;
	m_fLocked = refhistogram.m_fLocked;
	m_fValidSum1 = refhistogram.m_fValidSum1;
	m_fValidSum2 = refhistogram.m_fValidSum2;

	CopyMemory((m_rgul + m_iFirst), (refhistogram.m_rgul + m_iFirst),
			sizeof(unsigned long) * (m_iLim - m_iFirst));

	return *this;
}

CVisHistogram::~CVisHistogram(void)
{
	delete [] (m_rgul + m_iFirst);
}


void CVisHistogram::Lock(int iSumHigh)
{
	// UNDONE:  Combine this code into one loop.  Check m_fLocked and valid sums.

	// void FindEffectiveRange(void)
	m_iEffectiveFirst = m_iFirst;
	while ((m_iEffectiveFirst < m_iLim) && (m_rgul[m_iEffectiveFirst] == 0))
		++ m_iEffectiveFirst;

	// We assert that there are entries in the histogram.
	assert(m_iEffectiveFirst < m_iLim);

	m_iEffectiveLim = m_iEffectiveFirst;
	m_ulHigh = m_rgul[m_iEffectiveFirst];
	m_ulLow = m_rgul[m_iFirst];
	for (int i = m_iEffectiveFirst + 1; i < m_iLim; ++i)
	{
		if (m_rgul[i] != 0)
			m_iEffectiveLim = i;

		if (m_ulHigh < m_rgul[i])
			m_ulHigh = m_rgul[i];
		else if (m_ulLow < m_rgul[i])
			m_ulLow = m_rgul[i];
	}

	// We want m_iEffectiveLim to be one past the last nonzero value.
	++ m_iEffectiveLim;


	// void FindSums(int iSumHigh = 2)
	assert((iSumHigh >= 0) && (iSumHigh <= 2));

	m_ulSum0 = 0;
	m_ullSum1 = 0;
	m_ullSum2 = 0;

	if (iSumHigh < 1)
	{
		for (int i = m_iEffectiveFirst; i < m_iEffectiveLim; ++i)
			m_ulSum0 += m_rgul[i];
		m_fValidSum1 = false;
		m_fValidSum2 = false;
	}
	else if (iSumHigh == 1)
	{
		assert(sizeof(long) == 4);
		for (int i = m_iEffectiveFirst; i < m_iEffectiveLim; ++i)
		{
			m_ulSum0 += m_rgul[i];
			m_ullSum1 += UInt32x32To64(m_rgul[i], i);
		}
		m_fValidSum1 = true;
		m_fValidSum2 = false;
	}
	else if (iSumHigh > 1)
	{
		assert(sizeof(long) == 4);
		for (int i = m_iEffectiveFirst; i < m_iEffectiveLim; ++i)
		{
			m_ulSum0 += m_rgul[i];

			__int64 liT = UInt32x32To64(m_rgul[i], i);

			m_ullSum1 += liT;

			liT *= i;
			m_ullSum2 += liT;
		}
		m_fValidSum1 = true;
		m_fValidSum2 = true;
	}

	m_ullSum0Squared = UInt32x32To64(m_ulSum0, m_ulSum0);
	m_fLocked = true;
}


void CVisHistogram::Clear(void)
{
	Unlock();
	for (int i = m_iFirst; i < m_iLim; ++i)
		m_rgul[i] = 0;
}



int CVisHistogram::IThresholdOtsu(void) const
{
	// We assume that there are entries in the histogram and that the
	// effective range of values has already been found.
	assert(m_iEffectiveFirst < m_iLim);
	
	int iMinThreshold = m_iEffectiveFirst;
	unsigned long ulPartialSum0 = m_rgul[m_iEffectiveFirst];
	unsigned __int64 ullPartialSum1 = UInt32x32To64(m_rgul[m_iEffectiveFirst],
			m_iEffectiveFirst);
	double dblMaxVaraince = DBL_MIN;

	int i = m_iEffectiveFirst + 1;
	while ((i < m_iEffectiveLim - 1) && (m_rgul[i] == 0))
		++ i;  // Skip ahead to the next nonzero entry.

	// We should only check threshold values that separate the histogram
	// into parts that each contain at least 5% of the pixel values.
	// UNDONE:  Is this hack needed?
	unsigned long ulPartialSumLow = m_ulSum0 / 5;
	unsigned long ulPartialSumHigh = m_ulSum0 - ulPartialSumLow;

	for ( ; i < m_iEffectiveLim - 1; ++i)
	{
		ulPartialSum0 += m_rgul[i];

		if (ulPartialSum0 > ulPartialSumHigh)
			break;

		ullPartialSum1 += UInt32x32To64(m_rgul[i], i);

		if (ulPartialSum0 > ulPartialSumLow)
		{
			double dblVaraince = DblVarianceOtsu(ulPartialSum0, ullPartialSum1);

			if (dblVaraince > dblMaxVaraince)
			{
				dblMaxVaraince = dblVaraince;
				iMinThreshold = i;
			}
		}
	}

// Note:  This could happen if all pixels have the same value.
//	assert(dblMaxVaraince != DBL_MIN);

	return iMinThreshold;
}


int CVisHistogram::IThresholdKullback(void) const
{
	// UNDONE:  Some results seem strange.  Not sure if this is
	// implemented correctly.

	// We assume that there are entries in the histogram and that the
	// effective range of values has already been found.
	assert(m_iEffectiveFirst < m_iLim);

	if (m_iEffectiveFirst == m_iEffectiveLim - 1)
		return m_iEffectiveFirst;
	
	int iMinThreshold = m_iEffectiveFirst;
	unsigned long ulPartialSum0 = m_rgul[m_iEffectiveFirst];
	double dblMinMeasure = DBL_MAX;

	int i = m_iEffectiveFirst + 1;
	while ((i < m_iEffectiveLim - 1) && (m_rgul[i] == 0))
		++ i;  // Skip ahead to the next nonzero entry.

	// UNDONE:  Get rid of extra variables
	double dblMean = ((double) (__int64) m_ullSum1) / ((double) m_ulSum0);

//	double dblProbabilityCur;

	int iNext;
	double dblINext;
	double dblProbabilityNext;

	double dblT1 = ((double) ulPartialSum0) * ((double) m_iEffectiveFirst);

	double dblFraction1Cur = ((double) ulPartialSum0) / ((double) m_ulSum0);
	double dblMean1Cur = dblT1 / ((double) m_ulSum0);
	double dblVariance1Cur = 0;

	double dblFraction1Next;
	double dblMean1Next;
	double dblVariance1Next;

	double dblFraction2Cur = 1.0 - dblFraction1Cur;
	double dblMean2Cur = (dblMean - dblFraction1Cur * dblMean1Cur)
				/ dblFraction2Cur;
	double dblT2 = ((double) (__int64) m_ullSum2) - dblT1 * ((double) m_iEffectiveFirst);
	double dblT3 = (((double) (__int64) m_ullSum1) - dblT1) /
			(((double) m_ulSum0) - ((double) ulPartialSum0));
	double dblVariance2Cur = dblT2 / (((double) m_ulSum0) - ((double) ulPartialSum0))
			- dblT3 * dblT3;

	double dblFraction2Next;
	double dblMean2Next;
	double dblVariance2Next;

	// These temporary variables are used to find the variances.
	double dblMeanDifference1;
	double dblSquaredMeanDifference1;
	double dblMeanOffset1;
	double dblSquaredMeanOffset1;

	double dblMeanDifference2;
	double dblSquaredMeanDifference2;
	double dblMeanOffset2;
	double dblSquaredMeanOffset2;

	// We should only check threshold values that separate the histogram
	// into parts that each contain at least 5% of the pixel values.
	// UNDONE:  Is this hack needed?  Without it, we often choose the
	// first (or second) non-zero index as the threshold.  Should we do
	// something else here instead?
	unsigned long ulPartialSumLow = m_ulSum0 / 20;
//	unsigned long ulPartialSumLow = 0;
//	unsigned long ulPartialSumLow = 1;
	unsigned long ulPartialSumHigh = m_ulSum0 - ulPartialSumLow;

	for ( ; i < m_iEffectiveLim - 1; ++i)
	{
		ulPartialSum0 += m_rgul[i];

		if (ulPartialSum0 > ulPartialSumHigh)
			break;

		// UNDONE:  We should find the next values at the top of the loop.

		// UNDONE:  Clean up these calculations

		iNext = i + 1;
		dblINext = (double) iNext;
		dblProbabilityNext = ((double) m_rgul[iNext]) / ((double) m_ulSum0);

		dblFraction1Next = dblFraction1Cur + dblProbabilityNext;
		dblFraction2Next = dblFraction2Cur - dblProbabilityNext;

		// Assume that we aren't doing these calculations for degenerate cases.
		assert(dblFraction1Next != 0.0);
		assert(dblFraction2Next != 0.0);



		if (ulPartialSum0 > ulPartialSumLow)
		{
			double dblLogFraction1 = log(dblFraction1Cur);
			double dblLogVariance1 = log(dblVariance1Cur);
			double dblLogFraction2 = log(dblFraction2Cur);
			double dblLogVariance2 = log(dblVariance2Cur);

			double dblMeasure = dblFraction1Cur * (dblLogVariance1
							- dblLogFraction1 - dblLogFraction1)
					+ dblFraction2Cur * (dblLogVariance2
							- dblLogFraction2 - dblLogFraction2);

			if (dblMeasure < dblMinMeasure)
			{
				dblMinMeasure = dblMeasure;
				iMinThreshold = i;
			}
		}


		dblMean1Next = (dblFraction1Cur * dblMean1Cur
						+ dblINext * dblProbabilityNext)
				/ dblFraction1Next;
		dblMean2Next = (dblMean - dblFraction1Next * dblMean1Next)
				/ dblFraction2Next;

		dblMeanDifference1 = dblMean1Cur - dblMean1Next;
		dblSquaredMeanDifference1 = dblMeanDifference1 * dblMeanDifference1;
		dblMeanOffset1 = dblINext - dblMean1Next;
		dblSquaredMeanOffset1 = dblMeanOffset1 * dblMeanOffset1;

		dblVariance1Next =
				(dblFraction1Cur * (dblVariance1Cur + dblSquaredMeanDifference1)
						+ dblProbabilityNext * dblSquaredMeanOffset1)
				/ dblFraction1Next;

		dblMeanDifference2 = dblMean2Cur - dblMean2Next;
		dblSquaredMeanDifference2 = dblMeanDifference2 * dblMeanDifference2;
		dblMeanOffset2 = dblINext - dblMean2Next;
		dblSquaredMeanOffset2 = dblMeanOffset2 * dblMeanOffset2;

		dblVariance2Next =
				(dblFraction2Cur * (dblVariance2Cur + dblSquaredMeanDifference2)
						- dblProbabilityNext * dblSquaredMeanOffset2)
				/ dblFraction2Next;

		dblFraction1Cur = dblFraction1Next;
		dblMean1Cur = dblMean1Next;
		dblVariance1Cur = dblVariance1Next;

		dblFraction2Cur = dblFraction2Next;
		dblMean2Cur = dblMean2Next;
		dblVariance2Cur = dblVariance2Next;
	}

// Note:  This could happen if all pixels have the same value.
//	assert(dblMinMeasure != DBL_MAX);

	return iMinThreshold;
}


double CVisHistogram::DblVarianceOtsu(unsigned long ulPartialSum0,
		unsigned __int64 ullPartialSum1) const
{
	double dblRet;
	
	if ((ulPartialSum0 != 0) && (ulPartialSum0 != m_ulSum0))
	{
		__int64 llT = (__int64)
				(m_ulSum0 * ullPartialSum1 - m_ullSum1 * ulPartialSum0);
		double dblT = ((double) llT)
				/ ((double) (__int64) UInt32x32To64(ulPartialSum0,
						m_ulSum0 - ulPartialSum0));
		dblRet = dblT * (((double) llT) / ((double) (__int64) m_ullSum0Squared));
	}
	else
	{
		dblRet = DBL_MIN;
	}

	return dblRet;
}



void VisHistogramFromImage(CVisHistogram& refhistogram, const CVisByteImage& refimage)
{
	for (int y = refimage.Top(); y < refimage.Bottom(); ++y)
	{
		const BYTE *rgbRow = refimage.RowPointer(y);
		for (int x = refimage.Left(); x < refimage.Right(); ++x)
			refhistogram.Inc(rgbRow[x]);
	}

	// Do we want to lock it here?
	refhistogram.Lock();
}



#ifdef SAVE
// UNDONE:  Don't require VisDisplay to use histograms.
// (Remove this when no longer needed.)
CVisPane VisDisplayHistogram(CVisHistogram& refhistogram,
		bool fUseEffectiveRange)
{
	int iFirst;
	int iLim;

	if (fUseEffectiveRange)
	{
		iFirst = refhistogram.IEffectiveFirst();
		iLim = refhistogram.IEffectiveLim();
	}
	else
	{
		iFirst = refhistogram.IFirst();
		iLim = refhistogram.ILim();
	}

	int dx = iLim - iFirst;
	int dy = 256;

	CString strTitle;
	strTitle.Format("Histogram from %d to %d", iFirst, iLim);

	CVisPane pane(dx, dy, strTitle);

	int yHigh = dy - 1;
	int dxRounding = refhistogram.High() - 1;
	for (int i = iFirst; i < iLim; ++i)
	{
		pane.AddLine(i - iFirst, yHigh, i - iFirst,
				yHigh - (yHigh * refhistogram[i] + dxRounding)
						/ refhistogram.High());
	}

	return pane;
}
#endif // SAVE
