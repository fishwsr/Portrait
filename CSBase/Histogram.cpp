// Histogram.cpp: implementation of the CHistogram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Histogram.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHistogram1D::CHistogram1D()
: m_iBinNum(3),
  m_ulBins(new unsigned long[3]),
  m_iEffectiveFirst(1),
  m_iEffectiveLast(1),
  m_fLocked(false),
  m_fUseFalloutSample(true),
  m_ulMax(0),
  m_ulMin(0),
  m_ulSum0(0),
  m_lfSum1(0.),
  m_lfSum2(0.),
  m_lfLoBound(0.),
  m_lfUpBound(1.),
  m_lfStep(1.)
{
	Clear();
}

CHistogram1D::CHistogram1D(const CHistogram1D& refHistogram)
{
	if ( this != &refHistogram )
	{
		*this = refHistogram;
	}
}

CHistogram1D::CHistogram1D(int nBins, double lfLower /* =0. */, double lfUpper /* =1. */, bool fUseFalloutSample /* =false */)
: m_iBinNum(max(3, nBins+2)),
  m_ulBins(new unsigned long[max(3, nBins+2)]),
  m_iEffectiveFirst(1),
  m_iEffectiveLast(max(3, nBins+2)-2),
  m_fLocked(false),
  m_fUseFalloutSample(fUseFalloutSample),
  m_ulMax(0),
  m_ulMin(0),
  m_ulSum0(0),
  m_lfSum1(0.),
  m_lfSum2(0.)
{
	assert( lfLower < lfUpper );

	m_lfLoBound = lfLower;
	m_lfUpBound = lfUpper;

	m_lfStep = (m_lfUpBound-m_lfLoBound)/(m_iBinNum-2);

	Clear();
}

CHistogram1D::~CHistogram1D()
{
	if ( NULL != m_ulBins )
	{
		delete[] m_ulBins;
		m_ulBins = NULL;
	}
}

void CHistogram1D::Clear()
{
	Unlock();
	for ( int i=0; i<m_iBinNum; i++ )
	{
		m_ulBins[i] = 0;
	}
}

unsigned long CHistogram1D::operator[](int i) const
{
	assert(i>=0 && i<m_iBinNum);
	
	return m_ulBins[i];
}

int CHistogram1D::NBins() const
{
	return m_iBinNum - 2;
}

double CHistogram1D::UpperBound() const
{
	return m_lfUpBound;
}

double CHistogram1D::LowerBound() const
{
	return m_lfLoBound;
}

double CHistogram1D::Step() const
{
	return m_lfStep;
}

double CHistogram1D::DRange() const
{
	return m_lfUpBound-m_lfLoBound;
}

int CHistogram1D::IEffectiveFirst() const
{
	assert(m_fLocked);

	return m_iEffectiveFirst;
}

int CHistogram1D::IEffectiveLast() const
{
	assert(m_fLocked);

	return m_iEffectiveLast;
}

int CHistogram1D::NEffectiveBins() const
{
	assert(m_fLocked);

	return m_iEffectiveLast-m_iEffectiveFirst+1;
}

int CHistogram1D::iFirstBin() const
{
	if ( m_fUseFalloutSample )
		return 0;
	else
		return 1;
}

int CHistogram1D::iLastBin() const
{
	if ( m_fUseFalloutSample )
		return m_iBinNum-1;
	else
		return m_iBinNum-2;
}

unsigned long CHistogram1D::Max() const
{
	assert(m_fLocked);
	
	return m_ulMax;
}

unsigned long CHistogram1D::Min() const
{
	assert(m_fLocked);

	return m_ulMin;
}

double CHistogram1D::Mean() const
{
	assert(m_fLocked);
	assert(0 != m_ulSum0);

	return m_lfSum1/m_ulSum0;
}

double CHistogram1D::Var() const
{
	assert(m_fLocked);
	assert(0 != m_ulSum0);

	return (m_lfSum2-m_lfSum1*m_lfSum1/m_ulSum0)/m_ulSum0;
}

unsigned long CHistogram1D::Sum0() const
{
	assert(m_fLocked);

	return m_ulSum0;
}

double CHistogram1D::Sum1() const
{
	assert(m_fLocked);

	return m_lfSum1;
}

double CHistogram1D::Sum2() const
{
	assert(m_fLocked);

	return m_lfSum2;
}

double CHistogram1D::Freq(int i) const
{
	assert(m_fLocked);
	assert(0 != m_ulSum0);
	assert(i>=0 && i<m_iBinNum);

	return double(m_ulBins[i])/m_ulSum0;
}

