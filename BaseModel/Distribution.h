// Distribution.h: interface for the CDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTION_H__0592B998_6C0E_44A6_91FB_A584712B9DC6__INCLUDED_)
#define AFX_DISTRIBUTION_H__0592B998_6C0E_44A6_91FB_A584712B9DC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BASEMODEL_API CSDistribution  
{
public:
	double Probability(double data) const
	{
		return exp(LogP(data));
	};
	double ProbabilityWithBias(double data) const
	{
		return exp(LogPWithBias(data));
	};
	virtual double LogP(double data) const = 0;
	virtual double LogPWithBias(double data) const = 0;
};

class BASEMODEL_API CSSampler
{
public:
	CSSampler(const CSDistribution& model) : m_ModelRef(model) {};
	virtual double GetOneSample(int method = 0) = 0;
	virtual void GetSamples(int nSample, CVisDVector& vSample, int method = 0);
protected: 
	const CSDistribution& m_ModelRef;
};

class BASEMODEL_API CDistribution  
{
public:
	CDistribution();
	virtual ~CDistribution();
	
	double Probability(const CVisDVector& data) const
	{
		return exp(LogP(data));
	};
	double ProbabilityWithBias(const CVisDVector& data) const
	{
		return exp(LogPWithBias(data));
	};
	virtual double LogP(const CVisDVector& data) const = 0;
	virtual double LogPWithBias(const CVisDVector& data) const { return LogP(data);};

	int NDataDimension() const {return m_nDataDimesion; } ;

protected:
	int m_nDataDimesion;
};

class BASEMODEL_API CJointDistribution
{
public:
	CJointDistribution();
	virtual ~CJointDistribution();

	double Probability(const CVisDVector& data) const
	{
		return exp(LogP(data));
	};
	double ProbabilityWithBias(const CVisDVector& data) const
	{
		return exp(LogPWithBias(data));
	};
	virtual double LogP(const CVisDVector& data) const = 0;
	virtual double LogPWithBias(const CVisDVector& data) const { return LogP(data);};

	int NDataDimensionX() const {return m_nDataDimensionX; } ;
	int NDataDimensionY() const {return m_nDataDimensionY; } ;

protected:
	int m_nDataDimensionX;
	int m_nDataDimensionY;
};

class BASEMODEL_API CSampler
{
public:
	CSampler(const CDistribution& model) : m_ModelRef(model) {};
	 
	virtual void GetOneSample(CVisDVector& data, int method = 0) = 0;
	virtual void GetSamples(int nSample, CVisDMatrix& dataMat, int method = 0);
protected: 
	const CDistribution& m_ModelRef;
};

class CDSampler;
class BASEMODEL_API CDDistribution
{
public:
	static void NormalizeLogP(CVisDVector& vLogP); 

	friend class CDSampler;
	double Probability(int iEvent) const;
	virtual double LogP(int iEvent) const;

	void Init(const CVisDVector& vLogP);
protected:
	int m_nEvent;
	CVisDVector m_vLogP;
};

class BASEMODEL_API CDSampler
{
public:
	CDSampler(const CDDistribution& model);
	virtual void GetOneSample(int& iEvent);
protected: 
	CVisDVector m_vSumP;
};

// return SUM(vData[i])
BASEMODEL_API double Sum(const CVisDVector& vData);
// return log( SUM( exp(vLogData[i]) )
BASEMODEL_API double LogSum(const CVisDVector& vLogData);

#endif // !defined(AFX_DISTRIBUTION_H__0592B998_6C0E_44A6_91FB_A584712B9DC6__INCLUDED_)
