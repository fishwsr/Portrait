// Histogram.h: interface for the CHistogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTOGRAM_H__BF33260A_7D56_4F3A_A8B6_CC52F337F023__INCLUDED_)
#define AFX_HISTOGRAM_H__BF33260A_7D56_4F3A_A8B6_CC52F337F023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSBASE_API CHistogram1D
{
public:
	CHistogram1D();
	CHistogram1D(const CHistogram1D &refHistogram);
	CHistogram1D(int nBins, double lfLower=0., double lfUpper=1., bool fUseFalloutSample=true);
	virtual ~CHistogram1D();
	virtual void Clear(void);

	// initialization
	bool IsLocked(void) const;
	virtual void Lock();
	void Unlock(void);
	bool UseFalloutSample(void) const;
	void EnableUseFalloutSample(bool fb=true);

	// index
	int NBins() const;	// NBins()=m_iBinNum-2, user can access the real bins from 1 to NBins()
	unsigned long operator[](int i) const;
	double DRange() const;
	int Index(double lfVal) const;
	int IEffectiveFirst() const;
	int IEffectiveLast() const;
	int NEffectiveBins() const;
	
	// bin operations
	// these methods ONLY perform when the histogram is UNLOCKED
	// [1 ~ NBins()-1] are for valid samples
	// [0] and [NBins()] are for fallout samples
	void Inc(int i);
	void Inc(int i, unsigned long n);
	void Dec(int i);
	void Dec(int i, unsigned long n);
	void Set(int i, unsigned long n);

	// statistics
	// these methods ONLY perform when the histogram is LOCKED
	unsigned long Sum0() const;
	double Sum1() const;
	double Sum2() const;
	unsigned long Max() const;
	unsigned long Min() const;
	double UpperBound() const;
	double LowerBound() const;
	double Step() const;
	double Mean() const;
	double Var() const;
	double Freq(int i) const;
	double Entropy() const;
	double DistL1(const CHistogram1D &refHistogram) const;
	double DistL2(const CHistogram1D &refHistogram) const;
	double DistKL(const CHistogram1D &refHistogram) const;
	double Sample1() const;
	std::vector<double> SampleN(int n) const;
	void Uniform();

	// histogram operations
	// these methods perform both LOCKED and UNLOCKED
	const CHistogram1D& operator=(const CHistogram1D &refHistogram);
	const CHistogram1D& operator+=(const CHistogram1D &refHistogram);
	const CHistogram1D& operator-=(const CHistogram1D &refHistogram);
	CHistogram1D operator-(const CHistogram1D &refHistogram) const;
	CHistogram1D operator+(const CHistogram1D &refHistogram) const;

	int iFirstBin() const;
	int iLastBin() const;

protected:

	int m_iBinNum;				// number of all bins
	unsigned long *m_ulBins;	// bins, [1~m_iBinNum-1] are for valid samples
								// [0] and [m_iBinNum] are for fallout samples
	unsigned long m_ulMin;		// fewest count of bins
	unsigned long m_ulMax;		// most count of bins
	unsigned long m_ulSum0;     // sum of bin counts
	double m_lfSum1;			// sum of (bin_count * bin_value) : SUM(m_ulBins[i] * m_lfLoBound*i+0.5*m_lfStep);
	double m_lfSum2;			// sum of (bin_count * bin_value*bin_value) : SUM(m_ulBins[i] * square(m_lfLoBound*i+0.5*m_lfStep));
	
	bool m_fLocked;				// indicate the statistics are valid and refuse adding to or subtracting from bins
	bool m_fUseFalloutSample;	// indicate how to handle the fallen out samples

	int m_iEffectiveFirst;		// first bin which is not empty
	int m_iEffectiveLast;		// last bin which is not empty

	double m_lfLoBound; // lower bound of the bin values
	double m_lfUpBound; // upper bound of the bin values
	double m_lfStep;    // stepwise from lower bound to upper bound
};

CSBASE_API void HistogramToVector(const CHistogram1D& hist, CVisDVector& vec);
CSBASE_API void HistogramToDensity(const CHistogram1D& hist, CVisDVector& vec);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CHistogram1D& hist);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CHistogram1D& hist);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const std::vector<CHistogram1D*>& vpHist);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, std::vector<CHistogram1D*>& vpHist);

#endif // !defined(AFX_HISTOGRAM_H__BF33260A_7D56_4F3A_A8B6_CC52F337F023__INCLUDED_)