double CHistogram1D::Entropy() const
{
	assert(m_fLocked);
	
	int iLast = iLastBin();
	double sumf=0., fp;
	for ( int i=iFirstBin(); i<=iLast; i++ )
	{
		fp = Freq(i);
		if ( fp > 0.0000001 )
		{
			sumf -= fp*log(fp);
		}
	}

	return sumf;
}

double CHistogram1D::DistKL(const CHistogram1D &refHistogram) const
{
	assert(m_fLocked);
	assert(refHistogram.IsLocked());
	assert(refHistogram.m_iBinNum == m_iBinNum);

	int iLast = iLastBin();
	double sumf=0., fp, fq;
	for ( int i=iFirstBin(); i<=iLast; i++ )
	{
		fp = Freq(i);
		fq = refHistogram.Freq(i);

		if ( fp>0.0000001 )
		{
			fq = max(0.0000001, fq);
			sumf += fp*log(fp/fq);
		}
	}

	return sumf;
}

double CHistogram1D::DistL1(const CHistogram1D &refHistogram) const
{
	assert(m_fLocked);
	assert(refHistogram.IsLocked());
	assert(refHistogram.m_iBinNum == m_iBinNum);

	int iLast = iLastBin();
	double sumf=0.;
	for ( int i=iFirstBin(); i<=iLast; i++ )
	{
		sumf += fabs( Freq(i)-refHistogram.Freq(i) );
	}

	return sumf;
}

double CHistogram1D::DistL2(const CHistogram1D &refHistogram) const
{
	assert(m_fLocked);
	assert(refHistogram.IsLocked());
	assert(refHistogram.m_iBinNum == m_iBinNum);

	int iLast = iLastBin();
	double sumf=0., tempf;
	for ( int i=iFirstBin(); i<=iLast; i++ )
	{
		tempf = Freq(i) - refHistogram.Freq(i);
		sumf += tempf*tempf;
	}

	return sqrt(sumf);
}

void CHistogram1D::Uniform()
{
	Unlock();
	m_ulBins[0] = m_ulBins[m_iBinNum-1] = 0;
	for ( int i=1; i<m_iBinNum-1; i++ )
	{
		m_ulBins[i] = 1;
	}
	Lock();
}

double CHistogram1D::Sample1() const
{
	assert(m_fLocked);
	int iFirst = iFirstBin();
	int iLast = iLastBin();

	int i=iFirst;
	double total=Freq(i); 
	double frand=double(rand())/double(RAND_MAX);

	while ( total<frand && i<iLast )
	{
		++ i;
		total += Freq(i);
	}		

	return m_lfLoBound + (i-0.5)*m_lfStep;
}

std::vector<double> CHistogram1D::SampleN(int n) const
{
	assert(m_fLocked);
	int iFirst = iFirstBin();
	int iLast = iLastBin();

	int i, k;
	double total, frand;

	std::vector<double> vSamples(max(2,n));

	for ( k=0; k<n; k++ )
	{
		i = iFirst;
		total = Freq(i);
		frand = double(rand())/double(RAND_MAX);

		while ( total<frand && i<iLast )
		{
			++ i;
			total += Freq(i);
		}		

		vSamples[k] = m_lfLoBound + (i-0.5)*m_lfStep;
	}

	return vSamples;
}

const CHistogram1D& CHistogram1D::operator=(const CHistogram1D& refHistogram)
{
	if ( this != &refHistogram )
	{
		assert(refHistogram.m_iBinNum>0);
		assert(NULL!=refHistogram.m_ulBins);

		m_iBinNum = refHistogram.m_iBinNum;
		if ( NULL != m_ulBins )
		{
			delete[] m_ulBins;
			m_ulBins = new unsigned long[m_iBinNum];
			memcpy(m_ulBins, refHistogram.m_ulBins, m_iBinNum*sizeof(unsigned long));
//			for ( int i=0; i<m_iBinNum; i++ )
//			{
//				m_ulBins[i] = refHistogram.m_ulBins[i];
//			}

		}

		m_lfLoBound = refHistogram.m_lfLoBound;
		m_lfUpBound = refHistogram.m_lfUpBound;
		m_lfStep = refHistogram.m_lfUpBound;

		m_fUseFalloutSample = refHistogram.m_fUseFalloutSample;
		m_fLocked = refHistogram.m_fLocked;

		if ( m_fLocked )
		{
			m_iEffectiveFirst = refHistogram.m_iEffectiveFirst;
			m_iEffectiveLast = refHistogram.m_iEffectiveLast;

			m_ulSum0 = refHistogram.m_ulSum0;
			m_lfSum1 = refHistogram.m_lfSum1;
			m_lfSum2 = refHistogram.m_lfSum2;

			m_ulMax = refHistogram.m_ulMax;
			m_ulMin = refHistogram.m_ulMin;
		}
	}

	return *this;
}

