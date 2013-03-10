// VisHistogram.h
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <math.h>


class CVisHistogram
{
public:
    //------------------------------------------------------------------
	// Constructor and destructor

	CVisHistogram(int iFirst = 0, int iLim = 256);

	// Copy constructor and assignment operator.
	// The current implementation allocates a new buffer and
	// copies the histogram data.
	// LATER:  We might want to use reference counted memory in
	// this class.
	CVisHistogram(const CVisHistogram& refhistogram);
	CVisHistogram& operator=(const CVisHistogram& refhistogram);

	~CVisHistogram(void);


    //------------------------------------------------------------------
	// Range

	int IFirst(void) const;
	int ILim(void) const;
	int DiRange(void) const;


    //------------------------------------------------------------------
	// Locking and Clearing

	// When the histogram is locked, statictics are calculated and the
	// histogram should not be modified until it is unlocked.
	// (A lock count is not used with CVisHistogram.)
	
	bool IsLocked(void) const;

	// Use 0 or 1 for iSumHigh if higher-order sums are not needed.
	// (They are needed for the threashold methods, below.)
	void Lock(int iSumHigh = 2);
	void Unlock(void);

	// Clear will unlock the histogram and clear its entries.
	void Clear(void);


    //------------------------------------------------------------------
	// Entries

	unsigned long operator[](int i) const;

	// The histogram should be unlocked before it is modified by the
	// methods below.
	
	void Inc(int i);
	void Inc(int i, unsigned long n);

	void Dec(int i);
	void Dec(int i, unsigned long n);

	void Set(int i, unsigned long n);


    //------------------------------------------------------------------
	// Statistics

	unsigned long Low(void) const;
	unsigned long High(void) const;

	// Sum1 and Sum2 may not be valid if they were not calculated when
	// the histogram was locked.
	unsigned int Sum0(void) const;
	unsigned __int64 Sum1(void) const;
	unsigned __int64 Sum2(void) const;

	// The entries outside the effective range are all zero.
	int IEffectiveFirst(void) const;
	int IEffectiveLim(void) const;
	int DiEffectiveRange(void) const;


    //------------------------------------------------------------------
	// Thresholding

	// These results are computed and returned, but they are not stored
	// in the histogram.  You should save results if you need to reuse
	// them.

	int IThresholdOtsu(void) const;
	// UNDONE:  Some results seem strange.  Not sure if the Kullback
	// threshold is implemented correctly.
	int IThresholdKullback(void) const;


protected:
	double DblVarianceOtsu(unsigned long ulPartialSum0,
			unsigned __int64 ullPartialSum1) const;


private:
	int m_iFirst;
	int m_iLim;
	int m_iEffectiveFirst;
	int m_iEffectiveLim;
	unsigned long *m_rgul;
	unsigned long m_ulLow;
	unsigned long m_ulHigh;
	unsigned long m_ulSum0;
	unsigned __int64 m_ullSum0Squared;
	unsigned __int64 m_ullSum1;
	unsigned __int64 m_ullSum2;
	bool m_fLocked;  // LATER:  Do we want a lock count?
	bool m_fValidSum1;
	bool m_fValidSum2;
	bool m_fReserved;
};


void VisHistogramFromImage(CVisHistogram& refhistogram, const CVisByteImage& refimage);

// LATER:  Add code to display a histogram in a RECT in an HDC?

#ifdef SAVE
CVisPane VisDisplayHistogram(CVisHistogram& refhistogram,
		bool fUseEffectiveRange = false);
#endif // SAVE


#include "VisHistogram.inl"