const CHistogram1D& CHistogram1D::operator+=(const CHistogram1D &refHistogram)
{
	assert(refHistogram.m_iBinNum == m_iBinNum);
	assert(refHistogram.LowerBound() == m_lfLoBound);
	assert(refHistogram.UpperBound() == m_lfUpBound);
	assert(refHistogram.Step() == m_lfStep);

	for ( int i=0; i<m_iBinNum; i++ )
	{
		m_ulBins[i] += refHistogram.m_ulBins[i];
	}

	m_fLocked = false;
	return *this;
}

const CHistogram1D& CHistogram1D::operator-=(const CHistogram1D &refHistogram)
{
	assert(refHistogram.m_iBinNum == m_iBinNum);
	assert(refHistogram.LowerBound() == m_lfLoBound);
	assert(refHistogram.UpperBound() == m_lfUpBound);
	assert(refHistogram.Step() == m_lfStep);

	for ( int i=0; i<m_iBinNum; i++ )
	{
		if ( m_ulBins[i] > refHistogram.m_ulBins[i] )
		{
			m_ulBins[i] -= refHistogram.m_ulBins[i];
		}
		else
		{
			m_ulBins[i] = 0;
		}
	}

	m_fLocked = false;	
	return *this;
}
	
CHistogram1D CHistogram1D::operator-(const CHistogram1D &refHistogram) const
{
	CHistogram1D hist(*this);
	hist -= refHistogram;

	return hist;
}

CHistogram1D CHistogram1D::operator+(const CHistogram1D &refHistogram) const
{
	CHistogram1D hist(*this);
	hist += refHistogram;

	return hist;
}

int CHistogram1D::Index(double lfVal) const
{
	if ( lfVal < m_lfLoBound ) return 0;
	if ( lfVal == m_lfUpBound ) return m_iBinNum-2;
	if ( lfVal > m_lfUpBound ) return m_iBinNum-1;
	
	return int((lfVal-m_lfLoBound)/m_lfStep) + 1;
}

void CHistogram1D::Inc(int i)
{
	assert(!m_fLocked);
	assert(i>=0 && i<m_iBinNum);
	
	++ m_ulBins[i];
}

void CHistogram1D::Inc(int i, unsigned long n)
{
	assert(!m_fLocked);
	assert(i>=0 && i<m_iBinNum);

	m_ulBins[i] += n;
}

void CHistogram1D::Dec(int i)
{
	assert(!m_fLocked);
	assert(i>=0 && i<m_iBinNum);

	-- m_ulBins[i];
}

void CHistogram1D::Dec(int i, unsigned long n)
{
	assert(!m_fLocked);
	assert(i>=0 && i<m_iBinNum);

	if ( m_ulBins[i] > n )
	{
		m_ulBins[i] -= n;
	}
	else
	{
		m_ulBins = 0; 
	}
}

void CHistogram1D::Set(int i, unsigned long n)
{
	assert(!m_fLocked);
	assert(i>=0 && i<m_iBinNum);
	
	m_ulBins[i] = n;
}

bool CHistogram1D::IsLocked() const
{
	return m_fLocked;
}

void CHistogram1D::Unlock(void)
{
	m_fLocked = false;
}

bool CHistogram1D::UseFalloutSample() const
{
	return m_fUseFalloutSample;
}

void CHistogram1D::EnableUseFalloutSample(bool fb/* =true */)
{
	if ( m_fUseFalloutSample != fb )
		m_fLocked = false;

	m_fUseFalloutSample = fb;
}

void CHistogram1D::Lock()
{
	int iFirst = iFirstBin(), iLast = iLastBin();

	// finding effective bin index and max,min bin counts
	{
		m_iEffectiveFirst = iFirst;
		while ( m_iEffectiveFirst<=iLast && 0==m_ulBins[m_iEffectiveFirst] )
			++ m_iEffectiveFirst;
		
		assert(m_iEffectiveFirst <= iLast);
		
		m_iEffectiveLast = m_iEffectiveFirst;
		m_ulMax = m_ulBins[m_iEffectiveFirst];
		m_ulMin = m_ulBins[iFirst];
		for ( int i=m_iEffectiveFirst+1; i<=iLast; i++ )
		{
			if (m_ulBins[i] != 0)
				m_iEffectiveLast = i;
			
			if (m_ulMax < m_ulBins[i])
				m_ulMax = m_ulBins[i];
			else if (m_ulMin > m_ulBins[i])
				m_ulMin = m_ulBins[i];
		}
	}

	// calculate the sums
	{
		m_ulSum0 = 0;
		m_lfSum1 = 0.;
		m_lfSum2 = 0.;
	
		double tempf;
		for ( int i=m_iEffectiveFirst; i<=m_iEffectiveLast; i++ )
		{
			m_ulSum0 += m_ulBins[i];

			tempf = m_lfLoBound + (i-0.5)*m_lfStep;

			m_lfSum1 += m_ulBins[i] * tempf;
			m_lfSum2 += m_ulBins[i] * (tempf*tempf);
		}
	}

	m_fLocked = true;
	
}

COutputDataFile& Output(COutputDataFile& ofs, const std::vector<CHistogram1D*>& vpHist)
{
	ofs << ClassTag("Histogram1DArray") << int(vpHist.size()) << sep_endl;

	for ( int i=0; i<vpHist.size(); i++ )
	{
		if ( vpHist[i]==NULL )
		{
			ofs << false << sep_endl;
		}
		else
		{
			ofs << true << sep_endl;
			ofs << *vpHist[i] << sep_endl; 
		}
	}

	return ofs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const std::vector<CHistogram1D*>& vpHist)
{
	return Output(ofs, vpHist);
}

CInputDataFile& Input(CInputDataFile& ifs, std::vector<CHistogram1D*>& vpHist)
{
	int nHist;

	ifs >> ClassTag("Histogram1DArray") >> nHist >> sep_endl;
	vpHist.resize(nHist);

	for ( int i=0; i<vpHist.size(); i++ )
	{
		bool valid;
		ifs >> valid >> sep_endl;

		if ( valid )
		{
			vpHist[i] = new CHistogram1D;
			ifs >> *vpHist[i] >> sep_endl;
		}
		else
		{
			vpHist[i] = NULL;
		}
	}

	return ifs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, std::vector<CHistogram1D*>& vpHist)
{
	return Input(ifs, vpHist);
}

COutputDataFile& Output(COutputDataFile& ofs, const CHistogram1D& hist)
{
	ofs << ClassTag("Histogram1D") << sep_endl;

	ofs << hist.IsLocked() << hist.UseFalloutSample() 
		<< hist.LowerBound() << hist.UpperBound() << hist.Step()
		<< double(hist.Max()) << double(hist.Min())
		<< hist.IEffectiveFirst() << hist.IEffectiveLast()
		<< double(hist.Sum0()) << hist.Sum1() << hist.Sum2() << sep_endl;
		
	ofs << hist.NBins() << sep_endl;
	for (int i=0; i<hist.NBins()+2; i++)
		ofs << double(hist[i]) << sep_endl;

	return ofs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CHistogram1D& hist)
{
	return Output(ofs, hist);
}

CInputDataFile& Input(CInputDataFile& ifs, CHistogram1D& hist)
{
	ifs >> ClassTag("Histogram1D") >> sep_endl;

	bool fLocked, fUseFalloutSample;
	int nBins, iEffectiveFirst, iEffectiveLast;
	double ulMax, ulMin, ulSum0;
	double lfLoBound, lfUpBound, lfStep, lfSum1, lfSum2;

	ifs >> fLocked >> fUseFalloutSample 
		>> lfLoBound >> lfUpBound >> lfStep
		>> ulMax >> ulMin
		>> iEffectiveFirst >> iEffectiveLast
		>> ulSum0 >> lfSum1 >> lfSum2 >> sep_endl;
		
	ifs >> nBins >> sep_endl;

	CHistogram1D refHist(nBins, lfLoBound, lfUpBound, fUseFalloutSample);
	
	double ulTemp;
	for (int i=0; i<refHist.NBins()+2; i++)
	{
		ifs >> ulTemp >> sep_endl;
		refHist.Set(i, unsigned long(ulTemp));
	}
	
	refHist.Lock();
	hist = refHist;

	return ifs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CHistogram1D& hist)
{
	return Input(ifs, hist);
}

void HistogramToVector(const CHistogram1D& hist, CVisDVector& vec)
{
	vec.Resize(hist.NBins()+2);

	for ( int i=0; i<vec.Length(); i++ )
	{
		vec[i] = hist[i];
	}
}

void HistogramToDensity(const CHistogram1D& hist, CVisDVector& vec)
{
	vec.Resize(hist.NBins()+2);

	for ( int i=0; i<vec.Length(); i++ )
	{
		vec[i] = hist.Freq(i);
	}
}

CInputDataFile& operator>>(CInputDataFile& ifs, std::vector<CHistogram1D*>& vpHist)
{
	return Input(ifs, vpHist);
}
